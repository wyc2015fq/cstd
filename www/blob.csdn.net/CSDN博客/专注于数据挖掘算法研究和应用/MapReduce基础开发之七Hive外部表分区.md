# MapReduce基础开发之七Hive外部表分区 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年07月06日 11:54:55[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2070
个人分类：[Big data](https://blog.csdn.net/fjssharpsword/article/category/6309934)

所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









MapReduce输出的数据在实际应用中会加载在不同子目录下，比如按照日期、地区等，然后统一到外部表，这就需要用到hive表的分区。

假设输出的数据放在/tmp/fjs/dpi父目录下，下面分别有三个类型的数据作为子目录保存，分别是/tmp/fjs/dpi/adsl、/tmp/fjs/dpi/3g、/tmp/fjs/dpi/4g，具体分区如下：



Hive表：tmp_fjs_dpi

1） 建表脚本：

设置分区为DPI类型，分别是adsl、3g、4g

CREATE EXTERNAL TABLE`tmp_fjs_dpi`(

  `account` string, 

`date` string,

`city` string,

`domain` string,

  `para` string, 

  `value` string)

PARTITIONED BY (

  `dpitype` string)

ROW FORMAT DELIMITED 

  FIELDS TERMINATED BY '|' 

STORED AS INPUTFORMAT 

  'org.apache.hadoop.mapred.TextInputFormat' 

OUTPUTFORMAT 

 'org.apache.hadoop.hive.ql.io.HiveIgnoreKeyTextOutputFormat'

LOCATION

  'hdfs://nameservice-ha/tmp/fjs/dpi';

2）分区脚本：

查看分区：show partitionstmp_fjs_dpi;

增加分区：

alter table tmp_fjs_dpiadd partition (dpitype ='adsl') location 'adsl';

alter table tmp_fjs_dpiadd partition (dpitype ='3g') location '3g';

alter table tmp_fjs_dpiadd partition (dpitype ='4g') location '4g';

删除分区：

alter table tmp_fjs_dpidrop partition (dpitype ='4g');

3）查询结果：

select count(*) fromtmp_fjs_dpi where dpitype='adsl';

结果：92564960

select count(*) fromtmp_fjs_dpi where dpitype='3g';

结果：636277

select count(*) fromtmp_fjs_dpi where dpitype='4g';

结果：3828847

select para, count(*)from tmp_fjs_dpi group by para;

结果：

imei    97030084






