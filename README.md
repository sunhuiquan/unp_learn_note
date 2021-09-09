# unp_learn_note

我学习UNPv13e(《UNIX网络编程 卷一：套接字联网API》第三版)做到习题及部分笔记。

## Index

- [CH1](#CH1-简介)

### CH0 Source Code

[UNP repo](https://github.com/unpbook/unpv13e)  
[UNP 书上源码](./unpv13e)  
[UNP README](./unpv13e/README)

### Note CH0

如果运行 ./daytimetcpcli 127.0.0.1 显示refused，这是因为inetd默认没开daytime服务，你修改 /etc/inetd.conf, 然后 service inetd restart 重启服务即可，另外linux ubuntu 可能没安inetd，需要 apt install 一下。

---

### CH1 简介

### 1.1

---
