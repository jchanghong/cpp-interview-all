cmake_minimum_required(VERSION 3.16)

project(CASExample VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
find_package(Threads REQUIRED)

add_library(cas_lib
    src/cas_primitives.cpp
)

target_include_directories(cas_lib
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(cas_lib
    PUBLIC
    Threads::Threads
)

add_executable(cas_demo
    src/cas_demo.cpp
)

target_link_libraries(cas_demo PRIVATE cas_lib)

add_executable(cas_tests
    tests/test_cas.cpp
)

target_link_libraries(cas_tests PRIVATE cas_lib)

if(MSVC)
    target_compile_options(cas_lib PRIVATE /W4 /permissive-)
    target_compile_options(cas_demo PRIVATE /W4 /permissive-)
    target_compile_options(cas_tests PRIVATE /W4 /permissive-)
else()
    target_compile_options(cas_lib PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(cas_demo PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(cas_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_test(NAME cas_unit_tests COMMAND cas_tests)
