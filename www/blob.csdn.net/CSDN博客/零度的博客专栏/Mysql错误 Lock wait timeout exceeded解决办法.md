# Mysql错误: Lock wait timeout exceeded解决办法 - 零度的博客专栏 - CSDN博客
2016年04月27日 10:35:48[零度anngle](https://me.csdn.net/zmx729618)阅读数：13124
临时解决办法：
执行mysql命令:show full processlist;
然后找出插入语句的系统id
执行mysql命令:kill id
在网络上找了想相关资料,这里摘录如下:
首先,查看数据库的进程信息:
show full processlist;
/*结果略,主要看id列的值*/
再查看事物表:
SELECT * FROM information_schema.INNODB_TRX\G;
 /*结果略,注意结果中的trx_mysql_thread_id部分的值*/
查找对应的id,然后kill id的值即可;
mysql默认是自动提交的,可以用以下命令查看:
 select @@autocommit;
值为1就是自动提交,如果你的不是1,但你又想要自动提交,那么设置的命令是:
set global autocommit=1;
表数据量也不大，按照普通的情况来说，简单的update应该不会造成阻塞的，mysql都是autocommit，不会出现update卡住的情况，去查看下autocommit的值。
mysql> select @@autocommit;
 +--------------+
 | @@autocommit |
 +--------------+
 | 0 |
 +--------------+
 1 row in set (0.00 sec)
 mysql>
看到亮闪闪的0，这个设置导致原来的update语句如果没有commit的话，你再重新执行update语句，就会等待锁定，当等待时间过长的时候，就会报ERROR 1205 (HY000): Lock wait timeout exceeded; try restarting transaction的错误。
 所以赶紧commit刚才执行的update语句，之后 set global autocommit=1;
2、最终解决方案：
    出现该问题大多数是因为出现死锁或者connection/session没有关闭造成的，去检查执行该sql的语句，检查是否有finally{}中关闭连接。
