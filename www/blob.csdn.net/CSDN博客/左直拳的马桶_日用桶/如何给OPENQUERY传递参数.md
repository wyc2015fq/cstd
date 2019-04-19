# 如何给OPENQUERY传递参数 - 左直拳的马桶_日用桶 - CSDN博客
2009年11月10日 09:51:00[左直拳](https://me.csdn.net/leftfist)阅读数：9113

发现SQL SERVER里，直接调用链接服务器的函数是不可以的。
CREATE FUNCTION func_Test
(    
    @UserId INT
)
RETURNS TABLE 
AS
RETURN 
(
    SELECT * FROM [192.168.0.252].[User].dbo.func_GetData(@UserId)
)
GO
会报“不允许使用远程表值函数调用。”的错误
网上说可以使用OPENQUERY
但这个东西没办法直接传参数，
CREATE FUNCTION func_Test
(    
    @UserId INT
)
RETURNS TABLE 
AS
RETURN 
(
    SELECT * FROM OPENQUERY([192.168.0.252],'SELECT * FROM [192.168.0.252].[User].dbo.func_GetData(' + CAST(@UserId AS VARCHAR(20)) + ')')
)
GO
报错
消息102，级别15，状态1，过程func_Test，第9 行
'+' 附近有语法错误。
不管怎么调这个字符串，都不行。语法其实没错误，但一放到OPENQUERY就不行。
后来不用函数，改用存储过程来调用才解决：
CREATE PROCEDURE [dbo].[prd_Test]
    @UserId INT
AS
BEGIN
    DECLARE @myUserId VARCHAR(20);
    SET @myUserId = CAST(@UserId AS VARCHAR(20));
    DECLARE @sql VARCHAR(1000);
    SET @sql = 'SELECT * FROM OPENQUERY([192.168.0.252],''SELECT * FROM [User].dbo.func_GetData(' + @myUserId + ')'')';
    EXEC(@sql);
END
