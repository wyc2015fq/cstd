# Hadoop生态系统命令合集（更新中……） - Soul Joy Hub - CSDN博客

2016年08月10日 10:06:28[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：709标签：[hadoop																[hive																[hdfs](https://so.csdn.net/so/search/s.do?q=hdfs&t=blog)](https://so.csdn.net/so/search/s.do?q=hive&t=blog)](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)
个人分类：[大数据平台](https://blog.csdn.net/u011239443/article/category/6296513)


# HDFS

## fsck

hdfs  fsck

Usage: DFSck  [-move | -delete | -openforwrite] [-files [-blocks [-locations | -racks]]]

```
<path>             检查这个目录中的文件是否完整

    -move               破损的文件移至/lost+found目录
    -delete             删除破损的文件

    -openforwrite   打印正在打开写操作的文件

    -files                 打印正在check的文件名

    -blocks             打印block报告 （需要和-files参数一起使用）

    -locations         打印每个block的位置信息（需要和-files参数一起使用）

    -racks               打印位置信息的网络拓扑图 （需要和-files参数一起使用）
```

hdfs  fsck /

用这个命令可以检查整个文件系统的健康状况，但是要注意它不会主动恢复备份缺失的block，这个是由NameNode单独的线程异步处理的。

# Hive

## 查看hive表中数据所在路径

```
hive> describe database bak_spark_tpcds_parquet_1000;
OK
bak_spark_tpcds_parquet_1000        hdfs://holodesk01:8020/user/hive/warehouse/bak_spark_tpcds_parquet_1000.db      USER    
Time taken: 0.02 seconds, Fetched: 1 row(s)
```

