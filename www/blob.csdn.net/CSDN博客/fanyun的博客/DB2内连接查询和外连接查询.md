# DB2内连接查询和外连接查询 - fanyun的博客 - CSDN博客
2016年11月30日 20:52:26[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：866
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
DB2内连接查询返回连接表中符合连接条件和查询条件的数据行，下面就详细介绍DB2内连接查询的方法：
DB2内连接查询(INNER
 JOIN)：
DB2内连接查询有两种，显式的和隐式的，返回连接表中符合连接条件和查询条件的数据行。(所谓的链接表就是数据库在做查询形成的中间表)。
　　例如：下面的语句3和语句4的结果是相同的。
　　语句3：隐式的内连接，没有INNERJOIN，形成的中间表为两个表的笛卡尔积。
SELECT O.ID,O.ORDER_NUMBER,C.ID,C.NAME
FROM CUSTOMERS C,ORDERS O
WHERE C.ID=O.CUSTOMER_ID;
　　语句4：显示的内连接，一般称为内连接，有INNERJOIN，形成的中间表为两个表经过ON条件过滤后的笛卡尔积。
SELECT O.ID,O.ORDER_NUMBER,C.ID,C.NAME
FROM CUSTOMERS C INNER JOIN ORDERS O ONC.ID=O.CUSTOMER_ID;
　　语句3和语句4的查询结果：
　　外连接(OUTER JOIN)：
　　外连不但返回符合连接和查询条件的数据行，还返回不符合条件的一些行。外连接分三类：左外连接(LEFTOUTER JOIN)、右外连接(RIGHTOUTER
 JOIN)和全外连接(FULLOUTER JOIN)。
　　三者的共同点是都返回符合连接条件和查询条件(即：内连接)的数据行。不同点如下：
　　左外连接还返回左表中不符合连接条件单符合查询条件的数据行。
　　右外连接还返回右表中不符合连接条件单符合查询条件的数据行。
　　全外连接还返回左表中不符合连接条件单符合查询条件的数据行，并且还返回右表中不符合连接条件单符合查询条件的数据行。全外连接实际是上左外连接和右外连接的数学合集(去掉重复)，即“全外=左外UNION
右外”。
　　说明：左表就是在“(LEFTOUTER JOIN)”关键字左边的表。右表当然就是右边的了。在三种类型的外连接中，OUTER关键字是可省略的。
