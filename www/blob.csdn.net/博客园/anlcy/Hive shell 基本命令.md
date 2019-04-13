
# Hive shell 基本命令 - anlcy - 博客园






# [Hive shell 基本命令](https://www.cnblogs.com/camilla/p/8616537.html)
首先连接 hive shell
直接输入 hive启动, 使用--开头的字符串来表示注释
hive>quit; --退出hive
hive> exit; --exit会影响之前的使用，所以需要下一句kill掉hadoop的进程
hine>hadoop job -kill jobid
**1、显示表**
hive>create database database_name; 创建数据库
如果数据库已经存在就会抛出一个错误信息，使用如下语句可以避免抛出错误信息：
hive>creat database if not exists database_name；
*hive> show databases; 查看数据库；*
*如果数据库比较多的话，也可以用正则表达式来查看：*
*hive> show databases like 'h.*';*
hive> use default;    --使用哪个数据库；
hive> show tables; 或者支持模糊查询：hive> show tables '*t*';
hive> describe tab_name; --查看表的结构及表的路径
*hive> describe database database_name; --查看数据库的描述及路径*
**2、创建表**
hive> create table test(key string);
OK
Time taken: 0.265 seconds
**3、创建分区表：**
hive> create table logs(ts bigint,line string) partitioned by (dt String,country String);
**4、加载分区表数据：**
hive> load data local inpath '/home/Hadoop/input/file1' into table logs partition (dt='2014-03-11',country='CN');
**5、展示表中有多少分区：**
hive> show partitions logs;
**6、显示表的结构信息**
hive> describe table_name;
hive> describe database database_name;  --查看数据库的描述及路径
**7、更新表名称：**
hive>alter table table_name rename to another_name；
**8、添加新一列：**
hive> ALTER TABLE invites ADD COLUMNS (new_col2 INT COMMENT 'a comment');
**9、删除表：**
hive>drop table t1 ;      --删除表t1 或者：hive> drop table if exists t1;
删除表中数据，但要保持表的结构定义：
hive> dfs -rmr /user/hive/warehouse/records;
**10、可以用下面的命令来修改数据库的路径：**
*hive> creat database database_name location '路径';*
*hive> drop database if exists database_name; --删除空的数据库*
*hive> drop database if exists database_name cascade; --先删除数据库中的表再删除数据库；*
**11、hive不支持修改表中数据，但是可以修改表结构，而不影响数据**
有local的速度明显比没有local慢：
hive>load data inpath '/root/inner_table.dat' into table t1;   移动hdfs中数据到t1表中
hive>load  data local inpath '/root/inner_table.dat' into table t1;  上传本地数据到hdfs中
hive> !ls;  查询当前linux文件夹下的文件
hive> dfs -ls /; 查询当前hdfs文件系统下  '/'目录下的文件；
**它不支持行级插入操作、更新操作和删除操作，也不支持事务，那么往表里面装数据的唯一的途径就是使用一种“大量”的数据装载操作，**
**或者仅仅将文件写入到正确的目录下面,即以load的方式加载到建立好的表中，且数据一旦导入就不可以修改，加载的目标可以是一个表**
**或者分区，如果表包含分区，必须指定每一个分区名：**
a、使用o*verwrite关键字,加载本地数据，同时给分区信息：*
*hive>load data local inpath '${env:HOME}/目录'*overwrite into table table_name  partition (分区（ds='2018-05-05'）)；
目标表（或者分区）中的内容（如果有）会被删除，然后再将 filepath 指向的文件/目录中的内容添加到表/分区中，如果
目标表（分区）已经有一个文件，并且文件名和 filepath 中的文件名冲突，那么现有的文件会被新文件所替代。
b、将查询结果插入hive表：
hive>insert overwrite table tab_name [partition(par1=val1,par2=val2)] select_statement1 from from_statement;--基本模式
from from_statement
insert overwrite table tab_name [partition(par1=val1,par2=val2)] select_statement1
[insert overwrite table tab_name [partition(par1=val1,par2=val2)] select_statement2]...;--多插入模式
insert overwrite table tab_name partition(par1[=val1],par2[=val2]...) select_statement1 from from_statement;--自动分区模式
c、将查询结果写入HDFS文件系统:
insert overwrite [local] directory directory1 select ... from ...;--基本模式
from from_statement
insert overwrite[local] directory directory1 select_statement1
[ insert overwrite[local] directory directory2 select_statement2];--多插入模式
数据写入文件系统时进行文本序列化，且每列用^A 来区分，\n换行。
d、insert into:
insert into table tab_name [partition(par1=val1,par2=val2)] select_statement1 from from_statement;
*12、显示所有函数：*
hive>show functions;
hive>describe function fun_name;--查看函数的用法；
hive>select col1[0],col2['b'],col3.c from complex;--查看数组、map、结构；
**13、内连接：**
hive> SELECT sales.*, things.* FROM sales JOIN things ON (sales.id = things.id);
查看hive为某个查询使用多少个MapReduce作业
hive> Explain SELECT sales.*, things.* FROM sales JOIN things ON (sales.id = things.id);
**14、外连接：**
hive> SELECT sales.*, things.* FROM sales LEFT OUTER JOIN things ON (sales.id = things.id);
hive> SELECT sales.*, things.* FROM sales RIGHT OUTER JOIN things ON (sales.id = things.id);
hive> SELECT sales.*, things.* FROM sales FULL OUTER JOIN things ON (sales.id = things.id);
in查询：Hive不支持，但可以使用LEFT SEMI JOIN
hive> SELECT * FROM things LEFT SEMI JOIN sales ON (sales.id = things.id);
**15、Map连接**：Hive可以把较小的表放入每个Mapper的内存来执行连接操作
hive> SELECT /*+ MAPJOIN(things) */ sales.*, things.* FROM sales JOIN things ON (sales.id = things.id);
INSERT OVERWRITE TABLE ..SELECT：新表预先存在
hive> FROM records2
> INSERT OVERWRITE TABLE stations_by_year SELECT year, COUNT(DISTINCT station) GROUP BY year
> INSERT OVERWRITE TABLE records_by_year SELECT year, COUNT(1) GROUP BY year
> INSERT OVERWRITE TABLE good_records_by_year SELECT year, COUNT(1) WHERE temperature != 9999 AND (quality = 0 OR quality = 1 OR quality = 4 OR quality = 5 OR quality = 9) GROUP BY year;
CREATE TABLE ... AS SELECT：新表预先不存在
hive>CREATE TABLE target AS SELECT col1,col2 FROM source;
**16、创建视图：**
hive> CREATE VIEW valid_records AS SELECT * FROM records2 WHERE temperature !=9999;
**17、查看视图详细信息：**
hive> DESCRIBE EXTENDED valid_records;
**18、从表中导出数据：**
hadoop fs -cp source_path target_path
或者：用户可以使用 insert……directory……
insert overwrite local directory '/tmp/目录'     这里指定的路径也可以是全URL路径
**19、hive中使用正则表达式**
（1）   hive> select 'price.*' from table_name;
选出所有列名以price作为前缀的列
（2）  用Like或者RLike
**20、聚合函数**
可以通过设置属性hive.map.aggr值为true来提高聚合的性能：
hive>hive.map.aggr=true;
**21、什么情况下hive可以避免进行mapreduce?**
在本地模式的时候可以避免触发一个mr的job，此外，如果属性hive.execmode.local.auto的值为true的话，hive还户尝试本地模式进行其他的操作。
set hive.execmode.local.auto=true;
说明：最好将 set hive.execmode.local.auto=true;这个设置增加到你的$HOME/.hiverc配置文件中去。
**22、JOIN语句**
hive支持通常的SQL JOIN语句，但是只支持等值连接。hive也不支持在on子句中用谓词OR
**23、union all**
将两个表或者多个表进行合并，每一个union all子查询都必须具有相同的列，而且对应每个字段的每个类型都必须一致。





