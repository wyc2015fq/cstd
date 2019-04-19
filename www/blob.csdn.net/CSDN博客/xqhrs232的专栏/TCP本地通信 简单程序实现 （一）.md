# TCP本地通信 简单程序实现 （一） - xqhrs232的专栏 - CSDN博客
2013年06月28日 10:16:19[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：668
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.chinaunix.net/uid-22666248-id-295734.html](http://blog.chinaunix.net/uid-22666248-id-295734.html)
相关网帖
1、[http://blog.chinaunix.net/uid-22666248-id-295734.html](http://blog.chinaunix.net/uid-22666248-id-295734.html)----[http://blog.chinaunix.net/uid-22666248-id-296259.html](http://blog.chinaunix.net/uid-22666248-id-296259.html)
服务端：
- #include<sys/types.h>
- #include<sys/socket.h>
- #include<stdio.h>
- #include<sys/un.h>
- #include<unistd.h>
- #include<stdlib.h>
- 
- int main(int argc,char
*argv[])
- {
- int server_sockfd,client_sockfd;
- int server_len,client_len;
-     struct sockaddr_un server_address;
-     struct sockaddr_un client_address;
- 
- int i,byte;
-     char ch_send,ch_recv;
-     unlink("server_socket");
- 
-         server_sockfd=socket(AF_UNIX,SOCK_STREAM,0);// AF_INET IPV4域 AF_UNIX UNIX 域 SOCK_STREAM 可靠/双向
- //面向链接字节流，0 ：系统自动选择 协议
- 
-     server_address.sun_family=AF_UNIX;
-     strcpy(server_address.sun_path,"server_socket");//如果是
 AF_UNIX域，需要与一个文件进行绑定 //临时文件                           //会在当前目录下，生成临时文件  server_socket
-     server_len=sizeof(server_address);
- //绑定 ip 地址和端口
- bind(server_sockfd,(struct sockaddr
*)&server_address,server_len);
- //监听网络，
- listen(server_sockfd,5);
-     printf("is watiing for client connect...\n");
- 
- //客户端发起连接
-     client_len=sizeof(client_address);
-     client_sockfd=accept(server_sockfd,(struct sockaddr
*)&client_address,(socklen_t
*)&client_len);//当没有 客户端请求，accept 会阻塞
- while(1)
- {
- if((byte=read(client_sockfd,&ch_recv,1))==-1)
- {
-             perror("read");
- exit(EXIT_FAILURE);
- }
-         printf("receive a character from client is : %c \n",ch_recv);
- //sleep(1);
- 
- //    printf("input a charater to send :\n");
-         ch_send=getchar();
- if((byte=write(client_sockfd,&ch_send,1))==-1)
- {
-             perror("write");
- exit(EXIT_FAILURE);
- }
- //printf("");
- 
- }
-     close(client_sockfd);
-     unlink("server socket");
-     return 0;
- 
- }/**/
客户端：
- #include <sys/types.h>
- #include <sys/socket.h>
- #include <stdio.h>
- #include <sys/un.h>
- #include <stdlib.h>
- #include <unistd.h>
- 
- int main(int argc,char
*argv[])
- {
- int sockfd;
- intlen;
-     struct sockaddr_un address;
- int i,byte;
-     char ch_recv,ch_send;
- 
-     sockfd=socket(PF_UNIX,SOCK_STREAM,0);
- 
-     address.sun_family=AF_UNIX;
-     strcpy(address.sun_path,"server_socket");
- len=sizeof(address);
- connect(sockfd,(struct sockaddr*)&address,len);
- 
- while(1)
- {
- //    printf("intpur a charater to send:\n");
-         ch_send=getchar();
-         write(sockfd,&ch_send,1);
-         byte=read(sockfd,&ch_recv,1);
-         printf("receiver charater from server is %c:\n\n",ch_recv);
- }
- 
-     close(sockfd);
-     return 0;
- }
