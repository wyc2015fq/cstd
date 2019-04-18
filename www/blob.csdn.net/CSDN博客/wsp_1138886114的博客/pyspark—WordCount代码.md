# pyspark—WordCount代码 - wsp_1138886114的博客 - CSDN博客





2018年09月21日 21:06:24[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：275








pyspark入门

```python
```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import time
from pyspark import SparkContext, SparkConf

if __name__ == "__main__":
    #设置环境变量
    os.environ['JAVA_HOME'] = 'C:/Java/jdk1.8.0_91'                      # java环境配置
    os.environ['HADOOP_HOME'] = 'C:/Java/hadoop-2.6.0-cdh5.7.6'          # hadoop安装目录
    os.environ['SPARK_HOME'] = 'C:/Java/spark-2.2.0-bin-2.6.0-cdh5.7.6'  # 设置spark安装目录

    spark_conf = SparkConf()\
        .setAppName('Python_Spark_WordCount')\
        .setMaster('local[2]') 
        
    #使用spark最原始的API进行数据分析
    sc = SparkContext(conf=spark_conf)
    sc.setLogLevel('WARN')
    print (sc)

   # ------创建RDD，需要分析的数据 --------------------------------------
    def local_rdd(spark_context):
        datas = ['hadoop spark','hadoop hive spark','hadoop hive spark',\
                 'hadoop python spark','hadoop python spark',]
        return spark_context.parallelize(datas)

    rdd = local_rdd(sc)
    print (rdd.count())
    print (rdd.first())

    time.sleep(10000)
    sc.stop()
```
```

```python
```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import time
from pyspark import SparkContext, SparkConf

if __name__ == "__main__":
    #设置环境变量
    os.environ['JAVA_HOME'] = 'C:/Java/jdk1.8.0_91'                      # java环境配置
    os.environ['HADOOP_HOME'] = 'C:/Java/hadoop-2.6.0-cdh5.7.6'          # hadoop安装目录
    os.environ['SPARK_HOME'] = 'C:/Java/spark-2.2.0-bin-2.6.0-cdh5.7.6'  # 设置spark安装目录

    spark_conf = SparkConf()\
        .setAppName('Python_Spark_WordCount')\
        .setMaster('local[2]')  
    # 设置Spark程序运行的地方，此处设置运行在本地模式，启动2个线程分析数据

    sc = SparkContext(conf=spark_conf) # 获取SparkContext实例对象, 用于读取要处理的数据和Job执行
    # 设置日志级别  Valid log levels include: ALL, DEBUG, ERROR, FATAL, INFO, OFF, TRACE, WARN
    sc.setLogLevel('WARN')
    #  <SparkContext master=local[2] appName=Python_Spark_WordCount>
    print (sc)

    """
    创建RDD，读取要分析的数据：
        -1. 方式一：从本地集合（列表、元组、字典）进行并行化创建
        -2. 方式二：从外部文件系统读取数据（HDFS、LocalFS）
    """
    # 第一种方式：从集合并行创建RDD
    def local_rdd(spark_context):
        datas = ['hadoop spark', 'spark hive spark spark', 'spark hadoop python hive', ' ']
        return spark_context.parallelize(datas)     # Create RDD

    # 第二种方式：从本地文件系统中读取
    def hdfs_rdd(spark_context):
        return spark_context.textFile("/user/rdedu/wc.data")  # 从文件中读取数据

    # rdd = local_rdd(sc)    #方法1
    rdd = hdfs_rdd(sc)       #方法2
    print (rdd.count())
    print (rdd.first())

    # =============词频统计=======================================
    word_count_rdd = rdd\
        .filter(lambda line: len(line.strip()) != 0)\
        .flatMap(lambda line: line.strip().split(" "))\
        .map(lambda word: (word, 1))\
        .reduceByKey(lambda a, b: a + b)          # 将Key相同的Value进行合并

    for word, count in word_count_rdd.collect():  # collect()函数将rdd转换为列表
        print word, ', ', count

    print "===================================="

    # 依据统计的count值降序排序
    sort_rdd = word_count_rdd\
        .map(lambda (word, count): (count, word))\
        .sortByKey(ascending=False)
    print sort_rdd.collect()

    # def top(self, num, key=None):
    print (word_count_rdd.top(3, key=lambda (word, count): count))

    # def takeOrdered(self, num, key=None) -> Bottom
    print (word_count_rdd.takeOrdered(3, key=lambda (word, count): count))

    # 为了查看Spark程序运行是的WEB UI界面，让线程休眠一段时间
    time.sleep(100000)

    # SparkContext Stop
    sc.stop()
```
```



