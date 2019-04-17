# Mysql 存储引擎 - 在思索中前行！ - CSDN博客





2014年06月19日 10:09:44[_Tham](https://me.csdn.net/txl16211)阅读数：538标签：[mysql																[存储引擎](https://so.csdn.net/so/search/s.do?q=存储引擎&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
个人分类：[Database mysql](https://blog.csdn.net/txl16211/article/category/2333871)








mysql存储引擎：

MySQL服务器采用了模块化风格，各部分之间保持相对独立，尤其体现在存储架构上。存储引擎负责管理数据存储，以及MySQL的索引管理。通过定义的API，MySQL服务器能够与存储引擎进行通信。目前使用最多的是MyISAM和InnoDB。InnoDB被Oracle收购后，MySQL自行开发的新存储引擎Falcon将在MySQL6.0版本引进。

MyISAM引擎是一种非事务性的引擎，提供高速存储和检索，以及全文搜索能力，适合数据仓库等查询频繁的应用。MyISAM中，一个table实际保存为三个文件，.frm存储表定义，.MYD存储数据,.MYI存储索引。

InnoDB则是一种支持事务的引擎。所以的数据存储在一个或者多个数据文件中，支持类似于Oracle的锁机制。一般在OLTP（[联机事务处理](http://baike.baidu.com/view/8028.htm)系统(OLTP)也称为面向交易的处理系统，其基本特征是顾客的[原始数据](http://baike.baidu.com/view/3364695.htm)可以立即传送到[计算中心](http://baike.baidu.com/view/907824.htm)进行处理，并在很短的时间内给出处理结果。）应用中使用较广泛。如果没有指定InnoDB配置选项，MySQL将在MySQL数据目录下创建一个名为ibdata1的自动扩展数据文件，以及两个名为ib_logfile0和ib_logfile1的日志文件。

创建table时可以通过engine关键字指定使用的存储引擎，如果省略则使用系统默认的存储引擎：

CREATE TABLE t (i INT) ENGINE = MYISAM;

查看系统中支持的存储引擎类型：



|`1`|```mysql> show engines;| Engine | Support | Comment || MyISAM | YES | Default engine as of MySQL 3.23 with      greatperformance |```|
|----|----|


|`2`|```| MEMORY | YES | Hash based, stored in memory, useful for temporarytables |```|
|----|----|


|`3`|```| InnoDB | DEFAULT | Supports transactions, row-level locking, andforeign keys |```|
|----|----|


|`4`|```| BerkeleyDB | NO | Supports transactions and page-level locking|```|
|----|----|


|`5`|```| BLACKHOLE | NO | /dev/null storage engine (anything you write toit disappears) |```|
|----|----|


|`6`|```| EXAMPLE | NO | Example storage engine |```|
|----|----|


|`7`|```| ARCHIVE | YES | Archive storage engine |```|
|----|----|


|`8`|```| CSV | NO | CSV storage engine |```|
|----|----|


|`9`|```| ndbcluster | NO | Clustered, fault-tolerant, memory-based tables|```|
|----|----|


|`10`|```| FEDERATED | NO | Federated MySQL storage engine |```|
|----|----|


|`11`|```| MRG_MYISAM | YES | Collection of identical MyISAM tables |```|
|----|----|


|`12`|```| ISAM | NO | Obsolete storage engine |12 rows in set (0.00 sec)```|
|----|----|




标准安装程序中只提供部分引擎的支持，如果需要使用其他的存储引擎，需要使用源代码加不同的参数重新编译。其中DEFAULT表明系统的默认存储引擎，可以通过修改配置参数来变更：

default-storage-engine=MyISAM

查看某个存储引擎的具体信息



|`1`|```mysql> show engine InnoDB status\G;```|
|----|----|




一般安装系统默认是INNODB

default-storage-engine=INNODB

　　1.可以在启动数据库服务器时在命令行后面加上–default-storage-engine或–default-table-type选项。

　　2.更灵活的方式是在随MySQL服务器发布同时提供的MySQL客户端时指定使用的存储引擎。最直接的方式是在创建表时指定存储引擎的类型，向下面这样:



|`1`|```CREATE TABLE mytable (id int, titlechar(20)) ENGINE = INNODB```|
|----|----|




修改表的存储引擎：



|`1`|```ALTER TABLE engineTest ENGINE = INNODB；```|
|----|----|




修改默认存储引擎：

在mysql配置文件（linux下为/etc/my.cnf），在mysqld后面增加default-storage-engine=INNODB即可。

但是如果表建立的时候是MyISAM，要更改整个数据库表的存储引擎，一般要一个表一个表的修改，比较繁琐，可以采用先把数据库导出，得到SQL，把MyISAM修改成INNODB，再导入的方式。




