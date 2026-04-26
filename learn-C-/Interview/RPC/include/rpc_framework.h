#ifndef RPC_FRAMEWORK_H
#define RPC_FRAMEWORK_H

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace rpcdemo {

// 结构体：表示一个 RPC 请求。
struct RpcRequest {
    // 成员变量：保存要调用的方法名。
    std::string method;
    // 成员变量：保存请求参数列表。
    std::vector<std::string> arguments;
};

// 结构体：表示一个 RPC 响应。
struct RpcResponse {
    // 成员变量：表示本次调用是否成功。
    bool success;
    // 成员变量：保存成功结果或错误信息。
    std::string payload;
};

// 类：实现一个最小 RPC 方法注册与分发器。
class RpcServer {
public:
    // 类型别名：表示统一的方法处理函数类型。
    using Handler = std::function<std::string(const std::vector<std::string>&)>;

    // 业务函数：注册一个方法处理器。
    void Register(const std::string& method, Handler handler);

    // 业务函数：执行一次 RPC 请求。
    RpcResponse Handle(const RpcRequest& request) const;

private:
    // 成员变量：保存方法名到处理器的映射。
    std::unordered_map<std::string, Handler> handlers_;
};

// 函数：把 RPC 请求序列化为字符串。
std::string SerializeRequest(const RpcRequest& request);

// 函数：把字符串反序列化为 RPC 请求。
RpcRequest DeserializeRequest(const std::string& payload);

// 函数：把 RPC 响应序列化为字符串。
std::string SerializeResponse(const RpcResponse& response);

// 函数：把字符串反序列化为 RPC 响应。
RpcResponse DeserializeResponse(const std::string& payload);

}  // namespace rpcdemo

#endif
