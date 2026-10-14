#include "tst_gui.h"
#include "newdialog.h"

using namespace QLumEditTests;

namespace {

QPushButton *findButton(const QDialog &dialog, const char *sourceText) {
    for (QPushButton *button : dialog.findChildren<QPushButton *>())
        if (button->text() == NewDialog::tr(sourceText))
            return button;
    return nullptr;
}

// The combo boxes are created in order: type, symmetry, output flux.
QComboBox *comboBox(const QDialog &dialog, int index) {
    return dialog.findChildren<QComboBox *>().value(index);
}

} // namespace

class TestNewDialog : public QObject {
    Q_OBJECT

    // Shows the dialog, clicks the button and returns the chosen file; the dialog must close.
    QString clickAndReturnFile(NewDialog &dialog, QPushButton *button) {
        dialog.show();
        if (!QTest::qWaitForWindowExposed(&dialog))
            return "<not exposed>";
        QTest::mouseClick(button, Qt::LeftButton);
        return dialog.isVisible() ? "<still visible>" : Vars().newFile;
    }

private slots:
    void initTestCase() { useDefaultSettings(); }
    void init() { setRestrictedLimits(); }

    void constructor_clearsNewFileAndListsOptions() {
        Vars().newFile = "previous.ldt";
        NewDialog dialog;

        QCOMPARE(Vars().newFile, QString());
        QCOMPARE(dialog.windowTitle(), "New file");
        QCOMPARE(dialog.findChild<QTabWidget *>()->count(), 2);
        QCOMPARE(comboBox(dialog, 0)->count(), 2);
        QCOMPARE(comboBox(dialog, 1)->count(), 5);
        QCOMPARE(comboBox(dialog, 2)->count(), 3);
        QVERIFY(findButton(dialog, "Create"));
    }

    void rejectLeavesNewFileEmpty() {
        NewDialog dialog;
        handleNextModal([](QDialog *d) { d->reject(); });
        QCOMPARE(dialog.exec(), int(QDialog::Rejected));
        QCOMPARE(Vars().newFile, QString());
    }

    void example_data() {
        QTest::addColumn<QByteArray>("button");
        QTest::addColumn<QString>("name");
        QTest::addColumn<int>("isym");
        QTest::newRow("road") << QByteArray("Road luminaire") << "road_luminaire" << 3;
        QTest::newRow("projector") << QByteArray("Projector") << "projector" << 2;
        QTest::newRow("fluorescent") << QByteArray("Fluorescent luminaire") << "fluorescent_luminaire" << 4;
        QTest::newRow("uplight") << QByteArray("Floor standing uplight") << "floor_uplight" << 0;
    }
    void example() {
        QFETCH(QByteArray, button);
        QFETCH(QString, name);
        QFETCH(int, isym);
        NewDialog dialog;
        QPushButton *pushButton = findButton(dialog, button.constData());
        QVERIFY(pushButton);

        const QString file = clickAndReturnFile(dialog, pushButton);

        QCOMPARE(file, ":/templates/" + name + ".ldt");
        QVERIFY(kExampleTemplates.contains(name));
        Eulumdat ldt;
        QCOMPARE(ldt.loadFile(file), QStringList());
        QCOMPARE(ldt.iIsym, isym);
    }

    void templates_data() {
        QTest::addColumn<int>("type");
        QTest::addColumn<int>("symmetry");
        QTest::addColumn<int>("flux");
        for (int type = 0; type < 2; type++)
            for (int symmetry = 0; symmetry < 5; symmetry++)
                for (int flux = 0; flux < 3; flux++)
                    QTest::addRow("%d-%d-%d", type, symmetry, flux) << type << symmetry << flux;
    }
    void templates() {
        QFETCH(int, type);
        QFETCH(int, symmetry);
        QFETCH(int, flux);
        NewDialog dialog;
        comboBox(dialog, 0)->setCurrentIndex(type);
        comboBox(dialog, 1)->setCurrentIndex(symmetry);
        comboBox(dialog, 2)->setCurrentIndex(flux);

        const QString file = clickAndReturnFile(dialog, findButton(dialog, "Create"));

        QCOMPARE(file, QString(":/templates/%1-%2-%3.ldt").arg(type).arg(symmetry).arg(flux));
        Eulumdat ldt;
        QCOMPARE(ldt.loadFile(file), QStringList());
        // Type indicator: point source (1 with rotational symmetry, 3 otherwise) or linear (2).
        QCOMPARE(ldt.iItyp, type == 1 ? 2 : (symmetry == 1 ? 1 : 3));
        QCOMPARE(ldt.iIsym, symmetry);
        // Output flux: downlighting, downlighting with upward component, uplighting.
        const double downward = ldt.downward();
        if (flux == 0)
            QCOMPARE(downward, 100.0);
        else if (flux == 1)
            QVERIFY2(downward > 0 && downward < 100, qPrintable(QString::number(downward)));
        else
            QCOMPARE(downward, 0.0);
    }
};

QTEST_MAIN(TestNewDialog)
#include "tst_newdialog.moc"
