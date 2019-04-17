# TCP TIME_WAIT过多的解决方法 - DoubleLi - 博客园







总结：

最合适的解决方案是增加更多的四元组数目，比如，[服务器](https://www.baidu.com/s?wd=%E6%9C%8D%E5%8A%A1%E5%99%A8&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)监听端口，或服务器IP，让服务器能容纳足够多的TIME-WAIT状态连接。在我们常见的互联网架构中（[NGINX](https://www.baidu.com/s?wd=NGINX&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)反代跟NGINX，NGINX跟FPM，FPM跟redis、mysql、memcache等），减少TIME-WAIT状态的TCP连接，最有效的是使用长连接，不要用短连接，尤其是负载均衡跟web服务器之间。尤其是[链家事件中的PHP连不上redis](http://zhi.hu/oRZK)。


在服务端，不要启用net.ipv4.tcp_tw_recycle，除非你能确保你的服务器网络环境不是NAT。在服务端上启用net.ipv4.tw_reuse对于连接进来的TCP连接来说，并没有任何卵用。在客户端（尤其是服务器上，某服务以客户端形式运行时，比如上面提到的nginx反代，连接着redis、mysql的FPM等等）上启用net.ipv4.tcp_tw_reuse，还算稍微安全的解决TIME-WAIT的方案。再开启net.ipv4.tcp_tw_recycle的话，对客户端（或以客户端形式）的回收，也没有什么卵用，反而会发生很多诡异的事情（尤其是FPM这种服务器上，相对nginx是服务端，相对redis是客户端）





1. tw_reuse，tw_recycle 必须在客户端和服务端timestamps 开启时才管用（默认打开）

2. tw_reuse 只对客户端起作用，开启后客户端在1s内回收

3. tw_recycle 对客户端和服务器同时起作用，开启后在 3.5*RTO 内回收，RTO 200ms~ 120s 具体时间视网络状况。

　　内网状况比tw_reuse 稍快，公网尤其移动网络大多要比tw_reuse 慢，优点就是能够回收服务端的TIME_WAIT数量



## 对于客户端

1. 作为客户端因为有端口65535问题，TIME_OUT过多直接影响处理能力，打开tw_reuse 即可解决，不建议同时打开tw_recycle，帮助不大。

2. tw_reuse 帮助客户端1s完成连接回收，基本可实现单机6w/s请求，需要再高就增加IP数量吧。

3. 如果内网压测场景，且客户端不需要接收连接，同时tw_recycle 会有一点点好处。

4. 业务上也可以设计由服务端主动关闭连接



## 对于服务端

1. 打开tw_reuse无效

2. 线上环境 tw_recycle 不要打开

   服务器处于NAT 负载后，或者客户端处于NAT后（这是一定的事情，基本公司家庭网络都走NAT）；

　公网服务打开就可能造成部分连接失败，内网的话到时可以视情况打开；

   （分析：主机client1和client2通过NAT网关（1个ip地址）访问serverN，由于timestamp时间为系统启动到当前的时间，因此，client1和client2的timestamp不相同；根据上述syn包处理源码，在tcp_tw_recycle和tcp_timestamps同时开启的条件下，timestamp大的主机访问serverN成功，而timestmap小的主机访问失败）

   像我所在公司对外服务都放在负载后面，负载会把timestamp 都给清空，好吧，就算你打开也不起作用。

3. 服务器TIME_WAIT 高怎么办

   不像客户端有端口限制，处理大量TIME_WAIT Linux已经优化很好了，每个处于TIME_WAIT 状态下连接内存消耗很少，

而且也能通过tcp_max_tw_buckets = 262144 配置最大上限，现代机器一般也不缺这点内存。



参考博客：

http://blog.sina.com.cn/s/blog_781b0c850100znjd.html

http://www.tuicool.com/articles/3eYRb2A

http://www.cnblogs.com/lulu/p/4149312.html









