#include "tst_ldt.h"
#include "eulumdatwidget4.h"

using namespace QLumEditTests;

namespace {

QList<QDoubleSpinBox *> ratioSpinBoxes(EulumdatWidget4 &w) {
    return {w.DoubleSpinBox13, w.DoubleSpinBox14, w.DoubleSpinBox15, w.DoubleSpinBox16, w.DoubleSpinBox17,
            w.DoubleSpinBox18, w.DoubleSpinBox19, w.DoubleSpinBox20, w.DoubleSpinBox21, w.DoubleSpinBox22};
}

} // namespace

class TestEulumdatWidget4 : public QObject {
    Q_OBJECT

    QTemporaryDir tmp;

private slots:
    void initTestCase() {
        QVERIFY(tmp.isValid());
        setRestrictedLimits();
    }

    void showsDirectRatiosFromModel() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, TestLdt().lines());
        EulumdatWidget4 w(ldt);
        for (QDoubleSpinBox *box : ratioSpinBoxes(w))
            QCOMPARE(box->value(), 0.5);
    }

    void computeStoresRatiosForSelectedSpacing() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, isotropic(1, 1, 1, 5).lines());
        const QList<double> expected = ldt.directRadios("1.25");
        for (double k : expected)
            QVERIFY(k <= 1);

        EulumdatWidget4 w(ldt);
        handleNextModal([](QDialog *dialog) {
            if (auto *input = qobject_cast<QInputDialog *>(dialog))
                input->setTextValue("1.25");
            dialog->accept();
        });
        w.computePushButton->click();

        const QList<QDoubleSpinBox *> boxes = ratioSpinBoxes(w);
        for (int i = 0; i < 10; i++) {
            QCOMPARE(ldt.dDR[i], expected[i]);
            QCOMPARE(boxes[i]->value(), qBound(0.0, std::round(expected[i] * 1000) / 1000, 1.0));
        }
    }

    // Only reachable with invalid model data: the ratios are relative to the total flux, so they exceed 1 only
    // with negative intensities, which loading rejects. Here a narrow beam with a negative upper hemisphere.
    void computeWarnsAndKeepsRatiosAbove1() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, isotropic(1, 1, 1, 5).lines());
        for (int j = 0; j < ldt.iNg; j++)
            ldt.dLcd[0][j] = ldt.dG[j] <= 40 ? 1000 : ldt.dG[j] <= 90 ? 0 : -130;
        const QList<double> ratios = ldt.directRadios("1.00");
        const auto first = std::find_if(ratios.begin(), ratios.end(), [](double k) { return k > 1; });
        QVERIFY(first != ratios.end());
        const QString expected = QString("k[%1] = %2 -> value is out of range").arg(first - ratios.begin() + 1).arg(*first);

        EulumdatWidget4 w(ldt);
        QString message;
        handleNextModal([&message](QDialog *dialog) {
            // The warning follows the spacing dialog.
            handleNextModal([&message](QDialog *warning) {
                if (auto *box = qobject_cast<QMessageBox *>(warning))
                    message = box->text();
                warning->accept();
            });
            dialog->accept();
        });
        w.computePushButton->click();

        QVERIFY2(message.startsWith(expected), qPrintable(message));
        for (int i = 0; i < 10; i++)
            QCOMPARE(ldt.dDR[i], 0.5);
        for (QDoubleSpinBox *box : ratioSpinBoxes(w))
            QCOMPARE(box->value(), 0.5);
    }

    void computeCancelledKeepsRatios() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, isotropic(1, 1, 1, 5).lines());
        EulumdatWidget4 w(ldt);
        handleNextModal([](QDialog *dialog) { dialog->reject(); });
        w.computePushButton->click();

        for (int i = 0; i < 10; i++)
            QCOMPARE(ldt.dDR[i], 0.5);
        for (QDoubleSpinBox *box : ratioSpinBoxes(w))
            QCOMPARE(box->value(), 0.5);
    }
};

QTEST_MAIN(TestEulumdatWidget4)
#include "tst_eulumdatwidget4.moc"
