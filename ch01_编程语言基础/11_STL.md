### 📦 STL

#### STL 索引

STL 方法含义索引

#### STL 容器

容器 | 底层数据结构 | 时间复杂度 | 有无序 | 可不可重复 | 其他
---|---|---|---|---|---
array|数组|随机读改 O(1)|无序|可重复|支持随机访问
vector|数组|随机读改、尾部插入、尾部删除 O(1)<br/>头部插入、头部删除 O(n)|无序|可重复|支持随机访问
deque|双端队列|头尾插入、头尾删除 O(1)|无序|可重复|一个中央控制器 + 多个缓冲区，支持首尾快速增删，支持随机访问
forward_list|单向链表|插入、删除 O(1)|无序|可重复|不支持随机访问
list|双向链表|插入、删除 O(1)|无序|可重复|不支持随机访问
stack|deque / list|顶部插入、顶部删除 O(1)|无序|可重复|deque 或 list 封闭头端开口，不用 vector 的原因应该是容量大小有限制，扩容耗时
queue|deque / list|尾部插入、头部删除 O(1)|无序|可重复|deque 或 list 封闭头端开口，不用 vector 的原因应该是容量大小有限制，扩容耗时
priority_queue|vector + max-heap|插入、删除 O(log<sub>2</sub>n)|有序|可重复|vector容器+heap处理规则
set|红黑树|插入、删除、查找 O(log<sub>2</sub>n)|有序|不可重复|
multiset|红黑树|插入、删除、查找 O(log<sub>2</sub>n)|有序|可重复|
map|红黑树|插入、删除、查找 O(log<sub>2</sub>n)|有序|不可重复|
multimap|红黑树|插入、删除、查找 O(log<sub>2</sub>n)|有序|可重复|
unordered_set|哈希表|插入、删除、查找 O(1) 最差 O(n)|无序|不可重复|
unordered_multiset|哈希表|插入、删除、查找 O(1) 最差 O(n)|无序|可重复|
unordered_map|哈希表|插入、删除、查找 O(1) 最差 O(n)|无序|不可重复|
unordered_multimap|哈希表|插入、删除、查找 O(1) 最差 O(n)|无序|可重复|

#### STL 算法

算法 | 底层算法 | 时间复杂度 | 可不可重复
---|---|---|---
find|顺序查找|O(n)|可重复
sort|内省排序|O(n*log<sub>2</sub>n)|可重复


<a id="data-structure"></a>
