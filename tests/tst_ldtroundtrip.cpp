#include "tst_gui.h"

#include <memory>

using namespace QLumEditTests;

// Characterization of LDT saving: the bundled templates must be saved unchanged,
// and the normalization of unusual input (number formatting, precision) is pinned down.

namespace {

// Line indexes in TestLdt::lines().
constexpr int kLorlLine = 22;
constexpr int kTiltLine = 24;
constexpr int kLampFluxLine = 28;
constexpr int kWattageLine = 31;
constexpr int kFirstDirectRatioLine = 32;
constexpr int kFirstIntensityLine = 46;

void saveWithModel(const QString &input, const QString &output) {
    Eulumdat ldt;
    ldt.loadFile(input);
    ldt.saveFile(output);
}

// Opens a copy of the file in the main window and saves it with "Save" (QFileDialog is not deterministic offscreen).
void saveWithApp(const QString &input, const QString &output) {
    QVERIFY(QFile::copy(input, output));
    std::unique_ptr<Eulumdat> model;
    MainWindow window;
    window.loadFile(output);
    model.reset(window.ldt);

    QAction *save = findAction(window, "&Save");
    QVERIFY(save);
    // Save is enabled only after an edit.
    window.documentWasModified();
    save->trigger();
}

} // namespace

class TestLdtRoundTrip : public QObject {
    Q_OBJECT

    QTemporaryDir tmp;
    int counter = 0;

    QString outputPath() { return tmp.filePath(QString("out%1.ldt").arg(counter++)); }

    void addTemplateRows() {
        QTest::addColumn<QString>("name");
        for (const QString &f : QDir(TEMPLATES_DIR).entryList({"*.ldt"}, QDir::Files, QDir::Name))
            QTest::newRow(qPrintable(f)) << f;
    }

    // Line of TestLdt::lines() replaced by input, and the line expected after saving with the model
    // and with the app (the editor widgets round and clamp values to their decimals and ranges).
    void addNormalizationRows() {
        QTest::addColumn<int>("line");
        QTest::addColumn<QString>("input");
        QTest::addColumn<QString>("model");
        QTest::addColumn<QString>("app");
        QTest::newRow("comma and underscore") << kLengthLine << "1_000,5" << "1000.5" << "1000.5";
        QTest::newRow("trailing zeros") << kLengthLine << "77.710" << "77.71" << "77.7";
        QTest::newRow("length 7 significant digits") << kLengthLine << "1.234567" << "1.23457" << "1.2";
        QTest::newRow("LORL 3 decimals") << kLorlLine << "77.716" << "77.716" << "77.72";
        QTest::newRow("tilt 2 decimals") << kTiltLine << "12.34" << "12.34" << "12.3";
        QTest::newRow("flux decimals") << kLampFluxLine << "1000.6" << "1000.6" << "1001";
        QTest::newRow("flux 7 digits") << kLampFluxLine << "1234567" << "1.23457e+06" << "1.23457e+06";
        QTest::newRow("wattage 2 decimals") << kWattageLine << "10.26" << "10.26" << "10.3";
        QTest::newRow("direct ratio 5 decimals") << kFirstDirectRatioLine << "0.12345" << "0.12345" << "0.123";
        QTest::newRow("direct ratio above 1") << kFirstDirectRatioLine << "1.5" << "1.5" << "1";
        QTest::newRow("intensity 7 significant digits") << kFirstIntensityLine << "123.4567" << "123.457" << "123.457";
    }

private slots:
    void initTestCase() {
        QVERIFY(tmp.isValid());
        useDefaultSettings();
    }

    void templateWithModel_data() { addTemplateRows(); }
    void templateWithModel() {
        QFETCH(QString, name);
        const QString original = QDir(TEMPLATES_DIR).filePath(name);
        const QString saved = outputPath();
        saveWithModel(original, saved);
        const QString diff = firstDifference(readLines(saved), readLines(original));
        QVERIFY2(diff.isEmpty(), qPrintable(diff));
    }

    void templateWithApp_data() { addTemplateRows(); }
    void templateWithApp() {
        QFETCH(QString, name);
        const QString original = QDir(TEMPLATES_DIR).filePath(name);
        const QString saved = outputPath();
        saveWithApp(original, saved);
        const QString diff = firstDifference(readLines(saved), readLines(original));
        QVERIFY2(diff.isEmpty(), qPrintable(diff));
    }

    void normalizationWithModel_data() { addNormalizationRows(); }
    void normalizationWithModel() {
        QFETCH(int, line);
        QFETCH(QString, input);
        QFETCH(QString, model);
        QStringList lines = TestLdt().lines();
        lines[line] = input;
        const QString saved = outputPath();
        saveWithModel(writeLdt(tmp, lines), saved);
        QCOMPARE(readLines(saved).value(line), model);
    }

    void normalizationWithApp_data() { addNormalizationRows(); }
    void normalizationWithApp() {
        QFETCH(int, line);
        QFETCH(QString, input);
        QFETCH(QString, app);
        QStringList lines = TestLdt().lines();
        lines[line] = input;
        const QString saved = outputPath();
        saveWithApp(writeLdt(tmp, lines), saved);
        QCOMPARE(readLines(saved).value(line), app);
    }
};

QTEST_MAIN(TestLdtRoundTrip)
#include "tst_ldtroundtrip.moc"
