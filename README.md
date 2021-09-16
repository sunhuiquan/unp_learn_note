# unp_learn_note

我学习UNPv13e(《UNIX网络编程 卷一：套接字联网API》第三版)做到习题及部分笔记。

## Index

- [预备工作](#CH0-预备工作)
- [CH1 简介](#CH1-简介)
- [CH2 传输层：TCP、UDP、SCTP](#CH2-传输层TCPUDPSCTP)
- [CH3 套接字编程简介](#CH3-套接字编程简介)
- [CH4 基本TCP套接字编程](#CH4-基本TCP套接字编程)
- [CH5 TCP客户/服务器程序示例](#CH5-TCP客户/服务器程序示例)
- [CH6 I/O复用：select和poll函数](#CH6-I/O复用select和poll函数)
- [CH7 套接字选项](#CH7-套接字选项)
- [CH8 基本UDP套接字编程](#CH8-基本UDP套接字编程)
- [CH9 基本SCTP套接字编程](#CH9-基本SCTP套接字编程)
- [CH10 SCTP客户/服务器程序例子](#CH10-SCTP客户/服务器程序例子)
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

### 4.3

### 4.4

### 4.5

---

### CH5 TCP客户/服务器程序示例

### 5.1

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
