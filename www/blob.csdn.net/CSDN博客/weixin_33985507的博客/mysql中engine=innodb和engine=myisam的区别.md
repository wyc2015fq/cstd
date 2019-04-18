# mysql中engine=innodb和engine=myisam的区别 - weixin_33985507的博客 - CSDN博客
2016年11月29日 16:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
最开始用[MySQL](http://lib.csdn.net/base/mysql) Administrator建[数据库](http://lib.csdn.net/base/mysql)的时候，表缺省是InnoDB类型，也就没有在意。后来用Access2MySQL导数据的时候发现只能导成 MyISAM类型的表，不知道这两种类型有什么区别，就去查了查。原来是MyISAM类型不支持事务处理等高级处理，而InnoDB类型支持。 MyISAM类型的表强调的是性能，其执行数度比InnoDB类型更快，但是不提供事务支持，而InnoDB提供事务支持已经外部键等高级数据库功能。这 样就可以根据数据表不同的用处是用不同的存储类型。 
另外，MyISAM类型的二进制数据文件可以在不同[操作系统](http://lib.csdn.net/base/operatingsystem)中迁移。也就是可以直接从Windows系统拷贝到[Linux](http://lib.csdn.net/base/linux)系统中使用。
修改：
ALTER TABLE tablename ENGINE = MyISAM ；
MyISAM:这个是默认类型,它是基于传统的ISAM类型,ISAM是Indexed Sequential Access Method (有索引的 顺序访问方法) 的缩写,它是存储记录和文件的标准方法.与其他存储引擎比较,MyISAM具有检查和修复表格的大多数工具. MyISAM表格可以被压缩,而且它们支持全文搜索.它们不是事务安全的,而且也不支持外键。如果事物回滚将造成不完全回滚，不具有原子性。如果执行大量 的SELECT，MyISAM是更好的选择。
InnoDB:这种类型是事务安全的.它与BDB类型具有相同的特性,它们还支持外键.InnoDB表格速度很快.具有比BDB还丰富的特性,因此如果需要一个事务安全的存储引擎,建议使用它.如果你的数据执行大量的INSERT或UPDATE,出于性能方面的考虑，应该使用InnoDB表,
对于支持事物的InnoDB类型的标，影响速度的主要原因是AUTOCOMMIT默认设置是打开的，而且程序没有显式调用BEGIN 开始事务，导致每插入一条都自动Commit，严重影响了速度。可以在执行sql前调用begin，多条sql形成一个事物（即使autocommit打 开也可以），将大大提高性能。
===============================================================
1. 4.0以上mysqld都支持事务，包括非max版本。3.23的需要max版本mysqld才能支持事务。
2. 创建表时如果不指定type则默认为myisam，不支持事务。
可以用 show create table tablename 命令看表的类型。
2.1 对不支持事务的表做start/commit操作没有任何效果，在执行commit前已经提交，[测试](http://lib.csdn.net/base/softwaretest)：
执行一个msyql：
use test;
drop table if exists tn;
create table tn (a varchar(10)) type=myisam;
drop table if exists ty;
create table ty (a varchar(10)) type=innodb;
begin;
insert into tn values('a');
insert into ty values('a');
select * from tn;
select * from ty;
都能看到一条记录
执行另一个mysql：
use test;
select * from tn;
select * from ty;
只有tn能看到一条记录
然后在另一边
commit;
才都能看到记录。
3. 可以执行以下命令来切换非事务表到事务（数据不会丢失），innodb表比myisam表更安全：
   alter table tablename type=innodb;
3.1 innodb表不能用repair table命令和myisamchk -r table_name
但可以用check table，以及mysqlcheck [OPTIONS] database [tables]
4. 启动mysql数据库的命令行中添加了以下参数可以使新发布的mysql数据表都默认为使用事务（
只影响到create语句。）
--default-table-type=InnoDB
测试命令：
use test;
drop table if exists tn;
create table tn (a varchar(10));
show create table tn;
5. 临时改变默认表类型可以用：
set table_type=InnoDB;
show variables like 'table_type';
或：
c:/mysql/bin/mysqld-max-nt --standalone --default-table-type=InnoDB
