# Interview__CAS__README.md

## CAS

这个工程演示 `CAS`（Compare-And-Swap）在 C++ 面试里的两个常见落点：无锁原子计数器和基于 `atomic_flag` 的自旋锁。

### 构建

```bash
cd /home/wj/Code/C++/Interview/CAS
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

### 目标

- `cas_demo`：并发比较 CAS 计数器和加锁计数器。
- `cas_tests`：验证自旋锁、原子计数器和加锁计数器正确性。

### 面试知识点

- CAS 的核心语义：比较内存中的旧值是否等于预期值，若相等则原子写入新值。
- 常见接口：`compare_exchange_weak` 和 `compare_exchange_strong`。
- `weak` 允许伪失败，适合放在循环里反复重试。
- 自旋锁适合临界区很短、线程切换代价较高的场景。
- CAS 解决的是“单个位置的原子更新”，不能自动解决复杂结构的一致性。
- 高频延伸问题：ABA 问题、内存序、忙等 CPU 消耗、饥饿与公平性。


# Interview__SmartPointer__README.md

## SmartPointer

这个工程实现了一个面试友好的最小版 `UniquePtr` 和 `SharedPtr`，用来解释 RAII、所有权转移和引用计数。

### 构建

```bash
cd /home/wj/Code/C++/Interview/SmartPointer
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

### 目标

- `smart_pointer_demo`：演示独占和共享所有权的基本行为。
- `smart_pointer_tests`：验证移动语义、重置和引用计数。

### 面试知识点

- `unique_ptr` 体现的是独占所有权，不允许拷贝，允许移动。
- `shared_ptr` 通过控制块维护引用计数。
- 生产级 `shared_ptr` 通常需要线程安全的原子引用计数。
- `weak_ptr` 的意义是打破循环引用。
- 智能指针解决的是资源生命周期问题，不等于自动解决并发安全问题。


# STL__README.md

STL 详细文档

## 容器（containers）

### array

array 是固定大小的顺序容器，它们保存了一个以严格的线性顺序排列的特定数量的元素。

方法|含义
---|---
begin|返回指向数组容器中第一个元素的迭代器
end|返回指向数组容器中最后一个元素之后的理论元素的迭代器
rbegin|返回指向数组容器中最后一个元素的反向迭代器
rend|返回一个反向迭代器，指向数组中第一个元素之前的理论元素
cbegin|返回指向数组容器中第一个元素的常量迭代器（const_iterator）
cend|返回指向数组容器中最后一个元素之后的理论元素的常量迭代器（const_iterator）
crbegin|返回指向数组容器中最后一个元素的常量反向迭代器（const_reverse_iterator）
crend|返回指向数组中第一个元素之前的理论元素的常量反向迭代器（const_reverse_iterator）
size|返回数组容器中元素的数量
max_size|返回数组容器可容纳的最大元素数
empty|返回一个布尔值，指示数组容器是否为空
operator[]|返回容器中第 n（参数）个位置的元素的引用
at|返回容器中第 n（参数）个位置的元素的引用
front|返回对容器中第一个元素的引用
back|返回对容器中最后一个元素的引用
data|返回指向容器中第一个元素的指针
fill|用 val（参数）填充数组所有元素
swap|通过 x（参数）的内容交换数组的内容
get（array）|形如 `std::get<0>(myarray)`；传入一个数组容器，返回指定位置元素的引用
relational operators (array)|形如 `arrayA > arrayB`；依此比较数组每个元素的大小关系

### vector

vector 是表示可以改变大小的数组的序列容器。

方法|含义
---|---
vector|构造函数
~vector|析构函数，销毁容器对象
operator=|将新内容分配给容器，替换其当前内容，并相应地修改其大小
begin|返回指向容器中第一个元素的迭代器
end|返回指向容器中最后一个元素之后的理论元素的迭代器
rbegin|返回指向容器中最后一个元素的反向迭代器
rend|返回一个反向迭代器，指向中第一个元素之前的理论元素
cbegin|返回指向容器中第一个元素的常量迭代器（const_iterator）
cend|返回指向容器中最后一个元素之后的理论元素的常量迭代器（const_iterator）
crbegin|返回指向容器中最后一个元素的常量反向迭代器（const_reverse_iterator）
crend|返回指向容器中第一个元素之前的理论元素的常量反向迭代器（const_reverse_iterator）
size|返回容器中元素的数量
max_size|返回容器可容纳的最大元素数
resize|调整容器的大小，使其包含 n（参数）个元素
capacity|返回当前为 vector 分配的存储空间（容量）的大小
empty|返回 vector 是否为空
reserve|请求 vector 容量至少足以包含 n（参数）个元素
shrink_to_fit|要求容器减小其 capacity（容量）以适应其 size（元素数量）
operator[]|返回容器中第 n（参数）个位置的元素的引用
at|返回容器中第 n（参数）个位置的元素的引用
front|返回对容器中第一个元素的引用
back|返回对容器中最后一个元素的引用
data|返回指向容器中第一个元素的指针
assign|将新内容分配给 vector，替换其当前内容，并相应地修改其 size
push_back|在容器的最后一个元素之后添加一个新元素
pop_back|删除容器中的最后一个元素，有效地将容器 size 减少一个
insert|通过在指定位置的元素之前插入新元素来扩展该容器，通过插入元素的数量有效地增加容器大小
erase|从 vector 中删除单个元素（`position`）或一系列元素（`[first，last)`），这有效地减少了被去除的元素的数量，从而破坏了容器的大小
swap|通过 x（参数）的内容交换容器的内容，x 是另一个类型相同、size 可能不同的 vector 对象
clear|从 vector 中删除所有的元素（被销毁），留下 size 为 0 的容器
emplace|通过在 position（参数）位置处插入新元素 args（参数）来扩展容器
emplace_back|在 vector 的末尾插入一个新的元素，紧跟在当前的最后一个元素之后
get_allocator|返回与vector关联的构造器对象的副本
swap(vector)|容器 x（参数）的内容与容器 y（参数）的内容交换。两个容器对象都必须是相同的类型（相同的模板参数），尽管大小可能不同
relational operators (vector)|形如 `vectorA > vectorB`；依此比较每个元素的大小关系

### deque

deque（['dek]）（双端队列）是double-ended queue 的一个不规则缩写。deque是具有动态大小的序列容器，可以在两端（前端或后端）扩展或收缩。

方法|含义
---|---
deque|构造函数
push_back|在当前的最后一个元素之后 ，在 deque 容器的末尾添加一个新元素
push_front|在 deque 容器的开始位置插入一个新的元素，位于当前的第一个元素之前
pop_back|删除 deque 容器中的最后一个元素，有效地将容器大小减少一个
pop_front|删除 deque 容器中的第一个元素，有效地减小其大小
emplace_front|在 deque 的开头插入一个新的元素，就在其当前的第一个元素之前
emplace_back|在 deque 的末尾插入一个新的元素，紧跟在当前的最后一个元素之后

### forward\_list

forward_list（单向链表）是序列容器，允许在序列中的任何地方进行恒定的时间插入和擦除操作。

方法|含义
---|---
forward\_list|返回指向容器中第一个元素之前的位置的迭代器
cbefore\_begin|返回指向容器中第一个元素之前的位置的 const_iterator

### list

list，双向链表，是序列容器，允许在序列中的任何地方进行常数时间插入和擦除操作，并在两个方向上进行迭代。

### stack

stack 是一种容器适配器，用于在LIFO（后进先出）的操作，其中元素仅从容器的一端插入和提取。

### queue

queue 是一种容器适配器，用于在FIFO（先入先出）的操作，其中元素插入到容器的一端并从另一端提取。

### priority_queue

### set

set 是按照特定顺序存储唯一元素的容器。

### multiset

### map

map 是关联容器，按照特定顺序存储由 key value (键值) 和 mapped value (映射值) 组合形成的元素。

方法|含义
---|---
map|构造函数
begin|返回引用容器中第一个元素的迭代器
key_comp|返回容器用于比较键的比较对象的副本
value_comp|返回可用于比较两个元素的比较对象，以获取第一个元素的键是否在第二个元素之前
find|在容器中搜索具有等于 k（参数）的键的元素，如果找到则返回一个迭代器，否则返回 map::end 的迭代器
count|在容器中搜索具有等于 k（参数）的键的元素，并返回匹配的数量
lower_bound|返回一个非递减序列 `[first, last)`（参数）中的第一个大于等于值 val（参数）的位置的迭代器
upper_bound|返回一个非递减序列 `[first, last)`（参数）中第一个大于 val（参数）的位置的迭代器
equal_range|获取相同元素的范围，返回包含容器中所有具有与 k（参数）等价的键的元素的范围边界（`pair< map<char,int>::iterator, map<char,int>::iterator >`）

### multimap

### unordered\_set

### unordered\_multiset

### unordered\_map

### unordered\_multimap

### tuple

元组是一个能够容纳元素集合的对象。每个元素可以是不同的类型。

### pair

这个类把一对值（values）结合在一起，这些值可能是不同的类型（T1 和 T2）。每个值可以被公有的成员变量first、second访问。

## 算法（algorithms）

```cpp
// 简单查找算法，要求输入迭代器（input iterator）
find(beg, end, val); // 返回一个迭代器，指向输入序列中第一个等于 val 的元素，未找到返回 end
find_if(beg, end, unaryPred); // 返回一个迭代器，指向第一个满足 unaryPred 的元素，未找到返回 end
find_if_not(beg, end, unaryPred); // 返回一个迭代器，指向第一个令 unaryPred 为 false 的元素，未找到返回 end
count(beg, end, val); // 返回一个计数器，指出 val 出现了多少次
count_if(beg, end, unaryPred); // 统计有多少个元素满足 unaryPred
all_of(beg, end, unaryPred); // 返回一个 bool 值，判断是否所有元素都满足 unaryPred
any_of(beg, end, unaryPred); // 返回一个 bool 值，判断是否任意（存在）一个元素满足 unaryPred
none_of(beg, end, unaryPred); // 返回一个 bool 值，判断是否所有元素都不满足 unaryPred

// 查找重复值的算法，传入向前迭代器（forward iterator）
adjacent_find(beg, end); // 返回指向第一对相邻重复元素的迭代器，无相邻元素则返回 end
adjacent_find(beg, end, binaryPred); // 返回指向第一对相邻重复元素的迭代器，无相邻元素则返回 end
search_n(beg, end, count, val); // 返回一个迭代器，从此位置开始有 count 个相等元素，不存在则返回 end
search_n(beg, end, count, val, binaryPred); // 返回一个迭代器，从此位置开始有 count 个相等元素，不存在则返回 end

// 查找子序列算法，除 find_first_of（前两个输入迭代器，后两个前向迭代器） 外，都要求两个前向迭代器
search(beg1, end1, beg2, end2); // 返回第二个输入范围（子序列）在爹一个输入范围中第一次出现的位置，未找到则返回 end1
search(beg1, end1, beg2, end2, binaryPred); // 返回第二个输入范围（子序列）在爹一个输入范围中第一次出现的位置，未找到则返回 end1
find_first_of(beg1, end1, beg2, end2); // 返回一个迭代器，指向第二个输入范围中任意元素在第一个范围中首次出现的位置，未找到则返回end1
find_first_of(beg1, end1, beg2, end2, binaryPred); // 返回一个迭代器，指向第二个输入范围中任意元素在第一个范围中首次出现的位置，未找到则返回end1
find_end(beg1, end1, beg2, end2); // 类似 search，但返回的最后一次出现的位置。如果第二个输入范围为空，或者在第一个输入范围为空，或者在第一个输入范围中未找到它，则返回 end1
find_end(beg1, end1, beg2, end2, binaryPred); // 类似 search，但返回的最后一次出现的位置。如果第二个输入范围为空，或者在第一个输入范围为空，或者在第一个输入范围中未找到它，则返回 end1

// 其他只读算法，传入输入迭代器
for_each(beg, end, unaryOp); // 对输入序列中的每个元素应用可调用对象 unaryOp，unaryOp 的返回值被忽略
mismatch(beg1, end1, beg2); // 比较两个序列中的元素。返回一个迭代器的 pair，表示两个序列中第一个不匹配的元素
mismatch(beg1, end1, beg2, binaryPred); // 比较两个序列中的元素。返回一个迭代器的 pair，表示两个序列中第一个不匹配的元素
equal(beg1, end1, beg2); // 比较每个元素，确定两个序列是否相等。
equal(beg1, end1, beg2, binaryPred); // 比较每个元素，确定两个序列是否相等。

// 二分搜索算法，传入前向迭代器或随机访问迭代器（random-access iterator），要求序列中的元素已经是有序的。通过小于运算符（<）或 comp 比较操作实现比较。
lower_bound(beg, end, val); // 返回一个非递减序列 [beg, end) 中的第一个大于等于值 val 的位置的迭代器，不存在则返回 end
lower_bound(beg, end, val, comp); // 返回一个非递减序列 [beg, end) 中的第一个大于等于值 val 的位置的迭代器，不存在则返回 end
upper_bound(beg, end, val); // 返回一个非递减序列 [beg, end) 中第一个大于 val 的位置的迭代器，不存在则返回 end
upper_bound(beg, end, val, comp); // 返回一个非递减序列 [beg, end) 中第一个大于 val 的位置的迭代器，不存在则返回 end
equal_range(beg, end, val); // 返回一个 pair，其 first 成员是 lower_bound 返回的迭代器，其 second 成员是 upper_bound 返回的迭代器
binary_search(beg, end, val); // 返回一个 bool 值，指出序列中是否包含等于 val 的元素。对于两个值 x 和 y，当 x 不小于 y 且 y 也不小于 x 时，认为它们相等。

// 只写不读算法，要求输出迭代器（output iterator）
fill(beg, end, val); // 将 val 赋予每个元素，返回 void
fill_n(beg, cnt, val); // 将 val 赋予 cnt 个元素，返回指向写入到输出序列最有一个元素之后位置的迭代器
genetate(beg, end, Gen); // 每次调用 Gen() 生成不同的值赋予每个序列，返回 void
genetate_n(beg, cnt, Gen); // 每次调用 Gen() 生成不同的值赋予 cnt 个序列，返回指向写入到输出序列最有一个元素之后位置的迭代器

// 使用输入迭代器的写算法，读取一个输入序列，将值写入到一个输出序列（dest）中
copy(beg, end, dest); // 从输入范围将元素拷贝所有元素到 dest 指定定的目的序列
copy_if(beg, end, dest, unaryPred); // 从输入范围将元素拷贝满足 unaryPred 的元素到 dest 指定定的目的序列
copy_n(beg, n, dest); // 从输入范围将元素拷贝前 n 个元素到 dest 指定定的目的序列
move(beg, end, dest); // 对输入序列中的每个元素调用 std::move，将其移动到迭代器 dest 开始始的序列中
transform(beg, end, dest, unaryOp); // 调用给定操作（一元操作），并将结果写到dest中
transform(beg, end, beg2, dest, binaryOp); // 调用给定操作（二元操作），并将结果写到dest中
replace_copy(beg, end, dest, old_val, new_val); // 将每个元素拷贝到 dest，将等于 old_val 的的元素替换为 new_val
replace_copy_if(beg, end, dest, unaryPred, new_val); // 将每个元素拷贝到 dest，将满足 unaryPred 的的元素替换为 new_val
merge(beg1, end1, beg2, end2, dest); // 两个输入序列必须都是有序的，用 < 运算符将合并后的序列写入到 dest 中
merge(beg1, end1, beg2, end2, dest, comp); // 两个输入序列必须都是有序的，使用给定的比较操作（comp）将合并后的序列写入到 dest 中

// 使用前向迭代器的写算法，要求前向迭代器
iter_swap(iter1, iter2); // 交换 iter1 和 iter2 所表示的元素，返回 void
swap_ranges(beg1, end1, beg2); // 将输入范围中所有元素与 beg2 开始的第二个序列中所有元素进行交换。返回递增后的的 beg2，指向最后一个交换元素之后的位置。
replace(beg, end, old_val, new_val); // 用 new_val 替换等于 old_val 的每个匹配元素
replace_if(beg, end, unaryPred, new_val); // 用 new_val 替换满足 unaryPred 的每个匹配元素

// 使用双向迭代器的写算法，要求双向选代器（bidirectional iterator）
copy_backward(beg, end, dest); // 从输入范围中拷贝元素到指定目的位置。如果范围为空,则返回值为 dest；否则，返回值表示从 *beg 中拷贝或移动的元素。
move_backward(beg, end, dest);  // 从输入范围中移动元素到指定目的位置。如果范围为空,则返回值为 dest；否则,返回值表示从 *beg 中拷贝或移动的元素。
inplace_merge(beg, mid, end); // 将同一个序列中的两个有序子序列合并为单一的有序序列。beg 到 mid 间的子序列和 mid 到 end 间的子序列被合并，并被写入到原序列中。使用 < 比较元素。
inplace_merge(beg, mid, end, comp); // 将同一个序列中的两个有序子序列合并为单一的有序序列。beg 到 mid 间的子序列和 mid 到 end 间的子序列被合并，并被写入到原序列中。使用给定的 comp 操作。

// 划分算法，要求双向选代器（bidirectional iterator）
is_partitioned(beg, end, unaryPred); // 如果所有满足谓词 unaryPred 的元素都在不满足 unarypred 的元素之前，则返回 true。若序列为空，也返回 true
partition_copy(beg, end, dest1, dest2, unaryPred); // 将满足 unaryPred 的元素拷贝到到 dest1，并将不满足 unaryPred 的元素拷贝到到 dest2。返回一个迭代器 pair，其 first 成员表示拷贝到 dest1 的的元素的末尾，second 表示拷贝到 dest2 的元素的末尾。
partitioned_point(beg, end, unaryPred); // 输入序列必须是已经用 unaryPred 划分过的。返回满足  unaryPred 的范围的尾后迭代器。如果返回的迭代器不是 end，则它指向的元素及其后的元素必须都不满足 unaryPred
stable_partition(beg, end, unaryPred); // 使用 unaryPred 划分输入序列。满足 unaryPred 的元素放置在序列开始，不满足的元素放在序列尾部。返回一个迭代器，指向最后一个满足 unaryPred 的元素之后的位置如果所有元素都不满足 unaryPred，则返回 beg
partition(beg, end, unaryPred); // 使用 unaryPred 划分输入序列。满足 unaryPred 的元素放置在序列开始，不满足的元素放在序列尾部。返回一个迭代器，指向最后一个满足 unaryPred 的元素之后的位置如果所有元素都不满足 unaryPred，则返回 beg

// 排序算法，要求随机访问迭代器（random-access iterator）
sort(beg, end); // 排序整个范围
stable_sort(beg, end); // 排序整个范围（稳定排序）
sort(beg, end, comp); // 排序整个范围
stable_sort(beg, end, comp); // 排序整个范围（稳定排序）
is_sorted(beg, end); // 返回一个 bool 值，指出整个输入序列是否有序
is_sorted(beg, end, comp); // 返回一个 bool 值，指出整个输入序列是否有序
is_sorted_until(beg, end); // 在输入序列中査找最长初始有序子序列，并返回子序列的尾后迭代器
is_sorted_until(beg, end, comp); // 在输入序列中査找最长初始有序子序列，并返回子序列的尾后迭代器
partial_sort(beg, mid, end); // 排序 mid-beg 个元素。即，如果 mid-beg 等于 42，则此函数将值最小的 42 个元素有序放在序列前 42 个位置
partial_sort(beg, mid, end, comp); // 排序 mid-beg 个元素。即，如果 mid-beg 等于 42，则此函数将值最小的 42 个元素有序放在序列前 42 个位置
partial_sort_copy(beg, end, destBeg, destEnd); // 排序输入范围中的元素，并将足够多的已排序元素放到 destBeg 和 destEnd 所指示的序列中
partial_sort_copy(beg, end, destBeg, destEnd, comp); // 排序输入范围中的元素，并将足够多的已排序元素放到 destBeg 和 destEnd 所指示的序列中
nth_element(beg, nth, end); // nth 是一个迭代器，指向输入序列中第 n 大的元素。nth 之前的元素都小于等于它，而之后的元素都大于等于它
nth_element(beg, nth, end, comp); // nth 是一个迭代器，指向输入序列中第 n 大的元素。nth 之前的元素都小于等于它，而之后的元素都大于等于它

// 使用前向迭代器的重排算法。普通版本在输入序列自身内部重拍元素，_copy 版本完成重拍后写入到指定目的序列中，而不改变输入序列
remove(beg, end, val); // 通过用保留的元素覆盖要删除的元素实现删除 ==val 的元素，返回一个指向最后一个删除元素的尾后位置的迭代器
remove_if(beg, end, unaryPred); // 通过用保留的元素覆盖要删除的元素实现删除满足 unaryPred 的元素，返回一个指向最后一个删除元素的尾后位置的迭代器
remove_copy(beg, end, dest, val); // 通过用保留的元素覆盖要删除的元素实现删除 ==val 的元素，返回一个指向最后一个删除元素的尾后位置的迭代器
remove_copy_if(beg, end, dest, unaryPred); // 通过用保留的元素覆盖要删除的元素实现删除满足 unaryPred 的元素，返回一个指向最后一个删除元素的尾后位置的迭代器
unique(beg, end); // 通过对覆盖相邻的重复元素（用 == 确定是否相同）实现重排序列。返回一个迭代器，指向不重复元素的尾后位置
unique (beg, end, binaryPred); // 通过对覆盖相邻的重复元素（用 binaryPred 确定是否相同）实现重排序列。返回一个迭代器，指向不重复元素的尾后位置
unique_copy(beg, end, dest); // 通过对覆盖相邻的重复元素（用 == 确定是否相同）实现重排序列。返回一个迭代器，指向不重复元素的尾后位置
unique_copy_if(beg, end, dest, binaryPred); // 通过对覆盖相邻的重复元素（用 binaryPred 确定是否相同）实现重排序列。返回一个迭代器，指向不重复元素的尾后位置
rotate(beg, mid, end); // 围绕 mid 指向的元素进行元素转动。元素 mid 成为为首元素，随后是 mid+1 到到 end 之前的元素，再接着是 beg 到 mid 之前的元素。返回一个迭代器，指向原来在 beg 位置的元素
rotate_copy(beg, mid, end, dest); // 围绕 mid 指向的元素进行元素转动。元素 mid 成为为首元素，随后是 mid+1 到到 end 之前的元素，再接着是 beg 到 mid 之前的元素。返回一个迭代器，指向原来在 beg 位置的元素

// 使用双向迭代器的重排算法
reverse(beg, end); // 翻转序列中的元素，返回 void
reverse_copy(beg, end, dest);; // 翻转序列中的元素，返回一个迭代器，指向拷贝到目的序列的元素的尾后位置

// 使用随机访问迭代器的重排算法
random_shuffle(beg, end); // 混洗输入序列中的元素，返回 void
random_shuffle(beg, end, rand); // 混洗输入序列中的元素，rand 接受一个正整数的随机对象，返回 void
shuffle(beg, end, Uniform_rand); // 混洗输入序列中的元素，Uniform_rand 必须满足均匀分布随机数生成器的要求，返回 void

// 最小值和最大值，使用 < 运算符或给定的比较操作 comp 进行比较
min(val1, va12); // 返回 val1 和 val2 中的最小值，两个实参的类型必须完全一致。参数和返回类型都是 const的引引用，意味着对象不会被拷贝。下略
min(val1, val2, comp);
min(init_list);
min(init_list, comp);
max(val1, val2);
max(val1, val2, comp);
max(init_list);
max(init_list, comp);
minmax(val1, val2); // 返回一个 pair，其 first 成员为提供的值中的较小者，second 成员为较大者。下略
minmax(vall, val2, comp);
minmax(init_list);
minmax(init_list, comp);
min_element(beg, end); // 返回指向输入序列中最小元素的迭代器
min_element(beg, end, comp); // 返回指向输入序列中最小元素的迭代器
max_element(beg, end); // 返回指向输入序列中最大元素的迭代器
max_element(beg, end, comp); // 返回指向输入序列中最大元素的迭代器
minmax_element(beg, end); // 返回一个 pair，其中 first 成员为最小元素，second 成员为最大元素
minmax_element(beg, end, comp); // 返回一个 pair，其中 first 成员为最小元素，second 成员为最大元素

// 字典序比较，根据第一对不相等的元素的相对大小来返回结果。如果第一个序列在字典序中小于第二个序列，则返回 true。否则，返回 fa1se。如果个序列比另一个短，且所有元素都与较长序列的对应元素相等，则较短序列在字典序中更小。如果序列长度相等，且对应元素都相等，则在字典序中任何一个都不大于另外一个。
lexicographical_compare(beg1, end1, beg2, end2);
lexicographical_compare(beg1, end1, beg2, end2, comp);
```

# STL__STL.md

## STL

### 网站

* github . huihut/note/STL.md
* cplusplus . stl
* cppreference . C++ 参考手册
* CSDN专栏：STL学习笔记

### 组成

* 容器（containers）
* 算法（algorithms）
* 迭代器（iterators）
* 仿函数（functors）
* 配接器（adapters）
* 空间配置器（allocator）

### 容器（containers）

* 序列式容器（sequence containers）：元素都是可序（ordered），但未必是有序（sorted）
* 关联式容器（associattive containers）

#### array

array是固定大小的顺序容器，它们保存了一个以严格的线性顺序排列的特定数量的元素。

在内部，一个数组除了它所包含的元素（甚至不是它的大小，它是一个模板参数，在编译时是固定的）以外不保存任何数据。存储大小与用语言括号语法（[]）声明的普通数组一样高效。这个类只是增加了一层成员函数和全局函数，所以数组可以作为标准容器使用。

与其他标准容器不同，数组具有固定的大小，并且不通过分配器管理其元素的分配：它们是封装固定大小数组元素的聚合类型。因此，他们不能动态地扩大或缩小。

零大小的数组是有效的，但是它们不应该被解除引用（成员的前面，后面和数据）。

与标准库中的其他容器不同，交换两个数组容器是一种线性操作，它涉及单独交换范围内的所有元素，这通常是相当低效的操作。另一方面，这允许迭代器在两个容器中的元素保持其原始容器关联。

数组容器的另一个独特特性是它们可以被当作元组对象来处理：array头部重载get函数来访问数组元素，就像它是一个元组，以及专门的tuple_size和tuple_element类型。

```cpp
template < class T, size_t N > class array;
```

![]

##### array::begin

返回指向数组容器中第一个元素的迭代器。

![]

```cpp
      iterator begin() noexcept;
const_iterator begin() const noexcept;
```

Example

```cpp
#include <iostream>
#include <array>

int main()
{
    std::array<int, 5> myarray = {2, 16, 77,34, 50};
    std::cout << "myarray contains:";
    for(auto it = myarray.begin(); it != myarray.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';

    return 0;
}
```
Output
```
myarray contains: 2 16 77 34 50
```

##### array::end

返回指向数组容器中最后一个元素之后的理论元素的迭代器。

```cpp
      iterator end() noexcept;
const_iterator end() const noexcept;
```

Example

```cpp
#include <iostream>
#include <array>

int main ()
{
    std::array<int,5> myarray = { 5, 19, 77, 34, 99 };

    std::cout << "myarray contains:";
    for ( auto it = myarray.begin(); it != myarray.end(); ++it )
        std::cout << ' ' << *it;

    std::cout << '\n';

    return 0;
}
```
Output
```
myarray contains: 5 19 77 34 99
```

##### array::rbegin

返回指向数组容器中最后一个元素的反向迭代器。

```cpp
      reverse_iterator rbegin（）noexcept;
const_reverse_iterator rbegin（）const noexcept;
```
Example
```cpp
#include <iostream>
#include <array>

int main ()
{
    std::array<int,4> myarray = {4, 26, 80, 14} ;
    for(auto rit = myarray.rbegin(); rit < myarray.rend(); ++rit)
        std::cout << ' ' << *rit;

    std::cout << '\n';

    return 0;
}
```
Output
```
myarray contains: 14 80 26 4
```

##### array::rend

返回一个反向迭代器，指向数组中第一个元素之前的理论元素（这被认为是它的反向结束）。

```cpp
      reverse_iterator rend() noexcept;
const_reverse_iterator rend() const noexcept;
```

Example
```cpp
#include <iostream>
#include <array>

int main ()
{
    std::array<int,4> myarray = {4, 26, 80, 14};
    std::cout << "myarray contains";
    for(auto rit = myarray.rbegin(); rit < myarray.rend(); ++rit)
        std::cout << ' ' << *rit;

    std::cout << '\n';

    return 0;
}
```
Output
```
myarray contains: 14 80 26 4
```

##### array::cbegin

返回指向数组容器中第一个元素的常量迭代器（const_iterator）；这个迭代器可以增加和减少，但是不能用来修改它指向的内容。
```cpp
const_iterator cbegin（）const noexcept;
```
Example
```cpp
#include <iostream>
#include <array>

int main ()
{
    std::array<int,5> myarray = {2, 16, 77, 34, 50};

    std::cout << "myarray contains:";

    for ( auto it = myarray.cbegin(); it != myarray.cend(); ++it )
        std::cout << ' ' << *it;   // cannot modify *it

    std::cout << '\n';

    return 0;
}
```
Output
```
myarray contains: 2 16 77 34 50
```

##### array::cend
返回指向数组容器中最后一个元素之后的理论元素的常量迭代器（const_iterator）。这个迭代器可以增加和减少，但是不能用来修改它指向的内容。
```cpp
const_iterator cend() const noexcept;
```
Example
```cpp
#include <iostream>
#include <array>

int main ()
{
    std::array<int,5> myarray = { 15, 720, 801, 1002, 3502 };

    std::cout << "myarray contains:";
    for ( auto it = myarray.cbegin(); it != myarray.cend(); ++it )
        std::cout << ' ' << *it;   // cannot modify *it

    std::cout << '\n';

    return 0;
}
```
Output
```
myarray contains: 2 16 77 34 50
```

##### array::crbegin
返回指向数组容器中最后一个元素的常量反向迭代器（const_reverse_iterator）
```cpp
const_reverse_iterator crbegin（）const noexcept;
```
Example
```cpp
#include <iostream>
#include <array>

int main ()
{
    std::array<int,6> myarray = {10, 20, 30, 40, 50, 60} ;

    std::cout << "myarray backwards:";
    for ( auto rit=myarray.crbegin() ; rit < myarray.crend(); ++rit )
        std::cout << ' ' << *rit;   // cannot modify *rit

    std::cout << '\n';

    return 0;
}
```
Output
```
myarray backwards: 60 50 40 30 20 10
```
##### array::crend

返回指向数组中第一个元素之前的理论元素的常量反向迭代器（const_reverse_iterator），它被认为是其反向结束。

```cpp
const_reverse_iterator crend() const noexcept;
```
Example
```cpp
#include <iostream>
#include <array>

int main ()
{
    std::array<int,6> myarray = {10, 20, 30, 40, 50, 60} ;

    std::cout << "myarray backwards:";
    for ( auto rit=myarray.crbegin() ; rit < myarray.crend(); ++rit )
        std::cout << ' ' << *rit;   // cannot modify *rit

    std::cout << '\n';

    return 0;
}
```
Output
```
myarray backwards: 60 50 40 30 20 10
```

##### array::size

返回数组容器中元素的数量。

```cpp
constexpr size_type size（）noexcept;
```
Example
```cpp
#include <iostream>
#include <array>

int main ()
{
    std::array<int,5> myints;
    std::cout << "size of myints:" << myints.size() << std::endl;
    std::cout << "sizeof(myints):" << sizeof(myints) << std::endl;

    return 0;
}
```
Possible Output
```
size of myints: 5
sizeof(myints): 20
```
##### array::max_size
返回数组容器可容纳的最大元素数。数组对象的max_size与其size一样，始终等于用于实例化数组模板类的第二个模板参数。
```cpp
constexpr size_type max_size() noexcept;
```
Example
```cpp
#include <iostream>
#include <array>

int main ()
{
    std::array<int,10> myints;
    std::cout << "size of myints: " << myints.size() << '\n';
    std::cout << "max_size of myints: " << myints.max_size() << '\n';

    return 0;
}
```
Output
```
size of myints: 10
max_size of myints: 10
```

##### array::empty
返回一个布尔值，指示数组容器是否为空，即它的size()是否为0。
```cpp
constexpr bool empty() noexcept;
```
Example
```cpp
#include <iostream>
#include <array>

int main ()
{
  std::array<int,0> first;
  std::array<int,5> second;
  std::cout << "first " << (first.empty() ? "is empty" : "is not empty") << '\n';
  std::cout << "second " << (second.empty() ? "is empty" : "is not empty") << '\n';
  return 0;
}
```
Output:
```
first is empty
second is not empt
```
##### array::operator[]
返回数组中第n个位置的元素的引用。与array::at相似，但array::at会检查数组边界并通过抛出一个out_of_range异常来判断n是否超出范围，而array::operator[]不检查边界。
```cpp
      reference operator[] (size_type n);
const_reference operator[] (size_type n) const;
```
Example
```cpp
#include <iostream>
#include <array>

int main ()
{
    std::array<int,10> myarray;
    unsigned int i;

    // assign some values:
    for(i=0; i<10; i++)
        myarray[i] = i;

    // print content
    std::cout << "myarray contains:";
    for(i=0; i<10; i++)
        std::cout << ' ' << myarray[i];
    std::cout << '\n';

    return 0;
}
```
Output
```
myarray contains: 0 1 2 3 4 5 6 7 8 9
```
##### array::at
返回数组中第n个位置的元素的引用。与array::operator[]相似，但array::at会检查数组边界并通过抛出一个out_of_range异常来判断n是否超出范围，而array::operator[]不检查边界。
```cpp
      reference at ( size_type n );
const_reference at ( size_type n ) const;
```
Example
```cpp
#include <iostream>
#include <array>

int main()
{
	std::array<int, 10> myarray;
	unsigned int i;

	// assign some values:
	for (i = 0; i<10; i++)
		myarray[i] = i;

	// print content
	std::cout << "myarray contains:";
	for (i = 0; i<10; i++)
		std::cout << ' ' << myarray.at(i);
	std::cout << '\n';

	return 0;
}
```
Output
```
myarray contains: 0 1 2 3 4 5 6 7 8 9
```
##### array::front
返回对数组容器中第一个元素的引用。array::begin返回的是迭代器，array::front返回的是直接引用。
在空容器上调用此函数会导致未定义的行为。
```cpp
      reference front();
const_reference front() const;
```
Example
```cpp
#include <iostream>
#include <array>

int main ()
{
  std::array<int,3> myarray = {2, 16, 77};

  std::cout << "front is: " << myarray.front() << std::endl;   // 2
  std::cout << "back is: " << myarray.back() << std::endl;     // 77

  myarray.front() = 100;

  std::cout << "myarray now contains:";
  for ( int& x : myarray ) std::cout << ' ' << x;

  std::cout << '\n';

  return 0;
}
```
Output
```
front is: 2
back is: 77
myarray now contains: 100 16 77
```
##### array::back
返回对数组容器中最后一个元素的引用。array::end返回的是迭代器，array::back返回的是直接引用。
在空容器上调用此函数会导致未定义的行为。
```cpp
      reference back();
const_reference back() const;
```
Example
```cpp
#include <iostream>
#include <array>

int main ()
{
  std::array<int,3> myarray = {5, 19, 77};

  std::cout << "front is: " << myarray.front() << std::endl;   // 5
  std::cout << "back is: " << myarray.back() << std::endl;     // 77

  myarray.back() = 50;

  std::cout << "myarray now contains:";
  for ( int& x : myarray ) std::cout << ' ' << x;
  std::cout << '\n';

  return 0;
}
```
Output
```
front is: 5
back is: 77
myarray now contains: 5 19 50
```
##### array::data
返回指向数组对象中第一个元素的指针。

由于数组中的元素存储在连续的存储位置，所以检索到的指针可以偏移以访问数组中的任何元素。
```cpp
      value_type* data() noexcept;
const value_type* data() const noexcept;
```
Example
```cpp
#include <iostream>
#include <cstring>
#include <array>

int main ()
{
  const char* cstr = "Test string";
  std::array<char,12> charray;

  std::memcpy (charray.data(),cstr,12);

  std::cout << charray.data() << '\n';

  return 0;
}
```
Output
```
Test string
```
##### array::fill
用val填充数组所有元素，将val设置为数组对象中所有元素的值。
```cpp
void fill (const value_type& val);
```
Example
```cpp
#include <iostream>
#include <array>

int main () {
  std::array<int,6> myarray;

  myarray.fill(5);

  std::cout << "myarray contains:";
  for ( int& x : myarray) { std::cout << ' ' << x; }

  std::cout << '\n';

  return 0;
}
```
Output
```
myarray contains: 5 5 5 5 5 5
```
##### array::swap
通过x的内容交换数组的内容，这是另一个相同类型的数组对象（包括相同的大小）。

与其他容器的交换成员函数不同，此成员函数通过在各个元素之间执行与其大小相同的单独交换操作，以线性时间运行。
```cpp
void swap (array& x) noexcept(noexcept(swap(declval<value_type&>(),declval<value_type&>())));
```
Example
```cpp
#include <iostream>
#include <array>

int main ()
{
  std::array<int,5> first = {10, 20, 30, 40, 50};
  std::array<int,5> second = {11, 22, 33, 44, 55};

  first.swap (second);

  std::cout << "first:";
  for (int& x : first) std::cout << ' ' << x;
  std::cout << '\n';

  std::cout << "second:";
  for (int& x : second) std::cout << ' ' << x;
  std::cout << '\n';

  return 0;
}
```
Output
```
first: 11 22 33 44 55
second: 10 20 30 40 50
```
##### get（array）
形如：std::get<0>(myarray)；传入一个数组容器，返回指定位置元素的引用。
```cpp
template <size_t I，class T，size_t N> T＆get（array <T，N>＆arr）noexcept;
template <size_t I，class T，size_t N> T && get（array <T，N> && arr）noexcept;
template <size_t I，class T，size_t N> const T＆get（const array <T，N>＆arr）noexcept;
```
Example
```cpp
#include <iostream>
#include <array>
#include <tuple>

int main ()
{
  std::array<int,3> myarray = {10, 20, 30};
  std::tuple<int,int,int> mytuple (10, 20, 30);

  std::tuple_element<0,decltype(myarray)>::type myelement;  // int myelement

  myelement = std::get<2>(myarray);
  std::get<2>(myarray) = std::get<0>(myarray);
  std::get<0>(myarray) = myelement;

  std::cout << "first element in myarray: " << std::get<0>(myarray) << "\n";
  std::cout << "first element in mytuple: " << std::get<0>(mytuple) << "\n";

  return 0;
}
```
Output
```
first element in myarray: 30
first element in mytuple: 10
```
##### relational operators (array)
形如：arrayA != arrayB、arrayA > arrayB；依此比较数组每个元素的大小关系。
```cpp
（1）
template <class T，size_T N>
  bool operator ==（const array <T，N>＆lhs，const array <T，N>＆rhs）;
（2）
template <class T，size_T N>
  bool operator！=（const array <T，N>＆lhs，const array <T，N>＆rhs）;
（3）
template <class T，size_T N>
  bool operator <（const array <T，N>＆lhs，const array <T，N>＆rhs）;
（4）
template <class T，size_T N>
  bool operator <=（const array <T，N>＆lhs，const array <T，N>＆rhs）;
（5）
template <class T，size_T N>
  bool operator>（const array <T，N>＆lhs，const array <T，N>＆rhs）;
（6）
template <class T，size_T N>
  bool operator> =（const array <T，N>＆lhs，const array <T，N>＆rhs）;
```
Example
```cpp
#include <iostream>
#include <array>

int main ()
{
  std::array<int,5> a = {10, 20, 30, 40, 50};
  std::array<int,5> b = {10, 20, 30, 40, 50};
  std::array<int,5> c = {50, 40, 30, 20, 10};

  if (a==b) std::cout << "a and b are equal\n";
  if (b!=c) std::cout << "b and c are not equal\n";
  if (b<c) std::cout << "b is less than c\n";
  if (c>b) std::cout << "c is greater than b\n";
  if (a<=b) std::cout << "a is less than or equal to b\n";
  if (a>=b) std::cout << "a is greater than or equal to b\n";

  return 0;
}
```
Output
```
a and b are equal
b and c are not equal
b is less than c
c is greater than b
a is less than or equal to b
a is greater than or equal to b
```
#### vector
vector是表示可以改变大小的数组的序列容器。

就像数组一样，vector为它们的元素使用连续的存储位置，这意味着它们的元素也可以使用到其元素的常规指针上的偏移来访问，而且和数组一样高效。但是与数组不同的是，它们的大小可以动态地改变，它们的存储由容器自动处理。

在内部，vector使用一个动态分配的数组来存储它们的元素。这个数组可能需要重新分配，以便在插入新元素时增加大小，这意味着分配一个新数组并将所有元素移动到其中。就处理时间而言，这是一个相对昂贵的任务，因此每次将元素添加到容器时矢量都不会重新分配。

相反，vector容器可以分配一些额外的存储以适应可能的增长，并且因此容器可以具有比严格需要包含其元素（即，其大小）的存储更大的实际容量。库可以实现不同的策略的增长到内存使用和重新分配之间的平衡，但在任何情况下，再分配应仅在对数生长的间隔发生尺寸，使得在所述载体的末端各个元件的插入可以与提供分期常量时间复杂性。

因此，与数组相比，载体消耗更多的内存来交换管理存储和以有效方式动态增长的能力。

与其他动态序列容器（deques，lists和 forward\_lists ）相比，vector非常有效地访问其元素（就像数组一样），并相对有效地从元素末尾添加或移除元素。对于涉及插入或移除除了结尾之外的位置的元素的操作，它们执行比其他位置更差的操作，并且具有比列表和 forward\_lists 更不一致的迭代器和引用。

针对 vector 的各种常见操作的复杂度（效率）如下：
* 随机访问 - 常数 O(1)
* 在尾部增删元素 - 平摊（amortized）常数 O(1)}}
* 增删元素 - 至 vector 尾部的线性距离 O(n)}}

```cpp
template < class T, class Alloc = allocator<T> > class vector;
```
![]

##### vector::vector
（1）empty容器构造函数（默认构造函数）
构造一个空的容器，没有元素。
（2）fill构造函数
用n个元素构造一个容器。每个元素都是val的副本（如果提供）。
（3）范围（range）构造器
使用与[ range，first，last]范围内的元素相同的顺序构造一个容器，其中的每个元素都是emplace -从该范围内相应的元素构造而成。
（4）复制（copy）构造函数（并用分配器复制）
按照相同的顺序构造一个包含x中每个元素的副本的容器。
（5）移动（move）构造函数（和分配器移动）
构造一个获取x元素的容器。
如果指定了alloc并且与x的分配器不同，那么元素将被移动。否则，没有构建元素（他们的所有权直接转移）。
x保持未指定但有效的状态。
（6）初始化列表构造函数
构造一个容器中的每个元件中的一个拷贝的IL，以相同的顺序。

```cpp
default (1)
explicit vector (const allocator_type& alloc = allocator_type());
fill (2)
explicit vector (size_type n);
         vector (size_type n, const value_type& val,
                 const allocator_type& alloc = allocator_type());
range (3)
template <class InputIterator>
  vector (InputIterator first, InputIterator last,
          const allocator_type& alloc = allocator_type());
copy (4)
vector (const vector& x);
vector (const vector& x, const allocator_type& alloc);
move (5)
vector (vector&& x);
vector (vector&& x, const allocator_type& alloc);
initializer list (6)
vector (initializer_list<value_type> il,
       const allocator_type& alloc = allocator_type());
```
Example
```cpp
#include <iostream>
#include <vector>

int main ()
{
    // constructors used in the same order as described above:
    std::vector<int> first;             // empty vector of ints
    std::vector<int> second(4, 100);    // four ints with value 100
    std::vector<int> third(second.begin(), second.end());// iterating through second
    std::vector<int> fourth(third);     // a copy of third

    // the iterator constructor can also be used to construct from arrays:
    int myints[] = {16,2,77,29};
    std::vector<int> fifth(myints, myints + sizeof(myints) / sizeof(int));

    std::cout << "The contents of fifth are:";
    for(std::vector<int>::iterator it = fifth.begin(); it != fifth.end(); ++it)
        std::cout << ' ' << *it;
    std::cout << '\n';

    return 0;
}
```
Output
```
The contents of fifth are: 16 2 77 29
```
##### vector::~vector
销毁容器对象。这将在每个包含的元素上调用allocator_traits::destroy，并使用其分配器释放由矢量分配的所有存储容量。
```cpp
~vector();
```
##### vector::operator=
将新内容分配给容器，替换其当前内容，并相应地修改其大小。
```cpp
copy (1)
vector& operator= (const vector& x);
move (2)
vector& operator= (vector&& x);
initializer list (3)
vector& operator= (initializer_list<value_type> il);
```
Example
```
#include <iostream>
#include <vector>

int main ()
{
  std::vector<int> foo (3,0);
  std::vector<int> bar (5,0);

  bar = foo;
  foo = std::vector<int>();

  std::cout << "Size of foo: " << int(foo.size()) << '\n';
  std::cout << "Size of bar: " << int(bar.size()) << '\n';
  return 0;
}
```
Output
```
Size of foo: 0
Size of bar: 3
```
##### vector::begin
##### vector::end
##### vector::rbegin
##### vector::rend
##### vector::cbegin
##### vector::cend
##### vector::rcbegin
##### vector::rcend
##### vector::size

返回vector中元素的数量。

这是vector中保存的实际对象的数量，不一定等于其存储容量。

```cpp
size_type size() const noexcept;
```
Example
```cpp
#include <iostream>
#include <vector>

int main ()
{
  std::vector<int> myints;
  std::cout << "0. size: " << myints.size() << '\n';

  for (int i=0; i<10; i++) myints.push_back(i);
  std::cout << "1. size: " << myints.size() << '\n';

  myints.insert (myints.end(),10,100);
  std::cout << "2. size: " << myints.size() << '\n';

  myints.pop_back();
  std::cout << "3. size: " << myints.size() << '\n';

  return 0;
}
```
Output
```
0. size: 0
1. size: 10
2. size: 20
3. size: 19
```
##### vector::max_size
返回该vector可容纳的元素的最大数量。由于已知的系统或库实现限制，

这是容器可以达到的最大潜在大小，但容器无法保证能够达到该大小：在达到该大小之前的任何时间，仍然无法分配存储。
```cpp
size_type max_size() const noexcept;
```
Example
```cpp
#include <iostream>
#include <vector>

int main ()
{
  std::vector<int> myvector;

  // set some content in the vector:
  for (int i=0; i<100; i++) myvector.push_back(i);

  std::cout << "size: " << myvector.size() << "\n";
  std::cout << "capacity: " << myvector.capacity() << "\n";
  std::cout << "max_size: " << myvector.max_size() << "\n";
  return 0;
}
```
A possible output for this program could be:
```
size: 100
capacity: 128
max_size: 1073741823
```
##### vector::resize
调整容器的大小，使其包含n个元素。

如果n小于当前的容器size，内容将被缩小到前n个元素，将其删除（并销毁它们）。

如果n大于当前容器size，则通过在末尾插入尽可能多的元素以达到大小n来扩展内容。如果指定了val，则新元素将初始化为val的副本，否则将进行值初始化。

如果n也大于当前的容器的capacity（容量），分配的存储空间将自动重新分配。

注意这个函数通过插入或者删除元素的内容来改变容器的实际内容。
```cpp
void resize (size_type n);
void resize (size_type n, const value_type& val);
```
Example
```cpp
#include <iostream>
#include <vector>

int main ()
{
  std::vector<int> myvector;

  // set some initial content:
  for (int i=1;i<10;i++) myvector.push_back(i);

  myvector.resize(5);
  myvector.resize(8,100);
  myvector.resize(12);

  std::cout << "myvector contains:";
  for (int i=0;i<myvector.size();i++)
    std::cout << ' ' << myvector[i];
  std::cout << '\n';

  return 0;
}
```
Output
```
myvector contains: 1 2 3 4 5 100 100 100 0 0 0 0
```
##### vector::capacity
返回当前为vector分配的存储空间的大小，用元素表示。这个capacity(容量)不一定等于vector的size。它可以相等或更大，额外的空间允许适应增长，而不需要重新分配每个插入。
```cpp
size_type capacity() const noexcept;
```
Example
```cpp
#include <iostream>
#include <vector>

int main ()
{
  std::vector<int> myvector;

  // set some content in the vector:
  for (int i=0; i<100; i++) myvector.push_back(i);

  std::cout << "size: " << (int) myvector.size() << '\n';
  std::cout << "capacity: " << (int) myvector.capacity() << '\n';
  std::cout << "max_size: " << (int) myvector.max_size() << '\n';
  return 0;
}
```
A possible output for this program could be:
```
size: 100
capacity: 128
max_size: 1073741823
```
##### vector::empty
返回vector是否为空（即，它的size是否为0）
```cpp
bool empty() const noexcept;
```
Example
```cpp
#include <iostream>
#include <vector>

int main ()
{
  std::vector<int> myvector;
  int sum (0);

  for (int i=1;i<=10;i++) myvector.push_back(i);

  while (!myvector.empty())
  {
     sum += myvector.back();
     myvector.pop_back();
  }

  std::cout << "total: " << sum << '\n';

  return 0;
}
```
Output
```
total: 55
```
##### vector::reserve
请求vector容量至少足以包含n个元素。

如果n大于当前vector容量，则该函数使容器重新分配其存储容量，从而将其容量增加到n（或更大）。

在所有其他情况下，函数调用不会导致重新分配，并且vector容量不受影响。

这个函数对vector大小没有影响，也不能改变它的元素。
```cpp
void reserve (size_type n);
```
Example
```cpp
#include <iostream>
#include <vector>

int main ()
{
  std::vector<int>::size_type sz;

  std::vector<int> foo;
  sz = foo.capacity();
  std::cout << "making foo grow:\n";
  for (int i=0; i<100; ++i) {
    foo.push_back(i);
    if (sz!=foo.capacity()) {
      sz = foo.capacity();
      std::cout << "capacity changed: " << sz << '\n';
    }
  }

  std::vector<int> bar;
  sz = bar.capacity();
  bar.reserve(100);   // this is the only difference with foo above
  std::cout << "making bar grow:\n";
  for (int i=0; i<100; ++i) {
    bar.push_back(i);
    if (sz!=bar.capacity()) {
      sz = bar.capacity();
      std::cout << "capacity changed: " << sz << '\n';
    }
  }
  return 0;
}
```
Possible output
```
making foo grow:
capacity changed: 1
capacity changed: 2
capacity changed: 4
capacity changed: 8
capacity changed: 16
capacity changed: 32
capacity changed: 64
capacity changed: 128
making bar grow:
capacity changed: 100
```
##### vector::shrink_to_fit
要求容器减小其capacity(容量)以适应其尺寸。

该请求是非绑定的，并且容器实现可以自由地进行优化，并且保持capacity大于其size的vector。 这可能导致重新分配，但对矢量大小没有影响，并且不能改变其元素。
```cpp
void shrink_to_fit();
```
Example
```cpp
#include <iostream>
#include <vector>

int main ()
{
  std::vector<int> myvector (100);
  std::cout << "1. capacity of myvector: " << myvector.capacity() << '\n';

  myvector.resize(10);
  std::cout << "2. capacity of myvector: " << myvector.capacity() << '\n';

  myvector.shrink_to_fit();
  std::cout << "3. capacity of myvector: " << myvector.capacity() << '\n';

  return 0;
}
```
Possible output
```
1. capacity of myvector: 100
2. capacity of myvector: 100
3. capacity of myvector: 10
```
##### vector::operator[]
##### vector::at
##### vector::front
##### vector::back
##### vector::data

##### vector::assign
将新内容分配给vector，替换其当前内容，并相应地修改其大小。

在范围版本（1）中，新内容是从第一个和最后一个范围内的每个元素按相同顺序构造的元素。

在填充版本（2）中，新内容是n个元素，每个元素都被初始化为一个val的副本。

在初始化列表版本（3）中，新内容是以相同顺序作为初始化列表传递的值的副本。

所述内部分配器被用于（通过其性状），以分配和解除分配存储器如果重新分配发生。它也习惯于摧毁所有现有的元素，并构建新的元素。
```cpp
range (1)
template <class InputIterator>
  void assign (InputIterator first, InputIterator last);
fill (2)
void assign (size_type n, const value_type& val);
initializer list (3)
void assign (initializer_list<value_type> il);
```
Example
```cpp
#include <iostream>
#include <vector>

int main ()
{
  std::vector<int> first;
  std::vector<int> second;
  std::vector<int> third;

  first.assign (7,100);             // 7 ints with a value of 100

  std::vector<int>::iterator it;
  it=first.begin()+1;

  second.assign (it,first.end()-1); // the 5 central values of first

  int myints[] = {1776,7,4};
  third.assign (myints,myints+3);   // assigning from array.

  std::cout << "Size of first: " << int (first.size()) << '\n';
  std::cout << "Size of second: " << int (second.size()) << '\n';
  std::cout << "Size of third: " << int (third.size()) << '\n';
  return 0;
}
```
Output
```
Size of first: 7
Size of second: 5
Size of third: 3
```

补充：vector::assign 与 vector::operator= 的区别：

1. vector::assign 实现源码

```cpp
void assign(size_type __n, const _Tp& __val) { _M_fill_assign(__n, __val); }

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_assign(size_t __n, const value_type& __val)
{
  if (__n > capacity()) {
    vector<_Tp, _Alloc> __tmp(__n, __val, get_allocator());
    __tmp.swap(*this);
  }
  else if (__n > size()) {
    fill(begin(), end(), __val);
    _M_finish = uninitialized_fill_n(_M_finish, __n - size(), __val);
  }
  else
    erase(fill_n(begin(), __n, __val), end());
}
```

2. vector::operator= 实现源码

```cpp
template <class _Tp, class _Alloc>
vector<_Tp,_Alloc>&
vector<_Tp,_Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
  if (&__x != this) {
    const size_type __xlen = __x.size();
    if (__xlen > capacity()) {
      iterator __tmp = _M_allocate_and_copy(__xlen, __x.begin(), __x.end());
      destroy(_M_start, _M_finish);
      _M_deallocate(_M_start, _M_end_of_storage - _M_start);
      _M_start = __tmp;
      _M_end_of_storage = _M_start + __xlen;
    }
    else if (size() >= __xlen) {
      iterator __i = copy(__x.begin(), __x.end(), begin());
      destroy(__i, _M_finish);
    }
    else {
      copy(__x.begin(), __x.begin() + size(), _M_start);
      uninitialized_copy(__x.begin() + size(), __x.end(), _M_finish);
    }
    _M_finish = _M_start + __xlen;
  }
  return *this;
}
```

##### vector::push_back
在vector的最后一个元素之后添加一个新元素。val的内容被复制（或移动）到新的元素。

这有效地将容器size增加了一个，如果新的矢量size超过了当前vector的capacity，则导致所分配的存储空间自动重新分配。
```cpp
void push_back (const value_type& val);
void push_back (value_type&& val);
```
Example
```cpp
#include <iostream>
#include <vector>

int main ()
{
  std::vector<int> myvector;
  int myint;

  std::cout << "Please enter some integers (enter 0 to end):\n";

  do {
    std::cin >> myint;
    myvector.push_back (myint);
  } while (myint);

  std::cout << "myvector stores " << int(myvector.size()) << " numbers.\n";

  return 0;
}
```
##### vector::pop_back
删除vector中的最后一个元素，有效地将容器size减少一个。

这破坏了被删除的元素。
```cpp
void pop_back();
```
Example
```cpp
#include <iostream>
#include <vector>

int main ()
{
  std::vector<int> myvector;
  int sum (0);
  myvector.push_back (100);
  myvector.push_back (200);
  myvector.push_back (300);

  while (!myvector.empty())
  {
    sum+=myvector.back();
    myvector.pop_back();
  }

  std::cout << "The elements of myvector add up to " << sum << '\n';

  return 0;
}
```
Output
```
The elements of myvector add up to 600
```
##### vector::insert
通过在指定位置的元素之前插入新元素来扩展该vector，通过插入元素的数量有效地增加容器大小。 这会导致分配的存储空间自动重新分配，只有在新的vector的size超过当前的vector的capacity的情况下。

由于vector使用数组作为其基础存储，因此除了将元素插入到vector末尾之后，或vector的begin之前，其他位置会导致容器重新定位位置之后的所有元素到他们的新位置。与其他种类的序列容器（例如list或forward_list）执行相同操作的操作相比，这通常是低效的操作。
```cpp
single element (1)
iterator insert (const_iterator position, const value_type& val);
fill (2)
iterator insert (const_iterator position, size_type n, const value_type& val);
range (3)
template <class InputIterator>
iterator insert (const_iterator position, InputIterator first, InputIterator last);
move (4)
iterator insert (const_iterator position, value_type&& val);
initializer list (5)
iterator insert (const_iterator position, initializer_list<value_type> il);
```
Example
```cpp
#include <iostream>
#include <vector>

int main ()
{
  std::vector<int> myvector (3,100);
  std::vector<int>::iterator it;

  it = myvector.begin();
  it = myvector.insert ( it , 200 );

  myvector.insert (it,2,300);

  // "it" no longer valid, get a new one:
  it = myvector.begin();

  std::vector<int> anothervector (2,400);
  myvector.insert (it+2,anothervector.begin(),anothervector.end());

  int myarray [] = { 501,502,503 };
  myvector.insert (myvector.begin(), myarray, myarray+3);

  std::cout << "myvector contains:";
  for (it=myvector.begin(); it<myvector.end(); it++)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
```
Output
```
myvector contains: 501 502 503 300 300 400 400 200 100 100 100
```

补充：insert 迭代器野指针错误：

```cpp
int main()
{
	std::vector<int> v(5, 0);
	std::vector<int>::iterator vi;

  // 获取vector第一个元素的迭代器
	vi = v.begin();

  // push_back 插入元素之后可能会因为 push_back 的骚操作（创建一个新vector把旧vector的值复制到新vector），导致vector迭代器iterator的指针变成野指针，而导致insert出错
	v.push_back(10);

	v.insert(vi, 2, 300);

	return 0;
}
```
改正：应该把`vi = v.begin();`放到`v.push_back(10);`后面
##### vector::erase
从vector中删除单个元素（position）或一系列元素（[first，last））。

这有效地减少了被去除的元素的数量，从而破坏了容器的大小。

由于vector使用一个数组作为其底层存储，所以删除除vector结束位置之后，或vector的begin之前的元素外，将导致容器将段被擦除后的所有元素重新定位到新的位置。与其他种类的序列容器（例如list或forward_list）执行相同操作的操作相比，这通常是低效的操作。
```cpp
iterator erase (const_iterator position);
iterator erase (const_iterator first, const_iterator last);
```
Example
```cpp
#include <iostream>
#include <vector>

int main ()
{
  std::vector<int> myvector;

  // set some values (from 1 to 10)
  for (int i=1; i<=10; i++) myvector.push_back(i);

  // erase the 6th element
  myvector.erase (myvector.begin()+5);

  // erase the first 3 elements:
  myvector.erase (myvector.begin(),myvector.begin()+3);

  std::cout << "myvector contains:";
  for (unsigned i=0; i<myvector.size(); ++i)
    std::cout << ' ' << myvector[i];
  std::cout << '\n';

  return 0;
}
```
Output
```
myvector contains: 4 5 7 8 9 10
```
##### vector::swap
通过x的内容交换容器的内容，x是另一个相同类型的vector对象。尺寸可能不同。

在调用这个成员函数之后，这个容器中的元素是那些在调用之前在x中的元素，而x的元素是在这个元素中的元素。所有迭代器，引用和指针对交换对象保持有效。

请注意，非成员函数存在具有相同名称的交换，并使用与此成员函数相似的优化来重载该算法。
```cpp
void swap (vector& x);
```
Example
```cpp
#include <iostream>
#include <vector>

int main ()
{
  std::vector<int> foo (3,100);   // three ints with a value of 100
  std::vector<int> bar (5,200);   // five ints with a value of 200

  foo.swap(bar);

  std::cout << "foo contains:";
  for (unsigned i=0; i<foo.size(); i++)
    std::cout << ' ' << foo[i];
  std::cout << '\n';

  std::cout << "bar contains:";
  for (unsigned i=0; i<bar.size(); i++)
    std::cout << ' ' << bar[i];
  std::cout << '\n';

  return 0;
}
```
Output
```
foo contains: 200 200 200 200 200
bar contains: 100 100 100
```
##### vector::clear
从vector中删除所有的元素（被销毁），留下size为0的容器。

不保证重新分配，并且由于调用此函数， vector的capacity不保证发生变化。强制重新分配的典型替代方法是使用swap：`vector<T>().swap(x);   // clear x reallocating `
```cpp
void clear() noexcept;
```
Example
```cpp
#include <iostream>
#include <vector>

void printVector(const std::vector<int> &v)
{
	for (auto it = v.begin(); it != v.end(); ++it)
	{
		std::cout << *it << ' ';
	}
	std::cout << std::endl;
}

int main()
{
	std::vector<int> v1(5, 50);

	printVector(v1);
	std::cout << "v1 size  = " << v1.size() << std::endl;
	std::cout << "v1 capacity  = " << v1.capacity() << std::endl;

	v1.clear();

	printVector(v1);
	std::cout << "v1 size  = " << v1.size() << std::endl;
	std::cout << "v1 capacity  = " << v1.capacity() << std::endl;

	v1.push_back(11);
	v1.push_back(22);

	printVector(v1);
	std::cout << "v1 size  = " << v1.size() << std::endl;
	std::cout << "v1 capacity  = " << v1.capacity() << std::endl;

	return 0;
}
```
Output
```
50 50 50 50 50
v1 size  = 5
v1 capacity  = 5

v1 size  = 0
v1 capacity  = 5
11 22
v1 size  = 2
v1 capacity  = 5
```
##### vector::emplace
通过在position位置处插入新元素args来扩展容器。这个新元素是用args作为构建的参数来构建的。

这有效地增加了一个容器的大小。

分配存储空间的自动重新分配发生在新的vector的size超过当前向量容量的情况下。

由于vector使用数组作为其基础存储，因此除了将元素插入到vector末尾之后，或vector的begin之前，其他位置会导致容器重新定位位置之后的所有元素到他们的新位置。与其他种类的序列容器（例如list或forward_list）执行相同操作的操作相比，这通常是低效的操作。

该元素是通过调用allocator_traits::construct来转换args来创建的。插入一个类似的成员函数，将现有对象复制或移动到容器中。

```cpp
template <class... Args>
iterator emplace (const_iterator position, Args&&... args);
```
Example
```cpp
#include <iostream>
#include <vector>

int main ()
{
  std::vector<int> myvector = {10,20,30};

  auto it = myvector.emplace ( myvector.begin()+1, 100 );
  myvector.emplace ( it, 200 );
  myvector.emplace ( myvector.end(), 300 );

  std::cout << "myvector contains:";
  for (auto& x: myvector)
    std::cout << ' ' << x;
  std::cout << '\n';

  return 0;
}
```
Output
```
myvector contains: 10 200 100 20 30 300
```
##### vector::emplace_back
在vector的末尾插入一个新的元素，紧跟在当前的最后一个元素之后。这个新元素是用args作为构造函数的参数来构造的。

这有效地将容器大小增加了一个，如果新的矢量大小超过了当前的vector容量，则导致所分配的存储空间自动重新分配。

该元素是通过调用allocator_traits :: construct来转换args来创建的。

与push\_back相比，emplace\_back可以避免额外的复制和移动操作。
```cpp
template <class... Args>
  void emplace_back (Args&&... args);
```

Example

```cpp
#include <vector>
#include <string>
#include <iostream>

struct President
{
    std::string name;
    std::string country;
    int year;

    President(std::string p_name, std::string p_country, int p_year)
        : name(std::move(p_name)), country(std::move(p_country)), year(p_year)
    {
        std::cout << "I am being constructed.\n";
    }
    President(President&& other)
        : name(std::move(other.name)), country(std::move(other.country)), year(other.year)
    {
        std::cout << "I am being moved.\n";
    }
    President& operator=(const President& other) = default;
};

int main()
{
    std::vector<President> elections;
    std::cout << "emplace_back:\n";
    elections.emplace_back("Nelson Mandela", "South Africa", 1994);

    std::vector<President> reElections;
    std::cout << "\npush_back:\n";
    reElections.push_back(President("Franklin Delano Roosevelt", "the USA", 1936));

    std::cout << "\nContents:\n";
    for (President const& president: elections) {
        std::cout << president.name << " was elected president of "
                  << president.country << " in " << president.year << ".\n";
    }
    for (President const& president: reElections) {
        std::cout << president.name << " was re-elected president of "
                  << president.country << " in " << president.year << ".\n";
    }
}
```

Output

```
emplace_back:
I am being constructed.

push_back:
I am being constructed.
I am being moved.

Contents:
Nelson Mandela was elected president of South Africa in 1994.
Franklin Delano Roosevelt was re-elected president of the USA in 1936.
```
##### vector::get_allocator
返回与vector关联的构造器对象的副本。
```cpp
allocator_type get_allocator() const noexcept;
```
Example
```cpp
#include <iostream>
#include <vector>

int main ()
{
  std::vector<int> myvector;
  int * p;
  unsigned int i;

  // allocate an array with space for 5 elements using vector's allocator:
  p = myvector.get_allocator().allocate(5);

  // construct values in-place on the array:
  for (i=0; i<5; i++) myvector.get_allocator().construct(&p[i],i);

  std::cout << "The allocated array contains:";
  for (i=0; i<5; i++) std::cout << ' ' << p[i];
  std::cout << '\n';

  // destroy and deallocate:
  for (i=0; i<5; i++) myvector.get_allocator().destroy(&p[i]);
  myvector.get_allocator().deallocate(p,5);

  return 0;
}
```
Output
```
The allocated array contains: 0 1 2 3 4
```

注意：deallocate和destory的关系：

deallocate实现的源码：

	template <class T>
	inline void _deallocate(T* buffer)
	{
		::operator delete(buffer);    //为什么不用 delete [] ?  ,operator delete 区别于 delete
		                             //operator delete  是一个底层操作符
	}

destory：

	template <class T>
	inline void _destory(T *ptr)
	{
		ptr->~T();
	}

destory负责调用类型的析构函数，销毁相应内存上的内容（但销毁后内存地址仍保留）

deallocate负责释放内存（此时相应内存中的值在此之前应调用destory销毁，将内存地址返回给系统，代表这部分地址使用引用-1）




##### relational operators (vector)
##### swap (vector)
##### vector <bool>

#### deque
deque（['dek]）（双端队列）是double-ended queue 的一个不规则缩写。deque是具有动态大小的序列容器，可以在两端（前端或后端）扩展或收缩。

特定的库可以以不同的方式实现deques，通常作为某种形式的动态数组。但是在任何情况下，它们都允许通过随机访问迭代器直接访问各个元素，通过根据需要扩展和收缩容器来自动处理存储。

因此，它们提供了类似于vector的功能，但是在序列的开始部分也可以高效地插入和删除元素，而不仅仅是在结尾。但是，与vector不同，deques并不保证将其所有元素存储在连续的存储位置：deque通过偏移指向另一个元素的指针访问元素会导致未定义的行为。

两个vector和deques提供了一个非常相似的接口，可以用于类似的目的，但内部工作方式完全不同：虽然vector使用单个数组需要偶尔重新分配以增长，但是deque的元素可以分散在不同的块的容器，容器在内部保存必要的信息以提供对其任何元素的持续时间和统一的顺序接口（通过迭代器）的直接访问。因此，deques在内部比vector更复杂一点，但是这使得他们在某些情况下更有效地增长，尤其是在重新分配变得更加昂贵的很长序列的情况下。

对于频繁插入或删除开始或结束位置以外的元素的操作，deques表现得更差，并且与列表和转发列表相比，迭代器和引用的一致性更低。

deque上常见操作的复杂性（效率）如下：

* 随机访问 - 常数O(1)
* 在结尾或开头插入或移除元素 - 摊销不变O(1)
* 插入或移除元素 - 线性O(n)
```cpp
template < class T, class Alloc = allocator<T> > class deque;
```
![]
![]

##### deque::deque

构造一个deque容器对象，根据所使用的构造函数版本初始化它的内容：

Example
```cpp
#include <iostream>
#include <deque>

int main ()
{
  unsigned int i;

  // constructors used in the same order as described above:
  std::deque<int> first;                                // empty deque of ints
  std::deque<int> second (4,100);                       // four ints with value 100
  std::deque<int> third (second.begin(),second.end());  // iterating through second
  std::deque<int> fourth (third);                       // a copy of third

  // the iterator constructor can be used to copy arrays:
  int myints[] = {16,2,77,29};
  std::deque<int> fifth (myints, myints + sizeof(myints) / sizeof(int) );

  std::cout << "The contents of fifth are:";
  for (std::deque<int>::iterator it = fifth.begin(); it!=fifth.end(); ++it)
    std::cout << ' ' << *it;

  std::cout << '\n';

  return 0;
}
```
Output
```
The contents of fifth are: 16 2 77 29
```
##### deque::push_back
在当前的最后一个元素之后 ，在deque容器的末尾添加一个新元素。val的内容被复制（或移动）到新的元素。

这有效地增加了一个容器的大小。
```cpp
void push_back (const value_type& val);
void push_back (value_type&& val);
```
Example
```cpp
#include <iostream>
#include <deque>

int main ()
{
  std::deque<int> mydeque;
  int myint;

  std::cout << "Please enter some integers (enter 0 to end):\n";

  do {
    std::cin >> myint;
    mydeque.push_back (myint);
  } while (myint);

  std::cout << "mydeque stores " << (int) mydeque.size() << " numbers.\n";

  return 0;
}
```
##### deque::push_front
在deque容器的开始位置插入一个新的元素，位于当前的第一个元素之前。val的内容被复制（或移动）到插入的元素。

这有效地增加了一个容器的大小。
```cpp
void push_front (const value_type& val);
void push_front (value_type&& val);
```
Example
```cpp
#include <iostream>
#include <deque>

int main ()
{
  std::deque<int> mydeque (2,100);     // two ints with a value of 100
  mydeque.push_front (200);
  mydeque.push_front (300);

  std::cout << "mydeque contains:";
  for (std::deque<int>::iterator it = mydeque.begin(); it != mydeque.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
```
Output
```
300 200 100 100
```
##### deque::pop_back
删除deque容器中的最后一个元素，有效地将容器大小减少一个。

这破坏了被删除的元素。

```cpp
void pop_back();
```
Example
```cpp
#include <iostream>
#include <deque>

int main ()
{
  std::deque<int> mydeque;
  int sum (0);
  mydeque.push_back (10);
  mydeque.push_back (20);
  mydeque.push_back (30);

  while (!mydeque.empty())
  {
    sum+=mydeque.back();
    mydeque.pop_back();
  }

  std::cout << "The elements of mydeque add up to " << sum << '\n';

  return 0;
}
```
Output
```
The elements of mydeque add up to 60
```
##### deque::pop_front
删除deque容器中的第一个元素，有效地减小其大小。

这破坏了被删除的元素。
```cpp
void pop_front();
```
Example
```cpp
#include <iostream>
#include <deque>

int main ()
{
  std::deque<int> mydeque;

  mydeque.push_back (100);
  mydeque.push_back (200);
  mydeque.push_back (300);

  std::cout << "Popping out the elements in mydeque:";
  while (!mydeque.empty())
  {
    std::cout << ' ' << mydeque.front();
    mydeque.pop_front();
  }

  std::cout << "\nThe final size of mydeque is " << int(mydeque.size()) << '\n';

  return 0;
}
```
Output
```
Popping out the elements in mydeque: 100 200 300
The final size of mydeque is 0
```
##### deque::emplace_front
在deque的开头插入一个新的元素，就在其当前的第一个元素之前。这个新的元素是用args作为构建的参数来构建的。

这有效地增加了一个容器的大小。

该元素是通过调用allocator_traits::construct来转换args来创建的。

存在一个类似的成员函数push_front，它可以将现有对象复制或移动到容器中。
```cpp
template <class... Args>
  void emplace_front (Args&&... args);
```
Example
```cpp
#include <iostream>
#include <deque>

int main ()
{
  std::deque<int> mydeque = {10,20,30};

  mydeque.emplace_front (111);
  mydeque.emplace_front (222);

  std::cout << "mydeque contains:";
  for (auto& x: mydeque)
    std::cout << ' ' << x;
  std::cout << '\n';

  return 0;
}
```
Output
```
mydeque contains: 222 111 10 20 30
```
##### deque::emplace_back
在deque的末尾插入一个新的元素，紧跟在当前的最后一个元素之后。这个新的元素是用args作为构建的参数来构建的。

这有效地增加了一个容器的大小。

该元素是通过调用allocator_traits::construct来转换args来创建的。

存在一个类似的成员函数push_back，它可以将现有对象复制或移动到容器中
```cpp
template <class... Args>
  void emplace_back (Args&&... args);
```
Example
```cpp
#include <iostream>
#include <deque>

int main ()
{
  std::deque<int> mydeque = {10,20,30};

  mydeque.emplace_back (100);
  mydeque.emplace_back (200);

  std::cout << "mydeque contains:";
  for (auto& x: mydeque)
    std::cout << ' ' << x;
  std::cout << '\n';

  return 0;
}
```
Output
```
mydeque contains: 10 20 30 100 200
```
#### forward\_list

forward_list（单向链表）是序列容器，允许在序列中的任何地方进行恒定的时间插入和擦除操作。

forward\_list（单向链表）被实现为单链表; 单链表可以将它们包含的每个元素存储在不同和不相关的存储位置中。通过关联到序列中下一个元素的链接的每个元素来保留排序。forward\_list容器和列表

之间的主要设计区别容器是第一个内部只保留一个到下一个元素的链接，而后者每个元素保留两个链接：一个指向下一个元素，一个指向前一个元素，允许在两个方向上有效的迭代，但是每个元素消耗额外的存储空间并且插入和移除元件的时间开销略高。因此，forward_list对象比列表对象更有效率，尽管它们只能向前迭代。

与其他基本的标准序列容器（array，vector和deque），forward_list通常在插入，提取和移动容器内任何位置的元素方面效果更好，因此也适用于密集使用这些元素的算法，如排序算法。

的主要缺点修饰符Modifiers S和列表相比这些其它序列容器s是说，他们缺乏可以通过位置的元素的直接访问; 例如，要访问forward_list中的第六个元素，必须从开始位置迭代到该位置，这需要在这些位置之间的线性时间。它们还消耗一些额外的内存来保持与每个元素相关联的链接信息（这可能是大型小元素列表的重要因素）。

该修饰符Modifiersclass模板的设计考虑到效率：按照设计，它与简单的手写C型单链表一样高效，实际上是唯一的标准容器，为了效率的考虑故意缺少尺寸成员函数：由于其性质作为一个链表，具有一个需要一定时间的大小的成员将需要它保持一个内部计数器的大小（如列表所示）。这会消耗一些额外的存储空间，并使插入和删除操作效率稍低。要获取forward_list对象的大小，可以使用距离算法的开始和结束，这是一个需要线性时间的操作。

![]

##### forward\_list::forward\_list

```cpp
default (1)
explicit forward_list (const allocator_type& alloc = allocator_type());
fill (2)
explicit forward_list (size_type n);
explicit forward_list (size_type n, const value_type& val,
                        const allocator_type& alloc = allocator_type());
range (3)
template <class InputIterator>
  forward_list (InputIterator first, InputIterator last,
                const allocator_type& alloc = allocator_type());
copy (4)
forward_list (const forward_list& fwdlst);
forward_list (const forward_list& fwdlst, const allocator_type& alloc);
move (5)
forward_list (forward_list&& fwdlst);
forward_list (forward_list&& fwdlst, const allocator_type& alloc);
initializer list (6)
forward_list (initializer_list<value_type> il,
              const allocator_type& alloc = allocator_type());
```
Example
```cpp
#include <iostream>
#include <forward_list>

int main ()
{
  // constructors used in the same order as described above:

  std::forward_list<int> first;                      // default: empty
  std::forward_list<int> second (3,77);              // fill: 3 seventy-sevens
  std::forward_list<int> third (second.begin(), second.end()); // range initialization
  std::forward_list<int> fourth (third);            // copy constructor
  std::forward_list<int> fifth (std::move(fourth));  // move ctor. (fourth wasted)
  std::forward_list<int> sixth = {3, 52, 25, 90};    // initializer_list constructor

  std::cout << "first:" ; for (int& x: first)  std::cout << " " << x; std::cout << '\n';
  std::cout << "second:"; for (int& x: second) std::cout << " " << x; std::cout << '\n';
  std::cout << "third:";  for (int& x: third)  std::cout << " " << x; std::cout << '\n';
  std::cout << "fourth:"; for (int& x: fourth) std::cout << " " << x; std::cout << '\n';
  std::cout << "fifth:";  for (int& x: fifth)  std::cout << " " << x; std::cout << '\n';
  std::cout << "sixth:";  for (int& x: sixth)  std::cout << " " << x; std::cout << '\n';

  return 0;
}
```
Possible output
```
forward_list constructor examples:
first:
second: 77 77 77
third: 77 77 77
fourth:
fifth: 77 77 77
sixth: 3 52 25 90
```

##### forward\_list::~forward\_list

##### forward\_list::before\_begin
返回指向容器中第一个元素之前的位置的迭代器。

返回的迭代器不应被解除引用：它是为了用作成员函数的参数emplace\_after，insert\_after，erase\_after或splice\_after，指定序列，其中执行该动作的位置的开始位置。

```cpp
      iterator before_begin() noexcept;
const_iterator before_begin() const noexcept;
```
Example
```cpp
#include <iostream>
#include <forward_list>

int main ()
{
  std::forward_list<int> mylist = {20, 30, 40, 50};

  mylist.insert_after ( mylist.before_begin(), 11 );

  std::cout << "mylist contains:";
  for ( int& x: mylist ) std::cout << ' ' << x;
  std::cout << '\n';

  return 0;
}
```
Output
```
mylist contains: 11 20 30 40 50
```
##### forward\_list::cbefore\_begin
返回指向容器中第一个元素之前的位置的const_iterator。

一个常量性是指向常量内容的迭代器。这个迭代器可以增加和减少（除非它本身也是const），就像forward\_list::before\_begin返回的迭代器一样，但不能用来修改它指向的内容。

返回的价值不得解除引用。
```cpp
const_iterator cbefore_begin() const noexcept;
```
Example
```cpp
#include <iostream>
#include <forward_list>

int main ()
{
  std::forward_list<int> mylist = {77, 2, 16};

  mylist.insert_after ( mylist.cbefore_begin(), 19 );

  std::cout << "mylist contains:";
  for ( int& x: mylist ) std::cout << ' ' << x;
  std::cout << '\n';

  return 0;
}
```
Output
```
mylist contains: 19 77 2 16
```

#### list
list是一个双向链表容器，它存储元素的顺序由元素被插入的顺序决定，而不是由元素的值决定。list 不对元素进行排序，也不提供基于键的快速访问。

##### list::front,list::back
返回第一个与最后一个元素的引用，如果容器为空，则调用该函数会导致未定义行为。
```cpp
reference front();
const_reference front() const;
reference back();
const_reference back() const;
```
Example
```cpp
#include <iostream>
#include <list>

int main()
{
  std::list<int> mylist = {20, 30, 40, 50};
  mylist.front() = 10;
  mylist.back() = 99;
  std::cout << "mylist contains:";
  for (auto& x : mylist) std::cout << ' ' << x;
  std::cout << '\n';
  return 0;
}
```
Output
```cpp
mylist contains: 10 30 40 99
```

##### list::unique
移除容器中所有连续重复的元素，只保留一个。如果提供了比较函数，则使用该函数来判断元素是否相等。否则使用默认的比较函数operator==。
```cpp
template <class BinaryPredicate>
void unique(BinaryPredicate pred);
```
Example
```cpp
#include <iostream>
#include <list>
int main()
{
  std::list<int> mylist = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};
  mylist.unique();
  std::cout << "mylist contains:";
  for (int& x : mylist)
  std::cout << ' ' << x;
  return 0;
}
```
Output
```cpp
mylist contains: 1 2 3 4
```

##### list::merge
将两个已排序的list合并为一个有序的list。如果this和other中的元素都按非递减顺序排序，则merge会将other中的所有元素合并到this中，合并后的this仍然保持非递减顺序,other清空元素。如果this和other指向同一个list，则行为未定义。
```cpp
template <class Compare>
void merge(std::list<T, Allocator>& other, Compare comp);
```
Example
```cpp
#include <iostream>
#include <list>
int main()
{
  std::list<int> first = {1, 3, 5, 7};
  std::list<int> second = {2, 4, 6, 8};
  first.merge(second);
  std::cout << "first contains:";
  for (int& x : first) std::cout << ' ' << x;
  std::cout << '\n';
  std::cout << "second contains:";
  for (int& x : second) std::cout << ' ' << x;
  return 0;
}
```
Output
```cpp
first contains: 1 2 3 4 5 6 7 8
second contains:
```

##### list::splice
移除other中指定区间的元素，并插入到this中。
```cpp
void splice(iterator it, list& x, iterator first);
```
Example
```cpp
#include <iostream>
#include <list>
int main() {
    std::list<int> list1 = {1, 3, 5, 7};
    std::list<int> list2 = {2, 4, 6, 8};
    auto it = list1.begin();
    std::advance(it, 2);//Not support + for iterator
    auto first = list2.begin();
    std::advance(first, 1);
    list1.splice(it, list2, first);
    for (int value : list1) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    for (int value : list2) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    return 0;
}
```
Output:
```cpp
1 3 4 5 7
2 6 8
```

##### list::reverse
反转容器中的元素。
```cpp
void reverse() noexcept;
```
Example:
```cpp
#include <iostream>
#include <list>
int main()
{
  std::list<int> mylist = {1, 2, 3, 4, 5};
  mylist.reverse();
  std::cout << "mylist contains:";
  for (int& x : mylist) std::cout << ' ' << x;
  return 0;
}
```
Output:
```cpp
mylist contains:1 2 3 4 5
```
#### stack
后进先出（LIFO）,基于底层容器（deque或list或vector）实现。
##### stack::emplace
在栈顶构造一个新元素,减少了不必要的对象构造和销毁.
```cpp
template <class... Args>
void emplace(Args&&... args);
```
Example:
```cpp
#include <iostream>
#include <stack>
int main()
{
  std::stack<int> mystack;
  mystack.emplace(10);
  mystack.emplace(20);
  std::cout << "mystack contains:";
  while (!mystack.empty())
  {
    std::cout << ' ' << mystack.top();
    mystack.pop();
  }
  return 0;
}
```
Output:
```cpp
mystack contains: 20 10
```

##### stack::push_range(c++23)
将范围中的每个元素插入到栈的末尾
```cpp
template <class InputIterator>
void push_range(InputIterator first, InputIterator last);
```
Example:
```cpp
#include <iostream>
#include <stack>
#include <vector>
int main() {
    std::stack<int> st;
    std::vector<int> vec = {1, 2, 3, 4};
    st.push_range(vec);
    while (!st.empty()) {
        std::cout << st.top() <<" ";
        st.pop();
    }
}
```
Output:
```cpp
4 3 2 1
```

#### queue

先进先出（FIFO）,基于底层容器（deque或list）实现。

#### priority_queue

提供了一种基于优先级的队列，通常基于堆实现，默认使用最大堆。
##### lambda自定义函数比较
```cpp
#include <iostream>
#include <queue>
#include <vector>
int main() {
    std::priority_queue<int> maxHeap;
    maxHeap.push(10);
    maxHeap.push(20);
    maxHeap.push(15);
    std::cout << "Top element (max): " << maxHeap.top() << std::endl;
    maxHeap.pop();
    std::cout << "Top element after pop: " << maxHeap.top()<<std::endl;
    auto cmp = [](int a, int b) { return a > b; };//lambda
    std::priority_queue<int, std::vector<int>, decltype(cmp)> minHeap(cmp);
    minHeap.push(10);
    minHeap.push(20);
    minHeap.push(15);
    std::cout << "Top element (min): " << minHeap.top() << std::endl;
    return 0;
}
```
Output:
```cpp
Top element (max): 20
Top element after pop: 15
Top element (min): 10
```

#### set
存储唯一的元素，并且这些元素会自动按照特定的顺序排列.
##### set::upper_bound
返回指向第一个大于指定值的元素的迭代器。
```cpp
template <class K>
iterator upper_bound(const K& x);
template <class K>
const_iterator upper_bound(const K& x) const;
```
Example:
```cpp
#include <iostream>
#include <set>
#include <string>
int main() {
    std::set<std::string> mySet = {"apple", "banana", "cherry", "date"};
    auto it = mySet.upper_bound("banana");
    if (it != mySet.end()) {
        std::cout << "The first element greater than 'banana' is: " << *it;
    } else {
        std::cout << "No element greater than 'banana' found.";
    }
    return 0;
}
```
Output:
```cpp
The first element greater than 'banana' is: cherry
```
##### set::contains
判断元素是否存在。
```cpp
bool contains(const key_type& key) const;
```
Example:
```cpp
#include <iostream>
#include <set>
int main() {
    std::set<int> mySet = {1, 2, 3, 4, 5};
    int valueToFind = 3;
    if (mySet.contains(valueToFind)) {
        std::cout << valueToFind << " is in the set.";
    } else {
        std::cout << valueToFind << " is not in the set.";
    }
    return 0;
}
```
Output:
```cpp
3 is in the set.
```
##### set::value_comp
获取一个函数对象，用于比较两个键值。
```cpp
key_compare value_comp() const;
```
Example:
```cpp
#include <iostream>
#include <set>
int main() {
    std::set<int> mySet = {5, 3, 9, 1, 7};
    auto comp = mySet.value_comp();
    int a = 3;
    int b = 5;
    if (comp(a, b)) {
        std::cout << a << " is less than " << b << " according to the set's comparison function.";
    } else {
        std::cout << a << " is not less than " << b << " according to the set's comparison function.";
    }
    return 0;
}
```
Output:
```cpp
3 is less than 5 according to the set's comparison function.
```
##### set::extract
从set中提取指定的元素节点，并返回一个节点句柄。
```cpp
template<class K>
node_type extract(K&& x);
```
Example:
```cpp
#include <iostream>
#include <set>
int main() {
    std::set<int> mySet = {1, 2, 3, 4, 5};
    auto nh = mySet.extract(3);
    if (!nh.empty()) {
        std::cout << "Extracted element: " << nh.value();
    } else {
        std::cout << "Element not found.";
    }
    std::cout<<std::endl;
    nh.value() = 10;
    mySet.insert(std::move(nh));
    for (const auto& elem : mySet) {
        std::cout << elem << " ";
    }
    return 0;
}
```
Output:
```cpp
Extracted element: 3
1 2 4 5 10
```
#### multiset
在set的基础上，允许重复元素。
##### multiset::equal_range
返回一个范围pair，该范围包含所有满足条件的元素。
Example:
```cpp
#include <iostream>
#include <set>
int main() {
    std::multiset<int> ms = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};
    auto range = ms.equal_range(3);
    for (auto it = range.first; it != range.second; ++it) {
        std::cout << *it << " ";
    }
    return 0;
}
```
Output:
```cpp
3 3 3
```
##### multset::find
返回一个迭代器，指向给定键的元素。如果找不到给定键，则返回一个指向 multiset 的末尾的迭代器。
Example:
```cpp
#include <iostream>
#include <set>
int main() {
    std::multiset<int> myMultiset = {1, 2, 2, 3, 4, 4, 4};
    auto it = myMultiset.find(2);
    if (it != myMultiset.end()) {
        std::cout << "Found element with value " << *it << std::endl;
    } else {
        std::cout << "Element not found" << std::endl;
    }
    return 0;
}
```
Output:
```cpp
Found element with value 2
```
##### multiset::emplace_hint
通过提供一个位置，帮助优化插入操作的性能。
```cpp
template <class... Args>
iterator emplace_hint(const_iterator hint, Args&&... args);
```
Example:
```cpp
#include <iostream>
#include <set>
int main() {
    std::multiset<int> myMultiset{1, 2, 4, 5};
    auto it = myMultiset.find(2);
    myMultiset.emplace_hint(it, 3);//Given position of 2
    for (const auto& elem : myMultiset) {
        std::cout << elem << " ";
    }
    return 0;
}
```
Output:
```cpp
1 2 3 4 5
```
#### map

map 是关联容器，按照特定顺序存储由 key value (键值) 和 mapped value (映射值) 组合形成的元素。

在映射中，键值通常用于对元素进行排序和唯一标识，而映射的值存储与此键关联的内容。该类型的键和映射的值可能不同，并且在部件类型被分组在一起VALUE_TYPE，这是一种对类型结合两种：

```cpp
typedef pair<const Key, T> value_type;
```

在内部，映射中的元素总是按照由其内部比较对象（比较类型）指示的特定的严格弱排序标准按键排序。映射容器通常比unordered_map容器慢，以通过它们的键来访问各个元素，但是它们允许基于它们的顺序对子集进行直接迭代。 在该映射值地图可以直接通过使用其相应的键来访问括号运算符（（操作符[] ）。 映射通常如实施

```cpp
template < class Key,                                     // map::key_type
           class T,                                       // map::mapped_type
           class Compare = less<Key>,                     // map::key_compare
           class Alloc = allocator<pair<const Key,T> >    // map::allocator_type
           > class map;
```

##### map::map
构造一个映射容器对象，根据所使用的构造器版本初始化其内容：

（1）空容器构造函数（默认构造函数）

构造一个空的容器，没有元素。

（2）范围构造函数

构造具有一样多的元素的范围内的容器[第一，最后一个），其中每个元件布设构造的从在该范围内它的相应的元件。

（3）复制构造函数（并用分配器复制）

使用x中的每个元素的副本构造一个容器。

（4）移动构造函数（并与分配器一起移动）

构造一个获取x元素的容器。
如果指定了alloc并且与x的分配器不同，那么元素将被移动。否则，没有构建元素（他们的所有权直接转移）。
x保持未指定但有效的状态。

（5）初始化列表构造函数

用il中的每个元素的副本构造一个容器。

```cpp
empty (1)
explicit map (const key_compare& comp = key_compare(),
              const allocator_type& alloc = allocator_type());
explicit map (const allocator_type& alloc);
range (2)
template <class InputIterator>
  map (InputIterator first, InputIterator last,
       const key_compare& comp = key_compare(),
       const allocator_type& = allocator_type());
copy (3)
map (const map& x);
map (const map& x, const allocator_type& alloc);
move (4)
map (map&& x);
map (map&& x, const allocator_type& alloc);
initializer list (5)
map (initializer_list<value_type> il,
     const key_compare& comp = key_compare(),
     const allocator_type& alloc = allocator_type());
```
Example
```cpp
#include <iostream>
#include <map>

bool fncomp (char lhs, char rhs) {return lhs<rhs;}

struct classcomp {
  bool operator() (const char& lhs, const char& rhs) const
  {return lhs<rhs;}
};

int main ()
{
  std::map<char,int> first;

  first['a']=10;
  first['b']=30;
  first['c']=50;
  first['d']=70;

  std::map<char,int> second (first.begin(),first.end());

  std::map<char,int> third (second);

  std::map<char,int,classcomp> fourth;                 // class as Compare

  bool(*fn_pt)(char,char) = fncomp;
  std::map<char,int,bool(*)(char,char)> fifth (fn_pt); // function pointer as Compare

  return 0;
}
```
##### map::begin
返回引用map容器中第一个元素的迭代器。

由于map容器始终保持其元素的顺序，所以开始指向遵循容器排序标准的元素。

如果容器是空的，则返回的迭代器值不应被解除引用。
```cpp
      iterator begin() noexcept;
const_iterator begin() const noexcept;
```
Example
```cpp
#include <iostream>
#include <map>

int main ()
{
  std::map<char,int> mymap;

  mymap['b'] = 100;
  mymap['a'] = 200;
  mymap['c'] = 300;

  // show content:
  for (std::map<char,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  return 0;
}
```
Output
```
a => 200
b => 100
c => 300
```

##### map::key_comp
返回容器用于比较键的比较对象的副本。

```cpp
key_compare key_comp() const;
```
Example
```cpp
#include <iostream>
#include <map>

int main ()
{
  std::map<char,int> mymap;

  std::map<char,int>::key_compare mycomp = mymap.key_comp();

  mymap['a']=100;
  mymap['b']=200;
  mymap['c']=300;

  std::cout << "mymap contains:\n";

  char highest = mymap.rbegin()->first;     // key value of last element

  std::map<char,int>::iterator it = mymap.begin();
  do {
    std::cout << it->first << " => " << it->second << '\n';
  } while ( mycomp((*it++).first, highest) );

  std::cout << '\n';

  return 0;
}
```
Output
```
mymap contains:
a => 100
b => 200
c => 300
```
##### map::value_comp
返回可用于比较两个元素的比较对象，以获取第一个元素的键是否在第二个元素之前。

```cpp
value_compare value_comp() const;
```
Example
```cpp
#include <iostream>
#include <map>

int main ()
{
  std::map<char,int> mymap;

  mymap['x']=1001;
  mymap['y']=2002;
  mymap['z']=3003;

  std::cout << "mymap contains:\n";

  std::pair<char,int> highest = *mymap.rbegin();          // last element

  std::map<char,int>::iterator it = mymap.begin();
  do {
    std::cout << it->first << " => " << it->second << '\n';
  } while ( mymap.value_comp()(*it++, highest) );

  return 0;
}
```
Output
```
mymap contains:
x => 1001
y => 2002
z => 3003
```
##### map::find
在容器中搜索具有等于k的键的元素，如果找到则返回一个迭代器，否则返回map::end的迭代器。

如果容器的比较对象自反地返回假（即，不管元素作为参数传递的顺序），则两个key被认为是等同的。

另一个成员函数map::count可以用来检查一个特定的键是否存在。
```cpp
      iterator find (const key_type& k);
const_iterator find (const key_type& k) const;
```
Example
```cpp
#include <iostream>
#include <map>

int main ()
{
  std::map<char,int> mymap;
  std::map<char,int>::iterator it;

  mymap['a']=50;
  mymap['b']=100;
  mymap['c']=150;
  mymap['d']=200;

  it = mymap.find('b');
  if (it != mymap.end())
    mymap.erase (it);

  // print content:
  std::cout << "elements in mymap:" << '\n';
  std::cout << "a => " << mymap.find('a')->second << '\n';
  std::cout << "c => " << mymap.find('c')->second << '\n';
  std::cout << "d => " << mymap.find('d')->second << '\n';

  return 0;
}
```
Output
```
elements in mymap:
a => 50
c => 150
d => 200
```
##### map::count
在容器中搜索具有等于k的键的元素，并返回匹配的数量。

由于地图容器中的所有元素都是唯一的，因此该函数只能返回1（如果找到该元素）或返回零（否则）。

如果容器的比较对象自反地返回错误（即，不管按键作为参数传递的顺序），则两个键被认为是等同的。
```cpp
size_type count (const key_type& k) const;
```
Example
```cpp
#include <iostream>
#include <map>

int main ()
{
  std::map<char,int> mymap;
  char c;

  mymap ['a']=101;
  mymap ['c']=202;
  mymap ['f']=303;

  for (c='a'; c<'h'; c++)
  {
    std::cout << c;
    if (mymap.count(c)>0)
      std::cout << " is an element of mymap.\n";
    else
      std::cout << " is not an element of mymap.\n";
  }

  return 0;
}
```
Output
```
a is an element of mymap.
b is not an element of mymap.
c is an element of mymap.
d is not an element of mymap.
e is not an element of mymap.
f is an element of mymap.
g is not an element of mymap.
```

##### map::lower_bound
将迭代器返回到下限

返回指向容器中第一个元素的迭代器，该元素的键不会在k之前出现（即，它是等价的或者在其后）。

该函数使用其内部比较对象（key\_comp）来确定这一点，将迭代器返回到key\_comp（element\_key，k）将返回false的第一个元素。

如果map类用默认的比较类型（less）实例化，则函数返回一个迭代器到第一个元素，其键不小于k。

一个类似的成员函数upper\_bound具有相同的行为lower\_bound，除非映射包含一个key值等于k的元素：在这种情况下，lower\_bound返回指向该元素的迭代器，而upper\_bound返回指向下一个元素的迭代器。
```cpp
      iterator lower_bound (const key_type& k);
const_iterator lower_bound (const key_type& k) const;
```
Example
```cpp
#include <iostream>
#include <map>

int main ()
{
  std::map<char,int> mymap;
  std::map<char,int>::iterator itlow,itup;

  mymap['a']=20;
  mymap['b']=40;
  mymap['c']=60;
  mymap['d']=80;
  mymap['e']=100;

  itlow=mymap.lower_bound ('b');  // itlow points to b
  itup=mymap.upper_bound ('d');   // itup points to e (not d!)

  mymap.erase(itlow,itup);        // erases [itlow,itup)

  // print content:
  for (std::map<char,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  return 0;
}
```
Output
```
a => 20
e => 100
```
##### map::upper_bound

将迭代器返回到上限

返回一个指向容器中第一个元素的迭代器，它的关键字被认为是在k之后。

该函数使用其内部比较对象（key\_comp）来确定这一点，将迭代器返回到key\_comp（k，element\_key）将返回true的第一个元素。

如果map类用默认的比较类型（less）实例化，则函数返回一个迭代器到第一个元素，其键大于k。

类似的成员函数lower\_bound具有与upper\_bound相同的行为，除了map包含一个元素，其键值等于k：在这种情况下，lower\_bound返回指向该元素的迭代器，而upper\_bound返回指向下一个元素的迭代器。

```cpp
      iterator upper_bound (const key_type& k);
const_iterator upper_bound (const key_type& k) const;
```
Example
```cpp
#include <iostream>
#include <map>

int main ()
{
  std::map<char,int> mymap;
  std::map<char,int>::iterator itlow,itup;

  mymap['a']=20;
  mymap['b']=40;
  mymap['c']=60;
  mymap['d']=80;
  mymap['e']=100;

  itlow=mymap.lower_bound ('b');  // itlow points to b
  itup=mymap.upper_bound ('d');   // itup points to e (not d!)

  mymap.erase(itlow,itup);        // erases [itlow,itup)

  // print content:
  for (std::map<char,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
    std::cout << it->first << " => " << it->second << '\n';

  return 0;
}
```
Output
```
a => 20
e => 100
```

##### map::equal_range

获取相同元素的范围

返回包含容器中所有具有与k等价的键的元素的范围边界。 由于地图容器中的元素具有唯一键，所以返回的范围最多只包含一个元素。

如果没有找到匹配，则返回的范围具有零的长度，与两个迭代器指向具有考虑去后一个密钥对所述第一元件ķ根据容器的内部比较对象（key\_comp）。如果容器的比较对象返回false，则两个键被认为是等价的。


```cpp
pair<const_iterator,const_iterator> equal_range (const key_type& k) const;
pair<iterator,iterator>             equal_range (const key_type& k);
```
Example
```cpp
#include <iostream>
#include <map>

int main ()
{
  std::map<char,int> mymap;

  mymap['a']=10;
  mymap['b']=20;
  mymap['c']=30;

  std::pair<std::map<char,int>::iterator,std::map<char,int>::iterator> ret;
  ret = mymap.equal_range('b');

  std::cout << "lower bound points to: ";
  std::cout << ret.first->first << " => " << ret.first->second << '\n';

  std::cout << "upper bound points to: ";
  std::cout << ret.second->first << " => " << ret.second->second << '\n';

  return 0;
}
```
Output
```
lower bound points to: 'b' => 20
upper bound points to: 'c' => 30
```

#### multimap
在map的基础上，允许重复元素。

#### tuple

元组是一个能够容纳元素集合的对象。每个元素可以是不同的类型。

```cpp
template <class... Types> class tuple;
```

Example
```cpp
#include <iostream>     // std::cout
#include <tuple>        // std::tuple, std::get, std::tie, std::ignore

int main ()
{
  std::tuple<int,char> foo (10,'x');
  auto bar = std::make_tuple ("test", 3.1, 14, 'y');

  std::get<2>(bar) = 100;                                    // access element

  int myint; char mychar;

  std::tie (myint, mychar) = foo;                            // unpack elements
  std::tie (std::ignore, std::ignore, myint, mychar) = bar;  // unpack (with ignore)

  mychar = std::get<3>(bar);

  std::get<0>(foo) = std::get<2>(bar);
  std::get<1>(foo) = mychar;

  std::cout << "foo contains: ";
  std::cout << std::get<0>(foo) << ' ';
  std::cout << std::get<1>(foo) << '\n';

  return 0;
}
```
Output
```
foo contains: 100 y
```
##### tuple::tuple
构建一个 tuple（元组）对象。

这涉及单独构建其元素，初始化取决于调用的构造函数形式：

（1）默认的构造函数

构建一个 元组对象的元素值初始化。

（2）复制/移动构造函数

该对象使用tpl的内容进行初始化 元组目的。tpl
的相应元素被传递给每个元素的构造函数。

（3）隐式转换构造函数

同上。tpl中的
所有类型都可以隐含地转换为构造中它们各自元素的类型元组 目的。

（4）初始化构造函数
用elems中的相应元素初始化每个元素。elems
的相应元素被传递给每个元素的构造函数。

（5）对转换构造函数

该对象有两个对应于pr.first和的元素pr.second。PR中的所有类型都应该隐含地转换为其中各自元素的类型元组 目的。

（6）分配器版本

和上面的版本一样，除了每个元素都是使用allocator alloc构造的。

```cpp
default (1)
constexpr tuple();
copy / move (2)
tuple (const tuple& tpl) = default;
tuple (tuple&& tpl) = default;
implicit conversion (3)
template <class... UTypes>
  tuple (const tuple<UTypes...>& tpl);
template <class... UTypes>
  tuple (tuple<UTypes...>&& tpl);
initialization (4)
explicit tuple (const Types&... elems);
template <class... UTypes>
  explicit tuple (UTypes&&... elems);
conversion from pair (5)
template <class U1, class U2>
  tuple (const pair<U1,U2>& pr);
template <class U1, class U2>
  tuple (pair<U1,U2>&& pr);
allocator (6)
template<class Alloc>
  tuple (allocator_arg_t aa, const Alloc& alloc);
template<class Alloc>
  tuple (allocator_arg_t aa, const Alloc& alloc, const tuple& tpl);
template<class Alloc>
  tuple (allocator_arg_t aa, const Alloc& alloc, tuple&& tpl);
template<class Alloc,class... UTypes>
  tuple (allocator_arg_t aa, const Alloc& alloc, const tuple<UTypes...>& tpl);
template<class Alloc, class... UTypes>
  tuple (allocator_arg_t aa, const Alloc& alloc, tuple<UTypes...>&& tpl);
template<class Alloc>
  tuple (allocator_arg_t aa, const Alloc& alloc, const Types&... elems);
template<class Alloc, class... UTypes>
  tuple (allocator_arg_t aa, const Alloc& alloc, UTypes&&... elems);
template<class Alloc, class U1, class U2>
  tuple (allocator_arg_t aa, const Alloc& alloc, const pair<U1,U2>& pr);
template<class Alloc, class U1, class U2>
  tuple (allocator_arg_t aa, const Alloc& alloc, pair<U1,U2>&& pr);
```
Example
```cpp
#include <iostream>     // std::cout
#include <utility>      // std::make_pair
#include <tuple>        // std::tuple, std::make_tuple, std::get

int main ()
{
  std::tuple<int,char> first;                             // default
  std::tuple<int,char> second (first);                    // copy
  std::tuple<int,char> third (std::make_tuple(20,'b'));   // move
  std::tuple<long,char> fourth (third);                   // implicit conversion
  std::tuple<int,char> fifth (10,'a');                    // initialization
  std::tuple<int,char> sixth (std::make_pair(30,'c'));    // from pair / move

  std::cout << "sixth contains: " << std::get<0>(sixth);
  std::cout << " and " << std::get<1>(sixth) << '\n';

  return 0;
}
```
Output
```
sixth contains: 30 and c
```
##### tuple::tie
将元组中的值解包到多个变量中.
```cpp
template <class... Types>
constexpr tuple<Types&...> tie(Types&... args);
```
Example:
```cpp
#include <iostream>
#include <tuple>
int main() {
    std::tuple<int, double, std::string> myTuple(42, 3.14, "Hello");
    int a;
    double b;
    std::string c;
    std::tie(a, b, c) = myTuple;
    std::cout << "a: " << a << "\n";
    std::cout << "b: " << b << "\n";
    std::cout << "c: " << c << "\n";
    return 0;
}
```
Output:
```cpp
a: 42
b: 3.14
c: Hello
```
##### tuple::apply(c++23)
将函数 F 应用于 tuple t
```cpp
template <class F, class Tuple>
constexpr decltype(auto) apply(F&& f, Tuple&& t);
```
Example:
```cpp
#include <iostream>
#include <tuple>
void print(int a, double b, const std::string& c) {
    std::cout << "a: " << a << ", b: " << b << ", c: " << c << "\n";
}
int main() {
    std::tuple<int, double, std::string> myTuple(42, 3.14, "Hello");
    std::apply(print, myTuple);
    return 0;
}
```
Output:
```cpp
a: 42, b: 3.14, c: Hello
```
##### tuple::ignore
解包操作中忽略某个值。
```cpp
namespace std {
    struct _Ignored {};
    _Ignored const ignore;
}
```
Example:
```cpp
#include <iostream>
#include <tuple>
int main() {
    std::tuple<int, double, std::string, char> t(1, 2.5, "hello", 'a');
    int a;
    std::string c;
    std::tie(a, std::ignore, c, std::ignore) = t;
    std::cout << "a: " << a << ", c: " << c << std::endl;
    return 0;
}
```
Output:
```cpp
a: 1, c: hello
```
#### pair
这个类把一对值（values）结合在一起，这些值可能是不同的类型（T1 和 T2）。每个值可以被公有的成员变量first、second访问。

pair是tuple（元组）的一个特例。

pair的实现是一个结构体，主要的两个成员变量是first second 因为是使用struct不是class，所以可以直接使用pair的成员变量。

应用：

* 可以将两个类型数据组合成一个如map<key, value>
* 当某个函数需要两个返回值时

```cpp
template <class T1, class T2> struct pair;
```
##### pair::pair
构建一个pair对象。

这涉及到单独构建它的两个组件对象，初始化依赖于调用的构造器形式：

（1）默认的构造函数

构建一个 对对象的元素值初始化。

（2）复制/移动构造函数（和隐式转换）

该对象被初始化为pr的内容 对目的。pr
的相应成员被传递给每个成员的构造函数。

（3）初始化构造函数

会员 第一是由一个和成员构建的第二与b。

（4）分段构造

构造成员 first  和 second  到位，传递元素first\_args 作为参数的构造函数 first，和元素 second\_args 到的构造函数 second 。

```cpp
default (1)
constexpr pair();
copy / move (2)
template<class U, class V> pair (const pair<U,V>& pr);
template<class U, class V> pair (pair<U,V>&& pr);
pair (const pair& pr) = default;
pair (pair&& pr) = default;
initialization (3)
pair (const first_type& a, const second_type& b);
template<class U, class V> pair (U&& a, V&& b);
piecewise (4)
template <class... Args1, class... Args2>
  pair (piecewise_construct_t pwc, tuple<Args1...> first_args,
                                   tuple<Args2...> second_args);
```

Example

```cpp
#include <utility>      // std::pair, std::make_pair
#include <string>       // std::string
#include <iostream>     // std::cout

int main () {
  std::pair <std::string,double> product1;                     // default constructor
  std::pair <std::string,double> product2 ("tomatoes",2.30);   // value init
  std::pair <std::string,double> product3 (product2);          // copy constructor

  product1 = std::make_pair(std::string("lightbulbs"),0.99);   // using make_pair (move)

  product2.first = "shoes";                  // the type of first is string
  product2.second = 39.90;                   // the type of second is double

  std::cout << "The price of " << product1.first << " is $" << product1.second << '\n';
  std::cout << "The price of " << product2.first << " is $" << product2.second << '\n';
  std::cout << "The price of " << product3.first << " is $" << product3.second << '\n';
  return 0;
}
```
Output
```
The price of lightbulbs is $0.99
The price of shoes is $39.9
The price of tomatoes is $2.3
```
#### span
提供了一种方便的方式来引用连续的内存块。span不复制数据，使用方式类似于指针或引用，提供了更高的性能。

##### span::first
获取一个包含span前count个元素的子视图。
```cpp
template <std::size_t Count>
constexpr std::span<T, Count> first() const;
```
Example:
```cpp
#include <iostream>
#include <span>
#include <vector>
int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::span<int> span(vec);
    auto first3 = span.first<3>();
    std::cout << "First 3 elements: ";
    for (int i : first3) {
        std::cout << i << " ";
    }
    return 0;
}
```
Output:
```cpp
First 3 elements: 1 2 3
```
##### span::subspan
获取一个包含span中从offset开始的count个元素的子视图。
```cpp
constexpr std::span subspan(std::size_t offset, std::size_t count) const;
```
Example:
```cpp
#include <iostream>
#include <span>
#include <vector>
int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::span<int> span(vec);
    auto subspan1 = span.subspan(2, 3);
    std::cout << "Subspan : ";
    for (int i : subspan1) {
        std::cout << i << " ";
    }
    return 0;
}
```
Output:
```cpp
Subspan : 3 4 5
```
#### 无序容器（基于哈希表实现）
![]

##### unordered_set
存储唯一的元素，不允许重复，元素的顺序是无序的。
##### unordered_multiset
存储元素，允许重复，元素的顺序是无序的。
##### unordered_map
存储键值对，键是唯一的，元素的顺序是无序的。
##### unordered_multimap
存储键值对，允许键重复，元素的顺序是无序的。
#### 平坦容器(C++23)
使用连续的存储空间，速度优化。
![]

##### flat_set

##### flat_multiset

##### flat_map

##### flat_multimap

# docs__02-cpp-oop__class__README.md

## 2.1 类与对象

> 作者：青松与桑叶
> C/C++ 面试通关指南 -- 第二部分：C++ 面向对象

---

### 目录

- 一、什么是类和对象
- 二、类的定义
- 三、构造函数
- 四、析构函数
- 五、this 指针
- 六、拷贝控制：深拷贝 vs 浅拷贝
- 七、面试真题

---

### 一、什么是类和对象

#### 1.1 从生活理解类和对象

同学们好！在正式讲代码之前，我们先从生活中的例子来理解"类"和"对象"。

想象一下，你手里有一张**建筑图纸**，上面画着一栋三室两厅的房子，标注了每个房间的用途。这张图纸就是**类（Class）**——它只是一个**设计蓝图**，描述了"房子应该长什么样"。

而根据这张图纸，施工队实际盖出来的那一栋栋具体的房子，就是**对象（Object）**——它们是图纸的**具体实例**。

再举几个例子：

| 类（蓝图） | 对象（实例） |
|-----------|-------------|
| `Student`（学生类） | 张三、李四、王五 |
| `Car`（汽车类） | 你家的那辆白色丰田、隔壁的黑色本田 |
| `Phone`（手机类） | 你的 iPhone 15、同事的华为 Mate 60 |

#### 1.2 类和对象的关系

```
类（Class）  ──实例化──>  对象（Object）
  模板/蓝图                   具体的实体
  抽象的概念                   占用实际内存
  不占数据内存                 占用数据内存
```

**一句话总结：类是对象的类型，对象是类的实例。**

#### 1.3 面试怎么答

> **面试官：请解释一下类和对象的区别。**
>
> 答：类是一种用户自定义的数据类型，它定义了数据成员（属性）和成员函数（方法）。对象是类的具体实例，在内存中占有实际空间。类本身不占用数据内存（只有代码段），而每个对象都有自己的数据副本。打个比方，类就像建筑图纸，对象就是根据图纸建造的房子。

---

### 二、类的定义

#### 2.1 基本语法

C++ 中用 `class` 关键字来定义类：

```cpp
class 类名 {
public:       // 公有成员，外部可以访问
    // 成员函数声明
protected:    // 保护成员，子类可以访问
    // ...
private:      // 私有成员，只有类内部可以访问
    // 成员变量声明
};
```

#### 2.2 成员变量和成员函数

```cpp
class Student {
private:
    string name;    // 成员变量（属性）
    int age;
    double score;

public:
    // 成员函数（方法）
    void display() const {
        cout << "姓名: " << name << ", 年龄: " << age << endl;
    }

    // setter 方法
    void setName(const string& n) {
        name = n;
    }

    // getter 方法
    string getName() const {
        return name;
    }
};
```

#### 2.3 访问控制修饰符

C++ 提供三种访问控制：

| 修饰符 | 类内部 | 子类 | 外部代码 |
|--------|--------|------|----------|
| `public` | 可以 | 可以 | 可以 |
| `protected` | 可以 | 可以 | 不可以 |
| `private` | 可以 | 不可以 | 不可以 |

**设计原则：**
- 成员变量通常设为 `private`（封装性）
- 需要给外部使用的函数设为 `public`
- 需要给子类使用的设为 `protected`

> **为什么要把成员变量设为 private？**
> 这是面向对象三大特性之一——**封装**。好处是：
> 1. 可以控制访问权限（比如只读）
> 2. 可以在修改时加入校验逻辑
> 3. 内部实现改变不影响外部代码

#### 2.4 类的声明和实现分离

对于大型项目，通常把类的声明放在头文件（`.h`），实现放在源文件（`.cpp`）：

```cpp
// Student.h —— 声明
class Student {
private:
    string name;
    int age;
public:
    void display() const;
    void setAge(int a);
};

// Student.cpp —— 实现
void Student::display() const {
    cout << "姓名: " << name << ", 年龄: " << age << endl;
}

void Student::setAge(int a) {
    age = a;
}
```

注意 `Student::display()` 中的 `::` 是**作用域解析符**，表示 `display` 是 `Student` 类的成员函数。

---

### 三、构造函数

构造函数是一种特殊的成员函数，在**创建对象时自动调用**。它的名字和类名相同，没有返回值。

#### 3.1 默认构造函数

```cpp
class Student {
public:
    // 默认构造函数：不带参数
    Student() : name("未知"), age(0), score(0.0) {
        cout << "默认构造函数被调用" << endl;
    }
};
```

**注意：** 如果你没有定义任何构造函数，编译器会自动生成一个默认构造函数（什么都不做）。但如果你定义了带参构造函数，编译器就**不再自动生成**默认构造函数了。

#### 3.2 带参构造函数

```cpp
Student(string n, int a, double s) : name(n), age(a), score(s) {
    cout << "带参构造函数被调用: " << name << endl;
}
```

**初始化列表** `: name(n), age(a)` 比在函数体内赋值更高效，尤其是对于 `const` 成员和引用成员，**必须**使用初始化列表。

#### 3.3 拷贝构造函数

当一个对象需要用另一个同类型对象来初始化时，拷贝构造函数被调用：

```cpp
Student(const Student& other) : name(other.name), age(other.age), score(other.score) {
    cout << "拷贝构造函数被调用" << endl;
}
```

**拷贝构造函数在以下场景被调用：**
1. 用一个对象初始化另一个对象：`Student s2 = s1;`
2. 函数参数按值传递对象
3. 函数按值返回对象

#### 3.4 移动构造函数（C++11）

移动构造函数"偷取"源对象的资源，而不是拷贝：

```cpp
Student(Student&& other) noexcept
    : name(std::move(other.name)),
      age(other.age),
      score(other.score) {
    cout << "移动构造函数被调用" << endl;
    other.age = 0;       // 将源对象置于有效但不确定的状态
    other.score = 0.0;
}
```

**什么时候用移动构造？** 当源对象是临时的（右值），你不需要保留它时。移动比拷贝高效得多，因为不需要分配新内存和复制数据。

#### 3.5 委托构造函数（C++11）

一个构造函数可以调用同类的另一个构造函数：

```cpp
class Student {
public:
    Student() : Student("未知", 0, 0.0) {}  // 委托给带参构造
    Student(string n, int a, double s) : name(n), age(a), score(s) {}
};
```

#### 3.6 构造函数小结

```
构造函数类型          语法特征                    触发时机
─────────────────────────────────────────────────────────
默认构造函数          无参数或所有参数有默认值      Student s;
带参构造函数          有参数                      Student s("张三", 20, 95.5);
拷贝构造函数          参数是 const T&             Student s2 = s1;
移动构造函数          参数是 T&&（C++11）          Student s2 = std::move(s1);
委托构造函数          初始化列表调用另一个构造     减少重复代码
```

---

### 四、析构函数

析构函数在对象**生命周期结束时自动调用**，用于释放资源。

```cpp
~Student() {
    cout << "析构函数被调用: " << name << endl;
}
```

#### 4.1 析构函数的特点

1. 名字是 `~类名`
2. 没有参数，没有返回值
3. 不能重载（一个类只有一个析构函数）
4. 在以下时机被调用：
   - 局部对象离开作用域时
   - `delete` 动态分配的对象时
   - 程序结束时（全局/静态对象）

#### 4.2 析构顺序

析构的顺序和构造的顺序**相反**：

```cpp
void test() {
    Student s1;           // 先构造 s1
    Student s2("李四", 21, 88.0);  // 再构造 s2
}  // 先析构 s2，再析构 s1（栈的后进先出）
```

#### 4.3 虚析构函数

如果类可能被继承，并且会通过基类指针删除派生类对象，**必须将析构函数声明为虚函数**：

```cpp
class Base {
public:
    virtual ~Base() { cout << "Base 析构" << endl; }
};

class Derived : public Base {
public:
    ~Derived() override { cout << "Derived 析构" << endl; }
};

Base* p = new Derived();
delete p;  // 如果 Base 的析构不是虚函数，Derived 的析构不会被调用！
```

> **面试高频考点：** 为什么基类的析构函数要声明为虚函数？如果不声明为虚函数，通过基类指针 `delete` 派生类对象时，只会调用基类的析构函数，派生类的资源不会被释放，造成**内存泄漏**。

---

### 五、this 指针

#### 5.1 什么是 this 指针

每个成员函数都有一个隐藏的参数 `this`，它是一个指向**调用该函数的对象**的指针。

```cpp
class Student {
private:
    string name;
    int age;
public:
    void setName(string name) {
        // 这里的 name 是参数，this->name 是成员变量
        this->name = name;
    }
    void setAge(int age) {
        this->age = age;
    }
};
```

#### 5.2 this 指针的本质

```cpp
Student s;
s.setName("张三");
// 编译器实际处理为：
setName(&s, "张三");  // this = &s
```

#### 5.3 this 指针的常见用途

**1. 区分成员变量和参数同名**

```cpp
void setAge(int age) {
    this->age = age;  // this->age 是成员变量，age 是参数
}
```

**2. 返回对象自身，支持链式调用**

```cpp
Student& setAge(int a) {
    this->age = a;
    return *this;  // 返回对象自身的引用
}

// 使用：
s1.setAge(20).setName("张三").display();
```

**3. 在成员函数中返回对象的指针**

```cpp
Student* getThis() {
    return this;
}
```

#### 5.4 this 指针注意事项

- `this` 指针是 `const` 的（不能修改 `this` 的指向）
- 在 `const` 成员函数中，`this` 的类型是 `const ClassName* const`
- 静态成员函数**没有** `this` 指针（因为它不属于任何具体对象）

---

### 六、拷贝控制：深拷贝 vs 浅拷贝

#### 6.1 什么是浅拷贝

如果你没有自定义拷贝构造函数，编译器会生成一个**默认的拷贝构造函数**，它执行的是**浅拷贝**——逐字节复制成员变量。

对于只包含简单类型（int、double、string 等）的类，浅拷贝完全够用。但如果类中包含**指针**，浅拷贝会导致两个对象指向同一块内存，这就是灾难的开始。

#### 6.2 浅拷贝的问题

```
浅拷贝前：
  obj1.ptr ──> [动态内存: "Hello"]

浅拷贝后：
  obj1.ptr ──> [动态内存: "Hello"] <── obj2.ptr
                    ↑
              两个指针指向同一块内存！

问题：
  1. 修改 obj2 会影响 obj1（数据污染）
  2. obj1 析构时释放了内存
  3. obj2 析构时再次释放同一块内存 → double free！
```

#### 6.3 深拷贝的解决方案

深拷贝会为每个对象分配独立的内存，并复制数据：

```cpp
class Buffer {
private:
    char* data;
    size_t size;
public:
    // 构造函数
    Buffer(const char* str) {
        size = strlen(str) + 1;
        data = new char[size];
        strcpy(data, str);
    }

    // 深拷贝构造函数
    Buffer(const Buffer& other) {
        size = other.size;
        data = new char[size];        // 分配新内存
        strcpy(data, other.data);     // 复制数据
    }

    // 析构函数
    ~Buffer() {
        delete[] data;
    }
};
```

#### 6.4 Rule of Three / Rule of Five

如果你需要自定义以下任何一个，通常需要自定义全部：

**C++98 的 Rule of Three（三法则）：**
1. 析构函数
2. 拷贝构造函数
3. 拷贝赋值运算符

**C++11 的 Rule of Five（五法则）：**
1. 析构函数
2. 拷贝构造函数
3. 拷贝赋值运算符
4. 移动构造函数
5. 移动赋值运算符

> **面试高频：** 解释深拷贝和浅拷贝的区别。
>
> 答：浅拷贝只是简单地复制内存中的值，如果类中包含指针，浅拷贝会导致多个对象共享同一块动态内存，引发 double free 和数据污染。深拷贝会为指针成员分配新的内存并复制数据，确保每个对象拥有独立的资源。当类管理动态内存等资源时，必须实现深拷贝。

---

### 七、面试真题

#### 真题 1：构造函数可以是虚函数吗？

**答：** 不可以。虚函数的机制依赖于虚函数表（vtable），而 vtable 指针（vptr）是在构造函数中初始化的。如果构造函数是虚函数，那么在调用构造函数时 vptr 还没有初始化，无法找到虚函数表，产生矛盾。此外，构造函数的作用是初始化对象，在构造完成之前对象还没有确定的具体类型。

#### 真题 2：拷贝构造函数和赋值运算符有什么区别？

**答：**
- **拷贝构造函数**是用一个已存在的对象来**创建**一个新对象：`Student s2(s1);`
- **赋值运算符**是将一个对象的值**赋给**一个已存在的对象：`s2 = s1;`
- 拷贝构造函数不需要检查自赋值，因为新对象还没有被初始化
- 赋值运算符需要检查自赋值（`if (this == &other) return *this;`），并且需要释放原有资源

#### 真题 3：什么情况下编译器会自动生成拷贝构造函数？什么情况下不会？

**答：** 如果你没有声明任何拷贝控制成员（拷贝构造、拷贝赋值、移动构造、移动赋值、析构），编译器会生成默认的拷贝构造函数。但如果你声明了以下任何一项：
- 拷贝构造函数
- 拷贝赋值运算符
- 移动构造函数
- 移动赋值运算符
- 析构函数

编译器就不再自动生成拷贝构造函数（具体规则取决于 C++ 标准版本，C++11 之后规则有所变化）。

#### 真题 4：explicit 关键字的作用是什么？

**答：** `explicit` 关键字用于阻止构造函数的**隐式转换**。例如：

```cpp
class Student {
public:
    explicit Student(string n) : name(n) {}
};

Student s1("张三");     // OK，直接初始化
Student s2 = "李四";    // 错误！被 explicit 阻止
```

不加 `explicit` 时，`Student s2 = "李四"` 会隐式调用构造函数，这可能导致难以察觉的 bug。**单参数构造函数建议加 `explicit`。**

#### 真题 5：static 成员变量和 static 成员函数的特点？

**答：**
- `static` 成员变量属于类级别，所有对象共享同一份数据，必须在类外初始化
- `static` 成员函数没有 `this` 指针，只能访问 `static` 成员
- 可以通过 `ClassName::staticFunc()` 直接调用，不需要创建对象
- `static` 成员变量在程序启动时分配内存，程序结束时释放

---

### 完整示例代码

本节所有概念的演示代码请参考：

class_demo.cpp

编译运行：

```bash
g++ -std=c++17 -Wall -o class_demo class_demo.cpp
./class_demo
```

---

### 下一节

[2.2 继承与组合](docs__02-cpp-oop__inheritance__README.md)

---

> 本教程由**青松与桑叶**编写，面向 C/C++ 面试准备的保姆级中文教学。

# docs__02-cpp-oop__inheritance__README.md

## 2.2 继承与组合

> 作者：青松与桑叶
> C/C++ 面试通关指南 -- 第二部分：C++ 面向对象

---

### 目录

- 一、继承的基本概念
- 二、继承的语法和类型
- 三、单继承
- 四、多继承
- 五、虚继承——解决菱形问题
- 六、组合 vs 继承
- 七、面试真题

---

### 一、继承的基本概念

#### 1.1 什么是继承

继承是面向对象编程中实现**代码复用**的重要机制。它允许我们定义一个新类（派生类/子类），从已有类（基类/父类）那里继承属性和行为。

用生活中的例子来理解：

```
动物（基类）
├── 哺乳动物（派生类）
│   ├── 狗
│   └── 猫
└── 鸟类（派生类）
    ├── 鹰
    └── 企鹅
```

狗继承了哺乳动物的所有特征（恒温、胎生），又添加了自己的特征（会汪汪叫、嗅觉灵敏）。

#### 1.2 为什么要用继承

1. **代码复用**：子类直接使用父类已有的代码，不用重复编写
2. **多态的基础**：通过基类指针/引用调用派生类的方法
3. **建立类之间的关系模型**：体现"is-a"关系（狗"是一个"哺乳动物）

---

### 二、继承的语法和类型

#### 2.1 基本语法

```cpp
class 派生类 : 继承方式 基类 {
    // ...
};
```

#### 2.2 三种继承方式

C++ 有三种继承方式，它们决定了基类成员在派生类中的访问权限：

##### public 继承（最常用）

```cpp
class Base {
public:
    int pub = 1;
protected:
    int prot = 2;
private:
    int priv = 3;
};

class Derived : public Base {
public:
    void test() {
        cout << pub;   // OK：public -> public
        cout << prot;  // OK：protected -> protected
        // cout << priv;  // 错误：private 不可访问
    }
};

int main() {
    Derived d;
    cout << d.pub;    // OK
    // cout << d.prot; // 错误：外部不可访问 protected
    // cout << d.priv; // 错误：外部不可访问 private
}
```

**访问权限变化表：**

| 基类成员 | public 继承 | protected 继承 | private 继承 |
|---------|------------|----------------|-------------|
| public | public | protected | private |
| protected | protected | protected | private |
| private | 不可访问 | 不可访问 | 不可访问 |

#### 2.3 三种继承方式的区别

```
public 继承：
  "is-a" 关系 —— 派生类"是一种"基类
  外部代码可以访问基类的 public 成员
  最常用，符合面向对象设计原则

protected 继承：
  "has-access-to" 关系 —— 派生类可以访问基类的 public/protected
  外部代码不能访问基类的任何成员
  很少使用

private 继承：
  "implemented-in-terms-of" 关系 —— 派生类用基类来实现功能
  外部代码和子类都不能访问基类的任何成员
  通常可以用"组合"替代
```

> **面试怎么答：**
>
> **面试官：C++ 有哪几种继承方式？区别是什么？**
>
> 答：C++ 有 public、protected、private 三种继承方式。public 继承是最常用的，它保持了基类成员的访问级别不变，体现"is-a"关系。protected 继承将基类的 public 和 protected 成员都变为派生类的 protected。private 继承将所有可访问成员都变为 private。实际开发中几乎只用 public 继承，protected 和 private 继承可以用组合来替代。

---

### 三、单继承

#### 3.1 基本示例

```cpp
class Animal {
protected:
    string name;
    int age;
public:
    Animal(string n, int a) : name(n), age(a) {}
    void eat() {
        cout << name << " 在吃东西" << endl;
    }
};

class Dog : public Animal {
private:
    string breed;
public:
    Dog(string n, int a, string b) : Animal(n, a), breed(b) {}
    void bark() {
        cout << name << " 汪汪叫" << endl;
    }
};
```

#### 3.2 构造函数的调用顺序

创建派生类对象时，构造函数的调用顺序是：

```
1. 基类构造函数（先构造"父"部分）
2. 成员对象的构造函数（按声明顺序）
3. 派生类构造函数（最后构造"子"部分）
```

析构顺序则完全相反：

```
1. 派生类析构函数
2. 成员对象的析构函数
3. 基类析构函数
```

#### 3.3 向上转型

派生类对象可以赋值给基类引用或指针（向上转型是安全的）：

```cpp
Dog dog("旺财", 3, "金毛");
Animal& ref = dog;     // OK：向上转型
Animal* ptr = &dog;    // OK：向上转型

ref.eat();    // OK
// ref.bark(); // 错误：基类引用不知道 bark 的存在
```

---

### 四、多继承

#### 4.1 什么是多继承

一个类可以同时继承多个基类：

```cpp
class Teacher {
public:
    void teach() { cout << "老师在讲课" << endl; }
};

class Researcher {
public:
    void research() { cout << "在做研究" << endl; }
};

// 多继承：同时继承 Teacher 和 Researcher
class Professor : public Teacher, public Researcher {
public:
    void supervise() { cout << "在指导学生" << endl; }
};
```

#### 4.2 多继承的问题

多继承会带来一些复杂的问题：

**1. 名字冲突**

如果两个基类有同名成员，需要用作用域解析符来区分：

```cpp
class Base1 {
public:
    void show() { cout << "Base1::show" << endl; }
};

class Base2 {
public:
    void show() { cout << "Base2::show" << endl; }
};

class Derived : public Base1, public Base2 {
public:
    void test() {
        Base1::show();  // 明确指定调用哪个
        Base2::show();
    }
};
```

**2. 菱形继承问题**

这是多继承最经典的问题，下一节详细讲。

#### 4.3 多继承的内存布局

```
多继承下 Derived 的内存布局：

┌─────────────────┐
│   Base1 部分     │
├─────────────────┤
│   Base2 部分     │
├─────────────────┤
│   Derived 部分   │
└─────────────────┘
```

---

### 五、虚继承——解决菱形问题

#### 5.1 什么是菱形继承

```
        Animal
       /      \
    Mammal    Bird
       \      /
       Bat（蝙蝠）
```

蝙蝠同时继承了哺乳动物和鸟类，而两者都继承了动物。如果不做特殊处理，`Bat` 中会有**两份** `Animal` 的数据，这就是**菱形继承问题**。

#### 5.2 菱形继承的问题

```cpp
class Animal {
public:
    int age;
};

class Mammal : public Animal {};
class Bird : public Animal {};

class Bat : public Mammal, public Bird {};

Bat b;
b.age = 5;  // 错误！编译器不知道 age 是 Mammal::age 还是 Bird::age
```

#### 5.3 虚继承的解决方案

使用 `virtual` 关键字进行虚继承：

```cpp
class Animal {
public:
    int age;
};

class Mammal : virtual public Animal {};  // 虚继承
class Bird : virtual public Animal {};    // 虚继承

class Bat : public Mammal, public Bird {};

Bat b;
b.age = 5;  // OK！现在只有一份 Animal 的数据
```

#### 5.4 虚继承的内存布局

```
虚继承后 Bat 的内存布局：

┌─────────────────────┐
│   Mammal 部分        │
│   (vptr → Animal)   │
├─────────────────────┤
│   Bird 部分          │
│   (vptr → Animal)   │
├─────────────────────┤
│   Bat 部分           │
├─────────────────────┤
│   Animal 部分        │  ← 只有一份！
│   (共享)             │
└─────────────────────┘
```

虚继承通过引入**虚基类表（vbtable）**来保证虚基类只有一份实例。Mammal 和 Bird 中各有一个 vptr，指向共享的 Animal 部分。

#### 5.5 虚继承的代价

1. **额外的内存开销**：每个虚继承的类需要一个额外的 vptr
2. **性能开销**：访问虚基类成员需要间接寻址
3. **构造更复杂**：虚基类由最远派生类负责构造

> **面试高频：** 什么是菱形继承？如何解决？
>
> 答：菱形继承是指一个派生类同时继承两个基类，而这两个基类又继承自同一个更高层的基类，形成菱形结构。这会导致派生类中存在两份顶层基类的数据。解决方案是使用虚继承（`virtual public`），让顶层基类只保留一份实例。虚继承通过虚基类表实现，会有一定的内存和性能开销。

---

### 六、组合 vs 继承

#### 6.1 什么是组合

组合是指一个类包含另一个类的对象作为成员变量，体现的是 **"has-a"** 关系。

```cpp
class Engine {
public:
    void start() { cout << "引擎启动" << endl; }
};

class Car {
private:
    Engine engine;  // Car "has-a" Engine（组合）
    string brand;
public:
    void drive() {
        engine.start();
        cout << brand << " 开始行驶" << endl;
    }
};
```

#### 6.2 组合 vs 继承对比

```
┌──────────────────────────────────────────────────────────┐
│                    组合 vs 继承                           │
├──────────────┬───────────────────┬───────────────────────┤
│              │     继承           │     组合               │
├──────────────┼───────────────────┼───────────────────────┤
│ 关系         │ "is-a"            │ "has-a"               │
│ 耦合度       │ 高（白盒复用）      │ 低（黑盒复用）         │
│ 多态支持     │ 天然支持           │ 需要配合接口/指针       │
│ 修改影响     │ 修改基类影响所有子类 │ 修改被组合类影响使用者  │
│ 封装性       │ 破坏封装           │ 保持封装               │
│ 适用场景     │ 真正的"is-a"关系    │ 大多数情况             │
└──────────────┴───────────────────┴───────────────────────┘
```

#### 6.3 什么时候用继承，什么时候用组合

**优先使用组合的原则：**

> "Favor composition over inheritance." —— 《设计模式》GoF

**使用继承的场景：**
- 子类确实是父类的一种特殊类型（"is-a"关系明确）
- 需要利用多态特性
- 子类需要访问父类的 protected 成员

**使用组合的场景：**
- 只是想复用某个类的功能（"has-a"关系）
- 不需要多态
- 希望降低耦合度
- 被复用的类可能变化

#### 6.4 实际示例

```cpp
// 继承：狗"是一种"动物 —— 合理
class Dog : public Animal {};

// 组合：汽车"有一个"引擎 —— 合理
class Car {
    Engine engine;  // 组合
};

// 错误示范：用继承实现"has-a"
// class Car : public Engine {}  // 汽车"是一种"引擎？不合理！
```

> **面试高频：** 组合和继承怎么选？
>
> 答：面向对象设计的一条重要原则是"组合优于继承"。继承是"is-a"关系，组合是"has-a"关系。继承会带来强耦合，基类的修改可能影响所有子类，而且破坏了封装性。组合则保持了良好的封装和低耦合。只有当确实是"is-a"关系且需要多态时才使用继承，其他情况优先使用组合。

---

### 七、面试真题

#### 真题 1：C++ 中 public、protected、private 继承的区别？

**答：** public 继承保持基类成员的访问级别不变，是最常用的继承方式，体现"is-a"关系。protected 继承将基类的 public 和 protected 成员都变为派生类的 protected。private 继承将所有可访问成员变为 private。后两者很少使用，因为它们可以用组合来替代，语义更清晰。

#### 真题 2：什么是虚继承？解决什么问题？

**答：** 虚继承通过在继承列表中使用 `virtual` 关键字，解决菱形继承问题。菱形继承是指一个类同时继承两个基类，而这两个基类又继承自同一个更高层的基类，导致派生类中存在两份顶层基类的数据。虚继承保证虚基类只有一份实例，通过虚基类表（vbtable）实现，但会带来额外的内存和性能开销。

#### 真题 3：构造函数和析构函数的调用顺序？

**答：** 构造顺序：先调用基类构造函数，再调用成员对象构造函数，最后调用派生类构造函数。析构顺序完全相反：先调用派生类析构函数，再调用成员对象析构函数，最后调用基类析构函数。可以简记为"先有父再有子，析构时先子后父"。

#### 真题 4：多继承中如何解决名字冲突？

**答：** 当多个基类有同名成员时，可以通过作用域解析符来明确指定调用哪个基类的成员，例如 `Base1::func()` 和 `Base2::func()`。也可以在派生类中覆盖同名成员来统一接口。

#### 真题 5：组合和继承的区别？什么时候用哪个？

**答：** 继承是"is-a"关系，组合是"has-a"关系。继承的耦合度高，基类的修改会影响子类；组合的耦合度低，封装性好。设计原则是"组合优于继承"，只有当确实是"is-a"关系且需要多态时才使用继承，其他情况优先使用组合。

---

### 完整示例代码

本节所有概念的演示代码请参考：

inheritance_demo.cpp

编译运行：

```bash
g++ -std=c++17 -Wall -o inheritance_demo inheritance_demo.cpp
./inheritance_demo
```

---

### 上一节 | 下一节

[2.1 类与对象](docs__02-cpp-oop__class__README.md) | [2.3 多态与虚函数](docs__02-cpp-oop__polymorphism__README.md)

---

> 本教程由**青松与桑叶**编写，面向 C/C++ 面试准备的保姆级中文教学。

# docs__02-cpp-oop__polymorphism__README.md

## 2.3 多态与虚函数

> 作者：青松与桑叶
> C/C++ 面试通关指南 -- 第二部分：C++ 面向对象

---

### 目录

- 一、什么是多态
- 二、虚函数的声明和使用
- 三、纯虚函数和抽象类
- 四、动态绑定 vs 静态绑定
- 五、override 和 final 关键字
- 六、面试真题

---

### 一、什么是多态

#### 1.1 多态的概念

多态（Polymorphism）是面向对象编程的三大特性之一（封装、继承、多态）。它的字面意思是"多种形态"。

在 C++ 中，多态是指**同一接口，不同实现**。通过基类的指针或引用调用同名函数，根据实际对象的类型，执行不同的代码。

#### 1.2 生活中的多态

想象一个遥控器（基类指针），它可以控制不同的设备：

```
遥控器按钮 "powerOn()"
    ├── 电视 → 打开电视
    ├── 空调 → 启动空调
    └── 音响 → 播放音乐
```

同一个按钮（接口），按下后不同的设备（对象）有不同的反应（实现），这就是多态。

#### 1.3 C++ 中的两种多态

```
┌──────────────────────────────────────────────────────────┐
│                    C++ 的多态                             │
├────────────────────────┬─────────────────────────────────┤
│     静态多态            │     动态多态                    │
│  （编译期多态）          │  （运行期多态）                  │
├────────────────────────┼─────────────────────────────────┤
│  函数重载              │  虚函数                         │
│  模板                  │  虚函数重写（override）          │
│  运算符重载            │                                │
│                        │                                │
│  编译器在编译期确定     │  程序在运行期根据对象类型确定     │
│  调用哪个函数           │  调用哪个函数                   │
└────────────────────────┴─────────────────────────────────┘
```

本节重点讲解**动态多态**，也就是通过虚函数实现的多态。

#### 1.4 多态的基本示例

```cpp
class Animal {
public:
    virtual void speak() {   // 虚函数
        cout << "动物发出声音" << endl;
    }
};

class Dog : public Animal {
public:
    void speak() override {  // 重写虚函数
        cout << "汪汪汪！" << endl;
    }
};

class Cat : public Animal {
public:
    void speak() override {
        cout << "喵喵喵！" << endl;
    }
};

// 同一个函数，不同的行为 —— 这就是多态！
void makeSpeak(Animal& animal) {
    animal.speak();  // 运行时根据实际类型调用
}

int main() {
    Dog dog;
    Cat cat;
    makeSpeak(dog);  // 输出：汪汪汪！
    makeSpeak(cat);  // 输出：喵喵喵！
}
```

---

### 二、虚函数的声明和使用

#### 2.1 虚函数的声明

在成员函数前加 `virtual` 关键字：

```cpp
class Base {
public:
    virtual void func() {
        cout << "Base::func()" << endl;
    }
};
```

#### 2.2 虚函数的重写（Override）

派生类中定义与基类虚函数**签名完全相同**的函数，就构成重写：

```cpp
class Derived : public Base {
public:
    void func() override {  // override 是 C++11 的关键字（推荐使用）
        cout << "Derived::func()" << endl;
    }
};
```

**重写的条件（必须全部满足）：**
1. 函数名相同
2. 参数列表完全相同
3. 返回类型相同（或协变返回类型）
4. 基类函数必须是虚函数

#### 2.3 虚函数的调用机制

```cpp
Animal* p = new Dog();
p->speak();  // 调用的是 Dog::speak()，不是 Animal::speak()
```

**为什么？** 因为 `speak()` 是虚函数，程序在运行时查看 `p` 实际指向的对象类型（`Dog`），然后调用 `Dog` 的 `speak()`。这就是**动态绑定**。

#### 2.4 虚函数的注意事项

**1. 构造函数不能是虚函数**（上一节已解释）

**2. 内联函数可以是虚函数，但编译器通常会忽略 inline**

**3. 静态成员函数不能是虚函数**（没有 this 指针）

**4. 友元函数不能是虚函数**（友元不是成员函数）

**5. 只有类的成员函数才能是虚函数**（全局函数不行）

---

### 三、纯虚函数和抽象类

#### 3.1 纯虚函数

纯虚函数是在基类中声明的、没有实现的虚函数。语法是在函数声明后加 `= 0`：

```cpp
class Shape {
public:
    // 纯虚函数 —— 子类必须实现
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual void draw() const = 0;
};
```

#### 3.2 抽象类

含有纯虚函数的类称为**抽象类**。抽象类的特点：

1. **不能实例化**：`Shape s;` 是错误的
2. **只能作为基类**被继承
3. 派生类**必须实现所有纯虚函数**，否则派生类也是抽象类

```cpp
class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : radius(r) {}

    double area() const override {
        return 3.14159265 * radius * radius;
    }

    double perimeter() const override {
        return 2 * 3.14159265 * radius;
    }

    void draw() const override {
        cout << "画一个半径为 " << radius << " 的圆" << endl;
    }
};

class Rectangle : public Shape {
private:
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}

    double area() const override {
        return width * height;
    }

    double perimeter() const override {
        return 2 * (width + height);
    }

    void draw() const override {
        cout << "画一个 " << width << "x" << height << " 的矩形" << endl;
    }
};
```

#### 3.3 抽象类的意义

抽象类定义了一个**接口规范**，规定了子类必须实现哪些方法。这是面向对象设计中非常重要的概念：

```
抽象类 = 接口规范
  ↓
所有子类都必须遵守这个规范
  ↓
调用方只需要知道基类接口，不需要知道具体子类
  ↓
实现了"针对接口编程，而不是针对实现编程"
```

#### 3.4 纯虚函数可以有实现

C++ 允许纯虚函数有函数体，但子类仍然必须重写它才能实例化：

```cpp
class Base {
public:
    virtual void func() = 0;  // 纯虚函数
};

void Base::func() {  // 可以在类外提供默认实现
    cout << "Base 默认实现" << endl;
}

class Derived : public Base {
public:
    void func() override {
        Base::func();  // 可以调用基类的默认实现
        cout << "Derived 扩展实现" << endl;
    }
};
```

这种技巧在"提供默认实现但强制子类显式选择"的场景下很有用。

---

### 四、动态绑定 vs 静态绑定

#### 4.1 什么是绑定

绑定（Binding）是指将函数调用与具体函数代码关联起来的过程。

#### 4.2 静态绑定（编译期绑定）

**非虚函数**使用静态绑定，编译器在编译时就确定了调用哪个函数：

```cpp
class Animal {
public:
    void speak() {  // 非虚函数
        cout << "动物发出声音" << endl;
    }
};

class Dog : public Animal {
public:
    void speak() {
        cout << "汪汪汪！" << endl;
    }
};

Animal* p = new Dog();
p->speak();  // 输出：动物发出声音（静态绑定，看指针类型）
```

编译器看到 `p` 的类型是 `Animal*`，就直接调用 `Animal::speak()`，不管 `p` 实际指向什么。

#### 4.3 动态绑定（运行期绑定）

**虚函数**使用动态绑定，程序在运行时根据对象的实际类型决定调用哪个函数：

```cpp
class Animal {
public:
    virtual void speak() {  // 虚函数
        cout << "动物发出声音" << endl;
    }
};

class Dog : public Animal {
public:
    void speak() override {
        cout << "汪汪汪！" << endl;
    }
};

Animal* p = new Dog();
p->speak();  // 输出：汪汪汪！（动态绑定，看实际对象类型）
```

#### 4.4 对比总结

```
┌──────────────────────────────────────────────────────────┐
│              静态绑定 vs 动态绑定                         │
├──────────────┬───────────────────┬───────────────────────┤
│              │   静态绑定         │   动态绑定             │
├──────────────┼───────────────────┼───────────────────────┤
│ 函数类型     │ 普通成员函数       │ 虚函数                 │
│ 确定时机     │ 编译期             │ 运行期                 │
│ 判断依据     │ 指针/引用的类型    │ 实际对象的类型          │
│ 性能         │ 高（无额外开销）    │ 略低（查虚函数表）      │
│ 灵活性       │ 低                 │ 高                     │
└──────────────┴───────────────────┴───────────────────────┘
```

#### 4.5 动态绑定的条件

要实现动态绑定，必须**同时满足**以下条件：
1. 基类函数必须是虚函数
2. 必须通过**基类的指针或引用**来调用
3. 派生类必须重写了该虚函数

```cpp
Dog dog;
dog.speak();          // 直接调用，静态绑定（虽然 speak 是虚函数）

Animal& ref = dog;
ref.speak();          // 通过基类引用，动态绑定 ✓

Animal* ptr = &dog;
ptr->speak();         // 通过基类指针，动态绑定 ✓
```

> **面试高频：** 虚函数的实现机制是什么？
>
> 答：编译器为每个含有虚函数的类生成一个虚函数表（vtable），表中存储了该类虚函数的地址。每个对象内部有一个指向 vtable 的指针（vptr）。当通过基类指针调用虚函数时，程序先通过 vptr 找到 vtable，再从 vtable 中找到实际应该调用的函数地址。这个过程发生在运行时，因此称为动态绑定。

---

### 五、override 和 final 关键字

#### 5.1 override 关键字（C++11）

`override` 放在派生类成员函数声明的末尾，表示这个函数**重写**了基类的虚函数。它不是必须的，但**强烈推荐使用**，因为编译器会帮你检查是否真的重写了基类函数。

```cpp
class Base {
public:
    virtual void func(int x) {}
    virtual void hello() const {}
};

class Derived : public Base {
public:
    // 正确重写
    void func(int x) override { cout << "重写成功" << endl; }

    // 编译错误！参数不匹配，不是重写
    // void func(double x) override {}

    // 编译错误！基类没有这个虚函数
    // void world() override {}

    // 编译错误！缺少 const
    // void hello() override {}
};
```

**override 的好处：** 防止因为拼写错误、参数类型不匹配、缺少 const 等原因导致的"你以为重写了，实际没有重写"的 bug。

#### 5.2 final 关键字（C++11）

`final` 有两种用法：

**1. 修饰类：禁止该类被继承**

```cpp
class FinalClass final {
    // ...
};

// 编译错误！FinalClass 不能被继承
// class Derived : public FinalClass {};
```

**2. 修饰虚函数：禁止该虚函数在派生类中被重写**

```cpp
class Base {
public:
    virtual void func() final {
        cout << "这个函数不能被重写" << endl;
    }
};

class Derived : public Base {
public:
    // 编译错误！func 被声明为 final
    // void func() override {}
};
```

#### 5.3 override 和 final 的使用建议

```
建议：
  - 派生类重写虚函数时，始终使用 override
  - 不希望被继承的类，使用 final
  - 不希望被重写的虚函数，使用 final
  - 这两个关键字都是编译期检查，零运行时开销
```

---

### 六、面试真题

#### 真题 1：什么是多态？C++ 中如何实现？

**答：** 多态是指同一接口可以有不同的实现方式。C++ 中主要通过虚函数实现运行期多态（动态多态）：在基类中将函数声明为 `virtual`，在派生类中重写该函数，通过基类指针或引用调用时，程序在运行时根据实际对象的类型决定调用哪个版本的函数。C++ 还支持编译期多态（静态多态），如函数重载和模板。

#### 真题 2：虚函数和纯虚函数的区别？

**答：** 虚函数有默认实现，派生类可以选择重写也可以不重写。纯虚函数没有实现（`= 0`），含有纯虚函数的类是抽象类，不能实例化，派生类必须实现所有纯虚函数才能实例化。纯虚函数主要用于定义接口规范。

#### 真题 3：构造函数中可以调用虚函数吗？

**答：** 可以调用，但不会产生多态效果。在基类构造函数中调用虚函数时，调用的是基类自己的版本，而不是派生类重写的版本。因为在构造基类部分时，派生类部分还没有被构造，对象的类型还是基类。同理，析构函数中调用虚函数也类似。

#### 真题 4：虚函数表（vtable）是什么？

**答：** 虚函数表是编译器为每个含有虚函数的类创建的一个静态数组，表中存储了该类所有虚函数的地址。每个对象内部有一个指向其类的 vtable 的指针（vptr）。通过基类指针调用虚函数时，运行时通过 vptr 找到 vtable，再从 vtable 中取出正确的函数地址进行调用。同一类的所有对象共享同一个 vtable。

#### 真题 5：什么情况下不应该使用虚函数？

**答：** 以下情况不适合使用虚函数：
1. 类不会被继承时
2. 不需要通过基类指针/引用调用派生类方法时
3. 对性能要求极高的场景（虚函数调用有一次间接寻址开销）
4. 嵌入式系统等资源受限环境
5. 构造函数和析构函数中（析构函数可以是虚的，但构造函数不能）

---

### 完整示例代码

本节所有概念的演示代码请参考：

polymorphism_demo.cpp

编译运行：

```bash
g++ -std=c++17 -Wall -o polymorphism_demo polymorphism_demo.cpp
./polymorphism_demo
```

---

### 上一节 | 下一节

[2.2 继承与组合](docs__02-cpp-oop__inheritance__README.md) | [2.4 智能指针](docs__02-cpp-oop__smart-pointer__README.md)

---

> 本教程由**青松与桑叶**编写，面向 C/C++ 面试准备的保姆级中文教学。

# docs__02-cpp-oop__smart-pointer__README.md

## 2.4 智能指针

> 作者：青松与桑叶
> C/C++ 面试通关指南 -- 第二部分：C++ 面向对象

---

### 目录

- 一、为什么需要智能指针
- 二、RAII 思想
- 三、std::unique_ptr —— 独占所有权
- 四、std::shared_ptr —— 共享所有权
- 五、std::weak_ptr —— 解决循环引用
- 六、循环引用问题详解
- 七、面试真题

---

### 一、为什么需要智能指针

#### 1.1 手动内存管理的痛点

在 C++ 中，使用 `new` 分配的内存必须手动 `delete` 释放。但实际开发中，手动管理内存非常容易出错：

```cpp
void problem() {
    int* p = new int(42);
    // ... 一堆代码 ...
    if (someCondition) {
        return;  // 忘记 delete！内存泄漏！
    }
    // ... 更多代码 ...
    delete p;
}
```

常见的内存管理问题：

```
┌──────────────────────────────────────────────────────────┐
│              手动内存管理的三大问题                         │
├──────────────────────────────────────────────────────────┤
│  1. 内存泄漏：new 了但忘记 delete                          │
│  2. 重复释放：同一块内存 delete 了两次（double free）       │
│  3. 悬空指针：delete 后继续使用指针（野指针）               │
└──────────────────────────────────────────────────────────┘
```

#### 1.2 智能指针的登场

C++11 引入了智能指针，它可以**自动管理动态内存**，在适当的时候自动释放资源，从根本上解决上述问题。

```cpp
#include <memory>

void solution() {
    // 智能指针自动管理内存，不需要手动 delete
    auto p = make_unique<int>(42);
    // ... 任何情况下退出函数，内存都会被自动释放 ...
}
```

C++11 提供了三种智能指针，定义在 `<memory>` 头文件中：

| 智能指针 | 所有权模型 | 引用计数 | 头文件 |
|---------|-----------|---------|--------|
| `std::unique_ptr` | 独占所有权 | 无 | `<memory>` |
| `std::shared_ptr` | 共享所有权 | 有 | `<memory>` |
| `std::weak_ptr` | 不拥有所有权 | 无 | `<memory>` |

> **面试必答：** C++11 之后，请使用智能指针代替裸指针（`new`/`delete`），这是现代 C++ 的基本要求。

---

### 二、RAII 思想

#### 2.1 什么是 RAII

RAII（Resource Acquisition Is Initialization，资源获取即初始化）是 C++ 中最重要的资源管理思想。

核心原则：**将资源的生命周期绑定到对象的生命周期**。

```
RAII 的工作原理：

  对象创建（构造函数）→ 获取资源
  对象使用中           → 持有资源
  对象销毁（析构函数）→ 自动释放资源
```

#### 2.2 RAII 的示例

RAII 不仅仅用于内存管理，还可以管理任何资源：

```cpp
// 1. 内存管理
{
    vector<int> v;       // 构造时分配内存
    v.push_back(42);
    // 使用 v...
}  // 离开作用域，析构函数自动释放内存

// 2. 文件管理
{
    ifstream file("data.txt");  // 构造时打开文件
    // 读取文件...
}  // 离开作用域，析构函数自动关闭文件

// 3. 锁管理
{
    lock_guard<mutex> lock(mtx);  // 构造时加锁
    // 临界区操作...
}  // 离开作用域，析构函数自动解锁
```

#### 2.3 为什么 RAII 能解决问题

RAII 利用了 C++ 的一个关键特性：**局部对象离开作用域时，析构函数一定会被调用**（即使是因为异常退出）。

```
不管函数怎么退出：
  - 正常 return  ✓ 析构函数被调用
  - 异常抛出    ✓ 析构函数被调用（栈展开）
  - break       ✓ 析构函数被调用
  - 任何其他方式 ✓ 析构函数被调用

所以资源一定会被释放！
```

---

### 三、std::unique_ptr —— 独占所有权

#### 3.1 基本概念

`unique_ptr` 表示对一块内存的**独占所有权**。同一时刻，只能有一个 `unique_ptr` 指向某个对象。当 `unique_ptr` 被销毁时，它指向的对象也会被自动删除。

#### 3.2 创建 unique_ptr

```cpp
#include <memory>

// 推荐方式：使用 make_unique（C++14）
auto p1 = make_unique<int>(42);       // 指向 int
auto p2 = make_unique<string>("hello"); // 指向 string
auto p3 = make_unique<Dog>("旺财");    // 指向自定义类

// 不推荐的方式（直接用 new）
unique_ptr<int> p4(new int(42));  // 可以但不推荐，异常不安全
```

> **为什么推荐 `make_unique`？**
> 1. 更安全：避免因参数求值顺序导致的内存泄漏
> 2. 代码更简洁
> 3. 性能可能更好（减少一次内存分配）

#### 3.3 unique_ptr 的特性

```cpp
auto p1 = make_unique<int>(42);

// 1. 不能拷贝
// auto p2 = p1;  // 编译错误！unique_ptr 不可拷贝

// 2. 可以移动
auto p2 = std::move(p1);  // OK！p1 变为 nullptr，p2 接管所有权
// 此时 p1 为空，p2 指向原来的对象

// 3. 像裸指针一样使用
cout << *p2 << endl;    // 解引用
cout << p2.get() << endl; // 获取原始指针

// 4. 释放所有权（不删除对象）
int* raw = p2.release();  // p2 变为 nullptr，需要手动 delete raw

// 5. 重置
p2.reset();  // 删除当前对象，p2 变为 nullptr
p2.reset(new int(100));  // 删除当前对象，指向新对象
```

#### 3.4 unique_ptr 管理数组

```cpp
// 管理动态数组
auto arr = make_unique<int[]>(10);
arr[0] = 42;
arr[1] = 100;
```

#### 3.5 unique_ptr 在函数中的使用

```cpp
// 工厂函数：返回 unique_ptr（通过移动语义）
unique_ptr<Dog> createDog() {
    return make_unique<Dog>("旺财");  // 移动语义，无拷贝
}

// 接收 unique_ptr 参数
void process(unique_ptr<Dog> dog) {
    dog->bark();
}  // dog 离开作用域，自动释放

// 传递所有权
auto myDog = createDog();
process(std::move(myDog));  // 所有权转移给 process
// 此时 myDog 为 nullptr
```

---

### 四、std::shared_ptr —— 共享所有权

#### 4.1 基本概念

`shared_ptr` 允许多个指针**共享**同一个对象的所有权。它通过**引用计数**来跟踪有多少个 `shared_ptr` 指向同一个对象。当最后一个 `shared_ptr` 被销毁时，对象才会被删除。

#### 4.2 引用计数的工作原理

```
创建 shared_ptr：
  auto p1 = make_shared<int>(42);
  // 引用计数 = 1

拷贝 shared_ptr：
  auto p2 = p1;
  // 引用计数 = 2（p1 和 p2 共享同一个对象）

再拷贝：
  auto p3 = p2;
  // 引用计数 = 3

p3 离开作用域：
  // 引用计数 = 2

p2 离开作用域：
  // 引用计数 = 1

p1 离开作用域：
  // 引用计数 = 0 → 自动删除对象！
```

#### 4.3 创建 shared_ptr

```cpp
// 推荐方式
auto p1 = make_shared<int>(42);

// 不推荐（两次内存分配）
shared_ptr<int> p2(new int(42));

// 查看引用计数
cout << p1.use_count() << endl;  // 输出：1

auto p3 = p1;
cout << p1.use_count() << endl;  // 输出：2
cout << p3.use_count() << endl;  // 输出：2
```

#### 4.4 shared_ptr 的特性

```cpp
auto p1 = make_shared<string>("hello");

// 1. 可以拷贝
auto p2 = p1;  // OK！引用计数 +1

// 2. 可以移动
auto p3 = std::move(p1);  // p1 变为 nullptr，引用计数不变

// 3. 像裸指针一样使用
cout << *p2 << endl;
cout << p2->size() << endl;

// 4. 检查引用计数
cout << p2.use_count() << endl;  // 2（p2 和 p3）

// 5. 重置
p2.reset();  // 引用计数 -1
cout << p3.use_count() << endl;  // 1

// 6. 检查是否唯一持有
if (p3.unique()) {
    cout << "只有 p3 指向该对象" << endl;
}
```

#### 4.5 make_shared 的优势

```
直接 new 的方式：
  1. new 分配对象内存
  2. new 分配控制块内存
  总共：两次内存分配

make_shared 的方式：
  1. 一次分配足够大的内存，同时存放对象和控制块
  总共：一次内存分配

优势：
  - 减少内存分配次数，提高性能
  - 内存局部性更好（对象和控制块在连续内存中）
  - 异常安全
```

---

### 五、std::weak_ptr —— 解决循环引用

#### 5.1 为什么需要 weak_ptr

`shared_ptr` 有一个致命问题：**循环引用**。当两个对象互相持有对方的 `shared_ptr` 时，它们的引用计数永远不会变为 0，导致内存泄漏。

`weak_ptr` 就是为了解决这个问题而生的。它是对 `shared_ptr` 的**弱引用**，不会增加引用计数。

#### 5.2 weak_ptr 的特性

```cpp
auto sp = make_shared<int>(42);

// 1. 从 shared_ptr 创建 weak_ptr
weak_ptr<int> wp = sp;
// sp.use_count() 仍然是 1（weak_ptr 不增加引用计数）

// 2. 检查所指向的对象是否还存在
if (!wp.expired()) {
    cout << "对象仍然存活" << endl;
}

// 3. 访问对象：必须先 lock() 提升为 shared_ptr
if (auto locked = wp.lock()) {
    cout << *locked << endl;  // 安全访问
} else {
    cout << "对象已被销毁" << endl;
}
```

#### 5.3 weak_ptr 的使用场景

```
weak_ptr 的典型使用场景：

1. 解决 shared_ptr 的循环引用
2. 观察者模式中，观察者不希望影响被观察者的生命周期
3. 缓存系统，缓存不应该阻止对象被回收
4. 打破 parent-child 循环引用（child 持有 parent 的 weak_ptr）
```

---

### 六、循环引用问题详解

#### 6.1 什么是循环引用

```
对象 A 持有 shared_ptr<B>（指向 B）
对象 B 持有 shared_ptr<A>（指向 A）

A 的引用计数 = 1（来自 B）
B 的引用计数 = 1（来自 A）

当 A 和 B 离开作用域时：
  A 的引用计数：1 → 0？不！B 还持有 A 的 shared_ptr
  B 的引用计数：1 → 0？不！A 还持有 B 的 shared_ptr

结果：两个对象都无法被释放 → 内存泄漏！
```

#### 6.2 用代码演示循环引用

```cpp
#include <memory>
#include <iostream>
using namespace std;

class B;  // 前向声明

class A {
public:
    shared_ptr<B> bPtr;
    ~A() { cout << "A 析构" << endl; }
};

class B {
public:
    shared_ptr<A> aPtr;  // 循环引用！
    ~B() { cout << "B 析构" << endl; }
};

void test() {
    auto a = make_shared<A>();
    auto b = make_shared<B>();
    a->bPtr = b;
    b->aPtr = a;  // 循环引用！A 和 B 都不会被析构
}
// test() 结束后，a 和 b 的引用计数都是 2，不会降到 0
// A 和 B 的析构函数都不会被调用 → 内存泄漏！
```

#### 6.3 用 weak_ptr 解决循环引用

```cpp
class B;

class A {
public:
    shared_ptr<B> bPtr;
    ~A() { cout << "A 析构" << endl; }
};

class B {
public:
    weak_ptr<A> aPtr;  // 改用 weak_ptr！
    ~B() { cout << "B 析构" << endl; }
};

void test() {
    auto a = make_shared<A>();
    auto b = make_shared<B>();
    a->bPtr = b;
    b->aPtr = a;  // weak_ptr 不增加引用计数
}
// test() 结束后：
// a 的引用计数 = 1（只有 bPtr 持有）→ 降到 0，A 被析构
// A 析构后，bPtr 被释放，b 的引用计数降到 0，B 被析构
// 输出：A 析构 → B 析构（顺序可能不同）
```

#### 6.4 循环引用的常见场景

```
1. 双向链表：prev 和 next 都用 shared_ptr
2. 树结构：parent 和 children 互相引用
3. 观察者模式：被观察者和观察者互相引用
4. 缓存系统：缓存对象引用了原始对象

解决方案：
  - 确定谁"拥有"谁，只有拥有者使用 shared_ptr
  - 被引用方使用 weak_ptr
  - 例如：树结构中 parent 持有 children 的 shared_ptr，
    children 持有 parent 的 weak_ptr
```

---

### 七、面试真题

#### 真题 1：智能指针有哪些？分别有什么特点？

**答：** C++11 提供了三种智能指针：
1. `unique_ptr`：独占所有权，不可拷贝只能移动，零开销抽象，是最轻量的智能指针
2. `shared_ptr`：共享所有权，内部使用引用计数，可以拷贝，当引用计数为 0 时自动释放
3. `weak_ptr`：弱引用，不增加引用计数，用于解决 `shared_ptr` 的循环引用问题，使用前需要 `lock()` 提升为 `shared_ptr`

#### 真题 2：shared_ptr 的线程安全性如何？

**答：** `shared_ptr` 的引用计数操作是**原子操作**，因此多个线程可以安全地拷贝同一个 `shared_ptr`（引用计数的增减是线程安全的）。但是，通过 `shared_ptr` 访问所指向的对象**不是线程安全的**——如果多个线程同时读写同一个对象，仍然需要加锁。简单来说：控制块是线程安全的，数据不是。

#### 真题 3：make_shared 和直接 new 有什么区别？

**答：** `make_shared` 只进行一次内存分配（对象和控制块在同一块内存中），而直接用 `new` 需要两次分配（分别分配对象和控制块）。`make_shared` 的优势：性能更好、内存局部性更好、异常安全。但 `make_shared` 也有一个缺点：对象和控制块在同一块内存中，只有当 `weak_ptr` 全部过期后才能释放整块内存，如果 `weak_ptr` 生命周期很长，可能导致内存无法及时回收。

#### 真题 4：什么是循环引用？如何解决？

**答：** 循环引用是指两个或多个对象通过 `shared_ptr` 互相持有对方，导致引用计数永远无法降到 0，从而造成内存泄漏。解决方案是使用 `weak_ptr` 打破循环。`weak_ptr` 是对 `shared_ptr` 的弱引用，不增加引用计数，不会阻止对象被释放。使用时需要通过 `lock()` 方法提升为 `shared_ptr` 来安全访问对象。

#### 真题 5：智能指针的线程安全性？

**答：** 详细说明：
- `unique_ptr` 和裸指针一样，不是线程安全的
- `shared_ptr` 的引用计数是线程安全的（使用原子操作），但访问指向的对象不是线程安全的
- `weak_ptr` 的 `lock()` 是线程安全的
- 如果多个线程需要共享修改同一个对象，仍然需要使用 `mutex` 等同步机制

#### 真题 6：什么场景下用 unique_ptr，什么场景下用 shared_ptr？

**答：** 优先使用 `unique_ptr`，只有在确实需要共享所有权时才使用 `shared_ptr`。`unique_ptr` 零开销，语义清晰（独占所有权），可以隐式转换为 `shared_ptr`。`shared_ptr` 有引用计数的开销，而且可能引入循环引用问题。大部分场景下，通过移动语义传递 `unique_ptr` 就可以满足需求。

---

### 完整示例代码

本节所有概念的演示代码请参考：

smart_pointer_demo.cpp

编译运行：

```bash
g++ -std=c++17 -Wall -o smart_pointer_demo smart_pointer_demo.cpp
./smart_pointer_demo
```

---

### 上一节 | 下一节

[2.3 多态与虚函数](docs__02-cpp-oop__polymorphism__README.md) | [2.5 虚函数深度剖析](docs__02-cpp-oop__virtual__README.md)

---

> 本教程由**青松与桑叶**编写，面向 C/C++ 面试准备的保姆级中文教学。

# docs__02-cpp-oop__virtual__README.md

## 2.5 虚函数深度剖析

> 作者：青松与桑叶
> C/C++ 面试通关指南 -- 第二部分：C++ 面向对象

---

### 目录

- 一、虚函数表（vtable）的内存布局
- 二、单继承下的 vtable
- 三、多重继承下的 vtable（多个 vptr）
- 四、虚析构函数
- 五、虚函数的性能开销
- 六、面试真题

---

### 一、虚函数表（vtable）的内存布局

#### 1.1 vtable 是什么

当一个类中声明了虚函数时，编译器会为这个类生成一个**虚函数表（Virtual Function Table，简称 vtable）**。vtable 是一个静态数组，存储了该类所有虚函数的函数指针。

同时，该类的每个对象内部都会包含一个**虚函数表指针（vptr）**，指向该类的 vtable。

#### 1.2 vtable 和 vptr 的关系

```
类级别（所有对象共享）：
  ┌─────────────────────────────────┐
  │         vtable（虚函数表）        │
  │  ┌───────────────────────────┐  │
  │  │ &Base::func1              │  │  ← 虚函数指针
  │  │ &Base::func2              │  │  ← 虚函数指针
  │  │ &Base::func3              │  │  ← 虚函数指针
  │  └───────────────────────────┘  │
  └─────────────────────────────────┘

对象级别（每个对象独立）：
  ┌─────────────────────────────────┐
  │  对象内存布局                     │
  │  ┌───────────────────────────┐  │
  │  │ vptr ──────────────────────┼──┼──→ 指向类的 vtable
  │  ├───────────────────────────┤  │
  │  │ 成员变量 1                  │  │
  │  │ 成员变量 2                  │  │
  │  │ ...                        │  │
  │  └───────────────────────────┘  │
  └─────────────────────────────────┘
```

**关键理解：**
- vtable 是**类级别**的：同一个类的所有对象共享同一个 vtable
- vptr 是**对象级别**的：每个对象都有自己的 vptr，但都指向同一个 vtable
- vtable 在**编译期**生成
- vptr 在**构造函数**中初始化

#### 1.3 虚函数调用的过程

当我们通过基类指针调用虚函数时，实际发生了什么？

```
Base* p = new Derived();
p->func1();

调用过程：
  1. 通过指针 p 找到对象的内存
  2. 通过对象内存中的 vptr 找到 vtable
  3. 在 vtable 中查找 func1 对应的函数指针
  4. 通过函数指针调用实际的函数

  p -> [vptr] -> [vtable] -> [&Derived::func1] -> 调用 Derived::func1()
```

这就是动态绑定的底层实现：**一次额外的间接寻址**。

#### 1.4 用 ASCII 图理解完整布局

```
  代码段                          数据段/只读段
  ┌──────────┐                  ┌──────────────────┐
  │ Base::   │                  │ Base 的 vtable    │
  │  func1() │◄─────────────────│  [0] &Base::func1 │
  │          │                  │  [1] &Base::func2 │
  │ Base::   │                  └──────────────────┘
  │  func2() │◄─────────────────┐
  │          │                  │ Derived 的 vtable  │
  │ Derived::│◄─────────────────│  [0] &Derived::func1│ ← 重写了！
  │  func1() │                  │  [1] &Base::func2   │ ← 继承的
  │          │                  └──────────────────┘
  │ Derived::│
  │  func3() │                  栈/堆
  │          │                  ┌──────────────────┐
  └──────────┘                  │ Base 对象         │
                                │  [vptr] ──────────┼──→ Base 的 vtable
                                │  [base_data]      │
                                └──────────────────┘
                                ┌──────────────────┐
                                │ Derived 对象      │
                                │  [vptr] ──────────┼──→ Derived 的 vtable
                                │  [base_data]      │
                                │  [derived_data]   │
                                └──────────────────┘
```

---

### 二、单继承下的 vtable

#### 2.1 基本示例

```cpp
class Base {
public:
    virtual void func1() { cout << "Base::func1" << endl; }
    virtual void func2() { cout << "Base::func2" << endl; }
    int baseData = 10;
};

class Derived : public Base {
public:
    void func1() override { cout << "Derived::func1" << endl; }
    virtual void func3() { cout << "Derived::func3" << endl; }
    int derivedData = 20;
};
```

#### 2.2 单继承的 vtable 布局

```
Base 的 vtable：
  ┌──────────────────────────┐
  │ [0] &Base::func1         │
  │ [1] &Base::func2         │
  └──────────────────────────┘

Derived 的 vtable：
  ┌──────────────────────────┐
  │ [0] &Derived::func1      │  ← 重写了 Base::func1
  │ [1] &Base::func2         │  ← 继承 Base::func2，未重写
  │ [2] &Derived::func3      │  ← 新增的虚函数
  └──────────────────────────┘

Base 对象的内存布局：
  ┌──────────────────────────┐
  │ vptr ──→ Base 的 vtable   │
  │ baseData (int)            │
  └──────────────────────────┘

Derived 对象的内存布局：
  ┌──────────────────────────┐
  │ vptr ──→ Derived 的 vtable│
  │ baseData (int)            │  ← 继承自 Base
  │ derivedData (int)         │  ← Derived 自己的
  └──────────────────────────┘
```

**关键点：**
1. 派生类的 vtable 中，重写的函数指针被替换为派生类的版本
2. 未重写的函数指针保持基类的版本
3. 派生类新增的虚函数追加在 vtable 末尾
4. vptr 始终在对象内存的起始位置（大多数编译器如此）

#### 2.3 构造过程中的 vptr 变化

在构造派生类对象时，vptr 会经历变化：

```
构造 Derived 对象的过程：

  1. 调用 Base 构造函数
     → vptr 指向 Base 的 vtable
     → 此时如果调用虚函数，调用的是 Base 的版本

  2. 调用 Derived 构造函数
     → vptr 改为指向 Derived 的 vtable
     → 此时如果调用虚函数，调用的是 Derived 的版本

这就是为什么在基类构造函数中调用虚函数不会产生多态效果！
```

---

### 三、多重继承下的 vtable（多个 vptr）

#### 3.1 多重继承的 vtable 布局

当派生类继承多个基类时，每个含有虚函数的基类都会有自己的 vptr：

```cpp
class Base1 {
public:
    virtual void func1() { cout << "Base1::func1" << endl; }
    virtual void func2() { cout << "Base1::func2" << endl; }
    int data1 = 1;
};

class Base2 {
public:
    virtual void func3() { cout << "Base2::func3" << endl; }
    int data2 = 2;
};

class Derived : public Base1, public Base2 {
public:
    void func1() override { cout << "Derived::func1" << endl; }
    void func3() override { cout << "Derived::func3" << endl; }
    int data3 = 3;
};
```

#### 3.2 多重继承的内存布局

```
Derived 对象的内存布局：

  ┌──────────────────────────────┐
  │ Base1 子对象                  │
  │  ┌────────────────────────┐  │
  │  │ vptr1 ──→ vtable1      │  │  ← 指向 Base1 部分的 vtable
  │  │ data1 (int)            │  │
  │  └────────────────────────┘  │
  ├──────────────────────────────┤
  │ Base2 子对象                  │
  │  ┌────────────────────────┐  │
  │  │ vptr2 ──→ vtable2      │  │  ← 指向 Base2 部分的 vtable
  │  │ data2 (int)            │  │
  │  └────────────────────────┘  │
  ├──────────────────────────────┤
  │ Derived 部分                  │
  │  data3 (int)                 │
  └──────────────────────────────┘
```

#### 3.3 多重继承的 vtable 内容

```
vtable1（Base1 部分）：
  ┌──────────────────────────┐
  │ [0] &Derived::func1      │  ← 重写了 Base1::func1
  │ [1] &Base1::func2        │  ← 继承自 Base1
  └──────────────────────────┘

vtable2（Base2 部分）：
  ┌──────────────────────────┐
  │ [0] &Derived::func3      │  ← 重写了 Base2::func3
  └──────────────────────────┘
```

#### 3.4 指针转换时的地址调整

多重继承中，基类指针指向派生类对象时，可能需要调整地址：

```cpp
Derived d;
Base1* p1 = &d;   // p1 == &d（地址不变）
Base2* p2 = &d;   // p2 == &d + sizeof(Base1)（地址偏移！）
```

```
Derived 对象：
  地址 0x100: [vptr1] [data1]    ← Base1 部分，p1 指向这里
  地址 0x108: [vptr2] [data2]    ← Base2 部分，p2 指向这里
  地址 0x110: [data3]            ← Derived 部分

  p1 = &d        → 0x100
  p2 = &d        → 0x108（编译器自动偏移）
  (Derived*)p2   → 0x100（编译器自动偏移回来）
```

> **面试高频：** 多重继承中，将派生类指针赋给不同基类指针时，地址可能不同。这是因为不同基类子对象在派生类对象中的位置不同。编译器会自动进行地址调整（thunk）。

---

### 四、虚析构函数

#### 4.1 为什么需要虚析构函数

当通过基类指针删除派生类对象时，如果基类的析构函数不是虚函数，只会调用基类的析构函数，派生类的析构函数不会被调用，导致资源泄漏。

```cpp
class Base {
public:
    ~Base() { cout << "Base 析构" << endl; }  // 不是虚函数！
};

class Derived : public Base {
public:
    ~Derived() { cout << "Derived 析构" << endl; }
};

Base* p = new Derived();
delete p;  // 只调用 Base::~Base()！Derived::~Derived() 不会被调用！
```

#### 4.2 虚析构函数的 vtable 表现

```
非虚析构函数时：
  Base 的 vtable：
    [0] &Base::func1
    （没有析构函数条目）

  delete p 时：直接调用 Base::~Base()

虚析构函数时：
  Base 的 vtable：
    [0] &Base::func1
    [1] &Base::~Base()         ← 析构函数在 vtable 中

  Derived 的 vtable：
    [0] &Derived::func1
    [1] &Derived::~Derived()   ← 派生类的析构函数

  delete p 时：通过 vtable 调用 Derived::~Derived()
    → 先执行 Derived 析构函数体
    → 再自动调用 Base 析构函数
```

#### 4.3 虚析构函数的调用顺序

```cpp
delete p;  // p 是 Base*，实际指向 Derived 对象

// 实际调用顺序：
// 1. Derived::~Derived() 的函数体
// 2. Base::~Base() 的函数体
// 3. 释放内存
```

#### 4.4 纯虚析构函数

有时候基类是抽象类，但你又希望它有虚析构函数。这时可以使用**纯虚析构函数**：

```cpp
class Base {
public:
    virtual ~Base() = 0;  // 纯虚析构函数
};

// 纯虚析构函数必须提供实现！
Base::~Base() {
    cout << "Base 析构" << endl;
}
```

**为什么纯虚析构函数需要实现？** 因为在析构派生类对象时，基类的析构函数总是会被调用。如果基类析构函数没有实现，链接时会报错。

---

### 五、虚函数的性能开销

#### 5.1 虚函数的开销来源

```
虚函数的性能开销：

  1. 空间开销
     - 每个对象多一个 vptr（通常 8 字节，64 位系统）
     - 每个类多一个 vtable（每个虚函数一个指针，通常 8 字节）
     - 多重继承时可能有多个 vptr

  2. 时间开销
     - 每次虚函数调用需要一次额外的间接寻址（查 vtable）
     - 可能影响 CPU 分支预测和指令缓存
     - 编译器更难进行内联优化

  3. 间接开销
     - 虚函数调用通常不能被内联（除了去虚化优化）
     - 影响编译器优化空间
```

#### 5.2 开销有多大

```
虚函数调用 vs 普通函数调用：

  普通函数调用：
    call func_addr        ← 直接跳转，1 条指令

  虚函数调用：
    mov rax, [obj]        ← 取 vptr
    mov rax, [rax+offset] ← 查 vtable
    call rax              ← 间接调用

  额外开销：约 2-3 条指令 + 一次内存访问
  在现代 CPU 上，这通常只有几个纳秒的差距
```

#### 5.3 什么时候需要考虑虚函数的开销

```
需要考虑的场景：
  - 性能极其敏感的代码（游戏引擎、高频交易、信号处理）
  - 在 tight loop 中大量调用虚函数
  - 嵌入式系统，内存极其有限
  - 实时系统，需要确定性的执行时间

不需要考虑的场景：
  - 大多数业务逻辑代码
  - 虚函数调用不在性能热点上
  - 开发效率和可维护性更重要

优化建议：
  - 如果确定不需要多态，就不要用虚函数
  - 使用 final 关键字帮助编译器去虚化
  - 使用 CRTP（Curiously Recurring Template Pattern）实现静态多态
  - 考虑使用 std::variant（C++17）替代简单的多态层次
```

#### 5.4 编译器的去虚化优化

现代编译器可以进行**去虚化（Devirtualization）**优化：

```cpp
class Dog : public Animal {
public:
    void speak() override { cout << "汪汪汪" << endl; }
};

Dog dog;
dog.speak();  // 编译器知道 dog 的确切类型，可以内联！
```

当编译器能确定对象的实际类型时，就可以跳过 vtable 查找，直接调用目标函数，甚至进行内联优化。

---

### 六、面试真题

#### 真题 1：虚函数的实现机制？

**答：** 编译器为每个含有虚函数的类创建一个虚函数表（vtable），表中按顺序存储了该类所有虚函数的地址。每个对象内部有一个指向其类 vtable 的指针（vptr），通常放在对象内存的起始位置。通过基类指针调用虚函数时，运行时先通过对象的 vptr 找到 vtable，再从 vtable 中取出对应的函数指针进行调用。这就是动态绑定的底层实现。

#### 真题 2：多重继承下有几个 vptr？

**答：** 如果派生类的多个基类都有虚函数（即都有自己的 vtable），那么派生类对象中会有多个 vptr，每个有虚函数的基类对应一个。如果某个基类没有虚函数，则不会有对应的 vptr。多重继承下，将派生类指针赋给不同基类指针时，地址可能不同（编译器自动偏移），因为不同基类子对象在派生类对象中的位置不同。

#### 真题 3：构造函数中调用虚函数会怎样？

**答：** 在基类构造函数中调用虚函数时，调用的是基类自己的版本，不会产生多态效果。因为在构造基类部分时，派生类部分还没有被构造，此时 vptr 指向基类的 vtable。同理，在析构函数中调用虚函数也只会调用当前类（正在析构的类）的版本。

#### 真题 4：虚函数表存在哪里？

**答：** 虚函数表通常存在进程的只读数据段（.rodata）。vtable 在编译期生成，是类级别的，同一类的所有对象共享同一个 vtable。vptr 存在对象的内存中，在构造函数中被初始化。

#### 真题 5：虚函数的性能开销有哪些？如何优化？

**答：** 虚函数的开销包括：
1. 空间开销：每个对象多一个 vptr（8 字节），每个类多一个 vtable
2. 时间开销：每次调用需要间接寻址（查 vtable），约 2-3 条额外指令
3. 优化障碍：虚函数调用难以内联，影响编译器优化

优化方法：
- 不需要多态时不要用虚函数
- 使用 `final` 关键字帮助编译器去虚化
- 使用 CRTP 实现静态多态
- 使用 `std::variant`（C++17）替代简单的多态层次

#### 真题 6：纯虚析构函数为什么需要实现？

**答：** 因为在析构派生类对象时，基类的析构函数总是会被自动调用（析构顺序是从派生类到基类）。如果纯虚析构函数没有实现，链接时会报错。纯虚析构函数是唯一一个必须提供实现的纯虚函数。

---

### 完整示例代码

本节所有概念的演示代码请参考：

virtual_demo.cpp

编译运行：

```bash
g++ -std=c++17 -Wall -o virtual_demo virtual_demo.cpp
./virtual_demo
```

---

### 上一节

[2.4 智能指针](docs__02-cpp-oop__smart-pointer__README.md)

---

> 本教程由**青松与桑叶**编写，面向 C/C++ 面试准备的保姆级中文教学。

# docs__C++primer5笔记代码资料__第一章__C++primer5第一章代码.md


## 1.2 初识C++的输入输出

```
#include <iostream>
int main()
{
    std::cout << "Enter two numbers:" << std::endl;
    int v1 = 0, v2 = 0;
    std::cin >> v1 >> v2;
    std::cout << "The sum of " << v1 << " and " << v2 << " is " << v1 + v2 << std::endl;
    return 0;
}
```

### 测试缓冲区代码

```
void test_stream1() {
    cout << "代码随想录";
}

void test_stream2() {
    cout << "代码随想录";
    while (1) {
    }
}

void test_stream3() {
    cout << "代码随想录"<< endl;
    while (1) {
    }
}

```

## 1.4.3

```
#include <iostream>
int main()
{
    int sum = 0, value = 0;
    // read until end-of-file, calculating a running total of all values read
    while (std::cin >> value)
        sum += value; // equivalent to sum = sum + value
    std::cout << "Sum is: " << sum << std::endl;
    return 0;
}
```

## 1.5 类

```
#include <iostream>
#include "Sales_item.h"
int main()
{
    Sales_item book;
    // read ISBN, number of copies sold, and sales price
    std::cin >> book;
    // write ISBN, number of copies sold, total revenue, and average price
    std::cout << book << std::endl;
    return 0;
}

```

## 1.6

```
#include <iostream>
#include "Sales_item.h"
int main()
{
    Sales_item total; // variable to hold data for the next transaction
    // read the first transaction and ensure that there are data to process
    if (std::cin >> total) {
        Sales_item trans; // variable to hold the running sum
        // read and process the remaining transactions
        while (std::cin >> trans) {
            // if we're still processing the same book
            if (total.isbn() == trans.isbn())
                total += trans; // update the running total
            else {
                // print results for the previous book
                std::cout << total << std::endl;
                total = trans; // total now refers to the next book
            }
        }
        std::cout << total << std::endl; // print the last transaction
    } else {
        // no input! warn the user
        std::cerr << "No data?!" << std::endl;
        return -1; // indicate failure
    }
    return 0;
}
```



# problems__#define和const的区别有那些？.md

1. 作用域不同：
   - \#define定义的常量是一个预处理宏，它在编译之前被替换，作用域为定义处到文件结束。
   - const定义的常量是一个真正的变量，其作用域根据定义的位置而定，可以是局部或全局作用域。
2. 类型安全性：
   - \#define不具有类型检查，在预处理阶段只是简单地进行文本替换，容易导致一些潜在的错误。
   - const定义的常量具有类型检查，编译器会对其进行类型检查，提供更好的类型安全性。
3. 调试信息：
   - \#define在预处理阶段进行文本替换，因此在调试时无法查看使用#define定义的常量的值。
   - const定义的常量是真正的变量，可以被调试器识别并显示其值。

# problems__C++中struct和class有什么区别？.md

1. 默认的访问权限：
   - 在struct中，默认的成员变量和成员函数的访问权限是public的，意味着它们可以被外部访问。
   - 在class中，默认的成员变量和成员函数的访问权限是private的，意味着它们只能够在类的内部访问。
2. 继承方式：
   - 在struct中，继承的默认访问权限是public的，派生类可以访问基类的public和protected成员。
   - 在class中，继承的默认访问权限是private的，派生类可以访问基类的public和protected成员。

# problems__C++中动态链接库和静态连接库的区别是什么？.md

#### 1. 链接时期

- **静态链接库**：在程序编译时，静态库的内容会被复制到最终的可执行文件中。当你运行程序时，不需要库文件，因为所有的功能都已经包含在可执行文件里了。
- **动态链接库**：程序在编译时并不复制库中的代码，而是在程序运行时加载库文件。这意味着库文件必须在程序运行时可用。

#### 2. 文件大小

- **静态链接库**通常会导致较大的可执行文件大小，因为所有使用的库代码都被复制进去了。
- **动态链接库**允许可执行文件小一些，因为代码是在运行时才被加载。

#### 3. 内存占用

- **静态链接库**的缺点是如果有多个程序使用相同的库，每个程序都有自己的副本，这将导致内存的浪费。
- **动态链接库**可以由多个正在运行的程序共享，只需在内存中有一个副本即可。

#### 4. 分发和更新

- **静态链接库**使得更新库变得复杂，因为每个应用都有自己的副本，所以每个应用都需要重新编译和分发。
- 使用**动态链接库**时，只需替换库文件并且确保API兼容性，所有使用该库的应用程序就可以直接利用新版本的库，无须重新编译。

#### 5. 跨平台兼容性

- **静态链接库**生成的可执行文件更易于在没有安装相应库的不同系统上运行，因为它们包括了所有需要的代码。
- 对于**动态链接库**，需要确保目标系统上存在正确版本的库文件。

#### 6. 链接错误和冲突

- **静态链接库**可能会引起版本冲突问题，尤其是当不同的库依赖同一个库但又各自静态链接了不同版本时。
- **动态链接库**可以减少这种冲突，因为同一份库文件被所有依赖它的程序共享。

# problems__C++中右值引用有什么作用？.md

C++11引入了右值引用，用来支持移动语义和完美转发。

1. 移动语义：传统的复制操作需要额外的时间和空间，而有了移动语义后，可以直接将资源（如内存）从一个对象转移到另一个对象，而不必创建并删除临时对象。这对于大对象或者拥有独占所有权资源的对象特别有用。例如，unique_ptr和std::vector等STL容器就利用了移动语义实现了高效的操作。
2. 完美转发：在函数模板中，我们想把参数原封不动地传递给其他函数。由于传参可能存在值传递、左值引用、常量左值引用、右值引用等情况，为了保证参数的属性和类型不发生变化，我们需要使用std::forward实现完美转发。

右值引用主要用于两种场景：一是对象的移动（Move），二是万能引用（Forwarding Reference）。对于第一种情况，它是为了解决对象的复制效率问题；对于第二种情况，则是为了实现参数的完美传递，避免不必要的拷贝。

# problems__C++中四种cast的转换？.md



1. **static_cast**： `static_cast` 是用于编译时检测到的相关类型之间的转换，比如整型和浮点数、派生类和基类之间的指针或引用转换。它不能用于含有虚函数的多态基类指针或引用到派生类的转换，因为这需要运行时信息。

   示例：

   ```
   int i = 10;
   float f = static_cast<float>(i);    // 整型到浮点数的转换
   ```

2. **dynamic_cast**： `dynamic_cast` 主要用于处理多态性。当涉及到继承体系中的向下转换（将基类的指针或引用转换为派生类类型）时，这个转换会检查转换的安全性。如果转换无效，对于指针，它会返回nullptr；对于引用，则抛出一个`std::bad_cast`异常。使用`dynamic_cast`需要运行时类型信息（RTTI），因此它有一定的性能代价。

   示例：

   ```
   Base* b = new Derived();   // 基类指针指向派生类对象
   Derived* d = dynamic_cast<Derived*>(b);   // 向下转型成功
   if (d) {
       // 转型有效，'d' 不是 nullptr
   }
   ```

3. **const_cast**： `const_cast` 用于移除或添加`const`或`volatile`属性。通常情况下，它被用于移除对象的常量性，允许修改原本被声明为`const`的变量。需要注意的是，去除一个本质上确实是常量的对象的`const`标记并进行修改可能会导致未定义行为。

   示例：

   ```
   const int ci = 10;
   int& modifiable = const_cast<int&>(ci);   // 移除常量性以便修改
   modifiable = 20;  // 注意：如果原对象真的是const，这里可能是未定义行为
   ```

4. **reinterpret_cast**： `reinterpret_cast` 是最危险的cast，它能够执行低级的强制类型转换。尽管几乎没有任何语义检查，但它能够在几乎任意两种类型之间转换，例如整数与指针之间的转换。由于它的不安全性，应该尽可能避免使用`reinterpret_cast`，除非你完全理解所进行的转换。

   示例：

   ```
   int* p = new int(65);
   char* ch = reinterpret_cast<char*>(p);  // 强制指针类型转换
   ```

总结：

- `static_cast`在相关类型间做安全转换。
- `dynamic_cast`在类层次结构中转换，并支持运行时检查。
- `const_cast`改变类型的`const`或`volatile`限定。
- `reinterpret_cast`进行低级别、不安全的强制类型转换。

# problems__C++中常用的类优化技术有那些？.md

1. 使用成员函数而非友元函数：将函数作为类的成员函数而不是友元函数可以提高封装性和代码可读性，并避免频繁地访问类的私有成员。
2. 使用引用参数而非传值参数：通过使用引用参数而不是传值参数来传递参数，可以减少内存开销和提高程序性能。特别是对于大型对象，避免了不必要的对象拷贝操作。
3. 使用移动语义：对于需要频繁进行资源管理的类，例如具有动态分配内存的类或具有文件句柄等资源的类，使用移动语义可以避免不必要的复制开销，提高程序效率。
4. 使用智能指针：在需要动态内存管理的情况下，使用智能指针（如 std::shared_ptr、std::unique_ptr 等）可以避免内存泄漏和悬空指针问题，提高代码的安全性和可靠性。
5. 使用const成员函数：将不会修改对象状态的函数声明为 const 成员函数可以提高类的接口清晰度，并增强代码的可维护性。
6. 避免不必要的动态内存分配：在设计类时，可以考虑避免过多的动态内存分配，尽量减少内存申请和释放的次数，以提高程序的性能和稳定性。
7. 使用内联函数：将简单的、频繁调用的函数声明为内联函数可以减少函数调用的开销，提高程序的执行效率。

# problems__C++中类成员的访问权限有那些？.md

1. **Public**:
   - 使用`public`标签指定的成员可以被任何访问该类对象的代码访问。
   - 公开成员定义了类的外部接口。
2. **Protected**:
   - 使用`protected`标签指定的成员只能被以下几种代码访问：
     - 类本身内部的成员函数。
     - 继承自该类的派生类中的成员函数。
   - 保护成员通常用于在基类和派生类之间共享数据或行为，同时对类的其他使用者隐藏这些细节。
3. **Private**:
   - 使用`private`标签指定的成员只能被类本身内部的成员函数（以及其友元）访问。
   - 私有成员是实现类内部封装的关键，防止了对类实现细节的外部访问。

下面是一个简单的类声明示例，展示了如何使用这三种不同的访问说明符：

```c++
class MyClass {
public:    // 公开成员
    int publicVariable;

    void publicMethod() {
        // ...
    }

protected: // 保护成员
    int protectedVariable;

    void protectedMethod() {
        // ...
    }

private:   // 私有成员
    int privateVariable;

    void privateMethod() {
        // ...
    }
};
```

# problems__C++中结构体内存布局的规则是什么？.md

1. **成员顺序**: 结构体的成员变量在内存中按它们声明的顺序依次排列。
2. **数据对齐**: 为了提高访问速度，编译器会根据目标平台的要求对结构体成员进行对齐。这可能导致在成员之间或结构体末尾存在填充字节（padding）。
   - 对齐规则通常要求一个类型的数据地址必须是其大小的整数倍。例如，一个4字节的`int`通常需要放置在地址为4的倍数的位置上。
   - `#pragma pack`或编译器特定属性可用于改变或禁用默认的对齐行为。
3. **继承**: 如果结构体是从一个或多个其他结构体/类继承而来，则基类的成员将首先出现在派生类对象的内存中，后面跟着派生类自己的成员。
4. **虚函数**: 如果结构体有虚函数，编译器通常会在结构体的内存布局中加入一个指向虚函数表（vtable）的指针。这个指针位于对象的开始处，但具体位置取决于编译器的实现。
5. **虚继承**: 使用虚继承时，为了解决菱形继承问题，编译器会采取复杂的策略来安排内存布局，这通常涉及额外的指针和调整对象模型。
6. **静态成员**: 静态成员变量不作为结构体的一部分存储在每个对象的内存中，它们在全局/静态存储区有单独的存储空间。
7. **位域**: 如果结构体使用了位域，则相邻的位域可以被紧密打包以减少空间占用。但是，如果跨越了底层类型的边界，位域可能会被分割开。
8. **零大小数组**: 某些编译器允许在结构体末尾使用零大小数组作为柔性数组成员（flexible array member），而这通常不占用结构体的内存空间，只是作为一个符号占位符。

# problems__C++中，结构体可以直接赋值吗？.md

结构体是一种用户自定义的数据类型，可以**像内置数据类型一样进行赋值操作**。结构体可以直接进行赋值，包括使用另外一个同类型的结构体来对它进行赋值。

```
struct Point {
    int x;
    int y;
};

int main() {
    Point p1 = {3, 4};
    Point p2;
    p2 = p1; // 可以直接将p1的值赋给p2
}

```


# problems__C++内存分配.md


内存分配方式有三种：

（1）从静态存储区域分配。内存在程序编译的时候就已经分配好，这块内存在程序的整个运行期间都存在。例如全局变量，static变量。

（2）在栈上创建。在执行函数时，函数内局部变量的存储单元都可以在栈上创建，函数执行结束时这些存储单元自动被释放。栈内存分配运算内置于处理器的指令集中，效率很高，但是分配的内存容量有限。

（3）从堆上分配，亦称动态内存分配。程序在运行的时候用malloc或new申请任意多少的内存，程序员自己负责在何时用free或delete释放内存。动态内存的生存期由我们决定，使用非常灵活，但问题也最多。

c++运行时各类型内存分配（堆，栈，静态区，数据段，BSS，ELF），BSS段，
sizeof一个类求大小（字节对齐原则）、


# problems__C++多态的实现有那几种？他们有什么不同？.md

C++中，多态性主要通过两种方式实现：编译时多态（静态多态）和运行时多态（动态多态）。这两种多态的机制、特点和用途有所不同。

1. **编译时多态（静态多态）**: 编译时多态是在程序编译阶段实现的多态性。主要通过函数重载和运算符重载来实现。

   - **函数重载**: 同一个作用域内存在多个同名函数，但它们的参数类型或数量不同。根据调用时实际传递的参数类型和数量，编译器决定调用哪个函数。
   - **运算符重载**: 允许定义或重新定义大部分C++内置的运算符，使得它们可以根据操作数的类型执行不同的操作。

   **编译时多态的决策是在编译时做出的**，因此它不支持在运行时根据对象的实际类型来选择相应的成员函数。

2. **运行时多态（动态多态）**: 运行时多态是在程序运行阶段实现的多态性。它主要通过虚函数和继承来实现。

   - **虚函数**: 通过在基类中声明虚函数，允许派生类中重写该函数。当通过基类的指针或引用调用虚函数时，实际执行的是与指针或引用所指对象的实际类型相对应的函数版本。
   - **抽象类和纯虚函数**: 抽象类至少包含一个纯虚函数。纯虚函数在基类中没有实现，派生类必须重写这个函数。抽象类不能被实例化。

   **运行时多态的决策是在程序运行时做出的**，这就**需要运行时类型信息和虚函数表（vtable）**。在运行时，根据对象的实际类型来动态调用相应的成员函数，从而实现多态。

总的来说：

- **编译时多态**是静态的，主要通过函数重载和运算符重载来实现，决策发生在编译阶段。
- **运行时多态**是动态的，需要虚函数机制，并且决策发生在程序运行时。
- **运行时多态**能够提供更高的灵活性和扩展性，是实现框架和库中一些高级功能（如插件架构或事件处理系统）的关键。
- **编译时多态**由于在编译期就已经确定了调用哪个函数，通常性能更高，因为它避免了运行时查找虚函数表的开销。

# problems__C++的atomic-bool代码底层是如何实现的？.md

在C++中，`std::atomic` 是一个模板类，用于提供对基础类型的原子操作。`std::atomic<bool>` 是该模板类针对布尔类型的特化。原子操作保证了即使在多线程环境中，每个操作也是不可分割的，从而避免了竞态条件。

`std::atomic<bool>` 底层实现通常依赖于硬件和编译器的支持来提供原子性保证。具体实现可能涉及以下几个方面：

1. **内存屏障**（Memory Barriers/Fences）： 内存屏障用于确保指令的执行顺序，阻止编译器或者处理器重排操作顺序。
2. **锁前缀指令**（Lock Prefix）： 在x86架构中，处理器提供了带有`LOCK`前缀的指令，比如`LOCK XCHG`，它可以将操作变为原子性的。当CPU执行带有`LOCK`前缀的指令时，会确保指令完整执行，期间不会被其他处理器打断。
3. **特殊的原子指令**： 现代处理器提供了一系列原子指令，比如`XADD`（交换并加），`CMPXCHG`（比较并交换）等，可以用来实现原子变量。对于布尔变量，可能会使用这些指令来实现无锁的原子读写操作。
4. **编译器内建函数**（Compiler Intrinsics）： 编译器可能提供特殊的内建函数来映射到底层的原子指令。

例如，在GCC和Clang上，通常会使用GCC的内建函数来实现原子操作。例如：

```
bool old_value = __atomic_fetch_and(&my_atomic_bool, true, __ATOMIC_SEQ_CST);
```

这里的 `__atomic_fetch_and` 函数是GCC提供的内建函数，用于执行原子AND操作，并返回变量的旧值。第三个参数 `__ATOMIC_SEQ_CST` 表示使用最严格的内存顺序：Sequentially Consistent。

对于不支持原子指令的数据类型或复杂操作，可能需要使用锁（比如互斥锁）来保证操作的原子性。但由于 `bool` 类型非常简单，大多数平台都能够提供无锁的原子操作支持。

不同的平台和编译器可能有不同的实现方式，因此没有一个统一的实现细节。如果你想知道具体的实现，可以查看特定编译器的源代码或者汇编输出。

# problems__C++的智能指针及其原理.md

#### 智能指针简介

C++的智能指针是一种用于管理动态分配内存的对象，它们提供了自动内存管理机制，避免了手动释放内存的繁琐和潜在的内存泄漏问题。

智能指针的原理基于RAII原则，即资源获取即初始化。智能指针通过在构造函数中获取资源（动态分配的内存），并在析构函数中释放资源，从而确保资源的正确释放。

C++标准库提供了两种主要的智能指针：`std::unique_ptr`和`std::shared_ptr`。

1. `std::unique_ptr`： `std::unique_ptr`是独占所有权的智能指针，它确保只有一个指针可以访问所管理的对象。当`std::unique_ptr`被销毁时，它会自动释放所拥有的对象。它不能被复制，但可以通过std::move()函数进行所有权转移。
2. `std::shared_ptr`： `std::shared_ptr`是共享所有权的智能指针，它可以被多个指针同时访问和共享所管理的对象。它使用引用计数来追踪有多少个指针指向该对象，当引用计数为0时，即没有任何指针指向对象时，资源会被释放。`std::shared_ptr`可以被复制和赋值。



注意：weak_ptr严格来说，不能算是“智能指针”，他只是一个类的弱引用，是用来解决两个`std::shared_ptr`相互进行引用的问题的



#### 侵入式和非侵入式的智能指针

在C++中，智能指针又被分为两种:侵入式和非侵入式的

1. **侵入式智能指针**： 侵入式智能指针需要被管理的类提供特定的接口或继承指定的基类，以支持智能指针的操作。这意味着被管理的类必须拥有与智能指针相关的成员函数或遵循特定的约定。侵入式智能指针可以更好地控制资源的生命周期，但需要修改被管理类的定义。

   例如，Boost库中的`boost::intrusive_ptr`就是一种侵入式智能指针。被管理的类必须实现`add_ref()`和`release()`等函数，以增加和释放引用计数。

2. **非侵入式智能指针**： 非侵入式智能指针不需要被管理的类提供额外的接口或继承特定的基类。它通过自身的机制来管理资源的生命周期，而不需要对被管理的类做任何修改。这样可以更方便地将智能指针应用到已有的类中。

   C++标准库中的`std::shared_ptr`就是一种非侵入式智能指针。它可以管理任何动态分配的对象，而不需要对被管理的类做任何特殊要求。

非侵入式智能指针相对于侵入式智能指针更加灵活和方便，但在某些情况下侵入式智能指针可能提供更细粒度的资源管理控制。选择使用哪种类型的智能指针取决于具体的需求和设计考虑。

# problems__C++结构体内存对齐.md

C++中的结构体内存对齐是为了提高内存访问效率而采用的一种内存布局优化方式。在结构体中，根据处理器的架构和编译器设定的规则，可能会自动插入填充字节（padding），以确保结构体的成员变量按照一定的对齐方式存储。

以下是结构体内存对齐的基本原则：

1. 结构体的起始地址能够被其最宽基本类型成员的大小所整除。
2. 结构体中每个成员相对于结构体起始地址的偏移量（offset）都是该成员类型大小的整数倍，这就可能造成内存空间的浪费，即前面提到的填充字节。
3. 结构体的总大小为结构体最宽基本类型成员大小的整数倍，如果不足，会在结构体末尾添加填充字节。
4. 如果结构体中包含其他结构体或联合体，那么也需要按照这些内部结构体或联合体的对齐要求来对齐。

举例说明：

```
struct MyStruct {
    char a;        // 1字节
    int b;         // 4字节
    short c;       // 2字节
};
```

在上述结构体中，`int` 类型通常需要按照4字节对齐，在32位或64位架构下。假设编译器按照4字节对齐规则对该结构体进行内存对齐，则实际内存布局可能如下：

- `char a;` 占用1字节，后面跟着3字节的填充，以确保接下来的 `int b;` 能在4字节边界上对齐。
- `int b;` 直接按照4字节对齐，紧接着 `char a;` 后面的填充字节。
- `short c;` 占用2字节，并且因为已经处于4字节边界上，所以不需要额外填充。

结构体总大小：`sizeof(MyStruct)` 很可能是8个字节（1字节`char` + 3字节填充 + 4字节`int`），但实际上还有一个2字节的`short`，所以需要再加上2个字节的填充，使得整个结构体大小为12字节，满足最大对齐成员`int`的4字节的整数倍。

记住，具体的对齐情况取决于编译器设置（例如GCC的`__attribute__((packed))`、`#pragma pack`等）和目标平台的硬件架构。开发者可以通过这些手段来修改默认的对齐规则，以满足特定的内存或性能要求。

# problems__STL中一般都有那些常见的算法库呢？.md

一般常见的STL算法库包括：

1. 非修改性序列操作算法：如std::for_each, std::count, std::find, std::binary_search等。
2. 修改性序列操作算法：如std::sort, std::reverse, std::swap, std::rotate等。
3. 排列组合算法：如std::next_permutation, std::prev_permutation, std::merge等。
4. 数值操作算法：如std::accumulate, std::inner_product, std::partial_sum等。
5. 堆操作算法：如std::make_heap, std::push_heap, std::pop_heap等。
6. 划分操作算法：如std::partition, std::stable_partition等。

# problems__STL中的优先级队列是如何实现的？.md

## STL中的优先级队列是如何实现的？

`std::priority_queue` 定义在 `<queue>` 头文件中，底层通常使用 `std::vector<T>` 作为容器，并通过堆来维护优先级。

- 默认比较器是 `std::less<T>`，因此默认是**最大堆**。
- 底层会借助 `std::make_heap`、`std::push_heap`、`std::pop_heap` 维护堆性质。
- `push()` 先插入元素，再上浮调整；`pop()` 先把堆顶交换到末尾，再下沉调整；`top()` 直接返回堆顶元素。
- 通过自定义比较器，可以把它改造成**最小堆**。

```C++
#include <queue>
#include <vector>
using namespace std;

priority_queue<int> maxq; // 默认最大堆

priority_queue<int, vector<int>, greater<int>> minq; // 最小堆
```

### 合并前来源内容保留

以下保留合并前两个 priority_queue 文件中的有效原说明；截断句已在上方正文修复。

#### 原「STL中的优先级队列」有效说明

在 C++ STL 中，`std::priority_queue` 默认情况下使用 `std::vector` 作为其底层容器，并且使用 `std::make_heap`、`std::push_heap` 和 `std::pop_heap` 算法来维护堆的性质。

#### 原「简单说说STL中的优先级队列」说明

在C++标准模板库（STL）中，优先级队列是通过一个称为堆的数据结构实现的，通常用一个向量（通常是 `std::vector`）来表示。具体来说，默认情况下，优先级队列使用最大堆来组织元素，这意味着队列顶部总是最大的元素。如果需要最小元素优先，可以通过提供自定义比较函数来实现最小堆。

优先级队列在STL中是用模板类 `std::priority_queue` 实现的，该类在 `<queue>` 头文件中定义。它允许插入和取出元素的操作，其中插入操作是将新元素添加到正确位置以保持堆的性质，取出操作是移除队列顶部的元素。其余的元素会重新排列以保持堆的性质，确保下一个最大（或最小，取决于比较函数）元素移动到队列顶部。

# problems__STL中，map的底层是如何实现的？.md

在C++标准模板库（STL）中，`map` 是一种关联容器，它以键值对的方式存储元素，其中每个键都是唯一的。底层实现通常使用红黑树（Red-Black Tree），这是一种自平衡的二叉搜索树。

红黑树保持了树的平衡性，即从根到所有叶子节点的最长路径不会超过最短路径的两倍。这种性质确保了`map`中的操作（如插入、删除和查找）可以在对数时间复杂度O(log n)内完成。

红黑树有以下特性：

1. 每个节点要么是红色，要么是黑色。
2. 根节点是黑色。
3. 所有叶子节点（NIL节点，空节点）都是黑色。
4. 每个红色节点必须有两个黑色的子节点（不能有两个连续的红色节点）。
5. 从任一节点到其每个叶子的所有简单路径都包含相同数目的黑色节点。

因为`map`的底层是红黑树这种高度平衡的数据结构，所以它能够提供良好的性能保证，使得即使在大量元素存储的情况下也能保持效率。

需要注意的是，在C++11后，还引入了`unordered_map`，它使用哈希表作为底层实现，提供平均常数时间复杂度O(1)的访问性能，但它不保证元素的顺序，并且在最坏情况下可能退化为线性时间复杂度O(n)。

# problems__STL中，set的底层是如何实现的？.md

在C++标准模板库（STL）中，`set` 是基于关联容器的一个抽象数据类型，用于存储不重复的元素。与 `map` 类似，`set` 的底层实现也通常采用红黑树（一种自平衡的二叉搜索树）。这使得 `set` 中的大多数操作（例如插入、删除和搜索）都能以对数时间复杂度 O(log n) 来执行，其中 n 是集合中元素的数量。

使用红黑树作为底层数据结构，`set` 可以保证元素会按照特定的顺序排序，通常是按照键值的递增顺序。红黑树确保了任何时候树都是相对平衡的，所以 `set` 容器在处理大量动态插入和删除操作时依然能够提供良好的性能。

除了 `set`，STL 还提供了 `unordered_set` 容器，其底层实现是基于哈希表。`unordered_set` 不保证元素的有序性，但在理想情况下可以提供更快的平均时间复杂度 O(1) 的访问性能。不过，在最坏的情况下（例如，当哈希函数导致很多碰撞时），它的性能可能会退化到 O(n)。

# problems__STL原理及实现.md



STL提供六大组件，彼此可以组合套用：

1、容器（Containers）：各种数据结构，如：序列式容器vector、list、deque、关联式容器set、map、multiset、multimap。用来存放数据。从实现的角度来看，STL容器是一种class template。

2、算法（algorithms）：各种常用算法，如：sort、search、copy、erase。从实现的角度来看，STL算法是一种 function template。注意一个问题：任何的一个STL算法，都需要获得由一对迭代器所标示的区间，用来表示操作范围。这一对迭代器所标示的区间都是前闭后开区间，例如[first, last)

3、迭代器（iterators）：容器与算法之间的胶合剂，是所谓的“泛型指针”。共有五种类型，以及其他衍生变化。从实现的角度来看，迭代器是一种将 operator * 、operator->、operator++、operator- - 等指针相关操作进行重载的class template。所有STL容器都有自己专属的迭代器，只有容器本身才知道如何遍历自己的元素。原生指针(native pointer)也是一种迭代器。

4、仿函数（functors）：行为类似函数，可作为算法的某种策略（policy）。从实现的角度来看，仿函数是一种重载了operator（）的class或class template。一般的函数指针也可视为狭义的仿函数。

5、配接器（adapters）：一种用来修饰容器、仿函数、迭代器接口的东西。例如：STL提供的queue 和 stack，虽然看似容器，但其实只能算是一种容器配接器，因为它们的底部完全借助deque，所有操作都由底层的deque供应。改变 functors接口者，称为function adapter；改变 container 接口者，称为container adapter；改变iterator接口者，称为iterator adapter。

6、配置器（allocators）：负责空间配置与管理。从实现的角度来看，配置器是一个实现了动态空间配置、空间管理、空间释放的class template。

这六大组件的交互关系：container（容器） 通过 allocator（配置器） 取得数据储存空间，algorithm（算法）通过 iterator（迭代器）存取 container（容器） 内容，functor（仿函数） 可以协助 algorithm（算法） 完成不同的策略变化，adapter（配接器） 可以修饰或套接 functor（仿函数）

### 序列式容器：

vector-数组，元素不够时再重新分配内存，拷贝原来数组的元素到新分配的数组中。
list－单链表。
deque-分配中央控制器map(并非map容器)，map记录着一系列的固定长度的数组的地址.记住这个map仅仅保存的是数组的地址,真正的数据在数组中存放着.deque先从map中央的位置(因为双向队列，前后都可以插入元素)找到一个数组地址，向该数组中放入数据，数组不够时继续在map中找空闲的数组来存数据。当map也不够时重新分配内存当作新的map,把原来map中的内容copy的新map中。所以使用deque的复杂度要大于vector，尽量使用vector。

stack-基于deque。
queue-基于deque。
heap-完全二叉树，使用最大堆排序，以数组(vector)的形式存放。
priority_queue-基于heap。
slist-双向链表。

### 关联式容器

set,map,multiset,multimap-基于红黑树(RB-tree)，一种加上了额外平衡条件的二叉搜索树。

hash table-散列表。将待存数据的key经过映射函数变成一个数组(一般是vector)的索引，例如：数据的key%数组的大小＝数组的索引(一般文本通过算法也可以转换为数字)，然后将数据当作此索引的数组元素。有些数据的key经过算法的转换可能是同一个数组的索引值(碰撞问题，可以用线性探测，二次探测来解决)，STL是用开链的方法来解决的，每一个数组的元素维护一个list，他把相同索引值的数据存入一个list，这样当list比较短时执行删除，插入，搜索等算法比较快。

hash_map,hash_set,hash_multiset,hash_multimap-基于hashtable。

什么是“标准非STL容器”？

### list和vector有什么区别？

vector拥有一段连续的内存空间，因此支持随机存取，如果需要高效的随即存取，而不在乎插入和删除的效率，使用vector。
list拥有一段不连续的内存空间，因此不支持随机存取，如果需要大量的插入和删除，而不关心随即存取，则应使用list。


# problems__STL容器是线程安全的吗？.md

C++标准库（STL）中的容器本身不是线程安全的。这意味着在没有采取外部同步措施的情况下，如果有多个线程同时对同一个容器实例进行写操作，或者同时有一个线程在写操作和另一个线程在读操作，那么这可能会导致数据竞争和未定义行为。

因此，当多个线程需要访问相同的容器时，就需要通过其他方式来确保线程安全。常用的同步机制包括：

1. **互斥锁（Mutexes）**：使用互斥锁来同步对容器的访问。例如，可以在每次操作容器之前加锁，操作完毕后解锁。
2. **读写锁（Reader-Writer Locks）**：如果你的应用程序涉及到更多的读操作而较少的写操作，可以使用读写锁来允许多个读取者同时访问容器，而写入者则需要独占访问权限。
3. **原子操作**：对于简单的操作，如对单个元素的更新，可以考虑使用原子类型 `std::atomic`。
4. **并发容器**：某些场景下可以使用专为并发设计的容器，如 `boost` 库提供的一些线程安全版本的容器，或者 `tbb::concurrent_vector` 等。
5. **细粒度锁或无锁编程技术**：在高级应用程序中，可能会使用更复杂的策略，比如分段锁或无锁数据结构，以减小锁的粒度或避免锁的开销。

# problems__git的merge和rebase有什么区别.md

Git的merge和rebase都是将两个分支合并的方法，但它们的实现方式不同，会对代码库的历史记录造成影响。

merge是将两个分支上的更改集合合并到一起，并形成一个新的提交节点。在merge时，Git 会自动创建一个新的提交节点，这个节点包含了两个分支的更改，同时保留了两个分支的历史记录。这样做可以避免各种冲突，并且保证每个分支的历史记录都被记录下来。

而rebase则是将当前分支的更改重新基于另一个分支进行重放。重放是指将当前分支中的所有提交按序应用到目标分支的最新提交之后。这样做的效果是让当前分支看起来像是从目标分支最新提交开始开发的，避免了一些无意义的merge节点，并且让提交历史线变得更加清晰。然而，rebase会对提交历史进行修改，因为它会把当前分支的提交作为全新的提交，而不是merge提交。

# problems__lower_bound()和upper_bound()有什么区别？.md

1. **lower_bound()**:
   - 返回一个指向范围内第一个**不小于**（即大于或等于）给定值的元素的迭代器。
   - 如果给定值不存在于容器中，该函数返回指向第一个大于该值的元素的迭代器。如果所有元素都小于给定值，函数将返回一个指向容器末尾（end）的迭代器。
2. **upper_bound()**:
   - 返回一个指向范围内第一个**大于**给定值的元素的迭代器。
   - 如果所有元素都小于或等于给定值，函数将返回一个指向容器末尾（end）的迭代器。

**举例说明**:

假设我们有一个包含 {1, 2, 4, 4, 5, 6, 8} 的整数vector，并且我们想要搜索数字4。

- 使用 `lower_bound()` 寻找4会返回指向第一个数字4的迭代器，因为4是数组中第一个"不小于"4的值。
- 使用 `upper_bound()` 寻找4会返回指向数字5的迭代器，这是因为5是数组中第一个"大于"4的值。

# problems__override和overload的区别有那些.md

函数重载（Overload）：当一个作用域内有两个或更多个函数名相同但参数列表不同的函数时，我们就说这些函数构成了重载。参数列表不同可以是参数数量不同，也可以是参数类型不同。函数重载使得我们可以使用一样的函数名来完成类似的操作，提高代码的可读性和易用性。

示例：

```c++
void foo(int a);
void foo(double a);
```

函数覆盖（Override）：当一个派生类声明了一个与基类中虚函数完全相同（函数名、参数类型和个数、常量属性、返回值类型）的函数时，我们就说派生类的这个函数覆盖了基类的虚函数。这使得我们可以通过基类指针或引用来调用派生类的函数，实现多态。

示例：

```c++
class Base {
public:
    virtual void foo(int a);
};

class Derived : public Base {
public:
    void foo(int a) override; // 覆盖基类的虚函数foo
};
```


# problems__set，multiset，map，multimap之间都有什么区别？.md

1. `set`：
   - 存储唯一键值的有序集合，不允许重复元素。
   - 元素本身就是键值。
   - 默认按递增顺序存储。
2. `multiset`：
   - 类似 `set`，但允许重复元素。
   - 元素同样按顺序存储。
3. `map`：
   - 存储键值对（`pair`），每个键唯一。
   - 键值对按键的顺序存储。
4. `multimap`：
   - 类似 `map`，但允许键重复。
   - 键值对同样按键的顺序存储。

使用场景概述：

- 需要不重复的有序集合时，用 `set`。
- 需要允许重复的有序集合时，用 `multiset`。
- 需要一对一映射时，用 `map`。
- 需要一对多映射时，用 `multimap`。

这四类容器通常都基于红黑树实现；如果不需要排序，可以考虑 `unordered_set`、`unordered_multiset`、`unordered_map`、`unordered_multimap`。

### 合并前来源内容保留

以下保留改名前原文件中的有效说明；文件名中的 multiset/multimap 拼写已修正为 multiset/multimap。

1. `set`:
   - 存储唯一键值的集合，即不允许重复的元素。
   - 元素本身就是键值。
   - 元素按照特定顺序存储（默认为递增顺序）。
2. `multiset`:
   - 类似于 `set`，但它允许重复的键值，即可以有多个相等的元素。
   - 元素同样按照特定顺序存储。
3. `map`:
   - 存储键值对（pair），每个键值对由一个键和一个值组成。
   - 每个键在 `map` 中是唯一的，你不能有两个具有相同键的键值对。
   - 键值对按照键的顺序存储。
4. `multimap`:
   - 类似于 `map`，但它允许键不唯一，即可以有多个键值对拥有相同的键。
   - 键值对同样按照键的顺序存储。

使用情况概述：

- 当你需要存储不重复元素的有序集合时，使用 `set`。
- 当你需要存储可能重复元素的有序集合时，使用 `multiset`。
- 当你需要维护一组键到值的映射，并且每个键只能关联一个值时，使用 `map`。
- 当你需要维护一组键到值的映射，并且一个键可以关联多个值时，使用 `multimap`。

所有这四种类型的底层实现通常是红黑树，除了提供插入、删除和搜索操作外，还保证了元素的有序性。然而，如果你不需要元素的排序，并且关注更高效的插入和查找性能，可以选择使用基于哈希表实现的 `unordered_set`, `unordered_multiset`, `unordered_map`, `unordered_multimap` 等无序容器。

# problems__weak_ptr是如何解决shared_ptr循环引用的？.md

当两个对象相互引用并使用`shared_ptr`时，就会形成循环引用。例如，考虑一个简单的场景：

```
#include <memory>
#include <iostream>

class B; // 前置声明

class A {
public:
    std::shared_ptr<B> b_ptr;
    A() { std::cout << "A constructor" << std::endl; }
    ~A() { std::cout << "A destructor" << std::endl; }
};

class B {
public:
    std::shared_ptr<A> a_ptr;
    B() { std::cout << "B constructor" << std::endl; }
    ~B() { std::cout << "B destructor" << std::endl; }
};

int main() {
    std::shared_ptr<A> a = std::make_shared<A>();
    std::shared_ptr<B> b = std::make_shared<B>();

    a->b_ptr = b;
    b->a_ptr = a;

    return 0;
}
```

在这个例子中，类 `A` 拥有一个指向类 `B` 的 `shared_ptr`，而类 `B` 拥有一个指向类 `A` 的 `shared_ptr`。这样就形成了循环引用。

为了避免循环引用，我们可以改用 `weak_ptr` 来解决这个问题：

```
#include <memory>
#include <iostream>

class B; // 前置声明

class A {
public:
    std::shared_ptr<B> b_ptr;
    A() { std::cout << "A constructor" << std::endl; }
    ~A() { std::cout << "A destructor" << std::endl; }
};

class B {
public:
    std::weak_ptr<A> a_weak_ptr;  // 使用 weak_ptr
    B() { std::cout << "B constructor" << std::endl; }
    ~B() { std::cout << "B destructor" << std::endl; }
};

int main() {
    std::shared_ptr<A> a = std::make_shared<A>();
    std::shared_ptr<B> b = std::make_shared<B>();

    a->b_ptr = b;
    b->a_weak_ptr = a;  // 使用 weak_ptr

    return 0;
}
```

通过将类 `B` 中指向类 `A` 的指针改为 `weak_ptr`，我们成功地避免了循环引用问题。

# problems__为什么需要allocator？他在STL中有什么作用？.md

#### 1. 抽象化内存管理

- `allocator` 提供了一个抽象层，使得容器能够专注于数据结构和算法的实现，而不必担心具体的内存分配和回收细节。这样，容器的设计和实现就可以独立于底层的内存管理机制。

#### 2. 提供统一的接口

- 所有的 STL 容器都使用相同的 `allocator` 接口来分配和释放内存。这提供了一致性，并且使得开发者在需要时能够更容易地替换默认的内存分配策略。

#### 3. 支持自定义内存管理策略

- 通过自定义 `allocator`，开发者可以根据应用程序的特定需求调整内存分配策略。例如，在特定场景下可能需要一个高性能的内存池分配器，或者跟踪内存使用情况的分配器，这些都可以通过自定义 `allocator` 来实现。

#### 4. 性能优化

- 默认的内存分配器可能并不总是满足特定应用程序的性能需求。通过使用自定义的 `allocator`，开发者可以利用特定的内存分配技巧（如小对象优化、内存池分配等）来提升性能。

#### 5. 内存对齐

- 对于某些特定类型的对象，可能需要特殊的内存对齐以达到最佳性能或满足硬件要求。`allocator` 允许对这些对象的内存分配进行适当的对齐处理。

# problems__为什么需要虚析构函数,什么时候不需要.md


一般情况下类的析构函数里面都是释放内存资源，而析构函数不被调用的话就会造成内存泄漏。这样做是为了当用一个基类的指针删除一个派生类的对象时，派生类的析构函数会被调用。
当然，并不是要把所有类的析构函数都写成虚函数。因为当类里面有虚函数的时候，编译器会给类添加一个虚函数表，里面来存放虚函数指针，这样就会增加类的存储空间。所以，只有当一个类被用来作为基类的时候，才把析构函数写成虚函数。


# problems__为什么需要虚继承.md


虚拟继承是多重继承中特有的概念。虚拟基类是为解决多重继承而出现的。
如:类D继承自类B1、B2，而类B1、B2都继 承自类A，因此在类D中两次出现类A中的变量和函数。为了节省内存空间，可以将B1、B2对A的继承定义为虚拟继承，而A就成了虚拟基类,虚拟继承在一般的应用中很少用到，所以也往往被忽视，这也主要是因为在C++中，多重继承是不推荐的，也并不常用，而一旦离开了多重继承，虚拟继承就完全失去了存在的必要因为这样只会降低效率和占用更多的空间。

虚继承的特点是，在任何派生类中的virtual基类总用同一个（共享）对象表示，


# problems__什么时候会产生栈溢出，为什么一直递归就会栈溢出.md

栈溢出是指程序运行时使用的栈空间超过了系统为栈分配的内存大小，导致栈溢出错误。栈通常用来存储函数调用、局部变量和临时数据，当递归层次过深或者函数调用过多时，会消耗大量栈空间，导致栈溢出。

以下是一些产生栈溢出的常见情况：

1. 递归调用：在递归函数中，每次调用都会将函数的参数、返回地址等信息存放在栈中，如果递归层次过深，栈空间会被消耗殆尽。
2. 大量局部变量：如果函数中定义了大量的局部变量，也会增加栈空间的使用。
3. 无限循环：在一个循环中不断地调用函数或者产生新的栈帧，会迅速消耗栈空间。

递归函数容易导致栈溢出的原因包括：

1. 没有合适的终止条件：如果递归函数没有设置终止条件或者终止条件设计不当，会导致无限递归调用。
2. 递归调用次数过多：每次递归调用都会在栈中创建一个新的栈帧，如果递归调用次数过多，会消耗大量栈空间。

# problems__什么是C++的内存模型？.md

1. **对象存储期（Storage Duration）**:
   - 自动存储期（Automatic）: 在栈上自动创建和销毁的对象，例如函数内的局部变量。
   - 静态存储期（Static）: 程序开始执行时分配直到程序结束才销毁的对象，包括全局变量和静态变量。
   - 动态存储期（Dynamic）: 通过`new`或`malloc`在堆上分配的内存，必须显式用`delete`或`free`来释放。
   - 线程存储期（Thread-local）: 每个线程有自己独立实例的变量，其生命周期与所属线程一致。
2. **内存分区**:
   - 栈（Stack）: 用来存储自动存储期的局部变量，函数参数等。是由编译器自动管理内存的区域。
   - 堆（Heap）: 用于动态内存分配，需要程序员或智能指针来管理内存的释放。
   - 全局/静态存储区（Global/Static Area）: 存放全局变量和静态变量。
   - 常量存储区（Constant Storage Area）: 存储常量数据，如字符串常量等。
   - 代码区（Code or Text Segment）: 存放程序的机器代码。
3. **多线程与原子操作**: C++11引入了对并发的支持，定义了原子类型`std::atomic`和内存序(memory order)概念，使得程序可以在多线程环境下安全地进行读写操作而不引发数据竞争。
4. **内存顺序**: 内存模型还详细规定了操作的内存顺序，这涉及到编译器优化和处理器重排指令时必须遵守的规则，确保一致性和可见性。
5. **同步和互斥**: C++标准库提供了多种同步机制，如互斥锁`std::mutex`，条件变量`std::condition_variable`等，它们的行为也被内存模型所覆盖。
6. **顺序一致性**: 当多个线程访问共享数据时，内存模型定义了保证顺序一致性的方法，即在不同线程看来，所有操作都是以相同顺序发生的。

# problems__什么是RAII原则，他在STL是怎么应用的？.md

#### RAII原则的核心思想：

- 在构造函数中获取资源。
- 在析构函数中释放资源。
- 不直接操作资源，而是通过管理资源的对象来使用资源。

这样做有几个好处：

1. **安全性**：避免资源泄露。由于资源的释放是自动的，因此即使在异常发生时，对象的析构函数也会被调用，资源也相应地会被释放。
2. **简洁性**：代码通常更加简洁，因为资源的管理是自动的，不需要程序员显式编写资源释放代码。
3. **异常安全**：RAII可以帮助提供强异常安全保障，因为资源释放不依赖于程序路径。

#### RAII在STL中的应用：

在STL中，RAII广泛应用于各种容器和其他组件中。例如：

- **智能指针**：`std::unique_ptr` 和 `std::shared_ptr` 是智能指针类，它们对动态分配的内存进行管理。当智能指针的实例离开作用域时，其析构函数会自动释放其所管理的内存。
- **容器类**：如 `std::vector`、`std::string`、`std::map` 等，都负责自己内部数据的内存管理。当一个容器对象被销毁时，它的析构函数会释放所有占用的内存，并适当地销毁其元素。
- **锁管理类**：如 `std::lock_guard` 和 `std::unique_lock`，它们在构造时获取锁，在析构时释放锁，从而确保在持有锁的代码块执行完毕后，无论是正常退出还是因异常退出，锁都会被释放。

# problems__什么是完美转发？.md

完美转发是一个与模板和函数重载相关的概念，它允许一个函数将其接收到的参数以原始的值类别（左值或右值）传递给另一个函数。这意味着**如果你传递了一个左值给包装函数，那么被调用的函数也会接收到一个左值；如果传递的是一个右值，则同样地，被调用的函数会接收到一个右值。**



```
#include <utility>

// 这个函数负责“转发”它的参数到另一个函数
template<typename T>
void wrapper(T&& arg) {
    // 使用 std::forward 来确保 arg 的值类别得以保持不变
    target(std::forward<T>(arg));
}

// 一个可能接受左值或右值参数的目标函数
void target(int& x) {
    // 处理左值
}

void target(int&& x) {
    // 处理右值
}

int main() {
    int lv = 5;     // 左值
    wrapper(lv);    // 应该调用 void target(int& x)
    wrapper(10);    // 应该调用 void target(int&& x)
}

```


# problems__什么是左值？什么是右值？有什么不同？.md

#### 左值 (Lvalue):

一个**左值**表示表达式结束后依然存在的对象。它指向一个具体的内存位置，并且你可以取得其地址。通常情况下，左值表达式可能出现在赋值操作的左侧。

#### 右值 (Rvalue):

一个**右值**通常是暂时的并且不会长时间存在，它不能被赋予另一个值。右值通常是直接的数据值或者无法通过标识符直接访问的临时对象。

**不同点：**

- **身份**: 左值具有明确的内存地址，而右值通常没有固定的内存地址。
- **持久性**: 左值代表长期存在的对象，右值代表临时或即将销毁的对象。
- **可移动性**: 右值可以被移动，而左值通常不能，除非显式地转换成右值引用。
- **引用类型**: 可以声明左值引用指向左值（`T&`），而右值引用(`T&&`)可以绑定到右值上，优化资源使用。

# problems__什么是泛型编程，他在STL中是怎么使用的？.md

泛型编程是一种软件开发方法论，其核心思想是通过抽象和重用代码来增强软件的灵活性、可维护性和复用性。在泛型编程中，算法或数据结构被设计为独立于它们所操作的具体数据类型。这种抽象化允许程序员使用相同的代码基础处理不同类型的数据，只要这些数据类型支持算法所需的操作。

#### STL中的泛型编程

C++的标准模板库（STL）是泛型编程的一个经典实例。STL提供了一套通用的容器类（如`vector`、`list`、`map`等），算法（如`sort`、`find`、`accumulate`等），以及其他实用工具（如迭代器、函数对象等），它们都是泛型化的，可以与任何符合要求的数据类型一起工作。

##### 容器

STL容器是泛型的，因为它们可以存储任何类型的对象。例如，`std::vector<int>` 可以存储整数，而 `std::vector<std::string>` 可以存储字符串。容器通过模板参数化其元素的类型：

```
std::vector<int> intVec; // 存储整数的向量
std::list<double> dblList; // 存储双精度浮点数的列表
```

##### 算法

STL算法也是泛型的，它们通过迭代器与容器进行交互，而不是直接操作容器。这种设计使得相同的算法可以应用于不同类型的容器，只要容器提供了适当类型的迭代器。例如，`std::sort`函数可以对任何连续存储的元素序列进行排序，无论它是`std::vector`、数组还是`std::array`：

```
std::vector<int> vec = {4, 1, 3, 5, 2};
std::sort(vec.begin(), vec.end()); // 对向量进行排序
```

##### 迭代器

迭代器在STL的泛型编程中扮演着中介的角色。它们提供了一种访问容器中元素的方法，同时隐藏了容器的内部结构。通过使用迭代器，STL算法可以在不知道或不关心容器具体实现的情况下工作。

##### 函数对象和Lambda表达式

STL允许你通过函数对象（包括lambda表达式）来自定义某些操作，比如自定义比较函数。这增加了STL的灵活性和泛型能力，因为你可以定义算法的行为，而无需修改算法本身。

```
std::sort(vec.begin(), vec.end(), [](int a, int b) { return a > b; }); // 使用lambda表达式降序排序
```

总之，STL通过泛型编程提供了一套强大的、类型无关的工具，使得开发者能够写出既安全又高效的代码，而不用牺牲代码的通用性和复用性。

# problems__内存池是什么？在C++中如何设计一个简单的内存池？.md

内存池是一种内存分配方式，它预先在内存中分配一定数量的块或对象，形成一个“池”。当程序需要分配内存时，它从这个池中分配一个块；当内存被释放时，这个块返回到池中以供再次使用。内存池可以显著减少频繁分配和释放内存所带来的开销，并且有助于避免内存碎片化，提高内存使用效率。

下面展示了如何设计一个简单的内存池,这个简单的内存池设计包括以下几个关键特性：

- **预分配**：在构造函数中预先分配了一定数量的固定大小内存块。
- **分配与释放**：`allocate` 方法从池中分配一个内存块，而 `deallocate` 方法则将不再使用的内存块返还给池。
- **管理策略**：本例中使用 `std::list` 来管理空闲内存块，但实际应用中可能需考虑更高效的数据结构。

```
#include <iostream>
#include <list>

class MemoryPool {
public:
    MemoryPool(size_t size, unsigned int count) {
        for (unsigned int i = 0; i < count; ++i) {
            freeBlocks.push_back(new char[size]);
        }
        blockSize = size;
    }

    ~MemoryPool() {
        for (auto block : freeBlocks) {
            delete[] block;
        }
    }

    void* allocate() {
        if (freeBlocks.empty()) {
            throw std::bad_alloc();
        }

        char* block = freeBlocks.front();
        freeBlocks.pop_front();
        return block;
    }

    void deallocate(void* block) {
        freeBlocks.push_back(static_cast<char*>(block));
    }

private:
    std::list<char*> freeBlocks;
    size_t blockSize;
};

// 使用示例
int main() {
    const int blockSize = 32; // 块大小
    const int blockCount = 10; // 块数量
    MemoryPool pool(blockSize, blockCount);

    // 分配内存
    void* ptr1 = pool.allocate();
    void* ptr2 = pool.allocate();

    // 使用ptr1和ptr2...

    // 释放内存
    pool.deallocate(ptr1);
    pool.deallocate(ptr2);

    return 0;
}

```


# problems__内联函数、构造函数、静态成员函数可以是虚函数吗.md


inline, static, constructor三种函数都不能带有virtual关键字。
inline是编译时展开，必须有实体；
static属于class自己的，也必须有实体；
virtual函数基于vtable（内存空间），constructor函数如果是virtual的，调用时也需要根据vtable寻找，但是constructor是virtual的情况下是找不到的，因为constructor自己本身都不存在了，创建不到class的实例，没有实例，class的成员（除了public static/protected static for friend class/functions，其余无论是否virtual）都不能被访问了。

虚函数实际上不能被内联:虚函数运行时所需的代价主要是虚函数不能是内联函。这也是非常好理解的，是因为内联函数是指在编译期间用被调用的函数体本身来代替函数调用的指令，但是虚函数的“虚”是指“直到运行时才能知道要调用的是哪一个函数。”但虚函数的运行时多态特性就是要在运行时才知道具体调用哪个虚函数，所以没法在编译时进行内联函数展开。当然如果通过对象直接调用虚函数它是可以被内联，但是大多数虚函数是通过对象的指针或引用被调用的，这种调用不能被内联。 因为这种调用是标准的调用方式，所以虚函数实际上不能被内联。

构造函数不能是虚函数。而且，在构造函数中调用虚函数，实际执行的是父类的对应函数，因为自己还没有构造好, 多态是被disable的。

静态的对象是属于整个类的，不对某一个对象而言，同时其函数的指针存放也不同于一般的成员函数，其无法成为一个对象的虚函数的指针以实现由此带来的动态机制。


# problems__写string类的构造，析构，拷贝函数.md

下面是一个简单的示例（以C++ 03为例）

注意在写的时候，针对于指针资源的配置与释放需要尤其注意，以及字符串的自赋值。这是写的时候比较容易出错的两个关键点，切记。

```C++
class MyString {
private:
    char* m_Data;
    int m_Length;

public:
    // 默认构造函数
    MyString() : m_Data(new char[1]), m_Length(0) {
        m_Data[0] = '\0';
    }

    // 参数化构造函数
    MyString(const char* str) {
        m_Length = strlen(str);
        m_Data = new char[m_Length + 1];
        strcpy(m_Data, str);
    }

    // 拷贝构造函数
    MyString(const MyString& other) {
        m_Length = other.m_Length;
        if (m_Length > 0) {
            m_Data = new char[m_Length + 1];
            strcpy(m_Data, other.m_Data);
        } else {
            m_Data = new char[1];
            m_Data[0] = '\0';
        }
    }

    // 析构函数
    ~MyString() {
        delete[] m_Data;
    }

    // 拷贝赋值运算符
    MyString& operator=(const MyString& other) {

        // 注意！
        if (&other == this) {
            return *this;
        }

        delete[] m_Data;
        m_Length = other.m_Length;
        if (m_Length > 0) {
            m_Data = new char[m_Length + 1];
            strcpy(m_Data, other.m_Data);
        } else {
            m_Data = new char[1];
            m_Data[0] = '\0';
        }

        return *this;
    }
};

```


# problems__函数参数的入栈顺序是什么，从左到右还是从右到左.md

在C++中，函数参数的入栈顺序是从右到左的。也就是说，最后一个参数先入栈，第一个参数最后入栈。

# problems__动态链接和静态链接的区别.md

主要区别在于链接到程序的**时间**和**方式**：

1. 静态链接：
   - **在编译期间，静态库的代码就被包含进了目标可执行文件中，之后即使没有静态库文件，也能正常运行。**
   - 生成的可执行文件较大，因为它包含了所有需要的库函数。
   - 更改静态库的功能或修复错误需要重新编译应用程序，分发新版本的可执行文件。
2. 动态链接：
   - **动态链接是在运行时完成的**，也就是说，在编译阶段并不会把库文件的代码加入到可执行文件，而只是添加一些引用信息。在程序执行时，由操作系统负责将动态库加载到内存中供程序使用。
   - 可执行文件较小，因为它仅仅包含对动态库的引用，而非实际的代码。
   - 如果多个应用程序都使用相同的库，那么这个库只需要在内存中保留一份即可，节约了系统资源。
   - **当动态库更新时，不需要重新编译和链接应用程序，运行程序时自然会调用新版本的库。**

每种链接方式都有其利弊。静态链接产生的程序更独立，但可能导致二进制文件很大；动态链接可以共享库提高效率，但如果相关的库不在系统上，那么程序无法运行。

# problems__原子变量的内存序是什么？.md

在C++11及之后的标准中，为了给开发者提供更细粒度的控制以及可能的性能优化空间，引入了多种内存顺序选项：

1. **memory_order_relaxed**：放松的内存顺序。不对执行顺序做任何保证，除了原子操作本身的原子性。这意味着，在没有其他同步操作手段的情况下，读写操作的顺序可能与程序代码中的顺序不一致。
2. **memory_order_consume**：较轻量级的保序需求，用于指定操作依赖于先前的某些操作结果。这在实际实现中通常被视为与`memory_order_acquire`相同。
3. **memory_order_acquire**：获取操作，禁止后续的读或写被重排到当前操作之前。用于读取操作。
4. **memory_order_release**：释放操作，防止之前的读或写操作被重排到当前操作之后。用于写入操作。
5. **memory_order_acq_rel**：同时包含获取和释放语义。适用于同时具有读取和写入特性的操作。
6. **memory_order_seq_cst**：顺序一致性内存顺序。所有线程看到的操作顺序一致。这是默认的内存顺序，并且提供了最强的顺序保证。

内存顺序的选择影响着程序的正确性和性能。较弱的内存顺序（例如`memory_order_relaxed`）可能带来更好的性能，因为它们允许更多的指令重排序；但是，使用它们也需要更小心地设计程序，以避免数据竞争和其他并发相关的错误。相反，`memory_order_seq_cst`提供了最简单和最直观的并发模型，但可能因为额外的同步代价而影响性能。

# problems__同一个类的两个对象的虚函数表是如何维护的？.md

虚函数表是由类维护的，而非单独为每个对象维护的。虚函数表的维护发生在编译期和链接期间，编译器和链接器会处理虚函数表的创建和初始化。运行时，当调用对象的虚函数时，程序会通过对象的虚函数表指针（vptr）来查找相应函数的地址，并执行对应的函数。

以下是虚函数表的工作流程的简化视图：

1. 类定义：编译器检测到类中有虚函数声明，为该类创建一个虚函数表。
2. 类实例化：当创建类的对象时，每个对象在内存中都会包含一个指向类虚函数表的指针（vptr）。同一个类的所有对象共享同一虚函数表，不会为每个对象单独创建新的虚函数表。
3. 虚函数调用：当通过基类指针或引用调用虚函数时，程序使用vptr找到虚函数表，然后通过表中的适当条目来定位要调用的函数地址，从而实现多态性。

因此，同一个类的两个对象使用相同的虚函数表，但它们各自持有自己的指向这个虚函数表的指针。如果派生类重写了某些虚函数，那么它将拥有自己的虚函数表，其中包含了重写函数的新地址，以及未被重写函数的原始地址。派生类对象的虚函数表指针将指向这个派生类特有的虚函数表。

# problems__在32位和64位系统中，指针分别为多大？.md

在32位系统中，指针的大小通常为4字节，即32位。

而在64位系统中，指针的大小通常为8字节，即64位。

这是因为在不同位数的系统中，内存寻址空间的大小不同。32位系统的地址总线宽度为32位，所以能够寻址的内存空间为2^32个地址，即4GB；而64位系统的地址总线宽度为64位，因此可以寻址的内存空间更大，达到了2^64个地址。

# problems__在C++中为什么需要深拷贝，浅拷贝会存在哪些问题？.md

浅拷贝可能带来以下问题：

- **双重释放**：当原始对象和拷贝对象生命周期结束时，它们的析构函数可能都会尝试释放同一个内存块，导致运行时错误。
- **数据竞争**：两个对象操作相同的资源可能导致数据不一致。
- **野指针**：一个对象释放了共享内存后，另一个对象就持有了一个野指针，继续访问该内存区域会引发未定义行为。

深拷贝解决了浅拷贝的以下问题：

- **避免双重释放**：每个对象负责其自己的资源释放，因此不会出现双重释放问题。
- **避免数据竞争和野指针**：由于每个对象都有自己的数据副本，它们互不干扰。

# problems__在C++中，对一个对象先malloc后delete这样使用可以吗？有什么风险.md

不可以

风险如下：

1. 构造函数未被调用：使用 `malloc` 分配内存时，对象的构造函数不会被执行，导致对象可能处于未初始化的状态。
2. 析构函数问题：如果你使用 `malloc` 分配而后使用 `delete` 释放内存，会尝试调用对象的析构函数，但因为构造函数从未被调用过，这可能导致未定义行为，比如资源泄露、崩溃或数据损坏等问题。
3. 兼容性问题：`malloc` 和 `delete` 内部实现可能有差异，混用可能导致堆损坏。
4. 异常安全性：由于 `malloc` 和 `delete` 的异常处理机制不同（`new` 可以抛出异常，而 `malloc` 返回NULL），这会进一步使得代码难以维护，并可能导致错误处理不当。

# problems__在C++中，用堆和用栈谁更快一点？.md

在C++中，栈分配通常比堆分配要快。

- 栈分配非常快。分配内存仅仅是涉及到移动栈指针的操作。由于栈是线性且连续的内存区域，增加或减少栈空间只需要调整栈顶指针的位置，与栈相比，堆分配通常较慢。分配堆内存需要在堆的数据结构中寻找足够大的空闲块，可能还需要添加新的内存页到进程空间。这个过程可能涉及到复杂的内存管理算法，如空闲列表（free lists）、二叉堆等，并且还需要处理碎片化问题。
- 栈上的数据通常具有更好的缓存局部性。因为栈是连续分配的，最近分配的变量很可能在CPU缓存中，而堆上的对象可能分布在内存的不同地方，导致缓存命中率降低。

# problems__在C++的map中，[]与insert有那些区别？.md

1. 语法：使用`[]`运算符时，**如果键已经存在于map中，则会返回对应的值；如果不存在，则会插入一个新的键值对。**而`insert`函数则需要传入一个`std::pair`类型的参数，其中包括要插入的键值对。
2. 返回值：**`[]`运算符会返回键对应的值，而`insert`函数会返回一个`std::pair`类型的迭代器和一个布尔值**，表示插入是否成功。
3. 覆盖：**如果使用`[]`运算符插入已存在的键，它会覆盖掉原来的值**；而`insert`函数不会覆盖已存在的键，如果键已经存在，插入操作将不会生效。


# problems__在C++的算法库中，find()和binary_search()有什么区别？.md

1. **算法复杂度和预期使用场景**:
   - `find()` 函数执行线性查找。它逐个检查容器中的元素，直到找到等于指定值的元素或结束。因为它是通过遍历实现的，所以其时间复杂度为 O(n)，其中 n 是容器中元素的数量。`find()` 不要求容器中的元素是事先排序的。
   - `binary_search()` 函数执行二分查找。它要求容器中的元素已经按非降序排序，并且通过不断将搜索范围缩小一半来查找特定值。因此，其时间复杂度为 O(log n) 。由于这种查找方式依赖于容器的元素顺序，所以在未排序的容器上使用 `binary_search()` 会得到未定义的行为。
2. **返回值**:
   - `find()` 返回一个迭代器，指向在容器中找到的第一个等于指定值的元素。如果没有找到，它返回一个等于 end() 迭代器的值。
   - `binary_search()` 返回一个布尔值，如果找到指定值则返回 true，否则返回 false。注意，它并不返回目标元素的位置或迭代器。
3. **通用性**:
   - `find()` 可以用于任何类型的容器，包括列表、向量、集合等，而且不需要元素是排序的。
   - `binary_search()` 通常用于数组或向量等随机访问容器，并且前提是这些容器中的元素已经被排序。

# problems__如何定义一个只能在堆上定义对象的类栈上呢.md

**只能在堆上创建对象的类**

要实现这一点，我们需要将它的析构函数设置为私有。此外，我们需要提供一个public的接口来删除这个对象。

```C++
class HeapOnly {
private:
    ~HeapOnly() {}  // 私有析构函数

public:
    static HeapOnly* CreateInstance() {
        return new HeapOnly();
    }

    static void DeleteInstance(HeapOnly* p) {
        delete p;
    }
};
```

在这个例子中，我们不能在栈上创建`HeapOnly`类的对象，因为析构函数是私有的。但我们仍然可以在堆上创建，并且需要调用`DeleteInstance()`来删除这个对象。

**只能在栈上创建对象的类**

要实现这一点，我们可以将new操作符重载设为私有。这样就无法使用`new`来在堆上创建对象了。

```c++
class StackOnly {
private:
    void* operator new(size_t size) = delete;  // 禁用new操作符
    void operator delete(void* p) = delete;  // 禁用delete操作符

public:
    StackOnly() {}
    ~StackOnly() {}
};
```

在这个例子中，我们不能在堆上创建`StackOnly`类的对象，因为new操作符已被私有化，但我们仍然可以在栈上创建。

# problems__如何构造一个类使得只能在堆上或者栈上分配内存？.md

#### 只在栈上创建对象

要使得对象只能在栈上创建，可以将类的new操作符设置为private或者delete。这样一来，由于在堆上创建对象需要调用new操作符，这个类的对象就只能在栈上创建了。例如：

```
class StackOnly {
private:
    static void* operator new(size_t size) = delete;
    static void* operator new[](size_t size) = delete;

public:
    StackOnly() {}
    ~StackOnly() {}
};
```

#### 只在堆上创建对象

要使得对象只能在堆上创建，可以将类的析构函数设置为private，并提供一个public的函数来删除对象。这样一来，由于在栈上创建对象需要在作用域结束时调用析构函数，这个类的对象就只能在堆上创建了。

但是这样会导致一个问题，就是当我们创建对象后忘记手动删除，就会引发内存泄漏。因此，可以将删除函数封装在智能指针中，从而避免这个问题。例如：

```
class HeapOnly {
public:
    HeapOnly() {}

    static HeapOnly* Create() { return new HeapOnly(); }
    static void Destroy(HeapOnly* instance) { delete instance; }

private:
    ~HeapOnly() {}
};
```

# problems__如何选择合适的STL容器.md

选择合适的STL容器依赖于你的特定需求，包括你的数据结构、性能要求以及如何使用这些数据。下面是一些常用的STL容器和它们的特点，以及何时最适合使用它们：

1. **std::vector**:
   - 动态数组，提供快速的随机访问（O(1)时间复杂度）。
   - 适用于元素数量经常变化，但主要操作是在尾部添加或移除元素的场景。
   - 不适合频繁在中间或头部插入/删除操作，因为这样的操作会导致后续所有元素移动。
2. **std::deque**:
   - 双端队列，支持在头部和尾部高效的插入和删除操作。
   - 当需要快速地在序列的两端进行插入或删除时，更优于`std::vector`。
3. **std::list** 和 **std::forward_list**:
   - 分别代表双向和单向链表。
   - 提供在任意位置高效插入和删除操作（O(1)时间复杂度）。
   - 不支持快速随机访问。
   - 当数据结构需要频繁在中间位置插入和删除元素时，链表可能是一个好选择。
4. **std::set** 和 **std::multiset**:
   - 基于红黑树实现的有序集合和多重集合。
   - 自动对元素排序，并保证唯一性（`std::multiset` 允许重复元素）。
   - 插入、查找和删除操作具有对数时间复杂度（O(log n)）。
   - 当需要保存有序的唯一元素集合，并且频繁查询是否存在某个元素时使用。
5. **std::map** 和 **std::multimap**:
   - 基于红黑树的键-值对集合，自动按键排序。
   - `std::multimap` 允许键不唯一。
   - 适用于当需要根据键来存取元素，并且需要保持键的有序性时。
6. **std::unordered_set**、**std::unordered_map**、**std::unordered_multiset** 和 **std::unordered_multimap**:
   - 基于哈希表实现的无序容器。
   - 提供平均常数时间复杂度的插入、查找和删除操作，但最坏情况下会退化到线性时间。
   - 当元素的顺序不重要，且期望快速访问时使用。
7. **std::stack**、**std::queue** 和 **std::priority_queue**:
   - 封装了其他容器的适配器，分别提供了栈、队列和优先队列的接口。
   - `std::stack` 和 `std::queue` 通常基于 `std::deque` 实现。
   - `std::priority_queue` 通常基于 `std::vector` 实现，并通过使堆来管理元素的优先级。
   - 适用于特定的数据结构需求，如LIFO（后进先出）、FIFO（先进先出）或优先级排序。
8. **std::array**:
   - 固定大小的数组封装，提供了标准容器接口。
   - 当数组大小已知且不变时使用，它提供了比原始数组更安全和易于使用的接口。

选择合适容器的一般建议是：

- 首选 `std::vector`，除非有特定理由选择其他容器。
- 如果需要高效的插入和删除，考虑 `std::list` 或 `std::deque`。
- 如果需要保存唯一元素并保持顺序，使用 `std::set`。
- 如果元素顺序不重要，但想要快速查找，使用 `std::unordered_set` 或 `std::unordered_map`。
- 对于特殊用途的容器，如栈、队列或优先队列，选择相应的适配器。

模板参数、内存分配器选项和成员函数的选择也可以影响容器的行为和性能，所以在选择时还需要考虑这些因素。

# problems__如何避免悬挂指针？.md

1. 立即清空： 释放了指针指向的内存后，立即将该指针设置为`nullptr`（C++11引入的空指针字面量），这样就不会再指向之前的内存地址了。

   ```c++
   delete ptr;
   ptr = nullptr;
   ```

2. 范围意识： 尽量使用局部变量来管理动态分配的内存，这样当控制流离开变量的作用域时，可以自动释放资源。智能指针（如`std::unique_ptr`和`std::shared_ptr`）在C++中是自动管理动态内存的很好选择。

3. 避免野指针： 声明指针时，如果不立即初始化，就需要给它赋予`nullptr`，确保它不会成为野指针，指向某个随机的、不确定的内存位置。

   ```c++
   int* ptr = nullptr; // 初始化为nullptr，而不是未初始化的野指针
   ```

4. 适时销毁： 在对象的生命周期结束时（例如，在析构函数中），确保释放所有动态分配的内存。对于类的设计，要格外注意拷贝构造函数和赋值操作符的正确实现，避免复制和赋值导致的悬挂指针问题。

5. 引用计数： 使用引用计数如`std::shared_ptr`智能指针，它通过内部机制确保只有最后一个指针持有者销毁时才释放内存。

6. 定期检查： 使用代码审查、静态分析工具和运行时调试工具来检测和定位可能的悬挂指针。

7. 函数返回值： 避免从函数返回局部变量的地址或者引用，因为函数返回后，局部变量的生命周期就结束了。

8. 注意回调和事件处理： 如果你注册了指针为回调或事件处理器，确保在废弃指针之前注销这些回调或事件处理器。

# problems__如果A是某一个类的指针，那么在它等于nullptr的情况下能直接调用里面的A对应类里面的public函数吗.md

在A是某一个类的指针，并且等于nullptr的情况下，是不能直接调用A对应类里面的public函数的。当A为nullptr时，表示它不指向任何有效的对象，因此调用A对应类里面的函数会导致空指针解引用错误，从而引发未定义行为。

# problems__如果A这个对象对应的类是一个空类，那么sizeof(A)的值是多少？.md

如果A这个对象对应的类是一个空类（即没有任何成员变量或成员函数），那么sizeof(A)的值通常会是1。

C++标准规定，空类的实例在内存中至少占用一个字节的空间，这是为了确保每个实例都有独一无二的地址。因此，即使类里面没有定义任何成员变量，编译器也会给这个实例分配一个字节的空间。

# problems__如果A这个指针指向一个数组，那么sizeof(A)的值是多少？.md

如果A是一个指针，指向一个数组，那么sizeof(A)的值取决于编译器和操作系统的位数。

在大多数32位系统上，sizeof(A)的值通常是4，因为指针占用4个字节。而在64位系统上，sizeof(A)的值通常是8，因为指针占用8个字节。

# problems__如果拿到虚函数表的储存地址，是否可以改写虚函数表的内容？.md

**理论上，如果你能获取到虚函数表的存储地址，你确实可以修改虚函数表的内容**，将虚函数的地址替换为其他函数的地址。

然而，这样做通常是**非常危险**，**因为它破坏了C++的抽象和类型安全**，可能会导致未定义的行为。**在大多数操作系统和编译器实现中，虚函数表存放在只读数据段**，直接尝试修改虚函数表将会导致程序崩溃。

**此外，破坏虚函数表还会引入潜在的安全问题**。在某些情况下，恶意软件可能会尝试这样的操作来jc程序的控制流。因此，现代操作系统通常会采取一些保护措施，例如数据执行保护（DEP）和地址空间布局随机化（ASLR）来阻止这种类型的攻击。

# problems__宏定义和展开、内联函数区别.md


内联函数是代码被插入到调用者代码处的函数。如同 #define 宏，内联函数通过避免被调用的开销来提高执行效率，尤其是它能够通过调用（“过程化集成”）被编译器优化。 宏定义不检查函数参数，返回值什么的，只是展开，相对来说，内联函数会检查参数类型，所以更安全。	内联函数和宏很类似，而区别在于，宏是由预处理器对宏进行替代，而内联函数是通过编译器控制来实现的。而且内联函数是真正的函数，只是在需要用到的时候，内联函数像宏一样的展开，所以取消了函数的参数压栈，减少了调用的开销。

宏是预编译器的输入，然后宏展开之后的结果会送去编译器做语法分析。宏与函数等处于不同的级别，操作不同的实体。宏操作的是 token, 可以进行 token的替换和连接等操作，在语法分析之前起作用。而函数是语言中的概念，会在语法树中创建对应的实体，内联只是函数的一个属性。
对于问题：有了函数要它们何用？答案是：一：函数并不能完全替代宏，有些宏可以在当前作用域生成一些变量，函数做不到。二：内联函数只是函数的一种，内联是给编译器的提示，告诉它最好把这个函数在被调用处展开，省掉一个函数调用的开销（压栈，跳转，返回）

内联函数也有一定的局限性。就是函数中的执行代码不能太多了，如果，内联函数的函数体过大，一般的编译器会放弃内联方式，而采用普通的方式调用函数。这样，内联函数就和普通函数执行效率一样

内联函数必须是和函数体申明在一起，才有效。



# problems__悬挂指针和野指针有什么区别？.md

1. **悬挂指针**: 悬挂指针产生于指针所指向的内存已被释放或者失效后，指针本身没有及时更新或清空。在该内存释放之后，任何通过这个悬挂指针的引用或操作都是不安全的，因为这块内存可能已经重新分配给了其他的数据。

   示例：当一个指针指向动态分配（比如使用`malloc`或`new`）的内存，并且随后该内存被释放掉（使用`free`或`delete`），而没有将指针设置为`NULL`，此时这个指针就变成了悬挂指针。

2. **野指针**: 野指针通常是指未初始化的指针，它没有被设置为任何有效的地址。由于它可能指向任意位置，对野指针的解引用是危险的，并且可能会导致难以预测的行为甚至程序崩溃。

   示例：声明了一个指针变量但是没有给它赋予确定的初始值，然后就开始使用这个指针。

尽管两者看似相似，但是产生原因和解决方式有所不同：

- **悬挂指针问题**可以通过确保指针在释放关联的内存资源后立即被设为`NULL`来避免。
- **野指针问题**则需要确保每个指针变量在使用前都被明确初始化为一个合法的地址或`NULL`。

处理这两种类型的指针时，编程中的最佳实践是始终确保你的指针在声明后得到适当的初始化，在资源被释放之后更新状态，并且在解引用之前检查其有效性。

# problems__指针和引用在内存中的表现形式有何不同？.md

在很多实现中，引用通常通过编译器使用指针来实现。然而，从语义上讲，引用更类似于直接操作所引用的对象，而不需要通过解引用指针那样的间接层。

- 在**汇编层面，对引用的操作往往会被转换成直接对引用所绑定对象的内存地址的操作，就像使用指针一样。**
- 当函数参数以引用方式传递时，底层可能也是通过传递对象的地址（即指针）来实现的，但在函数内部，这个地址被自动解引用，因此开发者感受到的是直接操作对象本身。

编译器可能会对引用和指针的使用进行优化：

- 对于引用，在某些情况下（尤其是在优化等级较高时），**编译器可能完全不会分配用于存储地址的内存，特别是当引用作为函数参数或局部变量时，编译器可以选择直接操作原始对象**。
- 对于指针，由于存在指针算术和可能的重新赋值，编译器的优化能力可能会受到限制。

# problems__指针和引用有什么区别呢？.md

1. 基本概念：
   - 指针是一个变量，其值为另一个变量的地址，即直接存储了内存中的一个位置。可以通过解引用操作来访问或修改指针指向的数据。
   - 引用是一个变量的别名，对引用的操作就是对被引用变量的操作。引用在声明时必须初始化，并且不能改变引用的对象。
2. 语法：
   - 指针：`类型 *指针名;`
   - 引用：`类型 &引用名 = 被引用变量;`
3. 空值：
   - 指针可以有空值（NULL, nullptr），表示它不指向任何对象。
   - 引用必须连接到一块合法的内存，一旦绑定到一个对象上，就不能再改变引用的目标。
4. 初始化：
   - 指针可以在任何时候被初始化，也可以先声明后赋值。
   - 引用在声明的时候必须被初始化，并且一旦指定了一个对象后，就不能再引用其他对象。
5. 内存地址：
   - 指针本身是一个独立的变量，因此除了能存储地址之外，还有自己的内存地址。
   - 引用并不占据内存空间，它只是原始变量的一个别名。
6. 操作符：
   - 指针使用*进行解引用，用以访问目标变量；使用&取得一个变量的地址。
   - 引用使用&在声明时建立关联，之后无需特殊操作符，就像操作普通变量一样。
7. 用途差异：
   - 指针的灵活性更高，更适合于执行复杂的内存操作，例如动态内存分配、数组遍历等。
   - 引用通常用作函数参数或返回值，以简化代码和避免指针可能导致的错误。
8. 重新赋值：
   - 指针可以被重新赋值，指向另一个不同的地址。
   - 引用一旦被绑定到一个对象上，就不能改变这个绑定关系。

# problems__指针常量和常量指针的区别.md



1. **常量指针: 常量指针是一个指向常量数据的指针，这意味着指针指向的数据不可以通过这个指针被修改。然而，指针本身的值（即存储的地址）是可以更改的；它可以指向另一个常量数据或非常量数据的地址。

   在C/C++中，常量指针的声明如下：

   ```c
   const int *ptr;
   // 或者
   int const *ptr;
   ```

   上述两种声明方式都表示`ptr`是一个指向`int`类型常量的指针。你不能通过`ptr`来改变所指向的值，但可以改变`ptr`的值使其指向另一个`int`类型的地址。

2. **指针常量 : 指针常量是一个指针，它自身的值是常量，也就是说，一旦被初始化之后，指针的值（它所存储的地址）就不能再改变了。但是，如果指针指向的不是常量，那么你仍然可以通过这个指针修改指向的数据。

   在C/C++中，指针常量的声明如下：

   ```c
   int *const ptr = &someVariable;
   ```

   这里`ptr`必须在声明时初始化，并且以后不能再指向其他任何地址。但是，`ptr`所指向的`int`类型的数据可以通过`ptr`来修改。

因此，核心区别在于：

- 常量指针主要限制通过这个指针改变所指向的数据，但允许改变指针指向的地址。
- 指针常量主要限制改变指针所存储的地址，但允许通过这个指针改变所指向的数据（除非它同时也是一个常量指针，下文将说明）。

还有一种情况是两者结合使用，创建一个指向常量数据的固定指针（即指针的地址和它指向的数据都不能改变）：

```c
const int *const ptr = &someConstantVariable;
```

在这种声明中，`ptr`是一个指针常量，同时指向一个整型常量，因此无法修改`ptr`的值（地址），也不能通过`ptr`来修改其指向的数据。

# problems__构造函数中可以调用虚函数吗.md

先说结论：理论可以，但是真的去这么做了，会出大问题，所以一般情况下我们不允许这么做。



让我们来具体分析下：

​	首先，在C++编程语言中，构造函数中是可以调用虚函数的，C++的规则里面并没有任何对于这方面的限制，但是考虑一个问题，假设有两个对象，他们分别为继承关系，因为当一个类对象（包括派生类）正在被创建时，其类型会逐步从基类变化到最终的派生类。**如果在这个过程中调用了虚函数，那么这个虚函数调用将不会下降到更深层次的派生类。**

​	具体来说，如果在基类的构造函数中调用了虚函数，并且这个虚函数在某个派生类中被重写了，那么在创建这个派生类的对象时，虚函数调用将只会执行基类版本的虚函数，而不会执行派生类版本的虚函数。这是因为，此时派生类部分的对象还没有被完全构造好，所以不能调用派生类的成员函数。

​	至于为什么会这样，**主要是原因在于C++中的虚函数通过一个称为vtable（虚表）的机制来实现动态绑定。每个包含虚函数的类都有一个与之关联的虚表，其中包含了指向该类的虚函数的指针。**

​	当一个对象被创建时，它的构造函数会先调用其基类的构造函数，然后按照声明顺序初始化其成员，并最后执行其自身的构造函数代码。**当在一个构造函数中调用虚函数时，由于此时正在构造的对象尚未完成构造，其类型被视为当前正在执行的构造函数所在的类，而不是最终派生类。**因此，如果在基类的构造函数中调用虚函数，将使用基类的虚表，从而调用基类版本的虚函数，而不是派生类版本的虚函数。

​	至于为什么不是最终派生类，我们都知道，虚函数表只会被创建一份，但是具体在什么地方创建，这个取决于编译器如何实现，但是他的虚函数表指针一般会放在对象头部，也就是说，每个类只有一个虚函数表，但是每个对象都有自己的虚函数表指针。这意味着，**同一类型的所有对象都共享同一个虚函数表的内容，但是他们各自的虚函数表指针可能指向不同的虚函数表**（如果它们是不同类型的对象）。这也是为什么派生类可以覆盖基类的虚函数的原因：因为派生类的虚函数表中，对应的函数指针会被更新以指向派生类的版本。而在现在这个情况，由于派生类并未构造完成，所以派生类的的构造函数调用的虚函数只能参考基类而不是派生类，从而导致出现错误

​	总的来说，虽然技术上允许在构造函数中调用虚函数，但是实际操作中却应该避免。一般来说，构造函数应该尽量简单，只做必要的初始化工作，而将复杂的逻辑放入其他成员函数中。

# problems__用C++设计一个不能被继承的类.md

使用C++中的关键字`final`。将`final`关键字放在类的声明中，表示该类不能再被其他类所继承。如果有其他类试图进行继承，编译器将会报错。

```C++
class FinalClass final {
public:
    // 类的成员函数和数据成员...
};

```


# problems__类构造和析构的顺序.md

在C++中，类的构造和析构顺序遵循以下规则：

1. 构造顺序：
   - 基类优先于派生类进行构造。
   - 在同一个类中，成员变量按照声明的顺序进行初始化。
   - 对于多个基类的情况，会按照声明顺序来初始化。
2. 析构顺序：
   - 析构顺序与构造顺序相反。
   - 派生类对象先于基类对象进行析构。
   - 在同一个类中，成员变量按照声明顺序的逆序进行析构。
   - 对于多个基类的情况，会按照声明顺序的逆序进行析构。

举个例子：

```c++
#include <iostream>
using namespace std;

class Base {
public:
    Base() { cout << "构造基类对象\n"; }
    ~Base() { cout << "析构基类对象\n"; }
};

class Derived : public Base {
public:
    Derived() { cout << "构造派生类对象\n"; }
    ~Derived() { cout << "析构派生类对象\n"; }
};

int main() {
    Derived d;
    return 0;
}
```

输出将会是：

```c++
复制代码构造基类对象
构造派生类对象
析构派生类对象
析构基类对象
```

对于成员变量，我们可以稍微修改一下代码：

```c++
#include <iostream>
using namespace std;

class Member {
public:
    Member(const char* s) { cout << "构造成员对象: " << s << "\n"; }
    ~Member() { cout << "析构成员对象\n"; }
};

class WithMembers {
public:
    WithMembers() : m2("m2"), m1("m1") { cout << "构造包含成员的对象\n"; }
    ~WithMembers() { cout << "析构包含成员的对象\n"; }

private:
    Member m1;
    Member m2;
};

int main() {
    WithMembers wm;
    return 0;
}
```

你会看到如下的输出：

```
复制代码构造成员对象: m1
构造成员对象: m2
构造包含成员的对象
析构包含成员的对象
析构成员对象
析构成员对象
```

即使在构造函数中 `m2` 在 `m1` 之前初始化，`m1` 和 `m2` 的构造顺序还是按照它们在类中声明的顺序。同样，析构顺序与构造顺序相反。

# problems__纯虚函数，为什么需要纯虚函数.md


纯虚函数是在基类中声明的虚函数，它在基类中没有定义，但要求任何派生类都要定义自己的实现方法。在基类中实现纯虚函数的方法是在函数原型后加“=0”

`virtual void funtion1()=0`

原因：
1、为了方便使用多态特性，我们常常需要在基类中定义虚拟函数。
2、在很多情况下，基类本身生成对象是不合情理的。例如，动物作为一个基类可以派生出老虎、孔雀等子类，但动物本身生成对象明显不合常理。

为了解决上述问题，引入了纯虚函数的概念，将函数定义为纯虚函数（方法：virtual ReturnType Function()= 0;），则编译器要求在派生类中必须予以重写以实现多态性。同时含有纯虚拟函数的类称为抽象类，它不能生成对象。这样就很好地解决了上述两个问题。声明了纯虚函数的类是一个抽象类。所以，用户不能创建类的实例，只能创建它的派生类的实例。

**定义纯虚函数的目的在于，使派生类仅仅只是继承函数的接口。**
纯虚函数的意义，让所有的类对象（主要是派生类对象）都可以执行纯虚函数的动作，但类无法为纯虚函数提供一个合理的缺省实现。所以类纯虚函数的声明就是在告诉子类的设计者，“你必须提供一个纯虚函数的实现，但我不知道你会怎样实现它”。




# problems__虚函数是否可以声明为static？.md

虚函数不能声明为静态（static）。在C++中，虚函数是通过指针的动态绑定来实现多态性的，而静态成员函数是与类本身相关联的，不依赖于特定对象的实例化，因此无法被继承并重写。

当我们声明一个成员函数为虚函数时，编译器会为每个类维护一个虚函数表（vtable），用于存储该类的虚函数地址。然后，在运行时，通过对象的指针或引用调用虚函数时，程序会根据实际对象类型的虚函数表来确定要调用的函数。

相反，静态成员函数不与任何特定对象实例相关联，它属于类而不属于对象。因此，静态函数没有 this 指针，并且不参与多态性和动态绑定。由于静态成员函数没有虚函数表的概念，因此它们不能被声明为虚函数。

因此，在C++中，虚函数不能声明为静态。

# problems__虚函数是针对类还是针对对象的？.md

虚函数是针对类的一个概念，而不是针对单个对象。当在C++中定义一个类时，可以将类的成员函数声明为虚函数。这意味着当通过基类指针或引用调用该函数时，将会根据对象的实际类型来决定调用哪个版本的函数，这一过程称为动态绑定或多态。

当派生类继承了包含虚函数的基类时，它可以重写这些虚函数，提供自己特定的实现。如果对象是派生类的实例，即使使用基类类型的指针或引用去调用虚函数，程序也会调用派生类中重写的版本。

这是多态性的核心：同样的函数调用，依据对象的实际类型，可以有不同的行为。因此，虚函数机制是设计在类层面上的，但它影响的是通过那些类创建的对象的行为。每个对象不需要单独定义虚函数，它们都是从其类定义中继承而来的。

# problems__虚函数的作用和实现原理，什么是虚函数,有什么作用.md


C++的多态分为静态多态（编译时多态）和动态多态（运行时多态）两大类。静态多态通过重载、模板来实现；动态多态就是通过本文的主角虚函数来体现的。

虚函数实现原理:包括虚函数表、虚函数指针等

虚函数的作用说白了就是：当调用一个虚函数时，被执行的代码必须和调用函数的对象的动态类型相一致。编译器需要做的就是如何高效的实现提供这种特性。不同编译器实现细节也不相同。大多数编译器通过vtbl（virtual table）和vptr（virtual table pointer）来实现的。 当一个类声明了虚函数或者继承了虚函数，这个类就会有自己的vtbl。vtbl实际上就是一个函数指针数组，有的编译器用的是链表，不过方法都是差不多。vtbl数组中的每一个元素对应一个函数指针指向该类的一个虚函数，同时该类的每一个对象都会包含一个vptr，vptr指向该vtbl的地址。

结论：

每个声明了虚函数或者继承了虚函数的类，都会有一个自己的vtbl
同时该类的每个对象都会包含一个vptr去指向该vtbl
虚函数按照其声明顺序放于vtbl表中, vtbl数组中的每一个元素对应一个函数指针指向该类的虚函数
如果子类覆盖了父类的虚函数，将被放到了虚表中原来父类虚函数的位置
在多继承的情况下，每个父类都有自己的虚表。子类的成员函数被放到了第一个父类的表中

### 衍生问题:为什么 C++里访问虚函数比访问普通函数慢?

单继承时性能差不多，多继承的时候会慢

### 调用性能方面

从前面虚函数的调用过程可知。当调用虚函数时过程如下（引自More Effective C++）:

通过对象的 vptr 找到类的 vtbl。这是一个简单的操作,因为编译器知道在对象内 哪里能找到 vptr(毕竟是由编译器放置的它们)。因此这个代价只是一个偏移调整(以得到 vptr)和一个指针的间接寻址(以得到 vtbl)。
找到对应 vtbl 内的指向被调用函数的指针。这也是很简单的, 因为编译器为每个虚函数在 vtbl 内分配了一个唯一的索引。这步的代价只是在 vtbl 数组内 的一个偏移。
调用第二步找到的的指针所指向的函数。
在单继承的情况下，调用虚函数所需的代价基本上和非虚函数效率一样，在大多数计算机上它多执行了很少的一些指令，所以有很多人一概而论说虚函数性能不行是不太科学的。在多继承的情况下，由于会根据多个父类生成多个vptr，在对象里为寻找 vptr 而进行的偏移量计算会变得复杂一些，但这些并不是虚函数的性能瓶颈。 虚函数运行时所需的代价主要是虚函数不能是内联函。这也是非常好理解的，是因为内联函数是指在编译期间用被调用的函数体本身来代替函数调用的指令，但是虚函数的“虚”是指“直到运行时才能知道要调用的是哪一个函数。”但虚函数的运行时多态特性就是要在运行时才知道具体调用哪个虚函数，所以没法在编译时进行内联函数展开。当然如果通过对象直接调用虚函数它是可以被内联，但是大多数虚函数是通过对象的指针或引用被调用的，这种调用不能被内联。 因为这种调用是标准的调用方式，所以虚函数实际上不能被内联。

### 占用空间方面


在上面的虚函数实现原理部分，可以看到为了实现运行时多态机制，编译器会给每一个包含虚函数或继承了虚函数的类自动建立一个虚函数表，所以虚函数的一个代价就是会增加类的体积。在虚函数接口较少的类中这个代价并不明显，虚函数表vtbl的体积相当于几个函数指针的体积，如果你有大量的类或者在每个类中有大量的虚函数,你会发现 vtbl 会占用大量的地址空间。但这并不是最主要的代价，主要的代价是发生在类的继承过程中，在上面的分析中，可以看到，当子类继承父类的虚函数时，子类会有自己的vtbl，如果子类只覆盖父类的一两个虚函数接口，子类vtbl的其余部分内容会与父类重复。这在如果存在大量的子类继承，且重写父类的虚函数接口只占总数的一小部分的情况下，会造成大量地址空间浪费。在一些GUI库上这种大量子类继承自同一父类且只覆盖其中一两个虚函数的情况是经常有的，这样就导致UI库的占用内存明显变大。 由于虚函数指针vptr的存在，虚函数也会增加该类的每个对象的体积。在单继承或没有继承的情况下，类的每个对象会多一个vptr指针的体积，也就是4个字节；在多继承的情况下，类的每个对象会多N个（N＝包含虚函数的父类个数）vptr的体积，也就是4N个字节。当一个类的对象体积较大时，这个代价不是很明显，但当一个类的对象很轻量的时候，如成员变量只有4个字节，那么再加上4（或4N）个字节的vptr，对象的体积相当于翻了1（或N）倍，这个代价是非常大的。


### 总结

最后，总结一下关于虚函数的一些常见问题：

1) 虚函数是动态绑定的，也就是说，使用虚函数的指针和引用能够正确找到实际类的对应函数，而不是执行定义类的函数。这是虚函数的基本功能，就不再解释了。

2) 构造函数不能是虚函数。而且，在构造函数中调用虚函数，实际执行的是父类的对应函数，因为自己还没有构造好, 多态是被disable的。

3) 析构函数可以是虚函数，而且，在一个复杂类结构中，这往往是必须的。

4) 将一个函数定义为纯虚函数，实际上是将这个类定义为抽象类，不能实例化对象。

5) 纯虚函数通常没有定义体，但也完全可以拥有。

6)  析构函数可以是纯虚的，但纯虚析构函数必须有定义体，因为析构函数的调用是在子类中隐含的。

7) 非纯的虚函数必须有定义体，不然是一个错误。

8) 派生类的override虚函数定义必须和父类完全一致。除了一个特例，如果父类中返回值是一个指针或引用，子类override时可以返回这个指针（或引用）的派生。例如，在上面的例子中，在Base中定义了 virtual Base* clone(); 在Derived中可以定义为 virtual Derived* clone()。可以看到，这种放松对于Clone模式是非常有用的。


# problems__讲一讲C++中的原子操作有那些？.md

在C++中，原子操作是由 `<atomic>` 头文件提供的一组操作和类型，它们确保了在多线程环境中对共享数据的修改是不可分割的，即这些操作在执行过程中不会被其他线程打断。原子操作主要用于实现无锁（lock-free）编程，允许多个线程安全地并发访问和修改数据。



`std::atomic` 是C++11引入的一个模板类，用来封装任意类型（满足TriviallyCopyable）的值，并且保证对这些值的操作是原子的。以下是一些常见的原子操作：

1. `store`：将值写入原子对象。
2. `load`：从原子对象读取值。
3. `exchange`：替换原子对象的值，并返回该对象之前的值。
4. `compare_exchange_weak` / `compare_exchange_strong`：比较原子对象的值，并在相等时替换为新值。
5. `fetch_add` / `fetch_sub`：对原子对象进行加/减操作，并返回操作前的值。
6. `fetch_and` / `fetch_or` / `fetch_xor`：对原子对象进行位与/位或/位异或操作，并返回操作前的值。
7. `++` / `--`：原子地递增或递减对象的值。



原子操作还包括一些内存顺序（memory order）的概念，如 `memory_order_relaxed`, `memory_order_consume`, `memory_order_acquire`, `memory_order_release`, `memory_order_acq_rel`, `memory_order_seq_cst`。这些内存顺序用于控制操作的执行顺序，以及如何处理编译器和处理器级别的优化。

# problems__讲讲函数调用的过程.md

在C++中，函数的调用过程主要包括以下几个步骤：

1. 压入返回地址：当一个函数被调用时，程序会先将当前函数的返回地址压入栈中。这个返回地址指向调用该函数之后需要返回到的下一个指令地址。
2. 压入参数：接下来，函数的参数按照从右到左的顺序被压入栈中。这些参数将作为函数的局部变量使用。
3. 保存寄存器状态：如果函数需要使用一些寄存器，那么在执行函数之前，需要先将这些寄存器的值保存在栈中，以防止它们被修改。
4. 跳转到函数体：现在，程序跳转到被调用的函数体开始执行。在执行函数时，函数的局部变量以及其他需要用到的内存空间也会被分配在栈上。
5. 执行函数体：函数体被执行并返回结果。在函数执行过程中，程序会按照语句的顺序依次执行每一条语句，并可能调用其他的子函数。
6. 弹出栈中的内容：当函数执行完毕之后，会将所有被压入栈中的内容依次弹出。这个过程包括弹出所有的局部变量、恢复寄存器状态、弹出所有的参数以及返回地址。
7. 返回到调用者：最后，程序跳转回到调用该函数的地方，并将函数的返回值传递给调用者。这时候，程序会从返回地址指针中读取下一个指令地址，并开始执行下一个指令。

# problems__迭代器与普通指针有什么区别.md

1. 服务对象不同：迭代器主要面对STL容器提供服务，为STL容器提供遍历，删除等功能，而指针主要面向内存底层进行服务。
2. 安全性：迭代器可以提供更高级的安全性，因为它们可以对越界和空指针进行检查，并且可以被设计为只读或者只写。而普通指针则需要程序员自行管理边界和空指针的情况。
3. 可移植性：迭代器在不同容器和数据结构之间具有更好的可移植性，因为它们隐藏了底层的实现细节，使得代码更易于重用。普通指针则依赖于具体的数据结构和内存布局。
4. 功能扩展：标准库定义了多种类型的迭代器，这些迭代器具有不同的特性和功能，如输入迭代器、输出迭代器、前向迭代器、双向迭代器和随机访问迭代器等。这些迭代器提供了更丰富的功能和算法支持。普通指针则只能进行基本的指针运算。

总的来说，迭代器提供了更丰富的抽象和功能，使得它们在许多场景下比普通指针更加灵活和安全。

# problems__静态分配内存和动态分配内存有什么区别？.md

静态内存分配和动态内存分配是两种不同的内存管理方式，它们的主要区别在于分配时间、大小变化和灵活性：

1. 静态内存分配：在程序编译阶段，内存就已经被分配好。程序运行时，静态分配的内存大小是固定不变的。比如全局变量，常量，以及函数中声明的局部变量等都是静态分配内存的。优点是管理起来比较简单，缺点是可能会造成内存空间的浪费。
2. 动态内存分配：在程序运行过程中，根据需要动态地分配内存空间。使用者可以自己控制何时申请内存，何时释放内存，具有很大的灵活性。例如在C语言中，我们可以通过malloc，calloc，realloc等函数进行动态内存分配。优点是可以更有效地利用内存资源，缺点是如果管理不当（例如忘记释放内存），就可能导致内存泄漏问题。

# problems__面向对象的三大特征是什么.md

面向对象编程的三大特征是：封装、继承和多态。

1. 封装（Encapsulation）：封装是把数据（变量）和操作数据的函数结合在一起，形成一个“对象”。这个数据类型的内部实现细节可以被隐藏起来，只暴露必要的接口给外部使用。封装可以提高代码的重用性，改善程序设计的可维护性。
2. 继承（Inheritance）：继承是子类自动共享父类数据结构和方法的机制，这使得子类对象可以达到父类对象的所有属性和行为。子类还可以添加自己的新的属性和行为。这种特性有助于减少代码重复，并且可以提高代码的可维护性和复用性。
3. 多态（Polymorphism）：多态意味着调用哪个对象的哪个方法，取决于运行时该对象所属的类。多态可以提高代码的灵活性和可扩展性。在C++中，多态通常通过虚函数实现。
