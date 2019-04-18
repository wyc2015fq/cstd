# 使用epoll进行高性能网络编程  - ljx0305的专栏 - CSDN博客
2009年08月25日 23:06:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：910标签：[网络																[编程																[struct																[file																[http服务器																[list](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=http服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)
个人分类：[网络编程																[linux下软件安装																[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)](https://blog.csdn.net/ljx0305/article/category/440323)](https://blog.csdn.net/ljx0305/article/category/403915)
epoll和其他技术的性能比较. 
翻译：韩红军。[hanhj@vrlab.buaa.edu.cn](mailto:hanhj@vrlab.buaa.edu.cn) ; [hongjun_han@163.com](mailto:hongjun_han@163.com)
原文出自：[https://www.captech.com.cn](https://www.captech.com.cn/)
英文原文：[http://www.xmailserver.org/linux-patches/nio-improve.html](http://www.xmailserver.org/linux-patches/nio-improve.html)
由于水平有限，错误在所难免，希望各位指正。 
07-01-2001 – 初稿 - Davide Libenzi <davidel@xmailserver.org> 
10-30-2002 – epoll补丁成为Linux内核一部分。请参考这个版本的，因为这个版本将会成为标准，并得到广泛支持Davide Libenzi <davidel@xmailserver.org> 
绪论： 
眼下的工作是分析不同的方法，这些方法都是用来实现从内核模式高效传递网络事件到用户模式。我们考察了五种方法：作为一种相对较好的老的方法poll ，标准/dev/poll接口，标准RT信号，RT signals with one-sig-per-fd patch，和使用很特别的通知方法的新式/dev/epoll。工作有如下四部分组成： 
1) 新式 /dev/epoll 内核补丁 
2) Provos-Lever修改的能够在内核 2.4.6 工作的/dev/poll 补丁 
3) HTTP server 
4) 一个能够产生“dead“连接的deadconn(tm)工具。 
Httperf被采用作为度量工具，尽管不完美，但也提供了足够的网络负载选项。 
新式 /dev/epoll 内核补丁： 
这个补丁很简单，它在struct file的数据结构里面添加了通知回调链表项。如下代码段： 
******* include/linux/fs.h 
struct file { 
... 
   /* 文件回调列表 */ 
   rwlock_t f_cblock; 
  struct list_head f_cblist; 
}; 
****include/linux/fcblist.h 
/* 文件回调通知事件 */ 
#define ION_IN          1 
#define ION_OUT         2 
#define ION_HUP         3 
#define ION_ERR         4 
#define FCB_LOCAL_SIZE  4 
#define fcblist_read_lock(fp, fl)    read_lock_irqsave(&(fp)->f_cblock, fl) 
#define fcblist_read_unlock(fp, fl)  read_unlock_irqrestore(&(fp)->f_cblock, fl) 
#define fcblist_write_lock(fp, fl)  write_lock_irqsave(&(fp)->f_cblock, fl) 
#define fcblist_write_unlock(fp,fl) write_unlock_irqrestore(&(fp)->f_cblock, fl) 
struct fcb_struct { 
        struct list_head lnk; 
        void (*cbproc)(struct file *, void *, unsigned long *, long *); 
        void *data; 
        unsigned long local[FCB_LOCAL_SIZE]; 
}; 
extern long ion_band_table[]; 
extern long poll_band_table[]; 
static inline void file_notify_init(struct file *filep) 
{ 
        rwlock_init(&filep->f_cblock); 
        INIT_LIST_HEAD(&filep->f_cblist); 
} 
void file_notify_event(struct file *filep, long *event); 
int file_notify_addcb(struct file *filep, 
  void (*cbproc)(struct file *, void *, unsigned long *, long *), void *data); 
int file_notify_delcb(struct file *filep, 
  void (*cbproc)(struct file *, void *, unsigned long *, long *)); 
void file_notify_cleanup(struct file *filep); 
这些回调方法链就是提供一种机制，向文件系统注册其“兴趣”的上层在兴趣事件发生时能够收到底层I/O的通知。初始化和清理代码已经加在了fs/file_table.c里，回调方法链处理代码在fs/fcblist.c里面，如下所示：
****** fs/file_table.c 
struct file * get_empty_filp(void) 
{ 
        ... 
        file_notify_init(f); 
        ... 
} 
int init_private_file(struct file *filp, struct dentry *dentry, int mode) 
{ 
        ... 
        file_notify_init(filp); 
        ... 
} 
void fput(struct file * file) 
{ 
        ... 
        file_notify_cleanup(file); 
        ... 
} 
  ****** fs/fcblist.c 
void file_notify_event(struct file *filep, long *event) 
{ 
        unsigned long flags; 
        struct list_head *lnk; 
        fcblist_read_lock(filep, flags); 
        list_for_each(lnk, &filep->f_cblist) { 
                struct fcb_struct *fcbp = list_entry(lnk, struct fcb_struct, lnk); 
                fcbp->cbproc(filep, fcbp->data, fcbp->local, event); 
        } 
        fcblist_read_unlock(filep, flags); 
} 
int file_notify_addcb(struct file *filep, 
                void (*cbproc)(struct file *, void *, unsigned long *, long *), void *data) 
{ 
        unsigned long flags; 
        struct fcb_struct *fcbp; 
        if (!(fcbp = (struct fcb_struct *) kmalloc(sizeof(struct fcb_struct), GFP_KERNEL))) 
                return -ENOMEM; 
        memset(fcbp, 0, sizeof(struct fcb_struct)); 
        fcbp->cbproc = cbproc; 
        fcbp->data = data; 
        fcblist_write_lock(filep, flags); 
        list_add_tail(&fcbp->lnk, &filep->f_cblist); 
        fcblist_write_unlock(filep, flags); 
        return 0; 
} 
int file_notify_delcb(struct file *filep, 
                void (*cbproc)(struct file *, void *, unsigned long *, long *)) 
{ 
        unsigned long flags; 
        struct list_head *lnk; 
        fcblist_write_lock(filep, flags); 
        list_for_each(lnk, &filep->f_cblist) { 
                struct fcb_struct *fcbp = list_entry(lnk, struct fcb_struct, lnk); 
                if (fcbp->cbproc == cbproc) { 
                        list_del(lnk); 
                        fcblist_write_unlock(filep, flags); 
                        kfree(fcbp); 
                        return 0; 
                } 
        } 
        fcblist_write_unlock(filep, flags); 
        return -ENOENT; 
} 
void file_notify_cleanup(struct file *filep) 
{ 
        unsigned long flags; 
        struct list_head *lnk; 
        fcblist_write_lock(filep, flags); 
        while ((lnk = list_first(&filep->f_cblist))) { 
                struct fcb_struct *fcbp = list_entry(lnk, struct fcb_struct, lnk); 
                list_del(lnk); 
                fcblist_write_unlock(filep, flags); 
                kfree(fcbp); 
                fcblist_write_lock(filep, flags); 
        } 
        fcblist_write_unlock(filep, flags); 
} 
这些回调函数会收到一个long *参数，其第一个元素是ION_*事件中的一种，下一个可以存储额外的参数，这些额外参数的意义随这第一个元素即事件种类的不同而不同。 
这个接口只是一个草案，我使用它只是验证传输方法是不是足够高效。目前通知只在socket文件出现，如下：
****** include/net/sock.h 
static inline void sk_wake_async(struct sock *sk, int how, int band) 
{ 
        if (sk->socket) { 
                if (sk->socket->file) { 
                        long event[] = { ion_band_table[band - POLL_IN], poll_band_table[band - POLL_IN], -1 }; 
                        file_notify_event(sk->socket->file, event); 
                } 
                if (sk->socket->fasync_list) 
                        sock_wake_async(sk->socket, how, band); 
        } 
} 
文件fs/pipe.c和include/linux/pipe_fs_i.h也都被修改以扩展/dev/epoll的功能至管道（pipes pipe() ) 
两个新的文件driver/char/eventpoll.c和include/linux/eventpoll.h实现了/dev/epoll。 
新的/dev/epoll接口和以前的有很大的不同，因为他只是通过设备文件描述符映射，而基于效率考虑放弃了拷贝数据到用户空间（copy-data-to-user-space），通过共享内存页来避免不必要的拷贝数据/dev/epoll可以高效运作，理由有1)更少的CPU周期，因为不必拷贝数据，2)在现代的缓存的存储体系架构下，有更少的内存占用（memory footprint）。 
/dev/epoll实现利用新的文件回调通知机制来注册回调方法，这些回调方法将事件存储事件缓冲区里，初始化顺序如下：
       if ((kdpfd = open("/dev/epoll", O_RDWR)) == -1) { 
        } 
        if (ioctl(kdpfd, EP_ALLOC, maxfds)) 
        { 
        } 
        if ((map = (char *) mmap(NULL, EP_MAP_SIZE(maxfds), PROT_READ, 
                        MAP_PRIVATE, kdpfd, 0)) == (char *) -1) 
        { 
        } 
maxfds 能够存储在轮询设备里面的最多的文件描述符数目，文件通过下面代码注册进兴趣集里面：
        struct pollfd pfd; 
        pfd.fd = fd;  
        pfd.events = POLLIN | POLLOUT | POLLERR | POLLHUP; 
        pfd.revents = 0; 
        if (write(kdpfd, &pfd, sizeof(pfd)) != sizeof(pfd)) { 
                ... 
        } 
下面代码删除指定的文件描述符：
     struct pollfd pfd; 
        pfd.fd = fd; 
        pfd.events = POLLREMOVE; 
        pfd.revents = 0; 
        if (write(kdpfd, &pfd, sizeof(pfd)) != sizeof(pfd)) { 
                ... 
        } 
核心分派（dispatch）代码类似下面：
       struct pollfd *pfds; 
        struct evpoll evp; 
        for (;;) { 
                evp.ep_timeout = STD_SCHED_TIMEOUT; 
                evp.ep_resoff = 0; 
                nfds = ioctl(kdpfd, EP_POLL, &evp); 
                pfds = (struct pollfd *) (map + evp.ep_resoff); 
                for (ii = 0; ii < nfds; ii++, pfds++) { 
                        ... 
                } 
        } 
驱动程序分配两个页面集来作为双缓冲机制，存储文件事件。可以从ep_resoff字段得知结果集在map里面的哪一块。在使用其中一个页面集的同时，内核可以使用另外一个存储进来的事件们。没有拷贝到用户空间的麻烦，来自同一个文件的事件将被放在一个槽(slot)里面，EP_POLL函数决不会线性查找兴趣集来执行file->f_ops->poll()。为了使用/dev/epoll接口，你必须使用mknod创建该文件，并且major=10 and minor=124。命令如下： 
# mknod /dev/epoll c 10 124 
下载补丁在下面的链接 
epoll-lt-2.4.24-0.20.diff 
Provos-Lever的/dev/poll补丁： 
这个值得说的不多，修改virt_to_page()这个漏洞就可以使补丁正常工作，我修改了另外一个bug,程序为了调节哈希表大小，而试图使用kmalloc()分配大块内存却不能满足，现在使用vmalloc()来为哈希表分配内存。我修改了在CITI网站上发现的2.4.3的补丁，这个补丁应该是Provos-Lever用来使（2.2.x）升级到2.4.x的那个。你可以在如下地址下载到： 
[http://www.xmailserver.org/linux-patches/olddp_last.diff.gz](http://www.xmailserver.org/linux-patches/olddp_last.diff.gz)
实时信号一个文件描述符一个信号（one-sig-per-fd）补丁： 
Vitaly Luban贡献的这个补丁实现了实时信号量，当实时信号量队列满了的时候，尽量避免SIGIO传输信息，你可以下载这个补丁如下网址： 
[http://www.luban.org/GPL/gpl.html](http://www.luban.org/GPL/gpl.html)
HTTP服务器： 
http服务器很简单，它使用的是事件轮询和同步程序库（coroutines），服务器内部的coroutine库的实现来自如下网址： 
[http://www.goron.de/~froese/coro/](http://www.goron.de/~froese/coro/)
它很小，简单且高效。服务器使用的默认堆栈大小为8192，当试图有许多连接时，将造成内存浪费和虚存颠簸（vm trashing），其实4096的堆栈大小就足够了。另外一个问题是coro库使用mmap()方法来为堆栈分配空间，这样当accept()/close()比率较高时会损失性能，我改了这个库（只有一个文件coro.c）让其使用malloc()/free()而不是mmap()/munmap().每次的Http应答都一样，应答的报文大小可以通过命令行参数设置，另外还有两个命令行参数可以设置监听端口和fd集的大小。你可以在如下地址下载到服务器： 
ephttpd-0.2.tar.gz 
老版本： 
[http://www.xmailserver.org/linux-patches/dphttpd_last.tar.gz](http://www.xmailserver.org/linux-patches/dphttpd_last.tar.gz)
死连接工具： 
如果说上面的服务器简单，这个就更简单了，它的目的就是给服务器制造死连接来模拟真实的负栽，这些连接都是一批的低速连接。你可以下载到： 
[http://www.xmailserver.org/linux-patches/deadconn_last.c](http://www.xmailserver.org/linux-patches/deadconn_last.c)
测试： 
测试环境：PIII 600MHz, 128 Mb RAM, eepro100网卡连接到100Mbps快速以太网交换机上。RH 6.2，使用的内核是2.4.6， coroutine 库的版本是1.1.0-pre2 
我使用双PIII 1GHz, 256 Mb RAM 和双eepro100作为httperf机器，双PIII 900 MHz, 256 Mb RAM 和双 eepro100作为deadconn(tm)机器，因为httperf当使用较高的num-conns时占用fds空间（改为8000了）很快消失，我采用了下面的命令行： 
--think-timeout 5 --timeout 5 --num-calls 2500 --num-conns 100 --hog --rate 100 
这个将分配100个连接，使服务器负栽不同的死连接。我改变的另一个参数是应答报文大小有128，512和1024。另外一个更加忠实于互联网会话就是使用一些突发连接发出HTTP请求，然后关闭，测试使用httperf，并有如下命令： 
--think-timeout 5 --timeout 5 --num-calls 2 --num-conns 27000 --hog --rate 5000 
每个数字都是求得三次的平均值，如下地址下载httperf： 
[http://www.hpl.hp.com/personal/David_Mosberger/httperf.html](http://www.hpl.hp.com/personal/David_Mosberger/httperf.html)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ljx0305/EntryImages/20090825/1.png)
测试显示/dev/epoll约比RT signals one-sig实现快10-12%，/dev/epoll和RT signals都保持平坦在不同的死连接负栽下。 
RT-one-sig实现比简单 实时信号实现要快一点， 但是本测试只有很少的 SIGIO. 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ljx0305/EntryImages/20090825/2.png)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ljx0305/EntryImages/20090825/3.png)
512和1024 Content-Length 的应答报文测试表明 /dev/epoll, RT signals和RT one-sig 表现基本一致，图中重叠在一块了， 这是因为在本测试中以太网达到了饱和态( 100Mbps )。 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/ljx0305/EntryImages/20090825/4.png)
这个测试表明/dev/epoll, RT signals 和RT one-sig实现在不同死连接情况下的每秒的Http应答报文一直平坦， 并且/dev/epoll大约15% 快于RT one-sig ，而RT one-sig 大约10-15% 快于简单 RT signals. 
系统调用接口( aka sys_epoll )： 
事件查询设备对于系统调用接口的需要促成了sys_epoll系统调用的实现，这个简单接口为开发人员实现了同样的可扩展性。新的系统调用引入了三个新的映射到用户空间的调用： 
int epoll_create(int maxfds); 
int epoll_ctl(int epfd, int op, int fd, unsigned int events); 
int epoll_wait(int epfd, struct pollfd *events, int maxevents, int timeout); 
这些函数在他们的手册页面里面描述： 
epoll : PSTXTMAN 
epoll_create : PS TXTMAN 
epoll_ctl : PSTXT MAN 
epoll_wait : PSTXT MAN 
实现这些系统调用的补丁在这儿. 一个新的访问epoll ( 2.5.45 )的库在这个可得到： epoll-lib-0.11.tar.gz 
一个简单基于管道的epoll性能测试: 
pipetest 
用户空间支持epoll的库 : 
libevent 
ivykis 
在测试epoll过程中，做的一个 thttpd 的补丁: 
thttpd.epoll.diff 
结论： 
这些数据显示/dev/epoll ( and sys_epoll )提高了服务器的效率（从应答报文速率和CPU利用率角度看）。/dev/epoll的应答速率完全独立于死连接的数目，而标准poll()和旧的/dev/poll似乎不可避免的受到影响。和标准poll()以及旧的/dev/poll相比/dev/epoll的方差也明显小的多，这不尽使我觉得(1) 将有跟多的处理能力(2)在高负栽下有可以预见的应答速率。RT signals和RT one-sig 是现在死连接变化时也基本保持不变，并且one-sig大约比简单RT signals快10-12% 。RT singnals实现 （即使 one-sig 较少)似乎不能适用于大量突发请求模拟真实因特网的情况，在这种情况下有大量连接处于active状态。这是因为RT signals队列的限制，即使打了one-sig的补丁， 也很容易填满队列。 
链接： 
[1] The epoll scalability page at lse. 
[2] David Weekly - /dev/epoll Page 
References: 
[1] W. Richard Stevens - "UNIX Network Programming, Volume I: Networking APIs: Sockets and XTI, 2nd edition" 
Prentice Hall, 1998. 
[2] W. Richard Stevens - "TCP/IP Illustrated, Volume 1: The Protocols" 
Addison Wesley professional computing series, 1994. 
[3] G. Banga and J. C. Mogul - "Scalable Kernel Performance for Internet Servers Under Realistic Load" 
Proceedings of the USENIX Annual Technical Conference, June 1998. 
[4] G. Banga. P. Druschel. J. C. Mogul - "Better Operating System Features for Faster Network Servers" 
SIGMETRICS Workshop on Internet Server Performance, June 1998. 
[5] G. Banga and P. Druschel - "Measuring the Capacity of a Web Server" 
Proceedings of the USENIX Symposium on Internet Technologies and Systems, December 1997. 
[6] Niels Provos and Charles Lever - "Scalable Network I/O in Linux" 
[http://www.citi.umich.edu/techreports/reports/citi-tr-00-4.pdf](http://www.citi.umich.edu/techreports/reports/citi-tr-00-4.pdf)
[7] Dan Kegel - "The C10K problem" 
[http://www.kegel.com/c10k.html](http://www.kegel.com/c10k.html)
[8] Richard Gooch - "IO Event Handling Under Linux" 
[http://www.atnf.csiro.au/~rgooch/linux/docs/io-events.html](http://www.atnf.csiro.au/~rgooch/linux/docs/io-events.html)
[9] Abhishek Chandra and David Mosberger - "Scalability of Linux Event-Dispatch Mechanisms" 
[http://www.hpl.hp.com/techreports/2000/HPL-2000-174.html](http://www.hpl.hp.com/techreports/2000/HPL-2000-174.html)
[10] Niels Provos and Charles Lever - "Analyzing the Overload Behaviour of a Simple Web Server" 
[http://www.citi.umich.edu/techreports/reports/citi-tr-00-7.ps.gz](http://www.citi.umich.edu/techreports/reports/citi-tr-00-7.ps.gz)
[11] D. Mosberger and T. Jin - "httperf -- A Tool for Measuring Web Server Performance" 
SIGMETRICS Workshop on Internet Server Performance, June 1998.
引用:http://www.myfaq.com.cn/2005September/2005-09-13/201081.html
