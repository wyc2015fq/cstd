# DB2编程序技巧 - fanyun的博客 - CSDN博客
2016年11月02日 14:30:35[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：386标签：[数据库](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)
个人分类：[DB2](https://blog.csdn.net/fanyun_01/article/category/6499422)
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)
                正在看的db2教程是:DB2编程序技巧(1)。 
1 DB2编程  
1.1 建存储过程时Create 后一定不要用TAB键  
create procedure  
的create后只能用空格,而不可用tab健，否则编译会通不过。  
切记，切记。  
1.2 使用临时表  
要注意，临时表只能建在user tempory tables space 上，如果database只有system tempory table space是不能建临时表的。 
另外，DB2的临时表和sybase及oracle的临时表不太一样，DB2的临时表是在一个session内有效的。所以，如果程序有多线程，最好不要用临时表，很难控制。 
建临时表时最好加上  with  replace选项，这样就可以不显示的drop 临时表，建临时表时如果不加该选项而该临时表在该session内已创建且没有drop,这时会发生错误。 
1.3 从数据表中取指定前几条记录  
select  *  from tb_market_code fetch first 1 rows only  
但下面这种方式不允许  
select market_code into v_market_code   
from tb_market_code fetch first 1 rows only;       
选第一条记录的字段到一个变量以以下方式代替  
declare v_market_code char(1);  
declare cursor1 cursor for select market_code from tb_market_code   
fetch first 1 rows only for update;  
open cursor1;  
fetch cursor1 into v_market_code;  
close cursor1;  
1.4 游标的使用  
注意commit和rollback  
使用游标时要特别注意如果没有加with hold 选项,在Commit和Rollback时,该游标将被关闭。Commit 和Rollback有很多东西要注意。特别小心 
游标的两种定义方式  
一种为  
declare continue handler for not found  
begin  
set v_notfound = 1;  
end;  
declare cursor1 cursor with hold for select market_code from tb_market_code  for update; 
open cursor1;  
set v_notfound=0;  
fetch cursor1 into v_market_code;  
while v_notfound=0 Do  
--work  
set v_notfound=0;  
fetch cursor1 into v_market_code;  
end while;  
close cursor1;  
这种方式使用起来比较复杂，但也比较灵活。特别是可以使用with hold 选项。如果循环内有commit或rollback 而要保持该cursor不被关闭，只能使用这种方式。
另一种为  
pcursor1: for loopcs1 as  cousor1  cursor  as  
select  market_code  as market_code  
from tb_market_code  
for update  
do  
end for;  
这种方式的优点是比较简单，不用（也不允许）使用open,fetch,close。  
但不能使用with  hold 选项。如果在游标循环内要使用commit,rollback则不能使用这种方式。如果没有commit或rollback的要求，推荐使用这种方式(看来For这种方式有问题)。 
修改游标的当前记录的方法  
update tb_market_code set market_code=’0’ where current of cursor1;  
不过要注意将cursor1定义为可修改的游标  
declare cursor1 cursor for select market_code from tb_market_code   
for update;  
for update 不能和GROUP BY、 DISTINCT、 ORDER BY、 FOR READ ONLY及UNION, EXCEPT, or INTERSECT但 UNION ALL除外）一起使用。 
1.5 类似decode的转码操作  
oracle中有一个函数  select decode(a1,’1’,’n1’,’2’,’n2’,’n3’) aa1 from  
db2没有该函数，但可以用变通的方法  
select case a1   
when ’1’ then ’n1’   
when ’2’ then ’n2’   
else ’n3’  
end as aa1 from  
1.6 类似charindex查找字符在字串中的位置  
Locate(‘y','dfdasfay')  
查找'y' 在'dfdasfay'中的位置。  
1.7 类似datedif计算两个日期的相差天数  
days(date(‘2001-06-05')) – days             
