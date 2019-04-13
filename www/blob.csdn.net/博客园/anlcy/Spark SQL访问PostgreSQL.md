
# Spark SQL访问PostgreSQL - anlcy - 博客园






# [Spark SQL访问PostgreSQL](https://www.cnblogs.com/camilla/p/8303550.html)
随着Spark SQL的正式发布，以及它对DataFrame的支持，它可能会取代HIVE成为越来越重要的针对结构型数据进行分析的平台。在博客文章What’s new for Spark SQL in Spark 1.3中，Databricks的工程师Michael Armbrust着重介绍了改进了的Data Source API。
![img](http://7u2mak.com1.z0.glb.clouddn.com/datasource.png)
我们在对结构型数据进行分析时，总不可避免会遭遇多种数据源的情况。这些数据源包括Json、CSV、Parquet、关系型数据库以及NoSQL数据库。我们自然希望能够以统一的接口来访问这些多姿多态的数据源。
在我们产品的应用场景中，需要访问PostgreSQL的数据以进行数据分析。我们可以通过Spark SQL提供的JDBC来访问，前提是需要PostgreSQL的driver。方法是在build.sbt中添加对应版本的driver依赖。例如：
libraryDependencies ++= {
val sparkVersion = "1.3.0"
Seq(
"org.apache.spark" %% "spark-core"  % sparkVersion,
"org.apache.spark" %% "spark-sql"   % sparkVersion,
"org.postgresql"   %  "postgresql"  % "9.4-1201-jdbc41"
)
}
根据Spark SQL的官方文档，在调用Data Sources API时，可以通过SQLContext加载远程数据库为Data Frame或Spark SQL临时表。加载时，可以传入的参数（属性）包括：url、dbtable、driver、partitionColumn、lowerBound、upperBound与numPartitions。
PostgreSQL Driver的类名为org.postgresql.Driver。由于属性没有user和password，因此要将它们作为url的一部分。假设我们要连接的数据库服务器IP为192.168.1.110，端口为5432，用户名和密码均为test，数据库为demo，要查询的数据表为tab_users，则访问PostgreSQL的代码如下所示：
object PostgreSqlApp {
def main(args: Array[String]): Unit = {
val sparkConf = new SparkConf().setAppName("FromPostgreSql").setMaster("local[2]")
val sc = new SparkContext(sparkConf)
val sqlContext = new SQLContext(sc)
val query = "(SELECT * FROM tab_users) as USERS"
val url = "jdbc:postgresql://192.168.1.110:5432/demo?user=test&password=test"
val users = sqlContext.load("jdbc", Map(
"url" -> url,
"driver" -> "org.postgresql.Driver",
"dbtable" -> query
))
users.foreach(println)
}
}
上面的代码将查询语句直接放在query变量中，并传递给SQLContext用以加载。另一种方式是直接传递表名，然后通过调用registerTempTable()方法来注册临时表，并调用sql()方法执行查询：
object PostgreSqlApp {
def main(args: Array[String]): Unit = {
//val sparkConf = new SparkConf().setAppName("FromPostgreSql").setMaster("local[2]")
val sparkConf  =new SparkConf().setAppName("SparkSQL_Select_Table")
.set("spark.driver.allowMultipleContexts","true")
.set("spark.sql.shuffle.partitions","12")
//本地启动
.setMaster("local[2]");
val sc = new SparkContext(sparkConf)
val sqlContext = new SQLContext(sc)
val url = "jdbc:postgresql://192.168.1.110:5432/demo?user=test&password=test"
val dataFrame =
```python
sqlContext.read.format(
```
```python
"jdbc" ).options(
```
*Map(**"url" -> url,**"user" ->**"root",**"password" ->**"root",**"dbtable" ->**"users" )).load()*dataFrame.registerTempTable("USERS")
val users = sqlContext.sql("select * from USERS")
users.foreach(println)
}
}
从性能角度考虑，还可以在创建SQLContext时，设置一些配置项，例如：
val sqlContext = new SQLContext(sc)
sqlContext.setConf("spark.sql.inMemoryColumnarStorage.batchSize", "10000")





