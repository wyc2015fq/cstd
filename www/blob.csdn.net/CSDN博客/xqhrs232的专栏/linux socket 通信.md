# linux socket 通信 - xqhrs232的专栏 - CSDN博客
2011年06月01日 17:24:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：561标签：[socket																[linux																[unix																[domain																[struct																[服务器](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=domain&t=blog)](https://so.csdn.net/so/search/s.do?q=unix&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://hi.baidu.com/zhenwl00/blog/item/d5b4a2dd3d50b2eb76c638a3.html](http://hi.baidu.com/zhenwl00/blog/item/d5b4a2dd3d50b2eb76c638a3.html)
第10章介绍了如何使用管道和命名管道进行进程间通信。管道由于只能实现具有亲缘进程的进程间通信，使用受到了很大的限制，命名管道解决了这一问题。但是，无论是管道还是命名管道，都只能实现单向通信（在只创建一个管道的情况下）。
使用套接字除了可以实现网络间不同主机间的通信外，还可以实现同一主机的不同进程间的通信，且建立的通信是双向的通信。这里所指的使用套接字实现进程间通信，是由将通信域指定为PF_UNIX来实现的。表13.1为socket函数的具体定义信息，该函数的形式如下：
int socket(int domain, int type, int protocol);
socket函数中的domain参数用于指定通信域，表13.2为domain支持的通信域的参数。domain参数取PF_UNIX时，表示创建UNIX域的套接字。使用PF＿UNIX域的套接字可以实现同一机器上的不同进程间的通信。
调用bind函数实现了套接字与地址（这里是文件名）的绑定。bind函数的具体信息如下：
int bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen);
其参数my_addr为指向结构体sockaddr_un的指针，该结构体的定义如下：
    #define UNIX_PATH_MAX 108
    struct sockaddr_un {
    sa_family_t     sun_family;     /*PF_UNIX或AF_UNIX */
    char    sun_path[UNIX_PATH_MAX];        /* 路径名 */
    };
在该结构体中，sun_family为AF_UNIX。sun_path是套接字在文件系统中的路径名。
### 13.3.1 服务器端实现
程序p13.2.c为使用套接字在UNIX域内实现 进程间通信的服务端程序。图13.3为服务器流程图。首先，程序通过调用socket函数，建立了监听连接的套接字，然后调用bind函数，将套接字与地 址信息关联起来。调用listen函数实现对该端口的监听，当有连接请求时，通过调用accept函数建立与客户机的连接，最后，调用read函数来读取 客户机发送过来的消息，当然也可以使用recv函数实现相同的功能。p13.2.c的具体代码如下：
//p13.2.c UNIX域通信代码示例，服务器端
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
//定义用于通信的文件名
#define UNIX_DOMAIN "/tmp/UNIX.domain"
int main()
{
    socklen_t clt_addr_len;
    int listen_fd;
    int com_fd;
    int ret;
    int i;
   static char recv_buf[1024]; 
    int len;
    struct sockaddr_un clt_addr;
    struct sockaddr_un srv_addr;
    //创建用于通信的套接字，通信域为UNIX通信域
    listen_fd=socket(PF_UNIX,SOCK_STREAM,0);
    if(listen_fd<0){
        perror("cannot create listening socket");
        return 1;
    }
    //设置服务器地址参数
    srv_addr.sun_family=AF_UNIX;
    strncpy(srv_addr.sun_path,UNIX_DOMAIN,sizeof(srv_addr.sun_path)-1);
    unlink(UNIX_DOMAIN);
    //绑定套接字与服务器地址信息
    ret=bind(listen_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
    if(ret==-1){
        perror("cannot bind server socket");
        close(listen_fd);
        unlink(UNIX_DOMAIN);
        return 1;
    }
    //对套接字进行监听，判断是否有连接请求
    ret=listen(listen_fd,1);
    if(ret==-1){
        perror("cannot listen the client connect request");
        close(listen_fd);
        unlink(UNIX_DOMAIN);
        return 1;
    }
    //当有连接请求时，调用accept函数建立服务器与客户机之间的连接
    len=sizeof(clt_addr);
    com_fd=accept(listen_fd,(struct sockaddr*)&clt_addr,&len);
    if(com_fd<0){
         perror("cannot accept client connect request");
        close(listen_fd);
        unlink(UNIX_DOMAIN);
        return 1;
    }
    //读取并输出客户端发送过来的连接信息
    printf("/n=====info=====/n");
    for(i=0;i<4;i++){
        memset(recv_buf,0,1024);
        int num=read(com_fd,recv_buf,sizeof(recv_buf));
        printf("Message from client (%d)) :%s/n",num,recv_buf);  
    }
    close(com_fd);
    close(listen_fd);
    unlink(UNIX_DOMAIN);
    return 0;
}
### 13.3.2 客户端实现
程序p13.3.c为使用套接字在UNIX域内 实现进程间通信的客户端程序。相比服务端的程序，客户段较为简单。程序首先通过调用socket函数创建通信所需的套接字，然后，调用connect函数 来连接服务器，在成功建立连接后，通过调用write函数向服务器发送指定的消息。p13.3.c的具体代码如下：
//p13.3.c UNIX域通信代码示例，客户端
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
//定义用于通信的文件名
#define UNIX_DOMAIN "/tmp/UNIX.domain"
int main(void)
{
    int connect_fd;
    int ret;
    char snd_buf[1024];
    int i;
    static struct sockaddr_un srv_addr;
    //创建用于通信的套接字，通信域为UNIX通信域
    connect_fd=socket(PF_UNIX,SOCK_STREAM,0);
    if(connect_fd<0){
        perror("cannot create communication socket");
        return 1;
    }   
    srv_addr.sun_family=AF_UNIX;
    strcpy(srv_addr.sun_path,UNIX_DOMAIN);
    //连接服务器
    ret=connect(connect_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
    if(ret==-1){
        perror("cannot connect to the server");
        close(connect_fd);
        return 1;
    }
    memset(snd_buf,0,1024);
    strcpy(snd_buf,"message from client");
    //给服务器发送消息
    for(i=0;i<4;i++)
        write(connect_fd,snd_buf,sizeof(snd_buf));
   close(connect_fd);
    return 0;
}
### 13.3.3 程序运行结果分析
使用gcc编译p13.2.c和p13.3.c，获得名为srv和clt的可执行文件。先执行srv程序，然后运行clt程序，具体输出如下：
[program@localhost charter13]$ gcc p13.3.c -o clt
[program@localhost charter13]$ gcc -o srv p13.2.c
[program@localhost charter13]$ ./srv &
[1] 13450
[program@localhost charter13]$ ./clt &
[2] 13451
[program@localhost charter13]$
=====info=====
Message from client (1024)) :message from client
Message from client (1024)) :message from client
Message from client (1024)) :message from client
Message from client (1024)) :message from client
[1]- Done                    ./srv
[2]+ Done                    ./clt
[program@localhost charter13]$
当运行srv程序后，该程序将处于监听状态。这时，可以通过netstat命令查看程序运行情况，如图13.4所示。可以从图中看到，srv的套接字类型为流套接字，并处于监听状态。
![](http://book.csdn.net/BookFiles/545/img/image049.jpg)

