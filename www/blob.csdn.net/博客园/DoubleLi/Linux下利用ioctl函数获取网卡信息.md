# Linux下利用ioctl函数获取网卡信息 - DoubleLi - 博客园






linux下的ioctl函数原型如下：

#include <sys/ioctl.h>

int ioctl(int handle, int cmd, [int *argc, int argv])

函数成功返回0，失败返回-1.

其相关命令接口如下：


|类别|Request|说明|数据类型|
|----|----|----|----|
|套接口|SIOCATMARKSIOCSPGRPSIOCGPGRP|是否位于带外标记设置套接口的进程ID 或进程组ID获取套接口的进程ID 或进程组ID|intintint|
|文件|FIONBIOFIOASYNCFIONREADFIOSETOWNFIOGETOWN|设置/ 清除非阻塞I/O 标志设置/ 清除信号驱动异步I/O 标志获取接收缓存区中的字节数设置文件的进程ID 或进程组ID获取文件的进程ID 或进程组ID|intintintintint|
|接口|SIOCGIFCONFSIOCSIFADDRSIOCGIFADDRSIOCSIFFLAGSSIOCGIFFLAGSSIOCSIFDSTADDRSIOCGIFDSTADDRSIOCGIFBRDADDRSIOCSIFBRDADDRSIOCGIFNETMASKSIOCSIFNETMASKSIOCGIFMETRICSIOCSIFMETRICSIOCGIFMTUSIOCxxx|获取所有接口的清单设置接口地址获取接口地址设置接口标志获取接口标志设置点到点地址获取点到点地址获取广播地址设置广播地址获取子网掩码设置子网掩码获取接口的测度设置接口的测度获取接口MTU（还有很多取决于系统的实现）|struct ifconfstruct ifreqstruct ifreqstruct ifreqstruct ifreqstruct ifreqstruct ifreqstruct ifreqstruct ifreqstruct ifreqstruct ifreqstruct ifreqstruct ifreqstruct ifreq|
|ARP|SIOCSARPSIOCGARPSIOCDARP|创建/ 修改ARP 表项获取ARP 表项删除ARP 表项|struct arpreqstruct arpreqstruct arpreq|
|路由|SIOCADDRTSIOCDELRT|增加路径删除路径|struct rtentrystruct rtentry|
|||||





在这里我们需要用到的结构体

#include<netinet/in.h>

struct sockaddr_in {

short sin_family; /* Address family */

unsigned short sin_port; /* Port number */

struct [in_addr](http://baike.baidu.com/view/2907631.htm) sin_addr; /* Internet address */

unsigned char sin_zero[8]; /* Same size as struct sockaddr */

};

#include <net/if.h>

struct ifreq
{
#define IFHWADDRLEN 6
union
{
charifrn_name[IFNAMSIZ]; 
} ifr_ifrn;

union {
structsockaddr ifru_addr;
structsockaddr ifru_dstaddr;
structsockaddr ifru_broadaddr;
structsockaddr ifru_netmask;
struct sockaddr ifru_hwaddr;
shortifru_flags;
intifru_ivalue;
intifru_mtu;
struct ifmap ifru_map;
charifru_slave[IFNAMSIZ]; 
charifru_newname[IFNAMSIZ];
void __user * ifru_data;
structif_settings ifru_settings;
} ifr_ifru;
};

#define ifr_name ifr_ifrn.ifrn_name 
#define ifr_hwaddr ifr_ifru.ifru_hwaddr 
#defineifr_addr ifr_ifru.ifru_addr 
#defineifr_dstaddr ifr_ifru.ifru_dstaddr 
#defineifr_broadaddr ifr_ifru.ifru_broadaddr 
#defineifr_netmask ifr_ifru.ifru_netmask
#defineifr_flags ifr_ifru.ifru_flags 
#defineifr_metric ifr_ifru.ifru_ivalue
#defineifr_mtu ifr_ifru.ifru_mtu
#define ifr_map ifr_ifru.ifru_map
#define ifr_slave ifr_ifru.ifru_slave
#defineifr_data ifr_ifru.ifru_data 
#define ifr_ifindex ifr_ifru.ifru_ivalue
#define ifr_bandwidth ifr_ifru.ifru_ivalue 
#define ifr_qlen ifr_ifru.ifru_ivalue
#define ifr_newname ifr_ifru.ifru_newname
#define ifr_settings ifr_ifru.ifru_setting

ioctl函数能获取到IP地址、子网掩码、广播地址、硬件MAC地址等信息，至于网关及路由表比较复杂，在此不讨论。

具体代码如下：（测试通过）

#include <stdio.h>

#include <stdlib.h>

#include <net/if.h>

#include <unistd.h>

#include <sys/ioctl.h>

#include <arpa/inet.h>

#include <sys/stat.h>

#include <sys/types.h>

#include <errno.h>

#include <fcntl.h>

#include <netinet/in.h>

#include <net/route.h>

#include <string.h>

#include <net/if_arp.h>



int main()

{

   struct sockaddr_in *sin;

   struct ifreq ifr;

   FILE *dns;

   FILE *gw;

   char *ip = new char(16);

   char *netmask = new char(16);

   char *broadcast = new char(16);

   //char *ip = (char *)malloc(16);

   char *mac = new  char(32);

   //char *mac = (char *)malloc(32);

   int socket_fd;



       if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){

               perror("socket");

               exit(1);

       }



       memset(&ifr, 0, sizeof(ifr));

       strcpy(ifr.ifr_name, "eth0");

       memset(&sin, 0, sizeof(sin));

//获取IP地址

       if(ioctl(socket_fd, SIOCGIFADDR, &ifr) != -1){

               sin = (struct sockaddr_in *)&ifr.ifr_addr;

               strcpy(ip, inet_ntoa(sin->sin_addr));

               printf("IP address is %s\n", ip);

       }

//获取广播地址



       if(ioctl(socket_fd, SIOCGIFBRDADDR, &ifr) != -1){

               sin = (struct sockaddr_in *)&ifr.ifr_broadaddr;

               strcpy(broadcast, inet_ntoa(sin->sin_addr));

               printf("Broadcast is %s\n", broadcast);

       }

//获取子网掩码



       if(ioctl(socket_fd, SIOCGIFNETMASK, &ifr) != -1){

               sin = (struct sockaddr_in *)&ifr.ifr_broadaddr;

               strcpy(netmask, inet_ntoa(sin->sin_addr));

               printf("Net-mask is %s\n", netmask);

       }

//获取硬件MAC地址



       if(ioctl(socket_fd, SIOCGIFHWADDR, &ifr) != -1){

               sin = (struct sockaddr_in *)&ifr.ifr_netmask;

               sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",

               (unsigned char)ifr.ifr_netmask.sa_data[0],

               (unsigned char)ifr.ifr_netmask.sa_data[1],

               (unsigned char)ifr.ifr_netmask.sa_data[2],

               (unsigned char)ifr.ifr_netmask.sa_data[3],

               (unsigned char)ifr.ifr_netmask.sa_data[4],

               (unsigned char)ifr.ifr_netmask.sa_data[5]);

               printf("Mac address is %s\n", mac);

       }

   return 0;

}



至于获取网关以及DNS，我是通过相关命令获得的。

主要代码如下：

//获取网关，利用route -n 命令可以看到相关的网关。连接标志是‘UG’

if(gw_fd = popen("route -n | grep 'UG'", "r")){

   fread(temp,1,128, gw_fd);

   sscanf(temp, "%*s%s", szNetGate);

   printf("Gateway is %s\n", szNetGate);

}

//获取DNS；一般DNS保存在/etc/reslov.conf文件中。具体获得方法要根据实际情况而定。

我的配置文件中是这样的

root@nill:/home/arm-none-linux# cat /etc/resolv.conf



# Dynamic resolv.conf(5) file for glibc resolver(3) generated by resolvconf(8)

#     DO NOT EDIT THIS FILE BY HAND -- YOUR CHANGES WILL BE OVERWRITTEN

nameserver 202.96.134.133  

上面的202.96.134.133就是我需要获取的主DNS，没有备用DNS

if(dns_fd = popen("cat /etc/reslov.conf | grep 'nameserver'", "r")){

   fread(temp,1,128, gw_fd);

   sscanf(temp, "%*s%s%*s%s", szDNS1,szDNS2);

   printf("DNS1 is %s",szDNS1);

   printf("DNS2is %s", szDNS2);

}



转载地址：[http://5375342.blog.51cto.com/5365342/1209335](http://5375342.blog.51cto.com/5365342/1209335)

相关地址：[http://blog.csdn.net/bailyzheng/article/details/7489656](http://blog.csdn.net/bailyzheng/article/details/7489656)









