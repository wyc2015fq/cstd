# MySql使用心得 - z69183787的专栏 - CSDN博客
2016年11月29日 14:51:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：631
**主键索引**
MySQL会以主键的?方式构造?一棵树，叶?子节点存放该主键对应的整?行数据
**辅助索引**
辅助索引的树，叶?子节点存放两个东?西，?一个是索引?自?身的值，另外?一个是索引对应主键的值；!
联合索引，比如col1和col2的联合索引，索引叶?子节点会存储col1和col2之间的配对+主键的配对数据;
**MyISAM索引**
MyISAM引擎使?用B+Tree作为索引结构，叶节点的data域存放的是数据记录的地址。
**InnoDB聚集索引**
虽然InnoDB也使?用B+Tree作为索引结构，但具体实现?方式却与MyISAM截然不同。InnoDB的数据?文件本索引?文件，这个索引的key是数据表的主键。
**聚集索引规则**
一个表只能有一个聚集索引
INNODB选取聚集索引列的顺序是：
1. 如果声明了主键(primary key)，则这个列会被做为聚集索引。
2. 如果没有声明主键，则会用一个唯一且不为空的索引列作为主键，成为此表的聚集索引。
3. 上面二个条件都不满足，则MySQL自动生成一个隐含字段作为主键，这个字段长度为6个字节，类型为长整形。
尽量使用自增列做INNODB主键
**InnoDB辅助索引**
InnoDB的辅助索引data域存储相应记录主键的值?而不是地址。换句话说，InnoDB的所有
辅助索引都引?用主键作为data域。
**索引**
尽可能使用联合索引，Index（a，b，c）会覆盖 index(a)、index(a, b)
示例：
PRIMARY KEY(ID),
UNIQUE KEY IX_diashId_shopkey_isspecial (DishID,ShopKey,IsSpecialOffer),
KEY IX_ShopKey_valid (ShopKey,Valid),
KEY IX_diashId_shopkey_special (DiashID,ShopKey,IsSpecialOffer,Valid),
KEY IX_UpdateTime (UpdatedTime)
t：Index（a，b，c），t1：index（b，c）
Select * from t where a = ?
Select * from t where a = ? and b = ?
Select * from t where b = and c = ?
Select * from t where a > and b = ?
Select * from t where a = order by b
Select * from t where a = and b > ?
Select * from t where a = and b > ? and b < ? order by c
官网优化文档：[http://dev.mysql.com/doc/refman/5.5/en/order-by-optimization.html](http://dev.mysql.com/doc/refman/5.5/en/order-by-optimization.html)
Range 查询优化
Join 优化
排序优化
等
**索引建立的原则**
 使用区分度高的列作为索引
区分度越高，索引树的分叉也就越多，一次性找到的概率也就越高。
 尽量使用字段长度小的列作为索引
使用数据类型简单的列（int 型，固定长度）
 选用NOT NULL的列
含有空值的列很难进行查询优化，用0、一个特殊的值或者一个空串代替空值。
 尽量的扩展索引，不要新建索引
比如表中已经有a的索引，现在要加(a,b)的索引，那么只需要修改原来的索引即
可。这样也可避免索引重复。
**索引的限制**
 如果不是按照索引的最左列开始查找，则无法使用索引。
 不能跳过索引中的列，否则只能用到索引前面的部分。
 如果查询中有某个列的范围查询，则其右边所有的列都无法用到索引优化。
 索引列不能参与计算，另当like通配符在最左如：like’%dd’,not in,!=,<>等运算符都不会使用索引。
只要是查询语句，就建上索引吗
否。索引会使数据修改操作变慢，还会在硬盘上占?用相当?大空间。MySQL在运行时也要消耗资源维护索引，因此索引并不是越多越好。
哪种情况不建议建索引
表记录较少，如2000行以下。
索引的选择性较低。Index Selectivity = count(distinct column)/count(*)
**Explain：执行计划**
select_type 
 SIMPLE ：简单查询，查询子句不包含UNION或子查询
 PRIMARY：最外层的SELECT子句
 SUBQUERY ：子查询中第一个SELECT
 DERIVED ：衍生表的SELECT子句（from 后的子查询）
 UNION ： UNION子句右侧的SELECT子句
Keep it SIMPLE！
type
从优到差排序如下：
const：根据主键或唯一键进行查询，最多只匹配一行
eq_ref：多表的join时，被驱动表的过滤字段是主键或唯一索引，查询效率很好
ref：索引键值查询，根据普通索引进?行查询或join
range：索引范围查询，常用于in, >, <, between
index：全索引扫描查询，索引没起到过滤作用
all：全表扫描查询
and…
possible_keys：优化器可能用到的索引
key：实际用到的索引，没有则为null
key_len：使用了索引的字节数（对同一个索引这个值越大说明索引中字段利用率越高）
rows：预估检查行数（这个值越小越好）
**Extra 1**
 Using Index：查询在索引树中就可以找到所有数据，不用回主键索引树查找，往往说明性能不错!
 Using filesort：MySQL需要进行额外的排序来获取数据，无法通过索引顺序达到排序效果（黑名单，绝对禁止）
 Using temporary：MySQL需要创建临时表来存放数据，一般出现于排序，分组和多表join的情况（黑名单，绝对禁止）
**执行计划的原则**
 查询类型保持为SIMPLE
 避免index和all
 确保用到索引，key不能为NULL
 检查行数rows越少越好
 禁止temporary和filesort
**DB拆分**
先垂直拆分，再水平拆分
水平拆分：
容量
分布式事务
拆分基于准确的容量分析，不要为了拆分而拆分
**垂直拆分**
垂直拆分：按照不同的表来切分到不同的库（优先）
**⽔水平拆分**
⽔水平拆分：将同⼀一个表的数据按照某种规则拆分到多库

