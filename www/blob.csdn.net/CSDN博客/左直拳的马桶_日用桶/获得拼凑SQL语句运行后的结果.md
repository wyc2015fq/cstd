# 获得拼凑SQL语句运行后的结果 - 左直拳的马桶_日用桶 - CSDN博客
2013年10月17日 18:19:25[左直拳](https://me.csdn.net/leftfist)阅读数：2854
个人分类：[sql server](https://blog.csdn.net/leftfist/article/category/136470)
拼凑式的SQL语句太普遍了，比如说，任意条件的查询。
这种拼凑SQL语句，如果在数据库端组装，必然是一条长长的字符串，然后 exec(@sql)。
如果这条语句写在存储过程里边，存储过程的调用方会获得查询结果。但假如是这个存储过程里面就想拿到结果来进行处理，怎么办呢？
select exec(@sql)必然是错的。
可以酱紫：
```
declare @t TABLE
(
	[CatchResultID] [bigint] NOT NULL,
	[WebSiteKeyWordID] [int] NULL,
	[Url] [varchar](300) NULL,
	[Title] [varchar](300) NULL,
	[CatchDate] [smalldatetime] NULL
);
		
DECLARE @ID	INT = 525215449;
--使用了链接服务器，调用其数据库里的自定义函数
declare @sql varchar(1000) = 'SELECT * FROM OPENQUERY([192.168.15.2,8888]
,''SELECT * FROM [db].dbo.cr_func_GetCatchResultById(' + CAST(@ID AS varchar(50)) + ')'');';
INSERT INTO @t(
   [CatchResultID]
  ,[WebSiteKeyWordID]
  ,[Url]
  ,[Title]
  ,[CatchDate]
)
exec(@sql);
--看，里面有结果矣
select * from @t;
```
为什么使用链接服务器的函数要用拼凑SQL呢，主要是因为有参数。而使用链接服务器的函数，不可以这样的：
`SELECT * FROM OPENQUERY([192.168.15.2,8888],@sql);`
===========================================================
很遗憾，这种方式不能用在SQL的函数里，否则报错：
在函数内对带副作用的运算符 'INSERT EXEC' 的使用无效
