
# MYSQL基础：参数调优:4:常见的缓存调优参数 - 知行合一 止于至善 - CSDN博客

2019年03月20日 20:28:05[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：240


![在这里插入图片描述](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这篇文章整理一下缓存相关常用的调优参数，并给出建议。
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 设定参数
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## tmp_table_size 与 max_heap_table_size
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)此参数一起设定了临时表的最大值。临时表是MySQL用于存储中间结果，很多操作比如Union/子查询/join/not in/exist以及复杂的group by和order by都可能会使用到临时表。仅在当前连接可见，当连接关闭时会自动释放，另外在使用的时候也是按需进行分配。而临时表的保存也可以通过internal_tmp_disk_storage_engine来进行控制，在5.7.6之后默认使用Innodb作为存储引擎，之前为MyISAM。临时表可能会保存在内存或者磁盘上，自然，根据其所使用的是磁盘还是内存会有不同的性能。当max_heap_table_size超过tmp_table_size时，超过的部分可能会转化为基于磁盘的存储方式。
[
](https://img-blog.csdn.net/20170222150651784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1bWlhb2Nu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 设定方式与优化准则

> 可根据如下参数的现状进行判断是否需要优化：

> Created_tmp_disk_tables：磁盘中的临时表

> Created_tmp_files：内存中的临时文件

> Created_tmp_tables：内存中的临时表

> Created_tmp_disk_tables表示使用到的磁盘临时表的数量。所以可根据Created_tmp_disk_tables/Created_tmp_tables的比例进行判断：

> 设定tmp_table_size过高虽然会使得此比例较低，但是也会带来浪费

> 根据经验设定低于10%较为理想，可根据对性能的要求自行调节和判断

> 建议设定：

> Thumb Rule：缺省此两项值被设定为16M，稍小。建议调节为：

> tmp_table_size：512M

> max_heap_table_size：256M

> key_buffer_size

> key_buffer_size是一个重要的参数，是使用MyISAM存储引擎的关键参数之一，用于设置索引缓存的大小，为所有线程共享。

> 设定方式与优化准则

> 可根据如下参数的现状进行判断是否需要优化：

> key_reads / key_read_requests应该尽可能的低，一般低于1/100或者更低

> 设定方式与优化准则

> Thumb Rule：缺省设定为8M过小，可根据资源状况设定为512M或者1G

> back_log

> 此参数用于控制当连接数达到max_connections时，有多少连接可以暂时放在堆栈之中，等待当前连接的资源释放。

> 设定方式与优化准则

> 可根据如下参数的现状进行判断是否需要优化：

> 执行show full processlist命令，如果确认到大量的unauthenticated user xxx |login的等待进程，则是需要优化的状况之一。

> 设定方式与优化准则

> 根据经验和需求也可以直接进行设定，缺省值为80

> Linux本身对于连接也是有限定的，如果此机器非数据库专用服务器，需要考虑其他服务需求

> 建议低于1024，根据需要可设定为256或者512

> Linux的backlog设定值

> /proc/sys/net/ipv4/tcp_max_syn_backlog: 缺省是1024

> [
> root@liumiaocn tmp
> ]
> \# cat /proc/sys/net/ipv4/tcp_max_syn_backlog
> 1024
> [
> root@liumiaocn tmp
> ]
> \#
> read_buffer_size

> 首先，借用一张网络的整理来表现一下缓存的设定。可以看到，read_buffer_size属于连接级别的缓存设定。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190320131829403.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> read_buffer_size是读入缓冲区的大小，根据percona以前的实验结果，read_buffer_size的设定并不会引起性能的较大差别，差别都在10%之内。

> 设定方式与优化准则

> Thumb Rule：缺省值1/8M，建议稍微调大，由于是connection级别的调整，不要过大。建议1M

> read_rnd_buffer_size

> read_buffer_size是随机缓冲区的大小，在排序查询时MySQL首先确认的部分。

> 设定方式与优化准则

> Thumb Rule：缺省值1/4M，建议稍微调大，由于是connection级别的调整，不要过大。建议1M

> thread_cache_size

> thread_cache_size表示客户断开连接后所放置的线程缓冲。此参数对于性能调优效果也不是很明显。

> Thumb Rule：缺省值为9。建议稍微调大，经验值可设定为64

> query_cache_size

> MySQL将查询结果放到查询缓存中，如果碰到同样的查询语句，将直接从缓存中获取，query_cache_size就是做此项设定的参数。

> 设定方式

> 查询缓存缺省是关闭的状态，对查询缓存的控制可以通过使用query_cache_type来确认。通过改值来设定ON/OFF。

> mysql
> >
> show
> variables
> like
> 'query_cache_type'
> ;
> +
> ------------------+-------+
> |
> Variable_name
> |
> Value
> |
> +
> ------------------+-------+
> |
> query_cache_type
> |
> OFF
> |
> +
> ------------------+-------+
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
> 设定方式与优化准则

> 可根据如下参数的现状进行判断和优化：

> 查询缓存碎片率= Qcache_free_blocks / Qcache_total_blocks

> 超过20%显示碎片比率过高，可考虑使用FLUSH QUERY CACHE进行整理

> 查询缓存利用率= (query_cache_size – Qcache_free_memory) / query_cache_size

> 查询利用率虽然很高，但是Qcache_lowmem_prunes也比较大的时候，考虑是否仍然是设定不足。查询利用率低于20%时可能是因为设置过大造成了浪费，可考虑是适当减小设定。

> 设定经验值：可设定为128M

> binlog_cache_size

> binlog的缓存大小，binlog一般结合每日备份可进行实时恢复。

> 设定方式与优化准则

> 由于binlog很多情况下由于性能的考量，加之故障并不是频繁地出现，很多时候环境中都未被打开。在打开的情况下的经验设定值可以参考为：2M

> wait_timeout

> wait_timeout表示请求的最大连接时间。它的作用是非交互方式的连接如果超过这个时间，如果状态是sleep的连接，MySQL会主动进行切断。

> 设定方式与优化准则

> 缺省值为28800，可根据经验或需求进行设定，比如设定为30秒或者120秒

> 注：与此值类似的是interactive_timeout，表述的是交互式的超时，缺省也是28800的8个小时，可根据需要进行调整和设定

> join_buffer_size

> join_buffer_size是使用到JOIN的情况下的连接优化参数。

> 设定方式与优化准则

> Thumb Rule：缺省值1/8M，建议稍微调大，由于是connection级别的调整，不要过大。建议1M

> sort_buffer_size

> sort_buffer_size用于设定MySQL执行排序锁使用的缓冲大小。

> 设定方式与优化准则

> Thumb Rule：缺省值1/8M，建议稍微调大，由于是connection级别的调整，不要过大。建议1M

> 参考文章

> [https://dev.mysql.com/doc/refman/5.7/en/server-system-variables.html](https://dev.mysql.com/doc/refman/5.7/en/server-system-variables.html)

> [https://dev.mysql.com/doc/refman/5.7/en/internal-temporary-tables.html](https://dev.mysql.com/doc/refman/5.7/en/internal-temporary-tables.html)

> [https://www.percona.com/blog/2007/09/17/mysql-what-read_buffer_size-value-is-optimal/](https://www.percona.com/blog/2007/09/17/mysql-what-read_buffer_size-value-is-optimal/)


