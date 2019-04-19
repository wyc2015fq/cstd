# 利用远程数据库存储过程的OUTPUT参数来获得返回值 - 左直拳的马桶_日用桶 - CSDN博客
2010年08月20日 10:23:00[左直拳](https://me.csdn.net/leftfist)阅读数：1509标签：[数据库																[output																[存储																[sql server](https://so.csdn.net/so/search/s.do?q=sql server&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=output&t=blog)](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)
个人分类：[sql server](https://blog.csdn.net/leftfist/article/category/136470)
SQL SERVER中，是不能直接调用远程数据库的函数的，有时候觉得不方便。
但调用远程数据库的存储过程就没有问题，并且存储过程有OUTPUT类型的参数，某些情况下，可以这样子来变通使用，获得返回数据。
远程数据库：[192.168.0.1].dbo.[RemoteDb]：
CREATE PROCEDURE prd1
    @In INT,
    @out INT OUTPUT
AS
BEGIN
    SET @In = 0;
    SET @out = @out * -1;
END
GO
本地数据库：[localDb]：
declare @In int
declare @out int
SET @in = 1;
SET @out = 1;
EXEC [192.168.0.1].dbo.[RemoteDb].prd1 @in,@out OUTPUT;
SELECT @in;
SELECT @out;
--结果显示：
1
-1
这说明返回值真的起作用。如果有一些运算或数据必须要放在远程数据库处理，值得考虑。
