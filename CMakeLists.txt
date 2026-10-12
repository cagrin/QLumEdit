cmake_minimum_required(VERSION 3.22)
project(QLumEdit VERSION 1.0.3 LANGUAGES CXX)

option(QLUMEDIT_TESTS "Build unit tests" ON)
option(QLUMEDIT_SANITIZE "Build with AddressSanitizer and UBSan" OFF)
option(QLUMEDIT_COVERAGE "Build with gcov code coverage instrumentation" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC_PATH_PREFIX ON)

if(QLUMEDIT_SANITIZE)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

if(QLUMEDIT_COVERAGE)
    add_compile_options(--coverage -O0)
    add_link_options(--coverage)
endif()

# MERGE_QT_TRANSLATIONS needs Qt 6.9.
find_package(Qt6 6.9 REQUIRED COMPONENTS Widgets LinguistTools)
qt_standard_project_setup(I18N_TRANSLATED_LANGUAGES de pl)

add_library(qlumedit_core STATIC
    sources/eulumdat.h sources/eulumdat.cpp
    sources/eulumdatcalculate.cpp
    sources/vars.h sources/vars.cpp
)
target_include_directories(qlumedit_core PUBLIC sources)
target_link_libraries(qlumedit_core PUBLIC Qt6::Widgets)

# Widgets reach MainWindow through Vars(), so the GUI is one library (everything except main.cpp).
add_library(qlumedit_gui STATIC
    sources/configtabdialog.h sources/configtabdialog.cpp
    sources/eulumdattablewidget.h sources/eulumdattablewidget.cpp
    sources/eulumdatwidget.h sources/eulumdatwidget.cpp
    sources/eulumdatwidget2.h sources/eulumdatwidget2.cpp
    sources/eulumdatwidget3.h sources/eulumdatwidget3.cpp
    sources/eulumdatwidget4.h sources/eulumdatwidget4.cpp
    sources/exportto.cpp
    sources/maintabwidget.h sources/maintabwidget.cpp
    sources/mainwindow.h sources/mainwindow.cpp
    sources/newdialog.h sources/newdialog.cpp
    sources/renderwidget.h sources/renderwidget.cpp
    sources/validator.h sources/validator.cpp
)
target_link_libraries(qlumedit_gui PUBLIC qlumedit_core)

qt_add_executable(QLumEdit WIN32 MACOSX_BUNDLE
    sources/main.cpp
    sources/application.qrc
)
target_link_libraries(QLumEdit PRIVATE qlumedit_gui)

if(WIN32)
    target_sources(QLumEdit PRIVATE sources/application.rc)
elseif(APPLE)
    set_source_files_properties(sources/images/socket.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    target_sources(QLumEdit PRIVATE sources/images/socket.icns)
    set_target_properties(QLumEdit PROPERTIES MACOSX_BUNDLE_ICON_FILE socket.icns)
endif()

# Builds :/translations/qlumedit_<lang>.qm with Qt's own qtbase translations merged in.
qt_add_translations(QLumEdit
    SOURCE_TARGETS QLumEdit qlumedit_gui qlumedit_core
    TS_FILE_BASE qlumedit
    TS_OUTPUT_DIRECTORY sources/translations
    NO_GENERATE_PLURALS_TS_FILE
    RESOURCE_PREFIX /translations
    MERGE_QT_TRANSLATIONS
    QT_TRANSLATION_CATALOGS qtbase
)

if(WIN32)
    set(DEPLOY_DIR ${CMAKE_BINARY_DIR}/windows)
    # Translations are embedded in the executable, so Qt's .qm files are not needed.
    add_custom_target(deploy
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${DEPLOY_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${DEPLOY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:QLumEdit> ${DEPLOY_DIR}
        COMMAND $<TARGET_FILE:Qt6::windeployqt> --release --no-translations
                --no-system-d3d-compiler --no-system-dxc-compiler --no-opengl-sw
                --skip-plugin-types imageformats,iconengines,generic,networkinformation,tls
                ${DEPLOY_DIR}/$<TARGET_FILE_NAME:QLumEdit>
        USES_TERMINAL
        VERBATIM
    )
    add_dependencies(deploy QLumEdit)

    find_program(MAKENSIS_EXECUTABLE makensis PATHS "C:/Program Files (x86)/NSIS" "C:/Program Files/NSIS")
    if(MAKENSIS_EXECUTABLE)
        file(TO_NATIVE_PATH ${DEPLOY_DIR} DEPLOY_DIR_NATIVE)
        file(TO_NATIVE_PATH ${CMAKE_BINARY_DIR} OUTPUT_DIR_NATIVE)
        add_custom_target(installer
            COMMAND ${MAKENSIS_EXECUTABLE} /V2
                    /DPRODUCT_VERSION=${PROJECT_VERSION}
                    /DDEPLOY_DIR=${DEPLOY_DIR_NATIVE}
                    /DOUTPUT_DIR=${OUTPUT_DIR_NATIVE}
                    ${CMAKE_SOURCE_DIR}/tools/installer/QLumEdit-x64.nsi
            USES_TERMINAL
            VERBATIM
        )
    else()
        add_custom_target(installer
            COMMAND ${CMAKE_COMMAND} -E echo "makensis not found - install NSIS 3 and reconfigure"
            COMMAND ${CMAKE_COMMAND} -E false
        )
    endif()
    add_dependencies(installer deploy)
endif()

if(QLUMEDIT_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
