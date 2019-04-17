# mysql中的视图 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年04月05日 21:45:59[QuJack](https://me.csdn.net/A784586)阅读数：221标签：[mysql中的视图																[数据库视图																[视图																[MySQL](https://so.csdn.net/so/search/s.do?q=MySQL&t=blog)
个人分类：[数据库](https://blog.csdn.net/A784586/article/category/6777843)





**mysql中的视图**

**1.索引的优势：**

虚拟存在的表，行和列的数据来自定义视图的查询中使用的表；

- 简单：不需要关心表结构，关联条件，筛选条件；已经是过滤好的结果集；
- 安全：对表的权限管理并不能限制到某个行某个列，通过视图可以；
- 数据独立：视图结构定了，可以屏蔽表结构的变化，增加列对视图没有影响，修改列名，可以修改视图解决，不影响访问者；


2.创建或者修改视图[create or replace 或者 alter]

create [or replace] [ algorithm={undefined | merge | temptable}]

view view_name [(column_list)]

as select_statement

[with [cascaded | local] check option]

-------------------------------------------------------

create or replace view view_nameas

select_statement;

-----------------------------------------------------

from关键字后面不能包含子查询；

视图是否可以更新和视图中查询定义有关：

1.包含聚合函数sum,HAVING等；

2.常量视图；

3.select中包含子查询；

4.jion

5.from 一个不能更新的视图；

6.where 子句的子查询引用了from子句中的表；

例子：

--包含聚合函数：

create or replaceview
payment_sumas

selectstaff_id ,
sum(account) from paymentgroup
 by staff_id;

--常量视图：

create or replaceview
pias

select3.1415926
as pi;

--select中包含子查询

create or replaceview
city_viewas

select(select city from city where city_id=1 );

-----------------------------------------------------------------

[with [cascaded | local] check option] 决定是否允许更新视图使得记录不在满足视图的条件；

local :满足本视图就可以更新；

cascaded:默认的，必须满足针对该视图的所有视图的条件；

例子：

create or replaceview
**payment_view**as

selectpayment_id ,amout from
**payment**

where amount < 10with check option; //默认**cascaded**




create or replaceview
**payment_view1**as

selectpayment_id ,amout from
**payment_view**

where amount > 5with
**local**check option; //**local**




create or replaceview
**payment_view2**as

selectpayment_id ,amout from
**payment_view**

where amount > 5with
**cascaded**check option; //**cascaded**

-------------------------------------------------------

select * from payment_view1 limit l;




update**payment_view1**set amount=10

where payment_id =3; //正确




update**payment_view2**set amount=10

where payment_id =3; //报错，payment_view2 是cascaded，必须满足payment_view才可以。等于10，不小于10.

删除视图：

drop view [if exists] view_name;

查看视图：

show tables; //显示表和显示有哪些视图

show table status like 'staff_list'; //查看视图信息

show create view staff_list; //查看某个视图的定义](https://so.csdn.net/so/search/s.do?q=视图&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库视图&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql中的视图&t=blog)




