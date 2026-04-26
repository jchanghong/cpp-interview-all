cmake_minimum_required(VERSION 3.16)

project(MemoryPoolExample VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
option(ENABLE_COVERAGE "Enable GCC/Clang coverage instrumentation." OFF)

include(CTest)
find_package(Threads REQUIRED)

add_executable(memory_pool_demo
    src/fixed_memory_pool.cpp
    src/memory_pool_extensions.cpp
    src/main.cpp
)

add_executable(memory_pool_benchmark
    src/fixed_memory_pool.cpp
    src/memory_pool_extensions.cpp
    src/benchmark.cpp
)

add_executable(memory_pool_tests
    src/fixed_memory_pool.cpp
    src/memory_pool_extensions.cpp
    tests/test_memory_pool.cpp
)

target_include_directories(memory_pool_demo
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(memory_pool_benchmark
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(memory_pool_tests
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(memory_pool_demo PRIVATE Threads::Threads)
target_link_libraries(memory_pool_benchmark PRIVATE Threads::Threads)
target_link_libraries(memory_pool_tests PRIVATE Threads::Threads)

if(MSVC)
    target_compile_options(memory_pool_demo PRIVATE /W4 /permissive-)
    target_compile_options(memory_pool_benchmark PRIVATE /W4 /permissive-)
    target_compile_options(memory_pool_tests PRIVATE /W4 /permissive-)
else()
    target_compile_options(memory_pool_demo PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(memory_pool_benchmark PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(memory_pool_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_test(NAME memory_pool_unit_tests COMMAND memory_pool_tests)

if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(memory_pool_demo PRIVATE -O0 -g --coverage)
        target_compile_options(memory_pool_benchmark PRIVATE -O0 -g --coverage)
        target_compile_options(memory_pool_tests PRIVATE -O0 -g --coverage)
        target_link_options(memory_pool_demo PRIVATE --coverage)
        target_link_options(memory_pool_benchmark PRIVATE --coverage)
        target_link_options(memory_pool_tests PRIVATE --coverage)

        add_custom_target(coverage
            COMMAND ${CMAKE_COMMAND}
                    -DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}
                    -DBINARY_DIR=${CMAKE_BINARY_DIR}
                    -P ${PROJECT_SOURCE_DIR}/cmake/GenerateCoverage.cmake
            DEPENDS memory_pool_tests
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Running unit tests and generating gcov coverage report"
        )
    else()
        message(WARNING "Coverage is only configured for GCC/Clang toolchains.")
    endif()
endif()
