# mysql老司机之路 - 建建的博客 - CSDN博客
2017年03月15日 11:10:00[纪建](https://me.csdn.net/u013898698)阅读数：153
MYSQL数据库基础：
数据库帮我们解决以下数据存取难题：
- 较大数据量
- 事务控制
- 持久化和数据安全
- 高性能要求
- 高并发访问
关系型：mysql，oracle，sql server，postgresql
非关系型：Hadoop(大数据)，monogoDB（文档型 ）,redis（键值）,casscandra（架构先进的分布式数据库）
区别：是否使用SQL语句
MYSQL   基于GPL协议开放源代码
学习目标-DBA
- 和开发者一样学习Mysql的使用技巧
- 学习mysql体系结构知识
- 学习日常运维，性能调优，故障排查等知识
- 学习数据层架构设计，schema优化知识
- .........................
通向“Mysql”老司机之路：
- 正确使用数据库
- 运维调优数据库
- 数据库内在原理
DBA必备技能：
- SQL语言--过关过硬
- 安装部署
- 性能调优和应用优化
- 故障处理和原理掌握
- 容量规划和架构设计
命令行连接Mysql的两种方式：
- Socket连接（本地连接）
- TCP/IP连接（远程连接）
远程连接：
```
aiapple@ubuntu:~$ mysql -h127.0.0.1 -P3306 -uroot -p
Enter password:
Welcome to the MySQL monitor.
#-h 指定IP地址 -P指定端口号（mysql默认端口号3306）
```
mysql日常命令：
show status：主要关注字符编码，在线时间等
show processlist：看到当前连到数据库里面的连接状态
help：如help select，可以理解为Linux中man 或者 help；
常用mysql图形GUI工具：
Navicat
MySQLWrokBench
SQL语言入门：
|SQL 语句的分类|大致用途|
|----|----|
|DDL(Data Definition Language)|创建表，删除表，修改表.....|
|DML(Data Maniputation Language)|向表中插入记录，修改记录或者删除记录|
|select|根据条件从表中查询出想要得到的记录|
|DCL（Data Control Language）|控制数据库的访问权限等设置--包括新增用户，设置用户的权限|
|TCL（Transaction Control Language）|控制事务进展--提交事务，回滚事务|
ALTER       TABLE
DELETE     FROM     TABLE

REVOKE
ROLLBACK
DBA需要技能：
--  数据库原理
--  Linux与Shell
--  计算机体系结构
--  网络原理
--  数据库系统与操作
--  服务器硬件
--  业务架构设计
DBA ----->业务架构师
        ----->运维专家
        ----->数据库研发
