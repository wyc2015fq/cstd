# blink解决的一个flink分析痛点 - Spark高级玩法 - CSDN博客
2019年02月27日 22:24:46[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：23
blink正在紧锣密鼓的与flink合并，作为flink开发者，大数据开发者，还是有必要测试一下的，经常有粉丝微信群里问浪尖会不会用blink，关于这点，我发过文章了：
[浪尖你怎么看blink开源？](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486385&idx=1&sn=77c70011827bdf89ef9372c4ef54fe7c&chksm=9f38e899a84f618f6cd7da69c347adf65277db6f9c151fbb9ea206d8628b06c03c7c1997e7ae&scene=21#wechat_redirect)
我的建议是等合并之后，测试稳定了再用。
那么今天也主要是测试调研一下blink。前面说了对于大数据框架，**除了要理解底层的原理之外，还要掌握如何分析查看性能瓶颈**，blink除了从底层多处进行优化之外，还优化了flink的 web ui，而且都有了比较大的提升。
关于blink的官方优化点说明，大家可以查看flink 的github blink 分支，里面的readme.md文档。
https://github.com/apache/flink/tree/blink
首先，导入blink入idea
idea直接支持通过git上导入blink，方式是
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUWpDo98mkoIjlIsRkl3VZwTf9KCRSibVmBo31CrDM6EuPjyh1yUibu2vicTfjgXA9Rv2ImibwU84FlPA/640?wx_fmt=png)
跳出下面的窗口，在url栏输入flink的地址
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUWpDo98mkoIjlIsRkl3VZwjH7Y99TIia0xwPaClZ91ozenueMP80fg93uK6jYvhu0EvnZaSRNp55w/640?wx_fmt=png)
然后就是等待了，，，
导入成功之后，在右下角切换分支到blink
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUWpDo98mkoIjlIsRkl3VZwMKhBZLVYZefLK7fKSibMPYsY6fIk0fT7TJdlshhOxMJl8ciaeIMxrsnA/640?wx_fmt=png)
然后就可以编译了，编译之前呢，需要注释掉pom.xml里代码风格检测的内容。然后执行
mvn clean package -DskipTests
编译结束之后，会在build-target目录下生成blink的安装文件，然后你可以复制出来，仅仅想本地模式测试的话，可以直接，执行
bin/start-cluster.sh
启动完成之后，浏览器输入host:8081，即可登陆blink的web ui。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUWpDo98mkoIjlIsRkl3VZw58rslBxfbDOxdpk3ZWBDcZIwRfxEZpaz4lFibibnDnbnkeicAdKyVwOYA/640?wx_fmt=png)
可以看到web ui的内容风格已经完全重构，而且细节非常丰富。
下面是官方给出的web 优化点：
我们在 FlinkRuntimeWeb 的易用性与性能等多个方面进行了改进，从资源使用、作业调优、日志查询等维度新增了大量功能，使得用户可以更方便的对 Flink 作业进行运维。
资源使用：新增 Cluster、TaskManager 与 Job 三个级别的资源信息，资源的申请与使用情况一目了然。
作业调优：作业的拓扑关系及数据流向可以追溯至 Operator 级别，Vertex 增加 InQueue，OutQueue等多项指标，可以方便的追踪数据的反压、过滤及倾斜情况。
日志查询：TaskManager 和 JobManager 的日志功能得到大幅度加强，从Job、Vertex、SubTask 等多个维度都可以关联至对应日志，提供多日志文件访问入口，以及分页展示查询和日志高亮功能。
交互优化：对页面交互逻辑进行了整体优化，绝大部分关联信息在单个页面就可以完成查询和比对工作，减少了大量不必要的跳转。
性能提升：使用 Angular 7.0 进行了整体重构，页面运行性能有了一倍以上的提升。在大数据量情况下也不会发生页面假死或者卡顿情况。
日志管理
在这里浪尖主要想提的就是关于日志相关的监控，大家都知道flink日志只有taskmanager和jobmanager的，要是集群运行多任务的话就会导致日志分析很蛋疼，所以目前on yarn的情况都是单个任务。
blink的日志，给我们细分了文件，方便我们针对应用程序查看
运行两个简单的任务
bin/flink run examples/streaming/
bin/flink run examples/streaming/Kafka010Example.jar --input-topic test  --output-topic boo --bootstrap.servers mt-mdh.local:9093 --groupid test
查看taskmanager的日志，可以看到TaskManager 和 JobManager 的日志功能得到大幅度加强，从Job、Vertex、SubTask 等多个维度都可以关联至对应日志，提供多日志文件访问入口，以及分页展示查询和日志高亮功能。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUWpDo98mkoIjlIsRkl3VZwQ8YeKd4qnPkvQ8SdouTTykETGBcicaSgTSPgPXsroVrS4V26njOX2gg/640?wx_fmt=png)
当然这个日志文件名区分度还不是很高，主要要根据提交任务的主机，然后提交顺序来查找：**日志和程序之间的对应关系**，但是已经大幅度提高了我们分析日志的能力了，因为原有的flink日志比较集中于taskmanager，那么就假如运行时间久，日志文件会越滚越大，运行任务多日志交叉，然后导致问题分析变的难度比较高。
好了，今天分析就到这，后面继续揭开blink 的神秘面纱。
2019年欢迎加入浪尖知识星球，与580位球友一起学习，今天继续flink教程～
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWD0HKRNyZbjOPNVCKh0d98G5ydFibUfnEFa6LDKoUfXU5IYnFsBG8lhxmTFJtOrruqNl2NIFQAHhw/640?wx_fmt=png)
