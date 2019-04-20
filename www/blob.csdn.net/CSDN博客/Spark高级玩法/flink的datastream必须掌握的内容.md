# flink的datastream必须掌握的内容 - Spark高级玩法 - CSDN博客
2019年03月18日 22:15:15[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：15
flink作为目前比较火的流处理引擎，学好还是很有必要的，但是很多人喜欢问浪尖flink会不会超越spark，我觉得短期内还是不会，而且spark 批处理领域还是很高效靠谱的。但是作为大数据开发者，spark和flink并不是必须二选一的，而是都要掌握，所以也不用再问浪尖，关于是否直接跳过spark 直接学习flink这类问题了，没意义。
flink的处理模型可以分为基于事件驱动的处理和基于时间的处理模型，基于时间的处理模型又可以分为基于事件时间和处理时间(注入时间就是一种特殊的事件时间)。下面我们就来概述一下，flink的DataStream我们应该学习哪些内容。
**1.runtime**
必须要先非常了解flink的runtime机制，拓扑结构相关的如并行度设置，task划分原则，task链式原理，slot共享机制等。
关于flink的runtime可以参考浪尖前面的文章
[结合Spark讲一下Flink的runtime](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485964&idx=1&sn=224f6cc89d9e824055673f5ac313d28b&chksm=9f38e924a84f603203311d7c561bfcb3d5fca7241adf3540c4b50e0efca63040add04b2b64a1&scene=21#wechat_redirect)
要理解这个其实可以参考Spark Streaming和Structured Steaming的运行模型和flink的区别。可以参考
- 
[Spark Streaming VS Flink](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486214&idx=1&sn=8bd44797a23c9350c8884213e82c3830&chksm=9f38e82ea84f6138774af6bdbdd6e5332e8c98e97a5b0dea83de295c779026426ab458e05f2b&scene=21#wechat_redirect)
- 
[Structured Streaming VS Flink](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486237&idx=1&sn=427e452f78f725c535b450eb759173ea&chksm=9f38e835a84f6123c697e76bf4a3a2e77ddb09af433e958f1206a2aba897608675eccb973f93&scene=21#wechat_redirect)
这样便于我们了解flink内部运行原理，数据流动方式，shuffle机制，状态管理等，有助于数据倾斜调优，并行度设置，监控告警系统设计。最终才能做好一个比较稳定的应用程序。
**2.事件处理**
事件驱动处理模型，这个是实时处理的典型代表，就这点来说spark的流处理引擎确实比不上，因为其是基于处理时间的微批处理（当然，structured Streaming也支持基于事件时间的处理模型）。
对于flink的事件处理来说，除了前面所说的runtime，还要搞清楚flink datastream的事件时间机制，watermark生成器，并行度原理，shuffle划分，数据流动原理，状态管理及超时key状态删除等这几个非常重要的内容。这样便于理解数据在flink运行时内部流动过程，状态在flink自任务存储的过程，然后数据倾斜与否，状态过期删除及主要是数据倾斜及状态管理吧，这个是flink任务调优的。
当然，flink还是有很多比较骚的操作的，比如下面几篇文章：
- 
[flink的神奇分流器-sideoutput](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486266&idx=1&sn=d9e5b585ea70a1715514f621b4acda11&chksm=9f38e812a84f6104609912a96a41c80259ad2c4dab24303c667e6de85e4133ea7ed218e617e7&scene=21#wechat_redirect) -可以实现数据分流处理。
- 
[Flink迭代操作末文-迭代流](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486333&idx=1&sn=fbcaf4a19752e9fd5ab5ce284058322e&chksm=9f38e855a84f6143af8dfce3715ba6900a567c2c49dad140b4e118bd0f3b76abcf5db4350163&scene=21#wechat_redirect) -迭代计算处理。
- 
[flink一次对整个窗口进行聚合操作-ProcessWindowFunction](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486538&idx=1&sn=1e20f89177432563f23c775e00dd3e27&chksm=9f38ef62a84f6674dc26217849151672364bc274dc976112dcaaca9064873108f2c1935a8b47&scene=21#wechat_redirect) -
也即是process这类底层的api，可以对状态及时间进行更精细的草走，甚至可以自己实现自己的会话窗口。
- 
[Flink异步IO第一讲](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485776&idx=1&sn=bb78ed1f45c7509688526cbd29686f4f&chksm=9f38ea78a84f636e468765b2f870e5f78afe5063f6a94b96cca540c9116fc39d7e4a123b6b6e&scene=21#wechat_redirect) -异步IO 可以实现更加高效的维表join操作。
这些小技巧还是很有必要的掌握的。
**3. 窗口函数**
 主要是这个分为基于事件时间的窗口函数和基于处理时间的窗口函数。窗口函数又分为会话窗口函数，滑动时间窗口，滚动时间窗口。更骚的操做是更底层的窗口处理函数及窗口处理机制也即是ProcessFunction和ProcessWindowFunction，便于我们获取更深层次的状态和时间。
剩下的还有就是窗口的join操作：
- 
滚动窗口join
- 
滑动窗口join
- 
会话窗口join
- 
inerval join
有事件时间必然有事件延迟处理，如何处理基于事件时间的窗口函数延迟事件这个是令人头疼的问题。当然，对于编码来说还是可以很好的对延迟事件进行处理的，比如结合侧输出，watermark，延迟时间等。
**4.边缘生态**
常用的flink边缘生态，数据源是kafka，批处理的话是hdfs上数据，然后sink是hbase，mysql，hbase，mongodb。
**5.实现案例**
下面，浪尖在星球里分享的相关源码，有兴趣的球友可以去参考阅读一下：
- 
org.datastream.KafkaProducer
该类主要是用来生产测试数据用的。
watermark，自定义处理trigger窗口处理机制可以参考下面两个目录里的案例代码：
- 
org.datastream.trigger
- 
org.datastream.watermark
- 
org.datastream.windows
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWewic0sQ1SKiaibS6ojJGiboWB5FEWGIKeBicUqqJN2icsFqBribvmUQeYT19dJxiatqiaGzegKiadlVBgiciaRQ/640?wx_fmt=png)
对于join操作，目前flink不支持datastream与静态数据集的join操作，普通的窗口join可以参考下面源码
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWewic0sQ1SKiaibS6ojJGiboWBoicb7DItmSEJpuROegrMBYjPMrmcjcKdLTobRicxojfX999ZPiaticS5Zg/640?wx_fmt=png)
想要与静态数据集的join，可以实现同步和异步的join操作，浪尖这里实现了同步基于flatmap的，异步的基于异步IO的join操作，基本满足企业开发的需求。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWewic0sQ1SKiaibS6ojJGiboWBl0fayjibEnxIYjhcB7Mv6fupyYFFibfI2NnbbyHFNjFlPy9s68xSbv7Q/640?wx_fmt=png)
Sideoutput侧输出，这个可以实现数据分流的功能，也是非常好用，主要是在处理延迟数据和普通数据分流的时候使用。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWewic0sQ1SKiaibS6ojJGiboWBicmGcvSobHWPaoR34ZWEicTmAzgSFZaEonXjVrib865qun5e1km3X1N6A/640?wx_fmt=png)
迭代输出，主要是分批处理和流处理的迭代输出。代码案例总共三个，实际上机器学习哪些lib包里有更多。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWewic0sQ1SKiaibS6ojJGiboWBXuIqwqcgciajln7ljtaebGibIg1ibsKJJUOsFX4ZKxQU8icuPS7VJIs8cg/640?wx_fmt=png)
Source主要是kafka，sink实现了三种redis，mysql，hbase，这三个比较常用。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWewic0sQ1SKiaibS6ojJGiboWBicL2Rt25PyAALxtfqWqIRmMDAWn37BEdvk4bETh9IMeSpjyWyw0ZeOQ/640?wx_fmt=png)
还有比较重要的配置，比如checkpoint，时间戳分配器，事件时间，处理时间，自动故障恢复等等比较实用的需求。
现在分享了原理及案例，详细的讲解视频正在录制，欢迎大家加入浪尖知识星球
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWD0HKRNyZbjOPNVCKh0d98G5ydFibUfnEFa6LDKoUfXU5IYnFsBG8lhxmTFJtOrruqNl2NIFQAHhw/640?wx_fmt=png)
