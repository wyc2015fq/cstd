# DB2表数据导出、导入及常用sql使用总结 - fanyun的博客 - CSDN博客
2016年12月25日 21:59:35[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：29726
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
**一．DB2数据的导出：**
export to [path(例:D:"TABLE1.ixf)]of ixf select [字段(例: * or col1,col2,col3)] from TABLE1;
export to [path(例:D:"TABLE1.del)]of del select [字段(例: * or col1,col2,col3)] from TABLE1;
在DB2中对表数据的导出，可以用export命令，导出数据为一个文本文件，例如：
 export to d:\table1.txt of del select id, name, age, address, note fromtesttable order by id;
可以用import命令从文本文件导入数据到表里，如：
**二．DB2数据的导入：**
import from[path(例:D:"TABLE1.ixf)] of ixf insert into TABLE1;
load from [path(例:D:"TABLE1.ixf)]of ixf insert into TABLE1;
load from [path(例:D:"TABLE1.ixf)]of ixf replace into TABLE1; // 装入数据前，先删除已存在记录
load from [path(例:D:"TABLE1.ixf)]of ixf restart into TABLE1; // 当装入失败时，重新执行，并记录导出结果和错误信息
import from[path(例:D:"TABLE1.ixf)] of ixf savecount 1000 messages [path(例:D:"msg.txt)]insert into TABLE1;// 其中，savecount表示完成每1000条操作，记录一次.
存在自增长字段的数据导入：
load from [path(例:D:"TABLE1.ixf)]of ixf modified by identityignore insert into TABLE1;// 加入modified byidentityignore.
解除装入数据时，发生的检查挂起:
SET INTEGRITYFOR TABLE1 CHECK IMMEDIATE UNCHECKED;
命令只对数据通过约束检查的表有效，如果执行还不能解除，有必要检查数据的完整性，是否不符合约束条件，并试图重新整理数据，再执行装入操作.
另外，对load和import，字面上的区别是:装入和导入，但仍未理解两者之间的区别.
只是性能上load显然优于import.（load 需要更多的权限）
**三．DB2常用操作命令实例：**
1、查找员工的编号、姓名、部门和出生日期，如果出生日期为空值，显示日期不详,并按部门排序输出,日期格式为yyyy-mm-dd。 
复制代码 代码如下:
```
select emp_no,emp_name,dept,isnull(convert(char(10),birthday,120),'日期不详')birthday 
from employee 
order by dept
```
2、查找与喻自强在同一个单位的员工姓名、性别、部门和职称 
复制代码 代码如下:
```
select emp_no,emp_name,dept,title 
from employee 
where emp_name<>'小明' and dept in 
(select dept from employee 
where emp_name='小明')
```
3、按部门进行汇总，统计每个部门的总工资 
复制代码 代码如下:
```
select dept,sum(salary) 
from employee 
group by dept
```
4、查找商品名称为14寸显示器商品的销售情况，显示该商品的编号、销售数量、单价和金额 
复制代码 代码如下:
```
selecta.prod_id,qty,unit_price,unit_price*qty totprice 
from sale_item a,product b 
where a.prod_id=b.prod_id and prod_name='14寸显示器'
```
5、在销售明细表中按产品编号进行汇总，统计每种产品的销售数量和金额 
复制代码 代码如下:
```
select prod_id,sum(qty) totqty,sum(qty*unit_price)totprice 
from sale_item 
group by prod_id
```
6、使用convert函数按客户编号统计每个客户1996年的订单总金额 
复制代码 代码如下:
```
select cust_id,sum(tot_amt) totprice 
from sales 
where convert(char(4),order_date,120)='1996' 
group by cust_id
```
7、查找有销售记录的客户编号、名称和订单总额 
复制代码 代码如下:
```
select a.cust_id,cust_name,sum(tot_amt)totprice 
from customer a,sales b 
where a.cust_id=b.cust_id 
group by a.cust_id,cust_name
```
8、查找在1997年中有销售记录的客户编号、名称和订单总额 
复制代码 代码如下:
```
select a.cust_id,cust_name,sum(tot_amt)totprice 
from customer a,sales b 
where a.cust_id=b.cust_id and convert(char(4),order_date,120)='1997' 
group by a.cust_id,cust_name
```
9、查找一次销售最大的销售记录 
复制代码 代码如下:
```
select order_no,cust_id,sale_id,tot_amt 
from sales 
where tot_amt= 
(select max(tot_amt) 
from sales)
```
10、查找至少有3次销售的业务员名单和销售日期 
复制代码 代码如下:
```
select emp_name,order_date 
from employee a,sales b 
where emp_no=sale_id and a.emp_no in 
(select sale_id 
from sales 
group by sale_id 
having count(*)>=3) 
order by emp_name
```
11、用存在量词查找没有订货记录的客户名称 
复制代码 代码如下:
```
select cust_name 
from customer a 
where not exists 
(select * 
from sales b 
where a.cust_id=b.cust_id)
```
12、使用左外连接查找每个客户的客户编号、名称、订货日期、订单金额订货日期不要显示时间，日期格式为yyyy-mm-dd按客户编号排序，同一客户再按订单降序排序输出 
复制代码 代码如下:
```
selecta.cust_id,cust_name,convert(char(10),order_date,120),tot_amt 
from customer a left outer join sales b on a.cust_id=b.cust_id 
order by a.cust_id,tot_amt desc
```
13、查找16M DRAM的销售情况，要求显示相应的销售员的姓名、性别，销售日期、销售数量和金额，其中性别用男、女表示
复制代码 代码如下:
```
select emp_name 姓名, 性别= casea.sex when 'm' then '男' 
when 'f' then '女' 
else '未' 
end, 
销售日期= isnull(convert(char(10),c.order_date,120),'日期不详'), 
qty 数量, qty*unit_price as 金额 
from employee a, sales b, sale_item c,product d 
where d.prod_name='16M DRAM' and d.prod_id=c.prod_id and 
a.emp_no=b.sale_id and b.order_no=c.order_no
```
14、查找每个人的销售记录，要求显示销售员的编号、姓名、性别、产品名称、数量、单价、金额和销售日期 
复制代码 代码如下:
```
select emp_no 编号,emp_name 姓名, 性别= casea.sex when 'm' then '男' 
when 'f' then '女' 
else '未' 
end, 
prod_name 产品名称,销售日期= isnull(convert(char(10),c.order_date,120),'日期不详'), 
qty 数量, qty*unit_price as 金额 
from employee a left outer join sales b on a.emp_no=b.sale_id , sale_itemc,product d 
where d.prod_id=c.prod_id and b.order_no=c.order_no
```
15、查找销售金额最大的客户名称和总货款 
复制代码 代码如下:
```
select cust_name,d.cust_sum 
from customer a, 
(select cust_id,cust_sum 
from (select cust_id, sum(tot_amt) as cust_sum 
from sales 
group by cust_id ) b 
where b.cust_sum = 
( select max(cust_sum) 
from (select cust_id, sum(tot_amt) as cust_sum 
from sales 
group by cust_id ) c ) 
) d 
where a.cust_id=d.cust_id
```
16、查找销售总额少于1000元的销售员编号、姓名和销售额 
复制代码 代码如下:
```
select emp_no,emp_name,d.sale_sum 
from employee a, 
(select sale_id,sale_sum 
from (select sale_id, sum(tot_amt) as sale_sum 
from sales 
group by sale_id ) b 
where b.sale_sum <1000 
) d 
where a.emp_no=d.sale_id
```
17、查找至少销售了3种商品的客户编号、客户名称、商品编号、商品名称、数量和金额 
复制代码 代码如下:
```
selecta.cust_id,cust_name,b.prod_id,prod_name,d.qty,d.qty*d.unit_price 
from customer a, product b, sales c, sale_item d 
where a.cust_id=c.cust_id and d.prod_id=b.prod_id and 
c.order_no=d.order_no and a.cust_id in ( 
select cust_id 
from (select cust_id,count(distinct prod_id) prodid 
from (select cust_id,prod_id 
from sales e,sale_item f 
where e.order_no=f.order_no) g 
group by cust_id 
having count(distinct prod_id)>=3) h )
```
18、查找至少与世界技术开发公司销售相同的客户编号、名称和商品编号、商品名称、数量和金额 
复制代码 代码如下:
```
selecta.cust_id,cust_name,d.prod_id,prod_name,qty,qty*unit_price 
from customer a, product b, sales c, sale_item d 
where a.cust_id=c.cust_id and d.prod_id=b.prod_id and 
c.order_no=d.order_no and not exists 
(select f.* 
from customer x ,sales e, sale_item f 
where cust_name='世界技术开发公司' and x.cust_id=e.cust_id and 
e.order_no=f.order_no and not exists 
( select g.* 
from sale_item g, sales h 
where g.prod_id = f.prod_id and g.order_no=h.order_no and 
h.cust_id=a.cust_id) 
)
```
19、查找表中所有姓刘的职工的工号，部门，薪水 
复制代码 代码如下:
```
select emp_no,emp_name,dept,salary 
from employee 
where emp_name like '刘%'
```
20、查找所有定单金额高于2000的所有客户编号 
复制代码 代码如下:
```
select cust_id 
from sales 
where tot_amt>2000
```
21、统计表中员工的薪水在4000-6000之间的人数 
复制代码 代码如下:
```
select count(*)as 人数 
from employee 
where salary between 4000 and 6000
```
22、查询表中的同一部门的职工的平均工资，但只查询"住址"是"上海市"的员工 
复制代码 代码如下:
```
select avg(salary) avg_sal,dept 
from employee 
where addr like '上海市%' 
group by dept
```
23、将表中住址为"上海市"的员工住址改为"北京市" 
复制代码 代码如下:
```
update employee 
set addr like '北京市' 
where addr like '上海市'
```
24、查找业务部或会计部的女员工的基本信息。 
复制代码 代码如下:
```
select emp_no,emp_name,dept 
from employee 
where sex='F'and dept in ('业务','会计')
```
25、显示每种产品的销售金额总和，并依销售金额由大到小输出。 
复制代码 代码如下:
```
select prod_id ,sum(qty*unit_price) 
from sale_item 
group by prod_id 
order by sum(qty*unit_price) desc
```
26、选取编号界于'C0001'和'C0004'的客户编号、客户名称、客户地址。 
复制代码 代码如下:
```
select CUST_ID,cust_name,addr 
from customer 
where cust_id between 'C0001' AND 'C0004'
```
27、计算出一共销售了几种产品。 
复制代码 代码如下:
```
select count(distinct prod_id) as '共销售产品数' 
from sale_item
```
28、将业务部员工的薪水上调3%。 
复制代码 代码如下:
```
update employee 
set salary=salary*1.03 
where dept='业务'
```
29、由employee表中查找出薪水最低的员工信息。 
复制代码 代码如下:
```
select * 
from employee 
where salary= 
(select min(salary ) 
from employee )
```
30、使用join查询客户姓名为"客户丙"所购货物的"客户名称","定单金额","定货日期","电话号码" 
复制代码 代码如下:
```
selecta.cust_id,b.tot_amt,b.order_date,a.tel_no 
from customer a join sales b 
on a.cust_id=b.cust_id and cust_name like '客户丙'
```
31、由sales表中查找出订单金额大于"E0013业务员在1996/10/15这天所接每一张订单的金额"的所有订单。 
复制代码 代码如下:
```
select * 
from sales 
where tot_amt>all 
(select tot_amt 
from sales 
where sale_id='E0013'and order_date='1996/10/15') 
order by tot_amt
```
32、计算'P0001'产品的平均销售单价 
复制代码 代码如下:
```
select avg(unit_price) 
from sale_item 
where prod_id='P0001'
```
33、找出公司女员工所接的定单 
复制代码 代码如下:
```
select sale_id,tot_amt 
from sales 
where sale_id in 
(select sale_id from employee 
where sex='F')
```
34、找出同一天进入公司服务的员工 
复制代码 代码如下:
```
select a.emp_no,a.emp_name,a.date_hired 
from employee a 
join employee b 
on (a.emp_no!=b.emp_no and a.date_hired=b.date_hired) 
order by a.date_hired
```
35、找出目前业绩超过232000元的员工编号和姓名。 
复制代码 代码如下:
```
select emp_no,emp_name 
from employee 
where emp_no in 
(select sale_id 
from sales 
group by sale_id 
having sum(tot_amt)<232000)
```
