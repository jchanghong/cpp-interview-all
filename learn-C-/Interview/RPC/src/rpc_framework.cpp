#include "rpc_framework.h"

#include <sstream>
#include <stdexcept>

namespace rpcdemo {

// 函数：使用指定分隔符切分字符串。
static std::vector<std::string> Split(const std::string& text, char delimiter) {
    // 容器变量：保存切分后的结果片段。
    std::vector<std::string> parts;
    // 输入流变量：从字符串中逐段提取字段。
    std::istringstream stream(text);
    // 字符串变量：保存当前字段内容。
    std::string item;

    while (std::getline(stream, item, delimiter)) {
        parts.push_back(item);
    }

    return parts;
}

// 业务函数：保存给定方法名对应的处理器。
void RpcServer::Register(const std::string& method, Handler handler) {
    handlers_[method] = std::move(handler);
}

// 业务函数：根据方法名查找并执行处理器。
RpcResponse RpcServer::Handle(const RpcRequest& request) const {
    // 迭代器变量：查找目标方法对应的处理器。
    auto found = handlers_.find(request.method);
    if (found == handlers_.end()) {
        return RpcResponse{false, "method not found"};
    }

    return RpcResponse{true, found->second(request.arguments)};
}

// 函数：把方法名和参数拼成简单文本协议。
std::string SerializeRequest(const RpcRequest& request) {
    // 输出流变量：按约定格式组装文本协议。
    std::ostringstream stream;
    stream << request.method;

    // 引用变量：依次输出每个参数。
    for (const std::string& argument : request.arguments) {
        stream << "|" << argument;
    }

    return stream.str();
}

// 函数：把文本协议切分为方法名和参数列表。
RpcRequest DeserializeRequest(const std::string& payload) {
    // 容器变量：保存切分后的字段。
    std::vector<std::string> parts = Split(payload, '|');
    if (parts.empty()) {
        throw std::invalid_argument("invalid request payload");
    }

    // 结构体变量：构造解析后的请求对象。
    RpcRequest request;
    request.method = parts.front();
    request.arguments.assign(parts.begin() + 1, parts.end());
    return request;
}

// 函数：把响应对象编码为简单文本协议。
std::string SerializeResponse(const RpcResponse& response) {
    return std::string(response.success ? "1" : "0") + "|" + response.payload;
}

// 函数：把文本协议解析为响应对象。
RpcResponse DeserializeResponse(const std::string& payload) {
    // 容器变量：保存切分后的字段。
    std::vector<std::string> parts = Split(payload, '|');
    if (parts.size() < 2U) {
        throw std::invalid_argument("invalid response payload");
    }

    return RpcResponse{parts[0] == "1", payload.substr(2)};
}

}  // namespace rpcdemo
