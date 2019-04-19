# SQL Server客户端工具到底使用的是哪个provider呢？ - xqhrs232的专栏 - CSDN博客
2017年10月18日 16:36:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3097
原文地址::[http://www.ithov.com/server/123049.shtml](http://www.ithov.com/server/123049.shtml)
相关文章
1、[请问，关于SQL
 server 连接时 Provider=SQLOLEDB.1 的问题 ...](http://bbs.51cto.com/thread-1103962-1.html)----[http://bbs.51cto.com/thread-1103962-1-1.html](http://bbs.51cto.com/thread-1103962-1-1.html)
2、vb连接sql server 数据库时候那个provider字段是什么?应该等于什么？----[https://zhidao.baidu.com/question/234107547.html](https://zhidao.baidu.com/question/234107547.html)
3、VS与SQL
 Server连接时出现(provider: TCP Provider, error: 0）错误时的解决方法----[http://blog.csdn.net/zhaoyang829/article/details/51820277](http://blog.csdn.net/zhaoyang829/article/details/51820277)
我们经常困惑的是，我们一些常用的SQL Server客户端工具，如SQLCMD, OSQL, ISQL, SQL Server Management Studio到底是用ODBC, OLEDB还是System.Data.Sqlclient? 不了解工具用的是哪个provider, 则有时候会比较困惑。如:
这篇文档讲sqlcmd.exe 2012版本用的是OLEDB [http://msdn.microsoft.com/en-us/library/ms190611.aspx](http://msdn.microsoft.com/en-us/library/ms190611.aspx)
另外篇文档讲sqlcmd.exe 2012版本用的事ODBC [http://technet.microsoft.com/en-us/library/ms162773.aspx](http://technet.microsoft.com/en-us/library/ms162773.aspx)
哪篇MSDN文档是对的，哪篇是错的?
通过对各个客户端工具进行debugging，我们能比较容易的了解到每个工具使用了provider。下面是不同的工具，在不同版本下使用Data Access技术的列表，仅供参考。
||Isql.exe|Osql.exe|Sqlcmd|QA|Sqlwb.exe|Ssms.exe|
|----|----|----|----|----|----|----|
|SQL Server 2000|DB-LibraryNtwdblib.DLL|ODBCsqlsrv32.dll|N.A.|ODBCSqlsrv32.dll|N.A.|N.A.|
|SQL Server 2005|N.A.|ODBCSqlncli.dll|OLEDBSqlncli.dll|N.A.|System.Data.SqlClient.NET Framework 2.0|N.A.|
|SQL Server 2008|N.A.|ODBCSqlncli10.dll|OLEDBSqlncli10.dll|N.A.|N.A.|System.Data.SqlClient.NET Framework 3.5 SP1|
|SQL Server 2008 R2|N.A.|ODBCSqlncli10.dll|OLEDBSqlncli10.dll|N.A.|N.A.|System.Data.SqlClient.NET Framework 3.5 SP1|
|SQL Server 2012|N.A.|ODBCSqlncli11.dll|ODBCSqlncli11.dll|N.A.|N.A.|System.Data.sqlClient.NET Framework 4.0|
比较值得注意的是，从SQL Server 2008到SQL Server 2012, SQLCMD.EXE从OLEDB迁移到了ODBC。

