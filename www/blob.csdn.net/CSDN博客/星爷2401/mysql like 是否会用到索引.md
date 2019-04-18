# mysql like  是否会用到索引 - 星爷2401 - CSDN博客
2016年12月21日 11:20:49[星爷2401](https://me.csdn.net/leifukes)阅读数：4395
mysql在使用like查询中，能不能用到索引？在什么地方使用索引呢？
在使用like的时候，如果使用‘%%’，会不会用到索引呢？
EXPLAIN SELECT * FROM `user` WHERE username LIKE '%ptd_%';
 上面的结果是全表扫描(type == ALL)，并没有使用到索引。
只是使用一个%的查询结果：
EXPLAIN SELECT * FROM `user` WHERE username LIKE 'ptd_%';
这个使用到了索引(type == range)。
EXPLAIN SELECT * FROM `user` WHERE username LIKE '%ptd_';
在前面使用%时也是全表扫描，没有使用到索引。
综上，mysql在使用like查询的时候只有不以%开头的时候，才会使用到索引。
