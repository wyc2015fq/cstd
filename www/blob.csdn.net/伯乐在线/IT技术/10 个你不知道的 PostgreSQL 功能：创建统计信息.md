# 10 个你不知道的 PostgreSQL 功能：创建统计信息 - 文章 - 伯乐在线
原文出处： [Samay Sharma](https://www.citusdata.com/blog/2018/03/06/postgres-planner-and-its-usage-of-statistics/)   译文出处：[开源中国](https://www.oschina.net/translate/postgres-planner-and-its-usage-of-statistics)
如果你曾使用 Postgres 做过一些性能优化，你或许已经使用过 EXPLAIN 。EXPLAIN 向你展示了 PostgreSQL planner 为提供的语句生成的执行计划。它说明了语句涉及到的表将会使用顺序扫描、索引扫描等方式进行扫描，在使用多表的情况下将会使用连接算法。但是， Postgres 是如何产生这些规划的？
决定使用哪种规划的一个非常重要的输入是 planner 收集到的[数据统计](https://www.postgresql.org/docs/10/static/planner-stats.html)。这些统计的数据能够使 planner 评估执行规划的某一部分会返回多少行，继而影响到使用哪一种规划或连接算法。它们主要是通过运行 ANALYZE 或 VACUUM（和一些 DDL 命令，比如说 CREATE INDEX )来采集或更新的。
这些统计信息由 planner 存储在 [pg_class](https://www.postgresql.org/docs/9.3/static/catalog-pg-class.html) 和 [pg_statistics](https://www.postgresql.org/docs/10/static/catalog-pg-statistic.html) 中。Pg_class 基本上存储了每个表和索引中的条目总数，以及它们所占用的磁盘块数。Pg_statistic 存储关于每列的统计信息，例如哪些列的 % 值为 nul l，哪些是最常见的值，直方图边界等。你可以查看下面的示例，以了解 Postgres 在下表中为 col1 收集的统计信息类型。下面的查询输出展示了 planner（正确地）预估表中列 col1 中有 1000 个不同的值，并且还对最常见的值、频率等进行了其他预估。
请注意，我们已经查询了 [pg_stats](https://www.postgresql.org/docs/10/static/view-pg-stats.html)（一个拥有更多可读版本的列统计信息的视图）。


```
CREATE TABLE tbl (                                                                        
    col1 int,                                                                             
    col2 int                                                                              
);                                                                                        
INSERT INTO tbl SELECT i/10000, i/100000                                                  
FROM generate_series (1,10000000) s(i);                                                   
ANALYZE tbl;                                     
select * from pg_stats where tablename = 'tbl' and attname = 'col1';
-[ RECORD 1 ]----------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
schemaname             | public
tablename              | tbl
attname                | col1
inherited              | f
null_frac              | 0
avg_width              | 4
n_distinct             | 1000
most_common_vals       | {318,564,596,...}
most_common_freqs      | {0.00173333,0.0017,0.00166667,0.00156667,...}
histogram_bounds       | {0,8,20,30,39,...}
correlation            | 1
most_common_elems      | 
most_common_elem_freqs | 
elem_count_histogram   |
```
**单列统计数据不足时**
这些单列统计信息可帮助 planner 估算你的条件选择性（这是 planner 用来估算[索引扫描](https://www.postgresql.org/docs/10/static/row-estimation-examples.html)将选择多少行的内容）。 当查询中存在多个条件时，planner 假定列（或 where 子句条件）彼此独立。 当列相互关联或相互依赖并导致 planner 低估或高估这些条件将返回的行数时，就不适用。
我们来看下面的几个例子。 为了使查询计划易于阅读，我们通过设置 max_parallel_workers_per_gather  为 0 来关闭每个查询的并行性：


```
EXPLAIN ANALYZE SELECT * FROM tbl where col1 = 1;                            
                                                QUERY PLAN                                                 
-----------------------------------------------------------------------------------------------------------
 Seq Scan on tbl  (cost=0.00..169247.80 rows=9584 width=8) (actual time=0.641..622.851 rows=10000 loops=1)
   Filter: (col1 = 1)
   Rows Removed by Filter: 9990000
 Planning time: 0.051 ms
 Execution time: 623.185 ms
(5 rows)
```
正如你看到的那样，planner 估计 col1 的值为 1 的行数是 9584 ，而查询返回的实际行数是 10000 ，所以相当准确。
当你在 column 1 和 column 2 都包含过滤器时会发生什么情况。


```
EXPLAIN ANALYZE SELECT * FROM tbl where col1 = 1 and col2 = 0;                            
                                                QUERY PLAN                                                
----------------------------------------------------------------------------------------------------------
 Seq Scan on tbl  (cost=0.00..194248.69 rows=100 width=8) (actual time=0.640..630.130 rows=10000 loops=1)
   Filter: ((col1 = 1) AND (col2 = 0))
   Rows Removed by Filter: 9990000
 Planning time: 0.072 ms
 Execution time: 630.467 ms
(5 rows)
```
planner 的估计减少了100倍！ 让我们试着理解为什么发生这种情况。
第一个列的选择性约为 0.001（1/1000），第二个列的选择性为 0.01（1/100）。 要计算将由这两个“独立”条件过滤的行数，planner 会将它们的选择性相乘。 所以，我们得到：
选择性= 0.001 * 0.01 = 0.00001。
当它乘以我们在表中的行数即 10000000 时，我们得到 100。这就是 planner 对 100 的估计值的来源。 但是，这些列不是独立的，那么我们如何告知 planner ？
**在 PostgreSQL 中创建统计信息**
在 Postgres 10 之前，没有一种简易的方式去告诉 planner 采集捕捉列之间关系的数据统计。但是， Postgres 10 有一个新特性正好解决了这个问题，可以使用 [CREATE STATISTICS](https://www.postgresql.org/docs/10/static/sql-createstatistics.html) 来创建扩展统计的对象，告诉服务器去采集这些有意思的相关列的额外的统计信息。
**函数依赖统计**
回到我们先前评估的问题，col2 的值仅仅是 col1/10 。在数据库的术语中，我们会说 col2 是函数依赖于 col1 ，也就是说，col1 的值足以决定 col2 的值，并且不存在有两行数据拥有相同的 col1 值的同时有不同的 col2 值。因此，在 col2 列上的第二个过滤筛选并没有移除任何行！但是，planner 捕捉到了足够的统计信息去知道这件事情。
让我们来创建一个统计对象去捕获这些列和运行分析（ANALYZE）所依赖的函数统计。


```
CREATE STATISTICS s1 (dependencies) on col1, col2 from tbl; 
ANALYZE tbl;
```
让我们来看看现在的计划是怎么来的。


```
EXPLAIN ANALYZE SELECT * FROM tbl where col1 = 1 and col2 = 0;                            
                                                QUERY PLAN                                                 
-----------------------------------------------------------------------------------------------------------
 Seq Scan on tbl  (cost=0.00..194247.76 rows=9584 width=8) (actual time=0.638..629.741 rows=10000 loops=1)
   Filter: ((col1 = 1) AND (col2 = 0))
   Rows Removed by Filter: 9990000
 Planning time: 0.115 ms
 Execution time: 630.076 ms
(5 rows)
```
很好！让我们看一下对计划的测量。


```
SELECT stxname, stxkeys, stxdependencies                                                  
  FROM pg_statistic_ext                                                                   
  WHERE stxname = 's1';   
stxname | stxkeys |   stxdependencies    
---------+---------+----------------------
 s1      | 1 2     | {"1 => 2": 1.000000}
(1 row)
```
看这里，我们可以看到， Postgres 意识到 col1 完全决定 col2 ，因此用系数1来捕获这些信息。现在，所有的查询都过滤这些列之后，计划将会得到更好的评估。
**ndistinct 统计**
函数依赖是你可以在列之间捕获的一种关系。 你可以捕获的另一种统计信息是一组列的不同值。 我们之前指出，planner 可以获取每列不同值的统计数字，但再次合并多列时，这些统计数据往往是错误的。
这些不好的数据是在什么时候影响我们的呢？ 下面来看一个例子。


```
EXPLAIN ANALYZE SELECT col1,col2,count(*) from tbl group by col1, col2;                   
                                                         QUERY PLAN                                                          
-----------------------------------------------------------------------------------------------------------------------------
 GroupAggregate  (cost=1990523.20..2091523.04 rows=100000 width=16) (actual time=2697.246..4470.789 rows=1001 loops=1)
   Group Key: col1, col2
   ->  Sort  (cost=1990523.20..2015523.16 rows=9999984 width=8) (actual time=2695.498..3440.880 rows=10000000 loops=1)
         Sort Key: col1, col2
         Sort Method: external sort  Disk: 176128kB
         ->  Seq Scan on tbl  (cost=0.00..144247.84 rows=9999984 width=8) (actual time=0.008..665.689 rows=10000000 loops=1)
 Planning time: 0.072 ms
 Execution time: 4494.583 ms
```
聚合行时，Postgres 选择做散列聚合或组合。 如果它认为散列表合适，则选择散列聚合，否则它会选择对所有行进行排序，然后按照 col1、col2 对它们进行分组。
现在，planner 估计组的数量（等于 col1、col2 的不同值的数量）将为 100000。它预计到它没有足够的 work_mem 将该散列表存储在内存中。 因此，它使用基于磁盘的排序来运行该查询。 但是，正如在查询计划中所看到的那样，实际行数仅为 1001。也许，我们有足够的内存来执行哈希聚合。
让 planner 去捕获 n_distinct 统计信息，重新运行查询并找出结果。


```
CREATE STATISTICS s2 (ndistinct) on col1, col2 from tbl;                                  
ANALYZE tbl;
EXPLAIN ANALYZE SELECT col1,col2,count(*) from tbl group by col1, col2;                   
                                                      QUERY PLAN                                                       
-----------------------------------------------------------------------------------------------------------------------
 HashAggregate  (cost=219247.63..219257.63 rows=1000 width=16) (actual time=2431.767..2431.928 rows=1001 loops=1)
   Group Key: col1, col2
   ->  Seq Scan on tbl  (cost=0.00..144247.79 rows=9999979 width=8) (actual time=0.008..643.488 rows=10000000 loops=1)
 Planning time: 0.129 ms
 Execution time: 2432.010 ms
(5 rows)
```
可以看到，现在的估算精度更高了（即 1000 ），查询速度也提高了2倍左右。 通过运行下面的查询，我们可以看到 planner 学到了什么。


```
SELECT stxkeys AS k, stxndistinct AS nd                                                   
  FROM pg_statistic_ext                                                                   
  WHERE stxname = 's2'; 
  k  |       nd       
-----+----------------
 1 2 | {"1, 2": 1000}
```
**现实影响**
在实际的生产模式中，你总是会有某些与数据库不知道的相互依赖或关系的列。 以下是我们与 [Citus](https://www.citusdata.com/product/) 客户见过的一些例子：
- 有月份，季度和年份的列，因为你希望在报告中显示按所有人分组的统计信息。
- 地理层次之间的关系。 例如。 具有国家，州和城市的列，并由它们来过滤/分组。
这里的例子仅仅是在数据集中只有 10M 行的情况，并且我们已经看到，在存在相关列的情况下，使用 CREATE 统计信息可显着改善查询计划，并显示性能改进。在 Citus 使用案例中，我们有[客户](https://www.citusdata.com/customers/)存储数十亿行数据，糟糕查询计划的影响可能非常严重。在上述示例中，当 planner 选择了一个糟糕的查询计划时，我们不得不为 10M 行做一个基于磁盘的分类。想象一下如果是数十亿行，那会有多糟糕。
