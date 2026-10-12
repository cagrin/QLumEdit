#ifndef TESTS_TST_LDT_H
#define TESTS_TST_LDT_H

#include <QtTest>
#include <QtWidgets>
#include <cmath>
#include <functional>

#include "eulumdat.h"
#include "vars.h"

namespace QLumEditTests {

constexpr int kTypeLine = 1;
constexpr int kLengthLine = 12;

// Intensity (cd/klm) of an ideal isotropic source: 4*pi*I = 1000 lm.
inline const double kIsotropic = 1000.0 / (4.0 * M_PI);

// Builds the lines of a minimal, valid EULUMDAT file.
struct TestLdt {
    QString iden = "Test luminaire";
    int ityp = 1;
    int isym = 1;
    QList<double> c = {0};
    QList<double> g = {0, 90, 180};
    QList<QList<double>> lcd = {{100, 100, 100}};

    QStringList lines() const {
        auto n = [](double v) { return QString::number(v, 'g', 17); };
        QStringList l;
        l << iden << n(ityp) << n(isym)
          << n(c.size()) << n(c.size() > 1 ? c[1] - c[0] : 0)
          << n(g.size()) << n(g.size() > 1 ? g[1] - g[0] : 0)
          << "MRN-1" << "Name" << "NUM-1" << "test.ldt" << "2026"
          << "100" << "50" << "20" << "90" << "40" << "0" << "0" << "0" << "0" // L, B, H, La, B1, HC0..HC270
          << "100" << "80" << "1" << "0"                                      // DFF, LORL, CFLI, TILT
          << "1" << "1" << "LED" << "1000" << "3000K" << "80" << "10";      // n, NL, TL, TLF, CA, CRG, WB
        for (int i = 0; i < 10; i++)
            l << "0.5";
        for (double v : c)
            l << n(v);
        for (double v : g)
            l << n(v);
        for (const auto &row : lcd)
            for (double v : row)
                l << n(v);
        return l;
    }

    TestLdt &scaled(double factor) {
        for (auto &row : lcd)
            for (double &v : row)
                v *= factor;
        return *this;
    }
};

inline TestLdt isotropic(int isym, int nc, int mc, double gStep = 90) {
    TestLdt ldt;
    ldt.isym = isym;
    ldt.c.clear();
    for (int i = 0; i < nc; i++)
        ldt.c << i * 360.0 / nc;
    ldt.g.clear();
    for (double g = 0; g <= 180; g += gStep)
        ldt.g << g;
    ldt.lcd = QList<QList<double>>(mc, QList<double>(ldt.g.size(), kIsotropic));
    return ldt;
}

inline void addSymmetryRows() {
    QTest::addColumn<int>("isym");
    QTest::addColumn<int>("nc");
    QTest::addColumn<int>("mc");
    QTest::newRow("0: no symmetry") << 0 << 24 << 24;
    QTest::newRow("1: rotational") << 1 << 1 << 1;
    QTest::newRow("2: C0-C180 plane") << 2 << 24 << 13;
    QTest::newRow("3: C90-C270 plane") << 3 << 24 << 13;
    QTest::newRow("4: C0-C180 and C90-C270") << 4 << 24 << 7;
}

inline QString writeLdt(const QTemporaryDir &dir, const QStringList &lines) {
    static int counter = 0;
    const QString path = dir.filePath(QString("t%1.ldt").arg(counter++));
    QFile f(path);
    if (f.open(QFile::WriteOnly | QFile::Text))
        f.write((lines.join('\n') + '\n').toUtf8());
    return path;
}

inline QStringList loadLdt(Eulumdat &ldt, const QTemporaryDir &dir, const QStringList &lines) {
    return ldt.loadFile(writeLdt(dir, lines));
}

// "Restricted" limits from MainWindow::setValidatorOptions().
inline void setRestrictedLimits() {
    Vars().line = 78;
    Vars().lineFNam = 8;
    Vars().lineTL = 24;
    Vars().lineCA = 16;
    Vars().lineCRG = 6;
}

// Calls handler with the next modal dialog (QMessageBox, QInputDialog) once it is shown.
inline void handleNextModal(std::function<void(QDialog *)> handler) {
    auto *timer = new QTimer;
    timer->setInterval(10);
    QObject::connect(timer, &QTimer::timeout, [timer, handler] {
        if (auto *dialog = qobject_cast<QDialog *>(QApplication::activeModalWidget())) {
            timer->stop();
            timer->deleteLater();
            handler(dialog);
        }
    });
    timer->start();
}

} // namespace QLumEditTests

#endif // TESTS_TST_LDT_H
