# Linux c 开发 - Socket - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月23日 13:16:42[initphp](https://me.csdn.net/initphp)阅读数：1802标签：[终端																[linux																[语言																[io																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[Linux c 开发系列](https://blog.csdn.net/initphp/article/category/2609039)

所属专栏：[Linux c 开发系列](https://blog.csdn.net/column/details/mylinuxc.html)](https://so.csdn.net/so/search/s.do?q=io&t=blog)




Socket的英文原义是“孔”或“插座”。作为BSD UNIX的进程通信机制，取后一种意思。通常也称作"套接字"，用于描述IP地址和端口，是一个通信链的句柄。在Internet上的主机一般运行了多个服务软件，同时提供几种服务。每种服务都打开一个Socket，并绑定到一个端口上，不同的端口对应于不同的服务。




### 服务端例子

先看一个支持多线程，多用户连接的阻塞式IO模式的SOCKET例子。为何是**阻塞IO**，可以参考我的文章[http://blog.csdn.net/initphp/article/details/42011845](http://blog.csdn.net/initphp/article/details/42011845)

因为使用了线程，所以例子的编译：



```cpp
gcc main.c -o main -lpthread
```

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h> 
#include <pthread.h>  

//定义一个线程传参结构
typedef struct args {
    int client_socketfd;
} ARGS;

//线程执行的方法
void * recv_msg(void * args) {

    ARGS * args_p = (ARGS * ) args;

    //发送欢迎信息
    char * msg = "Welcome to My socket";
    int size = send(args_p->client_socketfd, msg, strlen(msg), 0);

    //继续等待接收数据
    char buf[1024];  //数据传送的缓冲区  
    int len;
    while ((len = recv(args_p->client_socketfd, buf, 1024, 0)) > 0) {  
        buf[len] = '\0';  
        printf("%s\n", buf);  
        if (send(args_p->client_socketfd, buf, len, 0) < 0) {  
            perror("write");  
        }  

    }

    free(args_p); //释放参数指针对应的内存块数据
    shutdown(args_p->client_socketfd, 2); //关闭客户端的socketid
} 

int server() {
    int server_socketfd; //服务端socket
    struct sockaddr_in server_addr;   //服务器网络地址结构体  
    memset(&server_addr,0,sizeof(server_addr)); //数据初始化--清零  
    server_addr.sin_family = AF_INET; //设置为IP通信  
    server_addr.sin_addr.s_addr = INADDR_ANY;//服务器IP地址--允许连接到所有本地地址上  
    server_addr.sin_port = htons(8000); //服务器端口号  

    //创建服务端套接字
    server_socketfd = socket(PF_INET,SOCK_STREAM,0);
    if (server_socketfd < 0) {
        puts("socket error");
        return 0;
    }

    //绑定IP
    if (bind(server_socketfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))<0) {
        puts("bind error");
        return 0;
    }

    //监听,监听队列长度 5
    listen(server_socketfd, 1);

    //循环等待客户端的连接
    while (1) {
        int client_socketfd;//客户端套接字  
        struct sockaddr_in client_addr; //客户端网络地址结构体 
        int in_size = sizeof(struct sockaddr_in);

        //客户端socket
        client_socketfd = accept(server_socketfd, (struct sockaddr *) &client_addr, &in_size); //等待接受请求，这边是阻塞式的
        if (client_socketfd < 0) {
            puts("accpet error");
            return 0;
        }

        printf("IP :%s", inet_ntoa(client_addr.sin_addr));

        //分配一个内存块，存储线程传参的参数结构，在线程中应当将这个内存块free，防止内存泄露
        ARGS *args = (ARGS *) malloc(sizeof (ARGS *));
        args->client_socketfd = client_socketfd;

        pthread_t t1;
        int x = pthread_create(&t1, NULL , recv_msg, args); //创建一个线程
        if (x != 0) {
            puts("线程创建失败");
        }        
    }

    shutdown(server_socketfd, 2); //关闭socket
}

int main(void) {  
    server(); //运行Server
    return 0;
}
```






### 客户端例子

可以看一个客户端的例子。这个例子主要通过socket连接服务端，并且向服务端发送消息和接收回应



```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h> 
#include <pthread.h>  

int main() {
	
	int client_fd; //定义一个客户端的SOCKET

	struct sockaddr_in server_addr; //服务器端
	memset(&server_addr,0,sizeof(server_addr)); //数据初始化--清零  
    server_addr.sin_family=AF_INET; //设置为IP通信  
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//服务器IP地址  
    server_addr.sin_port = htons(8000); //服务器端口号  

    client_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (client_fd < 1) {
    	puts("client socket error");
    	return 0;
    }

 	/*将套接字绑定到服务器的网络地址上,并且连接服务器端*/  
    int ret = connect(client_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    if (ret < 0) {
    	puts("client connect error!");
    	return 0;
    }

    char buf[1024];
    int len = recv(client_fd, buf, 1024, 0); //等待接收服务器端的数据
    buf[len] = '\0';
    puts(buf);

    char *x = "Hello World";
    send(client_fd, x, strlen(x), 0); //发送数据

	memset(buf, 0, 1024);
    int len2 = recv(client_fd, buf, 1024, 0); //继续接收服务端返回的数据
    buf[len2] = '\0';
    puts(buf);

    shutdown(client_fd,2); //关闭socket

}
```



### 常用接口

#### 1. socket() 函数

定义函数：



```cpp
int socket(int domain, int type, int protocol);
```
socket函数用来建立一个新的socket, 也就是向系统注册, 通知系统建立一通信端口。

参数：

1. domain 指定使用何种的地址类型


|协议|说明|
|----|----|
|PF_UNIX/PF_LOCAL/AF_UNIX/AF_LOCAL|UNIX 进程通信协议|
|PF_INET/AF_INET|Ipv4 网络协议|
|PF_INET6/AF_INET6|Ipv6 网络协议|
|PF_IPX/AF_IPX IPX-Novell|协议|
|PF_NETLINK/AF_NETLINK|核心用户接口装置|
|PF_X25/AF_X25|ITU-T X. 25/ISO-8208 协议|
|PF_AX25/AF_AX25|业余无线AX. 25 协议|
|PF_ATMPVC/AF_ATMPVC|存取原始 ATM PVCs|
|PF_APPLETALK/AF_APPLETALK|appletalk (DDP)协议|
|PF_PACKET/AF_PACKET|初级封包接口|
|PF_INET/AF_INET|Ipv4 网络协议|
|PF_INET/AF_INET|Ipv4 网络协议|




2. type的类别。见表：


|类型|说明|
|----|----|
|SOCK_STREAM|提供双向连续且可信赖的数据流, 即TCP|
|SOCK_DGRAM|使用不连续不可信赖的数据包连接|
|SOCK_SEQPACKET|提供连续可信赖的数据包连接|
|SOCK_RAW|提供原始网络协议存取|
|SOCK_RDM|提供可信赖的数据包连接|
|SOCK_PACKET|提供和网络驱动程序直接通信|




3. protocol 用来指定socket 所使用的传输协议编号，通常为0




返回值：

成功则返回socket处理代码，失败返回-1




错误码：

1、EPROTONOSUPPORT 参数domain 指定的类型不支持参数type 或protocol 指定的协议

2、ENFILE 核心内存不足, 无法建立新的socket 结构

3、EMFILE 进程文件表溢出, 无法再建立新的socket

4、EACCESS 权限不足, 无法建立type 或protocol 指定的协议

5、ENOBUFS/ENOMEM 内存不足

6、EINVAL 参数domain/type/protocol 不合法




#### 2. bind() 函数

定义函数：



```cpp
int bind(int sockfd, struct sockaddr * my_addr, int addrlen);
```
bind()用来设置给参数sockfd 的socket 一个名称. 

成功则返回0, 失败返回-1



错误码：

1、EBADF 参数sockfd 非合法socket 处理代码.

2、EACCESS 权限不足

3、ENOTSOCK 参数sockfd 为一文件描述词, 非socket.

例子：



```cpp
//绑定IP
    if (bind(server_socketfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))<0) {
        puts("bind error");
        return 0;
    }
```


#### 3. listen() 等待连线

定义函数：



```cpp
int listen(int s, int backlog);
```
listen()用来等待参数s 的socket 连线。



listen()只适用SOCK_STREAM 或SOCK_SEQPACKET 的socket 类型. 如果socket 为AF_INET 则参数backlog 最大值可设至128.


参数：

1. 参数s即 socketfd

2. 参数backlog 指定同时能处理的最大连接要求, 如果连接数目达此上限则client 端将收到ECONNREFUSED 的错误. 



返回值：

成功则返回0, 失败返回-1




错误码：

1. EBADF 参数sockfd 非合法socket 处理代码

2. EACCESS 权限不足

3. EOPNOTSUPP 指定的socket 并未支援listen 模式.




例子：



```cpp
//监听,监听队列长度 5
    listen(server_socketfd, 1);
```



#### 4. accept() 接受连接

定义函数：



```cpp
int accept(int s, struct sockaddr * addr, int * addrlen);
```
accept()用来接受参数s 的socket 连线. 参数s 的socket** 必需先经bind()、listen()函数处理过**, 当有连线进来时accept()会返回一个新的socket 处理代码, 往后的数据传送与读取就是经由新的socket处理, 而原来参数s 的socket 能继续使用accept()来接受新的连线要求.



连线成功时, 参数addr 所指的结构会被系统填入远程主机的地址数据, 参数addrlen 为scokaddr 的结构长度




返回值：

成功则返回新的socket 处理代码, 失败返回-1




错误码：

1、EBADF 参数s 非合法socket 处理代码.

2、EFAULT 参数addr 指针指向无法存取的内存空间.

3、ENOTSOCK 参数s 为一文件描述词, 非socket.

4、EOPNOTSUPP 指定的socket 并非SOCK_STREAM.

5、EPERM 防火墙拒绝此连线.

6、ENOBUFS 系统的缓冲内存不足.

7、ENOMEM 核心内存不足.




例子：



```cpp
int client_socketfd;//客户端套接字  
        struct sockaddr_in client_addr; //客户端网络地址结构体 
        int in_size = sizeof(struct sockaddr_in);

        //客户端socket
        client_socketfd = accept(server_socketfd, (struct sockaddr *) &client_addr, &in_size); //等待接受请求，这边是阻塞式的
```



#### 5. connect() 建立socket连接

定义函数：



```cpp
int connect(int sockfd, struct sockaddr * serv_addr, int addrlen);
```



connect()用来将参数sockfd 的socket 连至参数serv_addr 指定的网络地址。**一般用于客户端连接。**




返回值：

成功则返回0, 失败返回-1




错误码：

1、EBADF 参数sockfd 非合法socket 处理代码

2、EFAULT 参数serv_addr 指针指向无法存取的内存空间

3、ENOTSOCK 参数sockfd 为一文件描述词, 非socket.

4、EISCONN 参数sockfd 的socket 已是连线状态

5、ETIMEDOUT 企图连线的操作超过限定时间仍未有响应.

6、ENETUNREACH 无法传送数据包至指定的主机.

7、EAFNOSUPPORT sockaddr 结构的sa_family 不正确.

8、EALREADY socket 为不可阻断且先前的连线操作还未完成.




例子：



```cpp
/*将套接字绑定到服务器的网络地址上,并且连接服务器端*/  
    int ret = connect(client_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    if (ret < 0) {
    	puts("client connect error!");
    	return 0;
    }
```



#### 6. recv() 接收数据

定义函数：



```cpp
int recv(int s, void *buf, int len, unsigned int flags);
```
recv()用来接收远端主机经指定的socket 传来的数据, 并把数据存到由参数buf 指向的内存空间, 参数len 为可接收数据的最大长度。



recv函数一般是阻塞的。但是如果发生意外情况，例如断网，则socket会被关闭，而resv会一直阻塞。这种情况下我们**一般对于阻塞的socket都会用setsockopt来设置socket的超时。**

flags 一般设0。


|类型|说明|
|----|----|
|MSG_OOB|接收以out-of-band 送出的数据.|
|MSG_PEEK|返回来的数据并不会在系统内删除, 如果再调用recv()会返回相同的数据内容.|
|MSG_WAITALL|强迫接收到len 大小的数据后才能返回, 除非有错误或信号产生.|
|MSG_NOSIGNAL|此操作不愿被SIGPIPE 信号中断返回值成功则返回接收到的字符数, 失败返回-1,错误原因存于errno 中.|



错误码：

1. EBADF 参数s 非合法的socket 处理代码

2. EFAULT 参数中有一指针指向无法存取的内存空间

3. ENOTSOCK 参数s 为一文件描述词, 非socket.

4. EINTR 被信号所中断

5. EAGAIN 此动作会令进程阻断, 但参数s 的socket 为不可阻断

6. ENOBUFS 系统的缓冲内存不足.

7. ENOMEM 核心内存不足

8. EINVAL 传给系统调用的参数不正确.




例子：



```cpp
//继续等待接收数据
    char buf[1024];  //数据传送的缓冲区  
    int len;
    while ((len = recv(args_p->client_socketfd, buf, 1024, 0)) > 0) {  
        buf[len] = '\0';  
        printf("%s\n", buf);  
        if (send(args_p->client_socketfd, buf, len, 0) < 0) {  
            perror("write");  
        }  

    }
```




#### 7. send() 传送数据

定义函数：



```cpp
int send(int s, const void * msg, int len, unsigned int falgs);
```


send()用来将数据由指定的socket 传给对方主机. 参数s 为已建立好连接的socket. 参数msg 指向欲连线的数据内容, 参数len 则为数据长度. 参数flags 一般设0。




返回值：

成功则返回实际传送出去的字符数, 失败返回-1




错误码：

1. EBADF 参数s 非合法的socket 处理代码.

2. EFAULT 参数中有一指针指向无法存取的内存空间

3. ENOTSOCK 参数s 为一文件描述词, 非socket.

4. EINTR 被信号所中断.

5. EAGAIN 此操作会令进程阻断, 但参数s 的socket 为不可阻断.

6. ENOBUFS 系统的缓冲内存不足

7. ENOMEM 核心内存不足

8. EINVAL 传给系统调用的参数不正确.




例子：



```cpp
//发送欢迎信息
    char * msg = "Welcome to My socket";
    int size = send(args_p->client_socketfd, msg, strlen(msg), 0);
```


#### 8. shutdown() 关闭socket

定义函数：



```cpp
int shutdown(int s, int how);
```
shutdown()用来终止参数s 所指定的socket 连线。

how参数：

how=0 终止读取操作.

how=1 终止传送操作

how=2 终止读取及传送操作




错误码：

1、EBADF 参数s 不是有效的socket 处理代码

2、ENOTSOCK 参数s 为一文件描述词, 非socket

3、ENOTCONN 参数s 指定的socket 并未连线




例子：



```cpp
shutdown(server_socketfd, 2); //关闭socket
```](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=终端&t=blog)




