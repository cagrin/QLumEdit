#include "tst_gui.h"

#include <memory>

using namespace QLumEditTests;

// Exports the example templates from the File > Export as menu and compares the files with
// references in tests/exports/<kernel>/ (HTML and PNG per language, IES once).
// QLUMEDIT_UPDATE_EXPORTS=1 writes the current files as the new references.

namespace {

QString referenceDir() {
    return QString(EXPORTS_DIR) + "/" + QSysInfo::kernelType();
}

bool writeFile(const QString &path, const QByteArray &data) {
    QFile f(path);
    return QDir().mkpath(QFileInfo(path).path()) && f.open(QFile::WriteOnly) && f.write(data) == data.size();
}

QByteArray readFile(const QString &path) {
    QFile f(path);
    return f.open(QFile::ReadOnly) ? f.readAll() : QByteArray();
}

// Answers the save dialog of the export action with the given path.
void exportWithApp(MainWindow &window, const char *actionText, const QString &path) {
    QVERIFY(QDir().mkpath(QFileInfo(path).path()));
    // An existing file would open an overwrite confirmation.
    QFile::remove(path);
    QAction *action = findAction(window, actionText);
    QVERIFY(action);
    QVERIFY(action->isEnabled());
    handleNextModal([path](QDialog *dialog) {
        if (auto *fileDialog = qobject_cast<QFileDialog *>(dialog)) {
            fileDialog->setDirectory(QFileInfo(path).path());
            fileDialog->selectFile(QFileInfo(path).fileName());
        }
        dialog->accept();
    });
    action->trigger();
    QVERIFY2(QFile::exists(path), qPrintable("Not exported: " + path));
}

// Compares the exported file with the reference (or updates the reference).
void verifyExport(const QString &actualPath, const QString &fileName) {
    QVERIFY2(QFile::exists(actualPath), qPrintable("Not exported: " + actualPath));
    const QString referencePath = referenceDir() + "/" + fileName;
    if (qEnvironmentVariableIsSet("QLUMEDIT_UPDATE_EXPORTS")) {
        QVERIFY2(writeFile(referencePath, readFile(actualPath)), qPrintable("Cannot write " + referencePath));
        return;
    }
    if (!QFile::exists(referencePath))
        QSKIP(qPrintable("No reference " + referencePath + ", run with QLUMEDIT_UPDATE_EXPORTS=1"));

    if (fileName.endsWith(".png")) {
        const QImage actual = QImage(actualPath).convertToFormat(QImage::Format_ARGB32);
        const QImage expected = QImage(referencePath).convertToFormat(QImage::Format_ARGB32);
        QVERIFY2(actual == expected, qPrintable(QString("%1 differs from %2").arg(actualPath, referencePath)));
    } else {
        const QString diff = firstDifference(readLines(actualPath), readLines(referencePath));
        QVERIFY2(diff.isEmpty(), qPrintable(QString("%1 differs from %2: %3").arg(actualPath, referencePath, diff)));
    }
}

} // namespace

class TestExports : public QObject {
    Q_OBJECT

    std::unique_ptr<Eulumdat> model;
    std::unique_ptr<MainWindow> window;

    void openTemplate(const QString &name) {
        window = std::make_unique<MainWindow>();
        window->loadFile(QString(TEMPLATES_DIR) + "/" + name + ".ldt");
        model.reset(window->ldt);
    }

private slots:
    void initTestCase() { useDefaultSettings(); }

    void cleanup() {
        window.reset();
        model.reset();
    }

    void html_data() {
        addLanguageColumns();
        QTest::addColumn<QString>("name");
        for (const auto &[language, locale] : kLanguages)
            for (const QString &name : kExampleTemplates)
                QTest::addRow("%s/%s", qPrintable(language), qPrintable(name)) << language << locale << name;
    }
    void html() {
        QFETCH(QString, language);
        QFETCH(QString, locale);
        QFETCH(QString, name);
        QTranslator translator;
        QVERIFY(installLanguage(translator, language, locale));
        openTemplate(name);

        const QString fileName = language + "/" + name + ".html";
        const QString path = QString(EXPORTS_OUTPUT_DIR) + "/" + fileName;
        exportWithApp(*window, "HTML/PNG...", path);
        verifyExport(path, fileName);
        // The diagram is written next to the HTML file; its labels are not translated, so one reference serves all languages.
        verifyExport(QString(path).replace(".html", ".png"), name + ".png");
    }

    void ies_data() {
        QTest::addColumn<QString>("name");
        for (const QString &name : kExampleTemplates)
            QTest::newRow(qPrintable(name)) << name;
    }
    void ies() {
        QFETCH(QString, name);
        openTemplate(name);

        const QString fileName = name + ".ies";
        const QString path = QString(EXPORTS_OUTPUT_DIR) + "/" + fileName;
        exportWithApp(*window, "Iesna LM-63-1995...", path);
        verifyExport(path, fileName);
    }
};

QTEST_MAIN(TestExports)
#include "tst_exports.moc"
