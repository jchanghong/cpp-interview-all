# RPC

这个工程实现了一个极简 RPC 框架，覆盖面试中最容易被追问的几个核心点：请求结构、序列化、方法注册、服务端分发和响应返回。

## 构建

```bash
cd /home/wj/Code/C++/Interview/RPC
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## 目标

- `rpc_demo`：演示一次 `add` 请求从序列化到分发再到响应解析。
- `rpc_tests`：验证序列化和分发逻辑。

## 面试知识点

- RPC 的本质是“像调用本地函数一样调用远端服务”。
- 核心链路一般包括：接口定义、序列化、网络传输、服务发现、路由分发、超时重试。
- 实际工程里常见协议：JSON、Protobuf、Thrift、自定义二进制协议。
- 高频追问：幂等性、超时、重试、负载均衡、限流、熔断、服务治理。

