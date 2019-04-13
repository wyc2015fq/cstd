
# Spark sql DataFrame Datasets Guide - leofionn的博客 - CSDN博客


2018年04月08日 17:45:19[leofionn](https://me.csdn.net/qq_36142114)阅读数：571


Spark SQL不是SQL ！！！！！！！！

##### 一.Spark  SQL定义
##### Spark SQL is Apache Spark's module for working with structured data.
##### Spark结构查询语言是阿帕奇Spark用于处理结构化数据的模块。
##### （没有体现sql）
##### Spark SQL is not about SQL
##### Spark SQL is about more than SQL
##### 不仅仅是sql框架而是超出范围
二.Spark SQL的优势
1.集成
将SQL查询与Spark程序无缝混合。
2.统一的数据访问
以同样的方式连接到任何数据源。（不需要sqoop）
3.Hive集成
在现有仓库上运行SQL或HiveQL查询。
公用metastore为数据中心
4.标准连接
通过JDBC或ODBC连接。对稳定性，社区支持，效果非常好
也就是Data Source API！
三.DataFrame、Datasets
数据集是分布式数据集合。数据集是Spark 1.6中添加的新接口，它提供了RDD的优点（强打字，使用强大的lambda函数的能力）以及Spark SQL优化执行引擎的优势。数据集可以被[构造](http://spark.apache.org/docs/2.2.0/sql-programming-guide.html#creating-datasets)从JVM对象，然后使用功能性的转换（操作`map`，`flatMap`，`filter`等等）。数据集API可用于[Scala](http://spark.apache.org/docs/2.2.0/api/scala/index.html#org.apache.spark.sql.Dataset)和[Java](http://spark.apache.org/docs/2.2.0/api/java/index.html?org/apache/spark/sql/Dataset.html)。Python不支持数据集API。但是由于Python的动态特性，数据集API的许多优点已经可用（例如，您可以自然地通过名称访问行的字段`row.columnName`）。R的情况类似。
DataFrame是一个*数据集，*组织到命名列中。它在概念上等同于关系数据库中的表或R / Python中的数据框，但在引擎盖下具有更丰富的优化。DataFrame可以从各种[来源](http://spark.apache.org/docs/2.2.0/sql-programming-guide.html#data-sources)构建而成，例如：结构化数据文件，Hive中的表格，外部数据库或现有的RDD。数据帧API是Scala，Java的，可用[的Python](http://spark.apache.org/docs/2.2.0/api/python/pyspark.sql.html#pyspark.sql.DataFrame)和[[R ](http://spark.apache.org/docs/2.2.0/api/R/index.html)。在Scala和Java中，DataFrame由`Row`s的数据集表示。在[Scala API中](http://spark.apache.org/docs/2.2.0/api/scala/index.html#org.apache.spark.sql.Dataset)，`DataFrame`只是一个类型的别名`Dataset[Row]`。而在[Java API中](http://spark.apache.org/docs/2.2.0/api/java/index.html?org/apache/spark/sql/Dataset.html)，用户需要`Dataset<Row>`用来表示一个`DataFrame`。
四.操作
import org.apache.spark.sql.SparkSession
val spark = SparkSession
.builder()
.appName("Spark SQL")
.master("local [2]")
.getOrCreate()
//用于隐式转换，如将RDD转换为DataFrames
import spark.implicits._
val df = spark.read.json("/usr/data/data1.json")
df.show()
//DataFrame操作
import spark.implicits._
df.printSchema()
//树形式打印模式
df.select("xxx").show()
//20行"xxx"数据   、、
df.select($"xxx", $"ccc" + 1).show()
//21行“xxx"数据
df.filter($"ccc" > 21).show()
df.groupBy("ccc").count().show()


##### 五、Datasets Guide
##### case class Person(name: String, age: Long)
##### val caseClassDS = Seq(Person("xxx", 00)).toDS()
##### caseClassDS.show()
##### val primitiveDS = Seq(1, 2, 3).toDS()
##### primitiveDS.map(_ + 1).collect()

##### val path = "/usr/data/people.json"
##### val peopleDS = spark.read.json(path).as[Person]

##### peopleDS.show()
DataFrame 和 Datasets 对比

![](https://img-blog.csdn.net/2018040921582887)


【来自@若泽大数据】


