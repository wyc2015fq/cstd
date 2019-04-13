
# Spark-SQL之DataFrame操作 - anlcy - 博客园






# [Spark-SQL之DataFrame操作](https://www.cnblogs.com/camilla/p/8315746.html)
[Spark](http://lib.csdn.net/base/spark)SQL中的DataFrame类似于一张关系型数据表。在关系型[数据库](http://lib.csdn.net/base/mysql)中对单表或进行的查询操作，在DataFrame中都可以通过调用其API接口来实现。可以参考，[Scala](http://lib.csdn.net/base/scala)提供的[DataFrame API](http://spark.apache.org/docs/1.6.2/api/scala/index.html#org.apache.spark.sql.DataFrame)。
本文中的代码基于Spark-1.6.2的文档实现。
# 一、DataFrame对象的生成
Spark-SQL可以以其他RDD对象、parquet文件、json文件、[Hive](http://lib.csdn.net/base/hive)表，以及通过JDBC连接到其他关系型数据库作为数据源来生成DataFrame对象。本文将以[MySQL](http://lib.csdn.net/base/mysql)数据库为数据源，生成DataFrame对象后进行相关的DataFame之上的操作。
文中生成DataFrame的代码如下：
```python
object DataFrameOperations {
  def main (args: Array[String ]) {
    val sparkConf = new SparkConf().setAppName(
```
```python
"Spark SQL DataFrame Operations").setMaster(
```
```python
"local[2]" )
    val sparkContext = new SparkContext(sparkConf)
    val sqlContext = new SQLContext(sparkContext)
    val url =
```
```python
"jdbc:mysql://m000:3306/test"
    val jdbcDF = sqlContext.read.format(
```
```python
"jdbc" ).options(
      Map(
```
```python
"url" -> url,
```
```python
"user" ->
```
```python
"root",
```
```python
"password" ->
```
```python
"root",
```
```python
"dbtable" ->
```
```python
"spark_sql_test" )).load()
    val joinDF1 = sqlContext.read.format(
```
```python
"jdbc" ).options(
      Map(
```
```python
"url" -> url ,
```
```python
"user" ->
```
```python
"root",
```
```python
"password" ->
```
```python
"root",
```
```python
"dbtable" ->
```
```python
"spark_sql_join1" )).load()
    val joinDF2 = sqlContext.read.format(
```
```python
"jdbc" ).options(
      Map (
```
```python
"url" -> url ,
```
```python
"user" ->
```
```python
"root",
```
```python
"password" ->
```
```python
"root",
```
```python
"dbtable" ->
```
```python
"spark_sql_join2" )).load()
```
```python
...
```
```python
...
  }
}
```

后续代码都在上面`... ...`处。
# 二、DataFrame对象上Action操作
## 1、
## show
## ：展示数据
以表格的形式在输出中展示`jdbcDF`中的数据，类似于`select * from spark_sql_test`的功能。
`show`方法有四种调用方式，分别为，
**（1）****show**
只显示前20条记录。
示例：
```python
jdbcDF.
```
```python
show
```
**（2）****show(numRows: Int)**
显示`numRows`条
示例：
```python
jdbcDF.
```
```python
show(
```
```python
3)
```
**（3）****show(truncate: Boolean)**
是否最多只显示20个字符，默认为`true`。
示例：
```python
jdbcDF.
```
```python
show(
```
```python
true)
```

```python
jdbcDF.
```
```python
show(
```
```python
false)
```
**（4）****show(numRows: Int, truncate: Boolean)**
综合前面的显示记录条数，以及对过长字符串的显示格式。
示例：
```python
jdbcDF.
```
```python
show(
```
```python
3,
```
```python
false)
```

## 2、
## collect
## ：获取所有数据到数组
不同于前面的`show`方法，这里的`collect`方法会将`jdbcDF`中的所有数据都获取到，并返回一个`Array`对象。
```python
jdbcDF
```
```python
.collect()
```

## 3、
## collectAsList
## ：获取所有数据到List
功能和`collect`类似，只不过将返回结构变成了`List`对象，使用方法如下
```python
jdbcDF
```
```python
.collectAsList()
```

## 4、
## describe(cols: String*)
## ：获取指定字段的统计信息
这个方法可以动态的传入一个或多个`String`类型的字段名，结果仍然为`DataFrame`对象，用于统计数值类型字段的统计值，比如`count, mean, stddev, min, max`等。
使用方法如下，其中`c1`字段为字符类型，`c2`字段为整型，`c4`字段为浮点型
```python
jdbcDF
```
```python
.describe(
```
```python
"c1" ,
```
```python
"c2",
```
```python
"c4" )
```
```python
.show()
```

## 5、
## first, head, take, takeAsList
## ：获取若干行记录
这里列出的四个方法比较类似，其中
（1）`first`获取第一行记录
（2）`head`获取第一行记录，`head(n: Int)`获取前n行记录
（3）`take(n: Int)`获取前n行数据
（4）`takeAsList(n: Int)`获取前n行数据，并以`List`的形式展现
以`Row`或者`Array[Row]`的形式返回一行或多行数据。`first`和`head`功能相同。
`take`和`takeAsList`方法会将获得到的数据返回到Driver端，所以，使用这两个方法时需要注意数据量，以免Driver发生`OutOfMemoryError`
使用和结果略。
# 二、DataFrame对象上的条件查询和join等操作
以下返回为DataFrame类型的方法，可以连续调用。
## 1、where条件相关
**（1）****where(conditionExpr: String)****：SQL语言中where关键字后的条件**
传入筛选条件表达式，可以用`and`和`or`。得到DataFrame类型的返回结果，
示例：
```python
jdbcDF
```
```python
.where(
```
```python
"id = 1 or c1 = 'b'" )
```
```python
.show()
```

**（2）****filter****：根据字段进行筛选**
传入筛选条件表达式，得到DataFrame类型的返回结果。和`where`使用条件相同
示例：
```python
jdbcDF
```
```python
.filter(
```
```python
"id = 1 or c1 = 'b'" )
```
```python
.show()
```

## 2、查询指定字段
**（1）****select****：获取指定字段值**
根据传入的`String`类型字段名，获取指定字段的值，以DataFrame类型返回
示例：
```python
jdbcDF.
```
```python
select(
```
```python
"id" ,
```
```python
"c3" ).
```
```python
show(
```
```python
false)
```
还有一个重载的`select`方法，不是传入`String`类型参数，而是传入`Column`类型参数。可以实现`select id, id+1 from test`这种逻辑。
```python
jdbcDF.
```
```python
select(jdbcDF(
```
```python
"id" ), jdbcDF(
```
```python
"id") +
```
```python
1 ).
```
```python
show(
```
```python
false)   能得到
```
```python
Column
```
```python
类型的方法是
```
```python
apply
```
```python
以及
```
```python
col
```
```python
方法，一般用
```
```python
apply
```
```python
方法更简便。
```
**2）****selectExpr****：可以对指定字段进行特殊处理**
可以直接对指定字段调用UDF函数，或者指定别名等。传入`String`类型参数，得到DataFrame对象。
示例，查询`id`字段，`c3`字段取别名`time`，`c4`字段四舍五入：
```python
jdbcDF
```
```python
.selectExpr(
```
```python
"id" ,
```
```python
"c3 as time" ,
```
```python
"round(c4)" )
```
```python
.show(false)
```

**（3）****col****：获取指定字段**
只能获取一个字段，返回对象为Column类型。
val idCol = jdbcDF.col(“id”)果略。
**（4）****apply****：获取指定字段**
只能获取一个字段，返回对象为Column类型
示例：
```python
val idCol1 = jdbcDF.apply(
```
```python
"id")
```
```python
val idCol2 = jdbcDF(
```
```python
"id")
```

**（5）****drop****：去除指定字段，保留其他字段**
返回一个新的DataFrame对象，其中不包含去除的字段，一次只能去除一个字段。
示例：
```python
jdbcDF.
```
```python
drop(
```
```python
"id")
jdbcDF.
```
```python
drop(jdbcDF(
```
```python
"id"))
```

## 3、limit
`limit`方法获取指定DataFrame的前n行记录，得到一个新的DataFrame对象。和`take`与`head`不同的是，`limit`方法不是Action操作。
```python
jdbcDF.limit(3).
```
```python
show(
```
```python
false)
```

## 4、order by
（1）`orderBy`和`sort`：按指定字段排序，默认为升序
示例1，按指定字段排序。加个`-`表示降序排序。`sort`和`orderBy`使用方法相同
```python
jdbcDF
```
```python
.orderBy(- jdbcDF(
```
```python
"c4"))
```
```python
.show(false)
// 或者
jdbcDF
```
```python
.orderBy(jdbcDF(
```
```python
"c4")
```
```python
.desc)
```
```python
.show(false)
```

**（2）****sortWithinPartitions**
和上面的`sort`方法功能类似，区别在于`sortWithinPartitions`方法返回的是按Partition排好序的DataFrame对象。
## 5、group by
（1）`groupBy`：根据字段进行`group by`操作
`groupBy`方法有两种调用方式，可以传入`String`类型的字段名，也可传入`Column`类型的对象。
使用方法如下，
```python
jdbcDF
```
```python
.groupBy(
```
```python
"c1" )
jdbcDF
```
```python
.groupBy( jdbcDF(
```
```python
"c1"))
```

**（2）****cube****和****rollup****：group by的扩展**
功能类似于`SQL`中的`group by cube/rollup`，略。
**（3）GroupedData对象**
该方法得到的是`GroupedData`类型对象，在`GroupedData`的API中提供了`group by`之后的操作，比如，
max(colNames: String*)方法，获取分组中指定字段或者所有的数字类型字段的最大值，只能作用于数字型字段
min(colNames: String*)方法，获取分组中指定字段或者所有的数字类型字段的最小值，只能作用于数字型字段
mean(colNames: String*)方法，获取分组中指定字段或者所有的数字类型字段的平均值，只能作用于数字型字段
sum(colNames: String*)方法，获取分组中指定字段或者所有的数字类型字段的和值，只能作用于数字型字段
count()方法，获取分组中的元素个数
这里面比较复杂的是以下两个方法，agg，pivot该方法和下面介绍的类似，可以用于对指定字段进行聚合

## 6、distinct
**（1）****distinct****：返回一个不包含重复记录的DataFrame**
返回当前DataFrame中不重复的Row记录。该方法和接下来的`dropDuplicates()`方法不传入指定字段时的结果相同。
示例：
```python
jdbcDF.
```
```python
distinct()
```
**（2）****dropDuplicates****：根据指定字段去重**
根据指定字段去重。类似于`select distinct a, b`操作
示例：
```python
jdbcDF.dropDuplicates(Se
```
```python
q("c1"))
```
## 7、聚合
聚合操作调用的是`agg`方法，该方法有多种调用方式。一般与`groupBy`方法配合使用。
以下示例其中最简单直观的一种用法，对`id`字段求最大值，对`c4`字段求和。
```python
jdbcDF.agg(
```
```python
"id" ->
```
```python
"max",
```
```python
"c4" ->
```
```python
"sum")
```
## 8、union
`unionAll`方法：对两个DataFrame进行组合
类似于`SQL`中的`UNION ALL`操作。
示例：
```python
jdbcDF
```
```python
.unionALL(jdbcDF
```
```python
.limit(
```
```python
1))
```

## 9、join
重点来了。在`SQL`语言中用得很多的就是`join`操作，DataFrame中同样也提供了`join`的功能。
接下来隆重介绍`join`方法。在DataFrame中提供了六个重载的`join`方法。
**（1）、笛卡尔积**
```python
joinDF1.
```
```python
join(joinDF2)
```

**（2）、****using****一个字段形式**
下面这种join类似于`a join b using column1`的形式，需要两个DataFrame中有相同的一个列名，
```python
joinDF1.
```
```python
join(joinDF2,
```
```python
"id")
```

**（3）、****using****多个字段形式**
除了上面这种`using`一个字段的情况外，还可以`using`多个字段，如下
```python
joinDF1.
```
```python
join(joinDF2, Se
```
```python
q("id", "name")）
```

**（4）、指定****join****类型**
两个DataFrame的`join`操作有`inner, outer, left_outer, right_outer, leftsemi`类型。在上面的`using`多个字段的join情况下，可以写第三个`String`类型参数，指定`join`的类型，如下所示
```python
joinDF1.
```
```python
join(joinDF2, Se
```
```python
q("id", "name"),
```
```python
"inner"）
```

**（5）、使用****Column****类型来****join**
如果不用`using`模式，灵活指定`join`字段的话，可以使用如下形式
```python
joinDF1.
```
```python
join(joinDF2 , joinDF1(
```
```python
"id" ) === joinDF2(
```
```python
"t1_id"))
```

**（6）、在指定****join****字段同时指定****join****类型**
如下所示
```python
joinDF1.
```
```python
join(joinDF2 , joinDF1(
```
```python
"id" ) === joinDF2(
```
```python
"t1_id"),
```
```python
"inner")
```

## 10、获取指定字段统计信息
`stat`方法可以用于计算指定字段或指定字段之间的统计信息，比如方差，协方差等。这个方法返回一个`DataFramesStatFunctions`类型对象。
下面代码演示根据`c4`字段，统计该字段值出现频率在`30%`以上的内容。在`jdbcDF`中字段`c1`的内容为`"a, b, a, c, d, b"`。其中`a`和`b`出现的频率为`2 / 6`，大于`0.3`
```python
jdbcDF.
```
```python
stat.freqItems(Se
```
```python
q ("c1") ,
```
```python
0.
```
```python
3).show()
```

## 11、获取两个DataFrame中共有的记录
`intersect`方法可以计算出两个DataFrame中相同的记录，
```python
jdbcDF
```
```python
.intersect(jdbcDF
```
```python
.limit(
```
```python
1))
```
```python
.show(false)
```

## 2、获取一个DataFrame中有另一个DataFrame中没有的记录
示例：
```python
jdbcDF
```
```python
.except(jdbcDF
```
```python
.limit(
```
```python
1))
```
```python
.show(false)
```

## 13、操作字段名
**（1）****withColumnRenamed****：重命名DataFrame中的指定字段名**
如果指定的字段名不存在，不进行任何操作。下面示例中将`jdbcDF`中的`id`字段重命名为`idx`。
```python
jdbcDF
```
```python
.withColumnRenamed(
```
```python
"id" ,
```
```python
"idx" )
```

**（2）****withColumn****：往当前DataFrame中新增一列**
`whtiColumn(colName: String , col: Column)`方法根据指定`colName`往DataFrame中新增一列，如果`colName`已存在，则会覆盖当前列。
以下代码往`jdbcDF`中新增一个名为`id2`的列，
```python
jdbcDF
```
```python
.withColumn(
```
```python
"id2", jdbcDF(
```
```python
"id"))
```
```python
.show( false)
```

## 14、行转列
有时候需要根据某个字段内容进行分割，然后生成多行，这时可以使用`explode`方法
下面代码中，根据`c3`字段中的空格将字段内容进行分割，分割的内容存储在新的字段`c3_`中，如下所示
```python
jdbcDF.explode(
```
```python
"c3" ,
```
```python
"c3_" ){
```
```python
time:
```
```python
String =>
```
```python
time.
```
```python
split(
```
```python
" " )}
```

```python
博客来源：https://www.cnblogs.com/rxingyue/p/7113235.html
```






