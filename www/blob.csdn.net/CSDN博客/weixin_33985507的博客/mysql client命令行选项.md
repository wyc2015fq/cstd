# mysql client命令行选项 - weixin_33985507的博客 - CSDN博客
2015年06月01日 10:57:28[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
## **mysql命令行中指定程序选项的规则**
- 
在命令名后面紧跟选项。
- 
选项参量以一个和两个破折号开始，取决于它具有短名还是长名。例如，-?和--help是指导MySQL程序显示帮助消息的短选项和长选项。
- 
选项名对大小写敏感。-v和-V均有效，但具有不同的含义。(它们是--verbose和--version选项的短名）。
- 
部分选项在选项名后面紧随选项值。例如，-hlocalhost或--host=localhost表示客户程序的MySQL服务器主机。选项值可以告诉程序MySQL服务器运行的主机名。
- 
对于带选项值的长选项，通过一个‘=’将选项名和值隔离开来。对于带选项值的短选项，选项值可以紧随选项字母后面，或者二者之间可以用一个空格隔开。(-hlocalhost和-h localhost是等效的）。
最后的规则的例外情况是指定MySQL密码的选项。该选项的形式可以为--password=*pass_val*或--password。在后一种情况(未给出 密码值)，程序将提示输入密码。也可以给出密码选项，短形式为-p*pass_val*或-p。然而，对于短形式，如果给出了 密码值，必须紧跟在选项后面，*中间不能插入空格*。这样要求的原因是如果选项后面有空格，程序没有办法来告知后面的参量是 密码值还是其它某种参量。因此，下面两个命令的含义完全不同：
shell> mysql -ptest
shell> mysql -p test
第一个命令让**mysql**使用密码test，但没有指定默认数据库。第二个命令让**mysql**提示输入 密码并使用test作为默认数据库。
## **连接和断开服务器**
shell> mysql -h host -u user -p
Enter password: ********
host和user分别代表MySQL服务器运行的主机名和MySQL账户用户名。设置时替换为正确的值。******** 代表你的密码；当**mysql**显示Enter password:提示时输入它。
成功地连接后，可以在mysql>提示下输入QUIT (或\q)随时退出：
mysql> QUIT
Bye
在Unix中，也可以按control-D键断开服务器。
## **mysql的-e或--execute选项**
对**mysql**偶尔有用的另一个选项是-e或--execute选项，可用来将SQL语句传递给服务器。该语句必须用引号引起来(单引号或双引号)。(然而，如果想要在查询中将值引起来，则对于查询应使用双引号，查询中引用的值应使用单引号）。
当使用该选项时，语句被执行，然后**mysql**立即退出命令外壳。
例如，你可以用下面的命令获得用户账户列表：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
shell> mysql -u root -p -e "SELECT User, Host FROM User" mysql
Enter password: ******
+------+-----------+
| User | Host      |
+------+-----------+
|      | gigan     |
| root | gigan     |
|      | localhost |
| jon  | localhost |
| root | localhost |
+------+-----------+shell>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
请注意mysql数据库名作为一个独立的参量传递。然而，相同的查询可能已经使用**mysql -u root -p -e "SELECT User，Host FROM mysql.User"**从外壳中执行。
传递多个SQL语句，用分号隔开：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
shell> mysql -u root -p --execute="SELECT Name FROM Country WHERE Name LIKE 'AU%';SELECT COUNT(*) FROM City" world
Enter password: ******
+-----------+
| Name      |
+-----------+
| Australia |
| Austria   |
+-----------+
+----------+
| COUNT(*) |
+----------+
|     4079 |
+----------+
![复制代码](http://common.cnblogs.com/images/copycode.gif)
请注意长形式(--execute)后面必须紧跟一个等号(=)。
## **禁用mysql自动连接**
如果**mysql**客户程序发送查询时断开与服务器的连接，它立即并自动尝试重新连接服务器并再次发送查询。然而，即使**mysql**重新连接成功，你的第1个连接也已经结束，并且以前的会话对象和设定值被丢失：包括临时表、自动提交模式，以及用户和会话变量。该行为很危险.
如果有必要在连接断开时终止**mysql**并提示错误，你可以用--skip-reconnect选项启动**mysql**客户程序。
## **从文本文件执行SQL语句**
要想实现，创建一个文本文件text_file，并包含你想要执行的语句。然后按如下所示调用**mysql**：
shell> mysql db_name < text_file
还可以用一个USE *db_name*语句启动文本文件。在这种情况下，不需要在命令行中指定数据库名：
shell> mysql < text_file
如果正运行**mysql**，可以使用source或\.命令执行SQL脚本文件：
mysql> source filename
mysql> \. filename
