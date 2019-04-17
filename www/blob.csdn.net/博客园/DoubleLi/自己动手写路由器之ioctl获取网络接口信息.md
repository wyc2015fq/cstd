# 自己动手写路由器之ioctl获取网络接口信息 - DoubleLi - 博客园






最近打算写一个简单路由器，里面有用到ioctl获取网络接口信息，那就先把这部分单独拿出来说一说吧！

ioctl这个函数，可以用来对特殊文件的基础设备参数进行操作，它们可以完成与打开文件描述符相关联的控制功能。在程序启动时获得主机上所有接口的信息： 接口的地址、接口是否支持广播、是否支持多播等等。成功返回0，出错则返回-1。

本文里只是单独对获取网络接口信息部分讲解。

先列出一些本文里ioctl的相关ioctl 请求的request 参数以及arg 地址必须指向的数据类型：
|**request参数**|** 相关说明**|**指向数据类型**|
|----|----|----|
|SIOCGIFCONF|获取所有接口信息|struct ifconf|
|SIOCGIFADDR|获取接口地址|struct ifreq|
|SIOCGIFFLAGS|获取接口状态|struct ifreq|
|SIOCGIFBRDADDR|获取广播地址|struct ifreq|
|SIOCGIFNETMASK|获取子网掩码|struct ifreq|

接下来，就开始利用ioctl以及其参数来获取网络接口的信息了。

直接附上代码，详情请看注释：



```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

 1 int main()
 2 {//想要获取当前网口网线插入状态，需要用到ifreq结构体，获取网卡的信息，然后socket结合网卡驱动的ioctl，就可以得到与网线插入状态相关的数据。
 3     int number;                 
 4     char *tmpbuf;               
 5     struct ifconf ifc;          //用来保存所有接口信息的   
 6     struct ifreq buf[16];       //这个结构定义在net/if.h，用来配置ip地址，激活接口，配置MTU等接口信息
 7     ifc.ifc_len = sizeof(buf);
 8     ifc.ifc_buf = (caddr_t)buf;
 9 
10     int sockfd;
11     if(-1 == socket(AF_PACKET,SOCK_RAW,htons(ETH_P_ALL)))     
12     {
13         perror("socket build !");
14         return 1;
15     } 
16     if(-1 == ioctl(sockfd,SIOCGIFCONF,(char *)&ifc))//SIOCGIFCONF用来获取所有配置接口的信息，将所获取的信息保存到ifc里。
17     {
18         perror("SIOCGIFCONF !");
19         return 1;
20     }
21     
22     number = ifc.ifc_len / sizeof(struct ifreq);
23     printf("the interface number is %d \n",number);
24     int tmp;
25     for(tmp = number;tmp > 0;tmp--)
26     {
27         printf("the interface name is %s\n",buf[tmp].ifr_name);
28 
29         /* 接口的状态信息获取 */
30         if(0 == ioctl(sockfd,SIOCGIFFLAGS,(char *)&buf[tmp]))
31         { 
32             if(IFF_UP == buf[tmp].ifr_flags)
33                 printf("UP\n");
34             else 
35                 printf("DOWN");
36         } 
37 
38         /* IP地址的获取 */
39         if(0 == ioctl(sockfd,SIOCGIFADDR,(char *)&buf[tmp]))
40         {
41             tmpbuf = (char *)inet_ntoa(((struct sockaddr_in*) (&buf[tmp].ifr_addr))->sin_addr);
42             printf("IPAdress :%s \n",tmpbuf);
43         }   
44 
45         /* 子网掩码的获取 */
46         if(0 == ioctl(sockfd,SIOCGIFNETMASK,(char *)&buf[tmp]))   
47         {
48             tmpbuf = (char *)inet_ntoa(((struct sockaddr_in*) (&buf[tmp].ifr_addr))->sin_addr);
49             printf("netmask:%s \n",tmpbuf);
50         }  
51 
52         /* 广播地址的获取 */
53         if(0 == ioctl(sockfd,SIOCGIFBRDADDR,(char *)&buf[tmp]))
54         {
55             tmpbuf = (char *)inet_ntoa(((struct sockaddr_in*)(&buf[tmp].ifr_addr))->sin_addr);
56             printf("broadcast address:%s \n",tmpbuf);
57         } 
58 
59         /* MAC地址的获取 */
60         if(0 == ioctl(sockfd,SIOCGIFHWADDR,(char *)&buf[tmp]))
61         {
62  printf("MAC:%02x:%02x:%02x:%02x:%02x:%02x\n\n",
63                     (unsigned char) buf[tmp].ifr_hwaddr.sa_data[0],
64                     (unsigned char) buf[tmp].ifr_hwaddr.sa_data[1],
65                     (unsigned char) buf[tmp].ifr_hwaddr.sa_data[2],
66                     (unsigned char) buf[tmp].ifr_hwaddr.sa_data[3],
67                     (unsigned char) buf[tmp].ifr_hwaddr.sa_data[4],
68                     (unsigned char) buf[tmp].ifr_hwaddr.sa_data[5]);
69         } 
70     }
71 }

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```





在驱动程序中实现的ioctl函数体内，实际上是有一个switch{case}结构的，每一个case对应一个命令码，做出一些相应的操作。

下面附上ioctl全参数多对应的命令码的功能的链接：

http://riverzhou2000.blog.163.com/blog/static/105403248201183025729615/

ioctl这个函数还有很多其它的功能，有兴趣的朋友可以问度娘，这里就不加详述了！









