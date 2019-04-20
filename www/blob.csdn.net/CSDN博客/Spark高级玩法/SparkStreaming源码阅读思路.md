# SparkStreaming源码阅读思路 - Spark高级玩法 - CSDN博客
2018年06月02日 00:01:11[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：937
SparkStreaming的DirectAPI源码阅读思路
Spark Streaming的流式处理，尤其和kafka的集合，应该是企业应用的关键技术点，作为spark学习和工作者，要熟练的掌握其中原理，精读源码，才能更好的完成任务和相关工调优工作内容。对其原理简介，浪尖不啰嗦，请看前面的文章《[聊聊流式批处理](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484394&idx=1&sn=aa39073decf38582af74048891f8ad83&chksm=9f38e0c2a84f69d4eead745e651b072c4f15429db0b67a97e292b2f6617c0c66a283b8bcc781&scene=21#wechat_redirect)》。在这里浪尖主要介绍，Spark Streaming源码阅读时的注意事项及关注点，只有牢牢把握这几点，才能更好的使用Spark Streaming。
**阅读源码谨记的点**
对于SparkStreaming的相关操作呢，我们只需要谨记一下几点:
1,我们的输入输出流是如何注册到DStreamGraph，并生成job的。
输入流是在构建输入流对象的时候由InputDStream这个抽象类里面的ssc.graph.addInputStream(this)，将其加入到我们DStreamGraph里面。
输出流是在构建的时候调用register方法来将其自身加入到DStreamGraph
2,sparkStreaming是如何封装我们的函数的。
我们自定义的函数最终会被封装到流的compute函数里调用。
3,我们的调用关系链也即pipeline怎么产生的。
4,我们的job生成的周期是由哪些参数决定的。
(time - zeroTime).isMultipleOf(slideDuration)
slideDuration 和 batchDuration 这两个变量有什么关系呢？
5,slideDuration //尤其是窗口函数。
slideDuration 在我们创建输入流的时候被初始化为batchDuration
没有window操作的话，由InputStream衍生出来的流，都会以 slideDuration: Duration = parent.slideDuration这种方式去继承，父流的slideDuration。
这个操作由window操作打乱，在window操作中被赋值为我们为window操作设置的滑动窗口，由此将我们的rdd生成周期改为了我们设定的滑动窗口，以及SparkStreamingjob 的生成周期是我们的滑动窗口指定的时间。
**6，基于Receiver的方式，Receiver是如何被调度执行的？**
重点细节
其实，针对spark Streaming的任务，我们可以简单将其分为两个步骤：
**1， RDD的生成。**就拿与kafka结合的步骤来看，有两种RDD的生成策略。第一种就是基于Receiver的方式，生成blockRDD，这个我们在知识星球里面详细讲了。然后就是direct API的格式是生成KafkaRDD，这个RDD是和kakfa消费的分区数一一对应。Kafka产生的基本思路是，针对获取到分区，然后计算要消费的偏移，然后将这些分装成分区信息，那么在compute函数力计算的时候，会使用simpleConsumer针对分区和偏移信息，去kafka里面获取数据。
**2， Job的生成及执行。**
Job生成是按照批处理时间，但是由于窗口函数的存在，会导致job生成是批处理时间的若干倍。这个在视频里会详细讲的。
最终是在jobScheduler里面，遍历job，然后提交到一个固定线程数的线程池里。这个线程数实际上默认是1，我们可以通过spark.streaming.concurrentJobs设置，结合调度模式，来实现并发job调度。
private val jobExecutor =
ThreadUtils.newDaemonFixedThreadPool(numConcurrentJobs, "streaming-job-executor")
要记住上述1,2两个点，再结合开头的六个问题，就能很清楚的搞明白Spark Streaming运行原理及与kafka结合的两种模式的区别，最终可以完成更精致开发和调优。
**    kafkaRDD生成及获取数据的结构图**
job生成及调度的过程
详细源码视频，请加入星球获取。
**推荐阅读：**
1，[spark源码单步跟踪阅读-从毛片说起](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484895&idx=1&sn=b632547ec06f514ca27ba1e31c040a37&chksm=9f38e6f7a84f6fe1ab6a95bf25931214177988630b9530c74ecc2792d45cb75c09c6f3b027ed&scene=21#wechat_redirect)
2，[从零开始 Spark 性能调优](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484670&idx=1&sn=f1401758c5d177da61bfcddb5fb0e67b&chksm=9f38e7d6a84f6ec0d6cdafe0bcba3353436ac8faef8a397c6cd1cdda2d1bd5e8ce8ec181f12c&scene=21#wechat_redirect)
3，[Spark Streaming 中管理 Kafka Offsets 的几种方式](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484684&idx=1&sn=3c8f42b750574ce296f4870c4f0f72aa&chksm=9f38e624a84f6f329823812eea4d2c419e53a8a73713730228a281252e2274d55416d0f9e726&scene=21#wechat_redirect)
4，[必读：Spark与kafka010整合](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484604&idx=1&sn=c7fb09731502041562894ad9ef06a8ca&chksm=9f38e794a84f6e82dddf1b6c0b081f7f3e0512b85557e7165f87c443ca804f7af6e11846cffa&scene=21#wechat_redirect)
