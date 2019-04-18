# RDD，DataFrame与DataSet - Soul Joy Hub - CSDN博客

2017年03月16日 16:27:12[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：445标签：[dataset																[Spark																[DataFrame																[rdd](https://so.csdn.net/so/search/s.do?q=rdd&t=blog)](https://so.csdn.net/so/search/s.do?q=DataFrame&t=blog)](https://so.csdn.net/so/search/s.do?q=Spark&t=blog)](https://so.csdn.net/so/search/s.do?q=dataset&t=blog)
个人分类：[Spark](https://blog.csdn.net/u011239443/article/category/6280796)


[http://blog.csdn.net/wo334499/article/details/51689549](http://blog.csdn.net/wo334499/article/details/51689549)
[http://www.jianshu.com/p/c0181667daa0](http://www.jianshu.com/p/c0181667daa0)

# RDD

介绍可以参阅 ： [http://blog.csdn.net/u011239443/article/details/53894611](http://blog.csdn.net/u011239443/article/details/53894611)

## 优点:
- 编译时类型安全
- 编译时就能检查出类型错误
- 面向对象的编程风格
- 直接通过类名点的方式来操作数据

## 缺点:
- 序列化和反序列化的性能开销
- 无论是集群间的通信, 还是IO操作都需要对***对象的结构***和数据进行序列化和反序列化.
- GC的性能开销
- 频繁的创建和销毁对象, 势必会增加GC

# DataFrame

DataFrame引入了schema和off-heap
- 
schema : RDD每一行的数据, 结构都是一样的. 这个结构就存储在schema中. Spark通过schame就能够读懂数据, 因此在通信和IO时就只需要序列化和反序列化数据, 而***结构的部分***就可以省略了.

- 
off-heap : 意味着JVM堆以外的内存, 这些内存直接受操作系统管理（而不是JVM）。Spark能够以二进制的形式序列化数据(不包括结构)到off-heap中, 当要操作数据时, 就直接操作off-heap内存. 由于Spark理解schema, 所以知道该如何操作.

off-heap就像地盘, schema就像地图, Spark有地图又有自己地盘了, 就可以自己说了算了, 不再受JVM的限制, 也就不再收GC的困扰了.

![这里写图片描述](https://img-blog.csdn.net/20170316155316950?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上图直观地体现了DataFrame和RDD的区别。左侧的RDD[Person]虽然以Person为类型参数，但Spark框架本身不了解Person类的内部结构。而右侧的DataFrame却提供了详细的结构信息，使得Spark SQL可以清楚地知道该数据集中包含哪些列，每列的名称和类型各是什么。DataFrame多了数据的结构信息，即schema。RDD是分布式的Java对象的集合。DataFrame是分布式的Row对象的集合。DataFrame除了提供了比RDD更丰富的算子以外，更重要的特点是提升执行效率、减少数据读取以及执行计划的优化，比如filter下推、裁剪等。

## 提升执行效率

RDD API是函数式的，强调不变性，在大部分场景下倾向于创建新对象而不是修改老对象。这一特点虽然带来了干净整洁的API，却也使得Spark应用程序在运行期倾向于创建大量临时对象，对GC造成压力。在现有RDD API的基础之上，我们固然可以利用mapPartitions方法来重载RDD单个分片内的数据创建方式，用复用可变对象的方式来减小对象分配和GC的开销，但这牺牲了代码的可读性，而且要求开发者对Spark运行时机制有一定的了解，门槛较高。另一方面，Spark SQL在框架内部已经在各种可能的情况下尽量重用对象，这样做虽然在内部会打破了不变性，但在将数据返回给用户时，还会重新转为不可变数据。利用 DataFrame API进行开发，可以免费地享受到这些优化效果。

## 减少数据读取

分析大数据，最快的方法就是 ——忽略它。这里的“忽略”并不是熟视无睹，而是根据查询条件进行恰当的剪枝。

上文讨论分区表时提到的分区剪 枝便是其中一种——当查询的过滤条件中涉及到分区列时，我们可以根据查询条件剪掉肯定不包含目标数据的分区目录，从而减少IO。

对于一些“智能”数据格 式，Spark SQL还可以根据数据文件中附带的统计信息来进行剪枝。简单来说，在这类数据格式中，数据是分段保存的，每段数据都带有最大值、最小值、null值数量等 一些基本的统计信息。当统计信息表名某一数据段肯定不包括符合查询条件的目标数据时，该数据段就可以直接跳过（例如某整数列a某段的最大值为100，而查询条件要求a > 200）。

此外，Spark SQL也可以充分利用RCFile、ORC、Parquet等列式存储格式的优势，仅扫描查询真正涉及的列，忽略其余列的数据。

## 执行优化

![这里写图片描述](https://img-blog.csdn.net/20170316155651389?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

为了说明查询优化，我们来看上图展示的人口数据分析的示例。图中构造了两个`DataFrame`，将它们`join`之后又做了一次`filter`操作。如果原封不动地执行这个执行计划，最终的执行效率是不高的。因为`join`是一个代价较大的操作，也可能会产生一个较大的数据集。如果我们能将`filter`下推到 `join`下方，先对`DataFrame`进行过滤，再`join`过滤后的较小的结果集，便可以有效缩短执行时间。而`Spark SQL`的查询优化器正是这样做的。简而言之，逻辑查询计划优化就是一个利用基于关系代数的等价变换，将高成本的操作替换为低成本操作的过程。

得到的优化执行计划在转换成物 理执行计划的过程中，还可以根据具体的数据源的特性将过滤条件下推至数据源内。最右侧的物理执行计划中Filter之所以消失不见，就是因为溶入了用于执行最终的读取操作的表扫描节点内。

对于普通开发者而言，查询优化 器的意义在于，即便是经验并不丰富的程序员写出的次优的查询，也可以被尽量转换为高效的形式予以执行。 

此外，通过schema和off-heap, DataFrame解决了RDD的缺点, 但是却丢了RDD的优点. DataFrame不是类型安全的, API也不是面向对象风格的.

```
import org.apache.spark.sql.types.{DataTypes, StructField, StructType}
import org.apache.spark.sql.{Row, SQLContext}
import org.apache.spark.{SparkConf, SparkContext}

object Run {
  def main(args: Array[String]) {
    val conf = new SparkConf().setAppName("test").setMaster("local")
    val sc = new SparkContext(conf)
    sc.setLogLevel("WARN")
    val sqlContext = new SQLContext(sc)
    /**
      * id      age
      * 1       30
      * 2       29
      * 3       21
      */
    val idAgeRDDRow = sc.parallelize(Array(Row(1, 30), Row(2, 29), Row(4, 21)))

    val schema = StructType(Array(StructField("id", DataTypes.IntegerType), StructField("age", DataTypes.IntegerType)))

    val idAgeDF = sqlContext.createDataFrame(idAgeRDDRow, schema)
    // API不是面向对象的
    idAgeDF.filter(idAgeDF.col("age") > 25) 
    // 不会报错, DataFrame不是编译时类型安全的
    idAgeDF.filter(idAgeDF.col("age") > "") 
  }
}
```

# DataSet

`DataSet`结合了`RDD`和`DataFrame`的优点, 并带来的一个新的概念`Encoder`

当序列化数据时, `Encoder`产生字节码与`off-heap`进行交互, 能够达到***按需访问数据***的效果, 而不用反序列化整个对象. `Spark`还没有提供自定义`Encoder`的`API`, 但是未来会加入.

下面看`DataFrame`和`DataSet`在`2.0.0-preview`中的实现

下面这段代码, 在`1.6.x`中创建的是`DataFrame`：

```
// 上文DataFrame示例中提取出来的
val idAgeRDDRow = sc.parallelize(Array(Row(1, 30), Row(2, 29), Row(4, 21)))

val schema = StructType(Array(StructField("id", DataTypes.IntegerType), StructField("age", DataTypes.IntegerType)))

val idAgeDF = sqlContext.createDataFrame(idAgeRDDRow, schema)
```

但是同样的代码在2.0.0-preview中, 创建的虽然还叫DataFrame：

```
// sqlContext.createDataFrame(idAgeRDDRow, schema) 方法的实现, 返回值依然是DataFrame
def createDataFrame(rowRDD: RDD[Row], schema: StructType): DataFrame = {
sparkSession.createDataFrame(rowRDD, schema)
}
```

但是其实却是DataSet, 因为DataFrame被声明为Dataset[Row]：

```
package object sql {
  // ...省略了不相关的代码

  type DataFrame = Dataset[Row]
}
```

因此当我们从1.6.x迁移到2.0.0的时候, 无需任何修改就直接用上了DataSet.


