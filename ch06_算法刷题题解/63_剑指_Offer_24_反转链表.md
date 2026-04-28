## 剑指 Offer 24. 反转链表

### 解题思路：

如下图所示，题目要求将链表反转。本文介绍迭代（双指针）、递归两种实现方法。

<img width="400" src="Picture1.png" alt="Picture1.png">

### 方法一：迭代（双指针）

考虑遍历链表，并在访问各节点时修改 `next` 引用指向，算法流程见注释。

#### 复杂度分析：

- **时间复杂度 $O(N)$ ：** 遍历链表使用线性大小时间。
- **空间复杂度 $O(1)$ ：** 变量 `pre` 和 `cur` 使用常数大小额外空间。


#### 代码：

```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode *cur = head, *pre = nullptr;
        while(cur != nullptr) {
            ListNode* tmp = cur->next; // 暂存后继节点 cur.next
            cur->next = pre;           // 修改 next 引用指向
            pre = cur;                 // pre 暂存 cur
            cur = tmp;                 // cur 访问下一节点
        }
        return pre;
    }
};
```


```cpp
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        return recur(head, nullptr);           // 调用递归并返回
    }
private:
    ListNode* recur(ListNode* cur, ListNode* pre) {
        if (cur == nullptr) return pre;        // 终止条件
        ListNode* res = recur(cur->next, cur); // 递归后继节点
        cur->next = pre;                       // 修改节点引用指向
        return res;                            // 返回反转链表的头节点
    }
};
```
