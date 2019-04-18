# C++通过ADO访问数据库的连接字符串 - 深之JohnChen的专栏 - CSDN博客

2017年08月24日 14:18:31[byxdaz](https://me.csdn.net/byxdaz)阅读数：2621


一、连接字符串获取方法
1、OLEDB驱动
   新建一个***.txt重名为***.udl，双击运行udl文件弹出数据源配置对话框，配置好并测试连接成功以后点确定，会在udl文件里产生一个连接字符串。
2、ODBC驱动
   新建一个***.txt重名为***.udl，双击运行udl文件弹出数据源配置对话框，选择OLEDB驱动程序时选择Microsoft OLE DB Provider for ODBC Drivers,下一步，指定数据源时“使用连接字符串”，点编译，在“文件数据源”标签下单击“新建”，在弹出创建新数据源中选择要使用的ODBC驱动，单击下一步，输入保存路径，单击下一步，完成。一步步配置下来后即可生成连接字符串。

二、常见数据库连接字符串
ADO访问access数据库连接字符串
Provider=Microsoft.Jet.OLEDB.4.0;Data Source=.\\XDB.mdb

ADO访问sql server连接字符串
1）、Windows身份认证方式
Provider=SQLOLEDB;Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=db_monitor;Data Source=DESKTOP-M4325HH\\SQLEXPRESS
Provider=SQLNCLI;Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=db_monitor;Data Source=DESKTOP-M4325HH\\SQLEXPRESS
Provide=SQLOLEDB或SQLNCLI为数据库连接方式。其中OLE为对象连接与嵌入。
Integrated Security=SSPI指定的是Windows身份认证
Persist Security Info 属性的意思是表示是否保存安全信息，其实可以简单的理解为“ADO 在数据库连接成功后是否保存密码信息”，True表示保存，False表示不保存。 
Initial Catalog=数据库名字
Data Source=装有数据库的机器名或IP地址 + 实例名，注意转义字符（比如：Data Source=LI-PC\\SQLEXPRESS）。

Provider=SQLNCLI;Data Source=server;Initial Catalog=database;User Id=user;Password=password;

2）、用户密码登陆方式
Provider=SQLNCLI;Server=192.168.1.100\\SQL2012;Database=monitor;uid=sa;pwd=password;
Provider=SQLNCLI11;Server=192.168.1.100\\SQL2012;Database=monitor;uid=sa;pwd=password;  //也可以使用带上明确的版本号
Provider=SQLOLEDB;Server=192.168.1.100\\SQL2012;Database=monitor;uid=sa;pwd=password;
Provide=SQLOLEDB或SQLNCLI为数据库连接方式。其中OLE为对象连接与嵌入。
Server=数据库实例名称，如果是远程其他电脑数据库，就使用机器名或IP地址 + 实例名。local表示本地数据库连接。

OleDb本地连接SQLServer的Provider两种驱动：
Provider=SQLNCLI是SQL Server的原生驱动，功能比较完整，但是需要安装SQL Server的客户端驱动程序（即SQL Server Native Client），SQLNCLI11，SQLNCLI后面的数字表示版本号。
Provider=SQLOLEDB是Windows集成的驱动程序，功能不太完善，但对于支持SQL Server 2000已经具有非常完整的功能了。
从SQL Server 2005开始，官方推荐使用SQL Server Native Client。

ADO访问my sql数据库连接字符串
通过ADO连接MySql数据库，首先得安装MyODBC服务器程序。MyODBC版本要和MySql的版本对应上，否则会连接不上数据库。比如，我用的版本分别是mysql-5.1.48-win32.msi和mysql-connector-odbc-5.1.5-win32.msi。
安装好后，通过数据源(ODBC)可以获取到连接字符串。
Provider=MSDASQL.1;Persist Security Info=True;Extended Properties="Driver=MySQL ODBC 5.1 Driver;SERVER=localhost;UID=root;PWD=123456;DATABASE=seismon;PORT=3306;COLUMN_SIZE_S32=1"
如何如何正确获取MYSQL的ADO连接字符串，参考http://www.cnblogs.com/swek/articles/4338044.html

连接时提示host '192.168.0.1' is not allowed to connect to this mysql server。
可能是你使用的用户名不能远程连接。在Mysql本地之下下列语句赋予权限即可：
解决方法：
给用户授权：
GRANT ALL PRIVILEGES ON *.* TO '用户名'@'%' IDENTIFIED BY '连接密码' WITH GRANT OPTION;
设置用户只能在某个IP下才能访问服务器：
GRANT ALL PRIVILEGES ON *.* TO '用户名'@'IP地址' IDENTIFIED BY '连接密码' WITH GRANT OPTION;

ADO访问oracle数据库连接字符串
使用微软自带的oledb驱动(调用此驱动前也与需要安装Oracle客户端中的Oledb驱动)：
Provider=MSDAORA.1; Password=sa123; User ID=system; Data Source=192.168.0.221/orcl; Persist Security Info=True
使用Oracle自带的oledb驱动：
Provider=OraOLEDB.Oracle.1; Password=sa123; User ID=system; Data Source=192.168.0.221/orcl; Persist Security Info=True
使用Oracle自带的ODBC驱动：
DRIVER={Oracle in OraClient11g_home1};SERVER=192.168.0.221/ORCL;UID=system;PWD=sa123;DBQ=192.168.0.221/ORCL  

