# problems__keepalive是什么？如何使用？.md

Keepalive是一个网络功能，用于检测两台计算机之间的连接是否仍然活跃。在TCP/IP协议中，它是指定期发送探测包以确保连接仍然存在并且对方尚未崩溃或无法到达。Keepalive可以帮助识别死亡、挂起或不再活跃的连接，并允许应用程序采取相应的行动，如重新连接或释放资源。

在很多情况下，默认的TCP连接没有开启Keepalive，因为频繁地发送Keepalive探测包可能会增加不必要的网络流量。但是，在长时间打开的连接中（例如数据库连接），可能需要使用Keepalive来确保持续的服务可用性。

**如何使用Keepalive：**

1. 在Socket编程中设置：

   Keepalive参数通常可以通过socket的选项来设置。例如，在C语言中，你可以使用`setsockopt`函数来设置TCP Keepalive：

   ```c
   int optval = 1;
   socklen_t optlen = sizeof(optval);

   // 开启Keepalive属性
   if(setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
       perror("setsockopt");
       close(sock);
       exit(EXIT_FAILURE);
   }
   ```

   具体的Keepalive参数（例如探测包的发送频率和重试次数）依赖于操作系统，并且可能有不同的接口进行配置。

2. 在操作系统级别设置：

   对于类Unix系统，比如Linux，你可以通过修改系统文件中的以下参数来调整Keepalive的行为：

   - `/proc/sys/net/ipv4/tcp_keepalive_time`：在开始发送Keepalive探测前的空闲时间。
   - `/proc/sys/net/ipv4/tcp_keepalive_probes`：在断开连接前发送Keepalive探测的最大次数。
   - `/proc/sys/net/ipv4/tcp_keepalive_intvl`：两个Keepalive探测之间的间隔。

   **例如**：

   ```
   echo 600 > /proc/sys/net/ipv4/tcp_keepalive_time
   echo 10 > /proc/sys/net/ipv4/tcp_keepalive_probes
   echo 60 > /proc/sys/net/ipv4/tcp_keepalive_intvl
   ```

   这些命令将设置TCP连接在空闲10分钟后开始发送Keepalive探测，每次发送间隔1分钟，总共发送10次探测。

3. 在应用程序层面设置：

   很多高级语言的网络库也提供了设置Keepalive的接口。例如，在Python的`socket`模块中，可以这样设置Keepalive：

```c++
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_BUFFER_SIZE 4096
#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[MAX_BUFFER_SIZE] = {0};

    // 创建 socket 文件描述符
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 绑定 socket 到 localhost 的 8080 端口
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 使服务器开始监听，这里我们设定最大待处理连接数为 3
    if (listen(server_fd, 3) < 0) {
        std::cerr << "listen failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    while(1) {
        std::cout << "\nWaiting for a connection..." << std::endl;

        // 接受客户端连接
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            std::cerr << "accept failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        // 清空 buffer
        memset(buffer, 0, MAX_BUFFER_SIZE);

        // 读取客户端发送的数据
        int valread = read(new_socket , buffer, MAX_BUFFER_SIZE);
        std::cout << "Client says: " << buffer << std::endl;

        // 向客户端发送消息
        send(new_socket , "Hello from server" , strlen("Hello from server") , 0 );
    }

    return 0;
}

```


#### select、poll和epoll的区别

当有多个计算机对我们的服务端发起连接请求的时候，我们需要处理这些请求，于是我们就有了这些处理的方法，当然，这些是最常见的，面试常考的

1. select:
   - **select工作方式是轮询检查**用户所关心的**socket是否就绪**，然后进行处理。由于每次调用select都要在用户态和内核态之间进行切换，且需要重新传递数据结构，所以效率较低。
   - **select对socket数量也有限制，默认是1024个**，因为它使用了位图的方式来存储socket信息。
   - select无法扩展到大规模并发连接，主要是因为它采用线性遍历的方式来处理事件，同时每次调用select都需要遍历全部的文件描述符。
2. poll：
   - **poll没有最大文件描述符数量的限制**。在select中，由于它使用了位图的方式来存储文件描述符，所以默认的最大数目是1024。而**poll使用链表来存储**，所以理论上只受限于系统内存。
   - poll提供了更多的事件类型，并且它的事件类型是通过结构体的方式进行传递的，这远比select的位操作要清晰很多。
   - 当文件描述符就绪时，select需要重新设置所有文件描述符，但poll只需要关注那些就绪的文件描述符。
3. epoll
   - **epoll是Linux特有的I/O多路复用机制，相比于select和poll，通过使用基于红黑树的底层数据结构使得epoll更加灵活且没有最大并发限制。**
   - epoll使用一个文件描述符管理多个事件，通过系统调用epoll_ctl注册fd，然后epoll_wait获取已经准备好的描述符。
   - epoll中的系统调用会将所有监听的socket进行逻辑上的分组，应用程序只需要检查已经就绪的socket。这样在实际开发中，可以根据应用程序的实际需求，只监听并处理那些真正需要关注的事件，而无需像select和poll那样每次都进行全量的轮询。
   - epoll的效率比select高，主要体现在大量并发连接的处理上。
