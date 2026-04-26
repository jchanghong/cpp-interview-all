#include "rpc_framework.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

using rpcdemo::DeserializeRequest;
using rpcdemo::DeserializeResponse;
using rpcdemo::RpcRequest;
using rpcdemo::RpcResponse;
using rpcdemo::RpcServer;
using rpcdemo::SerializeRequest;
using rpcdemo::SerializeResponse;

// 函数：在条件不满足时抛出异常。
static void Require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

// 函数：验证请求与响应的序列化和反序列化逻辑。
static void TestSerialization() {
    // 结构体变量：构造测试请求。
    RpcRequest request{"echo", {"hello", "world"}};
    // 字符串变量：保存序列化请求文本。
    std::string request_payload = SerializeRequest(request);
    // 结构体变量：保存反序列化后的请求。
    RpcRequest parsed_request = DeserializeRequest(request_payload);

    Require(parsed_request.method == "echo", "rpc request method mismatch");
    Require(parsed_request.arguments.size() == 2U, "rpc request arguments size mismatch");

    // 结构体变量：构造测试响应。
    RpcResponse response{true, "ok"};
    // 字符串变量：保存序列化响应文本。
    std::string response_payload = SerializeResponse(response);
    // 结构体变量：保存反序列化后的响应。
    RpcResponse parsed_response = DeserializeResponse(response_payload);

    Require(parsed_response.success, "rpc response success mismatch");
    Require(parsed_response.payload == "ok", "rpc response payload mismatch");
}

// 函数：验证方法注册和请求分发逻辑。
static void TestDispatch() {
    // 对象变量：创建被测 RPC 服务端。
    RpcServer server;
    server.Register("concat", [](const std::vector<std::string>& arguments) {
        return arguments.at(0) + arguments.at(1);
    });

    // 结构体变量：构造待执行请求。
    RpcRequest request{"concat", {"foo", "bar"}};
    // 结构体变量：保存服务端响应结果。
    RpcResponse response = server.Handle(request);

    Require(response.success, "rpc dispatch should succeed");
    Require(response.payload == "foobar", "rpc dispatch payload mismatch");
}

// 主函数：顺序执行所有 RPC 测试。
int main() {
    try {
        TestSerialization();
        TestDispatch();
        std::cout << "All RPC tests passed." << std::endl;
        return EXIT_SUCCESS;
    } catch (const std::exception& ex) {
        std::cerr << "RPC test failed: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}
