# spark-shell用非sql API 改写 hql - Soul Joy Hub - CSDN博客

2016年08月29日 11:59:44[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：891


当我们在spark-shell用scala写程序的时候，如何不嵌入sql来查询Hive呢？这里我们来举个例子：

## hql

先来看下想要执行的sql，这里选用了TPC-DS中的query3：

```
select
           /*+MAPJOIN(dt, item)*/
             dt.d_year
            ,item.i_brand_id brand_id
            ,item.i_brand brand
            ,sum(ss_ext_sales_price) sum_agg
     from
          store_sales
          ,item
     ,date_dim dt
     where dt.d_date_sk = store_sales.ss_sold_date_sk
       and store_sales.ss_item_sk = item.i_item_sk
       and item.i_manufact_id = 436
       and dt.d_moy=12
     group by dt.d_year
          ,item.i_brand
          ,item.i_brand_id
     order by dt.d_year
             ,sum_agg desc
             ,brand_id
     limit 10
```

我们看下在Hive shell，use 2g数据库“tpcds_text_2”，查询结果：

```
Query ID = root_20160829113535_c357e9ac-8f93-4d79-b32c-efee18603f9c
Total jobs = 2
Execution log at: /tmp/root/root_20160829113535_c357e9ac-8f93-4d79-b32c-efee18603f9c.log
2016-08-29 11:35:41 Starting to launch local task to process map join;  maximum memory = 2024800256
2016-08-29 11:35:43 Dump the side-table for tag: 1 with group count: 6200 into file: file:/tmp/root/14cf9218-fe98-4d6d-940c-ec994c2a1552/hive_2016-08-29_11-35-35_363_2500629239737861774-1/-local-10006/HashTable-Stage-3/MapJoin-mapfile21--.hashtable
2016-08-29 11:35:44 Uploaded 1 File to: file:/tmp/root/14cf9218-fe98-4d6d-940c-ec994c2a1552/hive_2016-08-29_11-35-35_363_2500629239737861774-1/-local-10006/HashTable-Stage-3/MapJoin-mapfile21--.hashtable (156018 bytes)
2016-08-29 11:35:44 Dump the side-table for tag: 1 with group count: 32 into file: file:/tmp/root/14cf9218-fe98-4d6d-940c-ec994c2a1552/hive_2016-08-29_11-35-35_363_2500629239737861774-1/-local-10006/HashTable-Stage-3/MapJoin-mapfile31--.hashtable
2016-08-29 11:35:44 Uploaded 1 File to: file:/tmp/root/14cf9218-fe98-4d6d-940c-ec994c2a1552/hive_2016-08-29_11-35-35_363_2500629239737861774-1/-local-10006/HashTable-Stage-3/MapJoin-mapfile31--.hashtable (1644 bytes)
2016-08-29 11:35:44 End of local task; Time Taken: 2.481 sec.
Execution completed successfully
MapredLocal task succeeded
Launching Job 1 out of 2
Number of reduce tasks not specified. Estimated from input data size: 12
In order to change the average load for a reducer (in bytes):
  set hive.exec.reducers.bytes.per.reducer=<number>
In order to limit the maximum number of reducers:
  set hive.exec.reducers.max=<number>
In order to set a constant number of reducers:
  set mapreduce.job.reduces=<number>
Starting Job = job_1471250735061_0008, Tracking URL = http://holodesk01:8088/proxy/application_1471250735061_0008/
Kill Command = /usr/lib/hadoop/bin/hadoop job  -kill job_1471250735061_0008
Hadoop job information for Stage-3: number of mappers: 3; number of reducers: 12
2016-08-29 11:35:57,022 Stage-3 map = 0%,  reduce = 0%
2016-08-29 11:36:14,828 Stage-3 map = 22%,  reduce = 0%, Cumulative CPU 16.4 sec
2016-08-29 11:36:15,901 Stage-3 map = 100%,  reduce = 0%, Cumulative CPU 26.4 sec
2016-08-29 11:36:24,264 Stage-3 map = 100%,  reduce = 83%, Cumulative CPU 49.53 sec
2016-08-29 11:36:25,311 Stage-3 map = 100%,  reduce = 100%, Cumulative CPU 57.53 sec
MapReduce Total cumulative CPU time: 57 seconds 530 msec
Ended Job = job_1471250735061_0008
Launching Job 2 out of 2
Number of reduce tasks determined at compile time: 1
In order to change the average load for a reducer (in bytes):
  set hive.exec.reducers.bytes.per.reducer=<number>
In order to limit the maximum number of reducers:
  set hive.exec.reducers.max=<number>
In order to set a constant number of reducers:
  set mapreduce.job.reduces=<number>
Starting Job = job_1471250735061_0009, Tracking URL = http://holodesk01:8088/proxy/application_1471250735061_0009/
Kill Command = /usr/lib/hadoop/bin/hadoop job  -kill job_1471250735061_0009
Hadoop job information for Stage-4: number of mappers: 2; number of reducers: 1
2016-08-29 11:36:36,161 Stage-4 map = 0%,  reduce = 0%
2016-08-29 11:36:50,781 Stage-4 map = 100%,  reduce = 0%, Cumulative CPU 2.96 sec
2016-08-29 11:36:59,168 Stage-4 map = 100%,  reduce = 100%, Cumulative CPU 5.4 sec
MapReduce Total cumulative CPU time: 5 seconds 400 msec
Ended Job = job_1471250735061_0009
MapReduce Jobs Launched: 
Stage-Stage-3: Map: 3  Reduce: 12   Cumulative CPU: 57.53 sec   HDFS Read: 784204960 HDFS Write: 4354 SUCCESS
Stage-Stage-4: Map: 2  Reduce: 1   Cumulative CPU: 5.4 sec   HDFS Read: 14313 HDFS Write: 502 SUCCESS
Total MapReduce CPU Time Spent: 1 minutes 2 seconds 930 msec
OK
1998    10003008    exportiunivamalg #8 36243.50026702881
1998    4003001 exportiedu pack #1  34620.07957458496
1998    10014017    edu packamalgamalg #17  31645.54008293152
1998    3003001 exportiexporti #1   30119.83995437622
1998    5003001 exportischolar #1   28706.51041984558
1998    9016005 corpunivamalg #5    27940.650374412537
1998    2001001 amalgimporto #1 27937.100036621094
1998    8004005 edu packnameless #5 22627.530197143555
1998    4002001 importoedu pack #1  21856.61989593506
1998    1002001 importoamalg #1 20252.259941101074
Time taken: 86.116 seconds, Fetched: 10 row(s)
```

## spark API

下面是对应的scala程序：

```
import org.apache.spark.sql.Row
import org.apache.spark.sql.SparkSession

//Hive warehouseLocation
val warehouseLocation = "hdfs://holodesk01:9000/user/hive/warehouse"

val spark = SparkSession.builder().appName("Spark Hive Example").config("spark.sql.warehouse.dir", warehouseLocation).enableHiveSupport().getOrCreate()

import spark.implicits._
import spark.sql

/*
Spark SQL caches Parquet metadata for better performance. When Hive metastore Parquet table conversion is enabled, metadata of those converted tables are also cached. If these tables are updated by Hive or other external tools, you need to refresh them manually to ensure consistent metadata.

spark.catalog.refreshTable("tpcds_text_2.item")
spark.catalog.refreshTable("tpcds_text_2.date_dim")
spark.catalog.refreshTable("tpcds_text_2.store_sales")
*/

val item = spark.table("tpcds_text_2.item")
val date_dim = spark.table("tpcds_text_2.date_dim")
val store_sales = spark.table("tpcds_text_2.store_sales")
//先对表做filter，再做join，可大大提高性能
val itemFiltered = item.filter( $"i_manufact_id" === 436)
//groupBy 返回 RelationalGroupedDataset  对象，再对调用agg，可返回 包含 grouping columns 和 聚合列 的 DataFrame
date_dim.filter($"d_moy" === 12).join(store_sales, $"d_date_sk" === $"ss_sold_date_sk", "inner").join(itemFiltered, $"ss_item_sk" === $"i_item_sk", "inner").groupBy($"d_year",$"i_brand",$"i_brand_id").agg(Map("ss_ext_sales_price"->"sum")).sort($"d_year",$"sum(ss_ext_sales_price)".desc,$"i_brand_id").select($"d_year",$"i_brand",$"i_brand_id",$"sum(ss_ext_sales_price)").limit(10).show()

/***

+------+--------------------+----------+-----------------------+                
|d_year|             i_brand|i_brand_id|sum(ss_ext_sales_price)|
+------+--------------------+----------+-----------------------+
|  1998| exportiunivamalg #8|  10003008|      36243.50026702881|
|  1998|  exportiedu pack #1|   4003001|      34620.07957458496|
|  1998|edu packamalgamal...|  10014017|      31645.54008293152|
|  1998|   exportiexporti #1|   3003001|      30119.83995437622|
|  1998|   exportischolar #1|   5003001|      28706.51041984558|
|  1998|    corpunivamalg #5|   9016005|     27940.650374412537|
|  1998|     amalgimporto #1|   2001001|     27937.100036621094|
|  1998| edu packnameless #5|   8004005|     22627.530197143555|
|  1998|  importoedu pack #1|   4002001|      21856.61989593506|
|  1998|     importoamalg #1|   1002001|     20252.259941101074|
+------+--------------------+----------+-----------------------+

***/
```

## 总结

如果不嵌入sql，代码写起来还是会有点麻烦的。但是，不用sql写，反而能让我们学习到sql解析执行的过程，对其进行优化。

