# 我的MYSQL学习心得（三） - weixin_33985507的博客 - CSDN博客
2014年05月05日 13:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
原文:[我的MYSQL学习心得（三）](http://www.cnblogs.com/lyhabc/p/3696958.html)
# 我的MYSQL学习心得（三）
[我的MYSQL学习心得（一）](http://www.cnblogs.com/lyhabc/p/3691555.html)
[我的MYSQL学习心得（二）](http://www.cnblogs.com/lyhabc/p/3696629.html)
[我的MYSQL学习心得（四）](http://www.cnblogs.com/lyhabc/p/3697952.html)
[我的MYSQL学习心得（五）](http://www.cnblogs.com/lyhabc/p/3699969.html)
[我的MYSQL学习心得（六）](http://www.cnblogs.com/lyhabc/p/3703994.html)
查看字段的长度
SQLSERVER 
```
USE [sss]
GO
SELECT LEN([NAME]) FROM [dbo].[aa]
```
![](https://images0.cnblogs.com/i/257159/201404/281801280647643.jpg)
MYSQL
```
CREATE TABLE tmp13(vb VARBINARY(10))
INSERT INTO tmp13 (vb) VALUES(12)
SELECT LENGTH(vb) FROM tmp13
INSERT INTO tmp13 (vb) VALUES(1212)
```
![](https://images0.cnblogs.com/i/257159/201404/281803445959686.jpg)
**如有不对的地方，欢迎大家拍砖o(∩_∩)o **
