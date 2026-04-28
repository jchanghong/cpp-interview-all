## 剑指 Offer 34. 二叉树中和为某一值的路径

### 解题思路：

本题是典型的二叉树方案搜索问题，使用回溯法解决，其包含 **先序遍历 + 路径记录** 两部分。

- **先序遍历：** 按照 “根、左、右” 的顺序，遍历树的所有节点。
- **路径记录：** 在先序遍历中，记录从根节点到当前节点的路径。当路径满足 ① 根节点到叶节点形成的路径 **且** ② 各节点值的和等于目标值 `sum` 时，将此路径加入结果列表。

<img width="500" src="Picture1.png" alt="Picture1.png">

#### 算法流程：

**`pathSum(root, sum)` 函数：**

- **初始化：** 结果列表 `res` ，路径列表 `path` 。
- **返回值：** 返回 `res` 即可。

**`recur(root, tar) 函数：`**

- **递推参数：** 当前节点 `root` ，当前目标值 `tar` 。
- **终止条件：** 若节点 `root` 为空，则直接返回。
- **递推工作：**
  1. 路径更新： 将当前节点值 `root.val` 加入路径 `path` 。
  2. 目标值更新： `tar = tar - root.val`（即目标值 `tar` 从 `sum` 减至 $0$ ）。
  3. 路径记录： 当 ① `root` 为叶节点 **且** ② 路径和等于目标值 ，则将此路径 `path` 加入 `res` 。
  4. 先序遍历： 递归左 / 右子节点。
  5. 路径恢复： 向上回溯前，需要将当前节点从路径 `path` 中删除，即执行 `path.pop()` 。


#### 复杂度分析：

- **时间复杂度 $O(N)$ ：** $N$ 为二叉树的节点数，先序遍历需要遍历所有节点。
- **空间复杂度 $O(N)$ ：** 最差情况下，即树退化为链表时，`path` 存储所有树节点，使用 $O(N)$ 额外空间。

### 代码：

以 Python 语言为例，记录路径时若直接执行 `res.append(path)` ，则是将此 `path` 对象加入了 `res` ；后续 `path` 改变时， `res` 中的 `path` 对象也会随之改变，因此无法实现结果记录。正确做法为：

- Python: `res.append(list(path))` ；
- Java: `res.add(new LinkedList(path))` ；
- C++: `res.push_back(path)` ；

> 三者的原理都是避免直接添加 `path` 对象，而是 **拷贝** 了一个 `path` 对象并加入到 `res` 。

```cpp
class Solution {
public:
    vector<vector<int>> pathSum(TreeNode* root, int sum) {
        recur(root, sum);
        return res;
    }
private:
    vector<vector<int>> res;
    vector<int> path;
    void recur(TreeNode* root, int tar) {
        if(root == nullptr) return;
        path.push_back(root->val);
        tar -= root->val;
        if(tar == 0 && root->left == nullptr && root->right == nullptr)
            res.push_back(path);
        recur(root->left, tar);
        recur(root->right, tar);
        path.pop_back();
    }
};
```
