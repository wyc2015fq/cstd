# 学习ADO.NET技术（四）—Connection连接对象 - u014677855的博客 - CSDN博客
2018年08月14日 13:33:40[xiaobigben](https://me.csdn.net/u014677855)阅读数：99
# Connection连接对象
数据提供程序是ADO.NET技术里的核心部分，作为数据提供程序对象之一的Connection对象，尤为重要。上一篇学习了常用的几种数据库连接字符串，这篇开始学习Connection对象是如何连接数据源的。
# 目录
- [Connection连接对象](#connection连接对象)
- [目录](#目录)
- [1、理解Connection对象](#1理解connection对象)
- [2、DbConnection类的几个重要方法](#2dbconnection类的几个重要方法)
- [3、DbConnection类的几个重要属性](#3dbconnection类的几个重要属性)
- [4、ConnectionState](#4connectionstate)
- [5、实例](#5实例)- [连接SQL Server的SqlConnection对象](#连接sql-server的sqlconnection对象)
- [保证代码安全](#保证代码安全)
- [说明](#说明)
# 1、理解Connection对象
Connection对象，前面说过，表示与特定的数据源的连接。Connection对象（拿着钥匙的人）通过连接字符串（钥匙）连接到数据源（大门）。对于ADO.NET而言，不同的数据源，对应着不同的Connection对象。具体Connection对象如下表： 
可以使用冒号来定义对齐方式：
|名称|命名空间|描述|
|----|----|----|
|SqlConnection|System.Data.SqlClient|表示与SQL Server的连接对象|
|OleDbConnection|System.Data.OleDb|表示与OleDb的数据源的连接对象|
|OdbcConnection|System.Data.Odbc|表示与ODBC数据源的连接对象|
|OracleConnection|System.Data.OracleClient|表示与Orale数据库的连接对象|
不管哪种连接对象，它都继承于DbConnection类。
`public abstract class DbConnection:Component,IDbConnection,IDisposable`
从上面代码可以看出，DnConnection是抽象基类，并且继承于Compoent,IDbConnection,IDisposable类。由于DbConnection类是抽象基类，因此它不能实例化。DbConnection类封装了很多重要的方法和属性，下面学习几个重要的方法和属性。
# 2、DbConnection类的几个重要方法
- Open:使用ConnectionString所指定的设置打开数据库连接。
- DisPose:释放由Component使用的所有资源。
- Close：关闭与数据库的连接。它将连接释放到连接池，或者在连接池被禁用的情况下关闭连接。
# 3、DbConnection类的几个重要属性
**DataBase:**在连接打开之后获取当前数据库的名称，或者在连接打开之前获取连接字符串中指定的数据库名称。 
**DataSource:**获取要连接的数据库服务器的名称。 
**ConnectionTimeOut:**获取在建立连接时终止尝试并生成错误之前的等待时间。 
**ConnectionString:**获取或设置用于打开连接的连接字符串。 
**State:**获取描述连接状态的字符串。
# 4、ConnectionState
它是一个枚举类型，包括以下几个成员： 
**Closed**:连接处于关闭状态 
**Open**:连接处于打开状态 
**Connecting**：连接对象正在与数据源连接 
**Executing**:连接对象正在执行命令。 
**Fetching**:连接对象正在检索数据。 
**Broken**:与数据源的连接中断。
# 5、实例
## 连接SQL Server的SqlConnection对象
前面表格说明，连接SQLServer数据库的连接对象是SqlConnection对象。
```python
class Program
    {
        static void Main(string[] args)
        {
            SqlConnectionStringBuilder connstr = new SqlConnectionStringBuilder();
            connstr.DataSource = @".\SQLEXPRESS";
            connstr.InitialCatalog = "master";
            connstr.IntegratedSecurity = true;
            SqlConnection conn = new SqlConnection();
            conn.ConnectionString = connstr.ConnectionString;
            conn.Open();
            if(conn.State==System.Data.ConnectionState.Open)
            {
                Console.WriteLine("Database is linked.");
                Console.WriteLine("\nDataSource:{0}", conn.DataSource);
               Console.WriteLine("Database:{0}", conn.Database);
                Console.WriteLine("ConnectionTimeOut:{0}", conn.ConnectionTimeout);
            }
            conn.Close();
            conn.Dispose();
            if(conn.State == System.Data.ConnectionState.Closed)
            {
                Console.WriteLine("\nDatabase is closed.");
            }
            Console.Read();
        }
    }
```
## 保证代码安全
- （1）添加try…catch块 
    在finally块中添加关闭连接的代码，确保无论是否出现异常，都将关闭连接，释放资源。 
```
SqlConnection conn = new SqlConnection(connStr); 
2 try 
3 { 
4       conn.Open(); 
5 } 
6 catch(Exception ex) 
7 { 
8           ;//todo 
9 } 
10 finially 
11 { 
12      conn.Close(); 
13 }
```
- （2）使用using语句 
using语句的作用是确保资源使用后，很快释放资源。帮助减少意外的运行时错误带来的潜在问题。它具体执行以下任务： 
1、分配资源 
2、把Statement放入try块 
3、创建资源的Dispose方法，并把它放进finally块。
```
1 using(SqlConnection conn = new SqlConnection(connStr))
2 {
3      ;//todo
4 }
```
# 说明
本片博文主要是为了学习与帮助记忆ADO.NET的知识，主要参考了[原作者的这篇文章，详情请戳这里，作者写的很详细。](https://www.cnblogs.com/liuhaorain/archive/2012/02/15/2349886.html)
原作者的声明如下： 
我叫刘皓，很高兴您能阅读完我的这篇文章。 
我花了大量时间和精力来完成这篇文章，如果文章对您有帮助，请不要忘了点推荐哦！ 
如果您能点击右边的打赏按钮，打赏一杯咖啡钱，我将获得更多的动力和能量写出下一篇好文章。 
本文版权归作者和博客园共有，欢迎转载，但未经作者同意必须保留此段声明，且在文章页面明显位置给出原文链接，否则保留追究法律责任的权利。 
我的博客即将搬运同步至腾讯云+社区，邀请大家一同入驻：[https://cloud.tencent.com/developer/support-plan](https://cloud.tencent.com/developer/support-plan)
