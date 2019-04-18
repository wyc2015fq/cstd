# SQL的基本命令和几个常用函数汇总 - 深之JohnChen的专栏 - CSDN博客

2005年09月01日 14:04:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1808


 --创建对象(表、视图、存储过程、函数)命令]
CREATE TABLE/VIEW/PROCEDURE/FUNCTION
--创建表
CREATE TABLE tabTestVB 
(VbName varchar(10),value numeric(10))
GO
CREATE TABLE tabTestVB1
(VbName varchar(10),value1 numeric(10))
GO
--插入数据（两种方式）
INSERT INTO tabTestVB(VbName,value)
SELECT 'AAA',123
INSERT INTO tabTestVB1(VbName,value1)
SELECT 'AAA',456

INSERT INTO tabTestVB(VbName,value) VALUES ('BBB',345)
INSERT INTO tabTestVB1(VbName,value1) VALUES ('CCC',1002)
--更改数据
UPDATE tabTestVB SET value=798 WHERE VbName='AAA'
--关联更改
UPDATE tabTestVB SET value=tabTestVB1.value1 
FROM tabTestVB1 WHERE tabTestVB.VbName=tabTestVB1.VbName
--删除数据
DELETE tabTestVB WHERE  VbName='AAA'
--无日志删除数据
TRUNCATE TABLE tabTestVB
--删除对象(表、视图、存储过程、函数)命令
DROP TABLE/VIEW/PROC/FUNCTION
--删除表
DROP TABLE tabTestVB
DROP TABLE tabTestVB1
--赋值命令
SET 
--定义变量
DECLARE 

--流程控制语句
WHILE ... BREAK
BEGIN ... END
IF ...ELSE
----1...100 的和
DECLARE @NN NUMERIC(3)
DECLARE @SUM NUMERIC(8)
SET @NN=1
SET @SUM=0
WHILE @NN<=100
 BEGIN
  SET @SUM=@SUM+@NN
  SET @NN=@NN+1
 END
SELECT @SUM

--加上条件：当@NN=20 时退出循环(计算出1...19的和)
DECLARE @NN NUMERIC(3)
DECLARE @SUM NUMERIC(8)
SET @NN=1
SET @SUM=0
WHILE @NN<=100
 BEGIN
  IF @NN<>20
   --BEGIN
   SET @SUM=@SUM+@NN
   --END
  ELSE
   --BEGIN
   BREAK
   --END
  SET @NN=@NN+1
 END
SELECT @SUM

--全局变量
@@ROWCOUNT
--返回受上一语句影响的行数
SELECT '1'
UNION ALL
SELECT '3'
SELECT @@ROWCOUNT

@@ERROR
--返回最后执行的 Transact-SQL 语句的错误代码。
SET @N =1
SELECT @@ERROR

----函数的使用
--返回当前日期
SELECT GETDATE()

--生成16进制的标志列uniqueidentifier
SELECT NEWID()

--转换数据类型和格式
SELECT CONVERT(VARCHAR(10),GETDATE(),120)

CAST( )

