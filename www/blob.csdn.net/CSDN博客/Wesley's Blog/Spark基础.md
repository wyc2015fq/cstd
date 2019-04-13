
# Spark基础 - Wesley's Blog - CSDN博客


2018年05月10日 15:16:57[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：48标签：[Spark基础																](https://so.csdn.net/so/search/s.do?q=Spark基础&t=blog)[大数据																](https://so.csdn.net/so/search/s.do?q=大数据&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Spark基础&t=blog)个人分类：[大数据																](https://blog.csdn.net/yinanmo5569/article/category/7652577)



## Spark的特点
Spark是快速且通用的计算平台．
快速性（基于内存计算，扩充了流行的MapReduce计算模型）
通用性（容纳了其他分布式系统拥有的功能，批处理，迭代式计算，交互查询和流处理等）
高度开放（提供了Python,Java,Scala,SQL的API和丰富的内置库，和其他大数据工具整合得很好）
![这里写图片描述](https://img-blog.csdn.net/20180510151401588?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## Spark的组件
Spark包括多个紧密集成的组件
![这里写图片描述](https://img-blog.csdn.net/20180510151421821?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### Spark Core
包含Spark的基本功能，包含任务调度，内存管理，容错机制等
内部定义了RDDs（弹性分布式数据集）
提供了很多APIs来创建和操作这些RDDs
应用场景：为其他组件提供了底层的服务
### Spark SQL
是Spark处理结构化数据的库，就像Hive SQL,MySql一样
应用场景：企业中用来做报表统计
### Spark Streaming
是实时数据流处理组件，类似Storm
Spark Streaming提供了API来操作实时流数据
应用场景:企业中用来从Kafka接收数据做实时统计
### Mlib
一个包含通用机器学习功能的包，Machine Learning lib.
包含分类，聚类，回归等，还包括模型评估，和数据导入
MLib提供的这些方法，都支持集群上的横向扩展
应用场景:机器学习
### Graphx
是处理图的库（例如社交网络图），并进行图的并行计算
像Spark Streaming, Spark SQL一样，它也继承了RDD API
它提供了各种图的操作，和常用的图算法，例如PangeRank算法
应用场景:图计算
### Cluster Managers
集群管理，Spark自带一个集群管理是单独调度器
常见的集群管理包括Hadoop YARN, Apache Mesos
## Spark v.s. Hadoop
### Hadoop应用场景
离线处理
对时效性要求不高（处理数据时，中间数据会落到硬盘上）
### Spark应用场景
时效性要求较高的场景(基于内存计算）
机器学习场景
不具有HDFS的存储能努力，需要借助HDFS来持久化数据
## RDDs介绍
### Definition
Resilient distributed datasets(弹性分布式数据集，简写RDDs)
RDDs并行分布在整个集群中
RDDs是Spark分发数据和计算的基础抽象类
一个RDD是一个不可改变的分布式集合对象
Spark中，所有的计算都是通过RDDs的创建，转换，操作完成的
一个RDD内部由许多partitions（分片）组成，分片是Spark并行处理的单元，Spark顺序的，并行的处理分片
### Driver program
包含程序的main()方法，RDDs的定义和操作．它管理很多节点，称作executors
### SparkContext
Driver programs通过SparkContext对象访问Spark.
SparkContext对象代表和一个集群的连接(类似于Adapter和Controller)
在Shell中SparkContext自动创建好了，就是sc
![这里写图片描述](https://img-blog.csdn.net/20180510151450818?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## RDDs基本操作
### Transformation
从之前的RDD构建一个新的RDD，例如map()和filter().
map():逐元素Transformation
filter():接收函数，返回只包含满足filter()函数的元素的新RDD
flatMap():对于每个输入元素，输出多个输出元素，然后再放到一个RDD中
集合运算:例如distinct(), union(), innter()等运算
### Action
在RDD上计算出来一个结果，把结果返回给driver program或保存在文件系统，例如count(),save()等
reduce():接收一个函数，作用在RDD两个类型相同的元素上，返回新元素．可以实现RDD中元素的累加，计数和其他类型的聚集操作
collect():遍历整个RDD,向driver program返回RDD的内容，需要单机内存可以容纳下的(因为数据要拷贝给driver,测试使用).大数据的时候，使用saveAsTextFile() action等．
take(n):返回RDD的n个元素（同时尝试访问最少的partition，无序，一般测试使用)
top():排序(根据RDD中数据的比较器)
foreach():计算RDD中的每个元素，不返回到本地(一般与println使用，测试用）
![这里写图片描述](https://img-blog.csdn.net/20180510151511816?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## RDDs的特性
### RDDs的血统关系图
Spark维护着RDDs之间的依赖关系和创建关系，叫做血统关系图
Spark使用血统关系图来计算每个RDD的需求和恢复丢失的数据
![这里写图片描述](https://img-blog.csdn.net/20180510151525178?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 延迟计算(Lazy Evaluation)
Spark对RDDs的计算是他们第一次使用action操作的时候
可以减少数据的传输
Spark内部记录metadata表名transformations操作已经被响应了
加载数据也是延迟计算，必要的时候才会被加载进去
### 持久性
默认每次在RDDs上面进行action操作时，Spark都需要重新计算RDDs
如果想重复利用一个RDD，可以使用RDD.persist()
uppersist()方法从缓存中移除
![这里写图片描述](https://img-blog.csdn.net/20180510151549197?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180510151559621?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### KeyValue对RDDs
使用map()函数，返回key/value对
常用操作:
![这里写图片描述](https://img-blog.csdn.net/20180510151610691?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180510151620148?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
#### combineByKey()
最常用的基于key的聚合函数，返回的类型可以与输入类型不一样，许多基于key的聚合函数都用到了它，像groupByKey()
combinerByKey():(createCombiner,mergeValue,mergeCombiners,partitioner)(createCombiner, mergeValue, mergeCombiners, partitioner)

