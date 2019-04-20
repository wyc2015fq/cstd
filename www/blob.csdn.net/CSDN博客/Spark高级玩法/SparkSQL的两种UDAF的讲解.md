# SparkSQL的两种UDAF的讲解 - Spark高级玩法 - CSDN博客
2018年07月09日 00:07:35[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1648
Spark的dataframe提供了通用的聚合方法，比如count()，countDistinct()，avg(),max(),min()等等。然而这些函数是针对dataframe设计的，当然sparksql也有类型安全的版本，java和scala语言接口都有，这些就适用于强类型Datasets。本文主要是讲解spark提供的两种聚合函数接口:
1,UserDefinedAggregateFunction
2,Aggregator
这两个接口基本上满足了，用户自定义聚合函数的需求。
**UserDefinedAggregateFunction**
类UserDefinedAggregateFunction，在文件udaf.scala里面。是实现用户自定义聚合函数UDAF的基础类，首先，我们先看看该类的基本信息
abstract class UserDefinedAggregateFunction extends Serializable {
StructType代表的是该聚合函数输入参数的类型。例如，一个UDAF实现需要两个输入参数，
类型分别是DoubleType和LongType，那么该StructType格式如下：
    new StructType()
    .add("doubleInput",DoubleType)
    .add("LongType",LongType)
那么该udaf就只会识别，这种类型的输入的数据。
def inputSchema: StructType
   该StructType代表aggregation buffer的类型参数。例如，一个udaf的buffer有
   两个值，类型分别是DoubleType和LongType，那么其格式将会如下：
  new StructType()
      .add("doubleInput", DoubleType)
      .add("longInput", LongType)
     也只会适用于类型格式如上的数据
def bufferSchema: StructType
    dataTypeda代表该UDAF的返回值类型
def dataType: DataType
    如果该函数是确定性的，那么将会返回true，例如，给相同的输入，就会有相同
    的输出
def deterministic: Boolean
    初始化聚合buffer，例如，给聚合buffer以0值
    在两个初始buffer调用聚合函数，其返回值应该是初始函数自身，例如
    merge(initialBuffer,initialBuffer)应该等于initialBuffer。
def initialize(buffer: MutableAggregationBuffer): Unit
    利用输入输入去更新给定的聚合buffer，每个输入行都会调用一次该函数
def update(buffer: MutableAggregationBuffer, input: Row): Unit
    合并两个聚合buffer，并且将更新的buffer返回给buffer1
    该函数在聚合并两个部分聚合数据集的时候调用
def merge(buffer1: MutableAggregationBuffer, buffer2: Row): Unit
    计算该udaf在给定聚合buffer上的最终结果
def evaluate(buffer: Row): Any
    使用给定的Column作为输入参数，来为当前UDAF创建一个Column
@scala.annotation.varargs
def apply(exprs: Column*): Column = {
val aggregateExpression =
AggregateExpression(
ScalaUDAF(exprs.map(_.expr), this),
Complete,
isDistinct = false)
Column(aggregateExpression)
  }
    使用给定Column去重后的值作为参数来生成一个Column
@scala.annotation.varargs
def distinct(exprs: Column*): Column = {
val aggregateExpression =
AggregateExpression(
ScalaUDAF(exprs.map(_.expr), this),
Complete,
isDistinct = true)
Column(aggregateExpression)
  }
}
/**
 * A `Row` representing a mutable aggregation buffer.
 *
 * This is not meant to be extended outside of Spark.
 *
 * @since 1.5.0
 */
@InterfaceStability.Stable
abstract class MutableAggregationBuffer extends Row {
/** Update the ith value of this buffer. */
def update(i: Int, value: Any): Unit
}
给出一个非类型安全的UDAF实现：
import org.apache.spark.sql.expressions.MutableAggregationBuffer
import org.apache.spark.sql.expressions.UserDefinedAggregateFunction
import org.apache.spark.sql.types._
import org.apache.spark.sql.Row
import org.apache.spark.sql.SparkSession
object UserDefinedUntypedAggregation {
object MyAverage extends UserDefinedAggregateFunction {
//输入参数的数据类型
def inputSchema: StructType = StructType(StructField("inputColumn", LongType) :: Nil)
//聚合 buffer的数据类型
def bufferSchema: StructType = {
StructType(StructField("sum", LongType) :: StructField("count", LongType) :: Nil)
    }
// 返回值的数据类型
def dataType: DataType = DoubleType
// 给定的相同的输入，就会有相同的输入。
def deterministic: Boolean = true
//初始化给定的聚合buffer
def initialize(buffer: MutableAggregationBuffer): Unit = {
      buffer(0) = 0L
buffer(1) = 0L
}
// 用输入的row来更新聚合buffer
def update(buffer: MutableAggregationBuffer, input: Row): Unit = {
if (!input.isNullAt(0)) {
        buffer(0) = buffer.getLong(0) + input.getLong(0)
        buffer(1) = buffer.getLong(1) + 1
}
    }
// 合并两个聚合buffer，并将合并后的值返回给 `buffer1`
def merge(buffer1: MutableAggregationBuffer, buffer2: Row): Unit = {
      buffer1(0) = buffer1.getLong(0) + buffer2.getLong(0)
      buffer1(1) = buffer1.getLong(1) + buffer2.getLong(1)
    }
// 计算最终输出
def evaluate(buffer: Row): Double = buffer.getLong(0).toDouble / buffer.getLong(1)
  }
// $example off:untyped_custom_aggregation$
def main(args: Array[String]): Unit = {
val spark = SparkSession
      .builder()
      .appName("Spark SQL user-defined DataFrames aggregation example")
      .getOrCreate()
    //为了使用聚合函数，需要先注册
spark.udf.register("myAverage", MyAverage)
val df = spark.read.json("examples/src/main/resources/employees.json")
    df.createOrReplaceTempView("employees")
    df.show()
// +-------+------+
    // |   name|salary|
    // +-------+------+
    // |Michael|  3000|
    // |   Andy|  4500|
    // | Justin|  3500|
    // |  Berta|  4000|
    // +-------+------+
val result = spark.sql("SELECT myAverage(salary) as average_salary FROM employees")
    result.show()
// +--------------+
    // |average_salary|
    // +--------------+
    // |        3750.0|
    // +--------------+
    // $example off:untyped_custom_aggregation$
spark.stop()
  }
}
Aggregator
用户自定义聚合函数的基类，可以在Dataset中使用，取出一个组的数据，然后聚合。该类的源码
        举个栗子
 *   val customSummer =  new Aggregator[Data, Int, Int] {
 *     def zero: Int = 0
 *     def reduce(b: Int, a: Data): Int = b + a.i
 *     def merge(b1: Int, b2: Int): Int = b1 + b2
 *     def finish(r: Int): Int = r
 *   }.toColumn()
 *
 *   val ds: Dataset[Data] = ...
 *   val aggregated = ds.select(customSummer)
 * }}}
 * @tparam IN The input type for the aggregation.
 * @tparam BUF The type of the intermediate value of the reduction.
 * @tparam OUT The type of the final output result.
 * @since 1.6.0
 */
@Experimental
@InterfaceStability.Evolving
abstract class Aggregator[-IN, BUF, OUT] extends Serializable {
    该剧和函数的0值。需要满足对于任何输入b，那么b+zero=b
def zero: BUF
    聚合两个值产生一个新的值，为了提升性能，该函数会修改b，然后直接返回b，而
    不适新生成一个b的对象。
def reduce(b: BUF, a: IN): BUF
    合并两个中间值
def merge(b1: BUF, b2: BUF): BUF
    转换reduce的输出
def finish(reduction: BUF): OUT
    为中间值类型提供一个编码器
def bufferEncoder: Encoder[BUF]
    为最终的输出结果提供一个编码器 
def outputEncoder: Encoder[OUT]
    将该聚合函数返回为一个TypedColumn，目的是为了能在Dataset中使用
def toColumn: TypedColumn[IN, OUT] = {
implicit val bEncoder = bufferEncoder
implicit val cEncoder = outputEncoder
val expr =
AggregateExpression(
TypedAggregateExpression(this),
Complete,
isDistinct = false)
new TypedColumn[IN, OUT](expr, encoderFor[OUT])
  }
}
该类的一个实现
// 
import org.apache.spark.sql.expressions.Aggregator
import org.apache.spark.sql.Encoder
import org.apache.spark.sql.Encoders
import org.apache.spark.sql.SparkSession
// 
object UserDefinedTypedAggregation {
// 自定义输入类型和输出类型
case class Employee(name: String, salary: Long)
case class Average(var sum: Long, var count: Long)
object MyAverage extends Aggregator[Employee, Average, Double] {
// 0值
def zero: Average = Average(0L, 0L)
// 合并，输入和buffer
def reduce(buffer: Average, employee: Employee): Average = {
      buffer.sum += employee.salary
      buffer.count += 1
buffer
    }
// 合并中间结果
def merge(b1: Average, b2: Average): Average = {
      b1.sum += b2.sum
      b1.count += b2.count
      b1
    }
// 转换reduce的输出类型
def finish(reduction: Average): Double = reduction.sum.toDouble / reduction.count
// 为中间值类型指定编码器
def bufferEncoder: Encoder[Average] = Encoders.product
// 为输出类型指定编码器。
def outputEncoder: Encoder[Double] = Encoders.scalaDouble
}
//
def main(args: Array[String]): Unit = {
val spark = SparkSession
      .builder()
      .appName("Spark SQL user-defined Datasets aggregation example")
      .getOrCreate()
import spark.implicits._
//
val ds = spark.read.json("examples/src/main/resources/employees.json").as[Employee]
    ds.show()
// +-------+------+
    // |   name|salary|
    // +-------+------+
    // |Michael|  3000|
    // |   Andy|  4500|
    // | Justin|  3500|
    // |  Berta|  4000|
    // +-------+------+
    // Convert the function to a `TypedColumn` and give it a name
val averageSalary = MyAverage.toColumn.name("average_salary")
val result = ds.select(averageSalary)
    result.show()
// +--------------+
    // |average_salary|
    // +--------------+
    // |        3750.0|
    // +--------------+
    // $example off:typed_custom_aggregation$
spark.stop()
  }
}
该例子整理自spark2.2.1源码，希望对大家有帮助，欢迎大家进入知识星球，想大神学习，进入高级大牛行列。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWEbA0t2WgVoUbCSPAic4uAuwkHOg8iczLJQ4gpogt2Gy2FelL1iaM9jW6IPAtjuB4c3ZmfmNb1vRy2w/640?wx_fmt=jpeg)
