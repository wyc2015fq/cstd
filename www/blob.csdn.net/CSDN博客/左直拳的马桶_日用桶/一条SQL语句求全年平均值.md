# 一条SQL语句求全年平均值 - 左直拳的马桶_日用桶 - CSDN博客
2014年04月28日 22:44:34[左直拳](https://me.csdn.net/leftfist)阅读数：4088
一年有8760个小时！（才这么点。。。）
有个气候表，存储了当地从1到8760小时的温度数据。现在，要求全年的温度每天平均值。
```
CREATE TABLE #Climate(h INT ,t DECIMAL(18,4));--h:小时 t:温度值
--里面有8760条记录，对应全年每小时的温度值
--现在，想将全年气温按天计算平均值，保存到表#tD
CREATE TABLE #tD(d INT ,avgt DECIMAL(18,4));--d:天 avgt:平均温度值
--最简单，就是用循环
DECLARE @d INT;
DECLARE @h1 INT,@h2 INT;
SET @d = 1;
WHILE @d <= 365
BEGIN
	SET @h1 = 24 * (@d - 1);
	SET @h2 = @h1 + 24; 
	--缓存计算值到表变量
	INSERT INTO #tD(d,avgt)
		SELECT @d
			,AVG(t) 
		FROM [#Climate]
		WHERE h BETWEEN @h1 AND @h2;
	SET @d = @d + 1;
END --end of @d <= 365
```
但是，存储过程用循环并没有什么优势，效率不高。我们可以使用一条SQL语句来搞定它。
首先要设置一张 天 表，一年有365天，对应有365条记录
```
CREATE TABLE #days(d INT);
DECLARE @i INT = 1;
WHILE @i <= 365
BEGIN
	INSERT INTO #days(d) VALUES(@i);
	SET @i = @i + 1;
END
```
利用这个天表，进行聚合运算
```
INSERT INTO #tD(d,avgt)
		SELECT ds.d
			,AVG(c.t) 
		FROM [#Climate] c,[#days] AS ds
		WHERE c.h BETWEEN 24 * (ds.d - 1) AND ds.d * 24
		GROUP BY ds.d;
```
