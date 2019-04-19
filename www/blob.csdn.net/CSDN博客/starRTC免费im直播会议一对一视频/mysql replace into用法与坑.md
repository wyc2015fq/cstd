# mysql replace into用法与坑 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月07日 09:22:57[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1334
需要主键一致
PRIMARY KEY (id)
PRIMARY KEY (id, ts)

坑：
[https://blog.xupeng.me/2013/10/11/mysql-replace-into-trap/](https://blog.xupeng.me/2013/10/11/mysql-replace-into-trap/)
可以看到 MySQL 说 “2 rows affected”，可是明明是只写一条记录，为什么呢？这是因为 MySQL 在执行 REPLACE INTO auto (k) VALUES (1) 时首先尝试 INSERT INTO auto (k) VALUES (1)，但由于已经存在一条 k=1 的记录，发生了 duplicate key error，于是 MySQL 会先删除已有的那条
 k=1 即 id=1 的记录，然后重新写入一条新的记录。
这时候 slave 上出现了诡异的问题：
AUTO_INCREMENT 却并未更新，这会有什么问题呢？把这个 slave 提升为 master 之后，由于 AUTO_INCREMENT 比实际的 next id 还要小，写入新记录时就会发生 duplicate key error，每次冲突之后 AUTO_INCREMENT += 1，直到增长为 max(id) + 1 之后才能恢复正常：
由于新写入记录时并未给 extra 字段指定值，原记录 extra 字段的值就「丢失」了
实际上是，当存在 k=1 的记录时，就把 v 字段的值更新为 ‘1-1’，其他未指定的字段则保持原状，而满足这一需求的 MySQL 方言是 INSERT INTO auto (k, v) VALUES (1, ‘1-1’) ON DUPLICATE KEY UPDATE v=VALUES(v);
参考
[http://dev.mysql.com/doc/refman/5.7/en/replace.html](http://dev.mysql.com/doc/refman/5.7/en/replace.html)
