# MySQL优化GROUP BY－松散索引扫描与紧凑索引扫描 - z69183787的专栏 - CSDN博客
2016年11月29日 16:17:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：431
个人分类：[Mysql-基础知识](https://blog.csdn.net/z69183787/article/category/3258085)
官网：[http://dev.mysql.com/doc/refman/5.5/en/group-by-optimization.html](http://dev.mysql.com/doc/refman/5.5/en/group-by-optimization.html)
       满足GROUP BY子句的最一般的方法是扫描整个表并创建一个新的临时表，表中每个组的所有行应为连续的，然后使用该临时表来找到组并应用累积函数(如果有)。在某些情况中，[MySQL](http://lib.csdn.net/base/mysql)能够做得更好，即通过索引访问而不用创建临时表。
       为GROUP BY使用索引的最重要的前提条件是所有GROUP BY列引用同一索引的属性，并且索引按顺序保存其关键字。是否用索引访问来代替临时表的使用还取决于在查询中使用了哪部分索引、为该部分指定的条件，以及选择的累积函数。
       由于GROUP BY 实际上也同样会进行排序操作，而且与ORDER BY 相比，GROUP BY 主要只是多了排序之后的分组操作。当然，如果在分组的时候还使用了其他的一些聚合函数，那么还需要一些聚合函数的计算。所以，在GROUP BY 的实现过程中，与 ORDER BY 一样也可以利用到索引。在MySQL 中，GROUP BY 的实现同样有多种（三种）方式，其中有两种方式会利用现有的索引信息来完成 GROUP BY，另外一种为完全无法使用索引的场景下使用。下面我们分别针对这三种实现方式做一个分析。
### 1、使用松散索引扫描（Loose index scan）实现 GROUP BY
对“松散索引扫描”的定义，本人看了很多网上的介绍，都不甚明白。在此逻列如下：
定义1：松散索引扫描，实际上就是当 MySQL 完全利用索引扫描来实现 GROUP BY 的时候，并不需要扫描所有满足条件的索引键即可完成操作得出结果。
定义2：优化Group By最有效的办法是当可以直接使用索引来完全获取需要group的字段。使用这个访问方法时，MySQL使用对关键字排序的索引的类型（比如BTREE索引）。这使得索引中用于group的字段不必完全涵盖WHERE条件中索引对应的key。由于只包含索引中关键字的一部分，因此称为松散的索引扫描。
**意思是索引中用于group的字段，没必要包含多列索引的全部字段。例如：有一个索引idx(c1,c2,c3)，那么group by c1、group by c1,c2这样c1或c1、c2都只是索引idx的一部分。要注意的是，索引中用于group的字段必须符合索引的“最左前缀”原则。group by c1,c3是不会使用松散的索引扫描的**
例如：
explain
SELECT group_id,gmt_create
FROM group_message
WHERE user_id>1
GROUP BY group_id,gmt_create;
本人理解“定义2”的例子说明
有一个索引idx(c1,c2,c3)
SELECT c1, c2 FROM t1 WHERE c1 < const GROUP BY c1, c2;
索引中用于group的字段为c1,c2
不必完全涵盖WHERE条件中索引对应的key（where条件中索引，即为c1；c1对应的key，即为idx）
索引中用于group的字段(c1,c2)只包含索引中关键字(c1,c2,c3)的一部分，因此称为松散的索引扫描。
要利用到松散索引扫描实现GROUP BY，需要至少满足以下几个条件：
**◆ 查询针对一个单表**
**◆ GROUP BY 条件字段必须在同一个索引中最前面的连续位置；GROUP BY包括索引的第1个连续部分(如果对于GROUP BY，查询有一个DISTINCT子句，则所有DISTINCT的属性指向索引开头)。**
**◆ 在使用GROUP BY 的同时，如果有聚合函数，只能使用 MAX 和 MIN 这两个聚合函数，并且它们均指向相同的列。**
**◆ 如果引用(where条件中)到了该索引中GROUP BY 条件之外的字段条件的时候，必须以常量形式存在，但MIN()或MAX() 函数的参数例外；   或者说：索引的任何其它部分（除了那些来自查询中引用的GROUP BY）必须为常数(也就是说，必须按常量数量来引用它们)，但MIN()或MAX() 函数的参数例外。**
**补充：如果sql中有where语句，且select中引用了该索引中GROUP BY 条件之外的字段条件的时候，where中这些字段要以常量形式存在。**
**◆ 如果查询中有where条件，则条件必须为索引，不能包含非索引的字段**
**松散索引扫描**
explain
SELECT group_id,user_id
FROM group_message
WHERE group_id between 1 and 4
GROUP BY group_id,user_id;
松散索引扫描
explain
SELECT group_id,user_id
FROM group_message
WHERE user_id>1 and group_id=1
GROUP BY group_id,user_id;
非松散索引扫描
explain
SELECT group_id,user_id
FROM group_message
WHERE abc=1
GROUP BY group_id,user_id;
非松散索引扫描
explain
SELECT group_id,user_id
FROM group_message
WHERE user_id>1 and abc=1
GROUP BY group_id,user_id;
**松散索引扫描，此类查询的EXPLAIN输出显示Extra列的Using index for group-by**
下面的查询提供该类的几个例子，假定表t1(c1,c2,c3,c4)有一个索引idx(c1，c2，c3)：
SELECT c1, c2 FROM t1 GROUP BY c1, c2;
SELECT DISTINCT c1, c2 FROM t1;
SELECT c1, MIN(c2) FROM t1 GROUP BY c1;
SELECT c1, c2 FROM t1 WHERE c1 < const GROUP BY c1, c2;
SELECT MAX(c3), MIN(c3), c1, c2 FROM t1 WHERE c2 > const GROUP BY c1, c2;
SELECT c2 FROM t1 WHERE c1 < const GROUP BY c1, c2;
SELECT c1, c2 FROM t1 WHERE c3 = const GROUP BY c1, c2；
-------------------------------------------------------------------------------------------------
由于上述原因，不能用该快速选择方法执行下面的查询：
1、除了MIN()或MAX()，还有其它累积函数，例如：
     SELECT c1, SUM(c2) FROM t1 GROUP BY c1;
2、GROUP BY子句中的域不引用索引开头，如下所示：
     SELECT c1,c2 FROM t1 GROUP BY c2, c3;
3、查询引用了GROUP BY部分后面的关键字的一部分，并且没有等于常量的等式，例如：
     SELECT c1,c3 FROM t1 GROUP BY c1, c2；
这个例子中，引用到了c3（c3必须为组合索引中的一个），因为group by 中没有c3。并且没有等于常量的等式。所以不能使用松散索引扫描
可以这样改一下：SELECT c1,c3 FROM t1 where c3='a' GROUP BY c1, c2
为什么松散索引扫描的效率会很高？
答：因为在没有WHERE 子句，也就是必须经过全索引扫描的时候， 松散索引扫描需要读取的键值数量与分组的组数量一样多，也就是说比实际存在的键值数目要少很多。而在WHERE 子句包含范围判断式或者等值表达式的时候， 松散索引扫描查找满足范围条件的每个组的第1 个关键字，并且再次读取尽可能最少数量的关键字。
### 2、使用紧凑索引扫描（Tight index scan）实现 GROUP BY
紧凑索引扫描实现 GROUP BY 和松散索引扫描的区别主要在于：
紧凑索引扫描需要在扫描索引的时候，读取所有满足条件的索引键，然后再根据读取出的数据来完成 GROUP BY 操作得到相应结果。
这时候的执行计划的 Extra 信息中已经没有“Using index for group-by”了，但并不是说 MySQL 的 GROUP BY 操作并不是通过索引完成的，只不过是需要访问 WHERE 条件所限定的所有索引键信息之后才能得出结果。这就是通过紧凑索引扫描来实现 GROUP BY 的执行计划输出信息。
**在 MySQL 中，MySQL Query Optimizer 首先会选择尝试通过松散索引扫描来实现 GROUP BY 操作，当发现某些情况无法满足松散索引扫描实现 GROUP BY 的要求之后，才会尝试通过紧凑索引扫描来实现。**
**当 GROUP BY 条件字段并不连续或者不是索引前缀部分的时候，MySQL Query Optimizer 无法使用松散索引扫描。**
**这时检查where 中的条件字段是否有索引的前缀部分，如果有此前缀部分，且该部分是一个常量，且与group by 后的字段组合起来成为一个连续的索引。这时按紧凑索引扫描。**
SELECT max(gmt_create)
FROM group_message
WHERE group_id = 2
GROUP BY user_id
需读取group_id=2的所有数据，然后在读取的数据中完成group by操作得到结果。（这里group by 字段并不是一个连续索引，正好where 中group_id正好弥补缺失的索引键，又恰好是一个常量，因此使用紧凑索引扫描）
group_id user_id 这个顺序是可以使用该索引。如果连接的顺序不符合索引的“最左前缀”原则，则不使用紧凑索引扫描。
以下例子使用紧凑索引扫描
GROUP BY中有一个差距，但已经由条件user_id = 1覆盖。
explain
SELECT group_id,gmt_create
FROM group_message
WHERE user_id = 1 GROUP BY group_id,gmt_create
GROUP BY不以关键字的第1个元素开始，但是有一个条件提供该元素的常量
explain
SELECT group_id,gmt_create
FROM group_message
WHERE group_id = 1 GROUP BY user_id,gmt_create
-----------------------------------------------------------------------------------------------------
下面的例子都不使用紧凑索引扫描
user_id,gmt_create 连接起来并不符合索引“最左前缀”原则
explain
SELECT group_id,gmt_create
FROM group_message
WHERE user_id = 1 GROUP BY gmt_create
group_id,gmt_create 连接起来并不符合索引“最左前缀”原则
explain
SELECT gmt_create
FROM group_message
WHERE group_id=1 GROUP BY gmt_create;
###  3、使用临时表实现 GROUP BY
MySQL Query Optimizer 发现仅仅通过索引扫描并不能直接得到 GROUP BY 的结果之后，他就不得不选择通过使用临时表然后再排序的方式来实现 GROUP BY了。在这样示例中即是这样的情况。 group_id 并不是一个常量条件，而是一个范围，而且 GROUP BY 字段为 user_id。所以 MySQL 无法根据索引的顺序来帮助 GROUP BY 的实现，只能先通过索引范围扫描得到需要的数据，然后将数据存入临时表，然后再进行排序和分组操作来完成 GROUP BY。
explain
SELECT group_id
FROM group_message
WHERE group_id between 1 and 4
GROUP BY user_id;
## 示例数据库文件
**[sql]**[view
 plain](http://blog.csdn.net/zm2714/article/details/7887093#)[copy](http://blog.csdn.net/zm2714/article/details/7887093#)
- -- --------------------------------------------------------
- -- Host:                         127.0.0.1
- -- Server version:               5.1.57-community - MySQL Community Server (GPL)
- -- Server OS:                    Win32
- -- HeidiSQL version:             7.0.0.4156
- -- Date/time:                    2012-08-20 16:52:10
- -- --------------------------------------------------------
- 
- /*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;  
- /*!40101 SET NAMES utf8 */;  
- /*!40014 SET FOREIGN_KEY_CHECKS=0 */;  
- 
- -- Dumping structure for table test.group_message
- DROPTABLE IF EXISTS `group_message`;  
- CREATETABLE IF NOT EXISTS `group_message` (  
-   `id` int(10) unsigned NOTNULL AUTO_INCREMENT,  
-   `group_id` int(10) unsigned DEFAULTNULL,  
-   `user_id` int(10) unsigned DEFAULTNULL,  
-   `gmt_create` timestampNOTNULLDEFAULTCURRENT_TIMESTAMP,  
-   `abc` int(11) NOTNULLDEFAULT'0',  
- PRIMARYKEY (`id`),  
- KEY `group_id_user_id_gmt_create` (`group_id`,`user_id`,`gmt_create`)  
- ) ENGINE=MyISAM AUTO_INCREMENT=27 DEFAULT CHARSET=utf8;  
- 
- -- Dumping data for table test.group_message: 0 rows
- DELETEFROM `group_message`;  
- /*!40000 ALTERTABLE `group_message` DISABLE KEYS */;  
- INSERTINTO `group_message` (`id`, `group_id`, `user_id`, `gmt_create`, `abc`) VALUES
-     (1, 1, 1, '2012-08-20 09:25:35', 1),  
-     (2, 2, 1, '2012-08-20 09:25:39', 1),  
-     (3, 2, 2, '2012-08-20 09:25:47', 1),  
-     (4, 3, 1, '2012-08-20 09:25:50', 2),  
-     (5, 3, 2, '2012-08-20 09:25:52', 2),  
-     (6, 3, 3, '2012-08-20 09:25:54', 0),  
-     (7, 4, 1, '2012-08-20 09:25:57', 0),  
-     (8, 4, 2, '2012-08-20 09:26:00', 0),  
-     (9, 4, 3, '2012-08-20 09:26:02', 0),  
-     (10, 4, 4, '2012-08-20 09:26:06', 0),  
-     (11, 5, 1, '2012-08-20 09:26:09', 0),  
-     (12, 5, 2, '2012-08-20 09:26:12', 0),  
-     (13, 5, 3, '2012-08-20 09:26:13', 0),  
-     (14, 5, 4, '2012-08-20 09:26:15', 0),  
-     (15, 5, 5, '2012-08-20 09:26:17', 0),  
-     (16, 6, 1, '2012-08-20 09:26:20', 0),  
-     (17, 7, 1, '2012-08-20 09:26:23', 0),  
-     (18, 7, 2, '2012-08-20 09:26:28', 0),  
-     (19, 8, 1, '2012-08-20 09:26:32', 0),  
-     (20, 8, 2, '2012-08-20 09:26:35', 0),  
-     (21, 9, 1, '2012-08-20 09:26:37', 0),  
-     (22, 9, 2, '2012-08-20 09:26:40', 0),  
-     (23, 10, 1, '2012-08-20 09:26:42', 0),  
-     (24, 10, 2, '2012-08-20 09:26:44', 0),  
-     (25, 10, 3, '2012-08-20 09:26:51', 0),  
-     (26, 11, 1, '2012-08-20 09:26:54', 0);  
- /*!40000 ALTERTABLE `group_message` ENABLE KEYS */;  
- /*!40014 SET FOREIGN_KEY_CHECKS=1 */;  
- /*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;  
![](http://static.blog.csdn.net/images/save_snippets.png)
## 参考文献
1、MySQL如何优化GROUP BY [http://www.iteedu.com//database/mysql/mysqlmanualcn/optimization/group-by-optimization.php](http://www.iteedu.com//database/mysql/mysqlmanualcn/optimization/group-by-optimization.php)
2、详解MySQL分组查询Group By实现原理 [http://www.51testing.com/html/52/n-229952.html](http://www.51testing.com/html/52/n-229952.html)
3、松散的索引扫描(Loose index scan) [http://www.coder4.com/archives/1344](http://www.coder4.com/archives/1344)
4、MySQL学习笔记 [http://leeon.me/a/mysql-notes-2](http://leeon.me/a/mysql-notes-2)
