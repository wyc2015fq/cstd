# 用临时表改善嵌套SQL语句的执行速度 - 左直拳的马桶_日用桶 - CSDN博客
2007年09月30日 17:03:00[左直拳](https://me.csdn.net/leftfist)阅读数：2526
用临时表改善嵌套SQL语句的执行速度
左直拳
这两天检查一条嵌套SQL语句，发觉非常耗时。形如：
SELECT KeyId,COUNT(1)AS Num 
FROM Table1
WHERE 1=1
AND CreateDate>='2007-09-21'
AND KeyId IN(SELECT KeyId FROM Table2 WHERE Id=1611)
GROUPBY KeyId
究其原因，大约该SQL语句执行的步骤是从Table1中每拿出一条记录，都要执行
IN(SELECT KeyId FROM Table2 WHERE Id=1611) 一番
靠，数据库也太弱智了吧。学编译方法时就知道，编译器会自动优化代码，将一些计算从循环中提取出来，数据库怎么就不能先查找出
(SELECT KeyId FROM Table2 WHERE Id=1611)
的结果，然后再代入整条SQL语句中执行呢？
先是这样修改：
SELECT a.KeyId,COUNT(1)AS Num 
FROM Table1 a
, (SELECT KeyId FROM Table2 WHERE Id=1611) AS b
WHERE 1=1
AND a.CreateDate>='2007-09-21'
AND a.KeyId=b.KeyId
GROUPBY a.KeyId
结果发现没什么改进，有时甚至效果更坏。
把心一横，祭出临时表来：
SELECT KeyId INTO t# FROM Table2 WHERE Id=1611;
SELECT a.KeyId,COUNT(1)AS Num 
FROM Table1 a
, t# AS b
WHERE 1=1
AND a.CreateDate>='2007-09-21'
AND a.KeyId=b.KeyId
GROUPBY a.KeyId；
DROP TABLE #t;
结果速度改善非常明显。不必担心并发操作时临时表会有冲突，说这个会话创建了一个t#,那个会话也创建了一个t#。临时表就好象局部变量，只在某个会话里有意义。
