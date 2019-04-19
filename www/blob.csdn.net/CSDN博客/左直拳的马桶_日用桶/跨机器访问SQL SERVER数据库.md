# 跨机器访问SQL SERVER数据库 - 左直拳的马桶_日用桶 - CSDN博客
2007年12月04日 23:24:00[左直拳](https://me.csdn.net/leftfist)阅读数：1478
跨机器访问SQL SERVER数据库
左直拳
如果在SQL SERVER上想访问另一台机器上的数据库，怎么办呢？
一、运行以下代码：
EXECsp_addlinkedserver'别名',N'SQL Server'
go
execsp_addlinkedsrvlogin'别名','false',NULL,'目标数据库的帐号','目标数据库的密码'
go
然后就可以
SELECT * FROM [别名].[数据库名].dbo.Table名了。
二、在SQL SERVER2005的 Manage Studio上还可以直接配置。
链接服务器，鼠标右键新建链接服务器，然后输入目标机器名，选适当的选项，添加登陆用户、密码就可以了。
