# sqlite3常用操作命令 和mysql的区别及优缺点 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年06月30日 13:22:22[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1019
SQLite 的数据库权限只依赖于文件系统，没有用户帐户的概念。
sqlite3 testdb.db
**.databases **命令查看数据库列表
create table tbl(name char(10), age smallint, score float);
列出表和架构
.tables
插入数据
insert into tbl values('yanggang', 24, 98);
insert into tbl values('sunboy', 20, 78.5);
查询数据
select * from tbl;

[http://blog.csdn.net/ithomer/article/details/7027424](http://blog.csdn.net/ithomer/article/details/7027424)
[http://ivanzz.blog.51cto.com/6378766/1130544](http://ivanzz.blog.51cto.com/6378766/1130544)
