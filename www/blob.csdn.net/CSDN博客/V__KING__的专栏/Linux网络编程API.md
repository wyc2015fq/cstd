# Linux网络编程API - V__KING__的专栏 - CSDN博客





2015年11月23日 11:23:02[v__king__](https://me.csdn.net/V__KING__)阅读数：564








# 网络编程API

1：socket（）创建套接字 

2：bind() 绑定本机地址和端口 

3：connect（）建立连接 

4：listen（）设置监听套接字 

5：accept（）接收TCP连接 

6：recv()，read(), recvfrom() 数据接收 

7：send(), write(), sendto() 数据发送 

8：close（），shutdown（）关闭套接字
## 具体
- 1：socket（）创建套接字

```
#include <sys/types.h>
#include <sys/socket.h>
int socket(int domain, int type, int protocol);

//a:  domain  是协议族
//PF_INET //internet协议
//PF_UNIX//unix internal协议
//PF_IMPLINK//interface message协议

//b: type//套接字类型
//SOCK_STREAM //流式套接字
//SOCK_DGRAM //数据报套接字
//SOCK_RAW    //原始套接字

//c: protoco 通常设置为0
```

```
int bind(int sockfd,struct sockaddr *my_addr,int addrlen);  
//a: sockfd  socket调用返回的文件描述符 
//b: addrlen   sockaddr地址结构的长度
//返回值：0或者-1
```

```
int listen（int sockfd,int backlog);
//a:  sockfd   同上
//b:  backlog  指定了正在等待连接的最大队列长度；
```

```
int accept（int sockfd, struct sockaddr *addr, socklen_t *addrlen）;
//a:  返回值   已经建立好连接的套接字或者-1
//b: sockfd   监听套接字
//c: addr   对方地址
//d:  addrlen  传递结构体addr的长度，并返回对方地址的长度；
```

```
int connect(int sockfd, struct sockaddr *serv_addr,int addrlen);
//a:  返回值0或者-1
//b:  ser_addr   服务器端的地址信息
//c: addrlen  serv_addr的长度
//注意：connect（）是客户端使用的系统调用
```

```
ssize_t send(int sockfd, const void *buffer,size_t length,int flags);
//a: 成功的话返回实际发送的字节数
//b: 失败返回-1，并设置errno;
//c: buffer  发送缓冲区首地址；
//d: length 发送的字节数
//e: flags 发送的方式
```

```
ssize_t  recv(int sockfd, const void *buffer, size_t  length,int flags);

//a: 成功返回实际接收的字节数，失败返回-1，并设置errno

//b: buffer 发送缓冲区首地址

//c: length  发送的字节数

//d: flags  发送的方式

//sendto ，recvfrom这两个函数一般在使用udp协议的时候使用
```
`ssize_t sendto （int socket ,void *message ,size_t length, int flags, struct sockaddr * dest_addr,socklen_t dest_len);``ssize_t recvfrom(int sockfd,void *buffer,size_t length,int flags,struct sockaddr *address, socklen_t *address _len)``int close（int sockfd）；`
```
//关闭双向通讯

int shutdown（int sockfd，int howto);

//a:howto = 0
//关闭读通道，但是可以继续往套接字里面写数据

//b:howto = 1
//和上面相反，关闭写通道，只能从套接字里读取数据

//c:howto = 2
关闭读写通道，和close（）一样
```

## 地址相关的数据结构：

1：通用地址结构

```
struct　sockaddr
{
    u_short sa_family;
    char sa_data[14];
}
```

2:internet协议地址结构

```
struct sockaddr_in
{
    u_short sin_family;
    u_short sin_port;
    struct in_addr sin_addr;
    char sin_zero[8];
}
```




