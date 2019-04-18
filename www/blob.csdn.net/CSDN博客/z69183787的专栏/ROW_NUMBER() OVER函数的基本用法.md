# ROW_NUMBER() OVER函数的基本用法 - z69183787的专栏 - CSDN博客
2014年04月10日 09:57:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2347
 转自：[http://www.cnblogs.com/icebutterfly/archive/2009/08/05/1539657.html](http://www.cnblogs.com/icebutterfly/archive/2009/08/05/1539657.html)
语法：ROW_NUMBER() OVER(PARTITION BY COLUMN ORDER BY COLUMN)
简单的说row_number()从1开始，为每一条分组记录返回一个数字，这里的ROW_NUMBER() OVER (ORDER BY xlh DESC) 是先把xlh列降序，再为降序以后的没条xlh记录返回一个序号。 
示例： 
xlh           row_num 
1700              1 
1500              2 
1085              3 
710                4
**row_number() OVER (PARTITION BY COL1 ORDER BY COL2)** 表示根据COL1分组，在分组内部根据 COL2排序，而此函数计算的值就表示每组内部排序后的顺序编号（组内连续的唯一的)
实例：
初始化数据
create table employee (empid int ,deptid int ,salary decimal(10,2))
insert into employee values(1,10,5500.00)
insert into employee values(2,10,4500.00)
insert into employee values(3,20,1900.00)
insert into employee values(4,20,4800.00)
insert into employee values(5,40,6500.00)
insert into employee values(6,40,14500.00)
insert into employee values(7,40,44500.00)
insert into employee values(8,50,6500.00)
insert into employee values(9,50,7500.00)
数据显示为
empid       deptid      salary
----------- ----------- ---------------------------------------
1           10          5500.00
2           10          4500.00
3           20          1900.00
4           20          4800.00
5           40          6500.00
6           40          14500.00
7           40          44500.00
8           50          6500.00
9           50          7500.00
需求：根据部门分组，显示每个部门的工资等级
预期结果：
empid       deptid      salary                                  rank
----------- ----------- --------------------------------------- --------------------
1           10          5500.00                                 1
2           10          4500.00                                 2
4           20          4800.00                                 1
3           20          1900.00                                 2
7           40          44500.00                               1
6           40          14500.00                               2
5           40          6500.00                                 3
9           50          7500.00                                 1
8           50          6500.00                                 2
SQL脚本：
SELECT *, Row_Number() OVER (partition by deptid ORDER BY salary desc) rank FROM employee
转自：[http://www.cnblogs.com/digjim/archive/2006/09/20/509344.html](http://www.cnblogs.com/digjim/archive/2006/09/20/509344.html)
我们知道，SQL Server 2005和SQL Server 2000 相比较，SQL Server 2005有很多新特性。这篇文章我们要讨论其中的一个新函数Row_Number()。数据库管理员和开发者已经期待这个函数很久了，现在终于等到了！
 通常，开发者和管理员在一个查询里，用临时表和列相关的子查询来计算产生行号。现在SQL Server 2005提供了一个函数，代替所有多余的代码来产生行号。
 我们假设有一个资料库[EMPLOYEETEST]，资料库中有一个表[EMPLOYEE]，你可以用下面的脚本来产生资料库，表和对应的数据。
 USE [MASTER]
GO
IF  EXISTS (SELECT NAME FROM SYS.DATABASES WHERE NAME = N'EMPLOYEE TEST')
DROP DATABASE [EMPLOYEE TEST]
GOCREATE DATABASE [EMPLOYEE TEST]
GOUSE [EMPLOYEE TEST]
GO
IF  EXISTS SELECT * FROM SYS.OBJECTS HERE OBJECT_ID = OBJECT_ID(N'[DBO].[EMPLOYEE]') AND TYPE IN (N'U'))
DROP TABLE [DBO].[EMPLOYEE]
GO
CREATE TABLE EMPLOYEE (EMPID INT, FNAME VARCHAR(50),LNAME VARCHAR(50))
GOINSERT INTO EMPLOYEE  (EMPID, FNAME, LNAME) VALUES (2021110, 'MICHAEL', 'POLAND')
INSERT INTO EMPLOYEE  (EMPID, FNAME, LNAME) VALUES (2021110, 'MICHAEL', 'POLAND')
INSERT INTO EMPLOYEE  (EMPID, FNAME, LNAME) VALUES (2021115, 'JIM', 'KENNEDY')
INSERT INTO EMPLOYEE  (EMPID, FNAME, LNAME) VALUES (2121000, 'JAMES', 'SMITH')
INSERT INTO EMPLOYEE  (EMPID, FNAME, LNAME) VALUES (2011111, 'ADAM', 'ACKERMAN')
INSERT INTO EMPLOYEE  (EMPID, FNAME, LNAME) VALUES (3015670, 'MARTHA', 'LEDERER')
INSERT INTO EMPLOYEE  (EMPID, FNAME, LNAME) VALUES (1021710, 'MARIAH', 'MANDEZ')
GO
 我们可以用下面的脚本查询EMPLOYEE表。
 SELECT EMPID, RNAME, LNAME FROM EMPLOYEE
 这个查询的结果应该如图1.0 
|2021110|MICHAEL|POLAND|
|----|----|----|
|2021110|MICHAEL|POLAND|
|2021115|JIM|KENNEDY|
|2121000|JAMES|SMITH|
|2011111|ADAM|ACKERMAN|
|3015670|MARTHA|LEDERER|
|1021710|MARIAH|MANDEZ|
图1.0
 在SQL Server 2005，要根据这个表中的数据产生行号，我通常使用下面的查询。 
SELECT ROWID=IDENTITY(int,1,1) , EMPID, FNAME, LNAME INTO EMPLOYEE2 FROM EMPLOYEE ORDER BY EMPID
 这个查询创建了一个新的表，用identify函数来产生行号。我们用下面的查询来看看这个表的数据。 
SELECT ROWID, EMPID, FNAME, LNAME FROM EMPLOYEE2
 上面的查询结果如图1.1 
|1|1021710|MARIAH|MANDEZ|
|----|----|----|----|
|2|2011111|ADAM|ACKERMAN|
|3|2021110|MICHAEL|POLAND|
|4|2021110|MICHAEL|POLAND|
|5|2021115|JIM|KENNEDY|
|6|2121000|JAMES|SMITH|
|7|3015670|MARTHA|LEDERER|
图1.1
 这个查询结果很明显EMP=2021110的行是重复的数据。
 要删除EMPID=2021110的重复数据，我们必须在EMPLOYEE2表中删除，不能直接在EMPLOYEE中删除。
 SQL Server 2005提供了一个新的函数（Row_Number()）来产生行号。我们可以使用这个新函数来删除原来表中的重复数据，只用通常的表达方式再加上Row_Number()函数。
 让我们用Row_Number()函数根据EMPID来产生ROWID。
 SELECT ROW_NUMBER() OVER (ORDER BY EMPID ASC) AS ROWID, * FROM EMPLOYEE
 上面的查询结果如图1.2 
|1|1021710|MARIAH|MANDEZ|
|----|----|----|----|
|2|2011111|ADAM|ACKERMAN|
|3|2021110|MICHAEL|POLAND|
|4|2021110|MICHAEL|POLAND|
|5|2021115|JIM|KENNEDY|
|6|2121000|JAMES|SMITH|
|7|3015670|MARTHA|LEDERER|
图1.2
 在这个结果中，我们可以区别EMPID是2021110的重复数据。
 我们可以用通用表查询表达式和Row_Numner()函数来选出重复的那行数据。
 WITH [EMPLOYEE ORDERED BY ROWID] AS
(SELECT ROW_NUMBER() OVER (ORDER BY EMPID ASC) AS ROWID, * FROM EMPLOYEE)
SELECT * FROM [EMPLOYEE ORDERED BY ROWID] WHERE ROWID =4
上面的查询结果如图1.3 
|4|2021110|MICHAEL|POLAND|
图1.3
 这一行重复的数据可以用下面这个通用表和Row_Number()函数来删除。
 WITH [EMPLOYEE ORDERED BY ROWID] AS
(SELECT ROW_NUMBER() OVER (ORDER BY EMPID ASC) AS ROWID, * FROM EMPLOYEE)
DELETE FROM [EMPLOYEE ORDERED BY ROWID] WHERE ROWID =4
 删除以后，我们可以用下面的查询语句看一下结果。
 SELECT * FROM EMPLOYEE
 这个查询结果如图1.4 
|2021110|MICHAEL|POLAND|
|----|----|----|
|2021115|JIM|KENNEDY|
|2121000|JAMES|SMITH|
|2011111|ADAM|ACKERMAN|
|3015670|MARTHA|LEDERER|
|1021710|MARIAH|MANDEZ|
图 1.4
 这里我们可以看到，重复的数据已经被删除了。
 总结
在这篇文章中，我们讨论了SQL Server 2005 的新特性Row_Number()函数，还有通常的表表达式，然后如何使用这两个来删除重复的行。
