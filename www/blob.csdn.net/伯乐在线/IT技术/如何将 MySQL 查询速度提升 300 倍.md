# 如何将 MySQL 查询速度提升 300 倍 - 文章 - 伯乐在线
原文出处： [opensource](https://opensource.com/article/17/5/speed-your-mysql-queries-300-times)   译文出处：[开源中国](https://www.oschina.net/translate/speed-your-mysql-queries-300-times?print)
在进行性能分析前，需要先定位耗时查询。
MySQL 提供了内建的耗时查询日志。使用前，打开 **my.cnf **文件，将**slow_query_log **设置为”On”。 同时将 **long_query_time **设置为一个对一次查询来说比较慢的时间（秒数），比如 0.2。**slow_query_log_file **设置为你想保存日志文件的路径。然后运行代码，执行时间超过设定上限的查询将会被记录到日志中。
一旦定位这些烦人的查询，就可以探究慢的原因。MySQL提供了**EXPLAIN**关键字。可以与**SELECT、****DELETE、****INSERT、****REPLACE、****UPDATE**语句一起使用。只需要如下一样添加到查询开始处：


```
EXPLAIN SELECT picture.id, picture.title
 FROM picture
 LEFT JOIN album ON picture.album_id = album.id WHERE album.user_id = 1;
```
结果是对数据访问过程的解析。每行代表一个查询相关的数据表：![An explanation of how data is accessed](http://jbcdn2.b0.upaiyun.com/2017/05/ec8027ae03fe43fb6cc2b8fd31cad241.png)
重点是表名、使用的key、查询执行时所扫描的行数。![The important pieces here are the table name, the key used, and the number of rows scanned.](http://jbcdn2.b0.upaiyun.com/2017/05/a56b96248706d91e9179efb1b76fa54a.png)
这个查询扫描了picture表2百万行，对每个picture记录扫描了album表2万行。意味着，该查询实际扫描了album表400亿行。这个过程可以更高效。
## 索引
使用索引，可以极大提升性能。比如地址簿中的名字。找一个名字，你可以从头翻到尾，也可以找到相应的字符标签页来快速定位。
使用索引可以避免无必要的表扫描。比如在**picture.album_id**上添加索引：


```
ALTER TABLE picture ADD INDEX(album_id);
```
现在执行查询，就不会扫描整个picture列表了。首先，所有的album表会被扫描以找到属于该用户的album，然后，使用**album_id**列索引快速定位对应的picture。扫描的行数减少到了20万，比原始查询快了317倍。
![The query is also about 317 times faster than the original.](http://jbcdn2.b0.upaiyun.com/2017/05/2bd465bc36d78d15022ee192e4297ad6.png)
按照以下方式添加索引可以确保两张表都能使用对应键：


```
ALTER TABLE album ADD INDEX(user_id);
```
![Make sure that both tables use a key](http://jbcdn2.b0.upaiyun.com/2017/05/9b83ba9ee464210cb1d5a0e966041536.png)
这次，album表不会被全部扫描，通过**user_id**键就可以精确定位相应的album。然后这100个album被扫描后，通过**album_id**键相关的picture也可以快速找到。每张表都用到了一个键进行查询性能优化，结果比原始查询快了380倍。
但是并不意味着可以随意添加索引，因为索引会导致写数据库用时增长。所以使用索引导致读用时缩短，写用时增长。因此，如果能提升读性能时再添加索引。使用**EXPLAIN**来确认索引是否被查询用到，没有就删除。
