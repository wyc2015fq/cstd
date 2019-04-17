# MySQL的表类型的（存储引擎）的选择MyISAM+InnoDB+MEMORY+MERGE - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年03月09日 19:32:45[QuJack](https://me.csdn.net/A784586)阅读数：1168








MySQL的表类型的（存储引擎）的选择

1.数据库基础

1.1 MySQL 的存储引擎：表类型（存储引擎）的选择

不同的存储需求选择最优化的存储引擎。

MySQL特性：插件式存储引擎，用户选择如何存储和索引数据，是否使用事务；


InnoDB,BDB提供事务安全表；


MyISAM， MEMORY, MERGE, EXAMPLE, NDB Cluster, ARCHIVE, CSV, BLACKHOLE, FEDERATED

查看当前默认的存储引擎类型：

MySQL>show variables like 'table_type';

查看数据库支持的存储引擎： \G或者分号表示命令结束

MySQL>show engines \G
or show variables like 'have%';

mysql -h localhost -u root -p123


show databases;

use database;

show tables;




创建指定存储引擎的表：

create table tableA(


i bigint(20) not null auto_increment,


primary key(i)

)engine=MyISAM default charset=gbk;




改变表的存储引擎：

alter
table tableA engine=innodb;

---------------------------------------------------------------------------------------------------------------




各种存储引擎的特性：

### 一

MyISAM：不支持事务和外键；

访问速度快，对事务完整性没有要求的或者以select insert为主的应用适合；

每个MyISAM 在磁盘上存储三个文件，文件名都和表名相同，但扩展名分别是：

.frm 存储表定义

myd 存储数据

myi 存储索引





数据文件和存储文件可以放在不同目录，平均分布io,获得更快的速度；





表容易被损坏，check table, repair table;


MyISAM的表还支持三种不同的存储格式：

静态表（固定长度）：存储迅速，容易缓存，出现故障容易恢复；占用空间比动态表多；空格补充，返回时去掉空格；


create databases test_mysql;

create table Myisam_char (name char(10))engine=myisam;

insert into Myisam_char 
values('abcde'),('abcde '),(' abcde'),(' abcde '); //5 5 7 7

select name,length(name) 
from Myisam_char;


插入进去前面的空格保留，后面的空格都被去掉了。




动态表包含变长字段，记录不是固定长度的，这样存储：占用的空间少，频繁地删除记录会产生碎片，定期执行优化改善性能（optimize table 或者 myisamchk-r ），出故障恢复困难；

压缩表：由myisampack工具创建，占据非常小的磁盘空间，每条记录被单独压缩，只有非常小访问开支；





---------------------------------------------------------------------------------------------------------------

### 二

InnoDB:


具有提交，回滚，崩溃恢复能力的事务安全。相遇于MyISAM引擎，写处理速度略差，占用更多存储空间保留数据和索引。

2.1自动增长列：插入的是0或者null，则实际插入的是增长后的值；


create table autoincre_demo ( i smallint not null auto_increment, name varchar(10), primary key(i) ) engine=innodb;


insert into autoincre_demo values(1,'1'),(0,'2'),(3,'3');


select * from autoincre_demo;





select last_insert_id(); //查询当前线程最后插入记录使用的值；如果是多条，返回第一条；

改变初始值，初始值默认为1，存在内存中，重启数据库需要重新设定；

alert table tableA auto_increment=n;

InnoDB表：自动增长列必须是索引，如果是组合索引，必须是组合索引的第一列；

MyISAM表：自动增长列可以是组合索引的其他列；


create table autoincre_demo ( d1 samllint not null auto_increment,
d2 smalliint not null, 
name varchar(10), index(d2,d1) )engine=MyISAM；

insert into autoincre_demo(d2,name) values(2,'2'),(3,'3'),(4,'4'),(2,'2'),(3,'3'),(4,'4');

select * from autoincre_demo;



2.2 外键约束：


mysql支持外键的存储引擎只有InnoDB；




表：国家country和城市city

|country_id|country|last_update|
|----|----|----|
|1|China|aaa|





|city_id|city|country_id|last_update|
|----|----|----|----|
|121|Beijing|1|bbb|


create table country (

country_id 
smallint unsignednot null
auto_increment,


country varchar(50) not null,


last_update timestamp
not null default current_timestamp on update current_timestamp,

primary key ( country_id )

)engine=InnoDB default charset=utf8;




create table city(


city_id smallint unsigned
not nullauto_increment,


city varchar(50) not null,


country_id smallint unsigned
not null,


last_update timestamp
not null default current_timestamp on update current_timestamp,

primary key ( city_id ),

key idx_fx_country_id ( country_id ), //

constraint 'fk_city_country' 
foreign key (country_id) references 
country (country_id) on delete restrict on update cascade

)engine=InnoDB,default charset=utf8;




restrict+No action: 子表有关联记录的时候，不准许更新父表；

cascade : 对父表的更新或者删除，对子表进行更新或者删除；

set null：父表更新或者删除数据的时候，字表相关字段set null ；




当某个表被其他表创建了外键参照，该表对应的索引或者主键禁止被删除；

set foreign_key_checks=0 关闭外键检查: 1.忽略表的导入顺序，2.在执行load data 和 alter table时候加快处理速度；




2.3 存储方式：

2.3.1 共享表空间存储：


表结构: .frm


数据和索引：innodb_data_home_dir 和 innodb_data_file_path定义的表空间，可以是多个文件；

2.3.2 多表空间存储：


表结构：.frm


数据和索引： .idb 如果是分区表，每个分区对应单独的.idb中（表名+分区名），创建分区的时候指定每个数据文件的位置，来平均io。





查看表的信息：show table status like 'city ';

---------------------------------------------------------------------------------------------------------------

### 三

MEMORY:

使用存在内存中的数据进行建表，每个memory表只实际对应一个磁盘文件。.frm。

访问非常的快；数据放在内存中，默认使用hash索引。服务器关闭，表中的数据没就没了。




create table
tab_memory engine=memory

select
city_id, city, country_id

from
city group by city_id;







select count( * ) from tab_memory;

show table status like 'tab_memory ' \G




create index mem_hash
using hash on tab_memory (city_id);

show index from tab_memory ;

drop index mem_hash on tab_memory;

create index mem_hash using btree on tab_memory (city_id);




释放memory表，delete from表，truncate table 或者 drop table




适用：内容变化不平凡的代码表，统计操作的中间结果表，更新需要谨慎，因为数据并没有真正写入磁盘中。

---------------------------------------------------------------------------------------------------------------------------

### 四

MERGE:

本身没有数据，一组结构完全相同的MyISAM表的组合；

查询，更新，删除，删除操作， 删除不掉MyISAM。

create table payment_2006(


country_id smallint ,


payment_date datetime,


amount decimal(15,2),

key idx_fk_country_id( country_id)

)engine=myisam;




create table payment_2007(


country_id smallint ,


payment_date datetime,


amount decimal(15,2),

key idx_fk_country_id( country_id)　　　　　　　//键

)engine=myisam;




create table payment_all(


country_id smallint ,


payment_date datetime,


amount decimal(15,2),

index(country_id) //索引

)engine=mergeunion=( payment_2006, payment_2007)
insert_method=last;//last,first,no(最后/前一个表插入，不准插入)




--------------------------------------------------------------------------------------------------------------

**key**
 是[数据库](http://lib.csdn.net/base/mysql)的物理结构，它包含两层意义，一是约束（偏重于约束和规范数据库的结构完整性），二是索引（辅助查询用的）

**index**是数据库的物理结构，它只是辅助查询的，它创建时会在另外的表空间（mysql中的innodb表空间）以一个类似目录的结构存储。索引要分类的话，分为前缀索引、全文本索引等；因此，索引只是索引，它不会去约束索引的字段的行为。

----------------------------------------------------------------------------------------------------------------------------------




各种引擎对比：

MyISAM：读和插入为主，很少更新和删除；并对事务的完整性，并发性要求不高 ；

InnoDB：事务处理提交和回滚，支持外键。插入查询，更新删除。类似的计费系统财务系统；

MEMORY：快速定位记录；

MERGE：突破对单个MyISAM表大小的限制，使用VLＤＢ；




－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－

帮助的使用：？contents


？data type


? int 


? show 


?create table




查看表的定义：desc tableA;

更全：show create table emp \G;

\G使得记录可以按字段竖着排列；




key的用途：主要是用来加快查询速度的。

UNIQUE KEY的用途：主要是用来防止数据插入的时候重复的。




































2.mybatis



