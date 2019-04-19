# SQL SERVER的整型运算，让人大吃一惊 - 左直拳的马桶_日用桶 - CSDN博客
2014年04月22日 15:43:53[左直拳](https://me.csdn.net/leftfist)阅读数：2506
                SQL SERVER的整型运算，让人大吃一惊
好比下面代码：
DECLARE @i INT = 50;
DECLARE @dec DECIMAL(18,4);
SET @dec = @i / 100;
SELECT @dec;
结果 @dec = 0.0000 !
我靠，这是什么逻辑。
将代码换成这个就正确：
DECLARE @i INT = 50;
DECLARE @dec DECIMAL(18,4);
SET @dec = @i / 100.00;
SELECT @dec;
得到 @dec = 0.5000 。
看起来，SQL SERVER的整除，好像是等同于FLOOR函数，取等于或小于的整数。比如，SELECT 19/10，结果是1，真操蛋。
所以，如果要应用 CEILING 这样的函数，像CEILING(19 /10)这样里面数字或变量全是整型，一定要先转换成浮点数，比如
CEILING(19 * 1.00 /10)
不清楚这是不是数据库的一个选项，可以设置？
