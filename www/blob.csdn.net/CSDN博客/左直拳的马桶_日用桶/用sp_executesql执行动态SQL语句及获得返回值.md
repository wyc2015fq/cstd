# 用sp_executesql执行动态SQL语句及获得返回值 - 左直拳的马桶_日用桶 - CSDN博客
2014年04月28日 22:08:41[左直拳](https://me.csdn.net/leftfist)阅读数：9696
过去我执行拼凑出来的动态SQL语句，都直接使用EXEC @sql 的方式。有好几次，都看到有资料说，应该尽量使用 sp_executesql。
究其原因，是因为仅仅参数不同的情况下，sp_executesql可以重用执行计划，这不就有跟存储过程一样的优势了吗？同时，sp_executesql还可以提供动态SQL语句执行的返回值，方便得很。
但sp_executesql的使用方式看上去比较复杂，一点都不像EXEC那样直观。
用法：
EXEC sp_executesql SQL语句,参数声明,参数
如
```
CREATE TABLE #tmp(t INT NOT NULL,it DECIMAL(18,4),pt DECIMAL(18,4));
	INSERT INTO #tmp(t,it,pt) VALUES(1,10.00,15.00);
	INSERT INTO #tmp(t,it,pt) VALUES(2,20.00,30.00);
	DECLARE @sql NVARCHAR(2000) = N'SELECT @it=it,@pt=pt FROM #tmp WHERE t=@tN;';
	DECLARE @Param NVARCHAR(500) = N'@tN INT,@it DECIMAL(18,4) OUTPUT,@pt DECIMAL(18,4) OUTPUT';
	DECLARE @tN INT,@it DECIMAL(18,4),@pt DECIMAL(18,4);
	
	SET @tN = 1;
	--@tN=@tN，第一个@tN是@Param里的声明，而第二个@tN则是外部的声明，其他类似
	EXEC sp_executesql @sql,@Param,@tN=@tN,@it=@it OUTPUT,@pt=@pt OUTPUT;
	SELECT @it,@pt;
	
	DROP TABLE #tmp;
```
为什么有返回值那么重要呢？我在不知道这个用法以前，如果需要捕捉动态SQL语句里的返回值，要借用表对象：
详见拙作：
[http://blog.csdn.net/leftfist/article/details/12840785](http://blog.csdn.net/leftfist/article/details/12840785)
```
declare @sql NVARCHAR(MAX) = N'SELECT @i,@j;';
declare @paramdefine NVARCHAR(MAX) = N'@i INT,@j INT,@k INT=3';
exec sp_executesql @sql,@paramdefine,@i=1,@j=2;
```
