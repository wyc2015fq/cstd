# 淘宝内部分享：MySQL & MariaDB性能优化 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月06日 21:25:44[boonya](https://me.csdn.net/boonya)阅读数：2259








 原文地址：[http://www.csdn.net/article/2015-01-20/2823634](http://www.csdn.net/article/2015-01-20/2823634)


##    MySQL是目前使用比较多的开源数据库，但是MySQL数据库的默认设置性能非常的差，必须进行不断的优化，而优化是一个复杂的任务，本文描述淘宝数据库团队针对MySQL数据库Metadata Lock子系统的优化，hash_scan 算法的实现解析的性能优化，TokuDB·版本优化，以及MariaDB·的性能优化。本文来自淘宝团队内部经验分享。



## MySQL·　5.7优化·Metadata Lock子系统的优化


背景

引入MDL锁的目的，最初是为了解决著名的bug#989，在MySQL 5.1及之前的版本，事务执行过程中并不维护涉及到的所有表的Metatdata 锁，极易出现复制中断，例如如下执行序列：

> 
Session 1: BEGIN;

Session 1: INSERT INTO t1 VALUES (1);

Session 2: Drop table t1; --------SQL写入BINLOG

Session 1: COMMIT; -----事务写入BINLOG

在备库重放 binlog时，会先执行DROP TABLE，再INSERT数据，从而导致复制中断。


在MySQL 5.5版本里，引入了MDL, 在事务过程中涉及到的所有表的MDL锁，直到事务结束才释放。这意味着上述序列的DROP TABLE 操作将被Session 1阻塞住直到其提交。


不过用过5.5的人都知道，MDL实在是个让人讨厌的东西，相信不少人肯定遇到过在使用mysqldump做逻辑备份时，由于需要执行FLUSH TABLES WITH READ LOCK (以下用FTWRL缩写代替)来获取全局GLOBAL的MDL锁，因此经常可以看到“wait for global read lock”之类的信息。如果备库存在大查询，或者复制线程正在执行比较漫长的DDL，并且FTWRL被block住，那么随后的QUERY都会被block住，导致业务不可用引发故障。


为了解决这个问题，Facebook为MySQL增加新的接口替换掉FTWRL 只创建一个read view ，并返回与read view一致的binlog位点；另外Percona Server也实现了一种类似的办法来绕过FTWRL，具体点击[文档连接](http://www.percona.com/doc/percona-server/5.6/management/backup_locks.html)以及[percona的博客](http://www.mysqlperformanceblog.com/2014/03/11/introducing-backup-locks-percona-server-2/)，不展开阐述。


MDL解决了[bug#989](http://bugs.mysql.com/bug.php?id=989)，却引入了一个新的热点，所有的MDL锁对象被维护在一个hash对象中；对于热点，最正常的想法当然是对其进行分区来分散热点，不过这也是Facebook的大神Mark Callaghan在report了[bug#66473](http://bugs.mysql.com/bug.php?id=66473)后才加入的，当时Mark观察到MDL_map::mutex的锁竞争非常高，进而推动官方改变。因此在MySQL
 5.6.8及之后的版本中，引入了新参数metadata_locks_hash_instances来控制对mdl hash的分区数([Rev:4350](http://bazaar.launchpad.net/~mysql/mysql-server/5.6/revision/4350))；

不过故事还没结束，后面的测试又发现哈希函数有问题，somedb. someprefix1 … .somedb .someprefix8 的hash key值相同，都被hash到同一个桶下面了，相当于hash分区没生效。这属于hash算法的问题，喜欢考古的同学可以阅读下bug#66473后面Dmitry Lenev的分析。


Mark进一步的测试发现Innodb的hash计算算法比my_hash_sort_bin要更高效， Oracle的开发人员重开了个[bug#68487](http://bugs.mysql.com/bug.php?id=68487)来跟踪该问题，并在MySQL5.6.15对hash key计算函数进行优化，包括fix 上面说的hash计算问题([Rev:5459](http://bazaar.launchpad.net/~mysql/mysql-server/5.6/revision/5459))，使用MurmurHash3算法来计算mdl
 key的hash值。

**MySQL 5.7 对MDL锁的优化**

在MySQL 5.7里对MDL子系统做了更为彻底的优化。主要从以下几点出发：

**第一，尽管对MDL HASH进行了分区，但由于是以表名+库名的方式作为key值进行分区，如果查询或者DML都集中在同一张表上，就会hash到相同的分区，引起明显的MDL HASH上的锁竞争。**

针对这一点，引入了LOCK-FREE的HASH来存储MDL_lock，LF_HASH无锁算法基于论文"Split-Ordered Lists: Lock-Free Extensible Hash Tables"，实现还比较复杂。 注：实际上LF_HASH很早就被应用于Performance Schema，算是比较成熟的代码模块。由于引入了LF_HASH，MDL HASH分区特性自然直接被废除了 。对应[WL#7305](http://dev.mysql.com/worklog/task/?id=7305)，
 PATCH([Rev:7249](http://bazaar.launchpad.net/~mysql/mysql-server/5.7/revision/7249))

**第二，从广泛使用的实际场景来看，DML/SELECT相比DDL等高级别MDL锁类型，是更为普遍的，因此可以针对性的降低DML和SELECT操作的MDL开销。**

为了实现对DML/SELECT的快速加锁，使用了类似LOCK-WORD的加锁方式，称之为FAST-PATH，如果FAST-PATH加锁失败，则走SLOW-PATH来进行加锁。

每个MDL锁对象（MDL_lock）都维持了一个long long类型的状态值来标示当前的加锁状态，变量名为MDL_lock::m_fast_path_state 举个简单的例子：（初始在sbtest1表上对应MDL_lock::m_fast_path_state值为0）

> 
Session 1: BEGIN;

Session 1: SELECT * FROM sbtest1 WHERE id =1; //m_fast_path_state = 1048576, MDL ticket 不加MDL_lock::m_granted队列

Session 2: BEGIN;

Session 2: SELECT * FROM sbtest1 WHERE id =2; //m_fast_path_state=1048576+1048576=2097152，同上，走FAST PATH

Session 3: ALTER TABLE sbtest1 ENGINE = INNODB; //DDL请求加的MDL_SHARED_UPGRADABLE类型锁被视为unobtrusive lock，可以认为这个是比上述SQL的MDL锁级别更高的锁，并且不相容，因此被强制走slow path。而slow path是需要加MDL_lock::m_rwlock的写锁。m_fast_path_state = m_fast_path_state | MDL_lock::HAS_SLOW_PATH | MDL_lock::HAS_OBTRUSIVE

注:DDL还会获得库级别的意向排他MDL锁或者表级别的共享可升级锁，但为了表述方便，这里直接忽略了，只考虑涉及的同一个MDL_lock锁对象。

Session 4: SELECT * FROM sbtest1 WHERE id =3; // 检查m_fast_path_state &HAS_OBTRUSIVE，如果DDL还没跑完，就会走slow path。

从上面的描述可以看出，MDL子系统显式的对锁类型进行了区分（OBTRUSIVE or UNOBTRUSIVE），存储在数组矩阵m_unobtrusive_lock_increment。 因此对于相容类型的MDL锁类型，例如DML/SELECT，加锁操作几乎没有任何读写锁或MUTEX开销。对应[WL#7304](http://dev.mysql.com/worklog/task/?id=7304), [WL#7306](http://dev.mysql.com/worklog/task/?id=7306) ，
 PATCH（[Rev:7067](http://bazaar.launchpad.net/~mysql/mysql-server/5.7/revision/7067),[Rev:7129](http://bazaar.launchpad.net/~mysql/mysql-server/5.7/revision/7129)）([Rev:7586](http://bazaar.launchpad.net/~mysql/mysql-server/5.7/revision/7586))

**第三，由于引入了MDL锁，实际上早期版本用于控制Server和引擎层表级并发的THR_LOCK 对于Innodb而言已经有些冗余了，因此Innodb表完全可以忽略这部分的开销。**

不过在已有的逻辑中，Innodb依然依赖THR_LOCK来实现LOCK TABLE tbname READ，因此增加了新的MDL锁类型来代替这种实现。实际上代码的大部分修改都是为了处理新的MDL类型，Innodb的改动只有几行代码。对应[WL#6671](http://dev.mysql.com/worklog/task/?id=6671)，PATCH([Rev:8232](http://bazaar.launchpad.net/~mysql/mysql-server/5.7/revision/8232))

**第四，Server层的用户锁（通过GET_LOCK函数获取）使用MDL来重新实现。**

用户可以通过GET_LOCK()来同时获取多个用户锁，同时由于使用MDL来实现，可以借助MDL子系统实现死锁的检测。注意由于该变化，导致用户锁的命名必须小于64字节，这是受MDL子系统的限制导致。对应[WL#1159](http://dev.mysql.com/worklog/task/?id=1159),
 PATCH([Rev:8356](http://bazaar.launchpad.net/~mysql/mysql-server/5.7/revision/8356))


## MySQL·性能优化·hash_scan 算法的实现解析



**问题描述**

首先，我们执行下面的TestCase：



**[js]**[view plain](http://www.csdn.net/article/2015-01-20/2823634#)[copy](http://www.csdn.net/article/2015-01-20/2823634#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- --source include/master-slave.inc  
- --source include/have_binlog_format_row.inc  
- connection slave;  
- set global slave_rows_search_algorithms='TABLE_SCAN';  
- connection master;  
- create table t1(id int, name varchar(20);  
- insert into t1 values(1,'a');  
- insert into t2 values(2, 'b');  
- ......  
- insert into t3 values(1000, 'xxx');  
- delete from t1;  
- ---source include/rpl_end.inc  


随着 t1 数据量的增大，rpl_hash_scan.test 的执行时间会随着 t1 数据量的增大而快速的增长，因为在执行 'delete from t1;' 对于t1的每一行删除操作，备库都要扫描t1,即全表扫描，如果 select count(*) from t1 = N, 则需要扫描Ｎ次 t1 表， 则读取记录数为： O(N + (N-1) + (N-2) + .... + 1) = O(N^2)，在 replication 没有引入 hash_scan，binlog_format=row时，对于无索引表，是通过
 table_scan 实现的，如果一个update_rows_log_event/delete_rows_log_event 包含多行修改时，每个修改都要进行全表扫描来实现，其 stack 如下：



**[js]**[view plain](http://www.csdn.net/article/2015-01-20/2823634#)[copy](http://www.csdn.net/article/2015-01-20/2823634#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- #0 Rows_log_event::do_table_scan_and_update
- #1 0x0000000000a3d7f7 in Rows_log_event::do_apply_event 
- #2 0x0000000000a28e3a in Log_event::apply_event
- #3 0x0000000000a8365f in apply_event_and_update_pos
- #4 0x0000000000a84764 in exec_relay_log_event 
- #5 0x0000000000a89e97 in handle_slave_sql (arg=0x1b3e030) 
- #6 0x0000000000e341c3 in pfs_spawn_thread (arg=0x2b7f48004b20) 
- #7 0x0000003a00a07851 in start_thread () from /lib64/libpthread.so.0
- #8 0x0000003a006e767d in clone () from /lib64/libc.so.6


这种情况下，往往会造成备库延迟，这也是无索引表所带来的复制延迟问题。

如何解决问题：
- RDS 为了解这个问题，会在每个表创建的时候检查一下表是否包含主建或者唯一建，如果没有包含，则创建一个隐式主建，此主建对用户透明，用户无感，相应的show create, select * 等操作会屏蔽隐式主建，从而可以减少无索引表带来的影响;
- 官方为了解决这个问题，在5.6.6 及以后版本引入参数 slave_rows_search_algorithms ，用于指示备库在 apply_binlog_event时使用的算法，有三种算法TABLE_SCAN,INDEX_SCAN,HASH_SCAN，其中table_scan与index_scan是已经存在的，本文主要研究HASH_SCAN的实现方式，关于参数slave_rows_search_algorithms的设置。

**hash_scan 的实现方法：**

简单的讲，在 apply rows_log_event时，会将 log_event 中对行的更新缓存在两个结构中，分别是：m_hash, m_distinct_key_list。 m_hash：主要用来缓存更新的行记录的起始位置，是一个hash表； m_distinct_key_list：如果有索引，则将索引的值push 到m_distinct_key_list，如果表没有索引，则不使用这个List结构； 其中预扫描整个调用过程如下： Log_event::apply_event



**[js]**[view plain](http://www.csdn.net/article/2015-01-20/2823634#)[copy](http://www.csdn.net/article/2015-01-20/2823634#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- Rows_log_event::do_apply_event  
-    Rows_log_event::do_hash_scan_and_update   
-      Rows_log_event::do_hash_row  (add entry info of changed records)  
- if (m_key_index < MAX_KEY) (index used instead of table scan)  
-          Rows_log_event::add_key_to_distinct_keyset ()  


当一个event 中包含多个行的更改时，会首先扫描所有的更改，将结果缓存到m_hash中，如果该表有索引，则将索引的值缓存至m_distinct_key_list List 中，如果没有，则不使用这个缓存结构，而直接进行全表扫描；

**执行 stack 如下：**




**[js]**[view plain](http://www.csdn.net/article/2015-01-20/2823634#)[copy](http://www.csdn.net/article/2015-01-20/2823634#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- #0 handler::ha_delete_row 
- #1 0x0000000000a4192b in Delete_rows_log_event::do_exec_row 
- #2 0x0000000000a3a9c8 in Rows_log_event::do_apply_row
- #3 0x0000000000a3c1f4 in Rows_log_event::do_scan_and_update 
- #4 0x0000000000a3c5ef in Rows_log_event::do_hash_scan_and_update 
- #5 0x0000000000a3d7f7 in Rows_log_event::do_apply_event 
- #6 0x0000000000a28e3a in Log_event::apply_event
- #7 0x0000000000a8365f in apply_event_and_update_pos
- #8 0x0000000000a84764 in exec_relay_log_event 
- #9 0x0000000000a89e97 in handle_slave_sql
- #10 0x0000000000e341c3 in pfs_spawn_thread
- #11 0x0000003a00a07851 in start_thread () 
- #12 0x0000003a006e767d in clone () 


**执行过程说明：**

Rows_log_event::do_scan_and_update



**[js]**[view plain](http://www.csdn.net/article/2015-01-20/2823634#)[copy](http://www.csdn.net/article/2015-01-20/2823634#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- open_record_scan()  
- do
-     next_record_scan()  
- if (m_key_index > MAX_KEY)  
-          ha_rnd_next();  
- else
-          ha_index_read_map(m_key from m_distinct_key_list)         
-       entry= m_hash->get()  
-       m_hash->del(entry);  
-       do_apply_row()  
- while (m_hash->size > 0);  


从执行过程上可以看出，当使用hash_scan时，只会全表扫描一次，虽然会多次遍历m_hash这个hash表，但是这个扫描是O(1),所以，代价很小，因此可以降低扫描次数，提高执行效率。

**hash_scan 的一个 bug**

> 
bug详情：[http://bugs.mysql.com/bug.php?id=72788](http://bugs.mysql.com/bug.php?id=72788)bug原因：m_distinct_key_list 中的index key 不是唯一的，所以存在着对已经删除了的记录重复删除的问题。

bug修复：[http://bazaar.launchpad.net/~mysql/mysql-server/5.7/revision/8494](http://bazaar.launchpad.net/~mysql/mysql-server/5.7/revision/8494)

**问题扩展：**
- 在没有索引的情况下，是不是把 hash_scan 打开就能提高效率，降低延迟呢？不一定，如果每次更新操作只一条记录，此时仍然需要全表扫描，并且由于entry 的开销，应该会有后退的情况；
- 一个event中能包含多少条记录的更新呢？这个和表结构以及记录的数据大小有关，一个event 的大小不会超过9000 bytes, 没有参数可以控制这个size；
- hash_scan 有没有限制呢？hash_scan 只会对更新、删除操作有效，对于binlog_format=statement 产生的 Query_log_event 或者binlog_format=row 时产生的 Write_rows_log_event 不起作用；



## TokuDB·版本优化·7.5.0

TokuDB　7.5.0大版本已发布，是一个里程碑的版本，这里谈几点优化，以飨存储引擎爱好者们。

**a) shutdown加速**

有用户反馈TokuDB在shutdown的时候，半个小时还没完事，非常不可接受。在shutdown的时候，TokuDB在干什么呢？在做checkpoint，把内存中的节点数据序列化并压缩到磁盘。

那为什么如此耗时呢？如果tokudb_cache_size开的比较大，内存中的节点会非常多，在shutdown的时候，大家都排队等着被压缩到磁盘（串行的）。

在7.5.0版本，TokuDB官方针对此问题进行了优化，使多个节点并行压缩来缩短时间。

BTW: TokuDB在早期设计的时候已保留并行接口，只是一直未开启。

**b) 内节点读取加速**

在内存中，TokuDB内节点(internal node)的每个message buffer都有２个重要数据结构：

> 
 1) FIFO结构，保存{key, value}

 2) OMT结构，保存{key, FIFO-offset}

由于FIFO不具备快速查找特性，就利用OMT来做快速查找(根据key查到value)。这样，当内节点发生cache miss的时候，索引层需要做：

> 
 1) 从磁盘读取节点内容到内存

 2) 构造FIFO结构

 3) 根据FIFO构造OMT结构(做排序)

由于TokuDB内部有不少性能探(ji)针(shu)，他们发现步骤3)是个不小的性能消耗点，因为每次都要把message buffer做下排序构造出OMT，于是在7.5.0版本，把OMT的FIFO-offset(已排序)也持久化到磁盘，这样排序的损耗就没了。

**c) 顺序写加速**

当写发生的时候，会根据当前的key在pivots里查找(二分)当前写要落入哪个mesage buffer，如果写是顺序(或局部顺序，数据走向为最右边路径)的，就可以避免由＂查找＂带来的额外开销。

![](http://img.ptcms.csdn.net/article/201501/20/54be0f35dd41d_middle.jpg)



如何判断是顺序写呢？TokuDB使用了一种简单的启发式方法(heurstic)：seqinsert_score积分式。如果：



> 
1) 当前写入落入最右节点，对seqinsert_score加一分(原子)

2) 当前写入落入非最右节点，对seqinsert_score清零(原子)

当seqinsert_score大于100的时候，就可以认为是顺序写，当下次写操作发生时，首先与最右的节点pivot进行对比判断，如果确实为顺序写，则会被写到该节点，省去不少compare开销。方法简单而有效。

## MariaDB·　性能优化·filesort with small LIMIT optimization

从MySQL 5.6.2/MariaDB 10.0.0版本开始，MySQL/MariaDB针对"ORDER BY ...LIMIT n"语句实现了一种新的优化策略。当n足够小的时候，优化器会采用一个容积为n的优先队列来进行排序，而不是排序所有数据然后取出前n条。 这个新算法可以这么描述：（假设是ASC排序）
- 建立一个只有n个元素的优先队列（堆），根节点为堆中最大元素
- 根据其他条件，依次从表中取出一行数据
- 如果当前行的排序关键字小于堆头，则把当前元素替换堆头，重新Shift保持堆的特性
- 再取一条数据重复2步骤，如果没有下一条数据则执行5
- 依次取出堆中的元素（从大到小排序），逆序输出（从小到大排序），即可得ASC的排序结果

这样的算法，时间复杂度为m*log(n)，m为索引过滤后的行数，n为LIMIT的行数。而原始的全排序算法，时间复杂度为m*log(m)。只要n远小于m，这个算法就会很有效。

不过在MySQL 5.6中，除了optimizer_trace，没有好的方法来看到这个新的执行计划到底起了多少作用。MariaDB 10.013开始，提供一个系统状态，可以查看新执行计划调用的次数：

> 
 Sort_priority_queue_sorts

 描述: 通过优先队列实现排序的次数。(总排序次数=Sort_range+Sort_scan)

 范围: Global, Session

 数据类型: numeric

 引入版本: MariaDB 10.0.13

此外，MariaDB还将此信息打入了Slow Log中。只要指定 log_slow_verbosity=query_plan，就可以在Slow Log中看到这样的记录：



**[js]**[view plain](http://www.csdn.net/article/2015-01-20/2823634#)[copy](http://www.csdn.net/article/2015-01-20/2823634#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- # Time: 140714 18:30:39
-  # User@Host: root[root] @ localhost []
-  # Thread_id: 3  Schema: test  QC_hit: No
-  # Query_time: 0.053857  Lock_time: 0.000188  Rows_sent: 11  Rows_examined: 100011
-  # Full_scan: Yes  Full_join: No  Tmp_table: No  Tmp_table_on_disk: No
-  # Filesort: Yes  Filesort_on_disk: No  Merge_passes: 0  Priority_queue: Yes
-  SET timestamp=1405348239;SET timestamp=1405348239;  
-  select * from t1 where col1 between 10 and 20 order by col2 limit 100;  


"Priority_queue: Yes" 就表示这个Query利用了优先队列的执行计划(pt-query-digest 目前已经可以解析 Priority_queue 这个列)。更多精彩内容，敬请期待！



本文转载自MySQL.taobao.org ，感谢淘宝数据库项目组丁奇、鸣嵩、彭立勋、皓庭、项仲、剑川、武藏、祁奚、褚霸、一工。审校：刘亚琼





