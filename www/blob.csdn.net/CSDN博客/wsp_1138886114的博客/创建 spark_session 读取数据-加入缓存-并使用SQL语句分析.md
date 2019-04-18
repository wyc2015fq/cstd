# 创建 spark_session 读取数据-加入缓存-并使用SQL语句分析 - wsp_1138886114的博客 - CSDN博客





2018年09月21日 21:04:14[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：282











### 文章目录
- [1 创建 spark_session 读取数据-加入缓存](#1__spark_session__2)
- [2 SparkSQL数据分析（DSL，SQL）](#2_SparkSQLDSLSQL_41)



c.spark_session_quick_start.py


##### 1 创建 spark_session 读取数据-加入缓存

```python
```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import time
from pyspark.sql import SparkSession

if __name__ == "__main__":
	#配置环境变量
    os.environ['JAVA_HOME'] = 'C:/Java/jdk1.8.0_91'
    os.environ['HADOOP_HOME'] = 'C:/Java/hadoop-2.6.0-cdh5.7.6'
    os.environ['SPARK_HOME'] = 'C:/Java/spark-2.2.0-bin-2.6.0-cdh5.7.6'

    # 实例化SparkSession对象，以本地模式是运行Spark程序
    spark = SparkSession \
        .builder \
        .appName("Hello_World_Application") \
        .master("local[2]")\
        .getOrCreate()
    # print type(spark)   检验spark是否可用

    # 读取数据, 一行一行的读取，每行数据的字段名称为value，类型为字符串
    log_data = spark.read.text("datas/README.md")
    print (type(log_data))
    print (log_data.first())
    print ("Count: " + str(log_data.count()))
    print ('\n')

    # 在Spark框架中可以将数据进行缓存，以便再次使用时，直接从缓存中读取数据
    # 默认缓存级别：MEMORY_AND_DISK，先放内存，不足放磁盘
    log_data.cache()

    # 对DataFrame中每条数据进行过滤，获取每条数据中的value字段的只值，进行筛选
    nums_spark = log_data.filter(log_data.value.contains('Spark')).count()
    nums_python = log_data.filter(log_data.value.contains('Python')).count()
    print("Lines with Spark: %i, lines with Python: %i" % (nums_spark, nums_python))
```
```

##### 2 SparkSQL数据分析（DSL，SQL）

```python
```python
# 使用SparkSession读取wc.data，数据封装在DataFrame集合中
    wc_df = spark.read.text('datas/wc.data')
    print (type(wc_df))
    wc_df.show(n=5,  truncate=False)

    # DataFrame = RDD + schema, 如何将DataFrame转换为RDD
    """
    SparkSQL中对数据分析两种方式：
    -1. DSL分析    调用DataFrame中函数  
    -2. SQL分析    需要将DataFrame注册为临时视图，编写类似MySQL中SQL进行分析    
    """
    # 导入SparkSQL中函数库
    from pyspark.sql.functions import *

    word_df = wc_df\
        .select(explode(split(wc_df.value, '\\s+'))\
        .alias('word'))

    word_count_df = word_df.groupBy('word').count()  # 操作以后, 聚合count以后的字段名称为count
    word_count_df.show()


    # 注册时临时视图
    word_df.createOrReplaceTempView('view_tmp_word')
    spark.sql('SELECT word, COUNT(1) AS count FROM view_tmp_word GROUP BY word').show()

    # 读取CSV文件
    csv_df = spark.read.csv('datas/flights.csv', header=True, inferSchema=True)
    csv_df.printSchema()
    csv_df.show(n=10, truncate=False)

    csv_df.write.csv('datas/flights.tsv', header=True, sep='\t')

    # 为了查看Spark程序运行是的WEB UI界面，让线程休眠一段时间
    time.sleep(100000)

    # SparkContext Stop
    spark.stop()
```
```



