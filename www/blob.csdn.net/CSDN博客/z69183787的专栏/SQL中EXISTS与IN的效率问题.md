# SQL中EXISTS与IN的效率问题 - z69183787的专栏 - CSDN博客
2014年05月16日 23:10:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5872
一起学习一下；
有两个简单例子，以说明 “exists”和“in”的效率问题
1) select * from T1 where exists(select 1 from T2 where T1.a=T2.a) ;
    T1数据量小而T2数据量非常大时，T1<<T2 时，1) 的查询效率高。
2) select * from T1 where T1.a in (select T2.a from T2) ;
     T1数据量非常大而T2数据量小时，T1>>T2 时，2) 的查询效率高。
exists 用法：
请注意 1）句中的有颜色字体的部分 ，理解其含义；
其中 “select 1 from T2 where T1.a=T2.a” 相当于一个关联表查询，相当于
“select 1 from T1,T2     where T1.a=T2.a”
但是，如果你当当执行 1） 句括号里的语句，是会报语法错误的，这也是使用exists需要注意的地方。
“exists（xxx）”就表示括号里的语句能不能查出记录，它要查的记录是否存在。
因此“select 1”这里的 “1”其实是无关紧要的，换成“*”也没问题，它只在乎括号里的数据能不能查找出来，是否存在这样的记录，如果存在，这 1） 句的where 条件成立。
in 的用法：
继续引用上面的例子
“2) select * from T1 where T1.a in (select T2.a from T2) ”
这里的“in”后面括号里的语句搜索出来的字段的内容一定要相对应，一般来说，T1和T2这两个表的a字段表达的意义应该是一样的，否则这样查没什么意义。
打个比方：T1，T2表都有一个字段，表示工单号，但是T1表示工单号的字段名叫“ticketid”，T2则为“id”，但是其表达的意义是一样的，而且数据格式也是一样的。这时，用 2）的写法就可以这样：
“select * from T1 where T1.ticketid in (select T2.id from T2) ”
Select name from employee where name not in (select name from student);
Select name from employee where not exists (select name from student);
第一句SQL语句的执行效率不如第二句。
通过使用EXISTS，Oracle会首先检查主查询，然后运行子查询直到它找到第一个匹配项，这就节省了时间。Oracle在执行IN子查询时，首先执行子查询，并将获得的结果列表存放在一个加了索引的临时表中。在执行子查询之前，系统先将主查询挂起，待子查询执行完毕，存放在临时表中以后再执行主查询。这也就是使用EXISTS比使用IN通常查询速度快的原因
[比如在Northwind数据库中有一个查询为](http://blog.csdn.net/xiwu1616/article/details/14160855)
[SELECT
 c.CustomerId,CompanyName FROM Customers c
WHERE EXISTS(
SELECT OrderID FROM Orders o WHERE o.CustomerID=c.CustomerID) 
这里面的EXISTS是如何运作呢？子查询返回的是OrderId字段，可是外面的查询要找的是CustomerID和CompanyName字段，这两个字段肯定不在OrderID里面啊，这是如何匹配的呢？ 
EXISTS用于检查子查询是否至少会返回一行数据，该子查询实际上并不返回任何数据，而是返回值True或False
EXISTS 指定一个子查询，检测 行 的存在。
语法： EXISTS subquery
参数： subquery 是一个受限的 SELECT 语句 (不允许有 COMPUTE 子句和 INTO 关键字)。
结果类型： Boolean 如果子查询包含行，则返回 TRUE ，否则返回 FLASE 。](http://blog.csdn.net/xiwu1616/article/details/14160855)|例表A：TableIn|例表B：TableEx|
|----|----|
|![](http://images.cnblogs.com/cnblogs_com/netserver/tableA.jpg)|![](http://images.cnblogs.com/cnblogs_com/netserver/tableB.jpg)|
（一）. 在子查询中使用 NULL 仍然返回结果集
select * from TableIn where exists(select null)
等同于： select * from TableIn
![](http://images.cnblogs.com/cnblogs_com/netserver/tableA.jpg)
（二）. 比较使用 EXISTS 和 IN 的查询。注意两个查询返回相同的结果。
select * from TableIn where exists(select BID from TableEx where BNAME=TableIn.ANAME)
select * from TableIn where ANAME in(select BNAME from TableEx)
![](http://images.cnblogs.com/cnblogs_com/netserver/008.jpg)
（三）. 比较使用 EXISTS 和 = ANY 的查询。注意两个查询返回相同的结果。
select * from TableIn where exists(select BID from TableEx where BNAME=TableIn.ANAME)
select * from TableIn where ANAME=ANY(select BNAME from TableEx)
![](http://images.cnblogs.com/cnblogs_com/netserver/008.jpg)
NOT EXISTS 的作用与 EXISTS 正好相反。如果子查询没有返回行，则满足了 NOT EXISTS 中的 WHERE 子句。
结论：
EXISTS(包括 NOT EXISTS )子句的返回值是一个BOOL值。 EXISTS内部有一个子查询语句(SELECT ... FROM...)， 我将其称为EXIST的内查询语句。其内查询语句返回一个结果集。 EXISTS子句根据其内查询语句的结果集空或者非空，返回一个布尔值。
一种通俗的可以理解为：将外查询表的每一行，代入内查询作为检验，如果内查询返回的结果取非空值，则EXISTS子句返回TRUE，这一行行可作为外查询的结果行，否则不能作为结果。
分析器会先看语句的第一个词，当它发现第一个词是SELECT关键字的时候，它会跳到FROM关键字，然后通过FROM关键字找到表名并把表装入内存。接着是找WHERE关键字，如果找不到则返回到SELECT找字段解析，如果找到WHERE，则分析其中的条件，完成后再回到SELECT分析字段。最后形成一张我们要的虚表。
WHERE关键字后面的是条件表达式。条件表达式计算完成后，会有一个返回值，即非0或0，非0即为真(true)，0即为假(false)。同理WHERE后面的条件也有一个返回值，真或假，来确定接下来执不执行SELECT。
分析器先找到关键字SELECT，然后跳到FROM关键字将STUDENT表导入内存，并通过指针找到第一条记录，接着找到WHERE关键字计算它的条件表达式，如果为真那么把这条记录装到一个虚表当中，指针再指向下一条记录。如果为假那么指针直接指向下一条记录，而不进行其它操作。一直检索完整个表，并把检索出来的虚拟表返回给用户。EXISTS是条件表达式的一部分，它也有一个返回值(true或false)。
在插入记录前，需要检查这条记录是否已经存在，只有当记录不存在时才执行插入操作，可以通过使用 EXISTS 条件句防止插入重复记录。
INSERT INTO TableIn (ANAME,ASEX) 
SELECT top 1 '张三', '男' FROM TableIn
WHERE not exists (select * from TableIn where TableIn.AID = 7)
EXISTS与IN的使用效率的问题，通常情况下采用exists要比in效率高，因为IN不走索引，但要看实际情况具体使用：
IN适合于外表大而内表小的情况；EXISTS适合于外表小而内表大的情况。
