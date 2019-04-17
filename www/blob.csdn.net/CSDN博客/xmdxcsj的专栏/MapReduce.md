# MapReduce - xmdxcsj的专栏 - CSDN博客





2015年09月05日 22:46:54[xmucas](https://me.csdn.net/xmdxcsj)阅读数：913








# mapreduce的执行



一、执行流程

![](https://img-blog.csdn.net/20150905224953363?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Map将输入分割为M片，reduce将中间件分为R片（比如使用hash函数），详细步奏如下：

1.      用户程序中的Mapreduce库首先将输入文件分为M片（比如每片文件16到64MB），复制到集群中的worker上。

2.      集群中的master负责给worker分配任务。将M个map任务和R和reduce任务分配给空闲的worker。比如上图中的M为5（对应于3个worker），R为2（对应于2个worker）。

3.      负责map任务的worker读取相应的输入文件片，解析输入数据的key/value数据对，把数据对交给用户定义的map函数。Map函数的处理结果对应的key/value数据对将会缓存在内存中。

4.      缓存的数据对将会周期性写入本地磁盘对应的R区域。本地磁盘的写入位置将会通知master，然后master将该文职告知recude对应的worker机器。

5.      负责ruduce的worker接到master发过来的文件位置信息以后，将会远程读取该位置的文件，并且对intermediate key进行排序（如果数据量太大，将使用外部排序）。

6.      负责Reduce的worker随后会迭代处理读取的中间结果，将key和对应的中间value传入reduce函数，reduce函数的输入将会添加到这个reduce任务对应的最终输出文件。

7.      当所有的map和reduce完成以后，master会返回用户程序。

二、Master的数据结构

对应于每一个map和reduce任务，master都会存储其对应的任务状态（空闲、进行中、完成）和worker机器的标识。

Master在map和reduce之间起到数据通道的作用。Map任务产生的中间结果文件的位置和大小会保存在master，然后经由master发送给reduce所在的worker机器。



三、容错性

1.      Worker failure

Master会周期性地与每台worker保持通信，如果worker挂掉，该worker上对应的任务存在以下几种情况：

l  完成的map任务会标示为idle被其他worker重新执行

l  正在执行中的map和reduce任务也会重新执行

l  完成的reduce不需要重新执行，因为它的输出结果被保存在global file system

l  Map任务在A失败以后转换到B执行，所有的reduce任务会被告知，如果reduce任务没有读取A的数据将会从B读取

2.      master failure

如果master失败，将会终止任务；也可以checkpoint master的数据结构，然后从client重新开始mapreduce工作。

3.      Semantics in the Presence ofFailures

主要针对non-deterministic操作。

四、Locality

数据拷贝尽量本地操作，减少网络io，如果失败就近分配map任务。

五、任务粒度

选取M使得每个文件的大小介于16M和64M之间。

R的大小是worker个数的较小整数倍。

六、Backup

当mapreduce任务快完成的时候，将剩余的未完成的任务进行backup，当backup的任务或者未完成的任务有一个完成的时候，就认为整个mapreduce任务完成。

# Mapreduce的改进

一、Partitionfunction

该函数用于将中间key结果划分为R部分，默认partition函数是.hash(key) mod R，可以根据需求修改。

二、Combinerfunction

该函数用在map之后，对map以后的结果进行部分合并，以减少map和reduce之间的操作。

和reduce函数不同的地方在于reduce函数将处理后的结果存放到最后的输出文件，combiner将处理后的结果存放在中间结果文件（该文件被reduce读取处理）。

三、OrderingGuarantees

中间结果的key/value对会按照key的升序进行处理。

四、输入输出格式

Mapreduce支持三种文件格式：第一种是逐行读入；第二种是key/value读入；第三种是用户自定义reader，可以从文件、数据库或者内存中的数据结构读取。

五、Side-effects

六、Skipping badrecords

有些数据会导致程序崩溃，对于这些数据，master会忽略。

七、LocalExecution

分布式任务执行不容易调试，可以设置mapreduce在本地执行，所有的任务将会在本地执行。

八、StatusInformation

Master通过http server展示任务执行情况。

九、Counters

各类事件的计数器



参考文献：

MapReduce: Simpli_ed Data Processing onLarge Clusters




