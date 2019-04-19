# 去除重复数据的SQL语句 - 左直拳的马桶_日用桶 - CSDN博客
2010年10月19日 16:00:00[左直拳](https://me.csdn.net/leftfist)阅读数：1076标签：[sql																[table](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=sql&t=blog)
使用 MAX + group by：
SELECT BookId,MAX(CreateDate) AS MaxCreateDate
 FROM Table1
    WHERE StationID=2
    GROUP BY BookId
    ORDER BY MaxCreateDate
 DESC
MAX在这里主要是用于排序。过滤重复的话，group by足矣
