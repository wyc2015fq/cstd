# select 变量=。。。 from 表 - 左直拳的马桶_日用桶 - CSDN博客
2013年06月07日 00:25:11[左直拳](https://me.csdn.net/leftfist)阅读数：2315
个人分类：[sql server](https://blog.csdn.net/leftfist/article/category/136470)
有表如下：
```
use [test]
go
create table A(id int,pid int);
insert into A(id,pid) values(1,1);
insert into A(id,pid) values(1,2);
insert into A(id,pid) values(1,3);
insert into A(id,pid) values(2,1);
insert into A(id,pid) values(2,2);
insert into A(id,pid) values(3,1);
```
如果
```
declare @str varchar(1000) = '';
select @str = @str + ',' + CAST(pid as varchar(50)) from [A] where [id]=1;
select right(@str,LEN(@str) - 1);
```
则有结果：
(无列名)
1,2,3
如果
`select CAST(pid as varchar(50)) + ',' from [A] where [id]=1;`
则有结果：
(无列名)
1,
2,
3,
觉得有点意思，select 变量=。。。 from 表    跟    select 。。。 from 表 差别这么大。前者好像会自动处理一个结果集。
