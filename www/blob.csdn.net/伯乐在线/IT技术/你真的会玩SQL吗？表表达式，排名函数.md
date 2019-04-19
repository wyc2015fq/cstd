# 你真的会玩SQL吗？表表达式，排名函数 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [欢醉](http://www.jobbole.com/members/huanzui2336) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
这次讲的有些可能是经常用但不会注意到，所以来统一总结一下用法。
我们往往需要临时存储某些结果集。除了用临时表和表变量，还可以使用公用表表达式的方法。
### **表表达式**
- 期待单个值的地方可以使用**标量子查询**
- 期待多个值的地方可以使用**多值子查询**
- 在期待出现表的地方可用**表值子查询**或**表表达式**
**1.派生表**
是从查询表达式派生出虚拟结果表的表表达式，派生表的存在范围只是外部查询。
使用形式：from 派生表 as 派生表列名
规则：
- 所有列必须有名称
- 列名必须唯一
- 不允许使用order by（除非指定了top）
不同于标量和多值子查询，派生表不能是相关的，它必须是独立的。
**2.公用表表达式（CTE）**
**非递归公用表表达式**（CTE）是查询结果仅仅一次性返回一个结果集用于外部查询调用。


MySQL
```
WITH CTE_Test
　　AS
　　(
    　　SELECT * FROM Person_1
　　)
　　SELECT * FROM CTE_Test AS a　　--第一次引用
　　INNER JOIN  CTE_Test AS b　　　 --第二次引用
　　ON a.Id = b.Id
　　ORDER BY a.Id DESC--SELECT * FROM CTE_Test 再查询一次会报错
```
### **递归公用表达式**
来引用他人的一个示例：
先建一张表栏目表如下，栏目Id，栏目名称，栏目的父栏目
![](http://jbcdn2.b0.upaiyun.com/2015/11/9c1be1b090858b1ca656f91ffb3e99cc.jpg)
现在使用CTE查询其每个栏目是第几层栏目的代码如下：


MySQL
```
WITH COL_CTE(Id,Name,ParentId,tLevel )
AS
(
    --基本语句
    SELECT Id,Name,ParentId,0 AS tLevel FROM Col
    WHERE ParentId = 0
    UNION ALL
    --递归语句
    SELECT c.Id,c.Name,c.ParentId,ce.tLevel+1 AS tLevel FROM COL as c 
    INNER JOIN COL_CTE AS ce 　　--递归调用
    ON c.ParentId = ce.Id
)
SELECT * FROM COL_CTE
```
结果：
![](http://jbcdn2.b0.upaiyun.com/2015/11/dad2f617ef386f6606892d1a3c49cb4f.jpg)
0表示顶级栏目。1就是1级栏目
### **排名函数**
**四个排名函数：**
**　　1.row_number**
**　　2.rank**
**　　3.dense_rank**
**　　4.ntile**
排名函数order by子句是必需的。我们这里不讲定义，直接讲实例用法。
**利用row_number生成连续行号**


MySQL
```
SELECT  empid ,
        qty ,
        ROW_NUMBER() OVER ( ORDER BY qty ) AS rownum
FROM    sales
ORDER BY qty
```
**小的分组范围内排序**，**通过PARTITION BY选项来重新排序，给数据分区或者数据区域唯一的递增序号**
如：LastName以‘A’开头的作为第一组，在这个组内进行排序。以‘B’开头的作为第二组，在这个组内排序。以‘C’开头的作为第三组，在这个组内进行排序，如此等等


MySQL
```
select
ROW_NUMBER() over(PARTITION by substring(LastName,1,1) order by LastName) as RowNum,
FirstName+' '+ LastName as FullName
from HumanResources.vEmployee
```
结果
![](http://jbcdn2.b0.upaiyun.com/2015/11/9c57efbc054aed6762f184222ea45df3.png)
假设LastName以‘A’开头的是男子组，这个组有共有三个人，Kim Abercrombie是冠军，Jay Adams是亚军，Nancy Anderson是季军。假设LastName以‘B’开头的是女子组，这个组只有一个人Bryan Baker，无论如何她都是冠军。等等如此类推。这样一眼就能看出他们的小组名次了。
**RANK**
果有同时撞线的情况发生应该怎么计名次呢？例如A第一个撞线，B和C同时第二个撞线，D第三个撞线，如果我们想把D的名次计为第4名应该怎么处理呢？就是说不计顺序名次，只计人数。这时就可以使用RANK函数了。
在order by子句中定义的列上，如果返回一行数据与另一行具有相同的值，rank函数将给这些行赋予相同的排名数值。在排名的过程中，保持一个内部计数值，当值有所改变时，排名序号将有一个跳跃。


MySQL
```
SELECT  ROW_NUMBER() OVER ( ORDER BY Department ) AS RowNum ,
        RANK() OVER ( ORDER BY Department ) AS Ranking ,
        FirstName + ' ' + LastName AS FullName ,
        Department
FROM    HumanResources.vEmployeeDepartment
ORDER BY RowNum
```
rank()函数右面也要跟上一个over子句。为了看到效果我们以Department作为排序字段，可以看到RowNum作为升序连续排名，Ranking作为计同排名，当Department的值相同时，Ranking中的值保持不变，当Ranking中的值发生变化时，Ranking列中的值将跳跃到正确的排名数值。来看结果：
![](http://jbcdn2.b0.upaiyun.com/2015/11/5270cb9a3b3b8419ca5afd95e2b1292a.png)
从这个结果中我们可以说这次马拉松赛跑的排名是：Tengiz Kharatishvili，Zainal Arifin，Sean Chai，Karen Berge，Chris Norred并列第1，Michael Sullivan，Sharon Salavaria，Roberto Tamburello，Gail Erickson，Jossef Goldberg并列第6，如此等等。
**DENSE_RANK**
A第一个撞线，B和C同时第二个撞线，D第三个撞线，如果我们想把B和C的名次计位第2名，D的名次计为第3名应该怎么处理呢？就是说考虑并列名次。这里使用DENSE_RANK函数


MySQL
```
SELECT  ROW_NUMBER() OVER ( ORDER BY Department ) AS RowNum ,
        DENSE_RANK() OVER ( ORDER BY Department ) AS Ranking ,
        FirstName + ' ' + LastName AS FullName ,
        Department
FROM    HumanResources.vEmployeeDepartment
ORDER BY RowNum
```
结果
![](http://jbcdn2.b0.upaiyun.com/2015/11/551a55691dc632527886363d6393dd82.png)
按照这个结果，我们可以说这次马拉松赛跑的排名是：Tengiz Kharatishvili，Zainal Arifin，Sean Chai，Karen Berge，Chris Norred并列第1，Michael Sullivan，Sharon Salavaria，Roberto Tamburello，Gail Erickson，Jossef Goldberg，Terri Duffy并列第2，等等。
**NTILE**
梭罗是铅笔的发明者，不过他没有申请专利。据说他天赋异禀，在父亲的铅笔厂里面打包铅笔的时候，从一堆铅笔里面抓取一把，每次都能精确地抓到一打12支。他在森林中目测两颗树之间的距离，和护林员用卷尺测量的结果相差无几。现在如果我们想从一张表中抓取多比数据，每一笔都是相同的数目，并且标明第几组该怎么办呢？NTILE函数提供了这个功能。


MySQL
```
SELECT  NTILE(30) OVER ( ORDER BY Department ) AS NTiles ,
        FirstName + ' ' + LastName AS FullName ,
        Department
FROM    HumanResources.vEmployeeDepartment
```
![](http://jbcdn2.b0.upaiyun.com/2015/11/3715f4ffef131d35232a300bbe310ec5.png)
现在我们要抓取30个组的数据，并保证尽可能的保证每组数目相同
这个视图中共290条数据，290/30=9.7约等于10，所以每组10条数据，如图每一条数据都有一个组号。这个结果要比索罗精确。
**练习**
**用CTE删除重复数据**
创建一个用于测试的表，并在该表里插入几条数据(包括重复的数据)


MySQL
```
/* Create Table with 7 records- 3 are duplicate records*/
CREATE TABLE DeleteDuplicateTest ( Col1 INT, Col2 INT )
INSERT  INTO DeleteDuplicateTest
        SELECT  1 ,
                1
        UNION ALL
        SELECT  1 ,
                1 --duplicate
        UNION ALL
        SELECT  1 ,
                1 --duplicate
        UNION ALL
        SELECT  1 ,
                2
        UNION ALL
        SELECT  1 ,
                2 --duplicate
        UNION ALL
        SELECT  1 ,
                3
        UNION ALL
        SELECT  1 ,
                4
GO
```
用CTE删除重复数据4条惟一的记录
![](http://jbcdn2.b0.upaiyun.com/2015/11/9cacc575388484f13a2727ad639f990d.png)
参考SQL

MySQL
```
WITH    CTE ( COl1, Col2, DuplicateCount )
          AS ( SELECT   COl1 ,
                        Col2 ,
                        ROW_NUMBER() OVER ( PARTITION BY COl1, Col2 ORDER BY Col1 ) AS DuplicateCount
               FROM     DELETEDUPLICATETEST
             )
    DELETE  FROM CTE
    WHERE   DuplicateCount > 1
GO
/*用SQL SERVER 的CTE，它将重新生成一个相同的但附加了一行编号的表。在此方案中，我们有Col1，Col2以及包含这个两列重复数的列，对于不同的查询，这个重复数的列可能有不同的值。另一点需要注意的是，一旦CTE被创建，DELETE语句就可以被运行了。这里我们设置一个条件——当我们读取到的记录大于一条（即有重复数据），我们删除除了第一条的所有其他（这里可能有点绕，简单的话就是保留一条重复的记录）*/
```
以上部分示例总结于：[T-SQL中的排名函数](http://www.cnblogs.com/jhxk/articles/2531595.html)
