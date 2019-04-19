# mysql 字符集设置 - fanyun的博客 - CSDN博客
2016年03月18日 13:57:38[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2068
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
一、 **MySQL字符集设置**
（1）       系统变量：
– character_set_server：默认的内部操作字符集
– character_set_client：客户端来源数据使用的字符集
– character_set_connection：连接层字符集
– character_set_results：查询结果字符集
– character_set_database：当前选中数据库的默认字符集
– character_set_system：系统元数据(字段名等)字符集
– 还有以collation_开头的同上面对应的变量，用来描述字符序。
（2）       用introducer指定文本字符串的字符集：
– 格式为：[_charset]’string’ [COLLATE collation]
– 例如：
       SELECT _latin1 ’string’;
       SELECT _utf8 ‘你好’ COLLATE utf8_general_ci;
– 由introducer修饰的文本字符串在请求过程中不经过多余的转码，直接转换为内部字符集处理。
二、 **MySQL中的字符集转换过程**
（1）       MySQL Server收到请求时将请求数据从character_set_client转换为character_set_connection；
（2）       进行内部操作前将请求数据从character_set_connection转换为内部操作字符集，其确定方法如下：
       - 使用每个数据字段的CHARACTER SET设定值；
       - 若上述值不存在，则使用对应数据表的DEFAULT CHARACTER SET设定值(MySQL扩展，非SQL标准)；
       - 若上述值不存在，则使用对应数据库的DEFAULT CHARACTER SET设定值；
       - 若上述值不存在，则使用character_set_server设定值。
三、**MySQL默认字符集**
MySQL对于字符集的指定可以细化到一个数据库，一张表，一列.传统的程序在创建数据库和数据表时并没有使用那么复杂的配置，它们用的是默认的配置.
    (1)      编译MySQL时，指定了一个默认的字符集，这个字符集是 latin1；
    (2)      安装MySQL时，可以在配置文件 (my.ini)中指定一个默认的的字符集，如果没指定，这个值继承自编译时指定的；
    (3)      启动mysqld时，可以在命令行参数中指定一个默认的的字符集，如果没指定，这个值继承自配置文件中的配置,此时
 character_set_server 被设定为这个默认的字符集；
    (4)      安装MySQL选择多语言支持，安装程序会自动在配置文件中把default_character_set设置为
 UTF-8，保证缺省情况下所有的数据库所有表的所有列的都用 UTF-8存储。
四、**修改默认字符集**
(1)        最简单的修改方法，就是修改mysql的my.cnf（这里注意：windows 是my.ini）文件中的字符集键值，
![](https://img-blog.csdn.net/20160303134631036?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160303134644880?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
修改完后，重启mysql的服务
(2)        还有一种修改字符集的方法，就是使用mysql的命令
设置了表的默认字符集为utf8并且通过UTF-8编码发送查询，存入数据库的仍然是乱码。那connection连接层上可能出了问题。解决方法是在发送查询前执行一下下面这句：
 SET NAMES 'utf8';它相当于下面的三句指令：
SET character_set_client = utf8;
SET character_set_results = utf8;
SET character_set_connection = utf8;
![](https://img-blog.csdn.net/20160303135532338?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
五、**使用MySQL字符集时的建议**
（1）建立数据库/表和进行数据库操作时尽量显式指出使用的字符集，而不是依赖于MySQL的默认设置，否则MySQL升级时可能带来很大困扰；
数据库和连接字符集都使用latin1时，虽然大部分情况下都可以解决乱码问题，但缺点是无法以字符为单位来进行SQL操作，一般情况下将数据库和连接字符集都置为utf8是较好的选择；
（2）使用mysql CAPI（mysql提供C语言操作的API）时，初始化数据库句柄后马上用mysql_options设定MYSQL_SET_CHARSET_NAME属性为utf8，这样就不用显式地用SET
 NAMES语句指定连接字符集，且用mysql_ping重连断开的长连接时也会把连接字符集重置为utf8；
（3）对于mysql PHP API，一般页面级的PHP程序总运行时间较短，在连接到数据库以后显式用SET
 NAMES语句设置一次连接字符集即可；但当使用长连接时，请注意保持连接通畅并在断开重连后用SET NAMES语句显式重置连接字符集。
六、**其他注意事项**
                 my.cnf中的default_character_set设置只影响mysql命令连接服务器时的连接字符集，不会对使用libmysqlclient库的应用程序产生任何作用！
对字段进行的SQL函数操作通常都是以内部操作字符集进行的，不受连接字符集设置的影响。
