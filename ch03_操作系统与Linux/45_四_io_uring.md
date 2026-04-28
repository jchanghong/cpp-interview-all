## 四、io_uring

### 4.1 什么是 io_uring

io_uring 是 Linux 5.1+ 引入的新一代异步 I/O 接口，由 Jens Axboe 开发。它通过**共享环形缓冲区**实现用户态和内核态的零拷贝通信。

### 4.2 核心优势

- **零系统调用**：通过共享内存的 Submission Queue (SQ) 和 Completion Queue (CQ)，一次 `io_uring_enter` 可提交多个 I/O 请求
- **支持异步**：真正的异步 I/O，不阻塞调用线程
- **通用 I/O**：支持 read/write、accept/connect、send/recv、open/close、splice 等
- **高性能**：比 epoll + 非阻塞 I/O 吞吐量更高，延迟更低

### 4.3 基本使用流程

```cpp
// 1. 初始化
struct io_uring ring;
io_uring_queue_init(32, &ring, 0);

// 2. 提交请求
struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
io_uring_prep_read(sqe, fd, buf, size, offset);
io_uring_sqe_set_data(sqe, user_data);
io_uring_submit(&ring);

// 3. 等待完成
struct io_uring_cqe *cqe;
io_uring_wait_cqe(&ring, &cqe);
// 处理完成结果
io_uring_cqe_seen(&ring, cqe);
```

### 4.4 与 epoll 对比

| 特性 | epoll | io_uring |
|------|-------|----------|
| 系统调用次数 | 每次事件循环至少 1 次 | 批量提交，大幅减少 |
| 数据拷贝 | 需要用户态 read/write | 内核直接拷贝到用户 buffer |
| 适用场景 | 事件驱动、连接管理 | 高吞吐 I/O、文件 I/O |
| 内核版本 | 2.6+ | 5.1+ |
