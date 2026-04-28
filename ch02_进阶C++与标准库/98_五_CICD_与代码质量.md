## 五、CI/CD 与代码质量

### 5.1 推荐的 CI 检查流程

```yaml
# .github/workflows/ci.yml 示例
steps:
  - uses: actions/checkout@v4
  - name: Build
    run: |
      cmake -B build -DCMAKE_BUILD_TYPE=Debug
      cmake --build build -j$(nproc)
  - name: Test
    run: ctest --test-dir build --output-on-failure
  - name: ASan
    run: |
      cmake -B build-asan -DCMAKE_CXX_FLAGS="-fsanitize=address"
      cmake --build build-asan
      ctest --test-dir build-asan
  - name: clang-tidy
    run: run-clang-tidy -p build
```

### 5.2 编译警告级别

```cmake
# 开启所有警告，将警告视为错误
target_compile_options(myapp PRIVATE
    -Wall -Wextra -Wpedantic -Werror
    -Wconversion -Wsign-conversion
    -Wold-style-cast
)
```

---
