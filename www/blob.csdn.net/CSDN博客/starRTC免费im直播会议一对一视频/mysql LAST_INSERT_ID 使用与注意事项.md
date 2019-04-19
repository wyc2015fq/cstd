# mysql LAST_INSERT_ID 使用与注意事项 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年09月07日 10:44:49[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：6142
在使用MySQL时，若表中含自增字段（auto_increment类型），则向表中insert一条记录后，可以调用last_insert_id()来获得最近insert的那行记录的自增字段值

$mdb->lastInsertId();
但事实上，使用last_insert_id()时有很多注意事项，否则很容易踩到坑。
若在同一条insert语句中插入多行（如"insert into tbl_name (col_a, col_b) values ('aa', 'bb'), ('aaa', 'bbb')"这类SQL语句），则last_insert_id()返回的自增字段的"当前值"只在旧值的基础上加1，这与实际情况不符（表中的实际情况是自增字段值在旧值基础上加N）！
3. 假设用形如"INSERT ... ON DUPLICATE KEY UPDATE"的SQL语句更新表，此时，若该语句的实际作用是insert操作时，调用last_insert_id()会返回本次insert后自增字段的当前值；而若该语句的实际作用是update操作时，调用last_insert_id()返回的是自增字段的旧值，而非当前更新行的自增字段值，所以这个值无意义（因为调用last_insert_id()是想获取sql影响到的行的自增字段值进而做其它逻辑业务的，如果得到的值并非sql操作影响到的行对应的自增值，则这个值对业务来说无意义），按照[MySQL手册](http://dev.mysql.com/doc/refman/5.0/en/insert-on-duplicate.html)的说明，若业务开发者想得到实际update操作影响到的行的自增值，可以用形如"INSERT
 INTO table (a,b,c) VALUES (1,2,3) ON DUPLICATE KEY UPDATE id=LAST_INSERT_ID(id), c=3;"的SQL语句来获取。
关于带参数的last_insert_id()函数的使用说明，可以参考本文第8条的说明。
若在SQL中显式指定自增字段的值，如假设某张表由两列(id,
 name)构成，其中id为自增类型，假设当前表中id值为2，那么，执行"insert into test_tbl (id, name) values (11, 'test3');"后，再执行"select last_insert_id()"，可以发现，得到的结果依旧是2。也即，只有自增字段由mysql来分配时，last_insert_id()才可能得到正确的值；SQL中显式更新自增字段值时，last_insert_id()返回的值不可用！
如果sql语句执行出错，则调用last_insert_id()的值未定义。例如，若事务因执行出错回滚，则last_insert_id()的值不会恢复到事务执行前的那个值。
last_insert_id()的值是由MySQL server来维护的，而且是为每条连接维护独立的值，也即，某条连接调用last_insert_id()获取到的值是这条连接最近一次insert操作执行后的自增值，该值不会被其它连接的sql语句所影响。这个行为保证了不同的连接能正确地获取到它最近一次insert
 sql执行所插入的行的自增值，也就是说，last_insert_id()的值不需要通过加锁或事务机制来保证其在多连接场景下的正确性。
如果通过"insert ignore"语句尝试插入新纪录，假设由于unique key冲突导致插入不成功，则auto_increment计数器不会变化，根据MySQL手册的说明，此时调用last_insert_id()会返回0表示没有新行被插入。但我在MySQL
 5.1.73版本上测试的结果显示，last_insert_id()只是维持旧值而已，并不会返回0。
8. last_insert_id(expr)的行为
若调用last_insert_id()时传入了参数，则它会将参数值返回给调用者，并记住这个值，下次调用不带参数的last_insert_id()时，仍会返回这个值。可以利用这个特性实现一个多用户安全的全局计数器，示例如下：
1) 先创建一张表
mysql> CREATE TABLE
sequence (id INT NOT NULL); 
mysql> INSERT INTO sequence VALUES (0);
2) 每条连接执行下面的SQL语句来获取为其自动分配的全局唯一ID
mysql> UPDATE sequence SET id=LAST_INSERT_ID(id+1);
mysql> SELECT LAST_INSERT_ID();
每次都会id+1
当然，上面给出的全局ID分配器只是一种思路，在实际工程实现中，频繁的update操作可能会造成系统瓶颈，可以参考[PERCONA
 MYSQL PERFORMANCE BLOG](http://www.percona.com/blog/2007/07/01/implementing-efficient-counters-with-mysql/)这篇文章的优化思路（比如MySQL前面用cache抗read压力，通过delay update来减缓update压力）。
[https://www.percona.com/blog/2007/07/01/implementing-efficient-counters-with-mysql/](https://www.percona.com/blog/2007/07/01/implementing-efficient-counters-with-mysql/)
 一个不错的mysql博客，有时间再翻翻了。

参考：
[http://dev.mysql.com/doc/refman/5.7/en/information-functions.html#function_last-insert-id](http://dev.mysql.com/doc/refman/5.7/en/information-functions.html#function_last-insert-id)
[http://blog.csdn.net/slvher/article/details/42298355](http://blog.csdn.net/slvher/article/details/42298355)
