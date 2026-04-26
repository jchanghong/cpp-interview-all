#include "rpc_framework.h"

#include <iostream>

using rpcdemo::DeserializeRequest;
using rpcdemo::DeserializeResponse;
using rpcdemo::RpcRequest;
using rpcdemo::RpcServer;
using rpcdemo::SerializeRequest;
using rpcdemo::SerializeResponse;

// 主函数：演示一次最小 RPC 请求的注册、序列化和分发。
int main() {
    // 对象变量：创建最小 RPC 服务端。
    RpcServer server;
    server.Register("add", [](const std::vector<std::string>& arguments) {
        // 整数变量：解析第一个参数。
        const int left = std::stoi(arguments.at(0));
        // 整数变量：解析第二个参数。
        const int right = std::stoi(arguments.at(1));
        return std::to_string(left + right);
    });

    // 结构体变量：构造客户端侧请求。
    RpcRequest request{"add", {"7", "8"}};
    // 字符串变量：模拟网络上传输的请求内容。
    std::string wire_request = SerializeRequest(request);
    // 结构体变量：服务端解析接收到的请求。
    rpcdemo::RpcRequest parsed_request = DeserializeRequest(wire_request);
    // 结构体变量：服务端处理后的响应。
    rpcdemo::RpcResponse response = server.Handle(parsed_request);
    // 字符串变量：模拟网络上传输的响应内容。
    std::string wire_response = SerializeResponse(response);
    // 结构体变量：客户端解析收到的响应。
    rpcdemo::RpcResponse parsed_response = DeserializeResponse(wire_response);

    std::cout << "rpc_demo success=" << parsed_response.success
              << " payload=" << parsed_response.payload << std::endl;
    return 0;
}
