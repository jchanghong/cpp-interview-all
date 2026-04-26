# CrossProcess

这是一个面向面试场景的 C++ 跨进程通信示例项目，使用 `CMake` 组织构建，包含 3 类常见跨进程方案：

- `shared_counter_demo`：基于 `POSIX shared memory + pthread_mutex(process-shared)` 的共享计数器。
- `shared_queue_demo`：基于 `POSIX shared memory + semaphore + mutex` 的共享队列。
- `file_lock_demo`：基于 `fcntl` 文件锁的串行化写文件示例。
- `cross_process_tests`：不依赖第三方测试框架的轻量单元测试程序，可直接通过 `ctest` 运行。

## 目录结构

```text
CrossProcess
├── CMakeLists.txt
├── README.md
├── include
│   ├── ipc_file_lock.h
│   ├── ipc_shared_counter.h
│   └── ipc_shared_queue.h
├── src
│   ├── file_lock_demo.cpp
│   ├── ipc_file_lock.cpp
│   ├── ipc_shared_counter.cpp
│   ├── ipc_shared_queue.cpp
│   ├── shared_counter_demo.cpp
│   └── shared_queue_demo.cpp
└── tests
    └── test_cross_process.cpp
```

## 构建与运行

```bash
cd /home/wj/Code/C++/Interview/CrossProcess
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

运行示例程序：

```bash
./build/shared_counter_demo
./build/shared_queue_demo
./build/file_lock_demo
```

## 示例说明

### 1. 共享计数器

- 核心对象：`shm_open`、`ftruncate`、`mmap`、`pthread_mutex_t`
- 适合场景：多个进程共享一小段状态，例如计数器、统计信息、状态位。
- 关键点：`pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED)` 让互斥量可跨进程使用。

### 2. 共享队列

- 核心对象：共享内存 + `sem_t` + `pthread_mutex_t`
- 适合场景：父子进程或多个本机进程之间做高效的小消息传递。
- 关键点：`slots` 管空位，`items` 管数据量，`mutex` 保护环形队列指针。

### 3. 文件锁

- 核心对象：`fcntl(F_SETLKW)`
- 适合场景：多个进程竞争写同一个文件，但对性能要求不如共享内存高。
- 关键点：文件锁更偏“协调访问”，而不是“高吞吐通信”。

## 单元测试覆盖点

- 多子进程并发自增后，共享计数器结果是否正确。
- 共享队列是否保持 FIFO 顺序。
- 文件锁是否让多进程追加写入保持完整的行记录。

## 面试知识点补充：跨进程与内存池的联系

下面这部分专门补充“内存池”相关知识点，方便你在讲跨进程题目时顺带展开。

### 1. 为什么跨进程场景会谈到内存池

- 共享内存本质上就是一块被多个进程映射的连续地址空间。
- 如果这块共享内存里要频繁创建对象、消息节点、任务描述符，就会遇到“谁来分配/回收”的问题。
- 这时常见做法是在共享内存内部再实现一个“共享内存池”或“对象池”。

### 2. 共享内存内存池和普通进程内内存池的区别

- 普通内存池保存的是当前进程内可直接使用的裸指针。
- 跨进程内存池不能直接依赖裸指针，因为不同进程映射到的虚拟地址可能不同。
- 更稳妥的方式是保存“偏移量 offset”而不是绝对地址，再通过共享基址恢复访问位置。

### 3. 设计共享内存内存池时的核心点

- 固定大小块分配最简单，适合消息节点、任务节点、连接描述符等。
- 空闲链表如果跨进程共享，通常要配合互斥锁、原子操作或 CAS。
- 元数据必须也位于共享内存中，否则多个进程看到的池状态不一致。
- 要考虑进程异常退出后的恢复问题，否则锁和空闲链表可能损坏。

### 4. 面试里高频延伸问题

- 为什么共享内存速度快：因为少了一次内核态数据拷贝，多个进程直接访问同一块物理页。
- 为什么共享内存还需要同步：共享的是数据，不共享访问时序，所以还要锁、信号量或原子变量。
- 共享内存池为什么常用固定块：实现简单、碎片少、分配回收时间稳定。
- 为什么不能把 STL 容器直接丢进共享内存：很多实现内部保存的是进程私有地址，跨进程不安全。

### 5. 一句话回答模板

如果面试官追问“跨进程通信和内存池有什么结合点”，可以这样回答：

> 共享内存解决的是“多进程看到同一块数据”的问题，内存池解决的是“这块数据里如何高效分配对象”的问题。实际工程里经常把共享内存作为底层载体，再在其上实现固定块内存池、对象池或环形队列。
