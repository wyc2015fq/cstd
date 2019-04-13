
# Linux 平台下 误删 oracle 数据文件的恢复方法 - David Dai -- Focus on Oracle - CSDN博客


2015年03月05日 14:36:54[Dave](https://me.csdn.net/tianlesoftware)阅读数：12881




# 1  问题描述

之前写过一篇删除oracle home目录的blog，参考：
[Linux 平台误删 home oracle 根目录的解决方法](http://blog.csdn.net/tianlesoftware/article/details/43794273)
[http://www.cndba.cn/Dave/article/423](http://www.cndba.cn/Dave/article/423)
本篇是这边的引深，本来应该是年前整理的，拖到年后了。
模拟现状：
数据库在正常运行，误操作，直接rm 掉了数据文件。
测试环境：
[oracle@dg1 trace]$ cat /etc/redhat-release
Red Hat Enterprise Linux Server release 6.1(Santiago)
Oracle 11.2.0.3 单实例。
[oracle@dg1 ~]$ sqlplus / as sysdba
SQL*Plus: Release 11.2.0.3.0 Production onWed Aug 27 18:36:32 2014
Copyright (c) 1982, 2011, Oracle.  All rights reserved.

Connected to:
Oracle Database 11g Enterprise EditionRelease 11.2.0.3.0 - 64bit Production
With the Partitioning, OLAP, Data Miningand Real Application Testing options
SQL>

这个问题也要分2种情况，一种是归档模式，一种是非归档模式，归档模式处理就容易很多了。但现在很多由开发人员管理的库是非归档，并且也缺乏专业的运维技能，误操作的概率也会增加很多。

# 2  创建测试数据
SQL> create tablespacedropspacedatafile '/u01/dropspace01.dbf' size 100m;
Tablespace created.
SQL>create user ahzhixinidentified by ahzhixin default tablespace dropspace;
User created.
SQL>grantconnect,resource,dba to ahzhixin;
Grant succeeded.
SQL> conn ahzhixin/ahzhixin
Connected.
SQL> create table test1 as select * fromall_users;
Table created.
SQL> create table test2 as select * fromall_users;
Table created.
SQL> create table test3 as select * fromall_users;
Table created.


# 3  归档模式处理

## 3.1            模拟故障
SQL> archive log list;
Database log mode        Archive Mode
Automatic archival          Enabled
Archive destination         /u01/archivelog
Oldest online log sequence     83
Next log sequence to archive   85
Current log sequence        85
SQL>

在操作系统上直接删除数据文件：
[oracle@dg1 u01]$rm-rf /u01/dropspace01.dbf
此时数据库还是正常运行，我们查询我们之前创建的几张表：
SQL> select count(1) from test1;
COUNT(1)
----------
31
SQL> select count(1) from test2;
COUNT(1)
----------
31
SQL> select count(1) from test3;
COUNT(1)
----------
31

--insert 也没有问题：
SQL> insert into test1 select * fromall_users;
31 rows created.
SQL> commit;
Commit complete.
SQL> select count(1) from test1;
COUNT(1)
----------
62
至少从目前看，一切都是正常。  这里正常也是因为我们的操作系统是Linux，当数据文件从操作系统级别被rm掉，但之前打开该文件的进程仍然持有相应的文件句柄，所以指向的文件仍然可以读写，并且该文件的文件描述符可以从/proc目录中获得，也可以利用这个句柄恢复文件。
如果在这个时候，重启了数据库或者操作系统，那么句柄就会消失，也就只能通过扫描磁盘进行文件恢复。

## 3.2            恢复
dbwr进程会打开所有数据文件的句柄，在proc目录中可以查到这些数据文件的信息，目录名是进程PID，fd表示文件描述符。
检查dbwr的进程PID：
[oracle@dg1 trace]$ps -ef|grep dbw0|grep-v grep
oracle   9964     1  0 00:49 ?        00:00:03 ora_dbw0_dave
[oracle@dg1 trace]$ cd /proc/9964/fd
[oracle@dg1 fd]$ ls -l
![](https://img-blog.csdn.net/20150305143454974?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdGlhbmxlc29mdHdhcmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里的259 就是我们删掉的数据文件。
//直接cp该句柄文件名回原位置：
[oracle@dg1 fd]$ cp 259/u01/dropspace01.dbf
[oracle@dg1 fd]$

因为数据库一直是open的，那么SCN也会不断的变化，我们cp出来的数据文件和数据库当前的信息不一致，所以我们需要进行recover：
SQL> alter database datafile'/u01/dropspace01.dbf' offline;
Database altered.
SQL> recover datafile'/u01/dropspace01.dbf';
Media recovery complete.
SQL>alter database datafile'/u01/dropspace01.dbf' online;
Database altered.
SQL>
恢复正常。
//文件存在：
[oracle@dg1 u01]$ ls -la/u01/dropspace01.dbf
-rw-r----- 1 oracle oinstall 104865792 Aug27 21:41 /u01/dropspace01.dbf
//重启数据库：
SQL> shutdown immediate
Database closed.
Database dismounted.
ORACLE instance shut down.
SQL> startup
ORACLE instance started.
Total System Global Area  814227456 bytes
Fixed Size        2232760 bytes
Variable Size       490737224 bytes
Database Buffers    318767104 bytes
Redo Buffers          2490368 bytes
Database mounted.
Database opened.
SQL>
也正常。 这里有2个注意的问题：数据库是归档模式，数据库或者操作系统没有重启。这2点非常关键。 也正式如此，才让操作比较简单。
如果是非归档模式，那就要复杂很多了。
在非归档模式下，如果删除了数据文件，并且又触发了CKPT，那么CKPT 会直接把整个实例中断掉，也就是说，如果是比较繁忙的数据库，如果误删除数据文件，实例可能会中断，一旦实例中断，那么用之前讲的通过句柄恢复就没有可能性了。
当然也有另一种可能性，就是删除数据文件之后，可以先通过句柄恢复，然后用expdp导出数据，尽可能的挽救部分数据。 这个动作就是与时间赛跑的过程了。
总之生产环境，操作一定要小心，还有要开归档，除非数据允许丢失。



--------------------------------------------------------------------------------------------
版权所有，文章禁止转载，否则追究法律责任!
**AboutDave:**
--------------------------------------------------------------------------------------------
QQ:492913789
Email:ahdba@qq.com
Blog:http://www.cndba.cn/dave
Weibo:[http://weibo.com/tianlesoftware](http://weibo.com/tianlesoftware)
Twitter:[http://twitter.com/tianlesoftware](http://twitter.com/tianlesoftware)
Facebook:[http://www.facebook.com/tianlesoftware](http://www.facebook.com/tianlesoftware)
Linkedin:[http://cn.linkedin.com/in/tianlesoftware](http://cn.linkedin.com/in/tianlesoftware)
**Dave 的QQ群：**
--------------------------------------------------------------------------------------------
注意:加群必须注明表空间和数据文件关系 | 不要重复加群
CNDBA_1: 62697850 (空)    CNDBA_2: 62697716 (满)  CNDBA_3: 283816689
CNDBA_4: 391125754   CNDBA_5:104207940    CNDBA_6: 62697977   CNDBA_7: 142216823（满）

