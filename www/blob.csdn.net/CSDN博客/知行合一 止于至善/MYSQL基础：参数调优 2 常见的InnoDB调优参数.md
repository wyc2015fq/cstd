
# MYSQL基础：参数调优:2:常见的InnoDB调优参数 - 知行合一 止于至善 - CSDN博客

2019年03月20日 07:29:23[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：102


![在这里插入图片描述](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这篇文章整理一下Innodb常用的调优参数，并给出建议。
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 设定参数
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## innodb_buffer_pool_size
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)innodb_buffer_pool_size是Innodb中最重要的设定参数，如果只能选一个进行优化，那就最好选择innodb_buffer_pool_size。此项参数在5.6.2引进，5.7之前的版本需要重启数据库才能生效，而5.7之后则可以动态调整。
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 设定方式和准则：

> innodb_buffer_pool_size缓存的除了数据还有索引的信息.

> 根据官方给出的建议，如果是专用的服务器的话，可以将机器内存的80%作为innodb_buffer_pool_size值。这一经验也被广泛的采纳，但是需要注意的是官方的文章中还有一个但是：“但是，需要注意的是这样做可能会导致一些潜在性的问题”，这些问题包括：

> 过大的内存设定可能会导致操作系统的页面交换操作导致性能降低

> 由于会预留一部分内存用于缓存和结构控制，所以设定值最好是比所需数据量大10%

> 需要和innodb_buffer_pool_instances一起结合考虑进行设定，在内存小于1G的情况下，innodb_buffer_pool_instances缺省值为1。当内存较大的时候可以考虑结合innodb_buffer_pool_instances设定提高性能。

> 建议一个16G内存的机器，如果不是完全作为数据库专用，可考虑innodb_buffer_pool_size设定为8G，innodb_buffer_pool_instances设定为4

> innodb_buffer_pool_chunk_size

> innodb_buffer_pool_size的动态调整是建立在innodb_buffer_pool_chunk_size基础之上的，innodb_buffer_pool_chunk_size用于动态调整时优化移动的速度。

> 设定方式和准则：

> innodb_buffer_pool_chunk_size=innodb_buffer_pool_size/innodb_buffer_pool_instances

> 8G的innodb_buffer_pool_size + innodb_buffer_pool_instances为4的情况下，innodb_buffer_pool_chunk_size可设定为2G

> innodb_file_per_table

> innodb_file_per_table用于是否设定单表的独立表空间。Innodb存储引擎可以将数据存储在共享的表空间，也可以将每张表存放于单独的表空间。

> 设定方式和准则：

> 此参数在5.6.5之后缺省打开，建议使用独立表空间，相较而言，独立的表空间方式效率和性能会稍好一些。

> innodb_flush_method

> innodb_flush_method用于控制Innodb数据文件和redo log的打开与flush的方式，此值根据OS的不同设定不同。

> 取值（Windows）：async_unbuffered/normal/unbuffered

> 取值（Linux/Unix）：fsync/O_DSYNC/littlesync/nosync/O_DIRECT/O_DIRECT_NO_FSYNC

> 以下参看smooth-z的分析，O_DIRECT会绕过OS缓冲直接更新到磁盘，但是日志还是会经过OS的缓冲的。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190319145006841.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> 设定方式和准则：

> Windows上可以不必设定。Linux上建议设定为O_DIRECT，可以较好地降低IO。

> 读写线程控制

> 在MySQL的5.6版本之前，只能通过innodb_file_io_threads来设定整体的线程控制。而在5.7之后，此参分成两个，可以对读写分开限制。

> innodb_write_io_threads： 限制写相关的线程，缺省值为4

> innodb_read_io_threads： 限制写相关的线程，缺省值为4

> 根据如下官方参考可知，此值无法动态生效，取值的允许范围为1-64

> [https://dev.mysql.com/doc/refman/5.7/en/innodb-performance-multiple_io_threads.html](https://dev.mysql.com/doc/refman/5.7/en/innodb-performance-multiple_io_threads.html)

> 设定方式和准则：

> 可根据可用的CPU核数来设定innodb_write_io_threads和innodb_read_io_threads。innodb_read_io_threads + innodb_write_io_threads = 可用的CPU核数。

> 同时innodb_write_io_threads和innodb_read_io_threads并不一定保证一定相等，可根据情况进行调节。

> innodb_additional_mem_pool_size

> 此参数本来是用来设置 Innodb 存储的数据目录信息和其它内部数据结构的内存池，需要根据应用程序里的表的数量调整相应的内存，缺省值为1M，但是由于5.7.4之后已经废弃，此参不必再调优。

> innodb_io_capacity与innodb_io_capacity_max

> innodb_io_capacity与innodb_io_capacity_max主要是在有频繁的写操作时所进行的优化选项，具体跟磁盘性能的数据相关联，设置过低，可能导致SSD等强大的IO性能得不到体现。

> 设定方式和准则：

> 可根据经验将innodb_io_capacity设定为2000，innodb_io_capacity_max设定为6000

> innodb_thread_concurrency

> innodb_thread_concurrency用于对于线程并发的控制。一旦并发线程达到个数时，可以通过设定innodb_thread_sleep_delay来确认sleep时间来将其放到队列之中。缺省状态下设定为0表示对此不做限制。

> 设定方式和准则：

> 为提高性能的情况下，可考虑使用缺省值0，对其不做控制。

> innodb_autoinc_lock_mode

> 如果表中有auto_increment 列的情况下，可以使用innodb_autoinc_lock_mode参数进行控制在向表中进行数据插入是锁的行为，从而达到性能和安全的平衡。根据取值，主要有如下三种模式：

> innodb_autoinc_lock_mode=0时

> 提供了向后兼容的方式，在插入的语句执行之前进行锁定，结束之后才释放。会影响到并发的性能。

> innodb_autoinc_lock_mode=1时

> mysql缺省的方式，进行了一定的优化，不必等到语句完全结束。

> innodb_autoinc_lock_mode=2时

> 此模式下不再有auto_inc锁，此模式性能最优，但不能保证递增的连续不中断。

> 设定方式和准则：

> 结合binlog_format缺省设定为ROW的情况，此三种模式均为安全的行为，为提高性能建议将此值设定为2.

> innodb_log_file_size

> innodb_log_file_size用于指定一个日志组中单个日志的大小，此值的大小和数据库的写入速度于事务大小有关。

> 设定方式和准则：

> 结合innodb_buffer_pool_size进行设定，避免不必要的flush操作

> 具体根据事务和数据的大小进行设定

> 上限值为4G，建议可取值256M兼顾性能和恢复的速度。

> innodb_log_buffer_size

> innodb_log_buffer_size是事务在内存中的缓冲，是日志缓冲区的大小。默认设定为16M。

> 设定方式和准则：

> 有大量事务的可以考虑设定为16M

> 如果增长过快，可以进行适当的增加

> 如果有大量的TEXT或者BLOB字段需要处理，也可以考虑增加这个参数的值。

> innodb_flush_logs_at_trx_commit

> innodb_flush_logs_at_trx_commit用于控制事务的提交方式，即刷新日志到磁盘的方式。其取值主要有如下三种：

> innodb_flush_logs_at_trx_commit=0

> 数据以每秒一次的频度写入，同时进行文件系统到磁盘的同步，但事务的提交并不会触发日志缓冲到日志文件的更新或者文件系统到磁盘的刷新操作。由于事务的提交不会触发同步，所有有可能丢失一秒中的事务。

> innodb_flush_logs_at_trx_commit=1

> 为系统默认设定。每次提交是将日志缓冲中的数据写到日志文件，同时触发文件系统到磁盘的同步操作。此种方式可以做到不丢失事务。

> innodb_flush_logs_at_trx_commit=2

> 事务提交会触发日志缓冲到日志文件的刷新，但不会触发磁盘文件系统到磁盘的同步。文件系统到磁盘的同步操作以每秒一次的频度执行。性能是最好的，但是可能在系统宕机或者断电是会丢失最后一秒的事务信息

> 设定方式和准则：

> 在对于最后一秒数据在出现异常时的高可用性的要求不是特别高的情况下，可设定为2以追求最好的性能。

> 参考文章

> [https://dev.mysql.com/doc/refman/5.7/en/innodb-parameters.html](https://dev.mysql.com/doc/refman/5.7/en/innodb-parameters.html)

> [https://blog.csdn.net/smooth00/article/details/72725941](https://blog.csdn.net/smooth00/article/details/72725941)

> [https://dev.mysql.com/doc/refman/5.6/en/innodb-parameters.html\#sysvar_innodb_buffer_pool_size](https://dev.mysql.com/doc/refman/5.6/en/innodb-parameters.html#sysvar_innodb_buffer_pool_size)

> [https://www.percona.com/sites/default/files/innodb_performance_optimization_final.pdf](https://www.percona.com/sites/default/files/innodb_performance_optimization_final.pdf)

> [https://www.percona.com/blog/2016/10/12/mysql-5-7-performance-tuning-immediately-after-installation/](https://www.percona.com/blog/2016/10/12/mysql-5-7-performance-tuning-immediately-after-installation/)


