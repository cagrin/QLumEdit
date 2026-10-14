#include "tst_ldt.h"

using namespace QLumEditTests;

class TestEulumdatCalculate : public QObject {
    Q_OBJECT

    QTemporaryDir tmp;

private slots:
    void initTestCase() {
        QVERIFY(tmp.isValid());
        setRestrictedLimits();
    }

    void output_isotropicSourceIs100Percent_data() { addSymmetryRows(); }
    void output_isotropicSourceIs100Percent() {
        QFETCH(int, isym);
        QFETCH(int, nc);
        QFETCH(int, mc);
        Eulumdat ldt;
        loadLdt(ldt, tmp, isotropic(isym, nc, mc).lines());
        QCOMPARE(ldt.output(), 100.0);
    }

    void output_scalesLinearlyWithIntensity() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, isotropic(1, 1, 1).scaled(2).lines());
        QCOMPARE(ldt.output(), 200.0);
    }

    void downward_isotropicSourceIs50Percent_data() { addSymmetryRows(); }
    void downward_isotropicSourceIs50Percent() {
        QFETCH(int, isym);
        QFETCH(int, nc);
        QFETCH(int, mc);
        Eulumdat ldt;
        loadLdt(ldt, tmp, isotropic(isym, nc, mc).lines());
        QCOMPARE(ldt.downward(), 50.0);
    }

    void downward_doesNotDependOnIntensityScale() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, isotropic(1, 1, 1).scaled(3).lines());
        QCOMPARE(ldt.downward(), 50.0);
    }

    void downward_upwardOnlySourceIsZero() {
        TestLdt spec;
        spec.lcd = {{0, 0, 100}};
        Eulumdat ldt;
        loadLdt(ldt, tmp, spec.lines());
        QCOMPARE(ldt.downward(), 0.0);
        QVERIFY(ldt.output() > 0);
    }

    // With gamma step 90 only the 90-degree zone has flux, so k[i] = 50 * h[i] / 100000.
    void directRadios_usesTableForRatio_data() {
        QTest::addColumn<QString>("shr");
        QTest::addColumn<double>("h0");
        QTest::addColumn<double>("h9");
        QTest::newRow("1.00") << "1.00" << -27.0 << 107.0;
        QTest::newRow("1.25") << "1.25" << -26.0 << 108.0;
        QTest::newRow("1.50") << "1.50" << -24.0 << 109.0;
    }
    void directRadios_usesTableForRatio() {
        QFETCH(QString, shr);
        QFETCH(double, h0);
        QFETCH(double, h9);
        Eulumdat ldt;
        loadLdt(ldt, tmp, isotropic(1, 1, 1).lines());
        const QList<double> dr = ldt.directRadios(shr);
        QCOMPARE(dr.size(), 10);
        QCOMPARE(dr[0], 50 * h0 / 100000);
        QCOMPARE(dr[9], 50 * h9 / 100000);
    }

    void directRadios_sameForAllSymmetries_data() { addSymmetryRows(); }
    void directRadios_sameForAllSymmetries() {
        QFETCH(int, isym);
        QFETCH(int, nc);
        QFETCH(int, mc);
        Eulumdat reference, ldt;
        loadLdt(reference, tmp, isotropic(1, 1, 1, 5).lines());
        loadLdt(ldt, tmp, isotropic(isym, nc, mc, 5).lines());
        const QList<double> expected = reference.directRadios("1.00");
        const QList<double> actual = ldt.directRadios("1.00");
        for (int i = 0; i < 10; i++)
            QVERIFY2(std::abs(actual[i] - expected[i]) < 1e-9, qPrintable(QString("k[%1]").arg(i + 1)));
    }

    void directRadios_unknownRatioFallsBackTo150() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, isotropic(1, 1, 1, 5).lines());
        QCOMPARE(ldt.directRadios("unknown"), ldt.directRadios("1.50"));
    }

    // The C-planes may start at C90, the first stored plane (read past the end of the list before).
    void symmetry3_planesStartingAtC90() {
        TestLdt spec = isotropic(3, 4, 3, 5);
        spec.c = {90, 180, 270, 300};
        Eulumdat reference, ldt;
        loadLdt(reference, tmp, isotropic(1, 1, 1, 5).lines());
        QCOMPARE(loadLdt(ldt, tmp, spec.lines()), QStringList());

        QCOMPARE(ldt.output(), reference.output());
        QCOMPARE(ldt.downward(), reference.downward());
        const QList<double> expected = reference.directRadios("1.00");
        const QList<double> actual = ldt.directRadios("1.00");
        for (int i = 0; i < 10; i++)
            QVERIFY2(std::abs(actual[i] - expected[i]) < 1e-9, qPrintable(QString("k[%1]").arg(i + 1)));
    }
};

QTEST_MAIN(TestEulumdatCalculate)
#include "tst_eulumdatcalculate.moc"
