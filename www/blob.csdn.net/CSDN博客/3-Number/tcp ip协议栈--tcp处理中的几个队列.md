
# tcp-ip协议栈--tcp处理中的几个队列 - 3-Number - CSDN博客


2017年09月25日 10:43:38[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：865


# 0x01 缘由
昨天简单过了下tcp层的syn的过程，其中对几个队列处理有点疑问，对数据如何到用户态的过程也存在的疑问？带着这几个问题网上查找了相关资料，发现一位大神讲解得比较清楚。传送：[http://blog.csdn.net/russell_tao/article/details/9950615](http://blog.csdn.net/russell_tao/article/details/9950615)
讲解得比较到位，博文编写也是我后面要改进的方向。原理已经讲解得比较清楚，下面主要做一下单步跟踪分析。
# 0x02 单步跟踪分析
## 1.recv调用
![](https://img-blog.csdn.net/20170925104336177?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### 1.1 recv调用过程
```python
SYSCALL_DEFINE6(recvfrom, int, fd, void __user *, ubuf, size_t, size,
        unsigned, flags, struct sockaddr __user *, addr,
        int __user *, addr_len)
{
    .......
    //根据fd找对象的socket结构
    sock = sockfd_lookup_light(fd, &err, &fput_needed);
    if (!sock)
        goto out;
    //相关结构填充，io地址等
    msg.msg_control = NULL;
    msg.msg_controllen = 0;
    msg.msg_iovlen = 1;
    msg.msg_iov = &iov;
    iov.iov_len = size;
    iov.iov_base = ubuf;
    msg.msg_name = (struct sockaddr *)&address;
    msg.msg_namelen = sizeof(address);
    //阻塞与非阻塞调用
    if (sock->file->f_flags & O_NONBLOCK)
        flags |= MSG_DONTWAIT;
    //sock接收消息
    err = sock_recvmsg(sock, &msg, size, flags);
    .......
}
```

### 1.2 sock_recvmsg调用过程
```python
int sock_recvmsg(struct socket *sock, struct msghdr *msg,
         size_t size, int flags)
{
    //相关io操作，这里还不太明白
    struct kiocb iocb;
    struct sock_iocb siocb;
    int ret;
    init_sync_kiocb(&iocb, NULL);
    iocb.private = &siocb;
    //继续调用
    ret = __sock_recvmsg(&iocb, sock, msg, size, flags);
    if (-EIOCBQUEUED == ret)
        ret = wait_on_sync_kiocb(&iocb);
    return ret;
}
```

### 1.3 sock_recvmsg调用过程
```python
static inline int __sock_recvmsg(struct kiocb *iocb, struct socket *sock,
                 struct msghdr *msg, size_t size, int flags)
{
    int err;
    struct sock_iocb *si = kiocb_to_siocb(iocb);
    si->sock = sock;
    si->scm = NULL;
    si->msg = msg;
    si->size = size;
    si->flags = flags;
    err = security_socket_recvmsg(sock, msg, size, flags);
    if (err)
        return err;
    //根据sock回调指针情况执行，有inet\unix\netlink。tcp则调用tcp.c中tcp_recvmsg函数。这里就是我传送的那篇文章内容；
    return sock->ops->recvmsg(iocb, sock, msg, size, flags);
}
```

# 0x03 队列分析
上层应用调用和底层tcp收包通过队列进行异步缓冲等解耦。所以通过多种队列来进行各种场景下的数据缓冲。下面根据对tcp_recvmsg的分析来处理。
```python
/*
 *    这个函数是将sock结构拷贝到用户的缓冲区。
 * 参数：len对应read、recv方法里的内存长度，flags对应方法的flags参数，nonblock则是阻塞、非阻塞标志位
 */
int tcp_recvmsg(struct kiocb *iocb, struct sock *sk, struct msghdr *msg,
        size_t len, int nonblock, int flags, int *addr_len)
{
    struct tcp_sock *tp = tcp_sk(sk);//对应结构
    ......
    //锁住socket，防止多进程并发访问TCP连接，告知软中断目前socket在进程上下文中。
    lock_sock(sk);
    TCP_CHECK_TIMER(sk);
    //初始化errno这个错误码
    err = -ENOTCONN;
    //sock状态监听状态，还未建立连接，所以释放目前处理sock。
    if (sk->sk_state == TCP_LISTEN)
        goto out;
    //如果socket是阻塞套接字，则取出SO_RCVTIMEO作为读超时时间；若为非阻塞，则timeo为0。下面会看到timeo是如何生效的
    timeo = sock_rcvtimeo(sk, nonblock);
    /* 带有URG的紧急数据特殊处理*/
    if (flags & MSG_OOB)
        goto recv_urg;
    //获取下一个要拷贝的字节序号 
    //注意：seq的定义为u32 *seq;，它是32位指针。为何？因为下面每向用户态内存拷贝后，会更新seq的值，这时就会直接更改套接字上的copied_seq 
    seq = &tp->copied_seq;
    //当flags参数有MSG_PEEK标志位时，意味着这次拷贝的内容，当再次读取socket时（比如另一个进程）还能再次读到
    if (flags & MSG_PEEK) {
        //所以不会更新copied_seq，当然，下面会看到也不会删除报文，不会从receive队列中移除报文
        peek_seq = tp->copied_seq;
        seq = &peek_seq;
    }
    //获取SO_RCVLOWAT最低接收阀值，当然，target实际上是用户态内存大小len和SO_RCVLOWAT的最小值
    //注意：flags参数中若携带MSG_WAITALL标志位，则意味着必须等到读取到len长度的消息才能返回，此时target只能是len
    target = sock_rcvlowat(sk, flags & MSG_WAITALL, len);
#ifdef CONFIG_NET_DMA //配置网络DMA
    tp->ucopy.dma_chan = NULL;
    preempt_disable();
    skb = skb_peek_tail(&sk->sk_receive_queue);
    {
        int available = 0;
        if (skb)
            available = TCP_SKB_CB(skb)->seq + skb->len - (*seq);
        if ((available < target) &&
            (len > sysctl_tcp_dma_copybreak) && !(flags & MSG_PEEK) &&
            !sysctl_tcp_low_latency &&
            dma_find_channel(DMA_MEMCPY)) {
            preempt_enable_no_resched();
            tp->ucopy.pinned_list =
                    dma_pin_iovec_pages(msg->msg_iov, len);
        } else {
            preempt_enable_no_resched();
        }
    }
#endif
    //以下开始读取消息
    do {
        u32 offset;
        /* urgent data 处理*/
        if (tp->urg_data && tp->urg_seq == *seq) {
            if (copied)
                break;
            if (signal_pending(current)) {
                copied = timeo ? sock_intr_errno(timeo) : -EAGAIN;
                break;
            }
        }
        //从sk_receive_queue队列中获取skb
        skb_queue_walk(&sk->sk_receive_queue, skb) {
            //不应该有两个recv队列
            if (WARN(before(*seq, TCP_SKB_CB(skb)->seq),
                 KERN_INFO "recvmsg bug: copied %X "
                       "seq %X rcvnxt %X fl %X\n", *seq,
                       TCP_SKB_CB(skb)->seq, tp->rcv_nxt,
                       flags))
                break;
            //offset是待拷贝序号在当前这个报文中的偏移量
            offset = *seq - TCP_SKB_CB(skb)->seq;
            //syn偏移-1
            if (tcp_hdr(skb)->syn)
                offset--;
            //偏移小于skb长度
            if (offset < skb->len)
                goto found_ok_skb;
            //fin标识
            if (tcp_hdr(skb)->fin)
                goto found_fin_ok; //处理skb
            WARN(!(flags & MSG_PEEK), KERN_INFO "recvmsg bug 2: "
                    "copied %X seq %X rcvnxt %X fl %X\n",
                    *seq, TCP_SKB_CB(skb)->seq,
                    tp->rcv_nxt, flags);
        }
        /*backLog队列的处理*/
        if (copied >= target && !sk->sk_backlog.tail)
            break;
     ......
            /* 如果prequeue不是空的，我们必须在释放套接字之前处理它，否则在第二次迭代时将会被打破。
             * 需要更优雅的解决方案！
             * 看：我们有以下（伪）队列：
             *
             * 1. packets in flight  --在处理的队列
             * 2. backlog   -- 后备队列
             * 3. prequeue  --  处理队列
             * 4. receive_queue --接收队列
             * 每个队列仅仅在下一个队列为空时才能处理。
             */
            if (!skb_queue_empty(&tp->ucopy.prequeue))
                goto do_prequeue;
            /* __ Set realtime policy in scheduler __ */
        }
        if (copied >= target) {
            /* Do not sleep, just process backlog. */
            release_sock(sk);
            lock_sock(sk);
        } else
            sk_wait_data(sk, &timeo);
#ifdef CONFIG_NET_DMA
        tp->ucopy.wakeup = 0;
#endif
        if (user_recv) {
            int chunk;
            /* __ Restore normal policy in scheduler __ */
            if ((chunk = len - tp->ucopy.len) != 0) {
                NET_ADD_STATS_USER(sock_net(sk), LINUX_MIB_TCPDIRECTCOPYFROMBACKLOG, chunk);
                len -= chunk;
                copied += chunk;
            }
            if (tp->rcv_nxt == tp->copied_seq &&
                !skb_queue_empty(&tp->ucopy.prequeue)) {
do_prequeue:
                tcp_prequeue_process(sk);
                if ((chunk = len - tp->ucopy.len) != 0) {
                    NET_ADD_STATS_USER(sock_net(sk), LINUX_MIB_TCPDIRECTCOPYFROMPREQUEUE, chunk);
                    len -= chunk;
                    copied += chunk;
                }
            }
        }
        if ((flags & MSG_PEEK) &&
            (peek_seq - copied - urg_hole != tp->copied_seq)) {
            if (net_ratelimit())
                printk(KERN_DEBUG "TCP(%s:%d): Application bug, race in MSG_PEEK.\n",
                       current->comm, task_pid_nr(current));
            peek_seq = tp->copied_seq;
        }
        continue;
    found_ok_skb:
        /* receive队列的这个报文从其可以使用的偏移量offset，到总长度len之间，可以拷贝的长度为used  */
        used = skb->len - offset;
        //len是用户态空闲内存，len更小时，当然只能拷贝len长度消息，总不能导致内存溢出吧
        if (len < used)
            used = len;
        /* 有紧急标识数据吗？ */
        if (tp->urg_data) {
            u32 urg_offset = tp->urg_seq - *seq;
            if (urg_offset < used) {
                if (!urg_offset) {
                    if (!sock_flag(sk, SOCK_URGINLINE)) {
                        ++*seq;
                        urg_hole++;
                        offset++;
                        used--;
                        if (!used)
                            goto skip_copy;
                    }
                } else
                    used = urg_offset;
            }
        }
         //MSG_TRUNC标志位表示不要管len这个用户态内存有多大，只管拷贝数据吧 
        if (!(flags & MSG_TRUNC)) {
        ......
            {
                ////向用户态拷贝数据，开始复制数据到iovec
                err = skb_copy_datagram_iovec(skb, offset,
                        msg->msg_iov, used);
                if (err) {
                    /* Exception. Bailout! */
                    if (!copied)
                        copied = -EFAULT;
                    break;
                }
            }
        }
        *seq += used;
        copied += used;
        len -= used;
        /*每次将数据复制到用户空间时，都应该调用此函数。它计算适当的TCP接收缓冲区空间。*/
        tcp_rcv_space_adjust(sk);
skip_copy:
        if (tp->urg_data && after(tp->copied_seq, tp->urg_seq)) {
            tp->urg_data = 0;
            tcp_fast_path_check(sk);
        }
        if (used + offset < skb->len)
            continue;
        if (tcp_hdr(skb)->fin)
            goto found_fin_ok;
        if (!(flags & MSG_PEEK)) {
            sk_eat_skb(sk, skb, copied_early);
            copied_early = 0;
        }
        continue;
    found_fin_ok:
        /* Process the FIN. */
        ++*seq;
        if (!(flags & MSG_PEEK)) {
            sk_eat_skb(sk, skb, copied_early); //释放相关队列
            copied_early = 0;
        }
        break;
    } while (len > 0);
     //已经装载了接收器 
    if (user_recv) {
        //prequeue队列不为空则处理之
        if (!skb_queue_empty(&tp->ucopy.prequeue)) {
            int chunk;
            tp->ucopy.len = copied > 0 ? len : 0;
            tcp_prequeue_process(sk);
            if (copied > 0 && (chunk = len - tp->ucopy.len) != 0) {
                NET_ADD_STATS_USER(sock_net(sk), LINUX_MIB_TCPDIRECTCOPYFROMPREQUEUE, chunk);
                len -= chunk;
                copied += chunk;
            }
        }
    /准备返回用户态，socket上不再装载接收任务
        tp->ucopy.task = NULL;
        tp->ucopy.len = 0;
    }
    ......
    TCP_CHECK_TIMER(sk);
    //释放socket时，还会检查、处理backlog队列中的报文 
    release_sock(sk);
    return copied;
out:
    TCP_CHECK_TIMER(sk);
    //释放socket时，还会检查、处理backlog队列中的报文 
    release_sock(sk);
    return err;
recv_urg:
    err = tcp_recv_urg(sk, msg, len, flags);
    goto out;
}
```

# 0x04 总结
此次结合大牛的分析，重新学习了一次内核态数据如何到用户态的，且了解了几个关键队列。（学习过程，大牛勿喷，多指教）

