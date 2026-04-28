## 三、Proactor 模式

### 3.1 与 Reactor 的区别

| 特性 | Reactor | Proactor |
|------|---------|----------|
| I/O 模型 | 同步 I/O 多路复用 | 异步 I/O（AIO） |
| 事件通知 | "FD 可读/可写" | "读/写操作已完成" |
| 数据拷贝 | 用户态手动 read/write | 内核自动完成，回调通知 |
| 实现难度 | 较低 | 较高 |
| Linux 支持 | epoll（成熟） | io_uring / AIO（有限） |

### 3.2 Proactor 工作流程

```
1. 发起异步读请求（传递 buffer 和回调）
2. 内核在后台完成数据读取（DMA 直接拷贝到用户 buffer）
3. 内核通知用户态："读操作完成，数据已在 buffer 中"
4. 用户回调直接处理数据，无需 read() 调用
```

> **Linux 现状**：Linux 原生 AIO 仅支持 Direct I/O（绕过页缓存），对普通文件和网络 socket 支持有限。io_uring 是目前 Linux 上最接近 Proactor 语义的方案。
