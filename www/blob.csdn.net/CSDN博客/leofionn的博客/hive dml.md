
# hive dml - leofionn的博客 - CSDN博客


2018年01月20日 15:33:22[leofionn](https://me.csdn.net/qq_36142114)阅读数：75


## LOAD操作
HIVE的DML，从OS或者hdfs中load数据到表相关：
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
create
```
```python
table
```
```python
dept(
deptno
```
```python
int
```
```python
,
dname  string,
loc    string)
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
-- 创建一张dept表
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
'/tmp/dept'
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
dept;
```
```python
-- 从OS文件装载数据到表
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
into
```
```python
table
```
```python
emp;
```
```python
-- 再次上传，不使用关键字overwrite，追加数据到表中
```
```python
-- 手动上传/tmp/emp为hdfs的一个目录下的test文件
```
```python
-- 注意该目录不能和当前表一个目录
```
```python
-- hdfs dfs -put /tmp/emp /user/hive/warehouse/test1.db/test
```
```python
load
```
```python
data inpath
```
```python
'/user/hive/warehouse/test1.db/test'
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
-- 将hdfs中的一个文件overwrite到emp表中
```
```python
-- 该操作实际上是将hdfs的对应文件移动到了emp表对应的目录中
```
```python
-- 因此该文件不能是emp目录中的文件
```
```python
-- hdfs dfs -put /tmp/emp /user/hive/warehouse/test1.db/test
```
```python
load
```
```python
data inpath
```
```python
'/user/hive/warehouse/test1.db/test'
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
-- 再次关联，不使用关键字overwrite，追加数据到表中
```
## INSERT到表的操作
HIVE的DML，insert表数据到另外一张表：
```python
INSERT
```
```python
OVERWRITE
```
```python
TABLE
```
```python
tablename1
  [PARTITION (partcol1=val1, partcol2=val2 ...)
  [
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
]]
  select_statement1
```
```python
FROM
```
```python
from_statement;
```
```python
INSERT
```
```python
INTO
```
```python
TABLE
```
```python
tablename1
  [PARTITION (partcol1=val1, partcol2=val2 ...)]
  select_statement1
```
```python
FROM
```
```python
from_statement;
```
```python
insert
```
```python
overwrite
```
```python
table
```
```python
emp2
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
-- 表emp2结构和select子句查出来的列数量和类型要一致，否则报错
```
```python
-- 如果列数量和类型一致，但是列顺序相反，则会造成业务上的问题
```
```python
-- 比如将ename和job插入成了job和ename，或者俩job
```
```python
-- 不会报错，但是表的数据就是混乱的了
```
```python
insert
```
```python
into
```
```python
table
```
```python
emp2
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
-- 注意点和上方一样，该操作是追加数据
```

## INSERT到目录的操作
HIVE的DML，insert表数据到OS目录或hdfs目录相关：
```python
INSERT
```
```python
OVERWRITE [
```
```python
LOCAL
```
```python
] DIRECTORY directory1
  [
```
```python
ROW
```
```python
FORMAT row_format] [STORED
```
```python
AS
```
```python
file_format]
```
```python
SELECT
```
```python
...
```
```python
FROM
```
```python
...;
```
```python
use test1;
```
```python
INSERT
```
```python
OVERWRITE
```
```python
LOCAL
```
```python
DIRECTORY
```
```python
'/tmp/test_emp'
```
```python
ROW
```
```python
FORMAT DELIMITED FIELDS TERMINATED
```
```python
BY
```
```python
"\t"
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
-- 会查询test1.emp表，写入操作系统的/tmp/test_emp目录中的000000_0文件
```
```python
-- 目录和文件会自动创建，当然运行hive的用户要有相应的OS权限才可以
```
```python
INSERT
```
```python
OVERWRITE DIRECTORY
```
```python
'/user/hive/warehouse/insert_test_emp'
```
```python
ROW
```
```python
FORMAT DELIMITED FIELDS TERMINATED
```
```python
BY
```
```python
"\t"
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
-- 查询test1.emp表，写入hdfs的/user/hive/warehouse/insert_test_emp目录的000000_0文件
```
```python
-- 目录和文件会自动创建，当然运行hive的用户要有相应的hdfs权限才可以
```
```python
FROM from_statement
```
```python
INSERT
```
```python
OVERWRITE [
```
```python
LOCAL
```
```python
] DIRECTORY directory1 select_statement1
[
```
```python
INSERT
```
```python
OVERWRITE [
```
```python
LOCAL
```
```python
] DIRECTORY directory2 select_statement2] ...;
```
```python
from (
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
emp) tmp
```
```python
INSERT
```
```python
OVERWRITE
```
```python
LOCAL
```
```python
DIRECTORY
```
```python
'/tmp/hivetmp1'
```
```python
ROW
```
```python
FORMAT DELIMITED FIELDS TERMINATED
```
```python
BY
```
```python
"\t"
```
```python
select
```
```python
empno, ename
```
```python
INSERT
```
```python
OVERWRITE DIRECTORY
```
```python
'/user/hive/warehouse/hivetmp2'
```
```python
ROW
```
```python
FORMAT DELIMITED FIELDS TERMINATED
```
```python
BY
```
```python
"\t"
```
```python
select
```
```python
ename;
```
```python
-- 从一个from子句，插入数据到多个目录
```
```python
-- 如果数据来自一个from子句，那么这个子句必须起一个别名
```
```python
from emp
```
```python
INSERT
```
```python
OVERWRITE
```
```python
LOCAL
```
```python
DIRECTORY
```
```python
'/tmp/hivetmp2'
```
```python
ROW
```
```python
FORMAT DELIMITED FIELDS TERMINATED
```
```python
BY
```
```python
"\t"
```
```python
select
```
```python
empno, ename
```
```python
INSERT
```
```python
OVERWRITE DIRECTORY
```
```python
'/user/hive/warehouse/hivetmp3'
```
```python
ROW
```
```python
FORMAT DELIMITED FIELDS TERMINATED
```
```python
BY
```
```python
"\t"
```
```python
select
```
```python
ename;
```
```python
-- 从一个表，插入数据到多个目录
```

## SELECT操作
HIVE的DML，select相关：跟通用的SQL是一样的，略。
分组聚合会产生数据倾斜，而 union all 常用于处理数据倾斜问题。
CASE语句和通用的SQL语法中的CASE一样的：
```python
select
```
```python
ename, salary,
```
```python
case
```
```python
when
```
```python
salary >
```
```python
1
```
```python
and
```
```python
salary <=
```
```python
1000
```
```python
then
```
```python
'LOWER'
```
```python
when
```
```python
salary >
```
```python
1000
```
```python
and
```
```python
salary <=
```
```python
2000
```
```python
then
```
```python
'MIDDLE'
```
```python
when
```
```python
salary >
```
```python
2000
```
```python
and
```
```python
salary <=
```
```python
4000
```
```python
then
```
```python
'HIGH'
```
```python
else
```
```python
'HIGHEST'
```
```python
end
```
```python
from
```
```python
emp;
```





# 来自@若泽大数据



