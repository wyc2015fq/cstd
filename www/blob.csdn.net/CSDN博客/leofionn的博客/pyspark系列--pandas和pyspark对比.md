
# pyspark系列--pandas和pyspark对比 - leofionn的博客 - CSDN博客


2018年09月22日 21:41:25[leofionn](https://me.csdn.net/qq_36142114)阅读数：327


目录
pandas和pyspark对比
1.1. 工作方式
1.2. 延迟机制
1.3. 内存缓存
1.4. DataFrame可变性
1.5. 创建
1.6. index索引
1.7. 行结构
1.8. 列结构
1.9. 列名称
1.10. 列添加
1.11. 列修改
1.12. 显示
1.13. 排序
1.14. 选择或切片
1.15. 过滤
1.16. 整合
1.17. 统计
1.18. 合并
1.19. 失数据处理
1.20. SQL语句
1.21. 两者互相转换
1.22. 函数应用
1.23. map-reduce操作
1.24. diff操作
pandas和pyspark对比
1.1. 工作方式
pandas
单机single machine tool，没有并行机制parallelism，不支持Hadoop，处理大量数据有瓶颈
pyspark
分布式并行计算框架，内建并行机制parallelism，所有的数据和操作自动并行分布在各个集群结点上。以处理in-memory数据的方式处理distributed数据。支持Hadoop，能处理大量数据。
1.2. 延迟机制
pandas
not lazy-evaluated
pyspark
lazy-evaluated
1.3. 内存缓存
pandas
单机缓存
pyspark
persist() or cache()将转换的RDDs保存在内存
1.4. DataFrame可变性
pandas
Pandas中DataFrame是可变的
pyspark
Spark中RDDs是不可变的，因此DataFrame也是不可变的
1.5. 创建
pandas
从spark_df转换：pandas_df = spark_df.toPandas()，或读取其他数据
pyspark
从pandas_df转换：spark_df = SQLContext.createDataFrame(pandas_df)
另外，createDataFrame支持从list转换spark_df，其中list元素可以为tuple，dict，rdd
1.6. index索引
pandas
自动创建
pyspark
没有index索引，若需要则要额外创建该列
1.7. 行结构
pandas
Series结构，属于Pandas DataFrame结构
pyspark
Row结构，属于Spark DataFrame结构
1.8. 列结构
pandas
Series结构，属于Pandas DataFrame结构
pyspark
Column结构，属于Spark DataFrame结构，如：DataFrame[name: string]
1.9. 列名称
pandas
不允许重名
pyspark
允许重名,修改列名采用alias方法
1.10. 列添加
pandas
df[“xx”] = 0
pyspark
df.withColumn(“xx”, 0).show() 会报错
from pyspark.sql import functions
df.withColumn(“xx”, functions.lit(0)).show()
1.11. 列修改
pandas
原来有df[“xx”]列，df[“xx”] = 1
pyspark
原来有df[“xx”]列，df.withColumn(“xx”, 1).show()
1.12. 显示
pandas
df 输出具体内容
pyspark
df 不输出具体内容，输出具体内容用show方法.
输出形式：DataFrame[age: bigint, name: string]
以树的形式打印概要：df.printSchema()
用df.collect(5)
1.13. 排序
pandas
df.sort_index() 按轴进行排序
df.sort() 在列中按值进行排序
pyspark
df.sort() 在列中按值进行排序
1.14. 选择或切片
pandas	pyspark
[df.name](http://df.name)输出具体内容	df[] 不输出具体内容，输出具体内容用show方法。
df[‘name’] 不输出具体内容，输出具体内容用show方法
df[‘name’] 输出具体内容	df.select() 选择一列或多列.
df.select(‘name’)
切片 df.select(df[‘name’], df[‘age’]+1)
df[0] ,df.ix[0]	df.first()
df.head(2)	df.head(2), df.take(2)
df.tail(2)
切片 df.ix[:3]或者df.ix[:‘xx’]或者df[:‘xx’]
df.loc[] 通过标签进行选择
df.iloc[] 通过位置进行选择
1.15. 过滤
pandas
df[df[‘age’]>21]
pyspark
df.filter(df[‘age’]>21) 或者 df.where(df[‘age’]>21)
1.16. 整合
pandas
df.groupby(‘age’)
df.groupby(‘A’).avg(‘B’)
pyspark
df.groupBy(‘age’)
df.groupBy(‘A’).avg(‘B’).show() 应用单个函数
from pyspark.sql import functions 导入内置函数库
df.groupBy(‘A’).agg(functions.avg(‘B’), functions.min(‘B’), functions.max(‘B’)).show() 应用多个函数
1.17. 统计
pandas
df.count() 输出每一列的非空行数
df.describe() 描述某些列的count, mean, std, min, 25%, 50%, 75%, max
pyspark
df.count() 输出总行数
df.describe() 描述某些列的count, mean, stddev, min, max
1.18. 合并
pandas
Pandas下有concat方法，支持轴向合并
Pandas下有merge方法，支持多列合并
同名列自动添加后缀，对应键仅保留一份副本
df.join() 支持多列合并
df.append() 支持多行合并
pyspark
Spark下有join方法即df.join()
同名列不自动添加后缀，只有键值完全匹配才保留一份副本
1.19. 失数据处理
pandas	pyspark
对缺失数据自动添加NaNs	不自动添加NaNs，且不抛出错误
fillna函数：df.fillna()	fillna函数：df.na.fill()
dropna函数：df.dropna()	dropna函数：df.na.drop()
1.20. SQL语句
pandas
import sqlite3
pd.read_sql(“SELECT name, age FROM people WHERE age >= 13 AND age <= 19”)
pyspark
1.表格注册：把DataFrame结构注册成SQL语句使用类型
df.registerTempTable(‘people’) 或者 sqlContext.registerDataFrameAsTable(df, ‘people’)
spark.sql(‘SELECT name, age FROM people WHERE age >= 13 AND age <= 19’)
2.功能注册：把函数注册成SQL语句使用类型
spark.registerFunction(‘stringLengthString’, lambda x: len(x))
spark.sql(“SELECT stringLengthString(‘test’)”)
1.21. 两者互相转换
pandas_df = spark_df.toPandas()
spark_df = spark.createDataFrame(pandas_df)
1.22. 函数应用
pandas
df.apply(f) 将df的每一列应用函数f
pyspark
df.foreach(f) 或者 df.rdd.foreach(f) 将df的每一列应用函数f
df.foreachPartition(f) 或者 df.rdd.foreachPartition(f) 将df的每一块应用函数f
1.23. map-reduce操作
pandas
map-reduce操作map(func, list)，reduce(func, list) 返回类型seq
pyspark
df.map(func)，df.reduce(func) 返回类型seqRDDs
1.24. diff操作
pandas
有diff操作，处理时间序列数据（Pandas会对比当前行与上一行）
pyspark
没有diff操作（Spark的上下行是相互独立，分布式存储的）
---
本文来自 振裕 的CSDN 博客 ，全文地址请点击：[https://blog.csdn.net/suzyu12345/article/details/79673483?utm_source=copy](https://blog.csdn.net/suzyu12345/article/details/79673483?utm_source=copy)

