cmake_minimum_required(VERSION 3.16)

project(ThreadPoolExample VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
find_package(Threads REQUIRED)

add_library(thread_pool_lib
    src/thread_pool.cpp
)

target_include_directories(thread_pool_lib
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(thread_pool_lib
    PUBLIC
    Threads::Threads
)

add_executable(thread_pool_demo
    src/main.cpp
)

target_link_libraries(thread_pool_demo PRIVATE thread_pool_lib)

add_executable(thread_pool_tests
    tests/test_thread_pool.cpp
)

target_link_libraries(thread_pool_tests PRIVATE thread_pool_lib)

if(MSVC)
    target_compile_options(thread_pool_lib PRIVATE /W4 /permissive-)
    target_compile_options(thread_pool_demo PRIVATE /W4 /permissive-)
    target_compile_options(thread_pool_tests PRIVATE /W4 /permissive-)
else()
    target_compile_options(thread_pool_lib PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(thread_pool_demo PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(thread_pool_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_test(NAME thread_pool_unit_tests COMMAND thread_pool_tests)
