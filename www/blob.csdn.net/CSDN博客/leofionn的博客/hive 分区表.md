
# hive 分区表 - leofionn的博客 - CSDN博客


2018年01月20日 15:36:18[leofionn](https://me.csdn.net/qq_36142114)阅读数：120个人分类：[hadoop搭建																](https://blog.csdn.net/qq_36142114/article/category/7383244)



## 1. 相关说明
分区表的一个分区对应hdfs上的一个目录
分区表包括静态分区表和动态分区表，根据分区会不会自动创建来区分






多级分区表，即创建的时候指定 PARTITIONED BY (event_month string,loc string)，根据顺序，级联创建 event_month=XXX/loc=XXX目录，其他和一级的分区表是一样的2. 静态分区表
创建静态分区表，加载数据：
use test1;droptabletest1.order_created_partition purge;CREATETABLEorder_created_partition (
order_number string,
event_time string
)
PARTITIONEDBY(event_month string)ROWFORMAT DELIMITED FIELDS TERMINATEDBY"\t";-- 建表语句，指定分区字段为event_month，这个字段是伪列-- 会在数据load到表的这个分区时，在hdfs上创建名为event_month=2017-12的子目录LOADDATALOCALINPATH"/tmp/order_created.txt"OVERWRITEINTOTABLEorder_created_partition
PARTITION (event_month='2017-12');-- 使用 hdfs dfs -cat .../order_created_partition/event_month=2017-12/order_created.txt-- 查看数据文件中并没有event_month这一列select*fromtest1.order_created_partition;-- 分区表全表扫描，不推荐select*fromtest1.order_created_partitionwhereevent_month='2017-12';-- 使用where子句，过滤分区字段，遍历某个分区-- 以上两个SQL可以查到列event_month信息-- 而使用hdfs dfs -cat看不到该列，说明分区表的分区列是伪列-- 实际上是hdfs中的分区目录的体现1
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
18
19
20
21
22
23
24
25
添加分区，load数据：
altertabletest1.order_created_partitionaddpartition (event_month='2018-01');LOADDATALOCALINPATH"/tmp/order_created.txt"OVERWRITEINTOTABLEorder_created_partition
PARTITION (event_month='2018-01');-- 添加一个分区，将一模一样的数据文件加载到该分区select*fromtest1.order_created_partitionwhereevent_month='2018-01';-- 查到了该分区的记录LOADDATALOCALINPATH"/tmp/order_created.txt"INTOTABLEorder_created_partition
PARTITION (event_month='2018-01');select*fromtest1.order_created_partitionwhereevent_month='2018-01';-- 不使用OVERWRITE参数，会追加数据到分区LOADDATA INPATH"/user/hive/warehouse/test1.db/order_created_partition/event_month=2017-12/order_created.txt"INTOTABLEorder_created_partition
PARTITION (event_month='2018-01');-- 如果从hdfs中加载数据，则原来的路径文件被转移掉1
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
18
19
20
21
可以看出，所谓的分区，是人为定义的，跟业务数据实际上是不是属于该分区没关系，比如将相同的数据分别插入两个分区中，再比如插入的数据有2017-12月份的和2018-01月份的数据
删除分区：
altertabletest1.order_created_partitiondroppartition (event_month='2018-01');-- 从hdfs中已经看不到event_month=2018-01的分区子目录了1
2
3
查询装入数据：
insertoverwritetableorder_created_partition 
 partition(event_month='2017-11')selectorder_number,event_timefromorder_created_partitionwhereevent_month='2018-02';-- 使用查询装入数据到分区中，分区可以是当前不存在的-- 因为查询的是分区表，需要注意伪列和被装入的分区表列的对应关系1
2
3
4
5
6
7
手工创建hdfs目录和文件，添加分区的情况：
静态分区表如果手工创建对应的hdfs目录上传文件，而不使用分区创建命令和load数据到分区的命令，分区表中无法查到该分区信息，需要刷新，这种添加分区的途径是不合法的：
hdfs dfs -mkdir -p /user/hive/warehouse/test1.db/order_created_partition/event_month=2018-02
hdfs dfs -put /tmp/order_created.txt  /user/hive/warehouse/test1.db/order_created_partition/event_month=2018-02
hdfs dfs -ls /user/hive/warehouse/test1.db/order_created_partition/event_month=2018-02
hiveselect*fromtest1.order_created_partitionwhereevent_month='2018-02';-- 此时是查不到该分区的MSCK REPAIR TABLE order_created_partition;-- 修复表信息之后可以查询showpartitions order_created_partition;-- 查看该表的所有分区1
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
3. 动态分区表
use test1;select*fromemp;-- 根据从前实验创建的emp表-- 将emp表的数据按照部门分组，并将数据加载到其对应的分组中去createtableemp_partition(
empnoint, 
ename string, 
job string, 
mgrint, 
hiredate string, 
saldouble, 
commdouble)
PARTITIONEDBY(deptnoint)rowformat delimited fields terminatedby'\t';-- 根据部门编号分区，原表中的部门编号字段就没有必要创建了-- 而是由分区表创建的伪列来替代sethive.exec.dynamic.partition.mode=nonstrict;-- 设置动态分区模式为非严格模式insertintotableemp_partition partition(deptno)selectempno,ename,job,mgr,hiredate,sal,comm ,deptnofromemp;-- 动态分区表的数据插入语句-- partition(deptno) 而不是 partition(deptno=XXX)-- select 子句从原表查出来的列数和列序要和分区表列数和列序保持一致-- select 子句最后一列要为分区表的分区列-- 不在需要where子句-- 设置动态分区模式为非严格模式-- set hive.exec.dynamic.partition.mode=nonstrict;1
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
18
19
20
21
22
23
24
25
26
27
来自@若泽大数据


