# 利用Memcached的反射型DDOS攻击技术分析 - Spark高级玩法 - CSDN博客
2018年04月15日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1800
**背景**
最近几天，我们看到利用Memcached服务的基于UDP协议进行放大反射的DDoS攻击数量大增，攻击者发送大量带有被害者IP地址的UDP数据包给Memcached服务，从而导致Memcached主机对伪造的IP地址源做出大量回应，形成分布式拒绝服务攻击。
360网络安全研究院通过全网DDOS实时攻击的采样数据可以很清楚的看到，这种利用Memcached服务进行DDOS的攻击事件在最近几天突然大量增加：
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/2AqAgxkehicib4ySUT68ia5lhCFFYcFcAsbJhebXDj8JTkBhIxSjkicFWEJPdfkKYiaqB8iarNgIFsJ4U1fad7KLthYw/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
360网络研究院对目前此类反射型攻击状况做了深入分析，更详情请参考链接：
http://blog.netlab.360.com/what-we-know-about-memcache-udp-reflection-ddos/
**Memcached**
Memcached是一个高性能的分布式内存对象缓存系统，用于动态Web应用以减轻数据库负载。它通过在内存中缓存数据和对象来减少读取数据库的次数，从而提高动态、数据库驱动网站的速度。
Memcached通过缓存数据库查询结果，减少数据库访问次数，以提高动态Web应用的速度、提高可扩展性：
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/2AqAgxkehicib4ySUT68ia5lhCFFYcFcAsbzGZEKsOx7oa4VyKcPof89XNNodxjOuuHgmRtajGqoIiaoZm09icIMy3w/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
**基于UDP协议的反射型DDOS攻击**
分布式拒绝服务（DDoS：Distributed Denial of Service）攻击指借助于客户/服务器技术，将多个计算机联合起来作为攻击平台，对一个或多个目标发动拒绝服务攻击，从而成倍地提高拒绝服务攻击的威力。
而本次事件中基于UDP协议的DDOS反射攻击靠的是发送大量带有被害者IP地址的UDP数据包给放大器主机（Memcached），最后放大器主机对伪造的IP地址源做出大量回应，形成分布式拒绝服务攻击。这样就巧妙的利用了无需交互认证的服务（UDP）将DDOS攻击数据“放大”并“反射”到受害者主机IP上。而黑客往往会选择那些响应包远大于请求包的UDP服务，这样只需要极小的攻击流量就可以造成上百Gbps的攻击效果。
下图引用了CLOUDFLARE对UDP协议的DDOS反射攻击图例来说明攻击过程：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/2AqAgxkehicib4ySUT68ia5lhCFFYcFcAsbxlyxPROrKNyDW3KhqjfBmtWGJQJ8GwA94Nu30ImLIbnLxhXu2t8iaLA/640?wx_fmt=png)
**基于Memcached的反射型攻击技术分析**
利用Memcache的反射型攻击最初由360信息安全部0kee Team在2017年6月发现，并于当年11月在 PoC 2017 会议上做了公开报告，详细报告可参见参考链接[6]。
**要完成一次UDP反射放大攻击，需要满足以下几个基本条件：**
- 
作为攻击者，需要能伪造IP，并发送海量伪造来源的请求。
- 
作为反射服务器，上面需要运行着容易放大反射攻击的服务，而运行了设计不当的UDP协议的服务是最佳的选择。
- 
反射服务器的响应包最好远远大于请求包，这样才能使用较小的攻击流量进行高流量的DDOS攻击。
- 
反射服务器对应的协议或服务在互联网上有一定的使用量，比如本次攻击中的Memcached。
**而Memcached提供的基于UDP协议的服务正好满足上诉的UDP反射放大攻击的各种条件：**
- 
从协议看，Memcached支持UDP。
- 
Memcached大部分是作为企业应用的组件，往往具有很高的上传带宽。
- 
Memcached不需要认证就可以随意交互。
- 
很多用户编译安装时，错误的监听了0.0.0.0，且未设置iptables或云安全组，这使得攻击者可以自由访问这些服务，从而进行UDP反射攻击。
**攻击流程**
**完成一次针对Memcached的UDP反射放大攻击，基本攻击流程如下：**
- 
扫描端口和服务，抓取协议指纹，获取未认证的Memcached。
- 
过滤出可以反射的UDP Memcached，再获取可以成功放大的Memcached主机IP地址。
- 
向可以放大UDP返回数据的Memcached主机IP发送伪造成被攻击主机IP的UDP包。
- 
Memcached主机IP将放大了上万倍的UDP数据发送给被攻击主机IP，造成DDOS攻击。
**放大倍数**
测试了多个Memcached Server，反射放大系数是有上限的，放大倍数最高可以在6W倍左右，使用tcpdump分析请求响应的报文：
**攻击机器A：**
测试指令：
python -c "print 'gets a b c d e
'" |nc -nvvu 94.**.251.*** 11211 |less
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/2AqAgxkehicib4ySUT68ia5lhCFFYcFcAsbAUYUhfrIwnTfMWGZEBK0C0LLEZhfYGyjE14icYkQbwbdrjmBJhUEvFg/640?wx_fmt=png)
**被攻击机器B：**
测试指令：
tcpdump -ni eth0 port 11211 -t
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/2AqAgxkehicib4ySUT68ia5lhCFFYcFcAsbhkbia7nyZrUwASa2qkCxZRD3paricI3Sic0U9uW1cRHFR0icWib5SjIFozg/640?wx_fmt=png)
可以看到，一个15字节的UDP请求触发了分片后总量达到近百个，单个1400字节长度的响应，重复数百次，DDOS攻击的放大倍数达到了近万倍。
**利用条件**
- 
Memcached服务必须开了UDP 11211端口。
- 
可被利用的Memcached存在Authentication Disable缺陷，即无需认证。
- 
Memcached服务存在的节点有比较充足的带宽资源。
以上3个条件必须同时满足时，才可以打出放大倍数比较大的反射流量。
**影响面**
国内影响面统计，国内受影响Memcached服务器数量在20000+
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/2AqAgxkehicib4ySUT68ia5lhCFFYcFcAsbiaQUptU3oWicl0Ya5icc3XGibdTdlgHTOdoc44RWMcYJDq6qHziaKDyPduQ/640?wx_fmt=png)
全球影响面统计，全球受影响Memcached服务器数量在10万左右
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/2AqAgxkehicib4ySUT68ia5lhCFFYcFcAsbvkreOgagGTevc7xNn3T2SUcNWO9RSJXEyvibaADUujImS5KXQ7zYicUQ/640?wx_fmt=png)
**可行缓解措施**
非常时期，建议采取快速有效防御措施：
**针对于国内存在风险的机器：**
云厂商、运营商、IDC限流UDP 11211双向流量；若有强业务需求最好单独申请点对点白名单，然后启用放行机制。
**针对于国外存在风险的机器：**
在国际出口限制UDP源端口为11211的回包流量，封堵利用国外机器打国内的情况。
**参考**
[1] https://blog.cloudflare.com/memcrashed-major-amplification-attacks-from-port-11211/
[2] http://blog.netlab.360.com/what-we-know-about-memcache-udp-reflection-ddos/
[3] https://www.anquanke.com/post/id/87233
[4] https://ddosmon.net/insight/
[5] https://github.com/Memcached/Memcached/blob/master/doc/protocol.txt
[6] http://powerofcommunity.net/poc2017/shengbao.pdf
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
