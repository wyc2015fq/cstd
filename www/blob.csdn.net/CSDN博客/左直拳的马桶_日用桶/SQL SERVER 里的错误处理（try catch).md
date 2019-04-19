# SQL SERVER 里的错误处理（try catch) - 左直拳的马桶_日用桶 - CSDN博客
2009年02月02日 12:01:00[左直拳](https://me.csdn.net/leftfist)阅读数：1624
 SQL SERVER里，也有TRY CATCH。格式如下
BEGIN TRY
       ……
END TRY
BEGIN CATCH
       ……
END CATCH。
另外，WITH 语句如果前面还有别的SQL语句，应该在前面的SQL语句结尾加上分号";"。比如在这个TRY CATCH里，就应该在前面加个";"，如下：
    BEGIN TRY
;
    WITH w AS(
        SELECT f1,f2,f3
              ,ROW_NUMBER() OVER(ORDER BY Id DESC) AS Row
        FROM [t1]
        WHERE Code=@Code
    )
    INSERT INTO [t2](
            f1,f2,f3
        SELECT f1,f2,f3
        FROM w WHERE Row>100;
    END TRY
    BEGIN CATCH
    END CATCH;
