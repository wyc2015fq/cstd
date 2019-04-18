# select poll epoll系统调用详细分析 - 爱拍凯特的专栏 - CSDN博客
2016年07月02日 11:39:16[爱拍凯特](https://me.csdn.net/apacat)阅读数：905
1、select，poll，epoll都是多路复用IO的函数，简单说就是在一个线程里，可以同时处理多个文件描述符的读写。
select/poll的实现很类似，epoll是从select/poll扩展而来，主要是为了解决select/poll天生的缺陷。
epoll在内核版本2.6以上才出现的新的函数，而他们在linux内核中的实现都是十分相似。
这三种函数都需要设备驱动提供poll回调函数，对于套接字而言，他们是 tcp_poll，udp_poll和datagram_poll;
2、select系统调用相关细节（2.6的内核，其他版本的内核，应该都相差不多）
下图是select和poll在调用过程中，会使用到的内核函数，以及这写内核函数的作用。
![](https://img-blog.csdn.net/20160702110943059?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
简单来说就是：
应用程序调用select，进入内核调用sys_select，做些简单初始化工作，接着进入 core_sys_select，此函数主要工作是把描述符集合从用户空间复制到内核空间，
 最终进入do_select，完成其主要的功能。do_select里，调用 poll_initwait，主要工作是注册poll_wait的回调函数为__pollwait，当在设备驱动的poll回调函数里调用poll_wait，其实就是调用__pollwait， __pollwait的主要工作是把当前进程挂载到等待队列里，当等待的事件到来就会唤醒此进程。接着执行for循环，循环里首先遍历每个文件描述符，调用对应描述符的poll回调函数，检测是否就绪，遍历完所有描述符之后，只要有描述符处于就绪状态,信号中断,出错或者超时，就退出循环，否则会调用schedule_xxx函数，让当前进程睡眠，一直到超时或者有描述符就绪被唤醒。接着又会再次遍历每个描述符，调用poll再次检测。如此循环，直到符合条件才会退出。
3、poll的系统调用相关细节
下图是poll调用的相关内核函数：
![](https://img-blog.csdn.net/20160702111342487?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
其中do_pollfd是对每个描述符调用 其回调poll状态轮训。
poll比select的好处就是没有描述多少限制，select 有1024 的限制，描述符不能超过此值，poll不受限制。
**我们从上面分析，可以总结出select/poll天生的缺陷：**
1）每次调用select/poll都需要要把描述符集合从用户空间copy到内核空间，检测完成之后，又要把检测的结果集合从内核空间copy到用户空间。当描述符很多，而且select经常被唤醒，这种开销会比较大。
2）如果说描述符集合来回复制不算什么，那么**多次**的**全部描述符遍历**就比较恐怖了，我们在应用程序中，每次调用select/poll 都必须首先遍历描述符，把他们加到fd_set集合里，这是应用层的第一次遍历，接着进入内核空间，至少进行一次遍历和调用每个描述符的poll回调检测，一般可能是2次遍历，第一次没发现就绪描述符，加入等待队列，第二次是被唤醒，接着再遍历一遍。再回到应用层，我们还必须再次遍历所有描述符，用 FD_ISSET检测结果集。如果描述符很多，这种遍历就很消耗CPU资源了。
![](https://img-blog.csdn.net/20160702113023484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3）描述符多少限制，当然poll没有限制，select却有1024的硬性限制，除了修改内核增加1024限制外没别的办法。
既然有这么些缺点 ，那不是 select/poll变得一无是处了，那就大错特错了。他们依然是代码移植的最好函数，因为几乎所有平台都有对它们的实现提供接口。在描述符不是太多，他们依然十分出色的完成多路复用IO， 而且如果每个连接上的描述符都处于活跃状态，他们的效率其实跟epoll也差不了多少。曾经使用多个线程+每个线程采用poll的办法开发TCP服务器，处理文件收发，连接达到几千个，当时的瓶颈已经不在网络IO，而在磁盘IO了。
3、epoll系统调用相关细节(图好像有点问题.....)
![](https://img-blog.csdn.net/20160702171826914?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
epoll只是select/poll的扩展，他不是在linux内核中另起炉灶，做颠覆性的设计的，他只是在select的基础上来解决他们的缺陷。**他的底层依然需要设备驱动提供poll回调来作为状态检测基础。**
epoll分为三个函数 epoll_create,epoll_ctl, epoll_wait 、他们的实现在 eventpoll.c代码里。
epoll_create创建epoll描述符，用来管理所有添加进去的描述符，epoll_ctl
 用来添加，修改或者删除描述符。epoll_wait等待描述符事件。
epoll_wait的等待已经不再是轮训方式的等待了，epoll内部有个描述符就绪队列，epoll_wait只检测这个队列即可，他采用睡眠一会检测一下的方式，如果发现描述符就绪队列不为空，就把此队列中的描述符copy到用户空间，然后返回。
描述符就绪队列里的数据又是从何而来的？
原来使用 epoll_ctl添加新描述符时候，epoll_ctl内核实现里会修改两个回调函数，
一个是 poll_table结构里的qproc回调函数指针，在 select中是 __pollwait函数，在epoll中换成 ep_ptable_queue_proc，当在epoll_ctl中调用新添加的描述符的poll回调时候，底层驱动就会调用
 poll_wait添加等待队列，底层驱动调用poll_wait时候，其实就是调用ep_ptable_queue_proc，此函数会修改等待队列的回调函数为 ep_poll_callback， 并加入到等待队列头里；一旦底层驱动发现数据就绪，就会调用wake_up唤醒等待队列，从而 ep_poll_callback将被调用，在ep_poll_callback中
 会把这个就绪的描述符添加到 epoll的描述符就绪队列里，并同时唤醒 epoll_wait 所在的进程。如此这般，就是epoll的内核实现的精髓。看他是如何解决 select/poll的缺陷的， 首先他通过 epoll_ctl的EPOLL_CTL_ADD命令把描述符添加进epoll内部管理器里，只需添加一次即可，直到用
 epoll_ctl的EPOLL_CTL_DEL命令删除此描述符为止，而不像select/poll是每次执行都必须添加，很显然大量减少了描述符在内核和用户空间不断的来回copy的开销。其次虽然 epoll_wait内部也是循环检测，但是它只需检测描述符就绪队列是否为空即可，比起select/poll必须轮训每个描述符的poll，其开销简直可以忽略不计。他同时也没描述符多少的限制，只要你机器的内存够大，就能容纳非常多的描述符。

