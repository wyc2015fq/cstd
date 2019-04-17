# linux编程获取本机网络相关参数 - DoubleLi - 博客园








### [getifaddrs()和struct ifaddrs的使用，获取本机IP](http://canlynet.iteye.com/blog/1440054)
**博客分类：**- [Linux C编程](http://canlynet.iteye.com/category/92553)





    ifaddrs结构体定义如下：



C代码  ![收藏代码](http://canlynet.iteye.com/images/icon_star.png)

- struct ifaddrs   
- {   
- struct ifaddrs  *ifa_next;    /* Next item in list */   
- char            *ifa_name;    /* Name of interface */   
-     unsigned int     ifa_flags;   /* Flags from SIOCGIFFLAGS */   
- struct sockaddr *ifa_addr;    /* Address of interface */   
- struct sockaddr *ifa_netmask; /* Netmask of interface */   
- union   
-     {   
- struct sockaddr *ifu_broadaddr; /* Broadcast address of interface */   
- struct sockaddr *ifu_dstaddr; /* Point-to-point destination address */   
-     } ifa_ifu;   
-     #define              ifa_broadaddr ifa_ifu.ifu_broadaddr   
-     #define              ifa_dstaddr   ifa_ifu.ifu_dstaddr   
- void            *ifa_data;    /* Address-specific data */   
- };   


    ifa_next指向链表的下一个成员；ifa_name是接口名称，以0结尾的字符串，比如eth0，lo；ifa_flags是接口的标识位（比如当IFF_BROADCAST或IFF_POINTOPOINT设置到此标识位时，影响联合体变量ifu_broadaddr存储广播地址或ifu_dstaddr记录点对点地址）；ifa_netmask存储该接口的子网掩码；结构体变量存储广播地址或点对点地址（见括弧介绍ifa_flags）；ifa_data存储了该接口协议族的特殊信息，它通常是NULL（一般不关注他）。

    函数getifaddrs（int getifaddrs (struct ifaddrs **__ifap)）获取本地网络接口信息，将之存储于链表中，链表头结点指针存储于__ifap中带回，函数执行成功返回0，失败返回-1，且为errno赋值。
    很显然，函数getifaddrs用于获取本机接口信息，比如最典型的获取本机IP地址。




### [linux编程获取本机IP地址的三种方法](http://blog.csdn.net/jcwkyl/article/details/6324147)


分类： [C/C++](http://blog.csdn.net/jcwKyl/article/category/377213)[Linux Tech](http://blog.csdn.net/jcwKyl/article/category/469832)2011-04-14 20:33 861人阅读 [评论](http://blog.csdn.net/jcwkyl/article/details/6324147#comments)(0) [收藏](http://blog.csdn.net/jcwkyl/article/details/6324147)[举报](http://blog.csdn.net/jcwkyl/article/details/6324147#report)


这 是一项不太清晰而且没有多大意义的工作。一个原因是网络地址的设置非常灵活而且都是允许用户进行个性化设置的，比如一台计算机上可以有多块物理网卡或者虚 拟网卡，一个网卡上可以绑定多个IP地址，用户可以为网卡设置别名，可以重命名网卡，用户计算机所在网络拓扑结构未知，主机名设置是一个可选项并且同样可 以为一个计算机绑定多个主机名等，这些信息都会有影响。脱离了网络连接，单独的网络地址没有任何意义。编程中遇到必须获取计算机IP的场景，应该考虑将这 一选项放到配置文件中，由用户自己来选择。

通过google，编程获取IP地址大约有以下三种思路：
1. 通过gethostname()和gethostbyname()

#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    char hname[128];
    struct hostent *hent;
    int i;

    gethostname(hname, sizeof(hname));

    //hent = gethostent();
    hent = gethostbyname(hname);

    printf("hostname: %s/naddress list: ", hent->h_name);
    for(i = 0; hent->h_addr_list[i]; i++) {
        printf("%s/t", inet_ntoa(*(struct in_addr*)(hent->h_addr_list[i])));
    }
    return 0;
}

运行：
[whb@jcwkyl c]$ ./local_ip 
hostname: jcwkyl.jlu.edu.cn
address list: 10.60.56.90       


2. 通过枚举网卡，API接口可查看man 7 netdevice

/*代码来自StackOverflow: http://stackoverflow.com/questions/212528/linux-c-get-the-ip-address-of-local-computer */
#include <stdio.h>      
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>

int main (int argc, const char * argv[]) {
    struct ifaddrs * ifAddrStruct=NULL;
    void * tmpAddrPtr=NULL;

    getifaddrs(&ifAddrStruct);

    while (ifAddrStruct!=NULL) {
        if (ifAddrStruct->ifa_addr->sa_family==AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            char addressBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            printf("%s IP Address %s/n", ifAddrStruct->ifa_name, addressBuffer); 
        } else if (ifAddrStruct->ifa_addr->sa_family==AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
            char addressBuffer[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
            printf("%s IP Address %s/n", ifAddrStruct->ifa_name, addressBuffer); 
        } 
        ifAddrStruct=ifAddrStruct->ifa_next;
    }
    return 0;
}

运行 ：
[whb@jcwkyl c]$ ./local_ip2 
lo IP Address 127.0.0.1
eth0 IP Address 10.60.56.90
eth0:1 IP Address 192.168.1.3
lo IP Address ::
eth0 IP Address ::2001:da8:b000:6213:20f:1fff
eth0 IP Address 0:0:fe80::20f:1fff

3. 打开一个对外界服务器的网络连接，通过getsockname()反查自己的IP






### [linux下获取IP等信息函数](http://blog.csdn.net/zhangxinrun/article/details/6457642)


分类： [linux内核](http://blog.csdn.net/zhangxinrun/article/category/721099)[Linux系统](http://blog.csdn.net/zhangxinrun/article/category/686868)2011-05-31 18:14 306人阅读 [评论](http://blog.csdn.net/zhangxinrun/article/details/6457642#comments)(0) [收藏](http://blog.csdn.net/zhangxinrun/article/details/6457642)[举报](http://blog.csdn.net/zhangxinrun/article/details/6457642#report)




# 在linux下 获取，修改本机IP地址的两个函数



//获取本机IP地址函数





[view plain](http://blog.csdn.net/davidge333/archive/2010/01/24/5250566.aspx)[copy to clipboard](http://blog.csdn.net/davidge333/archive/2010/01/24/5250566.aspx)[print](http://blog.csdn.net/davidge333/archive/2010/01/24/5250566.aspx)[?](http://blog.csdn.net/davidge333/archive/2010/01/24/5250566.aspx)

- QString GetLocalIp()  
- {  
- 
- int sock_get_ip;  
- char ipaddr[50];  
- 
- struct   sockaddr_in *sin;  
- struct   ifreq ifr_ip;     
- 
- if ((sock_get_ip=socket(AF_INET, SOCK_STREAM, 0)) == -1)  
-     {  
-          printf("socket create failse...GetLocalIp!/n");  
- return "";  
-     }  
- 
-     memset(&ifr_ip, 0, sizeof(ifr_ip));     
-     strncpy(ifr_ip.ifr_name, "eth0", sizeof(ifr_ip.ifr_name) - 1);     
- 
- if( ioctl( sock_get_ip, SIOCGIFADDR, &ifr_ip) < 0 )     
-     {     
- return "";     
-     }       
-     sin = (struct sockaddr_in *)&ifr_ip.ifr_addr;     
-     strcpy(ipaddr,inet_ntoa(sin->sin_addr));         
- 
-     printf("local ip:%s /n",ipaddr);      
-     close( sock_get_ip );  
- 
- return QString( ipaddr );  
- }  






//修改本机IP地址的函数



- int SetLocalIp( const char *ipaddr )  
- {  
- 
- int sock_set_ip;  
- 
- struct sockaddr_in sin_set_ip;  
- struct ifreq ifr_set_ip;  
- 
-     bzero( &ifr_set_ip,sizeof(ifr_set_ip));  
- 
- if( ipaddr == NULL )  
- return -1;  
- 
- if(sock_set_ip = socket( AF_INET, SOCK_STREAM, 0 ) == -1);  
-     {  
-         perror("socket create failse...SetLocalIp!/n");  
- return -1;  
-     }  
- 
-     memset( &sin_set_ip, 0, sizeof(sin_set_ip));  
-     strncpy(ifr_set_ip.ifr_name, "eth0", sizeof(ifr_set_ip.ifr_name)-1);     
- 
-     sin_set_ip.sin_family = AF_INET;  
-     sin_set_ip.sin_addr.s_addr = inet_addr(ipaddr);  
-     memcpy( &ifr_set_ip.ifr_addr, &sin_set_ip, sizeof(sin_set_ip));  
- 
- if( ioctl( sock_set_ip, SIOCSIFADDR, &ifr_set_ip) < 0 )  
-     {  
-         perror( "Not setup interface/n");  
- return -1;  
-     }  
- 
- //设置激活标志  
-     ifr_set_ip.ifr_flags |= IFF_UP |IFF_RUNNING;  
- 
- //get the status of the device  
- if( ioctl( sock_set_ip, SIOCSIFFLAGS, &ifr_set_ip ) < 0 )  
-     {  
-          perror("SIOCSIFFLAGS");  
- return -1;  
-     }  
- 
-     close( sock_set_ip );  
- return 0;  
- }  




# 在linux下 获取本机MAC地址的函数


获取本机MAC地址函数



- QString GetLocalMac()  
- {  
- int sock_mac;  
- 
- struct ifreq ifr_mac;  
- char mac_addr[30];     
- 
-     sock_mac = socket( AF_INET, SOCK_STREAM, 0 );  
- if( sock_mac == -1)  
-     {  
-         perror("create socket falise...mac/n");  
- return "";  
-     }  
- 
-     memset(&ifr_mac,0,sizeof(ifr_mac));     
-     strncpy(ifr_mac.ifr_name, "eth0", sizeof(ifr_mac.ifr_name)-1);     
- 
- if( (ioctl( sock_mac, SIOCGIFHWADDR, &ifr_mac)) < 0)  
-     {  
-         printf("mac ioctl error/n");  
- return "";  
-     }  
- 
-     sprintf(mac_addr,"%02x%02x%02x%02x%02x%02x",  
-             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[0],  
-             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[1],  
-             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[2],  
-             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[3],  
-             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[4],  
-             (unsigned char)ifr_mac.ifr_hwaddr.sa_data[5]);  
- 
-     printf("local mac:%s /n",mac_addr);      
- 
-     close( sock_mac );  
- return QString( mac_addr );  
- }  






# 在linux下 获取，修改子网掩码NETMASK的两个函数



//获取子网掩码的函数

- QString GetLocalNetMask()  
- {  
- int sock_netmask;  
- char netmask_addr[50];  
- 
- struct ifreq ifr_mask;  
- struct sockaddr_in *net_mask;  
- 
-     sock_netmask = socket( AF_INET, SOCK_STREAM, 0 );  
- if( sock_netmask == -1)  
-     {  
-         perror("create socket failture...GetLocalNetMask/n");  
- return "";  
-     }  
- 
-     memset(&ifr_mask, 0, sizeof(ifr_mask));     
-     strncpy(ifr_mask.ifr_name, ifname, sizeof(ifr_mask.ifr_name )-1);     
- 
- if( (ioctl( sock_netmask, SIOCGIFNETMASK, &ifr_mask ) ) < 0 )   
-     {  
-         printf("mac ioctl error/n");  
- return "";  
-     }  
- 
-     net_mask = ( struct sockaddr_in * )&( ifr_mask.ifr_netmask );  
-     strcpy( netmask_addr, inet_ntoa( net_mask -> sin_addr ) );  
- 
-     printf("local netmask:%s/n",netmask_addr);      
- 
-     close( sock_netmask );  
- return QString( netmask_addr );  
- }  




//修改子NETMASK的函数



- QString SetLocalNetMask(const char *szNetMask)  
- {  
- int sock_netmask;  
- char netmask_addr[32];     
- 
- struct ifreq ifr_mask;  
- struct sockaddr_in *sin_net_mask;  
- 
-     sock_netmask = socket( AF_INET, SOCK_STREAM, 0 );  
- if( sock_netmask == -1)  
-     {  
-         perror("Not create network socket connect/n");  
- return "";  
-     }  
- 
-     memset(&ifr_mask, 0, sizeof(ifr_mask));     
-     strncpy(ifr_mask.ifr_name, "eth0", sizeof(ifr_mask.ifr_name )-1);     
-     sin_net_mask = (struct sockaddr_in *)&ifr_mask.ifr_addr;  
-     sin_net_mask -> sin_family = AF_INET;  
-     inet_pton(AF_INET, szNetMask, &sin_net_mask ->sin_addr);  
- 
- if(ioctl(sock_netmask, SIOCSIFNETMASK, &ifr_mask ) < 0)   
-     {  
-         printf("sock_netmask ioctl error/n");  
- return "";  
-     }  
- }  






//获去GateWay



- QString GetGateWay()  
- {  
- FILE *fp;  
- char buf[512];  
- char cmd[128];  
- char gateway[30];  
- char *tmp;  
- 
-     strcpy(cmd, "ip route");  
-     fp = popen(cmd, "r");  
- if(NULL == fp)  
-     {  
-         perror("popen error");  
- return "";  
-     }  
- while(fgets(buf, sizeof(buf), fp) != NULL)  
-     {  
-         tmp =buf;  
- while(*tmp && isspace(*tmp))  
-             ++ tmp;  
- if(strncmp(tmp, "default", strlen("default")) == 0)  
- break;  
-     }  
-     sscanf(buf, "%*s%*s%s", gateway);         
-     printf("default gateway:%s/n", gateway);  
-     pclose(fp);  
- 
- return QString(gateway);  
- }  




//设置网关



- int SetGateWay(const char *szGateWay)  
- {  
- int ret = 0;      
- char cmd[128];  
-     QString DefGW = GetGateWay();  
- 
- const char *strGW = DefGW.latin1();   
- 
-     strcpy(cmd, "route del default gw ");  
-     strcat(cmd, strGW);  
-     ret = system(cmd);  
- if(ret < 0)  
-     {  
-         perror("route error");  
- return -1;  
-     }  
-     strcpy(cmd, "route add default gw ");  
-     strcat(cmd, szGateWay);  
- 
-     ret = system(cmd);  
- if(ret < 0)  
-     {  
-         perror("route error");  
- return -1;  
-     }  
- 
- return ret;  
- }  






### Linux下如何获取网卡信息  

2010-12-17 00:12:26|  分类： [Linux应用](http://xxnull.blog.163.com/blog/#m=0&t=1&c=fks_084066083087085070086094081095085081087064093094087070083)|  标签：[网卡](http://xxnull.blog.163.com/blog/#m=0&t=3&c=网卡)[dns](http://xxnull.blog.163.com/blog/#m=0&t=3&c=dns)[网关](http://xxnull.blog.163.com/blog/#m=0&t=3&c=网关)[mac](http://xxnull.blog.163.com/blog/#m=0&t=3&c=mac)[linux](http://xxnull.blog.163.com/blog/#m=0&t=3&c=linux)  |字号订阅



有时候，写程序的时候需要获取计算机的网络信息，比如IP地址、电脑名称、DNS等信息。IP地址和电脑名称是比较容易获取到的，而要想获取地址掩码、DNS、网关等信息就有些麻烦了。

在Windows下我们一般都是通过从注册表读取这些信息。在Linux怎么做呢？其实，Linux下更加容易一些。因为我们可以拿现成的程序看它的源代码。通过阅读其源代码找到解决该问题的方法。那么，看哪个程序的源代码呢？如果你使用过Linux，并且比较熟悉的话就肯定知道一个命令ifconfig。这个命令和Windows下的ipconfig差不多，都可以输出网卡的信息，其中就包含DNS、掩码等信息。所以，我们可以通过看它的源代码来找到解决该问题的方法。

# 获取系统中的网卡数量
并没有那个系统调用提供网卡数量的获取。但是，我们可以通过强大的proc文件系统获取网卡数量的信息。实际上，ifconfig也是这样做的，请看示例代码如下： 0001 #include <stdio.h>0002 #include <string.h>0003 #include <errno.h>0004 0005 int GetNetCardCount()0006 {0007     int nCount = 0;0008     FILE* f = fopen("/proc/net/dev", "r");0009     if (!f)0010     {0011         fprintf(stderr, "Open /proc/net/dev failed!errno:%d\n", errno);0012         return nCount;0013     }0014 0015     char szLine[512];0016 0017     fgets(szLine, sizeof(szLine), f);    /* eat line */0018     fgets(szLine, sizeof(szLine), f);0019 0020     while(fgets(szLine, sizeof(szLine), f))0021     {0022         char szName[128] = {0};0023         sscanf(szLine, "%s", szName);0024         int nLen = strlen(szName);0025         if (nLen <= 0)continue;0026         if (szName[nLen - 1] == ':') szName[nLen - 1] = 0;0027         if (strcmp(szName, "lo") == 0)continue;0028         nCount++;0029     }0030 0031     fclose(f);0032     f = NULL;0033     return nCount;0034 }0035 0036 int main(int argc, char* argv[])0037 {0038     printf("NetCardCount: %d\n", GetNetCardCount());0039     return 0;0040 }
# 获取IP、掩码、MAC及网关

获取IP、掩码、MAC和广播地址是比较容易的，只需要调用对应的IOCTL即可。只是大家对Linux下的IOCTL可能不太熟悉。却看示例代码：


0001 void DispNetInfo(const char* szDevName)0002 {0003     int s = socket(AF_INET, SOCK_DGRAM, 0);0004     if (s < 0)0005     {0006         fprintf(stderr, "Create socket failed!errno=%d", errno);0007         return;0008     }0009 0010     struct ifreq ifr;0011     unsigned char mac[6];0012     unsigned long nIP, nNetmask, nBroadIP;0013 0014     printf("%s:\n", szDevName);0015 0016     strcpy(ifr.ifr_name, szDevName);0017     if (ioctl(s, SIOCGIFHWADDR, &ifr) < 0)0018     {0019         return;0020     }0021     memcpy(mac, ifr.ifr_hwaddr.sa_data, sizeof(mac));0022     printf("\tMAC: %02x-%02x-%02x-%02x-%02x-%02x\n",0023             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);0024 0025     strcpy(ifr.ifr_name, szDevName);0026     if (ioctl(s, SIOCGIFADDR, &ifr) < 0)0027     {0028         nIP = 0;0029     }0030     else0031     {0032         nIP = *(unsigned long*)&ifr.ifr_broadaddr.sa_data[2];0033     }0034     printf("\tIP: %s\n", inet_ntoa(*(in_addr*)&nIP));0035 0036     strcpy(ifr.ifr_name, szDevName);0037     if (ioctl(s, SIOCGIFBRDADDR, &ifr) < 0)0038     {0039         nBroadIP = 0;0040     }0041     else0042     {0043         nBroadIP = *(unsigned long*)&ifr.ifr_broadaddr.sa_data[2];0044     }0045     printf("\tBroadIP: %s\n", inet_ntoa(*(in_addr*)&nBroadIP));0046 0047     strcpy(ifr.ifr_name, szDevName);0048     if (ioctl(s, SIOCGIFNETMASK, &ifr) < 0)0049     {0050         nNetmask = 0;0051     }0052     else0053     {0054         nNetmask = *(unsigned long*)&ifr.ifr_netmask.sa_data[2];0055     }0056     printf("\tNetmask: %s\n", inet_ntoa(*(in_addr*)&nNetmask));0057     close(s);0058 }


       那么如何获取网关地址呢？更加容易，但是，好像很少有人知道。反正我在网上没有找到有人知道。最后看了nslookup的源代码以后才知道正确的做法。代码如下：

       res_init();      

       for (int i = 0; i < _res.nscount; i++)

       {

              struct sockaddr* server = (struct sockaddr*)&_res.nsaddr_list[i];

              printf("Server:  %s\n", inet_ntoa(*(in_addr*)&(server->sa_data[2])));

       }

代码很简单，就不做解释了。



怎么获取网关呢？这个稍微有点麻烦一些，不过和获取网卡数量相似，都是通过proc文件系统。这次分析的/proc/net/route文件。我就不再贴出示例代码了。

最后，我把运行示例程序获取到的信息附上，以供大家有个直观的认识：

eth0:

       MAC: 08-00-27-98-bf-f3

       IP: 192.168.1.106

       BroadIP: 255.255.255.255

       Netmask: 255.255.255.0

Gateway: 192.168.1.1

eth1:

       MAC: 08-00-27-16-f4-bf

       IP: 192.168.1.108

       BroadIP: 192.168.1.255

       Netmask: 255.255.255.0

Gateway: 0.0.0.0

eth2:

       MAC: 08-00-27-37-9c-91

       IP: 0.0.0.0

       BroadIP: 0.0.0.0

       Netmask: 0.0.0.0

Gateway: 0.0.0.0

eth3:

       MAC: 08-00-27-5a-d2-39

       IP: 0.0.0.0

       BroadIP: 0.0.0.0

       Netmask: 0.0.0.0

Gateway: 0.0.0.0

NetCardCount: 4

DNS 0:  218.2.135.1

DNS 1:  61.147.37.1







Linux下C语言配置网络与获取网络配置信息的方法

2009-01-19 21:06
|Linux下的网络配置包含三个要素，分别是IP地址、子网掩码和网关。本文将介绍如何在C语言中进行网络的配置和配置信息的获取。【配置】方法一使用system()或exec*()调用ifconfig和route命令进行配置。这种方法的优点是使用简单，缺点是效率比较低，且依赖于ifconfig与route命令。示例：见所附代码中的函数ip_config_system()和ip_config_exec()。方法二建立一个socket，用ioctl()进行配置。这种方法的优点是效率较高，缺点是程序实现起来比较麻烦。示例：见所附代码中的函数ip_config_ioctl()。【获取】方法一用popen()建立一个管道，管道的一端执行命令ifconfig和route，管道的另一端读取收到的数据并进行相应的解析。这种方法的优点是使用简单，缺点是效率比较低，且依赖于ifconfig与route命令。示例：见所附代码中的函数ip_get_pipe()。方法二用fopen()打开/proc/net/route，可以获取网关（在/proc/net中尚未发现比较好的获取IP地址和掩码的方法，知道的请发邮件至cugfeng at gamil.com，谢谢）。这种方法的优点是使用简单，效率比执行命令高，缺点是依赖于proc文件系统。示例：见所附代码中的函数ip_get_proc()。方法三建立一个socket，用ioctl()进行获取（用ioctl()尚未发现比较好的获取网关的方法，知道的请发邮件至cugfeng at gamil.com，谢谢）。这种方法的优点是效率较高，缺点是程序实现起来比较麻烦。示例：见所附代码中的函数ip_get_ioctl()。BTW，用ioctl()的方法还可以获取MAC地址，ioctl()命令为SIOCGIFHWADDR，具体用法与ioctl()获取IP地址的方法相同，这里就不多说了。|
|----|





### [LINUX下的getifaddrs()函数的内存释放问题](http://xinzhiwen198941-163-com.iteye.com/blog/994704)

[Linux](http://www.iteye.com/blogs/tag/Linux)[XP](http://www.iteye.com/blogs/tag/XP)[FreeBSD](http://www.iteye.com/blogs/tag/FreeBSD)[数据结构](http://www.iteye.com/blogs/tag/%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84)[PHP](http://www.iteye.com/blogs/tag/PHP)


LINUX下的getifaddrs()函数的内存释放问题 
在LINUX下获取网卡信息需要用到IOCTL或者getifaddrs 
而我在用getifaddrs的时候遇到了内存方面的问题 

先看相关定义: 
========== 

函数定义: 
/* Create a linked list of `struct ifaddrs' structures, one for each 
   network interface on the host machine.  If successful, store the 
   list in *IFAP and return 0.  On errors, return -1 and set `errno'. 

   The storage returned in *IFAP is allocated dynamically and can 
   only be properly freed by passing it to `freeifaddrs'.  */ 
extern int getifaddrs (struct ifaddrs **__ifap) __THROW; 

/* Reclaim the storage allocated by a previous `getifaddrs' call.  */ 
extern void freeifaddrs (struct ifaddrs *__ifa)  __THROW; 
============== 
此函数需要的结构体定义: 

struct ifaddrs 
{ 
  struct ifaddrs *ifa_next;     /* Pointer to the next structure.  */ 

  char *ifa_name;               /* Name of this network interface.  */ 
  unsigned int ifa_flags;       /* Flags as from SIOCGIFFLAGS ioctl.  */ 

  struct sockaddr *ifa_addr;    /* Network address of this interface.  */ 
  struct sockaddr *ifa_netmask; /* Netmask of this interface.  */ 
  union 
  { 
    /* At most one of the following two is valid.  If the IFF_BROADCAST 
       bit is set in `ifa_flags', then `ifa_broadaddr' is valid.  If the 
       IFF_POINTOPOINT bit is set, then `ifa_dstaddr' is valid. 
       It is never the case that both these bits are set at once.  */ 
    struct sockaddr *ifu_broadaddr; /* Broadcast address of this interface. */ 
    struct sockaddr *ifu_dstaddr; /* Point-to-point destination address.  */ 
  } ifa_ifu; 
  /* These very same macros are defined by <net/if.h> for `struct ifaddr'. 
     So if they are defined already, the existing definitions will be fine.  */ 
# ifndef ifa_broadaddr 
#  define ifa_broadaddr ifa_ifu.ifu_broadaddr 
# endif 
# ifndef ifa_dstaddr 
#  define ifa_dstaddr   ifa_ifu.ifu_dstaddr 
# endif 

  void *ifa_data;               /* Address-specific data (may be unused).  */ 
}; 
============= 
我在调用了getifaddrs()之后,正常地完成了需要的工作 
但是最后如果用freeifaddrs,则出现运行时错误 
*** glibc detected *** d: free(): invalid pointer: 0x0804a4d4 *** 
======= Backtrace: ========= 
/lib/libc.so.6[0xb7eda911] 
/lib/libc.so.6(__libc_free+0x84)[0xb7edbf84] 
/lib/libc.so.6(freeifaddrs+0x1d)[0xb7f512dd] 
d[0x8048989] 
d[0x80486a5] 
/lib/libc.so.6(__libc_start_main+0xdc)[0xb7e8c87c] 
d[0x8048491] 
======= Memory map: ======== 
08048000-08049000 r-xp 00000000 03:07 48637 /home/souldump/bin/d 
08049000-0804a000 rw-p 00000000 03:07 48637 /home/souldump/bin/d 
0804a000-0806b000 rw-p 0804a000 00:00 0 [heap] 
b7d00000-b7d21000 rw-p b7d00000 00:00 0 
b7d21000-b7e00000 ---p b7d21000 00:00 0 
b7e76000-b7e77000 rw-p b7e76000 00:00 0 
b7e77000-b7f90000 r-xp 00000000 03:05 16184 /lib/libc-2.4.so 
b7f90000-b7f92000 r--p 00118000 03:05 16184 /lib/libc-2.4.so 
b7f92000-b7f94000 rw-p 0011a000 03:05 16184 /lib/libc-2.4.so 
b7f94000-b7f98000 rw-p b7f94000 00:00 0 
b7fab000-b7fb5000 r-xp 00000000 03:05 20108 /lib/libgcc_s.so.1 
b7fb5000-b7fb6000 rw-p 00009000 03:05 20108 /lib/libgcc_s.so.1 
b7fb6000-b7fb7000 rw-p b7fb6000 00:00 0 
b7fb7000-b7fd1000 r-xp 00000000 03:05 16177 /lib/ld-2.4.so 
b7fd1000-b7fd3000 rw-p 00019000 03:05 16177 /lib/ld-2.4.so 
bfb2b000-bfb41000 rw-p bfb2b000 00:00 0 [stack] 
ffffe000-fffff000 ---p 00000000 00:00 0 [vdso] 

实际上也有人出现相同问题: 
http://p.g.yupoo.com/nph-proxy.cgi/000110A/http/www.linuxdby.com/bbs/viewthread.php=3ftid=3d10756 
此人说:"这说明不是真正的链表，指针非法" 
但是又没有进一步说明怎么解决 
他干脆没有调用freeifaddrs,自然会内存泄漏..... 

我去看了afaddrs.c 
freeifaddrs的定义居然是: 
void 
freeifaddrs (struct ifaddrs *ifa) 
{ 
free (ifa); 
} 
怎么样,很囧吧,明明在头文件里说"必须用freeifaddrs才能正确free..." 
然后我看了一下getifaddrs的函数体 

他在getifaddrs内部定义了一个结构 
struct ifaddrs_storage 
{ 
struct ifaddrs ifa; 
union 
{ 
/* Save space for the biggest of the four used sockaddr types and 
avoid a lot of casts. */ 
struct sockaddr sa; 
struct sockaddr_ll sl; 
struct sockaddr_in s4; 
struct sockaddr_in6 s6; 
} addr, netmask, broadaddr; 
char name[IF_NAMESIZE + 1]; 
}; 

然后把获取的各网卡信息一个个填充到此结构的struct ifaddrs ifa中,ifa的next值手动设置为下一个struct ifaddrs_storage中的ifa的地址... 
这酒是所谓的"伪链表"吧? 
这就是我无法正确free掉它的原因? 
我究竟要怎么把它free掉?freeifaddrs一运行就运行时错误 




LINUX取得本机IP的简单C程序 
注意这里用了两个struct ifaddrs 



//代码根据UNP和man手册编写 
//适用于LINUX/BSD(FreeBSD, MacOS X) 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <time.h> 

#include <sys/types.h> 
#include <sys/socket.h> 

#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <ifaddrs.h> 

int main(void) 
{ 
        struct ifaddrs        *ifc, *ifc1; 
        char                ip[64]; 
        char                nm[64]; 

        if (0 != getifaddrs(&ifc)) return(-1); 
        ifc1 = ifc; 

        printf("Iface\tIP address\tNetmask\n"); 
        for(; NULL != ifc; ifc = (*ifc).ifa_next) { 
                printf("%s", (*ifc).ifa_name); 
                if (NULL != (*ifc).ifa_addr) { 
                        inet_ntop(AF_INET, &(((struct sockaddr_in*)((*ifc).ifa_addr))->sin_addr), ip, 64); 
                        printf("\t%s", ip); 
                } else { 
                        printf("\t\t"); 
                } 
                if (NULL != (*ifc).ifa_netmask) { 
                        inet_ntop(AF_INET, &(((struct sockaddr_in*)((*ifc).ifa_netmask))->sin_addr), nm, 64); 
                        printf("\t%s", nm); 
                } else { 
                        printf("\t\t"); 
                } 
                printf("\n"); 
        } 

        freeifaddrs(ifc1); 
        return(0); 
} 



-------------------------------------------------------------------------------- 



============原来的============ 
struct ifaddrs *ifap, *ifaphead, *ifaTmp; 
getifaddrs(&ifap); 
ifaphead = ifap; 
while((ifapTmp = ifap) != NULL) 
{ 
//实际任务代码  
ifap = ifapTmp->ifa_next; 
} 
freeifaddrs(ifaphead); 
=========修改后======== 
struct ifaddrs *ifap, *ifaphead; 
getifaddrs(&ifap); 
ifaphead = ifap; 
while(ifap != NULL) 
{ 
//实际任务代码  
ifap = ifa_next; 
} 
freeifaddrs(ifaphead); 
================== 
仅仅是用了一个ifapTmp来代替ifap做事,区别仅此而已(而且我也忘了一开始为什么要用ifapTmp....) 
但是最后都是用了freeifaddrs(ifaphead)啊,并没有传错指针啊???? 
中间的代码并没有对这段数据做任何修改啊..... 

请指教一下,这唯一的区别为什么会造成我原先的代码freeifaddrs失败?谢谢! 



ifaphead = ifap;                                //这里用ifaphead保存ifap指针地址 
while((ifapTmp = ifap) != NULL) 
{ 
ifap = ifapTmp->ifa_next;                //这里修改了ifap的地址 
} 
freeifaddrs(ifaphead);                       //由于ifap的地址修改，所以ifaphead已经是无效指针。 



![](http://blog.51cto.com/image/skin/artType01.jpg) ioctl及getifaddrs读取IPv4,IPv6网卡信息

2011-05-08 21:18:30

标签：[c](http://blog.51cto.com/tagindex.php?keyword=c)[vin_do，vin_do学习笔记，笔记](http://blog.51cto.com/tagindex.php?keyword=vin_do%A3%ACvin_do%D1%A7%CF%B0%B1%CA%BC%C7%A3%AC%B1%CA%BC%C7)[休闲](http://blog.51cto.com/tagindex.php?keyword=%D0%DD%CF%D0)[职场](http://blog.51cto.com/tagindex.php?keyword=%D6%B0%B3%A1)


使用ioctl的SIOCGIFCONF可以读取所有网卡信息。ioctl调用后返回指向ifconf的结构链表，其中包含了指向ifreq的结构指针。ifconf及ifreq定义在net/if.h中。

《UNIX网络编程》中提供了get_ifi_info函数的实现方法，使用这种方式来获取网络信息。在LINUX下，这种方式不能获得IPV6的网卡信息。《UNIX网络编程》中有如下描述：

> 
在支持IPV6的系统中，没有关于对SIOCGIFCONF请求是否返回IPV6地址的标准。我们给支持IPV6的新系统增加了一个case语句， 这是为了预防万一。问题在于ifreq中的联合把返回的地址定义成一个通用的16字节套接口地址结构，适合16字节的IPV4 socket_in结构，但对于24字节的IPV6 socket_in6结构太小了。如果返回IPV6地址，将可能破环现有的在每个ifreq结构中采用固定大小的套接口地址结构的代码。


经测试，在fedor6-2.6.18kernel中无法返回ipv6地址，事实上，返回的地址簇总是AF_INET,而并非AF_INET6。
这种方法的实现代码如下：
![](http://www.vi1129.com/wp-includes/js/tinymce/plugins/wordpress/img/trans.gif)
net_if.h
#ifndef __NET_INF_H
#define __NET_INF_H

#include 
#include 
#include 
#include 
#include 
#include 
#include 
#include 
#include 
#include 
#include 

#define IFI_NAME 16
#define IFI_HADDR 8

typedef struct ifi_info
{
  char ifi_name[IFI_NAME];
  u_char ifi_haddr[IFI_HADDR];
  u_short ifi_hlen;
  short ifi_flags;
  short ifi_myflags;
  struct sockaddr *ifi_addr;
  struct sockaddr *ifi_brdaddr;
  struct sockaddr *ifi_dstaddr;
  struct ifi_info *ifi_next;
}ifi_info;

#define IFI_ALIAS 1

struct ifi_info *get_ifi_info(int, int);

void free_ifi_info(struct ifi_info *);

#endif

net_if.c
#include "net_if.h"

ifi_info *get_ifi_info(int family, int doaliases)
{
  ifi_info *ifi, *ifihead, **ifipnext;
  int sockfd, len, lastlen, flags, myflags;
  char *ptr, *buf, lastname[IFNAMSIZ], *cptr;
  struct ifconf ifc;
  struct ifreq *ifr, ifrcopy;
  struct sockaddr_in *sinptr;

  if ((sockfd=socket(family, SOCK_DGRAM, 0))<0)
  {
    printf("socket error.\n");
    exit(1);
  }

  lastlen = 0;

  len = 10*sizeof(struct ifreq);
  while (1)
  {
    buf = (char*)malloc(len);
    ifc.ifc_len = len;
    ifc.ifc_buf = buf;
    if (ioctl(sockfd, SIOCGIFCONF, &ifc)<0)
    {
      if (errno!=EINVAL||lastlen!=0)
      {
        printf("ioctl error.\n");
      }
    }
    else
    {
      if (ifc.ifc_len == lastlen)
        break;
      lastlen = ifc.ifc_len;
    }
    len += 10*sizeof(struct ifreq);
    free(buf);
  }

  ifihead = NULL;
  ifipnext = &ifihead;
  lastname[0] = 0;

  for (ptr = buf; ptrifr->ifr_addr.sa_len?sizeof(struct sockaddr):ifr->ifr_addr.sa_len;
#else
    switch (ifr->ifr_addr.sa_family)
    {
#ifdef IPV6
    case AF_INET6:
      len = sizeof(struct sockaddr_in6);
      break;
#endif
    case AF_INET:
    default:
      len = sizeof(struct sockaddr);
      break;
    }
#endif

    ptr += sizeof(ifr->ifr_name) + len;

    if (ifr->ifr_addr.sa_family != family)
      continue;

    myflags = 0;
    if ((cptr=strchr(ifr->ifr_name, ':'))!=NULL)
      *cptr = 0;
    if (strncmp(lastname, ifr->ifr_name, IFNAMSIZ)==0)
    {
      if (doaliases == 0)
        continue;
      myflags = IFI_ALIAS;
    }

    memcpy(lastname, ifr->ifr_name, IFNAMSIZ);

    ifrcopy = *ifr;
    ioctl(sockfd, SIOCGIFFLAGS, &ifrcopy);
    flags = ifrcopy.ifr_flags;
    if ((flags&IFF_UP)==0)
      continue;
    /*
    if ((flags&IFF_BROADCAST)==0)
      continue;
    */
    ifi = calloc(1, sizeof(struct ifi_info));
    *ifipnext = ifi;
    ifipnext = &ifi->ifi_next;
    ifi->ifi_flags = flags;
    ifi->ifi_myflags = myflags;
    memcpy(ifi->ifi_name, ifr->ifr_name, IFI_NAME);
    ifi->ifi_name[IFI_NAME-1] = '\0';

    switch (ifr->ifr_addr.sa_family)
    {
    case AF_INET:
      sinptr = (struct sockaddr_in *)&ifr->ifr_addr;
      if (ifi->ifi_addr == NULL)
      {
        ifi->ifi_addr = calloc(1, sizeof(struct sockaddr_in));
        memcpy(ifi->ifi_addr, sinptr, sizeof(struct sockaddr_in));
#ifdef SIOCGIFBRDADDR
        if (flags & IFF_BROADCAST)
        {
          ioctl(sockfd, SIOCGIFBRDADDR, &ifrcopy);
          sinptr = (struct sockaddr_in *)&ifrcopy.ifr_broadaddr;
          ifi->ifi_brdaddr = calloc(1, sizeof(struct sockaddr_in));
          memcpy(ifi->ifi_brdaddr, sinptr, sizeof(struct sockaddr_in));
        }
#endif
#ifdef SIOCGIFDSTADDR
        if (flags & IFF_POINTOPOINT)
        {
          ioctl(sockfd, SIOCGIFDSTADDR, &ifrcopy);
          sinptr = (struct sockaddr_in*)&ifrcopy.ifr_dstaddr;
          ifi->ifi_dstaddr = calloc(1, sizeof(struct sockaddr_in));
          memcpy(ifi->ifi_dstaddr, sinptr, sizeof(struct sockaddr_in));
        }
#endif
      }
      break;
    default:
      break;
    }
  }
  free(buf);
  return(ifihead);
}

void free_ifi_info(ifi_info *ifihead)
{
  ifi_info *ifi, *ifinext;
  for (ifi=ifihead; ifi!=NULL; ifi=ifinext)
  {
    if (ifi->ifi_addr!=NULL)
      free(ifi->ifi_addr);

    if (ifi->ifi_brdaddr!=NULL)
      free(ifi->ifi_brdaddr);
    if (ifi->ifi_dstaddr!=NULL)
      free(ifi->ifi_dstaddr);
    ifinext = ifi->ifi_next;

    free(ifi);
  }

}

char *sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
  char portstr[7];
  static char str[128];

  switch (sa->sa_family)
  {
  case AF_INET:
    {
      struct sockaddr_in *sin = (struct sockaddr_in *)sa;

      if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str))==NULL)
        return NULL;

      if (ntohs(sin->sin_port)!=0)
      {
        snprintf(portstr, sizeof(portstr), ".%d", ntohs(sin->sin_port));
        strcat(str, portstr);
      }
      return str;
    }
    break;
  case AF_INET6:
    {
      struct sockaddr_in6 *sin = (struct sockaddr_in6 *)sa;

      if (inet_ntop(AF_INET6, &sin->sin6_addr, str, sizeof(str))==NULL)
        return NULL;

      if (ntohs(sin->sin6_port)!=0)
      {
        snprintf(portstr, sizeof(portstr), ".%d", ntohs(sin->sin6_port));
        strcat(str, portstr);
      }
      return str;
    }
    break;
  default:
    return NULL;
    break;
  }

}

int main(int argc, char *argv[])
{
  ifi_info *ifi, *ifihead;
  struct sockaddr *sa;
  u_char *ptr;
  int i, family, doaliases;
  if (argc!=3)
  {
    printf("usage: ./prifinfo  ");
    exit(1);
  }

  if (strcmp(argv[1], "inet4") == 0)
    family = AF_INET;
#ifdef IPV6
  else if (strcmp(argv[1], "inet6") == 0)
    family =AF_INET6;
#endif
  else
  {
    printf("invalid
");
    exit(1);
  }

  doaliases = atoi(argv[2]);

  for(ifihead = ifi = get_ifi_info(family, doaliases);
      ifi!=NULL;ifi=ifi->ifi_next)
  {
    printf("%s:<", ifi->ifi_name);
    if (ifi->ifi_flags&IFF_UP) printf("UP");
    if (ifi->ifi_flags&IFF_BROADCAST) printf("BCAST");
    if (ifi->ifi_flags&IFF_MULTICAST) printf("MCAST");
    if (ifi->ifi_flags&IFF_LOOPBACK) printf("LOOP");
    if (ifi->ifi_flags&IFF_POINTOPOINT) printf("P2P");
    printf(">\n");

    if ((i=ifi->ifi_hlen)>0)
    {
      ptr = ifi->ifi_haddr;
      do
      {
        printf("%s%x", (i==ifi->ifi_hlen)?" ":":", *ptr++);
      }while(--i>0);

      printf("\n");
    }

    if ((sa=ifi->ifi_addr)!=NULL)
      printf(" IP addr: %s\n",
             sock_ntop(sa, sizeof(*sa)));
    if ((sa=ifi->ifi_brdaddr)!=NULL)
      printf(" broadcast addr: %s\n",
             sock_ntop(sa, sizeof(*sa)));
    if ((sa=ifi->ifi_dstaddr)!=NULL)
      printf(" destnation addr: %s\n",
             sock_ntop(sa, sizeof(*sa)));
  }

  free_ifi_info(ifihead);

  exit(0);
}

使用gcc net_if.c -o net_if -DIPV6编译，在IPV4模式下运行输出为：

> 
[root@localhost net_if]./net_if inet4 1
lo:
IP addr: 127.0.0.1
eth1:
IP addr: 192.168.1.2
broadcast addr: 192.168.1.255
eth0:
IP addr: 192.168.125.99
broadcast addr: 192.168.125.255


执行./net_if inet6 1在输出为空。

第二种方式是使用getifaddrs函数获取，需要包含ifaddrs.h头文件，这种方式可以获得IPV6地址，改写的《UNIX网络编程》中的get_ifi_info函数如下所示：

znet.h
#ifndef __ZNET_H__
#define __ZNET_H__

#include 
#include 
#include 
#include 
#include 
#include 
#include 
#include 

#define	IFI_NAME	16			/* same as IFNAMSIZ in  */
#define	IFI_HADDR	 8			/* allow for 64-bit EUI-64 in future */

struct ifi_info {
  char    ifi_name[IFI_NAME];	/* interface name, null-terminated */
  short   ifi_index;			/* interface index */
  short   ifi_flags;			/* IFF_xxx constants from  */
  struct sockaddr  *ifi_addr;	/* primary address */
  struct sockaddr  *ifi_brdaddr;/* broadcast address */
  struct ifi_info  *ifi_next;	/* next of these structures */
};

struct ifi_info* get_ifi_info(int, int);
void free_ifi_info(struct ifi_info *);

#endif

znet.c
#include "znet.h"

struct ifi_info* get_ifi_info(int family, int doaliases) {
	struct ifi_info		*ifi, *ifihead, **ifipnext,*p;
	struct sockaddr_in	*sinptr;
	struct sockaddr_in6	*sin6ptr;
	struct ifaddrs *ifas;
//	char addr[128];
	int sockfd;

	ifihead = NULL;
	ifipnext = &ifihead;

	if(getifaddrs(&ifas)!=0)
		return ;

	for(;ifas!=NULL;ifas=(*ifas).ifa_next) {
		if (((*ifas).ifa_addr)->sa_family != family)
			continue;	// ignore if not desired address family
/*
		printf("%s %d\n",(*ifas).ifa_name,((*ifas).ifa_addr)->sa_family);
		if(((*ifas).ifa_addr)->sa_family!=AF_INET6)
			inet_ntop(AF_INET,&(((struct sockaddr_in *)((*ifas).ifa_addr))->sin_addr),addr,sizeof(addr));
		else
			inet_ntop(AF_INET6,&(((struct sockaddr_in6 *)((*ifas).ifa_addr))->sin6_addr),addr,sizeof(addr));
		printf("%s\t",addr);
		printf("\n");
*/
		ifi = (struct ifi_info*)calloc(1,sizeof(struct ifi_info));
		*ifipnext = ifi;
		ifipnext = &ifi->ifi_next;	

		ifi->ifi_flags = (*ifas).ifa_flags;
		memcpy(ifi->ifi_name, (*ifas).ifa_name, IFI_NAME);
		ifi->ifi_name[IFI_NAME-1] = '\0';

		switch (((*ifas).ifa_addr)->sa_family) {
			case AF_INET:
				sinptr = (struct sockaddr_in *) (*ifas).ifa_addr;
				ifi->ifi_addr = (struct sockaddr*)calloc(1, sizeof(struct sockaddr_in));
				memcpy(ifi->ifi_addr, sinptr, sizeof(struct sockaddr_in));
#ifdef	SIOCGIFBRDADDR
				if (ifi->ifi_flags & IFF_BROADCAST) {
					sinptr = (struct sockaddr_in *) (*ifas).ifa_broadaddr;
					ifi->ifi_brdaddr = (struct sockaddr*)calloc(1, sizeof(struct sockaddr_in));
					memcpy(ifi->ifi_brdaddr, sinptr, sizeof(struct sockaddr_in));
				}
#endif
				break; 

			case AF_INET6:
				sin6ptr = (struct sockaddr_in6 *) (*ifas).ifa_addr;
				ifi->ifi_addr = (struct sockaddr*)calloc(1, sizeof(struct sockaddr_in6));
				memcpy(ifi->ifi_addr, sin6ptr, sizeof(struct sockaddr_in6));
				break; 

			default:
				break;
		}
	}
	freeifaddrs(ifas);	

	return(ifihead);
}

int main(int argc, char *argv[]) {
	int family;
	if (argc!=2) {
	    printf("usage: ./znet \n");
	    exit(1);
	}
	if (strcmp(argv[1], "inet4") == 0)
		family = AF_INET;
	else if (strcmp(argv[1], "inet6") == 0)
		family =AF_INET6;
	else {
    printf("invalid
\n");
    exit(1);
    	}

	char addr[128];
	struct ifi_info	*ifi, *ifihead;
	printf("name\tflag\tIP\t\tbroadcastaddr\n");
	for (ifihead = ifi = get_ifi_info(family,1); ifi != NULL; ifi = ifi->ifi_next) {

		printf("%s\t",ifi->ifi_name);
		printf("%d\t",ifi->ifi_flags);
		if((ifi->ifi_addr)->sa_family!=AF_INET6)
			inet_ntop(AF_INET,&(((struct sockaddr_in *)(ifi->ifi_addr))->sin_addr),addr,sizeof(addr));
		else
			inet_ntop(AF_INET6,&(((struct sockaddr_in6 *)(ifi->ifi_addr))->sin6_addr),addr,sizeof(addr));
		printf("%s\t",addr);
#ifdef	SIOCGIFBRDADDR
		if ((ifi->ifi_flags & IFF_BROADCAST) && (ifi->ifi_addr)->sa_family!=AF_INET6) {
			inet_ntop(AF_INET,&(((struct sockaddr_in *) (ifi->ifi_brdaddr))->sin_addr),addr,sizeof(addr));
			printf("%s\t",addr);
		}
#endif
		printf("\n+++++++++++++++++++++++++++++++++++++++++++\n");
	}
	return 0;
}

这段代码输出如下：

> 
[root@localhost net_if]./znet inet4
name flag IP broadcastaddr
lo 73 127.0.0.1
++++++++++++++++++++++++++++++
eth1 4099 192.168.1.2 192.168.1.255
++++++++++++++++++++++++++++++
eth0 4163 192.168.125.99 192.168.125.255
++++++++++++++++++++++++++++++
[root@localhost net_if]./znet inet6
name flag IP broadcastaddr
lo 73 ::1
++++++++++++++++++++++++++++++
eth1 4163 2001:250:1800:1::1
++++++++++++++++++++++++++++++
eth0 4163 2001:250:1888:1::1












