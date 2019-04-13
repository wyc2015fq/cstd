
# tcp-ip协议栈--socket API 之connect() - 3-Number - CSDN博客


2017年09月29日 15:37:08[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：203标签：[connect																](https://so.csdn.net/so/search/s.do?q=connect&t=blog)[inet_stream_connect																](https://so.csdn.net/so/search/s.do?q=inet_stream_connect&t=blog)[tcp_connect																](https://so.csdn.net/so/search/s.do?q=tcp_connect&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=inet_stream_connect&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=connect&t=blog)个人分类：[Linux 网络协议栈																](https://blog.csdn.net/pangyemeng/article/category/7130138)
[
																								](https://so.csdn.net/so/search/s.do?q=connect&t=blog)


# 0x01 缘由
前面一些博文简单学习了服务端 socket（）、bind（）、listen（）、accept（)过程，当没有客户端连接服务端时，服务端进程阻塞在accept处，等待客户端连接。客户端连接服务端利用connect（） API。此处学习。
# 0x02 API介绍
![](https://img-blog.csdn.net/20170929153818853?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 0x03 源码单步跟踪
在另一台服务器运行server端，在guest主机运行client，host主机在源码中设置断点。
![](https://img-blog.csdn.net/20170929153850438?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170929153932788?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


## 3.1 SYSCALL_DEFINE2
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
     ......
        case SYS_CONNECT:
            err = sys_connect(a0, (struct sockaddr __user *)a1, a[2]);
            break;
    .......
    default:
        err = -EINVAL;
        break;
    }
    return err;
}
```

## 3.2 SYSCALL_DEFINE3
```python
/*
 *  尝试连接服务器，对用户态地址进行校验然后移到内地址空间。
 */
SYSCALL_DEFINE3(connect, int, fd, struct sockaddr __user *, uservaddr,
        int, addrlen)
{
    struct socket *sock;
    struct sockaddr_storage address;
    int err, fput_needed;
    /* 根据文件描述符fd查询相关结构 */
    sock = sockfd_lookup_light(fd, &err, &fput_needed);
    if (!sock)
        goto out;
    /* 将地址移到内核地址空间 */
    err = move_addr_to_kernel(uservaddr, addrlen, (struct sockaddr *)&address);
    if (err < 0)
        goto out_put;
    /* SELinux相关 */
    err = security_socket_connect(sock, (struct sockaddr *)&address, addrlen);
    if (err)
        goto out_put;
    /* tcp协议 调用 inet_stream_connect*/
    err = sock->ops->connect(sock, (struct sockaddr *)&address, addrlen,
                 sock->file->f_flags);
out_put:
    fput_light(sock->file, fput_needed);
out:
    return err;
}
```

## 3.3 inet_stream_connect
```python
/*
 *    连接远程主机
 */
int inet_stream_connect(struct socket *sock, struct sockaddr *uaddr,
            int addr_len, int flags)
{
    struct sock *sk = sock->sk;
    int err;
    long timeo;
    /* 对相关结构上锁 */
    lock_sock(sk);
    /* 协议族不存在 */
    if (uaddr->sa_family == AF_UNSPEC) {
        err = sk->sk_prot->disconnect(sk, flags);
        sock->state = err ? SS_DISCONNECTING : SS_UNCONNECTED;
        goto out;
    }
    /* 客户端sock状态 */
    switch (sock->state) {
    default:
        err = -EINVAL;
        goto out;
    case SS_CONNECTED: //连接状态，显然是异常
        err = -EISCONN;
        goto out;
    case SS_CONNECTING: //正在连接
        err = -EALREADY;
        /* Fall out of switch with err, set for this state */
        break;
    case SS_UNCONNECTED: //未连接
        err = -EISCONN;
        if (sk->sk_state != TCP_CLOSE) //不能等于TCP_CLOSE
            goto out;
        /* 调用下面要介绍的 tcp_v4_connect 函数。*/
        err = sk->sk_prot->connect(sk, uaddr, addr_len);
        if (err < 0)
            goto out;
        /*只是进入 SS_CONNECTING 状态，和 SS_CONNECTED 唯一的区别是在非阻塞的情况下返回值是EINPROGRESS，而不是 EALREADY*/
        sock->state = SS_CONNECTING;
        err = -EINPROGRESS;
        break;
    }
    timeo = sock_sndtimeo(sk, flags & O_NONBLOCK);
    if ((1 << sk->sk_state) & (TCPF_SYN_SENT | TCPF_SYN_RECV)) {
        /* 如果是TCPF_SYN_SENT 则等待服务端响应 ack + syn  */
        if (!timeo || !inet_wait_for_connect(sk, timeo))
            goto out;
        err = sock_intr_errno(timeo);
        if (signal_pending(current))
            goto out;
    }
    /* 连接被 RST, 超时， ICMP 错误或者另外的进程关闭*/
    if (sk->sk_state == TCP_CLOSE)
        goto sock_error;
    /*到这里连接就是 ed 状态了*/
    sock->state = SS_CONNECTED;
    err = 0;
out:
    release_sock(sk);
    return err;
sock_error:
    err = sock_error(sk) ? : -ECONNABORTED;
    sock->state = SS_UNCONNECTED;
    if (sk->sk_prot->disconnect(sk, flags))
        sock->state = SS_DISCONNECTING;
    goto out;
}
```

## 3.4 tcp_v4_connect
```python
/* This will initiate an outgoing connection. */
int tcp_v4_connect(struct sock *sk, struct sockaddr *uaddr, int addr_len)
{
    struct inet_sock *inet = inet_sk(sk); //变量inet指向套接字struct sock中的inet选项
    struct tcp_sock *tp = tcp_sk(sk); //变量tp指向套接字struct sock中的TCP选项
    struct sockaddr_in *usin = (struct sockaddr_in *)uaddr; //将通用地址结构转换为IPv4的地址结构
    struct rtable *rt;  //记录路由表项
    __be32 daddr, nexthop; //daddr记录目的地址，nesthop记录下一条地址
    int tmp;
    int err;
    /* 判断地址长度是否合法，应该要大于或者等于其地址的长度 */
    if (addr_len < sizeof(struct sockaddr_in))
        return -EINVAL;
 //判断是否为inet协议族
    if (usin->sin_family != AF_INET)
        return -EAFNOSUPPORT;
    //初始化目的地址和下一条地址
    nexthop = daddr = usin->sin_addr.s_addr;
    if (inet->opt && inet->opt->srr) {
        if (!daddr)
            return -EINVAL;
        nexthop = inet->opt->faddr;
    }
  /* 查询路由表 */
    tmp = ip_route_connect(&rt, nexthop, inet->saddr,
                   RT_CONN_FLAGS(sk), sk->sk_bound_dev_if,
                   IPPROTO_TCP,
                   inet->sport, usin->sin_port, sk, 1);
    if (tmp < 0) {
        if (tmp == -ENETUNREACH)
            IP_INC_STATS_BH(sock_net(sk), IPSTATS_MIB_OUTNOROUTES);
        return tmp;
    }
    /* 不能是组播路由或广播路由 */
    if (rt->rt_flags & (RTCF_MULTICAST | RTCF_BROADCAST)) {
        ip_rt_put(rt);
        return -ENETUNREACH;
    }
    if (!inet->opt || !inet->opt->srr)
        daddr = rt->rt_dst; //如果源地址为0，则把rt_ src赋给源地址
    /* 如果报文没有带源地址，就用路由表项里的源地址*/
    if (!inet->saddr)
        inet->saddr = rt->rt_src;
    inet->rcv_saddr = inet->saddr;
    //初始化TCP选项
    if (tp->rx_opt.ts_recent_stamp && inet->daddr != daddr) {
        /* Reset inherited state */
        tp->rx_opt.ts_recent       = 0;
        tp->rx_opt.ts_recent_stamp = 0;
        tp->write_seq           = 0;
    }
    if (tcp_death_row.sysctl_tw_recycle &&
        !tp->rx_opt.ts_recent_stamp && rt->rt_dst == daddr) {
        struct inet_peer *peer = rt_get_peer(rt);
        /*
         * VJ's idea. We save last timestamp seen from
         * the destination in peer table, when entering state
         * TIME-WAIT * and initialize rx_opt.ts_recent from it,
         * when trying new connection.
         */
        if (peer != NULL &&
            peer->tcp_ts_stamp + TCP_PAWS_MSL >= get_seconds()) {
            tp->rx_opt.ts_recent_stamp = peer->tcp_ts_stamp;
            tp->rx_opt.ts_recent = peer->tcp_ts;
        }
    }
    inet->dport = usin->sin_port; //目的端口地址
    inet->daddr = daddr; //目的IP地址
    inet_csk(sk)->icsk_ext_hdr_len = 0;
    if (inet->opt)
        inet_csk(sk)->icsk_ext_hdr_len = inet->opt->optlen; //inet的opt选项的长度
    tp->rx_opt.mss_clamp = 536;
    /* Socket identity is still unknown (sport may be zero).
     * However we set state to SYN-SENT and not releasing socket
     * lock select source port, enter ourselves into the hash tables and
     * complete initialization after this.
     */
     /*把套接字结构sk的状态置为TCP_SYN_SENT,到此 socket 的二元组还没有唯一确定，因为 sport 可能是 0，但是我们会设置 socket 的状态
为 SYN-SENT，并且不会*/
    tcp_set_state(sk, TCP_SYN_SENT);
    /*为套接字绑定一个端口，并记录在TCP的哈希表中,tcp_death_row 是一个全局变量，它有一个指向 tcp_hashinfo 的指针，这个函数会用到它*/
    err = inet_hash_connect(&tcp_death_row, sk);
    if (err)
        goto failure;
    /*根据我们本地端口和端口是否与路由查询表中的相同来决定新建一个路由键值，并且调整路由表。*/
    err = ip_route_newports(&rt, IPPROTO_TCP,
                inet->sport, inet->dport, sk);
    if (err)
        goto failure;
    /* 现在提交socket  */
    sk->sk_gso_type = SKB_GSO_TCPV4;
    sk_setup_caps(sk, &rt->u.dst);
    /*生成一个序列号，用作将来的 3 次握手协议*/
    if (!tp->write_seq)
        tp->write_seq = secure_tcp_sequence_number(inet->saddr,
                               inet->daddr,
                               inet->sport,
                               usin->sin_port);
    inet->id = tp->write_seq ^ jiffies;
    /* tcp 连接 */
    err = tcp_connect(sk);
    rt = NULL;
    if (err)
        goto failure;
    return 0;
failure:
    /*如果连接失败，则需把套接字状态设置为：TCP_CLOSE*/
    tcp_set_state(sk, TCP_CLOSE);
    ip_rt_put(rt);
    sk->sk_route_caps = 0;
    inet->dport = 0;
    return err;
}
```

## 3.5 tcp_connect
```python
/* 构建一个syn包然后发送去 */
int tcp_connect(struct sock *sk)
{
    struct tcp_sock *tp = tcp_sk(sk); //tcp选项
    struct sk_buff *buff; //缓存指针
    tcp_connect_init(sk); //初始化
    //分配一个缓存
    buff = alloc_skb_fclone(MAX_TCP_HEADER + 15, sk->sk_allocation);
    if (unlikely(buff == NULL))
        return -ENOBUFS;
    /* 保留头部空间. */
    skb_reserve(buff, MAX_TCP_HEADER);
    tp->snd_nxt = tp->write_seq; //序号
    tcp_init_nondata_skb(buff, tp->write_seq++, TCPCB_FLAG_SYN); //syn 非数据标识
    TCP_ECN_send_syn(sk, buff); //封装syn包
    /* Send it off. */
    TCP_SKB_CB(buff)->when = tcp_time_stamp; //时间搓
    tp->retrans_stamp = TCP_SKB_CB(buff)->when;
    skb_header_release(buff);
    __tcp_add_write_queue_tail(sk, buff); //加入到发送队列
    sk->sk_wmem_queued += buff->truesize; //buffer的真正大小
    sk_mem_charge(sk, buff->truesize);
    tp->packets_out += tcp_skb_pcount(buff);
    tcp_transmit_skb(sk, buff, 1, sk->sk_allocation); //发送
    /* We change tp->snd_nxt after the tcp_transmit_skb() call
     * in order to make this packet get counted in tcpOutSegs.
     */
    tp->snd_nxt = tp->write_seq; //下一个序号
    tp->pushed_seq = tp->write_seq;  //下一个序号
    TCP_INC_STATS(sock_net(sk), TCP_MIB_ACTIVEOPENS);
    /* 定时发送，直到有响应 */
    inet_csk_reset_xmit_timer(sk, ICSK_TIME_RETRANS,
                  inet_csk(sk)->icsk_rto, TCP_RTO_MAX);
    return 0;
}
```

# 0x04 总结
到此将客户端客户端和服务端连接过程全部学习完，tcp数据的发送和接收过程前期也有涉及，但未介绍socket API ，下面介绍API。（学习过程，大神勿喷，多指点）。

