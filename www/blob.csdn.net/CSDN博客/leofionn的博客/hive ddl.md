
# hive ddl - leofionn的博客 - CSDN博客


2018年01月20日 15:34:58[leofionn](https://me.csdn.net/qq_36142114)阅读数：143


## Database相关DDL
hive部署启动后，会有一个默认的hive数据库：default；
hive中的一个库对应hdfs中的一个目录；
default库对应的hdfs目录为：/user/hive/warehouse。
```python
hive
```
```python
show
```
```python
databases;
```
```python
-- OK
```
```python
-- default
```
```python
-- Time taken: 0.024 seconds, Fetched: 1 row(s)
```
```python
-- 默认库为default，进入hive后默认使用该库
```
```python
create
```
```python
table
```
```python
test(id
```
```python
int
```
```python
)
```
```python
row
```
```python
format delimited fields terminated
```
```python
by
```
```python
'\t'
```
```python
;
```
```python
-- 创建一张测试表，查看hdfs对应的信息
```
```python
exit;
# 退出hive，使用hdfs查看default库对应的目录
hdfs dfs -ls /user/hive/warehouse
# Found 1 items
# drwxrwxrwt - root hive 0 2018-01-05 23:08 /user/hive/warehouse/test
# 看到default库下创建的test表，实际上就是/user/hive/warehouse目录下的test目录
```
Create Database 语法和常用命令：
```python
CREATE
```
```python
(
```
```python
DATABASE
```
```python
|
```
```python
SCHEMA
```
```python
) [
```
```python
IF
```
```python
NOT
```
```python
EXISTS
```
```python
] database_name
 [COMMENT database_comment]
 [LOCATION hdfs_path]
 [
```
```python
WITH
```
```python
DBPROPERTIES (property_name=property_value, ...)];
```
```python
-- 已上是建库的全部语法
```
```python
-- 常用写法：
```
```python
CREATE
```
```python
DATABASE
```
```python
IF
```
```python
NOT
```
```python
EXISTS
```
```python
test1;
```
```python
-- 创建一个库，使用 IF NOT EXISTS 防止库存在时报错
```
```python
-- 对应hdfs：/user/hive/warehouse/test1.db
```
```python
-- 对应metadata：MYSQL的HIVE库中DBS表
```
```python
CREATE
```
```python
DATABASE
```
```python
IF
```
```python
NOT
```
```python
EXISTS
```
```python
test2
  COMMENT
```
```python
'This is a test database.'
```
```python
;
```
```python
-- 加注释信息
```
```python
-- 对应hdfs：/user/hive/warehouse/test2.db
```
```python
-- 对应metadata：MYSQL的HIVE库中DBS表的DESC列
```
```python
CREATE
```
```python
DATABASE
```
```python
IF
```
```python
NOT
```
```python
EXISTS
```
```python
test3
  COMMENT
```
```python
'This is a test database.'
```
```python
LOCATION
```
```python
'/user/hive/warehouse/test3.abc'
```
```python
;
```
```python
-- 对应hdfs：/user/hive/warehouse/test3.abc
```
```python
-- 需要使用hdfs命令预先创建该目录
```
```python
-- 对应metadata：MYSQL的HIVE库中DBS表的DESC列和DB_LOCATION_URI列
```
```python
CREATE
```
```python
DATABASE
```
```python
IF
```
```python
NOT
```
```python
EXISTS
```
```python
test4
  COMMENT
```
```python
'This is a test database.'
```
```python
WITH
```
```python
DBPROPERTIES (
```
```python
"creator"
```
```python
=
```
```python
"vincent"
```
```python
,
```
```python
"date"
```
```python
=
```
```python
"2018-01-09"
```
```python
);
```
```python
-- 使用WITH DBPROPERTIES参数，以键值对格式传入一些相信信息
```
Drop Database 命令：
删库命令是很危险的命令，会删除对应的mysql中的元数据，生产慎用。
```python
DROP
```
```python
(
```
```python
DATABASE
```
```python
|
```
```python
SCHEMA
```
```python
) [
```
```python
IF
```
```python
EXISTS
```
```python
] database_name [
```
```python
RESTRICT
```
```python
|
```
```python
CASCADE
```
```python
];
```
```python
-- 删库的全部语法
```
```python
use test3;
```
```python
create
```
```python
table
```
```python
test(id
```
```python
int
```
```python
)
```
```python
row
```
```python
format delimited fields terminated
```
```python
by
```
```python
'\t'
```
```python
;
```
```python
-- 在test3库中创建测试表
```
```python
drop
```
```python
database
```
```python
IF
```
```python
EXISTS
```
```python
test3
```
```python
cascade
```
```python
;
```
```python
-- 当库中有表的时候，需要使用cascade关键字级联删除
```
```python
-- 使用 IF EXISTS 防止库不存在时报错
```
```python
-- 删除库会不自动删除该目录和对应子目录
```
```python
use test2;
```
```python
create
```
```python
table
```
```python
test(id
```
```python
int
```
```python
)
```
```python
row
```
```python
format delimited fields terminated
```
```python
by
```
```python
'\t'
```
```python
;
```
```python
drop
```
```python
database
```
```python
IF
```
```python
EXISTS
```
```python
test2
```
```python
cascade
```
```python
;
```
```python
-- 删除库会不自动删除该目录和对应子目录
```
```python
drop
```
```python
database
```
```python
IF
```
```python
EXISTS
```
```python
test1;
```
Alter Database 命令：
```python
ALTER
```
```python
(
```
```python
DATABASE
```
```python
|
```
```python
SCHEMA
```
```python
) database_name
```
```python
SET
```
```python
DBPROPERTIES (property_name=property_value,
...);
```
```python
-- (Note: SCHEMA added in Hive 0.14.0)
```
```python
ALTER
```
```python
(
```
```python
DATABASE
```
```python
|
```
```python
SCHEMA
```
```python
) database_name
```
```python
SET
```
```python
OWNER [
```
```python
USER
```
```python
|ROLE] user_or_role;
```
```python
-- (Note:
```
```python
Hive 0.13.0 and later; SCHEMA added in Hive 0.14.0)
```
```python
ALTER
```
```python
(
```
```python
DATABASE
```
```python
|
```
```python
SCHEMA
```
```python
) database_name
```
```python
SET
```
```python
LOCATION hdfs_path;
```
```python
-- (Note: Hive 2.2.1,
```
```python
2.4.0 and later)
```
```python
-- 修改库的语法
```
```python
CREATE
```
```python
DATABASE
```
```python
IF
```
```python
NOT
```
```python
EXISTS
```
```python
test1;
```
```python
ALTER
```
```python
DATABASE
```
```python
test1
```
```python
SET
```
```python
DBPROPERTIES (
```
```python
"CREATE_TIME"
```
```python
=
```
```python
"20180105"
```
```python
,
```
```python
"GRANTOR_test"
```
```python
=
```
```python
"Vincent"
```
```python
);
```
```python
-- 修改库设置相关的参数信息，键值对方式
```
```python
-- 该信息保存在MYSQL中的对应库的DATABASE_PARAMS表中
```
Use Database 命令：
```python
USE database_name;
USE DEFAULT;
```
```python
show
```
```python
databases
```
```python
like
```
```python
'test*'
```
```python
;
```
```python
-- 查看库可以使用通配符
```
```python
use test1;
```
```python
SELECT
```
```python
current_database();
```
```python
-- 查询当前的库
```
```python
desc database test1;
```
```python
-- 查询库的详细信息
```
```python
desc database extended test1;
```
```python
-- 查询更多的信息
```
## Table相关DDL
Create Table 语法和常用命令：
```python
CREATE [TEMPORARY] [EXTERNAL] TABLE [IF NOT EXISTS] [db_name.]table_name
 -- (Note: TEMPORARY available
```
```python
in
```
```python
Hive
```
```python
0.14
```
```python
.0
```
```python
and later)
  [(col_name data_type [COMMENT col_comment],
```
```python
...
```
```python
[constraint_specification])]
  [COMMENT table_comment]
  [PARTITIONED BY (col_name data_type [COMMENT col_comment],
```
```python
...
```
```python
)]
  [CLUSTERED BY (col_name, col_name,
```
```python
...
```
```python
) [SORTED BY (col_name [ASC|DESC],
```
```python
...
```
```python
)] INTO
num_buckets BUCKETS]
  [SKEWED BY (col_name, col_name,
```
```python
...
```
```python
)
   -- (Note: Available
```
```python
in
```
```python
Hive
```
```python
0.10
```
```python
.0
```
```python
and later)]
  ON ((col_value, col_value,
```
```python
...
```
```python
), (col_value, col_value,
```
```python
...
```
```python
),
```
```python
...
```
```python
)
  [STORED AS DIRECTORIES]
  [
   [ROW FORMAT row_format]
   [STORED AS file_format]
   | STORED BY
```
```python
'storage.handler.class.name'
```
```python
[WITH SERDEPROPERTIES (
```
```python
...
```
```python
)]
    -- (Note: Available
```
```python
in
```
```python
Hive
```
```python
0.6
```
```python
.0
```
```python
and later)
  ]
  [LOCATION hdfs_path]
  [TBLPROPERTIES (property_name=property_value,
```
```python
...
```
```python
)]
   -- (Note: Available
```
```python
in
```
```python
Hive
```
```python
0.6
```
```python
.0
```
```python
and later)
  [AS select_statement]; 
   -- (Note: Available
```
```python
in
```
```python
Hive
```
```python
0.5
```
```python
.0
```
```python
and later; not supported
```
```python
for
```
```python
external tables)
CREATE [TEMPORARY] [EXTERNAL] TABLE [IF NOT EXISTS] [db_name.]table_name
 LIKE existing_table_or_view_name [LOCATION hdfs_path];
data_type
 : primitive_type
 | array_type
 | map_type
 | struct_type
 | union_type -- (Note: Available
```
```python
in
```
```python
Hive
```
```python
0.7
```
```python
.0
```
```python
and later)
primitive_type
 : TINYINT
 | SMALLINT
 | INT
 | BIGINT
 | BOOLEAN
 | FLOAT
 | DOUBLE
 | DOUBLE PRECISION -- (Note: Available
```
```python
in
```
```python
Hive
```
```python
2.2
```
```python
.0
```
```python
and later)
 | STRING
 | BINARY -- (Note: Available
```
```python
in
```
```python
Hive
```
```python
0.8
```
```python
.0
```
```python
and later)
 | TIMESTAMP -- (Note: Available
```
```python
in
```
```python
Hive
```
```python
0.8
```
```python
.0
```
```python
and later)
 | DECIMAL -- (Note: Available
```
```python
in
```
```python
Hive
```
```python
0.11
```
```python
.0
```
```python
and later)
 | DECIMAL(precision, scale) -- (Note: Available
```
```python
in
```
```python
Hive
```
```python
0.13
```
```python
.0
```
```python
and later)
 | DATE -- (Note: Available
```
```python
in
```
```python
Hive
```
```python
0.12
```
```python
.0
```
```python
and later)
 | VARCHAR -- (Note: Available
```
```python
in
```
```python
Hive
```
```python
0.12
```
```python
.0
```
```python
and later)
 | CHAR -- (Note: Available
```
```python
in
```
```python
Hive
```
```python
0.13
```
```python
.0
```
```python
and later)
array_type
 : ARRAY < data_type >
map_type
 : MAP < primitive_type, data_type >
struct_type
 : STRUCT < col_name : data_type [COMMENT col_comment],
```
```python
...
```
```python
>
union_type
 : UNIONTYPE < data_type, data_type,
```
```python
...
```
```python
> -- (Note: Available
```
```python
in
```
```python
Hive
```
```python
0.7
```
```python
.0
```
```python
and
later)
row_format
 : DELIMITED [FIELDS TERMINATED BY char [ESCAPED BY char]] [COLLECTION ITEMS
TERMINATED BY char]
 [MAP KEYS TERMINATED BY char] [LINES TERMINATED BY char]
 [
```
```python
NULL
```
```python
DEFINED AS char] -- (Note: Available
```
```python
in
```
```python
Hive
```
```python
0.13
```
```python
and later)
 | SERDE serde_name [WITH SERDEPROPERTIES (property_name=property_value,
property_name=property_value,
```
```python
...
```
```python
)]
file_format:
 : SEQUENCEFILE
 | TEXTFILE -- (Default, depending on hive.default.fileformat configuration)
 | RCFILE -- (Note: Available
```
```python
in
```
```python
Hive
```
```python
0.6
```
```python
.0
```
```python
and later)
 | ORC -- (Note: Available
```
```python
in
```
```python
Hive
```
```python
0.11
```
```python
.0
```
```python
and later)
 | PARQUET -- (Note: Available
```
```python
in
```
```python
Hive
```
```python
0.13
```
```python
.0
```
```python
and later)
 | AVRO -- (Note: Available
```
```python
in
```
```python
Hive
```
```python
0.14
```
```python
.0
```
```python
and later)
 | INPUTFORMAT input_format_classname OUTPUTFORMAT output_format_classname
constraint_specification:
 : [, PRIMARY KEY (col_name,
```
```python
...
```
```python
) DISABLE NOVALIDATE ]
 [, CONSTRAINT constraint_name FOREIGN KEY (col_name,
```
```python
...
```
```python
) REFERENCES
table_name(col_name,
```
```python
...
```
```python
) DISABLE NOVALIDATE
```
建表相关的语法太多，现在简单整理下常用的建表语句：
```python
use test1;
```
```python
create
```
```python
table
```
```python
emp(
empno
```
```python
int
```
```python
, 
ename string, 
job string, 
mgr
```
```python
int
```
```python
, 
hiredate string, 
sal
```
```python
double
```
```python
, 
comm
```
```python
double
```
```python
, 
deptno
```
```python
int
```
```python
)
```
```python
row
```
```python
format delimited fields terminated
```
```python
by
```
```python
'\t'
```
```python
;
```
```python
-- 创建一张emp表
```
```python
show
```
```python
create
```
```python
table
```
```python
emp;
```
```python
-- 查看emp表的创建语句
```
```python
show
```
```python
tables
```
```python
'emp*'
```
```python
;
```
```python
-- 使用通配符匹配表
```
```python
load
```
```python
data
```
```python
local
```
```python
inpath
```
```python
'/tmp/emp'
```
```python
overwrite
```
```python
into
```
```python
table
```
```python
emp;
```
```python
-- 将OS上的对应目录下的对应文件上传关联到emp表
```
```python
-- 该文件是oracle的scott.emp表的数据导出，列分隔符为tab
```
```python
-- 使用overwrite 参数覆盖原表
```

使用CTAS从已知表克隆一个表：
```python
create
```
```python
table
```
```python
emp2
```
```python
as
```
```python
select
```
```python
*
```
```python
from
```
```python
emp;
```
```python
-- Job Submission failed with exception 'org.apache.hadoop.security.AccessControlException(Permission denied: user=root, access=WRITE, inode="/user":hdfs:supergroup:drwxr-xr-x
```
```python
-- 如果报该信息错误，则 export HADOOP_USER_NAME=hdfs
```
```python
-- 使用CTAS语句克隆表，数据也会克隆
```
只复制表结构的克隆表：
```python
create
```
```python
table
```
```python
emp3
```
```python
like
```
```python
emp;
```
```python
-- 只克隆表结构，不复制数据
```
查看表信息：
```python
desc
```
```python
emp;
```
```python
desc
```
```python
extended emp;
```
```python
desc
```
```python
formatted emp;
-- 查看表结构，查看表更详细的信息，格式化的查看表更详细的信息
```
Drop Table 和 Truncate Table 以及 Alter Table：
```python
DROP
```
```python
TABLE
```
```python
[
```
```python
IF
```
```python
EXISTS
```
```python
] table_name [PURGE];
```
```python
-- (Note: PURGE available in Hive 0.14.0 and later)
```
```python
TRUNCATE
```
```python
TABLE
```
```python
table_name [PARTITION partition_spec];
```
```python
partition_spec:
 : (partition_column = partition_col_value, partition_column = partition_col_value,
...)
```
```python
ALTER
```
```python
TABLE
```
```python
table_name RENAME
```
```python
TO
```
```python
new_table_name;
```
## Managed和External表区别
表分为两类：Managed内部表和External外部表。
内部表和外部表的区别：
内部表在删除的时候元数据和数据均会被删除，会造成数据丢失，不安全；
外部表在删除的时候，只删除元数据，较为安全。
推荐尽量使用外部表，保证数据安全。
```python
CREATE EXTERNAL TABLE XXX
```
```python
...
```
```python
location
```
```python
'HDFS path'
```
```python
;
-- 使用关键字 EXTERNAL 来创建一张外部表，使用 location 指定路径。
```
实验测试：
```python
use test1;
```
```python
create
```
```python
table
```
```python
emp_MT(
info string)
```
```python
row
```
```python
format delimited fields terminated
```
```python
by
```
```python
'\t'
```
```python
;
```
```python
load
```
```python
data
```
```python
local
```
```python
inpath
```
```python
'/tmp/emp'
```
```python
overwrite
```
```python
into
```
```python
table
```
```python
emp_MT;
```
```python
-- 指定了tab为列分隔符，但是整个表只有一列
```
```python
-- 加载数据时，本地文件emp有多个tab分割的列
```
```python
-- 只会加载第一列进去
```
```python
create
```
```python
EXTERNAL
```
```python
table
```
```python
emp_ET(
info string)
```
```python
row
```
```python
format delimited fields terminated
```
```python
by
```
```python
'\t'
```
```python
;
```
```python
load
```
```python
data
```
```python
local
```
```python
inpath
```
```python
'/tmp/emp'
```
```python
overwrite
```
```python
into
```
```python
table
```
```python
emp_ET;
```
```python
-- hdfs dfs -ls /user/hive/warehouse/test1.db/emp_mt/
```
```python
-- hdfs dfs -ls /user/hive/warehouse/test1.db/emp_et/
```
```python
-- 俩目录是存在的
```
```python
drop
```
```python
table
```
```python
emp_MT purge;
```
```python
drop
```
```python
table
```
```python
emp_et purge;
```
```python
-- hdfs dfs -ls /user/hive/warehouse/test1.db/emp_mt/
```
```python
-- hdfs dfs -ls /user/hive/warehouse/test1.db/emp_et/
```
```python
-- 发现EXTERNAL TABLE对应的目录没有因为表的删除而删除
```
来自@若泽大数据


