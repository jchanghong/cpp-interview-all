cmake_minimum_required(VERSION 3.16)

project(RPCExample VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

add_library(rpc_lib
    src/rpc_framework.cpp
)

target_include_directories(rpc_lib
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(rpc_demo
    src/main.cpp
)

target_link_libraries(rpc_demo PRIVATE rpc_lib)

add_executable(rpc_tests
    tests/test_rpc.cpp
)

target_link_libraries(rpc_tests PRIVATE rpc_lib)

if(MSVC)
    target_compile_options(rpc_lib PRIVATE /W4 /permissive-)
    target_compile_options(rpc_demo PRIVATE /W4 /permissive-)
    target_compile_options(rpc_tests PRIVATE /W4 /permissive-)
else()
    target_compile_options(rpc_lib PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(rpc_demo PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(rpc_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_test(NAME rpc_unit_tests COMMAND rpc_tests)
