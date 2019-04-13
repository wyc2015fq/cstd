
# hive优化心得 - leofionn的博客 - CSDN博客


2018年11月07日 22:00:20[leofionn](https://me.csdn.net/qq_36142114)阅读数：20个人分类：[hadoop搭建																](https://blog.csdn.net/qq_36142114/article/category/7383244)


limit 限制调整
– 因为使用 limit 语句时候，是先执行整个查询语句，然后再返回部分结果的
set hive.limit.optimize.enable=true;
set hive.limit.row.max.size=10000;
set hive.limit.optimize.limit.file=10;
2.JOIN 优化
。。。
本地模式
–hive 尝试使用本地模式执行查询，要不然 hive 会使用 MapReduce 来执行其他所有的查询
set hive.exec.mode.local.auto=true;
并行执行
set hive.exec.parallel=true;
严格模式
– 对分区表进行查询，在 where 子句中没有加分区过滤的话，将禁止提交任务 ( 默认： nonstrict)
set hive.mapred.mode=strict;
注：使用严格模式可以禁止 3 种类型的查询：
（ 1 ）对于分区表，不加分区字段过滤条件，不能执行
（ 2 ）对于 order by 语句，必须使用 limit 语句。
（ 3 ）限制笛卡尔积的查询（ join 的时候不使用 on ，而使用 where 的）。
调整 mapper 和 reducer 个数
set hive.exec.reducers.max=( 集群总 reduce 槽位个数 *1.5)/( 执行中的查询的平均个数 )
7.JVM 重用
set mapred.job.reuse.jvm.num.tasks=10; --10 为重用个数
索引
索引可以加快含有 group by 语句的查询的计算速度
动态分区调整
– 动态分区属性：设置为 true 表示开启动态分区功能（默认为 false ）
hive.exec.dynamic.partition=true;
– 动态分区属性：设置为 nonstrict, 表示允许所有分区都是动态的（默认为 strict ）
– 设置为 strict ，表示必须保证至少有一个分区是静态的
hive.exec.dynamic.partition.mode=strict;
– 动态分区属性：每个 mapper 或 reducer 可以创建的最大动态分区个数
hive.exec.max.dynamic.partitions.pernode=100;
– 动态分区属性：一个动态分区创建语句可以创建的最大动态分区个数
hive.exec.max.dynamic.partitions=1000;
– 动态分区属性：全局可以创建的最大文件个数
hive.exec.max.created.files=100000;
`-- 控制 DataNode 一次可以打开的文件个数
       -- 这个参数必须设置在 DataNode 的 $HADOOP_HOME/conf/hdfs-site.xml文件中`dfs.datanode.max.xcievers
8192
推测执行
– 目的：是通过加快获取单个 task 的结果以及进行侦测将执行慢的 TaskTracker 加入到黑名单的方式来提高整体的任务执行效率
（ 1 ）修改   $HADOOP_HOME/conf/mapred-site.xml 文件
mapred.map.tasks.speculative.execution
true
mapred.reduce.tasks.speculative.execution
true
（ 2 ）修改 hive 配置
set hive.mapred.reduce.tasks.speculative.execution=true;
单个 MapReduce 中多个 group by
– 多个 group by 操作组装到单个 MapReduce 任务中
set hive.multigroupby.singlemr=false;
虚拟列
– 当 hive 产生了非预期的或 null 的时候，可以通过虚拟列进行诊断，判断哪行数据出现问题
INPUT__FILE__NAME     （输入文件名）
BLOCK__OFFSET__INSIDE__FILE     （块内偏移量）
ROW__OFFSET__INSIDE__BLOCK     ( 行偏移量，需要设置 hive.exec.rowoffset=true; 启用 )
其他参数调优
– 开启 CLI 提示符前打印出当前所在的数据库名
set hive.cli.print.current.db=true;
– 让 CLI 打印出字段名称
hive.cli.print.header=true;
– 提高聚合的性能
set hive.map.aggr=true;
– 对于简单的不需要聚合的类似 SELECT  from
LIMIT n 语句，不需要起 MapReduce job ，直接通过 Fetch task 获取数据
## set hive.fetch.task.conversion=more;
原文：[https://blog.csdn.net/wang1127248268/article/details/53079028](https://blog.csdn.net/wang1127248268/article/details/53079028)

