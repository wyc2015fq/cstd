
# tcp-ip-路由相关结构 - 3-Number - CSDN博客


2017年09月20日 16:21:23[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：168


# 0x01 缘由
上章节已经学习了网络层（ip层），其中关键部分就是找下一跳路由，本次学习主要围绕路由相关知识点进行学习。
# 0x02 相关结构
## 1.路由缓存机制:
```python
struct rtable
{
    union
    {
        struct dst_entry    dst; //目的条目
    } u;
    struct flowi        fl;//包含实际的hash键，存放的是查找该路由节点的哈希值，该哈希值用SRC_IP + DST_IP + TOS 确定。
    struct in_device    *idev; //该指针指向egress设备的IP配置块。注意对送往本地的ingress报文的路由，设置的egress设备为loopback设备
    int            rt_genid;
    unsigned        rt_flags;//能被提供应用程序接口的路由表使用。因为在单个 hash 桶内也许有多个路由，那么这些路由会冲突。当垃圾回收程序处理这些路由 cache，如果和高价值的路由发生冲突时，低价值的路由倾向于被清除出去，路由控制 flags 决定这些路由的值。
    __u16            rt_type; //路由的类型，它确定这些路由是否单播、组播或本地路由。
    __be32            rt_dst;    /* 用来存放目的IP地址    */
    __be32            rt_src;    /* 路由路径的起点ip地址*/
    int            rt_iif; /* iif 是路由输入接口索引 */
    __be32            rt_gateway; //网关或邻居的 IP 地址。
    /* Miscellaneous cached information */
    __be32            rt_spec_dst; /* RFC1122 specific destination */
    struct inet_peer    *peer; //用于long-living ip peer,虽然普通的IP报文没有状态，但是内核会记录IP报文的一些信息以提高效率，主要是记录IP报文的packet-id以检查是否收到了重复的报文，还需要检查packet-id的增量。
};
```

### 1.1流标识机制：
```python
struct flowi {
    /*
    下面两个字段确定 input 和 output 接口， iif 是输入接口索引，它是从 net_device 结构里的 ifIndex
获取的，这个 net_device 是指收到报文的设备。 oif 是输出接口索引。通常，对于一个指定路由 iif 或者
oif 会被赋值，而其他字段是 0。
    */
    int    oif;
    int    iif;
    __u32    mark; //防火墙 mark
    //下面这个结构是通用的，所以我们用联合来定义 Ipv4， Ipv6 和 DECnet：
    union {
        struct {
            __be32            daddr; //目的地址
            __be32            saddr; //源地址
            __u8            tos;     //服务类型标识
            __u8            scope; //范围标识
        } ip4_u;
        struct {
            struct in6_addr        daddr;
            struct in6_addr        saddr;
            __be32            flowlabel;
        } ip6_u;
        struct {
            __le16            daddr;
            __le16            saddr;
            __u8            scope;
        } dn_u;
    } nl_u;
#define fld_dst        nl_u.dn_u.daddr
#define fld_src        nl_u.dn_u.saddr
#define fld_scope    nl_u.dn_u.scope
#define fl6_dst        nl_u.ip6_u.daddr
#define fl6_src        nl_u.ip6_u.saddr
#define fl6_flowlabel    nl_u.ip6_u.flowlabel
#define fl4_dst        nl_u.ip4_u.daddr
#define fl4_src        nl_u.ip4_u.saddr
#define fl4_tos        nl_u.ip4_u.tos
#define fl4_scope    nl_u.ip4_u.scope
    __u8    proto; //协议类型
    __u8    flags; //标识
#define FLOWI_FLAG_ANYSRC 0x01
    union {
        struct {
            __be16    sport; //源端口
            __be16    dport; //目的端口
        } ports;
        struct {
            __u8    type;
            __u8    code;
        } icmpt;
        struct {
            __le16    sport;
            __le16    dport;
        } dnports;
        __be32        spi;
        struct {
            __u8    type;
        } mht;
    } uli_u;
#define fl_ip_sport    uli_u.ports.sport
#define fl_ip_dport    uli_u.ports.dport
#define fl_icmp_type    uli_u.icmpt.type
#define fl_icmp_code    uli_u.icmpt.code
#define fl_ipsec_spi    uli_u.spi
#define fl_mh_type    uli_u.mht.type
    __u32           secid;    /* used by xfrm; see secid.txt */
} __attribute__((__aligned__(BITS_PER_LONG/8)));
```
从上面结构定义可以看到，一个数据报文有源、目的地址端口，有 proto 选项，有用户定义的类型，甚至有入接口和出接口，那么，通过这些标识，就可以唯一的确定某用户的业务流。然后
你就可以对某一个指定的流查找其路由。好啦，可以这么说，路由是网络内不同业务流的标识， 而 flowi 是操作系统内部不同业务流的标识。内核通过从 TCP 或 IP 报文头中抽取相应的信息填入 到
flowi 结构中，然后路由查找模块根据这个信息为相应的流找到对应路由。所以说， flowi 就是一个查找 key。
## 2.路由查找过程
ip_route_input()给进来的包skb查找路由，现在缓冲区的路由hash表中查找，如果没找到，在调用ip_route_input_slow()到路由表中查找。
1.Linux内核没有名字叫做路由表(route table)的表，不要被 rtable 迷惑了，它不是存放真正路由的地方，它是cache。FIB表才是值得叫做路由表的东西。
2.FIB存放所有的路由信息，只有当要发送报文的时候（也许在接受报文的时候也是这样的）才将已经查过的路由信息放入route cache中，在没有进行数据通信之前，cache中是没有数据的。
![](https://img-blog.csdn.net/20170920162832243?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 0x03 源码跟踪
## 1.调用过程如下：
![](https://img-blog.csdn.net/20170920162859058?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 2.简单函数分析
### 2.1 ip_route_input
```python
int ip_route_input(struct sk_buff *skb, __be32 daddr, __be32 saddr,
           u8 tos, struct net_device *dev)
{
    struct rtable * rth;
    unsigned    hash;
    int iif = dev->ifindex;
    struct net *net;
    net = dev_net(dev);
    //路由缓存是否会超过系统设置
    if (!rt_caching(net))
        goto skip_cache;
    tos &= IPTOS_RT_MASK;
    //hash查找
    hash = rt_hash(daddr, saddr, iif, rt_genid(net));
    rcu_read_lock();
    for (rth = rcu_dereference(rt_hash_table[hash].chain); rth;
         rth = rcu_dereference(rth->u.dst.rt_next)) {
        if (((rth->fl.fl4_dst ^ daddr) |
             (rth->fl.fl4_src ^ saddr) |
             (rth->fl.iif ^ iif) |
             rth->fl.oif |
             (rth->fl.fl4_tos ^ tos)) == 0 &&
            rth->fl.mark == skb->mark &&
            net_eq(dev_net(rth->u.dst.dev), net) &&
            !rt_is_expired(rth)) {
            dst_use(&rth->u.dst, jiffies);
            RT_CACHE_STAT_INC(in_hit);
            rcu_read_unlock();
            skb_dst_set(skb, &rth->u.dst);
            return 0;
        }
        RT_CACHE_STAT_INC(in_hlist_search);
    }
    rcu_read_unlock();
skip_cache:
    /* 组播处理
     */
    if (ipv4_is_multicast(daddr)) {
        struct in_device *in_dev;
        rcu_read_lock();
        if ((in_dev = __in_dev_get_rcu(dev)) != NULL) {
            int our = ip_check_mc(in_dev, daddr, saddr,
                ip_hdr(skb)->protocol);
            if (our
#ifdef CONFIG_IP_MROUTE
                || (!ipv4_is_local_multicast(daddr) &&
                IN_DEV_MFORWARD(in_dev))
#endif
                ) {
                rcu_read_unlock();
                return ip_route_input_mc(skb, daddr, saddr,
                             tos, dev, our);
            }
        }
        rcu_read_unlock();
        return -EINVAL;
    }
    //查找路由慢路径，要查找fib路由
    return ip_route_input_slow(skb, daddr, saddr, tos, dev);
}
```

### 2.2 ip_route_input_slow
```python
static int ip_route_input_slow(struct sk_buff *skb, __be32 daddr, __be32 saddr,
                   u8 tos, struct net_device *dev)
{
    /* 略 */
    struct net    * net = dev_net(dev);
    /* ip禁用 */
    if (!in_dev)
        goto out;
    /* 组包和广播不检测
     */
    if (ipv4_is_multicast(saddr) || ipv4_is_lbcast(saddr) ||
        ipv4_is_loopback(saddr))
        goto martian_source;
    //地址校验
    if (daddr == htonl(0xFFFFFFFF) || (saddr == 0 && daddr == 0))
        goto brd_input;
    /* 略 */
    /*
     *    前面都是通过检查包的源和目的地址的合理性来确定对不同包的处理，开始路由包
     */
    if ((err = fib_lookup(net, &fl, &res)) != 0) { /*查找路由*/
        if (!IN_DEV_FORWARD(in_dev)) /*没找到，先判断转发标志是否打开*/
            goto e_hostunreach;
        goto no_route;
    }
    /*找到路由的标志*/
    free_res = 1;
    RT_CACHE_STAT_INC(in_slow_tot);
    /*根据查找到的路由类型，分类处理*/
    if (res.type == RTN_BROADCAST)
        goto brd_input;
    if (res.type == RTN_LOCAL) {
        int result;
        /*如果是发给本机的包，则验证原地址是否合法*/
        result = fib_validate_source(saddr, daddr, tos,
                         net->loopback_dev->ifindex,
                         dev, &spec_dst, &itag, skb->mark);
        if (result < 0)
            goto martian_source;
        if (result)
            flags |= RTCF_DIRECTSRC;
        spec_dst = daddr;
        goto local_input;
    }
    if (!IN_DEV_FORWARD(in_dev))
        goto e_hostunreach;
    if (res.type != RTN_UNICAST)
        goto martian_destination;
    /*当查到的路由类型是指向远端的主机，把此路由加入cache中*/
    err = ip_mkroute_input(skb, &res, &fl, in_dev, daddr, saddr, tos);
done:
    in_dev_put(in_dev);
    if (free_res)
        fib_res_put(&res);
out:    return err;
/*当目的地址是广播地址，或查到的路由类型是广播类型*/
brd_input:
    /* 略 ... ...*/
/*当查找到的路由指向本机时*/
local_input:
    /*分配缓存路由项空间，并以确定的spec_dest等信息给路由项赋值*/
    rth = dst_alloc(&ipv4_dst_ops);
    if (!rth)
        goto e_nobufs;
    /* 略 ... ...*/
    rth->u.dst.input= ip_local_deliver; /*路由查找结束后会调用此函数把报文送给上层处理*/
    /* 略 ... ...*/
    rth->rt_type    = res.type;
    hash = rt_hash(daddr, saddr, fl.iif, rt_genid(net));
    err = rt_intern_hash(hash, rth, NULL, skb);/*向缓存中插入新的路由项*/
    goto done;
/*没有查找到路由的时候，向缓存中添加一条不可达路由项*/
no_route:
    /* 略 ... ...*/
}
```
以上函数都是从ip_route_input函数开始，所调用到的部分函数。当网卡收到报文到达IP层后，IP层先作路由查询以便决定把它送到哪。而根据源与目的地址以及其他方面的检测，路由分了多播路由，广播路由，单播路由，还有本地路由。这些路由在cache中的生存策略也是不同的。一个绝对重点的函数赋值就是rth->u.dst.input，最后函数在IP层的分发就是通过这个函数实现。
# 0x04 总结
路由这块暂时分析到这里，毕竟已经有点偏离原来的主题，下一章节学习tcp层相关内容。(学习过程，大神勿喷，多指正)

