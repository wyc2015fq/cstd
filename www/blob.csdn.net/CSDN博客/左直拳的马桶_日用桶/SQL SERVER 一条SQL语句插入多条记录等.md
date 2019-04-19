# SQL SERVER:一条SQL语句插入多条记录等 - 左直拳的马桶_日用桶 - CSDN博客
2014年07月08日 22:11:33[左直拳](https://me.csdn.net/leftfist)阅读数：4942
在学习排名第二的mySql过程中，发现它的插入语句可以这样写：
```
use test;
create table fruits(
	fid char(10) not null
	,s_id int null
	,f_name varchar(100)
	,f_price decimal(8,2)
	,primary key(fid)
);
insert into fruits(fid,s_id,f_name,f_price)
values('1',1,'zhang3',10.0)
,('2',1,'li4',10.0)
,('3',1,'zhang3',10.0)
,('4',1,'qian5',10.0);
```
不禁感叹，挺方便喔，一次可以插多条记录。
在SQL SERVER下试了一下，发现也可以的。惭愧！太孤陋寡闻了。
还学到这种写法：ALL
```
CREATE TABLE #temptb(id INT ,NAME VARCHAR(200))
  GO
  
  INSERT INTO [#temptb] ( [id], [NAME] )
  SELECT 1,'中国' UNION ALL
  SELECT 2,'中国' UNION ALL
 SELECT 3,'英国' UNION ALL
 SELECT 4,'英国' UNION ALL
 SELECT 5,'美国' UNION ALL
 SELECT 6,'美国' UNION ALL
 SELECT null, '法国' UNION ALL
 SELECT 8,'法国' 
 GO
 
 SELECT * FROM [#temptb]
 GO
 select * from #temptb a
	where id !<all(select id from #temptb where name=a.name)
```
另外，还有 CUBE、ROLLUP，GROUPING，这些都闻所未闻，用于汇总、报表统计。一时之间也搞不懂，也没心情搞，姑且记下来。
参考文章：
[http://www.cnblogs.com/lyhabc/p/3280634.html](http://www.cnblogs.com/lyhabc/p/3280634.html)

