# MySql避免重复插入记录方法(ignore,Replace,ON DUPLICATE KEY UPDATE) - z69183787的专栏 - CSDN博客
2016年11月18日 11:19:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：641
本文章来给大家提供三种在mysql中避免重复插入记录方法，主要是讲到了ignore,Replace,ON DUPLICATE KEY UPDATE三种方法，各位同学可尝试参考。
**案一：使用ignore关键字**
如果是用主键primary或者唯一索引unique区分了记录的唯一性,避免重复插入记录可以使用：
|代码如下|复制代码|
|----|----|
|1 INSERT IGNORE INTO `table_name` (`email`, `phone`, `user_id`) VALUES ('test9@163.com', '99999', '9999');| |
这样当有重复记录就会忽略,执行后返回数字0
还有个应用就是复制表,避免重复记录：
|代码如下|复制代码|
|----|----|
|1 INSERT IGNORE INTO `table_1` (`name`) SELECT `name` FROM `table_2`;| |
**方案二：使用Replace**
语法格式：
|代码如下|复制代码|
|----|----|
|REPLACE INTO `table_name`(`col_name`, ...) VALUES (...);REPLACE INTO `table_name` (`col_name`, ...) SELECT ...;REPLACE INTO `table_name` SET `col_name`='value',| |
...[算法](http://lib.csdn.net/base/datastructure)说明：
REPLACE的运行与INSERT很相像,但是如果旧记录与新记录有相同的值，则在新记录被插入之前，旧记录被删除，即：
尝试把新行插入到表中 
当因为对于主键或唯一关键字出现重复关键字错误而造成插入失败时： 
从表中删除含有重复关键字值的冲突行 
再次尝试把新行插入到表中 
旧记录与新记录有相同的值的判断标准就是：
表有一个PRIMARY KEY或UNIQUE索引，否则，使用一个REPLACE语句没有意义。该语句会与INSERT相同，因为没有索引被用于确定是否新行复制了其它的行。
返回值：
REPLACE语句会返回一个数，来指示受影响的行的数目。该数是被删除和被插入的行数的和
受影响的行数可以容易地确定是否REPLACE只添加了一行，或者是否REPLACE也替换了其它行：检查该数是否为1（添加）或更大（替换）。
示例:
# eg:(phone字段为唯一索引)
|代码如下|复制代码|
|----|----|
|REPLACE INTO `table_name` (`email`, `phone`, `user_id`) VALUES ('test569', '99999', '123');| |
另外,在 SQL Server 中可以这样处理：
|代码如下|复制代码|
|----|----|
|if not exists ([select](http://www.111cn.net/tags.php/select/) phone from t where phone= '1')   insert into t(phone, update_time) values('1', getdate()) else    update t set update_time = getdate() where phone= '1'| |
更多信息请看：http://dev.[MySQL](http://lib.csdn.net/base/mysql).com/doc/refman/5.1/zh/sql-syntax.html#replace
**方案三：ON DUPLICATE KEY UPDATE**
如‍上所写，你也可以在INSERT INTO…..后面加上 ON DUPLICATE KEY UPDATE方法来实现。如果您指定了ON DUPLICATE KEY UPDATE，并且插入行后会导致在一个UNIQUE索引或PRIMARY KEY中出现重复值，则执行旧行UPDATE。
例如，如果列a被定义为UNIQUE，并且包含值1，则以下两个语句具有相同的效果：
|代码如下|复制代码|
|----|----|
|INSERT INTO `table` (`a`, `b`, `c`) VALUES (1, 2, 3) ON DUPLICATE KEY UPDATE `c`=`c`+1; UPDATE `table` SET `c`=`c`+1 WHERE `a`=1;| |
如果行作为新记录被插入，则受影响行的值为1；如果原有的记录被更新，则受影响行的值为2。
注释：如果列b也是唯一列，则INSERT与此UPDATE语句相当：
|代码如下|复制代码|
|----|----|
|UPDATE `table` SET `c`=`c`+1 WHERE `a`=1 OR `b`=2 LIMIT 1;| |
如果a=1 OR b=2与多个行向匹配，则只有一个行被更新。通常，您应该尽量避免对带有多个唯一关键字的表使用ON DUPLICATE KEY子句。
您可以在UPDATE子句中使用VALUES(col_name)函数从INSERT…UPDATE语句的INSERT部分引用列值。换句话说，如果没有发生重复关键字冲突，则UPDATE子句中的VALUES(col_name)可以引用被插入的col_name的值。本函数特别适用于多行插入。VALUES()函数只在INSERT…UPDATE语句中有意义，其它时候会返回NULL。
|代码如下|复制代码|
|----|----|
|INSERT INTO `table` (`a`, `b`, `c`) VALUES (1, 2, 3), (4, 5, 6) ON DUPLICATE KEY UPDATE `c`=VALUES(`a`)+VALUES(`b`);| |
本语句与以下两个语句作用相同：
|代码如下|复制代码|
|----|----|
|INSERT INTO `table` (`a`, `b`, `c`) VALUES (1, 2, 3) ON DUPLICATE KEY UPDATE `c`=3; INSERT INTO `table` (`a`, `b`, `c`) VALUES (4, 5, 6) ON DUPLICATE KEY UPDATE c=9;| |
注释：当您使用ON DUPLICATE KEY UPDATE时，DELAYED选项被忽略。
示例：
这个例子是我在实际项目中用到的：是将一个表的数据导入到另外一个表中，数据的重复性就得考虑(如下)，唯一索引为：email：
|代码如下|复制代码|
|----|----|
|INSERT INTO `table_name1` (`title`, `first_name`, `last_name`, `email`, `phone`, `user_id`, `role_id`, `status`, `campaign_id`)     SELECT '', '', '', `table_name2`.`email`, `table_name2`.`phone`, NULL, NULL, 'pending', 29 FROM `table_name2`     WHERE `table_name2`.`status` = 1 ON DUPLICATE KEY UPDATE `table_name1`.`status`='pending'| |
再贴一个例子：
|代码如下|复制代码|
|----|----|
|1 INSERT INTO `class` SELECT * FROM `class1` ON DUPLICATE KEY UPDATE `class`.`course`=`class1`.`course`| |
其它关键：DELAYED  做为快速插入，并不是很关心失效性，提高插入性能。 
IGNORE  只关注主键对应记录是不存在，无则添加，有则忽略。
更多信息请看:  http://dev.mysql.com/doc/refman/5.1/zh/sql-syntax.html#insert
特别说明：在MYSQL中UNIQUE索引将会对null字段失效，也就是说(a字段上建立唯一索引)：
|代码如下|复制代码|
|----|----|
|1 INSERT INTO `test` (`a`) VALUES (NULL);| |
是可以重复插入的（联合唯一索引也一样）。
原文地址：http://www.111cn[.NET](http://lib.csdn.net/base/dotnet)/database/mysql/50135.htm
