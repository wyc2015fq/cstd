# Hadoop性能调优概要说明 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年07月07日 11:54:42[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1026
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









Hadoop容易遇到的问题有：Namenode/jobtracker单点故障、HDFS小文件问题、数据处理性能等。为此 “Hadoop Performance Optimization”(HPO)是必要的。本文试着从性能调优的总体原则入手来了解概要，实际生产中遇到的问题也会在这个框架下处理。

Hadoop运行环境：

![](https://img-blog.csdn.net/20170707115340671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




下面大致给出这四个层次的调优原则。

## 1、硬件选型原则
![](https://img-blog.csdn.net/20170707115355435?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





## 2、操作系统调优

1）避免使用swap分区 将hadoop守护进程的数据交换到硬盘的行为可能会导致操作超时。

在Linux系统当中，如果一个进程的内存不足，其内存中的部分数据会暂时写到磁盘上，在需要的时候，会再将磁盘中的数据动态的置换到内存当中，这样一来，一些不必要的流程就会显现出来。通常，这会导致进程的执行效率降低。再分布式环境当中，使用MapReduce这样的计算模型时，可以通过控制每个Job的处理数量和每个Task运行过程使用的缓冲区的大小，避免我们去使用Swap分区。通过/etc/sysctl.conf文件中的vm.swappiness参数来达到目的。

2）调整内存分配策略：操纵系统内核根据vm.oversommit_memory 的值来决定分配策略，并且通过vm.overcommit_ratio的值来设定超过物理内存的比例。

3）修改net.core.somaxconn参数：

该参数表示socker监听backlog的上限，默认为128,socker的服务器会一次性处理backlog中的所有请求，hadoop的ipc.server.listen.queue.size参数和linux的net.core.somaxconn

参数控制了监听队列的长度，需要调大。

4）增大同时打开文件描述符的上限

对内核来说，所有打开的文件都通过文件描述符引用，文件描述符是一个非负整数，hadoop的作业经常会读写大量文件，需要增大同时打开文件描述符的上限。

5）选择合适的文件系统，并禁用文件的访问时间

ext4 xfs ,文件访问时间可以让用户知道那些文件近期被查看或修改，但对hdfs来说， 获取某个文件的某个块 被修改过，没有意义，可以禁用。

6）关闭THP(transparent Huge Pages)

THP 是一个使管理Huge Pages自动化的抽象层， 它会引起cpu占用率增大， 需要关闭。

echo never> /sys/kernel/mm/redhat_transparent_hugepage/defrag

echo never> /sys/kernel/mm/redhat_transparent_hugepage/enabled 

echo never> /sys/kernel/mm/transparent_hugepage/enabled 

echo never> /sys/kernel/mm/transparent_hugepage/defrag

7）增大网络连接上限

在Hadoop集群当中，其内部的一些通信依赖网络，需调整Linux参数net.core.somaxconn，让其处于一个足够大的状态。

8）预读取

磁盘IO性能没有CPU和内存这样发展迅猛，因而它成为OS当中一个主要的性能瓶颈。改进磁盘IO性能也是重要的优化手段之一。可以使用Linux的blockdev命令来设置预读取的缓冲区大小，以便提高Hadoop的文件读取性能。

## 3、JVM调优

JVM本身的调优，实际还是和OS以及Hadoop结合，如mapreduce作业的堆内存执行设置。

在YARN里面，可以启用JVM的重用机制来得到性能的提升。启用该功能能够让一些Task的执行效率提高2～3倍。

YARN的默认配置会禁用该组件，即不允许重用JVM。首先，我们需要明白YARN是如何执行一个MapReduce的Job。其步骤如下所示：

1）RM（Resource Manager）里面的AM（Application Manager）会为每个Application（一个MR的Job）在NM（NodeManager）里面申请一个Container

2）在申请到的Container里面启动一个Application Master，Container在YARN中分配资源的容器（内存、CPU、磁盘空间等），它启动便会对应的启动一个JVM

3）ApplicationMaster会持续为Application包含的每一个Task（一个Map Task或者Reduce Task）向RM申请一个Container

4）每得到一个Container，该Container所属的NM将此Container启动

5）该Container执行对应的Task

6）对应的Task执行完毕，该Container被NM回收，而Container所拥有的JVM相应的推出

通过上述的流程可以看出，这种情况下，每一个JVM仅只执行了一个Task，JVM并未被重用。

因而，用户可以通过启用ubertask属性来重用JVM，在同一个Container里面一次执行多个Task，可以在mapred-site.xml中配置对应的参数即可，内容如下所示：

<property>

<name>mapreduce.job.ubertask.enable</name>

<value>true</value>

</property>

如果启用该功能，会将一个Application中的所有子Task在同一个JVM里面执行，到达JVM重用的目的。该JVM负责Application中的Application Master中所用的JVM，即运行在Container当中。

最后，当ubertask功能被启用的时候，YARN是如何执行一个application的。首先，RM里的AM会为每一个Application在NM里面申请一个Container，然后在该container里面启动一个Application Master。Containe启动时便会相应启动一个JVM。此时，如果ubertask功能被启用，Application Master会在JVM中按照顺序依次在Container中执行每一个Task，这样Application Master便不用再为每一个Task向RM去申请一个单独的Container，从而达到了重用JVM（资源重用）的目的。

## 4、Hadoop调优

4.1 HDFS调优

1）设置合理的块大小(dfs.block.size)

2）将中间结果目录设置为分布在多个硬盘以提升写入速度(mapred.local.dir)

3）设置datanode处理RPC的线程数，大集群可以适当加大（dfs.datanode.handler.count）,默认为3，可以适当加大为10

4）设置namenode能同时处理的请求数，(dfs.namenode.handler.count),为集群模式的自然对数(lnN)的20倍。



4.2 YARN调优

yarn的资源表示模型为ceontainer（容器）,container 将资源抽象为两个维度，内存和虚拟cpu(vcore)

1）兼容各种计算框架

2）动态分配资源，减少资源浪费

容器内存yarn.nodemanager.resource.memory-mb

最小容器内存yarn.scheduler.minimum-allocation-mb

容器内存增量yarn.scheduler.increment-allocation-mb

最大容器内存yarn.scheduler.maximum-allocation-mb

容器虚拟cpu内核yarn.nodemanager.resource.cpu-vcores

最小容器虚拟cpu内核数量yarn.scheduler.minimum-allocation-vcores

容器虚拟cpu内核增量yarn.scheduler.increment-allocation-vcores

最大容器虚拟cpu内核数量yarn.scheduler.maximum-allocation-vcores



4.3 MapReduce调优，调优三大原则

1）增大作业并行程度

2）给每个任务足够的资源

3）在满足前2个条件下，尽可能的给shuffle预留资源。

参考hadoop官网对参数配置的说明，结合实际问题做调优。在对Hadoop调优时，这是一个庞大的任务，这里进行分解来看，按Hadoop的组成模块来分，比如HDFS、MapReduce、YARN等模块去优化对应的模块。若是在细分，我们可以优化其各个组件的相关配置文件，其每个模块都有对应的XML文件，在系统启动时，会通过Configure加载到系统当中，而对应的XML文件当中，配置的参数和属性比较多，有些参数是根据业务本身去优化，如：心跳间隔、缓冲区大小、JVM子进程最大内存、小文件的合并数、归并map输出数据占比等等。

　　另外，在处理一些IO密集的应用，会在执行MapReduce时产生大量的中间输出数据（Map Task执行阶段），而产生的这些数据对于使用者来说是并不关心的（透明化）。这里，可以思考一下，有木有一种办法能够集中处理这些输出数据。答案是肯定的，在MapReduce中支持压缩算法，我们可以在执行这部分流程时，将中间输出数据压缩存储，这样在IO性能方面有会有明显的提升。然而，万物皆有因果，在选择压缩算法时，需考虑压缩比和压缩效率，在一些压缩算法当中，有的压缩比非常可观，然而其压缩效率却非常低下；反之，有的压缩比较差，然其压缩效率非常理想。因为，我们需要在压缩比和压缩效率之间做一个平衡，选择合适的算法，去平衡二者的关系。

　　目前，存在许多的压缩格式，如：GZIP，ZIP，LZO，Snappy等等，测试表明其中LZO和Snappy较为可观（具体量化指标图不方便给出）。当然，这个也不是绝对的，是当下业务去测试，然后选择合适的压缩格式。

　　上面提点过预读取机制，可以通过预读取机制来有效的提升磁盘IO的读性能。通过改机制提高HDFS的读性能以及MapReduce作业的执行效率。

　　当然，从应用程序也是有优化的空间的，处理应用程序当中配置必要的作业参数之外，其本身的编写方式对性能也是有影响的。在执行一大批MapReduce作业时，若是设置一个Combiner，对于提供作业的性能大有裨益。在了解MapReduce（其分两部分，其一为计算模型，其二为运行环境，尽管Hadoop版本升级到2.x，然其计算模型不变，变得只是其运行环境。其运行环境是基于YARN的资源管理）的计算模型时，在弄明白Combiner阶段的好处后，会发现，我们在编写相关作业时，添加Combiner可减少Map Task的中间输出结果，从而减少各个Reduce
 Task的远程Copy数据量，最终带来的益处是缩短了Map和Reduce两者的执行时间。

　　同样，我们在选择Hadoop的相关类型时，如Writeable。在MapReduce中，Map Task和Reduce Task的输入和输出的数据类型均为Writable的衍生类型，其包含IntWritable、LongWriteable、FloatWritable等。在编写相关代码时，选择合适的类型可以大大提升其性能。例如在处理整型数据之时，直接采用IntWritable比先以Text类型读取在通过对应的方法转化为整型来的高效。




