# 【python】python本地windows调试spark安装配置及代码示例 - zkq_1986的博客 - CSDN博客





2019年01月23日 14:35:51[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：64








# 安装

要想在windows下搭建Spark(python版本)主要分为：

JDK安装配置

Scala安装配置 （可不安装）

Spark安装配置

Hadoop安装配置

Python安装配置

PyCharm运行

## JDK安装配置



JDK 版本需要在 1.8 

下载地址：http://www.oracle.com/technetwork/java/javase/downloads/index.html 

配置环境变量 

添加变量 JAVA_HOME 值为jdk目录 

在path 追加 ;%JAVA_HOME%\bin;%JAVA_HOME%\jre\bin; 

保存后 

cmd 运行 java -version 

## Scala安装配置（可不安装）



Scala download 下载对应scala 版本 【注意：Scala 1.6.2版本只能使用Spark 2.10各个版本；Scala 2.10.6 可适配Spark1.3.0到1.6.2】 

windows 下直接下载msi 直接运行 会自动写入到path 

安装完毕，验证Scala 



[如果系统找不到scala 命令，就自己手动将scala安装目录加入到 系统环境变量的path]

## Spark安装配置



spark download 安装spark 

 安装的目录应该是要与spark bin的 同一层上 

解压后 

运行cmd 

spark-shell 

这时应该还没成功的，会有报错 

## Hadoop安装配置



hadoop 2.6.4 download 

下载hadoop解压之后 

设置环境变量 

添加 HADOOP_HOME 值为hadoop解压目录 

添加path ;%HADOOP_HOME%\bin

下载2.6版本hadoop 会缺少winutils.exe 

去 http://blog.csdn.net/u011513853/article/details/52865076 下载对应hadoop 版本的winutils.exe 

下载完毕后 将 winutils.exe 复制到 %HADOOP_HOME%\bin 目录下 

注意：2.7版本也适用于2.6 

还要重点注意！！ 

需要修改 winutils.exe 访问权限 

运行 spark-shell

从图中可以看到，在直接输入spark-shell命令后，Spark开始启动，并且输出了一些日志信息，大多数都可以忽略，需要注意的是两句话：



## Python安装配置



windows 下的话就很简单啦，直接下载msi点击运行就可以了

https://python-xy.github.io/downloads.html

这里是python(X,Y)已经包含spark需要的lib了，所以很方便 

python 下载，如果是msi就自动加入path,如果不行就手动加入吧 

cmd输入 python

PyCharm运行

Pycharm IDE 下载 

http://www.jetbrains.com/pycharm/download/#section=windows





# 代码示例

```python
#coding=utf-8

import os
import sys

os.environ['SPARK_HOME'] = r'D:\software\bigdata\spark-2.2.0-bin-hadoop2.6'
sys.path.append(r'D:\software\bigdata\spark-2.2.0-bin-hadoop2.6\python')

from pyspark.sql import SparkSession
from pyspark.sql import Row
import sys
import numpy as np


if __name__ == '__main__':
    spark = SparkSession.builder.appName('python_test').getOrCreate()
    inpath='file:///D:/temp/dev/a.parq'
    outpath='file:///D:/temp/dev/b.parq'
   
    df = spark.read.parquet(inpath)
    df.write.parquet(outpath)
```



【参考文献】

1. [https://blog.csdn.net/funfun0/article/details/77802590](https://blog.csdn.net/funfun0/article/details/77802590)

2. [https://blog.csdn.net/shiheyingzhe/article/details/80714301](https://blog.csdn.net/shiheyingzhe/article/details/80714301)





