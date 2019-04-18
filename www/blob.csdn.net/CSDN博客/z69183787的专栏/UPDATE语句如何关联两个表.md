# UPDATE语句如何关联两个表 - z69183787的专栏 - CSDN博客
2013年07月09日 10:00:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：15364
请问UPDATE语句如何关联两个表? 
MS SQLSERVER 7.0
如有A,B两个表,
A :字段cust_id,cust_name
B :字段cust_id,cust_name
如何用一个UPDATE语句关联字段cust_id把A中cust_name改为相对应的B表中的cust_name?
记得在ORACLE里是可以的，MSSQL里就不知道怎么写了，这种情况只能用游标来处理吗？
update a set a.cust_name=b.cust_name from a
join b
on a.cust_id=b.cust_id
ORACLE下应该是：
update A表 a 
set cust_name=(select b.cust_name from B表 b where b.cust_id=a.cust_id)
where exists (select 1 from B表 b where b.cust_id=a.cust_id);
'cust_name='处应该还可以支持多个字段，如(cust_name,cust_sex)=(select b.cust_name,b.cust_sex from .
# [多表关联的update语句](http://www.cnblogs.com/shiningrise/archive/2007/05/25/759555.html)
MSSql的多表关联的update语句
例如A表 存在一下字段:
AID A1 A2 A3 A4
B表中存在字段：
BID B1 B2 B3 B4
如果实现用B表的所有字段更新A表中相应的字段，在MS SQL Server里面可以写成：
update A
set A1=B.B1,A2=B.B2,A3=B.B3,A4=B.B4
from A,B
where A.AID=B.BID
ORACLE UPDATE 多表关联的update语句
  -- update 超过2个值
   update customers a   -- 使用别名
   set    (city_name,customer_type)=(select b.city_name,b.customer_type 
                                     from   tmp_cust_city b 
                                     where  b.customer_id=a.customer_id)
   where  exists (select 1 
                  from   tmp_cust_city b
                  where  b.customer_id=a.customer_id
                 )
