## 剑指 Offer 56 - II. 数组中数字出现的次数 II

### 解题思路：

如下图所示，考虑数字的二进制形式，对于出现三次的数字，各 **二进制位** 出现的次数都是 $3$ 的倍数。
因此，统计所有数字的各二进制位中 $1$ 的出现次数，并对 $3$ 求余，结果则为只出现一次的数字。

<img width="450" src="Picture1.png" alt="Picture1.png">

### 方法一：有限状态自动机

各二进制位的 **位运算规则相同** ，因此只需考虑一位即可。如下图所示，对于所有数字中的某二进制位 $1$ 的个数，存在 3 种状态，即对 3 余数为 $0, 1, 2$ 。

- 若输入二进制位 $1$ ，则状态按照以下顺序转换；
- 若输入二进制位 $0$ ，则状态不变。

$$
0 \rightarrow 1 \rightarrow 2 \rightarrow 0 \rightarrow \cdots
$$

<img width="400" src="Picture2.png" alt="Picture2.png">

如下图所示，由于二进制只能表示 $0, 1$ ，因此需要使用两个二进制位来表示 $3$ 个状态。设此两位分别为 $two$ , $one$ ，则状态转换变为：

$$
00 \rightarrow 01 \rightarrow 10 \rightarrow 00 \rightarrow \cdots
$$

<img width="400" src="Picture3.png" alt="Picture3.png">

接下来，需要通过 **状态转换表** 导出 **状态转换的计算公式** 。首先回忆一下位运算特点，对于任意二进制位 $x$ ，有：

- 异或运算：`x ^ 0 = x`​ ， `x ^ 1 = ~x`
- 与运算：`x & 0 = 0` ， `x & 1 = x`

**计算 $one$ 方法：**

设当前状态为 $two$ $one$ ，此时输入二进制位 $n$ 。如下图所示，通过对状态表的情况拆分，可推出 $one$ 的计算方法为：

```cpp
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int ones = 0, twos = 0;
        for(int num : nums){
            ones = ones ^ num & ~twos;
            twos = twos ^ num & ~ones;
        }
        return ones;
    }
};
```

### 方法二：遍历统计

> 此方法相对容易理解，但效率较低，总体推荐方法一。

使用 **与运算** ，可获取二进制数字 $num$ 的最右一位 $n_1$ ：

$$
n_1 = num \& i
$$

配合 **右移操作** ，可从低位至高位，获取 $num$ 所有位的值（设 int 类型从低至高的位数为第 0 位 至第 31 位，即 $n_0$ ~ $n_{31}$ ）：

$$
num = num >> 1
$$

建立一个长度为 32 的数组 $counts$ ，通过以上方法可记录所有数字的各二进制位的 $1$ 的出现次数之和。

```cpp
int counts[32] = {0};         // C++ 初始化数组需要写明初始值 0
for(int num : nums) {
    for(int i = 0; i < 32; i++) {
        counts[i] += num & 1; // 更新第 i 位 1 的个数之和
        num >>= 1;            // 第 i 位 --> 第 i + 1 位
    }
}
```

将 $counts$ 各元素对 $3$ 求余，则结果为 “只出现一次的数字” 的各二进制位。

```cpp
for(int i = 31; i >= 0; i--) {
    int x = counts[i] % 3;  // 得到 “只出现一次的数字” 的第 i 位
}
```

利用 **左移操作** 和 **或运算** ，可将 $counts$ 数组中各二进位的值恢复到数字 $res$ 上。

```cpp
for(int i = 31; i >= 0; i--) {
    res <<= 1;
    res |= counts[i] % 3; // 恢复第 i 位
}
```

最终返回 $res$ 即可。

> 由于 Python 的存储负数的特殊性，需要先将 $0$ - $31$ 位取反（即 `res ^ 0xffffffff` ），再将所有位取反（即 `~` ）。
> **此组合操作含义：** 将数字 $31$ 以上位取反， $0$ - $31$ 位不变。

#### 复杂度分析：

- **时间复杂度 $O(N)$ ：** 其中 $N$ 位数组 $nums$ 的长度；遍历数组占用 $O(N)$ ，每轮中的常数个位运算操作占用 $O(1)$ 。
- **空间复杂度 $O(1)$ ：** 数组 $counts$ 长度恒为 $32$ ，占用常数大小的额外空间。

#### 代码：

实际上，只需要修改求余数值 $m$ ，即可实现解决 **除了一个数字以外，其余数字都出现 $m$ 次** 的通用问题。

> 设 int 类型从低至高的位数为第 0 位 至第 31 位。

```cpp
class Solution {
public:
    int singleNumber(vector<int>& nums) {
        int counts[32] = {0};         // C++ 初始化数组需要写明初始值 0
        for(int num : nums) {
            for(int i = 0; i < 32; i++) {
                counts[i] += num & 1; // 更新第 i 位 1 的个数之和
                num >>= 1;            // 第 i 位 --> 第 i 位
            }
        }
        int res = 0, m = 3;
        for(int i = 31; i >= 0; i--) {
            res <<= 1;
            res |= counts[i] % m;     // 恢复第 i 位
        }
        return res;
    }
};
```
