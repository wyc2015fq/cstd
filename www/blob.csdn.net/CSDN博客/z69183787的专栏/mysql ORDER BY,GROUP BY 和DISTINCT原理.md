# mysql ORDER BY,GROUP BY 和DISTINCT原理 - z69183787的专栏 - CSDN博客
2017年01月16日 16:55:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1980
## **前言**
除了常规的Join语句之外，还有一类Query语句也是使用比较频繁的，那就是ORDERBY，GROUP BY以及DISTINCT这三类查询。考虑到这三类查询都涉及到数据的排序等操作，所以我将他们放在了一起，下面就针对这三类Query语句做基本的分析。
## **ORDER BY 的实现与优化**
在MySQL中，ORDERBY的实现有如下两种类型：
一种是通过有序索引而直接取得有序的数据，这样不用进行任何排序操作即可得到满足客户端要求的有序数据返回给客户端；
另外一种则需要通过MySQL的排序算法将存储引擎中返回的数据进行排序然后再将排序后的数据返回给客户端。
下面我们就针对这两种实现方式做一个简单的分析。首先分析一下第一种不用排序的实现方式。同样还是通过示例来说话吧：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
sky@localhost : example 09:48:41> EXPLAIN
-> SELECT m.id,m.subject,c.content
-> FROM group_message m,group_message_content c
-> WHERE m.group_id = 1 AND m.id = c.group_msg_id
-> ORDER BY m.user_id\G
*************************** 1. row ***************************
id: 1
select_type: SIMPLE
table: m
type: ref
possible_keys: PRIMARY,idx_group_message_gid_uid
key: idx_group_message_gid_uid
key_len: 4
ref: const
rows: 4
Extra: Using where
*************************** 2. row ***************************
id: 1
select_type: SIMPLE
table: c
type: ref
possible_keys: group_message_content_msg_id
key: group_message_content_msg_id
key_len: 4
ref: example.m.id
rows: 11
Extra:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
看看上面的这个Query语句，明明有ORDER BY user_id，为什么在执行计划中却没有排序操作呢？其实这里正是因为MySQL Query Optimizer选择了一个有序的索引来进行访问表中的数据（idx_group_message_gid_uid），这样，我们通过group_id的条件得到的数据已经是按照group_id和user_id进行排序的了。而虽然我们的排序条件仅仅只有一个user_id，但是我们的WHERE条件决定了返回数据的group_id全部一样，也就是说不管有没有根据group_id来进行排序，返回的结果集都是完全一样的。
我们可以通过如下的图示来描述整个执行过程：
![clip_image002](http://images.cnblogs.com/cnblogs_com/ggjucheng/201211/201211182239397812.jpg)
图中的TableA和TableB分别为上面Query中的group_message和gruop_message_content这两个表。
这种利用索引实现数据排序的方法是MySQL中实现结果集排序的最佳做法，可以完全避免因为排序计算所带来的资源消耗。所以，在我们优化Query语句中的ORDERBY的时候，尽可能利用已有的索引来避免实际的排序计算，可以很大幅度的提升ORDERBY操作的性能。在有些Query的优化过程中，即使为了避免实际的排序操作而调整索引字段的顺序，甚至是增加索引字段也是值得的。当然，在调整索引之前，同时还需要评估调整该索引对其他Query所带来的影响，平衡整体得失。
如果没有索引利用的时候，MySQL又如何来实现排序呢？这时候MySQL无法避免需要通过相关的排序算法来将存储引擎返回的数据进行排序运算了。下面我们再针对这种实现方式进行相应的分析。
在MySQL第二种排序实现方式中，必须进行相应的排序算法来实现数据的排序。MySQL目前可以通过两种算法来实现数据的排序操作。
取出满足过滤条件的用于排序条件的字段以及可以直接定位到行数据的行指针信息，在Sort Buffer中进行实际的排序操作，然后利用排好序之后的数据根据行指针信息返回表中取得客户端请求的其他字段的数据，再返回给客户端；
根据过滤条件一次取出排序字段以及客户端请求的所有其他字段的数据，并将不需要排序的字段存放在一块内存区域中，然后在SortBuffer中将排序字段和行指针信息进行排序，最后再利用排序后的行指针与存放在内存区域中和其他字段一起的行指针信息进行匹配合并结果集，再按照顺序返回给客户端。
上面第一种排序算法是MySQL一直以来就有的排序算法，而第二种则是从MySQL4.1版本才开始增加的改进版排序算法。第二种算法与第一种相比较，主要优势就是减少了数据的二次访问。在排序之后不需要再一次回到表中取数据，节省了IO操作。当然，第二种算法会消耗更多的内存，正是一种典型的通过内存空间换取时间的优化方式。下面我们同样通过一个实例来看看当MySQL不得不使用排序算法的时候的执行计划，仅仅只是更改一下排序字段：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
sky@localhost : example 10:09:06> explain
-> select m.id,m.subject,c.content
-> FROM group_message m,group_message_content c
-> WHERE m.group_id = 1 AND m.id = c.group_msg_id
-> ORDER BY m.subject\G
*************************** 1. row ***************************
id: 1
select_type: SIMPLE
table: m
type: ref
possible_keys: PRIMARY,idx_group_message_gid_uid
key: idx_group_message_gid_uid
key_len: 4
ref: const
rows: 4
Extra: Using where; Using filesort
*************************** 2. row ***************************
id: 1
select_type: SIMPLE
table: c
type: ref
possible_keys: group_message_content_msg_id
key: group_message_content_msg_id
key_len: 4
ref: example.m.id
rows: 11
Extra:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
大概一看，好像整个执行计划并没有什么区别啊？但是细心的读者朋友可能已经发现，在group_message表的Extra信息中，多了一个“Using filesort”的信息，实际上这就是MySQL Query Optimizer在告诉我们，他需要进行排序操作才能按照客户端的要求返回有序的数据。执行图示如下：
![clip_image002[5]](http://images.cnblogs.com/cnblogs_com/ggjucheng/201211/201211182239409581.jpg)
这里我们看到了，MySQL在取得第一个表的数据之后，先根据排序条件将数据进行了一次filesort，也就是排序操作。然后再利用排序后的结果集作为驱动结果集来通过Nested Loop Join访问第二个表。当然，大家不要误解，这个filesort并不是说通过磁盘文件进行排序，仅仅只是告诉我们进行了一个排序操作。
上面，我们看到了排序结果集来源仅仅只是单个表的比较简单的filesort操作。而在我们实际应用中，很多时候我们的业务要求可能并不是这样，可能需要排序的字段同时存在于两个表中，或者MySQL在经过一次Join之后才进行排序操作。这样的排序在MySQL中并不能简单的里利用Sort Buffer进行排序，而是必须先通过一个临时表将之前Join的结果集存放入临时表之后在将临时表的数据取到Sort Buffer中进行操作。下面我们通过再次更改排序要求来示例这样的执行计划，当我们选择通过group_message_content表上面的content字段来进行排序之后：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
sky@localhost : example 10:22:42> explain
-> select m.id,m.subject,c.content
-> FROM group_message m,group_message_content c
-> WHERE m.group_id = 1 AND m.id = c.group_msg_id
-> ORDER BY c.content\G
*************************** 1. row ***************************
id: 1
select_type: SIMPLE
table: m
type: ref
possible_keys: PRIMARY,idx_group_message_gid_uid
key: idx_group_message_gid_uid
key_len: 4
ref: const
rows: 4
Extra: Using temporary; Using filesort
*************************** 2. row ***************************
id: 1
select_type: SIMPLE
table: c
type: ref
possible_keys: group_message_content_msg_id
key: group_message_content_msg_id
key_len: 4
ref: example.m.id
rows: 11
Extra:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
这时候的执行计划中出现了“Using temporary”，正是因为我们的排序操作需要在两个表Join之后才能进行，下图展示了这个Query的执行过程：
![clip_image002[7]](http://images.cnblogs.com/cnblogs_com/ggjucheng/201211/201211182239425352.jpg)
首先是TableA和TableB进行Join，然后结果集进入临时表，再进行filesort，最后得到有序的结果集数据返回给客户端。
上面我们通过两个不同的示例展示了当MySQL无法避免要使用相应的排序算法进行排序操作的时候的实现原理。虽然在排序过程中所使用的排序算法有两种，但是两种排序的内部实现机制大体上差不多。
当我们无法避免排序操作的时候，我们又该如何来优化呢？很显然，我们应该尽可能让MySQL选择使用第二种算法来进行排序。这样可以减少大量的随机IO操作，很大幅度的提高排序工作的效率。
1.加大max_length_for_sort_data参数的设置；
在MySQL中，决定使用第一种老式的排序算法还是新的改进算法的依据是通过参数max_length_for_sort_data来决定的。当我们所有返回字段的最大长度小于这个参数值的时候，MySQL就会选择改进后的排序算法，反之，则选择老式的算法。所以，如果我们有充足的内存让MySQL存放需要返回的非排序字段的时候，可以加大这个参数的值来让MySQL选择使用改进版的排序算法。
2. 去掉不必要的返回字段；
当我们的内存并不是很充裕的时候，我们不能简单的通过强行加大上面的参数来强迫MySQL去使用改进版的排序算法，因为如果那样可能会造成MySQL不得不将数据分成很多段然后进行排使用序，这样的结果可能会得不偿失。在这种情况下，我们就需要去掉不必要的返回字段，让我们的返回结果长度适应max_length_for_sort_data参数的限制。
3.增大sort_buffer_size参数设置；
增大sort_buffer_size并不是为了让MySQL可以选择改进版的排序算法，而是为了让MySQL可以尽量减少在排序过程中对需要排序的数据进行分段，因为这样会造成MySQL不得不使用临时表来进行交换排序。
## **GROUP BY 的实现与优化**
由于GROUP BY实际上也同样需要进行排序操作，而且与ORDER BY相比，GROUP BY主要只是多了排序之后的分组操作。当然，如果在分组的时候还使用了其他的一些聚合函数，那么还需要一些聚合函数的计算。所以，在GROUP BY的实现过程中，与ORDERBY一样也可以利用到索引。
在MySQL中，GROUP BY的实现同样有多种（三种）方式，其中有两种方式会利用现有的索引信息来完成GROUP BY，另外一种为完全无法使用索引的场景下使用。下面我们分别针对这三种实现方式做一个分析。
1. 使用松散（Loose）索引扫描实现GROUP BY
何谓松散索引扫描实现GROUP BY呢？实际上就是当MySQL完全利用索引扫描来实现GROUP BY的时候，并不需要扫描所有满足条件的索引键即可完成操作得出结果。
下面我们通过一个示例来描述松散索引扫描实现GROUP BY，在示例之前我们需要首先调整一下group_message表的索引，将gmt_create字段添加到group_id和user_id字段的索引中：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
sky@localhost : example 08:49:45> create index idx_gid_uid_gc
-> on group_message(group_id,user_id,gmt_create);
Query OK, rows affected (0.03 sec)
Records: 96 Duplicates: 0 Warnings: 0
sky@localhost : example 09:07:30> drop index idx_group_message_gid_uid
-> on group_message;
Query OK, 96 rows affected (0.02 sec)
Records: 96 Duplicates: 0 Warnings: 0
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
然后再看如下Query的执行计划：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
sky@localhost : example 09:26:15> EXPLAIN
-> SELECT user_id,max(gmt_create)
-> FROM group_message
-> WHERE group_id < 10
-> GROUP BY group_id,user_id\G
*************************** 1. row ***************************
id: 1
select_type: SIMPLE
table: group_message
type: range
possible_keys: idx_gid_uid_gc
key: idx_gid_uid_gc
key_len: 8
ref: NULL
rows: 4
Extra: Using where; Using index for group-by
1 row in set (0.00 sec)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
我们看到在执行计划的Extra信息中有信息显示“Using index for group-by”，实际上这就是告诉我们，MySQLQueryOptimizer通过使用松散索引扫描来实现了我们所需要的GROUP BY操作。
下面这张图片描绘了扫描过程的大概实现：要利用到松散索引扫描实现GROUP BY，需要至少满足以下几个条件：
![clip_image002[9]](http://images.cnblogs.com/cnblogs_com/ggjucheng/201211/20121118223943783.jpg)
GROUP BY 条件字段必须在同一个索引中最前面的连续位置；
在使用GROUP BY的同时，只能使用MAX和MIN这两个聚合函数；
如果引用到了该索引中GROUP BY条件之外的字段条件的时候，必须以常量形式存在；
为什么松散索引扫描的效率会很高？
因为在没有WHERE子句，也就是必须经过全索引扫描的时候，松散索引扫描需要读取的键值数量与分组的组数量一样多，也就是说比实际存在的键值数目要少很多。而在WHERE子句包含范围判断式或者等值表达式的时候，松散索引扫描查找满足范围条件的每个组的第1个关键字，并且再次读取尽可能最少数量的关键字。
2. 使用紧凑（Tight）索引扫描实现GROUP BY
紧凑索引扫描实现GROUP BY和松散索引扫描的区别主要在于他需要在扫描索引的时候，读取所有满足条件的索引键，然后再根据读取恶的数据来完成GROUP BY操作得到相应结果。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
sky@localhost : example 08:55:14> EXPLAIN
-> SELECT max(gmt_create)
-> FROM group_message
-> WHERE group_id = 2
-> GROUP BY user_id\G
*************************** 1. row ***************************
id: 1
select_type: SIMPLE
table: group_message
type: ref
possible_keys: idx_group_message_gid_uid,idx_gid_uid_gc
key: idx_gid_uid_gc
key_len: 4
ref: const
rows: 4
Extra: Using where; Using index
1 row in set (0.01 sec)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
这时候的执行计划的Extra信息中已经没有“Using index for group-by”了，但并不是说MySQL的GROUP BY操作并不是通过索引完成的，只不过是需要访问WHERE条件所限定的所有索引键信息之后才能得出结果。这就是通过紧凑索引扫描来实现GROUP BY的执行计划输出信息。
下面这张图片展示了大概的整个执行过程：
![clip_image002[11]](http://images.cnblogs.com/cnblogs_com/ggjucheng/201211/201211182239443390.jpg)
在MySQL中，MySQL Query Optimizer首先会选择尝试通过松散索引扫描来实现GROUP BY操作，当发现某些情况无法满足松散索引扫描实现GROUP BY的要求之后，才会尝试通过紧凑索引扫描来实现。
当GROUP BY条件字段并不连续或者不是索引前缀部分的时候，MySQL Query Optimizer无法使用松散索引扫描，设置无法直接通过索引完成GROUP BY操作，因为缺失的索引键信息无法得到。但是，如果Query语句中存在一个常量值来引用缺失的索引键，则可以使用紧凑索引扫描完成GROUP BY操作，因为常量填充了搜索关键字中的“差距”，可以形成完整的索引前缀。这些索引前缀可以用于索引查找。而如果需要排序GROUP BY结果，并且能够形成索引前缀的搜索关键字，MySQL还可以避免额外的排序操作，因为使用有顺序的索引的前缀进行搜索已经按顺序检索到了所有关键字。3.
 使用临时表实现GROUP BY
MySQL在进行GROUP BY操作的时候要想利用所有，必须满足GROUP BY的字段必须同时存放于同一个索引中，且该索引是一个有序索引（如Hash索引就不能满足要求）。而且，并不只是如此，是否能够利用索引来实现GROUP BY还与使用的聚合函数也有关系。
前面两种GROUP BY的实现方式都是在有可以利用的索引的时候使用的，当MySQL Query Optimizer无法找到合适的索引可以利用的时候，就不得不先读取需要的数据，然后通过临时表来完成GROUP BY操作。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
sky@localhost : example 09:02:40> EXPLAIN
-> SELECT max(gmt_create)
-> FROM group_message
-> WHERE group_id > 1 and group_id < 10
-> GROUP BY user_id\G
*************************** 1. row ***************************
id: 1
select_type: SIMPLE
table: group_message
type: range
possible_keys: idx_group_message_gid_uid,idx_gid_uid_gc
key: idx_gid_uid_gc
key_len: 4
ref: NULL
rows: 32
Extra: Using where; Using index; Using temporary; Using filesort
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
这次的执行计划非常明显的告诉我们MySQL通过索引找到了我们需要的数据，然后创建了临时表，又进行了排序操作，才得到我们需要的GROUP BY结果。整个执行过程大概如下图所展示：
![clip_image002[13]](http://images.cnblogs.com/cnblogs_com/ggjucheng/201211/201211182239465191.jpg)
当MySQL Query Optimizer发现仅仅通过索引扫描并不能直接得到GROUP BY的结果之后，他就不得不选择通过使用临时表然后再排序的方式来实现GROUP BY了。
在这样示例中即是这样的情况。group_id并不是一个常量条件，而是一个范围，而且GROUP BY字段为user_id。所以MySQL无法根据索引的顺序来帮助GROUP BY的实现，只能先通过索引范围扫描得到需要的数据，然后将数据存入临时表，然后再进行排序和分组操作来完成GROUP BY。
对于上面三种MySQL处理GROUP BY的方式，我们可以针对性的得出如下两种优化思路：
1.尽可能让MySQL可以利用索引来完成GROUP BY操作，当然最好是松散索引扫描的方式最佳。在系统允许的情况下，我们可以通过调整索引或者调整Query这两种方式来达到目的；
2.当无法使用索引完成GROUP BY的时候，由于要使用到临时表且需要filesort，所以我们必须要有足够的sort_buffer_size来供MySQL排序的时候使用，而且尽量不要进行大结果集的GROUP BY操作，因为如果超出系统设置的临时表大小的时候会出现将临时表数据copy到磁盘上面再进行操作，这时候的排序分组操作性能将是成数量级的下降；
至于如何利用好这两种思路，还需要大家在自己的实际应用场景中不断的尝试并测试效果，最终才能得到较佳的方案。此外，在优化GROUP BY的时候还有一个小技巧可以让我们在有些无法利用到索引的情况下避免filesort操作，也就是在整个语句最后添加一个以null排序（ORDER BY null）的子句，大家可以尝试一下试试看会有什么效果。
## **DISTINCT 的实现与优化**
DISTINCT实际上和GROUP BY的操作非常相似，只不过是在GROUP BY之后的每组中只取出一条记录而已。所以，DISTINCT的实现和GROUP BY的实现也基本差不多，没有太大的区别。同样可以通过松散索引扫描或者是紧凑索引扫描来实现，当然，在无法仅仅使用索引即能完成DISTINCT的时候，MySQL只能通过临时表来完成。但是，和GROUP BY有一点差别的是，DISTINCT并不需要进行排序。也就是说，在仅仅只是DISTINCT操作的Query如果无法仅仅利用索引完成操作的时候，MySQL会利用临时表来做一次数据的“缓存”，但是不会对临时表中的数据进行filesort操作。当然，如果我们在进行DISTINCT的时候还使用了GROUP
 BY并进行了分组，并使用了类似于MAX之类的聚合函数操作，就无法避免filesort了。
下面我们就通过几个简单的Query示例来展示一下DISTINCT的实现。
1.首先看看通过松散索引扫描完成DISTINCT的操作：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
sky@localhost :  example 11:03:41> EXPLAIN SELECT  DISTINCT group_id
->  FROM  group_message\G
*************************** 1.  row  ***************************
id:  1
SELECT_type: SIMPLE
table: group_message
type: range
possible_keys: NULL
key:  idx_gid_uid_gc
key_len: 4
ref:  NULL
rows: 10
Extra: Using  index for  group-by
1  row  in  set  (0.00 sec)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
我们可以很清晰的看到，执行计划中的Extra信息为“Using index for group-by”，这代表什么意思？为什么我没有进行GROUP BY操作的时候，执行计划中会告诉我这里通过索引进行了GROUP BY呢？其实这就是于DISTINCT的实现原理相关的，在实现DISTINCT的过程中，同样也是需要分组的，然后再从每组数据中取出一条返回给客户端。而这里的Extra信息就告诉我们，MySQL利用松散索引扫描就完成了整个操作。当然，如果MySQL Query Optimizer要是能够做的再人性化一点将这里的信息换成“Using
 index for distinct”那就更好更容易让人理解了，呵呵。
2.  我们再来看看通过紧凑索引扫描的示例：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
sky@localhost : example 11:03:53> EXPLAIN SELECT DISTINCT user_id
-> FROM group_message
-> WHERE group_id = 2\G
*************************** 1. row ***************************
id: 1
SELECT_type: SIMPLE
table: group_message
type: ref
possible_keys: idx_gid_uid_gc
key: idx_gid_uid_gc
key_len: 4
ref: const
rows: 4
Extra: Using WHERE; Using index
1 row in set (0.00 sec)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
这里的显示和通过紧凑索引扫描实现GROUP BY也完全一样。实际上，这个Query的实现过程中，MySQL会让存储引擎扫描group_id=2的所有索引键，得出所有的user_id，然后利用索引的已排序特性，每更换一个user_id的索引键值的时候保留一条信息，即可在扫描完所有gruop_id=2的索引键的时候完成整个DISTINCT操作。
3.下面我们在看看无法单独使用索引即可完成DISTINCT的时候会是怎样：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
sky@localhost : example 11:04:40> EXPLAIN SELECT DISTINCT user_id
-> FROM group_message
-> WHERE group_id > 1 AND group_id < 10\G
*************************** 1. row ***************************
id: 1
SELECT_type: SIMPLE
table: group_message
type: range
possible_keys: idx_gid_uid_gc
key: idx_gid_uid_gc
key_len: 4
ref: NULL
rows: 32
Extra: Using WHERE; Using index; Using temporary
1 row in set (0.00 sec)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
当MySQL无法仅仅依赖索引即可完成DISTINCT操作的时候，就不得不使用临时表来进行相应的操作了。但是我们可以看到，在MySQL利用临时表来完成DISTINCT的时候，和处理GROUP BY有一点区别，就是少了filesort。实际上，在MySQL的分组算法中，并不一定非要排序才能完成分组操作的，这一点在上面的GROUP BY优化小技巧中我已经提到过了。实际上这里MySQL正是在没有排序的情况下实现分组最后完成DISTINCT操作的，所以少了filesort这个排序操作。
4.最后再和GROUP BY结合试试看：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
sky@localhost : example 11:05:06> EXPLAIN SELECT DISTINCT max(user_id)
-> FROM group_message
-> WHERE group_id > 1 AND group_id < 10
-> GROUP BY group_id\G
*************************** 1. row ***************************
id: 1
SELECT_type: SIMPLE
table: group_message
type: range
possible_keys: idx_gid_uid_gc
key: idx_gid_uid_gc
key_len: 4
ref: NULL
rows: 32
Extra: Using WHERE; Using index; Using temporary; Using filesort
1 row in set (0.00 sec)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
最后我们再看一下这个和GROUP BY一起使用带有聚合函数的示例，和上面第三个示例相比，可以看到已经多了filesort排序操作了，因为我们使用了MAX函数的缘故。
对于DISTINCT的优化，和GROUP BY基本上一致的思路，关键在于利用好索引，在无法利用索引的时候，确保尽量不要在大结果集上面进行DISTINCT操作，磁盘上面的IO操作和内存中的IO操作性能完全不是一个数量级的差距。
## **总结**
本章重点介绍了MySQL Query语句相关的性能调优的部分思路和方法，也列举了部分的示例，希望能够帮助读者朋友在实际工作中开阔一点点思路。虽然本章涉及到的内容包含了最初的索引设计，到编写高效Query语句的一些原则，以及最后对语句的调试，但Query语句的调优远不只这些内容。很多的调优技巧，只有到在实际的调优经验中才会真正体会，真正把握其精髓。所以，希望各位读者朋友能多做实验，以理论为基础，以事实为依据，只有这样，才能不断提升自己对Query调优的深入认识。
转自 《mysql性能调优与架构设计》
