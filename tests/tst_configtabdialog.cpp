#include "tst_gui.h"
#include "configtabdialog.h"

using namespace QLumEditTests;

namespace {

template <typename T>
T *findByText(const QDialog &dialog, const char *sourceText) {
    for (T *widget : dialog.findChildren<T *>())
        if (widget->text() == TabDialog::tr(sourceText))
            return widget;
    return nullptr;
}

QRadioButton *restrictedRadio(const QDialog &d) { return findByText<QRadioButton>(d, "Eulumdat/1 restricted (78/24 chars)"); }
QRadioButton *unrestrictedRadio(const QDialog &d) { return findByText<QRadioButton>(d, "limit to 255 chars"); }
QCheckBox *fillBox(const QDialog &d) { return findByText<QCheckBox>(d, "Fill curves of luminous intesity"); }
QCheckBox *legendBox(const QDialog &d) { return findByText<QCheckBox>(d, "Draw legend"); }
QComboBox *languageCombo(const QDialog &d) { return d.findChild<QComboBox *>(); }

QAbstractButton *dialogButton(const QDialog &d, QDialogButtonBox::StandardButton button) {
    return d.findChild<QDialogButtonBox *>()->button(button);
}

QString setting(const QString &key) { return QSettings("Cagrinlabs", "QLumEdit").value(key).toString(); }

void addOptionColumns() {
    QTest::addColumn<QString>("language");
    QTest::addColumn<bool>("restricted");
    QTest::addColumn<bool>("fill");
    QTest::addColumn<bool>("legend");
}

} // namespace

class TestConfigTabDialog : public QObject {
    Q_OBJECT

private slots:
    void init() { useDefaultSettings(); }

    void constructor_showsDefaultsWithoutSettings() {
        TabDialog dialog;
        // Current behavior: no language is selected until one is saved.
        QCOMPARE(languageCombo(dialog)->currentIndex(), -1);
        QVERIFY(unrestrictedRadio(dialog)->isChecked());
        QVERIFY(fillBox(dialog)->isChecked());
        QVERIFY(legendBox(dialog)->isChecked());
    }

    void constructor_showsSavedSettings_data() {
        addOptionColumns();
        QTest::newRow("all on") << "pl" << true << true << true;
        QTest::newRow("all off") << "de" << false << false << false;
    }
    void constructor_showsSavedSettings() {
        QFETCH(QString, language);
        QFETCH(bool, restricted);
        QFETCH(bool, fill);
        QFETCH(bool, legend);
        QSettings settings("Cagrinlabs", "QLumEdit");
        settings.setValue("lang", language);
        settings.setValue("restricted", restricted ? "true" : "false");
        settings.setValue("filldiagram", fill ? "true" : "false");
        settings.setValue("drawlegend", legend ? "true" : "false");

        TabDialog dialog;
        QCOMPARE(languageCombo(dialog)->currentText(), language);
        QCOMPARE(restrictedRadio(dialog)->isChecked(), restricted);
        QCOMPARE(unrestrictedRadio(dialog)->isChecked(), !restricted);
        QCOMPARE(fillBox(dialog)->isChecked(), fill);
        QCOMPARE(legendBox(dialog)->isChecked(), legend);
    }

    void ok_savesSettingsAndAppliesThem_data() {
        addOptionColumns();
        QTest::newRow("all on") << "pl" << true << true << true;
        QTest::newRow("all off") << "en" << false << false << false;
    }
    void ok_savesSettingsAndAppliesThem() {
        QFETCH(QString, language);
        QFETCH(bool, restricted);
        QFETCH(bool, fill);
        QFETCH(bool, legend);
        // accept() applies the line limits through Vars().mainWindow.
        MainWindow window;
        TabDialog dialog(&window);
        languageCombo(dialog)->setCurrentText(language);
        (restricted ? restrictedRadio(dialog) : unrestrictedRadio(dialog))->setChecked(true);
        fillBox(dialog)->setChecked(fill);
        legendBox(dialog)->setChecked(legend);
        Vars().fill = !fill;
        Vars().legend = !legend;

        dialogButton(dialog, QDialogButtonBox::Ok)->click();

        QCOMPARE(dialog.result(), QDialog::Accepted);
        QCOMPARE(setting("lang"), language);
        QCOMPARE(setting("restricted"), restricted ? "true" : "false");
        QCOMPARE(setting("filldiagram"), fill ? "true" : "false");
        QCOMPARE(setting("drawlegend"), legend ? "true" : "false");
        QCOMPARE(Vars().fill, fill);
        QCOMPARE(Vars().legend, legend);
        QCOMPARE(Vars().line, restricted ? 78 : 255);
    }

    void cancel_keepsSettings() {
        MainWindow window;
        TabDialog dialog(&window);
        languageCombo(dialog)->setCurrentText("pl");
        restrictedRadio(dialog)->setChecked(true);
        fillBox(dialog)->setChecked(false);

        dialogButton(dialog, QDialogButtonBox::Cancel)->click();

        QCOMPARE(dialog.result(), QDialog::Rejected);
        QVERIFY(!QSettings("Cagrinlabs", "QLumEdit").contains("lang"));
        QVERIFY(!QSettings("Cagrinlabs", "QLumEdit").contains("filldiagram"));
        QCOMPARE(Vars().line, 255);
    }
};

QTEST_MAIN(TestConfigTabDialog)
#include "tst_configtabdialog.moc"
