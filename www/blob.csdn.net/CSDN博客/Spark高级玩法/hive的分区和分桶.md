# hive的分区和分桶 - Spark高级玩法 - CSDN博客
2018年01月12日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：371
**本文转载自CSDN：http://blog.csdn.net/wl1411956542/article/details/52931499**
**由于不知道作者详细信息，文章作者暂时用其ID。**
**1、Hive分区表**
在Hive Select查询中一般会扫描整个表内容，会消耗很多时间做没必要的工作。有时候只需要扫描表中关心的一部分数据，因此建表时引入了partition概念。分区表指的是在创建表时指定的partition的分区空间。 Hive可以对数据按照某列或者某些列进行分区管理，所谓分区我们可以拿下面的例子进行解释。 
当前互联网应用每天都要存储大量的日志文件，几G、几十G甚至更大都是有可能。存储日志，其中必然有个属性是日志产生的日期。在产生分区时，就可以按照日志产生的日期列进行划分。把每一天的日志当作一个分区。 
将数据组织成分区，主要可以提高数据的查询速度。至于用户存储的每一条记录到底放到哪个分区，由用户决定。即用户在加载数据的时候必须显示的指定该部分数据放到哪个分区。 
**1.1****实现细节**
1、一个表可以拥有一个或者多个分区，每个分区以文件夹的形式单独存在表文件夹的目录下。 
2、表和列名不区分大小写。 
3、分区是以字段的形式在表结构中存在，通过describetable命令可以查看到字段存在， 但是该字段不存放实际的数据内容，仅仅是分区的表示（伪列） 。 
**1.2语法**
1.创建一个分区表，以 ds 为分区列： 
create table invites(id int, name string) partitioned by (ds string) row format delimited fieldsterminated by '	' stored as textfile; 
2. 将数据添加到时间为 2013-08-16这个分区中： 
load data local inpath'/home/hadoop/Desktop/data.txt' overwrite into table invites partition(ds='2013-08-16'); 
3. 将数据添加到时间为 2013-08-20这个分区中： 
load data local inpath'/home/hadoop/Desktop/data.txt' overwrite into table invites partition(ds='2013-08-20'); 
4. 从一个分区中查询数据： 
select * from inviteswhere ds ='2013-08-12'; 
5.  往一个分区表的某一个分区中添加数据： 
insert overwrite tableinvites partition (ds='2013-08-12') select id,max(name) from test group by id; 
可以查看分区的具体情况，使用命令： 
hadoop fs -ls /home/hadoop.hive/warehouse/invites 
或者： 
show partitionstablename;
**2、Hive桶**
对于每一个表（table）或者分区， Hive可以进一步组织成桶，也就是说桶是更为细粒度的数据范围划分。Hive也是 针对某一列进行桶的组织。Hive采用对列值哈希，然后除以桶的个数求余的方式决定该条记录存放在哪个桶当中。
把表（或者分区）组织成桶（Bucket）有两个理由：
（1）获得更高的查询处理效率。桶为表加上了额外的结构，Hive 在处理有些查询时能利用这个结构。具体而言，连接两个在（包含连接列的）相同列上划分了桶的表，可以使用 Map 端连接 （Map-side join）高效的实现。比如JOIN操作。对于JOIN操作两个表有一个相同的列，如果对这两个表都进行了桶操作。那么将保存相同列值的桶进行JOIN操作就可以，可以大大较少JOIN的数据量。
（2）使取样（sampling）更高效。在处理大规模数据集时，在开发和修改查询的阶段，如果能在数据集的一小部分数据上试运行查询，会带来很多方便。
**1.创建带桶的 table ：**
create tablebucketed_user(id int,name string)clustered by (id) sorted by(name) into 4buckets row format delimited fields terminated by '	' stored as textfile; 
首先，我们来看如何告诉Hive
—个表应该被划分成桶。我们使用CLUSTERED BY 子句来指定划分桶所用的列和要划分的桶的个数：
 CREATE TABLE bucketed_user(id INT) name STRING) 
CLUSTERED BY (id) INTO 4BUCKETS; 在这里，我们使用用户ID
来确定如何划分桶(Hive使用对值进行哈希并将结果除 以桶的个数取余数。这样，任何一桶里都会有一个随机的用户集合（PS：其实也能说是随机，不是吗？）。
** 对于map**
端连接的情况，两个表以相同方式划分桶。处理左边表内某个桶的 mapper知道右边表内相匹配的行在对应的桶内。因此，mapper只需要获取那个桶 (这只是右边表内存储数据的一小部分)即可进行连接。这一优化方法并不一定要求两个表必须桶的个数相同，两个表的桶个数是倍数关系也可以。
**用HiveQL**
对两个划分了桶的表进行连接，可参见“map连接”部分（P400）。
 桶中的数据可以根据一个或多个列另外进行排序。由于这样对每个桶的连接变成了高效的归并排序(merge-sort),因此可以进一步提升map
端连接的效率。以下语法声明一个表使其使用排序桶：
 CREATE TABLE bucketed_users(id INT, name STRING) 
CLUSTERED BY (id) SORTED BY(id ASC) INTO 4 BUCKETS; 我们如何保证表中的数据都划分成桶了呢？把在Hive
外生成的数据加载到划分成 桶的表中，当然是可以的。其实让Hive来划分桶更容易。这一操作通常针对已有的表。
 Hive并不检查数据文件中的桶是否和表定义中的桶一致(
无论是对于桶的数量或用于划分桶的列）。如果两者不匹配，在査询时可能会碰到错 误或未定义的结果。因此，建议让Hive来进行划分桶的操作。
**2. 往表中插入数据：**
INSERT OVERWRITE TABLEbucketed_users SELECT * FROM users; 物理上，每个桶就是表(
或分区）目录里的一个文件。它的文件名并不重要，但是桶 n 是按照字典序排列的第 n 个文件。
事实上，桶对应于 MapReduce
的输出文件分区：一个作业产生的桶(输出文件)和reduce任务个数相同.
**3. 对桶中的数据进行采样：**
hive> SELECT * FROMbucketed_users 
>   TABLESAMPLE(BUCKET 1 OUT OF 4 ON id); 
0 Nat 
4 Ann 桶的个数从1
开始计数。因此，前面的查询从4个桶的第一个中获取所有的用户。 对于一个大规模的、均匀分布的数据集，这会返回表中约四分之一的数据行。我们 也可以用其他比例对若干个桶进行取样(因为取样并不是一个精确的操作，因此这个 比例不一定要是桶数的整数倍)。
**注：**tablesample是抽样语句，语法：TABLESAMPLE(BUCKET x OUTOF y)
y必须是table总bucket数的倍数或者因子。hive根据y的大小，决定抽样的比例。例如，table总共分了64份，当y=32时，抽取(64/32=)2个bucket的数据，当y=128时，抽取(64/128=)1/2个bucket的数据。x表示从哪个bucket开始抽取。例如，table总bucket数为32，tablesample(bucket 3 out of 16)，表示总共抽取（32/16=）2个bucket的数据，分别为第3个bucket和第（3+16=）19个bucket的数据。
**浪尖总结一句话：**
**分区按照我们指定列范围进行分区(层级文件夹)，而分桶是按照列值的hash值,这样想同值都会在一个桶内**(一级文件夹内不同文件区别不同的桶)**，便于join操作。**
