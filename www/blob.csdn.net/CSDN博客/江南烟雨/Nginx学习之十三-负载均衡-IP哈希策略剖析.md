# Nginx学习之十三-负载均衡-IP哈希策略剖析 - 江南烟雨 - CSDN博客
2013年07月15日 17:29:28[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：22435
所属专栏：[Nginx模块开发与原理剖析](https://blog.csdn.net/column/details/nginxroad.html)

前面介绍过nginx负载均衡的加权轮询策略（[http://blog.csdn.net/xiajun07061225/article/details/9318871](http://blog.csdn.net/xiajun07061225/article/details/9318871)），它是Nginx负载均衡的基础策略，所以一些初始化工作，比如配置值转储，其他策略可以直接复用他。在后面的初始化的代码中将可以看到。
注：本文中源代码版本为Nginx-1.4.0。
## IP哈希初始化
IP哈希的初始化函数ngx_http_upstream_init_ip_hash（ngx_http_upstream_ip_hash_module.c）：
```cpp
static ngx_int_t
ngx_http_upstream_init_ip_hash(ngx_conf_t *cf, ngx_http_upstream_srv_conf_t *us)
{
    //调用了加权轮询
    if (ngx_http_upstream_init_round_robin(cf, us) != NGX_OK) {
        return NGX_ERROR;
    }
    //修改了针对单个请求进行初始化的回调函数
    us->peer.init = ngx_http_upstream_init_ip_hash_peer;
    return NGX_OK;
}
```
## 选择后端服务器
当客户端请求过来之后，将会执行初始化函数ngx_http_upstream_init_ip_hash_peer。其中调用了轮询算法中的初始化函数。
源代码：
```cpp
static ngx_int_t
ngx_http_upstream_init_ip_hash_peer(ngx_http_request_t *r,
    ngx_http_upstream_srv_conf_t *us)
{
    struct sockaddr_in                     *sin;
    //针对IPv6的支持
#if (NGX_HAVE_INET6)
    struct sockaddr_in6                    *sin6;
#endif
    ngx_http_upstream_ip_hash_peer_data_t  *iphp;
    iphp = ngx_palloc(r->pool, sizeof(ngx_http_upstream_ip_hash_peer_data_t));
    if (iphp == NULL) {
        return NGX_ERROR;
    }
    r->upstream->peer.data = &iphp->rrp;
    //调用了RR算法中的初始化函数
    if (ngx_http_upstream_init_round_robin_peer(r, us) != NGX_OK) {
        return NGX_ERROR;
    }
    //回调函数设置，具体做选择的回调函数
    r->upstream->peer.get = ngx_http_upstream_get_ip_hash_peer;
    switch (r->connection->sockaddr->sa_family) {
    //保存客户端地址
    case AF_INET:
        sin = (struct sockaddr_in *) r->connection->sockaddr;
        iphp->addr = (u_char *) &sin->sin_addr.s_addr;
        //转储IPv4只用到了前3个字节，因为在后面的hash计算过程中只用到了3个字节
        iphp->addrlen = 3;
        break;
#if (NGX_HAVE_INET6)
    case AF_INET6:
        sin6 = (struct sockaddr_in6 *) r->connection->sockaddr;
        iphp->addr = (u_char *) &sin6->sin6_addr.s6_addr;
        iphp->addrlen = 16;
        break;
#endif
    default:
        iphp->addr = ngx_http_upstream_ip_hash_pseudo_addr;
        iphp->addrlen = 3;
    }
    //初始化hash种子
    iphp->hash = 89;
    //初始化尝试失败次数
    iphp->tries = 0;
    //做RR选择的函数
    iphp->get_rr_peer = ngx_http_upstream_get_round_robin_peer;
    return NGX_OK;
}
```
其中结构体ngx_http_upstream_ip_hash_peer_data_t：
```cpp
typedef struct {
    /* the round robin data must be first */
    ngx_http_upstream_rr_peer_data_t   rrp;
    //hash种子值
    ngx_uint_t                         hash;
    //IP地址
    u_char                             addrlen;
    u_char                            *addr;
    //尝试连接的次数
    u_char                             tries;
    ngx_event_get_peer_pt              get_rr_peer;
} ngx_http_upstream_ip_hash_peer_data_t;
typedef struct {
    //指向所有服务器的指针
    ngx_http_upstream_rr_peers_t   *peers;
    //当前服务器
    ngx_uint_t                      current;
    //指向位图的指针
    uintptr_t                      *tried;
    //位图的实际存储位置
    uintptr_t                       data;
} ngx_http_upstream_rr_peer_data_t;
typedef struct ngx_http_upstream_rr_peers_s  ngx_http_upstream_rr_peers_t;
struct ngx_http_upstream_rr_peers_s {
    ngx_uint_t                      number;//所有服务器地址总数
 /* ngx_mutex_t                    *mutex; */
    ngx_uint_t                      total_weight;//所有服务总权重
    unsigned                        single:1;//是否只有一个后端服务
    unsigned                        weighted:1;//number ！= total_weight ?
    ngx_str_t                      *name;
    ngx_http_upstream_rr_peers_t   *next;
    ngx_http_upstream_rr_peer_t     peer[1];
};
```
具体做选择的函数是ngx_http_upstream_get_ip_hash_peer：
```cpp
static ngx_int_t
ngx_http_upstream_get_ip_hash_peer(ngx_peer_connection_t *pc, void *data)
{
    ngx_http_upstream_ip_hash_peer_data_t  *iphp = data;
    time_t                        now;
    ngx_int_t                     w;
    uintptr_t                     m;
    ngx_uint_t                    i, n, p, hash;
    ngx_http_upstream_rr_peer_t  *peer;
    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, pc->log, 0,
                   "get ip hash peer, try: %ui", pc->tries);
    /* TODO: cached */
    //如果失败次数太多，或者只有一个后端服务，那么直接做RR选择
    if (iphp->tries > 20 || iphp->rrp.peers->single) {
        return iphp->get_rr_peer(pc, &iphp->rrp);
    }
    now = ngx_time();
    pc->cached = 0;
    pc->connection = NULL;
    hash = iphp->hash;
    for ( ;; ) {
        //计算IP的hash值
        for (i = 0; i < iphp->addrlen; i++) {
            //113质数，可以让哈希结果更散列
            hash = (hash * 113 + iphp->addr[i]) % 6271;
        }
        //根据哈希结果得到被选中的后端服务器
        if (!iphp->rrp.peers->weighted) {
            p = hash % iphp->rrp.peers->number;
        } else {
            w = hash % iphp->rrp.peers->total_weight;
            for (i = 0; i < iphp->rrp.peers->number; i++) {
                w -= iphp->rrp.peers->peer[i].weight;
                if (w < 0) {
                    break;
                }
            }
            p = i;
        }
        //服务器对应在位图中的位置计算
        n = p / (8 * sizeof(uintptr_t));
        m = (uintptr_t) 1 << p % (8 * sizeof(uintptr_t));
        if (!(iphp->rrp.tried[n] & m)) {
            ngx_log_debug2(NGX_LOG_DEBUG_HTTP, pc->log, 0,
                           "get ip hash peer, hash: %ui %04XA", p, m);
            //获取服务器
            peer = &iphp->rrp.peers->peer[p];
            /* ngx_lock_mutex(iphp->rrp.peers->mutex); */
            //服务器未挂掉
            if (!peer->down) {
                //失败次数已达上限
                if (peer->max_fails == 0 || peer->fails < peer->max_fails) {
                    break;
                }
                if (now - peer->checked > peer->fail_timeout) {
                    peer->checked = now;
                    break;
                }
            }
            //更改位图标记值
            iphp->rrp.tried[n] |= m;
            /* ngx_unlock_mutex(iphp->rrp.peers->mutex); */
            //在连接一个远端服务器时，当前连接异常失败后可以尝试的次数
            pc->tries--;
        }
        //已经尝试的次数超过阈值，采用RR轮询
        if (++iphp->tries >= 20) {
            return iphp->get_rr_peer(pc, &iphp->rrp);
        }
    }
    //当前服务索引
    iphp->rrp.current = p;
    //服务器地址及名字保存
    pc->sockaddr = peer->sockaddr;
    pc->socklen = peer->socklen;
    pc->name = &peer->name;
    /* ngx_unlock_mutex(iphp->rrp.peers->mutex); */
    //位图更新
    iphp->rrp.tried[n] |= m;
    //保留种子，使下次get_ip_hash_peer的时候能够选到同一个peer上
    iphp->hash = hash;
    return NGX_OK;
}
```
上述计算过程中，依据ip的hash值进行映射的时候，依据服务器列表头部结构中weighted字段分了两种不同的情况。
下面看看weighted的计算过程（ngx_http_upstream_round_robin.c）：
```cpp
//指向服务器列表指针        
        server = us->servers->elts;
        n = 0;
        w = 0;
        //遍历服务器列表，计算地址总数以及总的权值
        for (i = 0; i < us->servers->nelts; i++) {
            if (server[i].backup) {
                continue;
            }
            n += server[i].naddrs;
            w += server[i].naddrs * server[i].weight;
        }
        //weighted计算
        peers->weighted = (w != n);
```
server的类型是ngx_http_upstream_server_t。
```cpp
typedef struct {
    ngx_addr_t                      *addrs;//指向存储IP地址的数组的指针，host信息(对应的是 ngx_url_t->addrs )
    ngx_uint_t                       naddrs;//与第一个参数配合使用，数组元素个数(对应的是 ngx_url_t->naddrs )
    ngx_uint_t                       weight;//权值
    ngx_uint_t                       max_fails;
    time_t                           fail_timeout;
    unsigned                         down:1;
    unsigned                         backup:1;
} ngx_http_upstream_server_t;
```
一个域名可能对应多个IP地址。
server wegiht 字段，作为server权重，对应虚拟节点数目。
具体算法，将每个server虚拟成n个节点，均匀分布到hash环上，每次请求，根据配置的参数计算出一个hash值，在hash环上查找离这个hash最近的虚拟节点，对应的server作为该次请求的后端机器。
因此，当weighted字段等于0的时候，表示虚拟节点数和IP地址数是相等的，因此直接将hash值针对ip地址总数取模即可。如果weighted不等于0，表示虚拟节点数和IP地址数不等，因此需要按照虚拟节点数另外计算。查找离这个hash最近的虚拟节点，作为该请求的后端机器。
## 整个IP哈希选择流程
流程图如下：
- ![](https://img-blog.csdn.net/20130715172756578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 轮询策略和IP哈希策略对比
加权轮询策略
优点：适用性更强，不依赖于客户端的任何信息，完全依靠后端服务器的情况来进行选择。能把客户端请求更合理更均匀地分配到各个后端服务器处理。
缺点：同一个客户端的多次请求可能会被分配到不同的后端服务器进行处理，无法满足做会话保持的应用的需求。
IP哈希策略
优点：能较好地把同一个客户端的多次请求分配到同一台服务器处理，避免了加权轮询无法适用会话保持的需求。
缺点：当某个时刻来自某个IP地址的请求特别多，那么将导致某台后端服务器的压力可能非常大，而其他后端服务器却空闲的不均衡情况、
## 参考资料：
[http://blog.dccmx.com/2011/07/nginx-upstream-src-1/](http://blog.dccmx.com/2011/07/nginx-upstream-src-1/)
[http://itoedr.blog.163.com/blog/static/120284297201341044034750/](http://itoedr.blog.163.com/blog/static/120284297201341044034750/)
《深入剖析Nginx》
《深入理解Nginx-模块开发与架构解析》
