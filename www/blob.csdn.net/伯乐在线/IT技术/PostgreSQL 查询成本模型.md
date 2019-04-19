# PostgreSQL 查询成本模型 - 文章 - 伯乐在线
原文出处： [igor_sarcevic](http://shiroyasha.io/the-postgresql-query-cost-model.html)   译文出处：[CSDN/无阻我飞扬](http://blog.csdn.net/dev_csdn/article/details/78841624)
*摘要：PostgreSQL是一个自由的对象-关系数据库服务器(数据库管理系统)，被业界誉为“最先进的开源数据库”，本文作者以代码实例说明了如何计算和评估PostgreSQL查询成本，以下是译文。*
数据库查询速度如果太慢会从很多方面损害机构，比如可能会损害一些优秀应用程序的声誉，因为数据库查询速度过慢，造成后台处理速度慢得令人痛苦，并大幅增加基础设施的成本。作为一名经验丰富的Web开发人员，了解数据层的优化策略是绝对必要的。
![这里写图片描述](http://jbcdn2.b0.upaiyun.com/2017/12/e5e6809a4b86af8aff9897d8f816be4c.jpeg)
在本文中，我们将探讨PostgreSQL的成本模型，如何了解`explain`命令（关于explain命令可参见这篇[文章](http://toplchx.iteye.com/blog/2091860)）的输出，最重要的是如何利用这些模型数据来提高应用程序的吞吐量。
# 使用PostgreSQL Explain 命令
在应用程序中部署一个新的查询语句之前，最好通过PostgreSQL中的 `explain`中的命令来运行它，以评估新查询对应用系统性能的影响。
以一个示例数据库表开始，来说明`explain`命令的用法。这个表存有一百万条数据记录。


```
db # CREATE TABLE users (id serial, name varchar);
db # INSERT INTO users (name) SELECT 'John'
 FROM generate_series(1, 1000000);
db # SELECT count(*) FROM users;
      count
 1000000
(1 row)
db # SELECT id, name FROM users LIMIT 10;
 id | name
----+------
  1 | John
  2 | John
  3 | John
  4 | John
  5 | John
  6 | John
  7 | John
  8 | John
  9 | John
 10 | John
(10 rows)
```
假设需要用一个给定的id来查找一个用户名，但是在部署新的查询代码之前，要评估这个查询操作的成本。运行一个explain语句来做相关查询：


```
db # EXPLAIN SELECT * FROM users WHERE id = 870123;
        QUERY PLAN                  
--------------------------------------------------
 Gather  (cost=1000.00..11614.43 rows=1 width=9)
   Workers Planned: 2
           ->  Parallel Seq Scan on users  (cost=0.00..10614.33 rows=1 width=9)
     Filter: (id = 870123)
(4 rows)
```
在上面的例子中有很多的输出，但是可以得到它的要点。为了运行这个查询，PostgreSQL计划启动两个并行的工作进程（workers）。每个工作进程将在表上进行顺序扫描，最后，收集器合并来自两个工作进程的结果。
在本文中，重点介绍上面输出的`cost`以及PostgreSQL如何计算它。
为了简化成本探索，运行上面的查询，但限制可并行的工作进程数量为0。


```
db # SET max_parallel_workers_per_gather = 0;
db # EXPLAIN SELECT * FROM users WHERE id = 870123;
                   QUERY PLAN
--------------------------------------------------
 Seq Scan on users  (cost=0.00..17906.00 rows=1 width=9)
   Filter: (id = 870123)
(2 rows)
```
这有点简单。在只有单CPU内核的情况下，评估成本是`17906`。
# 成本值背后的数学
在PostgreSQL中，成本或惩罚点大多是一个抽象的概念。PostgreSQL可以执行查询的方式很多，而PostgreSQL总是选择最低成本值的执行规划。
计算成本，PostgreSQL首先查看表的字节数大小。接下来看看用户表的大小。


```
db # select pg_relation_size('users');
 pg_relation_size
--------------------------
     44285952
(1 row)
```
PostgreSQL会为每个要依次读取的块添加成本点。如果知道每个块都包含了`8kb`，那么就可以计算从表中读取的顺序块的成本值。


```
block_size = 8192 # block size in bytes
relation_size = 44285952
blocks = relation_size / block_size # => 5406
```
现在，已经知道块的数量，找出PostgreSQL为每个块读取分配多少个成本点。


```
db # SHOW seq_page_cost;
 seq_page_cost
 ----------
 1
(1 row)
```
换句话说，PostgreSQL为每个块分配一个成本点。这就需要 `5406`个成本点从表中读取数据。
从磁盘读取值并不是PostgreSQL需要做的。它必须将这些值发送给CPU并应用一个`WHERE`子句过滤。对于这个计算来说，如下的两个值非常有趣。


```
db # SHOW cpu_tuple_cost;
 cpu_tuple_cost
--------------------
  0.01
db # SHOW cpu_operator_cost;
 cpu_operator_cost
 ----------------
  0.0025
```
现在，用所有的值来计算在`explain` 语句中得到的值。


```
number_of_records = 1000000
block_size    = 8192     # block size in bytes
relation_size = 44285952
blocks = relation_size / block_size # => 5406
seq_page_cost   = 1
cpu_tuple_cost  = 0.01
cpu_filter_cost = 0.0025;
cost = blocks * seq_page_cost +
   number_of_records * cpu_tuple_cost +
   number_of_records * cpu_filter_cost
cost # => 17546
```
# 索引和PostgreSQL成本模型
索引在数据库工程师的生活中很可能仍然是最重要的话题。添加索引是否可以降低SELECT语句的成本呢？通过下面的例子来找出答案。
首先，在users表中添加一个索引：


```
db # CREATE INDEX idx_users_id ON users (id);
```
观察一下新索引的查询规划。


```
db # EXPLAIN SELECT * FROM users WHERE id = 870123;
           QUERY PLAN
----------  ----------  ----------
     Index Scan using idx_users_id on users  (cost=0.42..8.44 rows=1 width=9)
   Index Cond: (id = 870123)
(2 rows)
```
成本函数显著下降。索引扫描的计算比顺序扫描的计算要复杂一些。它由两个阶段组成。
PostgreSQL会考虑`random_page_cost`和`cpu_index_tuple_cost` 变量，并返回一个基于索引树的高度的值。


```
db # SHOW random_page_cost;
 random_page_cost
 ----------------
  4
db # SHOW cpu_index_tuple_cost;
 cpu_index_tuple_cost
----------
  0.005
```
对于实际的计算，请考虑阅读[成本指数](https://github.com/postgres/postgres/blob/ab72716778128fb63d54ac256adf7fe6820a1185/src/backend/optimizer/path/costsize.c#L466)计算器的源代码。
# 工作进程的成本
PostgreSQL可以启动并行的工作进程（worker）来执行查询。但是，开启一个新的工作进程，性能会受到影响。
为了计算使用并行工作进程的成本，PostgreSQL使用 `parallel_tuple_cost`这个命令，它定义了从一个工作进程传送元组到另一个工作进程的成本，`parallel_setup_cost`命令意味着启动一个新的工作进程（worker）的成本，以下是查询示例。


```
db # SHOW parallel_tuple_cost;
 parallel_tuple_cost
---------------------
  0.1
db # SHOW parallel_setup_cost;
 parallel_setup_cost
---------------------
 1000
```
