# MySQL5.6之Index Condition Pushdown(ICP,索引条件下推) - z69183787的专栏 - CSDN博客
2016年12月31日 12:49:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：840
http://mdba.cn/2014/01/21/index-condition-pushdownicp%E7%B4%A2%E5%BC%95%E6%9D%A1%E4%BB%B6%E4%B8%8B%E6%8E%A8/
ICP（index condition pushdown）是mysql利用索引（二级索引）元组和筛字段在索引中的where条件从表中提取数据记录的一种优化操作。ICP的思想是：存储引擎在访问索引的时候检查筛选字段在索引中的where条件（pushed
 index condition，推送的索引条件），如果索引元组中的数据不满足推送的索引条件，那么就过滤掉该条数据记录。ICP（优化器）尽可能的把index condition的处理从server层下推到storage engine层。storage engine使用索引过过滤不相关的数据，仅返回符合index condition条件的数据给server层。也是说数据过滤尽可能在storage engine层进行，而不是返回所有数据给server层，然后后再根据where条件进行过滤。使用ICP（mysql
 5.6版本以前）和没有使用ICP的数据访问和提取过程如下（插图来在[MariaDB
 Blog](https://mariadb.com/kb/en/index-condition-pushdown/)）：
优化器没有使用ICP时，数据访问和提取的过程如下：
1)    当storage engine读取下一行时，首先读取索引元组（index tuple），然后使用索引元组在基表中（base table）定位和读取整行数据。
2)    sever层评估where条件，如果该行数据满足where条件则使用，否则丢弃。
3)    执行1），直到最后一行数据。
![index-access-2phases](http://mdba.cn/wp-content/uploads/2014/01/index-access-2phases.png)
优化器使用ICP时，server层将会把能够通过使用索引进行评估的where条件下推到storage engine层。数据访问和提取过程如下：
1)    storage engine从索引中读取下一条索引元组。
2)    storage engine使用索引元组评估下推的索引条件。如果没有满足wehere条件，storage engine将会处理下一条索引元组（回到上一步）。只有当索引元组满足下推的索引条件的时候，才会继续去基表中读取数据。
3)    如果满足下推的索引条件，storage engine通过索引元组定位基表的行和读取整行数据并返回给server层。
4)    server层评估没有被下推到storage engine层的where条件，如果该行数据满足where条件则使用，否则丢弃。
![index-access-with-icp](http://mdba.cn/wp-content/uploads/2014/01/index-access-with-icp.png)
而使用ICP时，如果where条件的一部分能够通过使用索引中的字段进行评估，那么mysql server把这部分where条件下推到storage engine（存储引擎层）。存储引擎通过索引元组的索引列数据过滤不满足下推索引条件的数据行。
索引条件下推的意思就是筛选字段在索引中的where条件从server层下推到storage
 engine层，这样可以在存储引擎层过滤数据。由此可见，ICP可以减少存储引擎访问基表的次数和mysql server访问存储引擎的次数。
注意一下ICP的使用条件：
- 
只能用于二级索引(secondary index)。
- 
explain显示的执行计划中type值（join 类型）为range、 ref、 eq_ref或者ref_or_null。且查询需要访问表的整行数据，即不能直接通过二级索引的元组数据获得查询结果(索引覆盖)。
- 
ICP可以用于MyISAM和InnnoDB存储引擎，不支持分区表（5.7将会解决这个问题）。
ICP的开启优化功能与关闭
MySQL5.6可以通过设置optimizer_switch([global|session],dynamic)变量开启或者关闭index_condition_push优化功能，默认开启。
mysql > set optimizer_switch=’index_condition_pushdown=on|off’
用explain查看执行计划时，如果执行计划中的Extra信息为“using index condition”，表示优化器使用的index condition pushdown。
在mysql5.6以前，还没有采用ICP这种查询优化，where查询条件中的索引条件在某些情况下没有充分利用索引过滤数据。假设一个组合索引（多列索引）K包含（c1,c2,…,cn）n个列，如果在c1上存在范围扫描的where条件，那么剩余的c2,…,cn这n-1个上索引都无法用来提取和过滤数据（不管不管是唯一查找还是范围查找），索引记录没有被充分利用。即组合索引前面字段上存在范围查询，那么后面的部分的索引将不能被使用，因为后面部分的索引数据是无序。比如，索引key（a，b）中的元组数据为(0,100)、(1,50)、（1，100）
 ，where查询条件为 a < 2 and b = 100。由于b上得索引数据并不是连续区间，因为在读取（1，50）之后不再会读取（1，100），mysql优化器在执行索引区间扫描之后也不再扫描组合索引其后面的部分。
表结构定义如下：
```
`CREATE`
```
TABLE
```
```
`person` (
```
```
`person_id`
```
`smallint`
```
(5)
 unsigned
```
`NOT`
```
NULL
```
```
AUTO_INCREMENT,
```
```
`postadlcode`
```
`int`
```
(11)
```
`DEFAULT`
```
NULL
```
`,`
```
`age`
 tinyint(4)
```
`DEFAULT`
```
NULL
```
`,`
```
`first_name`
```
`varchar`
```
(45)
```
`NOT`
```
NULL
```
`,`
```
`last_name`
```
`varchar`
```
(45)
```
`NOT`
```
NULL
```
`,`
```
`last_update`
```
`timestamp`
```
NOT
```
```
NULL
```
```
DEFAULT
```
```
CURRENT_TIMESTAMP
```
```
ON
```
```
UPDATE
```
```
CURRENT_TIMESTAMP
```
`,`
`PRIMARY`
```
KEY
```
```
(`person_id`),
```
`KEY`
```
`idx_p_a` (`postadlcode`,`age`),
```
`KEY`
```
`idx_f_l` (`first_name`,`last_name`)
```
```
)
 ENGINE=InnoDB
```
`DEFAULT`
```
CHARSET=utf8
```
```
关闭ICP优化，Extra信息为“Using Where”
```
```
mysql>
```
`set`
```
optimizer_switch =
```
`"index_condition_pushdown=off"``;`
```
mysql>
 explain
```
`select`
```
*
```
`from`
```
person
```
`where`
```
postadlcode
```
`between`
```
300000
```
`and`
```
400000
```
`and`
```
age > 40;
```
`+``----+-------------+--------+-------+---------------+---------+---------+------+------+-------------+`
```
|
 id | select_type |
```
`table`
```
| type  | possible_keys |
```
`key`
```
| key_len | ref  |
```
`rows`
```
| Extra       |
```
`+``----+-------------+--------+-------+---------------+---------+---------+------+------+-------------+`
```
| 
 1 | SIMPLE      | person | range | idx_p_a       | idx_p_a | 7       |
```
`NULL`
```
|   21 | Using
```
`where`
```
|
```
`+``----+-------------+--------+-------+---------------+---------+---------+------+------+-------------+`
```
开启ICP之后，Extra信息为“Using Index Condition”
```
```
mysql>
```
`set`
```
optimizer_switch =
```
`"index_condition_pushdown=on"``;`
```
mysql>
 explain
```
`select`
```
*
```
`from`
```
person
```
`where`
```
postadlcode
```
`between`
```
300000
```
`and`
```
400000
```
`and`
```
age > 40;
```
`+``----+-------------+--------+-------+---------------+---------+---------+------+------+-----------------------+`
```
|
 id | select_type |
```
`table`
```
| type  | possible_keys |
```
`key`
```
| key_len | ref  |
```
`rows`
```
| Extra                 |
```
`+``----+-------------+--------+-------+---------------+---------+---------+------+------+-----------------------+`
```
| 
 1 | SIMPLE      | person | range | idx_p_a       | idx_p_a | 7       |
```
`NULL`
```
|   21 | Using
```
`index`
```
condition |
```
`+``----+-------------+--------+-------+---------------+---------+---------+------+------+-----------------------+`
```
参考资料：
### [The MySQL range access method explained](http://mdba.cn/2014/01/21/index-condition-pushdownicp%E7%B4%A2%E5%BC%95%E6%9D%A1%E4%BB%B6%E4%B8%8B%E6%8E%A8/The%20MySQL%20range%20access%20method%20explained) （需要梯子）
### [MySQL 5.6: Index Condition Pushdown](http://olavsandstaa.blogspot.co.uk/2011/04/mysql-56-index-condition-pushdown.html) （需要梯子）
#### [Index Condition Pushdown Optimization](https://dev.mysql.com/doc/refman/5.6/en/index-condition-pushdown-optimization.html)
