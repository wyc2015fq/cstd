# ORACLE 多表关联 UPDATE 语句 - z69183787的专栏 - CSDN博客
2015年08月31日 11:35:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1336
为了方便起见,建立了以下简单模型,和构造了部分测试数据:
在某个业务受理子系统BSS中，
SQL 代码
- --客户资料表
- createtable customers
- (
- customer_id number(8) notnull, -- 客户标示
- city_name varchar2(10) notnull, -- 所在城市
- customer_type char(2) notnull, --
 客户类型
- ...
- )
- createuniqueindex PK_customers on customers
 (customer_id)
由于某些原因，客户所在城市这个信息并不什么准确，但是在
客户服务部的CRM子系统中，通过主动服务获取了部分客户20%的所在
城市等准确信息，于是你将该部分信息提取至一张临时表中：
SQL 代码
- createtable tmp_cust_city
- (
- customer_id number(8) notnull,
- citye_name varchar2(10) notnull，
- customer_type char(2) notnull
- )
1) 最简单的形式
SQL 代码
- --经确认customers表中所有customer_id小于1000均为'北京'
- --1000以内的均是公司走向全国之前的本城市的老客户:)
- update customers
- set city_name='北京'
- where customer_id<1000
2) 两表(多表)关联update --
 仅在where字句中的连接
SQL 代码
- --这次提取的数据都是VIP，且包括新增的,所以顺便更新客户类别
- update customers a -- 使用别名
- set customer_type='01'--01
 为vip，00为普通
- where exists (select 1
- from tmp_cust_city b
- where b.customer_id=a.customer_id
- )
3) 两表(多表)关联update --
 被修改值由另一个表运算而来
SQL 代码
- update customers a -- 使用别名
- set city_name=(select b.city_name from tmp_cust_city
 b where b.customer_id=a.customer_id)
- where exists (select 1
- from tmp_cust_city b
- where b.customer_id=a.customer_id
- )
- -- update 超过2个值
- update customers a -- 使用别名
- set (city_name,customer_type)=(select b.city_name,b.customer_type
- from tmp_cust_city b
- where b.customer_id=a.customer_id)
- where exists (select 1
- from tmp_cust_city b
- where b.customer_id=a.customer_id
- )
注意在这个语句中，
=(select b.city_name,b.customer_type from tmp_cust_city b
where b.customer_id=a.customer_id )
与
(select 1 from tmp_cust_city b
where b.customer_id=a.customer_id)
是两个独立的子查询，查看执行计划可知，对b表/索引扫描了2篇；
如果舍弃where条件，则默认对A表进行全表
更新，但由于
SQL 代码
- select b.city_name from tmp_cust_city
 b where b.customer_id=a.customer_id
有可能不能提供"足够多"值，因为tmp_cust_city只是一部分客户的信息，所以报错（如果指定的列--city_name可以为NULL则另当别论）：
SQL 代码
- 01407, 00000, "cannot update (%s) to NULL"
- // *Cause:
- // *Action:
一个替代的方法可以采用：
SQL 代码
- update customers a -- 使用别名
- set city_name=nvl((select b.city_name from tmp_cust_city
 b where b.customer_id=a.customer_id),a.city_name)
或者
SQL 代码
- set city_name=nvl((select b.city_name from tmp_cust_city
 b where b.customer_id=a.customer_id),'未知')
-- 当然这不符合业务逻辑了
4) 上述3)在一些情况下，因为B表的纪录只有A表的20-30%的纪录数，
考虑A表使用INDEX的情况，使用cursor也许会比关联update带来更好的性能：
SQL 代码
- set serveroutput on
- 
- declare
- cursor city_cur is
- select customer_id,city_name
- from tmp_cust_city
- orderby customer_id;
- begin
- for my_cur in city_cur loop
- 
- update customers
- set city_name=my_cur.city_name
- where customer_id=my_cur.customer_id;
- 
- /** 此处也可以单条/分批次提交，避免锁表情况 **/
- -- if mod(city_cur%rowcount,10000)=0 then
- -- dbms_output.put_line('----');
- -- commit;
- -- end if;
- end loop;
- end;
5) 关联update的一个特例以及性能再探讨
在oracle的update语句语法中，除了可以update表之外，也可以是视图，所以有以下1个特例：
SQL 代码
- update (select a.city_name,b.city_name as new_name
- from customers a,
- tmp_cust_city b
- where b.customer_id=a.customer_id
- )
- set city_name=new_name
这样能避免对B表或其索引的2次扫描，但前提是 A(customer_id) b(customer_id)必需是unique index或primary key。否则报错：
SQL 代码
- 01779, 00000, "cannot modify a column which maps to a non key-preserved table"
- // *Cause: An attempt was made toinsertorupdate columns of a joinview which
- // map to a non-key-preserved table.
- // *Action: Modify the underlying base tables
 directly.
6）oracle另一个常见错误
回到3)情况,由于某些原因，tmp_cust_city customer_id 不是唯一index/primary key
SQL 代码
- update customers a -- 使用别名
- set city_name=(select b.city_name from tmp_cust_city
 b where b.customer_id=a.customer_id)
- where exists (select 1
- from tmp_cust_city b
- where b.customer_id=a.customer_id
- )
当对于一个给定的a.customer_id
(select b.city_name from tmp_cust_city b where b.customer_id=a.customer_id)
返回多余1条的情况，则会报如下错误：
SQL 代码
- 01427, 00000, "single-row subquery returns more than one row"
- // *Cause:
- // *Action:
一个比较简单近似于不负责任的做法是
SQL 代码
- update customers a -- 使用别名
- set city_name=(select b.city_name from tmp_cust_city
 b where b.customer_id=a.customer_id and rownum=1)
如何理解 01427 错误，在一个很复杂的多表连接update的语句，经常因考虑不周，出现这个错误，
仍已上述例子来描述，一个比较简便的方法就是将A表代入 值表达式 中,使用group by 和
having 字句查看重复的纪录 
SQL 代码
- (select b.customer_id,b.city_name,count(*)
- from tmp_cust_city b,customers a
- where b.customer_id=a.customer_id
- groupby b.customer_id,b.city_name
- havingcount(*)>=2
- )
