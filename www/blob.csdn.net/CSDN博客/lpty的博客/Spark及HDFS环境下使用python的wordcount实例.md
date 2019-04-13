
# Spark及HDFS环境下使用python的wordcount实例 - lpty的博客 - CSDN博客

2017年01月24日 17:17:00[lpty](https://me.csdn.net/sinat_33741547)阅读数：3892


一 前言
本文基于hadoop及spark完全分布式环境，详情查看以下文章：
1[
Hadoop环境](http://blog.csdn.net/sinat_33741547/article/details/54427113)
2[
spark环境](http://blog.csdn.net/sinat_33741547/article/details/54708749)
二 实例
代码默认存放路径：/usr/local/work
1.在HDFS上已经存放了文档，上传方式在我另一篇文章中有，这里就不贴出来了
2.wordcount.py

```python
from pyspark import SparkContext
inputFile = 'hdfs://master:9000/temp/hdin/*'        #测试文档
outputFile = 'hdfs://master:9000/temp/spark-out'    #结果目录
sc = SparkContext('local', 'wordcount')
text_file = sc.textFile(inputFile)
counts = text_file.flatMap(lambda line: line.split(' ')).map(lambda word: (word, 1)).reduceByKey(lambda a, b: a+b)
counts.saveAsTextFile(outputFile)
```
3.wordcount.sh

```python
export CURRENT=/usr/local/work
$SPARK_HOME/bin/spark-submit $CURRENT/wordcount.py
```
4.执行

```python
sh wordcount.sh
```
5.查看结果

```python
cd $HADOOP_HOME
bin/hdfs dfs -cat /temp/spark-out/*
```

