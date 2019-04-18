# mysql中orderby和limit同时使用的bug - z69183787的专栏 - CSDN博客
2016年09月07日 15:47:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2000
官网地址：[http://bugs.mysql.com/bug.php?id=32933](http://bugs.mysql.com/bug.php?id=32933)
以下转载：[http://bbs.chinaunix.net/thread-1276235-1-1.html](http://bbs.chinaunix.net/thread-1276235-1-1.html)
我的SQL很简单
我想从一个表中检索所有标题含有“中国”的数据，将它们按id排序，取前5条，所以我写了以下语句
SQL语句1：
select fi_id, fi_url, fi_title
from InfoTable
where fi_title like '%中国%'
order by fi_id desc
limit 5
当我只使用order by时，能够返回13条结果
只使用limit时，能够返回其中的5条结果
但是当我同时使用order by和limit时，却返回0条结果
同时我们还发现，这种情况并不是在所有检索词上都会出现，如把检索词由“中国”换为“足球”等就不会出现这种现象。
在和同事的讨论中，有人提出了以下方法
SQL语句2：
select * from
(select fi_id, fi_url, fi_title
from InfoTable
where fi_title like '%中国%'
order by fi_id desc ) a
limit 5
语句2能够正确的得到5条结果
这给了我们一些启发：语句1和语句2的执行并不相同，所以语句1很可能与我们最初设想不符，所以，我们对语句1进行了以下修改
limit 5 ---> limit 50000
得到SQL语句3：
select fi_id, fi_url, fi_title
from InfoTable
where fi_title like '%中国%'
order by fi_id desc
limit 50000
此SQL语句能够正确得到13条结果
由此我们推测：在同时使用order by和limit时，MySQL进行了某些优化，将语句执行逻辑从"where——order by——limit"变成了"order by——limit——where"，导致在某些情况下，同时使用order by和limit会导致查无结果（或结果数量不足）。具体出现问题与否是与表中数据有关的。
