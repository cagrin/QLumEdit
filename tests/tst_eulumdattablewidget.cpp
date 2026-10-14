#include "tst_ldt.h"
#include "eulumdattablewidget.h"
#include "mainwindow.h"

using namespace QLumEditTests;

namespace {

QString degrees(double v) { return QLocale().toString(v) + QChar(0xB0); }

// Tab-separated grid as copied from a spreadsheet: C-planes in the header, one row per gamma angle.
QString grid(const QList<double> &c, const QList<double> &g, double value = 100) {
    QString text;
    for (double v : c)
        text += "\t" + QString::number(v);
    text += "\n";
    for (double v : g) {
        text += QString::number(v);
        for (int i = 0; i < c.size(); i++)
            text += "\t" + QString::number(value);
        text += "\n";
    }
    return text;
}

const QList<double> kGamma = {0, 45, 90, 135, 180};

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

    void exportToClipboardStartsAtC90ForSymmetry3() {
        MainWindow window;
        TestLdt spec;
        spec.isym = 3;
        spec.c = {0, 90, 180, 270};
        spec.lcd = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
        Eulumdat ldt;
        loadLdt(ldt, tmp, spec.lines());
        EulumdatTableWidget w(ldt);

        w.exportToClipboard();

        const QLocale l;
        auto row = [&l](const QList<double> &values) {
            QStringList cells;
            for (double v : values)
                cells << l.toString(v);
            return cells.join('\t') + "\n";
        };
        const QString expected = "\t" + row({90, 180, 270})
                                 + row({0, 1, 4, 7})
                                 + row({90, 2, 5, 8})
                                 + row({180, 3, 6, 9});
        QCOMPARE(QApplication::clipboard()->text(), expected);
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

    void importFromClipboardDetectsSymmetry_data() {
        QTest::addColumn<QList<double>>("c");
        QTest::addColumn<QList<double>>("g");
        QTest::addColumn<int>("isym");
        QTest::addColumn<int>("mc");
        QTest::addColumn<double>("dc");
        QTest::addColumn<double>("dg");
        QTest::addColumn<QList<double>>("planes");

        const QList<double> uneven = {0, 30, 90, 135, 180};
        QTest::newRow("0: no symmetry") << QList<double>{0, 45, 90, 135, 180, 225, 270, 315} << kGamma
                                        << 0 << 8 << 45.0 << 45.0
                                        << QList<double>{0, 45, 90, 135, 180, 225, 270, 315};
        QTest::newRow("0: uneven") << QList<double>{0, 90, 180, 270, 300} << uneven
                                   << 0 << 5 << 0.0 << 0.0 << QList<double>{0, 90, 180, 270, 300};
        QTest::newRow("1: uneven gamma") << QList<double>{0} << uneven
                                         << 1 << 1 << 0.0 << 0.0 << QList<double>{0};
        QTest::newRow("2: uneven") << QList<double>{0, 30, 90, 180} << uneven
                                   << 2 << 4 << 0.0 << 0.0 << QList<double>{0, 30, 90, 180, 270, 330};
        QTest::newRow("3: C90-C270") << QList<double>{90, 135, 180, 225, 270} << kGamma
                                     << 3 << 5 << 45.0 << 45.0
                                     << QList<double>{0, 45, 90, 135, 180, 225, 270, 315};
        QTest::newRow("3: uneven") << QList<double>{90, 120, 180, 270} << uneven
                                   << 3 << 4 << 0.0 << 0.0 << QList<double>{0, 60, 90, 120, 180, 270};
        QTest::newRow("4: C0-C90") << QList<double>{0, 45, 90} << kGamma
                                   << 4 << 3 << 45.0 << 45.0
                                   << QList<double>{0, 45, 90, 135, 180, 225, 270, 315};
        QTest::newRow("4: uneven") << QList<double>{0, 30, 90} << uneven
                                   << 4 << 3 << 0.0 << 0.0
                                   << QList<double>{0, 30, 90, 150, 180, 210, 270, 330};
    }
    void importFromClipboardDetectsSymmetry() {
        QFETCH(QList<double>, c);
        QFETCH(QList<double>, g);
        QFETCH(int, isym);
        QFETCH(int, mc);
        QFETCH(double, dc);
        QFETCH(double, dg);
        QFETCH(QList<double>, planes);
        MainWindow window;
        window.loadFile(writeLdt(tmp, TestLdt().lines()));
        QApplication::clipboard()->setText(grid(c, g));

        window.central->table->importFromClipboard();

        Eulumdat *ldt = window.ldt.get();
        QCOMPARE(ldt->iIsym, isym);
        QCOMPARE(ldt->iNc, planes.size());
        QCOMPARE(ldt->iMc, mc);
        QCOMPARE(ldt->iNg, g.size());
        QCOMPARE(ldt->dDc, dc);
        QCOMPARE(ldt->dDg, dg);
        QCOMPARE(ldt->dC, planes);
        QCOMPARE(ldt->dG, g);
        QCOMPARE(window.central->table->tableWidget->columnCount(), mc);
        QCOMPARE(window.central->table->tableWidget->horizontalHeaderItem(0)->text(), "C" + degrees(c.first()));
        QCOMPARE(window.statusBar()->currentMessage(), "Data pasted");
    }

    void importFromClipboardCountsColumnsFromHtml() {
        MainWindow window;
        window.loadFile(writeLdt(tmp, TestLdt().lines()));
        auto *mimeData = new QMimeData;
        mimeData->setHtml("<table><TR><td></td><td>0</td><TD>90</TD><td>180</td></TR>"
                          "<tr><td>0</td><td>100</td><td>100</td><td>100</td></tr></table>");
        mimeData->setText(grid({0, 90, 180}, kGamma));
        QApplication::clipboard()->setMimeData(mimeData);

        window.central->table->importFromClipboard();

        QCOMPARE(window.ldt->iIsym, 2);
        QCOMPARE(window.ldt->iMc, 3);
        QCOMPARE(window.ldt->iNg, 5);
    }

    void importFromClipboardRejectsEmptyClipboard() {
        MainWindow window;
        window.loadFile(writeLdt(tmp, TestLdt().lines()));
        QApplication::clipboard()->setMimeData(new QMimeData);
        QString message;
        handleNextModal([&message](QDialog *dialog) {
            if (auto *box = qobject_cast<QMessageBox *>(dialog))
                message = box->text();
            dialog->accept();
        });

        window.central->table->importFromClipboard();

        QCOMPARE(message, "Nothing to paste");
        QCOMPARE(window.ldt->iNg, 3);
    }

    void importFromClipboardRejectsInvalidData_data() {
        QTest::addColumn<QString>("clipboard");
        QTest::addColumn<QString>("error");
        QTest::newRow("single column") << "100\n200\n" << "None useful data";
        QTest::newRow("too small") << "\t0\n0\t1\n" << "Lack of useful data";
        QTest::newRow("negative") << grid({0}, kGamma, -1) << "Values out of range";
        QTest::newRow("too high") << grid({0}, kGamma, 2000000) << "Values out of range";
        QTest::newRow("C not sorted") << grid({90, 0}, kGamma) << "C-planes not sorted";
        QTest::newRow("gamma above 180") << grid({0}, {0, 45, 90, 135, 200}) << "Wrong Gamma-planes scheme";
        QTest::newRow("single C10") << grid({10}, kGamma) << "Wrong C-planes scheme";
        QTest::newRow("ragged") << grid({0, 90}, kGamma).chopped(5) + "\n" << "Lack of useful data";
        QTest::newRow("C0-C270") << grid({0, 90, 180, 270}, kGamma) << "Wrong C-planes scheme";
        QTest::newRow("C0-C300") << grid({0, 300}, kGamma) << "Wrong C-planes scheme";
        QTest::newRow("C0-C180") << grid({0, 180}, kGamma) << "Wrong C-planes scheme";
        QTest::newRow("C90-C180") << grid({90, 180}, kGamma) << "Wrong C-planes scheme";
        QTest::newRow("C90-C270") << grid({90, 270}, kGamma) << "Wrong C-planes scheme";
        QTest::newRow("0: missing C90") << grid({0, 60, 180, 270, 300}, kGamma) << "Missing C90-plane";
        QTest::newRow("0: missing C180") << grid({0, 90, 200, 270, 300}, kGamma) << "Missing C180-plane";
        QTest::newRow("0: missing C270") << grid({0, 90, 180, 280, 300}, kGamma) << "Missing C270-plane";
        QTest::newRow("2: missing C90") << grid({0, 60, 180}, kGamma) << "Missing C90-plane";
        QTest::newRow("3: missing C180") << grid({90, 200, 270}, kGamma) << "Missing C180-plane";
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
