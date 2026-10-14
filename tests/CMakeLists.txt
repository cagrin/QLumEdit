# Lines like "x = new X(...);" or "X *x = new X(...);". At -O0 GCC adds a never-taken flag check
# to their exception cleanup (the last two branches of the line), so gcovr excludes those lines.
# Written without \s and \w, so the same regex works in CMake and in gcovr (Python).
set(new_pattern "^[ \t]*([A-Za-z0-9_]+[ \t]*\\*[ \t]*)?[A-Za-z0-9_]+[ \t]*=[ \t]*new[ \t]")

# Script mode, run by the coverage target: cmake -DJSON=<gcovr json without the pattern> -P tests/CMakeLists.txt
# Fails if new_pattern would hide anything else than the cleanup branches.
if(CMAKE_SCRIPT_MODE_FILE)
    cmake_minimum_required(VERSION 3.22)
    file(READ "${JSON}" json)
    string(JSON files_count LENGTH "${json}" files)
    math(EXPR files_last "${files_count} - 1")
    set(errors "")
    foreach(i RANGE ${files_last})
        string(JSON file_entry GET "${json}" files ${i})
        string(JSON file_name GET "${file_entry}" file)
        file(READ "${CMAKE_CURRENT_LIST_DIR}/../sources/${file_name}" source)
        # Replace list-special characters; none of them can be matched by new_pattern.
        string(REPLACE "\\" "/" source "${source}")
        string(REPLACE ";" "," source "${source}")
        string(REPLACE "[" "(" source "${source}")
        string(REPLACE "]" ")" source "${source}")
        string(REPLACE "\n" ";" source "${source}")
        string(JSON lines_count LENGTH "${file_entry}" lines)
        if(lines_count EQUAL 0)
            continue()
        endif()
        math(EXPR lines_last "${lines_count} - 1")
        foreach(j RANGE ${lines_last})
            string(JSON line GET "${file_entry}" lines ${j})
            string(JSON number GET "${line}" line_number)
            math(EXPR index "${number} - 1")
            list(GET source ${index} text)
            if(NOT text MATCHES "${new_pattern}")
                continue()
            endif()
            string(JSON count GET "${line}" count)
            string(JSON branches_count LENGTH "${line}" branches)
            if(count EQUAL 0 OR branches_count EQUAL 0)
                continue()
            endif()
            set(counts "")
            math(EXPR branches_last "${branches_count} - 1")
            foreach(k RANGE ${branches_last})
                string(JSON branch_count GET "${line}" branches ${k} count)
                list(APPEND counts ${branch_count})
            endforeach()
            # Expected: only the last two branches (the cleanup) were never taken.
            list(FIND counts 0 first_zero)
            list(GET counts -1 last_count)
            math(EXPR cleanup "${branches_count} - 2")
            if(NOT first_zero EQUAL cleanup OR NOT last_count EQUAL 0)
                string(JOIN ", " counts ${counts})
                list(APPEND errors "${file_name}:${number}: branches ${counts}")
            endif()
        endforeach()
    endforeach()
    if(errors)
        string(JOIN "\n" errors ${errors})
        message(FATAL_ERROR "new_pattern would hide real branches:\n${errors}")
    endif()
    return()
endif()

find_package(Qt6 REQUIRED COMPONENTS Test)

if(QLUMEDIT_COVERAGE)
    find_program(GCOVR gcovr REQUIRED)
    set(gcovr_args -r sources ${CMAKE_BINARY_DIR} --filter sources/
        --exclude ".*moc_.*" --exclude "sources/main\\.cpp"
        --exclude-throw-branches --exclude-unreachable-branches)
    add_custom_target(coverage
        COMMAND ${CMAKE_CTEST_COMMAND} --test-dir ${CMAKE_BINARY_DIR} --output-on-failure
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
        COMMAND ${GCOVR} ${gcovr_args} --json ${CMAKE_BINARY_DIR}/coverage/unfiltered.json
        COMMAND ${CMAKE_COMMAND} -DJSON=${CMAKE_BINARY_DIR}/coverage/unfiltered.json
                -P ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
        COMMAND ${GCOVR} ${gcovr_args} --exclude-branches-by-pattern ${new_pattern} --delete
                --txt --html-details ${CMAKE_BINARY_DIR}/coverage/index.html
                --lcov ${CMAKE_BINARY_DIR}/coverage/coverage.info
                --json ${CMAKE_BINARY_DIR}/coverage/coverage.json
        COMMAND ${GCOVR} -r sources -a ${CMAKE_BINARY_DIR}/coverage/coverage.json
                --txt-metric branch --txt --txt-summary
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        USES_TERMINAL
        VERBATIM
    )
endif()

function(qlumedit_add_test name lib)
    add_executable(${name} ${name}.cpp tst_ldt.h tst_gui.h)
    target_link_libraries(${name} PRIVATE ${lib} Qt6::Test)
    target_compile_definitions(${name} PRIVATE TEMPLATES_DIR="${PROJECT_SOURCE_DIR}/sources/templates")
    add_test(NAME ${name} COMMAND ${name})
    set_tests_properties(${name} PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
    if(TARGET coverage)
        add_dependencies(coverage ${name})
    endif()
endfunction()

# Embeds application.qrc and the .qm files built by qt_add_translations() in the root CMakeLists.txt
# (QM_FILES_OUTPUT_VARIABLE conflicts with RESOURCE_PREFIX, so the paths are derived from the languages).
function(qlumedit_add_app_resources name)
    target_sources(${name} PRIVATE ${PROJECT_SOURCE_DIR}/sources/application.qrc)
    list(TRANSFORM QT_I18N_TRANSLATED_LANGUAGES REPLACE "(.+)" "${PROJECT_BINARY_DIR}/qlumedit_\\1.qm" OUTPUT_VARIABLE qm_files)
    set_source_files_properties(${qm_files} PROPERTIES GENERATED TRUE)
    qt_add_resources(${name} ${name}_translations
        PREFIX /translations
        BASE ${PROJECT_BINARY_DIR}
        FILES ${qm_files}
    )
    add_dependencies(${name} QLumEdit_lrelease)
endfunction()

qlumedit_add_test(tst_eulumdat qlumedit_core)
qlumedit_add_test(tst_eulumdatcalculate qlumedit_core)
qlumedit_add_test(tst_configtabdialog qlumedit_gui)
qlumedit_add_test(tst_eulumdatwidget qlumedit_gui)
qlumedit_add_test(tst_eulumdatwidget2 qlumedit_gui)
qlumedit_add_test(tst_eulumdatwidget3 qlumedit_gui)
qlumedit_add_test(tst_eulumdatwidget4 qlumedit_gui)
qlumedit_add_test(tst_eulumdattablewidget qlumedit_gui)
qlumedit_add_test(tst_ldtroundtrip qlumedit_gui)
qlumedit_add_test(tst_validator qlumedit_gui)

qlumedit_add_test(tst_exports qlumedit_gui)
set_tests_properties(tst_exports PROPERTIES LABELS exports)
qlumedit_add_app_resources(tst_exports)
target_compile_definitions(tst_exports PRIVATE
    EXPORTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/exports"
    EXPORTS_OUTPUT_DIR="${CMAKE_CURRENT_BINARY_DIR}/exports"
)

qlumedit_add_test(tst_screenshots qlumedit_gui)
set_tests_properties(tst_screenshots PROPERTIES LABELS screenshots)
qlumedit_add_app_resources(tst_screenshots)
target_compile_definitions(tst_screenshots PRIVATE
    SCREENSHOTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/screenshots"
    SCREENSHOTS_OUTPUT_DIR="${CMAKE_CURRENT_BINARY_DIR}/screenshots"
)
