# struct ifreq结构体与ip,子网掩码，网关等信息 - DoubleLi - 博客园






总结一下，今天学习的关于通过socket,ioctl来获得ip,netmask等信息，其中很多内容参照了很多网上的信息，我会一一列出的

我用的这个函数，就是下面这个函数，其中的有一些全局变量，很好懂，也就不多做解释了
一。下面对这个函数进行注解一下：

int get_nic_IP_Address()//获取各网卡IP地址、子网掩码
{
 struct ifreq ifreq;  //声明一个struct ifreq结构体（这个结构体中有很多重要的参数，具体可以参照第二的补充）
   int sock;
 int i;
 int tmpint;
 read_dev(); //这个函数的功能是获得网卡名字（保存在下面提到的sys_nic_ip[][]数组中）并计算网卡总数（就是下面的sys_nic_count）

 for (i=0;i<sys_nic_count;i++)
 {
     if((sock=socket(AF_INET,SOCK_STREAM,0))<0){  //建立一个套接字
         perror("socket");
         return ;
        }
     strcpy(ifreq.ifr_name,sys_nic_name[i]);   //把网卡名字复制到ifreq结构体中的name变量（感觉这个地方是必须的）
     if(ioctl(sock,SIOCGIFADDR,&ifreq)<0) {   //这里涉及ioctl函数对于网络文件的控制（下面会介绍）
       sprintf(sys_nic_ip[i],"Not set");
     } else {
       sprintf(sys_nic_ip[i],"%d.%d.%d.%d",      //把ip地址提取出来，保存（理解一下socketaddr_in和socketaddr的关系）
       (unsigned char)ifreq.ifr_addr.sa_data[2],
       (unsigned char)ifreq.ifr_addr.sa_data[3],
       (unsigned char)ifreq.ifr_addr.sa_data[4],
       (unsigned char)ifreq.ifr_addr.sa_data[5]);
     }
     if(ioctl(sock,SIOCGIFNETMASK,&ifreq)<0) {  //我的理解是这个地方用SIOCGIFNETMASK，那么ifreq中原本是存的ip地址，现在存成了子网掩码了。。
       sprintf(sys_nic_mask[i],"Not set");       //把子网掩码提取出来（但得到的只是超网的划分方式就是/xx）
     } else {
       sprintf(sys_nic_mask[i],"%d",
       Count((unsigned char)ifreq.ifr_netmask.sa_data[2])+
       Count((unsigned char)ifreq.ifr_netmask.sa_data[3])+
       Count((unsigned char)ifreq.ifr_netmask.sa_data[4])+
       Count((unsigned char)ifreq.ifr_netmask.sa_data[5]));

     }
 }
}
列出上面最后调用函数（Count()）和一些全副变量：
char sys_nic_ip[20][20];//各网卡IP
char sys_nic_mask[20][20];//各网卡子网掩码"/xx"

int countTable[256] =
{
0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};
int Count(int v)
{ 
  return countTable[v]; 
}
应该理解了吧。。。挺经典的。。。不过网上的貌似就有一个版本。。。很是气恼

二。对涉及的知识点进行补充

1.struct ifreq {
    char ifr_name[IFNAMSIZ];
     union
      {
        struct sockaddr ifru_addr;
        struct sockaddr ifru_dstaddr;
        struct sockaddr ifru_broadaddr;
        struct sockaddr ifru_netmask;
        struct sockaddr ifru_hwaddr;
        short int ifru_flags;
        int ifru_ivalue;
        int ifru_mtu;
        struct ifmap ifru_map;
        char ifru_slave[IFNAMSIZ]; /* Just fits the size */
        char ifru_newname[IFNAMSIZ];
        __caddr_t ifru_data;
      } ifr_ifru;
};

# define ifr_name ifr_ifrn.ifrn_name /* interface name */
# define ifr_hwaddr ifr_ifru.ifru_hwaddr /* MAC address */
# define ifr_addr ifr_ifru.ifru_addr /* address */
# define ifr_dstaddr ifr_ifru.ifru_dstaddr /* other end of p-p lnk */
# define ifr_broadaddr ifr_ifru.ifru_broadaddr /* broadcast address */
# define ifr_netmask ifr_ifru.ifru_netmask /* interface net mask */
# define ifr_flags ifr_ifru.ifru_flags /* flags */
# define ifr_metric ifr_ifru.ifru_ivalue /* metric */
# define ifr_mtu ifr_ifru.ifru_mtu /* mtu */
# define ifr_map ifr_ifru.ifru_map /* device map */
# define ifr_slave ifr_ifru.ifru_slave /* slave device */
# define ifr_data ifr_ifru.ifru_data /* for use by interface */
# define ifr_ifindex ifr_ifru.ifru_ivalue /* interface index */
# define ifr_bandwidth ifr_ifru.ifru_ivalue /* link bandwidth */
# define ifr_qlen ifr_ifru.ifru_ivalue /* queue length */
# define ifr_newname ifr_ifru.ifru_newname /* New name */
# define _IOT_ifreq _IOT(_IOTS(char),IFNAMSIZ,_IOTS(char),16,0,0)
# define _IOT_ifreq_short _IOT(_IOTS(char),IFNAMSIZ,_IOTS(short),1,0,0)
# define _IOT_ifreq_int _IOT(_IOTS(char),IFNAMSIZ,_IOTS(int),1,0,0)


2.ioctl 函数 (在网络中的作用)
关于这个网络相关的请求，就是ioctl在这里面起的作用和各个参数的作用。。。可以参照这个网页,讲解的很详细：
[http://www.iteye.com/topic/309442](http://www.iteye.com/topic/309442)
本例中用的2个ioctl控制函数。。上面已经解释很清楚了

3.关于socketaddr_in和socketaddr的关系，下面贴出具体的定义：
struct sockaddr_in 
{ 
short int sin_family; /* 地址族 */ 
unsigned short int sin_port; /* 端口号 */ 
struct in_addr sin_addr; /* IP地址 */ 
unsigned char sin_zero[8]; /* 填充0 以保持与struct sockaddr同样大小 */ 
};

struct sockaddr 
{ 
unsigned short sa_family; /* 地址族， AF_xxx */ 
char sa_data[14]; /* 14 字节的协议地址 */ 
};

比较一下，会发现长度一样，所以这2个可以通用的，不过要进行类型转换，比较一下就得出了为什么上面程序中可以用：
(unsigned char)ifreq.ifr_addr.sa_data[2],这种形式了，还是解释一下吧：这个ifr_addr是一个struct sockaddr结构体。它其中的sa_date[2]是不是照着上面sockaddr_in中的sin_add(也就是ip地址呢)，该明白了吧。。。。

总结：通过这个函数，可以很好的理解怎么得到ip和子网掩码的过程。。。。









