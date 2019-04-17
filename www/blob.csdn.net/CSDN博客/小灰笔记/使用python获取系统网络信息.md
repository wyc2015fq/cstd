# 使用python获取系统网络信息 - 小灰笔记 - CSDN博客





2017年04月13日 00:27:33[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：2033
个人分类：[Python](https://blog.csdn.net/grey_csdn/article/category/6639459)









psutil模块确实是运维工作中比较有用的模块，除了获取系统的硬件信息之外还能够获取到网络信息。

简单的测试如下：

In [**8**]:
psutil.net_io_counters()

Out[**8**]:
snetio(bytes_sent=40186207L, bytes_recv=620160806L, packets_sent=454876L, packets_recv=469843L, errin=0L, errout=0L, dropin=0L, dropout=0)




In [**9**]:
psutil.net_io_counters(pernic = True

   ...: )

Out[**9**]: 

{'awdl0': snetio(bytes_sent=3121L, bytes_recv=0L, packets_sent=30L, packets_recv=0L, errin=0L, errout=0L, dropin=0L, dropout=0),

 'bridge0': snetio(bytes_sent=342L, bytes_recv=0L, packets_sent=1L, packets_recv=0L, errin=0L, errout=0L, dropin=0L, dropout=0),

 'en0': snetio(bytes_sent=0L, bytes_recv=0L, packets_sent=0L, packets_recv=0L, errin=0L, errout=0L, dropin=0L, dropout=0),

 'en1': snetio(bytes_sent=40969506L, bytes_recv=645656449L, packets_sent=469695L, packets_recv=486500L, errin=0L, errout=0L, dropin=0L, dropout=0),

 'en2': snetio(bytes_sent=0L, bytes_recv=0L, packets_sent=0L, packets_recv=0L, errin=0L, errout=0L, dropin=0L, dropout=0),

 'en3': snetio(bytes_sent=0L, bytes_recv=0L, packets_sent=0L, packets_recv=0L, errin=0L, errout=0L, dropin=0L, dropout=0),

 'gif0': snetio(bytes_sent=0L, bytes_recv=0L, packets_sent=0L, packets_recv=0L, errin=0L, errout=0L, dropin=0L, dropout=0),

 'lo0': snetio(bytes_sent=869906L, bytes_recv=869906L, packets_sent=3527L, packets_recv=3527L, errin=0L, errout=0L, dropin=0L, dropout=0),

 'p2p0': snetio(bytes_sent=0L, bytes_recv=0L, packets_sent=0L, packets_recv=0L, errin=0L, errout=0L, dropin=0L, dropout=0),

 'stf0': snetio(bytes_sent=0L, bytes_recv=0L, packets_sent=0L, packets_recv=0L, errin=0L, errout=0L, dropin=0L, dropout=0),

 'utun0': snetio(bytes_sent=268L, bytes_recv=0L, packets_sent=3L, packets_recv=0L, errin=0L, errout=0L, dropin=0L, dropout=0)}




这是一个很有意思的功能，似乎常见的360安全卫视等流量统计工具能够统计到的信息这里都有了。不过，不同的网络节点的单独信息，

不过，获取的信息应该作什么解析现在还没有进行更为详细的学习。暂且先做一个方向性的接触，后期再来一点深入。



