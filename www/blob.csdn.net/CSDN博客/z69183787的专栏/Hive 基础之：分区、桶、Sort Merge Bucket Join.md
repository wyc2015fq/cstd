# Hive 基础之：分区、桶、Sort Merge Bucket Join - z69183787的专栏 - CSDN博客
2016年10月26日 11:40:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：769
个人分类：[大数据-Hive](https://blog.csdn.net/z69183787/article/category/5568487)
[Hive](http://lib.csdn.net/base/hive) 已是目前业界最为通用、廉价的构建[大数据](http://lib.csdn.net/base/spark)时代数据仓库的解决方案了，虽然也有
 Impala 等后起之秀，但目前从功能、稳定性等方面来说，Hive 的地位尚不可撼动。
其实这篇博文主要是想聊聊 SMB join 的，Join 是整个 MR/Hive 最为核心的部分之一，是每个 [Hadoop](http://lib.csdn.net/base/hadoop)/Hive/DW
 RD 必须掌握的部分，之前也有几篇文章聊到过 MR/Hive 中的 join，其实底层都是相同的，只是上层做了些封装而已，如果你还不了解究竟 Join 有哪些方式，以及底层怎么实现的，请参考如下链接：
[http://my.oschina.net/leejun2005/blog/95186](http://my.oschina.net/leejun2005/blog/95186) MapReduce
 中的两表 join 几种方案简介
[http://my.oschina.net/leejun2005/blog/111963](http://my.oschina.net/leejun2005/blog/111963) Hadoop
 多表 join：map side join 范例
[http://my.oschina.net/leejun2005/blog/158491](http://my.oschina.net/leejun2005/blog/158491) Hive
 & Performance 学习笔记
在最后一篇链接中，有这么两副图：
![](http://static.oschina.net/uploads/space/2013/0901/135056_wVBt_568818.png)
前面两个很好理解，基本上每个人都会接触到，但最后一种，可能有同学还是比较陌生，SMB 存在的目的主要是为了解决大表与大表间的 Join 问题，分桶其实就是把大表化成了“小表”，然后
 Map-Side Join 解决之，这是典型的分而治之的思想。在聊 SMB Join 之前，我们还是先复习下相关的基础概念。
### 1、Hive 分区表
在Hive Select查询中一般会扫描整个表内容，会消耗很多时间做没必要的工作。有时候只需要扫描表中关心的一部分数据，因此建表时引入了partition概念。分区表指的是在创建表时指定的partition的分区空间。
Hive可以对数据按照某列或者某些列进行分区管理，所谓分区我们可以拿下面的例子进行解释。
当前互联网应用每天都要存储大量的日志文件，几G、几十G甚至更大都是有可能。存储日志，其中必然有个属性是日志产生的日期。在产生分区时，就可以按照日志产生的日期列进行划分。把每一天的日志当作一个分区。
将数据组织成分区，主要可以提高数据的查询速度。至于用户存储的每一条记录到底放到哪个分区，由用户决定。即用户在加载数据的时候必须显示的指定该部分数据放到哪个分区。
#### 1.1 实现细节
1、一个表可以拥有一个或者多个分区，每个分区以文件夹的形式单独存在表文件夹的目录下。
2、表和列名不区分大小写。
3、分区是以字段的形式在表结构中存在，通过describe table命令可以查看到字段存在，但是该字段不存放实际的数据内容，仅仅是分区的表示（伪列）。
#### 1.2 语法
1. 创建一个分区表，以 ds 为分区列：
create table invites (id int, name string) partitioned by (ds string) row format delimited fields terminated by 't' stored as textfile;
2. 将数据添加到时间为 2013-08-16 这个分区中：
load data local inpath '/home/hadoop/Desktop/data.txt' overwrite into table invites partition (ds='2013-08-16');
3. 将数据添加到时间为 2013-08-20 这个分区中：
load data local inpath '/home/hadoop/Desktop/data.txt' overwrite into table invites partition (ds='2013-08-20');
4. 从一个分区中查询数据：
select * from invites where ds ='2013-08-12';
5.  往一个分区表的某一个分区中添加数据：
insert overwrite table invites partition (ds='2013-08-12') select id,max(name) from test group by id;
可以查看分区的具体情况，使用命令：
hadoop fs -ls /home/hadoop.hive/warehouse/invites
或者：
show partitions tablename;
### 2、Hive 桶
对于每一个表（table）或者分区，Hive可以进一步组织成桶，也就是说桶是更为细粒度的数据范围划分。Hive也是针对某一列进行桶的组织。Hive采用对列值哈希，然后除以桶的个数求余的方式决定该条记录存放在哪个桶当中。
把表（或者分区）组织成桶（Bucket）有两个理由：
（1）获得更高的查询处理效率。桶为表加上了额外的结构，Hive 在处理有些查询时能利用这个结构。具体而言，连接两个在（包含连接列的）相同列上划分了桶的表，可以使用
 Map 端连接 （Map-side join）高效的实现。比如JOIN操作。对于JOIN操作两个表有一个相同的列，如果对这两个表都进行了桶操作。那么将保存相同列值的桶进行JOIN操作就可以，可以大大较少JOIN的数据量。
（2）使取样（sampling）更高效。在处理大规模数据集时，在开发和修改查询的阶段，如果能在数据集的一小部分数据上试运行查询，会带来很多方便。
#### 1. 创建带桶的 table ：
create table bucketed_user(id int,name string) clustered by (id) sorted by(name) into 4 buckets row format delimited fields terminated by '\t' stored
 as textfile;
首先，我们来看如何告诉Hive—个表应该被划分成桶。我们使用CLUSTERED BY 子句来指定划分桶所用的列和要划分的桶的个数：
CREATE TABLE bucketed_user (id INT) name STRING)
CLUSTERED BY (id) INTO 4 BUCKETS;
在这里，我们使用用户ID来确定如何划分桶(Hive使用对值进行哈希并将结果除 以桶的个数取余数。这样，任何一桶里都会有一个随机的用户集合（PS：其实也能说是随机，不是吗？）。
对于map端连接的情况，两个表以相同方式划分桶。处理左边表内某个桶的 mapper知道右边表内相匹配的行在对应的桶内。因此，mapper只需要获取那个桶 (这只是右边表内存储数据的一小部分)即可进行连接。这一优化方法并不一定要求
 两个表必须桶的个数相同，两个表的桶个数是倍数关系也可以。用HiveQL对两个划分了桶的表进行连接，可参见“map连接”部分（P400）。
桶中的数据可以根据一个或多个列另外进行排序。由于这样对每个桶的连接变成了高效的归并排序(merge-sort), 因此可以进一步提升map端连接的效率。以下语法声明一个表使其使用排序桶：
CREATE TABLE bucketed_users (id INT, name STRING)
CLUSTERED BY (id) SORTED BY (id ASC) INTO 4 BUCKETS;
我们如何保证表中的数据都划分成桶了呢？把在Hive外生成的数据加载到划分成 桶的表中，当然是可以的。其实让Hive来划分桶更容易。这一操作通常针对已有的表。
Hive并不检查数据文件中的桶是否和表定义中的桶一致(无论是对于桶 的数量或用于划分桶的列）。如果两者不匹配，在査询时可能会碰到错 误或未定义的结果。因此，建议让Hive来进行划分桶的操作。
有一个没有划分桶的用户表：
hive> SELECT * FROM users;
0    Nat
2    Doe
B    Kay
4    Ann
#### 2. 强制多个 reduce 进行输出：
要向分桶表中填充成员，需要将 hive.enforce.bucketing 属性设置为 true。①这样，Hive
 就知道用表定义中声明的数量来创建桶。然后使用 INSERT 命令即可。需要注意的是：clustered
 by和sorted by不会影响数据的导入，这意味着，用户必须自己负责数据如何如何导入，包括数据的分桶和排序。
'set hive.enforce.bucketing = true' 可以自动控制上一轮reduce的数量从而适配bucket的个数，当然，用户也可以自主设置mapred.reduce.tasks去适配bucket个数，推荐使用'set hive.enforce.bucketing =
 true' 
#### 3. 往表中插入数据：
INSERT OVERWRITE TABLE bucketed_users SELECT * FROM users;
物理上，每个桶就是表(或分区）目录里的一个文件。它的文件名并不重要，但是桶 n 是按照字典序排列的第 n 个文件。事实上，桶对应于 MapReduce 的输出文件分区：一个作业产生的桶(输出文件)和reduce任务个数相同。我们可以通过查看刚才
 创建的bucketd_users表的布局来了解这一情况。运行如下命令： 
#### 4. 查看表的结构：
hive> dfs -ls /user/hive/warehouse/bucketed_users;
将显示有4个新建的文件。文件名如下(文件名包含时间戳，由Hive产生，因此 每次运行都会改变)：
attempt_201005221636_0016_r_000000_0
attempt_201005221636_0016_r-000001_0
attempt_201005221636_0016_r_000002_0
attempt_201005221636_0016_r_000003_0
第一个桶里包括用户IDO和4，因为一个INT的哈希值就是这个整数本身，在这里 除以桶数(4)以后的余数：②
#### 5. 读取数据，看每一个文件的数据：
hive> dfs -cat /user/hive/warehouse/bucketed_users/*0_0;
0 Nat
4 Ann
用TABLESAMPLE子句对表进行取样，我们可以获得相同的结果。这个子句会将 查询限定在表的一部分桶内，而不是使用整个表：
#### 6. 对桶中的数据进行采样：
hive> SELECT * FROM bucketed_users
>    TABLESAMPLE(BUCKET 1 OUT OF 4 ON id);
0 Nat
4 Ann
桶的个数从1开始计数。因此，前面的查询从4个桶的第一个中获取所有的用户。 对于一个大规模的、均匀分布的数据集，这会返回表中约四分之一的数据行。我们 也可以用其他比例对若干个桶进行取样(因为取样并不是一个精确的操作，因此这个 比例不一定要是桶数的整数倍)。例如，下面的查询返回一半的桶：
#### 7. 查询一半返回的桶数：
hive> SELECT * FROM bucketed_users
>    TABLESAMPLE(BUCKET 1 OUT OF 2 ON id)；
0 Nat
4 Ann
2 Joe
因为查询只需要读取和TABLESAMPLE子句匹配的桶，所以取样分桶表是非常高效的操作。如果使用rand()函数对没有划分成桶的表进行取样，即使只需要读取很小一部分样本，也要扫描整个输入数据集：
hive〉 SELECT * FROM users
> TABLESAMPLE(BUCKET 1 OUT OF 4 ON rand());
2 Doe
①从Hive 0.6.0开始，对以前的版本，必须把mapred.reduce .tasks设为表中要填 充的桶的个数。如果桶是排序的，还需要把hive.enforce.sorting设为true。
②显式原始文件时，因为分隔字符是一个不能打印的控制字符，因此字段都挤在一起。
### 3、举个完整的小例子：
#### （1）建student & student1 表：
|`1`|`create``table``student(id ``INT````, age````INT``, ``name``STRING)`|
|`2`|`partitioned ``by````(stat_date STRING)```|
|`3`|`clustered ``by````(id) sorted````by``(age) ``into````2 buckets```|
|`4`|```row format delimited fields terminated````by``','``;`|
|`5`||
|`6`|`create``table``student1(id ``INT````, age````INT``, ``name``STRING)`|
|`7`|`partitioned ``by````(stat_date STRING)```|
|`8`|`clustered ``by````(id) sorted````by``(age) ``into````2 buckets```|
|`9`|```row format delimited fields terminated````by``','``;`|
#### （2）设置环境变量：
set hive.enforce.bucketing = true; 
#### （3）插入数据：
|`01`|```cat bucket.txt```|
|`02`||
|`03`|`1,20,zxm`|
|`04`|`2,21,ljz`|
|`05`|`3,19,cds`|
|`06`|`4,18,mac`|
|`07`|`5,22,android`|
|`08`|`6,23,symbian`|
|`09`|`7,25,wp`|
|`10`||
|`11`|`LOAD``DATA ``local``INPATH ``'/home/lijun/bucket.txt'``OVERWRITE ``INTO``TABLE````student partition(stat_date=````"20120802"``);`|
|`12`||
|`13`|`from``student`|
|`14`|`insert``overwrite ``table````student1 partition(stat_date=````"20120802"``)`|
|`15`|`select``id,age,``name``where``stat_date=``"20120802"``sort ``by``age;`|
#### （4）查看文件目录：
hadoop fs -ls /hive/warehouse/test.db/student1/stat_date=20120802
Found 2 items
-rw-r--r--   2 lijun supergroup         31 2013-11-24 19:16 /hive/warehouse/test.db/student1/stat_date=20120802/000000_0
-rw-r--r--   2 lijun supergroup         39 2013-11-24 19:16 /hive/warehouse/test.db/student1/stat_date=20120802/000001_0
#### （5）查看sampling数据：
hive> select * from student1 tablesample(bucket 1 out of 2 on id);
Total MapReduce jobs = 1
Launching Job 1 out of 1
.......
OK
4       18      mac     20120802
2       21      ljz     20120802
6       23      symbian 20120802
Time taken: 20.608 seconds
注：tablesample是抽样语句，语法：TABLESAMPLE(BUCKET x OUT OF y)
y必须是table总bucket数的倍数或者因子。hive根据y的大小，决定抽样的比例。例如，table总共分了64份，当y=32时，抽取(64/32=)2个bucket的数据，当y=128时，抽取(64/128=)1/2个bucket的数据。x表示从哪个bucket开始抽取。例如，table总bucket数为32，tablesample(bucket
 3 out of 16)，表示总共抽取（32/16=）2个bucket的数据，分别为第3个bucket和第（3+16=）19个bucket的数据。
### 4、Refer:
[http://rdc.taobao.org/?p=1457](http://rdc.taobao.org/?p=1457)  从MR到Hive – 一次迁移的过程
[http://blog.573114.com/Blog/Html/A031/516857.html](http://blog.573114.com/Blog/Html/A031/516857.html)  Hadoop权威指南
 第12章 Hive简介 P384
[http://superlxw1234.iteye.com/blog/1545150](http://superlxw1234.iteye.com/blog/1545150)  hive--Sort
 Merge Bucket Map Join
[http://blog.csdn.net/yfkiss/article/details/7816916](http://blog.csdn.net/yfkiss/article/details/7816916)
insert into 和overwrite的用法：
```
INSERT INTO TABLE tablename1 [PARTITION \
(partcol1=val1, partcol2=val2 ...)] \
select_statement1 FROM from_statement;
```
```
INSERT OVERWRITE TABLE tablename1 [PARTITION \
(partcol1=val1, partcol2=val2 ...) [IF NOT EXISTS]] \
select_statement1 FROM from_statement;
```
两种方式的相同点：
1.两个表的维度必须一样，才能够正常写入
2.如果查询出来的数据类型和插入表格对应的列数据类型不一致，将会进行转换，但是不能保证转换一定成功，比如如果查询出来的数据类型为int，插入表格对应的列类型为string，可以通过转换将int类型转换为string类型；但是如果查询出来的数据类型为string，插入表格对应的列类型为int，转换过程可能出现错误，因为字母就不可以转换为int，转换失败的数据将会为NULL。
不同点：
1.insert into是增加数据
2.insert overwrite是删除原有数据然后在新增数据，如果有分区那么只会删除指定分区数据，其他分区数据不受影响
另：
hive引入partition和bucket的概念，中文翻译分别为分区和桶（我觉的不是很合适，但是网上基本都是这么翻译，暂时用这个吧），这两个概念都是把数据划分成块，分区是粗粒度的划分桶是细粒度的划分，这样做为了可以让查询发生在小范围的数据上以提高效率。
首先介绍分区的概念，还是先来个例子看下如果创建分区表：
[code lang=”sql”]
create table logs_partition(ts bigint,line string) –ts timestamp line 每一行日志
partitioned by (dt string,country string) — 分区列 dt 日志产生日期
[/code]
创建分区表需要在定义表的时候声明分区列，这个分区列是个比较有意思的东西下面来看看，向表中导入数据：
[code lang=”sql”]
load data local inpath ‘input/hive/partitions/file1′
into table logs_partition
partition(dt=’2001-01-01′,country=’GB’);
…….
— 看下表的结构
hive> desc logs_partition;
OK
ts bigint None
line string None
dt string None
country string None
# Partition Information
# col_name data_type comment
dt string None
country string None
Time taken: 0.265 seconds, Fetched: 10 row(s)
查看一个表的所有分区
hive> show partitions logs_partition;
OK
dt=2001-01-01/country=GB
dt=2001-01-01/country=US
dt=2001-01-02/country=GB
dt=2001-01-02/country=US
Time taken: 0.186 seconds, Fetched: 4 row(s)
[/code]
导入完数据后看下hive数据仓库表logs_partition下的文件目录结构
/user/hive/warehouse/logs_partition
![Screenshot from 2013-10-10 17:46:25](http://i1.wp.com/www.aahyhaa.com/wp-content/uploads/2013/10/Screenshot-from-2013-10-10-174625.png?resize=225%2C274)
看到了吧分区列都成了目录了，这样查询的时候就会定位到某个目录下而大大提高了查询效率，在查看表结构的时候分区列跟其他列并无区别，看个查询语句：
[code lang=”sql”]
SELECT ts, dt, line
FROM logs
WHERE country=’GB’;
1 2001-01-01 Log line 1
2 2001-01-01 Log line 2
4 2001-01-02 Log line 4
Time taken: 36.316 seconds, Fetched: 3 row(s)
[/code]
这个查询只会查询file1, file2, file4这三个文件还有一个有趣的问题就是，查看下数据文件fieldX
里面都只包含两列ts和line并不包含dt和country这两个分区列，但是从查询结果看分区列和非分区列并无差别，实际上分区列都是从数据仓库的分区目录名得来的。
接下来说说桶，桶是更为细粒度的数据范围划分，它能使一些特定的查询效率更高，比如对于具有相同的桶划分并且jion的列刚好就是在桶里的连接查询，还有就是示例数据，对于一个庞大的数据集我们经常需要拿出来一小部分作为样例，然后在样例上验证我们的查询，优化我们的程序。
下面看看如何创建带桶的表
[code lang=”sql”]
create table bucket_user (id int,name string)
clustered by (id) into 4 buckets;
[/code]
关键字clustered声明划分桶的列和桶的个数，这里以用户的id来划分桶，划分4个桶。
以下为了简便划分桶的列简称为桶列
hive会计算桶列的hash值再以桶的个数取模来计算某条记录属于那个桶
向这种带桶的表里面导入数据有两种方式，一种是外部生成的数据导入到桶表，一种是利用hive来帮助你生成桶表数据
由于hive在load数据的时候不能检查数据文件的格式与桶的定义是否匹配，如果不匹配在查询的时候就会报错，所以最好还是让hive来帮你生成数据，简单来说就是利用现有的表的数据导入到新定义的带有桶的表中，下面来看看：
已经存在的表：
[code lang=”bash”]
hive> select * from users;
OK
0 Nat
2 Joe
3 Kay
4 Ann
hive> set hive.enforce.bucketing=true –必须设置这个数据，hive才会按照你设置的桶的个数去生成数据
[/code]
下面把user的数据导入到bucketed_users中
[code language=”lang='sql”]
insert overwrite table bucketed-users
select * from users;
[/code]
然后见证奇迹的时刻：
[code lang=”bash”]
hive> dfs -ls /user/hive/warehouse/bucketed_users;
-rw-r–r– 1 root supergroup 12 2013-10-10 18:48 /user/hive/warehouse/bucketed_users/000000_0
-rw-r–r– 1 root supergroup 0 2013-10-10 18:48 /user/hive/warehouse/bucketed_users/000001_0
-rw-r–r– 1 root supergroup 6 2013-10-10 18:48 /user/hive/warehouse/bucketed_users/000002_0
-rw-r–r– 1 root supergroup 6 2013-10-10 18:48 /user/hive/warehouse/bucketed_users/000003_0
hive> dfs -cat /user/hive/warehouse/bucketed_users/000000_0;
0Nat
4Ann
[/code]
下面来看看利用bucket来对示例数据进行查询
[code lang=”sql”]
—带桶的表
select * from bucketed_users
tablesample(bucket 1 out of 4 on id);
—不带桶的表
select * from users
tablesample(bucket 1 out of 4 on rand());
[/code]
tablesample的作用就是让查询发生在一部分桶上而不是整个数据集上，上面就是查询4个桶里面第一个桶的数据
相对与不带桶的表这无疑是效率很高的,因为同样都是需要一小部分数据，但是不带桶的表需要使用rand（）函数，需要在整个数据集上检索。
