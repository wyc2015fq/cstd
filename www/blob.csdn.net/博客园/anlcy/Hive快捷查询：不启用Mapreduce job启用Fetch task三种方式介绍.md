
# Hive快捷查询：不启用Mapreduce job启用Fetch task三种方式介绍 - anlcy - 博客园






# [Hive快捷查询：不启用Mapreduce job启用Fetch task三种方式介绍](https://www.cnblogs.com/camilla/p/8986555.html)
如果查询表的某一列，Hive中默认会启用MapReduce job来完成这个任务，如下：
hive>select id,name from m limit 10;--执行时hive会启用MapReduce job
我们都知道，启用MapReduce Job是会消耗系统开销的。对于这个问题，从Hive0.10.0版本开始，对于简单的不需要聚合的类似
SELECT <col> from <table> LIMIT n语句，不需要起MapReduce job，直接通过Fetch task获取数据，可以通过下面几种方法实现：
方法一：
hive>set hive.fetch.task.conversion=more;--开启fetch任务，就不启用MapReduce job；
hive>select id,name from m limit 10;
方法二：
hive>bin/hive  --hiveconf hive.fetch.task.conversion=more
方法三：
上面的两种方法都可以开启了Fetch任务，但是都是临时起作用的；如果你想一直启用这个功能，可以在${HIVE_HOME}/conf/hive-site.xml里面加入以下配置：
<property>
<name>hive.fetch.task.conversion</name>
<value>more</value>
<description>
Some select queries can be converted to single FETCH task
minimizing latency.Currently the query should be single
sourced not having any subquery and should not have
any aggregations or distincts (which incurrs RS),
lateral views and joins.
1. minimal : SELECT STAR, FILTER on partition columns, LIMIT only
2. more    : SELECT, FILTER, LIMIT only (+TABLESAMPLE, virtual columns)
</description>
</property>





