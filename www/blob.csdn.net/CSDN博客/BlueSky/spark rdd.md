# spark rdd - BlueSky - CSDN博客
2016年05月03日 17:50:02[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：529
     RDD是弹性分布式数据集，即一个RDD代表一个被分区的只读数据集。一个RDD生成有两种途径，一个来自于内存集合和外部存储系统，另一种是通过转换操作来自于其他RDD，比如：map、filter、jion等。
    RDD分区：对于一个RDD而言，分区的多少代表着对这个RDD进行并行计算的粒度，每一个RDD分区的计算操作都在一个单独的任务中被执行。对于分区的多少用户可以自己指定，如果没有指定将会使用默认值（程序所分配到的资源的CPU核的个数）。
    RDD优先位置(preferredLocations)：RDD优先位置属性与Spark中的调度相关，返回的是此RDD的每个partition所存储的位置，按照“移动数据不如移动计算”的理念，在Spark进行任务调度的时候，尽可能的将任务分配到数据块所存储的位置。preferredLocations返回每个数据块所在的机器名或IP地址。
    RDD依赖关系(dependencies):由于RDD是粗粒度的操作数据集，每一个转换操作都会生成一个新的RDD，所以RDD之间形成类似流水线一样的前后依赖关系，Spark中有两种依赖：窄依赖和宽依赖。
    RDD分区计算(compute)：Spark中每个RDD的计算都是以partition(分区)为单位的，而且RDD中的compute函数都是在对迭代器进行渡河，不需要保存每次计算的结果。
    RDD分区函数(partitioner):partitioner是RDD的分区函数，目前Spark中实现了两种类型的分区函数，即HashPatitioner(哈希分区)和RangePatitioner(区域分区)，且partitioner这个属性只存在于（K,V）类型的RDD中，对于非（K，V）类型的partitioner值就是None。partitioner函数即决定了RDD本身的分区数量，也可以作为RDD Shuffle输出（MapOutPut）中每个分区进行数据切割的依据。
    **创建操作**:1、集合创建操作：RDD的形成可以由内部集合类型生成。Spark中提供了parallelize和makeRDD两类函数来实现从集合生成RDD，eg：`val rdd = sc.makeRDD(1 to 10,3)`;2、存储创建操作，通过对文件的读取构造RDD
