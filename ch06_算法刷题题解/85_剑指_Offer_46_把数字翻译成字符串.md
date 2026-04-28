## 剑指 Offer 46. 把数字翻译成字符串

### 解题思路：

根据题意，可按照下图的思路，总结出 “递推公式” （即转移方程）。

<img width="600" src="Picture1.png" alt="Picture1.png">

因此，此题可用动态规划解决，以下按照流程解题。

#### 动态规划解析：

> 记数字 $num$ 第 $i$ 位数字为 $x_i$ ，数字 $num$ 的位数为 $n$ ；
> 例如： $num = 12258$ 的 $n = 5$ , $x_1 = 1$ 。

- **状态定义：** 设动态规划列表 $dp$ ，$dp[i]$ 代表以 $x_i$ 为结尾的数字的翻译方案数量。

- **转移方程：** 若 $x_i$ 和 $x_{i-1}$ 组成的两位数字可被整体翻译，则 $dp[i] = dp[i - 1] + dp[i - 2]$ ，否则 $dp[i] = dp[i - 1]$ 。

$$
dp[i] =
\begin{cases}
dp[i - 1] + dp[i - 2] & {, (10 x_{i-1} + x_i) \in [10,25]} \\
dp[i - 1] & {, (10 x_{i-1} + x_i) \in [0, 10) \cup (25, 99]}
\end{cases}
$$

> **可被整体翻译的两位数区间分析：** 当 $x_{i-1} = 0$ 时，组成的两位数无法被整体翻译（例如 $00, 01, 02, \cdots$ ），大于 $25$ 的两位数也无法被整体翻译（例如 $26, 27, \cdots$ ），因此区间为 $[10, 25]$ 。

- **初始状态：** $dp[0] = dp[1] = 1$ ，即 “无数字” 和 “第 $1$ 位数字” 的翻译方法数量均为 $1$ ；

- **返回值：** $dp[n]$ ，即此数字的翻译方案数量；

> **Q：** 无数字情况 $dp[0] = 1$ 从何而来？
> **A：** 当 $num$ 第 $1, 2$ 位的组成的数字 $\in [10,25]$ 时，显然应有 $2$ 种翻译方法，即 $dp[2] = dp[1] + dp[0] = 2$ ，而显然 $dp[1] = 1$ ，因此推出 $dp[0] = 1$ 。

### 方法一：字符串遍历

- 为方便获取数字的各位 $x_i$ ，考虑先将数字 $num$ 转化为字符串 $s$ ，通过遍历 $s$ 实现动态规划。
- 通过字符串切片 $s[i - 2:i]$ 获取数字组合 $10 x_{i-1} + x_i$ ，通过对比字符串 ASCII 码判断字符串对应的数字区间。
- **空间使用优化：** 由于 $dp[i]$ 只与 $dp[i - 1]$ 有关，因此可使用两个变量 $a, b$ 分别记录 $dp[i]$ , $dp[i - 1]$  ，两变量交替前进即可。此方法可省去 $dp$ 列表使用的 $O(N)$ 的额外空间。

#### 复杂度分析：

- **时间复杂度 $O(N)$ ：** $N$ 为字符串 $s$ 的长度（即数字 $num$ 的位数 $\log(num)$ ），其决定了循环次数。
- **空间复杂度 $O(N)$ ：** 字符串 $s$ 使用 $O(N)$ 大小的额外空间。


#### 代码：


```cpp
class Solution {
public:
    int translateNum(int num) {
        string s = to_string(num);
        int a = 1, b = 1, len = s.size();
        for(int i = 2; i <= len; i++) {
            string tmp = s.substr(i - 2, 2);
            int c = tmp.compare("10") >= 0 && tmp.compare("25") <= 0 ? a + b : a;
            b = a;
            a = c;
        }
        return a;
    }
};
```

```cpp
class Solution {
public:
    int translateNum(int num) {
        string s = to_string(num);
        int a = 1, b = 1, len = s.size();
        for(int i = len - 2; i > -1; i--) {
            string tmp = s.substr(i, 2);
            int c = tmp.compare("10") >= 0 && tmp.compare("25") <= 0 ? a + b : a;
            b = a;
            a = c;
        }
        return a;
    }
};
```

```cpp
class Solution {
public:
    int translateNum(int num) {
        int a = 1, b = 1, x, y = num % 10;
        while(num > 9) {
            num /= 10;
            x = num % 10;
            int tmp = 10 * x + y;
            int c = (tmp >= 10 && tmp <= 25) ? a + b : a;
            b = a;
            a = c;
            y = x;
        }
        return a;
    }
};
```

```cpp
class Solution {
public:
    int maxValue(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < n; j++) {
                if(i == 0 && j == 0) continue;
                if(i == 0) grid[i][j] += grid[i][j - 1] ;
                else if(j == 0) grid[i][j] += grid[i - 1][j];
                else grid[i][j] += max(grid[i][j - 1], grid[i - 1][j]);
            }
        }
        return grid[m - 1][n - 1];
    }
};
```

以上代码逻辑清晰，和转移方程直接对应，但仍可提升效率，这是因为：当 $grid$ 矩阵很大时， $i = 0$ 或 $j = 0$ 的情况仅占极少数，相当循环每轮都冗余了一次判断。因此，可先初始化矩阵第一行和第一列，再开始遍历递推。

```cpp
class Solution {
public:
    int maxValue(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        for(int j = 1; j < n; j++) // 初始化第一行
            grid[0][j] += grid[0][j - 1];
        for(int i = 1; i < m; i++) // 初始化第一列
            grid[i][0] += grid[i - 1][0];
        for(int i = 1; i < m; i++)
            for(int j = 1; j < n; j++)
                grid[i][j] += max(grid[i][j - 1], grid[i - 1][j]);
        return grid[m - 1][n - 1];
    }
};
```
