find_package(Qt6 REQUIRED COMPONENTS Test)

if(QLUMEDIT_COVERAGE)
    find_program(GCOVR gcovr REQUIRED)
    add_custom_target(coverage
        COMMAND ${CMAKE_CTEST_COMMAND} --test-dir ${CMAKE_BINARY_DIR} --output-on-failure
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
        COMMAND ${GCOVR} -r sources ${CMAKE_BINARY_DIR} --filter sources/
                --exclude ".*moc_.*" --exclude "sources/main\\.cpp" --delete
                --txt --html-details ${CMAKE_BINARY_DIR}/coverage/index.html
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        USES_TERMINAL
        VERBATIM
    )
endif()

function(qlumedit_add_test name lib)
    add_executable(${name} ${name}.cpp tst_ldt.h)
    target_link_libraries(${name} PRIVATE ${lib} Qt6::Test)
    target_compile_definitions(${name} PRIVATE TEMPLATES_DIR="${PROJECT_SOURCE_DIR}/sources/templates")
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

qlumedit_add_test(tst_screenshots qlumedit_gui)
set_tests_properties(tst_screenshots PROPERTIES LABELS screenshots)
target_sources(tst_screenshots PRIVATE ${PROJECT_SOURCE_DIR}/sources/application.qrc)
# .qm files built by qt_add_translations() in the root CMakeLists.txt (QM_FILES_OUTPUT_VARIABLE conflicts with RESOURCE_PREFIX).
list(TRANSFORM QT_I18N_TRANSLATED_LANGUAGES REPLACE "(.+)" "${PROJECT_BINARY_DIR}/qlumedit_\\1.qm" OUTPUT_VARIABLE qm_files)
set_source_files_properties(${qm_files} PROPERTIES GENERATED TRUE)
qt_add_resources(tst_screenshots tst_screenshots_translations
    PREFIX /translations
    BASE ${PROJECT_BINARY_DIR}
    FILES ${qm_files}
)
add_dependencies(tst_screenshots QLumEdit_lrelease)
target_compile_definitions(tst_screenshots PRIVATE
    SCREENSHOTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/screenshots"
    SCREENSHOTS_OUTPUT_DIR="${CMAKE_CURRENT_BINARY_DIR}/screenshots"
)
