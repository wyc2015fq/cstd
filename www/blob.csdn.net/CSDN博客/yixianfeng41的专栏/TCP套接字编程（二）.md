# TCP套接字编程（二） - yixianfeng41的专栏 - CSDN博客
2017年02月15日 13:22:58[第2梦](https://me.csdn.net/yixianfeng41)阅读数：344标签：[服务器select																[IO-select																[select模型](https://so.csdn.net/so/search/s.do?q=select模型&t=blog)](https://so.csdn.net/so/search/s.do?q=IO-select&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器select&t=blog)
个人分类：[Linux/Unix](https://blog.csdn.net/yixianfeng41/article/category/6006179)
所属专栏：[Linux/Unix 网络编程](https://blog.csdn.net/column/details/14513.html)
[篇一：TCP套接字编程（一）](http://blog.csdn.net/yixianfeng41/article/details/55102751)
[篇二：TCP套接字编程（二）](http://blog.csdn.net/yixianfeng41/article/details/55194329)
前面通过多进程，多线程，支持处理多个客户端通信，本篇将讲述用select这个IO复用来实现。
## IO模型
首先连接一下IO模型，unix下有5种可用IO模型，分别是：阻塞式IO、非阻塞式IO、IO复用、信号驱动式IO。 
![这里写图片描述](https://img-blog.csdn.net/20170215130413400)
5种IO模型比较
#### 1、阻塞式IO
阻塞式IO是最常用的IO模式，它就是没有数据就阻塞，一直等到有数据位置，期间啥也不干，就一门心思等数据。
#### 2、非阻塞式IO
非阻塞式IO，就是不断轮询，问“数据有没有准备好啊”，“数据有没有准备好啊”….。数据准备好了，返回数据。不断询问，会耗费大量CPU时间。
#### 3、IO复用模型
slect和pool就是IO复用。它是阻塞在系统调用，而不是阻塞在真正的IO系统调用上。学会转移矛盾，避免呢伤害了，自己很忙，不能阻塞自己，让别人帮忙等待（系统调用）。
#### 4、信号驱动式IO模型
信号是个好东西，可以让内核在描述符就绪了，用信号（SIGIO）通知自己呀，这样不就工作学习两不误了。
#### 5、异步IO模型
信号驱动IO是由内核通知我们何时可以启动一个IO操作，这个不通知我们何时启动，而是直接替我们工作，并由内核告诉我们何时可以完成。一个是何时开始，一个是何时完成。
## select函数
slelect函数原型为：
`int select(int maxfdp1,fd_set *readfds,fd_set *writefds,fd_set *exceptfds,struct timeval *timeout);`
参数含义：  
- maxfdp1:指定待预测的描述符个数，它的值是待测试的最大描述符加1，描述符0,1,2，… ，maxfdp1-1；
- readfds,writefds,exceptfds:指定我们要让内核测试读、写和异常条件的描述符；
- timeout:告知内核等待就绪描述符可花多长时间（不等，等一段时间，永久等待）。
描述符操作：  
```
void FD_ZERO(fd_set *fdset); //initialize the set:all bits off
void FD_SET(int fd,fd_set *fdset);  //turn on the bit for fd in dfset
void FD_CLR(int fd,fd_set *fdset);  //turn off the bit for fd in dfset
int  FD_ISSET(int fd,fd_set *fdset);  // is the bit for fd on in fdset
```
了解了select的使用，下面就来用select实现echo服务器。
## server select版
```
/**
    Handle multiple socket connections with select and fd_set on Linux
*/
#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/select.h> //FD_SET, FD_ISSET, FD_ZERO macros
#define TRUE   1
#define FALSE  0
#define PORT 8888
#define BUF_SIZE 1205
#define MAX_ENQ 1024
int main(int argc , char *argv[])
{
    int opt = TRUE;
    int listenfd ,connfd,max_fd,sockfd, activity,i ,valread ;
    int client[FD_SETSIZE],max_client_index;
    socklen_t socklen=sizeof(struct sockaddr_in);
    char buffer[BUF_SIZE];  //data buffer of 1K
    struct sockaddr_in servaddr,cliaddr;
    //set of socket descriptors
    fd_set rset,allset;
    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < FD_SETSIZE; i++) 
    {
        client[i] = -1;
    }
    max_client_index=-1;
    //create a master socket
    if( (listenfd = socket(AF_INET , SOCK_STREAM , 0)) <0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    //set master socket to allow multiple connections , this is just a good habit, it will work without this
    if( setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    //type of socket created
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons( PORT );
    //bind the socket to localhost port 8888
    if (bind(listenfd, (struct sockaddr *)&servaddr, socklen)<0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    //try to specify maximum of 3 pending connections for the master socket
    if (listen(listenfd, MAX_ENQ) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    //accept the incoming connection
    printf("echo server use select startup, listen on port %d\n", PORT);
    printf("max connection: %d\n", FD_SETSIZE);
    puts("Waiting for connections ...");  
    //clear the socket set
    FD_ZERO(&allset);    
    //add master socket to set
    FD_SET(listenfd, &allset); 
    max_fd = listenfd;
    while(TRUE) 
    {
        rset=allset;
        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_fd + 1 , &rset , NULL , NULL , NULL);   
        if ((activity < 0) && (errno!=EINTR)) 
        {
            printf("select error");
        }
        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(listenfd, &rset)) 
        {          
            if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &socklen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , connfd , inet_ntoa(cliaddr.sin_addr) , ntohs(cliaddr.sin_port));
            //send new connection greeting message
            //a message
            char *message = "good ,connect successfully \r\n";
            if( send(connfd, message, strlen(message), 0) != strlen(message) ) 
            {
                perror("send");
            }
            //add new socket to array of sockets
            for (i = 0; i < FD_SETSIZE; i++) 
            {
                if( client[i] ==-1)
                {
                    client[i] = connfd;                   
                    break;
                }
            }
            if (i == FD_SETSIZE)
            {
                fprintf(stderr, "too many connection, more than %d\n", FD_SETSIZE);
                close(connfd);
                continue;
            }
            if (connfd>max_fd)
               max_fd=connfd;
           if(i>max_client_index)
               max_client_index=i;
            FD_SET(connfd, &allset);  //一定不能丢，一定不能丢
            if(--activity<=0)
                continue;
        }
        //else its some IO operation on some other socket :)
        for (i = 0; i <= max_client_index; i++) 
        {
            if((sockfd = client[i])!=-1)
            {
                if (FD_ISSET( sockfd , &rset)) 
                {
                    //Check if it was for closing , and also read the incoming message
                    if ((valread = read( sockfd , buffer, BUF_SIZE)) == 0)
                    {
                        //Somebody disconnected , get his details and print
                        getpeername(sockfd , (struct sockaddr*)&cliaddr , &socklen);
                        printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(cliaddr.sin_addr) , ntohs(cliaddr.sin_port));
                        //Close the socket and mark as 0 in list for reuse
                        close(sockfd);
                        FD_CLR(sockfd, &allset);
                        client[i] = -1;
                        continue;
                    }
                    else if (valread<0)
                    {
                        perror("read error");
                        close(sockfd);
                        FD_CLR(sockfd, &allset);
                        client[i] = -1;
                        continue;
                    }
                    //Echo back the message that came in
                    else 
                    {
                        printf("get message form client %d,meeasge is %s\n" , i,buffer);
                        //set the string terminating NULL byte on the end of the data read
                        buffer[valread] = '\0';
                        send(sockfd , buffer , strlen(buffer) , 0 );
                    }
                    if(--activity<=0)
                        break;
                }
            }                
        }
    }
    return 0;
}
```
真是不容易，花了一个早上才调试通。测试结果如下： 
服务器： 
![](https://img-blog.csdn.net/20170215131550103)
客户端1： 
![](https://img-blog.csdn.net/20170215131656933)
客户端2： 
![](https://img-blog.csdn.net/20170215131741518)
客户端3： 
![](https://img-blog.csdn.net/20170215131822190)
select学习就到这里了，接下来试着用pool和epool实现服务器，加油！！！！！
