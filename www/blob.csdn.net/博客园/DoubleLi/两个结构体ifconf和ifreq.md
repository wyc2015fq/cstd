# 两个结构体ifconf和ifreq - DoubleLi - 博客园






用ioctl获得本地ip地址时要用到两个结构体ifconf和ifreq，它们对于大多数人
来说都是比较陌生的，这里给大家一种比较简单的理解方法，当然只一种帮助
理解的方法，在描述中可能会有一些地方与真实定义有所出入，仅供参考.

首先先认识一下ifconf和ifreq：


//ifconf通常是用来保存所有接口信息的//if.hstruct ifconf {    int    ifc_len;            /* size of buffer    */    union     {        char *ifcu_buf;                        /* input from user->kernel*/        struct ifreq *ifcu_req;        /* return from kernel->user*/    } ifc_ifcu;};#define    ifc_buf    ifc_ifcu.ifcu_buf        /* buffer address    */#define    ifc_req    ifc_ifcu.ifcu_req        /* array of structures    *///ifreq用来保存某个接口的信息//if.hstruct ifreq {    char ifr_name[IFNAMSIZ];    union {        struct sockaddr ifru_addr;        struct sockaddr ifru_dstaddr;        struct sockaddr ifru_broadaddr;        short ifru_flags;        int ifru_metric;        caddr_t ifru_data;    } ifr_ifru;};#define ifr_addr ifr_ifru.ifru_addr#define ifr_dstaddr ifr_ifru.ifru_dstaddr#define ifr_broadaddr ifr_ifru.ifru_broadaddr




上边这两个结构看起来比较复杂，我们现在把它们简单化一些：
比如说现在我们向实现获得本地IP的功能。

我们的做法是：
1. 先通过ioctl获得本地所有接口的信息，并保存在ifconf中
2. 再从ifconf中取出每一个ifreq中表示ip地址的信息

具体使用时我们可以认为ifconf就有两个成员：
ifc_len 和 ifc_buf，如图一所示： 　　

![](http://img.my.csdn.net/uploads/201304/17/1366181032_6205.jpg)





ifc_len:表示用来存放所有接口信息的缓冲区长度
ifc_buf:表示存放接口信息的缓冲区

所以我们需要在程序开始时对ifconf的ifc_led和ifc_buf进行初始化
接下来使用ioctl获取所有接口信息，完成后ifc_len内存放实际获得的借口信息总长度
并且信息被存放在ifc_buf中。

接下来我们只需要从一个一个的接口信息获取ip地址信息即可。

下面有一个简单的参考：








#include #include #include #include #include in.h>#include <string.h>#include if.h>#include int main(){    int i=0;    int sockfd;  struct ifconf ifconf;  unsigned char buf[512];  struct ifreq *ifreq;  //初始化ifconf  ifconf.ifc_len = 512;  ifconf.ifc_buf = buf;    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0)    {        perror("socket");        exit(1);    }    ioctl(sockfd, SIOCGIFCONF, &ifconf);    //获取所有接口信息  //接下来一个一个的获取IP地址  ifreq = (struct ifreq*)buf;    for(i=(ifconf.ifc_len/sizeof(struct ifreq)); i>0; i--)  {//      if(ifreq->ifr_flags == AF_INET){            //for ipv4          printf("name = [%s]\n", ifreq->ifr_name);      printf("local addr = [%s]\n",                       inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr));      ifreq++;//  }  }    return 0;}












