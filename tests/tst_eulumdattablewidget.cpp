#include "tst_ldt.h"
#include "eulumdattablewidget.h"
#include "mainwindow.h"

using namespace QLumEditTests;

namespace {

QString degrees(double v) { return QLocale().toString(v) + QChar(0xB0); }

} // namespace

class TestEulumdatTableWidget : public QObject {
    Q_OBJECT

    QTemporaryDir tmp;

    QString importError(MainWindow &window, const QString &clipboardText) {
        QApplication::clipboard()->setText(clipboardText);
        QString message;
        handleNextModal([&message](QDialog *dialog) {
            if (auto *box = qobject_cast<QMessageBox *>(dialog))
                message = box->text();
            dialog->accept();
        });
        window.central->table->importFromClipboard();
        return message;
    }

private slots:
    void initTestCase() { QVERIFY(tmp.isValid()); }
    void init() { setRestrictedLimits(); }

    void setTableShowsIntensityGrid() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, isotropic(2, 24, 13, 45).lines());
        EulumdatTableWidget w(ldt);

        QCOMPARE(w.tableWidget->rowCount(), 5);
        QCOMPARE(w.tableWidget->columnCount(), 13);
        QCOMPARE(w.tableWidget->horizontalHeaderItem(0)->text(), "C" + degrees(0));
        QCOMPARE(w.tableWidget->horizontalHeaderItem(12)->text(), "C" + degrees(180));
        QCOMPARE(w.tableWidget->verticalHeaderItem(4)->text(), degrees(180));
        QCOMPARE(w.tableWidget->item(1, 0)->text(), QLocale().toString(kIsotropic));
    }

    void setTableStartsAtC90ForSymmetry3() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, isotropic(3, 24, 13).lines());
        EulumdatTableWidget w(ldt);
        QCOMPARE(w.tableWidget->horizontalHeaderItem(0)->text(), "C" + degrees(90));
        QCOMPARE(w.tableWidget->horizontalHeaderItem(12)->text(), "C" + degrees(270));
    }

    void exportToClipboardWritesTabSeparatedGrid() {
        MainWindow window;
        TestLdt spec;
        spec.lcd = {{300, 200, 100}};
        Eulumdat ldt;
        loadLdt(ldt, tmp, spec.lines());
        EulumdatTableWidget w(ldt);

        w.exportToClipboard();

        const QLocale l;
        const QString expected = "\t" + l.toString(0.0) + "\n"
                                 + l.toString(0.0) + "\t" + l.toString(300.0) + "\n"
                                 + l.toString(90.0) + "\t" + l.toString(200.0) + "\n"
                                 + l.toString(180.0) + "\t" + l.toString(100.0) + "\n";
        QCOMPARE(QApplication::clipboard()->text(), expected);
        QCOMPARE(window.statusBar()->currentMessage(), "Data copied");
    }

    void importFromClipboardReadsRotationalSymmetry() {
        MainWindow window;
        window.loadFile(writeLdt(tmp, TestLdt().lines()));
        QApplication::clipboard()->setText("\t0\n0\t100\n45\t110\n90\t120\n135\t130\n180\t140\n");

        window.central->table->importFromClipboard();

        Eulumdat *ldt = window.ldt.get();
        QCOMPARE(ldt->iIsym, 1);
        QCOMPARE(ldt->iNc, 1);
        QCOMPARE(ldt->iNg, 5);
        QCOMPARE(ldt->dDg, 45.0);
        QCOMPARE(ldt->dLcd[0][4], 140.0);
        QCOMPARE(window.central->table->tableWidget->rowCount(), 5);
        QCOMPARE(window.statusBar()->currentMessage(), "Data pasted");
    }

    void importFromClipboardMirrorsHalfPlanesForSymmetry2() {
        MainWindow window;
        window.loadFile(writeLdt(tmp, TestLdt().lines()));
        QString text = "\t0\t90\t180\n";
        for (int g = 0; g <= 180; g += 45)
            text += QString("%1\t100\t100\t100\n").arg(g);
        QApplication::clipboard()->setText(text);

        window.central->table->importFromClipboard();

        Eulumdat *ldt = window.ldt.get();
        QCOMPARE(ldt->iIsym, 2);
        QCOMPARE(ldt->iNc, 4);
        QCOMPARE(ldt->iMc, 3);
        QCOMPARE(ldt->dC[3], 270.0);
    }

    void importFromClipboardRejectsInvalidData_data() {
        QTest::addColumn<QString>("clipboard");
        QTest::addColumn<QString>("error");
        QTest::newRow("too small") << "\t0\n0\t1\n" << "Lack of useful data";
        QTest::newRow("not a number") << "\t0\n0\t100\n45\tx\n90\t100\n135\t100\n180\t100\n"
                                      << "Data are not real numbers";
        QTest::newRow("gamma not sorted") << "\t0\n0\t100\n90\t100\n45\t100\n135\t100\n180\t100\n"
                                          << "Gamma-planes not sorted";
        QTest::newRow("too low") << "\t0\n0\t0.5\n45\t0.5\n90\t0.5\n135\t0.5\n180\t0.5\n"
                                 << "Values are too low";
    }
    void importFromClipboardRejectsInvalidData() {
        QFETCH(QString, clipboard);
        QFETCH(QString, error);
        MainWindow window;
        window.loadFile(writeLdt(tmp, TestLdt().lines()));

        QCOMPARE(importError(window, clipboard), error);
        QCOMPARE(window.ldt->iNg, 3);
    }
};

QTEST_MAIN(TestEulumdatTableWidget)
#include "tst_eulumdattablewidget.moc"
