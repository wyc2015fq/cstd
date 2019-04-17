# JSP连接数据库 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年08月09日 16:42:37[boonya](https://me.csdn.net/boonya)阅读数：352










文章来自：[http://www.runoob.com/jsp/jsp-database-access.html](http://www.runoob.com/jsp/jsp-database-access.html)


本章节假设您已经对JDBC有一定的了解。在开始学习JSP数据库访问前，请确保JDBC环境已经正确配置。

首先，让我们按照下面的步骤来创建一个简单的表并插入几条简单的记录：

## 创建表

在数据库中创建一个Employees表，步骤如下：

### 步骤1：
打开CMD，然后进入数据库安装目录：C:\>
C:\>cd ProgramFiles\MySQL\bin
C:\Program Files\MySQL\bin>
### 步骤2：
C:\Program Files\MySQL\bin>mysql -u root -p
Enter password:********
mysql>
### 步骤3：

在TEST数据库中创建Employee表：
mysql>use TEST;
mysql> create table Employees(
     id intnotnull,
     age intnotnull,
     first varchar (255),last varchar (255));Query OK,0 rows affected (0.08 sec)
mysql>
## 插入数据记录

创建好Employee表后，往表中插入几条记录：
mysql> INSERT INTO Employees VALUES (100,18,'Zara','Ali');Query OK,1 row affected (0.05 sec)
 
mysql> INSERT INTO Employees VALUES (101,25,'Mahnaz','Fatma');Query OK,1 row affected (0.00 sec)
 
mysql> INSERT INTO Employees VALUES (102,30,'Zaid','Khan');Query OK,1 row affected (0.00 sec)
 
mysql> INSERT INTO Employees VALUES (103,28,'Sumit','Mittal');Query OK,1 row affected (0.00 sec)
 
mysql>
## SELECT操作

接下来的这个例子告诉我们如何使用JSTL SQL标签来运行SQL SELECT语句：
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="java.io.*,java.util.*,java.sql.*"%>
<%@ page import="javax.servlet.http.*,javax.servlet.*" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/sql" prefix="sql"%>
 
<html><head><title>SELECT 操作</title></head><body><sql:setDataSourcevar="snapshot"driver="com.mysql.jdbc.Driver"url="jdbc:mysql://localhost/TEST"user="root"password="pass123"/><sql:querydataSource="${snapshot}"var="result">
SELECT * from Employees;
</sql:query><tableborder="1"width="100%"><tr><th>Emp ID</th><th>First Name</th><th>Last Name</th><th>Age</th></tr><c:forEachvar="row"items="${result.rows}"><tr><td><c:outvalue="${row.id}"/></td><td><c:outvalue="${row.first}"/></td><td><c:outvalue="${row.last}"/></td><td><c:outvalue="${row.age}"/></td></tr></c:forEach></table></body></html>
访问这个JSP例子，运行结果如下：

![mysql-access-1](http://www.runoob.com/wp-content/uploads/2014/01/mysql-access-1.jpg)

## INSERT操作

这个例子告诉我们如何使用JSTL SQL标签来运行SQL INSERT语句：
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="java.io.*,java.util.*,java.sql.*"%>
<%@ page import="javax.servlet.http.*,javax.servlet.*" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/sql" prefix="sql"%>
 
<html><head><title>INSERT 操作</title></head><body><sql:setDataSourcevar="snapshot"driver="com.mysql.jdbc.Driver"url="jdbc:mysql://localhost/TEST"user="root"password="pass123"/><sql:updatedataSource="${snapshot}"var="result">
INSERT INTO Employees VALUES (104, 2, 'Nuha', 'Ali');
</sql:update><sql:querydataSource="${snapshot}"var="result">
SELECT * from Employees;
</sql:query><tableborder="1"width="100%"><tr><th>Emp ID</th><th>First Name</th><th>Last Name</th><th>Age</th></tr><c:forEachvar="row"items="${result.rows}"><tr><td><c:outvalue="${row.id}"/></td><td><c:outvalue="${row.first}"/></td><td><c:outvalue="${row.last}"/></td><td><c:outvalue="${row.age}"/></td></tr></c:forEach></table></body></html>
访问这个JSP例子，运行结果如下：

![mysql-access-2](http://www.runoob.com/wp-content/uploads/2014/01/mysql-access-2.jpg)

## DELETE操作

这个例子告诉我们如何使用JSTL SQL标签来运行SQL DELETE语句：
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="java.io.*,java.util.*,java.sql.*"%>
<%@ page import="javax.servlet.http.*,javax.servlet.*" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/sql" prefix="sql"%>
 
<html><head><title>DELETE 操作</title></head><body><sql:setDataSourcevar="snapshot"driver="com.mysql.jdbc.Driver"url="jdbc:mysql://localhost/TEST"user="root"password="pass123"/><c:setvar="empId"value="103"/><sql:updatedataSource="${snapshot}"var="count">
  DELETE FROM Employees WHERE Id = ?
  <sql:paramvalue="${empId}"/></sql:update><sql:querydataSource="${snapshot}"var="result">
   SELECT * from Employees;
</sql:query><tableborder="1"width="100%"><tr><th>Emp ID</th><th>First Name</th><th>Last Name</th><th>Age</th></tr><c:forEachvar="row"items="${result.rows}"><tr><td><c:outvalue="${row.id}"/></td><td><c:outvalue="${row.first}"/></td><td><c:outvalue="${row.last}"/></td><td><c:outvalue="${row.age}"/></td></tr></c:forEach></table></body></html>
访问这个JSP例子，运行结果如下：

![mysql-access-3](http://www.runoob.com/wp-content/uploads/2014/01/mysql-access-3.jpg)

## UPDATE操作

这个例子告诉我们如何使用JSTL SQL标签来运行SQL UPDATE语句：
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="java.io.*,java.util.*,java.sql.*"%>
<%@ page import="javax.servlet.http.*,javax.servlet.*" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/sql" prefix="sql"%>
 
<html><head><title>UPDATE 操作</title></head><body><sql:setDataSourcevar="snapshot"driver="com.mysql.jdbc.Driver"url="jdbc:mysql://localhost/TEST"user="root"password="pass123"/><c:setvar="empId"value="102"/><sql:updatedataSource="${snapshot}"var="count">
  UPDATE Employees SET last = 'Ali' WHERE Id = ?
  <sql:paramvalue="${empId}"/></sql:update><sql:querydataSource="${snapshot}"var="result">
   SELECT * from Employees;
</sql:query><tableborder="1"width="100%"><tr><th>Emp ID</th><th>First Name</th><th>Last Name</th><th>Age</th></tr><c:forEachvar="row"items="${result.rows}"><tr><td><c:outvalue="${row.id}"/></td><td><c:outvalue="${row.first}"/></td><td><c:outvalue="${row.last}"/></td><td><c:outvalue="${row.age}"/></td></tr></c:forEach></table></body></html>
访问这个JSP例子，运行结果如下：

![mysql-access-4](http://www.runoob.com/wp-content/uploads/2014/01/mysql-access-4.jpg)

注：使用JSP连接数据库一般不推荐这样使用，这里只把它作为一种数据访问方式。



