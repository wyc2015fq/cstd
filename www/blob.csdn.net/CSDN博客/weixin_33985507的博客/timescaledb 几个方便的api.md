# timescaledb 几个方便的api - weixin_33985507的博客 - CSDN博客
2018年06月28日 13:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：37
timescaledb 提供了内置的api 操作，方便我们进行操作控制
## hypertable 控制api
- add_dimension
> 
向hypertable添加一个额外的分片方式，可以做为分片列有时间间隔的以及hash分片
必须参数
|名称|说明|
|----|----|
|`main_table`|hypertable 名称.|
|`column_name`|使用的分片列|
可选参数
|名称|说明|
|----|----|
|`number_partitions`|hash 分片个数 `column_name`. 必须 > 0.|
|`chunk_time_interval`|每个chunk 的间隔 必须 > 0.|
|`partitioning_func`|分片函数|
|`if_not_exists`|避免对于已经存在而进行创建是报错|
参考：
```
SELECT create_hypertable('conditions', 'time');
SELECT add_dimension('conditions', 'location', number_partitions => 4);
```
- attach_tablespace
> 
给hypertable 附加tablespace 信息，数据chunk 会按照此进行存储
必须参数
|名称|描述|
|----|----|
|`tablespace`|tablespace|
|`hypertable`|hypertable 名称|
附加tablespace 之前必须创建好，tablespace 可以附加给多个hypertable 
可选参数
|名称|描述|
|----|----|
|`if_not_attached`|避免对于已经存在而进行创建是报错|
参考：
```
SELECT attach_tablespace('disk1', 'conditions'); 
SELECT attach_tablespace('disk2', 'conditions', if_not_attached => true);
```
- create_hypertable
> 
创建hypertable
必须参数
|名称|描述|
|----|----|
|`main_table`|将要转换为hypertable的表|
|`time_column_name`|做为时间指的分片列|
可选参数
|名称|描述|
|----|----|
|`partitioning_column`|分片列|
|`number_partitions`|分片个数|
|`chunk_time_interval`|每个chunk的间隔|
|`create_default_indexes`|分片列索引创建默认是true|
|`if_not_exists`|避免对于已经存在而进行创建是报错|
|`partitioning_func`|分片函数|
|`associated_schema_name`|内部hypertable 关联的schema默认为_timescaledb_internal|
|`associated_table_prefix`|hypertable chunk 数据表的后缀|
|`migrate_data`|是否进行数据迁移，会话时间，具体的可以参考官方文档|
参考
```
SELECT create_hypertable('conditions', 'time', chunk_time_interval => 86400000000); 
SELECT create_hypertable('conditions', 'time', chunk_time_interval => interval '1 day');
```
最佳实践： 具体的可以参考官方文档
- drop_chunks
> 
删除chunk 的数据
必须参数
|名称|描述|
|----|----|
|`older_than`|Timestamp of cut-off point for data to be dropped, i.e., anything older than this should be removed.|
可选参数
|名称|描述|
|----|----|
|`table_name`|hypertable 名称|
|`schema_name`|schema 默认是public|
|`cascade`|是否进行及联数据删除|
参考
`SELECT drop_chunks(interval '3 months', 'conditions');`
## 分析类函数Analytics
> 
方便进行时序分析的一些函数，比如first 、last、time_bucket
- first
必须参数
|名称|描述|
|----|----|
|`value`|The value to return (anyelement)|
|`time`|The timestamp to use for comparison (TIMESTAMP/TIMESTAMPTZ or integer type)|
参考
```
SELECT device_id, first(temp, time) 
FROM metrics 
GROUP BY device_id;
```
- last
必须参数
|名称|参数|
|----|----|
|`value`|The value to return (anyelement)|
|`time`|The timestamp to use for comparison (TIMESTAMP/TIMESTAMPTZ or integer type)|
参考
```
SELECT device_id, time_bucket('5 minutes', time) as interval, last(temp, time) 
FROM metrics WHERE time > now () - interval '1 day'
GROUP BY device_id, interval 
ORDER BY interval DESC;
```
## 工具类
> 
比较多，主要有以下几个
- chunk_relation_size
- chunk_relation_size_pretty
- hypertable_relation_size
- hypertable_relation_size_pretty
- show_tablespaces
## 参考资料
[https://docs.timescale.com/v0.9/api#hypertable-management](https://docs.timescale.com/v0.9/api#hypertable-management)

