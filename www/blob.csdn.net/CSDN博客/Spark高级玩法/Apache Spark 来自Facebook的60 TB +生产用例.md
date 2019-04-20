# Apache Spark:来自Facebook的60 TB +生产用例 - Spark高级玩法 - CSDN博客
2018年08月26日 11:39:42[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：97
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDd5tXcheeGtYr8aRQpSZTS7fncO0z5lyibEZI7v8YjVHkTXa79OYCZdqQ/640?wx_fmt=png)
本文主要讲Facebook扩展spark替换hive的过程中积累的经验和教训。
浪尖整理翻译https://databricks.com/blog/2016/08/31/apache-spark-scale-a-60-tb-production-use-case.html。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDdxOq06gyribibH9twuLMzRTeJWxqepiawF1VTZcLYub1wnWQic3lrqRrSiaw/640?wx_fmt=png)
## **使用案例：实体排名的特征准备**
实时实体排名在Facebook上以各种方式使用。对于这些在线服务平台中的一些原始特征值是通过Hive离线生成的，并且数据被加载到实时查询系统中。多年前构建的旧的基于Hive的基础架构是资源密集型的计算架构，并且难以维护，因为管道被分成数百个较小的Hive作业。为了实现更新的特征数据并提高可管理性，选取了一个现有的管道并尝试将其迁移到Spark。
## **原先的Hive实现**
基于Hive的管道由三个逻辑阶段组成，其中每个阶段对应于共用entity_id的数百个较小的Hive作业，因为为每个阶段运行大型Hive作业不太可靠并且受到每个作业的最大任务数量的限制。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXicjBsu1YySxcyK9FMNYzqYzu57abU3vWvv16ODlyfAjeeFLb1iayPkUHiaGrW0iajctZGame1NZxZgQ/640?wx_fmt=png)
 这三个逻辑步骤可归纳如下：
- 
过滤掉非生产特征和噪音。
- 
聚合每组（entity_id，target_id）。
- 
将表分成N个分片并通过自定义二进制使每个分片以管道形式运行，以便生成用于在线查询的自定义索引文件。
基于Hive的管道构建索引大约需要三天时间才能完成。管理也是具有挑战性，因为管道包含数百个分片job，使监控变得困难。没有简单的方法来衡量管道的整体进度或计算ETA。在考虑现有Hive管道的上述限制时，决定尝试使用Spark构建更快，更易管理的管道。
## **Spark实现**
全流成 调试可能是具有挑战性和资源密集型的。我们首先转换基于Hive的管道中资源最密集的部分：第二阶段。我们从50 GB压缩输入的样本开始，然后逐渐扩展到300 GB，1 TB，然后20 TB。在每个尺寸增量，我们解决了性能和稳定性问题，但测试20 TB使我们找到最大的改进机会。
在运行20 TB输入时，我们发现由于任务量很大，我们生成了太多输出文件（每个大小约为100 MB）。10个小时的作业运行时间中有3个小时用于将文件从staging director移动到HDFS中的最终目录。最初，我们考虑了两个选项：改进HDFS中的批量重命名来支持这个案例，或者配置Spark以生成更少的输出文件（由于大量任务（70,000）在此阶段很难）。我们退出了问题并考虑了第三种选择。由于我们在管道的第二步中生成的tmp_table2表是临时的并且仅用于存储管道的中间输出，因此我们基本上压缩，序列化和复制三个副本以用于具有数TB数据的单个读取工作负载。我们更进一步：删除两个临时表并将所有三个Hive stage合并为一个Spark作业，该作业读取60 TB的压缩数据并执行90 TB的随机和排序。最终的Spark工作如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXicjBsu1YySxcyK9FMNYzqYvayPVrroNZcOSTA6RXsuv3bTau7RnPE76bdsEJzwOWib9EGjvxibkjpw/640?wx_fmt=png)
## **我们是如何为该job扩展Spark的？**
当然，为这么大的管道运行单个Spark job在第一次尝试时甚至在第10次尝试时都没正常运行。据我们所知，这是在shuffle数据大小方面尝试的最大的Spark job（Databricks的Petabyte排序  是在合成数据上）。它对核心Spark基础架构和我们的应用程序进行了大量改进和优化，以使这项job得以运行。这项job的好处是，其中许多改进适用于Spark的其他大型工作负载，我们能够将所有工作贡献回开源Apache Spark项目 - 有关其他详细信息，请参阅JIRA。下面，我们重点介绍可以将其中一个实体排名管道部署到生产环境中的主要改进。
## **可靠性修复**
### 处理频繁的节点重新启动
为了可靠地执行长时间运行的作业，我们希望系统具有容错能力并从故障中恢复（主要是由于正常维护或软件错误导致的机器重启）。虽然Spark旨在容忍机器重新启动，要解决的各种错误/问题才能让他足够去处理公共错误。
**使 PipedRDD对fetch失败更有鲁棒性（SPARK-13793）**：PipedRDD 以前的实现不够强大，无法处理由于节点重启而导致的获取失败，并且只要出现获取失败，该作业就会失败。我们在 PipedRDD 中进行了更改，优雅的处理获取失败，使该作业可以从这种类型的获取失败中恢复。
**可配置的最大获取失败次数（SPARK-13369）**：对于这种长时间运行的作业，由于机器重启而引起的获取失败概率显着增加。在 Spark 中每个阶段的最大允许的获取失败次数是硬编码的，因此，当达到最大数量时该作业将失败。我们做了一个改变，使它是可配置的，并且在这个用例中将其从 4 增长到 20，从而使作业更稳健。
**较少破坏性的集群重启：**长期运行的作业应该能够在集群重启后继续存在。 Spark的可重启shuffle服务功能允许我们在节点重启后保留shuffle文件。 最重要的是，我们在Spark driver中实现了一项功能，以便能够暂停任务的调度，以便由于群集重新启动导致过多的任务失败不会导致job失败。
**其他可靠性修复**
**无响应的driver  （****SPARK-13279****）**：在添加任务时，由于O（N ^ 2）操作，Spark driver卡住了，导致作业最终被卡住并终止。我们通过删除不必要的O（N ^ 2）操作来解决问题。
**过多的driver推测：**我们发现Spark驱动程序在管理大量任务时花费了大量时间进行推测。在短期内，禁止对该job进行推测执行。目前正在努力改变Spark驱动程序，以减少推测时间。
**由于大缓冲区的整数溢出导致的TimSort问题  （****SPARK-13850****）：**测试发现Spark的unsafe内存操作有一个导致TimSort内存损坏的错误。感谢Databricks人员解决了这个问题，这使能够在大型内存缓冲区上运行。
**调优shuffle服务以处理大量连接：**在shuffle阶段，我们看到许多executor在尝试连接到shuffle服务时超时。增加Netty服务器线程（spark.shuffle.io.serverThreads）和backlog（spark.shuffle.io.backLog）的数量解决了这个问题。
**修复Spark执行器OOM (****SPARK-13958****）：**首先为每个主机打包四个以上的reducer任务是一项挑战。Spark executor内存不足，因为sorter中存在导致指针数组无限增长的错误。我们通过在没有更多可用于指针数组增长的内存时强制将数据溢出到磁盘来解决该问题。因此，现在我们可以运行24个任务/主机而不会耗尽内存。
## **性能改进**
在实现上述可靠性改进之后，我们能够可靠地运行Spark作业。在这一点上，我们将努力转向与性能相关的项目，以充分利用Spark。我们使用Spark的指标和几个分析器来查找一些性能瓶颈。
### **我们用来寻找性能瓶颈的工具**
Spark UI度量指标：Spark UI可以深入了解特定阶段的时间花费。每个任务的执行时间分为子阶段，以便更容易找到job中的瓶颈。
**Jstack：**Spark UI还在执行程序进程上提供按需jstack函数，可用于查找代码中的热点。
**Spark Linux Perf / Flame Graph支持：**虽然上面的两个工具非常方便，但它们不能同时为数百台计算机上运行的作业提供CPU概要分析的聚合视图。在每个作业的基础上，我们添加了对启用Perf概要分析的支持（通过libperfagent for Java符号），并可以自定义采样的持续时间/频率。使用我们的内部度量标准收集框架，将分析样本聚合并在执行程序中显示为Flame Graph。
### **性能优化**
修复分sorter中的内存泄漏  （SPARK-14363）  （加速30％）：当任务释放所有内存页但指针阵列未被释放时，我们发现了一个问题。结果，大块内存未被使用并导致频繁的溢出和executor OOM。我们的修复现在可以正确释放内存并使大型排序有效运行。我们注意到此次修复后CPU的性能提高了30％。
**Snappy优化  （****SPARK-14277****）  **（加速10％）：正在为每行读/写调用JNI方法 - （Snappy.ArrayCopy）。我们提出了这个问题，并将Snappy行为更改为使用非基于JNI的  System.ArrayCopy  。仅这一变化就提供了大约10％的CPU改进。
**减少随机写入延迟 （****SPARK-5581****）  **（最多加速50％）：在map侧，当将随机数据写入磁盘时，map任务是为每个分区打开和关闭相同的文件。我们做了一个修复，以避免不必要的打开/关闭，并观察到写入大量shuffle分区的作业的CPU改进高达50％。
**修复由于fetch失败导致的重复任务运行问题 **（SPARK-14649）：Spark driver在发生fetch失败时重新提交已在运行的任务，从而导致性能不佳。我们通过避免重新运行正在运行的任务来修复该问题，并且我们看到在发生获取失败时作业更稳定。
**PipedRDD的可配置缓冲区大小  **（SPARK-14542）  （加速10％）：使用PipedRDD时，我们发现将数据从分sorter传输到管道进程的默认缓冲区大小太小而且我们的工作是花费超过10％的时间复制数据。我们使缓冲区大小可配置以避免这个瓶颈。
**为shuffle fetch加速而缓存索引文件 ** （SPARK-15074）：我们观察到shuffle服务经常成为瓶颈，并且reducer花费10％到15％的时间等待获取map数据。深入研究这个问题，我们发现shuffle服务正在为每次shuffle fetch打开/关闭shuffle索引文件。我们进行了更改以缓存索引信息，以便我们可以避免文件打开/关闭，并重用索引信息以用于后续提取。此更改将总的shuffle时间减少了50％。
**减少shuffle字节写入指标的更新频率**  （SPARK-15569）  （加速高达20％）：使用Spark Linux Perf集成，我们发现大约20％的CPU时间用于探测和更新写入的shuffle字节指标。
**可配置的sorter初始缓冲区大小**  （SPARK-15958）  （加速率最高可达5％）：sorter的默认初始缓冲区大小太小（4 KB），我们发现它对于大型工作负载来说非常小 - 而且结果，我们浪费了大量时间来扩展缓冲区并复制内容。我们做了一个更改，使缓冲区大小可配置，并且大缓冲区大小为64 MB，我们可以避免大量的数据复制，使工作速度提高约5％。
**配置任务数量：**由于我们的输入大小为60 T，每个HDFS块大小为256 M，因此我们为该作业生成了超过250,000个任务。虽然我们能够以如此多的任务运行Spark作业，但我们发现当任务数量太多时，性能会显着下降。我们引入了一个配置参数来使map输入大小可配置，因此我们可以通过将输入分割大小设置为2 GB来将该数量减少8倍。
在完成所有这些可靠性和性能改进之后，我们很高兴地报告我们为我们的一个实体排名系统构建和部署了更快，更易管理的管道，并且我们提供了在Spark中运行其他类似作业的能力。
## **Spark管道与Hive管道性能比较**
我们使用以下性能指标来比较Spark管道与Hive管道。请注意，这些数字不是查询或作业级别的Spark与Hive的直接比较，而是构建优化管道与灵活计算引擎（例如Spark）的比较，而不是仅在以下操作的计算引擎查询/作业级别（例如，Hive）。
**CPU时间：**从操作系统的角度来看，这是CPU使用率。例如，如果您的作业在32核计算机上仅使用50％的CPU运行10秒，那么您的CPU时间将为32 * 0.5 * 10 = 160 CPU秒。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXicjBsu1YySxcyK9FMNYzqYpQQ7FcbvtXGibfsehtmV6ibojfjfSgVz22nwFH1zey0xNIl3kY8vUEEg/640?wx_fmt=png)
**CPU保留时间：** 这是从资源管理框架的角度来看的CPU预留。例如，如果我们预留32核机器10秒钟来运行作业，则CPU预留时间为32 * 10 = 320 CPU秒。CPU时间与CPU预留时间的比率反映了我们如何利用群集上的预留CPU资源。准确无误时，与CPU时间相比，运行相同工作负载时，预留时间可以更好地比较执行引擎。例如，如果一个进程需要1个CPU秒运行但必须保留100个CPU秒，则该指标的效率低于需要10个CPU秒但仅保留10个CPU秒来执行相同工作量的进程。我们还计算内存预留时间，但不包括在内，由于在同一硬件上运行实验，数字类似于CPU预留时间，而在Spark和Hive情况下，我们不会将数据缓存在内存中。Spark能够在内存中缓存数据，但由于我们的集群内存限制，我们决定使用类似于Hive的核外工作。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXicjBsu1YySxcyK9FMNYzqYHMkufzS1xMAU4fHdTyX4ibHcaUGlNvaKzcmx6n9lqdpYR8V5N0fCRBQ/640?wx_fmt=png)
延迟：作业的端到端经过时间。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXicjBsu1YySxcyK9FMNYzqYZPZhvshOHnvSpqtCicibib06wPRfaAsdjmHkw7K5j2dBrTEx5QibI4ptew/640?wx_fmt=png)
## **结论和未来的工作**
Facebook使用高性能和可扩展的分析来协助产品开发。Apache Spark提供了将各种分析用例统一到单个API和高效计算引擎中的独特功能。我们将分解为数百个Hive作业的管道替换为单个Spark作业。通过一系列性能和可靠性改进，我们能够扩展Spark以处理生产中的实体排名数据处理用例之一。在这个特定的用例中，我们展示了Spark可以可靠地shuffle和排序90 TB +中间数据，并在一个作业中运行250,000个任务。与旧的基于Hive的管道相比，基于Spark的管道产生了显着的性能改进（4.5-6x CPU，3-4x资源预留和~5x延迟），并且已经在生产中运行了几个月。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
