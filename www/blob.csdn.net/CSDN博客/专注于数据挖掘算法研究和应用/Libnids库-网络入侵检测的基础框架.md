# Libnids库-网络入侵检测的基础框架 - 专注于数据挖掘算法研究和应用 - CSDN博客





2013年04月27日 09:44:04[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3515








1.Libnids介绍：

   Libnids(library  network intrusion detection system)是网络入侵检测开发的专业编程接口，实现了网络入侵检测系统的基本框架，提供了一些基本功能。Libnids是基于libpcap和libnet而开发的，其主要功能包括捕获网络数据包、IP碎片重组、TCP数据流重组以及端口扫描攻击检测和异常数据包检测等。



2.Libnids安装：基于库libpcap和libnet

  1)安装libpcap开发包：

     tar xzvf  libpcap-x.x.x.tar.gz

     ./configure

     make

     make install

  2)安装libnet开发包：

     tar zxvf libnet-x.x.x.tar.gz

     ./configure

     make 

    make install

  3)安装libnids开发包：

     tar zxvf  libnids-x.x.x.tar.gz

     ./configure

    make 

    make install





3.Libnids基本常量




1)报警类型

> 

enum


{

> 

NIDS_WARN_IP =1,  //IP数据包异常


NIDS_WARN_TCP,//TCP数据包异常


NIDS_WARN_UDP,//UDP数据包异常


NIDS_WARN_SCAN//表示有扫描攻击发生



}


enum


{

> 

NIDS_WARN_UNDEFINED=0, //表示未定义


NIDS_WARN_IP_OVERSIZED.//表示IP数据包超长


NIDS_WARN_IP_INVLIST,//表示无效的碎片队列


NIDS_WARN_IP_OVERLAP,//表示发生重叠


NIDS_WARN_IP_HDR,//表示无效IP首部，IP数据包发生异常


NIDS_WARN_IP_SRR,//表示源路由IP数据包


NIDS_WARN_TCP_TOOMUCH,//表示tcp数据个数太多，因为在libnids中在同一时刻捕获的tcp个数最大值为tcp连接参数的哈希表长度3/4  


NIDS_WARN_TCP_HDR,//表示无效TCP首部，TCP数据包发生异常


NIDS_WARN_TCP_BIGAQUEUE,//表示TCP接收的队列数据过多


NIDS_WARN_TCP_BADFLAGS//表示错误标记



}



2)Libnids状态


在对TCP数据流进行重组时，必须考虑到TCP的连接状态，在Libnids中为了方便开发而定义了6种Libnids状态(描述的是连接的逻辑状态)


#define NIDS_JUST_EST 1//表示tcp连接建立


#define NIDS_DATA 2  //表示接受数据的状态


#define NIDS_CLOSE 3 //表示tcp连接正常关闭


#define NIDS_RESET 4 //表示tcp连接被重置关闭


#define NIDS_TIMED_OUT 5 //表示由于超时tcp连接被关闭


#define NIDS_EXITING 6  //表示libnids正在退出


真正的TCP连接状态有11种


enum


{

> 

TCP_ESTABLISHED=1, //表示ESTABLISH状态，TCP连接建立，开始传输数据


TCP_SYN_SENT,//表示syn_sent状态,主动打开


TCP_SYN_RECV,//表示syn_recv状态,接收SYN


TCP_FIN_WAIT1,//表示FIN_WAIT_1状态


TCP_FIN_WAIT2,//表示FIN_WAIT2状态


TCP_TIME_WAIT//表示TIME_WAIT状态


TCP_ClOSE,//表示Closed状态


TCP_CLOSE_WAIT,//表示CLose_WAIT状态


TCP_LAST_ACK,//表示LAST_ACK状态


TCP_LISTEN,//表示LISTEN状态


TCP_CLOSING//表示CLOSING 状态



}


3)校验和，与此相关的常量定义如下：


#define NIDS_DO_CHKSUM 0  //表示告诉Libnids要计算校验和


#define NIDS_DONT_CHKSUM 1//表示告诉LIbnids不需要计算校验和



4.Libnids数据结构




1)tuple4：此数据结构是Libnids中最基本的一种数据结构


struct tuple4


{

> 

u_short source;//源端口


u_short dest;//目标端口


u_int saddr; //源IP


u_int daddr;//目的IP



};//用于描述一个地址端口对，它表示发送方IP和端口以及接收方IP和端口


2)half_stream：此数据结构用来描述在tcp连接中一端的所有信息，可以使客户端也可以是服务端。


struct half_stream


{

> 

char state;//表示套接字的状态,也就是tcp连接状态


char collect;//表示是否存储数据到data中,如果大于0就存储,否则忽略


char collect_urg;//是否存储紧急数据到urgdata中,如果大于0就存储,否则忽略


char *data;//存储正常接收的数据


int offset;//存储在data中数据的第一个字节的偏移量


int count;//表示从tcp连接开始已经存储到data中的数据的字节数


int count_new;//表示有多少新数据寸到data 中


int bufsize;//


int rmem_alloc;


int urg_count;


u_int acked;


u_int seq;


u_int ack_seq;


u_int first_data_seg;


u_char urgdata;//用来存储紧急数据


u_char count_new_urg;//表示是否有新的紧急数据到达


u_char urg_seen;


u_int urg_ptr;


u_short window;


u_char ts_on;


u_int curr_ts;


struct skbuff *list;


struct skbuff *listtail;



}


3)tcp_stream:描述的是一个TCP连接的所有信息


struct tcp_stream


{

> 

struct tuple4 addr;//是一个tuple4类型的成员,它表示一个tcp连接的四个重要信息


char nids_state;//表示逻辑连接状态


struct lurker_node *listeners;


struct half_stream client;


struct half_stream server;


struct tcp_stream *next_node;


struct tcp_stream *prev_node;


int hash_index;


struct tcp_stream *next_time;


struct tcp_stream *prev_time;


int read;


struct tcp_stream *next_free;



};//描述了一个TCP连接的完整信息


4)nids_prm:描述libnids的一些全局参数信息


struct nids_prm


{

> 

int n_tcp_streams;//表示哈西表大小,此哈西表用来存放tcp_stream数据结构,


int n_hosts;//表示存放ip碎片信息的哈西表的大小


char *device;


char *filename;//用来存储网络数据捕获文件.如果设置了文件,与此同时就应该设置成员device为null,默认值为NULL


int sk_buff_size;//表示数据结构sk_buff的大小.数据结构sk_buff是linux内核中一个重要的数据结构,是用来进行数据包队列操作的


int dev_addon;//表示在数据结构sk_buff中用于网络接口上信息的字节数,如果是-1(默认值),那么libnids会根据不同的网络接口进行修正


void (*syslog)();//函数指针,默认值为nids_syslog()函数.在syslog中可以检测入侵攻击,如:网络扫描攻击


函数定义类型为nids_syslog(int type,int errnum,struct ip_header * iph,void *data)


int syslog_level;//表示日志等级,默认值为LOG_ALERT.


int scan_num_hosts;//表示存储端口扫描信息的哈西表的大小


int scan_delay;//表示在扫描检测中,两端口扫描的间隔时间


int scan_num_ports;//表示相同源地址必须扫描的tcp端口数目


void (*no_mem)(char *);//当libnids发生内存溢出时被调用


int (*ip_filter)();//函数指针,此函数可以用来分析ip数据包,当有ip数据包到达时,此函数被调用.默认值为nids_ip_filter,该函数的定义如下：


static int nids_ip_filter(struct ip * x,int len)


char *pcap_filter;//表示过滤规则


int promisc;//表示网卡模式,非0为混杂模式,否则为非混杂模式,默认值为1


int one_loop_less;//表示捕获数据返回的时间,以豪秒计算.默认值为1024


int pcap_timeout;



};


在Libnids中用nids_prm数据结构定义了一个全局变量nids_params,其定义和初始值如下:


struct nids_prm nids_params={

> 

1040,//n_tcp_streams


256,//n_hosts


NULL,//device


NULL,//filename


168,//sk_buff_size


-1,//dev_addon


nids_syslog,//syslog()


LOG_ALERT,//syslog_level


256,//scan_num_hosts


3000,//scan_delay


10,//scan_num_prots


nids_no_mem,//no_mem()


nids_ip_filter,//ip_filter


NULL,//pcap_filter


1,//promisc


0,//one_loop_less


1024//pcap_timeout



}在使用Libnids开发程序时,可以首先对nids_params全局变量的值进行修改,这样对整个Libnids就全部有效


5)nids_chksum_ctl:描述的是计算校验和


struct nids_chksum_ctl


{

> 

u_int netaddr;


u_int mask;


u_int action;//表示动作,如果是NIDS_DO_CHKSUM,表示要计算校验和;如果是NIDS_DONT_CHKSUM表示不计算校验和


u_int reserved;     



}

5.libnids函数




1)基本函数

> 

(1)int  nids_init(void);//对libnids进行初始化


(2)void nids_run(void);//运行Libnids,进入循环捕获数据包状态.


(3)int nids_getfd(void);//获得文件描述号


(4)int nids_dispatch(int cnt)//功能是调用Libpcap中的捕获数据包函数pcap_dispatch().


(5)int nids_next(void)//调用Libpcap中的捕获数据包函数pcap_next()


(6)void nids_register_chksum_ctl(struct nids_chksum_ctl *ptr,int nr)//决定是否计算校验和,它是根据数据结构nids_chksum_ctl中的action进行决定的



2)IP碎片函数

> 

(1)void nids_register_ip_frag(void(*))//此函数的功能是注册一个能够检测所有IP数据包的回调函数,包括IP碎片


eg:nids_register_ip_frag(ip_frag_function);


这样就定义了一个回调函数ip_frag_function的定义类型如下:


void ip_frag_function(struct ip *a_packet,int len)


(2)void nids_register_ip(void(*))//此函数定义一个回调函数,此回调函数可以接受正常的IP数据包,eg:


nids_register_ip(ip_function);


此回调函数的定义类型如下:


void ip_function(struct ip * a_packet)



3)TCP数据流重组函数

> 

(1)void nids_register_tcp(void(*))


回调函数的功能是注册一个TCP连接的回调函数,回调函数的类型定义如下:


void tcp_callback(struct tcp_stream *ns,void **param);


其中参数ns表示一个tcp连接的所有信息,它的类型是tcp_stream数据结构;参数param表示要传递的连接参数信息,可以指向一个TCP连接的私有数据


(2)void nids_killtcp(struct tcp_stream * a_tcp)//此函数功能是终止TCP连接


(3)void nids_discard(struct tcp_stream *a_tcp,int num)//丢弃num字节TCP数据,用于存储更多的数据



4)UDP注册函数

> 

(1)void nids_register_udp(void(*));


此函数的功能注册一个分析UDP协议的回调函数,回调函数的类型定义如下:


void udp_callback(struct tuple4 *addr,char *buf,int len,struct ip * iph);


其中参数addr表示的是端口的信息,参数buf表示UDP协议负载数据内容,参数len 表示UDP负载数据的长度;参数iph表示一个IP数据包,包括IP首部,UDP首部以及UDP负载内容



利用Libnids开发的流程


用函数nids_init()进行初始化。


然后注册相应的回调函数。不同的回调函数实现不同的功能


最后利用函数nids_run()进入循环捕获数据包的状态。





