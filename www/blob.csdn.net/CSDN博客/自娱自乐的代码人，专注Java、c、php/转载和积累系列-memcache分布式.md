# 转载和积累系列 - memcache分布式 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年11月02日 09:50:25[initphp](https://me.csdn.net/initphp)阅读数：1299









近日和同事探讨到关于 Memcached 本身是否是可以实现分布式和负载均衡的问题有些争论，不妨认真探讨一下。

## Memcached 的自我定位

在 Memcache 的[官方主页](http://memcached.org)的 “What is Memcached” 部分中，它将自己定位为一个“开源的，高性能的，分布式内存缓存系统”。

> 
Free & open source, high-performance, distributed memory object caching system, generic in nature, but intended for use in speeding up dynamic web applications by alleviating database load.


这说明，Memcache 认为自己是一个分布式系统。但是，这是一个怎样的分布式系统，是否满足软件架构中对于分布式的精确定义呢？还有对于负载均衡它是如何处理的呢？

## 什么是分布式？

在 [wikipedia](http://en.wikipedia.org/wiki/Distributed_computing) 上可以看到分布式计算的定义：

> 
Distributed computing is a field of computer science that studies distributed systems. A distributed system consists of multiple autonomous computers that communicate through a computer network. The computers interact with each other in order to achieve
 a common goal.


对于什么样的架构才属于分布式计算，此 wiki 中也给出了两条通用的定义：
- 有多个可以自治的计算实体，每个都有私有的本地内存。
- 实体间通过消息机制来相互通信。

这里说的计算实体不等同于物理上的计算机节点，一台物理计算机上可以多个计算实体。

一个分布式系统可能有一个共同目标，比如解决一个大型计算问题。每个节点也都可以面向有独立需求的个体。一个分布式系统的目标是协调共享资源的使用，或者向用户提供通信服务。

另外一个典型的分布式系统的定义是：
- 系统可容忍部分节点出现故障
- 系统的架构（网络拓扑，网络延迟，节点数量）并不可预知，系统可能包含多种计算机和网络连接，在分布式程序上也可有所不同
- 每一个节点都包含有限的，不完整的系统视界。每个节点都只知晓部分输入。

顺便提一句，分布式计算和[并行计算](http://en.wikipedia.org/wiki/Parallel_computing)的其中一点区别是：前者中的进程有自己的私有内存，通过消息相互通信，后者则使用同一块共享内存。当然，Memcached 不是并行计算。

## 什么是负载均衡？

> 
Load balancing is a computer networking methodology to distribute workload across multiple computers or a computer cluster, network links, central processing units, disk drives, or other resources, to achieve optimal resource utilization, maximize throughput,
 minimize response time, and avoid overload.


从定义可以看得出，负载均衡是指将任务分发到不同地方来优化资源利用，减少反应时间和系统负载压力。

从单个任务来说，负载均衡可以将任务切分给不同节点，然后返回汇总的处理结果。从数量上来说，负载均衡可以将大量任务分流到不同节点，返回此节点的处理结果。

## Memcached 是如何做的？

一台服务器上启动多个 Memcached 服务，他们通过端口来区分：


```bash
/usr/local/bin/memcached -d -p 11213 -u root -m 10 -c 1024 -t 8 -P /tmp/memcached.pid
/usr/local/bin/memcached -d -p 11214 -u root -m 10 -c 1024 -t 8 -P /tmp/memcached.pid
/usr/local/bin/memcached -d -p 11215 -u root -m 10 -c 1024 -t 8 -P /tmp/memcached.pid
```


也就是说，你可以通过 N 个 IP:Port 来提供 N 个 Memcached 节点。但是这些节点之间没有主从关系，相互没有通信。

Memcached 的这种结构可以说勉强基本满足分布式系统的定义。因为分布式侧重于节点的独立性，这里每个 Memcached 缓存池都是独立的。但是 Memcached 并没有对节点的容错性，如果 127.0.0.1:11211 上存放了信息，如果其宕机，存放其中的数据将会丢失，而且不更换缓存池就无法继续使用缓存。

所以说，Memcached 是“分布式”，但是不是一个完整的“分布式系统”。

## PHP + Memcached 如何实现分布式系统？

如果要同时使用多个缓存池，那么就需要给 Memcache 添加多个服务端：


```php
$memcache = new Memcache();
$memcache->addServer('localhost', 11213);
$memcache->addServer('localhost', 11214);
$memcache->addServer('localhost', 11215);
```


如果需要增减服务器，那么因为节点的变化，缓存会产生丢失。不过将 PHP 的 Memcached 扩展中存取时选定服务器的算法修改为 [Consistent Hashing](http://en.wikipedia.org/wiki/Consistent_hashing)，就可以基本避免这个问题。


```
// memcache.c
"memcache.hash_strategy" = standard // before
"memcache.hash_strategy" = consistent // after
```


根据[测试数据](http://www.ccvita.com/395.html)，增删 Memcache 的时候缓存的丢失情况为：
- 已有 10 个节点，增加 1 个节点，默认算法：92% 查询变化
- 已有 10 个节点，增加 1 个节点，Consistent Hash：6% 查询变化
- 已有 10 个节点，删除 1 个节点，默认算法：90% 查询变化
- 已有 10 个节点，删除 1 个节点，Consistent Hash：9% 查询变化

在这样满足基本的容错需求后，是可以称之为一个对应用层透明的分布式系统的。

但是，不管使用什么办法。Memcache 是无法为 PHP 应用提供一个负载均衡系统的。只能通过 PHP 应用层实现：
- 在 PHP 中使用算法将同一个数据的副本保存到多个 Memcache 缓存池中
- 接到查询请求后，轮询或随机方式从其中一个 Memcache 缓存池中取出数据

但是这样一旦数据变化，所有子节点的数据副本都需要更新。但是 Memcached 节点之间并没有通信。只能通过应用层来做一个通信机制。但是这样做又得不偿失，还不如使用专用的负载均衡技术。

## 结论
- Memcached 可以分布式部署，但是本身不是一个完整的分布式系统
- Memcached 在 PHP 中可以实现对应用透明的分布式系统
- Memcached 不是一个负载均衡系统，也无法对 PHP 应用进行透明实现





