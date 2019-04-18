# Linux内核收包过程 - xcw0754 - 博客园
# [Linux内核收包过程](https://www.cnblogs.com/xcw0754/p/10079901.html)
net/core/dev.c
```
int __init net_dev_init(void)
{
    queue->backlog.poll = process_backlog;
    open_softirq(NET_TX_SOFTIRQ, net_tx_action, NULL);
    open_softirq(NET_RX_SOFTIRQ, net_rx_action, NULL);
}
```
/net/core/dev.c
```
int process_backlog(struct napi_struct *napi, int quota)
{
    int work = 0;
    struct softnet_data *queue = &__get_cpu_var(softnet_data);  // 取出cpu变量(就一个指针)
    unsigned long start_time = jiffies;
    napi->weight = weight_p;
    do {
        struct sk_buff *skb;    // 关键
        struct net_device *dev;
        local_irq_disable();    // 关中断
        skb = __skb_dequeue(&queue->input_pkt_queue);   // 从队列中取出来(暂不管怎么挂到队列的)
        if (!skb) {
            __napi_complete(napi);
            local_irq_enable();
            break;
        }
        local_irq_enable(); // 开中断
        dev = skb->dev;
        netif_receive_skb(skb); // 交给IP层
        dev_put(dev);   // 允许释放dev结构体
    } while (++work < quota && jiffies == start_time);
    return work;
}
```
include/linux/skbuff.h
```
struct sk_buff *__skb_dequeue(struct sk_buff_head *list)
{
    struct sk_buff *next, *prev, *result;
    prev = (struct sk_buff *) list;
    next = prev->next;
    result = NULL;
    if (next != prev) {
        result       = next;
        next         = next->next;
        list->qlen--;
        next->prev   = prev;
        prev->next   = next;
        result->next = result->prev = NULL;
    }
    return result;
}
```
net/core/dev.c
```
/*
 *  netif_receive_skb - process receive buffer from network
 *  @skb: buffer to process
 *
 *  netif_receive_skb() is the main receive data processing function.
 *  It always succeeds. The buffer may be dropped during processing
 *  for congestion control or by the protocol layers.
 *
 *  This function may only be called from softirq context and interrupts
 *  should be enabled. (看这行)
 *
 *  Return values (usually ignored):
 *  NET_RX_SUCCESS: no congestion
 *  NET_RX_DROP: packet was dropped
 */
int netif_receive_skb(struct sk_buff *skb)
{
    struct packet_type *ptype, *pt_prev;
    struct net_device *orig_dev;
    int ret = NET_RX_DROP;
    __be16 type;
    /* if we've gotten here through NAPI, check netpoll */
    if (netpoll_receive_skb(skb))
        return NET_RX_DROP;
    if (!skb->tstamp.tv64)
        net_timestamp(skb);
    if (!skb->iif)
        skb->iif = skb->dev->ifindex;
    orig_dev = skb_bond(skb);
    if (!orig_dev)
        return NET_RX_DROP;
    __get_cpu_var(netdev_rx_stat).total++;
    skb_reset_network_header(skb);  // 调整相关header指针
    skb_reset_transport_header(skb);
    skb->mac_len = skb->network_header - skb->mac_header;
    pt_prev = NULL;
    rcu_read_lock();
#ifdef CONFIG_NET_CLS_ACT
    if (skb->tc_verd & TC_NCLS) {
        skb->tc_verd = CLR_TC_NCLS(skb->tc_verd);
        goto ncls;
    }
#endif
    // 这里 ptype_all 是针对任何协议的，视为空即可
    list_for_each_entry_rcu(ptype, &ptype_all, list) {
        if (!ptype->dev || ptype->dev == skb->dev) {
            if (pt_prev)
                ret = deliver_skb(skb, pt_prev, orig_dev);  // 交给上层
            pt_prev = ptype;
        }
    }
#ifdef CONFIG_NET_CLS_ACT
    skb = handle_ing(skb, &pt_prev, &ret, orig_dev);
    if (!skb)
        goto out;
ncls:
#endif
    skb = handle_bridge(skb, &pt_prev, &ret, orig_dev);
    if (!skb)
        goto out;
    skb = handle_macvlan(skb, &pt_prev, &ret, orig_dev);
    if (!skb)
        goto out;
    type = skb->protocol;
    list_for_each_entry_rcu(ptype, &ptype_base[ntohs(type)&15], list) {
        // ptype_base是协议栈，见inet_init(void)中的dev_add_pack(&ip_packet_type)
        if (ptype->type == type && (!ptype->dev || ptype->dev == skb->dev)) {
            if (pt_prev)
                ret = deliver_skb(skb, pt_prev, orig_dev);  // 交给上层
            pt_prev = ptype;
        }
    }
    if (pt_prev) {
        ret = pt_prev->func(skb, skb->dev, pt_prev, orig_dev);  // 这个也关键
    } else {
        kfree_skb(skb);
        ret = NET_RX_DROP;
    }
out:
    rcu_read_unlock();
    return ret;
}
```
net/core/dev.c
```
static struct packet_type ip_packet_type = {
    .type = __constant_htons(ETH_P_IP),
    .func = ip_rcv,
    .gso_send_check = inet_gso_send_check,
    .gso_segment = inet_gso_segment,
};
static int __init inet_init(void)
{
    ...
    dev_add_pack(&ip_packet_type);
    ...
}
```
net/core/dev.c
```
/**
 *  dev_add_pack - add packet handler
 *  @pt: packet type declaration
 *
 *  Add a protocol handler to the networking stack. The passed &packet_type
 *  is linked into kernel lists and may not be freed until it has been
 *  removed from the kernel lists.
 *
 *  This call does not sleep therefore it can not
 *  guarantee all CPU's that are in middle of receiving packets
 *  will see the new packet type (until the next received packet).
 */
void dev_add_pack(struct packet_type *pt)
{
    int hash;
    spin_lock_bh(&ptype_lock);
    if (pt->type == htons(ETH_P_ALL))
        list_add_rcu(&pt->list, &ptype_all);
    else {
        hash = ntohs(pt->type) & 15;
        list_add_rcu(&pt->list, &ptype_base[hash]); // 这里的ptype_base
    }
    spin_unlock_bh(&ptype_lock);
}
```
net/ipv4/ip_input.c
```
/*
 *  Main IP Receive routine.
 *  IP层入口函数
 */
int ip_rcv(struct sk_buff *skb, struct net_device *dev, struct packet_type *pt, struct net_device *orig_dev)
{
    struct iphdr *iph;
    u32 len;
    if (dev->nd_net != &init_net)
        goto drop;
    if (skb->pkt_type == PACKET_OTHERHOST)
        goto drop;
    IP_INC_STATS_BH(IPSTATS_MIB_INRECEIVES);
    if ((skb = skb_share_check(skb, GFP_ATOMIC)) == NULL) {
        IP_INC_STATS_BH(IPSTATS_MIB_INDISCARDS);
        goto out;
    }
    // 若包不完整，考虑重组
    if (!pskb_may_pull(skb, sizeof(struct iphdr)))
        goto inhdr_error;
    iph = ip_hdr(skb);
    if (iph->ihl < 5 || iph->version != 4)
        goto inhdr_error;
    if (!pskb_may_pull(skb, iph->ihl*4))
        goto inhdr_error;
    iph = ip_hdr(skb);
    if (unlikely(ip_fast_csum((u8 *)iph, iph->ihl)))
        goto inhdr_error;
    len = ntohs(iph->tot_len);
    if (skb->len < len) {
        IP_INC_STATS_BH(IPSTATS_MIB_INTRUNCATEDPKTS);
        goto drop;
    } else if (len < (iph->ihl*4))
        goto inhdr_error;
    if (pskb_trim_rcsum(skb, len)) {
        IP_INC_STATS_BH(IPSTATS_MIB_INDISCARDS);
        goto drop;
    }
    /* Remove any debris in the socket control block */
    memset(IPCB(skb), 0, sizeof(struct inet_skb_parm));
    // 钩子: NF_IP_PRE_ROUTING,关键看ip_rcv_finish。
    return NF_HOOK(PF_INET, NF_IP_PRE_ROUTING, skb, dev, NULL,
               ip_rcv_finish);
inhdr_error:
    IP_INC_STATS_BH(IPSTATS_MIB_INHDRERRORS);
drop:
    kfree_skb(skb);
out:
    return NET_RX_DROP;
}
```
net/ipv4/ip_input.c
```
static int ip_rcv_finish(struct sk_buff *skb)
{
    const struct iphdr *iph = ip_hdr(skb);
    struct rtable *rt;
    if (skb->dst == NULL) {
        // 初始化skb->dst,即路由过程
        int err = ip_route_input(skb, iph->daddr, iph->saddr, iph->tos,
                     skb->dev);
        if (unlikely(err)) {
            if (err == -EHOSTUNREACH)
                IP_INC_STATS_BH(IPSTATS_MIB_INADDRERRORS);
            else if (err == -ENETUNREACH)
                IP_INC_STATS_BH(IPSTATS_MIB_INNOROUTES);
            goto drop;
        }
    }
#ifdef CONFIG_NET_CLS_ROUTE
    if (unlikely(skb->dst->tclassid)) {
        struct ip_rt_acct *st = ip_rt_acct + 256*smp_processor_id();
        u32 idx = skb->dst->tclassid;
        st[idx&0xFF].o_packets++;
        st[idx&0xFF].o_bytes+=skb->len;
        st[(idx>>16)&0xFF].i_packets++;
        st[(idx>>16)&0xFF].i_bytes+=skb->len;
    }
#endif
    if (iph->ihl > 5 && ip_rcv_options(skb))
        goto drop;
    rt = (struct rtable*)skb->dst;
    if (rt->rt_type == RTN_MULTICAST)
        IP_INC_STATS_BH(IPSTATS_MIB_INMCASTPKTS);
    else if (rt->rt_type == RTN_BROADCAST)
        IP_INC_STATS_BH(IPSTATS_MIB_INBCASTPKTS);
    return dst_input(skb);  // 交给上层
drop:
    kfree_skb(skb);
    return NET_RX_DROP;
}
```
net/ipv4/ip_input.c
```
// 交给上层，没什么好看的
int dst_input(struct sk_buff *skb)
{
    int err;
    for (;;) {
        err = skb->dst->input(skb); // 执行
        if (likely(err == 0))
            return err;
        /* Oh, Jamal... Seems, I will not forgive you this mess. :-) */
        if (unlikely(err != NET_XMIT_BYPASS))
            return err;
    }
}
```
对于ipv4来说，路由结果其实只有两种:ip_local_deliver和ip_forward，分别表示提交到本地、转发。咱只关注提交到本地的。
net/ipv4/ip_input.c
```
/*
 *  Deliver IP Packets to the higher protocol layers.
 */
int ip_local_deliver(struct sk_buff *skb)
{
    /*
     *  Reassemble IP fragments.
     */
    if (ip_hdr(skb)->frag_off & htons(IP_MF | IP_OFFSET)) {
        if (ip_defrag(skb, IP_DEFRAG_LOCAL_DELIVER))
            return 0;
    }
    // 又一钩子
    return NF_HOOK(PF_INET, NF_IP_LOCAL_IN, skb, skb->dev, NULL,
               ip_local_deliver_finish);    // 看回调
}
```
net/ipv4/ip_input.c
```
static int ip_local_deliver_finish(struct sk_buff *skb)
{
    __skb_pull(skb, ip_hdrlen(skb));
    /* Point into the IP datagram, just past the header. */
    skb_reset_transport_header(skb);
    rcu_read_lock();
    {
        /* Note: See raw.c and net/raw.h, RAWV4_HTABLE_SIZE==MAX_INET_PROTOS */
        int protocol = ip_hdr(skb)->protocol;
        int hash;
        struct sock *raw_sk;
        struct net_protocol *ipprot;
    resubmit:
        hash = protocol & (MAX_INET_PROTOS - 1);
        raw_sk = sk_head(&raw_v4_htable[hash]);
        /* If there maybe a raw socket we must check - if not we
         * don't care less
         */
        if (raw_sk && !raw_v4_input(skb, ip_hdr(skb), hash))    // 关键
            raw_sk = NULL;
        if ((ipprot = rcu_dereference(inet_protos[hash])) != NULL) {
            int ret;
            if (!ipprot->no_policy) {
                if (!xfrm4_policy_check(NULL, XFRM_POLICY_IN, skb)) {
                    kfree_skb(skb);
                    goto out;
                }
                nf_reset(skb);
            }
            ret = ipprot->handler(skb);
            if (ret < 0) {
                protocol = -ret;
                goto resubmit;
            }
            IP_INC_STATS_BH(IPSTATS_MIB_INDELIVERS);
        } else {
            if (!raw_sk) {
                if (xfrm4_policy_check(NULL, XFRM_POLICY_IN, skb)) {
                    IP_INC_STATS_BH(IPSTATS_MIB_INUNKNOWNPROTOS);
                    icmp_send(skb, ICMP_DEST_UNREACH,
                          ICMP_PROT_UNREACH, 0);
                }
            } else
                IP_INC_STATS_BH(IPSTATS_MIB_INDELIVERS);
            kfree_skb(skb);
        }
    }
 out:
    rcu_read_unlock();
    return 0;
}
```
/net/ipv4/raw.c
```
int raw_v4_input(struct sk_buff *skb, struct iphdr *iph, int hash)
{
    struct sock *sk;
    struct hlist_head *head;
    int delivered = 0;
    read_lock(&raw_v4_lock);
    head = &raw_v4_htable[hash];
    if (hlist_empty(head))
        goto out;
    sk = __raw_v4_lookup(__sk_head(head), iph->protocol,
                 iph->saddr, iph->daddr,
                 skb->dev->ifindex);
    while (sk) {
        delivered = 1;
        if (iph->protocol != IPPROTO_ICMP || !icmp_filter(sk, skb)) {
            struct sk_buff *clone = skb_clone(skb, GFP_ATOMIC);
            /* Not releasing hash table! */
            if (clone)
                raw_rcv(sk, clone); // 关键
        }
        sk = __raw_v4_lookup(sk_next(sk), iph->protocol,
                     iph->saddr, iph->daddr,
                     skb->dev->ifindex);
    }
out:
    read_unlock(&raw_v4_lock);
    return delivered;
}
```
/net/ipv4/raw.c
```
int raw_rcv(struct sock *sk, struct sk_buff *skb)
{
    if (!xfrm4_policy_check(sk, XFRM_POLICY_IN, skb)) {
        kfree_skb(skb);
        return NET_RX_DROP;
    }
    nf_reset(skb);
    skb_push(skb, skb->data - skb_network_header(skb));
    raw_rcv_skb(sk, skb);   // 关键
    return 0;
}
```
/net/ipv4/raw.c
```
static int raw_rcv_skb(struct sock * sk, struct sk_buff * skb)
{
    /* Charge it to the socket. */
    if (sock_queue_rcv_skb(sk, skb) < 0) {  // 关键
        /* FIXME: increment a raw drops counter here */
        kfree_skb(skb);
        return NET_RX_DROP;
    }
    return NET_RX_SUCCESS;
}
```
/net/core/sock.c
```
int sock_queue_rcv_skb(struct sock *sk, struct sk_buff *skb)
{
    int err = 0;
    int skb_len;
    /* Cast skb->rcvbuf to unsigned... It's pointless, but reduces
       number of warnings when compiling with -W --ANK
     */
    if (atomic_read(&sk->sk_rmem_alloc) + skb->truesize >=
        (unsigned)sk->sk_rcvbuf) {
        err = -ENOMEM;
        goto out;
    }
    err = sk_filter(sk, skb);
    if (err)
        goto out;
    skb->dev = NULL;
    skb_set_owner_r(skb, sk);
    /* Cache the SKB length before we tack it onto the receive
     * queue.  Once it is added it no longer belongs to us and
     * may be freed by other threads of control pulling packets
     * from the queue.
     */
    skb_len = skb->len;
    skb_queue_tail(&sk->sk_receive_queue, skb); // 关键
    if (!sock_flag(sk, SOCK_DEAD))
        sk->sk_data_ready(sk, skb_len); // 通知已有数据了，如果有block的监听者就可以返回。
out:
    return err;
}
```
/net/core/skbuff.c
```
void skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk)
{
    unsigned long flags;
    spin_lock_irqsave(&list->lock, flags);
    __skb_queue_tail(list, newsk);      // 关键
    spin_unlock_irqrestore(&list->lock, flags);
}
```

