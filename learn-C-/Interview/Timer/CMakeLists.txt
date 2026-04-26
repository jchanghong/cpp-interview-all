cmake_minimum_required(VERSION 3.16)

project(TimerExample VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
find_package(Threads REQUIRED)

add_library(timer_lib
    src/timer_scheduler.cpp
)

target_include_directories(timer_lib
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(timer_lib
    PUBLIC
    Threads::Threads
)

add_executable(timer_demo
    src/main.cpp
)

target_link_libraries(timer_demo PRIVATE timer_lib)

add_executable(timer_tests
    tests/test_timer.cpp
)

target_link_libraries(timer_tests PRIVATE timer_lib)

if(MSVC)
    target_compile_options(timer_lib PRIVATE /W4 /permissive-)
    target_compile_options(timer_demo PRIVATE /W4 /permissive-)
    target_compile_options(timer_tests PRIVATE /W4 /permissive-)
else()
    target_compile_options(timer_lib PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(timer_demo PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(timer_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_test(NAME timer_unit_tests COMMAND timer_tests)
