#include "tst_ldt.h"
#include "eulumdatwidget3.h"
#include "mainwindow.h"

using namespace QLumEditTests;

namespace {

bool saveEnabled(MainWindow &window) {
    for (QAction *action : window.findChildren<QAction *>())
        if (action->text() == MainWindow::tr("&Save"))
            return action->isEnabled();
    return false;
}

} // namespace

class TestEulumdatWidget3 : public QObject {
    Q_OBJECT

    QTemporaryDir tmp;

private slots:
    void initTestCase() { QVERIFY(tmp.isValid()); }

    // MainWindow overrides these limits from QSettings, so reset them before each test.
    void init() { setRestrictedLimits(); }

    void showsLampSetFromModel() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, TestLdt().lines());
        EulumdatWidget3 w(ldt);

        QCOMPARE(w.tabWidget->count(), 1);
        QVERIFY(w.Label1->text().endsWith(" = 1"));
        QCOMPARE(w.SpinBox2[0]->value(), 1);
        QCOMPARE(w.LineEdit3[0]->text(), "LED");
        QCOMPARE(w.DoubleSpinBox4[0]->value(), 1000.0);
        QCOMPARE(w.LineEdit5[0]->text(), "3000K");
        QCOMPARE(w.LineEdit6[0]->text(), "80");
        QCOMPARE(w.DoubleSpinBox7[0]->value(), 10.0);
    }

    void lineEditsUseLengthLimitsFromVars() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, TestLdt().lines());
        EulumdatWidget3 w(ldt);
        QCOMPARE(w.LineEdit3[0]->maxLength(), 24);
        QCOMPARE(w.LineEdit5[0]->maxLength(), 16);
        QCOMPARE(w.LineEdit6[0]->maxLength(), 6);
    }

    void removeSetKeepsAtLeastOneSet() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, TestLdt().lines());
        EulumdatWidget3 w(ldt);
        w.removeButton->click();
        QCOMPARE(ldt.iN, 1);
        QCOMPARE(w.tabWidget->count(), 1);
    }

    void addAndRemoveSetUpdateModelAndMarkDocumentModified() {
        MainWindow window;
        Eulumdat ldt;
        loadLdt(ldt, tmp, TestLdt().lines());
        EulumdatWidget3 w(ldt);
        QVERIFY(!saveEnabled(window));

        w.addButton->click();
        QCOMPARE(ldt.iN, 2);
        QCOMPARE(w.tabWidget->count(), 2);
        QVERIFY(w.Label1->text().endsWith(" = 2"));
        QVERIFY(saveEnabled(window));

        w.removeButton->click();
        QCOMPARE(ldt.iN, 1);
        QCOMPARE(w.tabWidget->count(), 1);
    }

    void addSetStopsAt20Sets() {
        MainWindow window;
        Eulumdat ldt;
        loadLdt(ldt, tmp, TestLdt().lines());
        EulumdatWidget3 w(ldt);
        for (int i = 0; i < 25; i++)
            w.addButton->click();
        QCOMPARE(ldt.iN, 20);
        QCOMPARE(w.tabWidget->count(), 20);
    }

    // Pages of removed sets are kept and reused, so a set added again shows its previous values.
    void addSetAfterRemoveReusesPage() {
        MainWindow window;
        Eulumdat ldt;
        loadLdt(ldt, tmp, TestLdt().lines());
        EulumdatWidget3 w(ldt);
        w.addButton->click();
        QWidget *page = w.tabWidget->widget(1);
        w.LineEdit3[1]->setText("Halogen");
        w.removeButton->click();

        w.addButton->click();

        QCOMPARE(ldt.iN, 2);
        QCOMPARE(w.GroupBox.size(), 2);
        QCOMPARE(w.tabWidget->widget(1), page);
        QCOMPARE(w.LineEdit3[1]->text(), "Halogen");
    }
};

QTEST_MAIN(TestEulumdatWidget3)
#include "tst_eulumdatwidget3.moc"
