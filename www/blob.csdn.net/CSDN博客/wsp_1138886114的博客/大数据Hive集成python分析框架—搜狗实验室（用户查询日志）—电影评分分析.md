# 大数据Hive集成python分析框架—搜狗实验室（用户查询日志）—电影评分分析 - wsp_1138886114的博客 - CSDN博客





2018年07月26日 19:26:57[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：377










- - - [一、Spark 大数据分析框架](#一spark-大数据分析框架)- - [1.1 数据结构](#11-数据结构)
- [1.2 SQL语句简介](#12-sql语句简介)


- [二、搜狗实验室（用户查询日志）数据分析](#二搜狗实验室用户查询日志数据分析)- - [2.1获取数据集并初步分析：](#21获取数据集并初步分析)
- [2.2 创建数据库/表——导入数据分析](#22-创建数据库表导入数据分析)


- [三、电影评分分析](#三电影评分分析)- - [3.1 数据集 样本分析](#31-数据集-样本分析)
- [3.2 在Hive中使用CTAS方式创建临时结果表–方便后续分析](#32-在hive中使用ctas方式创建临时结果表方便后续分析)
- [3.3 使用Python脚本处理Hive表中数据](#33-使用python脚本处理hive表中数据)
- [3.4 时间戳处理-python unixtime_to_weekday.py](#34-时间戳处理-python-unixtimetoweekdaypy)








### 一、Spark 大数据分析框架

##### 1.1 数据结构

```
1. 功能：   类似MapReduce并行计算框架，对海量数据分析处理

2. 数据结构：
    -a. 最原始数据结构，最核心
        RDD（弹性分布式数据集）：类比Python中列表
                               处理数据调用函数Transformation（高阶函数）
        SparkContext: sc (实例对象名称) 
    -b. 最新数据结构
        DataFrame = RDD + Schema（字段类型和字段名称）
        SparkSession: spark
            spark.read / result_df.write
                .cvs(.tsv)
                .json
                .jdbc
                .hdfs
                .....
```

##### 1.2 SQL语句简介

无论是Hadoop框架还是Spark框架数据分析，通常都是通过SQL语句分析，类似于MySQL数据库SQL语句，95%类似 
- SQL 语言：
提供从RDBMS中检索数据（查询数据、分析数据） 

语法：`SELECT ... FROM ... WHERE ... GROUP BY ... ORDER BY ...`
SQL on HADOOP：对海量数据进行分析（底层MR/Spark）
- Hive 框架：
出现最早的SQL on Hadoop框架，将大数据分析使用SQL完成编码 

Facebook 为了方便DBA对基于HADOOP大数据平台对海量数据分析，开发提供SQL接口的大数据框架Hive。- Hive + Python集成分析数据
-1. Python 脚本 文件用于预处理数据-2. Hive提供SQL来分析数据
Hive 框架 类似MySQL数据库分析数据SQL语法非常类似 

        提供[erognda]虚拟机中已经在linux系统中安装mysql数据库和hive框架。

### 二、搜狗实验室（用户查询日志）数据分析

##### 2.1获取数据集并初步分析：

搜狗实验室：[http://www.sogou.com/labs/](http://www.sogou.com/labs/)

用户查询日志(SogouQ): [http://www.sogou.com/labs/resource/q.php](http://www.sogou.com/labs/resource/q.php)

```
数据格式：
访问时间 \t 用户ID \t [查询词] \t 该URL在返回结果中的排名space用户点击的顺序号 \t 用户点击的URL 

1. 样本数据
    00:00:00    2982199073774412    [360安全卫士]   8 3 \
    download.it.com.cn/softweb/software/firewall/antivirus/20067/17938.html
2. 对应字段名称
    access_time
    user_id
    search_word
    sort_id$click_id
    url 
3. Hive表中每条数据 对应  数据文件中 一行数据，各个字段之间的分隔符隔开（制表符）
如何使用Hive分析数据：
    -a. 创建数据库（如果不存在的话）
    -b. 创建表 
        指定表的字段名称和字段类型，字段值的分隔符
    -c. 加载数据到表中
        将日志文件数据 映射 到表 
    -d. 业务编写SQL分析数据
        HiveQL（类似MySQL中SQL）

Spark SQL读取Hive表中的数据
    -1. 前世今生
        Hive   ->  Shark    ->  SparkSQL
            将SQL转换MR
    -2. SparkSQL一开始就是为了处理Hive表中的数据，提供SQL
        SparkSQL与Hive天然无缝集成
    -3. 不仅仅使用SQL分析，又提供类似Pandas数据分析理念
        DataFrame

（VM虚拟机环境全部配置完成）
 SparkSQL读取Hive表中的数据分析，简单的配置
    -1. 将Hive配置文件hive-site.xml放入Spark安装目录下
        $ cp ${HIVE_HOME}/conf/hive-site.xml ${SPARK_HOME}/conf/
    -2. 将MySQL驱动包放入Spark框架jar包目录
        mysql-connector-java-5.1.27-bin.jar
```

##### 2.2 创建数据库/表——导入数据分析

```python
#启动 pyspark 框架中的主从机(打开终端执行以下命令：)
/opt/modules/hadoop-2.7.3/sbin/hadoop-daemon.sh start namenode 
/opt/modules/hadoop-2.7.3/sbin/hadoop-daemon.sh start datanode
jps
输出：
3191 jps
3111 DataNode
3021 NameNode
```

```
-- 创建并使用数据库
CREATE DATABASE IF NOT EXISTS db_hive ;
USE db_hive ;

-- 创建表
CREATE TABLE IF NOT EXISTS db_hive.sg_log(
access_time string,
user_id string,
search_word string,
sortId_clickId string,
url string)
ROW FORMAT DELIMITED FIELDS TERMINATED BY '\t';  --分隔符：制表符

-- 加载数据文件 到 表中
LOAD DATA LOCAL INPATH '/home/rdedu/SogouQ.sample' INTO TABLE db_hive.sg_log ;
LOAD DATA LOCAL INPATH '/home/rdedu/SogouQ.reduced' INTO TABLE db_hive.sg_log ;

-- 设置HiveQL转换MapReduce运行在LocalMode中
set hive.exec.mode.local.auto=true;

-- 查看条目数、样本数据、删除表
select count(1) from sg_log ;
select * from sg_log limit 5 ;
DROP TABLE IF EXISTS db_hive.sg_log ;

-- 设置HiveQL转换MapReduce运行在LocalMode中
set hive.exec.mode.local.auto=true;

-- 对搜狗用户日志分析，一天的额数据，设置参数，以便运行在本地模式
set hive.exec.mode.local.auto.inputbytes.max = 160939050 ;

-----------分析数据（依据业务）来分析------------

-- 1. 统计每个用户的PV数，降序排列desc   pv: pageview 用户浏览网页个数
SELECT user_id, COUNT(1) AS pv FROM db_hive.sg_log GROUP BY user_id ORDER BY pv DESC LIMIT 10 ;

-- 2.  统计每个小时的PV数
SELECT  t.hour_str, COUNT(1) AS pv 
FROM (
      SELECT SUBSTRING(access_time,0 ,2) AS hour_str, url  FROM db_hive.sg_log
     ) As t 
GROUP BY t.hour_str ORDER BY pv DESC ;

-- 3. 统计每个用户搜索关键词点击结果的次数-- 从某种程度上，检验搜索引擎底层技术
SELECT 
  user_id, search_word, COUNT(1) AS cnt
FROM
  db_hive.sg_log
GROUP BY 
  user_id, search_word
ORDER BY 
  cnt DESC 
LIMIT 20 ;

-- 4. 统计每个用户搜索关键词的的个数
SELECT user_id, COUNT(DISTINCT search_word) AS cnt FROM db_hive.sg_log GROUP BY user_id ORDER BY cnt DESC LIMIT 10 ;
```

### 三、电影评分分析

##### 3.1 数据集 样本分析

```
用户信息表-样本数据：
    1|24|M|technician|85711
    user id | age | gender | occupation | zip code 

电影评分表-样本数据： 
    196 242 3   881250949
    user id | item id | rating | timestamp
```

```
-- 创建数据库、用户信息表
CREATE DATABASE IF NOT EXISTS db_movie ;
CREATE TABLE IF NOT EXISTS db_movie.ml_user(
user_id string,
age string,
gender string,
occupation string,
zipcode string)
ROW FORMAT DELIMITED FIELDS TERMINATED BY '|';

-- 创建电影评分表
CREATE TABLE IF NOT EXISTS db_movie.ml_rating(
user_id string,
item_id string,
rating string,
unixtime string)
ROW FORMAT DELIMITED FIELDS TERMINATED BY '\t';

-- 加载数据到 用户信息表/电影评分表
LOAD DATA LOCAL INPATH '/home/rdedu/ml-100k/u.user' INTO TABLE db_movie.ml_user ;
LOAD DATA LOCAL INPATH '/home/rdedu/ml-100k/u.data' INTO TABLE db_movie.ml_rating ;

-- 统计表中的条目数
SELECT COUNT(1) AS FROM db_movie.ml_rating ;
SELECT COUNT(1) AS FROM db_movie.ml_user ;

-- 每部电影的评分人数及平均分
SELECT  
  item_id, COUNT(item_id) AS num, ROUND(AVG(rating), 2) AS avg_rating
FROM 
  db_movie.ml_rating
GROUP BY 
  item_id
ORDER BY 
  num DESC, avg_rating DESC 
LIMIT 10 ;

-- 每个用户评分的次数和工作
SELECT 
  t1.user_id, t1.cnt, t2.occupation
FROM(
  SELECT user_id, COUNT(1) AS cnt FROM db_movie.ml_rating GROUP BY user_id ORDER BY cnt DESC 
) As t1
JOIN 
  db_movie.ml_user t2 
ON
  t1.user_id = t2.user_id ;
```

##### 3.2 在Hive中使用CTAS方式创建临时结果表–方便后续分析

```
示例: CTAS：Create Table As Select
```

```
CREATE TABLE IF NOT EXISTS db_movie.user_cnt_occu
AS
SELECT 
  t1.user_id, t1.cnt, t2.occupation
FROM(
  SELECT user_id, COUNT(1) AS cnt FROM db_movie.ml_rating GROUP BY user_id ORDER BY cnt DESC 
) As t1
JOIN 
  db_movie.ml_user t2 
ON
  t1.user_id = t2.user_id ;  

  -- 统计 评分的用户中各个职业的人数
SELECT 
  occupation, SUM(cnt) AS total  
FROM 
  db_movie.user_cnt_occu 
GROUP BY 
  occupation 
ORDER BY total DESC limit 21 ;

-- 先统计有多少个职业
SELECT COUNT(DISTINCT occupation) AS cnt FROM db_movie.user_cnt_occu ;
```

##### 3.3 使用Python脚本处理Hive表中数据

```
如何使用Python脚本处理Hive表中数据
    python脚本处理表的每条数据
遵循原则: 
    -a. 输入数据  TRANSFORM
        表中的字段
    -b. 处理数据  USING
        指定python脚本处理数据
    -c. 输出数据   AS
        输出字段
```

```
CREATE TABLE IF NOT EXISTS db_movie.ml_rating_new(
user_id string,
item_id string,
rating string,
weekday string)
ROW FORMAT DELIMITED FIELDS TERMINATED BY '\t';

-- 加载脚本文件到集群开发环境 中
add file /home/rdedu/ml-100k/unixtime_to_weekday.py ;

-- 从原数据表中读取数据，加载到 业务数据表
INSERT OVERWRITE TABLE db_movie.ml_rating_new
SELECT 
  TRANSFORM (user_id, item_id, rating, unixtime)
  USING 'python unixtime_to_weekday.py'
  AS (user_id, item_id, rating, weekday)
FROM 
  db_movie.ml_rating ;

  --  统计一周中 每天观看电影的人数
SELECT weekday, COUNT(1) AS total FROM db_movie.ml_rating_new GROUP BY weekday ORDER BY total DESC ;
```

##### 3.4 时间戳处理-python unixtime_to_weekday.py

```python
#!/usr/local/bin/python
# -*- coding: utf-8 -*-

import sys
import datetime
"""
  将878887116 时间戳转换为星期几
  #样本数据(913 209 2   881367150)
"""
for line in sys.stdin:
   line = line.strip()
   user_id, item_id, rating, unixtime = line.split("\t")
   # 调用函数将时间戳为星期几
   weekday = datetime.datetime.fromtimestamp(float(unixtime)).isoweekday()
   # 标准输出
   print '\t'.join([user_id, item_id, rating, str(weekday)])
```



