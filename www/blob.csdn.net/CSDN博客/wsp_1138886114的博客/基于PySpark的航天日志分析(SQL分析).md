# 基于PySpark的航天日志分析(SQL分析) - wsp_1138886114的博客 - CSDN博客





2018年09月21日 21:18:50[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：169











### 文章目录
- [1、导入PySpark包](#1PySpark_4)
- [2、创建SparkSession实例对象](#2SparkSession_21)
- [3、读取数据（Schema()信息）](#3Schema_37)
- [读取数据方法1](#1_39)
- [读取数据方法2](#2_76)
- [4、查看DataFrame数据信息（显示完整【列名】不省略）](#4DataFrame_103)
- [6、SparkSQL模块中，结构化数据分析：DSL和SQL(filter)](#6SparkSQLDSLSQLfilter_216)
- [7、分组聚合（groupBy Rename）](#7groupBy_Rename_307)
- [8、可视化展示（SparkSQL中DataFrame转换为Pandas中DataFrame）](#8SparkSQLDataFramePandasDataFrame_378)



基于PySpark的航天日志
——运行环境`jupyter notebook`

### 1、导入PySpark包

```python
```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import time
from pyspark import SparkContext, SparkConf
from pyspark.sql import SparkSession
-----------------------------------------------------------
#设置环境变量
os.environ['JAVA_HOME'] = 'C:/Java/jdk1.8.0_91'
#hadoop安装目录
os.environ['HADOOP_HOME'] = 'C:/Java/hadoop-2.6.0-cdh5.7.6'
#设置spark安装目录
os.environ['SPARK_HOME'] = 'C:/Java/spark-2.2.0-bin-2.6.0-cdh5.7.6'
```
```

### 2、创建SparkSession实例对象

```python
```python
#创建spark实例：local[2]本地模式两个线程。
spark = SparkSession.builder\
    .appName('Python_Spark_test')\
    .master('local[2]')\
    .getOrCreate()
------------------------------------------------------------------
# 读取数据
print type(spark)
print spark
输出：
    <class 'pyspark.sql.session.SparkSession'>
    <pyspark.sql.session.SparkSession object at 0x000000000670BD68>
```
```

### 3、读取数据（Schema()信息）

###### 读取数据方法1

```python
```python
#读取csv格式文件
df = spark.read.csv("flights.csv",header = True)
df.head(5)
输出：
    [Row(year=u'2014', month=u'1', day=u'1', dep_time=u'1', dep_delay=u'96', arr_time=u'235', arr_delay=u'70', carrier=u'AS', tailnum=u'N508AS', flight=u'145', origin=u'PDX', dest=u'ANC', air_time=u'194', distance=u'1542', hour=u'0', minute=u'1'),
     Row(year=u'2014', month=u'1', day=u'1', dep_time=u'4', dep_delay=u'-6', arr_time=u'738', arr_delay=u'-23', carrier=u'US', tailnum=u'N195UW', flight=u'1830', origin=u'SEA', dest=u'CLT', air_time=u'252', distance=u'2279', hour=u'0', minute=u'4'),
     Row(year=u'2014', month=u'1', day=u'1', dep_time=u'8', dep_delay=u'13', arr_time=u'548', arr_delay=u'-4', carrier=u'UA', tailnum=u'N37422', flight=u'1609', origin=u'PDX', dest=u'IAH', air_time=u'201', distance=u'1825', hour=u'0', minute=u'8'),
     Row(year=u'2014', month=u'1', day=u'1', dep_time=u'28', dep_delay=u'-2', arr_time=u'800', arr_delay=u'-23', carrier=u'US', tailnum=u'N547UW', flight=u'466', origin=u'PDX', dest=u'CLT', air_time=u'251', distance=u'2282', hour=u'0', minute=u'28'),
     Row(year=u'2014', month=u'1', day=u'1', dep_time=u'34', dep_delay=u'44', arr_time=u'325', arr_delay=u'43', carrier=u'AS', tailnum=u'N762AS', flight=u'121', origin=u'SEA', dest=u'ANC', air_time=u'201', distance=u'1448', hour=u'0', minute=u'34')]
```
```

```python
```python
# 查看DataFrame的Schema信息
df.printSchema()

输出：
    root
     |-- year: string (nullable = true)
     |-- month: string (nullable = true)
     |-- day: string (nullable = true)
     |-- dep_time: string (nullable = true)
     |-- dep_delay: string (nullable = true)
     |-- arr_time: string (nullable = true)
     |-- arr_delay: string (nullable = true)
     |-- carrier: string (nullable = true)
     |-- tailnum: string (nullable = true)
     |-- flight: string (nullable = true)
     |-- origin: string (nullable = true)
     |-- dest: string (nullable = true)
     |-- air_time: string (nullable = true)
     |-- distance: string (nullable = true)
     |-- hour: string (nullable = true)
     |-- minute: string (nullable = true)
```
```

###### 读取数据方法2

```python
```python
# 指定程序自定推断Schema数据
df2 = spark.read.csv('flights.csv', header=True, inferSchema=True)
df2.printSchema()

输出：
    root
     |-- year: integer (nullable = true)
     |-- month: integer (nullable = true)
     |-- day: integer (nullable = true)
     |-- dep_time: string (nullable = true)
     |-- dep_delay: string (nullable = true)
     |-- arr_time: string (nullable = true)
     |-- arr_delay: string (nullable = true)
     |-- carrier: string (nullable = true)
     |-- tailnum: string (nullable = true)
     |-- flight: integer (nullable = true)
     |-- origin: string (nullable = true)
     |-- dest: string (nullable = true)
     |-- air_time: string (nullable = true)
     |-- distance: integer (nullable = true)
     |-- hour: string (nullable = true)
     |-- minute: string (nullable = true)
```
```

### 4、查看DataFrame数据信息（显示完整【列名】不省略）

```python
```python
df.printSchema()         # 查看Schame信息
输出：
    root
     |-- year: string (nullable = true)
     |-- month: string (nullable = true)
     |-- day: string (nullable = true)
     |-- dep_time: string (nullable = true)
     |-- dep_delay: string (nullable = true)
     |-- arr_time: string (nullable = true)
     |-- arr_delay: string (nullable = true)
     |-- carrier: string (nullable = true)
     |-- tailnum: string (nullable = true)
     |-- flight: string (nullable = true)
     |-- origin: string (nullable = true)
     |-- dest: string (nullable = true)
     |-- air_time: string (nullable = true)
     |-- distance: string (nullable = true)
     |-- hour: string (nullable = true)
     |-- minute: string (nullable = true)
```
```

```python
```python
df.schema
输出： 
StructType(List(StructField(year,StringType,true),StructField(month,StringType,true),
StructField(day,StringType,true),StructField(dep_time,StringType,true),
StructField(dep_delay,StringType,true),StructField(arr_time,StringType,true),
StructField(arr_delay,StringType,true),StructField(carrier,StringType,true),
StructField(tailnum,StringType,true),StructField(flight,StringType,true),
StructField(origin,StringType,true),StructField(dest,StringType,true),
StructField(air_time,StringType,true),StructField(distance,StringType,true),
StructField(hour,StringType,true),StructField(minute,StringType,true)))
```
```

```python
```python
df.show()   # 显示前N条数据, 默认显示前20条数据
    +----+-----+---+--------+---------+--------+---------+-------+-------+------+------+----+--------+--------+----+------+
    |year|month|day|dep_time|dep_delay|arr_time|arr_delay|carrier|tailnum|flight|origin|dest|air_time|distance|hour|minute|
    +----+-----+---+--------+---------+--------+---------+-------+-------+------+------+----+--------+--------+----+------+
    |2014|    1|  1|       1|       96|     235|       70|     AS| N508AS|   145|   PDX| ANC|     194|    1542|   0|     1|
    |2014|    1|  1|       4|       -6|     738|      -23|     US| N195UW|  1830|   SEA| CLT|     252|    2279|   0|     4|
    |2014|    1|  1|       8|       13|     548|       -4|     UA| N37422|  1609|   PDX| IAH|     201|    1825|   0|     8|
    |2014|    1|  1|      28|       -2|     800|      -23|     US| N547UW|   466|   PDX| CLT|     251|    2282|   0|    28|
    |2014|    1|  1|      34|       44|     325|       43|     AS| N762AS|   121|   SEA| ANC|     201|    1448|   0|    34|
    |2014|    1|  1|      37|       82|     747|       88|     DL| N806DN|  1823|   SEA| DTW|     224|    1927|   0|    37|
    |2014|    1|  1|     346|      227|     936|      219|     UA| N14219|  1481|   SEA| ORD|     202|    1721|   3|    46|
    |2014|    1|  1|     526|       -4|    1148|       15|     UA| N813UA|   229|   PDX| IAH|     217|    1825|   5|    26|
    |2014|    1|  1|     527|        7|     917|       24|     UA| N75433|  1576|   SEA| DEN|     136|    1024|   5|    27|
    |2014|    1|  1|     536|        1|    1334|       -6|     UA| N574UA|   478|   SEA| EWR|     268|    2402|   5|    36|
    |2014|    1|  1|     541|        1|     911|        4|     UA| N36476|  1569|   PDX| DEN|     130|     991|   5|    41|
    |2014|    1|  1|     549|       24|     907|       12|     US| N548UW|   649|   PDX| PHX|     122|    1009|   5|    49|
    |2014|    1|  1|     550|        0|     837|      -12|     DL| N660DL|  1634|   SEA| SLC|      82|     689|   5|    50|
    |2014|    1|  1|     557|       -3|    1134|      -16|     AA| N3JLAA|  1094|   SEA| DFW|     184|    1660|   5|    57|
    |2014|    1|  1|     557|       -3|     825|      -25|     AS| N562AS|    81|   SEA| ANC|     188|    1448|   5|    57|
    |2014|    1|  1|     558|       -2|     801|       -2|     AS| N402AS|   200|   SEA| SJC|     100|     697|   5|    58|
    |2014|    1|  1|     559|       -1|     916|       -9|     F9| N210FR|   796|   PDX| DEN|     125|     991|   5|    59|
    |2014|    1|  1|     600|        0|    1151|      -19|     AA| N3JFAA|  2240|   SEA| ORD|     206|    1721|   6|     0|
    |2014|    1|  1|     600|      -10|     842|       -8|     AS| N786AS|   426|   SEA| LAX|     125|     954|   6|     0|
    |2014|    1|  1|     602|       -3|     943|        5|     F9| N201FR|   144|   SEA| DEN|     129|    1024|   6|     2|
    +----+-----+---+--------+---------+--------+---------+-------+-------+------+------+----+--------+--------+----+------+
    only showing top 20 rows
```
```

```python
```python
# 显示前5条数据, 显示每列字段的所有内容，不进行省略
df.show(n=5, truncate=False)
输出：
    +----+-----+---+--------+---------+--------+---------+-------+-------+------+------+----+--------+--------+----+------+
    |year|month|day|dep_time|dep_delay|arr_time|arr_delay|carrier|tailnum|flight|origin|dest|air_time|distance|hour|minute|
    +----+-----+---+--------+---------+--------+---------+-------+-------+------+------+----+--------+--------+----+------+
    |2014|1    |1  |1       |96       |235     |70       |AS     |N508AS |145   |PDX   |ANC |194     |1542    |0   |1     |
    |2014|1    |1  |4       |-6       |738     |-23      |US     |N195UW |1830  |SEA   |CLT |252     |2279    |0   |4     |
    |2014|1    |1  |8       |13       |548     |-4       |UA     |N37422 |1609  |PDX   |IAH |201     |1825    |0   |8     |
    |2014|1    |1  |28      |-2       |800     |-23      |US     |N547UW |466   |PDX   |CLT |251     |2282    |0   |28    |
    |2014|1    |1  |34      |44       |325     |43       |AS     |N762AS |121   |SEA   |ANC |201     |1448    |0   |34    |
    +----+-----+---+--------+---------+--------+---------+-------+-------+------+------+----+--------+--------+----+------+
    only showing top 5 rows
```
```

```python
```python
df.columns    # 获取所有列的名称
输出：
    ['year',
     'month',
     'day',
     'dep_time',
     'dep_delay',
     'arr_time',
     'arr_delay',
     'carrier',
     'tailnum',
     'flight',
     'origin',
     'dest',
     'air_time',
     'distance',
     'hour',
     'minute']
```
```

```python
```python
df.count()  # 统计条目数
输出：
    52535
----------------------------------------------------------------------------
df.cache()  # 缓存数据

"""
对RDD数据缓存到内存中，DataFrame = RDD + schema（字段类型、字段名称）也可以缓存到内存中，
但是cache()是一个Lazy 懒惰性操作，必须RDD/DataFrame使用Action函数触发
"""
print df.count()
输出：
	52535
```
```

### 6、SparkSQL模块中，结构化数据分析：DSL和SQL(filter)

```python
```python
# 注册DataFrame为视图
df.createOrReplaceTempView("view_tmp_flights")

# 编写并执行SQL语句，95%类似于MySQL中SQL
sql_str = "SELECT dest, arr_delay FROM view_tmp_flights"
dest_df = spark.sql(sql_str)

# 查看分析结果
dest_df.show(n=10)
输出：
    +----+---------+
    |dest|arr_delay|
    +----+---------+
    | ANC|       70|
    | CLT|      -23|
    | IAH|       -4|
    | CLT|      -23|
    | ANC|       43|
    | DTW|       88|
    | ORD|      219|
    | IAH|       15|
    | DEN|       24|
    | EWR|       -6|
    +----+---------+
    only showing top 10 rows
```
```

```python
```python
# 上述语句可以使用DSL分析，类似Pandas分析
df.select(df['dest'], df['arr_delay']).show(n=10)
输出：
    +----+---------+
    |dest|arr_delay|
    +----+---------+
    | ANC|       70|
    | CLT|      -23|
    | IAH|       -4|
    | CLT|      -23|
    | ANC|       43|
    | DTW|       88|
    | ORD|      219|
    | IAH|       15|
    | DEN|       24|
    | EWR|       -6|
    +----+---------+
    only showing top 10 rows
```
```

```python
```python
# 过滤
help(df.filter)
>>> df.filter(df.age > 3).collect()
[Row(age=5, name=u'Bob')]
>>> df.where(df.age == 2).collect()
[Row(age=2, name=u'Alice')]
```
```

```python
```python
jfk_df = df.filter(df['dest'] == 'JFK')
jfk_df.show(n=3)

print jfk_df.count()
输出：
    +----+-----+---+--------+---------+--------+---------+-------+-------+------+------+----+--------+--------+----+------+
    |year|month|day|dep_time|dep_delay|arr_time|arr_delay|carrier|tailnum|flight|origin|dest|air_time|distance|hour|minute|
    +----+-----+---+--------+---------+--------+---------+-------+-------+------+------+----+--------+--------+----+------+
    |2014|    1|  1|     654|       -6|    1455|      -10|     DL| N686DA|   418|   SEA| JFK|     273|    2422|   6|    54|
    |2014|    1|  1|     708|       -7|    1510|      -19|     AA| N3DNAA|   236|   SEA| JFK|     281|    2422|   7|     8|
    |2014|    1|  1|     708|       -2|    1453|      -20|     DL| N3772H|  2258|   PDX| JFK|     267|    2454|   7|     8|
    +----+-----+---+--------+---------+--------+---------+-------+-------+------+------+----+--------+--------+----+------+
    only showing top 3 rows
    
    932
```
```

```python
```python
# 上述可以使使用如下SQL表示
spark.sql("SELECT * FROM view_tmp_flights WHERE dest = 'JFK'").show(n=3)
输出：
    +----+-----+---+--------+---------+--------+---------+-------+-------+------+------+----+--------+--------+----+------+
    |year|month|day|dep_time|dep_delay|arr_time|arr_delay|carrier|tailnum|flight|origin|dest|air_time|distance|hour|minute|
    +----+-----+---+--------+---------+--------+---------+-------+-------+------+------+----+--------+--------+----+------+
    |2014|    1|  1|     654|       -6|    1455|      -10|     DL| N686DA|   418|   SEA| JFK|     273|    2422|   6|    54|
    |2014|    1|  1|     708|       -7|    1510|      -19|     AA| N3DNAA|   236|   SEA| JFK|     281|    2422|   7|     8|
    |2014|    1|  1|     708|       -2|    1453|      -20|     DL| N3772H|  2258|   PDX| JFK|     267|    2454|   7|     8|
    +----+-----+---+--------+---------+--------+---------+-------+-------+------+------+----+--------+--------+----+------+
    only showing top 3 rows
```
```

### 7、分组聚合（groupBy Rename）

```python
```python
"""
  使用DataFrame对象中groupBy函数实现分组统计，agg函数实现聚合
      -1. groupBy(self, *cols)
          接收一个列表作为参数，用于分组依据
      -2. agg(self, *exprs) 
          接收一个 字典作为参数，其中Key表示的是列名称，Value表示的是对列进行操作的函数名称，比如max，min，avg等等
"""

daily_delay_df = df.groupBy(df.day).agg({'dep_delay': 'mean', 'arr_delay': 'mean'})
print daily_delay_df.count()
输出：
	31
```
```

```python
```python
daily_delay_df.show(n=10,  truncate=False)
输出：
    +---+--------------------+------------------+
    |day|avg(arr_delay)      |avg(dep_delay)    |
    +---+--------------------+------------------+
    |7  |0.025215252152521524|5.243243243243243 |
    |15 |1.0819155639571518  |4.818353236957888 |
    |11 |5.749170537491706   |7.250661375661376 |
    |29 |6.407451923076923   |11.32174955062912 |
    |3  |5.629350893697084   |11.526241799437676|
    |30 |9.433526011560694   |12.31663788140472 |
    |8  |0.52455919395466    |4.555904522613066 |
    |22 |-1.0817571690054912 |6.10231425091352  |
    |28 |-3.4050632911392404 |4.110270951480781 |
    |16 |0.31582125603864736 |4.2917420132610005|
    +---+--------------------+------------------+
    only showing top 10 rows
```
```

```python
```python
# 重命名DataFrame中各个字段的名称

daily_delay_df.withColumnRenamed?

"""
>>> df.withColumnRenamed('age', 'age2').collect()
[Row(age2=2, name=u'Alice'), Row(age2=5, name=u'Bob')]
"""
```
```

```python
```python
# 重命名
daily_delay_df2 = daily_delay_df.withColumnRenamed('avg(arr_delay)', 'avg_arr_delay')

daily_delay_df2.printSchema()
输出：
    root
     |-- day: string (nullable = true)
     |-- avg_arr_delay: double (nullable = true)
     |-- avg(dep_delay): double (nullable = true)
```
```

```python
```python
daily_delay_df3 = daily_delay_df2.withColumnRenamed('avg(dep_delay)', 'avg_dep_delay')
daily_delay_df3.show(n=5)


    +---+--------------------+------------------+
    |day|       avg_arr_delay|     avg_dep_delay|
    +---+--------------------+------------------+
    |  7|0.025215252152521524| 5.243243243243243|
    | 15|  1.0819155639571518| 4.818353236957888|
    | 11|   5.749170537491706| 7.250661375661376|
    | 29|   6.407451923076923| 11.32174955062912|
    |  3|   5.629350893697084|11.526241799437676|
    +---+--------------------+------------------+
    only showing top 5 rows
```
```

### 8、可视化展示（SparkSQL中DataFrame转换为Pandas中DataFrame）

```python
```python
# 将SparkSQL中DataFrame转换为Pandas中DataFrame, 注意此时DataFrame中数据条目数很少
local_daily_delay_df = daily_delay_df3.toPandas()
local_daily_delay_df.head(10)
```
```

||day|avg_arr_delay|avg_dep_delay|
|----|----|----|----|
|7|0.025215|5.243243| |
|15|1.081916|4.818353| |
|11|5.749171|7.250661| |
|29|6.407452|11.321750| |
|3|5.629351|11.526242| |
|30|9.433526|12.316638| |
|8|0.524559|4.555905| |
|22|-1.081757|6.102314| |
|28|-3.405063|4.110271| |
|16|0.315821|4.291742| |


```python
```python
# 使用matplotlib进行数据可视化，将图像内嵌 到Jupyter中如何操作
%matplotlib inline
```
```



