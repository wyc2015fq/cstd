# 不用写成 if @i=1 OR @i=2 OR ... 这么蠢 - 左直拳的马桶_日用桶 - CSDN博客
2010年06月29日 11:31:00[左直拳](https://me.csdn.net/leftfist)阅读数：1387
原来Transact-SQL可以这样写：
declare @i int;
set @i = 0;
if @i in(1,2,3,4)
    select 'OK';
else
    select 'oh god';
结果显示：oh 
god.
这说明，不用写成 if @i=1 OR @i=2 OR ... 这么蠢。
