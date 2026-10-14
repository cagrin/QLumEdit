#include "tst_gui.h"
#include "newdialog.h"

using namespace QLumEditTests;

namespace {

QAction *action(const MainWindow &window, const char *sourceText) {
    QAction *a = findAction(window, sourceText);
    if (!a)
        qFatal("No action %s", sourceText);
    return a;
}

QMenu *exportMenu(const MainWindow &window) {
    for (QMenu *menu : window.findChildren<QMenu *>())
        if (menu->title() == MainWindow::tr("Export as"))
            return menu;
    return nullptr;
}

bool isEditing(const MainWindow &window) { return qobject_cast<MainTabWidget *>(window.centralWidget()); }

const QString kSavePrompt = "QLumEdit: The file has been modified.\nDo you want to save your changes?";

QString setting(const QString &key) { return QSettings("Cagrinlabs", "QLumEdit").value(key).toString(); }

// Calls the handler for every modal dialog shown while the owner exists.
void answerModals(QObject *owner, std::function<void(QDialog *)> handler) {
    auto *timer = new QTimer(owner);
    timer->setInterval(10);
    QObject::connect(timer, &QTimer::timeout, [handler] {
        auto *dialog = qobject_cast<QDialog *>(QApplication::activeModalWidget());
        if (dialog && dialog->isVisible())
            handler(dialog);
    });
    timer->start();
}

// Records "title: text" of a message box and closes it with the button.
void answerMessageBox(QDialog *dialog, QStringList &shown, QMessageBox::StandardButton button = QMessageBox::Ok) {
    auto *box = qobject_cast<QMessageBox *>(dialog);
    if (!box) {
        shown << "unexpected " + QString(dialog->metaObject()->className());
        dialog->reject();
        return;
    }
    shown << box->windowTitle() + ": " + box->text();
    if (QAbstractButton *b = box->button(button))
        b->click();
    else
        box->reject();
}

// Types the path into the file name field; QFileDialog::selectFile() ignores files not yet listed in Open mode.
void selectFile(QDialog *dialog, const QString &path) {
    auto *fileDialog = qobject_cast<QFileDialog *>(dialog);
    auto *fileName = fileDialog ? fileDialog->findChild<QLineEdit *>("fileNameEdit") : nullptr;
    if (!fileName) {
        dialog->reject();
        return;
    }
    fileName->setText(path);
    dialog->accept();
}

QPushButton *findButton(const QDialog *dialog, const char *sourceText) {
    for (QPushButton *button : dialog->findChildren<QPushButton *>())
        if (button->text() == NewDialog::tr(sourceText))
            return button;
    return nullptr;
}

} // namespace

class TestMainWindow : public QObject {
    Q_OBJECT

    QTemporaryDir tmp;
    int counter = 0;

    QString path(const QString &suffix = ".ldt") { return tmp.filePath(QString("f%1%2").arg(counter++).arg(suffix)); }

    QString writeTestLdt(const TestLdt &spec = TestLdt()) {
        const QString p = path();
        QFile f(p);
        if (f.open(QFile::WriteOnly | QFile::Text))
            f.write((spec.lines().join('\n') + '\n').toUtf8());
        return p;
    }

    // Loads a file and edits the identification, so the document is modified.
    void loadAndModify(MainWindow &window, const QString &file) {
        window.loadFile(file);
        QVERIFY(isEditing(window));
        window.central->general->LineEdit1->setText("Edited");
        QVERIFY(action(window, "&Save")->isEnabled());
    }

private slots:
    void initTestCase() { QVERIFY(tmp.isValid()); }
    void init() { useDefaultSettings(); }

    void constructor_startsWithoutFile() {
        MainWindow window;
        QCOMPARE(window.windowTitle(), "QLumEdit");
        QVERIFY(!isEditing(window));
        QVERIFY(!action(window, "&Save")->isEnabled());
        QVERIFY(!action(window, "Save &As...")->isEnabled());
        QVERIFY(!exportMenu(window)->isEnabled());
        QCOMPARE(window.statusBar()->currentMessage(), "Ready");
    }

    void constructor_appliesValidatorOptions_data() {
        QTest::addColumn<QString>("restricted");
        QTest::addColumn<int>("line");
        QTest::addColumn<int>("lineFNam");
        QTest::newRow("restricted") << "true" << 78 << 8;
        QTest::newRow("unrestricted") << "false" << 255 << 255;
    }
    void constructor_appliesValidatorOptions() {
        QFETCH(QString, restricted);
        QFETCH(int, line);
        QFETCH(int, lineFNam);
        QSettings("Cagrinlabs", "QLumEdit").setValue("restricted", restricted);
        MainWindow window;
        QCOMPARE(Vars().line, line);
        QCOMPARE(Vars().lineFNam, lineFNam);
    }

    void loadFile_opensEditor() {
        MainWindow window;
        const QString file = writeTestLdt();
        window.loadFile(file);

        QVERIFY(isEditing(window));
        QCOMPARE(window.ldt->sIden, "Test luminaire");
        QCOMPARE(window.windowTitle(), QFileInfo(file).fileName() + "[*] - QLumEdit");
        QVERIFY(!action(window, "&Save")->isEnabled());
        QVERIFY(action(window, "Save &As...")->isEnabled());
        QVERIFY(exportMenu(window)->isEnabled());
        QCOMPARE(window.statusBar()->currentMessage(), "File loaded");
    }

    void loadFile_reportsMissingFile() {
        MainWindow window;
        window.loadFile(writeTestLdt());
        const QString missing = path();
        QStringList shown;
        answerModals(&window, [&shown](QDialog *d) { answerMessageBox(d, shown); });

        window.loadFile(missing);

        QCOMPARE(shown.size(), 1);
        QVERIFY2(shown[0].startsWith("QLumEdit: Cannot read file " + missing + ":\n"), qPrintable(shown[0]));
        // The previous file stays open.
        QVERIFY(isEditing(window));
        QCOMPARE(window.statusBar()->currentMessage(), "File loaded");
    }

    void loadFile_showsWarningsAndOpensEditor() {
        QSettings("Cagrinlabs", "QLumEdit").setValue("restricted", "true");
        MainWindow window;
        TestLdt spec;
        spec.iden = QString(100, 'x');
        QStringList shown;
        answerModals(&window, [&shown](QDialog *d) { answerMessageBox(d, shown); });

        window.loadFile(writeTestLdt(spec));

        QCOMPARE(shown.size(), 1);
        QVERIFY2(shown[0].startsWith("Loading file: <b>1 warnings:</b><br><br>Identification = 100"), qPrintable(shown[0]));
        QVERIFY2(shown[0].endsWith("<br><b>Auto corrected.</b>"), qPrintable(shown[0]));
        QVERIFY(isEditing(window));
        QCOMPARE(window.statusBar()->currentMessage(), "File loaded");
    }

    void loadFile_reportsErrors_data() {
        QTest::addColumn<int>("line");
        QTest::addColumn<QString>("value");
        QTest::addColumn<QString>("error");
        QTest::newRow("type out of range") << kTypeLine << "5" << "Type indicator = 5";
        QTest::newRow("not a number") << kTypeLine << "x" << "Error reading file at";
    }
    void loadFile_reportsErrors() {
        QFETCH(int, line);
        QFETCH(QString, value);
        QFETCH(QString, error);
        MainWindow window;
        const QString previous = writeTestLdt();
        // Edited, as after "Discard" in the save prompt of Open.
        loadAndModify(window, previous);
        QStringList lines = TestLdt().lines();
        lines[line] = value;
        const QString file = writeLdt(tmp, lines);
        QStringList shown;
        answerModals(&window, [&shown](QDialog *d) { answerMessageBox(d, shown); });

        window.loadFile(file);

        QCOMPARE(shown.size(), 1);
        QVERIFY2(shown[0].startsWith("Cannot load file: " + error), qPrintable(shown[0]));
        // The previous file is closed and nothing can be saved or exported.
        QVERIFY(!isEditing(window));
        QVERIFY(!window.central);
        QCOMPARE(window.windowTitle(), "QLumEdit");
        QVERIFY(!action(window, "&Save")->isEnabled());
        QVERIFY(!action(window, "Save &As...")->isEnabled());
        QVERIFY(!exportMenu(window)->isEnabled());
        // The discarded changes are not offered for saving again.
        QVERIFY(window.close());
        QCOMPARE(shown.size(), 1);
        QCOMPARE(readLines(previous).value(0), "Test luminaire");
    }

    void edit_enablesSave() {
        MainWindow window;
        window.loadFile(writeTestLdt());
        QVERIFY(!action(window, "&Save")->isEnabled());

        window.central->luminaire->DoubleSpinBox13->setValue(123);

        QVERIFY(action(window, "&Save")->isEnabled());
    }

    void save_writesEditedValuesToCurrentFile() {
        MainWindow window;
        const QString file = writeTestLdt();
        loadAndModify(window, file);

        action(window, "&Save")->trigger();

        QCOMPARE(readLines(file).value(0), "Edited");
        QCOMPARE(window.statusBar()->currentMessage(), "File saved");
    }

    void save_reportsUnwritableFile() {
        MainWindow window;
        const QString file = writeTestLdt();
        loadAndModify(window, file);
        // A directory in place of the file cannot be opened for writing (also as root).
        QVERIFY(QFile::remove(file));
        QVERIFY(QDir().mkdir(file));
        QStringList shown;
        answerModals(&window, [&shown](QDialog *d) { answerMessageBox(d, shown); });

        action(window, "&Save")->trigger();

        QCOMPARE(shown.size(), 1);
        QVERIFY2(shown[0].startsWith("QLumEdit: Cannot write file " + file + ":\n"), qPrintable(shown[0]));
        QVERIFY(action(window, "&Save")->isEnabled());
    }

    void saveAs_writesChosenFile() {
        MainWindow window;
        window.loadFile(writeTestLdt());
        const QString target = path();
        answerModals(&window, [target](QDialog *d) { selectFile(d, target); });

        action(window, "Save &As...")->trigger();

        QCOMPARE(readLines(target).value(0), "Test luminaire");
        QCOMPARE(window.windowTitle(), QFileInfo(target).fileName() + "[*] - QLumEdit");
        QCOMPARE(setting("savedir"), target);
        QCOMPARE(window.statusBar()->currentMessage(), "File saved");
    }

    void saveAs_cancelWritesNothing() {
        MainWindow window;
        const QString file = writeTestLdt();
        window.loadFile(file);
        const QStringList before = QDir(tmp.path()).entryList(QDir::Files);
        answerModals(&window, [](QDialog *d) { d->reject(); });

        action(window, "Save &As...")->trigger();

        QCOMPARE(QDir(tmp.path()).entryList(QDir::Files), before);
        QCOMPARE(setting("savedir"), QString());
        QCOMPARE(window.windowTitle(), QFileInfo(file).fileName() + "[*] - QLumEdit");
    }

    void close_withoutChangesDoesNotAsk() {
        MainWindow window;
        window.loadFile(writeTestLdt());
        QStringList shown;
        answerModals(&window, [&shown](QDialog *d) { answerMessageBox(d, shown); });

        QVERIFY(window.close());

        QCOMPARE(shown, QStringList());
        QVERIFY(QSettings("Cagrinlabs", "QLumEdit").contains("position"));
    }

    void close_asksAboutChanges_data() {
        QTest::addColumn<int>("button");
        QTest::addColumn<bool>("closed");
        QTest::addColumn<QString>("firstLine");
        QTest::newRow("save") << int(QMessageBox::Save) << true << "Edited";
        QTest::newRow("discard") << int(QMessageBox::Discard) << true << "Test luminaire";
        QTest::newRow("cancel") << int(QMessageBox::Cancel) << false << "Test luminaire";
    }
    void close_asksAboutChanges() {
        QFETCH(int, button);
        QFETCH(bool, closed);
        QFETCH(QString, firstLine);
        MainWindow window;
        const QString file = writeTestLdt();
        loadAndModify(window, file);
        QStringList shown;
        answerModals(&window, [&shown, button](QDialog *d) {
            answerMessageBox(d, shown, QMessageBox::StandardButton(button));
        });

        QCOMPARE(window.close(), closed);

        QCOMPARE(shown, QStringList{kSavePrompt});
        QCOMPARE(readLines(file).value(0), firstLine);
        QCOMPARE(QSettings("Cagrinlabs", "QLumEdit").contains("position"), closed);
    }

    void open_loadsChosenFile() {
        MainWindow window;
        const QString file = writeTestLdt();
        answerModals(&window, [file](QDialog *d) { selectFile(d, file); });

        action(window, "&Open...")->trigger();

        QVERIFY(isEditing(window));
        QCOMPARE(window.windowTitle(), QFileInfo(file).fileName() + "[*] - QLumEdit");
        QCOMPARE(setting("loaddir"), file);
    }

    void open_cancelKeepsCurrentFile() {
        MainWindow window;
        const QString file = writeTestLdt();
        window.loadFile(file);
        answerModals(&window, [](QDialog *d) { d->reject(); });

        action(window, "&Open...")->trigger();

        QVERIFY(isEditing(window));
        QCOMPARE(window.windowTitle(), QFileInfo(file).fileName() + "[*] - QLumEdit");
        QCOMPARE(setting("loaddir"), QString());
    }

    void open_cancelAtSavePromptShowsNoFileDialog() {
        MainWindow window;
        const QString file = writeTestLdt();
        loadAndModify(window, file);
        QStringList shown;
        answerModals(&window, [&shown](QDialog *d) { answerMessageBox(d, shown, QMessageBox::Cancel); });

        action(window, "&Open...")->trigger();

        QCOMPARE(shown, QStringList{kSavePrompt});
        QVERIFY(action(window, "&Save")->isEnabled());
    }

    void new_createsUntitledFileFromExample() {
        MainWindow window;
        answerModals(&window, [](QDialog *d) {
            if (QPushButton *button = findButton(d, "Road luminaire"))
                button->click();
            else
                d->reject();
        });

        action(window, "&New...")->trigger();

        QVERIFY(isEditing(window));
        QCOMPARE(window.ldt->iIsym, 3);
        QCOMPARE(window.windowTitle(), "untitled.ldt[*] - QLumEdit");
        QVERIFY(!action(window, "&Save")->isEnabled());
        QVERIFY(action(window, "Save &As...")->isEnabled());
        QVERIFY(exportMenu(window)->isEnabled());
    }

    void new_cancelClosesCurrentFile() {
        MainWindow window;
        window.loadFile(writeTestLdt());
        answerModals(&window, [](QDialog *d) { d->reject(); });

        action(window, "&New...")->trigger();

        QVERIFY(!isEditing(window));
        QVERIFY(!action(window, "&Save")->isEnabled());
        QVERIFY(!action(window, "Save &As...")->isEnabled());
        QVERIFY(!exportMenu(window)->isEnabled());
    }

    void new_cancelAtSavePromptKeepsEditedFile() {
        MainWindow window;
        const QString file = writeTestLdt();
        loadAndModify(window, file);
        QStringList shown;
        answerModals(&window, [&shown](QDialog *d) {
            if (qobject_cast<NewDialog *>(d)) {
                shown << "NewDialog";
                d->reject();
            } else {
                answerMessageBox(d, shown, QMessageBox::Cancel);
            }
        });

        action(window, "&New...")->trigger();

        QCOMPARE(shown, QStringList{kSavePrompt});
        QVERIFY(isEditing(window));
        QCOMPARE(window.central->general->LineEdit1->text(), "Edited");
        QVERIFY(action(window, "&Save")->isEnabled());
        QCOMPARE(window.windowTitle(), QFileInfo(file).fileName() + "[*] - QLumEdit");
    }

    void new_discardThenCancelLeavesNoFile() {
        MainWindow window;
        const QString file = writeTestLdt();
        loadAndModify(window, file);
        QStringList shown;
        answerModals(&window, [&shown](QDialog *d) {
            if (qobject_cast<NewDialog *>(d)) {
                shown << "NewDialog";
                d->reject();
            } else {
                answerMessageBox(d, shown, QMessageBox::Discard);
            }
        });

        action(window, "&New...")->trigger();

        QCOMPARE(shown, (QStringList{kSavePrompt, "NewDialog"}));
        QVERIFY(!isEditing(window));
        QVERIFY(!window.central);
        QCOMPARE(window.windowTitle(), "QLumEdit");
        QVERIFY(!action(window, "&Save")->isEnabled());
        // The discarded changes are not offered for saving again.
        QVERIFY(window.close());
        QCOMPARE(shown.size(), 2);
        QCOMPARE(readLines(file).value(0), "Test luminaire");
    }

    void save_afterNewAsksForFileName() {
        MainWindow window;
        const QString target = path();
        QStringList shown;
        answerModals(&window, [&shown, target](QDialog *d) {
            if (QPushButton *button = findButton(d, "Projector")) {
                button->click();
            } else if (qobject_cast<QFileDialog *>(d)) {
                shown << "QFileDialog";
                selectFile(d, target);
            } else {
                answerMessageBox(d, shown);
            }
        });
        action(window, "&New...")->trigger();
        window.central->general->LineEdit1->setText("New projector");

        action(window, "&Save")->trigger();

        QCOMPARE(shown, QStringList{"QFileDialog"});
        QCOMPARE(readLines(target).value(0), "New projector");
        QCOMPARE(window.windowTitle(), QFileInfo(target).fileName() + "[*] - QLumEdit");

        // Once saved, "Save" writes to the chosen file without asking.
        window.central->general->LineEdit1->setText("Saved again");
        action(window, "&Save")->trigger();
        QCOMPARE(shown.size(), 1);
        QCOMPARE(readLines(target).value(0), "Saved again");
    }

    void close_untitledCancelInFileDialogKeepsWindowOpen() {
        MainWindow window;
        QStringList shown;
        answerModals(&window, [&shown](QDialog *d) {
            if (QPushButton *button = findButton(d, "Projector")) {
                button->click();
            } else if (qobject_cast<QFileDialog *>(d)) {
                shown << "QFileDialog";
                d->reject();
            } else {
                answerMessageBox(d, shown, QMessageBox::Save);
            }
        });
        action(window, "&New...")->trigger();
        window.central->general->LineEdit1->setText("New projector");

        QVERIFY(!window.close());

        QCOMPARE(shown, (QStringList{kSavePrompt, "QFileDialog"}));
        QVERIFY(isEditing(window));
        QVERIFY(action(window, "&Save")->isEnabled());
    }
};

QTEST_MAIN(TestMainWindow)
#include "tst_mainwindow.moc"
