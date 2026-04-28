## 剑指 Offer 10- I. 斐波那契数列

### 解题思路：

斐波那契数列的定义是 $f(n + 1) = f(n) + f(n - 1)$ ，生成第 $n$ 项的做法有以下几种：

1. **递归法：**
    - **原理：** 把 $f(n)$ 问题的计算拆分成 $f(n-1)$ 和 $f(n-2)$ 两个子问题的计算，并递归，以 $f(0)$ 和 $f(1)$ 为终止条件。
    - **缺点：** 大量重复的递归计算，例如 $f(n)$ 和 $f(n - 1)$ 两者向下递归需要 **各自计算** $f(n - 2)$ 的值。
2. **记忆化递归法：**
    - **原理：** 在递归法的基础上，新建一个长度为 $n$ 的数组，用于在递归时存储 $f(0)$ 至 $f(n)$ 的数字值，重复遇到某数字则直接从数组取用，避免了重复的递归计算。
    - **缺点：** 记忆化存储需要使用 $O(N)$ 的额外空间。
3. **动态规划：**
    - **原理：** 以斐波那契数列性质 $f(n + 1) = f(n) + f(n - 1)$ 为转移方程。
    - 从计算效率、空间复杂度上看，动态规划是本题的最佳解法。

> 下图帮助理解递归法的 “重复计算” 概念。

<img width="500" src="Picture1.png" alt="Picture1.png">

### 动态规划解析：

- **状态定义：** 设 $dp$ 为一维数组，其中 $dp[i]$ 的值代表 斐波那契数列第 $i$ 个数字 。
- **转移方程：** $dp[i + 1] = dp[i] + dp[i - 1]$ ，即对应数列定义 $f(n + 1) = f(n) + f(n - 1)$ ；
- **初始状态：** $dp[0] = 0$, $dp[1] = 1$ ，即初始化前两个数字；
- **返回值：** $dp[n]$ ，即斐波那契数列的第 $n$ 个数字。

### 空间复杂度降低：

> 若新建长度为 $n$ 的 $dp$ 列表，则空间复杂度为 $O(N)$ 。

- 由于 $dp$ 列表第 $i$ 项只与第 $i-1$ 和第 $i-2$ 项有关，因此只需要初始化三个整形变量 `sum`, `a`, `b` ，利用辅助变量 $sum$ 使 $a, b$ 两数字交替前进即可 *（具体实现见代码）* 。
- 节省了 $dp$ 列表空间，因此空间复杂度降至 $O(1)$ 。

### 循环求余法：

> **大数越界：** 随着 $n$ 增大, $f(n)$ 会超过 `Int32` 甚至 `Int64` 的取值范围，导致最终的返回值错误。

- **求余运算规则：** 设正整数 $x, y, p$ ，求余符号为 $\odot$ ，则有 $(x + y) \odot p = (x \odot p + y \odot p) \odot p$ 。

- **解析：** 根据以上规则，可推出 $f(n) \odot p = [f(n-1) \odot p + f(n-2) \odot p] \odot p$ ，从而可以在循环过程中每次计算 $sum = (a + b) \odot 1000000007$ ，此操作与最终返回前取余等价。


### 复杂度分析：

- **时间复杂度 $O(n)$ ：** 计算 $f(n)$ 需循环 $n$ 次，每轮循环内计算操作使用 $O(1)$ 。
- **空间复杂度 $O(1)$ ：** 几个标志变量使用常数大小的额外空间。

### 代码：

```cpp
class Solution {
public:
    int fib(int n) {
        int a = 0, b = 1, sum;
        for(int i = 0; i < n; i++){
            sum = (a + b) % 1000000007;
            a = b;
            b = sum;
        }
        return a;
    }
};
```

由于 Python 中整形数字的大小限制取决计算机的内存（可理解为无限大），因此也可不考虑大数越界问题；但当数字很大时，加法运算的效率也会降低，因此不推荐此方法。

```cpp
class Solution {
public:
    int numWays(int n) {
        int a = 1, b = 1, sum;
        for(int i = 0; i < n; i++){
            sum = (a + b) % 1000000007;
            a = b;
            b = sum;
        }
        return a;
    }
};
```

由于 Python 中整形数字的大小限制取决计算机的内存（可理解为无限大），因此也可不考虑大数越界问题；但当数字很大时，加法运算的效率也会降低，因此不推荐此方法。

```cpp
class Solution {
public:
    int minArray(vector<int>& numbers) {
        int i = 0, j = numbers.size() - 1;
        while (i < j) {
            int m = (i + j) / 2;
            if (numbers[m] > numbers[j]) i = m + 1;
            else if (numbers[m] < numbers[j]) j = m;
            else j--;
        }
        return numbers[i];
    }
};
```

实际上，当出现 $nums[m] = nums[j]$ 时，一定有区间 $[i, m]$ 内所有元素相等 或 区间 $[m, j]$ 内所有元素相等（或两者皆满足）。对于寻找此类数组的最小值问题，可直接放弃二分查找，而使用线性查找替代。

```cpp
class Solution {
public:
    int minArray(vector<int>& numbers) {
        int i = 0, j = numbers.size() - 1;
        while (i < j) {
            int m = (i + j) / 2;
            if (numbers[m] > numbers[j]) i = m + 1;
            else if (numbers[m] < numbers[j]) j = m;
            else {
                int x = i;
                for(int k = i + 1; k < j; k++) {
                    if(numbers[k] < numbers[x]) x = k;
                }
                return numbers[x];
            }
        }
        return numbers[i];
    }
};
```
