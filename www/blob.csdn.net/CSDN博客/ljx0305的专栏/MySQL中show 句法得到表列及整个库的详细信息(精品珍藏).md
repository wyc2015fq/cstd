# MySQL中show 句法得到表列及整个库的详细信息(精品珍藏) - ljx0305的专栏 - CSDN博客
2009年07月28日 15:58:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：553标签：[mysql																[table																[insert																[buffer																[variables																[服务器](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=variables&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[MySql](https://blog.csdn.net/ljx0305/article/category/570123)
show databases;
show tables from db_name;
show columns from table_name from db_name;
show index from talbe_name [from db_name];
show status;
show variables;
show [full] processlist;
show table status [from db_name];
show grants for user;
除了status,processlist和grants外，其它的都可以带有like wild选项，它可以使用SQL的'%'和'_'字符；
show databases like '%t';
将会列出所有数据库名字末尾为't'字符的数据库
当然了，在这些sql中，你也可以用db_name.table_name来代替 table_name from db_name这样写会更简便些!
如果一个用户没有一个表的任何权限，表将不在`SHOW TABLES`或`mysqlshow db_name`中的输出中显示
大家可能还记得describe table_name ，它实现的是与show columns from db_name.table_name一样的效果
show status将可以用mysqlshow --status 来得到同样的效果
|**列**|**含义**|
|----|----|
|`Name`|表名|
|`Type`|表的类型 (ISAM，MyISAM或HEAP)|
|`Row_format`|行存储格式 (固定, 动态, 或压缩）|
|`Rows`|行数量|
|`Avg_row_length`|平均行长度|
|`Data_length`|数据文件的长度|
|`Max_data_length`|数据文件的最大长度|
|`Index_length`|索引文件的长度|
|`Data_free`|已分配但未使用了字节数|
|`Auto_increment`|下一个 autoincrement(自动加1）值|
|`Create_time`|表被创造的时间|
|`Update_time`|数据文件最后更新的时间|
|`Check_time`|最后对表运行一个检查的时间|
|`Create_options`|与`CREATE TABLE`一起使用的额外选项|
|`Comment`|当创造表时，使用的注释 (或为什么MySQL不能存取表信息的一些信息)。|
`SHOW FIELDS`是`SHOW COLUMNS`一个同义词，`SHOW KEYS`是`SHOW INDEX`一个同义词。你也可以用`mysqlshow db_name tbl_name`或`mysqlshow -k db_name tbl_name` 列出一张表的列或索引。
`SHOW INDEX`以非常相似于ODBC的`SQLStatistics`调用的格式返回索引信息。下面的列被返回： 
|**列**|**含义**|
|----|----|
|`Table`|表名|
|`Non_unique`|0，如果索引不能包含重复。|
|`Key_name`|索引名|
|`Seq_in_index`|索引中的列顺序号, 从 1 开始。|
|`Column_name`|列名。|
|`Collation`|列怎样在索引中被排序。在**MySQL**中，这可以有值`A`（升序) 或`NULL`（不排序)。|
|`Cardinality`|索引中唯一值的数量。这可通过运行`isamchk -a`更改.|
|`Sub_part`|如果列只是部分被索引，索引字符的数量。`NULL`，如果整个键被索引。|
`SHOW STATUS`提供服务器的状态信息(象`mysqladmin extended-status`一样)。输出类似于下面的显示，尽管格式和数字可以有点不同： 
+--------------------------+--------+
| Variable_name            | Value  |
+--------------------------+--------+
| Aborted_clients          | 0      |
| Aborted_connects         | 0      |
| Connections              | 17     |
| Created_tmp_tables       | 0      |
| Delayed_insert_threads   | 0      |
| Delayed_writes           | 0      |
| Delayed_errors           | 0      |
| Flush_commands           | 2      |
| Handler_delete           | 2      |
| Handler_read_first       | 0      |
| Handler_read_key         | 1      |
| Handler_read_next        | 0      |
| Handler_read_rnd         | 35     |
| Handler_update           | 0      |
| Handler_write            | 2      |
| Key_blocks_used          | 0      |
| Key_read_requests        | 0      |
| Key_reads                | 0      |
| Key_write_requests       | 0      |
| Key_writes               | 0      |
| Max_used_connections     | 1      |
| Not_flushed_key_blocks   | 0      |
| Not_flushed_delayed_rows | 0      |
| Open_tables              | 1      |
| Open_files               | 2      |
| Open_streams             | 0      |
| Opened_tables            | 11     |
| Questions                | 14     |
| Slow_queries             | 0      |
| Threads_connected        | 1      |
| Threads_running          | 1      |
| Uptime                   | 149111 |
+--------------------------+--------+
上面列出的状态变量有下列含义： 
|`Aborted_clients`|由于客户没有正确关闭连接已经死掉，已经放弃的连接数量。|
|----|----|
|`Aborted_connects`|尝试已经失败的MySQL服务器的连接的次数。|
|`Connections`|试图连接MySQL服务器的次数。|
|`Created_tmp_tables`|当执行语句时，已经被创造了的隐含临时表的数量。|
|`Delayed_insert_threads`|正在使用的延迟插入处理器线程的数量。|
|`Delayed_writes`|用`INSERT DELAYED`写入的行数。|
|`Delayed_errors`|用`INSERT DELAYED`写入的发生某些错误(可能`重复键值`)的行数。|
|`Flush_commands`|执行`FLUSH`命令的次数。|
|`Handler_delete`|请求从一张表中删除行的次数。|
|`Handler_read_first`|请求读入表中第一行的次数。|
|`Handler_read_key`|请求数字基于键读行。|
|`Handler_read_next`|请求读入基于一个键的一行的次数。|
|`Handler_read_rnd`|请求读入基于一个固定位置的一行的次数。|
|`Handler_update`|请求更新表中一行的次数。|
|`Handler_write`|请求向表中插入一行的次数。|
|`Key_blocks_used`|用于关键字缓存的块的数量。|
|`Key_read_requests`|请求从缓存读入一个键值的次数。|
|`Key_reads`|从磁盘物理读入一个键值的次数。|
|`Key_write_requests`|请求将一个关键字块写入缓存次数。|
|`Key_writes`|将一个键值块物理写入磁盘的次数。|
|`Max_used_connections`|同时使用的连接的最大数目。|
|`Not_flushed_key_blocks`|在键缓存中已经改变但是还没被清空到磁盘上的键块。|
|`Not_flushed_delayed_rows`|在`INSERT DELAY`队列中等待写入的行的数量。|
|`Open_tables`|打开表的数量。|
|`Open_files`|打开文件的数量。|
|`Open_streams`|打开流的数量(主要用于日志记载）|
|`Opened_tables`|已经打开的表的数量。|
|`Questions`|发往服务器的查询的数量。|
|`Slow_queries`|要花超过`long_query_time`时间的查询数量。|
|`Threads_connected`|当前打开的连接的数量。|
|`Threads_running`|不在睡眠的线程数量。|
|`Uptime`|服务器工作了多少秒。|
关于上面的一些注释： 
- 如果`Opened_tables`太大，那么你的`table_cache`变量可能太小。 
- 如果`key_reads`太大，那么你的`key_cache`可能太小。缓存命中率可以用`key_reads`/`key_read_requests`计算。 
- 如果`Handler_read_rnd`太大，那么你很可能有大量的查询需要MySQL扫描整个表或你有没正确使用键值的联结(join)。 
`SHOW VARIABLES`显示出一些**MySQL**系统变量的值，你也能使用`mysqladmin variables`命令得到这个信息。如果缺省值不合适，你能在`mysqld`启动时使用命令行选项来设置这些变量的大多数。输出类似于下面的显示，尽管格式和数字可以有点不同： 
+------------------------+--------------------------+
| Variable_name          | Value                    |
+------------------------+--------------------------+
| back_log               | 5                        |
| connect_timeout        | 5                        |
| basedir                | /my/monty/               |
| datadir                | /my/monty/data/          |
| delayed_insert_limit   | 100                      |
| delayed_insert_timeout | 300                      |
| delayed_queue_size     | 1000                     |
| join_buffer_size       | 131072                   |
| flush_time             | 0                        |
| interactive_timeout    | 28800                    |
| key_buffer_size        | 1048540                  |
| language               | /my/monty/share/english/ |
| log                    | OFF                      |
| log_update             | OFF                      |
| long_query_time        | 10                       |
| low_priority_updates   | OFF                      |
| max_allowed_packet     | 1048576                  |
| max_connections        | 100                      |
| max_connect_errors     | 10                       |
| max_delayed_threads    | 20                       |
| max_heap_table_size    | 16777216                 |
| max_join_size          | 4294967295               |
| max_sort_length        | 1024                     |
| max_tmp_tables         | 32                       |
| net_buffer_length      | 16384                    |
| port                   | 3306                     |
| protocol-version       | 10                       |
| record_buffer          | 131072                   |
| skip_locking           | ON                       |
| socket                 | /tmp/mysql.sock          |
| sort_buffer            | 2097116                  |
| table_cache            | 64                       |
| thread_stack           | 131072                   |
| tmp_table_size         | 1048576                  |
| tmpdir                 | /machine/tmp/            |
| version                | 3.23.0-alpha-debug       |
| wait_timeout           | 28800                    |
+------------------------+--------------------------+
`SHOW PROCESSLIST`显示哪个线程正在运行，你也能使用`mysqladmin processlist`命令得到这个信息。
如果你有**process**权限， 你能看见所有的线程，否则，你仅能看见你自己的线程。
 见7.20` KILL`句法。如果你不使用`FULL`选项，那么每个查询只有头100字符被显示出来。 
`SHOW GRANTS FOR user`列出对一个用户必须发出以重复授权的授权命令。 
mysql> SHOW GRANTS FOR root@localhost;
+---------------------------------------------------------------------+
| Grants for root@localhost                                           |
+---------------------------------------------------------------------+
| GRANT ALL PRIVILEGES ON *.* TO 'root''localhost' WITH GRANT OPTION  |
+---------------------------------------------------------------------+查看表结构==用来显示创建表源码: SHOW CREATE TABLE db_name.table_name
优化表: OPTIMIZE TABLE tablename
修复表: REPAIR TABLE '$tablename' 
