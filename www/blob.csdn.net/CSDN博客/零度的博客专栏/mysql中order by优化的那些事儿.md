# mysql中order by优化的那些事儿 - 零度的博客专栏 - CSDN博客
2017年03月01日 10:14:17[零度anngle](https://me.csdn.net/zmx729618)阅读数：435标签：[mysql																[查询优化](https://so.csdn.net/so/search/s.do?q=查询优化&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[mysql](https://blog.csdn.net/zmx729618/article/category/6441555)
                
为了测试方便和直观，我们需要先创建一张测试表并插入一些数据：
- CREATE TABLE `shop` ( 
-   `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '记录ID', 
-   `shop_id` int(11) NOT NULL COMMENT '商店ID', 
-   `goods_id` int(11) NOT NULL COMMENT '物品ID', 
-   `pay_type` tinyint(1) NOT NULL COMMENT '支付方式', 
-   `price` decimal(10,2) NOT NULL COMMENT '物品价格', 
-   `comment` varchar(200) NOT NULL COMMENT '备注', 
-   PRIMARY KEY (`id`), 
-   UNIQUE KEY `shop_id` (`shop_id`,`goods_id`), 
-   KEY `price` (`price`), 
-   KEY `pay_type` (`pay_type`) 
- ) ENGINE=InnoDBAUTO_INCREMENT=8 DEFAULT CHARSET=utf8COMMENT='商店物品表'
插入几行数据：
- INSERT INTO `shop` (`id`, `shop_id`, `goods_id`, `pay_type`, `price`, `comment`) VALUES 
- (1, 1, 1, 0, '1.00', ''), 
- (2, 2, 1, 0, '24.00', ''), 
- (3, 2, 3, 1, '5.99', ''), 
- (4, 3, 1, 0, '1.99', ''), 
- (5, 3, 2, 1, '81.00', ''), 
- (6, 4, 2, 0, '15.00', ''), 
- (7, 4, 3, 0, '22.00', ''); 
好了。现在我们可以开始我们的学习了。
对照一下官方手册：[http://dev.mysql.com/doc/refman/5.5/en/order-by-optimization.html](http://dev.mysql.com/doc/refman/5.5/en/order-by-optimization.html)
手册上说，如下的四种情况[MySQL](http://lib.csdn.net/base/14)是会作优化的：
- SELECT * FROM t1 ORDER BY key_part1,key_part2,... ; 
- SELECT * FROM t1 WHERE key_part1=constant ORDER BY key_part2; 
- SELECT * FROM t1 ORDER BY key_part1 DESC, key_part2 DESC; 
- SELECT * FROM t1 WHERE key_part1=1 ORDER BY key_part1 DESC, key_part2 DESC; 
真的是这样么？按手册上的说法，如果explain的extra中出现了Using filesort则是没有用到排序优化。来吧，让我们挨个测试一下：
可优化的第一种情况
- SELECT * FROM t1 ORDER BY key_part1,key_part2,...; 
指的是使用联合索引中的各个字段进行排序：
- mysql> explain select * from shop order by shop_id,goods_id; 
- +----+-------------+-------+------+---------------+------+---------+------+------+----------------+ 
- | id | select_type | table | type | possible_keys | key  | key_len | ref  | rows | Extra          | 
- +----+-------------+-------+------+---------------+------+---------+------+------+----------------+ 
- |  1 | SIMPLE      | shop  | ALL  | NULL          | NULL | NULL    | NULL |    7 | Using filesort | 
- +----+-------------+-------+------+---------------+------+---------+------+------+----------------+ 
- 1 row in set (0.00 sec) 
当我们检索所有记录时可以看到，索引优化是无效的。如果改成如下的查询就可以应用上索引优化：
- mysql> explain select id,shop_id,goods_id from shop order by shop_id,goods_id; 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------------+ 
- | id | select_type | table | type  | possible_keys | key     | key_len | ref  | rows | Extra       | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------------+ 
- |  1 | SIMPLE      | shop  | index | NULL          | shop_id | 8       | NULL |    7 | Using index | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------------+ 
- 1 row in set (0.00 sec) 
这里的三个字段是在索引树中存放的，因此可以直接从索引树检索出来，不用去检索行，所以extra显示的是Using index不会出现Using filesort。而一旦我们加上了除主键之外的非排序字段，索引优化就失效了。
另外，还可以强制指定索引，这样也可以应用上索引优化：
- mysql> explain select * from shop force index(shop_id) order by shop_id,goods_id; 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------+ 
- | id | select_type | table | type  | possible_keys | key     | key_len | ref  | rows | Extra | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------+ 
- |  1 | SIMPLE      | shop  | index | NULL          | shop_id | 8       | NULL |    7 |       | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------+ 
- 1 row in set (0.03 sec) 
可优化的第二种情况
- SELECT * FROM t1 WHERE key_part1=constant ORDER BY key_part2; 
这种情况指的是联合索引中的一部分指定了常量去检索，排序则使用了索引的另一部分。
- mysql> explain select * from shop where shop_id=2 order by goods_id; 
- +----+-------------+-------+------+---------------+---------+---------+-------+------+-------------+ 
- | id | select_type | table | type | possible_keys | key     | key_len | ref   | rows | Extra       | 
- +----+-------------+-------+------+---------------+---------+---------+-------+------+-------------+ 
- |  1 | SIMPLE      | shop  | ref  | shop_id       | shop_id | 4       | const |    2 | Using where | 
- +----+-------------+-------+------+---------------+---------+---------+-------+------+-------------+ 
- 1 row in set (0.00 sec) 
的确，该情况索引优化是有效的。
可优化的第三种情况
- SELECT * FROM t1 ORDER BY key_part1 DESC, key_part2 DESC;  
这种情况与第一种情况类似，仅仅是排序方向变更了，不出意外的话仍然不会有优化：
- mysql> explain select * from shop order by shop_id desc,goods_id desc; 
- +----+-------------+-------+------+---------------+------+---------+------+------+----------------+ 
- | id | select_type | table | type | possible_keys | key  | key_len | ref  | rows | Extra          | 
- +----+-------------+-------+------+---------------+------+---------+------+------+----------------+ 
- |  1 | SIMPLE      | shop  | ALL  | NULL          | NULL | NULL    | NULL |    7 | Using filesort | 
- +----+-------------+-------+------+---------------+------+---------+------+------+----------------+ 
- 1 row in set (0.00 sec) 
果然，没有排序优化。同样，如果只检索主键和排序字段，排序优化有效：
- mysql> explain select id,shop_id,goods_id from shop order by shop_id desc,goods_id desc; 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------------+ 
- | id | select_type | table | type  | possible_keys | key     | key_len | ref  | rows | Extra       | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------------+ 
- |  1 | SIMPLE      | shop  | index | NULL          | shop_id | 8       | NULL |    7 | Using index | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------------+ 
- 1 row in set (0.00 sec) 
如果强制指定索引呢？
- mysql> explain select * from shop force index(shop_id) order by shop_id desc,goods_id desc; 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------+ 
- | id | select_type | table | type  | possible_keys | key     | key_len | ref  | rows | Extra | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------+ 
- |  1 | SIMPLE      | shop  | index | NULL          | shop_id | 8       | NULL |    7 |       | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------+ 
- 1 row in set (0.00 sec) 
可见也是有效的。
可优化的第四种情况
- SELECT * FROM t1 WHERE key_part1=1 ORDER BY key_part1 DESC, key_part2 DESC; 
以索引的一部分为条件并且是常量，排序按索引的各字段倒排时，这种情况排序优化有效：
- mysql> explain select * from shop where shop_id=2 order by shop_id desc,goods_id desc; 
- +----+-------------+-------+------+---------------+---------+---------+-------+------+-------------+ 
- | id | select_type | table | type | possible_keys | key     | key_len | ref   | rows | Extra       | 
- +----+-------------+-------+------+---------------+---------+---------+-------+------+-------------+ 
- |  1 | SIMPLE      | shop  | ref  | shop_id       | shop_id | 4       | const |    2 | Using where | 
- +----+-------------+-------+------+---------------+---------+---------+-------+------+-------------+ 
- 1 row in set (0.00 sec) 
对于何时排序优化有效，官方手册上是这样说的：
The index can also be used even if the ORDER BY does not match the index exactly, as long as all of the unused
 portions of the index and all the extra ORDER BY columns are constants in the WHERE clause.
翻译：
即使ORDER BY不精确匹配索引也能使用索引，只要WHERE子句中的所有未使用的索引部分和所有额外的ORDER
 BY列为常数就行。
这句话中有两个细节，如上面标蓝的部分，下面举个例子来说明一下：
比如情况二，
- SELECT * FROM t1 WHERE key_part1=constant ORDER BY key_part2; 
ORDER BY子句是key_part2，并未精确的匹配索引(精确匹配就应当是key_part1,key_part2)，但是where子句中使用了索引的一部分(key_part1)并且为常数，而对于ORDER BY来说，key_part1就是额外的，它不出现在ORDER BY子句中便却是索引的一部分，这样，排序就可以用到索引来优化了。
------------------------------- 这里需要一根分隔线 -------------------------------
手册上还介绍了几种无法应用排序优化的情况，我们来看一下：
无法优化的情况1
- SELECT * FROM t1 ORDER BY key1, key2; 
这种情况是当排序时指定了两个索引时：
- mysql> explain select * from shop order by id,price; 
- +----+-------------+-------+------+---------------+------+---------+------+------+----------------+ 
- | id | select_type | table | type | possible_keys | key  | key_len | ref  | rows | Extra          | 
- +----+-------------+-------+------+---------------+------+---------+------+------+----------------+ 
- |  1 | SIMPLE      | shop  | ALL  | NULL          | NULL | NULL    | NULL |    7 | Using filesort | 
- +----+-------------+-------+------+---------------+------+---------+------+------+----------------+ 
- 1 row in set (0.00 sec) 
本例中用到了两个索引，其中id还是主键，但这也无助于排序优化。这种情况下，即使指定了检索结果集，也无法避免Using filesort。
无法优化的情况2
- SELECT * FROM t1 WHERE key2=constant ORDER BY key_part2; 
当查询条件使用了别的索引，且值为常量，但排序字段是另一个联合索引的非连续部分时：
- mysql> explain select * from shop where price=15 order by goods_id;  
- +----+-------------+-------+------+---------------+-------+---------+-------+------+-----------------------------+ 
- | id | select_type | table | type | possible_keys | key   | key_len | ref   | rows | Extra                       | 
- +----+-------------+-------+------+---------------+-------+---------+-------+------+-----------------------------+ 
- |  1 | SIMPLE      | shop  | ref  | price         | price | 5       | const |    1 | Using where; Using filesort | 
- +----+-------------+-------+------+---------------+-------+---------+-------+------+-----------------------------+ 
- 1 row in set (0.00 sec) 
无法优化的情况3
- SELECT * FROM t1 ORDER BY key_part1 DESC, key_part2 ASC; 
混用两种排序方向时，这种情况如果指定了结果集为主键或联合索引字段，也无法避免Using filesort:
- mysql> explain select * from shop order by shop_id asc,goods_id desc;  
- +----+-------------+-------+------+---------------+------+---------+------+------+----------------+ 
- | id | select_type | table | type | possible_keys | key  | key_len | ref  | rows | Extra          | 
- +----+-------------+-------+------+---------------+------+---------+------+------+----------------+ 
- |  1 | SIMPLE      | shop  | ALL  | NULL          | NULL | NULL    | NULL |    7 | Using filesort | 
- +----+-------------+-------+------+---------------+------+---------+------+------+----------------+ 
- 1 row in set (0.00 sec) 
无法优化的情况4
- SELECT * FROM t1 WHERE key2=constant ORDER BY key1; 
这种情况指的是查询时按索引2，而排序时按索引1，真的是不能优化么？
- mysql> explain select * from shop where pay_type=1 order by price;  
- +----+-------------+-------+------+---------------+----------+---------+-------+------+-----------------------------+ 
- | id | select_type | table | type | possible_keys | key      | key_len | ref   | rows | Extra                       | 
- +----+-------------+-------+------+---------------+----------+---------+-------+------+-----------------------------+ 
- |  1 | SIMPLE      | shop  | ref  | pay_type      | pay_type | 1       | const |    2 | Using where; Using filesort | 
- +----+-------------+-------+------+---------------+----------+---------+-------+------+-----------------------------+ 
- 1 row in set (0.00 sec) 
当我们指定了pay_type为1来检索行，并按price来排序时，发现的确不能优化排序。如果按id来排序呢？
- mysql> explain select * from shop where pay_type=1 order by id;  
- +----+-------------+-------+------+---------------+----------+---------+-------+------+-------------+ 
- | id | select_type | table | type | possible_keys | key      | key_len | ref   | rows | Extra       | 
- +----+-------------+-------+------+---------------+----------+---------+-------+------+-------------+ 
- |  1 | SIMPLE      | shop  | ref  | pay_type      | pay_type | 1       | const |    2 | Using where | 
- +----+-------------+-------+------+---------------+----------+---------+-------+------+-------------+ 
- 1 row in set (0.00 sec) 
很惊奇的发现，排序被优化了。
无法优化的情况5
- SELECT * FROM t1 ORDER BY ABS(key); 
- SELECT * FROM t1 ORDER BY -key; 
这种情况指的是按表达式来排序，为了更直观的看出区别，我们作了三种情况：
1）用主键排序
- mysql> explain select * from shop order by id;  
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------+ 
- | id | select_type | table | type  | possible_keys | key     | key_len | ref  | rows | Extra | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------+ 
- |  1 | SIMPLE      | shop  | index | NULL          | PRIMARY | 4       | NULL |    7 |       | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------+ 
- 1 row in set (0.00 sec) 
2）ABS表达式
- mysql> explain select * from shop order by ABS(id);  
- +----+-------------+-------+------+---------------+------+---------+------+------+----------------+ 
- | id | select_type | table | type | possible_keys | key  | key_len | ref  | rows | Extra          | 
- +----+-------------+-------+------+---------------+------+---------+------+------+----------------+ 
- |  1 | SIMPLE      | shop  | ALL  | NULL          | NULL | NULL    | NULL |    7 | Using filesort | 
- +----+-------------+-------+------+---------------+------+---------+------+------+----------------+ 
- 1 row in set (0.00 sec) 
3）负号
- mysql> explain select * from shop order by -id;  
- +----+-------------+-------+------+---------------+------+---------+------+------+----------------+ 
- | id | select_type | table | type | possible_keys | key  | key_len | ref  | rows | Extra          | 
- +----+-------------+-------+------+---------------+------+---------+------+------+----------------+ 
- |  1 | SIMPLE      | shop  | ALL  | NULL          | NULL | NULL    | NULL |    7 | Using filesort | 
- +----+-------------+-------+------+---------------+------+---------+------+------+----------------+ 
- 1 row in set (0.28 sec) 
可见，用表达式的情况的确不会排序优化。
无法优化的情况6
官方的翻译：当联接了多张表，并且ORDER BY中的列并不是全部来自第1个用于搜索行的非常量表.(这是EXPLAIN输出中的没有使用const联接类型的第1个表）。
分析一下标蓝的部分：
列来自表 -> 
列不是来自表 -> 
列不是全部来自表 -> 
列不是全部来自非常量表 -> 
列不是全部来自第一个用于搜索行的非常量表。
要测试这种情况，我们需要新增一张表：
- CREATE TABLE `pay_type` ( 
-   `type_id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'ID', 
-   `rate` decimal(6,2) NOT NULL COMMENT '费率', 
-   `name` varchar(20) NOT NULL COMMENT '名称', 
-   PRIMARY KEY (`type_id`) 
- ) ENGINE=InnoDBAUTO_INCREMENT=4 DEFAULT CHARSET=utf8COMMENT='支付方式表'
插入几条记录：
- INSERT INTO `pay_type` (`type_id`, `rate`, `name`) VALUES 
- (1, '0.01', '手机'), 
- (2, '0.02', '网银'), 
- (3, '0.00', '货到付款'); 
我们测试一下：
- mysql> explain select * from shop a left join pay_type b on a.pay_type=b.type_id where a.id>2 order by a.goods_id,b.type_id; 
- +----+-------------+-------+--------+---------------+---------+---------+-----------------+------+----------------------------------------------+ 
- | id | select_type | table | type   | possible_keys | key     | key_len | ref             | rows | Extra                                        | 
- +----+-------------+-------+--------+---------------+---------+---------+-----------------+------+----------------------------------------------+ 
- |  1 | SIMPLE      | a     | range  | PRIMARY       | PRIMARY | 4       | NULL            |    5 | Using where; Using temporary; Using filesort | 
- |  1 | SIMPLE      | b     | eq_ref | PRIMARY       | PRIMARY | 4       | test.a.pay_type |    1 |                                              | 
- +----+-------------+-------+--------+---------------+---------+---------+-----------------+------+----------------------------------------------+ 
- 2 rows in set (0.28 sec) 
a表是一个非常量表，并且是执行计划中的第一个，这满足上面所说的表类型。
order by中的列来自于两张表，所以不是全部来自于a表。
对于这样的情况，的确不能做排序优化。
无法优化的情况7
有不同的ORDER BY和GROUP BY表达式。
- mysql> explain select shop_id, max(price) max_price from shop group by shop_id order by max_price desc; 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+---------------------------------+ 
- | id | select_type | table | type  | possible_keys | key     | key_len | ref  | rows | Extra                           | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+---------------------------------+ 
- |  1 | SIMPLE      | shop  | index | NULL          | shop_id | 8       | NULL |    7 | Using temporary; Using filesort | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+---------------------------------+ 
- 1 row in set (0.04 sec) 
当我们按shop_id进行分组后，想按最高价的商品倒排时，可以看到没有排序优化。如果排序的字段与分组的字段一致，都是shop_id，排序优化就生效了。
- mysql> explain select shop_id, max(price) max_price from shop group by shop_id order by shop_id desc; 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------+ 
- | id | select_type | table | type  | possible_keys | key     | key_len | ref  | rows | Extra | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------+ 
- |  1 | SIMPLE      | shop  | index | NULL          | shop_id | 8       | NULL |    7 |       | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------+ 
- 1 row in set (0.01 sec) 
无法优化的情况8
如果指定了索引长度，且索引长度小于字段长度时，不能进行排序优化。
我们需要修改一下上面创建的pay_type表，加一个索引，索引长度是1：
- ALTER TABLE `test`.`pay_type` ADD INDEX `name` ( `name` ( 1 ) )  
然后测试：
- mysql> explain select * from pay_type force index(name) order by name; 
- +----+-------------+----------+------+---------------+------+---------+------+------+----------------+ 
- | id | select_type | table    | type | possible_keys | key  | key_len | ref  | rows | Extra          | 
- +----+-------------+----------+------+---------------+------+---------+------+------+----------------+ 
- |  1 | SIMPLE      | pay_type | ALL  | NULL          | NULL | NULL    | NULL |    3 | Using filesort | 
- +----+-------------+----------+------+---------------+------+---------+------+------+----------------+ 
- 1 row in set (0.00 sec) 
的确，没有排序优化。如果不指定name的长度呢，先修改一下索引：
- ALTER TABLE `test`.`pay_type` DROP INDEX `name` , 
- ADD INDEX `name` ( `name` )  
然后执行：
- mysql> explain select * from pay_type force index(name) order by name; 
- +----+-------------+----------+-------+---------------+------+---------+------+------+-------+ 
- | id | select_type | table    | type  | possible_keys | key  | key_len | ref  | rows | Extra | 
- +----+-------------+----------+-------+---------------+------+---------+------+------+-------+ 
- |  1 | SIMPLE      | pay_type | index | NULL          | name | 62      | NULL |    3 |       | 
- +----+-------------+----------+-------+---------------+------+---------+------+------+-------+ 
- 1 row in set (0.00 sec) 
这样排序优化是有效的。
无法优化的情况9
使用的表索引的类型不能按顺序保存行。例如，对于HEAP表的HASH索引情况即如此。有不同的ORDER BY和GROUP BY表达式
要测试这种情况，我们得新建一张表：
- CREATE TABLE `heap_test` ( 
-   `id` int(10) unsigned NOT NULL AUTO_INCREMENT COMMENT 'ID', 
-   `name` varchar(20) NOT NULL COMMENT '名称', 
-   PRIMARY KEY (`id`), 
-   KEY `name` (`name`) 
- ) ENGINE=MEMORYAUTO_INCREMENT=3 DEFAULT CHARSET=utf8COMMENT='测试heap表'
插入几条记录：
- INSERT INTO `heap_test` (`id`, `name`) VALUES 
- (1, '张三'), 
- (2, '李四'); 
测试一下：
- mysql> explain select * from heap_test force index(name) order by name; 
- +----+-------------+-----------+------+---------------+------+---------+------+------+----------------+ 
- | id | select_type | table     | type | possible_keys | key  | key_len | ref  | rows | Extra          | 
- +----+-------------+-----------+------+---------------+------+---------+------+------+----------------+ 
- |  1 | SIMPLE      | heap_test | ALL  | NULL          | NULL | NULL    | NULL |    2 | Using filesort | 
- +----+-------------+-----------+------+---------------+------+---------+------+------+----------------+ 
- 1 row in set (0.00 sec) 
的确排序优化无效。
除了以上的情况，还有几个要注意的地方。
注意点1
字段别名对排序的影响：
- mysql> explain select abs(shop_id) shop_id from shop force index(shop_id) order by shop_id; 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-----------------------------+ 
- | id | select_type | table | type  | possible_keys | key     | key_len | ref  | rows | Extra                       | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-----------------------------+ 
- |  1 | SIMPLE      | shop  | index | NULL          | shop_id | 8       | NULL |    7 | Using index; Using filesort | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-----------------------------+ 
- 1 row in set (0.00 sec) 
这个例子中我们对shop_id取绝对值并给了一个别名shop_id，不幸的是，这个表中的确有一个字段叫shop_id，此时按shop_id排序，用到的是别名而不是字段，即使强制使用了shop_id索引也无效。对于这种情况，换一个别名就可以解决：
- mysql> explain select abs(shop_id) shop_new_id from shop force index(shop_id) order by shop_id; 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------------+ 
- | id | select_type | table | type  | possible_keys | key     | key_len | ref  | rows | Extra       | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------------+ 
- |  1 | SIMPLE      | shop  | index | NULL          | shop_id | 8       | NULL |    7 | Using index | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+-------------+ 
- 1 row in set (0.00 sec) 
注意点2
group by默认会对字段排序，跟你显示的按分组字段order by一样，写不写出来都一样，没有性能损失。如果想避免group by的排序开销，可以强制指定取消排序，先看一下不取消的情况：
- mysql> explain select goods_id,count(1) from shop force index(shop_id) group by goods_id; 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+----------------------------------------------+ 
- | id | select_type | table | type  | possible_keys | key     | key_len | ref  | rows | Extra                                        | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+----------------------------------------------+ 
- |  1 | SIMPLE      | shop  | index | NULL          | shop_id | 8       | NULL |    7 | Using index; Using temporary; Using filesort | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+----------------------------------------------+ 
- 1 row in set (0.00 sec) 
再看一下取消的情况：
- mysql> explain select goods_id,count(1) from shop force index(shop_id) group by goods_id order by null; 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+------------------------------+ 
- | id | select_type | table | type  | possible_keys | key     | key_len | ref  | rows | Extra                        | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+------------------------------+ 
- |  1 | SIMPLE      | shop  | index | NULL          | shop_id | 8       | NULL |    7 | Using index; Using temporary | 
- +----+-------------+-------+-------+---------------+---------+---------+------+------+------------------------------+ 
- 1 row in set (0.00 sec) 
此时已经没有了Using filesort。
注意点3
如果排序不可避免，可以用下面的办法加速：
- 增加sort_buffer_size变量的大小。
- 增加read_rnd_buffer_size变量的大小。
- 更改tmpdir指向具有大量空闲空间的专用文件系统。
