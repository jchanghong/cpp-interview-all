set(COVERAGE_DIR "${BINARY_DIR}/coverage")

file(MAKE_DIRECTORY "${COVERAGE_DIR}")
file(REMOVE_RECURSE "${COVERAGE_DIR}")
file(MAKE_DIRECTORY "${COVERAGE_DIR}")

file(GLOB_RECURSE GCDA_FILES "${BINARY_DIR}/*.gcda")
if(GCDA_FILES)
    file(REMOVE ${GCDA_FILES})
endif()

execute_process(
    COMMAND "${BINARY_DIR}/memory_pool_tests"
    WORKING_DIRECTORY "${BINARY_DIR}"
    RESULT_VARIABLE TEST_RESULT
    OUTPUT_VARIABLE TEST_STDOUT
    ERROR_VARIABLE TEST_STDERR
)

file(WRITE "${COVERAGE_DIR}/test_output.txt" "${TEST_STDOUT}")
file(APPEND "${COVERAGE_DIR}/test_output.txt" "${TEST_STDERR}")

if(NOT TEST_RESULT EQUAL 0)
    message(FATAL_ERROR "memory_pool_tests failed while generating coverage. See coverage/test_output.txt")
endif()

set(COVERAGE_OUTPUT "")

function(run_gcov GCNO_FILE LABEL)
    execute_process(
        COMMAND gcov -b -c "${GCNO_FILE}"
        WORKING_DIRECTORY "${COVERAGE_DIR}"
        RESULT_VARIABLE GCOV_RESULT
        OUTPUT_VARIABLE GCOV_STDOUT
        ERROR_VARIABLE GCOV_STDERR
    )

    string(APPEND COVERAGE_OUTPUT "===== ${LABEL} =====\n")
    string(APPEND COVERAGE_OUTPUT "${GCOV_STDOUT}")
    string(APPEND COVERAGE_OUTPUT "${GCOV_STDERR}\n")
    set(COVERAGE_OUTPUT "${COVERAGE_OUTPUT}" PARENT_SCOPE)

    if(NOT GCOV_RESULT EQUAL 0)
        message(FATAL_ERROR "gcov failed for ${GCNO_FILE}")
    endif()
endfunction()

run_gcov("${BINARY_DIR}/CMakeFiles/memory_pool_tests.dir/src/fixed_memory_pool.cpp.gcno"
         "fixed_memory_pool.cpp")
run_gcov("${BINARY_DIR}/CMakeFiles/memory_pool_tests.dir/src/memory_pool_extensions.cpp.gcno"
         "memory_pool_extensions.cpp")
run_gcov("${BINARY_DIR}/CMakeFiles/memory_pool_tests.dir/tests/test_memory_pool.cpp.gcno"
         "test_memory_pool.cpp")

file(WRITE "${COVERAGE_DIR}/coverage_summary.txt" "${COVERAGE_OUTPUT}")

message(STATUS "Coverage report generated in ${COVERAGE_DIR}")
