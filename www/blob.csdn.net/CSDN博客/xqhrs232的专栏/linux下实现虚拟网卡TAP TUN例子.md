# linux下实现虚拟网卡TAP/TUN例子 - xqhrs232的专栏 - CSDN博客
2018年04月10日 16:45:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：829
原文地址::[https://blog.csdn.net/hshl1214/article/details/52936065](https://blog.csdn.net/hshl1214/article/details/52936065)
相关文章
1、linux驱动程序之虚拟以太网设备vmeth----[https://blog.csdn.net/hnhbdss/article/details/1537605](https://blog.csdn.net/hnhbdss/article/details/1537605)
2、**Linux下Tun/Tap设备通信原理----[https://blog.csdn.net/flyforfreedom2008/article/details/46038853](https://blog.csdn.net/flyforfreedom2008/article/details/46038853)**
3、
# [linux虚拟网卡驱动代码](https://blog.csdn.net/fanxiushu/article/details/8525749)----[https://blog.csdn.net/fanxiushu/article/details/8525749](https://blog.csdn.net/fanxiushu/article/details/8525749)
4、Linux-虚拟网络设备-tun/tap----[https://blog.csdn.net/sld880311/article/details/77854651](https://blog.csdn.net/sld880311/article/details/77854651)
5、tun/tap 设备----[https://blog.csdn.net/batmancn/article/details/50668399](https://blog.csdn.net/batmancn/article/details/50668399)
http://blog.csdn.net/xuxinyl/article/details/6454119
我们在使用VMWARE的虚拟化软件时经常会发现它们能都能虚拟出一个网卡,貌似很神奇的技术,其实在Linux下很简单,有两种虚拟设 备,TUN时点对点的设备,tap表示以太网设备的,做为虚拟网卡驱动，Tun/tap驱动程序的数据接收和发送并不直接和真实网卡打交道，而是通 过用户态来转交。在linux下，要实现核心态和用户态数据的交互，有多种方式：可以通用socket创建特殊套接字，利用套接字实现数据交 互；通过proc文件系统创建文件来进行数据交互；还可以使用设备文件的方式，访问设备文件会调用设备驱动相应的例程，设备驱动本身就 是核心态和用户态的一个接口，Tun/tap驱动就是利用设备文件实现用户态和核心态的数据交互。
[cpp][view plain](https://blog.csdn.net/hshl1214/article/details/52936065#)[copy](https://blog.csdn.net/hshl1214/article/details/52936065#)
- #include <unistd.h>
- #include <stdio.h>
- #include <curses.h>
- #include <string.h>
- #include <assert.h>
- #include <sys/types.h>
- #include <sys/socket.h>
- #include <netinet/in.h>
- #include <signal.h>
- #include <unistd.h>
- #include <linux/if_tun.h>
- #include <netinet/in.h>
- #include <sys/ioctl.h>
- #include <sys/time.h>
- #include <linux/if.h>
- #include <netinet/in.h>
- #include <arpa/inet.h>
- #include <errno.h>
- #include <fcntl.h>  
- int tun_creat(char *dev,int flags)  
- {  
- struct ifreq ifr;  
- int fd,err;  
-  assert(dev != NULL);  
- if((fd = open ("/dev/net/tun",O_RDWR))<0) //you can replace it to tap to create tap device.
- return fd;  
-  memset(&ifr,0,sizeof (ifr));  
-  ifr.ifr_flags|=flags;  
- if(*dev != '/0')  
-   strncpy(ifr.ifr_name,dev,IFNAMSIZ);  
- if((err = ioctl(fd,TUNSETIFF,(void *)&ifr))<0)  
-  {  
-   close (fd);  
- return err;  
-  }  
-  strcpy(dev,ifr.ifr_name);  
- return fd;  
- }  
- 
- int main()  
- {  
- int tun,ret;  
- char tun_name[IFNAMSIZ];  
-  unsigned char buf[4096];  
-  tun_name[0]='/0';  
-  tun = tun_creat(tun_name,IFF_TAP|IFF_NO_PI);//如果需要配置tun设备，则把"IFF_TAP"改成“IFF_TUN”
- if(tun<0)  
-  {  
-   perror("tun_create");  
- return 1;  
-  }  
-  printf("TUN name is %s/n",tun_name);  
- while (1) {  
-                 unsigned char ip[4];  
- 
-                  ret = read(tun, buf, sizeof(buf));  
- if (ret < 0)  
- break;  
-                 memcpy(ip, &buf[12], 4);  
-                 memcpy(&buf[12], &buf[16], 4);  
-                 memcpy(&buf[16], ip, 4);  
-                  buf[20] = 0;  
-                 *((unsigned short*)&buf[22]) += 8;  
-                 printf("read %d bytes/n", ret);  
-                  ret = write(tun, buf, ret);  
-                 printf("write %d bytes/n", ret);  
-         }  
- return 0;  
- }  
另开启一个终端
路由配置：
ifconfig devname 10.0.0.1 up;    //10.0.0.1是本虚拟网卡的IP地址,uP是激活该网卡
route add -net 10.0.0.2 netmask 255.255.255.255 dev devname
ping 10.0.0.2
开始测试
