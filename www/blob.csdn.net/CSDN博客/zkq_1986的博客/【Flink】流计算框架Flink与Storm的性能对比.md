# 【Flink】流计算框架Flink与Storm的性能对比 - zkq_1986的博客 - CSDN博客





2017年12月07日 20:16:10[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：2367








**1. 背景**


Apache Flink 和 Apache Storm 是当前业界广泛使用的两个分布式实时计算框架。其中 Apache Storm(以下简称“Storm”)在美团点评实时计算业务中已有较为成熟的运用(可参考 Storm 的可靠性保证测试)，有管理平台、常用 API 和相应的文档，大量实时作业基于 Storm 构建。而 Apache Flink(以下简称“Flink”)在近期倍受关注，具有高吞吐、低延迟、高可靠和精确计算等特性，对事件窗口有很好的支持，目前在美团点评实时计算业务中也已有一定应用。

![流计算框架 Flink 与 Storm 的性能对比](http://s2.51cto.com/oss/201711/20/0dace09519d7522e65838d56d68de5ef.jpg-wh_651x-s_3198230229.jpg)


为深入熟悉了解 Flink 框架，验证其稳定性和可靠性，评估其实时处理性能，识别该体系中的缺点，找到其性能瓶颈并进行优化，给用户提供最适合的实时计算引擎，我们以实践经验丰富的 Storm 框架作为对照，进行了一系列实验测试 Flink 框架的性能，计算 Flink 作为确保“至少一次”和“恰好一次”语义的实时计算框架时对资源的消耗，为实时计算平台资源规划、框架选择、性能调优等决策及 Flink 平台的建设提出建议并提供数据支持，为后续的 SLA 建设提供一定参考。

**Flink 与 Storm 两个框架对比：**

![流计算框架 Flink 与 Storm 的性能对比](http://s3.51cto.com/oss/201711/20/abb807ecb3406e0142f33c239b8ace48.png)

**2. 测试目标**


评估不同场景、不同数据压力下 Flink 和 Storm 两个实时计算框架目前的性能表现，获取其详细性能数据并找到处理性能的极限;了解不同配置对 Flink 性能影响的程度，分析各种配置的适用场景，从而得出调优建议。

**2.1 测试场景**

**“输入-输出”简单处理场景**


通过对“输入-输出”这样简单处理逻辑场景的测试，尽可能减少其它因素的干扰，反映两个框架本身的性能。


同时测算框架处理能力的极限，处理更加复杂的逻辑的性能不会比纯粹“输入-输出”更高。

**用户作业耗时较长的场景**


如果用户的处理逻辑较为复杂，或是访问了数据库等外部组件，其执行时间会增大，作业的性能会受到影响。因此，我们测试了用户作业耗时较长的场景下两个框架的调度性能。

**窗口统计场景**


实时计算中常有对时间窗口或计数窗口进行统计的需求，例如一天中每五分钟的访问量，每 100 个订单中有多少个使用了优惠等。Flink 在窗口支持上的功能比 Storm 更加强大，API 更加完善，但是我们同时也想了解在窗口统计这个常用场景下两个框架的性能。

**精确计算场景(即消息投递语义为“恰好一次”)**


Storm 仅能保证“至多一次” (At Most Once) 和“至少一次” (At Least Once) 的消息投递语义，即可能存在重复发送的情况。有很多业务场景对数据的精确性要求较高，希望消息投递不重不漏。Flink 支持“恰好一次” (Exactly Once) 的语义，但是在限定的资源条件下，更加严格的精确度要求可能带来更高的代价，从而影响性能。因此，我们测试了在不同消息投递语义下两个框架的性能，希望为精确计算场景的资源规划提供数据参考。

**2.2 性能指标**

**吞吐量(Throughput)**


单位时间内由计算框架成功地传送数据的数量，本次测试吞吐量的单位为：条/秒。


反映了系统的负载能力，在相应的资源条件下，单位时间内系统能处理多少数据。


吞吐量常用于资源规划，同时也用于协助分析系统性能瓶颈，从而进行相应的资源调整以保证系统能达到用户所要求的处理能力。假设商家每小时能做二十份午餐(吞吐量 20 份/小时)，一个外卖小哥每小时只能送两份(吞吐量 2 份/小时)，这个系统的瓶颈就在小哥配送这个环节，可以给该商家安排十个外卖小哥配送。

**延迟(Latency)**


数据从进入系统到流出系统所用的时间，本次测试延迟的单位为：毫秒。


反映了系统处理的实时性。


金融交易分析等大量实时计算业务对延迟有较高要求，延迟越低，数据实时性越强。


假设商家做一份午餐需要 5 分钟，小哥配送需要 25 分钟，这个流程中用户感受到了 30 分钟的延迟。如果更换配送方案后延迟变成了 60 分钟，等送到了饭菜都凉了，这个新的方案就是无法接受的。

**3. 测试环境**


为 Storm 和 Flink 分别搭建由 1 台主节点和 2 台从节点构成的 Standalone 集群进行本次测试。其中为了观察 Flink 在实际生产环境中的性能，对于部分测内容也进行了 on Yarn 环境的测试。

**3.1 集群参数**

![流计算框架 Flink 与 Storm 的性能对比](http://s2.51cto.com/oss/201711/20/dac234440dc2da6930e25037801cab52.png)

**3.2 框架参数**

![流计算框架 Flink 与 Storm 的性能对比](http://s1.51cto.com/oss/201711/20/eb2b1e5c25aee125236911a2a4f0572f.png)

**4. 测试方法**

**4.1 测试流程**

![流计算框架 Flink 与 Storm 的性能对比](http://s2.51cto.com/oss/201711/20/77c0cebd06e4067ded34f547748dfc7b.png)



**数据生产**


Data Generator 按特定速率生成数据，带上自增的 id 和 eventTime 时间戳写入 Kafka 的一个 Topic(Topic Data)。

**数据处理**


Storm Task 和 Flink Task (每个测试用例不同)从 Kafka Topic Data 相同的 Offset 开始消费，并将结果及相应 inTime、outTime 时间戳分别写入两个 Topic(Topic Storm 和 Topic Flink)中。

**指标统计**


Metrics Collector 按 outTime 的时间窗口从这两个 Topic 中统计测试指标，每五分钟将相应的指标写入 MySQL 表中。


Metrics Collector 按 outTime 取五分钟的滚动时间窗口，计算五分钟的平均吞吐(输出数据的条数)、五分钟内的延迟(outTime – eventTime 或 outTime – inTime)的中位数及 99 线等指标，写入 MySQL 相应的数据表中。最后对 MySQL 表中的吞吐计算均值，延迟中位数及延迟 99 线选取中位数，绘制图像并分析。

**4.2 默认参数**


Storm 和 Flink 默认均为 At Least Once 语义。


Storm 开启 ACK，ACKer 数量为 1。


Flink 的 Checkpoint 时间间隔为 30 秒，默认 StateBackend 为 Memory。


保证 Kafka 不是性能瓶颈，尽可能排除 Kafka 对测试结果的影响。


测试延迟时数据生产速率小于数据处理能力，假设数据被写入 Kafka 后立刻被读取，即 eventTime 等于数据进入系统的时间。


测试吞吐量时从 Kafka Topic 的最旧开始读取，假设该 Topic 中的测试数据量充足。

**4.3 测试用例**

**Identity**


Identity 用例主要模拟“输入-输出”简单处理场景，反映两个框架本身的性能。


输入数据为“msgId, eventTime”，其中 eventTime 视为数据生成时间。单条输入数据约 20 B。


进入作业处理流程时记录 inTime，作业处理完成后(准备输出时)记录 outTime。


作业从 Kafka Topic Data 中读取数据后，在字符串末尾追加时间戳，然后直接输出到 Kafka。


输出数据为“msgId, eventTime, inTime, outTime”。单条输出数据约 50 B。

![流计算框架 Flink 与 Storm 的性能对比](http://s3.51cto.com/oss/201711/20/4a9d5e6d057cfc6191311610502cb227.png)

**Sleep**


Sleep 用例主要模拟用户作业耗时较长的场景，反映复杂用户逻辑对框架差异的削弱，比较两个框架的调度性能。


输入数据和输出数据均与 Identity 相同。


读入数据后，等待一定时长(1 ms)后在字符串末尾追加时间戳后输出

![流计算框架 Flink 与 Storm 的性能对比](http://s5.51cto.com/oss/201711/20/18b4981e5543eff106baeb80c902649c.png)

**Windowed Word Count**


Windowed Word Count 用例主要模拟窗口统计场景，反映两个框架在进行窗口统计时性能的差异。


此外，还用其进行了精确计算场景的测试，反映 Flink 恰好一次投递的性能。


输入为 JSON 格式，包含 msgId、eventTime 和一个由若干单词组成的句子，单词之间由空格分隔。单条输入数据约 150 B。


读入数据后解析 JSON，然后将句子分割为相应单词，带 eventTime 和 inTime 时间戳发给 CountWindow 进行单词计数，同时记录一个窗口中最大最小的 eventTime 和 inTime，最后带 outTime 时间戳输出到 Kafka 相应的 Topic。


Spout/Source 及 OutputBolt/Output/Sink 并发度恒为 1，增大并发度时仅增大 JSONParser、CountWindow 的并发度。


由于 Storm 对 window 的支持较弱，CountWindow 使用一个 HashMap 手动实现，Flink 用了原生的 CountWindow 和相应的 Reduce 函数。

![流计算框架 Flink 与 Storm 的性能对比](http://s3.51cto.com/oss/201711/20/6bcac62d7ee2e7de0a409eb9c20b2b1d.png)

**5. 测试结果**

**5.1 Identity 单线程吞吐量**

![流计算框架 Flink 与 Storm 的性能对比](http://s5.51cto.com/oss/201711/20/8f2f87e14c2935bfe8752249a682976c.png)


上图中蓝色柱形为单线程 Storm 作业的吞吐，橙色柱形为单线程 Flink 作业的吞吐。


Identity 逻辑下，Storm 单线程吞吐为 8.7 万条/秒，Flink 单线程吞吐可达 35 万条/秒。


当 Kafka Data 的 Partition 数为 1 时，Flink 的吞吐约为 Storm 的 3.2 倍;当其 Partition 数为 8 时，Flink 的吞吐约为 Storm 的 4.6 倍。


由此可以看出，Flink 吞吐约为 Storm 的 3-5 倍。

**5.2 Identity 单线程作业延迟**

![流计算框架 Flink 与 Storm 的性能对比](http://s4.51cto.com/oss/201711/20/3369bc4db665efef074dbd539285927e.png)


采用 outTime – eventTime 作为延迟，图中蓝色折线为 Storm，橙色折线为 Flink。虚线为 99 线，实线为中位数。


从图中可以看出随着数据量逐渐增大，Identity 的延迟逐渐增大。其中 99 线的增大速度比中位数快，Storm 的 增大速度比 Flink 快。


其中 QPS 在 80000 以上的测试数据超过了 Storm 单线程的吞吐能力，无法对 Storm 进行测试，只有 Flink 的曲线。


对比折线最右端的数据可以看出，Storm QPS 接近吞吐时延迟中位数约 100 毫秒，99 线约 700 毫秒，Flink 中位数约 50 毫秒，99 线约 300 毫秒。Flink 在满吞吐时的延迟约为 Storm 的一半。

**5.3 Sleep 吞吐量**

![流计算框架 Flink 与 Storm 的性能对比](http://s1.51cto.com/oss/201711/20/1b5832209e1ea2a4487299009ea1d6b7.png)


从图中可以看出，Sleep 1 毫秒时，Storm 和 Flink 单线程的吞吐均在 900 条/秒左右，且随着并发增大基本呈线性增大。


对比蓝色和橙色的柱形可以发现，此时两个框架的吞吐能力基本一致。

**5.4 Sleep 单线程作业延迟(中位数)**

![流计算框架 Flink 与 Storm 的性能对比](http://s1.51cto.com/oss/201711/20/aa6019d8a1e33062c2f5ebcad030b83b.png)


依然采用 outTime – eventTime 作为延迟，从图中可以看出，Sleep 1 毫秒时，Flink 的延迟仍低于 Storm。

**5.5 Windowed Word Count 单线程吞吐量**

![流计算框架 Flink 与 Storm 的性能对比](http://s5.51cto.com/oss/201711/20/b2e08091a600fec0347df735c964454e.png)


单线程执行大小为 10 的计数窗口，吞吐量统计如图。


从图中可以看出，Storm 吞吐约为 1.2 万条/秒，Flink Standalone 约为 4.3 万条/秒。Flink 吞吐依然为 Storm 的 3 倍以上。

**5.6 Windowed Word Count Flink At Least Once 与 Exactly Once 吞吐量对比**

![流计算框架 Flink 与 Storm 的性能对比](http://s3.51cto.com/oss/201711/20/93f49a5e5ad1bfb8cbf9f0e19c3d6474.png)


由于同一算子的多个并行任务处理速度可能不同，在上游算子中不同快照里的内容，经过中间并行算子的处理，到达下游算子时可能被计入同一个快照中。这样一来，这部分数据会被重复处理。因此，Flink 在 Exactly Once 语义下需要进行对齐，即当前最早的快照中所有数据处理完之前，属于下一个快照的数据不进行处理，而是在缓存区等待。当前测试用例中，在 JSON Parser 和 CountWindow、CountWindow 和 Output 之间均需要进行对齐，有一定消耗。为体现出对齐场景，Source/Output/Sink
 并发度的并发度仍为 1，提高了 JSONParser/CountWindow 的并发度。具体流程细节参见前文 Windowed Word Count 流程图。


上图中橙色柱形为 At Least Once 的吞吐量，黄色柱形为 Exactly Once 的吞吐量。对比两者可以看出，在当前并发条件下，Exactly Once 的吞吐较 At Least Once 而言下降了 6.3%

**5.7 Windowed Word Count Storm At Least Once 与 At Most Once 吞吐量对比**

![流计算框架 Flink 与 Storm 的性能对比](http://s3.51cto.com/oss/201711/20/1f4a62076e871d002d54c1d1c4f61856.png)


Storm 将 ACKer 数量设置为零后，每条消息在发送时就自动 ACK，不再等待 Bolt 的 ACK，也不再重发消息，为 At Most Once 语义。


上图中蓝色柱形为 At Least Once 的吞吐量，浅蓝色柱形为 At Most Once 的吞吐量。对比两者可以看出，在当前并发条件下，At Most Once 语义下的吞吐较 At Least Once 而言提高了 16.8%

**5.8 Windowed Word Count 单线程作业延迟**

![流计算框架 Flink 与 Storm 的性能对比](http://s1.51cto.com/oss/201711/20/8ab597b29b35f60974848deed722e108.png)


Identity 和 Sleep 观测的都是 outTime – eventTime，因为作业处理时间较短或 Thread.sleep() 精度不高，outTime – inTime 为零或没有比较意义;Windowed Word Count 中可以有效测得 outTime – inTime 的数值，将其与 outTime – eventTime 画在同一张图上，其中 outTime – eventTime 为虚线，outTime – InTime 为实线。


观察橙色的两条折线可以发现，Flink 用两种方式统计的延迟都维持在较低水平;观察两条蓝色的曲线可以发现，Storm 的 outTime – inTime 较低，outTime – eventTime 一直较高，即 inTime 和 eventTime 之间的差值一直较大，可能与 Storm 和 Flink 的数据读入方式有关。


蓝色折线表明 Storm 的延迟随数据量的增大而增大，而橙色折线表明 Flink 的延迟随着数据量的增大而减小(此处未测至 Flink 吞吐量，接近吞吐时 Flink 延迟依然会上升)。


即使仅关注 outTime – inTime(即图中实线部分)，依然可以发现，当 QPS 逐渐增大的时候，Flink 在延迟上的优势开始体现出来。

**5.9 Windowed Word Count Flink At Least Once 与 Exactly Once 延迟对比**

![流计算框架 Flink 与 Storm 的性能对比](http://s2.51cto.com/oss/201711/20/a6d0a200d00ef3085699fbac176d480e.png)


图中黄色为 99 线，橙色为中位数，虚线为 At Least Once，实线为 Exactly Once。图中相应颜色的虚实曲线都基本重合，可以看出 Flink Exactly Once 的延迟中位数曲线与 At Least Once 基本贴合，在延迟上性能没有太大差异。

**5.10 Windowed Word Count Storm At Least Once 与 At Most Once 延迟对比**

![流计算框架 Flink 与 Storm 的性能对比](http://s4.51cto.com/oss/201711/20/de66da8bdc3cc4fb4e6b8e708e286864.png)
- 
图中蓝色为 99 线，浅蓝色为中位数，虚线为 At Least Once，实线为 At Most Once。QPS 在 4000 及以前的时候，虚线实线基本重合;QPS 在 6000 时两者已有差异，虚线略高;QPS 接近 8000 时，已超过 At Least Once 语义下 Storm 的吞吐，因此只有实线上的点。
- 
可以看出，QPS 较低时 Storm At Most Once 与 At Least Once 的延迟观察不到差异，随着 QPS 增大差异开始增大，At Most Once 的延迟较低。

**5.11 Windowed Word Count Flink 不同 StateBackends 吞吐量对比**

![流计算框架 Flink 与 Storm 的性能对比](http://s4.51cto.com/oss/201711/20/3566714e62b2edc94d0dddc957e829c5.png)
- 
Flink 支持 Standalone 和 on Yarn 的集群部署模式，同时支持 Memory、FileSystem、RocksDB 三种状态存储后端(StateBackends)。由于线上作业需要，测试了这三种 StateBackends 在两种集群部署模式上的性能差异。其中，Standalone 时的存储路径为 JobManager 上的一个文件目录，on Yarn 时存储路径为 HDFS 上一个文件目录。
- 
对比三组柱形可以发现，使用 FileSystem 和 Memory 的吞吐差异不大，使用 RocksDB 的吞吐仅其余两者的十分之一左右。
- 
对比两种颜色可以发现，Standalone 和 on Yarn 的总体差异不大，使用 FileSystem 和 Memory 时 on Yarn 模式下吞吐稍高，使用 RocksDB 时 Standalone 模式下的吞吐稍高。

**5.12 Windowed Word Count Flink 不同 StateBackends 延迟对比**

![流计算框架 Flink 与 Storm 的性能对比](http://s3.51cto.com/oss/201711/20/a20f1b674f00131d43d71d30e8cce657.png)
- 
使用 FileSystem 和 Memory 作为 Backends 时，延迟基本一致且较低。
- 
使用 RocksDB 作为 Backends 时，延迟稍高，且由于吞吐较低，在达到吞吐瓶颈前的延迟陡增。其中 on Yarn 模式下吞吐更低，接近吞吐时的延迟更高。

**6. 结论及建议**

**6.1 框架本身性能**
- 
由 5.1、5.5 的测试结果可以看出，Storm 单线程吞吐约为 8.7 万条/秒，Flink 单线程吞吐可达 35 万条/秒。Flink 吞吐约为 Storm 的 3-5 倍。
- 
由 5.2、5.8 的测试结果可以看出，Storm QPS 接近吞吐时延迟(含 Kafka 读写时间)中位数约 100 毫秒，99 线约 700 毫秒，Flink 中位数约 50 毫秒，99 线约 300 毫秒。Flink 在满吞吐时的延迟约为 Storm 的一半，且随着 QPS 逐渐增大，Flink 在延迟上的优势开始体现出来。


综上可得，Flink 框架本身性能优于 Storm。

**6.2 复杂用户逻辑对框架差异的削弱**
- 
对比 5.1 和 5.3、5.2 和 5.4 的测试结果可以发现，单个 Bolt Sleep 时长达到 1 毫秒时，Flink 的延迟仍低于 Storm，但吞吐优势已基本无法体现。


因此，用户逻辑越复杂，本身耗时越长，针对该逻辑的测试体现出来的框架的差异越小。

**6.3 不同消息投递语义的差异**
- 
由 5.6、5.7、5.9、5.10 的测试结果可以看出，Flink Exactly Once 的吞吐较 At Least Once 而言下降 6.3%，延迟差异不大;Storm At Most Once 语义下的吞吐较 At Least Once 提升 16.8%，延迟稍有下降。
- 
由于 Storm 会对每条消息进行 ACK，Flink 是基于一批消息做的检查点，不同的实现原理导致两者在 At Least Once 语义的花费差异较大，从而影响了性能。而 Flink 实现 Exactly Once 语义仅增加了对齐操作，因此在算子并发量不大、没有出现慢节点的情况下对 Flink 性能的影响不大。Storm At Most Once 语义下的性能仍然低于 Flink。

**6.4 Flink 状态存储后端选择**


Flink 提供了内存、文件系统、RocksDB 三种 StateBackends，结合 5.11、5.12 的测试结果，三者的对比如下：

![流计算框架 Flink 与 Storm 的性能对比](http://s4.51cto.com/oss/201711/20/016160606513b2be9d853becdbabdb90.png)

**6.5 推荐使用 Flink 的场景**


综合上述测试结果，以下实时计算场景建议考虑使用 Flink 框架进行计算：
- 
要求消息投递语义为 Exactly Once 的场景;
- 
数据量较大，要求高吞吐低延迟的场景;
- 
需要进行状态管理或窗口统计的场景。

**7. 展望**


本次测试中尚有一些内容没有进行更加深入的测试，有待后续测试补充。例如：
- 
Exactly Once 在并发量增大的时候是否吞吐会明显下降?
- 
用户耗时到 1ms 时框架的差异已经不再明显(Thread.sleep() 的精度只能到毫秒)，用户耗时在什么范围内 Flink 的优势依然能体现出来?


本次测试仅观察了吞吐量和延迟两项指标，对于系统的可靠性、可扩展性等重要的性能指标没有在统计数据层面进行关注，有待后续补充。
- 
Flink 使用 RocksDBStateBackend 时的吞吐较低，有待进一步探索和优化。
- 
关于 Flink 的更高级 API，如 Table API & SQL 及 CEP 等，需要进一步了解和完善。

转载自：http://bigdata.51cto.com/art/201711/558416.htm



