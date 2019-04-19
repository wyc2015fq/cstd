# hive表的存储格式 . ORC格式的使用 - 数据之美的博客 - CSDN博客
2018年01月24日 11:37:20[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：267
### hive表的源文件存储格式有几类：
```
1、TEXTFILE  
  默认格式，建表时不指定默认为这个格式，导入数据时会直接把数据文件拷贝到hdfs上不进行处理。源文件可以直接通过hadoop fs -cat 查看
  2、SEQUENCEFILE  一种Hadoop API提供的二进制文件，使用方便、可分割、可压缩等特点。
    SEQUENCEFILE将数据以<key,value>的形式序列化到文件中。序列化和反序列化使用Hadoop 的标准的Writable 接口实现。key为空，用value 存放实际的值， 这样可以避免map 阶段的排序过程。
    三种压缩选择：NONE, RECORD, BLOCK。 Record压缩率低，一般建议使用BLOCK压缩。使用时设置参数，
        SET hive.exec.compress.output=true;
        SET io.seqfile.compression.type=BLOCK; -- NONE/RECORD/BLOCK
        create table test2(str STRING)  STORED AS SEQUENCEFILE; 
  3、RCFILE  
    一种行列存储相结合的存储方式。首先，其将数据按行分块，保证同一个record在一个块上，避免读一个记录需要读取多个block。其次，块数据列式存储，有利于数据压缩和快速的列存取。
      理论上具有高查询效率（但hive官方说效果不明显，只有存储上能省10%的空间，所以不好用，可以不用）。
      RCFile结合行存储查询的快速和列存储节省空间的特点
        1）同一行的数据位于同一节点，因此元组重构的开销很低；
        2) 块内列存储，可以进行列维度的数据压缩，跳过不必要的列读取。
      查询过程中，在IO上跳过不关心的列。实际过程是，在map阶段从远端拷贝仍然拷贝整个数据块到本地目录，也并不是真正直接跳过列，而是通过扫描每一个row group的头部定义来实现的。
      但是在整个HDFS Block 级别的头部并没有定义每个列从哪个row group起始到哪个row group结束。所以在读取所有列的情况下，RCFile的性能反而没有SequenceFile高。
  4、ORC hive给出的新格式，属于RCFILE的升级版。
  5、自定义格式 用户的数据文件格式不能被当前 Hive 所识别的，时通过实现inputformat和outputformat来自定义输入输出格式，
  参考代码：.\hive-0.8.1\src\contrib\src\java\org\apache\hadoop\hive\contrib\fileformat\base64
  对前集中的介绍和建表语句参见：http://www.cnblogs.com/ggjucheng/archive/2013/01/03/2843318.html
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
##### 注意：
```
只有TEXTFILE表能直接加载数据，必须，本地load数据，和external外部表直接加载运路径数据，都只能用TEXTFILE表。
  更深一步，hive默认支持的压缩文件（hadoop默认支持的压缩格式），也只能用TEXTFILE表直接读取。其他格式不行。可以通过TEXTFILE表加载后insert到其他表中。
  换句话说，SequenceFile、RCFile表不能直接加载数据，数据要先导入到textfile表，再从textfile表通过insert select from 导入到SequenceFile,RCFile表。
  SequenceFile、RCFile表的源文件不能直接查看，在hive中用select看。RCFile源文件可以用 hive --service rcfilecat /xxxxxxxxxxxxxxxxxxxxxxxxxxx/000000_0查看，但是格式不同，很乱。
  hive默认支持压缩文件格式参考http://blog.csdn.net/longshenlmj/article/details/50550580
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
### ORC格式
```
ORC是RCfile的升级版，性能有大幅度提升，
    而且数据可以压缩存储，压缩比和Lzo压缩差不多，比text文件压缩比可以达到70%的空间。而且读性能非常高，可以实现高效查询。
    具体介绍https://cwiki.apache.org/confluence/display/Hive/LanguageManual+ORC
建表语句如下：
  同时，将ORC的表中的NULL取值，由默认的\N改为'',
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
#### 方式一
```
create table if not exists test_orc(
  advertiser_id string,
  ad_plan_id string,
  cnt BIGINT
) partitioned by (day string, type TINYINT COMMENT '0 as bid, 1 as win, 2 as ck', hour TINYINT)
STORED AS ORC;
alter table test_orc set serdeproperties('serialization.null.format' = '');
查看结果
hive> show create table test_orc;
CREATE  TABLE `test_orc`(
  `advertiser_id` string, 
  `ad_plan_id` string, 
  `cnt` bigint)
PARTITIONED BY ( 
  `day` string, 
  `type` tinyint COMMENT '0 as bid, 1 as win, 2 as ck', 
  `hour` tinyint)
ROW FORMAT DELIMITED 
  NULL DEFINED AS '' 
STORED AS INPUTFORMAT 
  'org.apache.hadoop.hive.ql.io.orc.OrcInputFormat' 
OUTPUTFORMAT 
  'org.apache.hadoop.hive.ql.io.orc.OrcOutputFormat'
LOCATION
  'hdfs://namenode/hivedata/warehouse/pmp.db/test_orc'
TBLPROPERTIES (
  'last_modified_by'='pmp_bi', 
  'last_modified_time'='1465992624', 
  'transient_lastDdlTime'='1465992624')
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
#### 方式二
```
drop table test_orc;
create table if not exists test_orc(
  advertiser_id string,
  ad_plan_id string,
  cnt BIGINT
) partitioned by (day string, type TINYINT COMMENT '0 as bid, 1 as win, 2 as ck', hour TINYINT)
ROW FORMAT SERDE 
  'org.apache.hadoop.hive.ql.io.orc.OrcSerde' 
with serdeproperties('serialization.null.format' = '')
STORED AS ORC;
查看结果
hive> show create table test_orc;
CREATE  TABLE `test_orc`(
  `advertiser_id` string, 
  `ad_plan_id` string, 
  `cnt` bigint)
PARTITIONED BY ( 
  `day` string, 
  `type` tinyint COMMENT '0 as bid, 1 as win, 2 as ck', 
  `hour` tinyint)
ROW FORMAT DELIMITED 
  NULL DEFINED AS '' 
STORED AS INPUTFORMAT 
  'org.apache.hadoop.hive.ql.io.orc.OrcInputFormat' 
OUTPUTFORMAT 
  'org.apache.hadoop.hive.ql.io.orc.OrcOutputFormat'
LOCATION
  'hdfs://namenode/hivedata/warehouse/pmp.db/test_orc'
TBLPROPERTIES (
  'transient_lastDdlTime'='1465992726')
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
#### 方式三
```
drop table test_orc;
create table if not exists test_orc(
  advertiser_id string,
  ad_plan_id string,
  cnt BIGINT
) partitioned by (day string, type TINYINT COMMENT '0 as bid, 1 as win, 2 as ck', hour TINYINT)
ROW FORMAT DELIMITED 
  NULL DEFINED AS '' 
STORED AS ORC;
查看结果
hive> show create table test_orc;
CREATE  TABLE `test_orc`(
  `advertiser_id` string, 
  `ad_plan_id` string, 
  `cnt` bigint)
PARTITIONED BY ( 
  `day` string, 
  `type` tinyint COMMENT '0 as bid, 1 as win, 2 as ck', 
  `hour` tinyint)
ROW FORMAT DELIMITED 
  NULL DEFINED AS '' 
STORED AS INPUTFORMAT 
  'org.apache.hadoop.hive.ql.io.orc.OrcInputFormat' 
OUTPUTFORMAT 
  'org.apache.hadoop.hive.ql.io.orc.OrcOutputFormat'
LOCATION
  'hdfs://namenode/hivedata/warehouse/pmp.db/test_orc'
TBLPROPERTIES (
  'transient_lastDdlTime'='1465992916')
```
