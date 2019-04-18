# Oracle 删除重复数据只留一条 - z69183787的专栏 - CSDN博客
2013年11月05日 10:15:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：951
个人分类：[Oracle																[数据库-Sql&Hql](https://blog.csdn.net/z69183787/article/category/2185915)](https://blog.csdn.net/z69183787/article/category/2175483)
查询及删除重复记录的SQL语句
1、查找表中多余的重复记录，重复记录是根据单个字段（Id）来判断
select * from 表 where Id in (select Id from 表 group byId having count(Id) > 1)
2、删除表中多余的重复记录，重复记录是根据单个字段（Id）来判断，只留有rowid最小的记录
DELETE from 表 WHERE (id) IN ( SELECT id FROM 表 GROUP BY id HAVING COUNT(id) > 1) AND ROWID NOT IN (SELECT MIN(ROWID) FROM 表 GROUP BY id HAVING COUNT(*) > 1);
3、查找表中多余的重复记录（多个字段）
select * from 表 a where (a.Id,a.seq) in(select Id,seq from 表 group by Id,seq having count(*) > 1)
4、删除表中多余的重复记录（多个字段），只留有rowid最小的记录
delete from 表 a where (a.Id,a.seq) in (select Id,seq from 表 group by Id,seq having count(*) > 1) and rowid not in (select min(rowid) from 表 group by Id,seq having count(*)>1)
5、查找表中多余的重复记录（多个字段），不包含rowid最小的记录
select * from 表 a where (a.Id,a.seq) in (select Id,seq from 表 group by Id,seq having count(*) > 1) and rowid not in (select min(rowid) from 表 group by Id,seq having count(*)>1)
