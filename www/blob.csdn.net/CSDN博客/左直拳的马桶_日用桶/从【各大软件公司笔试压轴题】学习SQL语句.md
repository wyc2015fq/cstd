# 从【各大软件公司笔试压轴题】学习SQL语句 - 左直拳的马桶_日用桶 - CSDN博客
2009年12月29日 11:32:00[左直拳](https://me.csdn.net/leftfist)阅读数：31965
从博客园中看到一篇文章，介绍大软件公司面试时常常会出的两道SQL题（见附录）。
我觉得受益很多，在此之前，我一直觉得，SQL2008似乎提供了这方面的支持，但更低的版本，包括2005，非游标做不出来（水平够菜）。总结心得如下：
1、    强大的group by
1select stdname,
2isnull(sum(case stdsubject when '化学' then Result end),0) [化学],
3isnull(sum(case stdsubject when '数学' then Result end),0) [数学],
4isnull(sum(case stdsubject when '物理' then Result end),0) [物理],
5isnull(sum(case stdsubject when '语文' then Result end),0) [语文] 
6from #student 
7group by stdname
在这里，group by与sum + case结合，可以将表1中的记录（行）变成表2的字段（列）。Sum里面如果没有case，那么出来的值，只能是全部科目的总和，用了case以后，就是某科的成绩；然后这里用了好几个sum，每个科目一个sum，于是表1中本来某人某科占一条记录的“行”就变成了表2里某人一条记录，每科做一个字段了。
这种心思巧妙和对语法的熟练运用让人击节赞叹。
2、    利用select from (select from)的模式生成SQL语句
1declare @sql varchar(4000) 
2set @sql ='select stdname'
3select @sql = @sql +',isnull(sum(case stdsubject when '''+stdsubject+''' then Result end),0) ['+stdsubject+']'
4from (select distinct stdsubject from #student) as a 
5select @sql = @sql+' from #student group by stdname'
6print @sql
7exec(@sql)
为了自动写上所有的科目，这里先将科目信息提炼出来：
4from (select distinct stdsubject from #student) as a
利用之拼接生成SQL语句。当然现实中，如果#student表很大，这种做法并不妥，应该都有一个专门的科目类别表的。
3、    在临时库中提炼出字段名。临时表是真实存在的表，保存在[tempdb]中，可以利用object_id('tempdb.dbo.表名')的方式获得字段信息。
============================================
附录：
[http://www.cnblogs.com/zhanglei644213943/archive/2009/12/27/1633356.html](http://www.cnblogs.com/zhanglei644213943/archive/2009/12/27/1633356.html)
      纵览各大社区、论坛，各大ORM框架火得不行了，如NHibernate、LINQ to SQL、ADO.NET Entityframework等，还有最近市场上出版的一本叫《领域驱动设计与模式实战》，里面也凸显了不少NHibernate在领域驱动设计中的作用与地位，也算是第一本与NHibernate相关的书籍吧！不过就NHibernate而言还是没有官方文档介绍得详细呵呵，园子里Kiler已经把他翻译成中文版的了，收益一大片仅仅是CET-4的人。不管你是用NHibernate也好，还是用LINQ toSQL也好，用profiler一跟踪，执行的都是SQL语句，所以所SQL是根。特别是对于那些以数据为中心的应用系统，在数据库中实现复杂的存储过程，复杂的报表查询，还是直接SQL来得痛快。当然对于那些在基于.NET的中间层应用中，它们实现面向对象的业务模型和商业逻辑的应用，NHibernate是最有用的。不管怎样，NHibernate一定可以帮助你消除或者包装那些针对特定厂商的SQL代码，并且帮你把结果集从表格式的表示形式转换到一系列的对象去（官方文档）。 
      有点跑题了，不再啰嗦----直接晾出压轴题。
压轴题第一问
1.把表一转换为表二
表一：                                                                     
![](http://images.cnblogs.com/cnblogs_com/zhanglei644213943/1.png)
表二：
![](http://images.cnblogs.com/cnblogs_com/zhanglei644213943/%E4%BD%9B%E6%8C%A1%E6%9D%80%E4%BD%9B.png)
数据库代码如下：
```
代码
 1 DROP table #student
 2 CREATE TABLE #student (stdname nvarchar(10),stdsubject nvarchar(10),result int)
 3 INSERT INTO #student VALUES ('张三','语文',80)
 4 INSERT INTO #student values ('张三','数学',90)
 5 INSERT INTO #student VALUES ('张三','物理',85)
 6 INSERT INTO #student VALUES ('李四','语文',85)
 7 INSERT INTO #student values ('李四','数学',92)
 8 INSERT INTO #student VALUES ('李四','物理',82)
 9 INSERT INTO #student VALUES ('李四','化学',82)
10 INSERT INTO #student VALUES ('李四','化学',82)
11 SELECT * FROM #student
```
 可能很多老手们，一看到这题目就有了答案。当然，贴出答案来不是我的目的，我要带着SQL新手们重构到答案。用MVP李建忠老师最爱说的话就是------我不建议一上来就套用模式，而应该从重构到模式。
首先大家会想到分两组
```
1 select stdname，····，from #student group by stdname
```
然后······中间该写什么呢？
```
代码
1 case stdsubject when '化学' then Result end
2 case stdsubject when '语文' then Result end
3 case stdsubject when '···' then Result end
4 case stdsubject when '···' then Result end
5 case stdsubject when '···' then Result end
```
表二里面得0是哪里来的呢？
```
代码
1 isnull(sum（case stdsubject when '化学' then Result end）,0)
2 isnull(sum(case stdsubject when '语文' then Result end),0)
3 isnull(sum(case stdsubject when '···' then Result end),0)
4 isnull(sum(case stdsubject when '···' then Result end),0)
5 isnull(sum(case stdsubject when '···' then Result end),0)
```
所以得出：
```
代码
1 select stdname,
2 isnull(sum(case stdsubject when '化学' then Result end),0) [化学],
3 isnull(sum(case stdsubject when '数学' then Result end),0) [数学],
4 isnull(sum(case stdsubject when '物理' then Result end),0) [物理],
5 isnull(sum(case stdsubject when '语文' then Result end),0) [语文] 
6 from #student 
7 group by stdname
```
然后得出答案：
```
代码
1 declare @sql varchar(4000) 
2 set @sql = 'select stdname' 
3 select @sql = @sql + ',isnull(sum(case stdsubject when '''+stdsubject+''' then Result end),0) ['+stdsubject+']' 
4 from (select distinct stdsubject from #student) as a 
5 select @sql = @sql+' from #student group by stdname' 
6 print @sql
7 exec(@sql)
8
```
压轴题第二问：把表二转化为表一
表一：
![](http://images.cnblogs.com/cnblogs_com/zhanglei644213943/1.png)
表二：
![](http://images.cnblogs.com/cnblogs_com/zhanglei644213943/%E4%BD%9B%E6%8C%A1%E6%9D%80%E4%BD%9B.png)
数据库代码如下：
```
代码
1 DROP table #student2
2 CREATE TABLE #student2 (stdname nvarchar(10),化学 int,数学 int,物理 int ,语文 int )
3 INSERT INTO #student2 VALUES ('李四',164,92,82,85)
4 INSERT INTO #student2 VALUES ('张三',0,90,85,80)
5 SELECT * FROM #student2
```
看到这题，直接想到：
```
代码
 1 SELECT'李四'as stdname,stdname='化学', 化学 as result from #student2 where stdname='李四'
 2 union all
 3 SELECT'李四'as stdname,stdname='数学', 数学 as result from #student2 where stdname='李四'
 4 union all
 5 SELECT'李四'as stdname,stdname='物理', 物理 as result from #student2 where stdname='李四'
 6 union all
 7 SELECT'李四'as stdname,stdname='语文', 语文 as result from #student2 where stdname='李四' 
 8 union all 
 9 SELECT'张三'as stdname,stdname='化学', 化学 as result from #student2 where stdname='张三'
10 union all
11 SELECT'张三'as stdname,stdname='数学', 数学 as result from #student2 where stdname='张三'
12 union all
13 SELECT'张三'as stdname,stdname='物理', 物理 as result from #student2 where stdname='张三'
14 union all
15 SELECT'张三'as stdname,stdname='语文', 语文 as result from #student2 where stdname='张三'
```
 重构到：
```
代码
 1 declare @sql2 varchar(4000)
 2  set @sql2 = '' 
 3 SELECT @sql2=@sql2+ 
 4 'SELECT'''+stdname+'''as stdname,stdname=''化学'', 化学 as result from #student2 where stdname='''+stdname+'''
 5 union all
 6 SELECT'''+stdname+'''as stdname,stdname=''数学'', 数学 as result from #student2 where stdname='''+stdname+'''
 7 union all
 8 SELECT'''+stdname+'''as stdname,stdname=''物理'', 物理 as result from #student2 where stdname='''+stdname+'''
 9 union all
10 SELECT'''+stdname+'''as stdname,stdname=''语文'', 语文 as result from #student2 where stdname='''+stdname+''' union all '
11 from (SELECT stdname FROM #student2) as a
12 SELECT @sql2 = LEFT(@sql2,LEN(@sql2) - 10)
13 PRINT(@sql2)
14 exec(@sql2)
```
如果要求不能出现  化学  数学  物理 语文 这样的关键字，那么可以这样写：
```
代码
 1 select [name] into #tmpCloumns
 2 from tempdb.dbo.syscolumns
 3 where id=object_id('tempdb.dbo.#student2')
 4 and [name]<>'stdname'
 5 select *  from #tmpCloumns
 6 
 7 declare @strSql nvarchar(800)
 8 select @strSql=''
 9 select @strSql=@strSql+'union all'+char(10)+char(13)+
10                 'select [stdname],'''+[name]+''' as [科目],['+[name]+']'+char(10)+char(13)+
11                 'from [#student2]'+char(10)+char(13)
12 from #tmpCloumns
13 
14 select @strSql=substring(@strSql,11,len(@strSql))+'order by stdname,[科目]'
15 --print @strSql
16 exec(@strsql)
```
 这种题目，在各种笔试中出现的概率还是非常大的，大家不用死记。以前有的朋友看着复杂的报表查询，几百行SQL,望而生畏，然后说："这是哪个SQL超人写的啊！"其实，谁一上来不可能写出那么长的SQL,也是慢慢重构--调试--重构-······

----------------------------------------------------------------------------------------------------------
对于行转列，或者列转行，2005及以上版本，有两个运算符：Pivot 和 UnPivot。
这两个运算符，不甚了了，帮助也貌似语焉不详。我试了一下，它们都只对数值型字段才有效。
```
use [test]
go
declare @isTempTable int = OBJECT_ID(N'tempdb.dbo.#t');
IF (@isTempTable IS NOT NULL)
BEGIN
	DROP TABLE #t;
END	
GO
select * into #t from score;
ALTER table #t add number int;
UPDATE #t set number=case subject when '语文' then 1 WHEN '数学' then 2 else 3 end;
select * from #t;
select [name],[1] as 语文,[2] as 数学,[3] as 物理
from (select [name],result,number from #t) as t
pivot(sum(result) for number in([1],[2],[3])) as p
```
得到的结果是
namesubjectresultnumber
张三语文801
张三数学902
张三物理853
李四语文851
李四数学922
李四物理823
name语文数学物理
李四859282
张三809085
但如果将
pivot(sum(result) for **number** in([1],[2],[3])) as p 换成 pivot(sum(result) for **result**in([1],[2],[3])) as p
```
select [name],[1] as 语文,[2] as 数学,[3] as 物理
from (select [name],result from #t) as t
pivot(sum(result) for result in([1],[2],[3])) as p
```
结果将是这样
name语文数学物理
李四NULLNULLNULL
张三NULLNULLNULL
================================================================
脚本
```
--CREATE TABLE #student (stdname nvarchar(10),stdsubject nvarchar(10),result int)
--INSERT INTO #student VALUES ('张三','语文',80)
--INSERT INTO #student values ('张三','数学',90)
--INSERT INTO #student VALUES ('张三','物理',85)
--INSERT INTO #student VALUES ('李四','语文',85)
--INSERT INTO #student values ('李四','数学',92)
--INSERT INTO #student VALUES ('李四','物理',82)
--INSERT INTO #student VALUES ('李四','化学',82)
--INSERT INTO #student VALUES ('李四','化学',82)
SELECT * FROM #student;
--方法一
select stdname
,(select result from #student where stdname=s.stdname and stdsubject='语文') as '语文'
,(select result from #student where stdname=s.stdname and stdsubject='数学') as '数学'
,(select result from #student where stdname=s.stdname and stdsubject='物理') as '物理'
from #student s
group by stdname;
--方法二
select stdname
,isnull(sum(case when stdsubject='语文' then result else 0 end),0) as '语文'
,isnull(sum(case when stdsubject='数学' then result else 0 end),0) as '数学'
,isnull(sum(case when stdsubject='物理' then result else 0 end),0) as '物理'
into #student2
from #student s
group by stdname;
select * from #student2;
select stdname,stdsubject='语文',result=语文 from #student2
union all
select stdname,stdsubject='数学',result=数学 from #student2
union all
select stdname,stdsubject='物理',result=物理 from #student2
order by stdname;
```
