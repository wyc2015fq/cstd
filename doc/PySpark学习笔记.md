# PySpark学习笔记

 

在spark 2.0中, HiveContext, SQLContext, StreamingContext, SparkContext 都被聚合到了spark模块中。另外要注意的一个事情是，读取文件时只能有一个活动进程，否则会报错。点击[这里](http://spark.apache.org/docs/latest/api/python/)可以查看官方文档。

## 1. dataframe

### 1.1 创建dataframe

**spark.createDataFrame(data, schema=None, samplingRatio=None)，直接创建**
 其中data是行或元组或列表或字典的RDD、list、pandas.DataFrame。

```python
df = spark.createDataFrame([
        (1, 144.5, 5.9, 33, 'M'),
        (2, 167.2, 5.4, 45, 'M'),
        (3, 124.1, 5.2, 23, 'F'),
        (4, 144.5, 5.9, 33, 'M'),
        (5, 133.2, 5.7, 54, 'F'),
        (3, 124.1, 5.2, 23, 'F'),
        (5, 129.2, 5.3, 42, 'M'),
    ], ['id', 'weight', 'height', 'age', 'gender']) #直接创建Dataframe

df = spark.createDataFrame([{'name':'Alice','age':1},
	{'name':'Polo','age':1}]) #从字典创建

schema = StructType([
    StructField("id", LongType(), True),    
    StructField("name", StringType(), True),
    StructField("age", LongType(), True),
    StructField("eyeColor", StringType(), True)
])
df = spark.createDataFrame(csvRDD, schema) #指定schema。
```

**spark.read，从文件中读取**

```
>>> airports = spark.read.csv(airportsFilePath, header='true', inferSchema='true', sep='\t')
>>> rdd = sc.textFile('python/test_support/sql/ages.csv') #可以用这种方法将用逗号分隔的rdd转为dataframe
>>> df2 = spark.read.csv(rdd)
>>> df = spark.read.format('json').load('python/test_support/sql/people.json') 
>>> df1 = spark.read.json('python/test_support/sql/people.json')
>>> df1.dtypes
[('age', 'bigint'), ('name', 'string')]
>>> rdd = sc.textFile('python/test_support/sql/people.json')
>>> df2 = spark.read.json(rdd) 
>>> df = spark.read.text('python/test_support/sql/text-test.txt')
>>> df.collect()
[Row(value='hello'), Row(value='this')]
>>> df = spark.read.text('python/test_support/sql/text-test.txt', wholetext=True)
>>> df.collect()
[Row(value='hello\nthis')]

```

**DataFrameWriter.save、DataFrameWriter.parquet、DataFrameWriter.saveAsTable**

### 1.2 dataframe处理

**collect、show、first、head(n)，显示数据**
 **describe，计算统计信息，包括计数，平均，标准差，最小和最大。如果没有指定任何列，这个函数计算统计所有数值列。**
 **count、cov、crosstab、freqItems(cols, support=None)、groupby，计数、计算方差、计算分组频数表、计算重复项、分组**
 **alias，复制成一个新名字的dataframe**
 **select、filter、where、limit，筛选过滤**
 **sort、orderBy，排序**
 **replace，替换**
 **join(other, on=None, how=None)，联合**
 **agg，是groupBy.agg的缩写，聚合函数包括avg, max, min, sum, count。里面可以是一个列:函数字典，也可以是函数(列)**
 **union、unionAll、unionByName，row合并**
 **contains、getItem、isin、like、asDict(recursive=False)、na.drop = dropna**

```python
>>> df = spark.createDataFrame([([1, 2], {"key": "value"})], ["l", "d"])
>>> df.select(df.l.getItem(0), df.d.getItem("key")).show()
+----+------+
|l[0]|d[key]|
+----+------+
|   1| value|
+----+------+
>>> Person = Row("name", "age")
>>> Person
<Row(name, age)>
>>> 'name' in Person
True
>>> 'wrong_key' in Person
False
>>> Person("Alice", 11)
Row(name='Alice', age=11)

df.select("id", "age").filter("age = 22") # 两者是等价的，注意等号
df.select(df.id, df.age).filter(df.age == 22)
df1 = df.alias("df1")
df2 = df.alias("df2")
joined_df = df1.join(df2, col("df1.id") == col("df2.id"))
df.crosstab("id","age").show()
df.groupby(df.id).avg().collect()
df.groupby('id').agg({'age':'mean'}).collect()
df.agg({"age":"max"}).show()
from pyspark.sql import functions as F #第二种方法，使用spark function
df.agg(F.min(df.age)).show()
df.sort(df.age.desc()).collect()#下面三种排序方法等价
df.sort("age", ascending=False).collect()
df.orderBy(df.age.desc()).collect()
df.join(df2, ['id','age'], 'outer').select('name', 'height').collect()
cond = [df.name == df3.name, df.age == df3.age]
df.join(df3, cond).select(df.name, df3.age).collect()
df.summary().show()
+-------+------------------+-----+
|summary|               age| name|
+-------+------------------+-----+
|  count|                 2|    2|
|   mean|               3.5| null|
| stddev|2.1213203435596424| null|
|    min|                 2|Alice|
|    25%|                 2| null|
|    50%|                 2| null|
|    75%|                 5| null|
|    max|                 5|  Bob|
+-------+------------------+-----+

>>> df1 = spark.createDataFrame([[1, 2, 3]], ["col0", "col1", "col2"])
>>> df2 = spark.createDataFrame([[4, 5, 6]], ["col1", "col2", "col0"])
>>> df1.unionByName(df2).show()
+----+----+----+
|col0|col1|col2|
+----+----+----+
|   1|   2|   3|
|   6|   4|   5|
+----+----+----+
```

**columns、drop、withColumn(colName, col)、withColumnRenamed(existing, new)，获取列、删除列、新增列、重命名列**
 **row、StructType、StructFiled**
 **distinct，去重**
 **dropDuplicates(subset = df)，去重**
 **dropna(how=‘any’, thresh=None,  subset=None)，去除空值。how：‘any’或者’all’。如果’any’,删除包含任何空值的行。如果’all’,删除所有值为null的行；thresh默认为None,如果指定这个值，删除小于阈值的非空值的行；subset：选择的列名称列表。**
 **toJSON（转为RDD）、toPandas（Pandas dataframe会全部存放在driver节点的内存中）、toDF、toLocalIterator**
 **fillna，填充空值**

```python
df = df.dropDuplicates(subset=[c for c in df.columns if c != 'id']) 
df_miss_no_income.dropna(thresh=3).show() #去除null值超过一定数量的记录
df_miss_no_income.fillna(means).show() #可以用一个固定值或者一个字典来指定null的填充值
```

**cache、persist/clearCache、unpersist，缓存和清空缓存**。cache只有一个默认的缓存级别MEMORY_ONLY ，而persist可以根据情况设置其它的缓存级别。

### 1.3 Spark function

**foreach(f)，应用f函数，将df的每一行作为f函数的输入**
 **apply(udf)**

```python
def f(person):
     print(person.name)
df.foreach(f)
123
>>> from pyspark.sql.functions import pandas_udf, PandasUDFType
>>> df = spark.createDataFrame(
...     [(1, 1.0), (1, 2.0), (2, 3.0), (2, 5.0), (2, 10.0)],
...     ("id", "v"))
>>> :pandas_udf("id long, v double", PandasUDFType.GROUPED_MAP)  # doctest: +SKIP
... def normalize(pdf):
...     v = pdf.v
...     return pdf.assign(v=(v - v.mean()) / v.std())
>>> df.groupby("id").apply(normalize).show()  # doctest: +SKIP
+---+-------------------+
| id|                  v|
+---+-------------------+
|  1|-0.7071067811865475|
|  1| 0.7071067811865475|
|  2|-0.8320502943378437|
|  2|-0.2773500981126146|
|  2| 1.1094003924504583|
+---+-------------------+
```

**spark.sql.functions，内建函数：**
 abs、add_months、.approx_count_distinct(col, rsd=None)、array(*cols)、array_contains(col, value)、avg
 ceil、coalesce（返回第一个非空的值）、col/column、collect_list（将列中的数据聚合成一个list）、collect_set、concat、concat_ws(sep,*cols)、cos、count、countDistinct、current_date、current_timestamp
 date_add(start,  days)、date_format、date_sub（往前）、date_trunc（format包括‘year’, ‘yyyy’, ‘yy’,  ‘month’, ‘mon’, ‘mm’, ‘day’, ‘dd’, ‘hour’, ‘minute’, ‘second’, ‘week’,  ‘quarter’）、datediff、dayofmonth、dayofyear、decode、degree（角度从raians转换为degree）、dense_rank（注意和rank的区别，一个是1223这样的排序下去，一个是1224这样的排序下去）、desc
 explode（将array或者map转换为dataframe）、explode_outer、expr（解析字符串成为命令）
 floor、format_number（小数点位数）、format_string、from_json、from_unixtime(timestamp, format=‘yyyy-MM-dd HH:mm:ss’)

```python
>>> from pyspark.sql import Row
>>> eDF = spark.createDataFrame([Row(a=1, intlist=[1,2,3], mapfield={"a": "b"})])
>>> eDF.select(explode(eDF.intlist).alias("anInt")).collect()
[Row(anInt=1), Row(anInt=2), Row(anInt=3)]
>>> eDF.select(explode(eDF.mapfield).alias("key", "value")).show()
+---+-----+
|key|value|
+---+-----+
|  a|    b|
+---+-----+
>>> df.select(expr("length(name)")).collect()
[Row(length(name)=5), Row(length(name)=3)]
>>> df = spark.createDataFrame([(5, "hello")], ['a', 'b'])
>>> df.select(format_string('%d %s', df.a, df.b).alias('v')).collect()
[Row(v='5 hello')]
123456789101112131415
>>> from pyspark.sql.functions import *
>>> from pyspark.sql.types import *
>>> data = [(1, '''{"a": 1,"b":4}'''),(2, '''{"a": 3,"b":2}''')]
>>> df = spark.createDataFrame(data, ("key", "value"))
>>> df.show()
+---+--------------+
|key|         value|
+---+--------------+
|  1|{"a": 1,"b":4}|
|  2|{"a": 3,"b":2}|
+---+--------------+
>>> schema = StructType([StructField("a", IntegerType()),StructField("b", IntegerType())])
>>> df.select(from_json(df.value, schema).alias("json")).show()
+------+
|  json|
+------+
|[1, 4]|
|[3, 2]|
+------+
>>> data = [(1, '''{"a": {"c":"chen"},"b":4}'''),(2, '''{"a": {"c":"min"},"b":2}''')]
>>> df = spark.createDataFrame(data, ("key", "value"))
>>> df3 = df.select('key',get_json_object('value','$.a.c').alias('c'),get_json_object('value','$.b').cast('integer').alias('b'))
>>> df3.show()
+---+----+---+
|key|   c|  b|
+---+----+---+
|  1|chen|  4|
|  2| min|  2|
+---+----+---+
```

### 1.4 Spark SQL

如果嫌spark function麻烦，可以直接使用SQL语句。使用之前要先将dataframe注册为表：

```python
df.createOrReplaceTempView("df") #使用sparksql时需要先注册为临时表
```

然后使用sql语句对dataframe进行操作：

```python
spark.sql("select id, age from swimmers where age = 22")
```

## 2. RDD

### 2.1 读取、保存数据

● paralleliz(context)：直接输入内容。
 ● textFile(filePath)：每一行一个数据。
 ● wholeTextFile(filePath)：生成<文件名，文件内容>的键值对。
 ● saveAsTextFile(filePath)：保存文件。
 ● coalesce(1)：保存时只存为一个文件。

### 2.2 transform算子

● map(f): 使用f映射，接收rdd，使用f进行映射。f函数可以将一元rdd转化为二元组数据
 ● filter(f): 使用f过滤，接收rdd，使用f返回值进行筛选。注意filter和map的区别。
 ● distinct()：去重
 ● sample():采样
 ● 要添加或删除元素时，用集合操作union/intersection/subtract/cartesian
 ● reduceByKey(f): 二元组按key使用f函数进行合并。
 ● groupByKey(): 二元组按key进行聚合，每个key的不同value聚合成一个list
 ● combineByKey(f):二元组按照key进行聚合
 ● mapValues(f)：二元组的value使用f映射
 ● flatMapValues(f):二元组的value使用f映射，f接收rdd元素返回rdd数组
 ● keys()、values():二元组的key，value
 ● sortByKey()：按照key进行排序
 ● 二元组还可以：substractByKey，删掉包含key的所有二元组；join，内连接，只选出两边key都有的；此外还有leftOutJoin，coGroup等
 ● aggregate(初始值，函数U，函数T)：函数U用来对当前分区上的文件进行聚合，函数T用来对不同分区之间的结果再进行聚合。
 ● aggregateByKey(初始值，函数U，函数T)：和aggregate基本相同，每一个key返回一个aggregate的结果。
 ●  combineByKey(函数C，函数U，函数T)：与aggregateByKey相比，多了一个C函数(第一次遇到某个key时调用，之后每次碰到这个key调用mergeValue函数。不同分区之间相同的key使用U函数进行合并)。combineByKey用于Combiner不能用固定初始值创建的情况。

### 2.3 action算子

● first(): 返回RDD里面的第一个值。
 ● take(n): 从RDD里面取出前n个值。
 ● collect(): 返回全部的RDD元素。
 ● sum(): 求和。
 ● count(): 求个数。
 ● reduce()：对rdd数据进行函数操作
 ● countByKey(): 二元组，返回dict的统计量
 ● lookup(): 返回key对应的所有value（一个list）。
 ● collectAsMap()：返回key最新value的map

## 3. 机器学习库MLib和ML

spark对常见的机器学习算法封装的很好，旧版本的spark使用MLlib包，基于RDD进行操作；新版本则使用ML包直接操作dataframe。

### 3.1 MLlib使用示例

这里用垃圾邮件分类的例子简单说明一下MLlib的使用方法。原始数据一般存放在Vector类中，带标签/评分的数据存放在LabeledPoint/Rating中，它们都是RDD，通过MLlib的model进行训练和预测。

读入文件：

```python
spam = sc.textFile("spam.txt") #垃圾邮件清单
normal = sc.textFile("normal.txt") #正常邮件清单
```

构建词频向量：

```python
from pyspark.mllib.feature import HashingTF
tf = HashingTF(numFeatures = 100)
spamFeatures = spam.map(lambda x:tf.transform(x.split(" ")))
normalFeatures = normal.map(lambda x:tf.transform(x.split(" ")))
```

我们可以看看词频向量是什么：

```python
>>> spamFeatures.collect()
[SparseVector(100, {36: 1.0, 40: 1.0, 49: 1.0}), SparseVector(100, {36: 1.0, 42: 1.0, 63: 1.0}), SparseVector(100, {26: 1.0, 46: 1.0, 65: 1.0})]
```

大致就是说生成了一个100维的稀疏向量，每个单词落在一个维度里，用key进行记录；然后value保存这个维度的单词出现的次数。
 使用逻辑回归模型，为每一条数据打标签，并生成训练数据：

```python
from pyspark.mllib.regression import LabeledPoint
p = spamFeatures.map(lambda x:LabeledPoint(1,x))
n = normalFeatures.map(lambda x:LabeledPoint(0,x))
trainData = p.union(n)
trainData.cache()
```

用随机梯度下降进行训练

```python
from pyspark.mllib.classification import LogisticRegressionWithSGD
model = LogisticRegressionWithSGD.train(trainData)  
```

然后进行一下测试：

```python
>>> pt = tf.transform("Get cheap stuff by sending money to me".split(" "))
>>> nt = tf.transform("Spark spark spark".split(" "))
>>> model.predict(pt)
1
>>> model.predict(nt)                       
0
```

### 3.2 ML使用示例

ML包有3个抽象类非常重要：Transformer、Estimator和Pipeline。可以参见 <https://blog.csdn.net/FlySky1991/article/details/79671106>

- Transformer（转换器）：里面有很多工具类，可以将dataframe的数据处理后变为新的一列添加到原始dataframe上。在spark.ml.feature中有很多转换器，常见的有：Binarizer、Bucketizer、HashingTF，IDF，IndexToString、MaxAbsScaler、MinMaxScaler、NGram、OneHotEncoder、StopWordsRemover、Tokenizer、**VectorAssembler**、Word2Vec……
- Estimator（评估器）：评估器是进行机器学习训练的地方。ML包目前有7种分类评估器，7种回归评估器，4种聚类评估器，1个推荐评估器。
- Pipeline（管道）：PySpark ML中的管道是指从转换到评估的端到端的过程，这个过程可以对输入的DataFrame原始数据执行必要的数据转换操作，最后评估统计模型。

下面是一个简单示例：

```python
from pyspark.ml import feature,clustering,Pipeline
data = spark.createDataFrame([[5],[1],[4],[17],[18],[1]],'name int')
featuresCreator = feature.VectorAssembler(
    inputCols=['name'], 
    outputCol='features'
)

kmeans = clustering.KMeans(k = 3, 
    featuresCol='features')

pipeline = Pipeline(stages=[
        featuresCreator, 
        kmeans]
)

model = pipeline.fit(data)
result = model.transform(data)
result.show()
```

输出为：

```python
+----+--------+----------+
|name|features|prediction|
+----+--------+----------+
|   5|   [5.0]|         0|
|   1|   [1.0]|         2|
|   4|   [4.0]|         0|
|  17|  [17.0]|         1|
|  18|  [18.0]|         1|
|   1|   [1.0]|         2|
+----+--------+----------+
12345678910
```

## 4. 自定义函数UDF

由于目前的pyspark不支持dataset，因此只能用dataframe+udf或者rdd的方式来对列进行数据处理。这里介绍一下udf，udf注册有两种方法，一种是调用register方法，一种是调用udf函数。两者都能在withColumn和apply中使用。两种方法的区别是：udf注册后可以使用dataframe的spark  function api，而register注册后可以使用spark sql。

### 4.1 Spark DF 和 Pandas DF

首先要区分spark dataframe和pandas dataframe。pandas df会将所有数据存储在driver node上，一定要慎用。
 spark df 与 pandas df 相互转化性能优化，需要开启配置，默认为关闭。

```python
spark.sql.execution.arrow.enabled true
import numpy as np
import pandas as pd

//初始化 pandas DF
pdf = pd.DataFrame(np.random.rand(100000, 3))
// pdf -> sdf
%time df = spark.createDataFrame(pdf)
// sdf -> pdf
%time result_pdf = df.select("*").toPandas()
```

### 4.2 Spark UDF

UDF是用户自定义函数的缩写。下面展示一下几种UDF使用方法：

```python
from pyspark.sql.functions import udf
df = spark.createDataFrame([(1, 1.0), (1, 2.0), (2, 3.0), (2, 5.0), (2, 10.0)],("id", "v"))

# 使用register注册的方法
def f1(v):
    return v+1
f1_udf = spark.udf.register('f1',f1)
df.withColumn('v2',f1_udf(df.v)).show() 

# 使用udf回调的方法
@udf('double') #这里也可以简单的使用@udf
def f2(v):
    return v+1
df.withColumn('v2',f2(df.v)).show()

# 使用udf函数注册
def f3(v):
    return v+1
f3_udf = udf(f3,'double')
df.withColumn('v2',f3_udf(df.v)).show()

# 使用register注册的可以使用spark sql
df.createOrReplaceTempView("df")
spark.sql('select v,f1(v) v2 from df').show()

# 使用udf注册的可以使用dataframe api
df.select(['v',f2('v').alias('v2')]).show()
```

### 4.3 Pandas UDF

Pandas UDF构建在 Apache Arrow 之上。Apache Arrow是一个跨平台的在内存中以列式存储的数据层，减少了大量java和python之间序列化和反序列化的工作。

```python
from pyspark.sql.functions import pandas_udf, PandasUDFType
df = spark.createDataFrame([(1, 1.0), (1, 2.0), (2, 3.0), (2, 5.0), (2, 10.0)],("id", "v"))

#使用回调函数@pandas_udf(schema,return type)来注册
@pandas_udf("id long, v double", PandasUDFType.GROUPED_MAP)
def normalize(pdf):
    v = pdf.v
    return pdf.assign(v=(v - v.mean()) / v.std())
df.groupby("id").apply(normalize).show()

#使用pandas_udf函数直接注册
def plus_one(a):
    return a + 1
plus_one_pd_udf = pandas_udf(plus_one, returnType=LongType())
```