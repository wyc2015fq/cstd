# MySql分支项目MariaDB 之开眼看世界 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月06日 21:37:57[boonya](https://me.csdn.net/boonya)阅读数：912








## MariaDB基础介绍

![MariaDB](https://downloads.mariadb.org/static/generated/images/v2/MariaDB-Foundation-500x500-1cc54aef598101656d5c5697a6fdde7e9f92624a.png)


MariaDB数据库管理系统是MySQL的一个分支，主要由开源社区在维护，采用GPL授权许可 MariaDB的目的是完全兼容MySQL，包括API和命令行，使之能轻松成为MySQL的代替品。在存储引擎方面，使用XtraDB（英语：XtraDB）来代替MySQL的InnoDB。 MariaDB由MySQL的创始人Michael Widenius（英语：Michael Widenius）主导开发，他早前曾以10亿美元的价格，将自己创建的公司MySQL AB卖给了SUN，此后，随着SUN被甲骨文收购，MySQL的所有权也落入Oracle的手中。MariaDB名称来自Michael
 Widenius的女儿Maria的名字。MariaDB基于事务的Maria存储引擎，替换了MySQL的MyISAM存储引擎，它使用了Percona的 XtraDB，InnoDB的变体，分支的开发者希望提供访问即将到来的MySQL 5.4 InnoDB性能。这个版本还包括了 PrimeBase XT (PBXT) 和 FederatedX存储引擎。






官方地址：[http://mariadb.org/](http://mariadb.org/)



官方下载：[https://downloads.mariadb.org/](https://downloads.mariadb.org/)






## 版本/mariaDB 



MariaDB直到5.5版本，均依照MySQL的版本。因此，使用MariaDB5.5的人会从MySQL 5.5中了解到MariaDB的所有功能。

从2012年11月12日起发布的10.0.0版开始，不再依照MySQL的版号。10.0.x版以5.5版为基础，加上移植自MySQL 5.6版的功能和自行开发的新功能。


## 发展/mariaDB 


![mariaDB](http://a0.att.hudong.com/71/98/01100000000000144721985913320_s.jpg)**mariaDB**

成立于2009年，MySQL之父Michael “Monty” Widenius用他的新项目MariaDB完成了对MySQL的“反戈一击”。开发这个分支的原因之一是：甲骨文公司收购了MySQL后，有将MySQL闭源的潜在风险，因此社区采用分支的方式来避开这个风险。 过去一年中，大型互联网用户以及Linux发行商纷纷抛弃MySQL，转投MariaDB阵营。MariaDB是目前最受关注的MySQL数据库衍生版，也被视为开源数据库MySQL的替代品。

MariaDB虽然被视为MySQL数据库的替代品，但它在扩展功能、存储引擎以及一些新的功能改进方面都强过MySQL。而且从MySQL迁移到MariaDB也是非常简单的：

1、数据和表定义文件（.frm）是二进制兼容的

2、所有客户端API、协议和结构都是完全一致的

3、所有文件名、二进制、路径、端口等都是一致的

4、所有的MySQL连接器，比如PHP、Perl、Python、Java、.NET、MyODBC、Ruby以及MySQL C connector等在MariaDB中都保持不变

5、mysql-client包在MariaDB服务器中也能够正常运行

6、共享的客户端库与MySQL也是二进制兼容的

也就是说，在大多数情况下，你完全可以卸载MySQL然后安装MariaDB，然后就可以像之前一样正常的运行。


## 起源/mariaDB 


为何改了个名字呢，这其中是有些典故的。

MySQL之父Widenius先生离开了Sun之后，觉得依靠Sun/Oracle来发展MySQL，实在很不靠谱，于是决定另开分支，这个分支的名字叫做MariaDB。

MariaDB跟MySQL在绝大多数方面是兼容的，对于开发者来说，几乎感觉不到任何不同。目前MariaDB是发展最快的MySQL分支版本，新版本发布速度已经超过了Oracle官方的MySQL版本。

在Oracle控制下的MySQL开发，有两个主要问题：1. MySQL核心开发团队是封闭的，完全没有Oracle之外的成员参加。很多高手即使有心做贡献，也没办法做到。2. MySQL新版本的发布速度，在Oracle收购Sun之后大为减缓。Widenius有一个ppt，用数据比较了收购之前和之后新版本的发布速度。有很多bugfix和新的feature，都没有及时加入到发布版本之中。

以上这两个问题，导致了各个大公司，都开发了自己定制的MySQL版本，包括Yahoo!/Facebook/Google/阿里巴巴+淘宝网等等。

MySQL是开源社区的资产，任何个人/组织都无权据为己有。为了依靠广大MySQL社区的力量来更快速的发展MySQL，另外开分支是必须的。

MariaDB默认的存储引擎是Maria，不是MyISAM。Maria可以支持事务，但是默认情况下没有打开事务支持，因为事务支持对性能会有影响。可以通过以下语句，转换为支持事务的Maria引擎。ALTER TABLE `tablename` ENGINE=MARIA TRANSACTIONAL=1;


## 特点/mariaDB 


![MariaDB相关书籍](http://a4.att.hudong.com/16/98/01100000000000144721985966813_s.jpg)**MariaDB相关书籍**

MariaDB 是一个采用 Maria 存储引擎的MySQL分支版本，是由原来 MySQL 的作者Michael Widenius创办的公司所开发的免费开源的数据库服务器。

这个项目的很多代码都改编于 MySQL 6.0，例如 “pool of threads”功能提供解决多数据连接问题。MariaDB 5.1.41 RC可以到这里下载，32位和64位已编译Linux版本，还包括源代码包。MariaDB基于GPL 2.0发布。

与 MySQL 相比较，MariaDB 更强的地方在于：

Maria 存储引擎

PBXT 存储引擎

XtraDB 存储引擎

FederatedX 存储引擎

更快的复制查询处理

线程池

更少的警告和bug

运行速度更快

更多的 Extensions (More index parts, new startup options etc)

更好的功能测试

数据表消除

慢查询日志的扩展统计

支持对 Unicode 的排序

相对于MySQL最新的版本5.6来说，在性能、功能、管理、NoSQL扩展方面，MariaDB包含了更丰富的特性。比如微秒的支持、线程池、子查询优化、组提交、进度报告等。详情见列表。
|High Performance|Developers|DBAs|NoSQL|
|----|----|----|----|
|Thread pool 线程池|Microsecond precision & type 微秒支持|Segmented MyISAM keycache MyISAM缓存分段|HandleSocket 直接InnoDB/XtrDB访问|
|Group commit for the binary log binlog组提交|SphinxSE for full-text search SphinxSE全文索引支持|Authentication plugins - PAM, Active Directory 授权插件|Dynamic columns 动态列|
|Non-blocking client library 非阻塞库|Subqueries materialize 子查询优化|LIMIT ROWS EXAMINED LIMIT行检查限制||
||GIS functionality 地理信息系统支持|Progress reporting 进度报告||


## 第三方工具/mariaDB


MariaDB

的API和协议兼容MySQL，另外又添加了一些功能，以支持本地的非阻塞操作和进度报告。这意味着，所有使用MySQL的连接器、库和应用程序也将会在MariaDB下工作。在此基础上，由于担心甲骨文MySQL的一个更加封闭的软件项目，Fedora的计划在Fedora 19中的以MariaDB取代MySQL

，维基媒体基金会的服务器同样也使用MariaDB取代了MySQL。

如下是支持MariaDB的客户端工具：

DBEdit 一个免费的MariaDB数据库和其他数据库管理应用程序。

Navicat 一系列Windows、Mac OS X、Linux下专有数据库管理应用程序。

HeidiSQL 一个Windows上自由和开放源码的MySQL客户端。它支持MariaDB的5.2.7版本和以后的版本。

phpMyAdmin 一个基于网络的MySQL数据库管理应用程序


## 兼容性/mariaDB


MariaDB是MySQL的二进制替代品

出于实用的目的，MariaDB是同一MySQL版本的二进制替代品(例如MySQL 5.1->MariaDB 5.1, MariaDB5.2和MariaDB 5.3是兼容的。MySQL 5.5将会和MariaDB 5.5保持兼容)。这意味着：

数据和表定义文件(.frm)文件是二进制兼容的。

所有客户端APIs，协议和结构都是相同的。

所有的文件名、二进制文件的路径、端口、套接字等等……应该是相同的。

所有MySQL的连接器(PHP Python Perl Java .NET MyODBC Ruby MySQL C连接器等) 和MariaDB的不变。

mysql-client包还可以与MariaDB服务器一起工作。

这意味着对于大多数情况下，您可以卸载MySQL和安装MariaDB，可以工作很好。(不需要转换成任何数据文件，如果使用同一主版本,比如5.1)。

我们每月会与MySQL代码库合并来确保我们的兼容性和得到任何Oracle修正的bug和所有特性。

我们在脚本升级方面也做了大量的工作，从MySQL 5.0升级到MariaDB 5.1比从MySQL 5.0到MySQL 5.1更容易。

这就是说：MariaDB有许多的新选项、扩展，存储引擎和bug修复,而MySQL是没有的。您可以在MariaDB分发版本差异页面找到不同版本的功能特性集。可在MariaDB versus MySQL – Features看到。

MariaDB 5.1与MySQL 5.1的不兼容性

为了让MariaDB比MySQL提供更多更好的信息，极少情况下MariaDB会出现不兼容情况。

如果使用MariaDB 5.1替代MySQL5.1，您将看到如下列举的全部已知用户级不兼容情况：

安装的包名称，用MariaDB代替MySQL。

时间控制可能不同，MariaDB在许多情况下比MySQL快。

mysqld在MariaDB读取是my.cnf中[MariaDB ]的部分。

如果它不是完全相同的MariaDB编译版本，不能使用仅提供二进制的存储引擎库给MariaDB使用 (这是因为服务器内部结构THD在MySQL和MariaDB之间不同。这也是与常见的MySQL版本不同的)。这应该不是问题，因为对于大多数人不加载新存储引擎，MariaDB比MySQL带有更多的存储引擎。

CHECKSUM TABLE可能产生不同的结果，由于MariaDB并不忽视NULL的列，MySQL 5.1忽略(未来的MySQL版本应该计算checksums和MariaDB一样)。在MariaDB 开启mysqld –old选项，您可以得到“旧式”的校验和。但是要注意,这个MyISAM存储引擎和Aria 存储引擎在MariaDB实际上在内部使用新的checksum,因此,如果您使用的是—old , CHECKSUM命令将会更慢,因为它需要,一行一行的计算checksum。

慢速查询日志有更多信息关于查询, 如果有一个脚本解析慢速查询日志这可能是一个问题。

MariaDB默认情况下比MySQL需要更多的内存,因为我们有默认情况下启用Aria存储引擎处理内部临时表。如果需要MariaDB使用很少的内存(这是以牺牲性能为代价的),您可以设置aria_pagecache_buffer_size的值为 1M(默认值为128M)。

如果正在使用MariaDB的新命令选项，新特性或新存储引擎，那么就不能在MySQL和MariaDB之间轻易来回切换了。

MariaDB 5.2与MySQL 5.1的不兼容性

同MariaDB 5.1和MySQL 5.1的一样，再补充一条：

新增SQL_MODE的取值：IGNORE_BAD_TABLE_OPTIONS。如果未设置该值，使用一个表、字段或索引的属性(选项)不被支持的存储引擎将会导致错误。这个变化可能引起警告不正确的表定义出现在错误日志中，请利用mysql_upgrade修复这个警告。

实际上，MariaDB 5.2是MariaDB 5.1和MySQL 5.1替代品。

MariaDB 5.3与MySQL 5.1和MariaDB 5.2的不兼容性

一些错误信息涉及到错误转换的，MariaDB的差别在于消息中提供了更多的信息关于到底是哪里出了错。

MariaDB专用的错误编号已经从1900开始，为的是不与MySQL的错误冲突。

MariaDB在所有工作场景中可以基于微秒计时工作；而MySQL在某些情况下，如datetime和time，确实会丢失微秒部分。

旧的--maria启动选项被删除。应该使用--aria前缀代替。(MariaDB 5.2都支持这—maria-和- aria-)

SHOW PROCESSLIST有一个额外的process列，显示一些命令的进度。您可以启动mysqld用–old 标志禁用它。

INFORMATION_SCHEMA.PROCESSLIST对进度报告有三个新列: STAGE, MAX_STAGE和 PROGRESS。

以/*M! 或/*M!#####起头的长注释会得到执行。

如果使用max_user_connections = 0(也就是说任何数量的连接)启动mysqld,您就不能在mysqld运行时修改全局变量了。这是因为当mysqld以max_user_connections = 0启动时,它不分配计数结构(包括每个连接的互斥锁)。如果稍后改变变量，这将导致错误的计数器。如果希望在运行时能改变这个变量，请在启动时将它设置为一个较高的值。

可以设置max_user_connections(包括全局变量和GRANT选项两种场景)为-1来阻止用户连接到服务器。全局变量max_user_connections变量不影响拥有super特权的用户连接。

IGNORE指令并不会忽略所有错误(比如致命错误)，只是会忽略可以安全忽略的那些。

MariaDB 5.5与MariaDB 5.3的不兼容性

XtraDB

XtraDB提供者Percona，在5.5代码库并未提供XtraDB所有早些时候的功能特性。正因为如此,MariaDB 5.5也不能提供它们所有功能特性。

在5.5版本中XtraDB缺少的选项

以下选项XtraDB 5.5是不支持的。如果在my.cnf文件中使用到这些选项，请删除后再升级到5.5。

innodb-adaptive-checkpoint；请使用innodb_adaptive_flushing_method 替代。

innodb-auto-lru-dump；请使用innodb_buffer_pool_restore_at_startup 替代。

innodb-blocking-lru-restore；请使用innodb-blocking-buffer-pool-restore 替代。

innodb-enable-unsafe-group-commit

innodb-expand-import；请使用 innodb_import_table_from_xtrabackup 替代。

innodb-extra-rsegments；请使用innodb_rollback_segment 替代。

innodb-extra-undoslots

innodb-fast-recovery

innodb-flush-log-at-trx-commit-session

innodb-overwrite-relay-log-info

innodb-pass-corrupt-table；请使用innodb_corrupt_table_action 替代。

innodb-use-purge-thread

xtradb-enhancements


# 正在使用Maria DB的互联网企业

![](https://img-blog.csdn.net/20170306213700310)







