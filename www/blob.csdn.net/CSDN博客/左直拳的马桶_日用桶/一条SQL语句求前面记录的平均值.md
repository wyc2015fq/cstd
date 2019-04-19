# 一条SQL语句求前面记录的平均值 - 左直拳的马桶_日用桶 - CSDN博客
2014年05月13日 21:32:51[左直拳](https://me.csdn.net/leftfist)阅读数：4745
有算法要求如下：
```
For (i=1，i<=10，i++)
{
	ta[i] = (t[1] + t[2] + ... + t[i]) / i;
}
```
用一条SQL语句实现它：
分别用表变量 @ta 和 @t 来对应 ta 和 t。
```
declare @t table(id int,d decimal(18,4));
declare @ta table(id int,da decimal(18,4));
insert into @t(id,d) values(1,1);
insert into @t(id,d) values(2,2);
insert into @t(id,d) values(3,3);
insert into @t(id,d) values(4,4);
insert into @t(id,d) values(5,5);
insert into @t(id,d) values(6,6);
insert into @t(id,d) values(7,7);
insert into @t(id,d) values(8,8);
insert into @t(id,d) values(9,9);
insert into @t(id,d) values(10,10);
insert into @ta(id,da) values(1,0);
insert into @ta(id,da) values(2,0);
insert into @ta(id,da) values(3,0);
insert into @ta(id,da) values(4,0);
insert into @ta(id,da) values(5,0);
insert into @ta(id,da) values(6,0);
insert into @ta(id,da) values(7,0);
insert into @ta(id,da) values(8,0);
insert into @ta(id,da) values(9,0);
insert into @ta(id,da) values(10,0);
```
则有SQL语句：
```
with w(id,av) as(
select a.id,avg(b.d)
	from 
	@ta a,@t b
	where a.id>=b.id
	group by a.id
)
update @ta set da=w.av
from @ta a inner join w on a.id=w.id;
```
=================================================
但是这种方法有很严重的性能问题。后面记录求前面所有记录的平均值，每条记录都要将前面的算一遍，而完全没有办法重用前面的计算结果，当列很多，记录也很多的情况下，耗时非常的长。
这个时候，老老实实用游标，用循环来算，搞一些变量来保存中间值，性能反而很好。
真让人感慨，都说SQL的强项不在循环，应该尽量用SQL语句来代替循环，但在这里，却恰好相反。兵无常势，水无常形，SQL的性能问题，也不是那么死板的。
