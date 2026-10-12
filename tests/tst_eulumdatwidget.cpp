#include "tst_ldt.h"
#include "eulumdatwidget.h"

using namespace QLumEditTests;

class TestEulumdatWidget : public QObject {
    Q_OBJECT

    QTemporaryDir tmp;

private slots:
    void initTestCase() {
        QVERIFY(tmp.isValid());
        setRestrictedLimits();
    }

    void showsTextFieldsFromModel() {
        TestLdt spec;
        spec.iden = "ACME";
        Eulumdat ldt;
        loadLdt(ldt, tmp, spec.lines());
        EulumdatWidget w(ldt);

        QCOMPARE(w.LineEdit1->text(), "ACME");
        QCOMPARE(w.LineEdit8->text(), "MRN-1");
        QCOMPARE(w.LineEdit9->text(), "Name");
        QCOMPARE(w.LineEdit10->text(), "NUM-1");
        QCOMPARE(w.LineEdit11->text(), "test.ldt");
        QCOMPARE(w.LineEdit12->text(), "2026");
    }

    void typeComboShowsLinearLuminaire_data() {
        QTest::addColumn<int>("ityp");
        QTest::addColumn<int>("index");
        QTest::newRow("point source, rotational") << 1 << 0;
        QTest::newRow("linear") << 2 << 1;
        QTest::newRow("point source, other") << 3 << 0;
    }
    void typeComboShowsLinearLuminaire() {
        QFETCH(int, ityp);
        QFETCH(int, index);
        TestLdt spec;
        spec.ityp = ityp;
        Eulumdat ldt;
        loadLdt(ldt, tmp, spec.lines());
        EulumdatWidget w(ldt);
        QCOMPARE(w.ComboBox2->currentIndex(), index);
    }

    void lineEditsUseLengthLimitsFromVars() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, TestLdt().lines());
        EulumdatWidget w(ldt);
        QCOMPARE(w.LineEdit1->maxLength(), 78);
        QCOMPARE(w.LineEdit11->maxLength(), 8);
    }

    void labelsDescribeSymmetryAndGrid() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, isotropic(2, 24, 13).lines());
        EulumdatWidget w(ldt);
        const QLocale locale;

        QVERIFY(w.Label3->text().endsWith("symmetry to plane C0-C180"));
        QVERIFY(w.Label4->text().endsWith(" = " + locale.toString(24)));
        QVERIFY(w.Label5->text().contains(" = " + locale.toString(15.0)));
        QVERIFY(w.Label6->text().endsWith(" = " + locale.toString(3)));
        QVERIFY(w.Label7->text().contains(" = " + locale.toString(90.0)));
    }

    void labelsShowNonEquidistantWhenDistanceIsZero() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, TestLdt().lines());
        EulumdatWidget w(ldt);
        QVERIFY(w.Label5->text().endsWith("non-equidistantly"));
        QVERIFY(!w.Label7->text().contains("non-equidistantly"));
    }

    void setValuesRefreshesLabelsFromModel() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, TestLdt().lines());
        EulumdatWidget w(ldt);
        ldt.iNc = 36;
        w.setValues();
        QVERIFY(w.Label4->text().endsWith(" = " + QLocale().toString(36)));
    }
};

QTEST_MAIN(TestEulumdatWidget)
#include "tst_eulumdatwidget.moc"
