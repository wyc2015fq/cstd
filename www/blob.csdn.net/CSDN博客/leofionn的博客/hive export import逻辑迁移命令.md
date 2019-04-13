
# hive export-import逻辑迁移命令 - leofionn的博客 - CSDN博客


2018年01月20日 15:37:24[leofionn](https://me.csdn.net/qq_36142114)阅读数：268


## 具体语法如下
```python
EXPORT TABLE tablename [PARTITION (part_column=
```
```python
"value"
```
```python
[,
```
```python
...
```
```python
])]
  TO
```
```python
'export_target_path'
```
```python
[ FOR replication(
```
```python
'eventid'
```
```python
) ]
IMPORT [[EXTERNAL] TABLE new_or_original_tablename [PARTITION (part_column=
```
```python
"value"
```
```python
[,
```
```python
...
```
```python
])]]
  FROM
```
```python
'source_path'
```
```python
[LOCATION
```
```python
'import_target_path'
```
```python
]
```
1
2
3
4
5
6
## 例1 简单表的导入导出：
```python
hive
use test1;
export table emp
```
```python
to
```
```python
'/user/hive/warehouse/exp_emp';
```
```python
exit
```
```python
;
hdfs dfs -ls /user/hive/warehouse/exp_emp
```
```python
# 将表emp导出到hdfs的/user/hive/warehouse/exp_emp目录中
```
```python
# 导出的数据包括"_metadata"和"data"两个文件
```
```python
# 将该目录 hdfs dfs -get 下来
```
```python
# 将该目录 hdfs dfs -put 到另外一套hadoop集群之上
```
```python
# 当然实验的话也可以直接放到一个hadoop集群的另外的一个库上
```
```python
hive
use test4;
show tables;
import
```
```python
from
```
```python
'/user/hive/warehouse/exp_emp';
```
```python
show tables;
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
test1.emp;
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
test4.emp;
```
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
## 例2 简单表的导出改名导入：
```python
hive
use test1;
export table emp to '/user/hive/warehouse/exp_emp';
exit;
hdfs dfs -ls /user/hive/warehouse/exp_emp
hive
use test4;
```
```python
show
```
```python
tables;
```
```python
import table rn_emp from '/user/hive/warehouse/exp_emp';
```
```python
show
```
```python
tables;
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
test1.emp;
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
test4.emp;
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
test4.rn_emp;
```
1
2
3
4
5
6
7
8
9
10
11
12
13
14
## 例3 分区表导入导出：
```python
hive
use test1;
export table emp_partition 
 partition (deptno=
```
```python
"10"
```
```python
)
```
```python
to
```
```python
'/user/hive/warehouse/exp_emp_part';
```
```python
exit
```
```python
;
hive
use test4;
import
```
```python
from
```
```python
'/user/hive/warehouse/exp_emp_part';
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
test1.emp_partition
```
```python
where
```
```python
deptno=
```
```python
10
```
```python
;
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
test4.emp_partition;
```
1
2
3
4
5
6
7
8
9
10
11
12
## 例4 分区表导出，改名装入某个分区：
```python
hive
use test1;
export table emp_partition
```
```python
to
```
```python
'/user/hive/warehouse/exp_emp_part_full';
```
```python
exit
```
```python
;
hive
use test4;
import table emp_partition_rn partition (deptno=
```
```python
"10"
```
```python
)
```
```python
from
```
```python
'/user/hive/warehouse/exp_emp_part_full';
```
```python
-- 这个地方做了改名和分区过滤之后的导入
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
test1.emp_partition;
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
test4.emp_partition_rn;
```
1
2
3
4
5
6
7
8
9
10
11
12
13
## 例5 分区表导出，改名装入某个分区并指定hdfs目录：
```python
hive
```
```python
use
```
```python
test1
```
```python
;
export table emp_partition
 to
```
```python
'/user/hive/warehouse/exp_emp_part_full'
```
```python
;
```
```python
exit
```
```python
;
hive
```
```python
use
```
```python
test4
```
```python
;
import table emp_partition_rn2 partition (deptno=
```
```python
"10"
```
```python
)
  from
```
```python
'/user/hive/warehouse/exp_emp_part_full'
```
```python
location
```
```python
'/user/hive/warehouse/exp_emp_part_full_rn2'
```
```python
;
-- 增加了location参数指定hdfs对应的目录
```
1
2
3
4
5
6
7
8
9
10
11
12
## 例6 内部表到外部表的导入导出转换：
```python
hive
```
```python
use
```
```python
test1
```
```python
;
export table emp to
```
```python
'/user/hive/warehouse/exp_emp_ext'
```
```python
;
```
```python
exit
```
```python
;
hive
```
```python
use
```
```python
test4
```
```python
;
show tables;
import external table rn_emp from
```
```python
'/user/hive/warehouse/exp_emp_ext'
```
```python
;
```
1
2
3
4
5
6
7
8
9
`[TOC]`
`来自@若泽大数据`



