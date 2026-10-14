#include "tst_gui.h"
#include "validator.h"

using namespace QLumEditTests;

namespace {

void writeFile(const QString &path, const QStringList &lines) {
    QVERIFY(QDir().mkpath(QFileInfo(path).path()));
    QFile f(path);
    QVERIFY(f.open(QFile::WriteOnly | QFile::Text));
    f.write((lines.join('\n') + '\n').toUtf8());
}

QPushButton *findButton(const QDialog &dialog, const char *sourceText) {
    for (QPushButton *button : dialog.findChildren<QPushButton *>())
        if (button->text() == ValidatorDialog::tr(sourceText))
            return button;
    return nullptr;
}

QLabel *statusLabel(const QDialog &dialog) { return dialog.findChild<QLabel *>(); }

QString reportText(const QDialog &dialog) { return dialog.findChild<QTextEdit *>()->toPlainText(); }

// Answers the directory dialog with the given path, or cancels it if the path is empty.
void openDirectory(ValidatorDialog &dialog, const QString &path) {
    handleNextModal([path](QDialog *modal) {
        auto *fileDialog = qobject_cast<QFileDialog *>(modal);
        if (path.isEmpty() || !fileDialog) {
            modal->reject();
            return;
        }
        fileDialog->setDirectory(path);
        modal->accept();
    });
    findButton(dialog, "Open directory...")->click();
}

// Clicks the button as soon as the condition holds; the validator calls processEvents() while it works.
void clickWhen(QAbstractButton *button, std::function<bool()> condition) {
    auto *timer = new QTimer;
    timer->setInterval(0);
    QObject::connect(timer, &QTimer::timeout, [timer, button, condition] {
        if (condition()) {
            timer->stop();
            timer->deleteLater();
            button->click();
        }
    });
    timer->start();
}

} // namespace

class TestValidator : public QObject {
    Q_OBJECT

    std::unique_ptr<QTemporaryDir> tmp;

    // 5 .ldt files: 2 with warnings (3 in total), 1 with an error, 2 valid (one hidden), and a .txt file.
    QString createFiles(bool withSubdir = true) {
        const QString dir = tmp->path();
        TestLdt twoWarnings;
        twoWarnings.iden = QString(79, 'x');
        QStringList lines = twoWarnings.lines();
        lines[kLengthLine] = "0";
        writeFile(dir + "/warnings.ldt", lines);

        TestLdt error;
        error.ityp = 4;
        writeFile(dir + "/error.ldt", error.lines());
        writeFile(dir + "/valid.ldt", TestLdt().lines());
        writeFile(dir + "/.hidden.ldt", TestLdt().lines());
        writeFile(dir + "/notes.txt", {"not a luminaire"});

        lines = TestLdt().lines();
        lines[kLengthLine] = "0";
        writeFile(dir + (withSubdir ? "/sub/nested.ldt" : "/nested.ldt"), lines);
        return dir;
    }

private slots:
    void initTestCase() {
        setRestrictedLimits();
    }

    void init() {
        // A remembered directory of a previous test no longer exists and blocks the directory dialog.
        useDefaultSettings();
        tmp = std::make_unique<QTemporaryDir>();
        QVERIFY(tmp->isValid());
    }

    void openDirectory_reportsWarningsAndErrors() {
        const QString dir = createFiles();
        ValidatorDialog dialog;
        openDirectory(dialog, dir);

        const QString status = statusLabel(dialog)->text();
        QVERIFY2(status.startsWith("Validated <b> 5 </b>files"), qPrintable(status));
        QVERIFY2(status.contains(">2 warnings<"), qPrintable(status));
        QVERIFY2(status.contains(">1 errors<"), qPrintable(status));
        QVERIFY2(status.endsWith(" " + dir), qPrintable(status));

        const QString report = reportText(dialog);
        QVERIFY(report.contains(dir + "/warnings.ldt"));
        QVERIFY(report.contains(dir + "/sub/nested.ldt"));
        QVERIFY(report.contains(dir + "/error.ldt\nerror: Type indicator"));
        QVERIFY(!report.contains("valid.ldt"));
        QVERIFY(!report.contains("notes.txt"));
        QCOMPARE(report.count("warning: "), 3);
        QVERIFY(findButton(dialog, "Open directory...")->isEnabled());
    }

    void openDirectory_cancelKeepsDialogUnchanged() {
        ValidatorDialog dialog;
        openDirectory(dialog, QString());
        QCOMPARE(statusLabel(dialog)->text(), "Choose directory to scan for Eulumdat files");
        QCOMPARE(reportText(dialog), QString());
    }

    void openDirectory_startsInLastDirectory() {
        const QString dir = createFiles();
        ValidatorDialog dialog;
        openDirectory(dialog, dir);
        QCOMPARE(QSettings("Cagrinlabs", "QLumEdit").value("validatordir").toString(), dir);

        QString startDir;
        handleNextModal([&startDir](QDialog *modal) {
            if (auto *fileDialog = qobject_cast<QFileDialog *>(modal))
                startDir = fileDialog->directory().absolutePath();
            modal->reject();
        });
        findButton(dialog, "Open directory...")->click();
        QCOMPARE(startDir, dir);
    }

    // The search stops in getAllSubdirs() with a subfolder, otherwise in getAllFilesFromPath().
    void stopWhileSearching_data() {
        QTest::addColumn<bool>("withSubdir");
        QTest::newRow("in subfolders") << true;
        QTest::newRow("in files") << false;
    }
    void stopWhileSearching() {
        QFETCH(bool, withSubdir);
        const QString dir = createFiles(withSubdir);
        ValidatorDialog dialog;
        QLabel *status = statusLabel(dialog);
        clickWhen(findButton(dialog, "Stop"), [status] { return status->text() == "Searching..."; });
        openDirectory(dialog, dir);

        QCOMPARE(status->text(), "Choose directory to scan for Eulumdat files");
        QCOMPARE(reportText(dialog), QString());
        QVERIFY(findButton(dialog, "Open directory...")->isEnabled());
    }

    // Current behavior: the count shows the index of the file stopped at, 0 after the first file.
    void stopWhileValidating_data() {
        QTest::addColumn<bool>("close");
        QTest::newRow("Stop button") << false;
        QTest::newRow("Close button") << true;
    }
    void stopWhileValidating() {
        QFETCH(bool, close);
        const QString dir = createFiles();
        ValidatorDialog dialog;
        dialog.show();
        QLabel *status = statusLabel(dialog);
        QAbstractButton *button = close ? dialog.findChild<QDialogButtonBox *>()->button(QDialogButtonBox::Close)
                                        : findButton(dialog, "Stop");
        clickWhen(button, [status] { return status->text().startsWith("<b>"); });
        openDirectory(dialog, dir);

        QVERIFY2(status->text().startsWith("Stopped. Validated <b> 0 / 5 </b>files"), qPrintable(status->text()));
        QVERIFY2(status->text().endsWith(" " + dir), qPrintable(status->text()));
        QCOMPARE(dialog.isVisible(), !close);
    }
};

QTEST_MAIN(TestValidator)
#include "tst_validator.moc"
