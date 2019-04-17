# hive外部表和内部表 - zj360202的专栏 - CSDN博客





2013年02月01日 11:24:16[zj360202](https://me.csdn.net/zj360202)阅读数：1971








1.内部表指hive创建并通过load data inpath进数据库的表，这种表可以理解为数据和表结构都保存在一起的数据表。当你通过DROP TABLE table_name 删除元数据中表结构的同时，表中的数据也同样会从hdfs中被删除。

```
CREATE TABLE new_hbase_table(rowkey string, x int, y int) 
STORED BY 'org.apache.hadoop.hive.hbase.HBaseStorageHandler'
WITH SERDEPROPERTIES ("hbase.columns.mapping" = ":key,cf:x,cf:y");

SET hive.hbase.bulk=true;

INSERT OVERWRITE TABLE new_hbase_table
SELECT rowkey_expression, x, y FROM ...any_hive_query...;
```

这里在hive中会将hbase的数据加载过来，并且保存相应的结构


2.外部表指在表结构创建以前，数据已经保存在hdfs中了，通过创建表结构，将数据格式化到表的结构里。当DROP TABLE table_name 的时候，hive仅仅会删除元数据的表结构，而不会删除hdfs上的文件，所以，相比内部表，外部表可以更放心大胆的使用。

```
create external table hb_range_keys(transaction_id_range_start string)
row format serde 
'org.apache.hadoop.hive.serde2.binarysortable.BinarySortableSerDe'
stored as 
inputformat 
'org.apache.hadoop.mapred.TextInputFormat'
outputformat 
'org.apache.hadoop.hive.ql.io.HiveNullValueSequenceFileOutputFormat'
location '/tmp/hb_range_keys';

insert overwrite table hb_range_keys
select transaction_id from
(select transaction_id
from transactions
tablesample(bucket 1 out of 10000 on transaction_id) s 
order by transaction_id 
limit 10000000) x
where (row_sequence() % 910000)=0
order by transaction_id
limit 11;
```



这里只保存数据的结构，应该就是1---910000，910001---2*910000...这样的一个分区结构



