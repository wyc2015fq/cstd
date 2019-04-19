# SQL Server代理服务无法启动的处理方法(转载) - xqhrs232的专栏 - CSDN博客
2017年10月19日 16:02:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：19117
原文地址::[http://www.cnblogs.com/scwyh/archive/2013/07/17/3195348.html](http://www.cnblogs.com/scwyh/archive/2013/07/17/3195348.html)
相关文章
1、**关于sql server 2008 R2中遇到的sql server代理启动不了的解决过程----[https://www.2cto.com/database/201306/216284.html](https://www.2cto.com/database/201306/216284.html)**
**2、****[关于SQL SERVER 2008 SQL Server 代理 (SQLEXPRESS)服务无法 ...](http://bbs.51cto.com/thread-1121681-1.html)----[http://bbs.51cto.com/thread-1121681-1-1.html](http://bbs.51cto.com/thread-1121681-1-1.html)**
3、SQL Server 2008 代理无法启动----[http://bbs.csdn.net/topics/390597405](http://bbs.csdn.net/topics/390597405)
--==配置环境
服务器环境为32位Intel® Xeon® CPU E54410 @2.33GHz双路双核开启超线程，8.00GB的内存，RAID10。
1.根据最小权限原则，安装完数据库及SP3补丁后，建立Users组用户SqlStartUser。
2.通过SQL Server配置管理器配置启用TCP/IP协议，IPALL的TCP端口改为32959。SQL Server(MSSQLSERVER)和SQL Server Agent(MSSQLSERVER)的服务启动账号为SqlStartUser。
3.本地安全设置->本地策略->用户权限分配->内存中锁定页面添加SqlStartUser。
4.勾选使用AWE分配内存。最小服务器内存(MB)5500，最大服务器内存(MB)7200，其他留给操作系统。
--==检查步骤
1.检查本地账号SqlStartUser隶属于哪几个组：本地Windows组之Users组；本地Windows组之SQL Server服务账户组SQLServer2005MSSQLUser$DBSRV1$MSSQLSERVER；本地Windows组之SQL Server服务账户组SQLServer2005SQLAgentUser$DBSRV1$MSSQLSERVER。
2.检查数据库登录名是否有DBSRV1\SqlStartUser。
3.检查事件查看器应用程序有关服务启动的过程信息：
类型来源 描述
①审核成功MSSQLSERVER用户 'DBSRV1 \SqlStartUser' 登录成功。连接: trusted. [客户端: <local machine>]
②信息MSSQLSERVER配置选项 'show advanced options' 已从 1 更改为 1。请运行 RECONFIGURE 语句进行安装。
③信息MSSQLSERVERSQL Server 阻止了对组件 'Agent XPs' 的 过程'dbo.sp_sqlagent_has_server_access' 的访问，因为此组件已作为此服务器安全配置的一部分而被关闭。系统管理员可以通过使用 sp_configure 启用 'Agent XPs'。有关启用 'Agent XPs' 的详细信息，请参阅 SQL Server 联机丛书中的 "外围应用配置器"。
④信息MSSQLSERVERSQL Server 阻止了对组件 'Agent XPs' 的 过程'dbo.sp_sqlagent_get_startup_info' 的访问......
⑤错误MSSQLSERVERSQLServerAgent could not be started (reason: SQLServerAgent 必须能够以 SysAdmin 身份连接到 SQLServer，但“(未知)”不是 SysAdmin 角色的成员)。
从信息③和④看到组件'Agent XPs'未能启动。通过系统管理员登录执行语句：
sp_configure 'show advanced options', 1;GORECONFIGURE;GO
sp_configure 'Agent XPs', 1;GORECONFIGURE;GO
报错没有内存物理地址扩展权限。判断原因为32位机器，开启了AWE支持，更改了服务启动账户，但这个账户没有加入到内存锁定页的安全管理中去，所以导致了SQL代理服务启动失败，把SqlStartUser账户加入到内存锁定就可以启动了。

//================================================================================
备注::
1>sql server express不支持sql server agent

