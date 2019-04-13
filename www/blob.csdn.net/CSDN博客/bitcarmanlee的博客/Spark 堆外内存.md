
# Spark 堆外内存 - bitcarmanlee的博客 - CSDN博客


2017年12月13日 16:55:44[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：3432



## 1.堆外内存有哪些
前面提到spark中的堆内存溢出，除了堆内存，还有堆外内存。该部分内存主要用于程序的共享库、Perm Space、 线程Stack和一些Memory mapping等, 或者类C方式allocate object.
堆外内存在Spark中可以从逻辑上分成两种: 一种是DirectMemory, 一种是JVM Overhead(下面统称为off heap), 这两者在Spark中的大小分别由两个参数设置.
Spark中有哪些地方会消耗堆外内存, 会在后面详细讲述.
如果堆外内存溢出，Spark UI表现如下：
![这里写图片描述](https://img-blog.csdn.net/20171213164807044?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Executor Off Heap超出被杀:
![这里写图片描述](https://img-blog.csdn.net/20171213164840588?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 2.堆外内存使用超出预期的涉及点
用户代码
off heap: 资源释放不当, 例如加载文件资源次数过多, 且不正常关闭, 例如多次调用ClassLoader().getResourceAsStream
Driver端
DirectMemory: 拉取Executor端Task Result数据回Driver节点时, 此处消耗的DirectMemory内存 = conf.getInt(“spark.resultGetter.threads”, 4) * TaskResultSize
Executor端
Executor可能消耗的情况如下:
(1)Direct Memory: RDD.cache()/RDD.persist()操作,
因为会涉及到拉取remote RDD Block时出现Direct OOM, 此时消耗的Direct Memory = 拉取的RDDBlockSize.
Tips: 查看RDD Block Size步骤: SparkUI->Storage Tabs -> 看众多RDD中Memory或者Disk中的totalSize/cached Partitions中最大的RDD, 点进去看详情页, 然后对RDD的大小 按照Memory或者Disk排序, 找到最大的RDD Block….
(2)Direct Memory, 拉取Shuffle数据时出现Direct OOM,
此时消耗的Direct Memory 通常= max(某个Shuffle Block的size, 50MB)
Tips: 可以在抛出该Direct OOM的Executor节点上检查是否有如下日志: Spark会在如果单个shuffleBlock的大小>1MB时输出该语句.
![这里写图片描述](https://img-blog.csdn.net/20171213165016762?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
还有一种预估的方式, 前一阶段Stage 对其内的每个Task的Shuffle Write排序, 找到最大的Shuffle Write / 下一stage的task个数, 即为一个预估的shuffle Block大小.
(3)Direct Memory, RDD.persist(StorageLevel.DISK_ONLY)/RDD.persit(StorageLevel.MEMORY_AND_DISK)/RDD.persist(StorageLevel.MEMORY_AND_DISK_SER)等含有disk level的cache rdd操作.会带来额外Direct Memory消耗, 最多64MB * 3
(4)Off-Heap, RDD.persist(StorageLevel.DISK_ONLY)/RDD.persit(StorageLevel.MEMORY_AND_DISK)/RDD.persist(StorageLevel.MEMORY_AND_DISK_SER)等含有disk level的cache RDD操作.
一般情况最大的值是整个作业中最大的disk level的RDD Block的size. 但除了些许特殊操作: zip类操作(包括相同partitoner的RDD做union操作, 因为会被Spark后台优化成zip操作) 使用的size等于该操作zip的rdd中涉及的所有disk level rdd block size之和.
此部分Size如第(1)点所示, 找到Disk中最大的RDD Block即可.
其他框架:
Off-Heap 涉及到读Hbase时会消耗比较多的off-heap内存, 但这部分已经通过参数（spark.hadoop.hbase.ipc.client.connection.maxidletime）控制使用上限制在256MB.
3.Spark堆外内存控制参数:
堆外内存的使用总量 = jvmOverhead(off heap) + directMemoryOverhead(direct memory) + otherMemoryOverhead
![这里写图片描述](https://img-blog.csdn.net/20171213165329701?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
4.解决思路
合理的参数推荐:
一般推荐总值:
spark.yarn.executor.directMemoryOverhead = 
{if存在memory levelordisk level 的 blockthen第1点的Sizeelse0} +
{ifShuffle阶段抛出Direct OOMthen第2点的Sizeelse0} +
｛if存在Disk level的Blockthen第3点的192MBelse0｝ +
{if存在其他框架的then其他框架的sizeelse0} +256MBspark.yarn.executor.jvmOverhead = 
{if存在disk level的Blockthen第4点的Sizeelse0} + 
{if存在其他框架的then其他框架的sizeelse0} +
max(executor-memory *0.1,384)// 如果没有Executor表现为堆外内存使用超出, 则不需要手动调整.

