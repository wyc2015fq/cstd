# Spark 下操作 HBase（1.0.0 新 API） - Spark高级玩法 - CSDN博客
2018年07月11日 00:09:55[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：195
hbase1.0.0版本提供了一些让人激动的功能，并且，在不牺牲稳定性的前提下，引入了新的API。虽然 1.0.0 兼容旧版本的 API，不过还是应该尽早地来熟悉下新版API。并且了解下如何与当下正红的 Spark 结合，进行数据的写入与读取。鉴于国内外有关 HBase 1.0.0 新 API 的资料甚少，故作此文。
本文将分两部分介绍，第一部分讲解使用 HBase 新版 API 进行 CRUD 基本操作；第二部分讲解如何将 Spark 内的 RDDs 写入 HBase 的表中，反之，HBase 中的表又是如何以 RDDs 形式加载进 Spark 内的。
## 环境配置
为了避免版本不一致带来不必要的麻烦，API 和 HBase环境都是 1.0.0 版本。HBase 为单机模式，分布式模式的使用方法类似，只需要修改`HBaseConfiguration`的配置即可。
开发环境中使用 SBT 加载依赖项
name := "SparkLearn"
version := "1.0"
scalaVersion := "2.10.4"
libraryDependencies += "org.apache.spark" %% "spark-core" % "1.3.0"
libraryDependencies += "org.apache.hbase" % "hbase-client" % "1.0.0"
libraryDependencies += "org.apache.hbase" % "hbase-common" % "1.0.0"
libraryDependencies += "org.apache.hbase" % "hbase-server" % "1.0.0"
## HBase 的 CRUD 操作
新版 API 中加入了 `Connection`，`HAdmin`成了`Admin`，`HTable`成了`Table`，而`Admin`和`Table`只能通过`Connection`获得。`Connection`的创建是个重量级的操作，由于`Connection`是线程安全的，所以推荐使用单例，其工厂方法需要一个`HBaseConfiguration`。
val conf = HBaseConfiguration.create()
conf.set("hbase.zookeeper.property.clientPort", "2181")
conf.set("hbase.zookeeper.quorum", "master")
//Connection 的创建是个重量级的工作，线程安全，是操作hbase的入口
val conn = ConnectionFactory.createConnection(conf)
### 创建表
使用`Admin`创建和删除表
val userTable = TableName.valueOf("user")
//创建 user 表
val tableDescr = new HTableDescriptor(userTable)
tableDescr.addFamily(new HColumnDescriptor("basic".getBytes))
println("Creating table `user`. ")
if (admin.tableExists(userTable)) {
admin.disableTable(userTable)
admin.deleteTable(userTable)
}
admin.createTable(tableDescr)
println("Done!")
### 插入、查询、扫描、删除操作
HBase 上的操作都需要先创建一个操作对象`Put`,`Get`,`Delete`等，然后调用`Table`上的相对应的方法
try{
//获取 user 表
val table = conn.getTable(userTable)
try{
//准备插入一条 key 为 id001 的数据
val p = new Put("id001".getBytes)
//为put操作指定 column 和 value （以前的 put.add 方法被弃用了）
p.addColumn("basic".getBytes,"name".getBytes, "wuchong".getBytes)
//提交
table.put(p)
//查询某条数据
val g = new Get("id001".getBytes)
val result = table.get(g)
val value = Bytes.toString(result.getValue("basic".getBytes,"name".getBytes))
println("GET id001 :"+value)
//扫描数据
val s = new Scan()
s.addColumn("basic".getBytes,"name".getBytes)
val scanner = table.getScanner(s)
try{
for(r <- scanner){
println("Found row: "+r)
println("Found value: "+Bytes.toString(
r.getValue("basic".getBytes,"name".getBytes)))
}
}finally {
//确保scanner关闭
scanner.close()
}
//删除某条数据,操作方式与 Put 类似
val d = new Delete("id001".getBytes)
d.addColumn("basic".getBytes,"name".getBytes)
table.delete(d)
}finally {
if(table != null) table.close()
}
}finally {
conn.close()
}
## Spark 操作 HBase
### 写入 HBase
首先要向 HBase 写入数据，我们需要用到`PairRDDFunctions.saveAsHadoopDataset`。因为 HBase 不是一个文件系统，所以`saveAsHadoopFile`方法没用。
> 
`def saveAsHadoopDataset(conf: JobConf): Unit`
Output the RDD to any Hadoop-supported storage system, using a Hadoop JobConf object for that storage system
这个方法需要一个 JobConf 作为参数，类似于一个配置项，主要需要指定输出的格式和输出的表名。
**Step 1：**我们需要先创建一个 JobConf。
//定义 HBase 的配置
val conf = HBaseConfiguration.create()
conf.set("hbase.zookeeper.property.clientPort", "2181")
conf.set("hbase.zookeeper.quorum", "master")
//指定输出格式和输出表名
val jobConf = new JobConf(conf,this.getClass)
jobConf.setOutputFormat(classOf[TableOutputFormat])
jobConf.set(TableOutputFormat.OUTPUT_TABLE,"user")
**Step 2：** RDD 到表模式的映射
在 HBase 中的表 schema 一般是这样的：
`row     cf:col_1    cf:col_2`
而在Spark中，我们操作的是RDD元组，比如`(1,"lilei",14)`, `(2,"hanmei",18)`。我们需要将 `RDD[(uid:Int, name:String, age:Int)]` 转换成 `RDD[(ImmutableBytesWritable, Put)]`。所以，我们定义一个 convert 函数做这个转换工作
def convert(triple: (Int, String, Int)) = {
val p = new Put(Bytes.toBytes(triple._1))
p.addColumn(Bytes.toBytes("basic"),Bytes.toBytes("name"),Bytes.toBytes(triple._2))
p.addColumn(Bytes.toBytes("basic"),Bytes.toBytes("age"),Bytes.toBytes(triple._3))
(new ImmutableBytesWritable, p)
}
**Step 3：** 读取RDD并转换
//read RDD data from somewhere and convert
val rawData = List((1,"lilei",14), (2,"hanmei",18), (3,"someone",38))
val localData = sc.parallelize(rawData).map(convert)
**Step 4：** 使用`saveAsHadoopDataset`方法写入HBase
localData.saveAsHadoopDataset(jobConf)
读取 HBase
Spark读取HBase，我们主要使用`SparkContext` 提供的`newAPIHadoopRDD`API将表的内容以 RDDs 的形式加载到 Spark 中。
val conf = HBaseConfiguration.create()
conf.set("hbase.zookeeper.property.clientPort", "2181")
conf.set("hbase.zookeeper.quorum", "master")
//设置查询的表名
conf.set(TableInputFormat.INPUT_TABLE, "user")
val usersRDD = sc.newAPIHadoopRDD(conf, classOf[TableInputFormat],
classOf[org.apache.hadoop.hbase.io.ImmutableBytesWritable],
classOf[org.apache.hadoop.hbase.client.Result])
val count = usersRDD.count()
println("Users RDD Count:" + count)
usersRDD.cache()
//遍历输出
usersRDD.foreach{ case (_,result) =>
val key = Bytes.toInt(result.getRow)
val name = Bytes.toString(result.getValue("basic".getBytes,"name".getBytes))
val age = Bytes.toInt(result.getValue("basic".getBytes,"age".getBytes))
println("Row key:"+key+" Name:"+name+" Age:"+age)
}
## 附
完整的demo，可以关注公众号输入hbase获取。
