# flink和spark Streaming中的Back Pressure - Spark高级玩法 - CSDN博客
2018年07月16日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：426
**Spark Streaming的back pressure**
在讲flink的back pressure之前，我们先讲讲Spark Streaming的back pressure。Spark Streaming的back pressure出现的原因呢，我想大家应该都知道，是为了应对短期数据尖峰。Spark Streaming的back pressure是从spark 1.5以后引入的，在之前呢，只能通过限制最大消费速度（这个要人为压测预估），对于基于Receiver 形式，我们可以通过配置 spark.streaming.receiver.maxRate 参数来限制每个 receiver 每秒最大可以接收的记录的数据；对于 Direct Approach 的数据接收，我们可以通过配置 spark.streaming.kafka.maxRatePerPartition 参数来限制每次作业中每个 Kafka 分区最多读取的记录条数。
这种限速的弊端很明显，比如假如我们后端处理能力超过了这个最大的限制，会导致资源浪费。需要对每个spark Streaming任务进行压测预估。成本比较高。由此，从1.5开始引入了back pressure，这种机制呢实际上是基于自动控制理论的pid这个概念。我们就简单讲一下其中思路：为了实现自动调节数据的传输速率，在原有的架构上新增了一个名为 RateController 的组件，这个组件继承自 StreamingListener，其监听所有作业的 onBatchCompleted 事件，并且基于 processingDelay 、schedulingDelay 、当前 Batch 处理的记录条数以及处理完成事件来估算出一个速率；这个速率主要用于更新流每秒能够处理的最大记录的条数。这样就可以实现处理能力好的话就会有一个较大的最大值，处理能力下降了就会生成一个较小的最大值。来保证Spark Streaming流畅运行。
**pid速率计算源码**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUz9zcmS29xtScmPHoECPGibJXWmjArVxSRhvRf1gdEibJWZ5hkSw0QFFzHUz5VmgKqRVSRHolBrU7A/640?wx_fmt=png)
**配置Spark Streaming的back pressure**
spark.streaming.backpressure.initialRate： 启用反压机制时每个接收器接收第一批数据的初始最大速率。默认值没有设置。
spark.streaming.backpressure.rateEstimator：速率估算器类，默认值为 pid ，目前 Spark 只支持这个，大家可以根据自己的需要实现。
spark.streaming.backpressure.pid.proportional：用于响应错误的权重（最后批次和当前批次之间的更改）。默认值为1，只能设置成非负值。weight for response to "error" (change between last batch and this batch)
spark.streaming.backpressure.pid.integral：错误积累的响应权重，具有抑制作用（有效阻尼）。默认值为 0.2 ，只能设置成非负值。weight for the response to the accumulation of error. This has a dampening effect.
spark.streaming.backpressure.pid.derived：对错误趋势的响应权重。 这可能会引起 batch size 的波动，可以帮助快速增加/减少容量。默认值为0，只能设置成非负值。weight for the response to the trend in error. This can cause arbitrary/noise-induced fluctuations in batch size, but can also help react quickly to increased/reduced capacity.
spark.streaming.backpressure.pid.minRate：可以估算的最低费率是多少。默认值为 100，只能设置成非负值。
更多Spark教程，关注浪尖公众号：Spark学习技巧
**Flink的BackPressure**
如果你看到一个task的back pressure告警（比如，high），这意味着生产数据比下游操作算子消费的速度快。Record的在你工作流的传输方向是向下游，比如从source到sink，而back pressure正好是沿着反方向，往上游传播。
举个简单的例子，一个工作流，只有source到sink两个步骤。假如你看到source端有个告警，这意味着sink消费数据速率慢于生产者的生产数据速率。Sink正在向上游进行back pressure。
**采样线程**
Back Pressure（后面翻译成背压）是通过重复采样正在运行的tasks的tack trace样本数据来监控任务的。JobManager会针对你的job的tasks重复触发调用Thread.getStackTrace()。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUoibFAjiauMU2Vsx6iccNjoxZKTs2ade0J0t3EnBYLQqh6PQSbmwKUA1Ed7QFtrXoZgm0CjJ5Nh9EOA/640?wx_fmt=png)
如果样本数据显示任务线程卡在某个内部方法调用中（从网络堆栈请求缓冲区），则表示该任务存在背压。
默认情况，为了判断是否进行背压，jobmanager会每50ms触发100次stack traces。Web界面中显示的比率，告诉你在这些stack traces中，阻塞在内部方法调用的stack traces占所有的百分比，例如，0.01，代表着100次中有一次阻塞在内部调用。
• OK: 0 <= Ratio <= 0.10
• LOW: 0.10 < Ratio <= 0.5
• HIGH: 0.5 < Ratio <= 1
为例避免stack trace采样导致task managers压力过大，web 界面仅仅在60s刷新一次。
**配置**
可以通过下面的属性进行配置
1， jobmanager.web.backpressure.refresh-interval:在这个时间之后，统计数据将会废弃，需要重新刷新。默认是60000，也即是1min。
2， jobmanager.web.backpressure.num-samples:判断背压需要进行stacktrace采样的个数，默认是100
3， jobmanager.web.backpressure.delay-between-samples:两次stacktrace个采用间隔。
**栗子**
在flink的webui 的job界面中可以看到背压。
正在进行的采样
这意味着JobManager对正在运行的tasks触发stack trace采样。默认配置，这将会花费五秒钟完成。
点击之后，就触发了该操作算子所有task的采样。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUoibFAjiauMU2Vsx6iccNjoxZG2ibMsKbOl58Jia30ibQ79g7hAyHyBT8SibfgPgY7h2Olvr3tEmudq56oQ/640?wx_fmt=png)
**背压状态**
如果您看到任务的状态ok，则表示没有背压指示。另一方面，HIGH意味着任务被加压。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUoibFAjiauMU2Vsx6iccNjoxZn18oj6s8CuSKS64HmSuxBMWdwNOQHibkUkq4o9DicxggeCpf3GdXpZdg/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUoibFAjiauMU2Vsx6iccNjoxZmDCo1yPnBDSe4OaOOGxuoyFtgv1GYcqVox9HeB6ApdXyiaMB8t6iaUtQ/640?wx_fmt=png)
**对比**
Spark Streaming的背压比较简单，主要是根据后端task的执行情况，调度时间等，来使用pid控制器计算一个最大offset，进而来调整Spark Streaming从kafka拉去数据的速度。
Flink的背压就不仅限于从kafka拉去数据这块，而且背压方式不相同，他是通过一定时间内stack traces采样，阻塞的比率来确定背压的。
