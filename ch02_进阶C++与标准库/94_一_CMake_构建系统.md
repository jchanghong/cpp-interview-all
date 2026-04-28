## 一、CMake 构建系统

### 1.1 为什么用 CMake

CMake 是跨平台的构建系统生成器，通过 `CMakeLists.txt` 描述项目结构，生成 Makefile、Ninja、Visual Studio 等构建文件。

### 1.2 最小 CMake 项目

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyProject LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(myapp src/main.cpp src/utils.cpp)
target_include_directories(myapp PRIVATE include)
target_link_libraries(myapp PRIVATE pthread)
```

### 1.3 现代 CMake 最佳实践

```cmake
# 使用 target-based 命令（而非全局命令）
target_compile_options(myapp PRIVATE -Wall -Wextra -Wpedantic)
target_compile_features(myapp PUBLIC cxx_std_17)

# 添加子目录
add_subdirectory(src)
add_subdirectory(tests)

# 查找外部库
find_package(Boost REQUIRED COMPONENTS filesystem system)
target_link_libraries(myapp PRIVATE Boost::filesystem Boost::system)

# 安装规则
install(TARGETS myapp DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
```

### 1.4 常用 CMake 命令

| 命令 | 作用 |
|------|------|
| `add_executable` | 添加可执行目标 |
| `add_library` | 添加库目标（STATIC/SHARED/INTERFACE） |
| `target_link_libraries` | 链接库（PUBLIC/PRIVATE/INTERFACE） |
| `target_include_directories` | 添加头文件搜索路径 |
| `find_package` | 查找外部依赖 |
| `option` | 定义编译选项 |
| `add_subdirectory` | 添加子目录 |
| `install` | 定义安装规则 |

### 1.5 构建与测试

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
ctest --output-on-failure
```
