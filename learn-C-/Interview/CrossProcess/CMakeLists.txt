cmake_minimum_required(VERSION 3.16)

project(CrossProcessExamples VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
find_package(Threads REQUIRED)

if(UNIX AND NOT APPLE)
    find_library(RT_LIBRARY rt)
endif()

add_library(cross_process_lib
    src/ipc_shared_counter.cpp
    src/ipc_shared_queue.cpp
    src/ipc_file_lock.cpp
)

target_include_directories(cross_process_lib
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(cross_process_lib
    PUBLIC
    Threads::Threads
)

if(RT_LIBRARY)
    target_link_libraries(cross_process_lib PUBLIC ${RT_LIBRARY})
endif()

add_executable(shared_counter_demo
    src/shared_counter_demo.cpp
)

target_link_libraries(shared_counter_demo
    PRIVATE
    cross_process_lib
)

add_executable(shared_queue_demo
    src/shared_queue_demo.cpp
)

target_link_libraries(shared_queue_demo
    PRIVATE
    cross_process_lib
)

add_executable(file_lock_demo
    src/file_lock_demo.cpp
)

target_link_libraries(file_lock_demo
    PRIVATE
    cross_process_lib
)

add_executable(cross_process_tests
    tests/test_cross_process.cpp
)

target_link_libraries(cross_process_tests
    PRIVATE
    cross_process_lib
)

if(MSVC)
    target_compile_options(cross_process_lib PRIVATE /W4 /permissive-)
    target_compile_options(shared_counter_demo PRIVATE /W4 /permissive-)
    target_compile_options(shared_queue_demo PRIVATE /W4 /permissive-)
    target_compile_options(file_lock_demo PRIVATE /W4 /permissive-)
    target_compile_options(cross_process_tests PRIVATE /W4 /permissive-)
else()
    target_compile_options(cross_process_lib PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(shared_counter_demo PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(shared_queue_demo PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(file_lock_demo PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(cross_process_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_test(NAME cross_process_unit_tests COMMAND cross_process_tests)
