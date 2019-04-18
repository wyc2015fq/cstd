# Mysql：日志管理：维护日志 - weixin_33985507的博客 - CSDN博客
2009年06月21日 18:21:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
**总的来说，mysql没有什么特别的机制提供日志的备份、删除等等管理操作，而很大程度上依赖于os级别的操作。**
**对于二进制事务日志来说：mysql提供了expire_logs_days系统变量**来指示过期的天数，如果你使用replication，注意应适当设置该值。
**开始新的日志**
- mysql语句：flush logs;
- mysql实用程序：mysqladmin flush-logs 、 mysqladmin refresh
以上操作会激发：
> 
> 
> - 一般查询日志、慢速查询日志：先关闭，在重新打开
- 二进制事务日志：关闭当前，依序打开新的文件
- 错误日志：关闭当前，另存为“-old”后缀的文件，打开新的文件
