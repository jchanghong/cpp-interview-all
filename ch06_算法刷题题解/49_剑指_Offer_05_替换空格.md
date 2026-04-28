## 剑指 Offer 05. 替换空格

### 方法一：遍历添加

在 Python 和 Java 等语言中，字符串都被设计成「不可变」的类型，即无法直接修改字符串的某一位字符，需要新建一个字符串实现。

#### 算法流程：

1. 初始化一个 list (Python) / StringBuilder (Java) ，记为 `res` ；
2. 遍历列表 `s` 中的每个字符 `c` ：
    - 当 `c` 为空格时：向 `res` 后添加字符串 "%20" ；
    - 当 `c` 不为空格时：向 `res` 后添加字符 `c` ；
3. 将列表 `res` 转化为字符串并返回。

#### 复杂度分析：

- **时间复杂度 $O(N)$ ：** 遍历使用 $O(N)$ ，每轮添加（修改）字符操作使用 $O(1)$ ；
- **空间复杂度 $O(N)$ ：** Python 新建的 list 和 Java 新建的 StringBuilder 都使用了线性大小的额外空间。


### 代码：

```cpp
class Solution {
public:
    string replaceSpace(string s) {
        int count = 0, len = s.size();
        // 统计空格数量
        for (char c : s) {
            if (c == ' ') count++;
        }
        // 修改 s 长度
        s.resize(len + 2 * count);
        // 倒序遍历修改
        for(int i = len - 1, j = s.size() - 1; i < j; i--, j--) {
            if (s[i] != ' ')
                s[j] = s[i];
            else {
                s[j - 2] = '%';
                s[j - 1] = '2';
                s[j] = '0';
                j -= 2;
            }
        }
        return s;
    }
};
```
