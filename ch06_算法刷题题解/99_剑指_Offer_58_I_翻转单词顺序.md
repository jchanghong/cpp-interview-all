## 剑指 Offer 58 - I. 翻转单词顺序

### 方法一：双指针

#### 算法解析：

- 倒序遍历字符串 $s$ ，记录单词左右索引边界 $i$ , $j$ ；
- 每确定一个单词的边界，则将其添加至单词列表 $res$ ；
- 最终，将单词列表拼接为字符串，并返回即可。

#### 复杂度分析：

- **时间复杂度 $O(N)$ ：** 其中 $N$ 为字符串 $s$ 的长度，线性遍历字符串。
- **空间复杂度 $O(N)$ ：** 新建的 list(Python) 或 StringBuilder(Java) 中的字符串总长度 $\leq N$ ，占用 $O(N)$ 大小的额外空间。


#### 代码：

```cpp
class Solution {
public:
    string reverseLeftWords(string s, int n) {
        return s.substr(n, s.size()) + s.substr(0, n);
    }
};
```

### 方法二：列表遍历拼接

> 若面试规定不允许使用 **切片函数** ，则使用此方法。

### 算法流程：

1. 新建一个 list (Python) 、StringBuilder (Java) ，记为 `res` ；
2. 先向 `res` 添加 “第 $n + 1$ 位至末位的字符” ；
3. 再向 `res` 添加 “首位至第 $n$ 位的字符” ；
4. 将 `res` 转化为字符串并返回；

#### 复杂度分析：

- **时间复杂度 $O(N)$ ：** 线性遍历 `s` 并添加，使用线性时间。
- **空间复杂度 $O(N)$ ：** 新建的辅助 `res` 使用 $O(N)$ 大小的额外空间。

<img width="550" src="Picture2.png" alt="Picture2.png">

#### 代码：

```cpp
class Solution {
public:
    string reverseLeftWords(string s, int n) {
        reverseString(s, 0, n - 1);
        reverseString(s, n, s.size() - 1);
        reverseString(s, 0, s.size() - 1);
        return s;
    }
private:
    void reverseString(string& s, int i, int j) {
        while(i < j) swap(s[i++], s[j--]);
    }
};
```

也可使用库函数实现，代码如下：

```cpp
class Solution {
public:
    string reverseLeftWords(string s, int n) {
        reverse(s.begin(), s.begin() + n);
        reverse(s.begin() + n, s.end());
        reverse(s.begin(), s.end());
        return s;
    }
};
```
