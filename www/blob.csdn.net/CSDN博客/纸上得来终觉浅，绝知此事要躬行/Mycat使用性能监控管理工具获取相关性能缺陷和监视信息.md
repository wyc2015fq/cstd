# Mycat使用性能监控管理工具获取相关性能缺陷和监视信息 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年09月15日 17:53:10[boonya](https://me.csdn.net/boonya)阅读数：935








Mycat性能监控测试管理工具有3种：1、Mycat控制台命令行；2、Mycat-web性能管理系统；3、第三方监控工具如Spotlight。虽然有了性能监视工具，但是实际的问题也不能完全分析出来，还是要在实战中积累经验。如果使用Mycat进行二次开发建议持续关注github上最新的issue来观察，同时加强对Mycat源码的阅读和理解。

## Mycat控制台命令行监控

进入mysql控制台命令：mysql -h10.10.10.117  -P9066 -ucvnavidb -pXJAKNEBCYlHefj7YHdi0 cvnavidb

参数备注：-h:host, -u:username,  -p:password,  -P:port,    cvnavidb:database instance




```
[root@localhost lib]# mysql -h10.10.10.117  -P9066 -ucvnavidb -pXJAKNEBCYlHefj7YHdi0 cvnavidb
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 632
Server version: 5.6.29-mycat-1.6-RELEASE-20170915144602 MyCat Server (monitor)

Copyright (c) 2000, 2013, Oracle and/or its affiliates. All rights reserved.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

mysql> SELECT F_ID, F_PID FROM t_fleet WHERE F_DEL_FLAG = 0;
[2]+  Stopped                 mysql -h10.10.10.117 -P9066 -ucvnavidb -pXJAKNEBCYlHefj7YHdi0 cvnavidb
[root@localhost lib]# mysql -h10.10.10.117  -P9066 -ucvnavidb -pXJAKNEBCYlHefj7YHdi0 cvnavidb
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 636
Server version: 5.6.29-mycat-1.6-RELEASE-20170915144602 MyCat Server (monitor)

Copyright (c) 2000, 2013, Oracle and/or its affiliates. All rights reserved.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

mysql> show @@help
    -> ;
+------------------------------------------+--------------------------------------------+
| STATEMENT                                | DESCRIPTION                                |
+------------------------------------------+--------------------------------------------+
| show @@time.current                      | Report current timestamp                   |
| show @@time.startup                      | Report startup timestamp                   |
| show @@version                           | Report Mycat Server version                |
| show @@server                            | Report server status                       |
| show @@threadpool                        | Report threadPool status                   |
| show @@database                          | Report databases                           |
| show @@datanode                          | Report dataNodes                           |
| show @@datanode where schema = ?         | Report dataNodes                           |
| show @@datasource                        | Report dataSources                         |
| show @@datasource where dataNode = ?     | Report dataSources                         |
| show @@datasource.synstatus              | Report datasource data synchronous         |
| show @@datasource.syndetail where name=? | Report datasource data synchronous detail  |
| show @@datasource.cluster                | Report datasource galary cluster variables |
| show @@processor                         | Report processor status                    |
| show @@command                           | Report commands status                     |
| show @@connection                        | Report connection status                   |
| show @@cache                             | Report system cache usage                  |
| show @@backend                           | Report backend connection status           |
| show @@session                           | Report front session details               |
| show @@connection.sql                    | Report connection sql                      |
| show @@sql.execute                       | Report execute status                      |
| show @@sql.detail where id = ?           | Report execute detail status               |
| show @@sql                               | Report SQL list                            |
| show @@sql.high                          | Report Hight Frequency SQL                 |
| show @@sql.slow                          | Report slow SQL                            |
| show @@sql.resultset                     | Report BIG RESULTSET SQL                   |
| show @@sql.sum                           | Report  User RW Stat                       |
| show @@sql.sum.user                      | Report  User RW Stat                       |
| show @@sql.sum.table                     | Report  Table RW Stat                      |
| show @@parser                            | Report parser status                       |
| show @@router                            | Report router status                       |
| show @@heartbeat                         | Report heartbeat status                    |
| show @@heartbeat.detail where name=?     | Report heartbeat current detail            |
| show @@slow where schema = ?             | Report schema slow sql                     |
| show @@slow where datanode = ?           | Report datanode slow sql                   |
| show @@sysparam                          | Report system param                        |
| show @@syslog limit=?                    | Report system mycat.log                    |
| show @@white                             | show mycat white host                      |
| show @@white.set=?,?                     | set mycat white host,[ip,user]             |
| show @@directmemory=1 or 2               | show mycat direct memory usage             |
| switch @@datasource name:index           | Switch dataSource                          |
| kill @@connection id1,id2,...            | Kill the specified connections             |
| stop @@heartbeat name:time               | Pause dataNode heartbeat                   |
| reload @@config                          | Reload basic config from file              |
| reload @@config_all                      | Reload all config from file                |
| reload @@route                           | Reload route config from file              |
| reload @@user                            | Reload user config from file               |
| reload @@sqlslow=                        | Set Slow SQL Time(ms)                      |
| reload @@user_stat                       | Reset show @@sql  @@sql.sum @@sql.slow     |
| rollback @@config                        | Rollback all config from memory            |
| rollback @@route                         | Rollback route config from memory          |
| rollback @@user                          | Rollback user config from memory           |
| reload @@sqlstat=open                    | Open real-time sql stat analyzer           |
| reload @@sqlstat=close                   | Close real-time sql stat analyzer          |
| offline                                  | Change MyCat status to OFF                 |
| online                                   | Change MyCat status to ON                  |
| clear @@slow where schema = ?            | Clear slow sql by schema                   |
| clear @@slow where datanode = ?          | Clear slow sql by datanode                 |
+------------------------------------------+--------------------------------------------+
58 rows in set (0.00 sec)

mysql> show @@processor;
+------------+------------+-----------+-------------+---------+---------+--------------+--------------+------------+----------+----------+----------+
| NAME       | NET_IN     | NET_OUT   | REACT_COUNT | R_QUEUE | W_QUEUE | FREE_BUFFER  | TOTAL_BUFFER | BU_PERCENT | BU_WARNS | FC_COUNT | BC_COUNT |
+------------+------------+-----------+-------------+---------+---------+--------------+--------------+------------+----------+----------+----------+
| Processor0 |  289440957 | 261472660 |           0 |       0 |       0 | 687194767360 | 687194767360 |          0 |        0 |        5 |       12 |
| Processor1 | 3329876701 | 390589667 |           0 |       0 |       0 | 687194767360 | 687194767360 |          0 |        0 |        7 |        9 |
| Processor2 |  385567819 | 298165916 |           0 |       0 |       0 | 687194767360 | 687194767360 |          0 |        0 |        8 |        9 |
| Processor3 |  128281110 | 167211713 |           0 |       0 |       0 | 687194767360 | 687194767360 |          0 |        0 |        5 |        6 |
+------------+------------+-----------+-------------+---------+---------+--------------+--------------+------------+----------+----------+----------+
4 rows in set (0.00 sec)

mysql> show @@connection;
+------------+------+--------------+------+------------+----------+----------+----------+------------+----------+---------------+-------------+------------+---------+------------+
| PROCESSOR  | ID   | HOST         | PORT | LOCAL_PORT | USER     | SCHEMA   | CHARSET  | NET_IN     | NET_OUT  | ALIVE_TIME(S) | RECV_BUFFER | SEND_QUEUE | txlevel | autocommit |
+------------+------+--------------+------+------------+----------+----------+----------+------------+----------+---------------+-------------+------------+---------+------------+
| Processor0 |  352 | 10.10.10.126 | 8066 |      51330 | cvnavidb | cvnavidb | utf8:33  |    2193778 | 77298070 |          3336 |       16384 |          0 | 3       | true       |
| Processor0 |   33 | 10.10.10.117 | 9066 |      54392 | cvnavidb | cvnavidb | utf8:33  |       3104 |  4589817 |          7662 |        4096 |          0 |         |            |
| Processor0 |    5 | 10.10.11.103 | 8066 |      10239 | cvnavidb | cvnavidb | utf8:33  |    3299363 |   144452 |          7693 |        4096 |          0 | 3       | true       |
| Processor0 |  533 | 10.10.11.103 | 8066 |      13583 | cvnavidb | cvnavidb | utf8:33  |        462 |    17236 |          1268 |        4096 |          0 | 3       | true       |
| Processor0 |  629 | 10.10.12.164 | 8066 |      54339 | cvnavidb | cvnavidb | utf8:33  |       1104 |     1257 |            91 |        4096 |          0 | 3       | true       |
| Processor1 |  626 | 10.10.12.164 | 8066 |      54336 | cvnavidb | cvnavidb | utf8:33  |       1104 |     1257 |            92 |        4096 |          0 | 3       | true       |
| Processor1 |    6 | 10.10.11.103 | 8066 |      10244 | cvnavidb | cvnavidb | utf8:33  | 3063195860 |  2479271 |          7692 |       90112 |          0 | 3       | true       |
| Processor1 |  630 | 10.10.12.164 | 8066 |      54342 | cvnavidb | cvnavidb | utf8:33  |       4508 |   585553 |            91 |        4096 |          0 | 3       | true       |
| Processor1 |  568 | 10.10.10.126 | 8066 |      39111 | cvnavidb | cvnavidb | utf8:33  |    2024531 | 13637493 |           787 |       16384 |          0 | 3       | true       |
| Processor1 |  632 | 10.10.10.117 | 9066 |      56815 | cvnavidb | cvnavidb | latin1:5 |        147 |      297 |            62 |        4096 |          0 |         |            |
| Processor1 |  458 | 10.10.10.126 | 8066 |      58989 | cvnavidb | cvnavidb | utf8:33  |     708555 |   823446 |          2107 |       16384 |          0 | 3       | true       |
| Processor1 |  636 | 10.10.10.117 | 9066 |      56829 | cvnavidb | cvnavidb | latin1:5 |        206 |     4066 |            53 |        4096 |          0 |         |            |
| Processor2 |  369 | 10.10.10.126 | 8066 |      52497 | cvnavidb | cvnavidb | utf8:33  |    1808708 | 39558581 |          3153 |       16384 |          0 | 3       | true       |
| Processor2 |  627 | 10.10.12.164 | 8066 |      54337 | cvnavidb | cvnavidb | utf8:33  |       1104 |     1257 |            92 |        4096 |          0 | 3       | true       |
| Processor2 |    7 | 10.10.11.103 | 8066 |      10246 | cvnavidb | cvnavidb | utf8:33  |    3160327 |   312504 |          7692 |       20480 |          0 | 3       | true       |
| Processor2 |   55 | 10.10.10.117 | 9066 |      54462 | cvnavidb | cvnavidb | utf8:33  |       3243 |  7603678 |          7362 |        4096 |          0 |         |            |
| Processor2 |  615 | 10.10.10.117 | 9066 |      56761 | cvnavidb | cvnavidb | latin1:5 |        221 |   103919 |           190 |        4096 |          0 |         |            |
| Processor2 |  600 | 10.10.10.126 | 8066 |      41762 | cvnavidb | cvnavidb | utf8:33  |    1789287 |  1632953 |           374 |        8192 |          0 | 3       | true       |
| Processor2 |   10 | 10.10.10.1   | 8066 |      65440 | cvnavidb | cvnavidb | latin1:5 |      80079 |  8421259 |          7685 |        4096 |          0 | 3       | true       |
| Processor2 |   31 | 10.10.10.117 | 9066 |      54390 | cvnavidb | cvnavidb | utf8:33  |       3797 | 13870810 |          7662 |        4096 |          0 |         |            |
| Processor3 |   32 | 10.10.10.117 | 9066 |      54391 | cvnavidb | cvnavidb | utf8:33  |       3400 |  8746466 |          7662 |        4096 |          0 |         |            |
| Processor3 |  355 | 10.10.10.126 | 8066 |      51333 | cvnavidb | cvnavidb | utf8:33  |    1785253 | 34577828 |          3336 |       16384 |          0 | 3       | true       |
| Processor3 |  628 | 10.10.12.164 | 8066 |      54338 | cvnavidb | cvnavidb | utf8:33  |       1104 |     1257 |            91 |        4096 |          0 | 3       | true       |
| Processor3 |  119 | 10.10.10.117 | 9066 |      54776 | cvnavidb | cvnavidb | utf8:33  |       2748 |  1757148 |          6282 |        4096 |          0 |         |            |
| Processor3 |  601 | 10.10.10.126 | 8066 |      41787 | cvnavidb | cvnavidb | utf8:33  |    1688404 |  1236539 |           371 |        4096 |          0 | 3       | true       |
+------------+------+--------------+------+------------+----------+----------+----------+------------+----------+---------------+-------------+------------+---------+------------+
25 rows in set (0.00 sec)

mysql> show @@cache;
+-------------------------------------+-------+------+--------+------+------+---------------+---------------+
| CACHE                               | MAX   | CUR  | ACCESS | HIT  | PUT  | LAST_ACCESS   | LAST_PUT      |
+-------------------------------------+-------+------+--------+------+------+---------------+---------------+
| ER_SQL2PARENTID                     |  1000 |    0 |      0 |    0 |    0 |             0 |             0 |
| SQLRouteCache                       | 10000 |   11 | 416891 |   11 |   33 | 1505465985643 | 1505465352022 |
| TableID2DataNodeCache.TESTDB_ORDERS | 50000 |    0 |      0 |    0 |    0 |             0 |             0 |
+-------------------------------------+-------+------+--------+------+------+---------------+---------------+
3 rows in set (0.00 sec)

mysql> show @@server;
+------------+-------------+--------------+------------+---------------+---------------+---------+--------+
| UPTIME     | USED_MEMORY | TOTAL_MEMORY | MAX_MEMORY | RELOAD_TIME   | ROLLBACK_TIME | CHARSET | STATUS |
+------------+-------------+--------------+------------+---------------+---------------+---------+--------+
| 2h 8m 53s  |   230734320 |   1069023232 | 3817865216 | 1505458275142 |            -1 | latin1  | ON     |
+------------+-------------+--------------+------------+---------------+---------------+---------+--------+
1 row in set (0.00 sec)

mysql> show @@version;
+-----------------------------------------+
| VERSION                                 |
+-----------------------------------------+
| 5.6.29-mycat-1.6-RELEASE-20170915144602 |
+-----------------------------------------+
1 row in set (0.00 sec)

mysql>
```

参考地址：[http://www.cnblogs.com/tonylovett/p/5251449.html](http://www.cnblogs.com/tonylovett/p/5251449.html)



## Mycat-web性能管理系统

![](https://img-blog.csdn.net/20170915174504796)

参考：[http://blog.csdn.net/mchdba/article/details/51059547](http://blog.csdn.net/mchdba/article/details/51059547)


## 第三方监控工具如Spotlight

![](https://img-blog.csdn.net/20170915175022129)

此工具是商业工具，可以下载破解版进行使用，或官网（[http://www.quest.com/](http://www.quest.com/)）下载试用。




