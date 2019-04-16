# SQL学习笔记（一） - lyx的专栏 - CSDN博客





2016年07月09日 22:31:48[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：389
个人分类：[数据库](https://blog.csdn.net/qtlyx/article/category/6304319)









```
#登入
MySql -u root [-p][...]
#创建一个数据库
CREATE DATABASE IF NOT EXISTS `luyixiao` DEFAULT CHARACTER SET 'UTF8';
#选中一个数据库，以供后面使用
USE `luyixiao`;
#在数据库中创建一个表格
 CREATE TABLE IF NOT EXISTS `user`（
 NAME TYPE [CONSTRAINT]
 ） ENGINE = INNODB CHARSET = UTF8
 #往该表格中插入数据
 INSERT `user` VALUES(NUM1,NUM2,NUM3......)
  #改变数据库编码格式
ALTER DATABASE `luyixiao` DEFAULT CHARACTER SET GBK;
#显示所有的数据库
SHOW DATABASES
#显示当前使用的数据库
SELECT DATABASE()
#显示上一个警告的具体内容
SHOW WARNING
#删除一个数据库
DROP DATABASE [IF EXISTS] `luyixiao`
#显示某个数据库的信息
SHOW CREATE DATABASE NAME
 #增加日志文件
 \T 日志路径
 #停止日志文件的记录
 \t
```

注意到，在创建一个table的时候，我们选择了引擎（engine），通常我们有三种engine，分别是InnoDB，MyISAM和MEMORY。第一个是MySql默认的存储引擎，也是功能最强大的一个，可以支撑点操作，回滚之类的；MyISAM是一个带压缩功能的引擎，可以这么理解。同时牺牲了一些功能，比如回滚。至于MEMORY，就是放在了内存里，好处当然不言而喻了，速度绝对快很多，但是一旦掉电。。。。。。







