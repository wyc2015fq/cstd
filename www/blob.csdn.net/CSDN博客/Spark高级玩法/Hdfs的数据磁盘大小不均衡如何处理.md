# Hdfs的数据磁盘大小不均衡如何处理 - Spark高级玩法 - CSDN博客
2018年03月13日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：2058
最近浪尖在纠结一个现在看起来很简单的问题。
**现象描述**
建集群的时候，datanode的节点数据磁盘总共是四块磁盘做矩阵成了一个7.2TB的sdb1(data1)，两块通过矩阵做了一个3.6TB的sdc1(data2)磁盘，运维做的，历史原因。刚开始没有发现，然后集群过了一段时间，随着数据量的增加，发现集群有很多磁盘超过使用率90%告警，浪尖设置磁盘告警阈值是90%，超过阈值就会发短信或者微信告警，提醒我们磁盘将要满了进行预处理，但是通过hadoop的监控指标获取的磁盘利用率维持在55%+，这种情况下不应该发生告警的。磁盘的使用率在hadoop的hdfs的namnode的web ui也可以看到，如下：
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH2LibzS04wNZ8bcrq1EWzibb9JsPlqg3f4SYHCFNXOicXGDUYagkyucdrVQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
这个时候，大家的怀疑会集中于hdfs的某些datanode节点数据存储过于集中，导致某些节点磁盘告警。但是大家都知道，hdfs允许datanode节点接入时datanode之间磁盘异构，数据存储hadoop会自动在datanode之间进行均衡。所以这个怀疑可以排除。
登录告警节点，发现确实data2磁盘使用率超过了90%，但是data1使用率维持在不足50%。
这时候问题就显而易见了，hadoop3.0之前hdfs数据存储只支持在datanode节点之间均衡，而不支持datanode内部磁盘间的数据存储均衡。
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH2VZ4nL4njH2qxpXzNlFlicSXz6OneCqK16U5CdtIGMe96vpq05sVfRdw/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
那么这个时候怎么办呢？
起初
浪尖想的是将data1那个矩阵，拆分成两块由两块磁盘组成的矩阵，然后重新滚动上下线Datanode（数据迁移或者通过副本变动让其进行均衡）。但是，后来很快否定了这种方法，原因是很简单。几百TB的数据，在集群中均衡，即使是滚动重启，那么多机器也要持续好久，然后在数据迁移或者均衡的时候，整个几群的带宽和磁盘都是会增加很大负担，导致集群的可用性降低。
**接着**
通过hadoop官网发现hadoop 3.0不仅支持datanode之间的数据均衡，也支持datanode内部管理的多磁盘的之间的数据均衡。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH2uibu3HI5EbJWztLZlDZMPa69ZL994uBaJbpW14dE8hAsEmkIxIDRibUw/640?wx_fmt=png)
这个时候，可以考虑升级hadoop集群到hadoop3.0，但是思考再三浪尖觉得浪费时间，不划算，最终放弃这种方案。
**最后**
几经思考，终于想出了一个原本就很简单的方案，只需要重启datanode，就可以实现提高大磁盘利用率的方法。首先，要知道的是datanode管理磁盘，是根据我们dfs.data.dir参数指定的目录。那么，我们的思路就很简单了，给data1多个目录，不就可以增加其写入的概率，进而提升磁盘的使用率了么。配置方式如下：
   <property>
        <name>dfs.data.dir</name>
        <value>/data1/dfs/dn,/data1/dfs/dn1,/data2/dfs/dn</value>
</property>
配置结束之后，重启datanode集群，过一定时间查看该目录的大小，然后发现有数据写入。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH2ZGBzmRUcBxiazQaMXjvUa0bCPGXIO4gctuPhZyf1JkyiahKrqyRero9A/640?wx_fmt=png)
由此证明，想法是可行的。
此方法的**缺点**是，原有的数据不会进行均衡，增加目录的方式只是增加了新数据写入大磁盘的概率，但是这样就可以了，等着原有数据自动删除即可。
                                                是不是很简单？
**推荐阅读：**
1，[第3篇：更新异常与规范化设计](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484526&idx=1&sn=ffb4c792fad7bdfe7ffb90407f7b73c7&chksm=9f38e746a84f6e50c8688af5a253893de06b4d58f8eeef575d6f4f9fedc3ad977d04f44a32ac&scene=21#wechat_redirect)
2，[hadoop系列之MR的经典代码案例一](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484001&idx=1&sn=aa2efdb2ac424d8771517d2130ba2f68&chksm=9f38e149a84f685f2a6d86fa7c09b91d10bad4baccb8e14effd15294c78a1240b89de3dcef9a&scene=21#wechat_redirect)
3，[hadoop系列之深入优化](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483973&idx=1&sn=fd4a53a88c1998b2db93e0537e35583f&chksm=9f38e16da84f687bbfde2211ce7c3654c34f9abc730fdde198c9328ef8fc355dd302a35e525f&scene=21#wechat_redirect)
4，[hive的分区和分桶](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484404&idx=1&sn=88c0418a866572c1dd85ced93d1168f0&chksm=9f38e0dca84f69ca45cd77722839004db02019297c8608835114e382d1b85c6bb6c1fe920934&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
