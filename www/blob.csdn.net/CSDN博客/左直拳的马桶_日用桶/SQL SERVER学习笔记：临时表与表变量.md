# SQL SERVER学习笔记：临时表与表变量 - 左直拳的马桶_日用桶 - CSDN博客
2014年06月24日 22:25:04[左直拳](https://me.csdn.net/leftfist)阅读数：1770
本文主要摘自徐海蔚的《Microsoft SQL SERVER企业级平台管理实践》
表变量可以作为存储过程的返回参数，而临时表不行。（存疑？表值参数只在SQL SERVER2008才开始支持，并且限制很多，要首先定义表类型）
```
use [test]
go
create type [user] as table(id int,name varchar(50));
go
create procedure test_prd
@u [user] readonly
as
begin
	select * from @u;
end
go
declare @u [user];
insert into @u(id,name) values(1,'zhang3');
exec test_prd @u;
go
drop procedure test_prd;
go
drop type [user];
go
```
临时表跟普通的表一样，SQL SERVER会维护其统计信息，以得到合适的执行计划；也可以在上面建立索引。而表变量既不能建索引，也不会有统计信息；SQL SERVER在做执行计划的时候，总是认为表变量里的数据量只有很少的几行。
因此，表变量的好处是维护成本很低，大量并发使用时系统开销要比临时表低。但因为缺乏统计信息，只适合存放少量数据
而临时表功能和普通表接近，能够为大数据量做优化；缺点是维护成本比较高，大量并发使用时，系统负荷比较大。
```
use [test]
go
select 1 as id,'li4' as name into #t;
go
declare @t table(id int,name varchar(50));
insert into @t(id,name) values(2,'wang8');
go
create procedure test_prd
as
begin
	exec('select * from #t;select * from @t;');
end
go
exec test_prd;
go
drop procedure test_prd;
go
drop table #t;
go
```
