# Linux 图片传输功能c/c++（初版） - 建建的博客 - CSDN博客
2017年08月30日 14:01:40[纪建](https://me.csdn.net/u013898698)阅读数：1142
由于深度学习任务的需要，要在程序里面嵌入一个module。
这个module 的功能是接收来自ios客户端的图片。并且传送给深度学习分类器进行处理。
于是看了看各种各样的 module 实现方案。
先是用IO函数把图片以数组的方式存起来。
再和服务器建立连接，socket传这个数组。
然后服务器接收完数组以后就再运用IO函数 把数组以图片的方式存起来！
然后用各种网络编程和操作系统的技巧，多线程接收多线程发送来改善速度。
现在先讲述一种最原始的 图片传输的module
![](http://images2015.cnblogs.com/blog/1043349/201702/1043349-20170223211757273-817313854.png) 这是服务端进程运行
它将接收fish-bike.jpg 并且将之存到/root/photo_out这目录下！
![](http://images2015.cnblogs.com/blog/1043349/201702/1043349-20170223211906866-847931238.png)这是客户端进程运行
它将发送/root/photo_in/fish-bike.jpg 给服务进程。同时在本地/root/photo_in/复制一个fish-bike.jpg（这是本地测试的demo）
![](http://images2015.cnblogs.com/blog/1043349/201702/1043349-20170224140809866-62008163.png)传送成功！但是慢！
以下是客户端代码
```
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#define SERVER_IP "127.0.0.1"
#define PORT 6675
int main()
{
         int socket_fd;
          socket_fd=socket(AF_INET,SOCK_STREAM,0);
          struct sockaddr_in server_addr;
              //initialize sockfd 
   
         memset(&server_addr,0,sizeof(server_addr));
         server_addr.sin_family=AF_INET;
         server_addr.sin_port=htons(PORT);//6675
         server_addr.sin_addr.s_addr=inet_addr(SERVER_IP);//127.0.0.1 local?
         memset(&(server_addr.sin_zero),0,8);
          //clean server addr 
            int res =connect(socket_fd,(struct sockaddr *)&server_addr,sizeof(server_addr));
         //connect server_addr
  
           printf("connect res is %d\n",res); 
           /*
             while(1)
             {
                 printf("send send ---->\n");
                 char input[100];
                 char output[100]; 
                 memset(input,0,sizeof(input));
                 gets(input);
                 res=write(socket_fd,input,strlen(input));
                 printf("the write resoult is %d\n",res);
                 
                 res=read(socket_fd,output,100);
                 output[res]=0;
                 printf("server says:'%s'\n",output);
                 sleep(1);
             }
         */
         char path[100];
         printf("please give the path of photo!\n");
         char input[100];
         gets(input);
         sprintf(path,"/root/photo_in/%s",input);
          printf("begin to write");
         write(socket_fd,input,strlen(input));
         printf("write %s success",input);
 
         FILE *out=fopen(path,"r");
         printf("read %s success",path);
         FILE *new=fopen("/root/photo_in/new.jpg","w");
          printf("open /root/photo_in/new.jpg success");
         int c;
         sleep(1);
         printf("ready to open!");
         while((c=fgetc(out))!=EOF)//open /root/photo_in/cat.jpg 
         {                        
                                    //get c of cat.jpg
            
             char photo[100];
             sprintf(photo,"%d",c);//write c into photo[]
              
              int b=atoi(photo);
              fputc(b,new);
                // printf("data %s\n",photo);
                 write(socket_fd,photo,strlen(photo)); //write photo[]to server
                 usleep(1500);//sleep(?)
         }
         char *end="#";
         write(socket_fd,end,strlen(end));
         close(socket_fd);
 }
```
以下是服务端代码
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> //sockaddr_in
 
#define PORT 6675
 
int main()
{
  int socket_fd;
    int new_fd;         //建立套接字
     
    socket_fd=socket(AF_INET,SOCK_STREAM,0);// int socket(int domain, int...
                                          
    printf("socket_fd is %d\n",socket_fd);
         //初始化设置地址和端口号
   struct sockaddr_in server_addr; 
    memset(&server_addr,0,sizeof(server_addr));
    
      server_addr.sin_family=AF_INET;
      server_addr.sin_port=htons(PORT);
      server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
      memset(&(server_addr.sin_zero),0,8);
      //设置端口可重用
         int contain;
         setsockopt(socket_fd,SOL_SOCKET, SO_REUSEADDR, &contain, sizeof(int));         
         //绑定fd和本机地址
         int res=bind(socket_fd,(struct sockaddr *)&server_addr,sizeof(server_addr));
         printf("bind res is %d\n",res);
         if(res==-1)                 return -1;
         if(listen(socket_fd,20)==-1)
         {
                 printf("Failed to listen!\n");
                 return -1;
         }
         printf("after listen\n");
 
         while(1)
         {
             struct sockaddr_in client_addr;
            int size=sizeof(client_addr);
            new_fd=accept(socket_fd, (struct sockaddr *)&client_addr, &size);
                if(new_fd==-1)
                 {
                         printf("wait for client to connect!\n");
                         sleep(1);
                         continue;
                 }
                 printf("A connect and the new fd is %d\n",new_fd);
         /*
                 while(1)
                 {          char input[100];
                            char output[100];
                     res=read(new_fd,input,100);
                      input[res]=0;
                   printf("client says '%s'\n",input);                   
                     gets(output);
                     res=write(new_fd,output,strlen(output));
                 }
         */
         
                 //while(1)
                 {
                         FILE *in;
                         char input[100];
                         res=read(new_fd,input,100);
                         input[res]=0;
                         char path[100];
                      sprintf(path,"/root/photo_out/%s",input);
                       printf("the path is %s\n",path);
                         in=fopen(path,"w");
                         printf("after path\n");
                        char photo[10];
                         while(1)
                         {
                                 memset(photo,0,10);
                                 res=read(new_fd,photo,10);
                                 if(res==0){printf("read nothing"); return 0;}
                                 photo[res]=0;
                                 int d=atoi(photo);
                               //  printf("data %s\n",photo);
                                 fputc(d,in);
                         }
                 }
         }
     close(socket_fd);
     close(new_fd); 
 }
```
所以接下来是不放在本地测试，而是放在腾讯云服务器上测试！
放在远端服务器上测试没想到套接字阻塞的现象比本地测试更加严重！
就是 客户端传4 18 3过去 
结果 服务端把4183 当做一个点的像素值写了进去！
![](http://images2015.cnblogs.com/blog/1043349/201702/1043349-20170224151910163-429579015.png)
