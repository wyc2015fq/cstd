# 一条DELETE语句 - 左直拳的马桶_日用桶 - CSDN博客
2009年02月02日 12:11:00[左直拳](https://me.csdn.net/leftfist)阅读数：1314
 WITH w AS(        
        SELECT Id,ROW_NUMBER() OVER(ORDER BY Id DESC) AS [Row]
        FROM [t1]
        WHERE Code=@Code
    )
    DELETE FROM t1 FROM t1 s INNER JOIN w ON s.Id=w.Id
    WHERE w.Row>100;
原来DELETE语句有 delete from *** from *** 的格式
