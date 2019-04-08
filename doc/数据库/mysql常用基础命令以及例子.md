# mysql常用基础命令以及例子

2017年09月25日 22:58:34 [刘翾](https://me.csdn.net/c_kite) 阅读数：177



 版权声明：原创文章，如想转载，请注明原文网址，注明出处；否则，禁止转载；谢谢配合！	https://blog.csdn.net/c_kite/article/details/78090512

## 基础命令

管理员权限命令窗口输入以下命令

| 名称                                                         | 作用                                                         | 例子                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| net start mysql                                              | 开启mysql服务器(Windows)                                     |                                                              |
| net stop mysql                                               | 关闭mysql服务器(Windows)                                     |                                                              |
| mysql -u用户名 -p                                            | 登录数据库(注意，如果是连接到另外的机器上，则需要加入一个参数-h机器IP) | mysql -u root -p                                             |
| mysqldump -u 用户名 -p 数据库名 > 导出的文件名               | 导出整个数据库                                               | mysqldump -u root -p text > 1.txt(导出文件在当前命令路径位置) |
| 导出一个表mysqldump -u 用户名 -p 数据库名 表名> 导出的文件名 | 导出一个表                                                   |                                                              |
| show variables like ‘%char%’;                                | 查看数据库字符集                                             |                                                              |

## 操作命令

登录到数据库后可以输入以下命令

| 名称                         | 作用                                               | 例子                                                         |
| ---------------------------- | -------------------------------------------------- | ------------------------------------------------------------ |
| show databases;              | 显示数据库                                         |                                                              |
| create database 库名;        | 增加数据库                                         | create database text;                                        |
| drop database 库名;          | 删除数据库                                         | drop database text;                                          |
| use 数据库名;                | 使用一个数据库进行操作, 不选择无法进行增删改查操作 | user text;                                                   |
| show tables;                 | 显示当前数据库的表                                 |                                                              |
| describe 表名;               | 显示表结构                                         |                                                              |
| create table 表名(字段列表); | 创建表                                             | create table employee(name char(10) not null, ssn char(18) not null, primary key(ssn)) engine=myisam default charset=utf8; |
| drop table 表名;             | 删除表                                             | drop table employee                                          |
| delete from 表名;            | 清空表中记录                                       |                                                              |
| mysql>source 文件路径        | 导入数据库                                         | mysql>source d:/dbname.sql                                   |

## 数据库增删改操作

| 名称                                                         | 作用             | 例子                                                         |
| ------------------------------------------------------------ | ---------------- | ------------------------------------------------------------ |
| ALTER TABLE <关系名> ADD <列名><列类型>                      | 在表中增加属性   | ALTER TABLE EMPLOYEE ADD JOB VARCHAR(12)                     |
| ALTER TABLE <关系名> drop <列名>                             | 删除表中某一属性 | ALTER TABLE EMPLOYEE DROP JOB                                |
| CREATE [UNIQUE] INDEX 〈索引名〉 ON 〈关系名〉(〈列名〉[ORDER], …,〈列名〉[ORDER]) [CLUSTER] | 建立索引         | CREATE UNIQUE INDEX SSN_INDEX ON EMPLOYEE(SSN ASC) CLUSTER  //在EMPLOYEE关系上以SSN属性为索引属性，建立一个聚集索引，索引文件名字为SSN_INDEX，并说明SSN是键属性，索引按照SSN的值递增排序 asc–递增 desc–递减 |
| DROP INDEX 〈索引名〉                                        | 删除索引         | DROP INDEX SSN_INDEX                                         |
| INSERT INTO <关系名> <列名>(VALUES(<常值>,…,<常值>))         | 插入信息         | INSERT INTO EMPLOYEE VALUES (“王明”, “655325678”, “1950年7月11日”) |
| DELETE FROM 〈关系名〉[WHERE 〈条件表达式〉]                 | 删除信息         |                                                              |
| UPDATE 〈关系名〉SET 〈列名〉=〈常值〉，…，〈列名〉=〈常值〉[WHERE 〈条件表达式〉] | 更新信息         | 例: 把PROJECT关系中的工程项目10的地址和所属部门号分别修改为“北京”和5。 UPDATE PROJECT SET PLOCATION=“北京”，DNUM=5 WHERE PNUMBER=10 |