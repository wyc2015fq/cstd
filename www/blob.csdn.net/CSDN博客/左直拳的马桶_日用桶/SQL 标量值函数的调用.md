# SQL 标量值函数的调用 - 左直拳的马桶_日用桶 - CSDN博客
2009年11月04日 10:23:00[左直拳](https://me.csdn.net/leftfist)阅读数：11696
调用 MS SQL 标量值函数，应该在函数前面加上 "dbo."，否则会报 “不是可以识别的 内置函数名称”错误。例如
    DECLARE @WhichDB TINYINT;
    SELECT @WhichDB = dbo.
user_GetWhichDB(1);--看看是哪个数据库的
另外，标量值函数就相当于一个**变量**
，而不是一个表
，所以这样写是错误的：
    SELECT * FROM dbo.
user_GetWhichDB(1);
应该这样写：
    SELECT dbo.
user_GetWhichDB(1);
加上别名：
    SELECT dbo.
user_GetWhichDB(1) AS FieldName;
=================================================
--标量值函数
ALTER FUNCTION [dbo].[user_GetWhichDB]
(
    @UserId INT = 0
)
RETURNS TINYINT
WITH EXECUTE AS CALLER
AS
BEGIN
    DECLARE @WhichDB TINYINT;
    SET @WhichDB = 1;
    IF @UserId >= 115098
        SET @WhichDB = 2;
    RETURN (@WhichDB);
END
