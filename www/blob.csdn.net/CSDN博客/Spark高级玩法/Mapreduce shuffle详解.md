# Mapreduce shuffle详解 - Spark高级玩法 - CSDN博客
2018年02月06日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：442
**Mapreduce shuffle详解**
Mapreduce确保每个reducer的的输入都是按键排序的。系统执行排序的过程(即将map输出作为输入 传给reducer)成为shuffle。从多个方面来看shuffle是mapreduce的心脏，是奇迹发生的地方。
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU4fc3VCdeoKiacwvmiadjOEEA0peojYy3B5NIBwf7ZD7hibChY9DGw9KB4d21Vu2PoWBaFoRJykqfibw/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
上图展示了，mapreduce的详细过程。
**1  输入分片**
对于数据的输入分片，要根据不同的存储格式有不同的介绍。对于，hdfs存储的文件，数据的分片就可分为两种，文件可切分(不压缩或者压缩格式bzip2)的按照一定大小进行分片有既定算法，默认是block的大小，具体算法不在这里细讲，前面hive调优的文章又说到，而且浪尖也会在后续的文章提到这个内容；
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU4fc3VCdeoKiacwvmiadjOEEialhzNlCWnQVYCS2hs1dmOAu68edpVx50FmMqyweC6B4o39en1Z9icbQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
分片的时候计算公式计算过程举例
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU4fc3VCdeoKiacwvmiadjOEEM3sATHbFM88CsbXfyBMwa5fW4lf0j8aicKDCuH11XbBeIhicWicwiay7mw/640?wx_fmt=png)
文件不可切分则一个文件一个分片。
**2  Map端**
从上图我们可以看到map端的处理过程。Map会读取输入分片数据。但是map函数开始产生输出时并不是简单的将数据写入磁盘。这个过程很复杂，他利用了缓冲的方式写到内存并出于效率的考虑进行排序。
每个map任务都是有一个环形缓冲区的用于存储任务的输出。在默认情况下，缓冲区的大小为100MB，辞职可以通过改变io.sort.mb来调整。一旦缓冲内容达到阈值(io.sort,spill,percent，默认是0.8)，一个后台线程会将内容spill到磁盘。在spill到磁盘的过程中，map输出并不会停止往缓冲区写入数据，但如果在此期间缓冲区被写满，map会被阻塞知道写磁盘过程完成。
溢出写过程安装轮询方式将缓冲区的内容写到mapred.local.dir指定的作业特定子目录中的目录中。
写磁盘之前，线程首先根据数据最终要传的reducer把数据划分成相应的分区。在每个分区中后台线程按键进行内排序，如果有一个combiner，它就在排序后的输出上运行。运行combinner使得map输出结果更紧凑，因此可以减少写到磁盘的数据和传递给reducer的数据。
每次内存缓冲区达到溢出阈值，就会新建一个溢出文件(spill file)，因此在map任务写完其最后一个输出记录之后，会有几个溢写文件。在任务完成之前，溢出文件被合并成一个已分区且已排序的输出文件。配置属性io.sort.factor控制着一次最多能合并多少流，默认是10。
如果至少存在3个溢出文件(通过min.num.spills.for.combine属性设置)时，则combiner就会在输出文件写到磁盘之前再次运行。前面曾说过，combiner可以在输入上反复运行，单不影响最终的结果。如果只有一两个溢出文件，那么对map输出的减少不值得调用combiner，就不会为map输出再次运行combiner。
在将压缩map输出写到磁盘的过程中对它进行压缩往往是个好主意，因为这样就会写磁盘的速度更快，更加节约时间，并且减少传给reducer的数据量。在默认情况下，输出是不压缩的，但是只要将mapred.compress.map.output设置为true，就可以启用这个功能。使用的压缩库由mapred.map.output.compression.codec指定。
Reducer是通过HTTP的方式得到输出文件的分区。在MRV2中使用netty进行数据传输，默认情况下netty的工作线程数是处理器数的2倍。MRV1中，默认值是40，由tracker.http.threads来在tasktracker端设定。
**3   Reducer端**
集群中往往一个mr任务会有若干map任务和reduce任务，map任务运行有快有慢，reduce不可能等到所有的map任务都运行结束再启动，因此只要有一个任务完成，reduce任务就开始复制器输出。复制线程的数量由mapred.reduce.parallel.copies属性来改变，默认是 5。
Reducer如何知道map输出的呢？对于MRv2 map运行结束之后直接就通知了appmaster，对于给定的job appmaster是知道map的输出和host之间的关系。在reduce端获取所有的map输出之前，Reduce端的线程会周期性的询问master 关于map的输出。Reduce并不会在获取到map输出之后就立即删除hosts，因为reduce有肯能运行失败。相反，是等待appmaster的删除消息来决定删除host。
Reduce对map输出的不同大小也有相应的调优处理。如果map输出相当小，会被复制到reduce任务JVM的内存(缓冲区大小由mapred.job.shuffle.input.buffer.percent属性控制，指定用于此用途的堆空间的百分比)，否则，map输出会被复制到磁盘。一旦内存缓冲区达到阈值(由mapred.job.shuffle.merge.percent决定)或达到map的输出阈值(mapred.inmem.merge,threshold控制)，则合并后溢出写到磁盘中。如果指定combiner，则在合并期间运行它已降低写入磁盘的数据量。
随着磁盘上副本的增多，后台线程会将它们合并为更大的，排序好的文件。这会为后面的合并节省一些时间。注意，为了合并，压缩的map输出(通过map任务)都必须在内存中解压缩。
复制完所有的map输出后，reduce任务进入排序阶段(更加恰当的说法是合并阶段，因为排序是在map端进行的)，这个阶段将合并map的输出，维持其顺序排序。这是循环进行的。比如，有50个map输出，而合并因子是10(默认值是10，由io.sort.factor属性设置，与map的合并类似)，合并将进行5趟。每趟将10个文件合并成一个文件，因此最后有5个中间文件。
在最后阶段，即reduce阶段，直接把数据输入reduce函数，从而省略了一次磁盘往返行程，并没有将这5个文件合并成一个已排序的文件最为最后一趟。最后的合并可以来自内存和磁盘片段。
在reduce阶段，对已排序输出中的每个键调用reduce函数。此阶段的输出直接写到输出文件系统，一般为hdfs。
**注意：**
每趟合并的文件数实际上比上面例子中展示的有所不同的。目标是合并最小数据量的文件以便满足最后一趟的合并系数。因此，如果有40个文件，我们不会再四趟中每趟合并10个文件而得到4个文件。相反，第一趟只合并4个文件，随后的三塘合并10个文件。最后一趟中，4个已经合并的文件和剩余的6个文件合计是个文件进行合并。如下图所述：
注意这并没有改变合并的次数，它只是一个优化措施，目的是尽量减少写到磁盘的数据量，因为最后一趟总是直接合并到reduce。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU4fc3VCdeoKiacwvmiadjOEEia8afZEUjjYq4icc4FQFaZVM3rLaXZribP6AIKlyoLiaiasGvmkfLMgqlGQ/640?wx_fmt=png)
本文摘自hadoop权威指南。
**推荐阅读：**
1，[重磅：关于hive的join使用必须了解的事情](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484426&idx=1&sn=50613f6f4f6fda4a139b01d2f04656d6&chksm=9f38e722a84f6e34c4c8df1d9849fda384a0e7659ac383b378ea9948c522b98c8a5462250a60&scene=21#wechat_redirect)
2，[Hive高级优化](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484413&idx=1&sn=1573cc39a44d65407223f08a6315c1e4&chksm=9f38e0d5a84f69c373b971283b905f291ebcdf28cd3d686fbf25316d2ac4de6d5c356f270a96&scene=21#wechat_redirect)
3，[sparksql调优之第一弹](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484416&idx=1&sn=ebd245c13da6d06841ffe6f4f7fd3dcc&chksm=9f38e728a84f6e3e15915503c0c0affa3fd89a5b25e5015e21102cf2c74b55e1476d60e3ddd3&scene=21#wechat_redirect)
![640.png?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png?)
**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg?)
**更多文章，敬请期待**

