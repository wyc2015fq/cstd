# struct ifconf和struct ifreq，获取网线插入状态 - DoubleLi - 博客园






这两天看用C获取当前网口的插入网线状态的程序，遇见了这两个不熟悉的结构体，看了头文件中的说明和详细。

# struct ifreq

这个结构定义在include/net/if.h，用来配置ip地址，激活接口，配置MTU等接口信息的





**[cpp]**[view plain](http://blog.csdn.net/jk110333/article/details/8832077)[copy](http://blog.csdn.net/jk110333/article/details/8832077)



- /* Interface request structure used for socket ioctl's.  All interface 
-    ioctl's must have parameter definitions which begin with ifr_name. 
-    The remainder may be interface specific.  */  
- 
- struct ifreq  
-   {  
- # define IFHWADDRLEN    6  
- # define IFNAMSIZ   IF_NAMESIZE  
- union  
-       {     
- char ifrn_name[IFNAMSIZ];   /* Interface name, e.g. "en0".  */  
-       } ifr_ifrn;  
- 
- union  
-       {     
- struct sockaddr ifru_addr;  
- struct sockaddr ifru_dstaddr;  
- struct sockaddr ifru_broadaddr;  
- struct sockaddr ifru_netmask;  
- struct sockaddr ifru_hwaddr;  
- short int ifru_flags;  
- int ifru_ivalue;  
- int ifru_mtu;  
- struct ifmap ifru_map;  
- char ifru_slave[IFNAMSIZ];  /* Just fits the size */  
- char ifru_newname[IFNAMSIZ];  
-     __caddr_t ifru_data;  
-       } ifr_ifru;  
-   };    
- # define ifr_name   ifr_ifrn.ifrn_name  /* interface name   */  
- # define ifr_hwaddr ifr_ifru.ifru_hwaddr    /* MAC address      */  
- # define ifr_addr   ifr_ifru.ifru_addr  /* address      */  
- # define ifr_dstaddr    ifr_ifru.ifru_dstaddr   /* other end of p-p lnk */  
- # define ifr_broadaddr  ifr_ifru.ifru_broadaddr /* broadcast address    */  
- # define ifr_netmask    ifr_ifru.ifru_netmask   /* interface net mask   */  
- # define ifr_flags  ifr_ifru.ifru_flags /* flags        */  
- # define ifr_metric ifr_ifru.ifru_ivalue    /* metric       */  
- # define ifr_mtu    ifr_ifru.ifru_mtu   /* mtu          */  
- # define ifr_map    ifr_ifru.ifru_map   /* device map       */  
- # define ifr_slave  ifr_ifru.ifru_slave /* slave device     */  
- # define ifr_data   ifr_ifru.ifru_data  /* for use by interface */  
- # define ifr_ifindex    ifr_ifru.ifru_ivalue    /* interface index      */  
- # define ifr_bandwidth  ifr_ifru.ifru_ivalue    /* link bandwidth   */  
- # define ifr_qlen   ifr_ifru.ifru_ivalue    /* queue length     */  
- # define ifr_newname    ifr_ifru.ifru_newname   /* New name     */  
- # define _IOT_ifreq _IOT(_IOTS(char),IFNAMSIZ,_IOTS(char),16,0,0)  
- # define _IOT_ifreq_short _IOT(_IOTS(char),IFNAMSIZ,_IOTS(short),1,0,0)  
- # define _IOT_ifreq_int _IOT(_IOTS(char),IFNAMSIZ,_IOTS(int),1,0,0)  






# struct  ifconf

通常是用来保存所有接口信息的





**[cpp]**[view plain](http://blog.csdn.net/jk110333/article/details/8832077)[copy](http://blog.csdn.net/jk110333/article/details/8832077)



- /* Structure used in SIOCGIFCONF request.  Used to retrieve interface 
-    configuration for machine (useful for programs which must know all 
-    networks accessible).  */  
- 
- struct ifconf  
-   {  
- int ifc_len;            /* Size of buffer.  */  
- union  
-       {  
-     __caddr_t ifcu_buf;  
- struct ifreq *ifcu_req;  
-       } ifc_ifcu;  
-   };  
- # define ifc_buf    ifc_ifcu.ifcu_buf   /* Buffer address.  */  
- # define ifc_req    ifc_ifcu.ifcu_req   /* Array of structures.  */  
- # define _IOT_ifconf _IOT(_IOTS(struct ifconf),1,0,0,0,0) /* not right */  






# 应用

想要获取当前网口网线插入状态，需要用到ifreq结构体，获取网卡的信息，然后socket结合网卡驱动的ioctl，就可以得到与网线插入状态相关的数据。
另外推荐一个百度文库的文章，包含代码

[http://wenku.baidu.com/view/59f4508d680203d8ce2f2412.html](http://wenku.baidu.com/view/59f4508d680203d8ce2f2412.html)









