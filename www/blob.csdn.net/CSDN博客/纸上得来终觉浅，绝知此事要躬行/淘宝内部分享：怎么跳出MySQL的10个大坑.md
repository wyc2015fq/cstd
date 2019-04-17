# 淘宝内部分享：怎么跳出MySQL的10个大坑 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月06日 21:19:04[boonya](https://me.csdn.net/boonya)阅读数：357标签：[mysql																[性能优化](https://so.csdn.net/so/search/s.do?q=性能优化&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[Mysql/Mycat](https://blog.csdn.net/boonya/article/category/6397509)








原文地址：http://www.csdn.net/article/2015-01-16/2823591


## 淘宝自从2010开始规模使用MySQL，替换了之前商品、交易、用户等原基于IOE方案的核心数据库，目前已部署数千台规模。同时和Oracle, Percona, Mariadb等上游厂商有良好合作，共向上游提交20多个Patch。目前淘宝核心系统研发部数据库组，根据淘宝的业务需求，改进数据库和提升性能，提供高性能、可扩展的、稳定可靠的数据库(存储)解决方案。

## 目前有以下几个方向：

## 单机，提升单机数据库的性能，增加我们所需特性；

## 集群，提供性能扩展，可靠性，可能涉及分布式事务处理；

## IO存储体系，跟踪IO设备变化潮流， 研究软硬件结合，输出高性能存储解决方案。

## 本文是来自淘宝内部数据库内容分享。

## MySQL · 性能优化· Group Commit优化

## **背景**

关于Group Commit网上的资料其实已经足够多了，我这里只简单的介绍一下。

众所周知，在MySQL5.6之前的版本，由于引入了Binlog/InnoDB的XA，Binlog的写入和InnoDB commit完全串行化执行，大概的执行序列如下：
 InnoDB prepare  （持有prepare_commit_mutex）；
 write/sync Binlog；
 InnoDB commit (写入COMMIT标记后释放prepare_commit_mutex)。当sync_binlog=1时，很明显上述的第二步会成为瓶颈，而且还是持有全局大锁，这也是为什么性能会急剧下降。

很快Mariadb就提出了一个Binlog Group Commit方案，即在准备写入Binlog时，维持一个队列，最早进入队列的是leader，后来的是follower，leader为搜集到的队列中的线程依次写Binlog文件, 并commit事务。Percona 的Group Commit实现也是Port自Mariadb。不过仍在使用Percona Server5.5的朋友需要注意，该Group Commit实现可能破坏掉Semisync的行为，感兴趣的点击 
[bug#1254571](https://bugs.launchpad.net/percona-server/5.5/+bug/1254571)

Oracle MySQL 在5.6版本开始也支持Binlog Group Commit，使用了和Mariadb类似的思路，但将Group Commit的过程拆分成了三个阶段：flush stage 将各个线程的binlog从cache写到文件中; sync stage 对binlog做fsync操作（如果需要的话）；commit stage 为各个线程做引擎层的事务commit。每个stage同时只有一个线程在操作。

Tips：当引入Group Commit后，sync_binlog的含义就变了，假定设为1000，表示的不是1000个事务后做一次fsync，而是1000个事务组。

Oracle MySQL的实现的优势在于三个阶段可以并发执行，从而提升效率。

**XA Recover**

在Binlog打开的情况下，MySQL默认使用MySQL_BIN_LOG来做XA协调者，大致流程为：

> 
1.扫描最后一个Binlog文件，提取其中的xid； 

2.InnoDB维持了状态为Prepare的事务链表，将这些事务的xid和Binlog中记录的xid做比较，如果在Binlog中存在，则提交，否则回滚事务。

通过这种方式，可以让InnoDB和Binlog中的事务状态保持一致。显然只要事务在InnoDB层完成了Prepare，并且写入了Binlog，就可以从崩溃中恢复事务，这意味着我们无需在InnoDB commit时显式的write/fsync redo log。

Tips：MySQL为何只需要扫描最后一个Binlog文件呢 ？ 原因是每次在rotate到新的Binlog文件时，总是保证没有正在提交的事务，然后fsync一次InnoDB的redo log。这样就可以保证老的Binlog文件中的事务在InnoDB总是提交的。

**问题**

其实问题很简单：每个事务都要保证其Prepare的事务被write/fsync到redo log文件。尽管某个事务可能会帮助其他事务完成redo 写入，但这种行为是随机的，并且依然会产生明显的log_sys->mutex开销。

**优化**

从XA恢复的逻辑我们可以知道，只要保证InnoDB Prepare的redo日志在写Binlog前完成write/sync即可。因此我们对Group Commit的第一个stage的逻辑做了些许修改，大概描述如下：

> 
 Step1. InnoDB Prepare，记录当前的LSN到thd中； 

 Step2. 进入Group Commit的flush stage；Leader搜集队列，同时算出队列中最大的LSN。 

 Step3. 将InnoDB的redo log write/fsync到指定的LSN 

 Step4. 写Binlog并进行随后的工作(sync Binlog, InnoDB commit , etc)

通过延迟写redo log的方式，显式的为redo log做了一次组写入，并减少了log_sys->mutex的竞争。

目前官方MySQL已经根据我们report的bug#73202锁提供的思路，对5.7.6的代码进行了优化，对应的Release Note如下：

> 
When using InnoDB with binary logging enabled, concurrent transactions written in the InnoDB redo log are now grouped together before synchronizing to disk when innodb_flush_log_at_trx_commit is set to 1, which reduces the amount of synchronization operations.
 This can lead to improved performance.

**性能数据**

简单测试了下，使用sysbench, update_non_index.lua, 100张表，每张10w行记录，innodb_flush_log_at_trx_commit=2, sync_binlog=1000，关闭Gtid
 并发线程        原生                  修改后
 32             25600                27000
 64             30000                35000
 128            33000                39000
 256            29800                38000

## MySQL · 新增特性· DDL fast fail

**背景**

项目的快速迭代开发和在线业务需要保持持续可用的要求，导致MySQL的ddl变成了DBA很头疼的事情，而且经常导致故障发生。本篇介绍RDS分支上做的一个功能改进，DDL fast fail。主要解决：DDL操作因为无法获取MDL排它锁，进入等待队列的时候，阻塞了应用所有的读写请求问题。

**MDL锁机制介绍**

首先介绍一下MDL(METADATA LOCK)锁机制，MySQL为了保证表结构的完整性和一致性，对表的所有访问都需要获得相应级别的MDL锁，比如以下场景：

> 
session 1: start transaction; select * from test.t1; 

 session 2: alter table test.t1 add extra int; 

 session 3: select * from test.t1;
- session 1对t1表做查询，首先需要获取t1表的MDL_SHARED_READ级别MDL锁。锁一直持续到commit结束，然后释放。
- session 2对t1表做DDL，需要获取t1表的MDL_EXCLUSIVE级别MDL锁，因为MDL_SHARED_READ与MDL_EXCLUSIVE不相容，所以session 2被session 1阻塞，然后进入等待队列。
- session 3对t1表做查询，因为等待队列中有MDL_EXCLUSIVE级别MDL锁请求，所以session3也被阻塞，进入等待队列。

这种场景就是目前因为MDL锁导致的很经典的阻塞问题，如果session1长时间未提交，或者查询持续过长时间，那么后续对t1表的所有读写操作，都被阻塞。 对于在线的业务来说，很容易导致业务中断。

**aliyun RDS分支改进**

DDL fast fail并没有解决真正DDL过程中的阻塞问题，但避免了因为DDL操作没有获取锁，进而导致业务其他查询/更新语句阻塞的问题。

其实现方式如下：

> 
alter table test.t1 no_wait/wait 1 add extra int; 

在ddl语句中，增加了no_wait/wait 1语法支持。

其处理逻辑如下：

首先尝试获取t1表的MDL_EXCLUSIVE级别的MDL锁：
- 当语句指定的是no_wait，如果获取失败，客户端将得到报错信息：ERROR : Lock wait timeout exceeded; try restarting transaction。
- 当语句指定的是wait 1，如果获取失败，最多等待1s，然后得到报错信息：ERROR : Lock wait timeout exceeded; try restarting transaction。

另外，除了alter语句以外，还支持rename，truncate，drop，optimize，create index等ddl操作。

**与Oracle的比较**

在Oracle 10g的时候，DDL操作经常会遇到这样的错误信息：

> 
ora-00054:resource busy and acquire with nowait specified 即DDL操作无法获取表上面的排它锁，而fast fail。

其实DDL获取排他锁的设计，需要考虑的就是两个问题：
- 雪崩，如果你采用排队阻塞的机制，那么DDL如果长时间无法获取锁，就会导致应用的雪崩效应，对于高并发的业务，也是灾难。
- 饿死，如果你采用强制式的机制，那么要防止DDL一直无法获取锁的情况，在业务高峰期，可能DDL永远无法成功。

在Oracle 11g的时候，引入了DDL_LOCK_TIMEOUT参数，如果你设置了这个参数，那么DDL操作将使用排队阻塞模式，可以在session和global级别设置， 给了用户更多选择。

## MySQL · 性能优化· 启用GTID场景的性能问题及优化

**背景**

MySQL从5.6版本开始支持GTID特性，也就是所谓全局事务ID，在整个复制拓扑结构内，每个事务拥有自己全局唯一标识。GTID包含两个部分，一部分是实例的UUID，另一部分是实例内递增的整数。

GTID的分配包含两种方式，一种是自动分配，另外一种是显式设置session.gtid_next，下面简单介绍下这两种方式：

**自动分配**

如果没有设置session级别的变量gtid_next，所有事务都走自动分配逻辑。分配GTID发生在GROUP COMMIT的第一个阶段，也就是flush stage，大概可以描述为：
-  Step 1：事务过程中，碰到第一条DML语句需要记录Binlog时，分配一段Gtid事件的cache，但不分配实际的GTID
-  Step 2：事务完成后，进入commit阶段，分配一个GTID并写入Step1预留的Gtid事件中，该GTID必须保证不在gtid_owned集合和gtid_executed集合中。 分配的GTID随后被加入到gtid_owned集合中。
-  Step 3：将Binlog 从线程cache中刷到Binlog文件中。
-  Step 4：将GTID加入到gtid_executed集合中。
-  Step 5：在完成sync stage 和commit stage后，各个会话将其使用的GTID从gtid_owned中移除。

**显式设置**

用户通过设置session级别变量gtid_next可以显式指定一个GTID，流程如下：
-  Step 1：设置变量gtid_next，指定的GTID被加入到gtid_owned集合中。
-  Step 2：执行任意事务SQL，在将binlog从线程cache刷到binlog文件后，将GTID加入到gtid_executed集合中。
-  Step 3：在完成事务COMMIT后，从gtid_owned中移除。

备库SQL线程使用的就是第二种方式，因为备库在apply主库的日志时，要保证GTID是一致的，SQL线程读取到GTID事件后，就根据其中记录的GTID来设置其gtid_next变量。

**问题**

由于在实例内，GTID需要保证唯一性，因此不管是操作gtid_executed集合和gtid_owned集合，还是分配GTID，都需要加上一个大锁。我们的优化主要集中在第一种GTID分配方式。

对于GTID的分配，由于处于Group Commit的第一个阶段，由该阶段的leader线程为其follower线程分配GTID及刷Binlog，因此不会产生竞争。

而在Step 5，各个线程在完成事务提交后，各自去从gtid_owned集合中删除其使用的gtid。这时候每个线程都需要获取互斥锁，很显然，并发越高，这种竞争就越明显，我们很容易从pt-pmp输出中看到如下类似的trace：
 ha_commit_trans—>MySQL_BIN_LOG::commit—>MySQL_BIN_LOG::ordered_commit—>MySQL_BIN_LOG::finish_commit—>Gtid_state::update_owned_gtids_impl—>lock_sidno
这同时也会影响到GTID的分配阶段，导致TPS在高并发场景下的急剧下降。

**解决**

实际上对于自动分配GTID的场景，并没有必要维护gtid_owned集合。我们的修改也非常简单，在自动分配一个GTID后，直接加入到gtid_executed集合中，避免维护gtid_owned，这样事务提交时就无需去清理gtid_owned集合了，从而可以完全避免锁竞争。

当然为了保证一致性，如果分配GTID后，写入Binlog文件失败，也需要从gtid_executed集合中删除。不过这种场景非常罕见。

**性能数据**

使用sysbench，100张表，每张10w行记录，update_non_index.lua，纯内存操作，innodb_flush_log_at_trx_commit = 2，sync_binlog = 1000
 并发线程       原生               修改后
 32           24500              25000
 64           27900              29000
 128          30800              31500
 256          29700              32000
 512          29300              31700
 1024         27000              31000

从测试结果可以看到，优化前随着并发上升，性能出现下降，而优化后则能保持TPS稳定。

## MySQL · 捉虫动态· InnoDB自增列重复值问题

**问题重现**

先从问题入手，重现下这个 [bug](http://bugs.MySQL.com/bug.php?id=199)
use test;
drop table if exists t1;
create table t1(id int auto_increment, a int, primary key (id)) engine=innodb;
insert into t1 values (1,2);
insert into t1 values (null,2);
insert into t1 values (null,2);
select * from t1;
+----+------+
| id | a |
+----+------+
| 1 | 2 |
| 2 | 2 |
| 3 | 2 |
+----+------+
delete from t1 where id=2;
delete from t1 where id=3;
select * from t1;
+----+------+
| id | a |
+----+------+
| 1 | 2 |
+----+------+
这里我们关闭MySQL，再启动MySQL,然后再插入一条数据
insert into t1 values (null,2);
select * FROM T1;
+----+------+
| id | a |
+----+------+
| 1 | 2 |
+----+------+
| 2 | 2 |
+----+------+
我们看到插入了（2,2），而如果我没有重启，插入同样数据我们得到的应该是（4,2)。 上面的测试反映了MySQLd重启后，InnoDB存储引擎的表自增id可能出现重复利用的情况。

自增id重复利用在某些场景下会出现问题。依然用上面的例子，假设t1有个历史表t1_history用来存t1表的历史数据，那么MySQLd重启前,ti_history中可能已经有了（2,2）这条数据，而重启后我们又插入了（2，2），当新插入的(2,2)迁移到历史表时，会违反主键约束。

**原因分析**

InnoDB 自增列出现重复值的原因：
MySQL> show create table t1\G;
*************************** 1. row ***************************
Table: t1
Create Table: CREATE TABLE `t1` (
`id` int(11) NOT NULL AUTO_INCREMENT,
`a` int(11) DEFAULT NULL,
PRIMARY KEY (`id`)
) ENGINE=innodb AUTO_INCREMENT=4 DEFAULT CHARSET=utf8
1 row in set (0.00 sec)
建表时可以指定 AUTO_INCREMENT值，不指定时默认为1，这个值表示当前自增列的起始值大小，如果新插入的数据没有指定自增列的值，那么自增列的值即为这个起始值。对于InnoDB表，这个值没有持久到文件中。而是存在内存中（dict_table_struct.autoinc）。那么又问，既然这个值没有持久下来，为什么我们每次插入新的值后， show create table t1看到AUTO_INCREMENT值是跟随变化的。其实show create table t1是直接从dict_table_struct.autoinc取得的（ha_innobase::update_create_info）。

知道了AUTO_INCREMENT是实时存储内存中的。那么，MySQLd 重启后，从哪里得到AUTO_INCREMENT呢? 内存值肯定是丢失了。实际上MySQL采用执行类似select max(id)+1 from t1;方法来得到AUTO_INCREMENT。而这种方法就是造成自增id重复的原因。

**MyISAM自增值**

MyISAM也有这个问题吗？MyISAM是没有这个问题的。myisam会将这个值实时存储在.MYI文件中（mi_state_info_write）。MySQLd重起后会从.MYI中读取AUTO_INCREMENT值（mi_state_info_read)。因此，MyISAM表重启是不会出现自增id重复的问题。

**问题修复**

MyISAM选择将AUTO_INCREMENT实时存储在.MYI文件头部中。实际上.MYI头部还会实时存其他信息，也就是说写AUTO_INCREMENT只是个顺带的操作，其性能损耗可以忽略。InnoDB 表如果要解决这个问题，有两种方法。

> 
1）将AUTO_INCREMENT最大值持久到frm文件中。 

2）将 AUTO_INCREMENT最大值持久到聚集索引根页trx_id所在的位置。

第一种方法直接写文件性能消耗较大，这是一额外的操作，而不是一个顺带的操作。我们采用第二种方案。为什么选择存储在聚集索引根页页头trx_id，页头中存储trx_id,只对二级索引页和insert buf 页头有效（MVCC)。而聚集索引根页页头trx_id这个值是没有使用的，始终保持初始值0。正好这个位置8个字节可存放自增值的值。我们每次更新AUTO_INCREMENT值时，同时将这个值修改到聚集索引根页页头trx_id的位置。 这个写操作跟真正的数据写操作一样，遵守write-ahead log原则，只不过这里只需要redo
 log ,而不需要undo log。因为我们不需要回滚AUTO_INCREMENT的变化（即回滚后自增列值会保留，即使insert 回滚了，AUTO_INCREMENT值不会回滚）。

因此，AUTO_INCREMENT值存储在聚集索引根页trx_id所在的位置，实际上是对内存根页的修改和多了一条redo log（量很小）,而这个redo log 的写入也是异步的，可以说是原有事务log的一个顺带操作。因此AUTO_INCREMENT值存储在聚集索引根页这个性能损耗是极小的。

修复后的性能对比，我们新增了全局参数innodb_autoinc_persistent 取值on/off； on 表示将AUTO_INCREMENT值实时存储在聚集索引根页。off则采用原有方式只存储在内存。
./bin/sysbench --test=sysbench/tests/db/insert.lua --MySQL-port=4001 --MySQL-user=root \--MySQL-table-engine=innodb --MySQL-db=sbtest --oltp-table-size=0 --oltp-tables-count=1 \--num-threads=100 --MySQL-socket=/u01/zy/sysbench/build5/run/MySQL.sock  --max-time=7200 --max-requests run
set global innodb_autoinc_persistent=off;
tps: 22199 rt:2.25ms
set global innodb_autoinc_persistent=on;
tps: 22003 rt:2.27ms
可以看出性能损耗在%1以下。

**改进**

新增参数innodb_autoinc_persistent_interval 用于控制持久化AUTO_INCREMENT值的频率。例如：innodb_autoinc_persistent_interval=100，auto_incrememt_increment=1时，即每100次insert会控制持久化一次AUTO_INCREMENT值。每次持久的值为：当前值+innodb_autoinc_persistent_interval。

测试结论

> 
 innodb_autoinc_persistent=ON, innodb_autoinc_persistent_interval=1时性能损耗在%1以下。 

 innodb_autoinc_persistent=ON, innodb_autoinc_persistent_interval=100时性能损耗可以忽略。

**限制**
-  innodb_autoinc_persistent=on， innodb_autoinc_persistent_interval=N>1时，自增N次后持久化到聚集索引根页,每次持久的值为当前AUTO_INCREMENT+(N-1)*innodb_autoextend_increment。重启后读取持久化的AUTO_INCREMENT值会偏大，造成一些浪费但不会重复。innodb_autoinc_persistent_interval=1
 每次都持久化没有这个问题。
-  如果innodb_autoinc_persistent=on，频繁设置auto_increment_increment的可能会导致持久化到聚集索引根页的值不准确。因为innodb_autoinc_persistent_interval计算没有考虑auto_increment_increment变化的情况，参看dict_table_autoinc_update_if_greater。而设置auto_increment_increment的情况极少，可以忽略。

注意：如果我们使用需要开启innodb_autoinc_persistent，应该在参数文件中指定
innodb_autoinc_persistent= on
如果这样指定set global innodb_autoinc_persistent=on;重启后将不会从聚集索引根页读取AUTO_INCREMENT最大值。

疑问：对于InnoDB表，重启通过select max(id)+1 from t1得到AUTO_INCREMENT值，如果id上有索引那么这个语句使用索引查找就很快。那么，这个可以解释MySQL 为什么要求自增列必须包含在索引中的原因。 如果没有指定索引，则报如下错误，

ERROR 1075 (42000): Incorrect table definition; there can be only one auto column and it must be defined as a key 而myisam表竟然也有这个要求，感觉是多余的。

## MySQL · 优化改进· 复制性能改进过程

**前言**

与oracle 不同，MySQL 的主库与备库的同步是通过 binlog 实现的，而redo日志只做为MySQL 实例的crash recovery使用。MySQL在4.x 的时候放弃redo 的同步策略而引入 binlog的同步，一个重要原因是为了兼容其它非事务存储引擎，否则主备同步是没有办法进行的。

redo 日志同步属于物理同步方法，简单直接，将修改的物理部分传送到备库执行，主备共用一致的 LSN，只要保证 LSN 相同即可，同一时刻，只能主库或备库一方接受写请求； binlog的同步方法属于逻辑复制，分为statement 或 row 模式，其中statement记录的是SQL语句，Row 模式记录的是修改之前的记录与修改之后的记录，即前镜像与后镜像；备库通过binlog dump 协议拉取binlog,然后在备库执行。如果拉取的binlog是SQL语句，备库会走和主库相同的逻辑，如果是row
 格式，则会调用存储引擎来执行相应的修改。

本文简单说明5.5到5.7的主备复制性能改进过程。

replication improvement (from 5.5 to 5.7)

**(1) 5.5 中，binlog的同步是由两个线程执行的**

io_thread: 根据binlog dump协议从主库拉取binlog, 并将binlog转存到本地的relaylog；

sql_thread: 读取relaylog，根据位点的先后顺序执行binlog event，进而将主库的修改同步到备库，达到主备一致的效果； 由于在主库的更新是由多个客户端执行的，所以当压力达到一定的程度时，备库单线程执行主库的binlog跟不上主库执行的速度，进而会产生延迟造成备库不可用，这也是分库的原因之一，其SQL线程的执行堆栈如下：
sql_thread:
exec_relay_log_event
    apply_event_and_update_pos
         apply_event
             rows_log_event::apply_event
                 storage_engine operation
         update_pos
(2) **5.6 中，引入了多线程模式，在多线程模式下，其线程结构如下**

io_thread: 同5.5

Coordinator_thread: 负责读取 relay log，将读取的binlog event以事务为单位分发到各个 worker thread 进行执行，并在必要时执行binlog event（Description_format_log_event， Rotate_log_event 等）。

worker_thread: 执行分配到的binlog event，各个线程之间互不影响；

**多线程原理**

sql_thread 的分发原理是依据当前事务所操作的数据库名称来进行分发，如果事务是跨数据库行为的，则需要等待已分配的该数据库的事务全部执行完毕，才会继续分发，其分配行为的伪码可以简单的描述如下：
get_slave_worker
  if (contains_partition_info(log_event))
     db_name= get_db_name(log_event);
     entry {db_name, worker_thread, usage} = map_db_to_worker(db_name);
     while (entry->usage > 0)
        wait();
    return worker;
  else if (last_assigned_worker)
    return last_assigned_worker;
  else
    push into buffer_array and deliver them until come across a event that have partition info
**需要注意的细节**
- 内存的分配与释放。relay thread 每读取一个log_event, 则需要 malloc 一定的内存，在work线程执行完后，则需要free掉；
- 数据库名 与 worker 线程的绑定信息在一个hash表中进行维护，hash表以entry为单位，entry中记录当前entry所代表的数据库名，有多少个事务相关的已被分发，执行这些事务的worker thread等信息；
- 维护一个绑定信息的array , 在分发事务的时候，更新绑定信息，增加相应 entry->usage, 在执行完一个事务的时候，则需要减少相应的entry->usage；
- slave worker 信息的维护，即每个 worker thread执行了哪些事务，执行到的位点是在哪，延迟是如何计算的，如果执行出错，mts_recovery_group 又是如何恢复的；
- 分配线程是以数据库名进行分发的，当一个实例中只有一个数据库的时候，不会对性能有提高，相反，由于增加额外的操作，性能还会有一点回退；
- 临时表的处理，临时表是和entry绑定在一起的，在执行的时候将entry的临时表挂在执行线程thd下面，但没有固化，如果在临时表操作期间，备库crash，则重启后备库会有错误；

**总体上说，5.6 的并行复制打破了5.5 单线程的复制的行为，只是在单库下用处不大，并且5.6的并行复制的改动引入了一些重量级的bug**
- MySQL slave sql thread memory leak (http://bugs.MySQL.com/bug.php?id=71197)
- Relay log without xid_log_event may case parallel replication hang (http://bugs.MySQL.com/bug.php?id=72794)
- Transaction lost when relay_log_info_repository=FILE and crashed (http://bugs.MySQL.com/bug.php?id=73482)

(3) 5.7中，并行复制的实现添加了另外一种并行的方式，即主库在 ordered_commit中的第二阶段的时候，将同一批commit的 binlog 打上一个相同的seqno标签，同一时间戳的事务在备库是可以同时执行的，因此大大简化了并行复制的逻辑，并打破了相同 DB 不能并行执行的限制。备库在执行时，具有同一seqno的事务在备库可以并行的执行，互不干扰，也不需要绑定信息，后一批seqno的事务需要等待前一批相同seqno的事务执行完后才可以执行。

详细实现可参考： [http://bazaar.launchpad.net/~MySQL/MySQL-server/5.7/revision/6256](http://bazaar.launchpad.net/~MySQL/MySQL-server/5.7/revision/6256) 。

reference： [http://geek.rohitkalhans.com/2013/09/enhancedMTS-deepdive.html](http://geek.rohitkalhans.com/2013/09/enhancedMTS-deepdive.html)

## MySQL · 谈古论今· key分区算法演变分析

本文说明一个物理升级导致的 "数据丢失"。

**现象**

在MySQL 5.1下新建key分表，可以正确查询数据。
drop table t1;

create table t1 (c1 int , c2 int) 
PARTITION BY KEY (c2) partitions 5; 
insert into t1  values(1,1785089517),(2,null); 
MySQL> select * from t1 where c2=1785089517;
+------+------------+
| c1   | c2         |
+------+------------+
|    1 | 1785089517 |
+------+------------+
1 row in set (0.00 sec)
MySQL> select * from t1 where c2 is null;
+------+------+
| c1   | c2   |
+------+------+
|    2 | NULL |
+------+------+
1 row in set (0.00 sec)
而直接用MySQL5.5或MySQL5.6启动上面的5.1实例，发现(1,1785089517)这行数据不能正确查询出来。
alter table t1 PARTITION BY KEY ALGORITHM = 1 (c2)  partitions 5;
MySQL> select * from t1 where c2 is null;
+------+------+
| c1   | c2   |
+------+------+
|    2 | NULL |
+------+------+
1 row in set (0.00 sec)
MySQL> select * from t1 where c2=1785089517;
Empty set (0.00 sec)
**原因分析**

跟踪代码发现，5.1 与5.5,5.6 key hash算法是有区别的。

5.1 对于非空值的处理算法如下
void my_hash_sort_bin(const CHARSET_INFO *cs __attribute__((unused)),
                     const uchar *key, size_t len,ulong *nr1, ulong *nr2)
{
  const uchar *pos = key; 
                         
  key+= len;
 
  for (; pos < (uchar*) key ; pos++)
  {
    nr1[0]^=(ulong) ((((uint) nr1[0] & 63)+nr2[0]) * 
             ((uint)*pos)) + (nr1[0] << 8);
    nr2[0]+=3;
  }
}
通过此算法算出数据(1,1785089517)在第3个分区

5.5和5.6非空值的处理算法如下
void my_hash_sort_simple(const CHARSET_INFO *cs,
                         const uchar *key, size_t len,
                         ulong *nr1, ulong *nr2)
{
  register uchar *sort_order=cs->sort_order;
  const uchar *end;
 
  /* 
    Remove end space. We have to do this to be able to compare
    'A ' and 'A' as identical
  */        
  end= skip_trailing_space(key, len);
 
  for (; key < (uchar*) end ; key++)
  {
    nr1[0]^=(ulong) ((((uint) nr1[0] & 63)+nr2[0]) * 
            ((uint) sort_order[(uint) *key])) + (nr1[0] << 8);
    nr2[0]+=3;
  }
}
通过此算法算出数据(1,1785089517)在第5个分区，因此，5.5,5.6查询不能查询出此行数据。

5.1,5.5,5.6对于空值的算法还是一致的,如下
if (field->is_null())
{
  nr1^= (nr1 << 1) | 1;
  continue;
}
都能正确算出数据(2, null)在第3个分区。因此，空值可以正确查询出来。

那么是什么导致非空值的hash算法走了不同路径呢？在5.1下，计算字段key hash固定字符集就是my_charset_bin，对应的hash 函数就是前面的my_hash_sort_simple。而在5.5，5.6下，计算字段key hash的字符集是随字段变化的，字段c2类型为int对应my_charset_numeric，与之对应的hash函数为my_hash_sort_simple。具体可以参考函数Field::hash

那么问题又来了，5.5后为什么算法会变化呢？原因在于官方关于字符集策略的调整，详见 [WL#2649](http://dev.MySQL.com/worklog/) 。

**兼容处理**

前面讲到，由于hash 算法变化，用5.5，5.6启动5.1的实例，导致不能正确查询数据。那么5.1升级5.5,5.6就必须兼容这个问题.MySQL 5.5.31以后，提供了专门的语法 ALTER TABLE ... PARTITION BY ALGORITHM=1 [LINEAR] KEY ...  用于兼容此问题。对于上面的例子，用5.5或5.6启动5.1的实例后执行
MySQL> alter table t1 PARTITION BY KEY ALGORITHM = 1 (c2) partitions 5;
Query OK, 2 rows affected (0.02 sec)
Records: 2  Duplicates: 0  Warnings: 0

MySQL> select * from t1 where c2=1785089517;
+------+------------+
| c1   | c2         |
+------+------------+
|    1 | 1785089517 |
+------+------------+
1 row in set (0.00 sec)
数据可以正确查询出来了。

而实际上5.5,5.6的MySQL_upgrade升级程序已经提供了兼容方法。MySQL_upgrade 执行check table xxx for upgrade 会检查key分区表是否用了老的算法。如果使用了老的算法，会返回
MySQL> CHECK TABLE t1  FOR UPGRADE\G
*************************** 1. row ***************************
   Table: test.t1
      Op: check
Msg_type: error
Msg_text: KEY () partitioning changed, please run:
ALTER TABLE `test`.`t1` PARTITION BY KEY /*!50611 ALGORITHM = 1 */ (c2)
PARTITIONS 5
*************************** 2. row ***************************
   Table: test.t1
      Op: check
Msg_type: status
Msg_text: Operation failed
2 rows in set (0.00 sec)
检查到错误信息后会自动执行以下语句进行兼容。
ALTER TABLE `test`.`t1` PARTITION BY KEY /*!50611 ALGORITHM = 1 */ (c2) PARTITIONS 5。
## MySQL · 捉虫动态· MySQL client crash一例

**背景**

客户使用MySQLdump导出一张表，然后使用MySQL -e 'source test.dmp'的过程中client进程crash，爆出内存的segment fault错误，导致无法导入数据。

**问题定位**

test.dmp文件大概50G左右，查看了一下文件的前几行内容，发现：
 A partial dump from a server that has GTIDs will by default include the GTIDs of all transactions, even those that changed suppressed parts of the database If you don't want to restore GTIDs pass set-gtid-purged=OFF. To make a complete dump, pass...
 -- MySQL dump 10.13  Distrib 5.6.16, for Linux (x86_64)
 --
 -- Host: 127.0.0.1    Database: carpath
 -- ------------------------------------------------------
 -- Server version       5.6.16-log
 /*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
 /*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
问题定位到第一行出现了不正常warning的信息，是由于客户使用MySQLdump命令的时候，重定向了stderr。即:

> 
MySQLdump ...>/test.dmp 2>&1

导致error或者warning信息都重定向到了test.dmp, 最终导致失败。

**问题引申**

问题虽然定位到了，但却有几个问题没有弄清楚：

问题1. 不正常的sql，执行失败，报错出来就可以了，为什么会导致crash？

> 
MySQL.cc::add_line函数中，在读第一行的时候，读取到了don't,发现有一个单引号，所以程序死命的去找匹配的另外一个单引号，导致不断的读取文件，分配内存，直到crash。


假设没有这个单引号，MySQL读到第六行，发现;号，就会执行sql，并正常的报错退出。

问题2. 那代码中对于大小的边界到底是多少？比如insert语句支持batch insert时，语句的长度多少，又比如遇到clob字段呢？
- 首先clob字段的长度限制。clob家族类型的column长度受限于max_allowed_packet的大小，MySQL 5.5中，对于max_allowd_packet的大小限制在(1024, 1024*1024*1024)之间。
- MySQLdump导出insert语句的时候，如何分割insert语句？MySQLdump时候支持insert t1 value(),(),();这样的batch insert语句。MySQLdump其实是根据opt_net_buffer_length来进行分割，当一个insert语句超过这个大小，就强制分割到下一个insert语句中，这样更多的是在做网络层的优化。又如果遇到大的clob字段怎么办？ 如果一行就超过了opt_net_buffer_length，那就强制每一行都分割。
- MySQL client端读取dump文件的时候, 到底能分配多大的内存？MySQL.cc中定义了:#define MAX_BATCH_BUFFER_SIZE (1024L * 1024L * 1024L)。也就是MySQL在执行语句的时候，最多只能分配1G大小的缓存。

所以，正常情况下，max_allowed_packet现在的最大字段长度和MAX_BATCH_BUFFER_SIZE限制的最大insert语句，是匹配的。

**RDS问题修复原则**

从问题的定位上来看，这一例crash属于客户错误使用MySQLdump导致的问题，Aliyun RDS分支对内存导致的crash问题，都会定位并反馈给用户。 但此例不做修复，而是引导用户正确的使用MySQLdump工具。

## MySQL · 捉虫动态· 设置 gtid_purged 破坏AUTO_POSITION复制协议

**bug描述**

Oracle 最新发布的版本 5.6.22 中有这样一个关于GTID的bugfix，在主备场景下，如果我们在主库上 SET GLOBAL GTID_PURGED = "some_gtid_set"，并且 some_gtid_set 中包含了备库还没复制的事务，这个时候如果备库接上主库的话，预期结果是主库返回错误，IO线程挂掉的，但是实际上，在这种场景下主库并不报错，只是默默的把自己 binlog 中包含的gtid事务发给备库。这个bug的造成的结果是看起来复制正常，没有错误，但实际上备库已经丢事务了，主备很可能就不一致了。

**背景知识**
- binlog GTID事件

binlog 中记录的和GTID相关的事件主要有2种，Previous_gtids_log_event 和 Gtid_log_event，前者表示之前的binlog中包含的gtid的集合，后者就是一个gtid，对应一个事务。一个 binlog 文件中只有一个 Previous_gtids_log_event，放在开头，有多个 Gtid_log_event，如下面所示
Previous_gtids_log_event   // 此 binlog 之前的所有binlog文件包含的gtid集合

Gtid_log_event // 单个gtid event
Transaction
Gtid_log_event
Transaction
.
.
.
Gtid_log_event
Transaction- 备库发送GTID集合给主库

我们知道备库的复制线程是分IO线程和SQL线程2种的，IO线程通过GTID协议或者文件位置协议拉取主库的binlog，然后记录在自己的relay log中；SQL线程通过执行realy log中的事件，把其中的操作都自己做一遍，记入本地binlog。在GTID协议下，备库向主库发送拉取请求的时候，会告知主库自己已经有的所有的GTID的集合，Retrieved_Gtid_Set + Executed_Gtid_Set，前者对应 realy log 中所有的gtid集合，表示已经拉取过的，后者对应binlog中记录有的，表示已经执行过的；主库在收到这2个总集合后，会扫描自己的binlog，找到合适的binlog然后开始发送。
- 主库如何找到要发送给备库的第一个binlog

主库将备库发送过来的总合集记为 slave_gtid_executed，然后调用 find_first_log_not_in_gtid_set(slave_gtid_executed)，这个函数的目的是从最新到最老扫描binlog文件，找到第一个含有不存在 slave_gtid_executed 这个集合的gtid的binlog。在这个扫描过程中并不需要从头到尾读binlog中所有的gtid，只需要读出 Previous_gtids_log_event ，如果Previous_gtids_log_event
 不是 slave_gtid_executed的子集，就继续向前找binlog，直到找到为止。

这个查找过程总会停止的，停止条件如下：

> - 找到了这样的binlog，其Previous_gtids_log_event 是slave_gtid_executed子集
- 在往前读binlog的时候，发现没有binlog文件了（如被purge了），但是还没找到满足条件的Previous_gtids_log_event，这个时候主库报错
- 一直往前找，发现Previous_gtids_log_event 是空集


在条件2下，报错信息是这样的

Got fatal error 1236 from master when reading data from binary log: 'The slave is connecting using CHANGE MASTER TO MASTER_AUTO_POSITION = 1, but the master has purged binary logs containing GTIDs that the slave requires.

其实上面的条件3是条件1的特殊情况，这个bugfix针对的场景就是条件3这种，但并不是所有的符合条件3的场景都会触发这个bug，下面就分析下什么情况下才会触发bug。

**bug 分析**

假设有这样的场景，我们要用已经有MySQL实例的备份重新做一对主备实例，不管是用 xtrabackup 这种物理备份工具或者MySQLdump这种逻辑备份工具，都会有2步操作，

> - 导入数据
- SET GLOBAL GTID_PURGED ="xxxx"


步骤2是为了保证GTID的完备性，因为新实例已经导入了数据，就需要把生成这些数据的事务对应的GTID集合也设置进来。

正常的操作是主备都要做这2步的，如果我们只在主库上做了这2步，备库什么也不做，然后就直接用 GTID 协议把备库连上来，按照我们的预期这个时候是应该出错的，主备不一致，并且主库的binlog中没东西，应该报之前停止条件2报的错。但是令人大跌眼镜的是主库不报错，复制看起来是完全正常的。

为啥会这样呢，SET GLOBAL GTID_PURGED 操作会调用 MySQL_bin_log.rotate_and_purge切换到一个新的binlog，并把这个GTID_PURGED 集合记入新生成的binlog的Previous_gtids_log_event，假设原有的binlog为A，新生成的为B，主库刚启动，所以A就是主库的第一个binlog，它之前啥也没有，A的Previous_gtids_log_event就是空集，并且A中也不包含任何GTID事件，否则SET GLOBAL GTID_PURGED是做不了的。按照之前的扫描逻辑，扫到A是肯定会停下来的，并且不报错。

**bug 修复**

官方的修复就是在主库扫描查找binlog之前，判断一下 gtid_purged 集合不是不比slave_gtid_executed大，如果是就报错，错误信息和条件2一样 Got fatal error 1236 from master when reading data from binary log: 'The slave is connecting using CHANGE MASTER TO MASTER_AUTO_POSITION = 1, but the master has purged binary
 logs containing GTIDs that the slave requires。

## MySQL · 捉虫动态· replicate filter 和 GTID 一起使用的问题

**问题描述**

当单个 MySQL 实例的数据增长到很多的时候，就会考虑通过库或者表级别的拆分，把当前实例的数据分散到多个实例上去，假设原实例为A，想把其中的5个库（db1/db2/db3/db4/db5）拆分到5个实例（B1/B2/B3/B4/B5）上去。

拆分过程一般会这样做，先把A的相应库的数据导出，然后导入到对应的B实例上，但是在这个导出导入过程中，A库的数据还是在持续更新的，所以还需在导入完后，在所有的B实例和A实例间建立复制关系，拉取缺失的数据，在业务不繁忙的时候将业务切换到各个B实例。

在复制搭建时，每个B实例只需要复制A实例上的一个库，所以只需要重放对应库的binlog即可，这个通过 replicate-do-db 来设置过滤条件。如果我们用备库上执行 show slave status\G 会看到Executed_Gtid_Set是断断续续的，间断非常多，导致这一列很长很长，看到的直接效果就是被刷屏了。

为啥会这样呢，因为设了replicate-do-db，就只会执行对应db对应的event，其它db的都不执行。主库的执行是不分db的，对各个db的操作互相间隔，记录在binlog中，所以备库做了过滤后，就出现这种断断的现象。

除了这个看着不舒服外，还会导致其它问题么？

假设我们拿B1实例的备份做了一个新实例，然后接到A上，如果主库A又定期purge了老的binlog，那么新实例的IO线程就会出错，因为需要的binlog在主库上找不到了；即使主库没有purge 老的binlog，新实例还要把主库的binlog都从头重新拉过来，然后执行的时候又都过滤掉，不如不拉取。

有没有好的办法解决这个问题呢？SQL线程在执行的时候，发现是该被过滤掉的event，在不执行的同时，记一个空事务就好了，把原事务对应的GTID位置占住，记入binlog，这样备库的Executed_Gtid_Set就是连续的了。

**bug 修复**

对这个问题，官方有一个相应的bugfix，参见 revno:  [5860](http://bazaar.launchpad.net/~MySQL/MySQL-server/5.6/revision/5860) ，有了这个patch后，备库B1的 SQL 线程在遇到和 db2-db5 相关的SQL语句时，在binlog中把对应的GTID记下，同时对应记一个空事务。

这个 patch 只是针对Query_log_event，即 statement 格式的 binlog event，那么row格式的呢？ row格式原来就已经是这种行为，通过check_table_map 函数来过滤库或者表，然后生成一个空事务。

另外这个patch还专门处理了下 CREATE/DROP TEMPORARY TABLE 这2种语句，我们知道row格式下，对临时表的操作是不会记入binlog的。如果主库的binlog格式是 statement，备库用的是 row，CREATE/DROP TEMPORARY TABLE 对应的事务传到备库后，就会消失掉，Executed_Gtid_Set集合看起来是不连续的，但是主库的binlog记的gtid是连续的，这个 patch 让这种情况下的CREATE/DROP TEMPORARY TABLE在备库同样记为一个空事务。

## TokuDB·特性分析· Optimize Table

来自一个TokuDB用户的“投诉”:

[https://mariadb.atlassian.net/browse/MDEV-6207](https://mariadb.atlassian.net/browse/MDEV-6207)

现象大概是:

用户有一个MyISAM的表test_table:
 CREATE TABLE IF NOT EXISTS `test_table` (
   `id` int(10) unsigned NOT NULL,
   `pub_key` varchar(80) NOT NULL,
   PRIMARY KEY (`id`),
   KEY `pub_key` (`pub_key`)
 ) ENGINE=MyISAM DEFAULT CHARSET=latin1;
转成TokuDB引擎后表大小为92M左右:
 47M     _tester_testdb_sql_61e7_1812_main_ad88a6b_1_19_B_0.tokudb
 45M     _tester_testdb_sql_61e7_1812_key_pub_key_ad88a6b_1_19_B_1.tokudb
执行"OPTIMIZE TABLE test_table":
 63M     _tester_testdb_sql_61e7_1812_main_ad88a6b_1_19_B_0.tokudb
 61M     _tester_testdb_sql_61e7_1812_key_pub_key_ad88a6b_1_19_B_1.tokudb
再次执行"OPTIMIZE TABLE test_table":
 79M     _tester_testdb_sql_61e7_1812_main_ad88a6b_1_19_B_0.tokudb
 61M     _tester_testdb_sql_61e7_1812_key_pub_key_ad88a6b_1_19_B_1.tokudb
继续执行:
 79M     _tester_testdb_sql_61e7_1812_main_ad88a6b_1_19_B_0.tokudb
 61M     _tester_testdb_sql_61e7_1812_key_pub_key_ad88a6b_1_19_B_1.tokudb
基本稳定在这个大小。

主索引从47M-->63M-->79M，执行"OPTIMIZE TABLE"后为什么会越来越大？

这得从TokuDB的索引文件分配方式说起，当内存中的脏页需要写到磁盘时，TokuDB优先在文件末尾分配空间并写入，而不是“覆写”原块，原来的块暂时成了“碎片”。

这样问题就来了，索引文件岂不是越来越大？No, TokuDB会把这些“碎片”在checkpoint时加入到回收列表，以供后面的写操作使用，看似79M的文件其实还可以装不少数据呢！

嗯，这个现象解释通了，但还有2个问题:

> - 在执行这个语句的时候，TokuDB到底在做什么呢？
在做toku_ft_flush_some_child，把内节点的缓冲区(message buffer)数据刷到最底层的叶节点。
- 在TokuDB里，OPTIMIZE TABLE有用吗？
作用非常小，不建议使用，TokuDB是一个"No Fragmentation"的引擎。


本文转载自MySQL.taobao.org ，感谢淘宝数据库项目组丁奇、鸣嵩、彭立勋、皓庭、项仲、剑川、武藏、祁奚、褚霸、一工。审校：刘亚琼




