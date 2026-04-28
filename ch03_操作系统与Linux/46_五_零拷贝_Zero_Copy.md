## 五、零拷贝（Zero Copy）

### 5.1 传统 I/O 的数据拷贝路径

```
磁盘 → 内核缓冲区（DMA） → 用户缓冲区（CPU 拷贝） → Socket 缓冲区（CPU 拷贝） → 网卡（DMA）
```

共 **4 次上下文切换** + **4 次数据拷贝**（其中 2 次 CPU 拷贝）。

### 5.2 sendfile()

```cpp
sendfile(out_fd, in_fd, &offset, count);
```

```
磁盘 → 内核缓冲区（DMA） → Socket 缓冲区（CPU 拷贝） → 网卡（DMA）
```

减少到 **2 次上下文切换** + **3 次数据拷贝**（1 次 CPU 拷贝）。

### 5.3 splice()

```cpp
splice(in_fd, NULL, out_fd, NULL, count, SPLICE_F_MOVE);
```

```
磁盘 → 内核缓冲区（DMA） → Socket 缓冲区（零拷贝，仅指针传递） → 网卡（DMA）
```

减少到 **2 次上下文切换** + **2 次数据拷贝**（0 次 CPU 拷贝，仅 DMA）。

### 5.4 sendfile + TCP_CORK

```cpp
// 启用 TCP_CORK，减少小包发送
int cork = 1;
setsockopt(fd, IPPROTO_TCP, TCP_CORK, &cork, sizeof(cork));
sendfile(fd, file_fd, &offset, file_size);
// 发送完成后关闭
cork = 0;
setsockopt(fd, IPPROTO_TCP, TCP_CORK, &cork, sizeof(cork));
```

### 5.5 零拷贝技术总结

| 技术 | CPU 拷贝次数 | 上下文切换 | 适用场景 |
|------|-------------|-----------|---------|
| 传统 read+write | 2 | 4 | 通用 |
| mmap + write | 1 | 4 | 需要修改数据 |
| sendfile | 1 | 2 | 文件 → socket |
| splice | 0 | 2 | 内核缓冲区间传递 |
