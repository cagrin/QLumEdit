# How to make universal build on MacOS

Configure with both architectures and deploy the bundle:

```bash
cmake --preset build -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64"
cmake --build --preset build
macdeployqt /tmp/qlumedit/build/QLumEdit.app
```

# How to build and run unit tests

The application and the tests are built with CMake (Qt 6.9 or newer is required).
In the dev container CMake, Ninja and gcovr are installed by the `apt-packages` feature.

Run from the repository root (presets are in `CMakePresets.json`):

```bash
cmake --workflow --preset build          # build the application (Release)
cmake --workflow --preset build-windows  # Windows only: build + windeployqt into /tmp/qlumedit/build/windows
cmake --workflow --preset test           # configure, build and run tests
cmake --workflow --preset test-coverage  # the same with code coverage (gcovr)
cmake --workflow --preset test-sanitize  # the same with AddressSanitizer and UBSan
```

Build directories are in `/tmp/qlumedit/<preset>`, because the workspace mount does not allow CMake to set file permissions.
The application executable is `/tmp/qlumedit/build/QLumEdit`.
The HTML coverage report is in `/tmp/qlumedit/test-coverage/coverage/index.html`.

Outside the dev container set `export CMAKE_PREFIX_PATH=<path to Qt>/6.12.0/gcc_64` if Qt is not in `PATH`.

## Run a single test

```bash
ctest --preset test -R tst_eulumdatcalculate
QT_QPA_PLATFORM=offscreen /tmp/qlumedit/test/tests/tst_eulumdat loadFile_readsFields
```

# Translations

Languages are listed in `qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES ...)` in `CMakeLists.txt`.
The `.qm` files are generated during the build, with Qt's `qtbase` translations merged in, and embedded under `:/translations/`.
To refresh `sources/translations/*.ts` after changing `tr()` strings:

```bash
cmake --build --preset build --target update_translations
```

