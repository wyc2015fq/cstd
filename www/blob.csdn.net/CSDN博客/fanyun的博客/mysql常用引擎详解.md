# mysql常用引擎详解 - fanyun的博客 - CSDN博客
2016年03月18日 14:00:28[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1320
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)

**（一）   myisam存储引擎**
 myisam的引擎的物理结构：包括三种文件：.frm.myd .myi三种。
** myisam特有的特性：**
（1）**可以支持将数据文件和索引文件放在不同的地方，以达到性能优化的目的。**
**[**root@rhel131mysql]# mkdir -p /tmp/mysql/data
[root@rhel131 mysql]# chown-R mysql.mysql /tmp/mysql/data
[root@rhel131 mysql]# mkdir-p /tmp/mysql/index
[root@rhel131 mysql]# chown-R mysql.mysql /tmp/mysql/index
mysql>createtable t(id int) engine=myisam data directory='/tmp/mysql/data'indexdirectory='/tmp/mysql/index';
Query OK, 0 rows affected(0.10 sec)
mysql>showtable status like 't' \G;
***************************1. row ***************************
          Name: t
        Engine:MyISAM
        Version: 10
     Row_format: Fixed
           Rows: 0
 Avg_row_length: 0
    Data_length: 0
Max_data_length: 1970324836974591
   Index_length: 1024
      Data_free: 0
 Auto_increment: NULL
    Create_time: 2013-10-24 12:46:07
    Update_time: 2013-10-24 12:46:07
     Check_time: NULL
      Collation: latin1_swedish_ci
       Checksum: NULL
 Create_options:
        Comment:
1 row in set (0.01 sec)
ERROR:
No query specified
**查看一个产生的文件：**
[root@rhel131 test]# pwd
/usr/local/mysql/data/test
[root@rhel131 test]# ll
total 16
-rw-r--r--  1 mysqlmysql   65 Jul 11 00:17 db.opt
-rw-rw----  1 mysqlmysql 8556 Oct 24 12:46 t.frm
lrwxrwxrwx  1 mysqlmysql   21 Oct 24 12:46 t.MYD ->/tmp/mysql/data/t.MYD
lrwxrwxrwx  1 mysql mysql  22 Oct 24 12:46 t.MYI ->/tmp/mysql/index/t.MYI
**数据文件和索引文件都是软链接文件，接到了我指定的目录。**
**（2）     灵活的自动增长列类型**
**对已存在的表增加一个自动增长的列：**
mysql>altertable t add column id1 int not null auto_increment,add primary key (id1);
Query OK, 0 rows affected(0.10 sec)
Records: 0  Duplicates:0  Warnings: 0
mysql>desct;
+-------+---------+------+-----+---------+----------------+
| Field |Type    | Null | Key | Default|Extra          |
+-------+---------+------+-----+---------+----------------+
| id    |int(11) | YES  |     |NULL   |               |
| id1   | int(11)| NO   | PRI | NULL    |auto_increment |
+-------+---------+------+-----+---------+----------------+
2 rows in set (0.00 sec)
mysql>insertinto t (id) values(1);
Query OK, 1 row affected(0.03 sec)
mysql>insertinto t (id) values(1);
Query OK, 1 row affected(0.01 sec)
mysql>insertinto t (id) values(1);
Query OK, 1 row affected(0.00 sec)
mysql>insertinto t (id) values(1);
Query OK, 1 row affected(0.00 sec)
mysql>select* from t;
+------+-----+
| id   | id1 |
+------+-----+
|    1|   1 |
|    1|   2 |
|    1|   3 |
|    1|   4 |
+------+-----+
4 rows in set (0.00 sec)
**（3）     不支持事务的特性：**
mysql>droptable t;
Query OK, 0 rows affected(0.00 sec)
mysql>createtable t(id int) engine=myisam;
Query OK, 0 rows affected(0.02 sec)
查询是否自动提交，将自动提交给关闭
mysql> show variableslike '%autocommit%';
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
|autocommit    | ON    |
+---------------+-------+
1 row in set (0.01 sec)
mysql>setsession autocommit=off;
Query OK, 0 rows affected(0.01 sec)
mysql>showvariables like '%autocommit%';
+---------------+-------+
| Variable_name | Value |
+---------------+-------+
|autocommit    | OFF   |
+---------------+-------+
1 row in set (0.01 sec)
插入一条数据
mysql>insertinto t values(1);
Query OK, 1 row affected(0.00 sec)
mysql>select* from t;
+------+
| id   |
+------+
|    1 |
+------+
1 row in set (0.01 sec)
做回滚操作后，如果支持事务操作的话，应该后取消我刚刚插入的一笔数据。
mysql>rollback;
Query OK, 0 rows affected,1 warning (0.00 sec)
不过做了回滚之后，这笔数据还在，说明myisam不支持事务操作，在插入数据后直接写到了磁盘上。
mysql>select* from t;
+------+
| id   |
+------+
|    1 |
+------+
1 row in set (0.00 sec)
## （二）InnoDB存储引擎
 ·遵循ACID，支持commit，rollback和故障恢复，是事务安全的
·行级锁定，Oracle-style读一致性改善了多用户并发操作性能
·支持FOREIGNKEY参照完整性
·轻松地与其他存储引擎表组合，例如与MEMORY表的JOIN
      InnoDB在内存中维持着自己的缓冲区，用来缓存数据和索引。InnoDB的数据和索引存放在表空间中，表空间可以是共享的，也可以是独享的。
### （1）     独享表空间
开启独享表空间模式之后InnoDB以table_name.idb命名在数据库目录之中保存新创建的表，数据和索引都保存在.idb文件，.frm仍然会创建用来保存元数据。
即使使用独享表空间，共享表空间也会存在，因为需要它存放一些undo信息和其他元数据信息。因此不能像MyISAM表文件那样在数据库目录间随便移动.idb文件，因为共享表空间里会保存数据库名，数据库间的移动应使用RENAMETABLE语句：RENAMETABLEdb1.tbl_nameTO
 db2.tbl_name;    
      .idb文件的恢复方法：
1、 ALTERTABLE tbl_nameDISCARD TABLESPACE;丢弃现有表空间文件
2、复制备份的.idb文件至正确的目录
3、 ALTERTABLE tbl_name IMPORTTABLESPACE;使用新的idb文件
### （2）     InnoDB启动选项和系统变量
|**Variables**|**Description**|
|----|----|
|innodb|控制InnoDB的加载。OFF，ON，FORCE|
|innodb_additional_mem_pool_size|InnoDB用于保存数据字典信息和其他内部数据结构的内存区大小，默认8M。超出时，使用操作系统内存并向error log写错误信息|
|innodb_autoextend_increment|自动增长的共享表空间写满时的增长大小|
|innodb_autoinc_lock_mode|auto_increment自动增长值的锁定模式|
|innodb_buffer_pool_size|InnoDB缓存数据和索引的缓冲区大小。默认是128M，数据库专用服务器可以将其设置为物理内存的80%|
|innodb_change_buffering|开启修改缓存的类型 inserts, deletes,  purges, changes, all, none|
|innodb_checksums|开启校验和|
|innodb_commit_concurrency|同时刻可以进行提交操作的线程数。值为0允许任意多事务同时提交|
|innodb_concurrency_tickets|线程通过innodb_thread_concurrency并发线程数验证后，可以得到一个innodb_concurrency_tickets数量的访问次数，在该次数范围内不需要再进行并发线程数验证。|
|innodb_data_file_path|指定数据文件，格式为file_name:file_size[:autoextend[:max:max_file_size]]autoextend和max选项只能用于最后一个数据文件|
|innodb_data_home_dir|数据文件根目录|
|innodb_doublewrite|启用后，InnoDB分两次存储数据，第一次写入buffer，第二次实际写入数据文件|
|innodb_fast_shutdown|InnoDB关闭模式，默认1为快速关闭（正常关闭）|
|innodb_file_format|新创建的InnoDB表的文件格式，Antelope和Barracuda|
|innodb_file_format_check|InnoDB是否检查共享表空间的file format tag，tag大于当前InnoDB所支持版本时InnoDB启动出错，反之，InnoDB向tag写入当前innodb_file_format_max的值|
|innodb_file_format_max|向共享表空间file format tag写入的值|
|innodb_file_per_table|开启独享表空间|
|innodb_flush_log_at_trx_commit|0：每隔一秒将日志写入log  file并flush到磁盘1：每次事务提交将日志写入log  file并flush到磁盘，默认2：每次事务提交将日志写入log  file，每隔一秒flush到磁盘|
|innodb_flush_method|设置flush模式fdatasync：InnoDB使用fsync()函数去更新日志和数据文件。默认。O_DSYNC：InnoDB使用O_SYNC模式打开并更新日志文件，用fsync()函数去更新数据文件。O_DIRECT：InnoDB使用O_DIRECT模式（跳过文件系统cache）打开数据文件，用fsync()函数去更新日志和数据文件|
|innodb_force_recovery|恢复模式 0-6|
|innodb_lock_wait_timeout|事务等待锁定的超时时间，仅对行锁定有效|
|innodb_log_buffer_size|InnoDB日志缓冲区大小|
|innodb_log_file_size|日志组中日志文件的大小，默认5MB，必须小于4GB|
|innodb_log_files_in_group|日志组中的日志成员数|
|innodb_log_group_home_dir|InnoDB日志根目录|
|innodb_max_dirty_pages_pct|脏数据所占的最大百分比|
|innodb_max_purge_lag|限制每次删除更新操作影响的最大行数，超过该值操作会被延迟|
|innodb_mirrored_log_groups|日志的镜像拷贝数量|
|innodb_old_blocks_pct|InnoDB缓冲区中old sublist百分比|
|innodb_old_blocks_time|在old sublist必须停留超过该变量的时间，才能移到new sublist中|
|innodb_open_files|限制同时打开.idb文件数，只有在独享表空间模式有效|
|innodb_purge_batch_size|redo中记录触发清除操作的粒度|
|innodb_purge_threads|InnoDB清除操作的线程数，默认0表示由InnoDB主线程完成清除操作。|
|innodb_read_ahead_threshold|顺序读取超过该值指定的页数才会预取|
|innodb_read_io_threads|读操作的I/O线程数，默认为4|
|innodb_replication_delay|innodb_thread_concurrency满时，slave端复制线程的延迟时间(ms)|
|innodb_rollback_on_timeout|默认情况下事务超时仅回滚最后一条语句。启用该选项后InnoDB中止并回滚整个事务|
|innodb_spin_wait_delay|两次尝试获取自旋锁的延迟时间|
|innodb_stats_on_metadata|启用后，InnoDB会在那些metadata statement（如SHOW  TABLE STATUS、SHOW INDEX）或访问INFORMATION_SCHEMA中TABLES、STATISTICS表时更新统计信息。|
|innodb_stats_sample_pages|ANALYZE TABLE计算时索引页采样数量|
|--innodb-status-file|InnoDB是否在数据目录中创建名为innodb_status.<pid>的状态文件，启用后，InnoDB每隔一段时间写入SHOW ENGINE INNODB STATUS的输出结果|
|innodb_strict_mode|遇到异常时返回错误而不是提示警告信息|
|innodb_support_xa|启用XA事务支持|
|innodb_sync_spin_loops|自旋锁超时等待时间|
|innodb_table_locks|InnoDB内部获取表锁|
|innodb_thread_concurrency|并发访问InnoDB的线程数，超过数量限制时进入FIFO队列等待。|
|innodb_thread_sleep_delay|并发访问数超过限制后线程的等待时间|
|innodb_use_native_aio|使用本地AIO|
|innodb_use_sys_malloc|InnoDB使用操作系统内存分配器还是InnoDB自己的|
|innodb_version|InnoDB版本号|
|innodb_write_io_threads|写操作的I/O线程数|
### （3）     InnoDB数据文件、日志文件的添加、删除、重置大小
**加入新数据文件：**
修改配置文件中innodb_data_file_path，加入新的数据文件名，大小。autoextend选项只能用在最后一个数据文件中。
**实验中遇到的问题：**
原配置：innodb_data_file_path=ibdata1:10M:autoextend
修改后：innodb_data_file_path=ibdata1: 10M;ibdata2:20M:autoextend
重新启动服务器，showengines发现innodb没有启动起来，查阅错误日志出现如下提示：
InnoDB:Error:data file/app/mysql/var/ibdata1 is of a different size
InnoDB:227968pages(rounded down to MB)
InnoDB:thanspecified inthe .cnf file 640 pages!
**原来，是因为第一个数据文件没有了autoextend选项，而该文件的实际大小超过了设置的10M，导致了文件大小不匹配的错误。**
解决：innodb_data_file_path =ibdata1:3562M;ibdata2:20M:autoextend将ibdata1的大小设置为其实际大小，不能多也不能少（错误提示中的**page=64MB**，du命令计算出的结果有偏差），之后重启服务器，InnoDB成功启动，ibdata2也自动创建了。
**删除数据文件：**
不能直接在配置文件将innodb_data_file_path的数据文件移除，重启后错误日志会有如下提示：
      InnoDB:Error: tablespace size stored in header is 229248pages, but
InnoDB:thesum of data filesizes is only 227968 pages
InnoDB:Cannotstart InnoDB.The tail of the system tablespace is
InnoDB:missing.Have youedited innodb_data_file_path in my.cnf in an
InnoDB:inappropriateway,removing ibdata files from there?
InnoDB:Youcan setinnodb_force_recovery=1 in my.cnf to force
InnoDB:astartup if you aretrying to recover a badly corrupt database.
说明InnoDB会在数据文件头部存放表空间的大小，检查innodb_data_file_path中的数据文件容量之和不匹配就会出现上述的错误提示
手册中推荐的方法：
1、 mysqldump转储InnoDB表
2、关闭服务器
3、删除（或移动至其他目录）所有存在的表空间文件，包括ibdata，ib_log和InnoDB表的所有.frm文件
4、 my.cnf重新配置表空间
5、重启服务器
6、导入转储文件
**日志文件的修改可以参考：**
[http://www.mysqlperformanceblog.com/2011/07/09/how-to-change-innodb_log_file_size-safely/](http://www.mysqlperformanceblog.com/2011/07/09/how-to-change-innodb_log_file_size-safely/)
### （4）     InnoDB锁定机制
共享锁：允许事务读取行
排他锁：允许事务更新、删除行
意向共享锁：需要共享锁而资源被占用，则在表上加意向共享锁
意向排他锁：需要排他锁而资源被占用，则在表上加意向排他锁
||**共享锁（S）**|**排他锁（X）**|**意向共享锁（IS）**|**意向排他锁（IX）**|
|----|----|----|----|----|
|**共享锁（S）**|**√**|**×**|**√**|**×**|
|**排他锁（X）**|**×**|**×**|**×**|**×**|
|**意向共享锁（IS）**|**√**|**×**|**√**|**√**|
|**意向排他锁（IX）**|**×**|**×**|**√**|**√**|
读一致性：在一个事务中，每次读取的结果都是一致的。
|**session  a**|**session  b**|
|----|----|
|SET  autocommit=0;|SET  autocommit=0;|
|mysql>  SELECT * FROM t1;Empty  set (0.00 sec)||
||mysql>  insert into t1 values(1,'a');|
|mysql>  SELECT * FROM t1;Empty  set (0.00 sec)||
||mysql>  commit;|
|mysql>  SELECT * FROM t1;Empty  set (0.00 sec)||
|mysql>  commit;||
|mysql>  select * from t1;+------+------+|  id   | name |+------+------+|    1 | a     |+------+------+||
看到，会话a在发出第一句SELECT后事务即算开始，一直到commit之前，无论会话b是否提交读取结果都保持一致。此时若想查看最新数据可以使用SELECT*
 FROM t1 LOCK INSHARE MODE;
再做一个测试：
CREATETABLE `t1` (
 `id` int(11) DEFAULTNULL,
 `name` varchar(20)DEFAULT NULL
)ENGINE=InnoDBDEFAULTCHARSET=utf8
|**session  a**|**session  b**|
|----|----|
|SET  autocommit=0;|SET  autocommit=0;|
|mysql>  select * from t1;+------+------+|  id   | name |+------+------+|    1 | a     ||    2 | b     ||    3 | c     |+------+------+|mysql>  select * from t1;+------+------+|  id   | name |+------+------+|    1 | a     ||     2 | b    ||    3 | c     |+------+------+|
|mysql>  update t1 set name='x' where id=3;Query  OK, 1 row affected (0.00 sec)||
||mysql>  update t2 set name='y' where id=2;等待中|
|mysql>  commit;||
||Query  OK, 1 row affected (11.34 sec)|
在这个测试中，发现insert语句等待，说明此时session a的操作触发了表锁定，InnoDB不是行锁定？怎么锁表了呢？原来，**只有当InnoDB使用了索引，才会触发行锁**，因此加上索引再做一次测试：
CREATETABLE `t2` (
 `id`int(11) DEFAULTNULL,
 `name`varchar(20)DEFAULT NULL,
 KEY`id` (`id`)
)ENGINE=InnoDB DEFAULTCHARSET=utf8
|**session  a**|**session  b**|
|----|----|
|SET  autocommit=0;|SET  autocommit=0;|
|mysql>  select * from t2;+------+------+|  id   | name |+------+------+|    1 | a     ||    2 | b     ||    3 | c     |+------+------+|mysql>  select * from t2;+------+------+|  id   | name |+------+------+|    1 | a     ||    2 | b     ||    3 | c     |+------+------+|
|mysql>  update t2 set name='y' where id=1;Query  OK, 1 row affected (0.00 sec)||
||mysql>  update t2 set name='z' where id=2;Query  OK, 1 row affected (0.00 sec)|
||mysql>  update t2 set name='i' where id=1;等待了|
|mysql>  update t2 set name='j' where id=2;ERROR  1213 (40001): Deadlock found when trying to get lock; try restarting  transaction||
看到，加了索引以后，update操作使用了行锁，行锁的一个弊端就是可能导致死锁。这么看来，InnoDB的行锁并不是随心所欲的使用，必须要考虑到索引，上面的这个例子中如果更新操作的where条件是name这个不加索引的列，一样触发表锁。而有些操作，使用索引还是需要全表扫描，优化器会放弃使用索引，这样的操作也会导致表锁定。
### （5）     InnoDB与MyISAM的对比
||**InnoDB**|**MyISAM**|
|----|----|----|
|**存储**|数据和索引存放在同一文件|数据和索引分开存储|
|可以共享表空间，也可以独享表空间|每张表分开存储| |
|不存储行数，count(*)全表扫描|会存储行数| |
|**事务**|事务安全，ACID|不支持事务|
|行级锁定|表级锁定| |
|通过设置autocommit决定|每句语句自动提交| |
|**缓存**|innodb_buffer  / log_pool_sizeinnodb_additional_mem_pool_size|key_buffer_size|
|缓存数据、索引，日志，数据字典|缓存索引| |
|**其他**|不支持REPAIR TABLE，可以CHECK|支持CHECK，REPAIR  TABLE|
|具备故障恢复能力|通过myisamchk进行故障恢复| |
|**性能对比测试**| | |
|**插入速度**|45.96 s|25.82 s|
|**查询行数**|2.11 s|0.00 s|
|**全表扫描**|10.44 s|7.25 s|
|**更新（索引查找）**|1.49 s|0.62 s|
|**更新（非索引查找）**|7.45 s|9.44 s|
|**数据容量**|561 MB|502 MB|
总结一下：
      MyISAM的前身ISAM在设计时就被设定为适合处理读大于写的环境，所以在测试数据中，我们也看到MyISAM在全表扫描和插入数据时的优异表现。而MyISAM会单独保存表的行数，因此查询行数时直接取出就好了。
      InnoDB则被设计成适用于高并发的事务处理环境，因此它在事务方面功能强大，并提供专门的事务日志，采用了表空间的概念。
### （6）     InnoDB索引
**聚集索引：**
InnoDB将表中数据按**主键顺序**构造成一颗B+树，叶子节点存放着整张表的行记录数据（索引组织表，即叶子节点就是数据页）。因为数据页只能按一棵B+树排序，因此每张表只能有一个聚集索引（因此也只能有一个PRIMARYKEY）。
Oracle中，表的默认类型是**堆表**，数据按插入顺序存放，堆表上的索引存储rowid。
**辅助索引：**
叶子节点除了包含索引键值外，还包含了**聚集索引键值**。一张表可以存在多个辅助索引。通过辅助索引查找时，InnoDB通过辅助索引叶子节点的指针获取主键，再通过主键索引找到完整的行。
**一个比较有意思的实验：**
CREATE TABLE `t1` (
  `a` int(11) NOT  NULL,
  `b` varchar(20)  DEFAULT NULL,
  PRIMARY KEY (`a`),
  KEY `b` (`b`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
INSERT INTO t1 VALUES(1,'d');
INSERT INTO t1 VALUES(2,'c');
INSERT INTO t1 VALUES(3,'b');
INSERT INTO t1 VALUES(4,'a');
此时执行SELECT *FROM t1的结果会是如何呢？恐怕多数人答案都应该是：
+---+------+
|a |b    |
+---+------+
|1 |d    |
|2 |c    |
|3 |b    |
|4 |a    |
+---+------+
但事实确实：
                    mysql>SELECT* FROM t1;
+---+------+
|a |b    |
+---+------+
|4 |a    |
|3 |b    |
|2 |c    |
|1 |d    |
+---+------+
查看一下这条语句的执行计划：
                    mysql>EXPLAINSELECT * FROM t1 \G
***************************1.row***************************
          id:1
 select_type: SIMPLE
       table:t1
        type:index
possible_keys:NULL
**         key:b**
     key_len:63
         ref:NULL
        rows:4
       Extra:Using index 
可以看到，优化器选择了辅助索引。这里面的原因，
第一，辅助索引存放了主键的值，**通过辅助索引能找完整的行信息**。
第二，因为聚集索引叶子节点存放的完整行信息肯定大于辅助索引叶子节点存放的数据，所以辅助索引页包含的行数多于聚集索引，**查找时需要的页数也就相对少于聚集索引**。
基于以上两点，InnoDB总是会先在辅助索引上判断是否能取得需要的信息。
**插入缓存（InsertBuffering）**
应用程序的插入行操作通常是按主键递增的顺序插入的，和聚集索引的结构一致，因此插入聚集索引通常都是顺序的，不需要随机读取。但是对于辅助索引，插入和更新操作影响的页并不是顺序的，因此会导致大量的随机I/O操作。
InnoDB的插入缓存机制解决了以上的问题，它会检查辅助索引页是否在缓冲池中，如果在，InnoDB就直接对索引页应用变更。否则，InnoDB在insertbuffer中记录变更，并周期性地**合并同一索引页上的操作**，大大提高了对辅助索引执行插入操作的性能。
**自适应哈希索引（adaptivehashindex）：**
InnoDB存在一个监控索引查找的机制，当发现建立哈希索引可以提升查询效率时，便会自动创建，因此称之为自适应（adaptive）哈希索引。哈希索引的建立基于表上已存在的B+树，并且可只在那些经常访问的索引页上建立。
mysql>SHOWENGINE INNODBSTATUS \G可以查询到当前insertbuffer和adaptivehashindex的信息：
-------------------------------------
INSERTBUFFERAND ADAPTIVEHASH INDEX
-------------------------------------
Ibuf:size1, free list len789, seg size 791,
**460662inserts, 460662merged recs,28908 merges **
Hashtablesize 34679, nodeheap has 51 buffer(s)
**0.00 hashsearches/s, 0.00non-hash searches/s **
### （7）     InnoDB磁盘I/O和文件空间管理
**两次写（DoublewriteBuffer）**
在将页写入数据文件之前，InnoDB先将页写入共享表空间中的doublewritebuffer（为连续的页，顺序写入），只有在这部操作完成后InnoDB才会将页写入数据文件。两次写机制增加了数据库的可靠性，如果在写入数据文件中出现了宕机，可能出现页损坏的情况，这时只要找到共享表空间doublewrite中该页的副本拷贝到数据文件，再应用重做日志便可完成恢复过程。
**页（Page），区（Extent），段（Segment），表空间（Tablespace）**
表空间：由段组成，最大的逻辑单位，共享表空间、独享表空间
段：由区组成，数据段、索引段、回滚段
区：64个连续的页组成一个区，大小为1MB
页：16KB，InnoDB管理的最小单位
### （8）     InnoDB重做日志
      InnoDB的重做日志文件在设计上类似Oracle的联机重做日志文件。至少要有1个重做日志组，每个日志组至少要有2个成员，默认情况下为ib_logfile0和ib_logfile1。通过设置镜像日志组带来更高的可靠性。这里和Oracle不同的是，**Oracle是组间切换，组内镜像**。**InnoDB则是组内切换，组间镜像**。
参数innodb_log_file_size指定重做日志文件的大小。innodb_log_file_in_group决定组内日志文件数量。innodb_mirrored_log_groups指令镜像日志组的数量。
      InnoDB重做日志和MySQL二进制日志的区别:
|**InnoDB  Redo Log**|**Bin  Log**|
|----|----|
|只记录InnoDB的事务|记录MySQL所有存储引擎，包括InnoDB|
|记录每个Page的物理更改|记录事务的具体操作，基于Statement或Row|
|事务过程中，不断生成redo，写入由innodb_flush_log_at_trx_commit决定|在发出commit之后，事务提交之前，写入二进制缓冲，刷入文件由sync_binlog决定|
|可设置多个镜像日志组，组内循环使用|写满了生成新的bin log文件|
### （9）     InnoDB事务
事务的实现——redo和undo
|**redo**|**undo**|
|----|----|
|用于恢复|用于回滚|
|保存在重做日志文件中|保存在共享表空间，回滚段中|
|redo  log在日志组内循环利用|undo页会由master thread回收|
**事务的控制语句：**
·STARTTRANSACTION| BEGIN显式地开始一个事务，BEGIN不能用在存储过程中（会保留字冲突）。打开自动提交时，需要显式地开始事务，不然每句语句结束后自动提交。
       ·COMMIT和COMMITWORK基本上一致。COMMITWORK的效果受到参数completion_type控制，为0时与COMMIT一样。为1时，COMMITAND
 CHAIN即提交后马上开始下一个事务。为2时，COMMITANDRELEASE即提交后释放连接。
针对COMMITWORK，进行如下的实验：
--  事务开始
BEGIN;
--  插入数据
INSERT  INTO t1 VALUES(1,'a'),(2,'b');
--  提交，根据completion_type不同产生不同的效果
COMMIT  WORK;
--  插入第三条数据
INSERT  INTO t1 VALUES(3,'c');
--  回滚操作结束事务
ROLLBACK;
--  查看t1表的内容
SELECT  * FROM t1;
**      completion_type=0**
                           mysql>SELECT *FROM t1;
                           +------+------+
|id   | name |
+------+------+
|   1 |a   |
|   2 |b   |
|   3 |c   |
+------+------+
3 rows inset (0.00 sec)
由于是自动提交，所以插入第三条数据后已经提交完成了事务，回滚没有达到预期的效果。
**      completion_type=1**
                           mysql>SELECT *FROM t1;
+------+------+
|id   | name |
+------+------+
|   1 |a   |
|   2 |b   |
+------+------+
2 rows inset (0.00 sec)
此时，COMMIT WORK提交后立即开始下一个事务，因此插入数据后不会自动提交，回滚操作将插入的第三条语句回滚掉，只剩下两条数据。
**completion_type=2**
mysql>INSERT INTOt1VALUES(3,'c');
ERROR2006 (HY000):MySQLserver has gone away
      COMMITWORK提交后释放连接，执行INSERT时提示连接失效。
    ·ROLLBACK和ROLLBACKWORK也是类似的区别。
·SAVEPOINT与ROLLBACKTO[SAVEPOINT]，返回保存点不会结束事务
|**session  a**|**session  b**|
|----|----|
|mysql>  BEGIN;||
|mysql>  INSERT INTO t1VALUES(1,'a');||
|mysql>  SAVEPOINT p1;||
||mysql>  SELECT * FROM t1;Empty  set (0.00 sec)|
|mysql>  INSERT INTO t1VALUES(2,'B');||
|mysql>  ROLLBACK TO p1;||
||**mysql> SELECT * FROM  t1;****Empty set (0.00 sec)**|
|mysql>  COMMIT;||
||mysql>  SELECT * FROM t1;+------+------+|  id   | name |+------+------+|    1 | a     |+------+------+1  row in set (0.00 sec)|
·SETTRANSACTION设置事务的隔离级别，即：
                    1、READUNCOMMITTED
                    2、READCOMMITTED
                    3、REPEATABLEREAD
                    4、SERIALIZABLE
|**ISOLATION LEVEL**|**Dirty Reads**|**Non-Repeatable Reads**|**Phantom Reads**|
|----|----|----|----|
|**READ UNCOMMITTED**|**√**|**√**|**√**|
|**READ COMMITTED**|**×**|**√**|**√**|
|**REPEATABLE READ**|**×**|**×**|**√**|
|**SERIALIZABLE**|**×**|**×**|**×**|
 InnoDB默认支持REPEATABLEREAD，但是由于InnoDB使用Next-keyLock（即除了对索引记录加锁，还对索引记录之前的区间加锁），可以避免幻读，所以InnoDB的REPEATABLEREAD隔离级别已经达到了SQL标准的SERIALIZABLE。
