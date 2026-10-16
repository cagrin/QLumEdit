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

    void loadFile_readsEmptyNumbersAsZero() {
        QStringList lines = TestLdt().lines();
        lines[13] = ""; // B
        lines[26] = ""; // NL
        Eulumdat ldt;
        const QStringList warnings = load(ldt, lines);
        QCOMPARE(ldt.dB, 0.0);
        QCOMPARE(ldt.iNL[0], 0);
        QCOMPARE(warnings.size(), 1);
        QVERIFY(warnings[0].contains("Number of lamps"));
    }

    void validate_rejectsTypeOutOfRange() {
        TestLdt spec;
        spec.ityp = 4;
        QVERIFY(loadError(spec.lines()).contains("Type indicator"));
    }

    void validate_errors_data() {
        QTest::addColumn<QStringList>("lines");
        QTest::addColumn<QString>("error");

        auto withLine = [](int index, const QString &value) {
            QStringList lines = TestLdt().lines();
            lines[index] = value;
            return lines;
        };
        TestLdt spec;

        QTest::newRow("type < 1") << withLine(1, "0") << "Type indicator";
        QTest::newRow("symmetry < 0") << withLine(2, "-1") << "Symmetry indicator";
        QTest::newRow("symmetry > 4") << withLine(2, "5") << "Symmetry indicator";
        QTest::newRow("Nc < 0") << withLine(3, "-1") << "Number of C-planes";
        QTest::newRow("Nc > 721") << withLine(3, "722") << "Number of C-planes";
        QTest::newRow("Dc < 0") << withLine(4, "-1") << "Distance between C-planes";
        QTest::newRow("Dc > 360") << withLine(4, "361") << "Distance between C-planes";
        QTest::newRow("Ng < 0") << withLine(5, "-1") << "Number of luminous intensities in each C-plane";
        QTest::newRow("Ng > 361") << withLine(5, "362") << "Number of luminous intensities in each C-plane";
        QTest::newRow("Dg < 0") << withLine(6, "-1") << "Distance between luminous intensities per C-plane";
        QTest::newRow("Dg > 180") << withLine(6, "181") << "Distance between luminous intensities per C-plane";

        QStringList noLamps = withLine(25, "0");
        noLamps.remove(26, 6);
        QTest::newRow("n < 1") << noLamps << "Number of standard sets of lamps";

        QStringList manyLamps = withLine(25, "21");
        const QStringList lampSet = manyLamps.mid(26, 6);
        for (int i = 0; i < 20; i++)
            manyLamps = manyLamps.mid(0, 32) + lampSet + manyLamps.mid(32);
        QTest::newRow("n > 20") << manyLamps << "Number of standard sets of lamps";

        // A damaged file must not allocate lists with these sizes (std::bad_alloc or many GB before).
        QTest::newRow("Nc huge") << withLine(3, "2000000000") << "Number of C-planes = 2000000000";
        QTest::newRow("Ng huge") << withLine(5, "2000000000")
                                 << "Number of luminous intensities in each C-plane = 2000000000";
        QTest::newRow("n huge") << withLine(25, "2000000000") << "Number of standard sets of lamps = 2000000000";

        spec = TestLdt();
        spec.lcd = {{100, -1, 100}};
        QTest::newRow("intensity < 0") << spec.lines() << "Luminous intensity distribution";

        spec = TestLdt();
        spec.lcd = {{100, 1000001, 100}};
        QTest::newRow("intensity > 1000000") << spec.lines() << "Luminous intensity distribution";

        spec = TestLdt();
        spec.g = {-10, 90, 180};
        QTest::newRow("gamma < 0") << spec.lines() << "Wrong Gamma-planes scheme";

        spec = TestLdt();
        spec.g = {0, 90, 190};
        QTest::newRow("gamma > 180") << spec.lines() << "Wrong Gamma-planes scheme";

        spec = isotropic(0, 4, 4);
        spec.c = {0, 90, 200, 270};
        QTest::newRow("isym 0 without C180") << spec.lines() << "Missing C180-plane";

        spec = isotropic(0, 4, 4);
        spec.c = {0, 90, 180, 300};
        QTest::newRow("isym 0 without C270") << spec.lines() << "Missing C270-plane";

        spec = isotropic(2, 4, 3);
        spec.c = {0, 60, 180, 270};
        QTest::newRow("isym 2 without C90") << spec.lines() << "Missing C90-plane";

        spec = isotropic(3, 4, 3);
        spec.c = {0, 90, 200, 270};
        QTest::newRow("isym 3 without C180") << spec.lines() << "Missing C180-plane";

        // Planes the code looks up among the stored ones (reading past the end of the lists before).
        spec = isotropic(2, 4, 3);
        spec.c = {0, 90, 100, 200};
        QTest::newRow("isym 2 without C180") << spec.lines() << "Missing C180-plane";

        spec = isotropic(3, 4, 3);
        spec.c = {0, 100, 180, 300};
        QTest::newRow("isym 3 without C90") << spec.lines() << "Missing C90-plane";

        spec = isotropic(3, 4, 3);
        spec.c = {0, 90, 180, 200};
        QTest::newRow("isym 3 without C270") << spec.lines() << "Missing C270-plane";

        spec = isotropic(3, 6, 4);
        spec.c = {0, 30, 60, 90, 180, 270};
        QTest::newRow("isym 3 with planes missing after C90") << spec.lines() << "Wrong C-planes scheme";

        QTest::newRow("isym 4 without C90") << isotropic(4, 10, 3).lines() << "Missing C90-plane";

        QTest::newRow("isym 0 with Mc < 4") << isotropic(0, 2, 2).lines() << "Wrong C-planes scheme";
        QTest::newRow("isym 2 with Mc < 3") << isotropic(2, 2, 2).lines() << "Wrong C-planes scheme";
        QTest::newRow("isym 3 with Mc < 3") << isotropic(3, 2, 2).lines() << "Wrong C-planes scheme";
        QTest::newRow("isym 4 with Mc < 2") << isotropic(4, 2, 1).lines() << "Wrong C-planes scheme";
    }
    void validate_errors() {
        QFETCH(QStringList, lines);
        QFETCH(QString, error);
        QVERIFY2(loadError(lines).startsWith(error), qPrintable(loadError(lines)));
    }

    void validate_warnings_data() {
        QTest::addColumn<int>("index");
        QTest::addColumn<QString>("value");
        QTest::addColumn<QString>("warning");

        QTest::newRow("Mrn") << 7 << QString(79, 'x') << "Measurement report number";
        QTest::newRow("Lnam") << 8 << QString(79, 'x') << "Luminaire name";
        QTest::newRow("Lnum") << 9 << QString(79, 'x') << "Luminaire number";
        QTest::newRow("Fnam") << 10 << QString(9, 'x') << "File name";
        QTest::newRow("Date") << 11 << QString(79, 'x') << "Date/user";
        QTest::newRow("L > 9999") << 12 << "10000" << "Length/diameter of luminaire";
        QTest::newRow("B < 0") << 13 << "-1" << "Width of luminaire";
        QTest::newRow("B > 9999") << 13 << "10000" << "Width of luminaire";
        QTest::newRow("H < 0") << 14 << "-1" << "Height of luminaire";
        QTest::newRow("H > 9999") << 14 << "10000" << "Height of luminaire";
        QTest::newRow("La < 0") << 15 << "-1" << "Length/diameter of luminous area";
        QTest::newRow("La > 9999") << 15 << "10000" << "Length/diameter of luminous area";
        QTest::newRow("B1 < 0") << 16 << "-1" << "Width of luminous area";
        QTest::newRow("B1 > 9999") << 16 << "10000" << "Width of luminous area";
        QTest::newRow("HC0 < 0") << 17 << "-1" << "Height of luminous area C0-plane";
        QTest::newRow("HC0 > 9999") << 17 << "10000" << "Height of luminous area C0-plane";
        QTest::newRow("HC90 < 0") << 18 << "-1" << "Height of luminous area C90-plane";
        QTest::newRow("HC90 > 9999") << 18 << "10000" << "Height of luminous area C90-plane";
        QTest::newRow("HC180 < 0") << 19 << "-1" << "Height of luminous area C180-plane";
        QTest::newRow("HC180 > 9999") << 19 << "10000" << "Height of luminous area C180-plane";
        QTest::newRow("HC270 < 0") << 20 << "-1" << "Height of luminous area C270-plane";
        QTest::newRow("HC270 > 9999") << 20 << "10000" << "Height of luminous area C270-plane";
        QTest::newRow("DFF < 0") << 21 << "-1" << "Downward flux fraction";
        QTest::newRow("DFF > 100") << 21 << "101" << "Downward flux fraction";
        QTest::newRow("LORL < 0") << 22 << "-1" << "Light output ratio of luminaire";
        QTest::newRow("LORL > 100") << 22 << "101" << "Light output ratio of luminaire";
        QTest::newRow("CFLI < 0") << 23 << "-1" << "Conversion factor for luminous intensities";
        QTest::newRow("CFLI > 10") << 23 << "11" << "Conversion factor for luminous intensities";
        QTest::newRow("TILT < -180") << 24 << "-181" << "Tilt of luminaire during measurement";
        QTest::newRow("TILT > 180") << 24 << "181" << "Tilt of luminaire during measurement";
        QTest::newRow("NL > 1000") << 26 << "1001" << "Number of lamps";
        QTest::newRow("TL") << 27 << QString(25, 'x') << "Type of lamps";
        QTest::newRow("TLF < 1") << 28 << "0" << "Total luminous flux of lamps";
        QTest::newRow("TLF > 9999999") << 28 << "10000000" << "Total luminous flux of lamps";
        QTest::newRow("CA") << 29 << QString(17, 'x') << "Color temperature of lamps";
        QTest::newRow("CRG") << 30 << QString(7, 'x') << "Color rendering index";
        QTest::newRow("WB < 0.1") << 31 << "0" << "Wattage including ballast";
        QTest::newRow("WB > 10000") << 31 << "10001" << "Wattage including ballast";
        QTest::newRow("DR < 0") << 32 << "-1" << "k[1]";
        QTest::newRow("DR > 10") << 32 << "11" << "k[1]";
    }
    void validate_warnings() {
        QFETCH(int, index);
        QFETCH(QString, value);
        QFETCH(QString, warning);
        QStringList lines = TestLdt().lines();
        lines[index] = value;
        Eulumdat ldt;
        const QStringList warnings = load(ldt, lines);
        QCOMPARE(warnings.size(), 1);
        QVERIFY2(warnings[0].startsWith(warning), qPrintable(warnings[0]));
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

    void saveFile_keepsType2() {
        TestLdt spec;
        spec.ityp = 2;
        Eulumdat original;
        load(original, spec.lines());

        const QString path = tmp.filePath("type2.ldt");
        original.saveFile(path);
        Eulumdat reloaded;
        reloaded.loadFile(path);
        QCOMPARE(reloaded.iItyp, 2);
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
