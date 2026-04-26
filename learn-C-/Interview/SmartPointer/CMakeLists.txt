cmake_minimum_required(VERSION 3.16)

project(SmartPointerExample VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

add_library(smart_pointer_lib
    src/smart_pointer_demo_lib.cpp
)

target_include_directories(smart_pointer_lib
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(smart_pointer_demo
    src/main.cpp
)

target_link_libraries(smart_pointer_demo PRIVATE smart_pointer_lib)

add_executable(smart_pointer_tests
    tests/test_smart_pointer.cpp
)

target_link_libraries(smart_pointer_tests PRIVATE smart_pointer_lib)

if(MSVC)
    target_compile_options(smart_pointer_lib PRIVATE /W4 /permissive-)
    target_compile_options(smart_pointer_demo PRIVATE /W4 /permissive-)
    target_compile_options(smart_pointer_tests PRIVATE /W4 /permissive-)
else()
    target_compile_options(smart_pointer_lib PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(smart_pointer_demo PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(smart_pointer_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_test(NAME smart_pointer_unit_tests COMMAND smart_pointer_tests)
