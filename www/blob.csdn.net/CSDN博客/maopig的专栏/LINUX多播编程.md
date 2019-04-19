# LINUX多播编程 - maopig的专栏 - CSDN博客
2011年11月23日 11:13:30[maopig](https://me.csdn.net/maopig)阅读数：2685标签：[linux																[struct																[socket																[路由器																[服务器																[网络协议](https://so.csdn.net/so/search/s.do?q=网络协议&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=路由器&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[网络](https://blog.csdn.net/maopig/article/category/869532)
一.单播，广播和多播
1.单播用于两个主机之间的端对端通信，广播用于一个主机对整个局域网上所有主机上的数据通信。单播和广播是两个极端，要么对一个主机进行通信，要么对整个局域网上的主机进行通信。实际情况下，经常需要对一组特定的主机进行通信，而不是整个局域网上的所有主机，这就是多播的用途。
2.多播的概念：
1>多播，也称为“组播”，将局域网中同一业务类型主机进行了逻辑上的分组，进行数据收发的时候其数据仅仅在同一分组中进行，其他的主机没有加入此分组不能收发对应的数据。
2>在广域网上广播的时候，其中的交换机和路由器指向需要获取数据的主机复制并转发数据。主机可以向路由器请求加入或退出某个组，网络中的路由器和交换机有选择地复制并传输数据，将数据仅仅传输给组内的主机。
3.多播的优点和缺点
1>优点：
?  具有同种业务的主机加入同一数据流，共享同一通道，节省了带宽和服务器的优点，具有广播的优点而又没有广播所需要的带宽。
?  服务器的总带宽不受客户端带宽的限制。由于组播协议由接收者的需求来确定是否进行数据流的转发，所以服务器端的带宽是常量，与客户端的数量无关。
?  与单播一样，多播是允许在广域网即Internet上进行传输的，而广播仅仅在同一局域网上才能进行。
2>组播的缺点：
?  多播与单播相比没有纠错机制，当发生错误的时候难以弥补，但是可以在应用层来实现此种功能
?  多播的网络支持存在缺陷，需要路由器及网络协议栈的支持。
4.目前多播的应用主要有网上视频，网上会议等
5.多播地址：
1>多播的地址是特定的，D类地址用于多播。D类IP地址就是多播IP地址，即224.0.0.0到239.255.255.255之间的IP地址，并被划分为局部连续多播地址，预留多播地址和管理权限多播地址3类：
?  局部多播地址：在224.0.0.0~224.0.0.251之间，这是路由协议和其他用途保留的地址，路由器并不转发属于此范围的IP包。（即此类范围的多播IP地址只能在局域网内使用）
?  预留多播地址：在224.0.1.0~238.255.255.255之间，可用于全球范围或网络协议
?  管理权限多播地址：在239.0.0.0~239.255.255.255之间，可供组织内部使用，类似于私有IP地址，不能用于Internet,可限制多播范围。
6.IPv4多播地址到以太网地址的映射方法：
1>IPv4的D类地址（从224.0.0.0到239.255.255.255）是IPv4多播地址;D类地址的低序28位构成多播组ID(group  ID),整个32位地址则称为组地址（group address）。
2>IPv4地址的映射，以太网地址的高24位总是01:00:5e下一位总是0,底序23位拷贝自多播组ID的底序23位。这样就组成了多播地址的以太网地址。
本篇文章来源于 Linux公社网站([www.linuxidc.com](http://www.linuxidc.com))  原文链接：[http://www.linuxidc.com/Linux/2011-09/43602.htm](http://www.linuxidc.com/Linux/2011-09/43602.htm)
1>建立一个socket;
2>设置多播的参数，例如超时时间TTL，本地回环许可LOOP等
3>加入多播组
4>发送和接收数据
5>从多播组离开
2.多播程序设计使用setsockopt()函数和getsockopt()函数来实现，组播的选项是IP层的。
3.setsockopt()的选项
1>IP_MULTICAST_TTL:设置多播组数据的TTL值（路由跳数），每跨过一个路由器，TTL值减一.范围为0~255之间的任何值。
int  ttl;
setsockopt(sock_fd,IPPROTO_IP,IP_MULTICAST_TTL,&ttl,sizeof(ttl));
2>IP_MULTICAST_LOOP:默认情况下，当本机发送组播数据到某个网络接口时，在IP层，数据会回送公道本地的回环接口，选项IP_MULTICAST_LOOP用于控制数据是否回送到本地的回环接口。
int  loop;
setsockopt(sock_fd,IPPROTO_IP,IP_MULTICAST_LOOP,&loop,sizeof(loop));
参数loop设置为0表示禁止回送，设置为1允许回送。
3>IP_ADD_MEMBERSHIP: 该选项通过对一个结构struct ip_mreq类型的变量进行控制而加入一个多播组。
?  struct ip_mreq
struct ip_mreq
{
       struct in_addr imr_multiaddr;/*加入的多播组IP地址*/
       struct  in_addr imr_interface;/*加入的网络接口IP地址*/
}；
?  选项IP_ADD_MEMBESHIP选项用于加入某个多播组，之后就可以向这个多播组发送数据或者从多播组接收数据。此选项的值为mreq结构，成员imr_multiaddr是需要加入的多播组IP地址，成员imr_interface是本机需要加入多播组的网络接口IP地址。
struct ip_mreq  mreq;
setsockopt(sock_fd,IPPROTO_IP,IP_ADD,MEMBERSHIP,&mreq,sizeof(mreq));
?  使用IP_ADD_MEMBERSHIP选项每次只能加入一个网络接口的IP地址到多播组，但并不是一个多播组仅允许一个主机IP地址加入，可以多次调用IP_ADD_MEMBERSHIP选项来实现多个IP地址加入同一个多播组，或者同一个IP地址加入多个多播组。当imr_interface为INADDR_ANY时，选择的本地默认网口。
4>IP_DROP_MEMBERSHIP:该选项用于从一个多播组中退出。
struct ip_mreq  mreq;
setsockopt(sock_fd,IPPROTP_IP,IP_DROP_MEMBERSHIP,&merq,sizeof(sreq));
三实例：
情景：
?  服务器端每隔2秒向目的端口号为5000和目的多播地址为224.0.0.255发送数据welcome you to multicast socket programme。
?  客户端从多播地址为224.0.0.255和端口号5000处接收5次多播数据。
LINUX 
 1.服务器端程序
![](http://hi.csdn.net/attachment/201111/23/0_1322032316lMUU.gif)
 1  #include<stdio.h>
  2 #include<string.h>
  3 #include<stdlib.h>
  4 #include<unistd.h>
  5 #include<sys/socket.h>
  6 #include<sys/types.h>
  7 #include<arpa/inet.h>
  8 #include<netinet/in.h>
  9 
 10 #define  SERV_PORT 5350
 11 #define  MCAST_PORT 5350
 12 #define  MCAST_ADDR "224.0.0.251"
 13 #define  MCAST_INTERVAL   2
 14 
 15 int main(int argc,char **argv)
 16 {
 17         int sock_fd;
 18         struct sockaddr_in mcast_addr,addr_serv,addr_client;
 19         char send_buf[]="tigerjibo";
 20         char recv_buf[1024];
 21         int serv_len,mcast_len;
 22         int send_num;
 23         int recv_num;
 24         int err;
 25         struct ip_mreq mreq;
 26         sock_fd = socket(AF_INET,SOCK_DGRAM,0);/*建立套接子*/
 27         if(sock_fd < 0){
 28                 perror("socket()");
 29                 exit(1);
 30         } else {
 31                 printf("socket sucefull!\n");
 32         }
 33         /*服务器端地址*/
 34         memset(&addr_serv,0,sizeof(struct sockaddr_in));
 35         addr_serv.sin_family = AF_INET;
 36         addr_serv.sin_addr.s_addr = inet_addr("192.168.1.11");
 37         addr_serv.sin_port = htons(SERV_PORT);
 38         serv_len = sizeof(struct sockaddr_in);
 39        //初始化多播地址
 40         memset(&mcast_addr,0,sizeof(mcast_addr));
 41         mcast_addr.sin_family = AF_INET;
 42         mcast_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);
 43         mcast_addr.sin_port = htons(MCAST_PORT);
 44 
 45         if(bind(sock_fd,(struct sockaddr*)&addr_serv,serv_len)<0){
 46                 perror("bind");
 47                 exit(1);
 48         } else {
 49                 printf("bind sucess\n");
 50         }
 51         while(1){
 52                 printf("begin send:\n");
 53                 send_num = sendto(sock_fd,send_buf,sizeof(send_buf),0,(struct sockaddr *)&mcast_addr,sizeof(mcast_addr));
 54                 if( send_num  < 0){
 55                         perror("sendto()");
 56                         exit(1);
 57                 } else{
 58                         printf("send sucessful\n");
 59                 }
 60                 sleep(MCAST_INTERVAL);
 61         }
 62         close(sock_fd);
 63         return 0;
 64 }
                                                                                                                                                                                   64,1         底端
 1.客户端程序                                              
![](http://hi.csdn.net/attachment/201111/23/0_13220323278r8N.gif)
 1 #include<stdio.h>
  2 #include<string.h>
  3 #include<stdlib.h>
  4 #include<errno.h>
  5 
  6 #include<sys/socket.h>
  7 #include<sys/types.h>
  8 #include<arpa/inet.h>
  9 #include<netinet/in.h>
 10 #include<unistd.h>
 11 #define  LOCAL_PORT      5350
 12 #define  MCAST_ADDR      "224.0.0.251"
 13 #define  MCAST_INTERVAL    5
 14 #define  LOCAL_ADDR   "192.168.1.11"
 15 /*
 16 static void mdns_mcast_group(struct sockaddr_in *mcast_addr){
 17         mcast_addr->sin_family = AF_INET;
 18         mcast_addr->sin_port = htons(MCAST_PORT);
 19         mcast_addr->sin_addr.s_addr = inet_addr(MCAST_ADDR);
 20 
 21 }
 22 int mdns_open_socket(void){
 23         struct ip_mreq mreq;
 24         struct sockaddr_in sa;
 25         int fd =-1,ttl,yes;//yes地址重用
 26         
 27         mdns_mcast_group(&sa);
 28 
 29         if((fd = socket(AF_INET,SOCK_DGRAG,0)) < 0){
 30                 printf("socket() failed: %s\n",strerror(errno));
 31         }
 32 }*/
 33 int main()
 34 {
 35         int sock_fd;
 36         struct sockaddr_in local_addr;
 37         int err = -1;
 38         int loop = 1;
 39         sock_fd = socket(AF_INET,SOCK_DGRAM,0);/*建立套接字*/
 40         if(sock_fd < 0){
 41                 perror("socket()");
 42                 exit(1);
 43         } else {
 44                 printf("socket() sucessful\n");
 45         }
 46         memset(&local_addr,0,sizeof(local_addr));
 47         local_addr.sin_family = AF_INET;
 48         local_addr.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr(LOCAL_ADDR);htonl(INADDR_ANY);固定为local_addr则接受不到
 49         local_addr.sin_port = htons(LOCAL_PORT);
 50         err = bind(sock_fd,(struct sockaddr *)&local_addr,sizeof(local_addr));
 51         if(err < 0){
 52                 perror("bind");
 53                 exit(1);
 54         } else {
 55                 printf("bind sucessful\n");
 56         }
 57         loop = 1;
 58         err = setsockopt(sock_fd,IPPROTO_IP,IP_MULTICAST_LOOP,&loop,sizeof(loop));
 59         if(err < 0){
 60                 perror("setsocket():IP MULTICAST_LOOP");
 61                 exit(1);
 62         } else {
 63                 printf("IP_MULTICAST_LOOP SUCESSFUL\n");
 64         }
 65 
 66         struct ip_mreq mreq;
 67         mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);
 68         mreq.imr_interface.s_addr = inet_addr(LOCAL_ADDR);
 69         err = setsockopt(sock_fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));//
 70         if(err < 0){
 71                 printf("%s setsockopt():IP ADD MEMBURSHIP\n",strerror(errno));
 72                 exit(1);
 73         } else {
 74                 printf("setsockopt() IP ADD MEMBURSHIP sucessful\n");
 75         }
 76         int  times = 0;
 77         int  addr_len = 0;
 78         char buff[1000];
 79         int  recv_num  =0;
 80         for(times = 0;times < 5;times++){
 81                 addr_len = sizeof(local_addr);
 82                 memset(buff,0,sizeof(buff));
 83                 recv_num = recvfrom(sock_fd,buff,sizeof(buff),0,(struct sockaddr*)&local_addr,&addr_len);
 84                 if(recv_num < 0){
 85                         perror("recvfrom()");
 86                         exit(1);
 87                 } else {
 88                         printf("Recv %dst message from server :%s\n",times,buff);
 89                         sleep(MCAST_INTERVAL);
 90                 }
 91         }
 92         err = setsockopt(sock_fd,IPPROTO_IP,IP_DROP_MEMBERSHIP,&mreq,sizeof(mreq));
 93         close(sock_fd);
 94         return 0;
 95 }
