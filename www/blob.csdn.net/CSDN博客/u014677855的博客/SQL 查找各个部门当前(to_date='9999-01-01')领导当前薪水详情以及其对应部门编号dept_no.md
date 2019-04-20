# SQL 查找各个部门当前(to_date='9999-01-01')领导当前薪水详情以及其对应部门编号dept_no - u014677855的博客 - CSDN博客
2018年08月09日 21:50:18[xiaobigben](https://me.csdn.net/u014677855)阅读数：184
依然以牛客上面例题出发，主要是帮助自己记忆一下数据库操作相关，免得一段时间不用又忘记了。 
 今天主要回顾SQL语句中的连接相关知识。 
 一、SQL JOIN 
      SQL JOIN子句主要用于把来自两个或多个表的行连接起来，基于这些表这件的共同字段。 
       最常见的SQL JOIN类型有SQL INNER JOIN，SQL LEFT JOIN，SQL RIGHT JOIN,SQL FULL JOIN。 
  二、SQL INNER JOIN 
      SQL INNER JOIN是最简单的JOIN，从多个表中返回满足JOIN条件的所有行。如果表中至少有一个匹配，就返回行。返回的表是组合之后的一个新表。 
        INNER JOIN的语法是： 
        SELECT column_name(s) 
        FROM table1 
        INNER JOIN table2 
        ON table1.column_name=table2.column_name; 
     例子：牛客网中的例子： 
     查找各个部门当前(to_date=’9999-01-01’)领导当前薪水详情以及其对应部门编号dept_no 
    查找各个部门当前(to_date=’9999-01-01’)领导当前薪水详情以及其对应部门编号dept_no 
    CREATE TABLE `dept_manager` ( 
`dept_no` char(4) NOT NULL, 
`emp_no` int(11) NOT NULL, 
`from_date` date NOT NULL, 
`to_date` date NOT NULL, 
    PRIMARY KEY (`emp_no`,`dept_no`)); 
    CREATE TABLE `salaries` ( 
`emp_no` int(11) NOT NULL, 
`salary` int(11) NOT NULL, 
`from_date` date NOT NULL, 
`to_date` date NOT NULL, 
    PRIMARY KEY (`emp_no`,`from_date`)); 
      表1dept_manager的主键是emp_no,dept_no;领导编号，部门编号 
      表2salaries的主键是emp_no,from_date，领导编号和上任日期 
      那么两个表是基于领导编号连接起来的，只要领导编号有一个匹配，就可以返回一行。 
      select salaries.emp_no, salaries.salary,salaries.from_date,salaries.to_date,dept_manager.dept_no 
from salaries INNER JOIN dept_manager 
on dept_manager.emp_no = salaries.emp_no 
and salaries.to_date = ‘9999-01-01’ 
and dept_manager.to_date = ‘9999-01-01’
