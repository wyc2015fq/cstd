# Linux中ifreq 结构体分析和使用 - DoubleLi - 博客园






**结构原型:**

struct ifreq
{
#define IFHWADDRLEN 6
 union
 {
  char ifrn_name[IFNAMSIZ];  
 } ifr_ifrn;

 union {
  struct sockaddr ifru_addr;
  struct sockaddr ifru_dstaddr;
  struct sockaddr ifru_broadaddr;
  struct sockaddr ifru_netmask;
  struct  sockaddr ifru_hwaddr;
  short ifru_flags;
  int ifru_ivalue;
  int ifru_mtu;
  struct  ifmap ifru_map;
  char ifru_slave[IFNAMSIZ]; 
  char ifru_newname[IFNAMSIZ];
  void __user * ifru_data;
  struct if_settings ifru_settings;
 } ifr_ifru;
};

#define ifr_name ifr_ifrn.ifrn_name 
#define ifr_hwaddr ifr_ifru.ifru_hwaddr 
#define ifr_addr ifr_ifru.ifru_addr 
#define ifr_dstaddr ifr_ifru.ifru_dstaddr 
#define ifr_broadaddr ifr_ifru.ifru_broadaddr 
#define ifr_netmask ifr_ifru.ifru_netmask 
#define ifr_flags ifr_ifru.ifru_flags 
#define ifr_metric ifr_ifru.ifru_ivalue 
#define ifr_mtu  ifr_ifru.ifru_mtu 
#define ifr_map  ifr_ifru.ifru_map 
#define ifr_slave ifr_ifru.ifru_slave 
#define ifr_data ifr_ifru.ifru_data 
#define ifr_ifindex ifr_ifru.ifru_ivalue 
#define ifr_bandwidth ifr_ifru.ifru_ivalue    
#define ifr_qlen ifr_ifru.ifru_ivalue 
#define ifr_newname ifr_ifru.ifru_newname 
#define ifr_settings ifr_ifru.ifru_settings 



**基本介绍: **

ifreq结构定义在/usr/include/net/if.h，用来配置ip地址，激活接口，配置MTU等接口信息的。其中包含了一个接口的名 字和具体内容——（是个共用体，有可能是IP地址，广播地址，子网掩码，MAC号，MTU或其他内容）。ifreq包含在ifconf结构中。而 ifconf结构通常是用来保存所有接口的信息的。



**举例说明：**

在Linux系统中，ifconfig命令是通过ioctl接口与内核通信，例如，当系统管理员输入如下命令来改变接口eth0的MTU大小：

    ifconfig eth0 mtu 1250

ifconfig命令首先打开一个socket，然后通过系统管理员输入的参数初始化一个数据结构，并通过ioctl调用将数据传送到内核。SIOCSIFMTU是命令标识符。

    struct ifreq data;
    fd = socket(PF_INET, SOCK_DGRAM, 0);
    < ... initialize "data" ...>
    err = ioctl(fd, SIOCSIFMTU, &data);



来源：http://blog.csdn.net/zhu114wei/article/details/6927513









