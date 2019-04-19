# 一条SQL面试题 - 左直拳的马桶_日用桶 - CSDN博客
2014年03月13日 16:28:42[左直拳](https://me.csdn.net/leftfist)阅读数：3855

**求其中同一个主叫号码的两次通话之间间隔大于10秒的通话记录ID**
**例如：6，7，8，9，10条记录均符合**
ID 主叫号码 被叫号码      通话起始时间            通话结束时间            通话时长
1  98290000 0215466546656 2007-02-01 09:49:53.000 2007-02-01 09:50:16.000 23
2  98290000 021546654666  2007-02-01 09:50:29.000 2007-02-01 09:50:41.000 12
3  98290000 021546654666  2007-02-01 09:50:58.000 2007-02-01 09:51:12.000 14
4  68290900 0755133329866 2007-02-01 10:04:31.000 2007-02-01 10:07:13.000 162
5  78290000 0755255708638 2007-02-01 10:48:26.000 2007-02-01 10:49:23.000 57
6  78290000 0755821119109 2007-02-01 10:49:39.000 2007-02-01 10:52:55.000 196
7  78290000 035730928370  2007-02-01 11:30:45.000 2007-02-01 11:31:58.000 73
8  78290000 0871138889904 2007-02-01 11:33:47.000 2007-02-01 11:35:00.000 73
9  68290000 035730928379  2007-02-01 11:52:20.000 2007-02-01 11:54:56.000 156
10 68290000 0298521811199 2007-02-01 12:44:45.000 2007-02-01 12:45:04.000 19
将数据复制到记事本，保存为time.txt，导入到数据库，生成表[time]，然后可做测试。得到三个方法：
```
--方法一
select distinct * from(
SELECT a.* FROM dbo.[time] a left join dbo.[time] b
ON a.phone=b.phone
WHERE a.id<b.id 
AND DATEDIFF(second,a.st,b.et)>10
UNION ALL
SELECT a.* FROM dbo.[time] a left join dbo.[time] b
ON a.phone=b.phone
WHERE a.id>b.id 
AND DATEDIFF(second,b.st,a.et)>10) a
order by id,phone
--方法二
SELECT 
distinct 
a.* FROM dbo.[time] a left join dbo.[time] b
ON a.phone=b.phone
WHERE a.id<>b.id 
AND (DATEDIFF(second,a.st,b.et)>10 or DATEDIFF(second,b.st,a.et)>10)
order by id,phone
--方法三
select * from dbo.[time] t
where exists(select 1 from dbo.[time] where id<>t.id
	and phone=t.phone
	AND (DATEDIFF(second,st,t.et)>10 or DATEDIFF(second,t.st,et)>10))
```
我最喜欢方法三，采用exists语句。网上的许多SQL面试答案，通常用IN之类，极少用EXISTS，可能是习惯问题。
这道面试题，是从网上一篇文章里抄过来，作者给出的答案是：
SELECT DISTINCT a.* FROM dbo.hc a left join dbo.hc b
ON a.主叫号码=b.主叫号码
WHERE a.id<>b.id AND (DATEDIFF(second,a.通话起始时间,b.通话结束时间)>10 AND
DATEDIFF(second,b.通话起始时间,a.通话结束时间)>10)
我觉得在这里用 AND 是有问题的。
参考文章：
[http://blog.sina.com.cn/s/blog_63041bb80101a8mn.html](http://blog.sina.com.cn/s/blog_63041bb80101a8mn.html)
