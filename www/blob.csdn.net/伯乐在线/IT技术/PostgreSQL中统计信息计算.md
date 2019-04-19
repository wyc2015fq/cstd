# PostgreSQL中统计信息计算 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [Rings](http://www.jobbole.com/members/Rings) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
## #0. 前言背景.
在PostgreSQL中当我们查询引擎在生成执行计划时候，一个重要的问题是如何才能使得我们所生成的执行计划是一个最优的结果。考虑到执行计划最终需要与磁盘进行IO操作以读取相应的数据，因此如果使得我们所生成的执行计划其代价最优，这便是查询引擎在查询优化过程中需要考虑的一个问题。而执行代价又包括两个方面：（1）计算资源；（2）IO资源。其中：计算资源主要涉及到CPU的计算时间，而IO资源主要包括磁盘的访问资源等（这两类资源具体包括哪些，在这里就不在详述了。）
对于一条查询语句来说，其最终会落到磁盘的读写上面，因此衡量一个查询引擎的好坏的标准是：是否可以将IO开销做到最少。例如：对于查询语句 SELECT * FROM tenk1 WHERE stringu1 = ‘xxx’; 其有无索引对于执行计划有着不同的影响。
## #1. 统计信息
(1) 元数据表 pg_statistic 和 pg_stats
对于pg_statistic里面保存了关于某个表的相关统计信息，比如：该表的所有的Page数据和tuples数据量。 为了防止为授权用户能够修改其中的数据（pg_statistic只能由管理员来访问，而pg_stat是视图可由非管理员用户查看数据。）。当planner需要统计数据时候其会从该表中获取，而并不是执行真正的表扫描，前提是对该表执行了analyze操作后。 relpages, reltuples， 当relpages发生变化后，在不执行analyze的情况会根据上一次relpage与reltuples的比例关系进行适当的变更。
对于selectivity 即：where子句中的选择率(该值描述了在一张表里面，对于某个值的区分度)。该选择由 pg_operator中来进行查找获取相应的selectivity。其由pg_operator中的oprrest属性所对应的函数来描述。 然后由该函数从pg_statistic中取出条件属性相应的柱状图统计进行(historgram)。
例如：

PgSQL
```
SELECT histogram_bounds FROM pg_stats
WHERE tablename='tenk1' AND attname='unique1';
histogram_bounds
------------------------------------------------------
{0,993,1997,3050,4040,5036,5957,7057,8029,9016,9995}
```
对于语句：

PgSQL
```
SELECT * FROM tenk1 WHERE unique1 < 1000;
```
其中1000在第二个桶里面，[993 1997]，假设在在每个桶里的数据是线性分布的。因此我们可以计算 < 1000时候的分布概率。
因此相应的概率分布为：
selectivity = (1 + (1000 – bucket[2].min)/(bucket[2].max – bucket[2].min))/num_buckets
= (1 + (1000 – 993)/(1997 – 993))/10
= 0.100697
相应的计算公式 选择率的：
[Num(No. (X-1)) + (N- X.min)/(X.max-X.min)]/ NumOfBucket
然后相应的满足条件的记录数： rows = rel_cardinality * selectivity ， 其中 rel_cardinality是该表中的记录基数。即表中总共有多少记录数。 对于等号，其柱状图的估计对于等号，其有效性不如其他操作。此时，我们使用最常用值来进行估计。在pg_stats中，我们根据表名和属性名来查询出 most_common_freqs, most_common_vals来。
例如：
“{1,100,10}”;”
对应的freqs
{0.97588,0.0126532,0.0106762}”
例如：对于SELECT * FROM tenk1 WHERE stringu1 = ‘xxx’; 查询语句， 当我们常量 xxx 属于 most common value时候，我们就可以使用该值对应的most common freqs作为该值的selectivity，但是当该值不在mcv列表里面时候，此时的selectivity的估算就不能简单的使用mcv列表里面的值。 此时，需要一个新的计算方式来完成对非条件不在mcv列表中的情况。我们采用的是将现有的知识进行想融合的方式来进行选择率的估计。
selectivity = (1 – sum(mvf))/(num_distinct – num_mcv)
该中方式下，将所有的most common freqs求和后, 然后计算其与1直接的差值，由此我们可以知道：在均匀分布的情况下，如果条件为非mvc情况下，其most common freqs的值就为 （1- sum (msf)）。对于该值的选择率，我们此概率与整个最优的值的分布之间的比例。
对于之前的 SELECT * FROM tenk1 WHERE unique1 < 1000; 过于简单。当条件列为mcv列表中的时候可以使用上述的计算方式，当时当条件列不在mcv列表中的时候，对于非唯一的列，其可以是柱状图或者mcv列表，且在柱状图中其并未包括哪些在mcv列表中所表示的那些元组数量。当此种情况下，我们将选择条件，应用于mcv列表中的所以值，并且将所有满足该条件的所有的mcf值相加。
例如：对于查询语句 SELECT * FROM tenk1 WHERE stringu1 < ‘IAAAAA’;
其相应的柱状图信息如下（我们可以从pg_stats进行查询获得）：

PgSQL
```
SELECT histogram_bounds FROM pg_stats WHERE tablename='tenk1' AND attname='stringu1';
 
histogram_bounds
--------------------------------------------------------------------------------
{AAAAAA,CQAAAA,FRAAAA,IBAAAA,KRAAAA,NFAAAA,PSAAAA,SGAAAA,VAAAAA,XLAAAA,ZZAAAA}
```
当对于条件中存在着连接操作时候的选择率的估计与之前的做法相似。
例如对于查询语句：

PgSQL
```
SELECT * FROM tenk1 t1, tenk2 t2 WHERE t1.unique1 < 50 AND t1.unique2 = t2.unique2;
```
首先会计算条件 t1.unique1 < 50的选择率，对于该条件的选择率的计算参照上面讨论。
在文件src/backend/optimizer/util/plancat.c中计算一个表的大小（在任何的where子句之前），计算子句的选择率的由src/backend/optimizer/path/clausesel.c，关于操作符的选择率的计算则由src/backend/utils/adt/selfuncs.c中的函数来完成。
当我们计算一个语句的选择率时候，如果该语句存在着子句时候，我们就将这些子句的选择率先计算出来，然后分别计算这些子句的选择率，然后将这些子句的选择相乘从而获得整个语句的选择率，前提条件是这些子句其有这相互独立的概率条件，这与我们在概率论中的，两个事件是相互独立的事件时候，我们计算该这两个事件的概率相似。但是在现实世界中，这些却是不容易满足的条件，因此我们需要根据合理的计算方式来计算一个语句的选择率。
当然，对于统计信息，由于其它是基于估算的基础上，因此必然会存在着一个不可回避的问题：该统计信息的准确性（或者说不确定性），以及由此不准确所带来的问题，导致查询引擎在基于该统计信息计算最优查询访问路径时候的偏差（更加准确的是查询计划的“漂移”问题）。
