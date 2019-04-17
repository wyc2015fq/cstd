# (转载)浅析Hadoop文件格式 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年04月25日 09:37:24[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2019








对于Hadoop学习要持续，这里转载下关于hadoop文件格式，后续实践中在加以理解。

转载地址：http://www.infoq.com/cn/articles/hadoop-file-format








# Hadoop 作为MR 的开源实现，一直以动态运行解析文件格式并获得比MPP数据库快上几倍的装载速度为优势。不过，MPP数据库社区也一直批评Hadoop由于文件格式并非为特定目的而建，因此序列化和反序列化的成本过高[7]。本文介绍Hadoop目前已有的几种文件格式，分析其特点、开销及使用场景。希望加深读者对Hadoop文件格式及其影响性能的因素的理解。



## Hadoop 中的文件格式

**1 SequenceFile **

SequenceFile是Hadoop API 提供的一种二进制文件，它将数据以<key,value>的形式序列化到文件中。这种二进制文件内部使用Hadoop 的标准的Writable 接口实现序列化和反序列化。它与Hadoop API中的MapFile 是互相兼容的。Hive 中的SequenceFile 继承自Hadoop API 的SequenceFile，不过它的key为空，使用value 存放实际的值， 这样是为了避免MR 在运行map 阶段的排序过程。如果你用Java API 编写SequenceFile，并让Hive
 读取的话，请确保使用value字段存放数据，否则你需要自定义读取这种SequenceFile 的InputFormat class 和OutputFormat class。

![](http://cdn1.infoqstatic.com/statics_s1_20160414-0116/resource/articles/hadoop-file-format/zh/resources/image1.png)

**图1：Sequencefile 文件结构**

**2 RCFile**





RCFile是Hive推出的一种专门面向列的数据格式。 它遵循“先按列划分，再垂直划分”的设计理念。当查询过程中，针对它并不关心的列时，它会在IO上跳过这些列。需要说明的是，RCFile在map阶段从远端拷贝仍然是拷贝整个数据块，并且拷贝到本地目录后RCFile并不是真正直接跳过不需要的列，并跳到需要读取的列， 而是通过扫描每一个row group的头部定义来实现的，但是在整个HDFS Block 级别的头部并没有定义每个列从哪个row group起始到哪个row group结束。所以在读取所有列的情况下，RCFile的性能反而没有SequenceFile高。

![](http://cdn1.infoqstatic.com/statics_s1_20160414-0116/resource/articles/hadoop-file-format/zh/resources/image2.png)

**图2：RCFile 文件结构**

3 Avro

Avro是一种用于支持数据密集型的二进制文件格式。它的文件格式更为紧凑，若要读取大量数据时，Avro能够提供更好的序列化和反序列化性能。并且Avro数据文件天生是带Schema定义的，所以它不需要开发者在API 级别实现自己的Writable对象。最近多个Hadoop 子项目都支持Avro 数据格式，如Pig 、Hive、Flume、Sqoop和Hcatalog。

![](http://cdn1.infoqstatic.com/statics_s1_20160414-0116/resource/articles/hadoop-file-format/zh/resources/image3.png)

**图3：Avro MR 文件格式**

**4. 文本格式**

除上面提到的3种二进制格式之外，文本格式的数据也是Hadoop中经常碰到的。如TextFile 、XML和JSON。 文本格式除了会占用更多磁盘资源外，对它的解析开销一般会比二进制格式高几十倍以上，尤其是XML 和JSON，它们的解析开销比Textfile 还要大，因此强烈不建议在生产系统中使用这些格式进行储存。 如果需要输出这些格式，请在客户端做相应的转换操作。 文本格式经常会用于日志收集，数据库导入，Hive默认配置也是使用文本格式，而且常常容易忘了压缩，所以请确保使用了正确的格式。另外文本格式的一个缺点是它不具备类型和模式，比如销售金额、利润这类数值数据或者日期时间类型的数据，如果使用文本格式保存，由于它们本身的字符串类型的长短不一，或者含有负数，导致MR没有办法排序，所以往往需要将它们预处理成含有模式的二进制格式，这又导致了不必要的预处理步骤的开销和储存资源的浪费。

**5. 外部格式**

Hadoop实际上支持任意文件格式，只要能够实现对应的RecordWriter和RecordReader即可。其中数据库格式也是会经常储存在Hadoop中，比如Hbase，Mysql，Cassandra，MongoDB。 这些格式一般是为了避免大量的数据移动和快速装载的需求而用的。他们的序列化和反序列化都是由这些数据库格式的客户端完成，并且文件的储存位置和数据布局(Data Layout)不由Hadoop控制，他们的文件切分也不是按HDFS的块大小（blocksize）进行切割。

## 文件存储大小比较与分析

我们选取一个TPC-H标准测试来说明不同的文件格式在存储上的开销。因为此数据是公开的，所以读者如果对此结果感兴趣，也可以对照后面的实验自行做一遍。Orders 表文本格式的原始大小为1.62G。 我们将其装载进Hadoop 并使用Hive 将其转化成以上几种格式，在同一种LZO 压缩模式下测试形成的文件的大小。
|Orders_text1|1732690045|1.61G|非压缩|TextFile|
|----|----|----|----|----|
|Orders_tex2|772681211|736M|LZO压缩|TextFile|
|Orders_seq1|1935513587|1.80G|非压缩|SequenceFile|
|Orders_seq2|822048201|783M|LZO压缩|SequenceFile|
|Orders_rcfile1|1648746355|1.53G|非压缩|RCFile|
|Orders_rcfile2|686927221|655M|LZO压缩|RCFile|
|Orders_avro_table1|1568359334|1.46G|非压缩|Avro|
|Orders_avro_table2|652962989|622M|LZO压缩|Avro|

**表1：不同格式文件大小对比**

从上述实验结果可以看到，SequenceFile无论在压缩和非压缩的情况下都比原始纯文本TextFile大，其中非压缩模式下大11%， 压缩模式下大6.4%。这跟SequenceFile的文件格式的定义有关： SequenceFile在文件头中定义了其元数据，元数据的大小会根据压缩模式的不同略有不同。一般情况下，压缩都是选取block 级别进行的，每一个block都包含key的长度和value的长度，另外每4K字节会有一个sync-marker的标记。对于TextFile文件格式来说不同列之间只需要用一个行间隔符来切分，所以TextFile文件格式比SequenceFile文件格式要小。但是TextFile
 文件格式不定义列的长度，所以它必须逐个字符判断每个字符是不是分隔符和行结束符。因此TextFile 的反序列化开销会比其他二进制的文件格式高几十倍以上。

RCFile文件格式同样也会保存每个列的每个字段的长度。但是它是连续储存在头部元数据块中，它储存实际数据值也是连续的。另外RCFile 会每隔一定块大小重写一次头部的元数据块（称为row group，由hive.io.rcfile.record.buffer.size控制，其默认大小为4M），这种做法对于新出现的列是必须的，但是如果是重复的列则不需要。RCFile 本来应该会比SequenceFile 文件大，但是RCFile 在定义头部时对于字段长度使用了Run Length Encoding进行压缩，所以RCFile
 比SequenceFile又小一些。Run length Encoding针对固定长度的数据格式有非常高的压缩效率，比如Integer、Double和Long等占固定长度的数据类型。在此提一个特例——Hive 0.8引入的TimeStamp 时间类型，如果其格式不包括毫秒，可表示为”YYYY-MM-DD HH:MM:SS”，那么就是固定长度占8个字节。如果带毫秒，则表示为”YYYY-MM-DD HH:MM:SS.fffffffff”，后面毫秒的部分则是可变的。

Avro文件格式也按group进行划分。但是它会在头部定义整个数据的模式（Schema）， 而不像RCFile那样每隔一个row group就定义列的类型，并且重复多次。另外，Avro在使用部分类型的时候会使用更小的数据类型，比如Short或者Byte类型，所以Avro的数据块比RCFile 的文件格式块更小。

## 序列化与反序列化开销分析

我们可以使用Java的profile工具来查看Hadoop 运行时任务的CPU和内存开销。以下是在Hive 命令行中的设置：
hive>set mapred.task.profile=true;hive>set mapred.task.profile.params =-agentlib:hprof=cpu=samples,heap=sites, depth=6,force=n,thread=y,verbose=n,file=%s
当map task 运行结束后，它产生的日志会写在$logs/userlogs/job- 文件夹下。当然，你也可以直接在JobTracker的Web界面的logs或jobtracker.jsp 页面找到日志。

我们运行一个简单的SQL语句来观察RCFile 格式在序列化和反序列化上的开销：
hive> select O_CUSTKEY,O_ORDERSTATUS from orders_rc2 where O_ORDERSTATUS='P';
其中的O_CUSTKEY列为integer类型，O_ORDERSTATUS为String类型。在日志输出的最后会包含内存和CPU 的消耗。

下表是一次CPU 的开销：
|rank|self|accum|count|trace|method|
|----|----|----|----|----|----|
|20|0.48%|79.64%|65|315554|org.apache.hadoop.hive.ql.io.RCFile$Reader.getCurrentRow|
|28|0.24%|82.07%|32|315292|org.apache.hadoop.hive.serde2.columnar.ColumnarStruct.init|
|55|0.10%|85.98%|14|315788|org.apache.hadoop.hive.ql.io.RCFileRecordReader.getPos|
|56|0.10%|86.08%|14|315797|org.apache.hadoop.hive.ql.io.RCFileRecordReader.next|

**表2：一次CPU的开销**

其中第五列可以对照上面的Track信息查看到底调用了哪些函数。比如CPU消耗排名20的函数对应Track：
TRACE 315554: (thread=200001)    org.apache.hadoop.hive.ql.io.RCFile$Reader.getCurrentRow(RCFile.java:1434)    org.apache.hadoop.hive.ql.io.RCFileRecordReader.next(RCFileRecordReader.java:88)    org.apache.hadoop.hive.ql.io.RCFileRecordReader.next(RCFileRecordReader.java:39)org.apache.hadoop.hive.ql.io.CombineHiveRecordReader.doNext(CombineHiveRecordReader.java:98)org.apache.hadoop.hive.ql.io.CombineHiveRecordReader.doNext(CombineHiveRecordReader.java:42)    org.apache.hadoop.hive.ql.io.HiveContextAwareRecordReader.next(HiveContextAwareRecordReader.java:67)
其中，比较明显的是RCFile，它为了构造行而消耗了不必要的数组移动开销。其主要是因为RCFile 为了还原行，需要构造RowContainer，顺序读取一行构造RowContainer，然后给其中对应的列进行赋值，因为RCFile早期为了兼容SequenceFile所以可以合并两个block，又由于RCFile不知道列在哪个row group结束，所以必须维持数组的当前位置，类似如下格式定义：
   Array<RowContainer extends List<Object>>
而此数据格式可以改为面向列的序列化和反序列化方式。如：
Map<array<col1Type>,array<col2Type>,array<col3Type>....>
这种方式的反序列化会避免不必要的数组移动，当然前提是我们必须知道列在哪个row group开始到哪个row group结束。这种方式会提高整体反序列化过程的效率。

## 关于Hadoop文件格式的思考

**1 高效压缩**

Hadoop目前尚未出现针对数据特性的高效编码（Encoding）和解码(Decoding)数据格式。尤其是支持Run Length Encoding、Bitmap 这些极为高效算法的数据格式。HIVE-2065 讨论过使用更加高效的压缩形式，但是对于如何选取列的顺序没有结论。关于列顺序选择可以看Daniel Lemire的一篇论文 《Reordering Columns for Smaller Indexes》[1]。作者同时也是Hive 0.8中引入的bitmap 压缩算法基础库的作者。该论文的结论是：当某个表需要选取多个列进行压缩时，需要根据列的选择性(selectivity)进行升序排列，即唯一值越少的列排得越靠前。
 事实上这个结论也是Vertica多年来使用的数据格式。其他跟压缩有关的还有HIVE-2604和HIVE-2600。

**2 基于列和块的序列化和反序列化**

不论排序后的结果是不是真的需要，目前Hadoop的整体框架都需要不断根据数据key进行排序。除了上面提到的基于列的排序，序列化和反序列化之外，Hadoop的文件格式应该支持某种基于块（Block） 级别的排序和序列化及反序列化方式，只有当数据满足需要时才进行这些操作。来自Google Tenzing论文中曾将它作为MR 的优化手段提到过。

“Block Shuffle：正常来说，MR 在Shuffle 的时候使用基于行的编码和解码。为了逐个处理每一行,数据必须先排序。然而，当排序不是必要的时候这种方式并不高效,我们在基于行的shuffle基础上实现了一种基于block的shuffle方式，每一次处理大概1M的压缩block，通过把整个block当成一行，我们能够避免MR框架上的基于行的序列化和反序列化消耗，这种方式比基于行的shuffle 快上3倍以上。”

**3 数据过滤（Skip List）**

除常见的分区和索引之外，使用排序之后的块（Block）间隔也是常见列数据库中使用的过滤数据的方法。Google Tenzing同样描述了一种叫做ColumnIO 的数据格式，ColumnIO在头部定义该Block的最大值和最小值，在进行数据判断的时候，如果当前Block的头部信息里面描述的范围中不包含当前需要处理的内容，则会直接跳过该块。Hive社区里曾讨论过如何跳过不需要的块 ，可是因为没有排序所以一直没有较好的实现方式。包括RCFile格式，Hive的index 机制里面目前还没有一个高效的根据头部元数据就可以跳过块的实现方式。

**4 延迟物化**

真正好的列数据库，都应该可以支持直接在压缩数据之上不需要通过解压和排序就能够直接操作块。通过这种方式可以极大的降低MR 框架或者行式数据库中先解压，再反序列化，然后再排序所带来的开销。Google Tenzing里面描述的Block Shuffle 也属于延迟物化的一种。更好的延迟物化可以直接在压缩数据上进行操作，并且可以做内部循环， 此方面在论文《Integrating Compression and Execution in Column-Oriented Database System》[5]的5.2
 章节有描述。 不过考虑到它跟UDF 集成也有关系，所以，它会不会将文件接口变得过于复杂也是一件有争议的事情。

**5 与Hadoop框架集成**

无论文本亦或是二进制格式，都只是最终的储存格式。Hadoop运行时产生的中间数据却没有办法控制。包括一个MR Job在map和reduce之间产生的数据或者DAG Job上游reduce 和下游map之间的数据，尤其是中间格式并不是列格式，这会产生不必要的IO和CPU 开销。比如map 阶段产生的spill，reduce 阶段需要先copy 再sort-merge。如果这种中间格式也是面向列的，然后将一个大块切成若干小块，并在头部加上每个小块的最大最小值索引，就可以避免大量sort-mege操作中解压—反序列化—排序—合并（Merge）的开销，从而缩短任务的运行时间。

## 其他文件格式

Hadoop社区也曾有对其他文件格式的研究。比如，IBM 研究过面向列的数据格式并发表论文《Column-Oriented Storage Techniques for MapReduce》[4]，其中特别提到IBM 的CIF（Column InputFormat)文件格式在序列化和反序列化的IO消耗上比RCFile 的消耗要小20倍。里面提到的将列分散在不同的HDFS Block 块上的实现方式RCFile 也有考虑过，但是最后因为重组行的消耗可能会因分散在远程机器上产生的延迟而最终放弃了这种实现。此外，最近Avro也在实现一种面向列的数据格式，不过目前Hive
 与Avro 集成尚未全部完成。有兴趣的读者可以关注avro-806 和hive-895。

## 总结

Hadoop 可以与各种系统兼容的前提是Hadoop MR 框架本身能够支持多种数据格式的读写。但如果要提升其性能，Hadoop 需要一种高效的面向列的基于整个MR 框架集成的数据格式。尤其是高效压缩，块重组（block shuffle），数据过滤（skip list）等高级功能，它们是列数据库相比MR 框架在文件格式上有优势的地方。相信随着社区的发展以及Hadoop 的逐步成熟，未来会有更高效且统一的数据格式出现。

## 参考资料

[1]压缩列顺序选择 [http://lemire.me/en/](http://lemire.me/en/) Reordering Columns for Smaller Indexes 论文地址

[2]Hive与Avro 集成 [https://issues.apache.org/jira/browse/HIVE-895](https://issues.apache.org/jira/browse/HIVE-895)

[3]Google 的Tenzing 论文 [http://research.google.com/pubs/DistributedSystemsandParallelComputing.html](http://research.google.com/pubs/DistributedSystemsandParallelComputing.html)

Tenzing A SQL Implementation On The MapReduce Framework

[4]IBM Column-Oriented Storage Techniques for MapReduce [http://pages.cs.wisc.edu/~jignesh/publ/colMR.pdf](http://www.infoq.com/cn/articles/publ/colMR.pdf)

[5]Integrating compression and execution in column-oriented database systems [http://db.lcs.mit.edu/projects/cstore/abadisigmod06.pdf](http://db.lcs.mit.edu/projects/cstore/abadisigmod06.pdf)

[6]Avro 项目主页 [http://avro.apache.org/](http://avro.apache.org/)

[7]MapReduce and Parallel DBMSs: Friends or Foes , repetitive record parsing 小节
[http://cacm.acm.org/magazines/2010/1/55743-mapreduce-and-parallel-dbmss-friends-or-](http://cacm.acm.org/magazines/2010/1/55743-mapreduce-and-parallel-dbmss-friends-or-foes/fulltext)







