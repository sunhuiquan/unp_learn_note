# unp_learn_note

我学习 UNPv13e(《UNIX 网络编程 卷一：套接字联网 API》第三版)做到习题及部分笔记。

## Index

- [预备工作](#CH0-预备工作)
- [CH1 简介](#CH1-简介)
- [CH2 传输层：TCP、UDP、SCTP](#CH2-传输层TCPUDPSCTP)
- [CH3 套接字编程简介](#CH3-套接字编程简介)
- [CH4 基本 TCP 套接字编程](#ch4-基本-tcp-套接字编程)
- [CH5 TCP 客户/服务器程序示例](#CH5-tcp-客户服务器程序示例)
- [CH6 I/O 复用：select 和 poll 函数](#ch6-io-复用select-和-poll-函数)
- [CH7 套接字选项](#CH7-套接字选项)
- [CH8 基本 UDP 套接字编程](#CH8-基本-UDP-套接字编程)
- [CH9 基本 SCTP 套接字编程](#CH9-基本-SCTP-套接字编程)
- [CH10 SCTP 客户/服务器程序例子](#CH10-SCTP-客户服务器程序例子)
- [CH11 名字与地址转换](#CH11-名字与地址转换)

### CH0 预备工作

[unpv13e repo](https://github.com/unpbook/unpv13e)  
[UNP 书上源码](./unpv13e)  
[UNP README](./unpv13e/README)

准备工作，先把 libunp.a(在 libfree make 生成) 放到 /usr/local/lib 中，然后把 unp.h(unpv13e/lib 目录) 和 config.h(unpv13e 目录) 放到 /usr/local/include 中，改一下 unp.h 让 #include "config.h" 而不是 "../config.h"。  
做好这些工作之后，可以直接代码中引用 unp.h，并 -lunp 连接静态库了，另外在 linux 上可能要去掉对 sys/sysctl.h 的引用，因为这个已经被弃用。  
最后为什么放在 /usr/local 下呢，因为这个是用户级第三方非正式的东西放的目录，比较合适。

### Note CH0

如果运行 ./daytimetcpcli 127.0.0.1 显示 refused，这是因为 inetd 默认没开 daytime 服务，你修改 /etc/inetd.conf, 然后 service inetd restart 重启服务即可，另外 linux ubuntu 可能没安 inetd，需要 apt install 一下。

---

### CH1 简介

### 1.1 - 1.4

太过简单，略

### 1.5

[代码](./exercise/ch1/5) linux ubuntu 本机上是 1，但不同 os 不同 tcp 具体实现的结果不同，说明这个几次 write 是不特定的，不能依赖于特定的实现。

---

### CH2 传输层：TCP、UDP、SCTP

### 2.1 - 2.2

略

### 2.3

MSS 正常情况下是 MTU - 40(TCP 头和 IP 头各 20)，如果未指定，那么默认是 536，这是一个较小的值。这个大小用于指明要传输且不需要分片的数据的最大大小(数据大小 + 40 <= MTU 就自然不会分片)。  
为了减少分片，MSS 协商(两端通过 MTU 算出 MSS(注意 MTU 和 MSS 都是一个方向一个单线链路的，全双工由两个单向通信链路组成，这两个单向链路的 MTU 完全可以不同，因为经过的路径的路由器的 MTU 可以不同))。  
另外一种减少分片的方式是路径 MTU 发现(PMTUD)，通过 ICMP 报文得知，但是由于现在网络的复杂性和防火墙等，可能会过滤掉 ICMP 报文，导致 PMTUD 失败，这各技术现在不大可靠。  
以太网 MTU 一般是 1500，局域网有些较小的一般是 576，当然这只是大部分值。

### 2.4

### 2.5

### 2.6

### 2.7

选择性确认(SACK)只确认了所选择的数据，并不是累计确认那样确定从之前所有数据到这个点，而是可以在这个 SACK 确认的点之前仍有未确认的数据。

---

### CH3 套接字编程简介

### 3.1

通过指针才能把结果返回给调用者。

### 3.2

void \*的指针值计算没有意义,C 不允许因此 void \*指针加减运算(本质是因为不知道 void \*指向的类型的大小，无法计算)，char \*的指针值计算才有意义，char \*指针加/减 1 分别代表地址加/减一个 char 类型大小的地址。

### 3.3

[代码](./exercise/ch3/3/inet_pton_loose.c) 一开始没看懂意思，然后才知道 1 是成功，0 是格式错误，-1 是其他错误，才明白这道题让我干什么。。。

---

### CH4 基本 TCP 套接字编程

### 4.1

看非全 0、全 1 之类的地址，比如 #define INADDR_MAX_LOCAL_GROUP ((in_addr_t) 0xe00000ff) /\* 224.0.0.255 \*/ 而我的 linux 正是小端机器，所以可以看出是主机序。

### 4.2

另外如果移动到 connect() 前，那么结果如图  
![IMG](./IMG/4_2.png)  
说明确实是 connect() 建立连接，内核才会给 socket 绑定了一个 ip(ip 如果是多宿主机有多个外出接口(网卡，有多个 ip 可选的情况)，那么具体是哪一个 ip 地址取决于路由算法的结果)和临时端口

### 4.3

按题意顺序  
1 子进程关闭 listenfd  
2 子进程关闭 connfd，因为父进程仍然有 connfd，所以并未关闭连接
3 父进程关闭 connfd，此时才彻底关闭 connfd，发出 FIN，父进程发起主动关闭

### 4.4

[代码](./exercise/ch4/4/daytimetcp_sv.c) 输出 accept error: Invalid argument，这是因为 accept 的 socket 参数不是监听套接字而只是一个主动套接字，导致 EINVAL 错误

### 4.5

[代码](./exercise/ch4/5/daytimetcp_sv.c) 发现 socket 生成的一个主动套接字一开始没有分配 ip 和端口，connect 或者 listen 在发现这种情况(没有 bind 显式绑定)，内核就会自动分配一个 ip 和临时端口(linux 下 listen 分配的 ip 是通配 ip，connect 是根据路由分配一个 ip)

---

### CH5 TCP 客户/服务器程序示例

### 5.1

[代码](./exercise/ch5/1) [记录](./exercise/ch5/1/record) 差不多 TIME_WAIT 持续了 61s 左右，反推出 MSL(最大分组生命期)是 30s 左右。

一开始想给 shell 脚本加个 set-user-ID-root，结果发现不行，然后忽然想起来 linux 下因为安全性问题，不能给 shell 脚本使用这种获取特权的机制，还是老老实实 sudo 执行吧。。。

犯了个弱智错误，那就是不要 grep 程序名，因为进程结束后，内核占用端口显示 TIME_WAIT，但这个时候进程都结束了，当然没有进程名了，通过监听端口可以 grep 到。

### 5.2

fgets 把'\0'作为结尾，而二进制文件里面的值可能有对应'\0'字符，这样会导致发送适以错误的'\0'为结尾，这样可以导致这次发送的没有'\n'，让服务器阻塞在 readline，导致死锁。(这里问的是书中的 ch5 里面那个 readline 的服务器程序)

### 5.3

Telnet 把输入行后面以 \r\n 结束，而我们的程序以 \n 结束。

### 5.4

### 5.5

### 5.6

### 5.7

### 5.8

### 5.9

### 5.10

会造成永久阻塞，32 位客户机发送 2 两个 32 位值到缓冲区，64 位服务器等待两个 64 位值，而缓冲区的两个 32 位值被接受到一个 64 位值取走，剩下一个 64 位值的读取会永远阻塞。

### 5.11

IP 路由功能，根据目的 IP 地址，查路由表确定外出接口和下一跳，这个接口的主 IP 地址用做源 IP 地址，前提是未绑定 bind 到一个地址，bind 后则内核不再分配。

---

### CH6 I/O 复用：select 和 poll 函数

### 6.1

```c
   __fd_mask __fds_bits[__FD_SETSIZE / __NFDBITS];
=> fd_mask fd_set::__fds_bits[16]
=> long fd_set::__fds_bits[16]
```

可见 fd_set 里面就一个 long 的数组，因为 C 不允许数组直接赋值，但允许结构体直接赋值，所以给数组包装一个结构体即可。

### 6.2

write 要写入的数据如果大于 socket 当前缓冲区可用的空间(如果是大于 socket 缓冲区的最大大小，那么会分批写入，不会永远阻塞)会阻塞，直到可用空间足够大，所以非阻塞 write 可能返回正值也可能阻塞，不会像 read 一样能返回不足值，只能设置非阻塞，这个时候是写入能写入的大小，返回一个不足值。  
这样避免了阻塞，非常重要，如果在 write 处长时间甚至永远阻塞，这个单进程服务器根本无法处理服务。

### 6.3

由于每次都是先检查 sockfd，而且是二选一，那么 sockfd 不停就绪可以让另一个 fd 处于饥饿。  
另外 select 不会实际读写数据，所以这次就绪但没操作，那么下一次会立即得知这个就绪。

### 6.4

[代码实现](./exercise/ch6/4/sv.c) 就是简单地把软限制改到内核所允许的最大，即硬限制，这样就能使用内核所允许的资源的硬上限了。  
RLIMIT_NOFILE 是最大的打开文件描述数。

### 6.5

首先配置下/etc/inetd.conf 配置文件(对于 stream 就用 nowait，对于内置程序就 internal，其他照抄)，然后重启 inetd 服务，注意我的 ubuntu 需要安装 inetd，且使用的名字是 openbsd-inetd，直接用 inetd service 是不认识的(可以 service --status-all 查看运行的服务的状态)。  
然后 telnet 查看下是不是开了这项服务,telnet 127.0.0.1 19 然后是结果——自动生成一大堆的字符，说明没问题了，剩下的就是简单的做题了。  
[代码实现](./exercise/ch6/5/cl.c)  
然后抓包进行解析，注意本机环回地址不会经过以太网卡，所以如果要监测 localhost 需要-i lo。

```bash
sudo tcpdump -i lo 'tcp and port 19' > a// 抓包，并把数据写入文件,并等待一些时间
//可以使用script之类的程序，这样还可以在终端中看见
more a | grep 'localhost.chargen >'
// grep看的清楚点，因为客户端不向服务器发报文，只是单纯的ack确认
```

首先没有 shutdown 直接 pause(),正常发来数据，直到缓冲区满发送阻塞  
开始是正常发送  
![IMG](./IMG/6_5a.png)  
然后就是缓冲区满了，不再发了  
![IMG](./IMG/6_5b.png)  
然后添加代码 shutdown(SHUT_RD);发现结果和上面一样，说明 linux 的实现不会扔掉缓冲区中收到的数据(其他实现可能会扔掉)，但之后的所有的 read 会得知 EOF(返回 0)，注意不是返回-1 错误。

### 6.6

这个是因为 shutdown 不管引用计数的问题，直接发送 FIN；而 close()只有引用计数变成 0 才会发 FIN 是关闭，其他情况的 close() 只是单纯单纯减少打开文件描述的引用计数而已。

### 6.7

read 返回错误，然后终止程序。  
如果是多进程这样子进程终止还算可以接受，但对着用一个进程处理所有请求的服务器，这显然是不可能接受的，需要针对 errno 错误类型进行不同的处理。

### 6.8

[代码实现](./exercise/ch6/8/sv.c)

---

### CH7 套接字选项

### 7.1

---

### CH8 基本 UDP 套接字编程

### 8.1

---

### CH9 基本 SCTP 套接字编程

### 9.1

---

### CH10 SCTP 客户/服务器程序例子

### 10.1

---

### CH11 名字与地址转换

### 11.1

---
