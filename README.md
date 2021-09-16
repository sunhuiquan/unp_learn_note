# unp_learn_note

我学习UNPv13e(《UNIX网络编程 卷一：套接字联网API》第三版)做到习题及部分笔记。

## Index

- [预备工作](#CH0-预备工作)
- [CH1 简介](#CH1-简介)
- [CH2 传输层：TCP、UDP、SCTP](#CH2-传输层TCPUDPSCTP)
- [CH3 套接字编程简介](#CH3-套接字编程简介)
- [CH4 基本TCP套接字编程](#CH4-基本TCP套接字编程)
- [CH5 TCP客户/服务器程序示例](#CH5-tcp客户服务器程序示例)
- [CH6 I/O复用：select和poll函数](#CH6-IO复用select和poll函数)
- [CH7 套接字选项](#CH7-套接字选项)
- [CH8 基本UDP套接字编程](#CH8-基本UDP套接字编程)
- [CH9 基本SCTP套接字编程](#CH9-基本SCTP套接字编程)
- [CH10 SCTP客户/服务器程序例子](#CH10-SCTP客户服务器程序例子)
- [CH11 名字与地址转换](#CH11-名字与地址转换)

### CH0 预备工作

[UNP repo](https://github.com/unpbook/unpv13e)  
[UNP 书上源码](./unpv13e)  
[UNP README](./unpv13e/README)

准备工作，先把 libunp.a(在libfree make生成) 放到 /usr/local/lib 中，然后把 unp.h(unpv13e/lib目录) 和 config.h(unpv13e目录) 放到 /usr/local/include 中，改一下 unp.h 让 #include "config.h" 而不是 "../config.h"。  
做好这些工作之后，可以直接代码中引用 unp.h，并 -lunp 连接静态库了，另外在linux上可能要去掉对 sys/sysctl.h 的引用，因为这个已经被弃用。  
最后为什么放在 /usr/local 下呢，因为这个是用户级第三方非正式的东西放的目录，比较合适。

### Note CH0

如果运行 ./daytimetcpcli 127.0.0.1 显示 refused，这是因为 inetd 默认没开 daytime 服务，你修改 /etc/inetd.conf, 然后 service inetd restart 重启服务即可，另外linux ubuntu 可能没安 inetd，需要 apt install 一下。

---

### CH1 简介

### 1.1 - 1.4

太过简单，略

### 1.5

[代码](./exercise/ch1/5) linux ubuntu本机上是1，但不同os不同tcp具体实现的结果不同，说明这个几次write是不特定的，不能依赖于特定的实现。

---

### CH2 传输层：TCP、UDP、SCTP

### 2.1

---

### CH3 套接字编程简介

### 3.1

通过指针才能把结果返回给调用者。

### 3.2

void \*的指针值计算没有意义,C不允许因此void \*指针加减运算(本质是因为不知道void \*指向的类型的大小，无法计算)，char \*的指针值计算才有意义，char \*指针加/减1分别代表地址加/减一个char类型大小的地址。

### 3.3

[代码](./exercise/ch3/3/inet_pton_loose.c) 一开始没看懂意思，然后才知道1是成功，0是格式错误，-1是其他错误，才明白这道题让我干什么。。。

---

### CH4 基本TCP套接字编程

### 4.1

看非全0、全1之类的地址，比如 #define INADDR_MAX_LOCAL_GROUP  ((in_addr_t) 0xe00000ff) /\* 224.0.0.255 \*/ 而我的linux正是小端机器，所以可以看出是主机序。

### 4.2

另外如果移动到 connect() 前，那么结果如图  
![IMG](./IMG/4_2.png)  
说明确实是 connect() 建立连接，内核才会给socket绑定了一个ip(ip如果是多宿主机有多个外出接口(网卡，有多个ip可选的情况)，那么具体是哪一个ip地址取决于路由算法的结果)和临时端口

### 4.3

按题意顺序  
1 子进程关闭listenfd  
2 子进程关闭connfd，因为父进程仍然有connfd，所以并未关闭连接
3 父进程关闭connfd，此时才彻底关闭connfd，发出FIN，父进程发起主动关闭

### 4.4

[代码](./exercise/ch4/4/daytimetcp_sv.c) 输出 accept error: Invalid argument，这是因为accept的socket参数不是监听套接字而只是一个主动套接字，导致EINVAL错误

### 4.5

[代码](./exercise/ch4/5/daytimetcp_sv.c) 发现socket生成的一个主动套接字一开始没有分配ip和端口，connect或者listen在发现这种情况(没有bind显式绑定)，内核就会自动分配一个ip和临时端口(linux下listen分配的ip是通配ip，connect是根据路由分配一个ip)

---

### CH5 TCP客户/服务器程序示例

### 5.1

[代码](./exercise/ch5/1) [记录](./exercise/ch5/1/record) 差不多TIME_WAIT持续了61s左右，反推出MSL(最大分组生命期)是30s左右。  

一开始想给shell脚本加个set-user-ID-root，结果发现不行，然后忽然想起来linux下因为安全性问题，不能给shell脚本使用这种获取特权的机制，还是老老实实sudo执行吧。。。  

犯了个弱智错误，那就是不要grep程序名，因为进程结束后，内核占用端口显示TIME_WAIT，但这个时候进程都结束了，当然没有进程名了，通过监听端口可以grep到。

### 5.2

fgets把'\0'作为结尾，而二进制文件里面的值可能有对应'\0'字符，这样会导致发送适以错误的'\0'为结尾，这样可以导致这次发送的没有'\n'，让服务器阻塞在readline，导致死锁。(这里问的是书中的ch5里面那个readline的服务器程序)

### 5.3

Telnet 把输入行后面以 \r\n 结束，而我们的程序以 \n 结束。

### 5.4

### 5.5

### 5.6

### 5.7

### 5.8

### 5.9

### 5.10

### 5.11

---

### CH6 I/O复用：select和poll函数

### 6.1

---

### CH7 套接字选项

### 7.1

---

### CH8 基本UDP套接字编程

### 8.1

---

### CH9 基本SCTP套接字编程

### 9.1

---

### CH10 SCTP客户/服务器程序例子

### 10.1

---

### CH11 名字与地址转换

### 11.1

---
