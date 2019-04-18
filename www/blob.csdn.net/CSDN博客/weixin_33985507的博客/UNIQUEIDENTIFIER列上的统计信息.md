# UNIQUEIDENTIFIER列上的统计信息 - weixin_33985507的博客 - CSDN博客
2015年08月15日 08:16:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
UNIQUEIDENTIFIER列上的统计信息非常有意思，在它上面有一些很令人讨厌的行为。我们来看下。
# 问题重现（The repro）
为了向你展示我们刚抱怨的行为，我用下列简单的表定义创建了一个数据库，我在**UNIQUEIDENTIFIER**列上强制主键约束。这意味着SQL Server在后台会生成唯一聚集索引，聚集索引本身有一个统计信息对象来描述那列的数据分布情况。当然，数据分布是线性的，因为在UNIQUEIDENTIFIER列每个值本身都是唯一的。
```
1 -- Create a new table with a UNIQUEIDENTIFIER column as primary key.
2 -- SQL Server will enforce the primary key constraint through unique clustered index in the background.
3 CREATE TABLE CustomersTableGuid
4 (
5     ID UNIQUEIDENTIFIER NOT NULL PRIMARY KEY,
6     FirstName VARCHAR(50),
7     LastName VARCHAR(50)
8 )
9 GO
```
下一步我往表里插入1百万条记录。
```
1 -- Insert 1 million records
 2 DECLARE @i INT = 0
 3 WHILE (@i <= 1000000)
 4 BEGIN
 5     INSERT INTO CustomersTableGuid (ID, FirstName, LastName)
 6     VALUES
 7     (
 8         NEWID(),
 9         'FirstName' + CAST(@i AS VARCHAR),
10         'LastName' + CAST(@i AS VARCHAR)
11     )
12     
13     SET @i +=1
14 END
15 GO
```
现在我们用**FULLSCAN**在表上更新我们的统计信息。**FULLSCAN**意味着SQL Server扫描整个表内在数据来更新统计信息对象。
```
1 -- Let's update the Statistics with a FULLSCAN.
2 UPDATE STATISTICS CustomersTableGuid WITH FULLSCAN
3 GO
```
但当你现在查看统计信息对象时，你会看到在直方图里SQL Server只生成了4个步长。
```
1 sp_helpindex 'dbo.CustomersTableGuid'
2 
3 DBCC SHOW_STATISTICS('dbo.CustomersTableGuid', 'PK__Customer__3214EC271273C1CD')
```
![](https://images0.cnblogs.com/blog2015/750348/201508/111008357707050.png)
在表头信息里你可以看到，在统计信息更新期间，1百万行被采样，但直方图只显示了4个步长！但当你现在用更小采样区间来更新统计信息对象，事情就会改变：
```
1 -- Let's update the Statistics with a smaller sampling interval.
2 UPDATE STATISTICS CustomersTableGuid WITH SAMPLE 50 PERCENT
3 GO
```
当你现在看下直方图，你会看到我们有很多不同的步长：
![](https://images0.cnblogs.com/blog2015/750348/201508/111014214737036.png)
# 小结
当你在数据库设计里用**UNIQUEIDENTIFIER**列时要记住这点了。只要这些值是唯一的，你就会有性能上的问题，因为对于直方图里，你有的巨量区间，**AVG_RANGE_ROW**只能做出1行的正确估计。
感谢关注！
# 参考文章：
[https://www.sqlpassion.at/archive/2015/03/16/statistics-uniqueidentifier-columns/](https://www.sqlpassion.at/archive/2015/03/16/statistics-uniqueidentifier-columns/)
