# How to make universal build on MacOS in Qt

## Modify Build Step

Check all ABIs:
* x86-darwin-generic-mach_o-64bit
* arm-darwin-generic-mach_o-64bit

## Add Custom Process Step

Command: ```%{Qt:QT_HOST_PREFIX}/bin/macdeployqt```

Arguments: ```%{ActiveProject:BuildConfig:Path}/%{ActiveProject:Name}.app -qmldir=%{ActiveProject:NativePath}```

# How to run unit tests

Tests use Qt Test and are built with CMake (the qmake build of the app in `sources/` is not affected).
In the dev container CMake, Ninja and gcovr are installed by the `apt-packages` feature.

Run from the repository root (presets are in `CMakePresets.json`):

```bash
cmake --workflow --preset build          # build the application (runs qmake + make on sources/QLumEdit.pro)
cmake --workflow --preset test           # configure, build and run tests
cmake --workflow --preset test-coverage  # the same with code coverage (gcovr)
cmake --workflow --preset test-sanitize  # the same with AddressSanitizer and UBSan
```

Build directories are in `/tmp/qlumedit/<preset>`, because the workspace mount does not allow CMake to set file permissions.
The application executable is `/tmp/qlumedit/build/app/QLumEdit`.
The HTML coverage report is in `/tmp/qlumedit/test-coverage/coverage/index.html`.

Outside the dev container set `export CMAKE_PREFIX_PATH=<path to Qt>/6.8.3/gcc_64` if Qt is not in `PATH`.

## Run a single test

```bash
ctest --preset test -R tst_eulumdatcalculate
QT_QPA_PLATFORM=offscreen /tmp/qlumedit/test/tests/tst_eulumdat loadFile_readsFields
```

