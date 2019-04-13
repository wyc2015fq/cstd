
# MYSQL基础：binlog使用基础:2：删库删表的误操作应对 - 知行合一 止于至善 - CSDN博客

2019年03月09日 05:51:53[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：68标签：[mysql																](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)[binlog																](https://so.csdn.net/so/search/s.do?q=binlog&t=blog)[误删																](https://so.csdn.net/so/search/s.do?q=误删&t=blog)[恢复																](https://so.csdn.net/so/search/s.do?q=恢复&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=误删&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=binlog&t=blog)个人分类：[数据库																](https://blog.csdn.net/liumiaocn/article/category/6328292)
[
																								](https://so.csdn.net/so/search/s.do?q=binlog&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)

![在这里插入图片描述](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这篇文章继续通过简单的示例来说明如何结合使用全备和binlog来进行误删库表。
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 基础知识
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)事前启动binlog和相关基础知识 ，请参看下文：
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[https://blog.csdn.net/liumiaocn/article/details/88321124](https://blog.csdn.net/liumiaocn/article/details/88321124)
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)binlog更多用于在全量备份的基础上进行恢复，关于备份可以参看如下文章：
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[https://blog.csdn.net/liumiaocn/article/details/88319749](https://blog.csdn.net/liumiaocn/article/details/88319749)
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# mysqldump备份
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)假设事前有如下的全量备份，比如通过crontab或者手工产生
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
# mysqldump -u root -pliumiaocn devops >devops_20190307112424.sql
```
```python
mysqldump:
```
```python
[
```
```python
Warning
```
```python
]
```
```python
Using a password on the
```
```python
command
```
```python
line interface can be insecure.
```
```python
#
```
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 事前准备
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)binlog类似Oracle的redo，它的实现是通过记录相关的DML或者DDL的操作来实现的，这里模拟一下
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)DDL： 建立一个名为people的表
DML：插入一条数据
DDL：误删一个表 drop table
DDL：误删了数据库
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 场景：

> 每日备份能够止损到当天备份的时刻，但是从那之后的内容则需要从binlog中止损。

> 重制日志

> 使用reset master可以清一下binlog，这里主要是为了演示清晰，实际执行时一定要小心，会雪上加霜。

> mysql
> >
> reset master
> ;
> Query OK
> ,
> 0
> rows
> affected
> (
> 0.00
> sec
> )
> mysql
> >
> 设定确认

> mysql
> >
> show
> variables
> like
> 'log_bin%'
> ;
> +
> ---------------------------------+-------------------------+
> |
> Variable_name
> |
> Value
> |
> +
> ---------------------------------+-------------------------+
> |
> log_bin
> |
> ON
> |
> |
> log_bin_basename
> |
> /
> var
> /
> lib
> /
> mysql
> /
> ON
> |
> |
> log_bin_index
> |
> /
> var
> /
> lib
> /
> mysql
> /
> ON
> .
> index
> |
> |
> log_bin_trust_function_creators
> |
> OFF
> |
> |
> log_bin_use_v1_row_events
> |
> OFF
> |
> +
> ---------------------------------+-------------------------+
> 5
> rows
> in
> set
> (
> 0.01
> sec
> )
> mysql
> >
> show
> variables
> like
> 'binlog_%'
> ;
> +
> -----------------------------------------+--------------+
> |
> Variable_name
> |
> Value
> |
> +
> -----------------------------------------+--------------+
> |
> binlog_cache_size
> |
> 32768
> |
> |
> binlog_checksum
> |
> CRC32
> |
> |
> binlog_direct_non_transactional_updates
> |
> OFF
> |
> |
> binlog_error_action
> |
> ABORT_SERVER
> |
> |
> binlog_format
> |
> ROW
> |
> |
> binlog_group_commit_sync_delay
> |
> 0
> |
> |
> binlog_group_commit_sync_no_delay_count
> |
> 0
> |
> |
> binlog_gtid_simple_recovery
> |
> ON
> |
> |
> binlog_max_flush_queue_time
> |
> 0
> |
> |
> binlog_order_commits
> |
> ON
> |
> |
> binlog_row_image
> |
> FULL
> |
> |
> binlog_rows_query_log_events
> |
> OFF
> |
> |
> binlog_stmt_cache_size
> |
> 32768
> |
> +
> -----------------------------------------+--------------+
> 13
> rows
> in
> set
> (
> 0.00
> sec
> )
> mysql
> >
> DDL: create table

> 创建一个简单的table

> mysql
> >
> create
> table
> people
> (
> -
> >
> person_id
> INT
> NOT
> NULL
> AUTO_INCREMENT
> ,
> -
> >
> person_name
> VARCHAR
> (
> 100
> )
> NOT
> NULL
> ,
> -
> >
> PRIMARY
> KEY
> (
> person_id
> )
> -
> >
> )
> ;
> Query OK
> ,
> 0
> rows
> affected
> (
> 0.02
> sec
> )
> mysql
> >
> desc
> people
> -
> >
> ;
> +
> -------------+--------------+------+-----+---------+----------------+
> |
> Field
> |
> Type
> |
> Null
> |
> Key
> |
> Default
> |
> Extra
> |
> +
> -------------+--------------+------+-----+---------+----------------+
> |
> person_id
> |
> int
> (
> 11
> )
> |
> NO
> |
> PRI
> |
> NULL
> |
> auto_increment
> |
> |
> person_name
> |
> varchar
> (
> 100
> )
> |
> NO
> |
> |
> NULL
> |
> |
> +
> -------------+--------------+------+-----+---------+----------------+
> 2
> rows
> in
> set
> (
> 0.00
> sec
> )
> mysql
> >
> DML：插入数据

> mysql
> >
> insert
> into
> people
> values
> (
> '1001'
> ,
> 'liumiao'
> )
> ;
> Query OK
> ,
> 1
> row
> affected
> (
> 0.01
> sec
> )
> mysql
> >
> select
> *
> from
> people
> ;
> +
> -----------+-------------+
> |
> person_id
> |
> person_name
> |
> +
> -----------+-------------+
> |
> 1001
> |
> liumiao
> |
> +
> -----------+-------------+
> 1
> row
> in
> set
> (
> 0.00
> sec
> )
> mysql
> >
> 删表

> 不小心把这样的一张表给drop掉了

> mysql
> >
> select
> count
> (
> *
> )
> from
> com_organ
> ;
> +
> ----------+
> |
> count
> (
> *
> )
> |
> +
> ----------+
> |
> 1
> |
> +
> ----------+
> 1
> row
> in
> set
> (
> 0.00
> sec
> )
> mysql
> >
> drop
> table
> com_organ
> ;
> Query OK
> ,
> 0
> rows
> affected
> (
> 0.01
> sec
> )
> mysql
> >
> desc
> com_organ
> ;
> ERROR
> 1146
> (
> 42
> S02
> )
> :
> Table
> 'devops.com_organ'
> doesn't exist
mysql
> >
> 删库

> 然后不小心把库给删掉了

> mysql
> >
> drop
> database
> devops
> ;
> Query OK
> ,
> 139
> rows
> affected
> (
> 1.26
> sec
> )
> mysql
> >
> select
> database
> (
> )
> ;
> +
> ------------+
> |
> database
> (
> )
> |
> +
> ------------+
> |
> NULL
> |
> +
> ------------+
> 1
> row
> in
> set
> (
> 0.00
> sec
> )
> mysql
> >
> 对应确认

> 确认基本信息

> mysql
> >
> show
> master logs
> ;
> +
> -----------+-----------+
> |
> Log_name
> |
> File_size
> |
> +
> -----------+-----------+
> |
> ON
> .000001
> |
> 1055
> |
> +
> -----------+-----------+
> 1
> row
> in
> set
> (
> 0.00
> sec
> )
> mysql
> >
> show
> master
> status
> ;
> +
> -----------+----------+--------------+------------------+-------------------+
> |
> File
> |
> Position
> |
> Binlog_Do_DB
> |
> Binlog_Ignore_DB
> |
> Executed_Gtid_Set
> |
> +
> -----------+----------+--------------+------------------+-------------------+
> |
> ON
> .000001
> |
> 1055
> |
> |
> |
> |
> +
> -----------+----------+--------------+------------------+-------------------+
> 1
> row
> in
> set
> (
> 0.00
> sec
> )
> mysql
> >
> 确认问题点

> 使用如下前文介绍过的命令用于确认问题点

> mysql
> >
> show
> binlog events
> in
> 'ON.000001'
> ;
> +
> -----------+-----+----------------+-----------+-------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
> |
> Log_name
> |
> Pos
> |
> Event_type
> |
> Server_id
> |
> End_log_pos
> |
> Info
> |
> +
> -----------+-----+----------------+-----------+-------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
> |
> ON
> .000001
> |
> 4
> |
> Format_desc
> |
> 1001
> |
> 123
> |
> Server ver:
> 5.7
> .16
> -
> log
> ,
> Binlog ver:
> 4
> |
> |
> ON
> .000001
> |
> 123
> |
> Previous_gtids
> |
> 1001
> |
> 154
> |
> |
> |
> ON
> .000001
> |
> 154
> |
> Anonymous_Gtid
> |
> 1001
> |
> 219
> |
> SET
> @
> @SESSION.GTID_NEXT
> =
> 'ANONYMOUS'
> |
> |
> ON
> .000001
> |
> 219
> |
> Query
> |
> 1001
> |
> 428
> |
> use
> `
> devops
> `
> ;
> create
> table
> people
> (
> person_id
> INT
> NOT
> NULL
> AUTO_INCREMENT
> ,
> person_name
> VARCHAR
> (
> 100
> )
> NOT
> NULL
> ,
> PRIMARY
> KEY
> (
> person_id
> )
> )
> |
> |
> ON
> .000001
> |
> 428
> |
> Anonymous_Gtid
> |
> 1001
> |
> 493
> |
> SET
> @
> @SESSION.GTID_NEXT
> =
> 'ANONYMOUS'
> |
> |
> ON
> .000001
> |
> 493
> |
> Query
> |
> 1001
> |
> 567
> |
> BEGIN
> |
> |
> ON
> .000001
> |
> 567
> |
> Table_map
> |
> 1001
> |
> 621
> |
> table_id:
> 408
> (
> devops
> .
> people
> )
> |
> |
> ON
> .000001
> |
> 621
> |
> Write_rows
> |
> 1001
> |
> 670
> |
> table_id:
> 408
> flags: STMT_END_F
> |
> |
> ON
> .000001
> |
> 670
> |
> Xid
> |
> 1001
> |
> 701
> |
> COMMIT
> /* xid=3775 */
> |
> |
> ON
> .000001
> |
> 701
> |
> Anonymous_Gtid
> |
> 1001
> |
> 766
> |
> SET
> @
> @SESSION.GTID_NEXT
> =
> 'ANONYMOUS'
> |
> |
> ON
> .000001
> |
> 766
> |
> Query
> |
> 1001
> |
> 892
> |
> use
> `
> devops
> `
> ;
> DROP
> TABLE
> `
> com_organ
> `
> /* generated by server */
> |
> |
> ON
> .000001
> |
> 892
> |
> Anonymous_Gtid
> |
> 1001
> |
> 957
> |
> SET
> @
> @SESSION.GTID_NEXT
> =
> 'ANONYMOUS'
> |
> |
> ON
> .000001
> |
> 957
> |
> Query
> |
> 1001
> |
> 1055
> |
> drop
> database
> devops
> |
> +
> -----------+-----+----------------+-----------+-------------+---------------------------------------------------------------------------------------------------------------------------------------------------+
> 13
> rows
> in
> set
> (
> 0.00
> sec
> )
> mysql
> >
> 可以确认到Pos=766 到End_log_pos=1055的这波操作之中，drop 掉了这张本来还有用的表以及整个的库。

> 恢复数据

> 因为之前导出的数据中没有包含create database语句，所以这里手工先生成一下数据库。

> mysql
> >
> flush logs
> ;
> Query OK
> ,
> 0
> rows
> affected
> (
> 0.01
> sec
> )
> mysql
> >
> show
> master logs
> ;
> +
> -----------+-----------+
> |
> Log_name
> |
> File_size
> |
> +
> -----------+-----------+
> |
> ON
> .000001
> |
> 1095
> |
> |
> ON
> .000002
> |
> 154
> |
> +
> -----------+-----------+
> 2
> rows
> in
> set
> (
> 0.00
> sec
> )
> mysql
> >
> create
> database
> devops
> charset
> =
> utf8
> ;
> Query OK
> ,
> 1
> row
> affected
> (
> 0.01
> sec
> )
> mysql
> >
> 首先使用之前mysqldump的数据进行恢复

> \# ls devops_20190307112424.sql
> devops_20190307112424.sql
> \# mysql -uroot -pliiumiaocn devops <devops_20190307112424.sql
> mysql:
> [
> Warning
> ]
> Using a password on the
> command
> line interface can be insecure.
> \#
> 确认误删的表

> drop掉的内容已经从全备中找到。

> mysql
> >
> use
> devops
Reading
> table
> information
> for
> completion
> of
> table
> and
> column
> names
You can turn
> off
> this feature
> to
> get a quicker startup
> with
> -
> A
> Database
> changed
mysql
> >
> select
> count
> (
> *
> )
> from
> com_organ
> ;
> +
> ----------+
> |
> count
> (
> *
> )
> |
> +
> ----------+
> |
> 1
> |
> +
> ----------+
> 1
> row
> in
> set
> (
> 0.00
> sec
> )
> mysql
> >
> 此时再确认binlog，会发现开着binlog进行restore，binlog的增长非常之快，因为它记录了所有的内容。

> mysql
> >
> show
> master logs
> ;
> +
> -----------+-----------+
> |
> Log_name
> |
> File_size
> |
> +
> -----------+-----------+
> |
> ON
> .000001
> |
> 1095
> |
> |
> ON
> .000002
> |
> 259724
> |
> +
> -----------+-----------+
> 2
> rows
> in
> set
> (
> 0.00
> sec
> )
> mysql
> >
> show
> master
> status
> ;
> +
> -----------+----------+--------------+------------------+-------------------+
> |
> File
> |
> Position
> |
> Binlog_Do_DB
> |
> Binlog_Ignore_DB
> |
> Executed_Gtid_Set
> |
> +
> -----------+----------+--------------+------------------+-------------------+
> |
> ON
> .000002
> |
> 259724
> |
> |
> |
> |
> +
> -----------+----------+--------------+------------------+-------------------+
> 1
> row
> in
> set
> (
> 0.00
> sec
> )
> mysql
> >
> 确认新创建的表和插入的数据

> 而问题是新创建的表和修改的内容没有了，也就是说止损点只到备份时点，接下来需要使用binlog进行继续止损了。

> mysql
> >
> desc
> people
> ;
> ERROR
> 1146
> (
> 42
> S02
> )
> :
> Table
> 'devops.people'
> doesn't exist
mysql
> >
> 然后再利用binlog进行恢复，使用binlog进行数据恢复，相关的命令如下所示

> mysqlbinlog [Option] binlog文件名 |mysql -u用户名 -p密码 数据库名

> 常用Option

> –start-position=nnn ：设定其实pos点

> –stop-position=nnn： 设定结束pos点

> –start-datetime=“yyyy-mm-dd hh:mm:ss” ：设定起始时间点

> –stop-datetime=“yyyy-mm-dd hh:mm:ss”  ：设定结束时间点

> –database=数据库名：指定恢复的数据库实例

> 更加详细的设定，可参看：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190307233414445.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190307233427921.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> 执行恢复

> 通过使用-v打开信息输出，可以看出其实就是类似redo的重做而已。

> \# mysqlbinlog --stop-position=766 --database=devops /var/lib/mysql/ON.000001 |mysql -uroot -pliumiaocn -v devops
> mysql:
> [
> Warning
> ]
> Using
> a password
> on
> the command line interface can be insecure
> .
> WARNING: The
> option
> --database has been used. It may filter parts of transactions, but will include the GTIDs in any case. If you want to exclude or include transactions, you should use the options --exclude-gtids or --include-gtids, respectively, instead.
> --------------
> /*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=1*/
> --------------
> --------------
> /*!50003 SET @OLD_COMPLETION_TYPE=@@COMPLETION_TYPE,COMPLETION_TYPE=0*/
> --------------
> --------------
> ROLLBACK
> --------------
> --------------
> BINLOG
> '
NF2BXA/pAwAAdwAAAHsAAAAAAAQANS43LjE2LWxvZwAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAA0XYFcEzgNAAgAEgAEBAQEEgAAXwAEGggAAAAICAgCAAAACgoKKioAEjQA
AQkaBGc=
'
> --------------
> --------------
> SET
> @
> @SESSION.GTID_NEXT
> =
> 'ANONYMOUS'
> --------------
> --------------
> SET
> TIMESTAMP
> =
> 1551981918
> --------------
> --------------
> SET
> @
> @session.pseudo_thread_id
> =
> 10
> --------------
> --------------
> SET
> @
> @session.foreign_key_checks
> =
> 1
> ,
> @
> @session.sql_auto_is_null
> =
> 0
> ,
> @
> @session.unique_checks
> =
> 1
> ,
> @
> @session.autocommit
> =
> 1
> --------------
> --------------
> SET
> @
> @session.sql_mode
> =
> 1436549152
> --------------
> --------------
> SET
> @
> @session.auto_increment_increment
> =
> 1
> ,
> @
> @session.auto_increment_offset
> =
> 1
> --------------
> --------------
> /*!*/
> --------------
> --------------
> SET
> @
> @session.character_set_client
> =
> 8
> ,
> @
> @session.collation_connection
> =
> 8
> ,
> @
> @session.collation_server
> =
> 8
> --------------
> --------------
> SET
> @
> @session.lc_time_names
> =
> 0
> --------------
> --------------
> SET
> @
> @session.collation_database
> =
> DEFAULT
> --------------
> --------------
> create
> table
> people
> (
> person_id
> INT
> NOT
> NULL
> AUTO_INCREMENT
> ,
> person_name
> VARCHAR
> (
> 100
> )
> NOT
> NULL
> ,
> PRIMARY
> KEY
> (
> person_id
> )
> )
> --------------
> --------------
> SET
> @
> @SESSION.GTID_NEXT
> =
> 'ANONYMOUS'
> --------------
> --------------
> SET
> TIMESTAMP
> =
> 1551981929
> --------------
> --------------
> BEGIN
> --------------
> --------------
> BINLOG
> '
aV2BXBPpAwAANgAAAG0CAAAAAJgBAAAAAAEABmRldm9wcwAGcGVvcGxlAAIDDwIsAQAsH/t0
aV2BXB7pAwAAMQAAAJ4CAAAAAJgBAAAAAAEAAgAC//zpAwAABwBsaXVtaWFvA93yIw==
'
> --------------
> --------------
> COMMIT
> --------------
> --------------
> SET
> @
> @SESSION.GTID_NEXT
> =
> 'ANONYMOUS'
> --------------
> --------------
> BEGIN
> --------------
> --------------
> ROLLBACK
> --------------
> --------------
> SET
> @
> @SESSION.GTID_NEXT
> =
> 'AUTOMATIC'
> --------------
> --------------
> /*!50003 SET COMPLETION_TYPE=@OLD_COMPLETION_TYPE*/
> --------------
> --------------
> /*!50530 SET @@SESSION.PSEUDO_SLAVE_MODE=0*/
> --------------
> \#
> PS：提示的信息可以根据情况通过–skip-gtids=true来进行处理。

> 结果确认

> 可以看到当日的信息也被复原了，更为复杂的情况，则可能需要多次指定开始和结束点进行重做方式的恢复。

> mysql
> >
> desc
> people
> ;
> +
> -------------+--------------+------+-----+---------+----------------+
> |
> Field
> |
> Type
> |
> Null
> |
> Key
> |
> Default
> |
> Extra
> |
> +
> -------------+--------------+------+-----+---------+----------------+
> |
> person_id
> |
> int
> (
> 11
> )
> |
> NO
> |
> PRI
> |
> NULL
> |
> auto_increment
> |
> |
> person_name
> |
> varchar
> (
> 100
> )
> |
> NO
> |
> |
> NULL
> |
> |
> +
> -------------+--------------+------+-----+---------+----------------+
> 2
> rows
> in
> set
> (
> 0.00
> sec
> )
> mysql
> >
> select
> *
> from
> people
> ;
> +
> -----------+-------------+
> |
> person_id
> |
> person_name
> |
> +
> -----------+-------------+
> |
> 1001
> |
> liumiao
> |
> +
> -----------+-------------+
> 1
> row
> in
> set
> (
> 0.00
> sec
> )
> mysql
> >
> 参考文章

> [https://dev.mysql.com/doc/refman/5.6/en/mysqlbinlog.html](https://dev.mysql.com/doc/refman/5.6/en/mysqlbinlog.html)

> [
						](https://img-blog.csdnimg.cn/20190307233414445.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)

> [
	](https://img-blog.csdnimg.cn/20190307233414445.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
