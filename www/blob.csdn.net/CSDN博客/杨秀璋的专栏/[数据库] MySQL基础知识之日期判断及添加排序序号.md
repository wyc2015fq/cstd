
# [数据库] MySQL基础知识之日期判断及添加排序序号 - 杨秀璋的专栏 - CSDN博客

2016年09月14日 08:48:05[Eastmount](https://me.csdn.net/Eastmount)阅读数：5428所属专栏：[数据库实战开发设计与优化](https://blog.csdn.net/column/details/14842.html)



这篇文章主要记录MySQL中遇到的几个基础问题，希望文章对你有所帮助！包括：
1.日期类型的判断
2.decode函数的替代方法
3.查询语句中添加一个排序的序号
4.子函数查询select a.*
**1.日期类型判断**
日期类型主要是：DATE(显示格式：YYYY-MM-DD)
DATETTIME(显示格式：YYYY-MM-DD HH:MM:SS)
假设存在学生表Student，如下图所示：
![](https://img-blog.csdn.net/20160819195231242)
![](https://img-blog.csdn.net/20160823211205705)

(1)如何判断日期的年月日
判断日：date_format(birthday,'%Y-%m-%d')='2016-08-23'
判断月：date_format(birthday,'%Y-%m')='2016-08'
判断年：date_format(birthday,'%Y')='2016'
对应的SQL语句如下：

```python
select * from student where date_format(birthday,'%Y-%m-%d')='2016-08-23';
王二	111111	1	2016-08-23 21:05:46.000000	94	85
select * from student where date_format(birthday,'%Y-%m')='2016-08';
王二	111111	1	2016-08-23 21:05:46.000000	94	85
杨三	123456	3	2016-08-17 21:06:28.000000	89	75
刘五	000000	4	2016-08-18 21:07:02.000000	61	92
select * from student where date_format(birthday,'%Y')='2016';
王二	111111	1	2016-08-23 21:05:46.000000	94	85
李四	123456	2	2016-07-23 21:06:15.000000	76	87
杨三	123456	3	2016-08-17 21:06:28.000000	89	75
刘五	000000	4	2016-08-18 21:07:02.000000	61	92
```
(2)如何判断时间范围，如在2016-08-17到2016-08-20之间的数据
通常会使用between and来进行比较
```python
select * from student where date_format(birthday,'%Y-%m-%d') 
between '2016-08-17' and '2016-08-20';
```
如下图所示：
![](https://img-blog.csdn.net/20160823213114002)
(3)获取日期的年月日，使用YEAR()、month()、day()函数

```python
select username, stuid, YEAR(birthday), month(birthday), day(birthday) from student;
```
输出如下图所示：
![](https://img-blog.csdn.net/20160823213331130)

**2.decode取代函数**
在Oracle中存在decode(b,0,0,a/b)函数，用于防止分母为0，如果b为0返回0，否则返回a/b。但是MySQL中没有该函数，需要通过CASE WHEN进行替换。
替代代码：case b when 0 then 0 else a/b end
具体含义：当b为0时返回0，否则返回a/b并结束
假设存在一个黄六，英语成绩为0，需要统计"数学/英语"的结果：
![](https://img-blog.csdn.net/20160823214119924)

```python
select username, stuid, math, english, math/english from student;
```
此时黄六输出NULL空值，如下图所示：
![](https://img-blog.csdn.net/20160823214332439)
而使用了case判断后：
```python
select username, stuid, math, english, 
case english when 0 then 0 else math/english end as cf from student;
```
输出如下图所示，同时可以输出自定义的值，如'分母为0'等。
![](https://img-blog.csdn.net/20160823214508720)

**3.添加排序序号**
通常MySQL排序使用order by（从小到大），然后增加desc是从大到小排序。

```python
select username, stuid, math, english from student order by math;
select username, stuid, math, english from student order by math desc;
```
输出如下图所示：
![](https://img-blog.csdn.net/20160823220043637)![](https://img-blog.csdn.net/20160823220100731)
但是如果需要增加一个序号，自定义的排名呢？
假设前段显示需要序号，通常是通过Java设置一个Num来加1显示，但是数据库如何实现呢？
通过：select @rownum:=@rownum+1 as num, name from table,
(select @rownum:=0) temp;
输出如下图所示，注意这是自定义一列，而不是表中某个字段属性。
```python
select @rownum:=@rownum+1 as num, username, stuid, math, english 
from student,(select @rownum:=0) B order by math;
```
![](https://img-blog.csdn.net/20160823220848386)
另一种方法：
set @i:= 0; select @i:= @i + 1 as `order`, a.* from a order by col desc;
同时，你可能会遇到一个问题，如下面的SQL语句：

```python
select @rownum:=@rownum+1 as num, A.UnitDepName, sum(CostSum), A.UnitArea, 
    (case A.UnitArea when 0 then 0 else sum(CostSum)/(A.UnitArea) end) as avCostSum 
from all_unitdepinfo A, gc_nhfxxdwzxfhzmon T, (select @rownum:=0) B 
where (A.UnitCode=T.UnitCode and A.UnitDepCode=T.UnitDepCode) and (A.UnitCode='GC001') 
group by A.UnitDepCode order by sum(CostSum)/sum(A.UnitArea) desc;
```
它的功能是统计各个学院能耗，并且排名、添加相应的序号，但输出的结果如下：
![](https://img-blog.csdn.net/20160827173729385)
存在的问题是自定义序号应该是从1到n，而它序列弄错了。怎么解决呢？
采用子查询进行修改，即：
select @rownum:=@rownum+1 as num, D.* from  (....) D,(select @rownum:=0) B;
```python
select @rownum:=@rownum+1 as num, D.* from 
	(select A.UnitDepName, sum(CostSum), sum(A.UnitArea), 
	(case A.UnitArea when 0 then 0 else sum(CostSum)/sum(A.UnitArea) end) as avCostSum 
	from all_unitdepinfo A, gc_nhfxxdwzxfhzmon T 
	where (A.UnitCode=T.UnitCode and A.UnitDepCode=T.UnitDepCode) and (A.UnitCode='GC001') 
	group by A.UnitDepCode order by sum(CostSum)/sum(A.UnitArea) desc) D,
(select @rownum:=0) B;
```
输出结果如下图所示，即可：
![](https://img-blog.csdn.net/20160827174043857)
注意：只有当你在实际操作数据库过程中遇到了该问题，才知道这篇文章的好处！


**4.子函数select a.*查询**
如果需要连接两个查询，并通过子查询实现，如果每个值能一一对应上，建议使用select a.*

```python
SELECT a.*, b.*   
FROM   
(SELECT SUM(DOMESTIC_TRAIN) + SUM(OVERSEA_TRAIN_TOTAL) AS zj,   
    SUM(DEGREE_PHD) AS qzgdbsx,   
    SUM(DOMESTIC_TRAIN) AS jnjxrcs,   
    SUM(OVERSEA_TRAIN_TOTAL) AS jwjxrcs   
 FROM TRAIN_INTERFLOW   
 where YEAR_START=to_char(sysdate,'yyyy')-2  
) a,   
(SELECT SUM(PARTICIPANT_NUMBER) AS cyjglxkrcs   
 FROM EDU_REVOLUTION   
 where YEAR_START=to_char(sysdate,'yyyy')-2  
) b;
```
通常用于统计总数，不同总数进行总和，返回一条数据的情况。
其中表对应各学院的信息，但希望你能学习这种SQL语句的格式。
输出如下图所示：
![](https://img-blog.csdn.net/20160423071215745)

字符串拼接输出用法如下，输出"1-8"时间段：
concat(TimeStart,'-',TimeEnd)

最后希望文章对你有所帮组，主要是记录实际遇到的几个问题，如果文章中存在不足之处，还请海涵~数据库SQL语句确实只有当你真正遇到相关问题时，才会觉得相关解决方法有用。
同时后面的文章会讲解如何使用Navicat for MySQL讲解设置主键、外键、递增序列等。
(By:Eastmount 2016-08-23 深夜3点http://blog.csdn.net//eastmount/)



