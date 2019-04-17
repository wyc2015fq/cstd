# 【Python】pands dataframe与spark dataframe互转 - zkq_1986的博客 - CSDN博客





2018年11月13日 15:55:26[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：86








#!/usr/bin/env python3

# -*- coding: utf-8 -*-

import pandas as pd

from pyspark.sql import SparkSession

spark= SparkSession\

                .builder \

                .appName("dataFrame") \

                .getOrCreate()

# Loads data.

ll3=pd.DataFrame([[1,2],[3,4]],columns=['a','b'])

cc=ll3.values.tolist()

dd=list(ll3.columns)

#df=spark.createDataFrame(ll3)

#turn pandas.DataFrame to spark.dataFrame

spark_df = spark.createDataFrame(cc, dd)

print('spark.dataFram=',spark_df.show())

#turn spark.dataFrame to pandas.DataFrame  

pandas_df = spark_df.toPandas()  

print('pandas.DataFrame=',pandas_df)

+---+---+

|  a|  b|

+---+---+

|  1|  2|

|  3|  4|

+---+---+

spark.dataFram= None

pandas.DataFrame= a  b

0  1  2

1  3  4


转载自：[https://blog.csdn.net/luoganttcc/article/details/80631922](https://blog.csdn.net/luoganttcc/article/details/80631922)



