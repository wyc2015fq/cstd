# RDD转为Dataset如何指定schema? - Spark高级玩法 - CSDN博客
2018年06月14日 04:54:29[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：866
**与RDD进行互操作**
Spark SQL支持两种不同方法将现有RDD转换为Datasets。第一种方法使用反射来推断包含特定类型对象的RDD的schema。这种基于反射的方法会导致更简洁的代码，并且在编写Spark应用程序时已经知道schema的情况下工作良好。
第二种创建Datasets的方法是通过编程接口，允许您构建schema，然后将其应用于现有的RDD。虽然此方法更详细，但它允许你在直到运行时才知道列及其类型的情况下去构件数据集。
**使用反射推断模式**
Spark SQL的Scala接口支持自动将包含case classes的RDD转换为DataFrame。Case class定义表的schema。使用反射读取case class的参数名称，并将其变为列的名称。Case class也可以嵌套或包含复杂类型，如Seqs或Arrays。此RDD可以隐式转换为DataFrame，然后将其注册为表格。表可以在随后的SQL语句中使用。
// For implicit conversions from RDDs to DataFrames
importspark.implicits._
// Create an RDD of Person objects from a text file, convert it to a Dataframe
val peopleDF = spark.sparkContext
.textFile("examples/src/main/resources/people.txt")
.map(_.split(","))
.map(attributes =>Person(attributes(0), attributes(1).trim.toInt))
.toDF()
// Register the DataFrame as a temporary view
peopleDF.createOrReplaceTempView("people")
// SQL statements can be run by using the sql methods provided by Spark
val teenagersDF = spark.sql("SELECT name, age FROM people WHERE age BETWEEN 13 AND 19")
// The columns of a row in the result can be accessed by field index
teenagersDF.map(teenager =>"Name: "+ teenager(0)).show()
// +------------+
// |       value|
// +------------+
// |Name: Justin|
// +------------+
// or by field name
teenagersDF.map(teenager =>"Name: "+ teenager.getAs[String]("name")).show()
// +------------+
// |       value|
// +------------+
// |Name: Justin|
// +------------+
// No pre-defined encoders for Dataset[Map[K,V]], define explicitly
implicitval mapEncoder = org.apache.spark.sql.Encoders.kryo[Map[String, Any]]
// Primitive types and case classes can be also defined as
// implicit val stringIntMapEncoder: Encoder[Map[String, Any]] = ExpressionEncoder()
// row.getValuesMap[T] retrieves multiple columns at once into a Map[String, T]
teenagersDF.map(teenager => teenager.getValuesMap[Any](List("name","age"))).collect()
// Array(Map("name" -> "Justin", "age" -> 19))
**以编程方式指定模式**
当case class不能提前定义时（例如，记录的结构用字符串编码，或者文本数据集将被解析并且字段对不同的用户值会不同），DataFrame可以以编程方式通过三个步骤创建 。
1， Row从原始RDD 创建元素类型为Row的RDD;
2，使用StructType创建一组schema，然后让其匹配步骤1中Rows的类型结构。
3，使用SparkSession 提供的方法createDataFrame，将schema应用于Rows 类型的RDD。
importorg.apache.spark.sql.types._
// Create an RDD
valpeopleRDD=spark.sparkContext.textFile("examples/src/main/resources/people.txt")
// The schema is encoded in a string
valschemaString="name age"
// Generate the schema based on the string of schema
valfields=schemaString.split(" ")
.map(fieldName=>StructField(fieldName,StringType,nullable=true))
valschema=StructType(fields)
// Convert records of the RDD (people) to Rows
valrowRDD=peopleRDD
.map(_.split(","))
.map(attributes=>Row(attributes(0),attributes(1).trim))
// Apply the schema to the RDD
valpeopleDF=spark.createDataFrame(rowRDD,schema)
// Creates a temporary view using the DataFrame
peopleDF.createOrReplaceTempView("people")
// SQL can be run over a temporary view created using DataFrames
valresults=spark.sql("SELECT name FROM people")
// The results of SQL queries are DataFrames and support all the normal RDD operations
// The columns of a row in the result can be accessed by field index or by field name
results.map(attributes=>"Name: "+attributes(0)).show()
// +-------------+
// |        value|
// +-------------+
// |Name: Michael|
// |   Name: Andy|
// | Name: Justin|
// +-------------+
**推荐阅读：**
1，[从零开始 Spark 性能调优](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484670&idx=1&sn=f1401758c5d177da61bfcddb5fb0e67b&chksm=9f38e7d6a84f6ec0d6cdafe0bcba3353436ac8faef8a397c6cd1cdda2d1bd5e8ce8ec181f12c&scene=21#wechat_redirect)
2，[用java提交一个Spark应用程序](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484324&idx=1&sn=9d68b15a297c965c24ccece28fa6cc00&chksm=9f38e08ca84f699ab7f50d02bdfecc8da9c697c672e5764dd1333ca0cf028712b5d556006435&scene=21#wechat_redirect)
3，[SparkStreaming如何解决小文件问题](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484933&idx=1&sn=67f44cb5528ee8b195ac1fffbca3855e&chksm=9f38e52da84f6c3b0105ef8132028a4bb9ff144c35d3ce5d9444fd94f2d452b728ee3449496e&scene=21#wechat_redirect)
4，[SparkStreaming源码阅读思路](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484929&idx=1&sn=f6bc0a527101d0488dc8422a82718780&chksm=9f38e529a84f6c3fa48bb3127bad052e83e11129f6d466d46f4874371f9b229fd721b714929d&scene=21#wechat_redirect)
环境进入spark技术学院，与业界大牛交流互动。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFU7Oic5ibSSV5sJGB2RgzSeqQpuBicxOPgCBglwQPt9rnatXN0VblKhw9WicqSGsuTU9o0Sj12698QpuQ/640?wx_fmt=jpeg)
