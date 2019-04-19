# LIKE语句也可以这样写 - 左直拳的马桶_日用桶 - CSDN博客
2011年03月10日 18:48:00[左直拳](https://me.csdn.net/leftfist)阅读数：1350
SELECT * 
FROM table1
WHERE '这是一个字符串'LIKE'%'
 + field1 + '%'
