#include "tst_gui.h"
#include "renderwidget.h"

using namespace QLumEditTests;

namespace {

constexpr int kSize = 400;
// The diagram fills the image less the 60 px margin.
constexpr double kSide = kSize - 60;

// Four C-planes; the stored planes depend on the symmetry (Isym 3 starts at C90).
TestLdt planes(int isym, const QList<double> &intensities) {
    TestLdt ldt;
    ldt.isym = isym;
    ldt.c = {0, 90, 180, 270};
    ldt.lcd.clear();
    for (double v : intensities)
        ldt.lcd << QList<double>{v, v, v};
    return ldt;
}

// Pure red (the dashed C90-C270 pen), not the light red of the fill.
int redPixels(const QImage &image) {
    int count = 0;
    for (int y = 0; y < image.height(); y++)
        for (int x = 0; x < image.width(); x++) {
            const QColor c = image.pixelColor(x, y);
            if (c.red() - c.green() > 100 && c.red() - c.blue() > 100)
                count++;
        }
    return count;
}

// Pure blue pixels in the bottom left corner, outside the diagram circle: the C0-C180 legend entry.
int legendPixels(const QImage &image) {
    int count = 0;
    for (int y = kSize - 40; y < kSize; y++)
        for (int x = 0; x < 60; x++) {
            const QColor c = image.pixelColor(x, y);
            if (c.blue() - c.red() > 100 && c.blue() - c.green() > 100)
                count++;
        }
    return count;
}

// Inside the curves of all test luminaires, between the grid lines and away from the labels.
QColor insidePixel(const QImage &image) {
    const double r = kSide * 3 / 16 / M_SQRT2;
    return image.pixelColor(qRound(kSize / 2 + r), qRound(kSize / 2 + r));
}

void verifyPoint(const QPointF &actual, double x, double y) {
    QVERIFY2(qAbs(actual.x() - x) < 1e-9 && qAbs(actual.y() - y) < 1e-9,
             qPrintable(QString("(%1, %2), expected (%3, %4)").arg(actual.x()).arg(actual.y()).arg(x).arg(y)));
}

} // namespace

class TestRenderWidget : public QObject {
    Q_OBJECT

    QTemporaryDir tmp;
    int counter = 0;

    // The legend reads the symmetry of the main window's file, so the widget is taken from a main window.
    RenderWidget *load(MainWindow &window, const TestLdt &spec) {
        window.loadFile(writeLdt(tmp, spec.lines()));
        return window.central ? window.central->render : nullptr;
    }

    QImage saved(RenderWidget *render) {
        const QString path = tmp.filePath(QString("d%1.png").arg(counter++));
        render->saveImage(path, kSize);
        return QImage(path).convertToFormat(QImage::Format_ARGB32);
    }

    QImage painted(RenderWidget *render) {
        render->resize(kSize, kSize);
        return render->grab().toImage().convertToFormat(QImage::Format_ARGB32);
    }

private slots:
    void initTestCase() { QVERIFY(tmp.isValid()); }
    void init() { useDefaultSettings(); }

    void constructor_readsOptions_data() {
        QTest::addColumn<QString>("fill");
        QTest::addColumn<QString>("legend");
        QTest::newRow("defaults") << QString() << QString();
        QTest::newRow("both on") << "true" << "true";
        QTest::newRow("fill off") << "false" << "true";
        QTest::newRow("legend off") << "true" << "false";
    }
    void constructor_readsOptions() {
        QFETCH(QString, fill);
        QFETCH(QString, legend);
        QSettings settings("Cagrinlabs", "QLumEdit");
        if (!fill.isNull())
            settings.setValue("filldiagram", fill);
        if (!legend.isNull())
            settings.setValue("drawlegend", legend);
        // Opposite values, so the test shows that the constructor sets them.
        Vars().fill = fill == "false";
        Vars().legend = legend == "false";
        Eulumdat ldt;
        loadLdt(ldt, tmp, TestLdt().lines());

        RenderWidget w(ldt);

        QCOMPARE(Vars().fill, fill != "false");
        QCOMPARE(Vars().legend, legend != "false");
    }

    void setVectors_rotationalSymmetry() {
        Eulumdat ldt;
        loadLdt(ldt, tmp, TestLdt().lines());
        RenderWidget w(ldt);

        // C0 from 0 to 180 degrees, then C180 back to 0 (mirrored); x points to C0, y down (gamma 0).
        QCOMPARE(w.vectorC0C180.size(), 6);
        verifyPoint(w.vectorC0C180[0], 0, 100);
        verifyPoint(w.vectorC0C180[1], 100, 0);
        verifyPoint(w.vectorC0C180[2], 0, -100);
        verifyPoint(w.vectorC0C180[3], 0, -100);
        verifyPoint(w.vectorC0C180[4], -100, 0);
        verifyPoint(w.vectorC0C180[5], 0, 100);
        QCOMPARE(w.vectorC90C270.size(), 0);
        QCOMPARE(w.max, 100.0);
    }

    void setVectors_usesPlanesOfSymmetry_data() {
        QTest::addColumn<int>("isym");
        QTest::addColumn<QList<double>>("intensities");
        QTest::addColumn<QList<double>>("expected"); // intensities drawn for C0, C180, C90, C270
        QTest::newRow("0") << 0 << QList<double>{10, 20, 30, 40} << QList<double>{10, 30, 20, 40};
        QTest::newRow("2") << 2 << QList<double>{10, 20, 30} << QList<double>{10, 30, 20, 20};
        QTest::newRow("3") << 3 << QList<double>{20, 30, 40} << QList<double>{30, 30, 20, 40};
        QTest::newRow("4") << 4 << QList<double>{10, 20} << QList<double>{10, 10, 20, 20};
    }
    void setVectors_usesPlanesOfSymmetry() {
        QFETCH(int, isym);
        QFETCH(QList<double>, intensities);
        QFETCH(QList<double>, expected);
        Eulumdat ldt;
        loadLdt(ldt, tmp, planes(isym, intensities).lines());
        RenderWidget w(ldt);

        // Gamma 90 of each half plane: C0 and C90 first (x > 0), then C180 and C270 (x < 0).
        QCOMPARE(w.vectorC0C180.size(), 6);
        QCOMPARE(w.vectorC90C270.size(), 6);
        verifyPoint(w.vectorC0C180[1], expected[0], 0);
        verifyPoint(w.vectorC0C180[4], -expected[1], 0);
        verifyPoint(w.vectorC90C270[1], expected[2], 0);
        verifyPoint(w.vectorC90C270[4], -expected[3], 0);
    }

    void setVectors_findsMaximumInAnyPlane_data() {
        QTest::addColumn<int>("isym");
        QTest::addColumn<QList<double>>("intensities");
        QTest::newRow("0: C0") << 0 << QList<double>{300, 100, 100, 100};
        QTest::newRow("0: C90") << 0 << QList<double>{100, 300, 100, 100};
        QTest::newRow("0: C180") << 0 << QList<double>{100, 100, 300, 100};
        QTest::newRow("0: C270") << 0 << QList<double>{100, 100, 100, 300};
        QTest::newRow("2: C90") << 2 << QList<double>{100, 300, 100};
        QTest::newRow("2: C180") << 2 << QList<double>{100, 100, 300};
        QTest::newRow("3: C90") << 3 << QList<double>{300, 100, 100};
        QTest::newRow("3: C180") << 3 << QList<double>{100, 300, 100};
        QTest::newRow("3: C270") << 3 << QList<double>{100, 100, 300};
        QTest::newRow("4: C90") << 4 << QList<double>{100, 300};
    }
    void setVectors_findsMaximumInAnyPlane() {
        QFETCH(int, isym);
        QFETCH(QList<double>, intensities);
        Eulumdat ldt;
        loadLdt(ldt, tmp, planes(isym, intensities).lines());
        RenderWidget w(ldt);
        QCOMPARE(w.max, 300.0);
    }

    void render_data() { addSymmetryRows(); }
    void render() {
        QFETCH(int, isym);
        QFETCH(int, nc);
        QFETCH(int, mc);
        MainWindow window;
        RenderWidget *render = load(window, isotropic(isym, nc, mc, 45));
        QVERIFY(render);

        const QImage image = saved(render);

        QCOMPARE(image.size(), QSize(kSize, kSize));
        // Rotational symmetry has no separate C90-C270 curve (and no red legend entry).
        QCOMPARE(redPixels(image) > 0, isym != 1);
        QVERIFY(insidePixel(image) != QColor(Qt::white));
        // The widget is drawn by a copy of the same code (text is antialiased differently).
        const QImage widget = painted(render);
        QCOMPARE(redPixels(widget) > 0, isym != 1);
        QCOMPARE(insidePixel(widget), insidePixel(image));
        QVERIFY(legendPixels(image) > 0);
        QVERIFY(legendPixels(widget) > 0);
    }

    void render_withoutFillOrLegend_data() {
        QTest::addColumn<QString>("key");
        QTest::addColumn<bool>("insideWhite");
        QTest::addColumn<bool>("legend");
        QTest::newRow("fill") << "filldiagram" << true << true;
        QTest::newRow("legend") << "drawlegend" << false << false;
    }
    void render_withoutFillOrLegend() {
        QFETCH(QString, key);
        QFETCH(bool, insideWhite);
        QFETCH(bool, legend);
        MainWindow window;
        const QImage on = saved(load(window, TestLdt()));
        QSettings("Cagrinlabs", "QLumEdit").setValue(key, "false");
        RenderWidget *render = load(window, TestLdt());
        QVERIFY(render);

        const QImage off = saved(render);

        QVERIFY(on != off);
        QCOMPARE(insidePixel(off) == QColor(Qt::white), insideWhite);
        QCOMPARE(legendPixels(off) > 0, legend);
        const QImage widget = painted(render);
        QCOMPARE(insidePixel(widget), insidePixel(off));
        QCOMPARE(legendPixels(widget) > 0, legend);
    }
};

QTEST_MAIN(TestRenderWidget)
#include "tst_renderwidget.moc"
