# ORA-12519: TNS:no appropriate service handler found 解决 - z69183787的专栏 - CSDN博客
2013年01月25日 10:36:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：14908
必须在在 sqlplus中  输入  
connect /nolog 
Connect sys/Wonders2012 as sysdba
有时候连得上数据库,有时候又连不上.
可能是数据库上当前的连接数目已经超过了它能够处理的最大值.
select count(*) from v$process --当前的连接数
select value from v$parameter where name = 'processes' --数据库允许的最大连接数
修改最大连接数:
alter system set processes = 300 scope = spfile;
重启数据库:
shutdown immediate;
startup;
--查看当前有哪些用户正在使用数据
SELECT osuser, a.username,cpu_time/executions/1000000||'s', sql_fulltext,machine 
from v$session a, v$sqlarea b
where a.sql_address =b.address order by cpu_time/executions desc;

    这几天碰到系统不能登陆的情况，初步判断可能是数据库连接满了，（后来检查不是这个原因），做了一次增加数据库最大连接数操作。操作步骤如下
     操作系统：SUSE linux11  数据库oracle10g
1，查看当前会话数
SQL> conn / as sysdba
已连接。
SQL> select count(*) from v＄session;
  COUNT(*)
----------
        45
注：session值=processes值*1.1+5
2,查看数据库最大进程数
SQL> show parameter processes
NAME                               TYPE        VALUE
---------------------------------- ----------- ------------
aq_tm_processes                    integer     0
db_writer_processes                integer     1
gcs_server_processes               integer     0
job_queue_processes                integer     10
log_archive_max_processes          integer     2
processes                          integer     50
3，查看数据库最大会话数
SQL> show parameter sessions
NAME                               TYPE        VALUE
---------------------------------- ----------- ------------
java_max_sessionspace_size         integer     0
java_soft_sessionspace_limit       integer     0
license_max_sessions               integer     0
license_sessions_warning           integer     0
logmnr_max_persistent_sessions     integer     1
sessions                           integer     60
shared_server_sessions             integer
4，修改processes和sessions值
SQL> alter system set processes=300 scope=spfile; 
系统已更改。
SQL> alter system set sessions=300 scope=spfile;
系统已更改。
注：一般只执行第一个命令就可以，session自动更改
5，创建pfile
SQL>create pfile from spfile;
注：如果不做此步，可能会导致更改最大连接数之后数据库不能启动现象
6，重启数据库，使更改生效
SQL> shutdown immediate
SQL> startup
注：shutdown normal正常关闭数据库可能会相当相当的慢呢
    shutdown abort 可能会导致启动数据库时很慢。
7，查看进程数，会话数，已经修改过来了
SQL> show parameter processes
NAME                                 TYPE        VALUE
------------------------------------ ----------- -----------
aq_tm_processes                      integer     0
db_writer_processes                  integer     1
gcs_server_processes                 integer     0
job_queue_processes                  integer     10
log_archive_max_processes            integer     2
processes                            integer     300
SQL> show parameter sessions
NAME                                 TYPE        VALUE
------------------------------------ ----------- ------------
java_max_sessionspace_size           integer     0
java_soft_sessionspace_limit         integer     0
license_max_sessions                 integer     0
license_sessions_warning             integer     0
logmnr_max_persistent_sessions       integer     1
sessions                             integer     335
shared_server_sessions               integer
PS：中间因为没有执行第5步，导致数据库无法启动
    通过使用startup pfile＝'....pfile/init.ora.11320060487'路径为绝对路径。
    然后执行create spfile from pfile;数据库就OK了，但是发现前面做的更改没了。
    然后重新修改连接数，记得执行第5步，完成。
