# 获得Linux系统中的IP、MAC地址等信息 . - ljx0305的专栏 - CSDN博客
2012年03月31日 10:44:20[ljx0305](https://me.csdn.net/ljx0305)阅读数：778
实际环境和特殊需求往往会将简单问题复杂化，比如计算机IP地址，对于一个连接中socket，可以直接获得本端和对端的IP、端口信息。但在一些特殊场合我们可能需要更多的信息，比如系统中有几块网卡，他们的Mac地址是多少，每块网卡分配了几个IP（一个网卡对应多个IP）等等。
这些信息往往需要通过ifconfig指令来获得，对于程序员来说，在代码中调用外部的shell指令可不是个最佳方案，因为没人能保障不同平台、不同版本的ifconfig指令输出的格式是一致的。本篇文章中将介绍通过ioctl函数实现上述需求。
> 
#include <sys/ioctl.h>
**int ioctl(int fd, int request, … /* void *arg */);**
返回：成功返回0，失败返回-1
ioctl函数的参数只有3个，但却是Unix中少有的几个“家族类”复杂函数，这里摘录一段《Unix网络编程》一书中对ioctl函数的描述：
> 
在传统上ioctl函数是用于那些普遍使用、但不适合归入其他类别的任何特殊的系统接口……网络程序（一般是服务器程序）中ioctl常用于在程序启动时获得主机上所有接口的信息：接口的地址、接口是否支持广播、是否支持多播，等等。
ioctl函数的第一个参数fd，可以表示一个打开的文件（文件句柄）或网络套接字，第二个和第三个参数体现了函数的家族特色，参数二request根据函数功能分类定义了多组宏，而参数三总是一个指针，指针的类型依赖于参数二request。因为ioctl的种类实在太多，这里只列出和本文相关的几个参数定义：
|分类|参数二（宏）|参数三|描述|
|----|----|----|----|
|接口|SIOCGIFCONF|struct ifconf|获得所有接口列表|
||SIOCGIFADDR|struct ifreq|获得接口地址|
||SIOCGIFFLAGS|struct ifreq|获得接口标志|
||SIOCGIFBRDADDR|struct ifreq|获得广播地址|
||SIOCGIFNETMASK|struct ifreq|获得子网掩码|
上表中列出了两个相关的结构体：struct ifconf 和 struct ifreq，要了解ioctl函数的具体运用，首先要了解这两个结构：
- 
/* net/if.h */
- 
struct ifconf
- 
{
- 
int ifc_len;            /* Size of buffer.  */
- 
union
- 
{
- 
        __caddr_t ifcu_buf;
- 
struct ifreq *ifcu_req;
- 
} ifc_ifcu;
- 
};
- 
- 
struct ifreq
- 
{
- 
# define IFHWADDRLEN    6
- 
# define IFNAMSIZ   IF_NAMESIZE
- 
- 
union
- 
{
- 
char ifrn_name[IFNAMSIZ];   /*
 Interface name, e.g. "en0".  */
- 
} ifr_ifrn;
- 
- 
union
- 
{
- 
struct sockaddr ifru_addr;
- 
struct sockaddr ifru_dstaddr;
- 
struct sockaddr ifru_broadaddr;
- 
struct sockaddr ifru_netmask;
- 
struct sockaddr ifru_hwaddr;
- 
shortint ifru_flags;
- 
int ifru_ivalue;
- 
int ifru_mtu;
- 
struct ifmap ifru_map;
- 
char ifru_slave[IFNAMSIZ];  /*
 Just fits the size */
- 
char ifru_newname[IFNAMSIZ];
- 
        __caddr_t ifru_data;
- 
} ifr_ifru;
- 
};
struct ifconf的第二个元素ifc_ifcu是一个联合，是指向struct ifreq结构的地址，通常是一组struct ifreq结构空间（每一个描述一个接口），struct ifconf的第一个元素ifc_len描述了struct ifreq结构空间的大小；结构struct ifreq也有两个元素，第一个元素ifr_ifrn内含一个字符串，用来描述接口的名称，比如“eth0″、”wlan0”等，第二个元素是联合，比较复杂，用来描述套接口的地址结构。
struct ifconf 和 struct ifreq的关系可以参考下图：
![ioctl函数中的struct ifconf 和 struct ifreq结构关系](http://programmerdigest.cn/wp-content/uploads/2010/07/ifconf.png)
ioctl函数中的struct ifconf 和 struct ifreq结构关系
通常运用ioctl函数的第一步是从内核获取系统的所有接口，然后再针对每个接口获取其地址信息。获取所有接口通过SIOCGIFCONF请求来实现：
- 
struct ifconf ifc;  /* ifconf结构 */
- 
struct ifreq ifrs[16];  /*
 ifreq结构数组(这里估计了接口的最大数量16) */
- 
- 
/* 初始化ifconf结构 */
- 
ifc.ifc_len = sizeof(ifrs);
- 
ifc.ifc_buf = (caddr_t) ifrs;
- 
- 
/* 获得接口列表 */
- 
ioctl(fd, SIOCGIFCONF, (char *) &ifc);
获得了接口列表，就可以通过struct ifconf结构中*ifcu_req的指针得到struct ifreq结构数组的地址，通过遍历获得每隔接口的详细地址信息：
- 
[printf](http://www.opengroup.org/onlinepubs/009695399/functions/printf.html)("接口名称：%s**/n**",
 ifrs[n].ifr_name); /*
 接口名称 */
- 
- 
/* 获得IP地址 */
- 
ioctl(fd, SIOCGIFADDR, (char *) &ifrs[n]);
- 
[printf](http://www.opengroup.org/onlinepubs/009695399/functions/printf.html)("IP地址:%s**/n**",
- 
(char*)inet_ntoa(((struct sockaddr_in*)(&ifrs[n].ifr_addr))->sin_addr));
- 
- 
/* 获得子网掩码 */
- 
ioctl(fd, SIOCGIFNETMASK, (char *) &ifrs[n]);
- 
[printf](http://www.opengroup.org/onlinepubs/009695399/functions/printf.html)("子网掩码:%s**/n**",
- 
(char*)inet_ntoa(((struct sockaddr_in*)(&ifrs[n].ifr_addr))->sin_addr));
- 
- 
/* 获得广播地址 */
- 
ioctl(fd, SIOCGIFBRDADDR, (char *) &ifrs[n]);
- 
[printf](http://www.opengroup.org/onlinepubs/009695399/functions/printf.html)("广播地址:%s**/n**",
- 
(char*)inet_ntoa(((struct sockaddr_in*)(&ifrs[n].ifr_addr))->sin_addr));
- 
- 
/* 获得MAC地址 */
- 
ioctl(fd, SIOCGIFHWADDR, (char *) &ifrs[n]);
- 
[printf](http://www.opengroup.org/onlinepubs/009695399/functions/printf.html)("MAC地址:%02x:%02x:%02x:%02x:%02x:%02x**/n**",
- 
(unsignedchar) ifrs[n].ifr_hwaddr.sa_data[0],
- 
(unsignedchar) ifrs[n].ifr_hwaddr.sa_data[1],
- 
(unsignedchar) ifrs[n].ifr_hwaddr.sa_data[2],
- 
(unsignedchar) ifrs[n].ifr_hwaddr.sa_data[3],
- 
(unsignedchar) ifrs[n].ifr_hwaddr.sa_data[4],
- 
(unsignedchar) ifrs[n].ifr_hwaddr.sa_data[5]);
最后，给出一个参考程序代码。
> 
ioctl函数没有纳入POXIS规范，各系统对ioctl的实现也不尽相同，下面的代码在我的Ubuntu10.04 linux上可执行通过，但在其他Unix系统上不一定能够通过编译，例如在Power AIX 5.3上需要将获得MAC地址的那段代码注释掉。
#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#define MAXINTERFACES 16    /* 最大接口数 */
int fd;         /* 套接字 */
int if_len;     /* 接口数量 */
struct ifreq buf[MAXINTERFACES];    /* ifreq结构数组 */
struct ifconf ifc;                  /* ifconf结构 */
int main(int argc,char** argv)
{
    /* 建立IPv4的UDP套接字fd */
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket(AF_INET, SOCK_DGRAM, 0)");
        return -1;
    }
    /* 初始化ifconf结构 */
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t) buf;
    /* 获得接口列表 */
    if (ioctl(fd, SIOCGIFCONF, (char *) &ifc) == -1)
    {
        perror("SIOCGIFCONF ioctl");
        return -1;
    }
    if_len = ifc.ifc_len / sizeof(struct ifreq); /* 接口数量 */
    printf("接口数量:%d\n\n", if_len);
    while (if_len-- > 0) /* 遍历每个接口 */
    {
        printf("接口：%s\n", buf[if_len].ifr_name); /* 接口名称 */
        /* 获得接口标志 */
        if (!(ioctl(fd, SIOCGIFFLAGS, (char *) &buf[if_len])))
        {
            /* 接口状态 */
            if (buf[if_len].ifr_flags & IFF_UP)
            {
                printf("接口状态: UP\n");
            }
            else
            {
                printf("接口状态: DOWN\n");
            }
        }
        else
        {
            char str[256];
            sprintf(str, "SIOCGIFFLAGS ioctl %s", buf[if_len].ifr_name);
            perror(str);
        }
        /* IP地址 */
        if (!(ioctl(fd, SIOCGIFADDR, (char *) &buf[if_len])))
        {
            printf("IP地址:%s\n",
                    (char*)inet_ntoa(((struct sockaddr_in*) (&buf[if_len].ifr_addr))->sin_addr));
        }
        else
        {
            char str[256];
            sprintf(str, "SIOCGIFADDR ioctl %s", buf[if_len].ifr_name);
            perror(str);
        }
        /* 子网掩码 */
        if (!(ioctl(fd, SIOCGIFNETMASK, (char *) &buf[if_len])))
        {
            printf("子网掩码:%s\n",
                    (char*)inet_ntoa(((struct sockaddr_in*) (&buf[if_len].ifr_addr))->sin_addr));
        }
        else
        {
            char str[256];
            sprintf(str, "SIOCGIFADDR ioctl %s", buf[if_len].ifr_name);
            perror(str);
        }
        /* 广播地址 */
        if (!(ioctl(fd, SIOCGIFBRDADDR, (char *) &buf[if_len])))
        {
            printf("广播地址:%s\n",
                    (char*)inet_ntoa(((struct sockaddr_in*) (&buf[if_len].ifr_addr))->sin_addr));
        }
        else
        {
            char str[256];
            sprintf(str, "SIOCGIFADDR ioctl %s", buf[if_len].ifr_name);
            perror(str);
        }
        /*MAC地址 */
        if (!(ioctl(fd, SIOCGIFHWADDR, (char *) &buf[if_len])))
        {
            printf("MAC地址:%02x:%02x:%02x:%02x:%02x:%02x\n\n",
                    (unsigned char) buf[if_len].ifr_hwaddr.sa_data[0],
                    (unsigned char) buf[if_len].ifr_hwaddr.sa_data[1],
                    (unsigned char) buf[if_len].ifr_hwaddr.sa_data[2],
                    (unsigned char) buf[if_len].ifr_hwaddr.sa_data[3],
                    (unsigned char) buf[if_len].ifr_hwaddr.sa_data[4],
                    (unsigned char) buf[if_len].ifr_hwaddr.sa_data[5]);
        }
        else
        {
            char str[256];
            sprintf(str, "SIOCGIFHWADDR ioctl %s", buf[if_len].ifr_name);
            perror(str);
        }
    }//–while end
    //关闭socket
    close(fd);
    return 0;
}
在我的系统上，程序输出：
> 
接口数量:4
接口：wlan0
接口状态: UP
IP地址:192.168.1.142
子网掩码:255.255.255.0
广播地址:192.168.1.255
MAC地址:00:14:a5:65:47:57
接口：eth0:0
接口状态: UP
IP地址:192.168.4.113
子网掩码:255.255.255.0
广播地址:192.168.4.255
MAC地址:00:14:c2:e5:45:57
接口：eth0
接口状态: UP
IP地址:192.168.4.111
子网掩码:255.255.255.0
广播地址:192.168.4.255
MAC地址:00:14:c2:e5:45:57
接口：lo
接口状态: UP
IP地址:127.0.0.1
子网掩码:255.0.0.0
广播地址:0.0.0.0
MAC地址:00:00:00:00:00:00
从输出可以看出，系统有4个接口，”wlan0″表示第一块无线网卡接口，”eth0″（IP地址:192.168.4.111）表示第一块连线网卡接口（我们最长用的RJ45连接口网卡），”lo”是回路地址接口（我们常用的127.0.0.1）。
注意：”eth0:0″（IP地址:192.168.4.113）是有线网卡的别名（单网卡绑定多个IP），这是为了测试这个参考程序特意在eth0上添加的一个IP地址。
转载自:[http://blog.csdn.net/tianmo2010/article/details/6834450](http://blog.csdn.net/tianmo2010/article/details/6834450)
