#include "network_echo.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <system_error>

namespace networkdemo {

// 构造函数：保存当前 socket 描述符。
ScopedSocket::ScopedSocket(int fd) : fd_(fd) {}

// 析构函数：在描述符有效时执行关闭。
ScopedSocket::~ScopedSocket() {
    if (fd_ >= 0) {
        close(fd_);
    }
}

// 移动构造函数：转移描述符并清空源对象。
ScopedSocket::ScopedSocket(ScopedSocket&& other) noexcept : fd_(other.fd_) {
    other.fd_ = -1;
}

// 移动赋值函数：释放旧描述符并接管新描述符。
ScopedSocket& ScopedSocket::operator=(ScopedSocket&& other) noexcept {
    if (this != &other) {
        if (fd_ >= 0) {
            close(fd_);
        }
        fd_ = other.fd_;
        other.fd_ = -1;
    }
    return *this;
}

// 查询函数：返回当前保存的描述符。
int ScopedSocket::Get() const {
    return fd_;
}

// 函数：创建监听 socket 并绑定到本地回环地址。
static ScopedSocket CreateListeningSocket(int port) {
    // 整数变量：保存 socket 系统调用返回值。
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        throw std::system_error(errno, std::generic_category(), "socket failed");
    }

    // 对象变量：接管刚创建的 socket。
    ScopedSocket server_socket(server_fd);
    // 整数变量：允许地址快速复用。
    int option = 1;
    setsockopt(server_socket.Get(), SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    // 结构体变量：描述监听地址。
    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(static_cast<uint16_t>(port));

    if (bind(server_socket.Get(), reinterpret_cast<const sockaddr*>(&address), sizeof(address)) != 0) {
        throw std::system_error(errno, std::generic_category(), "bind failed");
    }

    if (listen(server_socket.Get(), 1) != 0) {
        throw std::system_error(errno, std::generic_category(), "listen failed");
    }

    return server_socket;
}

// 函数：接收客户端消息并原样回写。
void RunEchoServerOnce(int port) {
    // 对象变量：创建本地监听 socket。
    ScopedSocket server_socket = CreateListeningSocket(port);
    // 整数变量：保存 accept 返回的客户端描述符。
    int client_fd = accept(server_socket.Get(), nullptr, nullptr);
    if (client_fd < 0) {
        throw std::system_error(errno, std::generic_category(), "accept failed");
    }

    // 对象变量：接管客户端 socket。
    ScopedSocket client_socket(client_fd);
    // 数组变量：保存接收到的字节数据。
    char buffer[256];
    // 整数变量：保存本次接收的字节数。
    ssize_t received = recv(client_socket.Get(), buffer, sizeof(buffer), 0);
    if (received < 0) {
        throw std::system_error(errno, std::generic_category(), "recv failed");
    }

    // 大小变量：记录当前已经发送的字节数。
    std::size_t sent = 0;
    while (sent < static_cast<std::size_t>(received)) {
        // 整数变量：保存本次 send 的返回值。
        ssize_t result = send(client_socket.Get(), buffer + sent, static_cast<std::size_t>(received) - sent, 0);
        if (result < 0) {
            throw std::system_error(errno, std::generic_category(), "send failed");
        }
        sent += static_cast<std::size_t>(result);
    }
}

// 函数：连接本地服务端并收发一条消息。
std::string RunEchoClientOnce(int port, const std::string& message) {
    // 整数变量：保存 socket 系统调用返回值。
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        throw std::system_error(errno, std::generic_category(), "socket failed");
    }

    // 对象变量：接管客户端 socket。
    ScopedSocket client_socket(client_fd);
    // 结构体变量：描述目标服务端地址。
    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(static_cast<uint16_t>(port));
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    if (connect(client_socket.Get(), reinterpret_cast<const sockaddr*>(&address), sizeof(address)) != 0) {
        throw std::system_error(errno, std::generic_category(), "connect failed");
    }

    // 大小变量：记录当前已经发送的字节数。
    std::size_t sent = 0;
    while (sent < message.size()) {
        // 整数变量：保存本次 send 的返回值。
        ssize_t result = send(client_socket.Get(), message.data() + sent, message.size() - sent, 0);
        if (result < 0) {
            throw std::system_error(errno, std::generic_category(), "send failed");
        }
        sent += static_cast<std::size_t>(result);
    }

    // 数组变量：保存服务端回包内容。
    char buffer[256];
    // 整数变量：保存本次接收的字节数。
    ssize_t received = recv(client_socket.Get(), buffer, sizeof(buffer), 0);
    if (received < 0) {
        throw std::system_error(errno, std::generic_category(), "recv failed");
    }

    return std::string(buffer, buffer + received);
}

}  // namespace networkdemo
