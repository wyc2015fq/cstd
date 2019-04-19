# 一条SQL语句求每月连续低温时间 - 左直拳的马桶_日用桶 - CSDN博客
2014年04月29日 21:32:44[左直拳](https://me.csdn.net/leftfist)阅读数：2312
个人分类：[sql server](https://blog.csdn.net/leftfist/article/category/136470)
近期为某个项目写存储过程做统计。其中有个是这样的：
求每个月至少连续3个小时低于某个温度值的时间（小时）。
假设有个全年温度表：
```
CREATE TABLE #t(m INT, h INT ,t DECIMAL(18,4));--m:月份 h:小时 t:温度值  
--一年有8760小时，因此气候表里面有8760条记录，对应全年每小时的温度值
```
思路是这样的：
1、先将低于指定温度的记录都找出来
2、然后在这些记录中找出符合条件的，即那些属于至少连续3小时低于指定温度的记录
至少连续3小时低于指定温度的记录有如下特征：
比如说，有记录 h= 3、4、5、6、7，它们显然满足条件。
mht
1314
1412
1510
1612
1714
1912
11011
11313
11514
对于 h=3，存在着 h+1，h+2 都低于指定温度
对于 h=4、5、6，存在着 h-1，h+1 都低于指定温度
对于 h=7，存在着 h-1，h-2 都低于指定温度
只要满足这三项条件的任意一项，即可认为该时刻符合条件。因此可得SQL语句：
```
WITH w AS(
	SELECT [m],[h],[t]
		FROM #t 
		WHERE   [t] < 15 
)
SELECT 
	m
	,Free = COUNT(*)
	 FROM w
	 WHERE (EXISTS(SELECT 1 FROM w as w1 where m=w.m and h=w.h+1) AND EXISTS(SELECT 1 FROM w as w1 where m=w.m and h=w.h+2))
		OR (EXISTS(SELECT 1 FROM w as w1 where m=w.m and h=w.h-1) AND EXISTS(SELECT 1 FROM w as w1 where m=w.m and h=w.h+1))
		OR (EXISTS(SELECT 1 FROM w as w1 where m=w.m and h=w.h-1) AND EXISTS(SELECT 1 FROM w as w1 where m=w.m and h=w.h-2))
	 GROUP BY m;
```
