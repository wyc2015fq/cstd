# Distinct和Group by去除重复字段记录 - weixin_33985507的博客 - CSDN博客
2016年11月18日 17:12:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
 重复记录 有两个意义，一是完全重复的记录，也即所有字段均重复的记录
             二是部分关键字段重复的记录，比如Name字段重复，而其他字段不一定重复或都重复可以忽略。
　　1、对于第一种重复，比较容易解决，使用
　　select distinct * from tableName
　　就可以得到无重复记录的结果集。
　　如果该表需要删除重复的记录(重复记录保留1条)，可以按以下方法删除
　　select distinct * into #Tmp from tableName
　　drop table tableName
　　select * into tableName from #Tmp
　　drop table #Tmp
　　发生这种重复的原因是表设计不周产生的，增加唯一索引列即可解决。
　　2、这类重复问题通常要求保留重复记录中的第一条记录，操作方法如下
　　假设有重复的字段为Name,Address，要求得到这两个字段唯一的结果集
　　select identity(int,1,1) as autoID, * into #Tmp from tableName
　　select min(autoID) as autoID into #Tmp2 from #Tmp group by Name
　　select * from #Tmp where autoID in(select autoID from #tmp2)
　　最后一个select即得到了Name，Address不重复的结果集(但多了一个autoID字段，实际写时可以写在select子句中省去此列)
其它的数据库可以使用序列，如：
create sequence seq1;
select seq1.nextval as autoID, * into #Tmp from tableName
============
zuolo: 我根据上面实例得到所需要的语句为 SELECT MAX(id) AS ID,Prodou_id,FinalDye FROM anwell..tblDBDdata GROUP BY Prodou_id,FinalDye ORDER BY id，之前一直想用Distinct来得到指定字段不重复的记录是个误区。
