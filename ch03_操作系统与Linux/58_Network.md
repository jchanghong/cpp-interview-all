## Network

这是历史工程的说明页：原文对应一个最小本地 `TCP echo server/client` 示例，适合面试里讲 socket 生命周期、绑定监听、连接、收发和资源管理；当前目录不包含可直接构建的源码。

### 构建（历史示例）

```bash
# 原文示例：旧工程目录
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

### 目标

- `network_demo`：历史工程中的示例目标，启动一次本地 echo server 并发起一次回显请求。
- `network_tests`：历史工程中的测试目标，验证客户端收到的回显内容正确。

### 面试知识点

- TCP 服务端典型流程：`socket -> bind -> listen -> accept -> recv/send -> close`。
- 客户端典型流程：`socket -> connect -> send/recv -> close`。
- 高频追问：半包粘包、阻塞与非阻塞、IO 多路复用、连接关闭时序、心跳机制。
- RAII 封装文件描述符是很常见的加分点。


> 跨目录学习路线索引和 Linux 命令大全已分别移除。Linux 命令相关内容请参见《操作系统与Linux》目录。
