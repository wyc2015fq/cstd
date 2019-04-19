# mysql分区 - 建建的博客 - CSDN博客
2017年03月15日 10:55:33[纪建](https://me.csdn.net/u013898698)阅读数：119
个人分类：[Mysql](https://blog.csdn.net/u013898698/article/category/6790249)

分区
就访问数据库的应用而言，逻辑上只有一个表或一个索引，但是实际上这个表可能由数10个物理分区对象组成，每个分区都是一个独立的对象，可以独自处理，可以作为表的一部分进行处理。
- 分区对应用来说是完全透明的，不影响应用的业务逻辑
- 创建分区，是对数据表，和索引表，同时创建分区的。
- 不能分别创建分区；同时也不能对部分数据进行分区；
- 分区在创建完成之后，管理员也无法动态更改；
mysql分区的优点主要包括：
- 和单个磁盘或者文件系统分区相比，可以存储更多数据
- 优化查询。在where子句中包含分区条件时，可以只扫描必要的一个或多个分区来提高查询效率。
- 对于已经过期或者不需要保存的数据，可以通过删除与这些数据有关的分区来快速删除数据
- 跨多个磁盘来分散数据查询，以获得更大的查询吞吐量
分区键：partition key
查看是否支持分区：
```
mysql> show variables like '%partition%';
+-------------------+-------+
| Variable_name     | Value |
+-------------------+-------+
| have_partitioning | YES   |
+-------------------+-------+
```
分区类型
- range分区：基于一个给定连续区间范围，把数据分配到不同的分区。
- list分区：类似range分区，区别在于list分区是基于枚举的值列表分区，range是基于给定的连续区间范围分区
- hash分区：基于给定的分区个数，把数据分配到不同的分区
- key分区：类似与hash分区
5.1 版本range，list，hash，分区键，必须是int，或者表达式返回int类型；
5.5 版本 的range，list分区，已经支持非整数分区了；
key分区，可以使用除blob，text之外的类型列作为分区键；
如果存在主键/唯一键字段，则不能使用除主键/唯一键字段之外的其他字段进行分区。
range分区：
利用取值范围将数据分成分区，区间要连续并且不能相互重叠；
使用values less than 操作符按照进行分区定义：
注意：分区只能在建表的时候进行，建完表之后，只能增加分区内容；
 PARTITION BY RANGE (store_id)
( PARTITION p0 VALUES LESS THAN (10) ）
```
mysql> show create table emp;
| emp   | CREATE TABLE `emp` (
  `id` int(**11**) NOT NULL,
  `store_id` int(**11**) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1
/*!50100 PARTITION BY RANGE (store_id)
(PARTITION p0 VALUES LESS THAN (10) ENGINE = InnoDB) */ |
**1** row in set (**0.00** sec)
 
mysql> insert into emp values(**1**,**11**);
ERROR **1526** (HY000): Table has no partition for value **11**
mysql> insert into emp values(**1**,**10**);
ERROR **1526** (HY000): Table has no partition for value **10**
mysql> insert into emp values(**1**,**9**);
Query OK, **1** row affected (**0.01** sec)
```
只可以插入，键值范围内的值；上面只能插入比10小的store_id;
```
mysql> alter table emp add partition (partition p1 values less than (**20**));
Query OK, **0** rows affected (**0.06** sec)
mysql> insert into emp values(**2**,**18**);
Query OK, **1** row affected (**0.00** sec)
```
新增了一个分区p1,范围小于20，大于等于p0 的10；分区增加应该是连续的从p0开始。
VALUES LESS THAN MAXVALUE :比最大值大的其他值的分区；list中不存在；
在5.5版本后，range支持非整数 分区；使用关键字 PARTITION BY RANGE COLUMNS (name),如下面例子：按时间分区；
```
PARTITION BY RANGE COLUMNS (TIME) (
     PARTITION p0 VALUES LESS THAN ('1996-01-01'),
     PARTITION p2 VALUES LESS THAN ('2006-01-01')
)
```
在mysql5.1中分区日期处理上只有year（），to_days(),两种；而5.5分区日期处理增加了to_seconds(),把日期转化成秒；
range分区功能特别适合以下两种情况：
- 当需要删除过期的数据时。只需要alter table emp drop partition p0 来删除p0分区中的数据。对于上百万条记录的表来说，删除分区要比运行一个delete 语句有效得多
- 经常运行包含分区键的查询，mysql可以很快地确定只有某一个或某些分区需要扫描。例如，上述例子中检索store_id ，大于10的记录，mysql只需要扫描p1 ；
可以使用explain 来检测，查询使用的是哪个分区；
List分区：
list 分区是建立离散的值列表告诉数据库特定的值属于哪个分区，与range相似，区别是list分区是从属一个枚举列表的值的集合，range分区是从属一个连续区间值的集合。
```
partition by list (id) (
     partition p0 values in (**3**,**5**),
     partition p1 values in (**1**,**10**),
     partition p0 values in (**4**,**6**),
     partition p1 values in (**2**,**8**),
)
```
插入只可以是，分区中枚举出来的值；且没有 像range 分区中 values less than maxvalue 这样包含其他值在内的定义方式；
同样，在mysql5.5 之后可以支持非整数分区：
```
partition by list  columns (category) (
     partition p0 values in ('lodging','food'),
     partition p1 values in ('good','bad'),
     partition p0 values in ('flights','groud')
)
```
columns 分区支持：
- 整数：tinyint,smallint,mediumint,int,bigint;不支持其他类型，如float，decimal；
- 日期时间：date,datetime
- 字符串:char,varchar,binary,varbinary;不支持text，blob；
注意：columns分区仅支持一个或多个字段名作为分区键，不支持表达式作为分区键，这个和不带 columns的range，list 分区 有区别；
多字段分区是，columns 分区的一个亮点；
多字段分区键的比较就是多列排序，先根据a字段排序，再根据b字段排序，然后根据排序结果来分区存放数据。和range单字段分区排序的规则实际上是一致的。
```
partition by range colulmns (a,b)(
 
     partition p01 values less than (**0**,**10**),
     partition p02 values less than (**10**,**10**),
     partition p03 values less than (**10**,**20**)
)
```
假设 partition p01 values less than (ma,mb),
此时进行元组比较：(a<ma)  or ( a=ma and b<mb);
即先比较a,如果a小于ma，则直接存，当a=ma且b<mb时 也是符合要求的；
Hash分区
主要用来分散热点读，确保数据在预先确定个数的分区中尽可能平均分布；
在执行hash分区时，mysql会对分区键应用一个散列函数，以此确定数据应当放在N个分区中的哪个分区中。
hash分区：
- 常规hash分区：通过MOD()取模的方式，确定插入数据在具体哪个分区内。
- - 语法：partition by hash (id) partition 4;分成4个分区；
- 线性hash分区：分区函数是一个线性的2的幂的运算法则，确定插入数据在具体哪个分区内。
- - 语法：partition by linear hash (id) partition 4；
- 优点：在分区维护（包含，增加，删除，合并，拆分分区）时，mysql能够处理得更加迅速；
- 缺点：对比常规hash分区（取模）的时候，线性hash各个分区之间数据的分布不太均衡
key分区
类似与hash分区；hash分区允许使用用户自定义的表达式，key分区不允许使用用户自定义的表达式，需要使用mysql服务器提供的hash函数。同时hash分区只支持整数分区，而key分区支持使用除blob，text以外的其他类型；
与hash分区不同，创建可以分区表的时候，可以不指定分区键，默认会首先选择使用主键作为分区键；没有主键时，会选择非空唯一键作为分区键；
子分区
对分区表中每一个分区再次分割，又被称为复合分区；适合用于保存非常大量的数据记录；
mysql分区处理null值的方式
mysql禁止分区键值使用null，分区键可能是一个字段或者一个用户定义的表达式；
- range分区中，null值会被当做最小值来处理；
- list分区中，null值必须出现在枚举列表中，否则不接受
- hash/key，null值会被当做零值来处理；
分区管理
使用alter table 来对分区进行，添加，删除，重定义，合并，拆分分区的命令；
range和list：
删除：
```
alter table emp drop partition p1;
```
增加：
```
alter table emp add partition (partition p3 values less than (**2000**) )
```
假设原有p1,p2,p3,现在要增加一个分区，px的命名随意，但是values less than (xxx)一定是大于当前分区中最大值的；
拆分：将p3分区（2000--2015）分为两个分区p2(2000-2005)和p3（2005-2015）
```
alter table emp reorganize partition p3 into (partition p2 values less than (**2005**),partition p3 values less than (**2015**) )
```
合并：将p3分区（30），p4(40) 合并成 p2(40);
```
mysql> alter table emp reorganize partition p3,p4 into  (partition p2 values less than (**40**));
```
注意：p3,p4，合并后范围只能变大，不能缩小；即 p2 范围>=40;
- 拆分与合并，后范围只能变大，不能缩小；
- 同时不能与其他分区 重叠，即只能够重新定义相邻的分区，不能跳分区进行重定义；
hash和key分区管理：
hash分区，删除，当是它可以从4个分区合并为两个分区
- - alter table emp coalesce partition 2；
- coalesce 不能用来增加分区数量
hash分区，增加分区，例如当前emp 有两个分区，现在增加8个分区，使分区数达到10个；
- - alter table emp add partition  partition 8   
相关命令
```
- PARTITION BY RANGE (store_id) ( PARTITION p0 VALUES LESS THAN (**10**) ）
- PARTITION BY RANGE COLUMNS (TIME) ( PARTITION p0 VALUES LESS THAN ('1996-01-01'),PARTITION p2 VALUES LESS THAN ('2006-01-01') )
- partition by range colulmns (a,b) ( partition p01 values less than (**0**,**10**) ）
- VALUES LESS THAN MAXVALUE :比最大值大的其他值的分区；list中不存在；
- partition by list (id) ( partition p0 values in (**3**,**5**)）
- partition by list  columns (category) ( partition p0 values in ('lodging','food') ）
- partition by hash (id) partition **4**;
- alter table emp drop partition p1;
- altertable emp add partition (partition p3 values less than (**2000**) )
- alter table emp reorganize partition p3 into (partition p2 values less than (**2005**),partition p3 values less than (**2015**) )
- alter table emp reorganize partition p3,p4 into  (partition p2 values less than (**40**));
- alter table emp coalesce partition **2**；
- alter table emp add partition  partition **8**
```
 总结
- 分区特点：
- - 对应用是透明的
- 对数据表和索引表同时创建分区
- 创建完成后无法动态修改这里指的是已经表结构中指定分区字段
- 不能对部分数据进行分区，
-  分区优点：
- - 优化查询
- 存储更多数据
- 分区处理过期数据
- 跨盘查询提高吞吐量
-  range：
- - 基与给定区间范围，把数据分配到不同分区；
- 区间要连续并且不能相互重叠；
- 特别适合：
- - 当需要删除过期的数据时
- 经常运行包含分区键的查询。
-  list：
- - 类似range分区，list分区是基于枚举值列表分区；
- 插入只可以是，分区中枚举出来的值；
-  hash：
- - 基于给定的分区个数，把数据均匀分配到不同的分区；
- 主要用来分散热点读；
-  KEY：类似与hash分区
-  如存在主键/唯一键字段，则只能使用其作为分区字段
-  columns 分区：partition by range colulmns（time）；
-  多字段分区 :
- - partition by range colulmns (a,b) (partition p01 values less than (ma,mb))；
- 先比较a,如果a小于ma，则直接存，当a=ma且b<mb时 也是符合要求的；
-  分区管理：
- - range和list：增，删，合并，拆分；
- 拆分与合并，后范围只能变大，不能缩小；
- 同时不能与其他分区 重叠，只能够重新定义相邻的分区，不能跳分区进行重定义；
- hash：删，增；
