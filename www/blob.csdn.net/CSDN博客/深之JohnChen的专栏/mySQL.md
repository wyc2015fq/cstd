# mySQL - 深之JohnChen的专栏 - CSDN博客

2009年03月05日 14:03:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：4005


** mySQL**

MySQL名字的来历　　MySQL最初的开发者的意图是用mSQL和他们自己的快速低级例程(ISAM)去连接表格。不管怎样，在经过一些测试后，开发者得出结论：mSQL并没有他们需要的那么快和灵活。这导致了一个使用几乎和mSQL一样的API接口的用于他们的数据库的新的SQL接口的产生，这样，这个API被设计成允许为用于mSQL而写的第三方代码更容易移植到MySQL。

　　MySQL这个名字是怎么来的已经不清楚了。基本指南和大量的库和工具带有前缀“my”已经有10年以上，而且不管怎样，MySQL AB创始人之一的Monty Widenius的女儿也叫My。这两个到底是哪一个给出了MySQL这个名字至今依然是个迷，包括开发者在内也不知道。

　　MySQL的海豚标志的名字叫“sakila”，它是由MySQL AB的创始人从用户在“海豚命名”的竞赛中建议的大量的名字表中选出的。获胜的名字是由来自非洲斯威士兰的开源软件开发者Ambrose Twebaze提供。根据Ambrose所说，Sakila来自一种叫SiSwati的斯威士兰方言，也是在Ambrose的家乡乌干达附近的坦桑尼亚的Arusha的一个小镇的名字。

## MySQL的概述

　　MySQL是一个小型关系型数据库管理系统，开发者为瑞典MySQL AB公司。在2008年1月16号被Sun公司收购。目前MySQL被广泛地应用在Internet上的中小型网站中。由于其体积小、速度快、总体拥有成本低，尤其是开放源码这一特点，许多中小型网站为了降低网站总体拥有成本而选择了MySQL作为网站数据库。MySQL的官方网站的网址是：www.mysql.com

## MySQL的特性

　　1.使用C和C++编写，并使用了多种[编译器](view/487018.htm)进行测试，保证源代码的可移植性

　　2.支持AIX、[FreeBSD](view/21459.htm)、HP-UX、[Linux](view/1634.htm)、Mac OS、[Novell](view/68826.htm) Netware、OpenBSD、OS/2 Wrap、Solaris、Windows等多种操作系统

　　3.为多种编程语言提供了API。这些编程语言包括[C](view/10075.htm)、C++、Eiffel、Java、[Perl](view/46614.htm)、[PHP](view/99.htm)、Python、Ruby和Tcl等。

　　4.支持[多线程](view/65706.htm)，充分利用CPU资源

　　5.优化的[SQL](view/34.htm)查询算法，有效地提高查询速度

　　6.既能够作为一个单独的应用程序应用在客户端服务器网络环境中，也能够作为一个库而嵌入到其他的软件中提供多语言支持，常见的[编码](view/237708.htm)如中文的GB 2312、BIG5，日文的Shift_JIS等都可以用作数据表名和数据列名

　　7.提供TCP/IP、ODBC和JDBC等多种数据库连接途径

　　8.提供用于管理、检查、优化数据库操作的管理工具

　　9.可以处理拥有上千万条记录的大型数据库 

## MySQL的应用

　　与其他的大型数据库例如Oracle、DB2、SQL Server等相比，MySQL自有它的不足之处，如规模小、功能有限（MySQL Cluster的功能和效率都相对比较差）等，但是这丝毫也没有减少它受欢迎的程度。对于一般的个人使用者和中小型企业来说，MySQL提供的功能已经绰绰有余，而且由于MySQL是开放源码软件，因此可以大大降低总体拥有成本。

　　目前Internet上流行的网站构架方式是LAMP（Linux+Apache+MySQL+PHP），即使用Linux作为操作系统，Apache作为Web服务器，MySQL作为数据库，PHP作为服务器端脚本解释器。由于这四个软件都是遵循GPL的开放源码软件，因此使用这种方式不用花一分钱就可以建立起一个稳定、免费的网站系统。

## MySQL管理

　　可以使用命令行工具管理MySQL数据库（命令mysql 和 mysqladmin)，也可以从MySQL的网站下载图形管理工具MySQL Administrator和MySQL Query Browser。

　　phpMyAdmin是由php写成的MySQL资料库系统管理程式，让管理者可用Web介面管理MySQL资料库。

　　phpMyBackupPro也是由PHP写成的，可以透过Web介面创建和管理数据库。它可以创建伪cronjobs，可以用来自动在某个时间或周期备份MySQL 数据库。

　　另外，还有其他的GUI管理工具，例如早先的mysql-front 以及 ems mysql manager,navicat 等等。 

## Mysql存储引擎

[MyISAM](view/1238940.htm) Mysql的默认数据库，最为常用。拥有较高的插入，查询速度，但不支持事务

[InnoDB](view/1238935.htm) 事务型数据库的首选引擎，支持[ACID](view/600227.htm)事务，支持行级锁定

[BDB](view/1303712.htm) 源自Berkeley DB，事务型数据库的另一种选择，支持COMMIT和ROLLBACK等其他事务特性

[Memory](view/171611.htm) 所有数据置于内存的存储引擎，拥有极高的插入，更新和查询效率。但是会占用和数据量成正比的内存空间。并且其内容会在Mysql重新启动时丢失

　　Merge 将一定数量的MyISAM表联合而成一个整体，在超大规模数据存储时很有用

[Archive](view/150865.htm) 非常适合存储大量的独立的，作为历史记录的数据。因为它们不经常被读取。Archive拥有高效的插入速度，但其对查询的支持相对较差

　　Federated 将不同的Mysql服务器联合起来，逻辑上组成一个完整的数据库。非常适合分布式应用

　　Cluster/NDB 高冗余的存储引擎，用多台数据机器联合提供服务以提高整体性能和安全性。适合数据量大，安全和性能要求高的应用

[CSV](view/468993.htm) 逻辑上由逗号分割数据的存储引擎

　　BlackHole 黑洞引擎，写入的任何数据都会消失，一般用于记录binlog做复制的中继

　　另外，Mysql的存储引擎接口定义良好。有兴趣的开发者通过阅读文档编写自己的存储引擎。 

## Mysql最常见的应用架构

　　单点(Single)，适合小规模应用

　　复制(Replication)，适合中小规模应用

　　集群(Cluster)，适合大规模应用 

## mysql历史版本

　　MySQL公司目前在同时开发两个版本的软件，4.1版以及5.0版。4.1版本的代码已经发布并有望在8个月后公布最终代码。而5.0版本的最后产品将在6个月后发布。

　　MySQL4.1版本中增加了不少新的性能，包括对主键的更高速度的缓存，对子查询的更好的支持，以及应网络约会网站所要求的，基于地理信息的查询。

　　而其同步开发的5.0版本则把目标对准了企业用户，对于4.1版本中的所有新特性，5.0版本悉数收入囊中，并且独具以下特点：对外键的良好支持；系统自动报错机制以及对存储过程的充分支持。

　　Mysql现在现已开发出5.1版本支持视图！

## Mysql中文视频教学

　　左光华的mysql网络数据库开发教学视频http://www.tudou.com/playlist/mysql/

　　Mysql6.0的alpha版于2007年初发布，新版增加了对falcon存储引擎的支持。Falcon是Mysql社区自主开发的引擎，支持ACID特性事务，支持行锁，拥有高性能的并发性。Mysql AB公司想用Falcon替代已经非常流行的InnoDB引擎，因为拥有后者技术的InnoBase已经被竞争对手Oracle所收购。

　　2008年1月16日，[Sun](view/24856.htm) Microsystems宣布收购MySQL AB，出价约10亿美元现金包括期权。 http://www.mysql.com/news-and-events/sun-to-acquire-mysql.html

## MySQL的基本命令

**[ ]中的内容为可选项**

# --连接Mysql

格式： mysql -h主机地址 -u用户名 －p用户密码
**1、连接到本机上的MYSQL。**
首先打开DOS窗口，然后进入目录mysql\bin，再键入命令mysql -u root -p，回车后提示你输密码.注意用户名前可以有空格也可以没有空格，但是密码前必须没有空格，否则让你重新输入密码。
如果刚安装好MYSQL，超级用户root是没有密码的，故直接回车即可进入到MYSQL中了，MYSQL的提示符是： mysql>
**2、连接到远程主机上的MYSQL。**假设远程主机的IP为：110.110.110.110，用户名为root,密码为abcd123。则键入以下命令：
    mysql -h110.110.110.110 -u root -p 123;（注:u与root之间可以不用加空格，其它也一样）
**3、退出MYSQL命令**： exit （回车）

--修改密码

格式：mysqladmin -u用户名 -p旧密码 password 新密码
**1、给root加个密码ab12。**
首先在DOS下进入目录mysql\bin，然后键入以下命令
    mysqladmin -u root -password ab12
注：因为开始时root没有密码，所以-p旧密码一项就可以省略了。
**2、再将root的密码改为djg345。**
    mysqladmin -u root -p ab12 password djg345

--增加新用户

注意：和上面不同，下面的因为是MYSQL环境中的命令，所以后面都带一个分号作为命令结束符

格式：grant select on 数据库.* to 用户名@登录主机 identified by “密码”
**1、增加一个用户test1密码为abc，让他可以在任何主机上登录，并对所有数据库有查询、插入、修改、删除的权限。首先用root用户连入MYSQL，然后键入以下命令：**
    grant select,insert,update,delete on *.* to [email=test1@”%]test1@”%[/email]” Identified by “abc”;

但增加的用户是十分危险的，你想如某个人知道test1的密码，那么他就可以在internet上的任何一台电脑上登录你的mysql数据库并对你的数据可以为所欲为了，解决办法见2。
**2、增加一个用户test2密码为abc,让他只可以在localhost上登录，并可以对数据库mydb进行查询、插入、修改、删除的操作（localhost指本地主机，即MYSQL数据库所在的那台主机）**，这样用户即使用知道test2的密码，他也无法从internet上直接访问数据库，只能通过MYSQL主机上的web页来访问了。
    grant select,insert,update,delete on mydb.* to [email=test2@localhost]test2@localhost[/email] identified by “abc”;

如果你不想test2有密码，可以再打一个命令将密码消掉。
    grant select,insert,update,delete on mydb.* to [email=test2@localhost]test2@localhost[/email] identified by “”;

　　--创建数据库

　　mysql> create database 数据库名称

　　--创建表

　　mysql> create table 表名 (

　　列的名字（id）类型（int（4））primary key(定义主键) auto_increment（描述 自增）,

　　……，

　　）；

　　--查看所有数据库

　　mysql> show databases 数据库名称;

　　--使用某个数据库

　　mysql> use database 数据库名称;

　　--查看所使用数据库下所有的表

　　mysql> show tables;

　　--显示表的属性结构

　　mysql> desc 表名;

　　--选择表中数据的显示

　　--* 代表选择所有列 ，

　　mysql> select * from 表名 where id=?[and name=？] [or name=?];

　　mysql> select id,name from 表名order by 某一列的名称 desc(降序，asc为升序)

　　--删除表中的数据

　　mysql> delete from table where id=？[or name=? (and name=?)];

　　--删除表

　　mysql> drop table;

　　--删除数据库

　　mysql> drop database;

        --备份数据库

命令在DOS的[url=file://\\mysql\\bin]\\mysql\\bin[/url]目录下执行
**1.导出整个数据库**
导出文件默认是存在mysql\bin目录下
    mysqldump -u 用户名 -p 数据库名 > 导出的文件名
    mysqldump -u user_name -p123456 database_name > outfile_name.sql
**2.导出一个表**
    mysqldump -u 用户名 -p 数据库名 表名> 导出的文件名
    mysqldump -u user_name -p database_name table_name > outfile_name.sql
**3.导出一个数据库结构**
    mysqldump -u user_name -p -d –add-drop-table database_name > outfile_name.sql
    -d 没有数据 –add-drop-table 在每个create语句之前增加一个drop table
**4.带语言参数导出**
    mysqldump -uroot -p –default-character-set=latin1 –set-charset=gbk –skip-opt database_name > outfile_name.sql
例如，将aaa库备份到文件back_aaa中：
　　[root@test1 root]# cd　/home/data/mysql
　　[root@test1 mysql]# mysqldump -u root -p --opt aaa > back_aaa

　　索引是一种特殊的文件(InnoDB数据表上的索引是表空间的一个组成部分)，它们包含着对数据表里所有记录的引用指针。

　　注：[1]索引不是万能的！索引可以加快数据检索操作，但会使数据修改操作变慢。每修改数据记录，索引就必须刷新一次。为了在某种程序上弥补这一缺陷，许多SQL命令都有一个DELAY_KEY_WRITE项。这个选项的作用是暂时制止 MySQL在该命令每插入一条新记录和每修改一条现有之后立刻对索引进行刷新，对索引的刷新将等到全部记录插入/修改完毕之后再进行。在需要把许多新记录插入某个数据表的场合，DELAY_KEY_WRITE选项的作用将非常明显。[2]另外，索引还会在硬盘上占用相当大的空间。因此应该只为最经常查询和最经常排序的数据列建立索引。注意，如果某个数据列包含许多重复的内容，为它建立索引就没有太大的实际效果。

　　从理论上讲，完全可以为数据表里的每个字段分别建一个索引，但MySQL把同一个数据表里的索引总数限制为16个。

　　1. InnoDB数据表的索引

　　与MyISAM数据表相比，在 InnoDB数据表上，索引对InnoDB数据表的重要性要在得多。在InnoDB数据表上，索引不仅会在搜索数据记录时发挥作用，还是数据行级锁定机制的苊、基础。"数据行级锁定"的意思是指在事务操作的执行过程中锁定正在被处理的个别记录，不让其他用户进行访问。这种锁定将影响到(但不限于)SELECT...LOCK IN SHARE MODE、SELECT...FOR UPDATE命令以及INSERT、UPDATE和DELETE命令。

　　出于效率方面的考虑，InnoDB数据表的数据行级锁定实际发生在它们的索引上，而不是数据表自身上。显然，数据行级锁定机制只有在有关的数据表有一个合适的索引可供锁定的时候才能发挥效力。

　　2. 限制

　　如果WEHERE子句的查询条件里有不等号(WHERE coloum != ...)，MySQL将无法使用索引。

　　类似地，如果WHERE子句的查询条件里使用了函数(WHERE DAY(column) = ...)，MySQL也将无法使用索引。

　　在JOIN操作中(需要从多个数据表提取数据时)，MySQL只有在主键和外键的数据类型相同时才能使用索引。

　　如果WHERE子句的查询条件里使用比较操作符LIKE和REGEXP，MySQL 只有在搜索模板的第一个字符不是通配符的情况下才能使用索引。比如说，如果查询条件是LIKE 'abc%'，MySQL将使用索引；如果查询条件是LIKE '%abc'，MySQL将不使用索引。

　　在ORDER BY操作中，MySQL只有在排序条件不是一个查询条件表达式的情况下才使用索引。(虽然如此，在涉及多个数据表查询里，即使有索引可用，那些索引在加快ORDER BY方面也没什么作用)

　　如果某个数据列里包含许多重复的值，就算为它建立了索引也不会有很好的效果。比如说，如果某个数据列里包含的净是些诸如"0/1"或"Y/N"等值，就没有必要为它创建一个索引。

　　普通索引、唯一索引和主索引

　　1. 普通索引

　　普通索引(由关键字KEY或INDEX定义的索引)的唯一任务是加快对数据的访问速度。因此，应该只为那些最经常出现在查询条件(WHERE column = ...)或排序条件(ORDER BY column)中的数据列创建索引。只要有可能，就应该选择一个数据最整齐、最紧凑的数据列(如一个整数类型的数据列)来创建索引。

　　2. 唯一索引

　　普通索引允许被索引的数据列包含重复的值。比如说，因为人有可能同名，所以同一个姓名在同一个"员工个人资料"数据表里可能出现两次或更多次。

　　如果能确定某个数据列将只包含彼此各不相同的值，在为这个数据列创建索引的时候就应该用关键字UNIQUE把它定义为一个唯一索引。这么做的好处：一是简化了MySQL对这个索引的管理工作，这个索引也因此而变得更有效率；二是 MySQL会在有新记录插入数据表时，自动检查新记录的这个字段的值是否已经在某个记录的这个字段里出现过了；如果是，MySQL将拒绝插入那条新记录。也就是说，唯一索引可以保证数据记录的唯一性。事实上，在许多场合，人们创建唯一索引的目的往往不是为了提高访问速度，而只是为了避免数据出现重复。

　　3. 主索引

　　在前面已经反复多次强调过：必须为主键字段创建一个索引，这个索引就是所谓的"主索引"。主索引与唯一索引的唯一区别是：前者在定义时使用的关键字是PRIMARY而不是UNIQUE。

　　4. 外键索引

　　如果为某个外键字段定义了一个外键约束条件，MySQL就会定义一个内部索引来帮助自己以最有效率的方式去管理和使用外键约束条件。

　　5. 复合索引

　　索引可以覆盖多个数据列，如像INDEX(columnA, columnB)索引。这种索引的特点是MySQL可以有选择地使用一个这样的索引。如果查询操作只需要用到columnA数据列上的一个索引，就可以使用复合索引INDEX(columnA, columnB)。不过，这种用法仅适用于在复合索引中排列在前的数据列组合。比如说，INDEX(A, B, C)可以当做A或(A, B)的索引来使用，但不能当做B、C或(B, C)的索引来使用。

　　6. 索引的长度

　　在为CHAR和VARCHAR类型的数据列定义索引时，可以把索引的长度限制为一个给定的字符个数(这个数字必须小于这个字段所允许的最大字符个数)。这么做的好处是可以生成一个尺寸比较小、检索速度却比较快的索引文件。在绝大多数应用里，数据库中的字符串数据大都以各种各样的名字为主，把索引的长度设置为10~15个字符已经足以把搜索范围缩小到很少的几条数据记录了。

　　在为BLOB和TEXT类型的数据列创建索引时，必须对索引的长度做出限制；MySQL所允许的最大索引jlkjljkjlj全文索引

　　文本字段上的普通索引只能加快对出现在字段内容最前面的字符串(也就是字段内容开头的字符)进行检索操作。如果字段里存放的是由几个、甚至是多个单词构成的较大段文字，普通索引就没什么作用了。这种检索往往以LIKE %word%的形式出现，这对MySQL来说很复杂，如果需要处理的数据量很大，响应时间就会很长。

　　这类场合正是全文索引(full-text index)可以大显身手的地方。在生成这种类型的索引时，MySQL将把在文本中出现的所有单词创建为一份清单，查询操作将根据这份清单去检索有关的数据记录。全文索引即可以随数据表一同创建，也可以等日后有必要时再使用下面这条命令添加：

　　ALTER TABLE tablename ADD FULLTEXT(column1, column2)

　　有了全文索引，就可以用SELECT查询命令去检索那些包含着一个或多个给定单词的数据记录了。下面是这类查询命令的基本语法：

　　SELECT * FROM tablename

　　WHERE MATCH(column1, column2) AGAINST('word1', 'word2', 'word3')

　　上面这条命令将把column1和column2字段里有word1、word2和word3的数据记录全部查询出来。

　　注解：InnoDB数据表不支持全文索引。

　　查询和索引的优化

　　只有当数据库里已经有了足够多的测试数据时，它的性能测试结果才有实际参考价值。如果在测试数据库里只有几百条数据记录，它们往往在执行完第一条查询命令之后就被全部加载到内存里，这将使后续的查询命令都执行得非常快--不管有没有使用索引。只有当数据库里的记录超过了1000条、数据总量也超过了MySQL服务器上的内存总量时，数据库的性能测试结果才有意义。

　　在不确定应该在哪些数据列上创建索引的时候，人们从EXPLAIN SELECT命令那里往往可以获得一些帮助。这其实只是简单地给一条普通的SELECT命令加一个EXPLAIN关键字作为前缀而已。有了这个关键字，MySQL将不是去执行那条SELECT命令，而是去对它进行分析。MySQL将以表格的形式把查询的执行过程和用到的索引(如果有的话)等信息列出来。

　　在EXPLAIN命令的输出结果里，第1列是从数据库读取的数据表的名字，它们按被读取的先后顺序排列。type列指定了本数据表与其它数据表之间的关联关系(JOIN)。在各种类型的关联关系当中，效率最高的是system，然后依次是const、eq_ref、ref、range、index和All(All的意思是：对应于上一级数据表里的每一条记录，这个数据表里的所有记录都必须被读取一遍--这种情况往往可以用一索引来避免)。

　　possible_keys数据列给出了MySQL在搜索数据记录时可选用的各个索引。key数据列是MySQL实际选用的索引，这个索引按字节计算的长度在key_len数据列里给出。比如说，对于一个INTEGER数据列的索引，这个字节长度将是4。如果用到了复合索引，在key_len数据列里还可以看到MySQL具体使用了它的哪些部分。作为一般规律，key_len数据列里的值越小越好(意思是更快)。

　　ref数据列给出了关联关系中另一个数据表里的数据列的名字。row数据列是MySQL在执行这个查询时预计会从这个数据表里读出的数据行的个数。row数据列里的所有数字的乘积可以让我们大致了解这个查询需要处理多少组合。

　　最后，extra数据列提供了与JOIN操作有关的更多信息，比如说，如果MySQL在执行这个查询时必须创建一个临时数据表，就会在extra列看到using temporary字样

## 安装MySQL时候的注意事项

　　1、如果您是用MySQL+Apache，使用的又是FreeBSD网路操作系统的话，安装时候你应按注意到FreeBSD的版本问题，在FreeBSD的3.0以下版本来说，MySQL Source内含的MIT-pthread运行是正常的，但在这版本以上，你必须使用native threads,也就是加入一个with-named-thread-libs=-lc_r的选项。

　　2、如果您在COMPILE过程中出了问题，请先检查你的gcc版本是否在2.81版本以上，gmake版本是否在3.75以上。

　　3、如果不是版本的问题，那可能是你的内存不足，请使用./configure -- with-low-memory来加入。

　　4、如果您要重新做你的configure，那么你可以键入rm config.cache和make clean来清除记录。

　　5、我们一般把MySQL安装在/usr/local目录下，这是缺省值，您也可以按照你的需要设定你所安装的目录。

 下载MySQL历史版本
https://downloads.mysql.com/archives/community/

**相关网站:**

1.Mysql中文社区: [http://www.mysql.net.cn/](http://www.mysql.net.cn/)

2.SQL/mysql专题 [http://www.hellocto.com/channel/sql/](http://www.hellocto.com/channel/sql/)

3.中国域名网 [http://www.chinadomain.com.cn](http://www.chinadomain.com.cn/)

4.MySQL官方：[http://www.mysql.com](http://www.mysql.com/)

