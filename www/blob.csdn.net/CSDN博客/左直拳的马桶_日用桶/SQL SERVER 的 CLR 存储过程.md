# SQL SERVER 的 CLR 存储过程 - 左直拳的马桶_日用桶 - CSDN博客
2009年10月29日 14:57:00[左直拳](https://me.csdn.net/leftfist)阅读数：1488
 SQL SERVER 的 CLR 存储过程
MS SQL的TransactSQL语法比较单一，用起来也很不习惯，如果想实现一些复杂一点的逻辑，不方便。
可以利用NET编码，编译成DLL，然后在SQL SERVER里引用。
一、    编码
1、    新建一个项目，数据库类型 --》SQL SERVER
2、    新建“存储过程”
3、    编码也没有什么特别，主要是有些数据类型有讲究。比如参数，应该能被SQL SERVER认识或方便调用传送，象整数就用SqlInt32什么的。但函数内部，就似乎没什么讲究。
 [Microsoft.SqlServer.Server.SqlProcedure]
public static void UserInit(SqlInt32 iUserId)
{
    int i = 0;
}
4、    数据库连接。由于这些函数和方法最终都在SQL SERVER里调用，那么调用它们之前，肯定已经和数据库连接上了，所以应该使用当前的数据库连接。
[Microsoft.SqlServer.Server.SqlProcedure]
public static void UserInit(SqlInt32 iUserId)
{
    int i = 0;
    using (SqlConnection connection = new SqlConnection("context connection = true"))
   {……}
}
二、    添加部署
1、    代码编写完后，编译成DLL。然后打开SQL SERVER的目标数据库 --》程序集 --》添加程序集，将DLL导入。给该程序集起个名字，比如叫myClrDll
2、    在目标数据库新建一个存储过程，使用程序集里的函数。
CREATE PROCEDURE [dbo].[clr_User_Init]
    @UserId [int]
WITH EXECUTE AS CALLER
AS
EXTERNAL NAME [myClrDll].[StoredProcedures].[UserInit]
GO
三、    更新
如果代码更改，重新编译成DLL，可以用以下语句重新导入
ALTER ASSEMBLY [myClrDll]
FROM 'd:/code/temp/myClrDll.dll'
注意'd:/code/temp/myClrDll.dll'一定是数据库所在机器的路径。通常我们程序和数据库不同机器，所以这个路径常常搞错。
四、注意
CLR中的SqlString，在T-sql里面，要写成：NVarChar(MAX)。
五、数据库不支持CLR怎么办？
### [禁止在.NET Framework 中执行用户代码。启用"clr enabled" 配置选项](http://www.google.com.hk/url?sa=t&rct=j&q=%E7%A6%81%E6%AD%A2%E5%9C%A8+.NET+Framework+%E4%B8%AD%E6%89%A7%E8%A1%8C%E7%94%A8%E6%88%B7%E4%BB%A3%E7%A0%81%E3%80%82%E5%90%AF%E7%94%A8+%22clr+enabled%22+%E9%85%8D%E7%BD%AE%E9%80%89%E9%A1%B9%E3%80%82&source=web&cd=1&ved=0CFoQFjAA&url=http%3A%2F%2Fblog.csdn.net%2Fheshengfen123%2Farticle%2Fdetails%2F3597125&ei=dYjIT7D9KqeyiQfQyMwx&usg=AFQjCNGEgdztcnLPw4whVwQrFS6yw_LhMw)
exec sp_configure 'show advanced options', '1';
go
reconfigure;
go
exec sp_configure 'clr enabled', '1'
go
reconfigure;
exec sp_configure 'show advanced options', '0';
go
