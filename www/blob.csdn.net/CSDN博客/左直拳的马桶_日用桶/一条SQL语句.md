# 一条SQL语句 - 左直拳的马桶_日用桶 - CSDN博客
2006年09月29日 11:28:00[左直拳](https://me.csdn.net/leftfist)阅读数：1745
一条SQL语句
 左直拳
如何写好SQL语句是一门很高深的学问，也有许多东西要学。
今天因为工作的关系，需要用SQL语句进行统计，求一个平均数，发现可以这样写：
SELECT (SELECT COUNT(1) AS stakeout FROM [表名A])/(SELECT COUNT(1) AS [user] FROM [表名B]) AS num,type='AVERAGE'
一条SQL，
学问宽，
风吹稻花香两岸
俺家就在岸上住
听惯了艄公的、耗子
看惯了船上的白哎哎帆
