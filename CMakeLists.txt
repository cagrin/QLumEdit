cmake_minimum_required(VERSION 3.16)
project(QLumEdit LANGUAGES CXX)

option(QLUMEDIT_TESTS "Build unit tests" ON)
option(QLUMEDIT_QMAKE_APP "Build the application with qmake from sources/QLumEdit.pro" OFF)

if(QLUMEDIT_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Temporary wrapper: the application is still built by qmake, CMake only runs it.
if(QLUMEDIT_QMAKE_APP)
    find_package(Qt6 REQUIRED COMPONENTS Core)
    find_program(MAKE_EXECUTABLE NAMES make gmake mingw32-make REQUIRED)
    include(ProcessorCount)
    ProcessorCount(JOBS)
    if(JOBS EQUAL 0)
        set(JOBS 1)
    endif()

    set(APP_BUILD_DIR ${CMAKE_BINARY_DIR}/app)
    add_custom_target(app ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${APP_BUILD_DIR}
        COMMAND ${CMAKE_COMMAND} -E chdir ${APP_BUILD_DIR}
                $<TARGET_FILE:Qt6::qmake> ${CMAKE_CURRENT_SOURCE_DIR}/sources/QLumEdit.pro
        COMMAND ${CMAKE_COMMAND} -E chdir ${APP_BUILD_DIR} ${MAKE_EXECUTABLE} -j${JOBS}
        USES_TERMINAL
        VERBATIM
    )
    # Lets `cmake --build --clean-first` remove the qmake build as well.
    set_property(TARGET app PROPERTY ADDITIONAL_CLEAN_FILES ${APP_BUILD_DIR})
endif()
