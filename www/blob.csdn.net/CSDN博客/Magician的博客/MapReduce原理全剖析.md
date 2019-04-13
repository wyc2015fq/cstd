
# MapReduce原理全剖析 - Magician的博客 - CSDN博客


2017年12月28日 17:00:37[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：2586所属专栏：[自学大数据之路](https://blog.csdn.net/column/details/18514.html)



## MapReduce剖析图
![这里写图片描述](https://img-blog.csdn.net/20171228164304301?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMTY2MzM0MDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如上图所示是MR的运行详细过程
首先mapTask读文件是通过InputFormat（内部是调RecordReader()–＞read()）来一次读一行，返回K,V值。（默认是TextInputFormat，还可以输入其他的类型如:音视频，图片等。）。
mapper(map(k,v))–＞context.write()即mapTask阶段。
输出数据到OutputCollector收集器（不会输出一组就传到下一步进行处理，而是需要一个收集的过程，减少IO）
将收集到的数据写到环形缓冲区–＞通过Spiller来将溢出的数据溢出到文件中去（在这里会通过hashPartitioner执行分区、通过key.comPareTo来实现排序（分为系统默认的快排和外部排序）即实现了shuffle的核心机制：分区和排序）。
将多个溢出文件进行Merge（采用归并排序），合并成一个大文件。
将文件下载到ReduceTask的本地磁盘工作目录–＞将多个MapTask的输出结果再进行归并排序（也可以说是ReduceTask去各个mapTask对应的分区去取对应的数据）。
Reducer(reduce（k,v))–＞context.write(k,v)–＞
OutputFormat(RecordWriter.write(k,v))即reduceTask阶段。
将数据写到part-r-00000这里
如上图所示，图中用红色粗线描述的组件都是可以重写的。
mapreduce在编程的时候，基本上一个固化的模式，没有太多可灵活改变的地方，除了以下几处：
1、输入数据接口：InputFormat   —>     FileInputFormat(文件类型数据读取的通用抽象类)  DBInputFormat （数据库数据读取的通用抽象类）
默认使用的实现类是 ：TextInputFormat。
job.setInputFormatClass(TextInputFormat.class)
TextInputFormat的功能逻辑是：一次读一行文本，然后将该行的起始偏移量作为key，行内容作为value返回
2、逻辑处理接口： Mapper
完全需要用户自己去实现其中  map()   setup()   clean()
3、map输出的结果在shuffle阶段会被partition以及sort，此处有两个接口可自定义：
Partitioner
有默认实现 HashPartitioner，逻辑是  根据key和numReduces来返回一个分区号； key.hashCode()&Integer.MAXVALUE % numReduces
通常情况下，用默认的这个HashPartitioner就可以，如果业务上有特别的需求，可以自定义Comparable
当我们用自定义的对象作为key来输出时，就必须要实现WritableComparable接口，override其中的compareTo()方法
4、reduce端的数据分组比较接口 ： Groupingcomparator
reduceTask拿到输入数据（一个partition的所有数据）后，首先需要对数据进行分组，其分组的默认原则是key相同，然后对每一组kv数据调用一次reduce()方法，并且将这一组kv中的第一个kv的key作为参数传给reduce的key，将这一组数据的value的迭代器传给reduce()的values参数
5、逻辑处理接口：Reducer
完全需要用户自己去实现其中  reduce()   setup()   clean()
6、输出数据接口： OutputFormat  —> 有一系列子类  FileOutputformat  DBoutputFormat  …..
默认实现类是TextOutputFormat，功能逻辑是：  将每一个KV对向目标文本文件中输出为一行
整个过程需要注意以下几点：
环形缓存区（数据从outputCollector中传入环形缓存区，直到达到80%的缓存时，缓存才会启用清理机制，将已经溢出的数据溢出到文件中去（通过spiller来将数据溢出到文件中去））会溢出多次，每次溢出都会对数据进行分区排序，形成多个分区排序后的数据，最终进行合并。
combiner的作用：对spiller阶段的溢出数据进行一个reduce处理，直接让相同k的value值相加，减少数据量以及传输过程中的开销，大大提高效率。（根据业务需求使用，并不是每个业务都要用。可自定义一个Combiner类，内部逻辑和Reduce类似）
shuffle:洗牌、发牌——（核心机制：数据分区，排序，缓存）
具体来说：就是将maptask输出的处理结果数据，分发给reducetask，并在分发的过程中，对数据按key进行了分区和排序；
数据倾斜：指的是任务在shuffle阶段时会进行一个分区操作（默认的是hashcode取模），如果有大部分数据被分到一个ReduceTask端进行处理，一小部分任务被分到其他的ReduceTask端进行处理，就会造成其他ReduceTask处理完成后，仍有一个ReduceTask还在处理数据。最终造成整个工程延迟的情况。（为了解决这个问题，引入了Partition）

## 总结：
MapReduce中最核心的知识点就是MR运行的整体流程；除此之外要达到博主菜鸟级别的水平，你还需要了解以下几个知识点：
1、MapReduce是什么，用来干什么的（无论学什么首先都得知道这一点）。
2、MapReduce框架的设计思想。
3、MapReduce框架中的程序实体角色以及对应的作用：maptask reducetask mrappmaster。
（1和2详见：[https://blog.csdn.net/qq_16633405/article/details/78404018](https://blog.csdn.net/qq_16633405/article/details/78404018)）
4、MapReduce程序中maptask任务切片规划的机制。
（详见：[https://blog.csdn.net/qq_16633405/article/details/79729172](https://blog.csdn.net/qq_16633405/article/details/79729172)）
5、Yarn在MapReduce中作用。
（详见：[https://blog.csdn.net/qq_16633405/article/details/79734021](https://blog.csdn.net/qq_16633405/article/details/79734021)）
6、掌握MapReduce的编程套路，通过不断的写MR案例领悟编写MR程序的核心思想（即如何确定对应K-V值）。
（后续会将对应的MR的一些特殊套路案例上传到Git上）
目前想到的就这么点，后续有遗漏的话会接着补充。

