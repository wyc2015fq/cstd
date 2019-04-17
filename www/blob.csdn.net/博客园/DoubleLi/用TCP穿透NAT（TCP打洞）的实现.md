# 用TCP穿透NAT（TCP打洞）的实现 - DoubleLi - 博客园






# 1. TCP穿透原理：

    我们假设在两个不同的局域网后面分别有2台客户机A和 B，AB所在的局域网都分别通过一个路由器接入互联网。互联网上有一台服务器S。 
    现在AB是无法直接和对方发送信息的，AB都不知道对方在互联网上真正的IP和端口， AB所在的局域网的路由器只允许内部向外主动发送的信息通过。对于B直接发送给A的路由器的消息，路由会认为其“不被信任”而直接丢弃。 
    要实现 AB直接的通讯，就必须进行以下3步：A首先连接互联网上的服务器S并发送一条消息（对于UDP这种无连接的协议其实直接初始会话发送消息即可），这样S就获取了A在互联网上的实际终端（发送消息的IP和端口号）。接着 B也进行同样的步骤，S就知道了AB在互联网上的终端（这就是“打洞”）。接着S分别告诉A和B对方客户端在互联网上的实际终端，也即S告诉A客户B的会话终端，S告诉B客户A的会话终端。这样，在AB都知道了对方的实际终端之后，就可以直接通过实际终端发送消息了（因为先前双方都向外发送过消息，路由上已经有允许数据进出的消息通道）。



# 2. 程序思路：

1：启动服务器，监听端口8877
2：第一次启动客户端（称为client1），连上服务器，服务器将返回字符串first，标识这个是client1，同时，服务器将记录下这个客户端的（经过转换之后的）IP和端口。
3：第二次启动客户端（称为client2），连上服务器，服务器将向其返回自身的发送端口（称为port2)，以及client1的（经过转换之后的）IP和端口。
4：然后服务器再发client1返回client2（经过转换之后的）IP和端口，然后断开与这两个客户端的连接（此时，服务器的工作已经全部完成了）
5：client2尝试连接client1，这次肯定会失败，但它会在路由器上留下记录，以帮忙client1成功穿透，连接上自己，然后设置port2端口为可重用端口，并监听端口port2。
6：client1尝试去连接client2，前几次可能会失败，因为穿透还没成功，如果连接10次都失败，就证明穿透失败了（可能是硬件不支持）,如果成功，则每秒向client2发送一次hello, world
7：如果client1不断出现send message: Hello, world，client2不断出现recv message: Hello, world，则证明实验成功了，否则就是失败了。



# 3. 声明

1：这个程序只是一个DEMO，所以肯定有很多不完善的地方，请大家多多见谅。
2：在很多网络中，这个程序并不能打洞成功，可能是硬件的问题（毕竟不是每种路由器都支持穿透），也可能是我程序的问题，如果大家有意见或建议，欢迎留言或给我发邮件（邮箱是：aa1080711@163.com）



# 4. 上代码：

服务器端：



Cpp代码  ![收藏代码](http://huaonline.iteye.com/images/icon_star.png)

- /* 
- 文件：server.c 
- PS：第一个连接上服务器的客户端，称为client1，第二个连接上服务器的客户端称为client2 
- 这个服务器的功能是： 
- 1：对于client1，它返回"first"，并在client2连接上之后，将client2经过转换后的IP和port发给client1； 
- 2：对于client2，它返回client1经过转换后的IP和port和自身的port，并在随后断开与他们的连接。 
- */  
- 
- #include <stdio.h>  
- #include <unistd.h>  
- #include <signal.h>  
- #include <sys/socket.h>  
- #include <fcntl.h>  
- #include <stdlib.h>  
- #include <errno.h>  
- #include <string.h>  
- #include <arpa/inet.h>  
- 
- #define MAXLINE 128  
- #define SERV_PORT 8877  
- 
- //发生了致命错误，退出程序  
- void error_quit(const char *str)      
- {      
-     fprintf(stderr, "%s", str);    
- //如果设置了错误号，就输入出错原因  
- if( errno != 0 )  
-         fprintf(stderr, " : %s", strerror(errno));  
-     printf("\n");  
-     exit(1);      
- }     
- 
- int main(void)        
- {            
- int i, res, cur_port;   
- int connfd, firstfd, listenfd;     
- int count = 0;  
- char str_ip[MAXLINE];    //缓存IP地址  
- char cur_inf[MAXLINE];   //当前的连接信息[IP+port]  
- char first_inf[MAXLINE];    //第一个链接的信息[IP+port]  
- char buffer[MAXLINE];    //临时发送缓冲区  
-     socklen_t clilen;        
- struct sockaddr_in cliaddr;        
- struct sockaddr_in servaddr;  
- 
- //创建用于监听TCP协议套接字          
-     listenfd = socket(AF_INET, SOCK_STREAM, 0);        
-     memset(&servaddr, 0, sizeof(servaddr));        
-     servaddr.sin_family = AF_INET;        
-     servaddr.sin_addr.s_addr = htonl(INADDR_ANY);        
-     servaddr.sin_port = htons(SERV_PORT);        
- 
- //把socket和socket地址结构联系起来         
-     res = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));      
- if( -1 == res )  
-         error_quit("bind error");  
- 
- //开始监听端口         
-     res = listen(listenfd, INADDR_ANY);      
- if( -1 == res )  
-         error_quit("listen error");  
- 
- while( 1 )  
-     {  
- //接收来自客户端的连接  
-         connfd = accept(listenfd,(struct sockaddr *)&cliaddr, &clilen);    
- if( -1 == connfd )  
-             error_quit("accept error");  
-         inet_ntop(AF_INET, (void*)&cliaddr.sin_addr, str_ip, sizeof(str_ip));  
- 
-         count++;  
- //对于第一个链接，将其的IP+port存储到first_inf中，  
- //并和它建立长链接，然后向它发送字符串'first'，  
- if( count == 1 )  
-         {  
-             firstfd = connfd;  
-             cur_port = ntohs(cliaddr.sin_port);  
-             snprintf(first_inf, MAXLINE, "%s %d", str_ip, cur_port);     
-             strcpy(cur_inf, "first\n");  
-             write(connfd, cur_inf, strlen(cur_inf)+1);  
-         }  
- //对于第二个链接，将其的IP+port发送给第一个链接，  
- //将第一个链接的信息和他自身的port返回给它自己，  
- //然后断开两个链接，并重置计数器  
- else if( count == 2 )  
-         {  
-             cur_port = ntohs(cliaddr.sin_port);  
-             snprintf(cur_inf, MAXLINE, "%s %d\n", str_ip, cur_port);  
-             snprintf(buffer, MAXLINE, "%s %d\n", first_inf, cur_port);  
-             write(connfd, buffer, strlen(buffer)+1);  
-             write(firstfd, cur_inf, strlen(cur_inf)+1);   
-             close(connfd);  
-             close(firstfd);  
-             count = 0;  
-         }  
- //如果程序运行到这里，那肯定是出错了  
- else  
-             error_quit("Bad required");  
-     }  
- return 0;  
- }  


 客户端：



Cpp代码  ![收藏代码](http://huaonline.iteye.com/images/icon_star.png)

- /* 
- 文件：client.c 
- PS：第一个连接上服务器的客户端，称为client1，第二个连接上服务器的客户端称为client2 
- 这个程序的功能是：先连接上服务器，根据服务器的返回决定它是client1还是client2， 
- 若是client1，它就从服务器上得到client2的IP和Port，连接上client2, 
- 若是client2，它就从服务器上得到client1的IP和Port和自身经转换后的port， 
- 在尝试连接了一下client1后（这个操作会失败），然后根据服务器返回的port进行监听。 
- 这样以后，就能在两个客户端之间进行点对点通信了。 
- */  
- 
- #include <stdio.h>  
- #include <unistd.h>  
- #include <signal.h>  
- #include <sys/socket.h>  
- #include <fcntl.h>  
- #include <stdlib.h>  
- #include <errno.h>  
- #include <string.h>  
- #include <arpa/inet.h>  
- 
- #define MAXLINE 128  
- #define SERV_PORT 8877  
- 
- typedef struct  
- {  
- char ip[32];  
- int port;  
- }server;  
- 
- //发生了致命错误，退出程序  
- void error_quit(const char *str)      
- {      
-     fprintf(stderr, "%s", str);   
- //如果设置了错误号，就输入出错原因  
- if( errno != 0 )  
-         fprintf(stderr, " : %s", strerror(errno));  
-     printf("\n");  
-     exit(1);      
- }     
- 
- int main(int argc, char **argv)       
- {            
- int i, res, port;  
- int connfd, sockfd, listenfd;   
-     unsigned int value = 1;  
- char buffer[MAXLINE];        
-     socklen_t clilen;          
- struct sockaddr_in servaddr, sockaddr, connaddr;    
-     server other;  
- 
- if( argc != 2 )  
-         error_quit("Using: ./client <IP Address>");  
- 
- //创建用于链接（主服务器）的套接字          
-     sockfd = socket(AF_INET, SOCK_STREAM, 0);   
-     memset(&sockaddr, 0, sizeof(sockaddr));        
-     sockaddr.sin_family = AF_INET;        
-     sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);        
-     sockaddr.sin_port = htons(SERV_PORT);        
-     inet_pton(AF_INET, argv[1], &sockaddr.sin_addr);  
- //设置端口可以被重用  
-     setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));  
- 
- //连接主服务器  
-     res = connect(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr));   
- if( res < 0 )  
-         error_quit("connect error");  
- 
- //从主服务器中读取出信息  
-     res = read(sockfd, buffer, MAXLINE);  
- if( res < 0 )  
-         error_quit("read error");  
-     printf("Get: %s", buffer);  
- 
- //若服务器返回的是first，则证明是第一个客户端  
- if( 'f' == buffer[0] )  
-     {  
- //从服务器中读取第二个客户端的IP+port  
-         res = read(sockfd, buffer, MAXLINE);  
-         sscanf(buffer, "%s %d", other.ip, &other.port);  
-         printf("ff: %s %d\n", other.ip, other.port);  
- 
- //创建用于的套接字          
-         connfd = socket(AF_INET, SOCK_STREAM, 0);   
-         memset(&connaddr, 0, sizeof(connaddr));        
-         connaddr.sin_family = AF_INET;        
-         connaddr.sin_addr.s_addr = htonl(INADDR_ANY);        
-         connaddr.sin_port = htons(other.port);      
-         inet_pton(AF_INET, other.ip, &connaddr.sin_addr);  
- 
- //尝试去连接第二个客户端，前几次可能会失败，因为穿透还没成功，  
- //如果连接10次都失败，就证明穿透失败了（可能是硬件不支持）  
- while( 1 )  
-         {  
- static int j = 1;  
-             res = connect(connfd, (struct sockaddr *)&connaddr, sizeof(connaddr));   
- if( res == -1 )  
-             {  
- if( j >= 10 )  
-                     error_quit("can't connect to the other client\n");  
-                 printf("connect error, try again. %d\n", j++);  
-                 sleep(1);  
-             }  
- else   
- break;  
-         }  
- 
-         strcpy(buffer, "Hello, world\n");  
- //连接成功后，每隔一秒钟向对方（客户端2）发送一句hello, world  
- while( 1 )  
-         {  
-             res = write(connfd, buffer, strlen(buffer)+1);  
- if( res <= 0 )  
-                 error_quit("write error");  
-             printf("send message: %s", buffer);  
-             sleep(1);  
-         }  
-     }  
- //第二个客户端的行为  
- else  
-     {  
- //从主服务器返回的信息中取出客户端1的IP+port和自己公网映射后的port  
-         sscanf(buffer, "%s %d %d", other.ip, &other.port, &port);  
- 
- //创建用于TCP协议的套接字          
-         sockfd = socket(AF_INET, SOCK_STREAM, 0);   
-         memset(&connaddr, 0, sizeof(connaddr));        
-         connaddr.sin_family = AF_INET;        
-         connaddr.sin_addr.s_addr = htonl(INADDR_ANY);        
-         connaddr.sin_port = htons(other.port);        
-         inet_pton(AF_INET, other.ip, &connaddr.sin_addr);  
- //设置端口重用  
-         setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));  
- 
- //尝试连接客户端1，肯定会失败，但它会在路由器上留下记录，  
- //以帮忙客户端1成功穿透，连接上自己   
-         res = connect(sockfd, (struct sockaddr *)&connaddr, sizeof(connaddr));   
- if( res < 0 )  
-             printf("connect error\n");  
- 
- //创建用于监听的套接字          
-         listenfd = socket(AF_INET, SOCK_STREAM, 0);   
-         memset(&servaddr, 0, sizeof(servaddr));        
-         servaddr.sin_family = AF_INET;        
-         servaddr.sin_addr.s_addr = htonl(INADDR_ANY);        
-         servaddr.sin_port = htons(port);  
- //设置端口重用  
-         setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));  
- 
- //把socket和socket地址结构联系起来   
-         res = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));      
- if( -1 == res )  
-             error_quit("bind error");  
- 
- //开始监听端口         
-         res = listen(listenfd, INADDR_ANY);      
- if( -1 == res )  
-             error_quit("listen error");  
- 
- while( 1 )  
-         {  
- //接收来自客户端1的连接  
-             connfd = accept(listenfd,(struct sockaddr *)&sockaddr, &clilen);    
- if( -1 == connfd )  
-                 error_quit("accept error");  
- 
- while( 1 )  
-             {  
- //循环读取来自于客户端1的信息  
-                 res = read(connfd, buffer, MAXLINE);  
- if( res <= 0 )  
-                     error_quit("read error");  
-                 printf("recv message: %s", buffer);  
-             }  
-             close(connfd);  
-         }  
-     }  
- 
- return 0;  
- }  




# 5. 运行示例：

（第一个终端）
qch@qch ~/program/tcode $ gcc server.c -o server
qch@qch ~/program/tcode $ ./server &
[1] 4688
qch@qch ~/program/tcode $ gcc client.c -o client
qch@qch ~/program/tcode $ ./client localhost
Get: first
ff: 127.0.0.1 38052
send message: Hello, world
send message: Hello, world
send message: Hello, world
.................


第二个终端：
qch@qch ~/program/tcode $ ./client localhost
Get: 127.0.0.1 38073 38074
connect error
recv message: Hello, world
recv message: Hello, world
recv message: Hello, world
..................









