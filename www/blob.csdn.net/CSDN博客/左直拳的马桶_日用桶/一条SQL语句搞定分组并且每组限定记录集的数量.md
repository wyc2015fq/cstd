# 一条SQL语句搞定分组并且每组限定记录集的数量 - 左直拳的马桶_日用桶 - CSDN博客
2011年02月23日 18:39:00[左直拳](https://me.csdn.net/leftfist)阅读数：3524
如果我想得到这样一个结果集：分组，并且每组限定记录集的数量，用一条SQL语句能办到吗？
比如说，我想找出学生期末考试中，每科的前3名，只用一条SQL语句，该怎么写？
表[TScore]的结构
code      学号 char
subject  科目 int
score     成绩 int
可以这样写：
    SELECT [code]
        ,[subject]
        ,[score]
    FROM (
        SELECT *
        ,RANK() OVER(PARTITION BY subject ORDER BY score DESC) AS Row
        FROM TScore
    ) AS a
    WHERE Row
 <= 3 ;
