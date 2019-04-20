# Hive高级优化 - Spark高级玩法 - CSDN博客
2018年01月17日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：321
**1，FetchTask**
不执行mapreduce，提高速度 
设置的三种方式： 
**方法一：**
- 
`set hive.fetch.task.conversion=more;`
**方法二：**
- 
`bin/hive --hiveconf hive.fetch.task.conversion=more`
**方法三： **
上面的两种方法都可以开启了Fetch任务，但是都是临时起作用的；如果你想一直启用这个功能，可以在${HIVE_HOME}/conf/hive-site.xml
- 
`<property>`
- 
`  <name>hive.fetch.task.conversion</name>`
- 
`  <value>minimal</value>`
- 
`  <description>`
- 
`    Some select queries can be converted to single FETCH task minimizing latency.`
- 
`    Currently the query should be single sourced not having any subquery and should not have`
- 
`    any aggregations or distincts (which incurs RS), lateral views and joins.`
- 
`    1. minimal : SELECT STAR, FILTER on partition columns, LIMIT only`
- 
`    2. more    : SELECT, FILTER, LIMIT only (TABLESAMPLE, virtual columns)`
- 
`  </description>`
- 
`</property>`
## **2，大表【拆分子表】**
### **1 背景**
一个日志文件中，每一行记录，会有很多很多字段，四五十个字段很正常.实际分析中，常常使用少数几个字段
### **2 方式**
将原始的表中数据，依据业务需求提取出要分析的字段，数据放入到对应的业务表（子表）中，实际的业务针对业务表进行分析。
### **3 举例**
在实际中，我们会发现，有些业务处理，会有共同数据集 
用户表、订单表、商品表，三个表需要进行join的操作 
    | 
    |   join 会产生一个结果集 
    | 
 会有很多的业务是针对此jion结果集进行分析 
业务一： 
    join 
结论： 
    将众多的业务中相同的中间结果集，抽取到一个Hive中的表中去
## **3，外部表、分区表**
外部表、分区表 
结合使用 
多级分区
数据  
存储格式（textfile、orcfile、parquet） 
数据压缩(snappy)
## **4，jvm重用**
VM重用是hadoop调优参数的内容，对hive的性能具有非常大的影响，特别是对于很难避免小文件的场景或者task特别多的场景，这类场景大多数执行时间都很短。hadoop默认配置是使用派生JVM来执行map和reduce任务的，这是jvm的启动过程可能会造成相当大的开销，尤其是执行的job包含有成千上万个task任务的情况。 
JVM重用可以使得JVM实例在同一个JOB中重新使用N次，N的值可以在Hadoop的mapre-site.xml文件中进行设置
- 
`mapred.job.reuse.jvm.num.tasks 1`
也可在hive的执行设置：
- 
`set  mapred.job.reuse.jvm.num.tasks=10; //一个java虚拟机可以被多个job使用.通常设置成八个`
JVM的一个缺点是，开启JVM重用将会一直占用使用到的task插槽，以便进行重用，直到任务完成后才能释放。如果某个“不平衡“的job中有几个reduce task 执行的时间要比其他reduce task消耗的时间多得多的话，那么保留的插槽就会一直空闲着却无法被其他的job使用，直到所有的task都结束了才会释放。
## **5，推测执行**
推测式任务，也叫做Speculative task。它是一种以空间换时间的策略。 
当所有task都开始运行之后，任务对应的ApplicationMaster会统计所有任务的平均进度，如果某个task所在的task node机器配置比较低或者CPU load很高（原因很多），导致任务执行比总体任务的平均执行要慢， 
此时AM会启动一个新的任务（Speculative task），原有任务和新任务哪个先执行完就把另外一个kill掉， 
这也是我们经常在RM页面看到任务执行成功，但是总有些task被kill，就是这个原因。  
目的：是通过加快获取单个 task 的结果以及进行侦测将执行慢的 TaskTracker 加入到黑名单的方式来提高整体的任务执行效率
(1)修改$HADOOP_HOME/conf/mapred-site.xml 文件
- 
`    <property>`
- 
`        <name>mapred.map.tasks.speculative.execution </name>`
- 
`        <value>true</value>`
- 
`    </property>`
- 
`    <property>`
- 
`        <name>mapred.reduce.tasks.speculative.execution </name>`
- 
`        <value>true</value>`
- 
`    </property>`
(2)修改hive 配置
- 
`set hive.mapred.reduce.tasks.speculative.execution=true;`
## **6，map数目**
- 
`dfs.block.size=268435456`
- 
`hive.merge.mapfiles=true`
- 
`hive.merge.mapredfiles=true`
- 
`hive.merge.size.per.task=256000000`
- 
`mapred.map.tasks=2 `
因为合并小文件默认为true，而dfs.block.size与hive.merge.size.per.task的搭配使得合并后的绝大部分文件都在300MB左右。 
CASE 1： 
现在我们假设有3个300MB大小的文件，那么goalsize = min(900MB/2,256MB) = 256MB 
所以整个JOB会有6个map，其中3个map分别处理256MB的数据，还有3个map分别处理44MB的数据。 
这时候木桶效应就来了，整个JOB的map阶段的执行时间不是看最短的1个map的执行时间，而是看最长的1个map的执行时间。所以，虽然有3个map分别只处理44MB的数据，可以很快跑完，但它们还是要等待另外3个处理256MB的map。显然，处理256MB的3个map拖了整个JOB的后腿。 
CASE 2： 
如果我们把mapred.map.tasks设置成6，再来看一下有什么变化：goalsize = min(900MB/6,256MB) = 150MB 
整个JOB同样会分配6个map来处理，每个map处理150MB的数据，非常均匀，谁都不会拖后腿，最合理地分配了资源，执行时间大约为CASE 1的59%(150/256)  
案例分析： 
虽然mapred.map.tasks从2调整到了6，但是CASE 2并没有比CASE 1多用map资源，同样都是使用6个map。而CASE 2的执行时间约为CASE 1执行时间的59%。 
从这个案例可以看出，对mapred.map.tasks进行自动化的优化设置其实是可以很明显地提高作业执行效率的。
要求：文件数据压缩的方式为可分割的，比如BZip2Codec。或者文件数据本身没有用压缩。
## **7，设置reduce个数**
### **1 reduce个数**
参数1：hive.exec.reducers.bytes.per.reducer=256000000：每个reduce任务处理的数据量 
参数2：hive.exec.reducers.max=1009：每个任务最大的reduce数目 
计算公式：reducer个数=min(参数2,总输入数据量/参数1) 
　set mapred.reduce.tasks =N： 
每个任务默认的reduce数目。典型为0.99*reduce槽数，hive默认为-1，即自动确定reduce数目。
reduce个数并不是越多越好 
同map一样，启动和初始化reduce也会消耗时间和资源；  
另外，有多少个reduce,就会有多少个输出文件，如果生成了很多个小文件，那么如果这些小文件作为下一个任务的输入，则也会出现小文件过多的问题
### **2 什么情况下只有一个reduce**
很多时候你会发现任务中不管数据量多大，不管你有没有设置调整reduce个数的参数，任务中一直都只有一个reduce任务；  
其实只有一个reduce任务的情况，除了数据量小于 
hive.exec.reducers.bytes.per.reducer参数值的情况外，还有以下原因：  
a)    没有group by的汇总  
b)    用了Order by
## **8，并行执行**
并行执行，意思是同步执行hive的多个阶段，hive在执行过程，将一个查询转化成一个或者多个阶段。某个特定的job可能包含众多的阶段，而这些阶段可能并非完全相互依赖的，也就是说可以并行执行的，这样可能使得整个job的执行时间缩短
- 
`hive.exec.parallel.thread.number    8//job并行执行的数目，一个SQL语句可能有很多mapreduce任务，限制`
- 
`hive.exec.parallel  false`
hive执行开启：
- 
```
set hive.exec.parallel=true
```
``
**推荐阅读：**
1，[hive的分区和分桶](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484404&idx=1&sn=88c0418a866572c1dd85ced93d1168f0&chksm=9f38e0dca84f69ca45cd77722839004db02019297c8608835114e382d1b85c6bb6c1fe920934&scene=21#wechat_redirect)
2，[Hive : SORT BY vs ORDER BY vs DISTRIBUTE BY vs CLUSTER BY](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484236&idx=1&sn=ee9622d02aa4d6888650014d139e0793&chksm=9f38e064a84f697293ecc7bcd2773c990e26f014dda4d095ff17ec48b35849f76fe7107ba9b2&scene=21#wechat_redirect)
3，[基于Spark Mllib的文本分类](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484354&idx=1&sn=7167fd21809c50ad4681e5ab2a6a6390&chksm=9f38e0eaa84f69fce1ec6cc094c74a3beac403a1dac0c50bdd2199ec4e9631d365af905c9068&scene=21#wechat_redirect)
4，[重磅：Flume1-7结合kafka讲解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484218&idx=1&sn=fe25af996f713b12359bd6e18cef5f31&chksm=9f38e012a84f69044ff9285b9f70e5e8aa6d6458faa306ac59645e307a8791b7acac77d156dc&scene=21#wechat_redirect)
![640.png?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png?)
**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg?)
**更多文章，敬请期待**
