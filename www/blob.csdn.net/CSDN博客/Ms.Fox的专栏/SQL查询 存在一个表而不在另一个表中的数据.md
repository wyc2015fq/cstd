# SQL查询~ 存在一个表而不在另一个表中的数据 - Ms.Fox的专栏 - CSDN博客
2018年10月23日 15:39:46[lulu-lu](https://me.csdn.net/smbluesky)阅读数：93标签：[数据库查询](https://so.csdn.net/so/search/s.do?q=数据库查询&t=blog)
# [SQL查询~ 存在一个表而不在另一个表中的数据](https://www.cnblogs.com/jameshappy/p/6038706.html)
A、B两表，找出ID字段中，存在A表，但是不存在B表的数据。A表总共13w数据，去重后大约3W条数据，B表有2W条数据，且B表的ID字段有索引。
## 方法一
　　使用 not in ,容易理解,效率低  ~执行时间为：1.395秒~
select distinct A.ID from  A where A.ID not in (select ID from B)
## 方法二
　　使用 left join...on... , "B.ID isnull" 表示左连接之后在B.ID 字段为 null的记录  ~执行时间：0.739秒~
select A.ID from A left join B on A.ID=B.ID where B.ID is null
　　图解
![](https://pic002.cnblogs.com/images/2012/394322/2012050916243279.png)
## 方法三
　　逻辑相对复杂,但是速度最快  ~执行时间: 0.570秒~
  select * from  B 
     where (select count(**1**) as num from A where A.ID = B.ID) = **0**
