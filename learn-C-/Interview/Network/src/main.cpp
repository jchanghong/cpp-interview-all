#include "network_echo.h"

#include <chrono>
#include <iostream>
#include <thread>

using networkdemo::RunEchoClientOnce;
using networkdemo::RunEchoServerOnce;

// 主函数：在本地线程里启动 echo server 并发起一次客户端请求。
int main() {
    // 整数变量：定义演示使用的本地端口。
    const int port = 34567;
    // 线程变量：后台运行一次性 echo server。
    std::thread server([port]() { RunEchoServerOnce(port); });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // 字符串变量：保存客户端收到的回显内容。
    std::string response = RunEchoClientOnce(port, "hello network");
    server.join();

    std::cout << "network_demo response=" << response << std::endl;
    return 0;
}
