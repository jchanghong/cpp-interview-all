#ifndef NETWORK_ECHO_H
#define NETWORK_ECHO_H

#include <string>

namespace networkdemo {

// 类：管理一个 POSIX socket 文件描述符的生命周期。
class ScopedSocket {
public:
    // 构造函数：接管已有 socket 描述符。
    explicit ScopedSocket(int fd = -1);

    // 析构函数：关闭当前持有的 socket。
    ~ScopedSocket();

    // 删除拷贝构造：避免重复关闭同一描述符。
    ScopedSocket(const ScopedSocket&) = delete;

    // 删除拷贝赋值：避免重复关闭同一描述符。
    ScopedSocket& operator=(const ScopedSocket&) = delete;

    // 移动构造函数：转移 socket 所有权。
    ScopedSocket(ScopedSocket&& other) noexcept;

    // 移动赋值函数：释放旧描述符并转移新描述符。
    ScopedSocket& operator=(ScopedSocket&& other) noexcept;

    // 查询函数：返回当前持有的原始文件描述符。
    int Get() const;

private:
    // 成员变量：保存 socket 文件描述符。
    int fd_;
};

// 函数：启动一个仅处理单次请求的本地 echo server。
void RunEchoServerOnce(int port);

// 函数：连接到本地 echo server 并执行一次收发。
std::string RunEchoClientOnce(int port, const std::string& message);

}  // namespace networkdemo

#endif
