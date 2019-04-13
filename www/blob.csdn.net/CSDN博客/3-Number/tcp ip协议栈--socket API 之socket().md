
# tcp-ip协议栈--socket API 之socket() - 3-Number - CSDN博客


置顶2017年09月27日 14:18:21[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：352


# 0x01 缘由
前面学习了基本tcp/ip协议栈相关处理流程，学习这些主要目的是为加强对网络相关处理的熟练程度，指导将来相关网络设备开发、网络安全产品开发。除此之外，应该进一步关注高性能服务器相关开发，后面几个章节通过结合应用层socket相关API来进一步加强对linux tcp/ip协议栈的理解，同时加强应用层socket开发的理解。
前面有些博文已经提到了应用层socket对应的系统调用，现在进一步细化。
# 0x02 socket API介绍
```python
extern int socket (int __domain, int __type, int __protocol) __THROW;
extern int socketpair (int __domain, int __type, int __protocol,
		       int __fds[2]) __THROW;
extern int bind (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len)
     __THROW;
extern int getsockname (int __fd, __SOCKADDR_ARG __addr,
			socklen_t *__restrict __len) __THROW;
extern int connect (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len);
extern int getpeername (int __fd, __SOCKADDR_ARG __addr,
			socklen_t *__restrict __len) __THROW;
extern ssize_t send (int __fd, __const void *__buf, size_t __n, int __flags);
extern ssize_t recv (int __fd, void *__buf, size_t __n, int __flags);
extern ssize_t sendto (int __fd, __const void *__buf, size_t __n,
		       int __flags, __CONST_SOCKADDR_ARG __addr,
		       socklen_t __addr_len);
extern ssize_t recvfrom (int __fd, void *__restrict __buf, size_t __n,
			 int __flags, __SOCKADDR_ARG __addr,
			 socklen_t *__restrict __addr_len);
extern ssize_t sendmsg (int __fd, __const struct msghdr *__message,
			int __flags);
extern ssize_t recvmsg (int __fd, struct msghdr *__message, int __flags);
extern int getsockopt (int __fd, int __level, int __optname,
		       void *__restrict __optval,
		       socklen_t *__restrict __optlen) __THROW;
extern int setsockopt (int __fd, int __level, int __optname,
		       __const void *__optval, socklen_t __optlen) __THROW;
extern int listen (int __fd, int __n) __THROW;
extern int accept (int __fd, __SOCKADDR_ARG __addr,
		   socklen_t *__restrict __addr_len);
extern int accept4 (int __fd, __SOCKADDR_ARG __addr,
		    socklen_t *__restrict __addr_len, int __flags);
extern int shutdown (int __fd, int __how) __THROW;
```

# 0x03 glibc之socket
应用层调用glibc接口，glibc从接口调用到linux内核调用中。详细的分析过程参考：[https://wenku.baidu.com/view/5948b9acc1c708a1284a44af.html](https://wenku.baidu.com/view/5948b9acc1c708a1284a44af.html)分析得比较透彻。
# 0x04 跟踪socket系统调用（/usr/include/sys/socket.h）
函数原型： extern int socket (int __domain, int __type, int __protocol) __THROW;
参数说明：
domain：协议域，又称协议族（family）。常用的协议族有AF_INET、AF_INET6、AF_LOCAL（或称AF_UNIX，Unix域Socket）、AF_ROUTE等。协议族决定了socket的地址类型，在通信中必须采用对应的地址，如AF_INET决定了要用ipv4地址（32位的）与端口号（16位的）的组合、AF_UNIX决定了要用一个绝对路径名作为地址。
type：指定Socket类型。常用的socket类型有SOCK_STREAM、SOCK_DGRAM、SOCK_RAW、SOCK_PACKET、SOCK_SEQPACKET等。流式Socket（SOCK_STREAM）是一种面向连接的Socket，针对于面向连接的TCP服务应用。数据报式Socket（SOCK_DGRAM）是一种无连接的Socket，对应于无连接的UDP服务应用。
protocol：指定协议。常用协议有IPPROTO_TCP、IPPROTO_UDP、IPPROTO_STCP、IPPROTO_TIPC等，分别对应TCP传输协议、UDP传输协议、STCP传输协议、TIPC传输协议。
注意：1.type和protocol不可以随意组合，如SOCK_STREAM不可以跟IPPROTO_UDP组合。当第三个参数为0时，会自动选择第二个参数类型对应的默认协议。
返回值说明：成功则返回socket处理代码，失败返回-1。
错误代码： EPROTONOSUPPORT 参数domain指定的类型不支持参数type或protocol指定协议。
ENFILE        核心内存不足，无法建立新的socket结构。
EMFILE        进程文件表溢出，无法再建立新的socket。
EACCESS        权限不足，无法建立参数type或protocol指定协议。
ENOBUFS/ENOMEM   内存不足。
EINVAL       参数domain、type、protocol不合法。
## 4.1 运行服务端server
设定断点在socket调用处，单步跟踪相关操作。
socket的系统调用完成如下工作：
1根据参数family, type, protocol创建socket结构；
1.1根据协议类型创建对应的sock结构，然后sock结构根据协议类型配置相关回调函数，sock 与 socket结构关联做初始化；
2系统中创建文件与对应的socket关联映射；
## 4.2 调用过程分析
![](https://img-blog.csdn.net/20170927142443645?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
### 4.2.1 SYSCALL_DEFINE2
```python
/*
 *    系统调用向量
 */
SYSCALL_DEFINE2(socketcall, int, call, unsigned long __user *, args)
{
    unsigned long a[6];
    unsigned long a0, a1;
    int err;
    unsigned int len;
    if (call < 1 || call > SYS_ACCEPT4)
        return -EINVAL;
    len = nargs[call];
    if (len > sizeof(a))
        return -EINVAL;
    /* 用户空间复制相关参数 */
    if (copy_from_user(a, args, len))
        return -EFAULT;
    audit_socketcall(nargs[call] / sizeof(unsigned long), a);
    a0 = a[0];
    a1 = a[1];
    /* 根据call子调用号，来处理socket相关调用状态。*/
    switch (call) {
    case SYS_SOCKET:
        //系统调用，参数a0 = 2(AF_INET IP协议) ,a1 = 1（系统调用号 SYS_SOCKET） a[2] = 6(tcp)
        err = sys_socket(a0, a1, a[2]);
        break;
    case SYS_BIND:
        err = sys_bind(a0, (struct sockaddr __user *)a1, a[2]);
        break;
    /* 代码略 */
    default:
        err = -EINVAL;
        break;
    }
    return err;
}
```

### 4.2.2 SYSCALL_DEFINE3
```python
SYSCALL_DEFINE3(socket, int, family, int, type, int, protocol)
{
    int retval;
    struct socket *sock; //指针
    int flags;
 
    /* 做一些类型检查，标识检查 */
    BUILD_BUG_ON(SOCK_CLOEXEC != O_CLOEXEC);
    BUILD_BUG_ON((SOCK_MAX | SOCK_TYPE_MASK) != SOCK_TYPE_MASK);
    BUILD_BUG_ON(SOCK_CLOEXEC & SOCK_TYPE_MASK);
    BUILD_BUG_ON(SOCK_NONBLOCK & SOCK_TYPE_MASK);
    flags = type & ~SOCK_TYPE_MASK;
    if (flags & ~(SOCK_CLOEXEC | SOCK_NONBLOCK))
        return -EINVAL;
    type &= SOCK_TYPE_MASK;
    if (SOCK_NONBLOCK != O_NONBLOCK && (flags & SOCK_NONBLOCK))
        flags = (flags & ~SOCK_NONBLOCK) | O_NONBLOCK;
    /* 创建socket结构，最后一个位输出参数 */
    retval = sock_create(family, type, protocol, &sock);
    if (retval < 0)
        goto out;
    /* 创建文件描述符与socket做映射绑定 */
    retval = sock_map_fd(sock, flags & (O_CLOEXEC | O_NONBLOCK));
    if (retval < 0)
        goto out_release;
out:
    /* It may be already another descriptor 8) Not kernel problem. */
    return retval;
out_release:
    /* 映射失败，释放sock结构*/
    sock_release(sock);
    return retval;
}
```

### 4.2.3 __sock_create
```python
static int __sock_create(struct net *net, int family, int type, int protocol,
             struct socket **res, int kern)
{
    int err;
    struct socket *sock;
    const struct net_proto_family *pf;
    /*
     * 查看协议的范围
     */
    if (family < 0 || family >= NPROTO)
        return -EAFNOSUPPORT;
    if (type < 0 || type >= SOCK_MAX)
        return -EINVAL;
    /* 穿件sock结构 */
    sock = sock_alloc();
    if (!sock) {
        if (net_ratelimit())
            printk(KERN_WARNING "socket: no more sockets\n");
        return -ENFILE;    /* Not exactly a match, but its the
                   closest posix thing */
    }
    /* .......*/
    sock->type = type; //类型
    /* .......*/
    err = pf->create(net, sock, protocol);
    if (err < 0)
        goto out_module_put;
 /* .......*/
    *res = sock;
    return 0;
out_module_busy:
    err = -EAFNOSUPPORT;
out_module_put:
    sock->ops = NULL;
    module_put(pf->owner);
out_sock_release:
    sock_release(sock);
    return err;
out_release:
    rcu_read_unlock();
    goto out_sock_release;
}
```

### 4.2.4 inet_create
```python
/*
 *    创建一个inet socket，不同的协议有不同的调用函数，如unix_create等。
 */
static int inet_create(struct net *net, struct socket *sock, int protocol)
{
    struct sock *sk;
    struct inet_protosw *answer;
    struct inet_sock *inet;
    struct proto *answer_prot;
    unsigned char answer_flags;
    char answer_no_check;
    int try_loading_module = 0;
    int err;
    if (unlikely(!inet_ehash_secret))
        if (sock->type != SOCK_RAW && sock->type != SOCK_DGRAM)
            build_ehash_secret();
    sock->state = SS_UNCONNECTED; //未连接状态
    /* 查询请求协议 type/protocol 关联 */
lookup_protocol:
    err = -ESOCKTNOSUPPORT;  //初始化错误码
    rcu_read_lock();
    list_for_each_entry_rcu(answer, &inetsw[sock->type], list) {
        err = 0;
        /* Check the non-wild match. */
        if (protocol == answer->protocol) {
            if (protocol != IPPROTO_IP) //不是ip协议退出循环
                break;
        } else {
            /* Check for the two wild cases. */
            if (IPPROTO_IP == protocol) { //ip协议
                protocol = answer->protocol;//赋值协议号
                break;
            }
            if (IPPROTO_IP == answer->protocol)
                break;
        }
        err = -EPROTONOSUPPORT; //状态！=0
    }
    if (unlikely(err)) {
        if (try_loading_module < 2) {
            rcu_read_unlock();
            /*
             * Be more specific, e.g. net-pf-2-proto-132-type-1
             * (net-pf-PF_INET-proto-IPPROTO_SCTP-type-SOCK_STREAM)
             */
            if (++try_loading_module == 1)
                request_module("net-pf-%d-proto-%d-type-%d",
                           PF_INET, protocol, sock->type);
            /*
             * Fall back to generic, e.g. net-pf-2-proto-132
             * (net-pf-PF_INET-proto-IPPROTO_SCTP)
             */
            else
                request_module("net-pf-%d-proto-%d",
                           PF_INET, protocol);
            goto lookup_protocol;
        } else
            goto out_rcu_unlock;
    }
    err = -EPERM;
    if (answer->capability > 0 && !capable(answer->capability))
        goto out_rcu_unlock;
    err = -EAFNOSUPPORT;
    if (!inet_netns_ok(net, protocol))
        goto out_rcu_unlock;
    sock->ops = answer->ops; //对应协议的相关操作函数。
    answer_prot = answer->prot;
    answer_no_check = answer->no_check;
    answer_flags = answer->flags;
    rcu_read_unlock();
    WARN_ON(answer_prot->slab == NULL);
    err = -ENOBUFS;
    sk = sk_alloc(net, PF_INET, GFP_KERNEL, answer_prot); //分配sock
    if (sk == NULL)
        goto out;
    err = 0;
    sk->sk_no_check = answer_no_check;
    if (INET_PROTOSW_REUSE & answer_flags)
        sk->sk_reuse = 1;
    inet = inet_sk(sk); //分配inet_sk
    inet->is_icsk = (INET_PROTOSW_ICSK & answer_flags) != 0;
    if (SOCK_RAW == sock->type) {
        inet->num = protocol;
        if (IPPROTO_RAW == protocol)
            inet->hdrincl = 1;
    }
    if (ipv4_config.no_pmtu_disc)
        inet->pmtudisc = IP_PMTUDISC_DONT;
    else
        inet->pmtudisc = IP_PMTUDISC_WANT;
    inet->id = 0;
    sock_init_data(sock, sk);//初始化相关结构
    sk->sk_destruct       = inet_sock_destruct;
    sk->sk_protocol       = protocol;
    sk->sk_backlog_rcv = sk->sk_prot->backlog_rcv; //后配接收队列
    inet->uc_ttl    = -1;
    inet->mc_loop    = 1;
    inet->mc_ttl    = 1;
    inet->mc_all    = 1;
    inet->mc_index    = 0;
    inet->mc_list    = NULL;
    sk_refcnt_debug_inc(sk);
    if (inet->num) {
        /* It assumes that any protocol which allows
         * the user to assign a number at socket
         * creation time automatically
         * shares.
         */
        inet->sport = htons(inet->num);
        /* Add to protocol hash chains. */
        sk->sk_prot->hash(sk);
    }
    if (sk->sk_prot->init) {
        err = sk->sk_prot->init(sk); //tcp_v4_init_sock 调用
        if (err)
            sk_common_release(sk);
    }
out:
    return err;
out_rcu_unlock:
    rcu_read_unlock();
    goto out;
}
```

### 4.2.5 sock_init_data
```python
void sock_init_data(struct socket *sock, struct sock *sk)
{
    skb_queue_head_init(&sk->sk_receive_queue); //初始化接收队列
    skb_queue_head_init(&sk->sk_write_queue);     //初始化发送队列
    skb_queue_head_init(&sk->sk_error_queue);        //初始化错误队列
#ifdef CONFIG_NET_DMA
    skb_queue_head_init(&sk->sk_async_wait_queue);    //初始化异步等待队列
#endif
    sk->sk_send_head    =    NULL;  //发送队列头置空
    init_timer(&sk->sk_timer); //初始化定时器
    sk->sk_allocation    =    GFP_KERNEL; //内核内存的正常分配. 可能睡眠.
    sk->sk_rcvbuf        =    sysctl_rmem_default; //可以用sysctl -a | grep mem 查看
    sk->sk_sndbuf        =    sysctl_wmem_default;
    sk->sk_state        =    TCP_CLOSE; //状态机初始状态
    sk_set_socket(sk, sock); //    sk->sk_socket = sock;
    sock_set_flag(sk, SOCK_ZAPPED);//设定标识
    if (sock) {
        sk->sk_type    =    sock->type; //socket类型
        sk->sk_sleep    =    &sock->wait; //等待队列
        sock->sk    =    sk;
    } else
        sk->sk_sleep    =    NULL;
    rwlock_init(&sk->sk_dst_lock);//读写锁初始化
    rwlock_init(&sk->sk_callback_lock);
    lockdep_set_class_and_name(&sk->sk_callback_lock,
            af_callback_keys + sk->sk_family,
            af_family_clock_key_strings[sk->sk_family]);
    sk->sk_state_change    =    sock_def_wakeup; //socket状态改变
    sk->sk_data_ready    =    sock_def_readable;  //socket有数据可读
    sk->sk_write_space    =    sock_def_write_space; //写空间
    sk->sk_error_report    =    sock_def_error_report; //错误
    sk->sk_destruct        =    sock_def_destruct; //析构
    sk->sk_sndmsg_page    =    NULL; //发送页，在发送过程中用到
    sk->sk_sndmsg_off    =    0; //发消息偏移
    sk->sk_peercred.pid     =    0; //进程id
    sk->sk_peercred.uid    =    -1;  //用户id
    sk->sk_peercred.gid    =    -1;  //组id
    sk->sk_write_pending    =    0;
    sk->sk_rcvlowat        =    1;
    sk->sk_rcvtimeo        =    MAX_SCHEDULE_TIMEOUT; //超时时间
    sk->sk_sndtimeo        =    MAX_SCHEDULE_TIMEOUT;//超时时间
    sk->sk_stamp = ktime_set(-1L, 0); //时间戳
    /*
     * Before updating sk_refcnt, we must commit prior changes to memory
     * (Documentation/RCU/rculist_nulls.txt for details)
     */
    smp_wmb();
    atomic_set(&sk->sk_refcnt, 1);
    atomic_set(&sk->sk_drops, 0);
}
```
![](https://img-blog.csdn.net/20170927142541896?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
### 4.2.6 tcp_v4_init_sock
```python
static int tcp_v4_init_sock(struct sock *sk)
{
    struct inet_connection_sock *icsk = inet_csk(sk); //创建连接结构
    struct tcp_sock *tp = tcp_sk(sk); //创建tcp_sock结构
    skb_queue_head_init(&tp->out_of_order_queue); //初始化短信队列
    tcp_init_xmit_timers(sk); //初始化发送定时器
    tcp_prequeue_init(tp); //初始prequeue队列
    icsk->icsk_rto = TCP_TIMEOUT_INIT; //超时初始化
    tp->mdev = TCP_TIMEOUT_INIT;
    tp->snd_cwnd = 2; //滑动窗口
    tp->snd_ssthresh = TCP_INFINITE_SSTHRESH;
    tp->snd_cwnd_clamp = ~0;
    tp->mss_cache = 536; //mss
    tp->reordering = sysctl_tcp_reordering; //排序包数net.ipv4.tcp_reordering = 3
    icsk->icsk_ca_ops = &tcp_init_congestion_ops; //拥塞处理
    sk->sk_state = TCP_CLOSE; //tcp初始状态
    sk->sk_write_space = sk_stream_write_space; //流写的空间
    sock_set_flag(sk, SOCK_USE_WRITE_QUEUE); //用写的队列
    icsk->icsk_af_ops = &ipv4_specific; //操作函数
    icsk->icsk_sync_mss = tcp_sync_mss; //mss
#ifdef CONFIG_TCP_MD5SIG
    tp->af_specific = &tcp_sock_ipv4_specific;
#endif
    sk->sk_sndbuf = sysctl_tcp_wmem[1]; //发送缓存
    sk->sk_rcvbuf = sysctl_tcp_rmem[1]; //接收缓存
    local_bh_disable();
    percpu_counter_inc(&tcp_sockets_allocated);
    local_bh_enable();
    return 0;
}
```
![](https://img-blog.csdn.net/20170927142624130?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

# 0x05 总结
此处是创建相关结构，此处创建的结构后续怎么关联怎么用？难道是文件关联的？ 这个疑问留到下次分析（学习过程，大神勿喷，多指点）


