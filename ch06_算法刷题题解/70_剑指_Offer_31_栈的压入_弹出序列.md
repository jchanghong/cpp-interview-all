## 剑指 Offer 31. 栈的压入、弹出序列

### 解题思路：

如下图所示，给定一个压入序列 $pushed$ 和弹出序列 $popped$ ，则压入 / 弹出操作的顺序（即排列）是 **唯一确定** 的。

<img width="500" src="Picture1.png" alt="Picture1.png">

如下图所示，栈的数据操作具有 **先入后出** 的特性，因此某些弹出序列是无法实现的。

<img width="500" src="Picture2.png" alt="Picture2.png">

考虑借用一个辅助栈 $stack$ ，**模拟** 压入 / 弹出操作的排列。根据是否模拟成功，即可得到结果。

- **入栈操作：** 按照压栈序列的顺序执行。
- **出栈操作：** 每次入栈后，循环判断 “栈顶元素 $=$ 弹出序列的当前元素” 是否成立，将符合弹出序列顺序的栈顶元素全部弹出。

> 由于题目规定 `栈的所有数字均不相等` ，因此在循环入栈中，每个元素出栈的位置的可能性是唯一的（若有重复数字，则具有多个可出栈的位置）。因而，在遇到 “栈顶元素 $=$ 弹出序列的当前元素” 就应立即执行出栈。

#### 算法流程：

1. **初始化：** 辅助栈 $stack$ ，弹出序列的索引 $i$ ；
2. **遍历压栈序列：** 各元素记为 $num$ ；
   1. 元素 $num$ 入栈；
   2. 循环出栈：若 $stack$ 的栈顶元素 $=$ 弹出序列元素 $popped[i]$ ，则执行出栈与 $i++$ ；
3. **返回值：** 若 $stack$ 为空，则此弹出序列合法。

#### 复杂度分析：

- **时间复杂度 $O(N)$ ：** 其中 $N$ 为列表 $pushed$ 的长度；每个元素最多入栈与出栈一次，即最多共 $2N$ 次出入栈操作。
- **空间复杂度 $O(N)$ ：** 辅助栈 $stack$ 最多同时存储 $N$ 个元素。


### 代码：

题目指出 `pushed 是 popped 的排列` 。因此，无需考虑 $pushed$ 和 $popped$ **长度不同** 或 **包含元素不同** 的情况。

```cpp
class Solution {
public:
    bool validateStackSequences(vector<int>& pushed, vector<int>& popped) {
        stack<int> stk;
        int i = 0;
        for(int num : pushed) {
            stk.push(num); // num 入栈
            while(!stk.empty() && stk.top() == popped[i]) { // 循环判断与出栈
                stk.pop();
                i++;
            }
        }
        return stk.empty();
    }
};
```
