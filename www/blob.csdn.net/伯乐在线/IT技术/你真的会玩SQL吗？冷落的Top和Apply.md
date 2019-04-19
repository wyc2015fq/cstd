# 你真的会玩SQL吗？冷落的Top和Apply - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [欢醉](http://www.jobbole.com/members/huanzui2336) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
本章预先想写一些Top和Apply基本的用法，但好像没什么意义，所以删掉了一些无用的东西，只留下几个示例，以保证系列的完整性。
Top和Apply解决的常见问题，如返回每个雇员的3个最新订单，订单的时间越新优先级就越高，但还需要引入一个决胜属性，以确定时间桢的订单的优先级，如可用id作为决胜属性。这里提供的解决方案比其它方案要简单得多，且执行速度更快。
返回每个雇员的3个最新订单：


MySQL
```
SELECT  empid ,
        orderid ,
        custid ,
        orderdate ,
        requireddate
FROM    sales.orders AS o1
WHERE   orderid IN ( SELECT TOP 3
                            orderid
                     FROM   sales.orders AS o2
                     WHERE  o2.empid = o1.empid
                     ORDER BY orderdate DESC ,
                            orderid DESC )
```
运用APPLY解决：


MySQL
```
SELECT  e.empid ,
        a.orderid ,
        a.custid ,
        a.orderdate ,
        a.requireddate
FROM    hr.employees AS e
        CROSS APPLY ( SELECT TOP 3
                                orderid ,
                                custid ,
                                orderdate ,
                                requireddate
                      FROM      sales.orders AS o
                      WHERE     o.empid = e.empid
                      ORDER BY  orderdate DESC ,
                                orderid DESC
                    ) AS a
```
先扫描employees 获得empid，对每个empid值对orders表查询返回 该雇员的3个最新订单。这里可以返回多个属性。
还有一种解决方案在特定情况下竟然比使用APPLY运算符的方法还要快，使用ROW_NUMBER函数。先为每个订单计算行号，按empid进行分区，并按orderdate desc, orderid desc 顺序排序。然后在外部查询中，只筛选行号小于或等于3的行。
如下：


MySQL
```
SELECT  orderid ,
        custid ,
        orderdate ,
        requireddate
FROM    ( SELECT    orderid ,
                    custid ,
                    orderdate ,
                    requireddate ,
                    ROW_NUMBER() OVER ( PARTITION BY empid ORDER BY orderdate DESC , orderid DESC ) AS rownum
          FROM      sales.orders
        ) AS d
WHERE   rownum  3
```
**练习：**
从学生表中选取对应班级的前num名学生成绩


MySQL
```
--显示结果
/*
bj         xh   name       cj
---------- ---- ---------- -----------
一班         A006 A6         100
一班         A005 A5         99
一班         A001 A1         89
一班         A002 A2         89
二班         B001 B7         100
二班         B001 B6         99
二班         B001 B9         97
二班         B001 B8         90
二班         B001 B5         88
*/
```


MySQL
```
-- 创建测试表
declare @student table(        ---学生表
bj varchar(10),          -- 班级
xh char(4),                 -- 学号
name varchar(10),         -- 姓名   
cj int)                     -- 成绩
declare @tj table(            ---统计表
bj varchar(10),          -- 班级
num int)                 -- 人数  ：从学生表中选取对应班级的前num名学生成绩 
set nocount on
-- 添加测试数据
insert @student select '一班' ,'A001','A1',89
insert @student select '一班' ,'A002','A2',89
insert @student select '一班' ,'A003','A3',59
insert @student select '一班' ,'A004','A4',80
insert @student select '一班' ,'A005','A5',99
insert @student select '一班' ,'A006','A6',100
insert @student select '一班' ,'A007','A7',82
insert @student select '二班' ,'B001','B1',19
insert @student select '二班' ,'B001','B2',81
insert @student select '二班' ,'B001','B3',69
insert @student select '二班' ,'B001','B4',86
insert @student select '二班' ,'B001','B5',88
insert @student select '二班' ,'B001','B6',99
insert @student select '二班' ,'B001','B7',100
insert @student select '二班' ,'B001','B8',90
insert @student select '二班' ,'B001','B9',97 
insert @tj select '一班',3
insert @tj select '二班',5
```
参考SQL：


MySQL
```
-- 2005.T-SQL 
select t.bj,s.xh,s.name,s.cj
from @tj t
cross apply (
    SELECT TOP(t.num) 
            with ties       -- 加 with ties，一班将选出4个人（2个人并列第三名） 
        xh,name,cj 
    from @student
    where t.bj=bj            -- 加where 功能类似于 inner join ；不加类似于 cross join
    order by cj desc 
)s
order by case when t.bj='一班' then 1 else 2 end asc,s.cj desc,s.xh asc    ---排序
```
