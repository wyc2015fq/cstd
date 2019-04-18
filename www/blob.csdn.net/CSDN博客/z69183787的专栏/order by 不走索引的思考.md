# order by 不走索引的思考 - z69183787的专栏 - CSDN博客
2015年08月29日 09:29:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6140
MySql性能优化，order by 不走索引的思考
2009-07-12 11:43
今天早上查看网站，发现非常慢！进linux 用top查看，发现mysql cpu到了100%。开始怀疑是mysql性能的问题，不会10万条数据就卡成这样吧？虽然我的linux是在服务器上放了个虚拟机，但也不至于10万条记录挂啊？ 网上找了一大把文章，my.cnf也设置了，我虚拟机内存是2G,将key_buf设置成512M 还是卡。非常郁闷！
最后没办法，只能用explain来找原因了。结果还真找到了。所以说用mysql写查询语句，一定要注意索引，一个不小心，性能那是十万八千里。OK,下面分享下查找过程：
首先上原始的查询语句，我只用了一个id字段，方便检查。
explain select id from collect where vtype=1 order by id asc;
+----+-------------+---------+------+---------------+-------+---------+-------+-------+-----------------------------+
| id | select_type | table   | type | possible_keys | key   | key_len | ref   | rows | Extra                       |
+----+-------------+---------+------+---------------+-------+---------+-------+-------+-----------------------------+
| 1 | SIMPLE      | collect | ref | vtype         | vtype | 5       | const | 93237 | Using where; Using filesort |
+----+-------------+---------+------+---------------+-------+---------+-------+-------+-----------------------------+
看上面，最后的Extra 部分：竟然出现 Using filesort ！ 我晕，全表查询。但是 prossible keys 已经使用到了vtype啊？ 看来并不是where 条件的问题。
为了测试，将2条语句分别执行下看：
mysql> select id from collect where vtype=1 order by id asc limit 0,20;
+----+
| id |
+----+
| 1 |
| 2 |
| 3 |
| 4 |
| 5 |
| 6 |
| 7 |
| 8 |
| 9 |
| 10 |
| 11 |
| 12 |
| 13 |
| 14 |
| 15 |
| 16 |
| 17 |
| 18 |
| 19 |
| 20 |
+----+
20 rows in set (10.28 sec)
查找20条数据，花了 10.28 秒！ 非常的晕。
再看：
mysql> select id from collect where vtype=1 limit 0,20;
+----+
| id |
+----+
| 1 |
| 2 |
| 3 |
| 4 |
| 5 |
| 6 |
| 7 |
| 8 |
| 9 |
| 10 |
| 11 |
| 12 |
| 13 |
| 14 |
| 15 |
| 16 |
| 17 |
| 18 |
| 19 |
| 20 |
+----+
20 rows in set (0.01 sec)
少了order by 基本上秒查！
为什么会出现以上问题呢？order by id asc, id 字段可是主键啊，按理说应该是非常快的索引，但是mysql 好像并没有用到。再测试
mysql> explain select * from collect order by id asc;
+----+-------------+---------+------+---------------+------+---------+------+--------+----------------+
| id | select_type | table   | type | possible_keys | key | key_len | ref | rows   | Extra          |
+----+-------------+---------+------+---------------+------+---------+------+--------+----------------+
| 1 | SIMPLE      | collect | ALL | NULL          | NULL | NULL    | NULL | 103997 | Using filesort |
+----+-------------+---------+------+---------------+------+---------+------+--------+----------------+
看上面的分析，我直接select * order by id asc ,也是用到了filesort ，用到了这个肯定查询在10秒以上了。
mysql> explain select id from collect order by id asc;
+----+-------------+---------+-------+---------------+---------+---------+------+--------+-------------+
| id | select_type | table   | type | possible_keys | key     | key_len | ref | rows   | Extra       |
+----+-------------+---------+-------+---------------+---------+---------+------+--------+-------------+
| 1 | SIMPLE      | collect | index | NULL          | PRIMARY | 4       | NULL | 103997 | Using index |
+----+-------------+---------+-------+---------------+---------+---------+------+--------+-------------+
而上面的，将* 改成id ,Using index 了，看来直接查找id 会使性能提高。
mysql> explain select id,url,title from collect order by id asc;
+----+-------------+---------+------+---------------+------+---------+------+--------+----------------+
| id | select_type | table   | type | possible_keys | key | key_len | ref | rows   | Extra          |
+----+-------------+---------+------+---------------+------+---------+------+--------+----------------+
| 1 | SIMPLE      | collect | ALL | NULL          | NULL | NULL    | NULL | 103997 | Using filesort |
+----+-------------+---------+------+---------------+------+---------+------+--------+----------------+
1 row in set (0.00 sec)
而再加上另外的字段，order by 就没任何性能提高了。还是Using fielsort ！
从上面可以看出order by 不能乱用！不知道网上哪个傻逼说用了limit 最好用order by 这样会让分页的性能加快！简直是放屁。经过实际测试，limit 和order by 没有任何关联！而以前写程序的时候经常会用order by id asc 。 可能是数据从来没有上过10万条的缘故，感觉还行，今天用了10万条采集数据做测试，慢得一塌糊涂！
好了，现在仔细想想order by 用到索引的场合：
1） 如果select 只查询索引字段，order by 索引字段会用到索引，要不然就是全表排列；
2） 如果有where 条件，比如where vtype=1 order by vtype asc . 这样order by 也会用到索引！
3） 综上，如果order by 真的影响limit的话，那么就请在没有where 查询的时候order by id(主键), 有where 查询的时候，order by (索引) 字段。
4） 别迷信网上文章，多explain，并且要相信mysql 依然很坚挺，别出现慢的情况就想，免费的性能就这样？
测试继续，到了100条记录，1000万条再来测试mysql 的性能。
测试环境：目前数据量1.4G,10万记录,core 4200,4G内存，vm6.0虚拟机（Linux centos5 , 2G内存，60G硬盘）下测试！
