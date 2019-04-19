# tcpdump源码分析 - xqhrs232的专栏 - CSDN博客
2018年01月08日 11:51:46[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：195
原文地址::[http://blog.csdn.net/jzzjsy/article/details/45111631](http://blog.csdn.net/jzzjsy/article/details/45111631)
相关文章
1、tcpdump最新版本源码----[http://download.csdn.net/download/mercator/7297369](http://download.csdn.net/download/mercator/7297369)
linux环境下libpcap 源[代码](http://www.07net01.com/program/)分析
韩大卫@吉林师范大学
libpcap 源[代码](http://www.07net01.com/program/)官方下载地址：
git clonehttps://github.com/the-tcpdump-group/libpcap.git
tcpdumpm源代码官方下载地址：
git clone git://bpf.tcpdump.org/tcpdump
tcpdump.c使用libpcap里的pcap_open_live和pcap_loop 完成两个最关键的动作：获取捕获报文的接口，和捕获报文并将报文交给callback。 (关于tcpdump源代码的构架，请参考作者的tcpdump源代码分析)
现结合libpcap源代码分析pcap_open_live和pcap_loop的实现机制，并进入linux[内核](http://www.07net01.com/linux/)，展示linux[内核](http://www.07net01.com/linux/)对这两个API的响应动作。
tcpdump.c对pcap_open_live的使用是：
pd = pcap_open_live(device, snaplen, !pflag, 1000, ebuf);
pcap_open_live定义如下：
pcap_t *pcap_open_live(const char *source, int snaplen,int promisc, int to_ms, char *errbuf)
source   为指定的网络接口。
snaplen 为最大报文长度。
Promisc 是否将设备设置为混杂模式。
to_ms    超时时间。
errbuf   为错误信息描述字符。
返回值为cap_t类型的指针，pcap_t 定义是：
typedef struct pcap pcap_t;
struct pcap {
/*typedef int (*read_op_t)(pcap_t *, int cnt,pcap_handler, u_char *);
read_op为从网络接口读取报文的函数指针，待其得到赋值后，调用实现函数*/
    read_op_tread_op;
//从文件里读取报文的函数指针
    int(*next_packet_op)(pcap_t *, struct pcap_pkthdr *, u_char **);
//文件描述符,即socket
    int fd;
    intselectable_fd;  
    intbufsize;    //read缓冲区大小
    u_char *buffer;//read缓冲区指针
    u_char *bp;
    int cc;
...
    int snapshot;
    intlinktype;       /* Network linktype */
    intlinktype_ext;     
    int tzoff;      /* timezone offset */
    intoffset;     /* offset [for](http://www.07net01.com/program/)proper alignment */
    intactivated;      /* true if the capture isreally started */
    intoldstyle;       /* if we're opening withpcap_open_live() */
    struct pcap_optopt;
    u_char *pkt;
...
   //激活函数，激活函数在得到调用后，会建立起与底层IPC的socket
    activate_op_tactivate_op;
...
};
pcap_t *
pcap_open_live(const char *source, int snaplen, intpromisc, int to_ms, char *errbuf){  
    pcap_t *p;
    int status;
   //创建捕获报文的接口句柄
    p =pcap_create(source, errbuf);
    if (p == NULL)
        return(NULL);
    //设置最大报文长度
    status =pcap_set_snaplen(p, snaplen);
    if (status <0)
        goto fail;
         //将设备设为混杂模式
    status =pcap_set_promisc(p, promisc);
    if (status <0)
        goto fail;
         //设置超时时间
    status =pcap_set_timeout(p, to_ms);
    if (status <0)
        goto fail;
    p->oldstyle= 1;
         //pcap_avtivate调用pcap_t的activate_op, 建立起与底层IPC通道
    status =pcap_activate(p);
    if (status <0)
        goto fail;
    return (p);
...
}
pcap_t *pcap_create(const char *source, char*errbuf){  
    size_t i;
    int is_theirs;
    pcap_t *p;
    if (source ==NULL)
        source ="any";
         //在capture_source_types数组里寻找是否有特定API集合的接口对应source
    for (i = 0;capture_source_types[i].create_op != NULL; i++) {
        is_theirs =0;
        p =capture_source_types[i].create_op(source, errbuf, &is_theirs);
        if(is_theirs) {
               return (p);
        }
    }
    //如果没有，那么就将source作为普通网络接口
    return(pcap_create_interface(source, errbuf));
}
pcap_create_interface() 函数在libpcap下有多个实现，可由[编译](http://www.07net01.com/linux/)宏来指定特定的pcap_create_interface来初始化read_op等函数指针。linux环境里默认是libpcap/pcap-linux.c中的
 pcap_create_interface():
pcap_t *
pcap_create_interface(const char *device, char *ebuf)
{ 
    pcap_t *handle;
/*可将 pcap_create_common看做pcap_t结构的构造函数，初始化一个pcap_t*/
    handle =pcap_create_common(device, ebuf, sizeof (struct pcap_linux));
    if (handle ==NULL)
        returnNULL;
         //为pcap_t 的激活函数指针填充具体实现函数
   handle->activate_op = pcap_activate_linux;
   handle->can_set_rfmon_op = pcap_can_set_rfmon_linux;
    return handle;
}
完成后回到pcap_open_live，设置snaplen,promisc,to_ms后，调用status = pcap_activate(p),该函数执行status = p->activate_op(p) ，
进而调用 pcap_activate_linux(), 完成read_op等重要函数指针的具体赋值。
static int
 pcap_activate_linux(pcap_t *handle)
{  
    structpcap_linux *handlep = handle->priv;
    const char  *device;
    int     status = 0;
    device =handle->opt.source;
   handle->inject_op = pcap_inject_linux;
   handle->setfilter_op = pcap_setfilter_linux;
   handle->setdirection_op = pcap_setdirection_linux;
   handle->set_datalink_op = pcap_set_datalink_linux;
   handle->getnonblock_op = pcap_getnonblock_fd;
   handle->setnonblock_op = pcap_setnonblock_fd;
   handle->cleanup_op = pcap_cleanup_linux;
         //最重要的函数指针read_op
   handle->read_op = pcap_read_linux;
   handle->stats_op = pcap_stats_linux;
    if(strcmp(device, "any") == 0) {
        if(handle->opt.promisc) {
           handle->opt.promisc = 0;
            /* Justa warning. */
           snprintf(handle->errbuf, PCAP_ERRBUF_SIZE,
               "Promiscuous mode not supported>先使用activete_new()
    status =activate_new(handle);
    if (status < 0) {
        goto fail;
    }
    //根据错误值具体处理
    if (status ==1) {
        switch(activate_mmap(handle, &status)) {
        case 1:
            returnstatus;
        case 0:
           break;
        case -1:
            gotofail;
        }
    }
         //如果status为0，再尝试使用activete_old()函数
    else if (status== 0) {
        /*Non-fatal error; try old way */
        if ((status= activate_old(handle)) != 1) {
            gotofail;
        }
    }
    status = 0;
    if(handle->opt.buffer_size != 0) {
        //设置socket的缓冲区和缓冲区长度
        if(setsockopt(handle->fd, SOL_SOCKET, SO_RCVBUF,
           &handle->opt.buffer_size,
           sizeof(handle->opt.buffer_size)) == -1) {
           snprintf(handle->errbuf, PCAP_ERRBUF_SIZE,
                 "SO_RCVBUF: %s",pcap_strerror(errno));
            status= PCAP_ERROR;
            gotofail;
        }
    }
    handle->selectable_fd = handle->fd;
    return status;
...
}    
static int
activate_new(pcap_t *handle)
{
   struct pcap_linux*handlep = handle->priv;
    const char  *device = handle->opt.source;
    int         is_any_device = (strcmp(device,"any") == 0);
    int         sock_fd = -1, arptype;
    int         err = 0;
    structpacket_mreq  mr;
/*指定网口情况下用PF_PACKET协议通信得到原始以太网数据帧数据
关于socket()函数，我个人认为可以将其理解为open()：
open()打开不同的文件，这样在返回的句柄里就可使用这个文件设备模块提供的ops，
socket()打开不同的协议，返回句柄里也包括了该协议的底层模块提供的ops. 只不过linux下面没法将网络协议当作普通文件（如/dev/xx）处理，所以才有了另一套socket特定的APIs*/
    sock_fd =is_any_device ?
        socket(PF_PACKET,SOCK_DGRAM, htons(ETH_P_ALL)) :
       socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
...
   handlep->sock_packet = 0;
/*iface_get_id()使用ioctl(fd, SIOCGIFINDEX, &ifr)获取lo还回设备的索引值*/
   handlep->lo_ifindex = iface_get_id(sock_fd, "lo",handle->errbuf);
   handle->offset   = 0;
    if(!is_any_device) {
       handlep->cooked = 0;
        if(handle->opt.rfmon) {
            err =enter_rfmon_mode(handle, sock_fd, device);
            if (err< 0) {
                close(sock_fd);
               return err;
            }
            if (err== 0) {
               close(sock_fd);
               return PCAP_ERROR_RFMON_NOTSUP;
            }
            if(handlep->mondevice != NULL)
                device= handlep->mondevice;
        }
/*iface_get_arptype()调用ioctl(fd, SIOCGIFHWADDR, &ifr)获取硬件类型 */
        arptype =iface_get_arptype(sock_fd, device, handle->errbuf);
        if (arptype< 0) {
           close(sock_fd);
            returnarptype;
        }
       map_arphrd_to_dlt(handle, arptype, 1);
  ...
      //获取指定设备的索引值
       handlep->ifindex = iface_get_id(sock_fd, device,
 handle->errbuf);
        if(handlep->ifindex == -1) {
           close(sock_fd);
            returnPCAP_ERROR;
/*iface_[bin](http://www.07net01.com/linux/)d()将设备的索引值作为struct socketadd_ll的索引值与socket绑定
    structsockaddr_ll  sll;
   sll.sll_family      =AF_PACKET;                                                              
    sll.sll_ifindex     = ifindex;
   sll.sll_protocol    =htons(ETH_P_ALL);
[bin](http://www.07net01.com/linux/)d(fd, (struct sockaddr *) &sll,sizeof(sll))
 == -1 */
        if ((err =iface_bind(sock_fd, handlep->ifindex,
handle->errbuf)) != 1) {
               close(sock_fd);
            if (err< 0)
               return err;
            else
               return 0;   /* try old mechanism*/
        }
...
    }
    if(!is_any_device && handle->opt.promisc) {
       memset(&mr, 0, sizeof(mr));
       mr.mr_ifindex = handlep->ifindex;
       mr.mr_type    = PACKET_MR_PROMISC;
        if(setsockopt(sock_fd, SOL_PACKET, PACKET_ADD_MEMBERSHIP,
           &mr, sizeof(mr)) == -1) {
           snprintf(handle->errbuf, PCAP_ERRBUF_SIZE,
               "setsockopt: %s", pcap_strerror(errno));
           close(sock_fd);
            returnPCAP_ERROR;
        }
    }
    if(handlep->cooked) {
        if(handle->snapshot < SLL_HDR_LEN + 1)
           handle->snapshot = SLL_HDR_LEN + 1;
    }
   handle->bufsize = handle->snapshot;
    //根据以太网链路层类型决定[VLAN](http://www.07net01.com/switch/) Tag在报文中的偏移值
    switch(handle->linktype) {
    caseDLT_EN10MB:
       handlep->vlan_offset = 2 * ETH_ALEN;
        break;
    case DLT_[LINUX](http://www.07net01.com/linux/)_SLL:
       handlep->vlan_offset = 14;
        break;
    default:
       handlep->vlan_offset = -1; /* unknown */
        break;
    }
//将sock_fd作为pcap_t的fd
    handle->fd =sock_fd;
...
}
至此，通过pcap_open_live完成全部准备阶段的内容，之后就可以使用pcap_loop()来获取来自底层的数据并提交给callback函数进行应用处理， tcpdump.c 对pcap_loop的使用是：
status = pcap_loop(pd, cnt, callback, pcap_userdata);
//cnt 为指定捕获报文的个数
在libpcap/pcap.c里有pcap_loop的定义： 
int
pcap_loop(pcap_t *p, int cnt, pcap_handler callback,u_char *user)
{  
    register int n;
    for (;;) {
        if(p->rfile != NULL) {
                 //从文件里读取报文
            n =pcap_offline_read(p, cnt, callback, user);
        } else {
                 //从指定网口读取报文
            do {
                          //read_op即为pcap_read_packet
                n =p->read_op(p, cnt, callback, user);
            } while(n == 0);
        }
        //当n<0时退出循环，退出pcap_loop
        if (n <=0)
            return(n);
      //如果达到捕获报文个数，退出pcap_loop
        if (cnt> 0) {
            cnt -=n;
            if (cnt<= 0)
               return (0);
        }
    }
函数指针read_op指向的就是pcap_read_linux_mmap
pcap_get_ring_frame(handle, TP_STATUS_USER)进行取数据， 这里的数据是是通过应用程序进行mmap一块内核的内存，内核取得的数据保存在mmap所映射的区域，进行取。具体可以搜索下pcap_get_ring_frame这接口，网络上有说明的
bp = (unsigned char*)h.raw + tp_mac;获取之后的数据给bp
callback(user, &pcaphdr, bp);调用这个进行打印
static void print_packet(u_char *user, const struct pcap_pkthdr *h, const u_char *sp) 接着调用这个
  hdrlen = (*print_info->p.printer)(h, sp);调用这个进行打印
经过初始化，调用u_int prism_if_print(const struct pcap_pkthdr *h, const u_char *p)
return PRISM_HDR_LEN + ieee802_11_print(p + PRISM_HDR_LEN,  length - PRISM_HDR_LEN, caplen - PRISM_HDR_LEN, 0, 0);这里把源数据去掉PRISM_HDR_LEN的长度。
fc = EXTRACT_LE_16BITS(p);
hdrlen = extract_header_length(fc);进行去掉额外的头信息
        p += hdrlen;源数据偏移26字节
       p+=8再进行偏移八个字节
      ip4 = (const struct ip *)p;
up = (struct udphdr *)((const u_char *)ip4 +  IP_HL(ip4) * 4);
dport = EXTRACT_16BITS(&up->uh_dport);
     最后就能得到相应的源地址的ip的端口号了，上述跟踪的是udp的广播包的形式打印的
}
