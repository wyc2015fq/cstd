# MySQL 执行计划(Using where,Using index 和 Using index condition) - z69183787的专栏 - CSDN博客
2016年11月29日 15:24:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：14934
关于执行计划的 Extra 字段，对这几个取值有一些疑惑，我说一下我的大致理解。
- 
Using where：表示优化器需要通过索引回表查询数据；
- 
Using index：表示直接访问索引就足够获取到所需要的数据，不需要通过索引回表；
- 
Using index condition：在5.6版本后加入的新特性（Index Condition Pushdown）;
![](https://segmentfault.com/img/bVrL3o) Using index condition 会先条件过滤索引，过滤完索引后找到所有符合索引条件的数据行，随后用 WHERE 子句中的其他条件去过滤这些数据行；
- 
Using where && Using index：这个确实不了解它和 Using index condition 的区别。
然后，我在 MySQL 的示例数据库 Sakila 中做了一些测试，但是结果却让我感到非常疑惑：
> 
测试使用 Sakila.rental 表，表中的 customer_id 建立了名为 idx_fk_customer_id 索引。
```
```
-- 第一个 SQL 语句
EXPLAIN SELECT customer_id FROM rental WHERE customer_id>=300;
```
```
![](https://segmentfault.com/img/bVrL3E)
结果是使用了 idx_fk_customer_id 索引，但是 Extra 信息竟然为 Using where;Using index;在这个 SQL 语句中，SELECT 子句 和 WHERE 子句不是都是可以从索引中过滤并取得的吗，为什么Extra 的值不是 Using index 呢？
```
```
-- 第二个 SQL 语句
EXPLAIN SELECT * FROM rental WHERE customer_id>=373;
```
```
![](https://segmentfault.com/img/bVrL3O)
这回更奇怪了，因为 SELECT 语句中包含索引中不存在的数据，所以需要通过索引回表查询数据，所以 Extra 为 Using where 我可以理解，但是这里竟然 type 竟然为 ALL，也就说执行计划中使用的是全表扫描！这又是为什么呢？
```
```
-- 第三个 SQL 语句
EXPLAIN SELECT customer_id FROM rental WHERE customer_id>=373 AND customer_id<400;
```
```
![](https://segmentfault.com/img/bVrL30)
这个语句的 Extra 值同样为 Using where;Using index
```
```
-- 第四个 SQL 语句
EXPLAIN SELECT * FROM rental WHERE customer_id>=373 AND customer_id<400;
```
```
![](https://segmentfault.com/img/bVrL4b)
这个语句的执行计划就比较好理解了，先使用 cusomter_id>373 或者 customer_id<400 中的一个条件过滤索引，过滤完索引后，通过索引回表扫描并再次过滤掉一部分信息，随后返回最终的结果，Extra 为 Using index condition.
> 
还望各位大神能不吝解答，或者您可以指点我最关心的三个问题：1.Using where && Using index 和 Using index condition 的区别；
> 
2.为什么EXPLAIN SELECT customer_id FROM rental WHERE customer_id>=300;会使用索引， 而 EXPLAIN SELECT * FROM rental WHERE customer_id>=300; 则不会使用索引呢？
> 
EXPLAIN SELECT * FROM rental WHERE customer_id>=300 AND customer_id<=350;会使用索引
EXPLAIN SELECT * FROM rental WHERE customer_id>=300 AND customer_id<=476;则不会使用索引
索引对 RANGE 值范围有要求吗？
customer_id 是 SMALLINT(5) 类型的
- 
我对 Using index condition 的理解是, er, 首先 mysql server 和 storage engine 是两个组件, server 负责 sql的parse, 执行; storage engine 去真正的 做 数据/index的 读取/写入. 以前是这样: server 命令 storage engine 按 index 把相应的 数据 从 数据表读出, 传给server, server来按
 where条件 做选择; 现在 ICP则是在 可能的情况下, 让storage engine 根据index 做判断, 如果不符合 条件 则无须 读 数据表. 这样 节省了disk IO. 
[https://dev.mysql.com/doc/refman/5.6/en/index-condition-pushdown-optimization.html](https://dev.mysql.com/doc/refman/5.6/en/index-condition-pushdown-optimization.html)
- 
不用index 因为 你 是 `select *`,
 而且你的where 是 >=, mysql 如果用index查找 则 会有 太多的 random disk IO. 所以它选择了 全表读.
[https://dev.mysql.com/doc/refman/5.6/en/how-to-avoid-table-scan.html](https://dev.mysql.com/doc/refman/5.6/en/how-to-avoid-table-scan.html)
> 
You are using a key with low cardinality (many rows match the key
value) through another column. In this case, MySQL assumes that by
using the key it probably will do many key lookups and that a table
scan would be faster.
- 
查以下 `customer_id>=300 AND customer_id<=350;` 和 
```
customer_id>=300
 AND customer_id<=476;
```
 分别有多少数据. 我 觉得 第一个的数据 可能 比较少.
3Q，差不多搞清楚啦，只是 Using where;Using index 和 Using index condition 还有一些疑问，待我再查查手册。。
> 
1.Using index condition 和 Using where;Using index 的区别
[http://stackoverflow.com/questions/28759576/mysql-using-index-condition-vs-using-where-using-index](http://stackoverflow.com/questions/28759576/mysql-using-index-condition-vs-using-where-using-index)
Using index 和 Using where;Using index 的区别;
[http://stackoverflow.com/questions/25672552/whats-the-difference-between-using-index-and-using-where-using-index-in-the](http://stackoverflow.com/questions/25672552/whats-the-difference-between-using-index-and-using-where-using-index-in-the)
2.这跟您说得差不多，但是并不是由于 where 是 >= ；
> 
优化器会在索引存在的情况下，通过符合 RANGE 范围的条数和总数的比例来选择是使用索引还是进行全表遍历
例如，在 rental 表中，表的总行数为 16044 行；
```
```
-- 不使用索引
EXPLAIN SELECT * FROM rental WHERE customer_id>492;
-- 使用索引
EXPLAIN SELECT * FROM rental WHERE customer_id>493;
-- 其中 id > 492 的行数为 2772, id > 493 的行数为 2749
```
```
```
```
-- 不使用索引
EXPLAIN SELECT * FROM rental WHERE customer_id<103;
-- 使用索引
EXPLAIN SELECT * FROM rental WHERE customer_id<102;
-- 其中 id < 103 的行数为 2767, id < 102 的行数为 2734
```
```
```
```
-- 不使用索引，count(*) 为 2758 条
EXPLAIN SELECT * FROM rental WHERE customer_id>100 AND customer_id < 202;
-- 使用索引, count(*) 为 2733 条
EXPLAIN SELECT * FROM rental WHERE customer_id>100 AND customer_id < 201;
```
```
> 
结论：当需要读取的数据超过一个临界值时，优化器会放弃从索引中读取而改为进行全表扫描，这是为了避免过多的 random disk.
