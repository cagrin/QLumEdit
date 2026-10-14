#include "tst_ldt.h"
#include "eulumdatwidget2.h"

using namespace QLumEditTests;

class TestEulumdatWidget2 : public QObject {
    Q_OBJECT

    QTemporaryDir tmp;

private slots:
    void initTestCase() {
        QVERIFY(tmp.isValid());
        setRestrictedLimits();
    }

    void showsLuminaireDataFromModel() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, TestLdt().lines());
        EulumdatWidget2 w(ldt);

        QCOMPARE(w.DoubleSpinBox13->value(), 100.0);
        QCOMPARE(w.DoubleSpinBox14->value(), 50.0);
        QCOMPARE(w.DoubleSpinBox15->value(), 20.0);
        QCOMPARE(w.DoubleSpinBox16->value(), 90.0);
        QCOMPARE(w.DoubleSpinBox17->value(), 40.0);
        QCOMPARE(w.DoubleSpinBox22->value(), 100.0);
        QCOMPARE(w.DoubleSpinBox23->value(), 80.0);
        QCOMPARE(w.DoubleSpinBox24->value(), 1.0);
        QCOMPARE(w.DoubleSpinBox25->value(), 0.0);
    }

    void lengthIsClampedToSpinBoxMinimum() {
        QStringList lines = TestLdt().lines();
        lines[kLengthLine] = "0";
        Eulumdat ldt;
        loadLdt(ldt, tmp, lines);
        EulumdatWidget2 w(ldt);
        QCOMPARE(w.DoubleSpinBox13->value(), 1.0);
    }

    void downwardButtonStoresCalculatedValue() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, isotropic(1, 1, 1).scaled(0.5).lines());
        EulumdatWidget2 w(ldt);
        w.downwardPushButton->click();
        QCOMPARE(ldt.dDFF, 50.0);
        QCOMPARE(w.DoubleSpinBox22->value(), 50.0);
    }

    // Only reachable with invalid model data: the flux fraction is a ratio, so it exceeds 100% only with
    // negative intensities, which loading rejects.
    void downwardButtonWarnsAndKeepsValueAbove100Percent() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, TestLdt().lines());
        ldt.dLcd[0][2] = -200;
        EulumdatWidget2 w(ldt);

        QString message;
        handleNextModal([&message](QDialog *dialog) {
            if (auto *box = qobject_cast<QMessageBox *>(dialog))
                message = box->text();
            dialog->accept();
        });
        w.downwardPushButton->click();

        QVERIFY2(message.startsWith("Downward flux fraction = 200 -> value is out of range"), qPrintable(message));
        QCOMPARE(ldt.dDFF, 100.0);
        QCOMPARE(w.DoubleSpinBox22->value(), 100.0);
    }

    void outputButtonStoresCalculatedValue() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, isotropic(1, 1, 1).scaled(0.5).lines());
        EulumdatWidget2 w(ldt);
        w.outputPushButton->click();
        QCOMPARE(ldt.dLORL, 50.0);
        QCOMPARE(w.DoubleSpinBox23->value(), 50.0);
    }

    void outputButtonWarnsAndKeepsValueAbove100Percent() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, isotropic(1, 1, 1).scaled(2).lines());
        EulumdatWidget2 w(ldt);

        QString message;
        handleNextModal([&message](QDialog *dialog) {
            if (auto *box = qobject_cast<QMessageBox *>(dialog))
                message = box->text();
            dialog->accept();
        });
        w.outputPushButton->click();

        QVERIFY(message.contains("value is out of range"));
        QCOMPARE(ldt.dLORL, 80.0);
        QCOMPARE(w.DoubleSpinBox23->value(), 80.0);
    }
};

QTEST_MAIN(TestEulumdatWidget2)
#include "tst_eulumdatwidget2.moc"
