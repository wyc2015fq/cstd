# SQL SERVER中查找某关键词位于哪些存储过程或函数 - 左直拳的马桶_日用桶 - CSDN博客
2010年12月21日 18:19:00[左直拳](https://me.csdn.net/leftfist)阅读数：1917标签：[sql server																[存储](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=sql server&t=blog)
个人分类：[sql server](https://blog.csdn.net/leftfist/article/category/136470)
USE [MYDB]
go
SELECT * FROM INFORMATION_SCHEMA.ROUTINES
 WHERE routine_type='PROCEDURE'
AND routine_definition LIKE '%关键词
%'
