# MYSQL索引 - 建建的博客 - CSDN博客
2017年03月15日 10:59:30[纪建](https://me.csdn.net/u013898698)阅读数：87
个人分类：[Mysql](https://blog.csdn.net/u013898698/article/category/6790249)

索引的类型
- 索引优化应该是对查询性能优化最有效的手段了。
- mysql只能高效地使用索引的最左前缀列。
- mysql中索引是在存储引擎层而不是服务器层实现的
B-Tree索引
B-Tree通常意味着所有的值都是按顺序存储的，并且每一个叶子页到根的距离相同。
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160721211316388-637070407.png)
图中根节点没有画出来。
B-Tree对索引列是顺序组织存储的，索引很适合查找范围数据。
B-Tree索引的限制
- 如果不是按照索引的最左列开始查找，则无法使用索引。
- 不能跳过索引中的列
- 如果查询中有某列的范围查询，则其右边所有列都无法使用索引优化查询。
这些限制都和索引列的顺序存储有关系。或者说是索引顺序存储导致了这些限制。
哈希索引（hash index）
哈希索引基于哈希表实现的，只有精确匹配索引所有列的查询才有效。
对于每一行数据，存储引擎都会对所有的索引列计算一个哈希值（hash code），哈希值是一个较小的值，并且不同键值的行计算出来的哈希值不一样。哈希索引将所有的哈希值存储在索引中，同时保存指向每个数据行的指针，这样就可以根据，索引中寻找对于哈希值，然后在根据对应指针，返回到数据行。
mysql中只有memory引擎显式支持哈希索引，innodb是隐式支持哈希索引的。
哈希索引限制：
- 哈希索引只包含哈希值和行指针，不存储字段值，所以不能使用"覆盖索引"的优化方式，去避免读取数据表。
- 哈希索引数据并不是按照索引值顺序存储的，索引也就无法用于排序
- 哈希索引页不支持部分索引列匹配查找，因为哈希索引始终是使用索引列的全部内容计算哈希值的。
- 哈希索引只支持等值比较查询，包括=，in(),<=>，不支持任何范围查询。列入where price>100
- 访问哈希索引的数据非常快，除非有很多哈希冲突（不同的索引列值却有相同的哈希值）
- 如果哈希冲突很多的话，一些索引维护操作的代价也会很高。
因为这些限制，哈希索引只适用于某些特定的场合。而一旦适合哈希索引，则它带来的性能提升将非常显著。
innodb引擎有一个特殊的功能“自适应哈希索引”，当innodb注意到一些索引值被使用的非常频繁时，且符合哈希特点（如每次查询的列都一样），它会在内存中基于B-Tree索引之上再创建一个哈希索引。这是一个完全自动的，内部行为。
创建自定义哈希索引，像模拟innodb一样创建哈希索引。
例如只需要很小的索引就可以为超长的键创建索引。
思路：在B-Tree基础上创建一个伪哈希索引。这和真正的哈希索引不是一回事，因为还是使用B-Tree进行查找，但是它使用哈希值而不是键本身进行索引查找。需要做的就是在查询的where 子句中手动指定使用哈希函数。
例子：
如果需要存储大量的url，并需要根据url进行搜索查找。如果使用B-Tree来存储URL，存储的内容就会很大，因为URL本身都很长。正常情况下会有如下查询：
```
mysql> select id from url where url='http://www.mysql.com';
```
若删除原来url列上的索引，而新增一个被索引的url_crc列，使用crc32做哈希。就可以实现一个伪哈希索引；查询就变成下面的方式：
```
mysql> select id from url where url='http://www.mysql.com'
        -> and url_crc=crc32("http://www.mysql.com");
```
这样性能会提高很多。
当然这样实现的缺陷是需要维护哈希值，就是url改变对应哈希值也应该改变。可以手动维护，当然最好是使用触发器实现。
创建URL表
```
create table URL （
     id  int unsigned NOT NULL auto_increment,
     url varchar(**255**) NOT NULL,
     url_crc  int unsigned NOT NULL DEFAULT **0**,
     PRIMARY KEY (id),
     KEY (url_crc)
）;
```
创建触发器：
```
delimiter //
create trigger url_hash_crc_ins before insert on URL FOR EACH ROW BEGIN
SET NEW.url_crc=crc32(NEW.url);
END;
//
 
CREATE TRIGGER url_hash_crc_upd BEFORE UPDATE ON URL FOR EACH ROW BEGIN
SET NEW.url_crc=crc32(NEW.url);
END;
//
 
delimiter ;
 
mysql> select * from URL;
+----+-----------------------+------------+
| id | url                   | url_crc    |
+----+-----------------------+------------+
|  **1** | htttp://www.mysql.com | **1727608869** |
+----+-----------------------+------------+
**1** row in set (**0.00** sec)
 
mysql> insert into URL(url) values('htttp://www.');
Query OK, **1** row affected (**0.00** sec)
 
mysql> select * from URL;
+----+-----------------------+------------+
| id | url                   | url_crc    |
+----+-----------------------+------------+
|  **1** | htttp://www.mysql.com | **1727608869** |
|  **2** | htttp://www.          | **1196108391** |
+----+-----------------------+------------+
**2** rows in set (**0.00** sec)
 
mysql> UPDATE URL SET url='http://www.baidu.com' where id=**2**;
Query OK, **1** row affected (**0.00** sec)
Rows matched: **1**  Changed: **1**  Warnings: **0**
 
mysql> select * from URL;
+----+-----------------------+------------+
| id | url                   | url_crc    |
+----+-----------------------+------------+
|  **1** | htttp://www.mysql.com | **1727608869** |
|  **2** | http://www.baidu.com  | **3500265894** |
+----+-----------------------+------------+
**2** rows in set (**0.00** sec)
```
如果采用这种方式，不要使用SHA1()和MD5()作为哈希函数，应该这个函数计算出来的哈希值是非常长的字符串，会浪费大量空间，比较时页回更慢。
而如果数据表非常大，crc32()会出现大量的哈希冲突，而解决哈希冲突，可以在查询中增加url本身，进行进一步排除；
如下面查询就可以解决哈希冲突的问题：
```
mysql> select id from url where url='http://www.mysql.com'
        -> and url_crc=crc32("http://www.mysql.com");
```
空间数据索引（R-Tree）
myisam 表支持空间索引，可以用作地理数据存储。
全文索引
全文索引是一种特殊类型的索引，它查找的是文本中的关键词，而不是直接比较索引中的值。第7章中会详细介绍
索引的优点
- 索引大大减少了服务器需要扫描的数据量
- 索引可以帮助服务器避免排序和临时表
- 索引可以将随机I/O变成顺序I/O
索引只要帮助存储引擎快速查找到记录，带来的好处大于其带来的额外工作时，索引才是有效的。对于非常小的表，就不适合索引。因为全表扫描来的更直接，索引还需要维护，开销也不小。
而对于特大型的表，建立和使用索引的代价随之增长。这种情况下，则需要一种技术可以直接区分出查询需要的一组数据，而不是一条记录。例如可以使用分区，或者可以建立元数据信息表等。对于TP级别的数据，定位单条记录的意义不大，索引经常会使用块级别元数据技术来替代索引。
高性能的索引策略
正确地创建和使用索引是实现高性能查询的基础。
 1 独立的列
“独立的列”是指索引列不能是表达式的一部分，也不能是函数的参数。
例如：下面则无法使用actor_id列的索引：
```
mysql> select actor_id from sakila.actor where actor_id + **1** = **5**
```
而下面的actor_id 列的索引则会被使用
```
mysql> select actor_id from sakila.actor where actor_id = **5** - **1**
```
2 前缀索引和索引选择性
前缀的选择性计算：
```
mysql> select count(DISTINCT city)/count(*) from table_name
前缀去重数 除 总数。
 
 
mysql> select
count(DISTINCT LEFT(city,**3**)) / count(*) AS sel3,                
count(DISTINCT LEFT(city,**4**)) / count(*)  AS sel4,
count(DISTINCT LEFT(city,**5**)) / count(*) AS sel5,
count(DISTINCT LEFT(city,**6**)) / count(*) AS sel6,
count(DISTINCT LEFT(city,**7**)) / count(*)  AS sel7
from city;
+--------+--------+--------+--------+--------+
| sel3   | sel4   | sel5   | sel6   | sel7   |
+--------+--------+--------+--------+--------+
| **0.7633** | **0.9383** | **0.9750** | **0.9900** | **0.9933** |
+--------+--------+--------+--------+--------+
```
可以看到当前缀长度达到6之后，选择性提升的幅度已经很小了。
因此选择前缀长度为6；
前缀索引是一种能使索引更小，更快的有效办法，但也是有缺点的：
mysql无法使用前缀索引做order by 和group by，也无法使用前缀索引做覆盖扫描。
3 多列索引
在多个列上建立的单列索引大部分情况下并不能提高mysql的查询性能。mysql5.0以后引入了一种叫"索引合并(index merge)"的策略，一定程度上可以使用表上的多个单列索引来定位指定的行。
例子：表film_actor在字段film_id 和 actor_id上各有一个单列索引。
```
mysql> show create table film_actor;
| film_actor | CREATE TABLE `film_actor` (
  `actor_id` smallint(**5**) unsigned NOT NULL,
  `film_id`  smallint(**5**) unsigned NOT NULL,
  `last_update` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`actor_id`),
  KEY `idx_fk_film_id` (`film_id`),
  CONSTRAINT `fk_film_actor_actor` FOREIGN KEY (`actor_id`) REFERENCES `actor` (`actor_id`) ON UPDATE CASCADE,
  CONSTRAINT `fk_film_actor_film`  FOREIGN KEY (`film_id`) REFERENCES `film` (`film_id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 |
 
mysql> explain select film_id,actor_id from film_actor where actor_id=**1** or film_id =**1**\G;
*************************** **1**. row ***************************
           id: **1**
  select_type: SIMPLE
        table: film_actor
         type: index_merge
possible_keys: PRIMARY,idx_fk_film_id
          key: PRIMARY,idx_fk_film_id
      key_len: **2**,**2**
          ref: NULL
         rows: **29**
        Extra: Using union(PRIMARY,idx_fk_film_id); Using where
```
可以看到使用合并索引（index_merge）技术，优化了此次查询；
实际上也说明了表上的索引建得很糟糕，不然就不用系统优化了；
合并索引有三个变种：OR条件的联合(union),and条件的相交(intersection),组合前两种情况的联合以及相交。
- 当出现服务器对多个索引做相交操作时（通常有多个AND条件），通常意味着需要一个包含所有相关列的多列索引，而不是多个独立的单列索引。
- 当服务器需要对多个索引做联合操作时（通常有多个OR条件），通常需要耗费大量CPU和内存资源在算法的缓存，排序，和合并操作上。特别是当其中有些索引的选择性不高，需要合并扫描返回大量数据的时候。
- 更重要的是，优化器不会把这些计算到"查询成本(cost)"中，优化器只关心随机页面读取。
总之如果在explain中看到索引合并，应该好好检查一下查询和表的结构，看是不是已经是最优的。也可以通过optimizaer_switch来关闭索引合并功能。也可以使用INGORE INDEX提示 让优化器忽略掉某些索引。
4 选择合适的索引列顺序
正确的顺序依赖于使用该索引的查询，并且同时需要考虑如何更好地满足排序和分组的需要。
在一个多列BTree索引中，索引列的顺序意味着索引首先按照最左列进行排序，其次是第二列等待。所以，索引可以按照升序或者降序进行扫描，以满足精确符合列顺序的ORDER BY ,GROUP BY,DISTINCT等子句的查询需求。
当不需要考虑排序和分组时，将选择性最高的列放在前面通常是很好的。这时候索引的作用只是用于优化where条件的查询。
以下面的查询为例：
```
mysql> select * from payment where staff_id =**2** and customer_id=**584**;
```
是应该创建一个(staff_id,customer_id)索引还是应该颠倒一下？可以跑一些查询来确定在这个表中值的分布情况，并确定哪个列的选择性更高。
```
mysql> select sum(staff_id=**2**),sum(customer_id=**584**) from payment \G;
*************************** **1**. row ***************************
     sum(staff_id=**2**): **7992**
sum(customer_id=**584**): **30**
**1** row in set (**0.04** sec)
```
应该讲customer_id放在前面，因为对于条件值的customer_id数量更小。
```
mysql> select sum(staff_id=**2**) from payment where customer_id=**584** \G;
*************************** **1**. row ***************************
sum(staff_id=**2**): **17**
**1** row in set (**0.00** sec)
```
可以看到custmoer_id=584时staff_id=2 只有17个；
需要注意，查询结果非常依赖于选定的具体指定值；
当然还可以使用计算两参数的选择性，来确定哪个参数放在前面：
```
mysql> select count(DISTINCT staff_id) / count(*) AS staff_id_first, count(DISTINCT customer_id) / count(*) AS customer_id_first from payment\G
*************************** **1**. row ***************************
   staff_id_first: **0.0001**
customer_id_first: **0.0373**
```
显然customer_id的选择性（列去重数  除  所有列总数） 更好，
索引列的基数（即特定条件下的数量），会影响索引性能；
尽管关于选择性和基数的经验法则值得去研究和分析，但一定要记住where 子句中的排序，分组和范围条件等其他因素，这些因素可能对查询的性能造成非常大的影响。
5  聚簇索引
聚簇索引并不是一种单独的索引类型，而是一种数据存储方式。
innodb的聚簇索引实际上在同一结构中保存了BTree索引和数据行。（主键是BTree索引+记录是数据行）
当表有聚簇索引时，它的数据行实际上存放在索引的叶子页中。术语"聚簇"表示数据行和相邻的键值紧凑地存储在一起。
下图展示了聚簇索引中的记录是如何存放的。注意到，叶子页包含了行的全部数据，但节点页只包含了索引列。在这个案例中，索引列包含的是整数值。
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160721211948497-25617729.png)
innodb通过主键聚集数据，上图中的"被索引的列"就是主键列。
聚集的优点：
- 可以把相关数据保存在一起。减少磁盘I/O
- 数据访问更快
- 使用覆盖索引扫描的查询可以直接使用页节点中的主键值
聚集的缺点：
- 聚簇数据最大限度地提高了I/O密集型应用的性能，但如果数据全部都放在内存中，则访问的顺序就没有那么重要了，聚簇索引也就没什么优势了。
- 插入速速严重依赖于插入顺序。
- 更新聚簇索引列的代价很高。
- 出入新行或者主键更新需要移动时，可能面临"页分裂(page split)"问题。当行的主键值要求必须插入到某个已满的页中时，存储引擎会将该页分裂成两个页面来容纳该行，这就是一次页分裂操作。页分裂会导致表占用更多的磁盘空间。
- 二级索引（非聚簇索引）即普通索引，在其叶子节点包含了引用行的主键列。
innodb和myisam的数据分布对比：
```
crate table layout_test(
     col1 int NOT NULL,
     col2 int NOT NULL,
     PRIMARY KEY(col1),
     KEY(col2)
);
```
假设col1 取值1--10000，按照随机顺序插入。col2取值从1--100之间随机赋值，所以有很多重复的值。
myisam的数据分布非常简单，按照数据插入的顺序存储在磁盘上。如下图：
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160721212026060-685667643.png)
这种分布方式很容易创建索引，下图，隐藏了页的物理细节，只显示索引中的"节点"
索引中的每个叶子节点包含"行号。表的主键和行号在叶子节点中，且叶子节点根据主键顺序排列。
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160721212101341-352690664.png)
那col2列上的索引又会怎么样呢？有什么特殊吗？答案是否定的，他和其他任何索引一样。
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160721212114544-935245812.png)
事实上，myisam中主键索引和其他索引在结构上没有什么不同。主键索引就是一个名为PRIMARY的唯一非空索引。
innodb的数据分布。因为innodb支持聚簇索引，索引使用非常不同的范式存储同样的数据。看下图：
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160721212130747-1066064623.png)
第一眼看上去，感觉和前面的图5-5没有什么不同，其实该图，显示了整个表，而不是只有索引。因为在innodb中，聚簇索引"就是"表，所以不用想myisam那样需要独立的行存储。
innodb二级索引的叶子节点中存储的不是"行指针"（即不是那个行号），而是主键值，并以此作为指向行的"指针"。这样的策略减少了当出现行移动或者数据页分裂时二级索引的维护工作。当然是用主键值当做 指针会让二级索引占用更多的空间，同时避免了行出现移动或者数据分页时二级索引的维护。
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160721212143044-382441898.png)
聚簇和非聚簇表的对比图
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160721212153716-43964677.png)
innodb 最好主键设置为自增类型 整数；
向聚簇索引插入顺序的索引值
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160721212219013-1275541867.png)
向聚簇索引中插入无序的值：
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160721212237763-1526638674.png)
这样的缺点:
- 写入的目标页可能已经刷新到磁盘上并从缓存中移除，或者还没有加载到缓存中，这样innodb在插入前不得不先找到并从磁盘读取目标页到内存中。导致了大量的随机I/O。
- 因为写入是乱序的，innodb不得不频繁地做页分裂操作，以便为新的行分配空间。页分裂会导致移动大量数据，一次插入最少需要修改三个页而不是一个页。
- 由于频繁的页分裂，页会变得稀疏被不规则地填充，所以最终数据会有碎片。
6 覆盖索引
覆盖索引，一个索引包含所有需要查询的字段的值。
优点：
- 索引条目通常远小于数据行大小，所以如果只需要读取索引，那么mysql就会极大地减少数据访问量。
- 因为索引是按照列值顺序存储的（至少在单个页内是如此），所以对于I/O密集型的范围查询会比随机从磁盘读取每一行数据的I/O要少得多。
- 一些存储引擎如Myisam在内存中只缓存索引，数据则依赖于操作系统来缓存，因此要访问数据需要一次系统调用。
- 由于innodb的聚簇索引，覆盖索引对innodb表特别有用。
使用覆盖索引的情况：
```
mysql> explain select store_id,film_id from inventory \G;
*************************** **1**. row ***************************
           id: **1**
  select_type: SIMPLE
        table: inventory
         type: index
possible_keys: NULL
          key: idx_store_id_film_id
      key_len: **3**
          ref: NULL
         rows: **3496**
        Extra: Using index
**1** row in set (**0.00** sec)
```
此时，有查的字段select store_id,film_id，有一个多列索引idx_store_id_film_id，此时便使用到了覆盖索引，不会再返回数据表去找数据行，因为索引中已经包含了；
假设索引覆盖了where条件中的字段，但是整个查询涉及的字段，来看看会发什么情况，以及如何重写查询以解决该问题。
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160721212312857-1300007022.png)
注意：extra列的"using index" 和type列的"index"不同，type列和覆盖索引毫无关系，它只是表示这个查询访问数据的方式，或者说mysql查找行的方式。而extra列的"using index"则说明。数据使用了 覆盖索引；
上面例子中，使用了ACTOR索引，但是没有使用覆盖索引直接得到数据原因：
- 没有任何索引能够覆盖这个查询。
- mysql能在索引中最左前缀匹配的like比较如"Apoll%",而无法做通配符开头的like 如"%Apoll%"
也有办法可以解决上面说的问题，使其使用覆盖索引。需要重写查询并巧妙地设计索引。先新建一个三个列索引(actor,title,prod_id)；然后重写查询：
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160721212336685-1821364310.png)
我们把这种方式叫做延迟关联(defferred join),因为延迟了对列的访问。
查询在子查询中使用了覆盖索引，并找到了prod_id,然后做了内连接，通过prod_id再去查其他列 会快很多。
当然这一切都要基于 数据集，假设这个products表中有100万行，我们来看一下上面两个查询在三个不同的数据集上的表现，每个数据集都包含100万行：
- 第一个数据集，Sean Carrey 出演了30000部作品，其中有20000部标题包含了Apollo
- 第一个数据集，Sean Carrey 出演了30000部作品，其中有40部标题包含了Apollo
- 第一个数据集，Sean Carrey 出演了50部作品，其中有10部标题包含了Apollo
测试结果：
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160721212352497-1329119532.png)
结果分析：
- 在第一个数据集中：
- - 原查询：从索引actor中读到30000条数据，再根据得到的主键ID回数据表中再读30000条数据；总共读取60000条；
- 优化后的查询：先从索引actor2中读到30000条sena carrey，之后在所有Sean Carrey 中做like 比较 ，找到20000条prod_id；之后还是要回到数据表中，根据prod_id再读取20000条记录；总共读取50000条；
- 分析：总数虽然少了17%，但是子查询中的like比较开销会比较大，相抵之后效率并没有什么提升。
- 在第二个数据集中：
- - 原查询：从索引actor中读到30000条数据，再根据得到的主键ID回数据表中再读30000条数据；总共读取60000条；
- 优化后的查询：先从索引actor2中读到30000条sena carrey，之后在所有Sean Carrey 中做like 比较 ，找到40条prod_id；之后还是要回到数据表中，根据prod_id再读取40条记录；总共读取30040条；
- 分析：读取总数降低了50%, 相比子查询中的开销 还是值得；
- 第三个数据集：显示了子查询效率反而下降的情况。因为索引过滤时符合第一个条件的结果集已经很小，索引子查询带来的成本反而比从表中直接提取完整行更高。
7 使用索引扫描来做排序
（即order by  ，group by 使用到了索引）
mysql设计索引时应该尽量同时满足排序，有又与查找行。
只有当索引的列顺序和order by子句的顺序完全一致，并且所有列的排序方向(倒序或正序)都是一样时，mysql才能使用索引来对结果做排序。
如果查询需要关联多张表，则只有当order by 子句引用的字段全部为一个表时，才能使用索引做排序。
order by 子句满足最左前缀的要求，或者最左前缀为常数，排序方向也要一致；
     idx_a_b (a,b)
     能够使用索引帮助排序的查询：
-      order by a          
- - 满足最左前缀要求           
-      a = 3 order by b
- - 满足最左前缀为常数
-      order by a,b
- - 满足最左前缀要求
-      order by a desc,b desc
- - 满足最左前缀要求
-  a>5 order by a，b
- - 满足最左前缀要求
     不能使用索引帮助排序的查询
-       order by b 
- - 不满足最左前缀要求
-       a >5 order by b
- - 不满足最左前缀，且，最左前缀不是常数
-       a in (1,3) order  by b
- - 不满足最左前缀，且，最左前缀不是常数
-       oder by a asc ,b desc
- - 排序方向不一致
idx_a_b_c(a,b,c)
    where a = 5 order by c
不能使用索引进行排序，不能跨越索引项进行排序；也是一种不满足最左前缀的情况；
8  压缩（前缀压缩）索引
myisam使用前缀压缩来减少索引的大小，从而让更多的索引可以放入内存，这在某些情况下能极大地提升性能。默认只压缩字符串，但通过参数设置也可以对整数压缩。
9  冗余和重复索引
mysql允许在相同列上创建多个索引，但需要单独维护重复的索引，并且优化器在优化查询的时候也需要逐个考虑，这会影响性能。
重复索引：
实际上在ID上建了三个索引，这就是重复索引。
冗余索引：
已有索引(A,B),再重建索引(A)就是冗余索引；
而此时(B,A)，则不是冗余索引。索引(B)也不是索引(A,B)的冗余索引；
已有索引(A),再建索引(A,ID),其中ID是主键，对innodb来说主键列已经包含在二级索引中了，所以这也是冗余索引；
大多数情况都不需冗余索引，应该尽量扩展已有的索引而不是创建新索引。
当然有时候也是需要冗余索引的，因为扩展已有的索引会导致其变得太大，从而影响其他使用该索引的查询的性能。
创建索引
单列索引
create index idx_test1 on tb_student(name);
联合索引
  create index idx_test2 on tb_student(name,age)
索引中先根据name排序，name相同的情况下，根据age排序
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160721212427310-2107006226.png)
设计索引原则：
- 搜索的索引列。
- - 不一定是所要选择的列；即where 后面的查询条件加索引，而不是select 后面的选择列
- 使用唯一索引。
- 使用短索引。
- - 如果对字符串列进行索引，应该指定一个前缀长度，只要有可能就应该这样做。
- 利用最左前缀。
- 不要过度索引
- innodb表，指定主键，并且是自增的最好；
BTREE索引和HASH索引：
- 都可以用在，where col=1 or col in (15,18,20),这样的定值查询中；
- 而在范围查询中，where col>1 and col<10 或者 col like 'ab%' or col between 'lisa' and 'simon';此时只有BTREE索引能使用；HASH索引在这种情况中，不会被使用到，会对全表进行扫描；
维护索引与表
维护索引和表
维护表有三个主要目的：
- 找到并修复损坏的表
- 维护准确的索引统计信息
- 减少碎片
找到并修复损坏的表
check table  tb_name:检查是否发生了表损坏
repair table  tb_name:
更新索引统计信息
mysql优化器通过两个API来了解存储引擎的索引值的分布信息，以决定如何使用索引。
records_in_range():通过向存储引擎传入两个边界值获取在这个范围大概有多少条记录。
info()：该接口返回各种类型的数据，包括索引的基数（每个键值有多少条记录）
mysql优化器使用的是基于成本的模型，而衡量成本的主要指标就是一个查询需要扫描多少行。如果表没有统计信息，或者统计信息不准确，优化器就很可能做出错误的决定。
analyze  table ：重新生成统计信息；
```
mysql> show index from actor\G;
*************************** **1**. row ***************************
        Table: actor
   Non_unique: **0**
     Key_name: PRIMARY
 Seq_in_index: **1**
  Column_name: actor_id
    Collation: A
  Cardinality: **200**
     Sub_part: NULL
       Packed: NULL
         Null:
   Index_type: BTREE
      Comment:
Index_comment:
*************************** **2**. row ***************************
        Table: actor
   Non_unique: **1**
     Key_name: idx_actor_last_name
 Seq_in_index: **1**
  Column_name: last_name
    Collation: A
  Cardinality: **200**
     Sub_part: NULL
       Packed: NULL
         Null:
   Index_type: BTREE
      Comment:
Index_comment:
**2** rows in set (**0.00** sec)
 
Cardinality，显示了存储引擎估算索引列有多少个不同的取值。
 
 mysql5.**6** 以后可以通过参数innodb_analyze_is_persistent，来控制analyze 是否启动；
减少索引和数据的碎片
 
数据碎片三种类型：
行碎片(row fragmentation)
数据行被存储为多个地方的多个片段中。
行间碎片(Intra-row fragmentation)
逻辑上顺序的页，在磁盘上不是顺序存储的。
剩余空间碎片(Free space fragmentation)
数据页中有大量的空余空间。
 
使用命令：
optimize table tb_name，清理碎片。
 
mysql> OPTIMIZE TABLE actor;
+--------------+----------+----------+-------------------------------------------------------------------+
| Table        | Op       | Msg_type | Msg_text                                                          |
+--------------+----------+----------+-------------------------------------------------------------------+
| sakila.actor | optimize | note     | Table does not support optimize, doing recreate + analyze instead |
| sakila.actor | optimize | status   | OK                                                                |
+--------------+----------+----------+-------------------------------------------------------------------+
**2** rows in set (**0.02** sec)
```
对于不支持该命令的引擎可以通过一个不做任何操作（no-op）的alter table 操作来重建表。
```
mysql> alter table actor engine=innodb;
Query OK, **200** rows affected (**0.02** sec)
Records: **200**  Duplicates: **0**  Warnings: **0**
```
索引项的值发生改变，此时索引项在索引表中的位置，就需要发生改变，这样一个行为称为索引维护；
因为如果不进行索引维护的话，就是说索引项的值改变后，并没有重新排序，这样改变项多了之后，就不是一个顺序排序了，就起不到索引的效果了；
- 索引维护由数据库自动完成
- 插入/修改/删除每一个索引行都变成一个内部封装的事务
- 索引越多，事务越长，代价越高
- 索引越多对表的插入和索引字段修改就越慢
假设一个表带了两个索引；
那么系统会总共创建3张表，一个数据表，两个索引表；
在修改一个索引项数据的时候，会内部封装成一个事务，同时这三张表进行修改；
使用索引
    1.使用WHERE查询条件建立索引
    select  a,b  from tab where c=?;
          idx_c (c)
     select a,b from tab where c=? and d=?
          idx_cd(c,d)
     2.排序ORDER BY,GROUP BY,DISTINCT 字段添加索引      ![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160721212548326-412118797.png)
3.联合索引与前缀查询
- 联合索引能为前缀单列，复列查询提供帮助
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160721212611122-958374906.png)
       在mysql5.6前，where a? and c? 只能部分
- 合理创建联合索引，避免冗余
    (a),(a,b),(a,b,c)
    其实只需要对(a,b,c)建立索引即可；
索引与字段选择性
    某个字段其值的重复程度，称为该字段的选择性；
![](http://images2015.cnblogs.com/blog/820365/201607/820365-20160721212628779-1214405589.png)
选择性很差的字段通常不适合创建单列索引
- 男女比例相仿的表中性别不适合创建单列索引
- 如果男女比例极不平衡，要查询的又是少数方（理工院校查女生）可以考虑使用索引
联合索引中选择性好的字段应该排在前面
长字段的索引
- 在非常长的字段上建立索引影响性能
- innodb索引单字段（utf8）只能取前767bytes
- 对长字段处理的方法
         email 类，建立前缀索引
              Mail_addr  varchar(2048)
              idx_mailadd (Mail_addr(30))----只保存前30个字符为索引
              mysql允许对字段进行前缀索引
              对长字段我们也可以主动只取字段的前半部分；   
          住址类，分拆字段
           Home_address  varchar(2048)
           idx_Homeadd (Home_addr(30)) ???? -做前缀索引很可能行不通的，因为很可能前半段都是相同的省市区街道名称
           方法：分拆字段
           Province  varchar(1024), City varchar(1024),District varchar（1024），Local_address varchar(1024)
            然后建立联合索引或单列索引；
索引覆盖扫描(直接使用索引中的数据，不需要从数据表中返回数据)
- 最核心SQL考虑索引覆盖
          select name from tb_user where UserId=?
          Key idx_uid_name(userid,name)
      不需要回表获取name字段，IO最少，效率最高；
无法使用索引
- 索引列进行数学运算或函数运算
          where   id+1 = 10   ×
          where  id = (10-1)  √
          year(col) < 2007  ×
          col < '2007-01-01'√
-  未含复合索引的前缀字段
          idx_abc (a,b,c):
          where b=? and c=? ×
          idx_bc(b,c) √
         注意：idx_adb （a,b,c）包含 idx_a (a),包含idx_ab(a,b),在5.6之后还包含idx_ac（a,c）
-  前缀通配‘_’ 和‘%’通配符   
         LIKE '%XXX%' ×
         LIKE 'XXX%'   √
当使用到 like'%xx%'时，无法使用索引，解决办法是，使用全文索引在5.6之后。或者，使用连接 内层扫描 全索引表，之后找到符合条件的，再回到表中 查找 记录，这样可以降低IO消耗，因为 一般来讲 索引表 比较小，全扫索引表的话相对开销 比 全扫数据表，要小很多；
-  用OR分割开的条件，如果or前的条件中的列有索引，而后面的列中没有索引，那么所涉及的索引都不会被用到。因为后面的查询肯定要走全表扫描，在存在全表扫描的情况下，就没有必要多一次索引扫描增加I/O访问，一次全表扫描过滤条件就足够了。
-   where条件使用NOT,<>,!=
-   字段类型匹配
           并不绝对，但是无法预测地会造成问题，不要使用；
           例子：a int(11) , idx_a (a)；
                     where a = '123' ×
                      where a = 123 √
          由于类型不同，mysql需要做隐式类型转换才能进行比较。
          注意字段的类型，尤其是int型时如果使用字符型去匹配，能得到正确结果，而不会使用索引；同样如果字段是，varchar型，那么where 后面如果是一个 INT，也是不能使用索引；
mysql比较转换规则：
两个参数至少一个是null是不需要转换；
两个参数类型一样时不需要转换；
TIMESTAMP/DATATIME 和 常量 比较-->常量转换为timestamp/datetime
decimal和整数比较---------------------->整数转换为decimal
decimal和浮点数------------------------->decimal转换为浮点数
两个参数都会被转换为浮点数再进行比较：
如果字符串型，比较，=，+，-，等；
一个字符串和一个整形-------------------->均转换成浮点型
```
mysql> select '18015376320243459'=**18015376320243459**;
+---------------------------------------+
| '18015376320243459'=**18015376320243459** |
+---------------------------------------+
|                                     **1** |
 
mysql> select '1801'+**0**;
+----------+
| '1801'+**0** |
+----------+
|     **1801** |
+----------+
```
如果 age int(10), index_age(age);
```
mysql> explain select name from indextest where age='30'\G;
*************************** **1**. row ***************************
           id: **1**
  select_type: SIMPLE
        table: indextest
         type: ref
possible_keys: idx_age
          key: idx_age
      key_len: **1**
          ref: const
         rows: **1**
        Extra:
**1** row in set (**0.00** sec)
```
索引是数值；查询条件是字符串'30',转换成确定数值30，转换使用索引；
索引时字符串，查询是数值时，无法使用索引；
使用cast函数对age做显示的类型转换，会使索引消失；
即对索引项做任何的函数改变，都会使索引失效；
 总结
-  BTREE
- - 存储索引项与主键
- BTREE索引可用在定值查询，范围查询，
-  HASH
- - 存储哈希值与行指针
- 仅用于定值查询，创建伪哈希索引；
-  前缀的选择性计算（去重前缀数除总数）
- - mysql> select count(DISTINCT city)/count(*) from table_name
-  索引合并（index merge）：说明此时表上索引，表结构等需要优化了；
-  选择合适的索引列顺序：需要根据表中实际数据进行选择，选择性高的放在前；
-  聚簇索引：innodb的聚簇索引实际上在同一结构中保存了BTree索引和数据行
-  myisam的数据分布
- -  myisam按照数据插入的顺序存储在磁盘上
-  主键索引时，自动增加行号，表的主键和行号在叶子节点中，且叶子节点根据主键顺序排列；
-  其他列索引和主键索引无区别；
-  innodb数据分布:
- - 使用聚簇索引；
- 二级索引包含索引项和主键值
-  覆盖索引：
- - extra中using index；
- 延迟关联(defferred join)；
- 当然覆盖索引并不是都能提升性能，需要根据集体数据集；
-  使用索引进行排序，不能跨越索引项进行排序；
-  索引维护：由数据库自动完成，将DML封装成内部事务，索引越多代价越高，
-  更新索引统计信息：
- - records_in_range()获取范围中有多少键值，
- info()获取索引基数
-  清理碎片：
- - optimize table tbl,
- alter table tbl engine=innodb;
-  使用索引
- - where
- order by 、group by、distinct,
- 联合索引：注意冗余，选择性好的放在联合索引左侧；
-  长字段的索引：
- - 建立前缀索引
- 分拆字段建立联合索引，
-  无法使用索引：
- - 索引列进行数学运算或函数运算
- 未遵守最左前缀原则
- or条件后一列没有索引
- where条件使用not <> !=
- 字段类型不匹配；
