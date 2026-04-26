cmake_minimum_required(VERSION 3.16)

project(NetworkExample VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
find_package(Threads REQUIRED)

add_library(network_lib
    src/network_echo.cpp
)

target_include_directories(network_lib
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(network_lib
    PUBLIC
    Threads::Threads
)

add_executable(network_demo
    src/main.cpp
)

target_link_libraries(network_demo PRIVATE network_lib)

add_executable(network_tests
    tests/test_network.cpp
)

target_link_libraries(network_tests PRIVATE network_lib)

if(MSVC)
    target_compile_options(network_lib PRIVATE /W4 /permissive-)
    target_compile_options(network_demo PRIVATE /W4 /permissive-)
    target_compile_options(network_tests PRIVATE /W4 /permissive-)
else()
    target_compile_options(network_lib PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(network_demo PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(network_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_test(NAME network_unit_tests COMMAND network_tests)
