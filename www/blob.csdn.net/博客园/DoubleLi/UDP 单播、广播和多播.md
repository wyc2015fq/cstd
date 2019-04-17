# UDP 单播、广播和多播 - DoubleLi - 博客园








**阅读目录(Content)**
- [一、UDP广播](http://www.cnblogs.com/jingliming/p/4477264.html#_label0)
- [二、UDP多播](http://www.cnblogs.com/jingliming/p/4477264.html#_label1)
- [1、多播（组播）的概念](http://www.cnblogs.com/jingliming/p/4477264.html#_lab2_1_0)
- [2、广域网的多播](http://www.cnblogs.com/jingliming/p/4477264.html#_lab2_1_1)
- [三、UDP广播与单播](http://www.cnblogs.com/jingliming/p/4477264.html#_label2)
- [广播与单播的比较](http://www.cnblogs.com/jingliming/p/4477264.html#_lab2_2_0)


     使用UDP协议进行信息的传输之前不需要建议连接。换句话说就是客户端向服务器发送信息，客户端只需要给出服务器的ip地址和端口号，然后将信息封装到一个待发送的报文中并且发送出去。至于服务器端是否存在，或者能否收到该报文，客户端根本不用管。     

     单播用于两个主机之间的端对端通信，广播用于一个主机对整个局域网上所有主机上的数据通信。单播和广播是两个极端，要么对一个主机进行通信，要么对整个局域网上的主机进行通信。实际情况下，经常需要对一组特定的主机进行通信，而不是整个局域网上的所有主机，这就是多播的用途。

　　通常我们讨论的udp的程序都是一对一的单播程序。本章将讨论一对多的服务：广播（broadcast）、多播（multicast）。对于广播，网络中的所有主机都会接收一份数据副本。对于多播，消息只是发送到一个多播地址，网络知识将数据分发给哪些表示想要接收发送到该多播地址的数据的主机。总得来说，只有UDP套接字允许广播或多播。

[回到顶部(go to top)](http://www.cnblogs.com/jingliming/p/4477264.html#_labelTop)

# 一、UDP广播

　　广播UDP与单播UDP的区别就是IP地址不同，广播使用广播地址255.255.255.255，将消息发送到在同一广播网络上的每个主机。值得强调的是：**本地广播信息是不会被路由器转发**。当然这是十分容易理解的，因为如果路由器转发了广播信息，那么势必会引起网络瘫痪。这也是为什么IP协议的设计者故意没有定义互联网范围的广播机制。

广播地址通常用于在网络游戏中处于同一本地网络的玩家之间交流状态信息等。

　　其实广播顾名思义，就是想局域网内所有的人说话，**但是广播还是要指明接收者的端口号的**，因为不可能接受者的所有端口都来收听广播。

**UDP服务端代码：**

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

 1 #include<iostream>
 2 #include<stdio.h>
 3 #include<sys/socket.h>
 4 #include<unistd.h>
 5 #include<sys/types.h>
 6 #include<netdb.h>
 7 #include<netinet/in.h>
 8 #include<arpa/inet.h>
 9 #include<string.h>
10 using namespace std;
11 int main()
12 {
13     setvbuf(stdout,NULL,_IONBF,0);
14     fflush(stdout);
15     int sock=-1;
16     if((sock=socket(AF_INET,SOCK_DGRAM,0))==-1)
17     {
18         cout<<"sock error"<<endl;
19         return -1;
20     }
21     const int opt=-1;
22     int nb=0;
23     nb=setsockopt(sock,SOL_SOCKET,SO_BROADCAST,(char*)&opt,sizeof(opt));//设置套接字类型
24     if(nb==-1)
25     {
26         cout<<"set socket error...\n"<<endl;
27         return -1;
28     }
29     struct sockaddr_in addrto;
30     bzero(&addrto,sizeof(struct sockaddr_in));
31     addrto.sin_family=AF_INET;
32     addrto.sin_addr.s_addr=htonl(INADDR_BROADCAST);//套接字地址为广播地址
33     addrto.sin_port=htons(6000);//套接字广播端口号为6000
34     int nlen=sizeof(addrto);
35     while(1)
36     {
37         sleep(1);
38         char msg[]={"the message broadcast"};
39         int ret=sendto(sock,msg,strlen(msg),0,(sockaddr*)&addrto,nlen);//向广播地址发布消息
40         if(ret<0)
41         {
42             cout<<"send error...\n"<<endl;
43             return -1;
44         }
45         else 
46         {
47             printf("ok\n");
48         }
49     }
50     return 0;
51 }

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

 UDP广播客户端代码：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

 1 #include<iostream>
 2 #include<stdio.h>
 3 #include<sys/socket.h>
 4 #include<unistd.h>
 5 #include<sys/types.h>
 6 #include<netdb.h>
 7 #include<netinet/in.h>
 8 #include<arpa/inet.h>
 9 #include<string.h>
10 
11 
12 using namespace std;
13 int main()
14 {
15         setvbuf(stdout,NULL,_IONBF,0);
16         fflush(stdout);
17         struct sockaddr_in addrto;
18         bzero(&addrto,sizeof(struct sockaddr_in));
19         addrto.sin_family=AF_INET;
20         addrto.sin_addr.s_addr=htonl(INADDR_ANY);
21         addrto.sin_port=htons(6000);
22         socklen_t len=sizeof(addrto);
23         int sock=-1;
24         if((sock=socket(AF_INET,SOCK_DGRAM,0))==-1)
25         {
26                 cout<<"socket error..."<<endl;
27                 return -1;
28         }
29         const int opt=-1;
30         int nb=0;
31         nb=setsockopt(sock,SOL_SOCKET,SO_BROADCAST,(char*)&opt,sizeof(opt));
32         if(nb==-1)
33         {
34                 cout<<"set socket errror..."<<endl;
35                 return -1;
36         }
37         if(bind(sock,(struct sockaddr*)&(addrto),len)==-1)
38         {
39                 cout<<"bind error..."<<endl;
40                 return -1;
41         }
42         char msg[100]={0};
43         while(1)
44         {
45                 int ret=recvfrom(sock,msg,100,0,(struct sockaddr*)&addrto,&len);
46                 if(ret<=0)
47                 {
48                         cout<<"read error..."<<endl;
49                 }
50                 else
51                 {
52                         printf("%s\t",msg);
53                 }
54                 sleep(1);
55         }
56         return 0;
57 }

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

[回到顶部(go to top)](http://www.cnblogs.com/jingliming/p/4477264.html#_labelTop)

# ** 二、UDP多播**



## 1、多播（组播）的概念

　　多播，也称为“组播”，将网络中同一业务类型主机进行了逻辑上的分组，进行数据收发的时候其数据仅仅在同一分组中进行，其他的主机没有加入此分组不能收发对应的数据。

　　在广域网上广播的时候，其中的交换机和路由器只向需要获取数据的主机复制并转发数据。主机可以向路由器请求加入或退出某个组，网络中的路由器和交换机有选择地复制并传输数据，将数据仅仅传输给组内的主机。多播的这种功能，可以一次将数据发送到多个主机，又能保证不影响其他不需要（未加入组）的主机的其他通 信。

相对于传统的一对一的单播，多播具有如下的优点：

　　1、具有同种业务的主机加入同一数据流，共享同一通道，节省了带宽和服务器的优点，具有广播的优点而又没有广播所需要的带宽。

　　2、服务器的总带宽不受客户端带宽的限制。由于组播协议由接收者的需求来确定是否进行数据流的转发，所以服务器端的带宽是常量，与客户端的数量无关。

　　3、与单播一样，多播是允许在广域网即Internet上进行传输的，而广播仅仅在同一局域网上才能进行。

组播的缺点：

　　1、多播与单播相比没有纠错机制，当发生错误的时候难以弥补，但是可以在应用层来实现此种功能。

　　2、多播的网络支持存在缺陷，需要路由器及网络协议栈的支持。

　　3、多播的应用主要有网上视频、网上会议等。



## 2、广域网的多播

　　多播的地址是特定的，D类地址用于多播。D类IP地址就是多播IP地址，即224.0.0.0至239.255.255.255之间的IP地址，并被划分为局部连接多播地址、预留多播地址和管理权限多播地址3类：

　　1、局部多播地址：在224.0.0.0～224.0.0.255之间，这是为路由协议和其他用途保留的地址，路由器并不转发属于此范围的IP包。

　　2、预留多播地址：在224.0.1.0～238.255.255.255之间，可用于全球范围（如Internet）或网络协议。

　　3、管理权限多播地址：在239.0.0.0～239.255.255.255之间，可供组织内部使用，类似于私有IP地址，不能用于Internet，可限制多播范围。

　　多播的程序设计使用setsockopt()函数和getsockopt()函数来实现，组播的选项是IP层的，其选项值和含义参见11.5所示。

　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　表11.5 多播相关的选项

|**getsockopt()/setsockopt()**的选项|含 义|
|----|----|
|IP_MULTICAST_TTL|设置多播组数据的TTL值|
|IP_ADD_MEMBERSHIP|在指定接口上加入组播组|
|IP_DROP_MEMBERSHIP|退出组播组|
|IP_MULTICAST_IF|获取默认接口或设置接口|
|IP_MULTICAST_LOOP|禁止组播数据回送|

## 3、多播程序设计的框架

要进行多播的编程，需要遵从一定的编程框架。多播程序框架主要包含套接字初始化、设置多播超时时间、加入多播组、发送数据、接收数据以及从多播组中离开几个方面。其步骤如下：

（1）建立一个socket。

（2）然后设置多播的参数，例如超时时间TTL、本地回环许可LOOP等。

（3）加入多播组。

（4）发送和接收数据。

（5）从多播组离开。

## 4、多播实现代码

**服务端代码：**

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

 1 #include<iostream>
 2 #include<stdio.h>
 3 #include<sys/socket.h>
 4 #include<netdb.h>
 5 #include<sys/types.h>
 6 #include<arpa/inet.h>
 7 #include<netinet/in.h>
 8 #include<unistd.h>
 9 #include<stdlib.h>
10 #include<string.h>
11 #define MCAST_PORT 8888
12 #define MCAST_ADDR "224.0.0.88"  // 多播地址
13 #define MCAST_DATA "BROADCAST TEST DATA"  // 多播内容
14 #define MCAST_INTERVAL 5  //多播时间间隔
15 using namespace std;
16 
17 int main()
18 {
19         int sock;
20         struct sockaddr_in mcast_addr;
21         sock=socket(AF_INET,SOCK_DGRAM,0);
22         if(sock==-1)
23         {
24                 cout<<"socket error"<<endl;
25                 return -1;
26         }
27         memset(&mcast_addr,0,sizeof(mcast_addr));
28         mcast_addr.sin_family=AF_INET;
29         mcast_addr.sin_addr.s_addr=inet_addr(MCAST_ADDR);
30         mcast_addr.sin_port=htons(MCAST_PORT);
31         while(1)
32         {       //向局部多播地址发送多播内容
33                 int n=sendto(sock,MCAST_DATA,sizeof(MCAST_DATA),0,(struct sockaddr*)&mcast_addr,sizeof(mcast_addr));
34                 if(n<0)
35                 {
36                         cout<<"send error"<<endl;
37                         return -2;
38                 }
39                 else
40                 {
41                         cout<<"send message is going ...."<<endl;
42                 }
43                 sleep(MCAST_INTERVAL);
44 
45         }
46         return 0;
47 }

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

**客户端代码：**

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

 1 #include<iostream>
 2 #include<stdio.h>
 3 #include<stdlib.h>
 4 #include<string.h>
 5 #include<sys/types.h>
 6 #include<unistd.h>
 7 #include<sys/socket.h>
 8 #include<netdb.h>
 9 #include<arpa/inet.h>
10 #include<netinet/in.h>
11 #define MCAST_PORT 8888
12 #define MCAST_ADDR "224.0.0.88" /*一个局部连接多播地址，路由器不进行转发*/
13 #define MCAST_INTERVAL 5  //发送时间间隔
14 #define BUFF_SIZE 256   //接收缓冲区大小
15 using namespace std;
16 int main()
17 {
18         int sock;
19         struct sockaddr_in local_addr;
20         int err=-1;
21         sock=socket(AF_INET,SOCK_DGRAM,0);
22         if(sock==-1)
23         {
24                 cout<<"sock error"<<endl;
25                 return -1;
26         }
27         /*初始化地址*/
28         local_addr.sin_family=AF_INET;
29         local_addr.sin_addr.s_addr=htonl(INADDR_ANY);
30         local_addr.sin_port=htons(MCAST_PORT);
31         /*绑定socket*/
32         err=bind(sock,(struct sockaddr*)&local_addr,sizeof(local_addr));
33         if(err<0)
34         {
35                 cout<<"bind error"<<endl;
36                 return -2;
37         }
38         /*设置回环许可*/
39         int loop=1;
40         err=setsockopt(sock,IPPROTO_IP,IP_MULTICAST_LOOP,&loop,sizeof(loop));
41         if(err<0)
42         {
43                 cout<<"set sock error"<<endl;
44                 return -3;
45         }
46         struct ip_mreq mreq;/*加入广播组*/
47         mreq.imr_multiaddr.s_addr=inet_addr(MCAST_ADDR);//广播地址
48         mreq.imr_interface.s_addr=htonl(INADDR_ANY); //网络接口为默认
49         /*将本机加入广播组*/
50         err=setsockopt(sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
51         if(err<0)
52         {
53                 cout<<"set sock error"<<endl;
54                 return -4;
55         }
56         int times=0;
57         socklen_t addr_len=0;
58         char buff[BUFF_SIZE];
59         int n=0;
60         /*循环接受广播组的消息，5次后退出*/
61         for(times=0;;times++)
62         {
63                 addr_len=sizeof(local_addr);
64                 memset(buff,0,BUFF_SIZE);
65                 n=recvfrom(sock,buff,BUFF_SIZE,0,(struct sockaddr*)&local_addr,&addr_len);
66                 if(n==-1)
67                 {
68                         cout<<"recv error"<<endl;
69                         return -5;
70                 }
71                 /*打印信息*/
72                 printf("RECV %dst message from server : %s\n",times,buff);
73                 sleep(MCAST_INTERVAL);
74         }
75         /*退出广播组*/
76         err=setsockopt(sock,IPPROTO_IP,IP_DROP_MEMBERSHIP,&mreq,sizeof(mreq));
77         close(sock);
78         return 0;
79 }

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

**关于此处bind函数的解析**

　　bind操作首先检查用户指定的端口是否可用，然后为socket的一些成员设置正确的值，并添加到哈希表myudp_hash中。然后，协议栈每次收到UDP数据，就会检查该数据报的源和目的地址，还有源和目的端口，在myudp_hash中找到匹配的socket，把该数据报放入该 socket的接收队列，以备用户读取。在这个程序中，bind操作把socket绑定到地址224.0.0.88:8888上， 该操作产生的直接结果就是，对于socket本身，下列值受影响：
    struct inet_sock{
        .rcv_saddr = 224.0.0.88;
        .saddr = 0.0.0.0;
        .sport = 8888;
        .daddr = 0.0.0.0;
        .dport = 0;
    }
    这五个数据表示，该套接字在发送数据包时，本地使用端口8888，本地可以使用任意一个网络设备接口，发往的目的地址不指定。在接收数据时，只接收发往IP地址224.0.0.88的端口为8888的数据。

**我的疑问？？？**

**为什么要广播方和接受方的端口号相同才能收到广播？我试了在一台linux机子上开两个客户端其中一个和广播方的端口号不同，这个客户端结果收不到广播，哪位网友知道恳请告之。**
    程序中，紧接着bind有一个setsockopt操作，它的作用是将socket加入一个组播组，因为socket要接收组播地址224.0.0.1的数据，它就必须加入该组播组。

[回到顶部(go to top)](http://www.cnblogs.com/jingliming/p/4477264.html#_labelTop)

# 三、UDP广播与单播



## 广播与单播的比较

　　广播和单播的处理过程是不同的，单播的数据只是收发数据的特定主机进行处理，而广播的数据整个局域网都进行处理。

　　例如在一个以太网上有3个主机，主机的配置如表11.4所示。

　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　表11.4 某局域网中主机的配置情况
|主 机|**A**|**B**|**C**|
|----|----|----|----|
|IP地址|192.168.1.150|192.168.1.151|192.168.1.158|
|MAC地址|00:00:00:00:00:01|00:00:00:00:00:02|00:00:00:00:00:03|

　　单播流程：主机A向主机B发送UDP数据报，发送的目的IP为192.168.1.151，端口为 80，目的MAC地址为00:00:00:00:00:02。此数据经过UDP层、IP层，到达数据链路层，数据在整个以太网上传播，在此层中其他主机会 判断目的MAC地址。主机C的MAC地址为00:00:00:00:00:03，与目的MAC地址00:00:00:00:00:02不匹配，数据链路层 不会进行处理，直接丢弃此数据。

　　主机B的MAC地址为00:00:00:00:00:02，与目的MAC地址00:00:00:00:00:02一致，此数据会经过IP层、UDP层，到达接收数据的应用程序。

　　广播的流程：主机A向整个网络发送广播数据，发送的目的IP为192.168.1.255，端口为 80，目的MAC地址为FF:FF:FF:FF:FF:FF。此数据经过UDP层、IP层，到达数据链路层，数据在整个以太网上传播，在此层中其他主机会 判断目的MAC地址。由于目的MAC地址为FF:FF:FF:FF:FF:FF，主机C和主机B会忽略MAC地址的比较（当然，如果协议栈不支持广播，则 仍然比较MAC地址），处理接收到的数据。

　　主机B和主机C的处理过程一致，此数据会经过IP层、UDP层，到达接收数据的应用程序。



no pains ,no gains. 给自己加油，为未来奋斗。









