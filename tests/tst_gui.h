#ifndef TESTS_TST_GUI_H
#define TESTS_TST_GUI_H

#include "tst_ldt.h"
#include "mainwindow.h"

namespace QLumEditTests {

// Language code and the system locale assumed for it.
inline const QList<std::pair<QString, QString>> kLanguages = {{"en", "en_US"}, {"pl", "pl_PL"}, {"de", "de_DE"}};

// Templates shown in the "Examples" tab of the New file dialog.
inline const QStringList kExampleTemplates = {"floor_uplight", "fluorescent_luminaire", "projector", "road_luminaire"};

// Isolates QSettings from the user's configuration, so default options are used.
inline void useDefaultSettings() {
    QStandardPaths::setTestModeEnabled(true);
    QSettings("Cagrinlabs", "QLumEdit").clear();
}

inline void addLanguageColumns() {
    QTest::addColumn<QString>("language");
    QTest::addColumn<QString>("locale");
}

// Same lookup as main(); there is no .qm for English.
inline bool installLanguage(QTranslator &translator, const QString &language, const QString &locale) {
    QLocale::setDefault(QLocale(locale));
    if (language == "en")
        return true;
    return translator.load(":/translations/qlumedit_" + language + ".qm") && QApplication::installTranslator(&translator);
}

inline QAction *findAction(const QWidget &window, const char *sourceText) {
    for (QAction *action : window.findChildren<QAction *>())
        if (action->text() == MainWindow::tr(sourceText))
            return action;
    return nullptr;
}

inline QStringList readLines(const QString &path) {
    QFile f(path);
    if (!f.open(QFile::ReadOnly | QFile::Text))
        return {};
    return QString::fromUtf8(f.readAll()).split('\n');
}

// Returns an empty string if the lines are equal.
inline QString firstDifference(const QStringList &actual, const QStringList &expected) {
    for (int i = 0; i < std::max(actual.size(), expected.size()); i++) {
        const QString a = actual.value(i, "<missing>");
        const QString e = expected.value(i, "<missing>");
        if (a != e)
            return QString("line %1: '%2', expected '%3'").arg(i + 1).arg(a, e);
    }
    return {};
}

} // namespace QLumEditTests

#endif // TESTS_TST_GUI_H
