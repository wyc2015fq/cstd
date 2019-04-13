
# tcp-ip协议栈-tcp层（1） - 3-Number - CSDN博客


2017年09月21日 15:08:52[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：206个人分类：[Linux 网络协议栈																](https://blog.csdn.net/pangyemeng/article/category/7130138)



# 0x01 缘由
上章节简单学习了内核网络栈路由的过程，这章节继续tcp三次握手过程。
# 0x02 调式环境
在guest虚拟机中跑一个server端，然后其他机器跑个客户端进行分析。
# 0x03 源码解析
主要分几部分来：
1.收到客户端syn包处理流程
2.服务端构造syn + ack相应客户端
3.客户端发送ack给服务端连接建立
1、2过程调用图
![](https://img-blog.csdn.net/20170921151141487?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
过程1的源码分析：
## 1.1 tcp_v4_rcv()函数解析
/*
```python
*    在 tcp_input.c 中调用
 */
int tcp_v4_rcv(struct sk_buff *skb)
{
    const struct iphdr *iph; //ip头部结构
    struct tcphdr *th;    //tcp头部结构
    struct sock *sk;            //对应的应用层头部结构
    int ret;
    struct net *net = dev_net(skb->dev); //网络设备相关
    if (skb->pkt_type != PACKET_HOST) //不是指向本机，抛弃
        goto discard_it;
    /* 省略部分代码，下面是对tcp头部进行校验，数据偏移 */
    th = tcp_hdr(skb);
    if (th->doff < sizeof(struct tcphdr) / 4)
        goto bad_packet;
    /* 省略部分代码 */
    th = tcp_hdr(skb);
    iph = ip_hdr(skb);
    TCP_SKB_CB(skb)->seq = ntohl(th->seq); //tcp序号
    TCP_SKB_CB(skb)->end_seq = (TCP_SKB_CB(skb)->seq + th->syn + th->fin +
                    skb->len - th->doff * 4); //tcp结束序号 开始序号 + 数据长度
    TCP_SKB_CB(skb)->ack_seq = ntohl(th->ack_seq); //ack序号
    TCP_SKB_CB(skb)->when     = 0;        //窗口大小
    TCP_SKB_CB(skb)->flags     = iph->tos; //标识
    TCP_SKB_CB(skb)->sacked     = 0; //拥塞处理
    /* 查询是否已经存在对应的sock结构.
        TCP 通过搜索 tcp_hashinfo 查找已经建立的 sock{}，不过出于性能的考虑，
        凡是出于 TCP_ESTABLISHED 状态的 sock{}放入了 tcp_hashinfo->ehash 表
        中，而处于 TCP_LISTEN 状态的 sock{}放入了 tcp_hashinfo->lhash 表中。
    */
    sk = __inet_lookup_skb(&tcp_hashinfo, skb, th->source, th->dest);
    if (!sk)
        goto no_tcp_socket;
process:
    //tcp目前的状态
    if (sk->sk_state == TCP_TIME_WAIT)
        goto do_time_wait;
    //过滤相关
    if (sk_filter(sk, skb))
        goto discard_and_relse;
    skb->dev = NULL;
    bh_lock_sock_nested(sk);
    ret = 0;
    if (!sock_owned_by_user(sk)) {
#ifdef CONFIG_NET_DMA
        struct tcp_sock *tp = tcp_sk(sk);
        if (!tp->ucopy.dma_chan && tp->ucopy.pinned_list)
            tp->ucopy.dma_chan = dma_find_channel(DMA_MEMCPY);
        if (tp->ucopy.dma_chan)
            ret = tcp_v4_do_rcv(sk, skb);
        else
#endif
        {
            if (!tcp_prequeue(sk, skb))
                ret = tcp_v4_do_rcv(sk, skb); //进入下一个处理函数；
        }
    } else
        sk_add_backlog(sk, skb);
    bh_unlock_sock(sk);
    sock_put(sk);
    return ret;
    /* 省略部分代码*/
}
```

## 1.2 tcp_v4_do_rcv ()函数解析
```python
int tcp_v4_do_rcv(struct sock *sk, struct sk_buff *skb)
{
    struct sock *rsk;
    /* 如果连接到了建立的状态，则走快速路径。*/
    if (sk->sk_state == TCP_ESTABLISHED) { /* Fast path */
        TCP_CHECK_TIMER(sk);
        //后面讲解
        if (tcp_rcv_established(sk, skb, tcp_hdr(skb), skb->len)) {
            rsk = sk;
            goto reset;
        }
        TCP_CHECK_TIMER(sk);
        return 0;
    }
    //检查数据长度
    if (skb->len < tcp_hdrlen(skb) || tcp_checksum_complete(skb))
        goto csum_err;
    //处理监听状态
    if (sk->sk_state == TCP_LISTEN) {
        //处理相关请求
        struct sock *nsk = tcp_v4_hnd_req(sk, skb);
        if (!nsk)
            goto discard;
        if (nsk != sk) {
            if (tcp_child_process(sk, nsk, skb)) {
                rsk = nsk;
                goto reset;
            }
            return 0;
        }
    }
    TCP_CHECK_TIMER(sk);
    //tcp状态机处理
    if (tcp_rcv_state_process(sk, skb, tcp_hdr(skb), skb->len)) {
        rsk = sk;
        goto reset;
    }
    TCP_CHECK_TIMER(sk);
    return 0;
reset:
    //发送rst包
    tcp_v4_send_reset(rsk, skb);
    /* 部分代码省略 */
}
```

## 1.3 tcp_rcv_state_process()函数解析
不管是客户端还是服务器端， 当连接不是 TCP_ESTABLISHED 状态的时候，就会调用 tcp_rcv_state_process 函数， 这就是专门处理 3 次握手协议的状态机处理函数。 对于服务器端 TCP
协议栈， 当收到一个 connect 请求时，会去调用此函数中出现的 tp->af_specific->conn_request(sk, skb)，也就是 tcp_v4_conn_request，如果不记得如何指向此函数，请参考 TCP 的 socket 初始化：
下面仅讲解单步调式过程中一些关键函数：
```python
int tcp_rcv_state_process(struct sock *sk, struct sk_buff *skb,
              struct tcphdr *th, unsigned len)
{
    struct tcp_sock *tp = tcp_sk(sk);
    struct inet_connection_sock *icsk = inet_csk(sk);
    int queued = 0;
    int res;
    tp->rx_opt.saw_tstamp = 0;
    switch (sk->sk_state) {
    case TCP_CLOSE:
        goto discard;
    case TCP_LISTEN:
        //服务端处理监听状态
        if (th->ack)//检查ack标识
            return 1;
        if (th->rst)//检查rst标识，rst标识导致数据包丢弃
            goto discard;
        if (th->syn) {
            //处理syn过程，下一个函数查看这个
            if (icsk->icsk_af_ops->conn_request(sk, skb) < 0)
                return 1;
            kfree_skb(skb);
            return 0;
        }
        goto discard;
    case TCP_SYN_SENT:
        queued = tcp_rcv_synsent_state_process(sk, skb, th, len);
        if (queued >= 0)
            return queued;
        /* Do step6 onward by hand. */
        tcp_urg(sk, skb, th);
        __kfree_skb(skb);
        tcp_data_snd_check(sk);
        return 0;
    }
    res = tcp_validate_incoming(sk, skb, th, 0);
    if (res <= 0)
        return -res;
    /* 省略大部分代码，暂不关注 */
    }
    return 0;
}
```

## 1.4 tcp_rcv_state_process()函数解析
```python
/*
 *  处理一个输入包SYN_RECV sockets代表一个请求sock。
 */
struct sock *tcp_check_req(struct sock *sk, struct sk_buff *skb,
               struct request_sock *req,
               struct request_sock **prev)
{
    /* 检查为纯syn重传 */
    if (TCP_SKB_CB(skb)->seq == tcp_rsk(req)->rcv_isn &&
        flg == TCP_FLAG_SYN &&
        !paws_reject) {
        req->rsk_ops->rtx_syn_ack(sk, req);
        return NULL;
    }
    /* 省略相关代码 */
}
```

## 1.5 tcp_v4_send_synack ()函数解析
```python
static int tcp_v4_send_synack(struct sock *sk, struct request_sock *req)
{
    return __tcp_v4_send_synack(sk, req, NULL);
}
/*
 *    在接收到一个SYN包时发送一个SYN-ACK
 *    尽在request_sock 中
 */
static int __tcp_v4_send_synack(struct sock *sk, struct request_sock *req,
                struct dst_entry *dst)
{
    const struct inet_request_sock *ireq = inet_rsk(req);
    int err = -1;
    struct sk_buff * skb;
    /* First, grab a route. */
    if (!dst && (dst = inet_csk_route_req(sk, req)) == NULL)
        return -1;
    skb = tcp_make_synack(sk, dst, req); //构造syn + ack包
    if (skb) {
        struct tcphdr *th = tcp_hdr(skb);
        //校验和
        th->check = tcp_v4_check(skb->len,
                     ireq->loc_addr,
                     ireq->rmt_addr,
                     csum_partial(th, skb->len,
                              skb->csum));
        //构造包并发送
        err = ip_build_and_send_pkt(skb, sk, ireq->loc_addr,
                        ireq->rmt_addr,
                        ireq->opt);
        err = net_xmit_eval(err);
    }
    dst_release(dst);
    return err;
}
```

# 0x04 总结
此处简单学了服务端收到一个syn收的处理过程，下一步继续学习三次握手的过程。(学习过程，大神勿喷，多指点)

