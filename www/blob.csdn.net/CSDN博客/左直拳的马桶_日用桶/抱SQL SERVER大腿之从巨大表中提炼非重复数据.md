# 抱SQL SERVER大腿之从巨大表中提炼非重复数据 - 左直拳的马桶_日用桶 - CSDN博客
2007年05月10日 17:09:00[左直拳](https://me.csdn.net/leftfist)阅读数：1541标签：[sql server																[insert																[delete																[table																[sql](https://so.csdn.net/so/search/s.do?q=sql&t=blog)](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=sql server&t=blog)
个人分类：[sql server](https://blog.csdn.net/leftfist/article/category/136470)
抱SQL SERVER大腿之从巨大表中提炼非重复数据
左直拳
有一个表，2千多万条记录，其中有很大部分只有一两个字段的内容不同，可视为重复记录。现在要从这个表里提炼出不重复的部分，转移到另一个新表里。同时这个转移不是简单的复制，因为两表的结构不同，新表中的新字段需要赋值。
凭我那两下子，是这么搞的：
1、不能用一条SQL语句搞定（也许有，我写不出来）。要用循环；
2、使用临时表；
3、删除临时表中的重复记录，然后转移到新表
过程如下：
--创建一个与新表一模一样的临时表
CREATETABLE #TEMP ……
--循环，将符合条件的记录（未过滤重复）插进临时表
--这里对这个巨大表是分批处理的，每个循环最多处理10万条
WHILE( @i < @j )
BEGIN
SET @k1 = @i * 100000
SET @k2 =(@i + 1)* 100000
INSERTINTO #TEMP(……)
SELECT
……
FROM a, b
WHERE……
AND a.Id BETWEEN @k1 AND @k2
GROUPBY……
--赋新值
UPDATE #TEMP SET……
FROM #TEMP,a
WHERE……
AND a.Id BETWEEN @k1 AND @k2
SET @i = @i + 1
END
--删除重复行
DELETEFROM #TEMP WHERE
EXISTS(SELECT 1 FROM #TEMP a WHERE a.Id<#TEMP.Id AND a. KeyId=#TEMP.KeyId)
--最后将数据从临时表复制到新表
INSERTINTO……
SELECT……
FROM #TEMP
两千多万条记录啊。
两千多万条记录算条毛？要是两千多万元人民币多好。
