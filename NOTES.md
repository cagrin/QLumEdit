# How to make universal build on MacOS

Configure with both architectures and deploy the bundle:

```bash
cmake --preset build -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64"
cmake --build --preset build
macdeployqt /tmp/qlumedit/build/QLumEdit.app
```

# How to build the Windows installer

Requires NSIS 3 (`winget install NSIS.NSIS`); `makensis` is searched in `PATH` and in `Program Files (x86)/NSIS`.

```bash
cmake --workflow --preset installer-windows
```

This runs the `build-windows` steps (build + windeployqt) and then `makensis` on `tools/installer/QLumEdit-x64.nsi`.
The version is taken from `project(QLumEdit VERSION ...)` in `CMakeLists.txt`.
The installer is written to `/tmp/qlumedit/build/QLumEdit-<version>-windows-x64-installer.exe`.

# How to build and run unit tests

The application and the tests are built with CMake (Qt 6.9 or newer is required).
In the dev container CMake, Ninja and gcovr are installed by the `apt-packages` feature.

Run from the repository root (presets are in `CMakePresets.json`):

```bash
cmake --workflow --preset build              # build the application (Release)
cmake --workflow --preset build-windows      # Windows only: build + windeployqt into /tmp/qlumedit/build/windows
cmake --workflow --preset installer-windows  # Windows only: build-windows + NSIS installer
cmake --workflow --preset test               # configure, build and run tests
cmake --workflow --preset test-coverage      # the same with code coverage (gcovr)
cmake --workflow --preset test-sanitize      # the same with AddressSanitizer and UBSan
cmake --workflow --preset test-screenshots   # compare screenshots with tests/screenshots/<os>/<language>/
cmake --workflow --preset test-exports       # compare HTML/PNG/IES exports with tests/exports/<os>/
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

## Screenshots

The screenshot test is excluded from the `test` and `test-sanitize` presets.
It renders the main window tabs and the dialogs in every language and compares them pixel by pixel with the references.
The current images are written to `/tmp/qlumedit/test/tests/screenshots/`.
To refresh the references after an intended UI change:

```bash
QLUMEDIT_UPDATE_SCREENSHOTS=1 cmake --workflow --preset test-screenshots
```

## Exports

The export test is excluded from the `test` and `test-sanitize` presets.
It exports the example templates to HTML/PNG in every language and to IES,
and compares the files with the references in `tests/exports/<os>/`.
The current files are written to `/tmp/qlumedit/test/tests/exports/`.
To refresh the references after an intended change of the export:

```bash
QLUMEDIT_UPDATE_EXPORTS=1 cmake --workflow --preset test-exports
```

# Translations

Languages are listed in `qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES ...)` in `CMakeLists.txt`.
The `.qm` files are generated during the build, with Qt's `qtbase` translations merged in, and embedded under `:/translations/`.
To refresh `sources/translations/*.ts` after changing `tr()` strings:

```bash
cmake --build --preset build --target update_translations
```
