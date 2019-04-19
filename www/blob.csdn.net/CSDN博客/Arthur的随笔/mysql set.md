# mysql set - Arthur的随笔 - CSDN博客
2013年06月03日 11:14:36[largetalk](https://me.csdn.net/largetalk)阅读数：4476
from: [http://www.itzk.com/thread-582269-53-1.shtml](http://www.itzk.com/thread-582269-53-1.shtml)
本篇文章主要来介绍一下set语句。SET 语句的作用是指定各种选项。最好是省去词OPTION，因为它在MySQL 的未来版本中将被删除。下面为set的语法：
SET [OPTION] option_setting,..
option_setting值可为下列清单中的任一项：
一、PASSWORD [FOR user] = PASSWORD("password " ) 在没有FOR 子句时，设置当前用户的口令为“password”。在有FOR 子句时，设置给定用户的口令。为了能够设置其他用户的口令，必须具有修改mysql 数据库的权限。user是以user_name@host_name 形式，利用与GRANT 语句所接受的user_name和host_name相同类型的值给出的。
SET PASSWORD = PASSWORD("root1234")
SET PASSWORD FOR PAUL = PASSWORD("root1234")
SET PASSWORD FOR PAUL@LOCALHOST = PASSWORD("root1234")
SET PASSWORD FOR bill@%.abc.com = PASSWORD("root1234")
二、CHARACTER SET {charset_name | DEFAULT} 指定客户机所用的字符集。客户机接收和发送的串用这个字符集进行映射。当前可用的惟一字符集是cp1251_koi8。字符集名DEFAULT恢复缺省字符集。
SET CHARACTER SET cp1251_koi8
SET CHARACTER SET DEFAULT
三、SQL_LOG_UPDATE = {0 | 1} 除影响更新日志的记录和不影响常规日志外，此选项类似于SQL_LOG_OFF（包括要求PROCESS 权限）。SQL_LOG_UPDATE 是在MySQL 3.22.5 中引入的。
SET SQL_LOG_UPDATE = 1
四、SQL_SELECT_LIMIT = {n | DEFAULT} 指定从SELECT 语句返回记录的最多个数。在一条语句中直接给出LIMIT 子句将优于此选项。此选项的缺省值为“不限制”。如果曾经做过更改，则设置为DEFAULT 将恢复缺省设置。
SET SQL_SELECT_LIMIT = 100000
SET SQL_SELECT_LIMIT = DEFAULT
五、TIMESTAMP = {timestamp_value | DEFAULT} 指定一个TIMESTAMP 值。此选项用于更新日志处理。
SET TIMESTAMP = DEFAULT
六、SQL_LOW_PRIORITY_UPDATES = {0 | 1} 如果此选项设置为1，修改表内容的语句（DELETE、INSERT、REPLACE、UPDATE）将等待直到无SELECT 语句活动时为止，或等待直到该表挂起为止。在其他语句活动中到达的SELECT 语句立即开始执行而不用等待较低优先级的修改语句。SQL_LOW_PRIORITY_UPDATES 是在MySQL 3.22.5 中引入的。
SET SQL_LOW_PRIORITY_UPDATES = 0
七、LASTINSERT_ID = n 指定LAST_INSERT_ID( ) 返回的值。此选项用于更新日志的处理。
SET LAST_INSERT_ID = 48731
八、SQL_BIG_TABLES = {0 | 1} 如果此选项设置为1，则所有临时表都存放在磁盘上而不是存入在内存中。性能降低，但需要大临时表的SELECT 语句将不会出现“table full（表满）”错误。此选项的缺省设置为0（将临时表装在内存中）。此选项一般对于MySQL 3.23 及以上的版本不需要。
SET SQL_BIG_TABLES = 1
九、SQL_LOG_OFF = {0 | 1} 如果此选项设置为1，当前客户机的需求不记录在通常的日志文件中。如果设置为0，则允许客户机记录日志。如果要此语句起作用，客户机必须具有PROCESS 权限。对更新日志的记录不受影响。
SET SQL_LOG_OFF = 1
十、SQL_WARNINGS = {1 | 0} 如果设置为1，MySQL 即使对单行插入也报告警告计数。一般，只对插入多行的INSERT语句报告警告计数。此选项的缺省设置为0。SQL_WARNINGS 是在MySQL 3.22.11 中引入的。
SET SQL_WARNINGS = 1
十一、SQL_BIG_SELECTS = {0 | 1} 如果此选项设置为1，则允许返回多于max_join_size行的SELECT 语句。否则，查询将被中止。此选项的缺省设置为1。
SET SQL_BIG_SELECTS = 0
十二、INSERT_ID = n 指定下一条INSERT 语句插入一行时，AUTO_INCREMENT 列所用的值。
十三、SQL_AUTO_IS_NULL = {0 | 1} 如果此选项设置为1，最后插入的包含一个AUTO_INCREMENT 值的行可利用WHERE autoinccolIS NULL 形式的WHERE 子句来选择。如Access 这样的ODBC 程序使用此选项。此选项的缺省设置为1，它是在MySQL 3.23.5 中引入的。
SET SQL_AUTO_IS_NULL = 0
