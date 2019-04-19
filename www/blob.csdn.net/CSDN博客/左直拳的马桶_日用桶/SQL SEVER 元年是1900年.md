# SQL SEVER 元年是1900年 - 左直拳的马桶_日用桶 - CSDN博客
2013年06月23日 08:37:27[左直拳](https://me.csdn.net/leftfist)阅读数：2366
用SQL语句求 本月第一天，怎么写？
可以这样写：
`SELECT DATEADD(mm,DATEDIFF(mm,0,GETDATE()),0);`
按照日期函数DATEDIFF的定义，第二个参数是开始日期，第三个参数是结束日期，那么在这里，0是什么意思？
好像在哪里看过，说日期类型，本质上也是个数值，那么在这里写0也不会有错。问题是，0是哪个日子？
`SELECT CAST(0 AS DATETIME)`
得到的结果是：1900-01-01 00:00:00.000
原来，0就是SQL SERVER的元年。因此，
`SELECT DATEADD(mm,DATEDIFF(mm,0,GETDATE()),0);`
的意思就是，先求得今天距离元年的月份，然后用元年 + 这个相距月份，得到的日期，就是本月的第一天。
与此类似，本周星期一是
`SELECT DATEADD(wk, DATEDIFF(wk,0,GETDATE()),0);`
来个复杂一点的，求本月第一周星期一呢？
```
--本月第一天
DECLARE @firstday DATETIME = DATEADD(mm,DATEDIFF(mm,0,GETDATE()),0);
--本月第七天
DECLARE @sevenday DATETIME = DATEADD(dd,6,@firstday);
--本月第一周星期一
SELECT DATEADD(wk,DATEDIFF(wk,0,@sevenday),0);
```
为什么最后要用第7天来求？
假如本月第1天是周一，那么第7天是周日，是同一周；
假如本月第1天是周日，那么第7天是周六，跨了一周，周一位于第7天所在周；
两个极端情况都考虑了，其他的更清楚，就是说，本月第一周的周一，必然位于本月第7天所在的这个星期里。
