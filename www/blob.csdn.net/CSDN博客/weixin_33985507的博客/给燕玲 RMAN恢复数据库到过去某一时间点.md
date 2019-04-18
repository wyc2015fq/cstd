# |给燕玲|RMAN恢复数据库到过去某一时间点 - weixin_33985507的博客 - CSDN博客
2013年01月13日 16:03:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
今天和13燕玲打电话沟通得知她实施的客户遇到过还原系统到几天前的情况（比如增加了很多的辅助，可是发现不合理，又不想让数据库产生垃圾数据，就需要用昨天的exp备份的东西imp还原），但是我考虑到了一个问题：
数据库在好好的，有数据的情况下，imp肯定会报有重复数据的错误，就比如：
*就像你电脑有一个qq了，如果坚持安装旧的版本，安装的时候会卸载旧的版本*
rman可以基于归档日志恢复到过去一个时间点，具体看测试
1，还原之前的scott.t1表，没有time字段
insert两条数据到这个表，表示更新了
![](https://images0.cnblogs.com/blog/412950/201301/13160004-550c627f673b4031bd948a778d4daab7.png)
2，恢复需要在mount下操作，和上一个rman试验一样
比如我认为现在的数据库有问题，我想回到昨天晚上9点的时候，从昨天晚上9点到现在的数据我不要了。
run{
set until time "to_date('2013-1-12 21','yyyy-mm-dd hh24')";
restore database;
recover database;
}
rman下copy上面的代码之后回车
之后不要忘记resetlogs
查询表t1，已经没有了time字段
 如果单独的恢复一个表到过去一个时间点，使用回滚段
http://www.cnblogs.com/sumsen/archive/2012/12/27/2835452.html
```insert``into``emp ``select``* ``from``emp ``as``of``timestamp``(systimestamp-interval ``'5'``minute``);`
 17:11 update
查看归档日志的路径
show parameter db_recovery_file_dest;
更改到D盘
SYS@ncbeta>alter system set db_recovery_file_dest='d:\xs\archivedlogb' scope=spfile;
重启生效
在强制切换日志之后，在'd:\xs\archivedlogb'第一次产生了归档日志，37.9M
SYS@ncbeta>alter system switch logfile;
