# Mysql慢查询优化 - 三少GG - CSDN博客
2013年07月28日 01:59:27[三少GG](https://me.csdn.net/scut1135)阅读数：1213

MySQL查询优化：查询慢原因和解决技巧
在开发的朋友特别是和mysql有接触的朋友会碰到有时mysql查询很慢，当然我指的是大数据量百万千万级了，不是几十条了，下面我们来看看解决查询慢的办法。
MySQL查询优化：查询慢原因和解决方法
会经常发现开发人员查一下没用索引的语句或者没有limit n的语句，这些没语句会对数据库造成很大的影响，例如一个几千万条记录的大表要全部扫描，或者是不停的做filesort，对数据库和服务器造成io影响等。这是镜像库上面的情况。
而到了线上库，除了出现没有索引的语句，没有用limit的语句，还多了一个情况,mysql连接数过多的问题。说到这里，先来看看以前我们的监控做法 ：
1. 部署zabbix等开源分布式监控系统，获取每天的数据库的io，cpu，连接数 
2. 部署每周性能统计，包含数据增加量，iostat，vmstat，datasize的情况 
3. Mysql slowlog收集，列出top 10 
以前以为做了这些监控已经是很完美了，现在部署了mysql节点进程监控之后，才发现很多弊端 
第一种做法的弊端: zabbix太庞大，而且不是在mysql内部做的监控，很多数据不是非常准备，现在一般都是用来查阅历史的数据情况 
第二种做法的弊端：因为是每周只跑一次，很多情况没法发现和报警 
第三种做法的弊端: 当节点的slowlog非常多的时候，top10就变得没意义了，而且很多时候会给出那些是一定要跑的定期任务语句给你。。参考的价值不大 
那么我们怎么来解决和查询这些问题呢 
对于排查问题找出性能瓶颈来说，最容易发现并解决的问题就是MYSQL的慢查询以及没有得用索引的查询。 
OK，开始找出mysql中执行起来不“爽”的SQL语句吧。 
方法一： 这个方法我正在用，呵呵，比较喜欢这种即时性的。 
Mysql5.0以上的版本可以支持将执行比较慢的SQL语句记录下来。 
mysql> show variables like 'long%'; 注：这个long_query_time是用来定义慢于多少秒的才算“慢查询” 
+-----------------+-----------+ 
| Variable_name | Value | 
+-----------------+-----------+ 
| long_query_time | 10.000000 | 
+-----------------+-----------+ 
1 row in set (0.00 sec) 
mysql> set long_query_time=1; 注： 我设置了1, 也就是执行时间超过1秒的都算慢查询。 
Query OK, 0 rows affected (0.00 sec) 
mysql> show variables like 'slow%'; 
+---------------------+---------------+ 
| Variable_name | Value | 
+---------------------+---------------+ 
| slow_launch_time | 2 | 
| slow_query_log | ON | 注：是否打开日志记录 
| slow_query_log_file | /tmp/slow.log | 注： 设置到什么位置 
+---------------------+---------------+ 
3 rows in set (0.00 sec) 
mysql> set global slow_query_log='ON' 注：打开日志记录 
一旦slow_query_log变量被设置为ON，mysql会立即开始记录。 
/etc/my.cnf 里面可以设置上面MYSQL全局变量的初始值。 
long_query_time=1 
slow_query_log_file=/tmp/slow.log 方法二:mysqldumpslow命令 
/path/mysqldumpslow -s c -t 10 /tmp/slow-log 
这会输出记录次数最多的10条SQL语句，其中： 
-s, 是表示按照何种方式排序，c、t、l、r分别是按照记录次数、时间、查询时间、返回的记录数来排序，ac、at、al、ar，表示相应的倒叙； 
-t, 是top n的意思，即为返回前面多少条的数据； 
-g, 后边可以写一个正则匹配模式，大小写不敏感的； 
比如 
/path/mysqldumpslow -s r -t 10 /tmp/slow-log 
得到返回记录集最多的10个查询。 
/path/mysqldumpslow -s t -t 10 -g “left join” /tmp/slow-log 
得到按照时间排序的前10条里面含有左连接的查询语句。 最后总结一下节点监控的好处 
1． 轻量级的监控，而且是实时的，还可以根据实际的情况来定制和修改 
2． 设置了过滤程序，可以对那些一定要跑的语句进行过滤 
3． 及时发现那些没有用索引，或者是不合法的查询，虽然这很耗时去处理那些慢语句，但这样可以避免数据库挂掉，还是值得的 
4. 在数据库出现连接数过多的时候，程序会自动保存当前数据库的processlist，DBA进行原因查找的时候这可是利器 
5. 使用mysqlbinlog 来分析的时候，可以得到明确的数据库状态异常的时间段 
有些人会建义我们来做mysql配置文件设置 
调节tmp_table_size 的时候发现另外一些参数 
Qcache_queries_in_cache 在缓存中已注册的查询数目 
Qcache_inserts 被加入到缓存中的查询数目 
Qcache_hits 缓存采样数数目 
Qcache_lowmem_prunes 因为缺少内存而被从缓存中删除的查询数目 
Qcache_not_cached 没有被缓存的查询数目 (不能被缓存的，或由于 QUERY_CACHE_TYPE) 
Qcache_free_memory 查询缓存的空闲内存总数 
Qcache_free_blocks 查询缓存中的空闲内存块的数目 
Qcache_total_blocks 查询缓存中的块的总数目 
Qcache_free_memory 可以缓存一些常用的查询,如果是常用的sql会被装载到内存。那样会增加数据库访问速度。
[mysql优化一般步聚(教程)](http://blog.haohtml.com/archives/2872)[mysql优化-缓存篇](http://blog.haohtml.com/archives/1420)[mysql优化数据库对象](http://blog.haohtml.com/archives/2886)[MySQL分表优化试验](http://blog.haohtml.com/archives/1939)[关于Mysql的Qcache优化](http://blog.haohtml.com/archives/10231)
