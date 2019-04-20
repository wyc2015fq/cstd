# Java 程序员必须了解的 7 个性能指标 - Spark高级玩法 - CSDN博客
2018年12月02日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：17182
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/TNUwKhV0JpQg0W94ic6iaI9vbAyTwwKfQYPZHyOohtbvAicG5XofWQW0foU90SbFmV1M4NiaFGp38SK8ibMMnczicr0w/640)
> 
*来源：王韵菲 *
*www.evget.com/article/2015/11/16/22990.html*
本文中，小编搜集了7个最有影响的衡量标注，让你可以不依赖日志文件来了解应用程序。现在，让我们看看这些性能指标，并了解如何查看并收集它们：
[**1.响应时间和吞吐量**](http://mp.weixin.qq.com/s?__biz=MzI3ODcxMzQzMw==&mid=2247484873&idx=1&sn=29b53be1cf632d78b52a0cd78532e3a1&chksm=eb5380ffdc2409e95183b57d2a931f44eec754a9c6b545d177e2f0590e0b023d230611a07faa&scene=21#wechat_redirect)
根据应用程序的响应时间可以知道程序完成传输数据所用的时间。也可以从HTTP请求级别，或者成为数据库级别来看。对那些缓慢的查询你需要做一些优化来缩短时间。吞吐量是另一个角度衡量传输数据的指标，是指单位时间内系统处理的客户请求的数量。
我们可以使用APMs（例如New Relic或AppDynamics）来衡量这些指标。使用这些工具，你可以在主报告仪表板中将平均响应时间与昨天的甚至上周的直接进行对比。这有助于我们观察新的部署是否会影响到我们的应用程序。你可以看到网络传输的百分比，测量HTTP完成请求需要多长时间。你也可以看看这篇：[网站性能测试指标（QPS，TPS，吞吐量，响应时间）详解](http://mp.weixin.qq.com/s?__biz=MzI3ODcxMzQzMw==&mid=2247484873&idx=1&sn=29b53be1cf632d78b52a0cd78532e3a1&chksm=eb5380ffdc2409e95183b57d2a931f44eec754a9c6b545d177e2f0590e0b023d230611a07faa&scene=21#wechat_redirect)。
推荐工具：
- 
AppDynamics
- 
New Relic
- 
Ruxit
New Relic报告：Web传输百分比和吞吐量
**2.平均负载**
第二个应用广泛的指标是平均负载。我们习惯上会把平均负载分为这三步测量，分别是第5分钟、第15分钟和最后1分钟。要保证数量低于机器的内核数。一旦超过内核数，机器就会运行在压力状态下。
除了简单测量CPU使用率，还需要关注每个内核的队列中有多少进程。在内核使用率都是100%的情况下，队列中只有1个任务和有6个任务有很大不同。因此，平均负载不能只考虑CPU使用率。
推荐工具：
- 
htop
**3.错误率**
大多数开发人员判断错误率是根据HTTP传输总失败百分比。但是他们忽略了一个更深层的东西：特定传输的错误率。这直接影响到您应用程序的运行状况。这可以显示出代码方法的错误以及错误或异常出现的次数。
但单纯的错误率数据对我们没有多大帮助。最重要的是我们要找到它们的根源并解决问题。随着Takipi的运行，我们要在日志文件中需找线索。你可以找到所有关于服务器状态的信息，包括堆栈跟踪、源代码和变量值。
推荐工具：
- 
Takipi
**4.GC率和暂停时间**
异常行为垃圾收集器应用程序的吞吐量和响应时间采取深潜的主要原因之一。了解GC暂停频率和持续时间的关键是分析GC日志文件。要分析它们，你需要收集GC日志和JVM参数。你要注意观察不同指标之间的数据是如何相互影响的。
推荐工具：
- 
jClarity Censum
- 
GCViewer
**5.业务指标**
应用程序的性能不完全取决于响应时间和错误率。业务指标也是一方面，例如收益、用户数。
推荐工具：
- 
Grafana
- 
The ELK stack
- 
Datadog
- 
Librato
**6.正常运行时间和服务运行状态**
这一指标奠定了整个应用程序性能的基础。不仅可以当做一个提醒指标，也可以让你定义一段时间内的SKA。我们可以使用Pingdom的servlet功能进行运行状态检查。我们可以查到应用程序的所有传输，包括数据库和S3。你也可以看看这篇：[SLA服务可用性4个9是什么意思？怎么达到？](http://mp.weixin.qq.com/s?__biz=MzI3ODcxMzQzMw==&mid=2247485120&idx=1&sn=9b41d713eda47ca6e0f545b7d8753498&chksm=eb5383f6dc240ae0fc4ad87ff9fca94ab71b22ca5ca1cefaafecd7cb9b85b32975ddbe0c1e48&scene=21#wechat_redirect)
推荐工具：
- 
Pingdom
**7.日志大小**
日志有一个缺点，它是一直在增加的。当您的服务器启动塞满了垃圾，一切都慢下来。因此，我们需要密切的关注日志大小。
目前通常的解决办法是使用logstash划分使用日志，并将它们发送并存储在Splunk、ELK或其他的日志管理工具中。
推荐工具：
- 
Splunk
- 
Sumo Logic
- 
Loggly
**推荐阅读**
[高级面试:JAVA内存模型](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485737&idx=1&sn=14212fb6edcdb88446dea6737b864196&chksm=9f38ea01a84f63176f61e11ea99650d26233bc776b5f1f5ff6f7b70ddd763291b6b7d5af9d17&scene=21#wechat_redirect)
[Java 程序员必备的高效 Intellij IDEA 插件](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485867&idx=1&sn=f3d408f6615faeb3a97d4ab6dac3f5e1&chksm=9f38ea83a84f63956a700a0fd922e61e31f7b09fff727434a17203c3b218ac93aca15ddb8f94&scene=21#wechat_redirect)
[Spark SQL用UDF实现按列特征重分区](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485842&idx=1&sn=cf7331c3a082a141366e5771640c5e52&chksm=9f38eabaa84f63acec983e9e15691b01239349f568bd8cb9725fbc42a01aa721e97145f8622c&scene=21#wechat_redirect)
