# MySQL Order By实现原理分析和Filesort优化 - z69183787的专栏 - CSDN博客
2018年10月12日 12:34:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：114
个人分类：[Mysql-基础知识](https://blog.csdn.net/z69183787/article/category/3258085)
[https://blog.csdn.net/a925907195/article/details/71244622](https://blog.csdn.net/a925907195/article/details/71244622)
.
目录[(?)](http://blog.csdn.net/hguisu/article/details/7161981#)[[-]](http://blog.csdn.net/hguisu/article/details/7161981#)
- [利用有序索引获取有序数据](http://blog.csdn.net/hguisu/article/details/7161981#t0)- [1 按照索引对结果进行排序order by 使用索引是有条件](http://blog.csdn.net/hguisu/article/details/7161981#t1)
- [文件排序](http://blog.csdn.net/hguisu/article/details/7161981#t2)- [1 MySQL 需要使用filesort 实现排序的实例](http://blog.csdn.net/hguisu/article/details/7161981#t3)
- [2 MySQL 需要使用Using temporary 临时表来filesort](http://blog.csdn.net/hguisu/article/details/7161981#t4)
- [优化Filesort](http://blog.csdn.net/hguisu/article/details/7161981#t5)
在[MySQL](http://lib.csdn.net/base/mysql)中的ORDER BY有两种排序实现方式:
**1、利用有序索引获取有序数据**
**2、文件排序**
在使用explain分析查询的时候**，**利用有序索引获取有序数据显示**Using index**。而文件排序显示**Using filesort。**
# 1.利用有序索引获取有序数据
          取出满足过滤条件作为排序条件的字段，以及可以直接定位到行数据的行指针信息，在 Sort Buffer 中进行实际的排序操作，然后利用排好序的数据根据行指针信息返回表中取得客户端请求的其他字段的数据，再返回给客户端.
          这种方式，在使用explain分析查询的时候，显示**Using index**。而文件排序显示**Using filesort。**
****注意：**MySQL在查询时最多只能使用一个索引。因此，如果WHERE条件已经占用了索引，那么在排序中就不使用索引了。**
## 1.1 按照索引对结果进行排序：order by 使用索引是有条件
**1)  返回选择的字段，**即只包括在有选择的此列上（**select后面的字段**），不一定适应*的情况)：
**[sql]**[view plain](http://blog.csdn.net/hguisu/article/details/7161981#)[copy](http://blog.csdn.net/hguisu/article/details/7161981#)
- CREATE TABLE `test` (  
-   `id` int(11) NOT NULLAUTO_INCREMENT,  
-   `rdate` datetime NOT NULL,  
-   `inventid` int(11) NOT NULL,  
-   `customerid` int(11) NOT NULL,  
-   `staffid` int(11) NOT NULL,  
-   `data` varchar(20) NOT NULL,  
-   PRIMARY KEY (`id`),  
-   UNIQUE KEY `rdate`(`rdate`,`inventid`,`customerid`),  
-   KEY `inventid` (`inventid`),  
-   KEY `customerid` (`customerid`),  
-   KEY `staffid` (`staffid`)  
- ) ENGINE=InnoDB AUTO_INCREMENT=27 DEFAULT CHARSET=latin1  
如：
**[sql]**[view plain](http://blog.csdn.net/hguisu/article/details/7161981#)[copy](http://blog.csdn.net/hguisu/article/details/7161981#)
- mysql>   
- explain select inventid from test where rdate='2011-12-1400:00:00' order by  inventid , customerid;  
+----+-------------+-------+------+---------------+-------+---------+-------+------+--------------------------+
| id | select_type | table | type | possible_keys |key    | key_len |ref      | rows |Extra                    |
+----+-------------+-------+------+---------------+-------+---------+-------+------+--------------------------+
|  1 |  SIMPLE      | test    |ref   |        rdate          |rdate  |      8     |const |   10   | Using where; **Using index **|
+----+-------------+-------+------+---------------+-------+---------+-------+------+--------------------------+
1 row in set (0.00 sec)
Select选择的列使用索引，而下面不使用索引：
mysql> explain select * from test where rdate='2011-12-14 00:00:00'order by  inventid , customerid ;
+----+-------------+-------+------+---------------+------+---------+------+------+-----------------------------+
| id | select_type | table | type | possible_keys | key     | key_len|ref     | rows | Extra                       |
+----+-------------+-------+------+---------------+------+---------+------+------+-----------------------------+
|  1 | SIMPLE      | test   | ALL  | rdate               | NULL  | NULL    |NULL |  13   |Using where;**Using filesort**|
+----+-------------+-------+------+---------------+------+---------+------+------+-----------------------------+
**2)  只有当ORDER BY中所有的列必须包含在相同的索引，并且索引的顺序和order by子句中的顺序完全一致，并且所有列的排序方向（升序或者降序）一样才有**，（混合使用ASC模式和DESC模式则不使用索引）
**[sql]**[view plain](http://blog.csdn.net/hguisu/article/details/7161981#)[copy](http://blog.csdn.net/hguisu/article/details/7161981#)
- mysql>   
- xplain select inventid from test order byrdate, inventid ;  
+----+-------------+-------+-------+---------------+-------+---------+------+------+-------------+
| id | select_type | table | type  | possible_keys | key   | key_len | ref  | rows | Extra       |
+----+-------------+-------+-------+---------------+-------+---------+------+------+-------------+
|  1 | SIMPLE      | test  | index | NULL          | rdate |16      | NULL |   13 |**Using index**|
+----+-------------+-------+-------+---------------+-------+---------+------+------+-------------+
1 row in set (0.00 sec)
**[sql]**[view plain](http://blog.csdn.net/hguisu/article/details/7161981#)[copy](http://blog.csdn.net/hguisu/article/details/7161981#)
- mysql>   
- explain select inventid from test where rdate="2011-12-16" order by  inventid ,staffid;  
+----+-------------+-------+------+---------------+-------+---------+-------+------+--------------------------
| id | select_type | table | type | possible_keys |key   | key_len | ref   | rows | Extra                       |
+----+-------------+-------+------+---------------+-------+---------+-------+------+--------------------------
|  1 | SIMPLE      | test | ref  | rdate         | rdate | 8       | const |    1 |**Using where;Using filesort**
+----+-------------+-------+------+---------------+-------+---------+-------+------+--------------------------
1 row in set (0.00 sec)
由于rdate, inventid使用了同一个索引。排序使用到了索引。这个也是满足了前缀索引。但是order  by  inventid ,staffid;就不是使用了索引，因为staffid和inventid不是同一个索引
**3)     where 语句与ORDER BY语句组合满足最左前缀:**
**[sql]**[view plain](http://blog.csdn.net/hguisu/article/details/7161981#)[copy](http://blog.csdn.net/hguisu/article/details/7161981#)
- mysql>   
- explain select inventid from test whererdate="2011-12-16" order by  inventid ;  
+----+-------------+-------+------+---------------+-------+---------+-------+------+--------------------------+
| id | select_type | table | type | possible_keys | key   | key_len | ref   | rows | Extra                    |
+----+-------------+-------+------+---------------+-------+---------+-------+------+--------------------------+
|  1 | SIMPLE      | test | ref  | rdate         | rdate | 8       | const |    1 | Using where;**Using index** |
+----+-------------+-------+------+---------------+-------+---------+-------+------+--------------------------+
1 row in set (0.00 sec)
**4)     如果查询联接了多个表，只有在order by子句的所有列引用的是第一个表的列才可以。**
**5)     在其他的情况下，mysql使用文件排序  **例如：
> 
1）   where语句与order by语句，使用了不同的索引
2）   检查的行数过多，且没有使用覆盖索引
3）   ORDER BY中的列不包含在相同的索引，也就是使用了不同的索引
4）   对索引列同时使用了ASC和DESC
5）   where语句或者ORDER BY语句中索引列使用了表达式，包括函数表达式
6）   where 语句与ORDER BY语句组合满足最左前缀，但where语句中使用了条件查询。查见第10句,虽然where与order by构成了索引最左有缀的条件，但是where子句中使用的是**条件查询**。
mysql> explain select inventid from tes**t where  rdate>"2011-12-16"** order by  inventid;
+----+-------------+-------+-------+---------------+-------+---------+------+------+----------------
| id | select_type | table | type  | possible_keys | key   | key_len | ref  | rows | Extra                                    
+----+-------------+-------+-------+---------------+-------+---------+------+------+----------------
|  1 |SIMPLE      | test  | range | rdate         | rdate | 8       | NULL |    1 | Using where;** Using index;****Usingfilesort**|
+----+-------------+-------+-------+---------------+-------+---------+------+------+----------------
1 row in set (0.00sec)
  7）   当使用left join，使用右边的表字段排序
# 2.文件排序
       这个 filesort 并不是说通过磁盘文件进行排序，而只是告诉我们进行了一个排序操作。即在MySQL Query Optimizer 所给出的执行计划(通过 EXPLAIN 命令查看)中被称为**文件排序（filesort）**
      文件排序是通过相应的排序[算法](http://lib.csdn.net/base/datastructure),将取得的数据在内存中进行排序: MySQL需要将数据在内存中进行排序，所使用的内存区域也就是我们通过**sort_buffer_size** 系统变量所设置的排序区。这个排序区是每个Thread 独享的，所以说可能在同一时刻在MySQL 中可能存在多个 sort buffer 内存区域。
在MySQL中filesort 的实现算法实际上是有两种：
**双路排序**：是首先根据相应的条件取出相应的排序字段和可以直接定位行数据的行指针信息，然后在sort buffer 中进行排序。
**单路排序**：是一次性取出满足条件行的所有字段，然后在sort buffer中进行排序。
在MySQL4.1版本之前只有第一种排序算法双路排序，第二种算法是从MySQL4.1开始的改进算法，**主要目的是为了减少第一次算法中需要两次访问表数据的 IO 操作，将两次变成了一次，但相应也会耗用更多的sortbuffer 空间。当然，MySQL4.1开始的以后所有版本同时也支持第一种算法，**
**MySQL主要通过比较我们所设定的系统参数 max_length_for_sort_data的大小和Query 语句所取出的字段类型大小总和来判定需要使用哪一种排序算法。**如果 max_length_for_sort_data更大，则使用第二种优化后的算法，反之使用第一种算法。所以如果希望 ORDER BY 操作的效率尽可能的高，一定要主义max_length_for_sort_data 参数的设置。曾经就有同事的[数据库](http://lib.csdn.net/base/mysql)出现大量的排序等待，造成系统负载很高，而且响应时间变得很长，最后查出正是因为MySQL 使用了传统的第一种排序算法而导致，在加大了**max_length_for_sort_data** 参数值之后，系统负载马上得到了大的缓解，响应也快了很多。
## 2.1 MySQL 需要使用filesort 实现排序的实例
　　假设有 Table A 和 B 两个表结构分别如下：
**# mysql**
**>**show create table A\G
			 　　*************************** **1**. row ***************************
			 　　Table: A
			 　　Create Table: CREATE TABLE `A` (
			 　　`id` int(**11**) NOT NULL default '0',
			  　 `c2` char(**2**) default NULL,
			 　　`c3` varchar(**16**) default NULL, 
			 　　`c4` datetime default NULL, 
			 　　PRIMARY KEY (`id`) 
			 　　) ENGINE=MyISAM DEFAULT CHARSET=utf8
#:mysql
> show create table B\G
			 　　*************************** **1**. row ***************************  　　Table: B
			 　　Create Table: CREATE TABLE `B` ( 
			 　　`id` int(**11**) NOT NULL default '0', 
			 　　`c2` char(**2**) default NULL,
			 　　`c3` varchar(**16**) default NULL, 
			 　　PRIMARY KEY (`id`),
 　　KEY `B_c2_ind` (`c2`)
			 　　) ENGINE=MyISAM DEFAULT CHARSET=utf8
A.c2不是索引将使用：
sky@localhost : example **01**:**54**:**23**> EXPLAIN SELECT A.* FROM A,B WHERE A.id >**2** AND A.c2 <**5** AND A.c2 = B.c2 ORDER BY A.c2\G
			　　*************************** **1**. row ***************************
			　　id: **1**
			　　select_type: SIMPLE
			　　table: A
			　　type: range
			　　possible_keys: PRIMARY
			　　key: PRIMARY
			　　key_len: **4**
			　　ref: NULL
			　　rows: **3**
			　　Extra: Using where; Using filesort
*************************** **2**. row ***************************
			　　id: **1**
			　　select_type: SIMPLE
			　　table: B
			　　type: ref
			　　possible_keys: B_c2_ind
			　　key: B_c2_ind
			　　key_len: **7**
			　　ref: example.A.c2
			　　rows: **2**
			　　Extra: Using where; Using index
MySQL 从 Table A 中取出了符合条件的数据，由于取得的数据并不满足 ORDER BY 条件，所以 MySQL 进行了 filesort 操作，其整个执行过程如下图所示：
![](http://hi.csdn.net/attachment/201112/29/0_132514049163ii.gif)
## 2.2 MySQL 需要使用Using temporary 临时表来filesort
         如果order by的子句只引用了联接中的第一个表，MySQL会先对第一个表进行排序，然后进行联接。也就是expain中的Extra的Using Filesort.否则MySQL先把结果保存到临时表(Temporary Table),然后再对临时表的数据进行排序.此时expain中的Extra的显示Using temporary Using Filesort.
例如如果我们的排序数据如果是两个(或者更多个) Table 通过 Join所得出的,如下例所示：
sky@localhost : example **02**:**46**:**15**> explain select A.* from A,B
			where A.id > **2** and A.c2 < **5** and A.c2 = B.c2 order by B.c3\G
			　　*************************** **1**. row***************************
			　　id: **1**
			  　select_type: SIMPLE
			　　table: A
			　　type: range
			　　possible_keys: PRIMARY
			　　key: PRIMARY
			　　key_len: **4**
			　　ref: NULL
			  　rows: **3**
Extra: Using where; Using temporary; Using filesort
			　　*************************** **2**. row ***************************
			　　id: **1**
			　　select_type: SIMPLE
			　　table: B
　　type: ref
			　　possible_keys: B_c2_ind
			　　key: B_c2_ind
			　　key_len: **7**
			　　ref: example.A.c2
			　　rows: **2**
			　　Extra: Using where
 实际执行过程应该是如下图所示：
![](http://hi.csdn.net/attachment/201112/29/0_1325140561Zcfo.gif)
# 3. 优化Filesort
当无法避免排序操作时，又该如何来优化呢？很显然，应该尽可能让 MySQL 选择使用第二种单路算法来进行排序。这样可以减少大量的随机IO操作，很大幅度地提高排序工作的效率。
**1. 加大 max_length_for_sort_data 参数的设置**
在 MySQL 中，决定使用老式排序算法还是改进版排序算法是通过参数 max_length_for_ sort_data 来决定的。当所有返回字段的最大长度小于这个参数值时，MySQL 就会选择改进后的排序算法，反之，则选择老式的算法。所以，如果有充足的内存让MySQL 存放须要返回的非排序字段，就可以加大这个参数的值来让 MySQL 选择使用改进版的排序算法。
**2. 去掉不必要的返回字段**
当内存不是很充裕时，不能简单地通过强行加大上面的参数来强迫 MySQL 去使用改进版的排序算法，否则可能会造成 MySQL 不得不将数据分成很多段，然后进行排序，这样可能会得不偿失。此时就须要去掉不必要的返回字段，让返回结果长度适应 max_length_for_sort_data 参数的限制。
**3. 增大 sort_buffer_size 参数设置**
增大 sort_buffer_size 并不是为了让 MySQL选择改进版的排序算法，而是为了让MySQL尽量减少在排序过程中对须要排序的数据进行分段，因为分段会造成 MySQL 不得不使用临时表来进行交换排序。
