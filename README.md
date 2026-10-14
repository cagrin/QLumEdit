[![Coverage Status](https://coveralls.io/repos/github/cagrin/QLumEdit/badge.svg?branch=main)](https://coveralls.io/github/cagrin/QLumEdit?branch=main)

# QLumEdit

### Project history
This project was created nearly 20 years ago as part of my Master's thesis. I built the application with Qt 4.3 and published it on SourceForge in 2007. A Windows installer followed a few years later and an x64 build came with the next version. The latest release is version 1.0.3, built with Qt 6.2.0 and packaged with NSIS 3.06.

### Current status
I continue to maintain and develop this project, keeping it compatible with and buildable on the current **Qt LTS (6.12.0)**.

### Containerization & CI/CD
The ```.devcontainer``` folder contains a development container with a lightweight Fluxbox desktop, which lets you run the application from the container in a web browser.

The ```.github``` folder shows how to build the application on all platforms: Windows, Linux and macOS.

### Unit tests
The ```tests``` folder contains unit tests written with **Qt Test**. They cover the Eulumdat data model (loading, saving and validating LDT files, photometric calculations) as well as the editor widgets. The application and the tests are built with CMake. From the repository root you can:

```bash
cmake --workflow --preset build          # build the application
cmake --workflow --preset test           # build and run the unit tests
```

See [NOTES.md](NOTES.md) for more presets and details, such as running a single test and the location of build outputs.