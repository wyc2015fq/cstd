# 学习ADO.NET技术（二） - u014677855的博客 - CSDN博客
2018年08月10日 17:19:59[xiaobigben](https://me.csdn.net/u014677855)阅读数：47
[参考此文，详情请戳原作者博客](http://www.cnblogs.com/liuhaorain/archive/2012/02/12/2347914.html)
Data Provider提供了访问外部数据源的功能。今天学习.NET数据提供程序具体是如何访问外部数据源的。
## 1、.Net数据提供程序
Data Provider用于连接数据库、执行命令和检索结果。
- 用于 SQL Server的数据提供程序，使用`System.Data.SqlClient`命名空间
- 用于 OLE DB的数据提供程序，使用`System.Data.OleDb`命名空间。
- 
用于ODBC的数据提供程序，`System.Data.Odbc` 命名空间。
Connection、Command、DataReader和DataAdapter对象是数据提供程序的主要组成对象。除此之外，数据提供程序还包括其他一些主要对象：
- Parameter对象 
  Parameter对象定义了命令和存储过程的输入、输出和返回值参数。 
`参数化查询`是指在设计与数据库连接并存取资料时，在需要填入数值或资料的地方，使用参数来给值。
## 2、用于SQL Server的.NET数据提供程序
用于SQL Server的数据提供程序（SqlClient）使用自己的协议与SQL Server进行通信。 
用于SQL Server的.Net Framework数据提供程序支持本地事务和分布式事务。
## 3、选择合适的.Net数据提供程序
![这里写图片描述](https://img-blog.csdn.net/20180810171722333?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180810171931238?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
