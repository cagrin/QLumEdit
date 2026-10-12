cmake_minimum_required(VERSION 3.16)
project(QLumEditTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

option(QLUMEDIT_SANITIZE "Build with AddressSanitizer and UBSan" OFF)
if(QLUMEDIT_SANITIZE)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

option(QLUMEDIT_COVERAGE "Build with gcov code coverage instrumentation" OFF)
if(QLUMEDIT_COVERAGE)
    add_compile_options(--coverage -O0)
    add_link_options(--coverage)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets Test)

enable_testing()

set(APP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../sources)

# Application model code compiled as-is from ../sources (the qmake app build is unaffected).
add_library(qlumedit_core STATIC
    ${APP_SRC}/eulumdat.h
    ${APP_SRC}/eulumdat.cpp
    ${APP_SRC}/eulumdatcalculate.cpp
    ${APP_SRC}/vars.cpp
)
target_include_directories(qlumedit_core PUBLIC ${APP_SRC})
target_link_libraries(qlumedit_core PUBLIC Qt6::Widgets)

# Widgets reach MainWindow through Vars(), so the GUI is linked as a whole (without main.cpp).
# eulumdatimage.cpp is skipped: it is not part of QLumEdit.pro and does not compile.
add_library(qlumedit_gui STATIC
    ${APP_SRC}/configtabdialog.h ${APP_SRC}/configtabdialog.cpp
    ${APP_SRC}/eulumdattablewidget.h ${APP_SRC}/eulumdattablewidget.cpp
    ${APP_SRC}/eulumdatwidget.h ${APP_SRC}/eulumdatwidget.cpp
    ${APP_SRC}/eulumdatwidget2.h ${APP_SRC}/eulumdatwidget2.cpp
    ${APP_SRC}/eulumdatwidget3.h ${APP_SRC}/eulumdatwidget3.cpp
    ${APP_SRC}/eulumdatwidget4.h ${APP_SRC}/eulumdatwidget4.cpp
    ${APP_SRC}/exportto.cpp
    ${APP_SRC}/maintabwidget.h ${APP_SRC}/maintabwidget.cpp
    ${APP_SRC}/mainwindow.h ${APP_SRC}/mainwindow.cpp
    ${APP_SRC}/newdialog.h ${APP_SRC}/newdialog.cpp
    ${APP_SRC}/renderwidget.h ${APP_SRC}/renderwidget.cpp
    ${APP_SRC}/validator.h ${APP_SRC}/validator.cpp
)
target_link_libraries(qlumedit_gui PUBLIC qlumedit_core)

if(QLUMEDIT_COVERAGE)
    find_program(GCOVR gcovr REQUIRED)
    add_custom_target(coverage
        COMMAND ${CMAKE_CTEST_COMMAND} --test-dir ${CMAKE_BINARY_DIR} --output-on-failure
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
        COMMAND ${GCOVR} -r sources ${CMAKE_BINARY_DIR} --filter sources/ --exclude ".*moc_.*" --delete
                --txt --html-details ${CMAKE_BINARY_DIR}/coverage/index.html
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..
        USES_TERMINAL
        VERBATIM
    )
endif()

function(qlumedit_add_test name lib)
    add_executable(${name} ${name}.cpp tst_ldt.h)
    target_link_libraries(${name} PRIVATE ${lib} Qt6::Test)
    target_compile_definitions(${name} PRIVATE TEMPLATES_DIR="${APP_SRC}/templates")
    add_test(NAME ${name} COMMAND ${name})
    set_tests_properties(${name} PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
    if(TARGET coverage)
        add_dependencies(coverage ${name})
    endif()
endfunction()

qlumedit_add_test(tst_eulumdat qlumedit_core)
qlumedit_add_test(tst_eulumdatcalculate qlumedit_core)
qlumedit_add_test(tst_eulumdatwidget qlumedit_gui)
qlumedit_add_test(tst_eulumdatwidget2 qlumedit_gui)
qlumedit_add_test(tst_eulumdatwidget3 qlumedit_gui)
qlumedit_add_test(tst_eulumdatwidget4 qlumedit_gui)
qlumedit_add_test(tst_eulumdattablewidget qlumedit_gui)
