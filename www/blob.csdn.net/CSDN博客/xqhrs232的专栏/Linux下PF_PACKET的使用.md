# Linux下PF_PACKET的使用 - xqhrs232的专栏 - CSDN博客
2018年04月11日 14:04:07[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：109
原文地址::[http://blog.sina.com.cn/s/blog_82f2fc28010132og.html](http://blog.sina.com.cn/s/blog_82f2fc28010132og.html)
相关文章
1、AF_INET和AF_PACKET区别----[https://blog.csdn.net/an_zhenwei/article/details/16861805](https://blog.csdn.net/an_zhenwei/article/details/16861805)
2、[TCP/IP协议（一）网络基础知识](http://www.cnblogs.com/imyalost/p/6086808.html)----[https://www.cnblogs.com/imyalost/p/6086808.html](https://www.cnblogs.com/imyalost/p/6086808.html)
sock_raw（注意一定要在root下使用）原始套接字编程可以接收到本机网卡上的数据帧或者数据包,对于监听网络的流量和分析是很有作用的.一共可以有3种方式创建这种socket
1.socket(AF_INET, SOCK_RAW, IPPROTO_TCP|IPPROTO_UDP|IPPROTO_ICMP)发送接收ip数据包，不能用IPPROTO_IP，因为如果是用了IPPROTO_IP，系统根本就不知道该用什么协议。
2.socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP|ETH_P_ARP|ETH_P_ALL))发送接收以太网数据帧
3.socket(AF_INET, SOCK_PACKET, htons(ETH_P_IP|ETH_P_ARP|ETH_P_ALL))过时了,不要用啊
1.介绍
在linux中提供了PF_PACKET接口可以操作链路层的数据。
2.使用方法
定义一个pf_packet = socket(PF_SOCKET, SOCK_RAW, htons(ETH_P_RARP));
就可以利用函数sendto和recefrom来读取和发送链路层的数据包了（当然，发送ARP包，上面第三个参数要变为 htons(ETH_P_ARP)，或者IP的包为ETH_P_IP，可查看文件/usr/include/linux/if_ether.h文件看到所有支持的协议）。
3.在使用SOCK_RAW, SOCK_DGRAM和SOCK_PACKET的区别
在socket的第一个参数使用PF_PACKET的时候，上述三种socket的类型都可以使用。但是有区别。
(1)使用SOCK_RAW发送的数据必须包含链路层的协议头，接受得到的数据包，包含链路层协议头。而使用SOCK_DGRAM则都不含链路层的协议头。
(2)SOCK_PACKET也是可以使用的，但是已经废弃，以后不保证还能支持，不推荐使用。
(3)在使用SOCK_RAW或SOCK_DGRAM和SOCK_PACKET时，在sendto和recvfrom中使用的地址类型不同，前两者使用sockaddr_ll类型的地址，而后者使用sockaddr类型的地址。
(4)如socket的第一个参数使用PF_INET，第二个参数使用SOCK_RAW，则可以得到原始的IP包。
4.下面的例子是一个简单的rarp协议的server程序和client程序
server程序一开始获得除lo接口以外接口的mac地址，等待rarp request请求的到来，如果请求的是自己的mac地址，则向客户端发送 rarp reply，回送自己的ip地址。应我使用的地方，一台机器的ip地址每次dhcp以后都会变。所以该程序还是有一些用处。
注意：本程序只为演示packet socket的工作原理，所以没有进行任何的错误处理，并假设工作的机器上只有ethernet接口。但是本程序有个缺点，就是两个程序工作在同一台机器上的时候，server无法接收到client的rarp request。请知道的朋友赐教，谢谢！
//File Name : rarp_server.cpp
extern "C"
{
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/if_ether.h>
#include <net/if_arp.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <netpacket/packet.h>
}
#include <cstdlib>
struct arpMsg {
 struct ethhdr ethhdr;   
 u_short htype;   
 u_short ptype;   
 u_char  hlen;   
 u_char  plen;   
 u_short operation;  
 u_char  sHaddr[6];  
 u_char  sInaddr[4];  
 u_char  tHaddr[6];  
 u_char  tInaddr[4];  
 u_char  pad[18];  
};
#define MAC_BCAST_ADDR  (uint8_t *) "\xff\xff\xff\xff\xff\xff"
#define OPT_CODE 0
#define OPT_LEN 1
#define OPT_DATA 2
struct interface_info
{
 char  ifname[64];
 unsigned char ip[4];
 unsigned char mac[6];
};
struct interface_info if_info[10];
int eth_num = 0;
void print_mac(unsigned char * mac_addr)
{
 for (int i =0; i < 6; ++i)
 {
  printf("X", mac_addr[i]);
  if (i != 5) printf(":");
 }
 printf("\n");
}
void print_ip(unsigned char * ip_addr)
{
 for (int i =0; i < 4; ++i)
 {
  printf("%d", ip_addr[i]);
  if (i != 3) printf(".");
 }
 printf("\n");
}
int get_iface_index(int fd, const char* interface_name)
{
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strcpy (ifr.ifr_name, interface_name);
    if (ioctl(fd, SIOCGIFINDEX, &ifr) == -1)
    {
        return (-1);
    }
    return ifr.ifr_ifindex;
}
int get_interfaces()
{
 int  sock;
 int  len = 64;
 int  last_len = 0;
 char  *pBuff = NULL;
 int  interface_num = 0;
 struct ifconf  interface_conf;
 struct ifreq  ifreq1;
 struct sockaddr_in *psockaddr_in = NULL;
 if ( (sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
 {
  perror("Could not create socket for geting interface info");
  exit(1);
 }
 while(1)
 {
  pBuff = (char*)malloc(len);
  interface_conf.ifc_len = len;
  interface_conf.ifc_buf = pBuff;
  if (ioctl(sock, SIOCGIFCONF, &interface_conf) < 0)
  {
   perror("ioctl error");
  }
  else
  {
   if (interface_conf.ifc_len == last_len)
   {
    break;
   }
   else
   {
    last_len = interface_conf.ifc_len;   
   }  
  }
  len += 2*sizeof(struct ifreq);
  free(pBuff);      
 }
 interface_num = last_len / sizeof(struct ifreq);
 for (int i =0; i < interface_num; ++i)
 {
  strcpy(ifreq1.ifr_name, interface_conf.ifc_ifcu.ifcu_req[i].ifr_name);
  if (strcmp(ifreq1.ifr_name, "lo") == 0)
  {
   continue;
  }   
  if (ioctl(sock, SIOCGIFHWADDR, &ifreq1) < 0)
  {
   continue;  
  }   
  memcpy(if_info[eth_num].mac, ifreq1.ifr_hwaddr.sa_data, 6);
  strcpy(if_info[eth_num].ifname, ifreq1.ifr_name);
  psockaddr_in = (struct sockaddr_in*)&interface_conf.ifc_req[i].ifr_addr;
  memcpy(if_info[eth_num].ip, &(psockaddr_in->sin_addr.s_addr), 4);
  printf("Interface name: %s", if_info[eth_num].ifname);
  printf(" ip address: ");
  print_ip(if_info[eth_num].ip);
  printf(" mac address:");
  print_mac(if_info[eth_num].mac);
  eth_num++;
 }
 free(pBuff);
 close(sock);
}
int equal_mac(unsigned char* mac1, unsigned char* mac2)
{
 for (int i =0; i < 6; ++i)
 {
  if (mac1[i] != mac2[i]) return 0;
 }
 return 1;
}
int main()
{
 int timeout = 2;
 int  optval = 1;
 int s;  
 int rv = 1;  
 struct sockaddr_ll addr; 
 struct arpMsg arp;
 struct arpMsg *parp;
 fd_set  fdset;
 struct timeval tm;
 time_t  prevTime;
 u_int32_t  ip;
 u_int32_t  yiaddr;
 struct in_addr my_ip;
 struct in_addr dst_ip;
 char  buff[2000];
 int nLen;
 char szBuffer[4096];
 if ((s = socket (PF_PACKET, SOCK_RAW, htons(ETH_P_RARP))) == -1)
 {
  printf("Could not open raw socket\n");
  return -1;
 }
 if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval)) == -1)
 {
  printf("Could not setsocketopt on raw socket\n");
