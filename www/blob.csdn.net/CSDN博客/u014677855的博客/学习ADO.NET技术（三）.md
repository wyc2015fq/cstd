# 学习ADO.NET技术（三） - u014677855的博客 - CSDN博客
2018年08月10日 17:31:44[xiaobigben](https://me.csdn.net/u014677855)阅读数：55
[参考此文，详情请戳原作者博客](http://www.cnblogs.com/liuhaorain/archive/2012/02/12/2347914.html)
ADO.Net的核心组成部分分为DataSet和Data Provider。今天学习数据提供程序包含的对象Connection.
## 1、连接字符串
- 
什么是连接字符串？ 
ADO.NET的作用是用于应用程序与数据源的连接，为外部数据源提供一致的访问。ADO.NET为不同的数据源编写了不同的数据提供程序，但是它的前提是要访问到正确的数据源，连接字符串就是告诉ADO.NET数据源在哪里，需要什么样的数据格式，提供什么样的访问信任级别以及其他任何包括连接的相关信息。
- 
连接字符串的语法格式 
连接字符串由一组元素组成，一个元素包含一个键值对，元素之间由“；”分开。语法如下：
`key1 = value;key2 = value2;key3 = value3...`- 连接字符串的元素包含内容有：数据源是基于文件的还是基于网络的数据库服务器，是否需要密码，超时限制等。元素对的键是根据需要连接的数据源来确定的。
## 2、几种常用的连接字符串
- 
SQL Server连接：
`DataSource=myServerAddress;Initial Catalog=myDataBase;UserId=myUsername;Password=myPassword;`
Data Source:需要连接的服务器。
Initial Catalog:默认使用的数据库名称。
User ID:数据库服务器账号。
Password:数据库服务器密码。
- 
Access连接字符串
`provider=Microsoft.Jet.oleDB.4.0;DataSource=C:\mydatabase.mdb;UserId=myUsername;Password=myPassword;`
- 
MySQL 连接字符串
`Server=myServerAddress;Database=myDataBase;Uid=myUsername;Pwd=myPassword;;`
- 
DB2连接字符串
`Server=myAddress:myPortNumber;Database=myDataBase;UID=myUsername;PWD=myPassword;`
- 
Oracle连接字符串 
`DataSource=TORCL;UserId=myUsername;Password=myPassword;`
## 3、构造连接字符串
`string connStr = "DataSource=myServerAddress;Initial Catalog=myDataBase;UserID=myUsername;Password=myPassword";`
## 4、在配置文件中存储连接字符串
在实际开发中，一般都把连接字符串写在配置文件里。这样便于维护，以Winform应用程序为例，我们一般把连接字符串写在App.Config中。在节点下添加连接字符串。
