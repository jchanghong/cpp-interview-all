# Network

这个工程实现了一个最小本地 `TCP echo server/client`，适合面试里讲 socket 生命周期、绑定监听、连接、收发和资源管理。

## 构建

```bash
cd /home/wj/Code/C++/Interview/Network
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## 目标

- `network_demo`：启动一次本地 echo server 并发起一次回显请求。
- `network_tests`：验证客户端收到的回显内容正确。

## 面试知识点

- TCP 服务端典型流程：`socket -> bind -> listen -> accept -> recv/send -> close`。
- 客户端典型流程：`socket -> connect -> send/recv -> close`。
- 高频追问：半包粘包、阻塞与非阻塞、IO 多路复用、连接关闭时序、心跳机制。
- RAII 封装文件描述符是很常见的加分点。

