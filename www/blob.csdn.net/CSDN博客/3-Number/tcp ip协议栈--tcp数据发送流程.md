
# tcp-ip协议栈--tcp数据发送流程 - 3-Number - CSDN博客


置顶2017年09月26日 17:37:48[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：417


# 0x01 缘由
前面一些章节学习数据的收，现在关注协议socket数据从用户态到内核态，然后发送出去的过程。学习手段还是借助他人的成果，加单步跟踪内核源码。
传送：[http://blog.csdn.net/zhangskd/article/details/48207553](http://blog.csdn.net/zhangskd/article/details/48207553)
# 0x02 调用栈
![](https://img-blog.csdn.net/20170926173745368?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170926173800040?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


# 0x03 关键代码分析
```python
/* tcp_sendmsg()的主要工作是把用户层的数据，填充到skb中，然后加入到sock的发送队列。
之后调用tcp_write_xmit()来把sock发送队列中的skb尽量地发送出去。*/
int tcp_sendmsg(struct kiocb *iocb, struct socket *sock, struct msghdr *msg,
        size_t size)
{
    struct sock *sk = sock->sk;
    struct iovec *iov;
    struct tcp_sock *tp = tcp_sk(sk);
    struct sk_buff *skb;
    int iovlen, flags;
    int mss_now, size_goal;
    int err, copied;
    long timeo;
    lock_sock(sk);
    TCP_CHECK_TIMER(sk);
    //用户端消息标识符
    flags = msg->msg_flags;
    //发送超时时间，是阻塞socket才有效，非阻塞为0
    timeo = sock_sndtimeo(sk, flags & MSG_DONTWAIT);
    /* 当发送数据时，连接未建立或者是关闭等待状态，这是就得等待处理 */
    if ((1 << sk->sk_state) & ~(TCPF_ESTABLISHED | TCPF_CLOSE_WAIT))
        if ((err = sk_stream_wait_connect(sk, &timeo)) != 0)
            goto out_err;
    /* 清除使用异步情况下，发送队列满了的标志 */
    clear_bit(SOCK_ASYNC_NOSPACE, &sk->sk_socket->flags);
    /* 获取当前的发送MSS */
    mss_now = tcp_send_mss(sk, &size_goal, flags);
    /* 开始处理用户数据块. */
    iovlen = msg->msg_iovlen;/* 应用层数据块的个数*/ 
    iov = msg->msg_iov; /* 应用层数据块数组的地址 */
    copied = 0; /* 已拷贝到发送队列的字节数 */
    err = -EPIPE; /* 初始化状态码 ，socket状态为shutdown或出错*/
    if (sk->sk_err || (sk->sk_shutdown & SEND_SHUTDOWN))
        goto out_err;
    //开始变了处理。
    while (--iovlen >= 0) {
        int seglen = iov->iov_len; /* 数据块的长度 */ 
        unsigned char __user *from = iov->iov_base; /* 数据块的地址 */ 
        iov++;/* 指向下一个数据块 */ 
        while (seglen > 0) { //段长度大于0
            int copy = 0;
            int max = size_goal; /* 单个skb的最大数据长度，如果使用了GSO，长度为MSS的整数倍 */
            skb = tcp_write_queue_tail(sk); /* 发送队列的最后一个skb */ 
            if (tcp_send_head(sk)) { /* 判断 sk->sk_send_head 是否为空，不为空，说明有数据需要发送。*/
                if (skb->ip_summed == CHECKSUM_NONE)
                    max = mss_now; /* 如果网卡不支持检验和计算，那么skb的最大长度为MSS，即不能使用GSO *
                copy = max - skb->len; /* 此skb可追加的数据长度，就是通过目前这个skb还可以发送多少数据。*/
            }
            if (copy <= 0) { /* 小于0，说明发送队列有数据，且大于mss，如果等于0，说明发送队列没有数据。此处判断说需要重新分配一个skb。*/
new_segment:
                /* 分配一个新的分片；
                 * 将skb配件分配到单页；
                 */
         /* 如果发送队列的总大小sk_wmem_queued大于等于发送缓存的上限sk_sndbuf，
        * 或者发送缓存中尚未发送的数据量超过了用户的设置值，就进入等待。 sk->sk_wmem_queued < sk->sk_sndbuf;
        */ 
                if (!sk_stream_memory_free(sk))
                    goto wait_for_sndbuf;
                  /* 申请一个skb，其线性数据区的大小为：
           * 通过select_size()得到的线性数据区中TCP负荷的大小 + 最大的协议头长度。
           * 如果申请skb失败了，或者虽然申请skb成功，但是从系统层面判断此次申请不合法，
           * 那么就进入睡眠，等待内存。
                skb = sk_stream_alloc_skb(sk, select_size(sk),
                        sk->sk_allocation);
                /* 判断skb是否申请成功，如果不成功则进入休眠等待 */
                if (!skb)
                    goto wait_for_memory;
                /*
                 * 检查是否有硬件校验和计算
                 */
                if (sk->sk_route_caps & NETIF_F_ALL_CSUM)
                    skb->ip_summed = CHECKSUM_PARTIAL;
       /* 更新skb的TCP控制块字段，把skb加入到sock发送队列的尾部，
       * 增加发送队列的大小，减小预分配缓存的大小。
       */ 
                skb_entail(sk, skb);
                copy = size_goal;
                max = size_goal;
            }
            /* 尝试追加数据到skb结构的尾部。复制数据长度不能大于 seglen */
            if (copy > seglen)
                copy = seglen;
            /* 复制到哪去？如果skb的线性数据区还有剩余空间，就先复制到线性数据区。 */
            if (skb_tailroom(skb) > 0) {
                /* skb头有部分数据空间。 */
                if (copy > skb_tailroom(skb))
                    copy = skb_tailroom(skb);
                    /*仅仅拷贝空间能放下的部分，拷贝用户空间的数据到内核空间*/ 
                if ((err = skb_add_data(skb, from, copy)) != 0)
                    goto do_fault;
            } else {
                /* 如果skb的线性数据区已经用完了，那么就使用分页区 */
                int merge = 0;
                int i = skb_shinfo(skb)->nr_frags; /* 分页数 */ 
                struct page *page = TCP_PAGE(sk);  /* 上次缓存的分页 */
                int off = TCP_OFF(sk); //分页缓存的偏移
                /* 判断能否往最后一个分页追加数据 */
                if (skb_can_coalesce(skb, i, page, off) &&
                    off != PAGE_SIZE) {
                    /* 能扩展到最后一页分片 */
                    merge = 1;
                } else if (i == MAX_SKB_FRAGS ||
                       (!i &&
                       !(sk->sk_route_caps & NETIF_F_SG))) {
                /* 不能追加时，检查分页数是否达到了上限，或者网卡不支持分散聚合。
         * 如果是的话，就为此skb设置PSH标志，尽快地发送出去。
         * 然后跳转到new_segment处申请新的skb，来继续填装数据。
         */ 
                    tcp_mark_push(tp, skb);
                    goto new_segment;
                } else if (page) {
                    if (off == PAGE_SIZE) {
                        put_page(page);
                        TCP_PAGE(sk) = page = NULL;
                        off = 0;
                    }
                } else
                    off = 0;
                if (copy > PAGE_SIZE - off)
                    copy = PAGE_SIZE - off;
                /* 从系统层面判断发送缓存的申请是否合法 */
                if (!sk_wmem_schedule(sk, copy))
                    goto wait_for_memory;
                //失败
                if (!page) {
                    /* Allocate new cache page. */
                    if (!(page = sk_stream_alloc_page(sk)))
                        goto wait_for_memory;
                }
                /* 拷贝用户空间的数据到内核空间，同时计算校验和。
         * 更新skb的长度字段，更新sock的发送队列大小和预分配缓存。
         */ 
                err = skb_copy_to_page(sk, from, skb, page,
                               off, copy);
                if (err) {
                    if (!TCP_PAGE(sk)) {
                        TCP_PAGE(sk) = page;
                        TCP_OFF(sk) = 0;
                    }
                    goto do_error;
                }
                /* 更新skb */
                if (merge) {
                    skb_shinfo(skb)->frags[i - 1].size +=
                                    copy;
                } else {
                    skb_fill_page_desc(skb, i, page, off, copy);
                    if (TCP_PAGE(sk)) {
                        get_page(page);
                    } else if (off + copy < PAGE_SIZE) {
                        get_page(page);
                        TCP_PAGE(sk) = page;
                    }
                }
                TCP_OFF(sk) = off + copy;
            }
            if (!copied)
                TCP_SKB_CB(skb)->flags &= ~TCPCB_FLAG_PSH;
            tp->write_seq += copy; /* 更新发送队列的最后一个序号 */ 
            TCP_SKB_CB(skb)->end_seq += copy;/* 更新skb的结束序号 */
            skb_shinfo(skb)->gso_segs = 0;
            from += copy; /* 下次拷贝的地址 */ 
            copied += copy;/* 已经拷贝到发送队列的数据量 */ 
             /* 如果所有数据都拷贝好了，就退出 */
            if ((seglen -= copy) == 0 && iovlen == 0)
                goto out;
            /* 如果skb还可以继续填充数据，或者发送的是带外数据，或者使用TCP REPAIR选项，
       * 那么继续拷贝数据，先不发送。
       */ 
            if (skb->len < max || (flags & MSG_OOB))
                continue;
            /* 如果需要设置PSH标志 */
            if (forced_push(tp)) {
                tcp_mark_push(tp, skb);
                /* 尽可能的将发送队列中的skb发送出去，禁用nalge */
                __tcp_push_pending_frames(sk, mss_now, TCP_NAGLE_PUSH);
            } else if (skb == tcp_send_head(sk))
                tcp_push_one(sk, mss_now); ///* 只发送一个skb */ 
            continue;
wait_for_sndbuf:
            set_bit(SOCK_NOSPACE, &sk->sk_socket->flags); //设定sock没空间的标识
wait_for_memory:
            /* 如果已经有数据复制到发送队列了，就尝试立即发送 */ 
            if (copied)
                tcp_push(sk, flags & ~MSG_MORE, mss_now, TCP_NAGLE_PUSH);
      /* 分两种情况：
       * 1. sock的发送缓存不足。等待sock有发送缓存可写事件，或者超时。
       * 2. TCP层内存不足，等待2~202ms之间的一个随机时间。
       */ 
            if ((err = sk_stream_wait_memory(sk, &timeo)) != 0)
                goto do_error;
            /* 睡眠后MSS和TSO段长可能会发生变化，重新计算 */
            mss_now = tcp_send_mss(sk, &size_goal, flags);
        }
    }
out:
    /* 如果已经有数据复制到发送队列了，就尝试立即发送 */ 
    if (copied)
        tcp_push(sk, flags, mss_now, tp->nonagle);
    TCP_CHECK_TIMER(sk);
    release_sock(sk);
    return copied;
do_fault:
    if (!skb->len) {
        tcp_unlink_write_queue(skb, sk);
        /* It is the one place in all of TCP, except connection
         * reset, where we can be unlinking the send_head.
         */
        tcp_check_send_head(sk, skb);
        sk_wmem_free_skb(sk, skb);
    }
do_error:
    //如果已经复制了部分数据，则发送出去；
    if (copied)
        goto out;
out_err:
    err = sk_stream_error(sk, flags, err);
    TCP_CHECK_TIMER(sk);
    release_sock(sk);
    return err;
}
```

# 0x04 总结
数据到达ip层后，进行相关操作，然后发送，此处不追求细节，仅仅从大致流程上进行分析。（学习阶段，大神勿喷，都指点）

