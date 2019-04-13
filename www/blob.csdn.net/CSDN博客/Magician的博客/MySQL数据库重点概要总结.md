
# MySQL数据库重点概要总结 - Magician的博客 - CSDN博客


2018年04月03日 15:14:30[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：238


[目录](#目录)
[前言：](#前言)[1、结构化查询语言(Structured Query Language)简称SQL。](#1结构化查询语言structured-query-language简称sql)
[2、基本SELECT语句语法](#2基本select语句语法)
[3、多表链接](#3多表链接)
[4、笛卡尔积](#4笛卡尔积)
[5、ANSI SQL ANSI SQL：1999标准的连接语法](#5ansi-sql-ansi-sql1999标准的连接语法)
[6、单行函数和分组函数的区别：](#6单行函数和分组函数的区别)
[7、分组函数](#7分组函数)
[8、SQL语句的执行顺序：](#8sql语句的执行顺序)
[9、子查询：](#9子查询)
[10、Data Manipulation Language ,简称DML，主要用来实现对 数据库表中的数据进行操作](#10data-manipulation-language-简称dml主要用来实现对-数据库表中的数据进行操作)
[11、多行子查询](#11多行子查询)
[总结：](#总结)


# 目录
# 前言：
最近由于工作需要，把数据库的知识复习了一遍。以下是自己总结的一些个人认为比较重要的知识点（有些是个人不太熟的知识点就记下来当笔记吧），如有不足或错误，请大家及时指出。
## 1、
## 结构化查询语言(Structured Query Language)简称SQL。
## 2、
## 基本SELECT语句语法
```python
SELECT [DISTINCT]{ |column|expression [alias],
```
```python
...
```
```python
} FROM table;
```
## 3、
## 多表链接
连接是在多个表之间通过 定的连接条件 连接是在多个表之间通过一定的连接条件，使表之间发生关联，进而**能从多个表之间获取数据**。
N个表相连时，至少需要N－1个连接条件
## 4、笛卡尔积
第一个表中的所有行和第二个表中的所有行都发生连接。
**笛卡尔积的写法**
```python
SQL> SELECT emp
```
```python
.empno
```
```python
, emp
```
```python
.ename
```
```python
, emp
```
```python
.deptno
```
```python
,
```
```python
2
```
```python
dept
```
```python
.deptno
```
```python
, dept
```
```python
.loc
```
```python
3
```
```python
FROM emp, d t ep
```
```python
;
```
## 5、
## ANSI SQL ANSI SQL：1999标准的连接语法
```python
SELECT
```
```python
table1.
```
```python
column
```
```python
, table2.
```
```python
column
```
```python
FROM
```
```python
table1
[
```
```python
CROSS
```
```python
JOIN
```
```python
table2] |
[
```
```python
NATURAL
```
```python
JOIN
```
```python
table2] |
[
```
```python
JOIN
```
```python
table2
```
```python
USING
```
```python
(
```
```python
USING
```
```python
(
```
```python
column
```
```python
name column_name)] |
[
```
```python
JOIN
```
```python
table2
```
```python
ON
```
```python
(table1.column_name = table2.column_name)] |
[
```
```python
LEFT
```
```python
|
```
```python
RIGHT
```
```python
|
```
```python
FULL
```
```python
OUTER
```
```python
JOIN
```
```python
[
```
```python
LEFT
```
```python
|
```
```python
RIGHT
```
```python
|
```
```python
FULL
```
```python
OUTER
```
```python
JOIN
```
```python
table2
```
```python
ON
```
```python
(table1.column_name = table2.column_name)];
```
## 6、
## 单行函数和分组函数的区别：
![这里写图片描述](https://img-blog.csdn.net/20180403143615290?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 7、
## 分组函数
– 分组函数是对表中一组记录进行操作，每组只返回一个结果，即首先要对表记录进行分组，然后再进行操作汇总， 每组返回一个结果，分组时可能是整个表分为一组，也可能根据条件分成多组。
分组函数常用到以下五个函数：
• MIN
• MAX
• SUM
• AVG
• COUNT
常用的语法格式：
```python
SELECT
```
```python
[
```
```python
column
```
```python
,] group_function(
```
```python
column
```
```python
)
```
```python
FROM
```
```python
tab el
[
```
```python
WHERE
```
```python
condition]
[
```
```python
GROUP
```
```python
BY
```
```python
column
```
```python
]
[
```
```python
HAVING
```
```python
group_function(
```
```python
column
```
```python
)expression
[
```
```python
ORDER
```
```python
BY
```
```python
column
```
```python
| group_function(
```
```python
column
```
```python
)expression];
```
一些错误案例：
![这里写图片描述](https://img-blog.csdn.net/20180403143942412?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 8、
## SQL语句的执行顺序：
![这里写图片描述](https://img-blog.csdn.net/20180403144236577?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
SELECT语句执行过程：
– 1.通过FROM子句中找到需要查询的表 子句中找到需要查询的表；
– 2.通过WHERE子句进行非分组函数筛选判断；
– 3.通过GROUP BY子句完成分组操作；
– 4.通过HAVING子句完成组函数筛选判断；
– 5.通过SELECT子句选择显示的列或表达式及组函数；
– 6.通过ORDER BY子句进行排序操作。
案例
![这里写图片描述](https://img-blog.csdn.net/20180403144158702?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180403144158702?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180403144345287?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180403144345287?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180403144401496?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 9、
## 子查询：
![这里写图片描述](https://img-blog.csdn.net/20180403144752373?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
子查询语法：
```python
SELECT
```
```python
select_list
```
```python
FROM
```
```python
table
```
```python
WHERE
```
```python
expr operator
(
```
```python
SELECT
```
```python
select_list
```
```python
FROM
```
```python
table
```
```python
);
```
• 括号内的查询叫做子查询，也叫内部查询，**先于主查询执行**。
• 子查询的结果被主查询（外部查询）使用
• expr operator包括比较运算符。
–**单行运算符：>、=、>=、<、<>、<=**
–**多行运算符： IN、ANY、ALL**
• 子查询可以嵌于以下SQL子句中：
–WHERE子句
–HAVING子句
–FROM子句
![这里写图片描述](https://img-blog.csdn.net/20180403145000706?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180403145000706?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**• 子查询使用指导**
–子查询要**用括号括起来**
–将子查询放在比较运算符的右边
–对于**单行子查询要使用单行运算符**（单行子查询是针对子查询的返回值只有一个）
–对于**多行查询要使用多行运算符**
**错误案例（子查询中使用分组函数返回多个结果）：**
![这里写图片描述](https://img-blog.csdn.net/20180403145054799?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 10、
## Data Manipulation Language ,简称DML，主要用来实现对 数据库表中的数据进行操作
索引( INDEX ):
– 是对数据库表中一个或多个列的值进行排序的**一种数据库对象**。
– 在数据库中,**通过索引可以加速对表的查询速度**;
**索引缺点**
• 占用空间；
• 降低DML的操作速度；
**适合创建索引情况**
• 表数据量很大
• 要查询的结果集在2%-4%左右
• 经常用来做WHERE条件中的列或者多表连接的列
• 查询列的数据范围分布很广
• 查询列中包含大量的NULL值,因为空值不包含在索引中
**不适合创建索引情况**
• 数据量很小的表
• 在查询中不常用来作为查询条件的列
• 频繁更新的表
• 索引列作为表达式的一部分被使用时，比如常查询的条件是SALARY*12 SALARY*12，此时在SALARY列上创建索引是没有效果的
• 查询条件中有单行函数时，用不上索引。
## 11、多行子查询
–子查询返回记录的条数 可以是一条或多条。
–和多行子查询进行比较时，需要使用多行操作符，多行操作符包括：
• IN
• ANY
• ALL
–IN操作符和以前介绍的功能一致，判断是否与子查询 的任意一个返回值相同。（单行子查询操作符不能接子查询中含有分组函数执行语句的情况）
![这里写图片描述](https://img-blog.csdn.net/20180406144250256?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180406144250256?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
ANY的使用
– ANY：表示和子查询的任意一行结果进行比较，有一个满足 条件即可。
• < ANY：表示小于子查询结果集中的任意 表示小于子查询结果集中的任意一个 ，即小于最 大值就可以。
• > ANY：表示大于子查询结果集 个 中的任意一 ，即大于最小值就可以。
• = ANY：表示等于子查询结果中的任意一个，即等于谁都 可以，相当于IN。
![这里写图片描述](https://img-blog.csdn.net/20180406144317148?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180406144317148?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
• ALL的使用
– ALL：表示和子查询的所有行结果进行比较，每一行必须都 满足条件。
• < ALL:表示小于子查询结果集中的所有行 表示小于子查询结果集中的所有行，即小于最小 值。
• > ALL:表示大于子查询结果集 行 中的所有 ，即大于最大 值。
• = ALL :表示等于子查询结果集中的所有行,即等于所有值，通常无意义。
![这里写图片描述](https://img-blog.csdn.net/20180406144342418?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 总结：
SQL这块需要掌握以下这些知识点：
掌握单表查询、多表查询、分组、子查询等查询方法，能写出相对复杂的查询语句，重点中的重点。
掌握Oracle数据库常用的单行函数及分组函数。
掌握插入数据、修改数据、删除数据语句。
总结成一句话就是能够利用SQL进行数据分析就OK了。
除此之外还需要了解一些相关的数据库概念。这里就不一一列举了。如果各位网友看完之后想练下手，（含有各种版本数据库的建表数据和对应的案例练习题以及答案）
请参考：[https://blog.csdn.net/qq_16633405/article/details/78483269](https://blog.csdn.net/qq_16633405/article/details/78483269)

