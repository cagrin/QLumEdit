# QLumEdit

### Project history
This project was created nearly 20 years ago as part of my Master's thesis. The application, built with the Qt 4.3 framework, was published on SourceForge in 2007. A few years later, I added the first Windows installer, and in the following version also an x64 build. The latest version, 1.0.3, was built with Qt 6.2.0 and packaged with the NSIS 3.06 installer.

### Current status
I continue to maintain and develop this project, keeping it compatible with and buildable on the current **Qt LTS (6.12.0)**.

### Containerization & CI/CD
The ```.devcontainer``` folder contains a development container with a lightweight Fluxbox desktop, which lets you run the application from the container in a web browser.

The ```.github``` folder shows how to build the application on all platforms: Windows, Linux and macOS.

### Unit tests
The ```tests``` folder contains unit tests written with **Qt Test**. They cover the Eulumdat data model (loading, saving and validating LDT files, photometric calculations) as well as the editor widgets. Each source file ```sources/<name>.cpp``` has a matching test file ```tests/tst_<name>.cpp```; shared test helpers are in ```tests/tst_ldt.h```.

The tests are built with CMake, while the application itself is still built with qmake from ```sources/QLumEdit.pro```. From the repository root you can:

```bash
cmake --workflow --preset build          # build the application (qmake + make)
cmake --workflow --preset test           # build and run the unit tests
cmake --workflow --preset test-coverage  # run the tests and generate a code coverage report
cmake --workflow --preset test-sanitize  # run the tests with AddressSanitizer and UBSan
```

See [NOTES.md](NOTES.md) for details, such as running a single test and the location of build outputs.