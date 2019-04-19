# MySQL 死锁与日志二三事 - 文章 - 伯乐在线
原文出处： [xrzs](https://my.oschina.net/leejun2005/blog/1484511)
最近线上 MySQL 接连发生了几起数据异常，都是在凌晨爆发，由于业务场景属于典型的数据仓库型应用，白天压力较小无法复现。甚至有些异常还比较诡异，最后 root cause 分析颇费周折。那实际业务当中咱们如何能快速的定位线上 MySQL 问题，修复异常呢？下文我会根据两个实际 case，分享下相关的经验与方法。
### 1、Case1：部分数据更新失败
某天渠道同学反馈某报表极个别渠道数据为 0，大部分渠道数据正常。这个数据是由一个统计程序每天凌晨例行更新的，按理来说，要么全部正常，要么全部失败，那会是什么原因导致极个别数据异常呢？
首先我们能想到的自然是根据统计任务日志来看了，但是看了统计程序打印的日志没有发现诸如 SQL update 失败的异常描述，那当时的数据库究竟发生了什么呢？在查看 MySQL-server 日志之前，习惯性的看了下数据库状态：


```
SHOW ENGINE INNODB STATUS\G
```
恰好看到了凌晨这个 update 发生了死锁：


```
------------------------
LATEST DETECTED DEADLOCK
------------------------
2017-07-17 04:09:01 0x7f6de03c8700
*** (1) TRANSACTION:
TRANSACTION 215208479, ACTIVE 0 sec fetching rows
mysql tables in use 3, locked 3
LOCK WAIT 5 lock struct(s), heap size 1136, 3 row lock(s)
MySQL thread id 27844824, OS thread handle 140092183037696, query id 412503674 10.126.95.84 zeye Searching rows for update
update t_channel_final_datas set nr_register=133,nr_add_goods=29,nr_order_normal=11,nr_pay_normal=8,nr_order_special=0,nr_pay_special=0,n_add_user_num=16 where count_date='2017-07-16' and channel_id='16' and channel_type='10' and terminal='26'
*** (1) WAITING FOR THIS LOCK TO BE GRANTED:
RECORD LOCKS space id 464 page no 5459 n bits 392 index index_countdate_type_terminal of table `db_zz_flow`.`t_channel_final_datas` trx id 215208479 lock_mode X locks rec but not gap waiting
Record lock, heap no 304 PHYSICAL RECORD: n_fields 4; compact format; info bits 0
 0: len 10; hex 323031372d30372d3136; asc 2017-07-16;;
 1: len 1; hex 30; asc 0;;
 2: len 4; hex 80000010; asc     ;;
 3: len 4; hex 8009055e; asc    ^;;
*** (2) TRANSACTION:
TRANSACTION 215208474, ACTIVE 0 sec fetching rows
mysql tables in use 3, locked 3
6 lock struct(s), heap size 1136, 7 row lock(s)
MySQL thread id 27844825, OS thread handle 140109890225920, query id 412503669 10.135.6.41 zeye Searching rows for update
update t_channel_final_datas set nr_register=24,nr_add_goods=32,nr_order_normal=0,nr_pay_normal=0,nr_order_special=0,nr_pay_special=0,n_add_user_num=11 where count_date='2017-07-16' and channel_id='114' and channel_type='10' and terminal='116'
*** (2) HOLDS THE LOCK(S):
RECORD LOCKS space id 464 page no 5459 n bits 392 index index_countdate_type_terminal of table `db_zz_flow`.`t_channel_final_datas` trx id 215208474 lock_mode X locks rec but not gap
Record lock, heap no 304 PHYSICAL RECORD: n_fields 4; compact format; info bits 0
 0: len 10; hex 323031372d30372d3136; asc 2017-07-16;;
 1: len 1; hex 30; asc 0;;
 2: len 4; hex 80000010; asc     ;;
 3: len 4; hex 8009055e; asc    ^;;
...
*** (2) WAITING FOR THIS LOCK TO BE GRANTED:
RECORD LOCKS space id 464 page no 4743 n bits 264 index PRIMARY of table `db_zz_flow`.`t_channel_final_datas` trx id 215208474 lock_mode X locks rec but not gap waiting
Record lock, heap no 168 PHYSICAL RECORD: n_fields 32; compact format; info bits 0
 0: len 4; hex 80090569; asc    i;;
 1: len 6; hex 00000cd3b9d0; asc       ;;
...
*** WE ROLL BACK TRANSACTION (1)
```
篇幅所限，上下文我这里省略了很多，从这段日志里可以看到，TRANSACTION 1 和 TRANSACTION 2 分别持有一定数量的行锁，然后又等待对方的锁，最后 MySQL 检测到 deadlock ，然后选择回滚了 TRANSACTION 1：Innodb目前处理死锁的方法是将持有最少行级排他锁的事务进行回滚。
那这里就有 3 个问题了：
#### （1）innodb 行锁不是只锁一行？
因为这张表是 innodb 引擎的，InnoDB 支持行锁和表锁。而InnoDB行锁是通过给索引上的索引项加锁来实现的，这一点MySQL与Oracle不同，后者是通过在数据块中对相应数据行加锁来实现的。InnoDB这种行锁实现特点意味着：只有通过索引条件检索数据，InnoDB才使用行级锁，否则，InnoDB将使用表锁，会把所有扫描过的行都锁定！在实际应用中，要特别注意InnoDB行锁的这一特性，不然的话，可能导致大量的锁冲突，从而影响并发性能。由于MySQL的行锁是针对索引加的锁，不是针对记录加的锁，所以虽然是访问不同行的记录，但是如果是使用相同的索引键，是会出现锁冲突的。当我们用范围条件而不是相等条件检索数据，并请求共享或排他锁时，InnoDB会给符合条件的已有数据记录的索引项加锁；另外间隙锁也会锁多行，InnoDB除了通过范围条件加锁时使用间隙锁外，如果使用相等条件请求给一个不存在的记录加锁，InnoDB也会使用间隙锁！
话都说到这了，那就看下咱们业务表的索引情况：


```
show INDEX from `db_zz_flow`.`t_channel_final_datas`;
+-----------------------+--------------+-------------------------------+----------------+-------------+----------+--------+--------------+-----------+-----------------+
| Table | Non_unique | Key_name | Seq_in_index | Column_namt | Packed | Null | Index_type | Comment | Index_comment |
|-----------------------+--------------+-------------------------------+----------------+-------------+----------+--------+--------------+-----------+-----------------|
| t_channel_final_datas | 0 | PRIMARY | 1 | id > | <null> | | BTREE | | |
| t_channel_final_datas | 1 | index_countdate_type_terminal | 1 | count_date> | <null> | YES | BTREE | | |
| t_channel_final_datas | 1 | index_countdate_type_terminal | 2 | channel_ty> | <null> | YES | BTREE | | |
| t_channel_final_datas | 1 | index_countdate_type_terminal | 3 | terminal > | <null> | YES | BTREE | | |
| t_channel_final_datas | 1 | index_countdate_channelid | 1 | count_date> | <null> | YES | BTREE | | |
| t_channel_final_datas | 1 | index_countdate_channelid | 2 | channel_id> | <null> | YES | BTREE | | |
+-----------------------+--------------+-------------------------------+----------------+-------------+----------+--------+--------------+-----------+-----------------+
```
可以看到这张表的索引极不合理：有3个索引，但是 update 却没有完全的用上索引，导致 update 没有精确的用上索引，需要锁定多行范围数据，从而引发死锁。
知道原理后，咱们再精心构建一个四字段的组合索引即可让 update 精准的走 innodb 索引，实际上，我们更新索引后，这个死锁问题即得到了解决。
注：innodb不仅会打印出事务和事务持有和等待的锁，而且还有记录本身，不幸的是，它可能超过innodb为输出结果预留的长度（只能打印1M的内容且只能保留最近一次的死锁信息），如果你无法看到完整的输出，此时可以在任意库下创建innodb_monitor或innodb_lock_monitor表，这样innodb status信息会完整且每15s一次被记录到错误日志中。如：create table innodb_monitor(a int)engine=innodb;，不需要记录到错误日志中时就删掉这个表即可。
#### （2）回滚的话，为什么只有部分 update 语句失败，而不是整个事务里的所有 update 都失败？
这是因为咱们的 innodb 默认是自动提交的：


```
show variables like 'autocommit';
+-----------------+---------+
| Variable_name   | Value   |
|-----------------+---------|
| autocommit      | ON      |
+-----------------+---------+
```
在多个 update 或 insert 语句情况下，每执行完一条 SQL，innodb 就立即 commit 一次以持久化变更，同时释放锁，这也正是本例中死锁回滚事务后只有极个别语句失败的原因。
需要注意的是，通常还有另外一种情况也可能导致部分语句回滚，需要格外留意。在 innodb 里有个参数叫：innodb_rollback_on_timeout


```
show VARIABLES LIKE 'innodb_rollback_on_timeout'
+----------------------------+---------+
| Variable_name              | Value   |
|----------------------------+---------|
| innodb_rollback_on_timeout | OFF     |
+----------------------------+---------+
```
官方手册里这样描述：
> 
In MySQL 5.1, InnoDB rolls back only the last statement on a transaction timeout by default. If –innodb_rollback_on_timeout is specified, a transaction timeout causes InnoDB to abort and roll back the entire transaction (the same behavior as in MySQL 4.1). This variable was added in MySQL 5.1.15.
解释：这个参数关闭或不存在的话遇到超时只回滚事务最后一个Query，打开的话事务遇到超时就回滚整个事务。
### （3）怎样降低 innodb 死锁几率？
死锁在行锁及事务场景下很难完全消除，但可以通过表设计和SQL调整等措施减少锁冲突和死锁，包括：
- 尽量使用较低的隔离级别，比如如果发生了间隙锁，你可以把会话或者事务的事务隔离级别更改为 RC(read committed)级别来避免，但此时需要把 binlog_format 设置成 row 或者 mixed 格式
- 精心设计索引，并尽量使用索引访问数据，使加锁更精确，从而减少锁冲突的机会；
- 选择合理的事务大小，小事务发生锁冲突的几率也更小；
- 给记录集显示加锁时，最好一次性请求足够级别的锁。比如要修改数据的话，最好直接申请排他锁，而不是先申请共享锁，修改时再请求排他锁，这样容易产生死锁；
- 不同的程序访问一组表时，应尽量约定以相同的顺序访问各表，对一个表而言，尽可能以固定的顺序存取表中的行。这样可以大大减少死锁的机会；
- 尽量用相等条件访问数据，这样可以避免间隙锁对并发插入的影响；
- 不要申请超过实际需要的锁级别；除非必须，查询时不要显示加锁；
- 对于一些特定的事务，可以使用表锁来提高处理速度或减少死锁的可能。
### 2、Case2：诡异的 Lock wait timeout
连续几天凌晨6点和早上8点 都分别有一个任务失败，load data local infile 的时候报 Lock wait timeout exceeded try restarting transaction innodb 的 Java SQL 异常，和平台的同学沟通得知，这是我们自己的业务数据库的 Lock 时间太短或者锁冲突的问题。但是回头一想不应该啊？这不一直好好的吗？而且基本都是单表单任务，不存在多人冲突。
甭管谁的问题，那咱们还是先看自己的数据库有没有问题：


```
show variables like 'innodb_lock_wait_timeout';
+--------------------------+---------+
| Variable_name            |   Value |
|--------------------------+---------|
| innodb_lock_wait_timeout |      50 |
+--------------------------+---------+
```
默认 lock 超时时间 50s，这个时间真心不短了，估计调了也没用，事实上确实死马当活马医的试了下没用。。。
而且这次 SHOW ENGINE INNODB STATUSG 也没出现任何死锁信息，然后又将目光转向 MySQL-server 日志，希望能从日志里看一看那个时刻前后数据究竟在做什么操作。这里先简单的介绍下MySQL日志文件系统的组成：
(a) error 日志：记录启动、运行或停止 mysqld 时出现的问题，默认开启。
(b) general 日志：通用查询日志，记录所有语句和指令，开启数据库会有 5% 左右性能损失。
(c) binlog 日志：二进制格式，记录所有更改数据的语句，主要用于 slave 复制和数据恢复。
(d) slow 日志：记录所有执行时间超过 long_query_time 秒的查询或不使用索引的查询，默认关闭。
(e) Innodb日志：innodb redo log、undo log，用于恢复数据和撤销操作。
从上面的介绍可以看到，目前这个问题的日志可能在 d 和 b 中，看了下 d 中没有，那就只能开启 b 了，但 b 对数据库的性能有一定损耗，由于是全量日志，量非常巨大，所以开启一定要谨慎：


```
-- general_log 日志默认关闭，开启会影响数据库 5% 左右性能：
show variables like 'general%';
+------------------+---------------------------------+
| Variable_name    | Value                           |
|------------------+---------------------------------|
| general_log      | OFF                             |
| general_log_file | /opt/data/mysql/tjtx-103-26.log |
+------------------+---------------------------------+
-- 全局 session 级别开启：
set global general_log=1
-- 如果需要对当前 session 生效需要：
set general_log=1
-- set 指令设置的动态参数在 MySQL 重启后失效，如果需要永久生效需要在 /etc/my.cnf 中配置静态变量/参数。
-- 如果不知道 my.cnf 位置，可以根据 mysql -? | grep ".cnf" 查询
                      order of preference, my.cnf, $MYSQL_TCP_PORT,
/etc/my.cnf /etc/mysql/my.cnf /usr/etc/my.cnf ~/.my.cnf
```
我这里只是每天在出问题的前后半小时开启下全量日志，结果没有发现任何 MySQL-client 请求到我们的业务数据库！该日志格式如下，记录了所有的连接与命令：


```
/usr/sbin/mysqld, Version: 5.7.12-log (MySQL Community Server (GPL)). started with:
Tcp port: 3306  Unix socket: /opt/data/mysql/mysql.sock
Time                 Id Command    Argument
2017-07-20T21:45:01.880828Z28556028 Quit
2017-07-20T21:45:02.708621Z28401469 Query       SELECT 1
2017-07-20T21:45:02.736734Z28556029 Connect     ooxx@127.0.0.1 on db_zz_system using TCP/IP
2017-07-20T21:45:02.737242Z28556029 Query       /* mysql-connector-java-5.1.6 ( Revision: ${svn.Revision} ) */SHOW VARIABLES WHERE Variable_name ='language' OR Variable_name = 'net_write_timeout' OR Variable_name = 'interactive_timeout' OR Variable_name = 'wait_timeout' OR Variable_name = 'character_set_client' OR Variable_name = 'character_set_connection' OR Variable_name = 'character_set' OR Variable_name = 'character_set_server' OR Variable_name = 'tx_isolation' OR Variable_name = 'transaction_isolation' OR Variable_name = 'character_set_results' OR Variable_name = 'timezone' OR Variable_name = 'time_zone' OR Variable_name = 'system_time_zone' OR Variable_name = 'lower_case_table_names' OR Variable_name = 'max_allowed_packet' OR Variable_name = 'net_buffer_length' OR Variable_name = 'sql_mode' OR Variable_name = 'query_cache_type' OR Variable_name = 'query_cache_size' OR Variable_name = 'init_connect'
2017-07-20T21:45:02.738868Z28556029 Query       SHOW COLLATION
2017-07-20T21:45:02.739941Z28556029 Query       SET character_set_results = NULL
2017-07-20T21:45:02.740275Z28556029 Query       SET autocommit=1
```
那问题基本确定了，客户端请求都没到我们这边就抛出了上述的异常，和平台方再三沟通确认下，最后平台方查证是因为在执行插入前他们需要先从 SQL task 表取出 SQL 和更新 task 状态，结果这张表由于在整点存在大量 insert 和 update 并发，导致部分 SQL 等待 lock 超时了。。。
![](http://jbcdn2.b0.upaiyun.com/2017/07/d29579706b846656c946e30891c2c075.jpg)
### 3、MySQL 日志分析脚本
由于凌晨是数据仓库的业务高峰，很多问题都是在这个时候爆发，一些诡异的问题往往是过了这个村就没这个店了，白天无法复现。如何能捕获我们关心的日志，便于快速的定位问题，这个是重中之重，这里我写了个小脚本，crontab 部署，可以选择时间范围开启，每分钟采样一次日志，需要说明的是 general log 没事别轻易开启，否则对数据库性能损耗较大。


```
# crontab 部署方式：
# */1 0-10 * * * cd /opt/ooxx/script && bash mysql_perf.sh >> logs/mysql_perf.log.`date -I` 2>&1
date -Iseconds
echo '>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> top -bn1|head'
top -bn1|head 
echo
echo '>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> SHOW ENGINE INNODB STATUS\G '
mysql -uroot -pooxx -h127.0.0.1 -e 'SHOW ENGINE INNODB STATUS\G'
echo
echo '>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> show open tables where in_use>0;'
mysql -uroot -pooxx -h127.0.0.1 -e 'show open tables where in_use>0;'
echo
echo '>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> show full processlist;'
mysql -uroot -pooxx -h127.0.0.1 -e 'show full processlist;'
echo
echo '>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> SELECT * FROM `information_schema`.`innodb_trx` ORDER BY `trx_started`; '
mysql -uroot -pooxx -h127.0.0.1 -e 'SELECT * FROM `information_schema`.`innodb_trx` ORDER BY `trx_started`;' 
echo
echo '>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> SELECT * FROM `information_schema`.`innodb_locks`;'
mysql -uroot -pooxx -h127.0.0.1 -e 'SELECT * FROM `information_schema`.`innodb_locks`;'
echo
echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> show status like '%lock%';"
mysql -uroot -pooxx -h127.0.0.1 -e "show status like '%lock%';"
echo
echo '>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> show global status like "table_locks%";'
mysql -uroot -pooxx -h127.0.0.1 -e 'show global status like "table_locks%";'
echo
echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> too long omit..."
mysql -uroot -pooxx -h127.0.0.1 -e "select r.trx_isolation_level, r.trx_id waiting_trx_id, r.trx_mysql_thread_id waiting_trx_thread, r.trx_state waiting_trx_state, lr.lock_mode waiting_trx_lock_mode, lr.lock_type waiting_trx_lock_type, lr.lock_table waiting_trx_lock_table, lr.lock_index waiting_trx_lock_index, r.trx_query waiting_trx_query, b.trx_id blocking_trx_id, b.trx_mysql_thread_id blocking_trx_thread, b.trx_state blocking_trx_state, lb.lock_mode blocking_trx_lock_mode, lb.lock_type blocking_trx_lock_type, lb.lock_table blocking_trx_lock_table, lb.lock_index blocking_trx_lock_index, b.trx_query blocking_query from information_schema.innodb_lock_waits w inner join information_schema.innodb_trx b on b.trx_id=w.blocking_trx_id inner join information_schema.innodb_trx r on r.trx_id=w.requesting_trx_id inner join information_schema.innodb_locks lb on lb.lock_trx_id=w.blocking_trx_id inner join information_schema.innodb_locks lr on lr.lock_trx_id=w.requesting_trx_id\G"
echo
echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> too long omit..."
mysql -uroot -pooxx -h127.0.0.1 -e "SELECT r.trx_id waiting_trx_id, r.trx_mysql_thread_id waiting_thread, r.trx_query waiting_query, b.trx_id blocking_trx_id, b.trx_mysql_thread_id blocking_thread, b.trx_query blocking_query FROM information_schema.innodb_lock_waits w INNER JOIN information_schema.innodb_trx b ON b.trx_id = w.blocking_trx_id INNER JOIN information_schema.innodb_trx r ON r.trx_id = w.requesting_trx_id\G"
echo
date -Iseconds
echo '>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>=================================================================================='
echo
file_name=mysql_perf.log.`date -I`
if [[ `date +%-H%-M` == 1059 ]]
then
 cd /opt/ooxx/script/logs
 chmod 777 /home/work/ooxx/$file_name
 find /home/work/ooxx -name 'mysql_perf.log.*' -mtime +7 -delete
 
 cd /opt/data/mysql
 cp tjtx-ooxx-slow.log /home/work/ooxx/tjtx-ooxx-slow.log.`date -I`
 chmod 777 /home/work/ooxx/tjtx-ooxx-slow.log.`date -I`
 find /home/work/ooxx -name 'tjtx-ooxx-slow.log.*' -mtime +7 -delete
 
 cp mysqld.log /home/work/ooxx/mysqld.log.`date -I`
 chmod 777 /home/work/ooxx/mysqld.log.`date -I`
 find /home/work/ooxx -name 'mysqld.log.*' -mtime +7 -delete
fi
################
# 开启 general_log 全量明细日志会降低数据库 5% 性能
#if [[ "`date +%H%M`" == "0545" ]]
#then
# echo "`date +%H%M` ------- set global general_log=1;"
# mysql -uroot -pooxx -h127.0.0.1 -e 'set global general_log=1;'
#elif [[ "`date +%H%M`" == "0630" ]]
#then
# echo "`date +%H%M` ------- set global general_log=0;"
# mysql -uroot -pooxx -h127.0.0.1 -e 'set global general_log=0;'
#elif [[ "`date +%H%M`" == "0745" ]]
#then
# echo "`date +%H%M` ------- set global general_log=1;"
# mysql -uroot -pooxx -h127.0.0.1 -e 'set global general_log=1;'
#elif [[ "`date +%H%M`" == "0830" ]]
#then
# echo "`date +%H%M` ------- set global general_log=0;"
# mysql -uroot -pooxx -h127.0.0.1 -e 'set global general_log=0;'
#elif [[ "`date +%H%M`" == "0001" ]]
#then
# echo "`date +%H%M` ------- set global general_log=1;"
# mysql -uroot -pooxx -h127.0.0.1 -e 'set global general_log=1;'
#elif [[ "`date +%H%M`" == "0002" ]]
#then
# echo "`date +%H%M` ------- set global general_log=0;"
# mysql -uroot -pooxx -h127.0.0.1 -e 'set global general_log=0;'
#fi
#[[ 10#`date +%H%M` -lt 10#0550 || 10#`date +%H%M` -gt 10#0830 ]] && echo "`date +%H%M` ------- set global general_log=0;" && mysql -uroot -pooxx -h127.0.0.1 -e 'set global general_log=0;'
# mysql -uroot -pooxx -h127.0.0.1 -e 'show open tables where in_use>0;show full processlist;SELECT * FROM `information_schema`.`innodb_trx` ORDER BY `trx_started`;SELECT * FROM `information_schema`.`innodb_locks`;SHOW ENGINE INNODB STATUS\G'
# --show variables like '%tx_isolation%';
# --SELECT @@GLOBAL.tx_isolation, @@tx_isolation, @@session.tx_isolation;
# -- SET GLOBAL tx_isolation = 'READ-COMMITTED';
# 
# --show variables like '%timeout%';
# --show variables like 'innodb_lock_wait_timeout';
# -- SET GLOBAL innodb_lock_wait_timeout=60
# 
# --show variables like 'long_query_time';
# -- SET global long_query_time=3;
# -- show variables like 'innodb_rollback_on_timeout';
# -- show VARIABLES like '%max_allowed_packet%';
# -- set global max_allowed_packet = 100*1024*1024;
# 自动提交
# -- show variables like 'autocommit';
# 慢查询
# -- show variables like '%slow_query_log%';
# set global 只对当前数据库生效，如果MySQL重启后则会失效。如果要永久生效，就必须修改配置文件my.cnf（其它系统变量也是如此）。
# set global slow_query_log=1;
# -- show variables like 'long_query_time%';
# set global long_query_time=4;
# show global variables like 'long_query_time'; 
# select sleep(5);
# -- show variables like 'log_queries_not_using_indexes';
# set global log_queries_not_using_indexes=1;
# -- show variables like 'log_slow_admin_statements';
# -- show global status like '%Slow_queries%';
# http://www.cnblogs.com/kerrycode/p/5593204.html
# -- show variables like "%time_zone%";
#set global time_zone = '+8:00';
#开启general_log日志
# -- show variables like 'general%';
#可以在my.cnf里添加,1开启（0关闭）,当然了,这样要重启才能生效,有点多余了
#general-log = 1
#log = /log/mysql_query.log路径
#也可以设置变量那样更改,1开启（0关闭）,即时生效,不用重启,首选当然是这样的了
# set global general_log=1
#这个日志对于操作频繁的库,产生的数据量会很快增长,出于对硬盘的保护,可以设置其他存放路径
#set global general_log_file=/tmp/general_log.log
#mysql记录客户端IP：init_connect，有super权限的用户是不记录的，
# create table t1 ( cur_user varchar(100), n_user varchar(100),in_time timestamp default current_timestamp()) ;
# set global init_connect='insert into test.t1 (cur_user,n_user) values (current_user(),user())'; 
# SHOW CREATE TABLE mysql.general_log\G ，开启general_log日志也行：
# https://dba.stackexchange.com/questions/33654/mysql-logging-activity-from-specific-user-or-ip 
#SELECT REVERSE(SUBSTRING_INDEX(REVERSE(USER()),'@',1)) as ip;
#SELECT SUBSTRING(USER(), LOCATE('@', USER())+1) as ip;
#select SUBSTRING_INDEX(host,':',1) as 'ip' from information_schema.processlist WHERE ID=connection_id();
```
### Refer：
[1] MySQL批量SQL插入性能优化
[http://database.51cto.com/art/201309/411050.htm](http://database.51cto.com/art/201309/411050.htm)
[2] MySQL 四种事务隔离级的说明
[http://www.cnblogs.com/zhoujinyi/p/3437475.html](http://www.cnblogs.com/zhoujinyi/p/3437475.html)
[3] innodb_rollback_on_timeout参数对锁的影响
[http://q.fireflyclub.org/?/article/37](http://q.fireflyclub.org/?/article/37)
[4] 日均万条数据丢失，一个隐式骚操作导致的奇葩事故！
[https://mp.weixin.qq.com/s/OpPV24ILewVc2lRDrV4vvA](https://mp.weixin.qq.com/s/OpPV24ILewVc2lRDrV4vvA)
[5] mysql之show engine innodb status解读（转）
[http://www.cnblogs.com/ajianbeyourself/p/6941905.html](http://www.cnblogs.com/ajianbeyourself/p/6941905.html)
[http://www.cnblogs.com/olinux/p/5497176.html](http://www.cnblogs.com/olinux/p/5497176.html)
[6] Mysql 锁机制笔记
[http://www.jianshu.com/p/fa28035656a9](http://www.jianshu.com/p/fa28035656a9)
[http://www.jianshu.com/p/ec4a343c49f0](http://www.jianshu.com/p/ec4a343c49f0)
[http://hudeyong926.iteye.com/blog/1490687](http://hudeyong926.iteye.com/blog/1490687)
[7] MySQL索引原理及慢查询优化
[https://tech.meituan.com/mysql-index.html](https://tech.meituan.com/mysql-index.html)
[8] [译文]MySQL发生死锁肿么办？
[http://www.10tiao.com/html/672/201612/2653929751/1.html](http://www.10tiao.com/html/672/201612/2653929751/1.html)
[9] MySQL的timeout那点事
[http://www.penglixun.com/tech/database/mysql_timeout.html](http://www.penglixun.com/tech/database/mysql_timeout.html)
[10] mysql死锁(deadlock)分析及解决 （索引 间隙锁 sql顺序）
[http://www.youyong.top/article/1158d0e3bcf](http://www.youyong.top/article/1158d0e3bcf)
