# TCP套接字编程（一） - yixianfeng41的专栏 - CSDN博客
2017年02月14日 17:43:44[第2梦](https://me.csdn.net/yixianfeng41)阅读数：313
所属专栏：[Linux/Unix 网络编程](https://blog.csdn.net/column/details/14513.html)
[篇一：TCP套接字编程（一）](http://blog.csdn.net/yixianfeng41/article/details/55102751)
[篇二：TCP套接字编程（二）](http://blog.csdn.net/yixianfeng41/article/details/55194329)
### **一、Tcp 客户端服务器通信模型**
首先要清楚socket通信整个过程是怎样的，首先肯定都要创建一个socket, 然后服务器收拾收拾，准备接客（listen,bind accept）.诶，有客户端来大保健了（accept）。来吧，谈谈价钱啥的（三次握手），好了，达成一致，开始工作。 
![这里写图片描述](https://img-blog.csdn.net/20170214174209262?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWl4aWFuZmVuZzQx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
下面首先总结一下几个重要的函数：
#### 1、  socket()函数
`int socket(int domain, int type, int protocol);  `
这个操作类似于打开文件操作，返回socket的socket描述符。
参数：
domain:协议域，又称为协议族(family)。常用的协议族有AF_INET、AF_INET6、AF_LOCAL、AF_ROUTE。协议族决定了socket的地址类型，通信时采用与其相符的地址，AF_INET用ipv4地址(32位)和16位端口号的组合
type：指定socket类型，常用的有SOCK_STREAM、SOCK_DGRAM、SOCK_RAW、SOCK_PACKET、SOCK_SEQPACKET，前两个分别对应TCP和UDP类型的socket
protocol：指定协议，常用有IPPROTO_TCP、IPPTOTO_UDP、IPPROTO_SCTP、IPPROTO_TIPC，协议和socket的类型要匹配。0会选择type对应的默认类型。
#### 2、 bind()函数
`int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);  `
把一个地址族的特定地址指定给socket，而不是由系统随机分配.
参数：
sockfd:socket描述符，socket()函数返回的int值
addr:一个地址结构体的const指针，指向要绑定给sockfd的地址，结构体的结构和地址协议相符。
如ipv4的地对应的
```
struct sockaddr_in {  
   sa_family_t    sin_family; /*地址族: AF_INET */  
   in_port_t      sin_port;   /*网络字节序的端口号 */  
   struct in_addr sin_addr;   /*internet 地址 */  
};  
 /* Internet 地址. */  
struct in_addr {  
   uint32_t       s_addr;     /* 网络字节序的地址 */  
};
```
应该注意使用htol,htos函数将主机字节顺序转换为网络字节顺序，避免潜在的错误。
#### 3、  listen()、connect()函数
使用时依次调用socket(),connect()，然后调用listen()来监听socket，客户端调用connect是，服务器就会收到这个请求。
`int listen(int sockfd, int backlog); `
sockfd是要监听的socket的描述符 
backlog是这个socket可以排队连接的最大链接个数，也就是这个socket的等待队列的长度。调用listen，socket开始等待客户的链接请求
`int  connect(int sockfd, const structsockaddr *addr, socklen_t addrlen);  `
sockfd 是客户端socket描述字
addr为服务器的socket地址
addr_len是socket地址的长度。
客户端通过调用connect函数来建立与TCP服务器的连接。调用listen()，socket开始等待客户的链接请求
#### 4、accept()函数
服务器端第四个要调用的函数，服务器收到请求后，用accept接受请求，然后链接就建立了，可以开始读写操作。
`int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);  `
sockfd是服务器socket描述字
addr是指针，用于返回客户端地址
addrlen是协议地址的长度。函数的返回值是内核自动生成的一个全新的描述字，代表一个和客户端的TCP链接
#### 5、read(),write()读写操作
相关函数原型如
```
#include <unistd.h>  
ssize_t read(int fd, void *buf, size_t count);  
ssize_t write(int fd, const void *buf, size_t count);  
#include <sys/types.h>  
#include <sys/socket.h>  
ssize_t send(int sockfd, const void *buf, size_t len, int flags);  
ssize_t recv(int sockfd, void *buf, size_t len, int flags);  
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,  
                      const struct sockaddr*dest_addr, socklen_t addrlen);  
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,  
                        struct sockaddr*src_addr, socklen_t *addrlen);  
ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);  
ssize_t recvmsg(int sockfd, struct msghdr*msg, int flags);
```
close()函数
读写完毕后要关闭相应的socket描述字
```
#include <unistd.h>  
int close(int fd);
```
上面的函数总结摘录于博客：[Socket通信——Linux下，使用C/C++](http://blog.csdn.net/giantpoplar/article/details/47657303)
下面将编写代码实现客户端，服务器通信。凡事从最简单的开始，那我们首先就服务器处理一个连接，然后处理多个连接，客户端显示用telnet，然后自己写。服务器也是由单个到并发，试着用pthread,fork，select,epoll，由简单到复杂逐步实现，本文不实现select和epoll，留着下次实现。
### **二、Server 最简单版：**
从最简单的开始，不写客户端，只写一个简单服务端程序，然后用telenet连接。
服务端socket编程步骤如下： 
（1）创建一个socket 
（2）绑定到ip地址和端口 
（3）开始监听 
（4）接受连接，然后处理后续通信。
按照上述步骤，写出的代码如下：
```cpp
/*
    C socket server example
*/
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
    //Listen
    listen(socket_desc , 3);
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");
    //Receive a message from client
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
        //Send the message back to client
        write(client_sock , client_message , strlen(client_message));
    }
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
    return 0;
}
```
上述代码创建了一个服务器,ip是本地地址（127.0.0.1）端口 8888，它从客户端收到消息后又会将这条消息返回给客户端。
测试： 
$telenet localhost 8888
### **三、Server 多线程版：**
刚服务器Server太简单了，只能处理一个连接，现在让它可以处理多个连接，用线程处理多个连接通信。
```cpp
/*
    C socket server example, handles multiple clients using threads
*/
#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
//the thread function
void *connection_handler(void *);
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
    //Listen
    listen(socket_desc , 3);
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    return 0;
}
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];
    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock , message , strlen(message));
    message = "Now type something and i shall repeat what you type \n";
    write(sock , message , strlen(message));
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        //Send the message back to client
        write(sock , client_message , strlen(client_message));
    }
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
    //Free the socket pointer
    free(socket_desc);
    return 0;
}
```
### **四、Server 多进程版:**
上面是多线程，用多进程试试.
```cpp
/*
    C socket server example, handles multiple clients using threads
*/
#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
//the thread function
void *connection_handler(void *);
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
    pid_t pid;
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
    //Listen
    listen(socket_desc , 3);
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
        pid = fork();    //创建子进程
        if(pid == -1)
            printf("fork 1 failed:");
        else if(pid==0)
        {
            close(socket_desc);  //关闭listen socket
            int read_size;
            char *message , client_message[2000];
            //Receive a message from client
            while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
            {
                //Send the message back to client
                write(client_sock , client_message , strlen(client_message));
            }
            if(read_size == 0)
            {
                puts("Client disconnected");
                fflush(stdout);
            }
            else if(read_size == -1)
            {
                perror("recv failed");
            }
        }
        else if(pid>0)
        {
            //父进程中关闭client_sockfd
            close(client_sock);
        }
    }
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }  
    return 0;
}
```
### **五、Client：**
不用telent了，自己写一个简单的客户端：
```cpp
/*
    client example using sockets
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    puts("Connected\n");
    //keep communicating with server
    while(1)
    {
        printf("Enter message : ");
        scanf("%s" , message);
        //Send some data
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        //Receive a reply from the server
        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
        puts("Server reply :");
        puts(server_reply);
    }
    close(sock);
    return 0;
}
```
上述代码将连接到localhost,port=8888.
所有代码，均是实验代码，可跑通，不排除有其他问题，仅供学习！
