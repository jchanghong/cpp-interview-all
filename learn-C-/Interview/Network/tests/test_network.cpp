#include "network_echo.h"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

using networkdemo::RunEchoClientOnce;
using networkdemo::RunEchoServerOnce;

// 函数：在条件不满足时抛出异常。
static void Require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

// 函数：验证本地 echo server/client 的回显结果正确。
static void TestEchoRoundTrip() {
    // 整数变量：定义测试使用的本地端口。
    const int port = 34568;
    // 线程变量：后台运行服务端逻辑。
    std::thread server([port]() { RunEchoServerOnce(port); });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // 字符串变量：保存客户端收到的回显内容。
    std::string response = RunEchoClientOnce(port, "ping");
    server.join();

    Require(response == "ping", "network echo mismatch");
}

// 主函数：顺序执行所有网络测试。
int main() {
    try {
        TestEchoRoundTrip();
        std::cout << "All network tests passed." << std::endl;
        return EXIT_SUCCESS;
    } catch (const std::exception& ex) {
        std::cerr << "Network test failed: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}
