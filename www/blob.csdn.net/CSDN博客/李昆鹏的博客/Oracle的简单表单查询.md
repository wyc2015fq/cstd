
# Oracle的简单表单查询 - 李昆鹏的博客 - CSDN博客


2018年05月21日 15:34:14[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：165


----------------------------Oracle的简单表单查询--------------------------------
## 1.sql简介
结构化查询语言(Structured Query Language)简称SQL(发音：/ˈɛs kjuː ˈɛl/ "S-Q-L")，结构化查询语言是一种数据库查询和程序设计语言，用于存取数据以及查询、更新和管理关系数据库系统；同时也是数据库脚本文件的扩展名。结构化查询语言是高级的非过程化编程语言，允许用户在高层数据结构上工作。它不要求用户指定对数据的存放方法，也不需要用户了解具体的数据存放方式，所以具有完全不同底层结构的不同数据库系统, 可以使用相同的结构化查询语言作为数据输入与管理的接口。结构化查询语言语句可以嵌套，这使它具有极大的灵活性和强大的功能。
Ø**DML**(数据库操作语言): 其语句包括动词INSERT，UPDATE和DELETE。它们分别用于添加，修改和删除表中的行。也称为动作查询语言。
Ø**DDL**(数据库定义语言):其语句包括动词CREATE和DROP。在数据库中创建新表或删除表（CREAT TABLE 或 DROP TABLE）；为表加入索引等。DDL包括许多与人数据库目录中获得数据有关的保留字。它也是动作查询的一部分。
**DCL**(数据库控制语言):它的语句通过GRANT或REVOKE获得许可，确定单个用户和用户     组对数据库对象的访问。某些RDBMS可用GRANT或REVOKE控制对表单个列的访问。
### 1.查询语法
Select * |列名 from 表名
![](https://img-blog.csdn.net/20180521153207434?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180521153213337?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 2.别名用法
在查询的结果列中可以使用别名
Select 列名 别名，列名别名，... from emp;
![](https://img-blog.csdn.net/20180521153232649?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 1.  除重复的数据
Selectdistinct*|列名, ... from emp;
![](https://img-blog.csdn.net/20180521153242753?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
使用distinct可以消除重复的行，如果查询多列的必须保证多列都重复才能去掉重复
### 2.  字符串连接查询
查询雇员编号，姓名，工作
编号是：7369的雇员， 姓名是：smith，工作是：clerk
![](https://img-blog.csdn.net/20180521153254498?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
字符串的连接使用‘||’
### 3.  查询中四则运算
查询每个雇员的年薪
Select ename, sal*12 from emp;
Select ename, sal*12 income from emp;
![](https://img-blog.csdn.net/20180521153307787?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Sql中支持四则运算“+，-，*，/”
代码示例：
|--|单行注释|/*|多行注释|*/
|select|*|from|emp;|--|查询|emp|表
|select|empno,ename,job|from|emp;|--|查询|emp|表中指定的列
|select|empno|as|员工编号|,ename|as|员工姓名|from|emp;|--|第一种按指定的别名显示查询|emp|表中指定的列
|select|empn0 en0,ename en|from|emp;|--|第二种按指定的别名显示查询|emp|表中指定的列，比较常用
|select|distinct|job|from|emp;|--|查询指定的列表并去除重复的数据
|select|distinct|job,deptno|from|emp;|--|只有所有的列数据全重复才可以合并
|select|'|员工编号是|'||| empno |||'|姓名是|'||| ename |||'|的工作是|'||| job|from|emp;|--sql|中字符串用单引号表示，连接符号是|||
|select|ename,sal*|12|from|emp;|--|查询中进行四则运算，必须是数值类型，在|sql|中并不推荐四则运算会影响性能
|

