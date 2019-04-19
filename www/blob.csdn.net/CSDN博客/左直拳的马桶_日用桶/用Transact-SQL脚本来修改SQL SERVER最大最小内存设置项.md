# 用Transact-SQL脚本来修改SQL SERVER最大最小内存设置项 - 左直拳的马桶_日用桶 - CSDN博客
2011年02月23日 15:46:00[左直拳](https://me.csdn.net/leftfist)阅读数：2216
sp_configure 'show advanced options', 1;
GO
RECONFIGURE;
GO
sp_configure 'min server memory', 2048;
GO
RECONFIGURE;
GO
sp_configure 'max server memory', 2560;
GO
RECONFIGURE;
GO
sp_configure 'show advanced options', 0;
GO
RECONFIGURE;
GO
这个脚本运行了以后，不用重启数据库。如果通过界面设置，要重启。
其实，Transact-SQL 是使用 SQL Server 的核心。与 SQL Server 实例通信的所有应用程序都通过将 Transact-SQL 语句发送到服务器进行通信，而不管应用程序的用户界面如何。好多时，直接使用脚本，要比通过友好的界面来设置更快、更有效，甚至也只能通过脚本。比如，备份数据库，用脚本要更快；建大一点表的索引，用界面往往提示超时，而脚本则无须担心。
