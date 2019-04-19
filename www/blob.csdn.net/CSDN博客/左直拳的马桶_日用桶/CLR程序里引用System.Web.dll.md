# CLR程序里引用System.Web.dll - 左直拳的马桶_日用桶 - CSDN博客
2010年08月24日 17:04:00[左直拳](https://me.csdn.net/leftfist)阅读数：1374标签：[sql server																[assembly																[database																[数据库																[.net																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=.net&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)](https://so.csdn.net/so/search/s.do?q=database&t=blog)](https://so.csdn.net/so/search/s.do?q=assembly&t=blog)](https://so.csdn.net/so/search/s.do?q=sql server&t=blog)
个人分类：[sql server](https://blog.csdn.net/leftfist/article/category/136470)
默认是没有，要在目标数据库里先附加，然后.NET这里才可以引用。
SQL SERVER 2005:
-------------------------------------------------------------------------------------------
USE [mydb]
GO
EXEC sp_changedbowner 'sa'
ALTER DATABASE [mydb] SET TRUSTWORTHY ON
GO
CREATE ASSEMBLY [System.Web]
    FROM 'C:/WINDOWS/Microsoft.NET/Framework/v2.0.50727/System.Web.dll'
    WITH PERMISSION_SET = UNSAFE
