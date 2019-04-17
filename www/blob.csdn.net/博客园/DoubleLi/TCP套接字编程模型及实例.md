# TCP套接字编程模型及实例 - DoubleLi - 博客园






**摘要：**

本文讲述了TCP套接字编程模块，包括服务器端的创建套接字、绑定、监听、接受、读/写、终止连接，客户端的创建套接字、连接、读/写、终止连接。先给出实例，进而结合代码分析。



PS：本文权当复习套接字编程的读书笔记。



**一、TCP套接字编程模型**

同一台计算机上运行的进程可以利用管道、消息队列、信号量、共享内存等进行相互通信，不同计算机上运行的进程可以通过**套接字网络IPC接口**进行相互通信。套接字编程基本步骤如下图所示：

![](http://blog.chinaunix.net/attachment/201205/7/9112803_1336386029FXhR.png)

图 TCP套接字编程模型[1]

**二、源代码**

本实例旨在实现简单的echo服务，客户端发送数据给服务端，在服务端打印出来并且回发给客户端，并在客户端显示。

TCP_socket_programming_example源文件![](http://blog.chinaunix.net/blog/image/attachicons/rar.gif) TCP_socket_programming_example.rar 

**2.1 TCP服务端**



- **//filename:TCPserver.c**
- **#include <stdio.h>**
- **#include <errno.h>**
- **#include <sys/socket.h>**
- **#include <netinet/in.h>**
- 
- **#define BACKLOG 10**
- **#define BUFFER_SIZE 1024**
- 
- **int main(int argc, char *argv[])**
- **{**
- **  if(2 != argc)**
- **  {**
- **    printf("Usage:%s portnumber\n", argv[0]);**
- **    return - 1;**
- **  }**
- 
- **  /***1.create a socket***/**
- **  int fd_server = socket(AF_INET, SOCK_STREAM, 0); //TCP**
- **  if( - 1 == fd_server)**
- **  {**
- **    printf("%s\n", strerror(errno));**
- **    return - 1;**
- **  }**
- 
- **  /***2.bind the socket***/**
- **  int listen_port = atoi(argv[1]);**
- **  struct sockaddr_in addr_server;**
- **  //memset(&addr_server, 0, sizeof(addr_server));**
- **  addr_server.sin_family = AF_INET;**
- **  addr_server.sin_port = htons(listen_port);**
- **  addr_server.sin_addr.s_addr = htonl(INADDR_ANY);**
- 
- **  if(bind(fd_server, (struct sockaddr*) &addr_server, sizeof(addr_server)) == - 1)**
- **  {**
- **    printf("%s\n", strerror(errno));**
- **    return - 1;**
- **  }**
- 
- **  /***3.listen the socket***/**
- **  if(listen(fd_server, BACKLOG) == - 1)**
- **  {**
- **    printf("%s\n", strerror(errno));**
- **    return - 1;**
- **  }**
- 
- **  /***4.accept the requirement of some client***/**
- **  struct sockaddr_in addr_client;**
- **  int len_addr_client = sizeof(addr_client);**
- **  int fd_client = accept(fd_server, (struct sockaddr*) &addr_client, &len_addr_client);**
- **  if( - 1 == fd_client)**
- **  {**
- **    printf("%s\n", strerror(errno));**
- **    return - 1;**
- **  }**
- 
- **  /****5.serve the client******/**
- **  char buf[BUFFER_SIZE];**
- **  int size;**
- **  while(1)**
- **  {**
- **    /***read from client***/**
- **    size = recv(fd_client, buf, sizeof(buf), 0);**
- **    buf[size] = '\0';**
- **    printf("%s\n", buf);**
- 
- **    /***write to client***/**
- **    size = send(fd_client, buf, strlen(buf), 0);**
- **  }**
- 
- **  /****6.close the socket******/**
- **  close(fd_server);**
- **  close(fd_client);**
- **}**






**2.2 TCP客户端**





**点击(此处)折叠或打开**


- **//filename:TCPclient.c**
- **#include <stdio.h>**
- **#include <errno.h>**
- **#include <sys/socket.h>**
- **#include <netinet/in.h>**
- 
- **#define BUFFER_SIZE 1024**
- 
- **int main(int argc, char *argv[])**
- **{**
- **  if(3 != argc)**
- **  {**
- **    printf("Usage:%s hostname portnumber\n", argv[0]);**
- **    return - 1;**
- **  }**
- 
- **  /***1.create a socket***/**
- **  int fd_client = socket(AF_INET, SOCK_STREAM, 0); //TCP**
- **  if( - 1 == fd_client)**
- **  {**
- **    printf("%s\n", strerror(errno));**
- **    return - 1;**
- **  }**
- **  /***2.connect to the server***/**
- **  int portnumber = atoi(argv[2]);**
- **  struct sockaddr_in addr_server;**
- **  addr_server.sin_family = AF_INET;**
- **  addr_server.sin_port = htons(portnumber);**
- **  if(0 == inet_pton(AF_INET, argv[1], (void*) &addr_server.sin_addr.s_addr))**
- **  {**
- **    printf("Invalid address.\n");**
- **    return - 1;**
- **  }**
- 
- **  if(connect(fd_client, (struct sockaddr*) &addr_server, sizeof(addr_server)) == - 1)**
- **  {**
- **    printf("%s\n", strerror(errno));**
- **    return - 1;**
- **  }**
- 
- 
- **  /****3.get the server******/**
- **  char buf[BUFFER_SIZE];**
- **  int size;**
- **  while(1)**
- **  {**
- **    /***write to server***/**
- **    scanf("%s", buf);**
- **    size = send(fd_client, buf, strlen(buf), 0);**
- 
- **    /***read from server***/**
- **    size = recv(fd_client, buf, BUFFER_SIZE, 0);**
- 
- **    buf[size] = '\0';**
- **    printf("%s\n", buf);**
- 
- **  }**
- **  /****4.close the socket******/**
- **  close(fd_client);**
- **}**





**2.3 测试结果**

$ ./TCPserver 2000

![](http://blog.chinaunix.net/attachment/201205/7/9112803_1336386029088h.png)

$ ./TCPclient 127.0.0.1 2000

![](http://blog.chinaunix.net/attachment/201205/7/9112803_1336386030hkN0.png)

**三、源码分析**

**3.1 创建套接字**



- int socket(int domain, int type, int protocol);//成功返回套接字描述符.出错返回-1




这一步事实上是确定**通信特征**，各个域domain有自己的格式表示地址，以AF_开头(address family)；type确定套接字类型，如数据报、字节流；协议protocol对同一个域和套接字类型支持的多个协议进行选择，通常为0，即按给定的域和套接字类型选择默认的协议。典型的TCP、UDP如下：



- TCP：(AF_INET, SOCK_DGRAM, 0)
- 
- UDP：(AF_INET, SOCK_STREAM, 0)




注：

尽管套接字本质是文件描述符，但不是所有用于文件操作的函数都能用于套接字操作，比如lseek，套接字不支持文件偏移量。

**3.2 绑定**



- int bind(int sockfd, const struct sockaddr *addr, socklen_t len);//成功返回0.出错返回-1




bind函数用于将地址绑定到一个套接字。**服务器需要给一个接收客户端请求套接字绑定一个众所周知的地址，**而客户端可以让系统选一个默认地址绑定(无须绑定)。

**(1) 套接字地址sockaddr_in**

在IPv4因特网域AF_INET中，套接字地址用结构sockaddr_in表示，如下：



- struct sockaddr_in
- {
-   sa_family_t sin_family; //unsigned short 地址族
-   in_port_t sin_sport; //uint16_t
-   struct in_addr sin_addr; //IPv4
- };
- 
- struct in_addr
- {
-   in_addr_t s_addr; //uint32_t
- };




注：

初始化sockaddr_in结构体时，因为sin_port和sin_addr被封装在网络传输，所以端口号和地址必须用网络字节序；而sin_family只是被内核用来决定数据结构包含什么类型的地址，没有发送到到网络，应该是本机字节顺序。处理器与网络字节序之间转换函数为htonl、htons、ntohl、ntohs(h指host主机，n指network网络，l指long32位，s指short16位)。

理论上，端口号可以是0~65535，但1~1023已由IANA管理，绑定时端口号不少于1024[2]。

此处的地址s_addr是二进制地址格式，如果参数是点分十进制字符串表示，则需通过函数inet_ntop(将网络字节序的二进制地址转换成点分十进制字符串表示)、inet_pton进行相互转换。其转换过程如下：



- 127.0.0.1 --> 7F.0.0.1 --> 100007F=16777343(网络字节序为大端)




如果地址s_addr为ANADDR_ANY，套接字端点可以被绑定到所有系统网络接口，即可以收到这个系统所安装的所有网卡的数据包。

**(2) 通用地址格式sockaddr**

地址格式与特定的通信域有关(如AF_INET、AF_INET6)，为使不同地址格式地址能够传入套接字函数，地址被强制转换成通用的地址结构sockaddr，如下(以Linux为例)：



- struct sockaddr
- {
-   unsigned short sa_family; /* address family, AF_xxx */
-   char sa_data[14]; /* 14 bytes of protocol address */
- };




**3.3 监听listen**



- int listen(int sockfd, int backlog);//成功返回0，出错返回-1




一旦服务器调用listen，套接字就能接收连接请求。backlog用于表示该进程所要入队的连接请求数量，实际值由系统决定，但上限由SOMAXCONN指定。一旦队列满，系统会拒绝多余连接请求。

**3.4 接受连接请求accept**



- //成功返回套接字描述符，出错返回-1
- int accept(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict len);




使用accept获得连接请求并建立连接，**新的套接字描述符连接到调用connect的客户端**。传给accept的原始套接字(sockfd)没有关联到这个连接，而是接收保持可用状态并接受其他请求连接，这样做是为了使新的套接字描述符和原始套接字具有相同的地址族domain和套接字类型type。

如果服务器调用accept并且当前没有连接请求，服务器会阻塞直到一个请求到来。如果不关心客户端标识，可以将参数addr和len设为NULL。

注：

关键字restrict是C99新引入的，所有修改该指针所指向内容的操作全部都是基于(base on)该指针的，即不存在其它进行修改操作的途径；从而帮助编译器进行更好的代码优化，生成更有效率的汇编代码[4]。

**3.5 建立连接connect**



- **//成功返回0，出错返回-1**
- **int connect(int sockfd, const struct sockaddr *addr, socklen_t len);**




addr是想与之通信的服务器地址，如果sockfd没有绑定到一个地址，connect会给调用者绑定一个默认的地址。成功连接需要以下条件：要连接的机器开启且正在运行，服务器绑定到一个想与之连接的地址，服务器的等待连接队列有足够的空间。

**3.6 读取数据**



- ssize_t read(int fd, void *buf, size_t nbytes); //成功返回读到的字节数，已到文件末尾返回0，出错返回-1
- 
- ssize_t recv(int sockfd, const void *buf, size_t nbytes, int flags); //成功返回字节计数的消息长度，无可用消息或对方已经按序结束返回0，出错返回-1
- 
- ssize_t recvfrom(int sockfd,void *restrict buf, size_t len, int flags, struct sockaddr *restrict addr, socketlen_t *restruct addrlen); //成功返回字节计数的消息长度，无可用消息或对方已经按序结束返回0，出错返回-1
- 
- ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);//成功返回字节计数的消息长度，无可用消息或对方已经按序结束返回0，出错返回-1




可以使用read通过套接字通信，但read只能交换数据，若想指定选项、从多个客户端接收数据包，则需选择套接字函数recv(指定标志控制接收数据的方式)、recvfrom(得到数据发送者的源地址)、resvmsg(将接收到数据送入多个缓冲区或接收辅助数据)。

![](http://blog.chinaunix.net/attachment/201205/7/9112803_1336386031sjJy.png)

**3.7 写入数据**



- ssize_t write(int fd, void *buf, size_t count); //成功返回写入字节数，出错返回-1
- 
- ssize_t send(int sockfd, const void *buf, size_t nbytes, int flags); //成功返回发送的字节数，出错返回-1
- 
- ssize_t sendto(int sockfd, const void *buf, size_t nbytes, int flags, 
-               const struct sockaddr *destaddr, socklen_t destlen); //成功返回发送的字节数，出错返回-1
- 
- ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags); //成功返回发送的字节数，出错返回-1




注：send与sendto的flags含义相同，sendmsg的flags与前两者不同

可以使用write通过套接字通信，但write只能交换数据，若想指定选项、发送带外数据，则需选择套接字函数send(指定标志改变处理传输数据的方式)、sendto(允许无连接的套接字上指定一个目标地址)、sendmsg(指定多重缓冲传输数据)。

![](http://blog.chinaunix.net/attachment/201205/7/9112803_1336386031hVIz.png)

**3.8 终止连接**



- int close(int fd); //成功返回0，出错返回-1
- 
- int shutdown(int sockfd, int how);//成功返回0，出错返回-1




关闭套接字close只有在最后一个活动引用被关闭后才释放网络端点，而shutdown提供更精细的控制，套接字通信是双向的，可以用shutdown禁止套接字上的输入/输出，即how为SHUT_RD、SHUT_WR、SHUT_RDWR。除此之外，shutdown允许使一个套接字处于不活动状态(不管引用它的文件描述符数目多少)，便于复制一个套接字(如dup)。



**参考资料：**

[1] 百度文库《[互联网络程序设计第3章](http://wenku.baidu.com/view/400240ef998fcc22bcd10d20.html)》

[2] 《UNIX环境高级编程》[M].

[3] 维基百科词条：[TCP/UDP端口列表](http://zh.wikipedia.org/wiki/TCP/UDP%E7%AB%AF%E5%8F%A3%E5%88%97%E8%A1%A8)

[4] 博文《[C99中的restrict关键字](http://blog.csdn.net/lovekatherine/article/details/1891806)》

套接字调用标志源文件![](http://blog.chinaunix.net/blog/image/attachicons/msoffice.gif) 套接字调用标志.xls 

from:http://blog.chinaunix.net/uid-9112803-id-3199813.html









