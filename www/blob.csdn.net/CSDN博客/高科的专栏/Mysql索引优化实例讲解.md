
# Mysql索引优化实例讲解 - 高科的专栏 - CSDN博客

2012年09月18日 13:55:23[高科](https://me.csdn.net/pbymw8iwm)阅读数：5749标签：[mysql																](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)[优化																](https://so.csdn.net/so/search/s.do?q=优化&t=blog)[sql																](https://so.csdn.net/so/search/s.do?q=sql&t=blog)[数据库																](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)[date																](https://so.csdn.net/so/search/s.do?q=date&t=blog)[磁盘																](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=date&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)个人分类：[MYSQL																](https://blog.csdn.net/pbymw8iwm/article/category/1240437)
[
																								](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=sql&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=sql&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=优化&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
MYSQL描述：
一个文章库，里面有两个表：category和article。category里面有10条分类数据。article里面有20万条。article里面有一个"article_category"字段是与category里的"category_id"字段相对应的。article表里面已经把 article_category字义为了索引。数据库大小为1.3G。
问题描述：
执行一个很普通的查询： SELECT * FROM `article` WHERE article_category=11 ORDER BY article_id DESC LIMIT 5 。执行时间大约要5秒左右
解决方案：
建一个索引：create index idx_u on article (article_category,article_id);
SELECT * FROM `article` WHERE article_category=11 ORDER BY article_id DESC LIMIT 5 减少到0.0027秒

继续问题：
SELECT * FROM `article` WHERE article_category IN (2,3) ORDER BY article_id DESC LIMIT 5 执行时间要11.2850秒。
使用OR:
select * from article
where article_category=2
or article_category=3
order by article_id desc
limit 5
执行时间：11.0777
解决方案：避免使用in 或者 or (or会导致扫表)，使用union all

使用UNION ALL：
(select * from article where article_category=2 order by article_id desc limit 5)
UNION ALL (select * from article where article_category=3 order by article_id desc limit 5)
ORDER BY article_id desc
limit 5
执行时间：0.0261
注:UNION 和UNION ALL 的区别
在数据库中，UNION和UNION ALL关键字都是将两个结果集合并为一个，但这两者从使用和效率上来说都有所不同。
UNION在进行表链接后会筛选掉重复的记录，所以在表链接后会对所产生的结果集进行排序运算，删除重复的记录再返回结果。
实际大部分应用中是不会产生重复的记录，最常见的是过程表与历史表UNION。如：
select * from gc_dfys union select * from ls_jg_dfys
这个SQL在运行时先取出两个表的结果，再用排序空间进行排序删除重复的记录，最后返回结果集，如果表数据量大的话可能会导致用磁盘进行排序。
而UNION ALL只是简单的将两个结果合并后就返回。这样，如果返回的两个结果集中有重复的数据，那么返回的结果集就会包含重复的数据了。
从效率上说，UNION ALL 要比UNION快很多，所以，如果可以确认合并的两个结果集中不包含重复的数据的话，那么就使用UNION ALL，如下：
select * from gc_dfys union all select * from ls_jg_dfys
注：**mysql中union all的order by问题**
今天写mysql数据库代码的时候，发现union的结果不是预期的
$stime = date("H:i:s");
$sql1 = "select * from T where  '$stime'>stime order by stime desc";
$sql2 = "select * from T where  stime>'$stime' order by stime asc";
$sql = "($sql) union  all ($sql2)";
分别执行$sql1 和 $sql2 的时候结果是对的
但是执行$sql的时候，发现结果反了，$sql1的部分变升序，$sql2的部分变成降序
搜索也没有得到满意的答案，好像有些数据库还是不支持字句order by 的
无意中发现这样可以，
$sql = "select * from ($sql1) as temp1 union all select * from ($sql2) as temp2";
这是因为你的union的用法不正确的原因。在union操作中，order by语句不能出现在由union操作组合的两个select语句中。排序可以通过在第二个select语句后指定order by子句。


