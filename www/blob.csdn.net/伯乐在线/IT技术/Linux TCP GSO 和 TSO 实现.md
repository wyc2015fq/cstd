# Linux TCP GSO 和 TSO 实现 - 文章 - 伯乐在线
原文出处： [lvyilong316](http://blog.chinaunix.net/uid-28541347-id-5763844.html)
（注：kernel版本：linux 2.6.32）
## 概念
TSO(TCP Segmentation Offload): 是一种利用网卡来对大数据包进行自动分段，降低CPU负载的技术。 其主要是延迟分段。
GSO(Generic Segmentation Offload): GSO是协议栈是否推迟分段，在发送到网卡之前判断网卡是否支持TSO，如果网卡支持TSO则让网卡分段，否则协议栈分完段再交给驱动。 **如果TSO****开启，GSO****会自动开启。**
以下是TSO和GSO的组合关系：
- GSO开启， TSO开启: 协议栈推迟分段，并直接传递大数据包到网卡，让网卡自动分段
- GSO开启， TSO关闭: 协议栈推迟分段，在最后发送到网卡前才执行分段
- GSO关闭， TSO开启: 同GSO开启， TSO开启
- GSO关闭， TSO关闭: 不推迟分段，在tcp_sendmsg中直接发送MSS大小的数据包
## 开启GSO/TSO
驱动程序在注册网卡设备的时候默认开启GSO: NETIF_F_GSO
驱动程序会根据网卡硬件是否支持来设置TSO: NETIF_F_TSO
可以通过ethtool -K来开关GSO/TSO


```
#define NETIF_F_SOFT_FEATURES (NETIF_F_GSO | NETIF_F_GRO)
int register_netdevice(struct net_device *dev)
{
              ...
              /* Transfer changeable features to wanted_features and enable
               * software offloads (GSO and GRO).
               */
              dev->hw_features |= NETIF_F_SOFT_FEATURES;
              dev->features |= NETIF_F_SOFT_FEATURES; //默认开启GRO/GSO
              dev->wanted_features = dev->features & dev->hw_features;
              ...
}
static int ixgbe_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
              ...
              netdev->features = NETIF_F_SG |
                                              NETIF_F_TSO |
                                              NETIF_F_TSO6 |
                                              NETIF_F_RXHASH |
                                              NETIF_F_RXCSUM |
                                              NETIF_F_HW_CSUM;
              register_netdev(netdev);
              ...
}
```
## 是否推迟分段
从上面我们知道GSO/TSO是否开启是保存在dev->features中，而设备和路由关联，当我们查询到路由后就可以把配置保存在sock中。
比如在tcp_v4_connect和tcp_v4_syn_recv_sock都会调用sk_setup_caps来设置GSO/TSO配置。
需要注意的是，只要开启了GSO，即使硬件不支持TSO，也会设置NETIF_F_TSO，使得sk_can_gso(sk)在GSO开启或者TSO开启的时候都返回true
l  **sk_setup_caps**


```
#define NETIF_F_GSO_SOFTWARE (NETIF_F_TSO | NETIF_F_TSO_ECN | NETIF_F_TSO6)
#define NETIF_F_TSO (SKB_GSO_TCPV4 << NETIF_F_GSO_SHIFT)
void sk_setup_caps(struct sock *sk, struct dst_entry *dst)
{
              __sk_dst_set(sk, dst);
              sk->sk_route_caps = dst->dev->features;
              if (sk->sk_route_caps & NETIF_F_GSO) /*GSO默认都会开启*/
                             sk->sk_route_caps |= NETIF_F_GSO_SOFTWARE; /*打开TSO*/
              if (sk_can_gso(sk)) { /*对于tcp这里会成立*/
                             if (dst->header_len) {
                                           sk->sk_route_caps &= ~NETIF_F_GSO_MASK;
                             } else {
                                           sk->sk_route_caps |= NETIF_F_SG | NETIF_F_HW_CSUM;
                                           sk->sk_gso_max_size = dst->dev->gso_max_size; /*GSO_MAX_SIZE=65536*/
                             }
              }
}
```
从上面可以看出，如果设备开启了GSO，sock都会将TSO标志打开，但是注意这和硬件是否开启TSO无关，硬件的TSO取决于硬件自身特性的支持。下面看下sk_can_gso的逻辑。
l  **sk_can_gso**


```
static inline int sk_can_gso(const struct sock *sk)
{
    /*对于tcp，在tcp_v4_connect中被设置：sk->sk_gso_type = SKB_GSO_TCPV4*/
              return net_gso_ok(sk->sk_route_caps, sk->sk_gso_type);
}
```
l  **net_gso_ok**


```
static inline int net_gso_ok(int features, int gso_type)
{
              int feature = gso_type << NETIF_F_GSO_SHIFT;
              return (features & feature) == feature;
}
```
由于对于tcp 在sk_setup_caps中sk->sk_route_caps也被设置有SKB_GSO_TCPV4，所以整个sk_can_gso成立。
## GSO的数据包长度
对紧急数据包或GSO/TSO都不开启的情况，才不会推迟发送， 默认使用当前MSS。开启GSO后，tcp_send_mss返回mss和单个skb的GSO大小，为mss的整数倍。
l  **tcp_send_mss**


```
static int tcp_send_mss(struct sock *sk, int *size_goal, int flags)
{
              int mss_now;
 
              mss_now = tcp_current_mss(sk);/*通过ip option，SACKs及pmtu确定当前的mss*/
              *size_goal = tcp_xmit_size_goal(sk, mss_now, !(flags & MSG_OOB));
 
              return mss_now;
}
```
l  **tcp_xmit_size_goal**


```
static unsigned int tcp_xmit_size_goal(struct sock *sk, u32 mss_now,
                                                                int large_allowed)
{
              struct tcp_sock *tp = tcp_sk(sk);
              u32 xmit_size_goal, old_size_goal;
 
              xmit_size_goal = mss_now;
    /*这里large_allowed表示是否是紧急数据*/
              if (large_allowed && sk_can_gso(sk)) { /*如果不是紧急数据且支持GSO*/
                             xmit_size_goal = ((sk->sk_gso_max_size - 1) -
                                                           inet_csk(sk)->icsk_af_ops->net_header_len -
                                                           inet_csk(sk)->icsk_ext_hdr_len -
                                                           tp->tcp_header_len);/*xmit_size_goal为gso最大分段大小减去tcp和ip头部长度*/
 
                             xmit_size_goal = tcp_bound_to_half_wnd(tp, xmit_size_goal);/*最多达到收到的最大rwnd窗口通告的一半*/
 
                             /* We try hard to avoid divides here */
                             old_size_goal = tp->xmit_size_goal_segs * mss_now;
 
                             if (likely(old_size_goal <= xmit_size_goal &&
                                              old_size_goal + mss_now > xmit_size_goal)) {
                                           xmit_size_goal = old_size_goal; /*使用老的xmit_size*/
                             } else {
                                           tp->xmit_size_goal_segs = xmit_size_goal / mss_now;
                                           xmit_size_goal = tp->xmit_size_goal_segs * mss_now; /*使用新的xmit_size*/
                             }
              }
 
              return max(xmit_size_goal, mss_now);
}
```
l  **tcp_sendmsg**
应用程序send()数据后，会在tcp_sendmsg中尝试在同一个skb，保存size_goal大小的数据，然后再通过tcp_push把这些包通过tcp_write_xmit发出去


```
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
 
    flags = msg->msg_flags;
    timeo = sock_sndtimeo(sk, flags & MSG_DONTWAIT);
 
    /* Wait for a connection to finish. */
    if ((1 << sk->sk_state) & ~(TCPF_ESTABLISHED | TCPF_CLOSE_WAIT))
        if ((err = sk_stream_wait_connect(sk, &timeo)) != 0)
             goto out_err;
 
    /* This should be in poll */
    clear_bit(SOCK_ASYNC_NOSPACE, &sk->sk_socket->flags);
    /* size_goal表示GSO支持的大小，为mss的整数倍，不支持GSO时则和mss相等 */
    mss_now = tcp_send_mss(sk, &size_goal, flags);/*返回值mss_now为真实mss*/
 
    /* Ok commence sending. */
    iovlen = msg->msg_iovlen;
    iov = msg->msg_iov;
    copied = 0;
 
    err = -EPIPE;
    if (sk->sk_err || (sk->sk_shutdown & SEND_SHUTDOWN))
        goto out_err;
 
    while (--iovlen >= 0) {
        size_t seglen = iov->iov_len;
        unsigned char __user *from = iov->iov_base;
 
        iov++;
 
        while (seglen > 0) {
            int copy = 0;
            int max = size_goal; /*每个skb中填充的数据长度初始化为size_goal*/
            /* 从sk->sk_write_queue中取出队尾的skb，因为这个skb可能还没有被填满 */
            skb = tcp_write_queue_tail(sk);
            if (tcp_send_head(sk)) { /*如果之前还有未发送的数据*/
                if (skb->ip_summed == CHECKSUM_NONE) /*比如路由变更，之前的不支持TSO,现在的支持了*/
                    max = mss_now; /*上一个不支持GSO的skb，继续不支持*/
                copy = max - skb->len; /*copy为每次想skb中拷贝的数据长度*/
            }
           /*copy<=0表示不能合并到之前skb做GSO*/
           if (copy <= 0) {
new_segment:
               /* Allocate new segment. If the interface is SG,
                * allocate skb fitting to single page.
                */
               /* 内存不足，需要等待 */
               if (!sk_stream_memory_free(sk))
                   goto wait_for_sndbuf;
               /* 分配新的skb */
               skb = sk_stream_alloc_skb(sk, select_size(sk), sk->sk_allocation);
               if (!skb)
                   goto wait_for_memory;
 
               /*
                * Check whether we can use HW checksum.
                */
               /*如果硬件支持checksum，则将skb->ip_summed设置为CHECKSUM_PARTIAL，表示由硬件计算校验和*/
               if (sk->sk_route_caps & NETIF_F_ALL_CSUM)
                   skb->ip_summed = CHECKSUM_PARTIAL;
               /*将skb加入sk->sk_write_queue队尾, 同时去掉skb的TCP_NAGLE_PUSH标记*/
               skb_entail(sk, skb);
               copy = size_goal; /*这里将每次copy的大小设置为size_goal，即GSO支持的大小*/
               max = size_goal;
           }
 
           /* Try to append data to the end of skb. */
           if (copy > seglen)
               copy = seglen;
 
           /* Where to copy to? */
           if (skb_tailroom(skb) > 0) { /*如果skb的线性区还有空间，则先填充skb的线性区*/
               /* We have some space in skb head. */
               if (copy > skb_tailroom(skb))
                   copy = skb_tailroom(skb);
               if ((err = skb_add_data(skb, from, copy)) != 0) /*copy用户态数据到skb线性区*/
                   goto do_fault;
           } else { /*否则尝试向SG的frags中拷贝*/
               int merge = 0;
               int i = skb_shinfo(skb)->nr_frags;
               struct page *page = TCP_PAGE(sk);
               int off = TCP_OFF(sk);
 
               if (skb_can_coalesce(skb, i, page, off) && off != PAGE_SIZE) {/*pfrag->page和frags[i-1]是否使用相同页，并且page_offset相同*/
                   /* We can extend the last page
                   * fragment. */
                   merge = 1; /*说明和之前frags中是同一个page，需要merge*/
               } else if (i == MAX_SKB_FRAGS || (!i && !(sk->sk_route_caps & NETIF_F_SG))) {
                   /* Need to add new fragment and cannot
                    * do this because interface is non-SG,
                    * or because all the page slots are
                    * busy. */
                   /*如果设备不支持SG，或者非线性区frags已经达到最大，则创建新的skb分段*/
                   tcp_mark_push(tp, skb); /*标记push flag*/
                       goto new_segment;
               } else if (page) {
                   if (off == PAGE_SIZE) {
                       put_page(page); /*增加page引用计数*/
                       TCP_PAGE(sk) = page = NULL;
                       off = 0;
                   }
              } else
                  off = 0;
                  if (copy > PAGE_SIZE - off)
                  copy = PAGE_SIZE - off;
                  if (!sk_wmem_schedule(sk, copy))
                      goto wait_for_memory;
 
                 if (!page) {
                     /* Allocate new cache page. */
                     if (!(page = sk_stream_alloc_page(sk)))
                     goto wait_for_memory;
                 }
 
                 err = skb_copy_to_page(sk, from, skb, page, off, copy); /*拷贝数据到page中*/
                 if (err) {
                     /* If this page was new, give it to the
                      * socket so it does not get leaked.
                      */
                     if (!TCP_PAGE(sk)) {
                         TCP_PAGE(sk) = page;
                         TCP_OFF(sk) = 0;
                     }
                     goto do_error;
                }
 
                /* Update the skb. */
                if (merge) { /*pfrag和frags[i - 1]是相同的*/
                     skb_shinfo(skb)->frags[i - 1].size += copy;
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
 
            tp->write_seq += copy;
            TCP_SKB_CB(skb)->end_seq += copy;
            skb_shinfo(skb)->gso_segs = 0; /*清零tso分段数，让tcp_write_xmit去计算*/
            from += copy;
            copied += copy;
            if ((seglen -= copy) == 0 && iovlen == 0)
                goto out;
            /* 还有数据没copy，并且没有达到最大可拷贝的大小(注意这里max之前被赋值为size_goal，即GSO支持的大小)， 尝试往该skb继续添加数据*/
            if (skb->len < max || (flags & MSG_OOB))
                continue;
            /*下面的逻辑就是：还有数据没copy，但是当前skb已经满了，所以可以发送了(但不是一定要发送)*/
            if (forced_push(tp)) { /*超过最大窗口的一半没有设置push了*/
                tcp_mark_push(tp, skb); /*设置push标记，更新pushed_seq*/
                __tcp_push_pending_frames(sk, mss_now, TCP_NAGLE_PUSH); /*调用tcp_write_xmit马上发送*/
            } else if (skb == tcp_send_head(sk)) /*第一个包，直接发送*/
                tcp_push_one(sk, mss_now);
                continue; /*说明发送队列前面还有skb等待发送，且距离之前push的包还不是非常久*/
wait_for_sndbuf:
                set_bit(SOCK_NOSPACE, &sk->sk_socket->flags);
wait_for_memory:
                if (copied)/*先把copied的发出去再等内存*/
                    tcp_push(sk, flags & ~MSG_MORE, mss_now, TCP_NAGLE_PUSH);
               /*阻塞等待内存*/
               if ((err = sk_stream_wait_memory(sk, &timeo)) != 0)
                   goto do_error;
               mss_now = tcp_send_mss(sk, &size_goal, flags);
          }
      }
 
out:
      if (copied) /*所有数据都放到发送队列中了，调用tcp_push发送*/
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
      if (copied)
          goto out;
out_err:
      err = sk_stream_error(sk, flags, err);
      TCP_CHECK_TIMER(sk);
      release_sock(sk);
      return err;
}
```
最终会调用tcp_push发送skb，而tcp_push又会调用tcp_write_xmit。tcp_sendmsg已经把数据按照GSO最大的size，放到一个个的skb中， 最终调用tcp_write_xmit发送这些GSO包。tcp_write_xmit会检查当前的拥塞窗口，还有nagle测试，tsq检查来决定是否能发送整个或者部分的skb， 如果只能发送一部分，则需要调用tso_fragment做切分。最后通过tcp_transmit_skb发送， 如果发送窗口没有达到限制，skb中存放的数据将达到GSO最大值。
l  **tcp_write_xmit**


```
static int tcp_write_xmit(struct sock *sk, unsigned int mss_now, int nonagle,
                                             int push_one, gfp_t gfp)
{
    struct tcp_sock *tp = tcp_sk(sk);
    struct sk_buff *skb;
    unsigned int tso_segs, sent_pkts;
    int cwnd_quota;
    int result;
 
    sent_pkts = 0;
 
    if (!push_one) {
        /* Do MTU probing. */
        result = tcp_mtu_probe(sk);
        if (!result) {
            return 0;
        } else if (result > 0) {
            sent_pkts = 1;
        }
    }
    /*遍历发送队列*/
    while ((skb = tcp_send_head(sk))) {
        unsigned int limit;
 
        tso_segs = tcp_init_tso_segs(sk, skb, mss_now); /*skb->len/mss，重新设置tcp_gso_segs，因为在tcp_sendmsg中被清零了*/
        BUG_ON(!tso_segs);
 
        cwnd_quota = tcp_cwnd_test(tp, skb);
        if (!cwnd_quota)
            break;
 
        if (unlikely(!tcp_snd_wnd_test(tp, skb, mss_now)))
            break;
 
        if (tso_segs == 1) { /*tso_segs=1表示无需tso分段*/
            /* 根据nagle算法，计算是否需要推迟发送数据 */
            if (unlikely(!tcp_nagle_test(tp, skb, mss_now, (tcp_skb_is_last(sk, skb) ? /*last skb就直接发送*/
                           nonagle : TCP_NAGLE_PUSH))))
                break;
        } else {/*有多个tso分段*/
            if (!push_one /*push所有skb*/
                 && tcp_tso_should_defer(sk, skb))/*/如果发送窗口剩余不多，并且预计下一个ack将很快到来(意味着可用窗口会增加)，则推迟发送*/
               break;
        }
        /*下面的逻辑是：不用推迟发送，马上发送的情况*/
        limit = mss_now;
        /*由于tso_segs被设置为skb->len/mss_now，所以开启gso时一定大于1*/
        if (tso_segs > 1 && !tcp_urg_mode(tp)) /*tso分段大于1且非urg模式*/
            limit = tcp_mss_split_point(sk, skb, mss_now, cwnd_quota);/*返回当前skb中可以发送的数据大小，通过mss和cwnd*/
        /* 当skb的长度大于限制时，需要调用tso_fragment分片,如果分段失败则暂不发送 */
        if (skb->len > limit &&
             unlikely(tso_fragment(sk, skb, limit, mss_now))) /*/按limit切割成多个skb*/
            break;
 
        TCP_SKB_CB(skb)->when = tcp_time_stamp;
        /*发送，如果包被qdisc丢了，则退出循环，不继续发送了*/
        if (unlikely(tcp_transmit_skb(sk, skb, 1, gfp)))
            break;
 
        /* Advance the send_head. This one is sent out.
         * This call will increment packets_out.
         */
        /*更新sk_send_head和packets_out*/
        tcp_event_new_data_sent(sk, skb);
        tcp_minshall_update(tp, mss_now, skb);
        sent_pkts++;
 
        if (push_one)
            break;
    }
 
    if (likely(sent_pkts)) {
        tcp_cwnd_validate(sk);
        return 0;
    }
    return !tp->packets_out && tcp_send_head(sk);
}
```
其中tcp_init_tso_segs会设置skb的gso信息后文分析。我们看到tcp_write_xmit 会调用tso_fragment进行“tcp分段”。而分段的条件是skb->len > limit。这里的关键就是limit的值，我们看到在tso_segs > 1时，也就是开启gso的时候，limit的值是由tcp_mss_split_point得到的，也就是min(skb->len, window)，即发送窗口允许的最大值。在没有开启gso时limit就是当前的mss。
l  **tcp_init_tso_segs**


```
static int tcp_init_tso_segs(struct sock *sk, struct sk_buff *skb, unsigned int mss_now)
{
    int tso_segs = tcp_skb_pcount(skb); /*skb_shinfo(skb)->gso_seg之前被初始化为0*/
 
    if (!tso_segs || (tso_segs > 1 && tcp_skb_mss(skb) != mss_now)) {
        tcp_set_skb_tso_segs(sk, skb, mss_now);
        tso_segs = tcp_skb_pcount(skb);
    }
    return tso_segs;
}
```


```
static void tcp_set_skb_tso_segs(struct sock *sk, struct sk_buff *skb, unsigned int mss_now)
{
    /* Make sure we own this skb before messing gso_size/gso_segs */
    WARN_ON_ONCE(skb_cloned(skb));
    if (skb->len <= mss_now || !sk_can_gso(sk) ||
        skb->ip_summed == CHECKSUM_NONE) {/*不支持gso的情况*/
       /* Avoid the costly divide in the normal
        * non-TSO case.
        */
       skb_shinfo(skb)->gso_segs = 1;
       skb_shinfo(skb)->gso_size = 0;
       skb_shinfo(skb)->gso_type = 0;
    } else {
        skb_shinfo(skb)->gso_segs = DIV_ROUND_UP(skb->len, mss_now); /*被设置为skb->len/mss_now*/
        skb_shinfo(skb)->gso_size = mss_now; /*注意mss_now为真实的mss，这里保存以供gso分段使用*/
        skb_shinfo(skb)->gso_type = sk->sk_gso_type;
    }
}
```
tcp_write_xmit最后会调用ip_queue_xmit发送skb，进入ip层。
## ip分片，tcp分段，GSO，TSO
之后的逻辑就是之前另一篇文章中分析的GSO逻辑了。下面我们看下整个协议栈中ip分片，tcp分段，GSO，TSO的关系。我将这个流程由下图表示。
![](http://blog.chinaunix.net/attachment/201705/6/28541347_1494074632KCCh.png)
