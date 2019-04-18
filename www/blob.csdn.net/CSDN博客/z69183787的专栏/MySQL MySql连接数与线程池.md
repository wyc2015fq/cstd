# MySQL MySql连接数与线程池 - z69183787的专栏 - CSDN博客
2016年10月21日 16:34:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6035
# 连接数
1、查看允许的最大并发连接数
SHOW VARIABLES LIKE 'max_connections';
![MySQL <wbr>MySql连接数与线程池](http://s8.sinaimg.cn/mw690/005NDN77zy739h6aaDd57&690)
2、修改最大连接数
方法1：临时生效
SET GLOBAL max_connections=200;
SET语法参考：
[http://dev.mysql.com/doc/refman/5.7/en/set-statement.html](http://dev.mysql.com/doc/refman/5.7/en/set-statement.html)
方法2：永久生效
查找my.ini或my.cnf文件，进行编辑设置：
# whereis my.cnf
my: /etc/my.cnf
# vim /etc/my.cnf
.....
[mysqld]
……
max_connections = 1000
重启MySQL服务
3、查看线程相关的状态变量
SHOW STATUS LIKE 'Threads%';
![MySQL <wbr>MySql连接数与线程池](http://s1.sinaimg.cn/bmiddle/005NDN77zy739hgwYqkf0&690)
输出说明：
Threads_cached：缓存中的线程连接数，该变量在嵌入式服务器(libmysqld)无意义
Threads_connected：当前打开的连接数，该值和SHOW
 PROCESSLIST;输出记录总数一样
Threads_created：为处理连接而创建的线程数。如果该值很大，可能要增加thread_cache_size。缓存未命中率=Theads_created/Connections
thread_cache_size：但客户端断开时，如果缓存中的线程连接数即Threads_cached，比thread_cache_size小，那么这个客户端线程会被放入缓存中，如果可能，通过重用缓存中线程来满足线程请求。如果有许多新连接，可以增加该项值来提高性能。
Threads_running：非睡眠状态的连接数，通常指并发连接数
# 线程池
线程池由许多线程组构成，每个组管理一系列客户端连接。一旦连接被建立，线程池会以轮询调度(round-robin)的方式把其分配给线程组。
每个线程组可拥有的最大线程数量为4096(或4095，在一些操作系统上，其中一个线程供内部使用)
线程池隔离了连接和线程，所以线程和连接之间没有固定的关系，这和缺省的线程处理模式不一样。缺省的线程处理模式会把线程同连接关联，这样以便线程执行来自连接的所有语句。
任何时刻，线程组尽量确保每个线程组中至多只有一个执行线程，但有时候，为了最佳性能，允许多余一个临时的执行线程。算法如下：
l每个线程组有一个监听线程，监听来自分配给线程组的连接的语句。当语句到达时，线程组或者立刻开始执行，或放入队列等待稍后执行。
n如果仅收到语句，且当前队列中没有排队等待执行的语句，或者无正在执行的语句，则立即执行
n如果语句不能立即被执行则放入队列。
l如果立即执行语句，那么由监听线程来执行（这意味着线程组中暂时没有线程在监听）。如果语句快速执行完，执行线程返回继续监听语句。否则，线程池会认为该语句执行滞后，并开启另一个线程作为监听线程(如果有必要的话)。为了保证没有线程组被执行滞后的语句阻塞，线程池有个后台线程定期监控线程组状态。
通过使用监听线程来执行可立执行的语句，如果语句可以快速执行完的话，不必创建一个额外线程。这样确保了在线程数很少的情况下，最大的执行效率。
当开启线程组插件时，它会为每个线程组创建一个线程(监听线程)，外加一个后台线程。有必要的话创建额外的线程来执行语句。
l线程池关注于限制当前短时间运行的语句数量。执行语句到达停滞时间(stall
 time)之前，它会阻止其它语句开始执行。如果语句超过停滞时间，则允许其继续执行，但是不再阻止其它语句的运行。通过这种方式，线程池尽力确保每个线程组中，不多于一个短时间运行的执行语句。
l如果语句遇到磁盘I/O操作或用户级锁(行锁或表锁)，语句将被阻塞。这种阻塞会导致线程组变得不可用，所以会有针对线程组的回调来确保线程池可以在改组中立即开启一个新的线程来执行其它的语句。当返回一个阻塞线程时，线程池允许立即重启它。
l有两个队列，一个高优先级(high-priority)队列和一个低优先(low-priority)级队列。事务中的第一条语句分配到低优先级队列。如果事务正在进行(刚开始执行事务中的语句)，接下来的其它任意语句分配到高优先级队列，否则进入低优先级队列。队列分配可通过[thread_pool_high_priority_connection](http://dev.mysql.com/doc/refman/5.6/en/server-system-variables.html#sysvar_thread_pool_high_priority_connection)系统变量来控制，开启该系统变量会导致会话中所有排队中的语句分配到高优先级队列
针对非事务性存储引擎或开启[autocommit](http://dev.mysql.com/doc/refman/5.7/en/server-system-variables.html#sysvar_autocommit)下的事务性存储引擎的语句，被视为低优先级语句，因为这种情况下，每条语句都是一个事务。如果给定的语句既有针对InonoDB表，也有MyISAM表的，那么线程池会为针对InnoDB表的语句安排更高的优先级，除非开启了autocommit。如果开启了autocommit，所有语句都是低优先级。
l当线程组从队列中选取语句进行执行时，它先在高优先级队列中查找，然后低优先级队列。如果找到语句则将其从队列中移除并开始执行它。
l如果语句在低优先级停留太久，线程池会把它移到高优先级队列。[thread_pool_prio_kickup_timer](http://dev.mysql.com/doc/refman/5.6/en/server-system-variables.html#sysvar_thread_pool_prio_kickup_timer)系统变量控制了这个“停留时间”。对每个线程组来说，语句停留的最大时间为10ms。
l线程池重用最活跃的线程来获取对CPU缓存更好的利用。这个小的调整对性能有重大的影响。
……
参考连接：
[http://dev.mysql.com/doc/refman/5.7/en/thread-pool-operation.html](http://dev.mysql.com/doc/refman/5.7/en/thread-pool-operation.html)
1、线程池调优
[thread_pool_size](http://dev.mysql.com/doc/refman/5.6/en/server-system-variables.html#sysvar_thread_pool_size)是控制线程池性能的最重要的参数。仅在启动服务器时可以被设置，经验表明如下：
l如果主引擎(primary
 storage engine)为InnoDB,thread_pool_size最佳设置可能在16和36之间，最常见的优化值倾向于24到36。
l如果主引擎为[MyISAM](http://dev.mysql.com/doc/refman/5.7/en/myisam-storage-engine.html), [thread_pool_size](http://dev.mysql.com/doc/refman/5.6/en/server-system-variables.html#sysvar_thread_pool_size)设置应该相当低。该值设置为4到8，倾向于获取最优性能。更高值设置对性能倾向于有点负面但不显著的影响。
查看thread_pool_size值
SHOW VARIABLES LIKE 'thread_pool_size';
修改thread_pool_siz值：参考修改最大连接值，通过修改my.conf来修改
另一个重要系统变量[thread_pool_stall_limit](http://dev.mysql.com/doc/refman/5.6/en/server-system-variables.html#sysvar_thread_pool_stall_limit)，用处理被阻塞和长时间运行的语句，确保服务器不完全被阻塞。thread_pool_stall_limit有个6秒的上限值，防止服务器死锁的风险。
thread_pool_stall_limit允许线程池处理长时间运行的语句。如果某个长时间运行的语句阻塞线程组，那么所有分配给该线程组的连接都会被阻塞，且在长时间运行的语句完成之前无法被执行。最差的情况下，这可能耗费几个小时甚至几天。
仔细考虑thread_pool_stall_limit值的选取，以便语句执行被认为停滞前，有更长的执行时间。执行停滞的语句(Stalled
 statment)，会调用额外的上下文切换，某些情况下，甚至是额外线程的创建，因此会产生许多额外超负载。另一方面，thread_pool_stall_limit值如果设置太高，意味着长时间运行的语句会更长时间的阻塞许多短时间运行的语句。短时间等待允许线程更快的启动，同时更有利于避免产生死锁。长时间等待利于包含长时间运行语句的工作负载，避免当前语句执行时，开启更多新的语句。
假设服务器执行一工作任务，即便是服务器处于负载的情况下，其99.9%的语句都在100ms内完成，剩余语句的执行时间相当均匀的分布在100ms和2小时之间。这种情况，把thread_pool_stall_limit设置为10会比较，即100ms（毫秒）。针对主要执行简单语句的的服务器来说，默认值，60ms已经很ok。
thread_pool_stall_limit参数可以在运行时被修改，以便服务器工作负载上能取得一个适当的平衡。假设开启了TP_THREAD_GROUP_STATS表，可以使用以下查询来判断停滞执行语句所占比例。
SELECT SUM(STALLED_QUERIES_EXECUTED) / SUM(QUERIES_EXECUTED)
FROM information_schema.TP_THREAD_GROUP_STATS;
该值应尽可能低，为了降低语句执行停滞的可能性，增加thread_pool_stall_limit的值。
修改thread_pool_stall_limit：同修改最大连接数“max_connections”
参考连接：
[http://dev.mysql.com/doc/refman/5.7/en/thread-pool-tuning.html](http://dev.mysql.com/doc/refman/5.7/en/thread-pool-tuning.html)
参考连接：
[http://dev.mysql.com/doc/refman/4.1/en/server-status-variables.html](http://dev.mysql.com/doc/refman/4.1/en/server-status-variables.html)
