# MYSQL的用户变量(@)和系统变量(@@) - Machine Learning with Peppa - CSDN博客





2018年01月03日 22:14:21[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：240
所属专栏：[深入浅出数据库与Mysql](https://blog.csdn.net/column/details/18665.html)









## 用户变量


可以先在用户变量中保存值然后在以后引用它；这样可以将值从一个语句传递到另一个语句。用户变量与连接有关。也就是说，一个客户端定义的变量不能被其它客户端看到或使用。当客户端退出时，该客户端连接的所有变量将自动释放。





用户变量的形式为@var_name，其中变量名var_name可以由当前字符集的文字数字字符、‘.’、‘_’和‘$’组成。
 默认字符集是cp1252 (Latin1)。可以用mysqld的--default-character-set选项更改字符集，用户变量名对大小写不敏感。





设置用户变量的一个途径是执行SET语句：
SET @var_name = expr [, @var_name = expr] ...




对于SET，可以使用=或:=作为分配符。分配给每个变量的expr可以为整数、实数、字符串或者NULL值。也可以用语句代替SET来为用户变量分配一个值。在这种情况下，分配符必须为:=而不能用=，因为在非SET语句中=被视为一个比较操作符：
mysql> SET @t1=0, @t2=0, @t3=0;mysql> SELECT @t1:=(@t2:=1)+@t3:=4,@t1,@t2,@t3;+----------------------+------+------+------+| @t1:=(@t2:=1)+@t3:=4 | @t1  | @t2  | @t3  |+----------------------+------+------+------+|                    5 |    5 |    1 |    4 |+----------------------+------+------+------+




用户变量可以用于表达式中。目前不包括明显需要文字值的上下文中，例如SELECT语句的LIMIT子句，或者LOAD DATA语句的IGNORE number LINES子句。


注意以下两点:


如果使用没有初始化的变量，其值是NULL。


如果用户变量分配了一个字符串值，其字符集和校对规则与该字符串的相同。





用户变量的可压缩性（coercibility）是隐含的。(即为表列值的相同的可压缩性（coercibility）。

注释：在SELECT语句中，表达式发送到客户端后才进行计算。这说明在HAVING、GROUP
 BY或者ORDER BY子句中，不能使用包含SELECT列表中所设的变量的表达式。





例如，下面的语句不能按期望工作：
mysql> SELECT (@aa:=id) AS a，(@aa+3) AS b 从tbl_name HAVING b=5；
HAVING子句中引用了SELECT列表中的表达式的别名，使用@aa。不能按期望工作：@aa不包含当前行的值，而是前面所选的行的id值。








一般原则是不要在语句的一个部分为用户变量分配一个值而在同一语句的其它部分使用该变量。可能会得到期望的结果，但不能保证。


设置变量并在同一语句中使用它的另一个问题是变量的默认结果的类型取决于语句前面的变量类型。下面的例子说明了该点：
mysql> SET @a='test';mysql> SELECT @a,(@a:=20) FROM tbl_name;




对于该 SELECT语句，MySQL向客户端报告第1列是一个字符串，并且将@a的所有访问转换为字符串，即使@a在第2行中设置为一个数字。执行完SELECT语句后，@a被视为下一语句的一个数字。


要想避免这种问题，要么不在同一个语句中设置并使用相同的变量，要么在使用前将变量设置为0、0.0或者''以定义其类型。


未分配的变量有一个值NULL，类型为字符串。




系统变量





MySQL可以访问许多系统和连接变量。当服务器运行时许多变量可以动态更改。这样通常允许你修改服务器操作而不需要停止并重启服务器。mysqld服务器维护两种变量。全局变量影响服务器整体操作。会话变量影响具体客户端连接的操作。





当服务器启动时，它将所有全局变量初始化为默认值。这些默认值可以在选项文件中或在命令行中指定的选项进行更改。服务器启动后，通过连接服务器并执行SET GLOBAL var_name语句，可以动态更改这些全局变量。要想更改全局变量，必须具有SUPER权限。


服务器还为每个连接的客户端维护一系列会话变量。在连接时使用相应全局变量的当前值对客户端的会话变量进行初始化。对于动态会话变量，客户端可以通过SET SESSION var_name语句更改它们。设置会话变量不需要特殊权限，但客户端只能更改自己的会话变量，而不能更改其它客户端的会话变量。








对于全局变量的更改可以被访问该全局变量的任何客户端看见。然而，它只影响更改后连接的客户的从该全局变量初始化的相应会话变量。不影响目前已经连接的客户端的会话变量(即使客户端执行SET GLOBAL语句也不影响)。


可以使用几种语法形式来设置或检索全局或会话变量。下面的例子使用了sort_buffer_sizeas作为示例变量名。





要想设置一个GLOBAL变量的值，使用下面的语法：
mysql> SET GLOBAL sort_buffer_size=value;mysql> SET @@global.sort_buffer_size=value;




要想设置一个SESSION变量的值，使用下面的语法：
mysql> SET SESSION sort_buffer_size=value;mysql> SET @@session.sort_buffer_size=value;mysql> SET sort_buffer_size=value;
LOCAL是SESSION的同义词。





如果设置变量时不指定GLOBAL、SESSION或者LOCAL，默认使用SESSION。参见[13.5.3节，“SET语法”](http://dev.mysql.com/doc/refman/5.1/zh/sql-syntax.html#set-option)。


要想检索一个GLOBAL变量的值，使用下面的语法：
mysql> SELECT @@global.sort_buffer_size;mysql> SHOW GLOBAL VARIABLES like 'sort_buffer_size';




要想检索一个SESSION变量的值，使用下面的语法：
mysql> SELECT @@sort_buffer_size;mysql> SELECT @@session.sort_buffer_size;mysql> SHOW SESSION VARIABLES like 'sort_buffer_size';

这里，LOCAL也是SESSION的同义词。





当你用SELECT @@var_name搜索一个变量时(也就是说，不指定global.、session.或者local.)，MySQL返回SESSION值（如果存在），否则返回GLOBAL值。


对于SHOW VARIABLES，如果不指定GLOBAL、SESSION或者LOCAL，MySQL返回SESSION值。





当设置GLOBAL变量需要GLOBAL关键字但检索时不需要它们的原因是防止将来出现问题。





如果我们移除一个与某个GLOBAL变量具有相同名字的SESSION变量，具有SUPER权限的客户可能会意外地更改GLOBAL变量而不是它自己的连接的SESSION变量。





如果我们添加一个与某个GLOBAL变量具有相同名字的SESSION变量，想更改GLOBAL变量的客户可能会发现只有自己的SESSION变量被更改了。




