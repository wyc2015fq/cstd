# MySQL常用数据类型小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年11月01日 09:21:36[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：329
在 MySQL 中，有三种主要的类型：字符串、数字和日期/时间类型。
## 目录
 [[隐藏](http://192.168.1.100/elesos_com/index.php?title=MySQL%E5%B8%B8%E7%94%A8%E6%95%B0%E6%8D%AE%E7%B1%BB%E5%9E%8B%E5%B0%8F%E7%BB%93#)] 
- [1 字符串类型](http://192.168.1.100/elesos_com/index.php?title=MySQL%E5%B8%B8%E7%94%A8%E6%95%B0%E6%8D%AE%E7%B1%BB%E5%9E%8B%E5%B0%8F%E7%BB%93#.E5.AD.97.E7.AC.A6.E4.B8.B2.E7.B1.BB.E5.9E.8B)
- [2 数值类型](http://192.168.1.100/elesos_com/index.php?title=MySQL%E5%B8%B8%E7%94%A8%E6%95%B0%E6%8D%AE%E7%B1%BB%E5%9E%8B%E5%B0%8F%E7%BB%93#.E6.95.B0.E5.80.BC.E7.B1.BB.E5.9E.8B)
- [3 日期和时间类型](http://192.168.1.100/elesos_com/index.php?title=MySQL%E5%B8%B8%E7%94%A8%E6%95%B0%E6%8D%AE%E7%B1%BB%E5%9E%8B%E5%B0%8F%E7%BB%93#.E6.97.A5.E6.9C.9F.E5.92.8C.E6.97.B6.E9.97.B4.E7.B1.BB.E5.9E.8B)
- [4 使用建议](http://192.168.1.100/elesos_com/index.php?title=MySQL%E5%B8%B8%E7%94%A8%E6%95%B0%E6%8D%AE%E7%B1%BB%E5%9E%8B%E5%B0%8F%E7%BB%93#.E4.BD.BF.E7.94.A8.E5.BB.BA.E8.AE.AE)
- [5 艺搜参考](http://192.168.1.100/elesos_com/index.php?title=MySQL%E5%B8%B8%E7%94%A8%E6%95%B0%E6%8D%AE%E7%B1%BB%E5%9E%8B%E5%B0%8F%E7%BB%93#.E8.89.BA.E6.90.9C.E5.8F.82.E8.80.83)
## 字符串类型
- CHAR 0-255字节 定长字符串
- VARCHAR(size) 保存可变长度的字符串。在括号中指定字符串的最大长度。最多 255 个字符。
- TINYTEXT 存放最大长度为 255 个字符的字符串
- TEXT 存放最大长度为 65,535 个字符的字符串。
- MEDIUMTEXT 存放最大长度为 16,777,215 个字符的字符串。
- LONGTEXT 存放最大长度为 4,294,967,295 个字符的字符串。
- BLOB 用于 BLOBs (Binary Large OBjects) 二进制形式的长文本数据。最多存放 65,535 字节的数据。
- MEDIUMBLOB 用于 BLOBs (Binary Large OBjects)。存放最多 16,777,215 字节的数据。
- LONGBLOB 用于 BLOBs (Binary Large OBjects)。存放最多 4,294,967,295 字节的数据。
- ENUM(x,y,z,etc.) 允许你输入可能值的列表。可以在 ENUM 列表中列出最大 65535 个值。
注：这些值是按照你输入的顺序存储的。可以按照此格式输入可能的值：ENUM('X','Y','Z')
- SET 与 ENUM 类似，SET 最多只能包含 64 个列表项。
CHAR(30)占用30个字符。当保存CHAR值时，在它们的右边填充空格以达到指定的长度。
VARCHAR 类型可以根据实际内容动态改变存储值的长度，所以在不能确定字段需要多少字符时使用 VARCHAR 类型可以大大地节约磁盘空间、提高存储效率。
## 数值类型
- TINYINT(size) -128 到 127。0 到 255 无符号。在括号中规定最大位数。
- SMALLINT(size) -32768 到 32767。0 到 65535 无符号。
- MEDIUMINT(size) -8388608 到 8388607 。0 to 16777215 无符号。
- INT(size) -2147483648 到 2147483647 。0 到 4294967295 （42亿，10bit）无符号。
- BIGINT(size) -9223372036854775808 到 9223372036854775807 。0 到 18446744073709551615 （20位）无符号。
- FLOAT(size,d) 在括号中规定最大位数。在 d 参数中规定小数点右侧的最大位数。
- DOUBLE(size,d) 在括号中规定最大位数。在 d 参数中规定小数点右侧的最大位数。
- DECIMAL(size,d) 作为字符串存储的 DOUBLE 类型，允许固定的小数点。
int后面括号中的数字，不表示长度，表示的是显示宽度，这点与 varchar、char 后面的数字含义是不同的。
也就是说不管 int 后面的数字是多少，它存储的范围始终是 -2^31 到 2^31 - 1。整型的数据类型括号内的数字不管是多少，所占的存储空间都是一样。
## 日期和时间类型
- DATE() 日期。格式：YYYY-MM-DD
- DATETIME() 日期和时间的组合。格式：YYYY-MM-DD HH:MM:SS。从 '1000-01-01 00:00:00' 到 '9999-12-31 23:59:59'
## 使用建议
在指定数据类型的时候一般采用从小原则，比如能用TINY INT的最好就不用INT，能用FLOAT类型的就不用DOUBLE类型，这样会对MYSQL在运行效率上提高很大，尤其是大数据量测试条件下。
## 艺搜参考
[http://www.w3school.com.cn/sql/sql_datatypes.asp](http://www.w3school.com.cn/sql/sql_datatypes.asp)
[http://www.w3cschool.cc/mysql/mysql-data-types.html](http://www.w3cschool.cc/mysql/mysql-data-types.html)
[http://www.metsky.com/archives/84.html](http://www.metsky.com/archives/84.html)
[http://blog.csdn.net/jiemushe1814/article/details/4716069](http://blog.csdn.net/jiemushe1814/article/details/4716069)
[http://dev.mysql.com/doc/refman/5.1/zh/column-types.html](http://dev.mysql.com/doc/refman/5.1/zh/column-types.html)
