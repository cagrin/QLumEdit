#include "tst_gui.h"

#include <memory>

using namespace QLumEditTests;

// Renders the main window tabs for the named templates and the dialogs, and compares the result
// with reference images in tests/screenshots/<kernel>/<language>/ (pixel exact).
// QLUMEDIT_UPDATE_SCREENSHOTS=1 writes the current images as the new references.

namespace {

const QStringList kTabs = {"general", "luminaire", "lamps", "utilization_factors", "luminous_intensity", "diagram"};

struct DialogCase {
    const char *name;
    const char *action; // MainWindow::tr() source text of the menu action
    int tab;            // -1 if the dialog has no tabs
};
const QList<DialogCase> kDialogs = {
    {"new_file_0_templates", "&New...", 0},
    {"new_file_1_examples", "&New...", 1},
    {"options", "&Options...", -1},
    {"validator", "Eulumdat &Validator...", -1},
    {"about", "&About", -1},
};

QString referenceDir() {
    return QString(SCREENSHOTS_DIR) + "/" + QSysInfo::kernelType();
}

bool saveImage(const QImage &image, const QString &path) {
    return QDir().mkpath(QFileInfo(path).path()) && image.save(path);
}

QImage grabImage(QWidget *widget) {
    return widget->grab().toImage().convertToFormat(QImage::Format_ARGB32);
}

// Writes the image to the build directory and compares it with the reference (or updates the reference).
void verifyScreenshot(const QImage &actual, const QString &fileName) {
    const QString actualPath = QString(SCREENSHOTS_OUTPUT_DIR) + "/" + fileName;
    QVERIFY2(saveImage(actual, actualPath), qPrintable("Cannot write " + actualPath));

    const QString referencePath = referenceDir() + "/" + fileName;
    if (qEnvironmentVariableIsSet("QLUMEDIT_UPDATE_SCREENSHOTS")) {
        QVERIFY2(saveImage(actual, referencePath), qPrintable("Cannot write " + referencePath));
        return;
    }
    if (!QFile::exists(referencePath))
        QSKIP(qPrintable("No reference " + referencePath + ", run with QLUMEDIT_UPDATE_SCREENSHOTS=1"));

    const QImage expected = QImage(referencePath).convertToFormat(QImage::Format_ARGB32);
    QVERIFY2(actual == expected, qPrintable(QString("%1 differs from %2").arg(actualPath, referencePath)));
}

} // namespace

class TestScreenshots : public QObject {
    Q_OBJECT

private slots:
    void initTestCase() {
        useDefaultSettings();
        // A blinking text cursor would make the images nondeterministic.
        QApplication::setCursorFlashTime(0);
    }

    void tab_data() {
        addLanguageColumns();
        QTest::addColumn<QString>("name");
        QTest::addColumn<int>("tab");
        for (const auto &[language, locale] : kLanguages)
            for (const QString &name : kExampleTemplates)
                for (int tab = 0; tab < kTabs.size(); tab++)
                    QTest::addRow("%s/%s/%d", qPrintable(language), qPrintable(name), tab)
                        << language << locale << name << tab;
    }

    void tab() {
        QFETCH(QString, language);
        QFETCH(QString, locale);
        QFETCH(QString, name);
        QFETCH(int, tab);
        const QString path = QString(TEMPLATES_DIR) + "/" + name + ".ldt";

        QTranslator translator;
        QVERIFY(installLanguage(translator, language, locale));

        std::unique_ptr<Eulumdat> model;
        MainWindow window;
        // Checked after MainWindow sets the validator limits; warnings would block on a modal dialog.
        QCOMPARE(Eulumdat().loadFile(path), QStringList());
        // Same order as main(), so the window keeps the application's default size.
        window.show();
        QVERIFY(QTest::qWaitForWindowExposed(&window));
        window.loadFile(path);
        model.reset(window.ldt);

        QTabWidget *tabs = window.central->tabWidget;
        QCOMPARE(tabs->count(), kTabs.size());
        tabs->setCurrentIndex(tab);
        QApplication::processEvents();

        verifyScreenshot(grabImage(&window), QString("%1/%2_%3_%4.png").arg(language, name).arg(tab).arg(kTabs[tab]));
    }

    void dialog_data() {
        addLanguageColumns();
        QTest::addColumn<QString>("name");
        QTest::addColumn<QByteArray>("action");
        QTest::addColumn<int>("tab");
        for (const auto &[language, locale] : kLanguages)
            for (const DialogCase &d : kDialogs)
                QTest::addRow("%s/%s", qPrintable(language), d.name)
                    << language << locale << QString(d.name) << QByteArray(d.action) << d.tab;
    }

    // Opens the dialog from the menu of a main window without a file, as the user does.
    void dialog() {
        QFETCH(QString, language);
        QFETCH(QString, locale);
        QFETCH(QString, name);
        QFETCH(QByteArray, action);
        QFETCH(int, tab);

        QTranslator translator;
        QVERIFY(installLanguage(translator, language, locale));

        MainWindow window;
        window.show();
        QVERIFY(QTest::qWaitForWindowExposed(&window));
        QAction *menuAction = findAction(window, action.constData());
        QVERIFY(menuAction);

        QImage actual;
        handleNextModal([&](QDialog *dialog) {
            if (QTest::qWaitForWindowExposed(dialog)) {
                if (auto *tabs = dialog->findChild<QTabWidget *>(); tabs && tab >= 0)
                    tabs->setCurrentIndex(tab);
                QApplication::processEvents();
                actual = grabImage(dialog);
            }
            dialog->reject();
        });
        menuAction->trigger();

        QVERIFY(!actual.isNull());
        verifyScreenshot(actual, QString("%1/dialog_%2.png").arg(language, name));
    }
};

QTEST_MAIN(TestScreenshots)
#include "tst_screenshots.moc"
