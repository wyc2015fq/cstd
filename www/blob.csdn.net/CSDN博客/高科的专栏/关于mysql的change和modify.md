
# 关于mysql的change和modify - 高科的专栏 - CSDN博客

2015年01月04日 20:55:34[高科](https://me.csdn.net/pbymw8iwm)阅读数：11875


前端时间要写个游戏里的邮件系统，定义了一个如下的表结构：

```python
CREATE TABLE `sysmail` (
  `mailid` int(20) NOT NULL AUTO_INCREMENT,
  `sendtime` int(11) NOT NULL DEFAULT '0',
  `mailtitle` varchar(512) COLLATE utf8_bin NOT NULL DEFAULT '',
  `mailcontext` varchar(2048) COLLATE utf8_bin NOT NULL DEFAULT '',
  `attachment` blob,
  `obj` blob,
  PRIMARY KEY (`mailid`)
) ENGINE=InnoDB AUTO_INCREMENT=2147483647 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
```
后来觉得mailid是自增的在合服的时候不好处理，我就把mailid的自增字段去掉，用的是很传统的

```python
alter table sysmail modify column mailid int(20) not null;
```
但是发现，在没有重启mysql服务器的前提下，插入不同的mailid的记录会提示1062的错误，后来又改成
```python
alter table sysmail change mailid mailid bigint(20)  not null;
```
就成功了，而不需要重启mysql服务器就生效。看来我还是没有搞懂modify和change的区别

