# problems__emplace_back和push_back有什么区别？.md

### emplace_back 和 push_back 有什么区别？

| 特性 | `push_back` | `emplace_back` |
|------|------------|---------------|
| 参数 | 接受已有对象（触发拷贝/移动） | 接受构造函数参数（原地构造） |
| 临时对象 | 会产生临时对象 | 不产生临时对象 |
| 性能 | 多一次移动/拷贝 | 直接构造，更高效 |
| C++版本 | C++98 | C++11 |

```cpp
struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {
        std::cout << "Point(" << x << "," << y << ")\n";
    }
    Point(const Point& other) : x(other.x), y(other.y) {
        std::cout << "copy\n";
    }
};

std::vector<Point> v;

// push_back：先构造临时对象，再拷贝/移动到容器
v.push_back(Point(1, 2));  // 输出: Point(1,2) + copy

// emplace_back：直接在容器内存中构造，无临时对象
v.emplace_back(1, 2);      // 输出: Point(1,2)
```

**面试标准答案：**

> `emplace_back` 直接在容器内存中原地构造对象，接受构造参数而非已构造对象，避免了临时对象的创建和额外的拷贝/移动。`push_back` 需要传入已构造的对象或临时对象。
>
> **建议**：构造新元素时优先用 `emplace_back`；传递已有对象时用 `push_back`。注意：`emplace_back` 不会阻止隐式类型转换，可能导致意外行为（如 `v.emplace_back(42)` 对 `vector<string>` 是合法的但可能不是你想要的结果）。

---
