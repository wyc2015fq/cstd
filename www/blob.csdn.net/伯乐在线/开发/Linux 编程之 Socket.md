# Linux 编程之 Socket - 文章 - 伯乐在线
原文出处： [hxf0223](http://blog.chinaunix.net/uid-670184-id-5754181.html)
### TCP/IP协议及socket封装
![](http://jbcdn2.b0.upaiyun.com/2016/11/1d3dcd17d9a242ff78cf14e0027b64ec.png)socket编程的基本流程
![](http://jbcdn2.b0.upaiyun.com/2016/11/4175c93766ba07baf7e241c98303bdc2.png)socket连接的建立（3次握手）
![](http://jbcdn2.b0.upaiyun.com/2016/11/0526d00ddf97a819273dbe15ce2d54bf.png)
socket连接的断开（3次握手）
![](http://jbcdn2.b0.upaiyun.com/2016/11/6c0cf3edc0608329bc9b7b57b1e527c2.jpg)
由于TCP连接是全双工的，因此每个方向都必须单独进行关闭。这个原则是当一方完成它的数据发送任务后就能发送一个FIN来终止这个方向的连接。收到一个 FIN只意味着这一方向上没有数据流动，一个TCP连接在收到一个FIN后仍能发送数据。首先进行关闭的一方将执行主动关闭，而另一方执行被动关闭。
（1）客户端A发送一个FIN，用来关闭客户A到服务器B的数据传送（报文段4）。
（2）服务器B收到这个FIN，它发回一个ACK，确认序号为收到的序号加1（报文段5）。和SYN一样，一个FIN将占用一个序号。
（3）服务器B关闭与客户端A的连接，发送一个FIN给客户端A（报文段6）。
（4）客户端A发回ACK报文确认，并将确认序号设置为收到序号加1（报文段7）。
对应函数接口如图：
![http://files.jb51.net/file_images/article/201508/201585154536177.png?201575154548](http://jbcdn2.b0.upaiyun.com/2016/11/ff411e69e1a019788788dcb003a4e982.png)
**socket编程之bind函数**


```
int bind(int sockfd, const struct sockaddr *addr,socklen_t *addrlen);
```
**功能描述：**
当用socket()函数创建套接字以后，套接字在名称空间(网络地址族)中存在，但没有任何地址给它赋值。bind()把用**addr**指定的地址赋值给用文件描述符代表的套接字**sockfd**。**addrlen**指定了以**addr**所指向的地址结构体的字节长度。一般来说，该操作称为“给套接字命名”。
通常，在一个**SOCK_STREAM**套接字接收连接之前，必须通过bind()函数用**本地地址**为套接字命名。
**备注：**
调用bind()函数之后，为socket()函数创建的套接字关联一个相应地址，发送到这个地址的数据可以通过该套接字读取与使用。
**备注：**
bind()函数并不是总是需要调用的，只有用户进程想与一个具体的地址或端口相关联的时候才需要调用这个函数。如果用户进程没有这个需要，那么程序可以依赖内核的自动的选址机制来完成自动地址选择，而不需要调用bind()函数，同时也避免不必要的复杂度。在一般情况下，对于服务器进程问题需要调用bind()函数，对于客户进程则不需要调用bind()函数。
**socket编程之accept函数**


```
int accept(int sockfd,struct sockaddr *addr,socklen_t *addrlen);
```
#### 功能参数描述
accept()系统调用主要用在基于连接的套接字类型，比如SOCK_STREAM和SOCK_SEQPACKET。它**提取出所监听套接字的等待连接队列中第一个连接请求**，**创建一个新的套接字，并返回指向该套接字的文件描述符**。新建立的套接字不在监听状态，原来所监听的套接字也不受该系统调用的影响。
**备注：**
新建立的套接字准备发送send()和接收数据recv()。
**参数：**
***sockfd***,    利用系统调用socket()建立的套接字描述符，通过bind()绑定到一个本地地址(一般为服务器的套接字)，并且**通过listen()一直在监听连接**；
服务器代码：

```
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#define QUEUE        20
#define MYPORT        8887
#define BUFF_SIZE    1024
int main(int argc, char *argv[])
{
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);    // 定义socket fd
    // 定义sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(MYPORT);            // host to network short
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);// host to network long
    // bind socket address to socket fd
    if (bind(server_sockfd, (struct sockaddr*)&server_sockaddr, sizeof(server_sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }
    if (listen(server_sockfd, QUEUE) == -1) {
        perror("listen");
        exit(2);
    }
    // 客户端套接字
    char buff[BUFF_SIZE];
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    // 监听并返回客户端fd
    int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    if (conn < 0) {
        perror("accept");
        exit(3);
    }
    while (1) {
        memset(buff, 0, sizeof(buff));
        int len = recv(conn, buff, sizeof(buff), 0);
        if (strcmp(buff, "exit\n") == 0)
            break;
        printf("server received:%s", buff);
        send(conn, buff, len, 0);
    }
    close(conn);
    close(server_sockfd);
    return 0;
}
```
客户端代码：

```
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#define MYPORT 8887
#define BUFF_SIZE 1024
int main()
{
    int sock_client_fd = socket(AF_INET, SOCK_STREAM, 0);
    // 定义server socket
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(0));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(MYPORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");    // 服务器ip
    // 连接服务器
    if (connect(sock_client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(1);
    }
    char sendbuff[BUFF_SIZE], recvbuff[BUFF_SIZE];
    while (fgets(sendbuff, sizeof(sendbuff), stdin) != NULL){
        send(sock_client_fd, sendbuff, strlen(sendbuff), 0);        // 发送
        if (strcmp(sendbuff, "exit\n") == 0) break;
        recv(sock_client_fd, recvbuff, sizeof(recvbuff), 0);
        printf("client received:%s", recvbuff);
        memset(recvbuff, 0, sizeof(recvbuff));
        memset(sendbuff, 0, sizeof(sendbuff));
    }
    close(sock_client_fd);
    return 0;
}
```
