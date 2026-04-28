## 剑指 Offer 32 - III. 从上到下打印二叉树 III

### 解题思路：

剑指 Offer 32 - I. 从上到下打印二叉树 主要考察 **树的按层打印** ；
剑指 Offer 32 - II. 从上到下打印二叉树 II 额外要求 **每一层打印到一行** ；
本题额外要求 **打印顺序交替变化**（建议按顺序做此三道题）。

<img width="400" src="Picture1.png" alt="Picture1.png">

### 方法一：层序遍历 + 双端队列

- 利用双端队列的两端皆可添加元素的特性，设打印列表（双端队列） `tmp` ，并规定：
  - 奇数层 则添加至 `tmp` **尾部** ，
  - 偶数层 则添加至 `tmp` **头部** 。

#### 算法流程：

1. **特例处理：** 当树的根节点为空，则直接返回空列表 `[]` ；
2. **初始化：** 打印结果空列表 `res` ，包含根节点的双端队列 `deque` ；
3. **BFS 循环：** 当 `deque` 为空时跳出；
   1. 新建列表 `tmp` ，用于临时存储当前层打印结果；
   2. **当前层打印循环：** 循环次数为当前层节点数（即 `deque` 长度）；
      1. **出队：** 队首元素出队，记为 `node`；
      2. **打印：** 若为奇数层，将 `node.val` 添加至 `tmp` 尾部；否则，添加至 `tmp` 头部；
      3. **添加子节点：** 若 `node` 的左（右）子节点不为空，则加入 `deque` ；
   3. 将当前层结果 `tmp` 转化为 list 并添加入 `res` ；
4. **返回值：** 返回打印结果列表 `res` 即可；


#### 复杂度分析：

- **时间复杂度 $O(N)$ ：** $N$ 为二叉树的节点数量，即 BFS 需循环 $N$ 次，占用 $O(N)$ ；双端队列的队首和队尾的添加和删除操作的时间复杂度均为 $O(1)$ 。
- **空间复杂度 $O(N)$ ：** 最差情况下，即当树为满二叉树时，最多有 $N/2$ 个树节点 **同时** 在 `deque` 中，使用 $O(N)$ 大小的额外空间。

#### 代码：

> Python 中使用 collections 中的双端队列 `deque()` ，其 `popleft()` 方法可达到 $O(1)$ 时间复杂度；列表 list 的 `pop(0)` 方法时间复杂度为 $O(N)$ 。
> Java 中将链表 LinkedList 作为双端队列使用。

```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        deque<TreeNode*> deque;
        vector<vector<int>> res;
        if(root != NULL) deque.push_back(root);
        while(!deque.empty()) {
            // 打印奇数层
            vector<int> tmp;
            for(int i = deque.size(); i > 0; i--) {
                // 从左向右打印
                TreeNode* node = deque.front();
                deque.pop_front();
                tmp.push_back(node->val);
                // 先左后右加入下层节点
                if(node->left != NULL) deque.push_back(node->left);
                if(node->right != NULL) deque.push_back(node->right);
            }
            res.push_back(tmp);
            if(deque.empty()) break; // 若为空则提前跳出
            // 打印偶数层
            tmp.clear();
            for(int i = deque.size(); i > 0; i--) {
                // 从右向左打印
                TreeNode* node = deque.back();
                deque.pop_back();
                tmp.push_back(node->val);
                // 先右后左加入下层节点
                if(node->right != NULL) deque.push_front(node->right);
                if(node->left != NULL) deque.push_front(node->left);
            }
            res.push_back(tmp);
        }
        return res;
    }
};
```

### 方法三：层序遍历 + 倒序

- 此方法的优点是只用列表即可，无需其他数据结构。
- **偶数层倒序：** 若 `res` 的长度为 **奇数** ，说明当前是偶数层，则对 `tmp` 执行 **倒序** 操作。

#### 复杂度分析：

- **时间复杂度 $O(N)$ ：** $N$ 为二叉树的节点数量，即 BFS 需循环 $N$ 次，占用 $O(N)$ 。**共完成** 少于 $N$ 个节点的倒序操作，占用 $O(N)$ 。
- **空间复杂度 $O(N)$ ：** 最差情况下，即当树为满二叉树时，最多有 $N/2$ 个树节点**同时**在 `queue` 中，使用 $O(N)$ 大小的额外空间。


#### 代码：

```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        queue<TreeNode*> que;
        vector<vector<int>> res;
        if(root != NULL) que.push(root);
        while(!que.empty()) {
            vector<int> tmp;
            for(int i = que.size(); i > 0; i--) {
                TreeNode* node = que.front();
                que.pop();
                tmp.push_back(node->val);
                if(node->left != NULL) que.push(node->left);
                if(node->right != NULL) que.push(node->right);
            }
            if(res.size() % 2 == 1) reverse(tmp.begin(),tmp.end());
            res.push_back(tmp);
        }
        return res;
    }
};
```
