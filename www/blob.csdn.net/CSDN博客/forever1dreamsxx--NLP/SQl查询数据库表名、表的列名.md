
# SQl查询数据库表名、表的列名 - forever1dreamsxx--NLP - CSDN博客


2013年05月14日 09:43:43[forever1dreamsxx](https://me.csdn.net/forever1dreamsxx)阅读数：894个人分类：[数据库																](https://blog.csdn.net/forever1dreamsxx/article/category/1241044)



## 我试了哈，貌似不对，多半是我技术有问题，先转到这里放到。
## 转载地址：
## [http://www.cnblogs.com/px7034/archive/2011/01/10/1931770.html](http://www.cnblogs.com/px7034/archive/2011/01/10/1931770.html)

## SQl查询数据库表名、表的列名

--读取库中的所有表名
select name from sysobjects where xtype='u'
--读取指定表的所有列名
select name from syscolumns where id=(select max(id) from sysobjects where xtype='u' and name='表名')
获取数据库表名和字段
sqlserver中各个系统表的作用
sysaltfiles     主数据库         
        保存数据库的文件
syscharsets     主数据库             
    字符集与排序顺序
sysconfigures   主数据库               
  配置选项
syscurconfigs   主数据库               
  当前配置选项
sysdatabases     主数据库             
    服务器中的数据库
syslanguages     主数据库             
    语言
syslogins       主数据库           
      登陆帐号信息
sysoledbusers   主数据库               
  链接服务器登陆信息
sysprocesses     主数据库             
    进程
sysremotelogins主数据库                 
远程登录帐号
syscolumns       每个数据库           
    列
sysconstrains   每个数据库               
限制
sysfilegroups   每个数据库               
文件组
sysfiles         每个数据库         
      文件
sysforeignkeys 每个数据库               外部关键字
sysindexs       每个数据库           
    索引
sysmenbers       每个数据库           
    角色成员
sysobjects       每个数据库           
    所有数据库对象
syspermissions 每个数据库               权限
systypes         每个数据库         
      用户定义数据类型
sysusers         每个数据库         
      用户

//
用什么方法可以得到一个表中所有的列名。SQl语句。
select     列名=name     from     syscolumns 
  where


