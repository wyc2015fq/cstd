# Spark中算子 - LC900730的博客 - CSDN博客
2017年12月10日 17:16:10[lc900730](https://me.csdn.net/LC900730)阅读数：162
## Value型Transformation算子
处理数据类型为Value型的Transformation算子可以根据RDD变换算子的输入分区与输出分区关系分为以下几种类型
- 1.输入分区与输出分区一对一型
- 2.输入分区与输出分区多对一型
- 3.输入分区与输出分区多对多
- 4.输出分区为输入分区子集型
- 5.还有一种特殊的输入与输出分区一对一的算子类型：Cache型，对RDD分区进行缓存。
### 1.输入分区与输出分区一对一型
#### (1)map
将原来RDD中每个数据项通过map中的用户自定义函数f映射转变为一个新的元素。源码中map算子相当于初始化一个RDD，新RDD叫做MappedRDD(this,sc.clean(f))。
只有等到Action算子触发后，这个f函数才会和其他函数在一个Stage中对数据进行运算。
#### (2)flatmap
将原来RDD中每个元素通过f转换为新的元素，并将生成的RDD的每个集合中的元素合并为一个集合。内部创建新的FlatMappedRDD(this,sc.clean(f))。
#### (3)mapPartitions
mapPartitions函数获取到每个分区的迭代器，在函数中通过这个分区整体的迭代器对整个分区的元素进行操作。内部实现是生成MapPartitionsRDD。 
如f(iter)=>iter.filter(_>=3)对分区中所有数据进行过滤，>=3的数据保留。
#### (3)glom
glom函数将每个分区形成一个数组，内部实现是返回的GlommedRDD。
![这里写图片描述](https://img-blog.csdn.net/20171210152008216?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 2.输入分区与输出分区多对一型
#### (1)union
使用union需要保证两个RDD元素数据类型相同，返回的RDD数据类型和被合并的RDD元素数据类型相同，并不进行去重操作，保存所有元素。
#### (2)cartesian
对两个RDD内的所有元素进行笛卡尔积操作。操作后内部实现返回CartesianRDD。
### 3.输入分区与输出分区多对多
groupBy：将元素通过函数生成相应的key，数据就转化为Key-Value格式，之后将Key相同的元素分为一组。 
函数实现如下： 
1️⃣：sc.clean()函数将用户函数预处理 
val cleanF=sc.clean() 
2️⃣：对数据map进行函数操作，最后对groupByKey进行分组操作。 
this.map(t=>(cleanF(t),t)).groupByKey(p) 
其中p中确定分区个数和分区函数，也就决定了并行化程度。
### 4.输出分区为输入分区子集型
#### (1) filter
对每个元素应用f函数，返回值为true的元素在RDD中保留，返回为false的过滤掉。内部实现相当于生成FilteredRDD(this,sc.clean(f))。 
def filter(f:T=>Boolean):RDD[T]=new FilteredRDD(this,sc.clean(f))
#### (2) distinct
将RDD中元素去重。
#### (3) subtract
subtract相当于进行集合的差操作。RDD1去除RDD1和RDD2交集中的所有元素。
#### (4) sample
sample将RDD这个集合内的元素这样采样，获取所有元素的子集。用户可以设定是否有放回的抽样、百分比、随机种子，进而决定采样方式。 
内部实现是生成SampleRDD(withReplacement,fraction,seed)。
#### (5) takeSample
按照设定的采样个数进行采样，同时返回结果不再是RDD，而是相当于对采样后的数据进行Collect(),返回结果的集合为单机的数组。
### 5.Cache型
#### (1) cache
cache将RDD元素从磁盘缓存到内存，相当于persist(MEMORY_ONLY)函数的功能。
#### (2) persist
persist函数对RDD进行缓存操作。
## Key-Value型Transformation算子
### 1.输入分区与输出分区一对一
mapValues：针对(Key,Value)型数据中的Value进行Map操作，而不对Key进行处理。 
a=>a+2代表只针对(V1,1)数据中1进行+2操作。
### 2.对单个或者2个RDD聚集
#### (1)单个RDD聚集
- combineByKey
```
//定义combineByKey算子代码如下
combineByKey[C](
    //createCombiner:V=>C,在C不存在情况下，通过V创建seq C
    createCombiner:(V)=>C,
    //当C已经存在情况下，需要merge，把item V加到seq C中或者叠加
    mergeValues:(C,V)=>C,
    //合并2个C
    mergeCombiners:(C,C)=>C,
    //分区器，Shuffle时需要通过Partitioner分区策略进行分区
    partitioner:Partitioner
    //为了减少传输量，很多combine可以在map端先做。
    mapSideCombine:Boolean=true,
    //传输需要序列化，用户可以自定义序列化类
    serializer:Serializer=null):RDD[(K,C)]
    //如将(Int,Int)的RDD转变为了（Int,Seq[Int]）类型元素的RDD
)
```
- 
reduceByKey：两个值合并成一个值
- 
partitionBy：partitionBy函数对RDD进行分区操作。
#### (2)两个RDD进行聚集
cogroup函数将两个RDD进行协同划分。 
对在两个RDD中的Key-Value类型的元素，每个RDD相同的Key的元素分别聚合为一个集合，并且返回两个RDD中对应Key的元素集合的迭代器。
### 连接
