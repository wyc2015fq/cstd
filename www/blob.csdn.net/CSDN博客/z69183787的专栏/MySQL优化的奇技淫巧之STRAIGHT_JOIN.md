# MySQL优化的奇技淫巧之STRAIGHT_JOIN - z69183787的专栏 - CSDN博客
2016年11月28日 20:29:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1008
转自：http://huoding.com/2013/06/04/261
最近没怎么搞SQL优化，碰巧数据库被慢查询搞挂了，于是拿来练练手。
## 问题
通过「SHOW FULL PROCESSLIST」语句很容易就能查到问题SQL，如下：
SELECT post.*
FROM post
INNER JOIN post_tag ON post.id = post_tag.post_id
WHERE post.status = 1 AND post_tag.tag_id = 123
ORDER BY post.created DESC
LIMIT 100
说明：因为post和tag是多对多的关系，所以存在一个关联表post_tag。
试着用EXPLAIN查询一下SQL执行计划（篇幅所限，结果有删减）：
+----------+---------+-------+-----------------------------+
| table    | key     | rows  | Extra                       |
+----------+---------+-------+-----------------------------+
| post_tag | tag_id  | 71220 | Using where; Using filesort |
| post     | PRIMARY |     1 | Using where                 |
+----------+---------+-------+-----------------------------+
下面给出优化后的SQL，唯一的变化就是把连接方式改成了「STRAIGHT_JOIN」：
SELECT post.*
FROM post
STRAIGHT_JOIN post_tag ON post.id = post_tag.post_id
WHERE post.status = 1 AND post_tag.tag_id = 123
ORDER BY post.created DESC
LIMIT 100
试着用EXPLAIN查询一下SQL执行计划（篇幅所限，结果有删减）：
+----------+----------------+--------+-------------+
| table    | key            | rows   | Extra       |
+----------+----------------+--------+-------------+
| post     | status_created | 119340 | Using where |
| post_tag | post_id        |      1 | Using where |
+----------+----------------+--------+-------------+
对比优化前后两次EXPLAIN的结果来看，优化后的SQL虽然「rows」更大了，但是没有了「Using filesort」，综合来看，性能依然得到了提升。
提醒：注意两次EXPLAIN结果中各个表出现的先后顺序，稍后会解释。
## 解释
对第一条SQL而言，为什么MySQL优化器选择了一个耗时的执行方案？对第二条SQL而言，为什么把连接方式改成STRAIGHT_JOIN之后就提升了性能？
这一切还得从MySQL对多表连接的处理方式说起，首先MySQL优化器要确定以谁为驱动表，也就是说以哪个表为基准，在处理此类问题时，MySQL优化器采用了简单粗暴的解决方法：哪个表的结果集小，就以哪个表为驱动表，当然MySQL优化器实际的处理方式会复杂许多，具体可以参考：[MySQL优化器如何选择索引和JOIN顺序](http://www.orczhou.com/index.php/2013/04/how-mysql-choose-index-in-a-join/)。
说明：在EXPLAIN结果中，第一行出现的表就是驱动表。
继续post连接post_tag的例子，MySQL优化器有如下两个选择，分别是：
- 
以post为驱动表，通过status_created索引过滤，结果集119340行
- 
以post_tag为驱动表，通过tag_id索引过滤，结果集71220行
显而易见，post_tag过滤的结果集更小，所以MySQL优化器选择它作为驱动表，可悲催的是我们还需要以post表中的created字段来排序，也就是说排序字段不在驱动表里，于是乎不可避免的出现了「Using filesort」，甚至「Using temporary」。
知道了来龙去脉，优化起来就容易了，要尽可能的保证排序字段在驱动表中，所以必须以post为驱动表，于是乎必须借助「STRAIGHT_JOIN」强制连接顺序。
实际上在某些特殊情况里，排序字段可以不在驱动表里，比如驱动表结果集只有一行记录，并且在连接其它表时，索引除了连接字段，还包含了排序字段，此时连接表后，索引中的数据本身自然就是排好序的。
既然聊到这里顺带说点题外话，大家可能会遇到类似下面的问题：原本运行良好的查询语句，过了一段时间后，可能会突然变得很糟糕。一个很大可能的原因就是数据分布情况发生了变化，从而导致MySQL优化器对驱动表的选择发生了变化，进而出现索引失效的情况，所以没事最好多查查，关注一下这些情况。
…
对于「STRAIGHT_JOIN」，我总觉得这种非标准的语法属于奇技淫巧的范畴，能不用尽量不用，毕竟多数情况下，MySQL优化器都能做出正确的选择。
