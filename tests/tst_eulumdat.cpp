#include "tst_ldt.h"

using namespace QLumEditTests;

class TestEulumdat : public QObject {
    Q_OBJECT

    QTemporaryDir tmp;

    QStringList load(Eulumdat &ldt, const QStringList &lines) { return loadLdt(ldt, tmp, lines); }

    QString loadError(const QStringList &lines) {
        Eulumdat ldt;
        try {
            load(ldt, lines);
        } catch (const QString &e) {
            return e;
        }
        return {};
    }

private slots:
    void initTestCase() {
        QVERIFY(tmp.isValid());
        setRestrictedLimits();
    }

    void calcMc_data() { addSymmetryRows(); }
    void calcMc() {
        QFETCH(int, isym);
        QFETCH(int, nc);
        QFETCH(int, mc);
        Eulumdat ldt;
        ldt.iIsym = isym;
        ldt.iNc = nc;
        ldt.calcMc();
        QCOMPARE(ldt.iMc, mc);
    }

    void loadFile_readsFields() {
        TestLdt spec;
        spec.iden = "ACME";
        spec.g = {0, 45, 90, 135, 180};
        spec.lcd = {{500, 400, 300, 200, 100}};
        Eulumdat ldt;
        load(ldt, spec.lines());

        QCOMPARE(ldt.sIden, "ACME");
        QCOMPARE(ldt.iItyp, 1);
        QCOMPARE(ldt.iIsym, 1);
        QCOMPARE(ldt.iNc, 1);
        QCOMPARE(ldt.iNg, 5);
        QCOMPARE(ldt.dDg, 45.0);
        QCOMPARE(ldt.dL, 100.0);
        QCOMPARE(ldt.iN, 1);
        QCOMPARE(ldt.sTL[0], "LED");
        QCOMPARE(ldt.dTLF[0], 1000.0);
        QCOMPARE(ldt.dG[4], 180.0);
        QCOMPARE(ldt.dLcd[0][1], 400.0);
        QCOMPARE(ldt.iMc, 1);
    }

    void loadFile_validFileHasNoWarnings() {
        Eulumdat ldt;
        QCOMPARE(load(ldt, TestLdt().lines()), QStringList());
    }

    void loadFile_acceptsCommaUnderscoreAndSpacesInNumbers() {
        QStringList lines = TestLdt().lines();
        lines[kLengthLine] = " 1_000,5 ";
        Eulumdat ldt;
        load(ldt, lines);
        QCOMPARE(ldt.dL, 1000.5);
    }

    void loadFile_rejectsNonNumericDouble() {
        QStringList lines = TestLdt().lines();
        lines[kLengthLine] = "abc";
        QVERIFY(loadError(lines).startsWith("Error reading file"));
    }

    void loadFile_rejectsNonNumericInt() {
        QStringList lines = TestLdt().lines();
        lines[kTypeLine] = "x";
        QVERIFY(loadError(lines).startsWith("Error reading file"));
    }

    void validate_rejectsTypeOutOfRange() {
        TestLdt spec;
        spec.ityp = 4;
        QVERIFY(loadError(spec.lines()).contains("Type indicator"));
    }

    void validate_warnsWhenIdentificationTooLong() {
        TestLdt spec;
        spec.iden = QString(79, 'x');
        Eulumdat ldt;
        const QStringList warnings = load(ldt, spec.lines());
        QCOMPARE(warnings.size(), 1);
        QVERIFY(warnings[0].contains("Identification"));
    }

    void validate_warnsWhenLengthOutOfRange() {
        QStringList lines = TestLdt().lines();
        lines[kLengthLine] = "0";
        Eulumdat ldt;
        const QStringList warnings = load(ldt, lines);
        QCOMPARE(warnings.size(), 1);
        QVERIFY(warnings[0].contains("Length/diameter of luminaire"));
    }

    void validate_rejectsUnsortedCPlanes() {
        TestLdt spec = isotropic(0, 4, 4);
        spec.c = {0, 180, 90, 270};
        QCOMPARE(loadError(spec.lines()), "C-planes not sorted");
    }

    void validate_rejectsUnsortedGammaPlanes() {
        TestLdt spec;
        spec.g = {0, 120, 90};
        QCOMPARE(loadError(spec.lines()), "Gamma-planes not sorted");
    }

    void validate_rejectsMissingC90Plane() {
        TestLdt spec = isotropic(0, 4, 4);
        spec.c = {0, 60, 180, 270};
        QCOMPARE(loadError(spec.lines()), "Missing C90-plane");
    }

    void validate_rejectsTooLowIntensities() {
        TestLdt spec;
        spec.lcd = {{0.5, 0.5, 0.5}};
        QVERIFY(loadError(spec.lines()).contains("values are too low"));
    }

    void saveFile_roundTripPreservesValues() {
        TestLdt spec;
        spec.g = {0, 45, 90, 135, 180};
        spec.lcd = {{500, 400, 300, 200, 100}};
        Eulumdat original;
        load(original, spec.lines());

        const QString path = tmp.filePath("saved.ldt");
        original.saveFile(path);
        Eulumdat reloaded;
        reloaded.loadFile(path);

        QCOMPARE(reloaded.sIden, original.sIden);
        QCOMPARE(reloaded.iItyp, original.iItyp);
        QCOMPARE(reloaded.iIsym, original.iIsym);
        QCOMPARE(reloaded.iNg, original.iNg);
        QCOMPARE(reloaded.dL, original.dL);
        QCOMPARE(reloaded.sTL[0], original.sTL[0]);
        for (int j = 0; j < original.iNg; j++) {
            QCOMPARE(reloaded.dG[j], original.dG[j]);
            QCOMPARE(reloaded.dLcd[0][j], original.dLcd[0][j]);
        }
    }

    // Current behavior: type 1 without rotational symmetry is saved as type 3.
    void saveFile_writesType3ForType1WithoutRotationalSymmetry() {
        Eulumdat original;
        load(original, isotropic(0, 4, 4).lines());
        QCOMPARE(original.iItyp, 1);

        const QString path = tmp.filePath("type.ldt");
        original.saveFile(path);
        Eulumdat reloaded;
        reloaded.loadFile(path);
        QCOMPARE(reloaded.iItyp, 3);
    }

    void bundledTemplates_load_data() {
        QTest::addColumn<QString>("file");
        for (const QString &f : QDir(TEMPLATES_DIR).entryList({"*.ldt"}, QDir::Files, QDir::Name))
            QTest::newRow(qPrintable(f)) << f;
    }
    void bundledTemplates_load() {
        QFETCH(QString, file);
        Eulumdat ldt;
        ldt.loadFile(QDir(TEMPLATES_DIR).filePath(file));
        QVERIFY(ldt.iMc > 0);
    }
};

QTEST_MAIN(TestEulumdat)
#include "tst_eulumdat.moc"
