# 详解MapReduce工作流程 - Orisun - 博客园







# [详解MapReduce工作流程](https://www.cnblogs.com/zhangchaoyang/articles/2648815.html)





###  一、客户端向JobTracker提交作业

这个阶段要完成以下工作：
- 向JobTracker申请 一下新的JobID
- 检查是否指定了output dir，并且确认output dir不存在
- 根据InputPath计算input split。这里的input split并不是MapReduce输入数据的完整拷贝，只是记录了每个split在什么地方存放着。split和block一样都是逻辑概念，一个split可能跨越不同的磁盘。
- 把运行作业所需的resource复制到jobtracker的文件系统上去，保存在一个包含jobID的目录下。这些resource包括：jar文件、配置文件、计算好的input split等。
- 告诉JobTracker已经ready for execution。

```
public interface InputSplit extends Writable {
        long getLength() throws IOException;
        String[] getLocations() throws IOException;
}
```

我们看到InputSplit中记录了原始数据的长度length，而location则有多个（是一个数组）。location只记录了主机名，它用于在指派map task的时候，让map task离它处理的split近一些。而记录length的作用是让最大的split先被处理，这是hadoop为了最小化作业运行时间而采取的贪心策略。

### 二、作业初始化

jobtracker根据input split从HDFS中获取每个split代表的真实数据，对于每个split都要生成一个map task来处理它。至于生成多少reduce task则由属性mapred.reduce.tasks来决定。每个task都获得一个ID。一个map task和它要处理的split在集群中尽可能地邻近，最好是在同一个TaskTracker上。

### 三、任务分配

只要task tracker一启动它就周期性地向job tracker发送心跳，表示自己还活着，心跳中还包含其他信息，告诉job tracer自己的忙闲状态，如果是闲，那job tracker就可以给它分配新任务了。

每个TaskTracker上都有固定数目（这取决于该节点拥有的CPU和内存）的slots来安放map task和reduce task。比如有的TaskTracker上可以同时运行2个map task和2个reduce task。

### 四、执行任务

首先每个task tracker要从共享的filesystem中拷贝两样东西到本地：包含程序代码的jar文件、DistributedCacheFile。然后为本次task创建一个工作目录。最后创建一个JVM运行作业。也就是说在task tracker上可以同时运行多个JVM，每个JVM运行一个task（map tack或reduce task） ，所以在map或reduce中发生的bug只会影响本JVM（使之中止或崩溃），但不会影响到task tracker。JVM在不同的task之间是可以重用的，但是一个JVM在同一时刻只能运行一个task。

### 五、更新进度和状态

每个task都要及时地向它所在的TaskTracker汇报进度和状态，TaskTracker及时地向JobTracker汇报执行进度和状态（比如执行了多少个task，哪个失效了等等），这些信息最终由JobTracker反馈给用户。

解释一下“进度”，对于map task来说，就是已处理的数据和input split总长度的比值；但是对于reduce task来说就有些特别，事实上reduce task包括3个阶段：copy、sort和reduce，每完成一个阶段进度就前进了33%。

### 六、完成任务

JobTracker清理一些状态数据和临时的输出文件，并通知TaskTacker做同样的清理工作。

### 失效

对于悬挂的task，如果task tracker在一段时间（默认是10min，可以通过mapred.task.timeout属性值来设置，单位是毫秒）内一直没有收到它的进度报告，则把它标记为失效。TaskTracker通过心跳包告知JobTracker某个task attempt失败了，则JobTracker把该task尽量分配给另外一个task tracker来执行。如果同一个task连续4次（该值可以通过mapred.map.max.attempts和mapred.reduce.max.attempts属性值来设置）都执行失败，那JobTracker就不会再做更多的尝试了，本次job也就宣告失败了。

对于某些应用可能不想因为个别task的失败而导致整个job的失败，你可以设置失效的task小于一定比例时job仍然是成功的，通过这两个属性：mapred.max.map.failures.percent和mapred.max.reduce.failures.percent。

上面说了task失效，还有一种情况是TaskTracker失效。如果TaskTracker运行很慢或直接crash了，则它停止向jobtracker发送心跳。若JobTracker在10min（这个值可以通过mapred.task.tracker.expiry.interval属性进行设置）没有收到TaskTracker的心跳就把它从调度池中移除。由于整个TaskTracker节点都失效，它上面的已经执行完毕的map task的输出也不能再被reduce task获取，所以之前分配给该TaskTracker的所有map task(不管是执行完毕还是没有完毕)都要放到另一个节点上重新执行，已经执行完毕的reduce task由于结果已经输出到了最终文件里面，就不需要重新执行了。即使TaskTracker没有失效，当它上面失效的task太多时同样会被列入黑名单。

如果失效的是JobTracker那就无药可救了，因为JobTracker只有一个，没有谁可以替代它。

### 作业调度

默认情况下采用FIFO原则，先提交的作业先被调度。其实job也可以设置优先级，通过mapred.job.priority属性或调用setJobPriority()方法。

“公平调度器”让每个job获得相同的资源，所以小作业会比大作业完成得早。

### 洗牌和排序

**在Map侧**

每个map task都有一个循环利用的缓冲区（默认大小是100M，通过io.sort.mb设置），它有输出先放到缓冲区里。当缓冲区满80%（该值可以通过io.sort.spill.percent设置）后，后台线程会把它spill到磁盘。当缓冲区填满100%时，map的输出没地方做，map task将被阻塞。

在被写入磁盘之前，数据首先会被分区（根据hadoop的Partitioner），在每个分区中数据会被放到内存中进行排序。MapTask结束的时候所有的分区会被合并成一个排序好的文件。

**在Reduce侧**

map的输出就在本节点上，而reduce的则不是。上文已提到过，reduce task包括3个阶段：copy、sort和reduce。Reducer有数个并行的copy线程，每当有map task结束时，它们就把map的输出拷贝过来。那reducer怎么会知道有mapper结束呢？因为mapper的执行进度jobtracker都会知道，reducer中有个线程专门负责向jobtracker询问哪个mapper结束了。mapper的输出是在整个job结束后才被删除的，mapper的输出被reducer拷贝之后并不会立即被删除，因为reducer可能会失效。

不需要等到所有的mapper都结束，当reducer已经拷贝了几个mapper的输出到本地后，它就开始把它们合并成一个更大的文件。当所有的mapper都结束时，reducer执行最后一次merge sort。这就是sort阶段（实际上sort在map侧都已经执行了，在reduce侧执行的是归并排序）。

至于reduce阶段自然就是执行reduce()函数喽.

由于shuffle要占用大量内存，所以我们设计的map和reduce要尽量地小，占用少量的内存为好。

### Speculative Task

一个task执行得慢可能出于多种原因，包括硬件老化。hadoop的策略是当发现task执行得慢时就在一个新的JVM甚至是新的节点上启动它的一个“备份”。

当所有的task都已经启动并执行了一段时间后，如果发现某些task比其他的运行的都慢，这时就会启动一个speculatie task跟原task执行相同的任务--只要其中有一个先完成，那另一个就可以kill掉了。

当然采用speculatie task就会产生双份的输出，要处理好这个问题。

### 跳过坏记录

对于海量的数据文件，存在坏记录（格式不对、数据缺失等）是很正常的，在程序中必须妥善处理，否则可能会因为极个别的“坏记录”中断了整个Job。另外你也可以选择skip bad records。Skipping模式默认情况下是关闭的，你可以使用SkipBadRecord类分别为map和reduce打开Skipping模式。但是Skipping模式默认情况下对于每个task只能跳过1坏记录而不妨碍task的成功结束，你可以通过设置mapred.map.max.attempts和mapred.reduce.max.attempts来把容忍值调得大一些。



Hadoop可以为task提供运行时的环境信息：

mapred.job.id　　　　JobID

mapred.tip.id　　　　TaskID

mapred.task.id　　　　task attempt ID,注意的task ID区别

mapred.task.partition　　　　task ID在Job中的序号

mapred.task.is.map　　　　判断task是否为map task

由于task可能会失效，这样一个task就会对应多个task attempt，就会有多次输出。这了解决这个问题，每次task attempt的输出目录为${mapred.output.dir}/_temporary/${mapred.task.id}，当任务成功结束后再把它复制到${mapred.output.dir}中。那一个task如何知道自己的工作目录呢？可以从配置文件中检索mapred.word.output.dir的属性值，也可调用FileOutputFormat的静态方法getWorkOutputPath()。












