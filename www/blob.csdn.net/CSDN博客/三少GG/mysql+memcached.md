# mysql+memcached - 三少GG - CSDN博客
2013年07月27日 17:43:32[三少GG](https://me.csdn.net/scut1135)阅读数：1510
个人分类：[Web网站设计](https://blog.csdn.net/scut1135/article/category/762263)
[安装和使用memcached](http://www.cnblogs.com/czh-liyu/archive/2010/04/27/1722084.html)
如何将 memcached 融入到您的环境中？
在开始安装和使用 using memcached 之前，我们需要了解如何将 memcached 融入到您的环境中。虽然在任何地方都可以使用 memcached，但我发现需要在数据库层中执行几个经常性查询时，memcached 往往能发挥最大的效用。我经常会在数据库和应用服务器之间设置一系列 memcached 实例，并采用一种简单的模式来读取和写入这些服务器。图 1 可以帮助您了解如何设置应用程序体系结构：
**图 1. 使用 memcached 的示例应用程序体系结构**
![使用 memcached 的示例应用程序体系结构](http://www.ibm.com/developerworks/cn/java/j-memcached1/memcached1-fig1.gif)
体系结构相当容易理解。我建立了一个 Web 层，其中包括一些 Apache 实例。下一层是应用程序本身。这一层通常运行于 Apache Tomcat 或其他开源应用服务器之上。再下面一层是配置 memcached 实例的地方 — 即应用服务器与数据库服务器之间。在使用这种配置时，需要采用稍微不同的方式来执行数据库的读取和写入操作。
**读取**
我执行读取操作的顺序是从 Web 层获取请求（需要执行一次数据库查询）并检查之前在缓存中存储的查询结果。如果我找到所需的值，则返回它。如果未找到，则执行查询并将结果存储在缓存中，然后再将结果返回给 Web 层。
**写入**
将数据写入到数据库中时，首先需要执行数据库写入操作，然后将之前缓存的任何受此写入操作影响的结果设定为无效。此过程有助于防止缓存和数据库之间出现数据不一致性。
安装 memcached
memcached 支持一些操作系统，包括 Linux®、Windows®、Mac OS 和 Solaris。在本文中，我将详细介绍如何通过源文件构建和安装 memcached。采用这种方式的主要原因是我在遇到问题时可以查看源代码。
**libevent**
[libevent](http://www.ibm.com/developerworks/cn/java/j-memcached1/#resources) 是安装 memcached 的唯一前提条件。它是 memcached 所依赖的异步事件通知库。您可以在[monkey.org](http://www.ibm.com/developerworks/cn/java/j-memcached1/#resources)
 上找到关于 libevent 的源文件。接下来，找到其最新版本的源文件。对于本文，我们使用稳定的 1.4.11 版本。获取了归档文件之后，将它解压到一个方便的位置，然后执行清单 1 中的命令：
**清单 1. 生成和安装 libevent**
				
cd libevent-1.4.11-stable/
./configure
			
make
				
make install
**memcached**
从 [Danga Interactive](http://www.ibm.com/developerworks/cn/java/j-memcached1/#resources) 获取 memcached 源文件，仍然选择最新的分发版。在撰写本文时，其最新版本是 1.4.0。将 tar.gz 解压到方便的位置，并执行清单 2 中的命令：
**清单 2. 生成和安装 memcached**
				
cd memcached-1.4.0/
./configure
			
make
				
make install
完成这些步骤之后，您应该安装了一个 memcached 工作副本，并且可以使用它了。让我们进行简单介绍，然后使用它。 
使用 memcached
要开始使用 memcached，您首先需要启动 memcached 服务器，然后使用 telnet 客户机连接到它。
要启动 memcached，执行清单 3 中的命令：
**清单 3. 启动 memcached**
				
./memcached -d -m 2048 -l 10.0.0.40 -p 11211
这会以守护程序的形式启动 memcached（`-d`），为其分配 2GB 内存（`-m 2048`），并指定监听 localhost，即端口 11211。您可以根据需要修改这些值，但以上设置足以完成本文中的练习。接下来，您需要连接到 memcached。您将使用一个简单的 telnet 客户机连接到 memcached 服务器。
大多数操作系统都提供了内置的 telnet 客户机，但如果您使用的是基于 Windows 的操作系统，则需要下载第三方客户机。我推荐使用 [PuTTy](http://www.ibm.com/developerworks/cn/java/j-memcached1/#resources)。
安装了 telnet 客户机之后，执行清单 4 中的命令：
**清单 4. 连接到 memcached**
				
telnet localhost 11211
如果一切正常，则应该得到一个 telnet 响应，它会指示 **Connected to localhost（已经连接到 localhost）**。如果未获得此响应，则应该返回之前的步骤并确保 libevent 和 memcached 的源文件都已成功生成。
您现现已经登录到 memcached 服务器。此后，您将能够通过一系列简单的命令来与 memcached 通信。9 个 memcached 客户端命令可以分为三类：
- 基本
- 高级
- 管理 
**基本 memcached 客户机命令**
您将使用五种基本 memcached 命令执行最简单的操作。这些命令和操作包括：  
- `set`
- `add`
- `replace`
- `get`
- `delete`
前三个命令是用于操作存储在 memcached 中的键值对的标准修改命令。它们都非常简单易用，且都使用清单 5 所示的语法：
**清单 5. 修改命令语法**
				
command <key> <flags> <expiration time> <bytes>
<value>
表 1 定义了 memcached 修改命令的参数和用法。
**表 1. memcached 修改命令参数**
|参数|用法|
|----|----|
|key|key 用于查找缓存值|
|flags|可以包括键值对的整型参数，客户机使用它存储关于键值对的额外信息|
|expiration time|在缓存中保存键值对的时间长度（以秒为单位，0 表示永远）|
|bytes|在缓存中存储的字节点|
|value|存储的值（始终位于第二行）|
现在，我们来看看这些命令的实际使用。
**set**
`set` 命令用于向缓存添加新的键值对。如果键已经存在，则之前的值将被替换。
注意以下交互，它使用了 `set` 命令：
set userId 0 0 5
12345
STORED
如果使用 `set` 命令正确设定了键值对，服务器将使用单词 **STORED** 进行响应。本示例向缓存中添加了一个键值对，其键为`userId`，其值为`12345`。并将过期时间设置为 0，这将向 memcached 通知您希望将此值存储在缓存中直到删除它为止。
**add**
仅当缓存中不存在键时，`add` 命令才会向缓存中添加一个键值对。如果缓存中已经存在键，则之前的值将仍然保持相同，并且您将获得响应 **NOT_STORED**。
下面是使用 `add` 命令的标准交互：
set userId 0 0 5
12345
STORED
add userId 0 0 5
55555
NOT_STORED
add companyId 0 0 3
564
STORED
**replace**
仅当键已经存在时，`replace` 命令才会替换缓存中的键。如果缓存中不存在键，那么您将从 memcached 服务器接受到一条 **NOT_STORED** 响应。 
下面是使用 `replace` 命令的标准交互： 
replace accountId 0 0 5
67890
NOT_STORED
set accountId 0 0 5
67890
STORED
replace accountId 0 0 5
55555
STORED
最后两个基本命令是 `get` 和 `delete`。这些命令相当容易理解，并且使用了类似的语法，如下所示：
command <key>
接下来看这些命令的应用。
**get**
`get` 命令用于检索与之前添加的键值对相关的值。您将使用 `get` 执行大多数检索操作。 
下面是使用 `get` 命令的典型交互：
set userId 0 0 5
12345
STORED
get userId
VALUE userId 0 5
12345
END
get bob
END
如您所见，`get` 命令相当简单。您使用一个键来调用 `get`，如果这个键存在于缓存中，则返回相应的值。如果不存在，则不返回任何内容。
**delete**
最后一个基本命令是 `delete`。`delete` 命令用于删除 memcached 中的任何现有值。您将使用一个键调用`delete`，如果该键存在于缓存中，则删除该值。如果不存在，则返回一条**NOT_FOUND** 消息。
下面是使用 `delete` 命令的客户机服务器交互：
set userId 0 0 5
98765
STORED
delete bob
NOT_FOUND
delete userId
DELETED
get userId
END
**高级 memcached 客户机命令**
可以在 memcached 中使用的两个高级命令是 `gets` 和 `cas`。`gets` 和`cas` 命令需要结合使用。您将使用这两个命令来确保不会将现有的名称/值对设置为新值（如果该值已经更新过）。我们来分别看看这些命令。
**gets**
`gets` 命令的功能类似于基本的 `get` 命令。两个命令之间的差异在于，`gets` 返回的信息稍微多一些：64 位的整型值，非常像名称/值对的**“版本” 标识符**。
下面是使用 `gets` 命令的客户机服务器交互：
set userId 0 0 5
12345
STORED
get userId
VALUE userId 0 5
12345
END
gets userId
VALUE userId 0 5 **4**
12345
END
考虑 `get` 和 `gets` 命令之间的差异。`gets` 命令将返回一个额外的值 — 在本例中是整型值 4，用于标识名称/值对。如果对此名称/值对执行另一个`set` 命令，则`gets` 返回的额外值将会发生更改，以表明名称/值对已经被更新。清单 6 显示了一个例子：
**清单 6. set 更新版本指示符**
				
set userId 0 0 5
33333
STORED
gets userId
VALUE userId 0 5 **5**
33333
END
您看到 `gets` 返回的值了吗？它已经更新为 5。您每次修改名称/值对时，该值都会发生更改。
**cas**
`cas`（check 和 set）是一个非常便捷的 memcached 命令，用于设置名称/值对的值（如果该名称/值对在您上次执行 
```
gets
```
 后没有更新过）。它使用与 `set` 命令相类似的语法，但包括一个额外的值：`gets` 返回的额外值。
注意以下使用 `cas` 命令的交互：
set userId 0 0 5
55555
STORED
gets userId
VALUE userId 0 5 **6**
55555
END
cas userId 0 0 5 **6**
33333
STORED
如您所见，我使用额外的整型值 6 来调用 cas 命令，并且操作运行非常顺利。现在，我们来看看清单 7 中的一系列命令：
**清单 7. 使用旧版本指示符的 `cas` 命令**
				
set userId 0 0 5
55555
STORED
gets userId
VALUE userId 0 5 **8**
55555
END
cas userId 0 0 5 **6**
33333
EXISTS
注意，我并未使用 `gets` 最近返回的整型值，并且 `cas` 命令返回 EXISTS 值以示失败。从本质上说，同时使用`gets` 和`cas` 命令可以防止您使用自上次读取后经过更新的名称/值对。
**缓存管理命令**
最后两个 memcached 命令用于监控和清理 memcached 实例。它们是 `stats` 和 `flush_all` 命令。
**stats**
`stats` 命令的功能正如其名：转储所连接的 memcached 实例的当前统计数据。在下例中，执行 `stats` 命令显示了关于当前 memcached 实例的信息：
stats
STAT pid 63
STAT uptime 101758
STAT time 1248643186
STAT version 1.4.11
STAT pointer_size 32
STAT rusage_user 1.177192
STAT rusage_system 2.365370
STAT curr_items 2
STAT total_items 8
STAT bytes 119
STAT curr_connections 6
STAT total_connections 7
STAT connection_structures 7
STAT cmd_get 12
STAT cmd_set 12
STAT get_hits 12
STAT get_misses 0
STAT evictions 0
STAT bytes_read 471
STAT bytes_written 535
STAT limit_maxbytes 67108864
STAT threads 4
END
此处的大多数输出都非常容易理解。稍后在讨论缓存性能时，我还将详细解释这些值的含义。至于目前，我们先来看看输出，然后再使用新的键来运行一些 `set` 命令，并再次运行`stats` 命令，注意发生了哪些变化。
**flush_all**
`flush_all` 是最后一个要介绍的命令。这个最简单的命令仅用于清理缓存中的所有名称/值对。如果您需要将缓存重置到干净的状态，则 
```
flush_all
```
 能提供很大的用处。下面是一个使用 `flush_all` 的例子： 
set userId 0 0 5
55555
STORED
get userId
VALUE userId 0 5
55555
END
flush_all
OK
get userId
END
缓存性能
在本文的最后，我将讨论如何使用高级 memcached 命令来确定缓存的性能。`stats` 命令用于调优缓存的使用。需要注意的两个最重要的统计数据是 et_hits 和 get_misses。这两个值分别指示找到名称/值对的次数（get_hits）和未找到名称/值对的次数（get_misses）。
结合这些值，我们可以确定缓存的利用率如何。初次启动缓存时，可以看到 get_misses 会自然地增加，但在经过一定的使用量之后，这些 get_misses 值应该会逐渐趋于平稳 — 这表示缓存主要用于常见的读取操作。如果您看到 get_misses 继续快速增加，而 get_hits 逐渐趋于平稳，则需要确定一下所缓存的内容是什么。您可能缓存了错误的内容。
确定缓存效率的另一种方法是查看缓存的命中率（hit ratio）。缓存命中率表示执行 `get` 的次数与错过 `get` 的次数的百分比。要确定这个百分比，需要再次运行`stats` 命令，如清单 8 所示：
**清单 8. 计算缓存命中率**
				
stats
STAT pid 6825
STAT uptime 540692
STAT time 1249252262
STAT version 1.2.6
STAT pointer_size 32
STAT rusage_user 0.056003
STAT rusage_system 0.180011
STAT curr_items 595
STAT total_items 961
STAT bytes 4587415
STAT curr_connections 3
STAT total_connections 22
STAT connection_structures 4
STAT cmd_get 2688
STAT cmd_set 961
STAT get_hits 1908
STAT get_misses 780
STAT evictions 0
STAT bytes_read 5770762
STAT bytes_written 7421373
STAT limit_maxbytes 536870912
STAT threads 1
END
现在，用 get_hits 的数值除以 cmd_gets。在本例中，您的命中率大约是 71%。在理想情况下，您可能希望得到更高的百分比 — 比率越高越好。查看统计数据并不时测量它们可以很好地判定缓存策略的效率。
常有命令如下：
启动/结束
memcached -d -m 10 -u root -l 192.168.0.122 -p 11200 -c 256 -P /tmp/memcached.pid
-d 选项是启动一个守护进程， 
-m 是分配给Memcache使用的内存数量，单位是MB，这里是10MB
-u 是运行Memcache的用户，这里是root
-l 是监听的服务器IP地址，如果有多个地址的话，这里指定了服务器的IP地址192.168.0.122 
-p 是设置Memcache监听的端口，这里设置了12000，最好是1024以上的端口
-c 选项是最大运行的并发连接数，默认是1024，这里设置了256，按照你服务器的负载量来设定
-P 是设置保存Memcache的pid文件
kill `cat /tmp/memcached.pid`
获取运行状态
echo stats | nc 192.168.1.123 11200
watch "echo stats | nc 192.168.1.123 11200" (实时状态)
## [将MySQL数据映射到Memcached中](http://www.cnblogs.com/cy163/archive/2009/08/12/1544141.html)
### 2月 6th, 2009
作者:张立冰
出处:http://www.libing.name/2009/02/06/mysql-map-data-to-memcachedmysql-map-data-to-memcached.html
差不多在一年前，写过一篇文章介绍[将MySQL数据映射到Memcached](http://www.libing.name/2008/02/26/mysql-map-memcached.html)，当时MySQL和Memcached Functions for MySQL都还不够成熟，时过一年，[Memcached
 Functions for MySQL](http://tangent.org/586/Memcached_Functions_for_MySQL.html)升级到了0.8版本，而MySQL也发布了GA版本，加上很多朋友反应前一篇文章中的实现他们因种种原因没能成功，于是便有了这篇文章，就当是上一篇文章的升级版本吧。
测试环境在Linux下进行，版本系统为CentOS5.
以下为相关软件，包括其版本和下载地址：
mysql-5.1.30 [下载](http://dev.mysql.com/get/Downloads/MySQL-5.1/mysql-5.1.30.tar.gz/from/http://mysql.byungsoo.net/)
memcached-1.2.6 [下载](http://danga.com/memcached/dist/memcached-1.2.6.tar.gz)
libevent-1.4.7-stable [下载](http://monkey.org/~provos/libevent-1.4.7-stable.tar.gz)
memcached_functions_mysql-0.8 [下载](http://download.tangent.org/memcached_functions_mysql-0.8.tar.gz)
libmemcached-0.26 [下载](http://download.tangent.org/libmemcached-0.26.tar.gz)
编译安装MySQL,安装因个人细好而定，省略许多与测试无关的编译细节及参数。
[root@localhost ~]#tar xzf mysql-5.1.30.tar_001.gz[root@localhost ~]#cd mysql-5.1.30[root@localhost ~]#./configure --prefix=/usr/local/mysql51[root@localhost ~]#make[root@localhost ~]#make install[root@localhost ~]#./scripts/mysql_install_db --user=mysql --skip-name-resolve[root@localhost ~]#/usr/local/mysql51/bin/mysqld_safe
省略列出安装memcached和libevent的相关命令，具体可按照实际情况安装，测试时我将libevent默认安装，memcached安装于/usr/local/memcached目录下。
启动memcached.
/usr/local/memcached/bin/memcached -d -m 50-u root -p 11211
编译安装libmemcache.
[root@localhost ~]#tar xzf libmemcached-0.26.tar.gz[root@localhost ~]#cd libmemcached-0.26[root@localhost ~]#./configure --with-memcached=/usr/local/memcached/bin/memcached[root@localhost ~]# make && make install
编译安装Memcache UDFs for MySQL.
[root@localhost ~]# tar xzf memcached_functions_mysql-0.8.tar.gz[root@localhost ~]# cd memcached_functions_mysql-0.8[root@localhost ~]# ./configure --with-mysql-config=/usr/local/mysql51/bin/mysql_config[root@localhost ~]# make && make install
编译完成后将编译好的库文件复制到mysql的插件目录下，以便于加载使用。
cp /usr/local/lib/libmemcached_functions_mysql*/usr/local/mysql51/lib/mysql/plugin/
进入memcached_functions_mysql的源码目录，在目录下有相关添加UDF的SQL文件用于初始化。
[root@localhost ~]# mysql <sql/install_functions.sql
注：如果对这些UDFs不熟悉或者不懂，可进行源码目录参看[README](http://hg.tangent.org/memcached_functions_mysql/file/724eeccc55bc/README)，里边有相应的说明。
至此，相关软件的编译和安装完成，进行测试，我们要达到的目的是当MySQL有新记录插入时，同时插入到Memcached中，当记录更新时同步更新Memcached中的记录，删除时同时也删除Memcached相关的记录，为此创建三个触发器来实现，如果对MySQL的触发程序不熟悉可以参考MySQL手册第21章，下面SQL中的memcached为需要操作的表名，SQL如下：
#插入数据时插入Memcached
create trigger mysqlmmci after insert on memcached for each row set @tmp = memc_set(NEW.key, NEW.value);#更新记录时更新Memcached
create trigger mysqlmmcu after update on memcached for each row set @tmp = memc_set(NEW.key, NEW.value);#删除记录时删除Memcached相应的记录
create trigger mysqlmmcd before delete on memcached for each row set @tmp = memc_delete(OLD.key);
以下为测试记录，在对MySQL操作的同时操作Memcached来查看情况，当然你也可以在启动Memcached的时候带-vv参数来查看相关信息.
MySQL操作相关的记录：
[root@localhost ~]#mysql -S /tmp/mysql51.sock 
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 6
Server version: 5.1.30 Source distribution
 
Type 'help;' or '\h'for help. Type'\c' to clear the buffer.
 
mysql> use test;
Database changed
 
mysql> create table `memcached` (`key` varchar(10), `value` varchar(100));                                     
Query OK,0 rows affected (0.00 sec)
 
mysql> create trigger mysqlmmci after insert on memcached for each row set @tmp = memc_set(NEW.key, NEW.value);
Query OK,0 rows affected (0.00 sec)
 
mysql> create trigger mysqlmmcu after update on memcached for each row set @tmp = memc_set(NEW.key, NEW.value);
Query OK,0 rows affected (0.00 sec)
 
mysql> create trigger mysqlmmcd before delete on memcached for each row set @tmp = memc_delete(OLD.key);
Query OK,0 rows affected (0.00 sec)
 
mysql> insert into memcached values("keyi","valuei"),("keyu","valueu"),("keyd","valued");                                         
Query OK,3 rows affected (0.00 sec)
Records:3  Duplicates:0  Warnings:0
 
mysql> update memcached set `value`="update" where `key`="keyu";
Query OK,1 row affected (0.00 sec)
Rows matched:1  Changed:1  Warnings:0
 
mysql> delete from memcached where `key`="keyd";
Query OK,1 row affected (0.00 sec)
 
mysql> quit
Bye
Memcache查看时的记录：
[root@localhost ~]#telnet 127.0.0.1 11211
Trying 127.0.0.1...
Connected to 127.0.0.1.
Escape character is '^]'.
get keyi
VALUE keyi 06
valuei
END
get keyu
VALUE keyu 06
valueu
END
get keyd
VALUE keyd 06
valued
END
get keyu
VALUE keyu 06
update
END
get keyd
END
quit
Connection closed by foreign host.
至此，我们基本实现的将MySQL的数据同步到Memcached中，性能暂时还没有测试，当然上面只是简单的实现的数据映射的功能，如果在实现的生产环境中，则需要考虑名字空间，高可靠性的问题，这些都是可以通过数据库名-表名-关键字的方面能达到KEY唯一的目的，而高可靠性则是一个比较大的问题。
这次是Fotolog的经验，传说中比Flickr更大的网站，Fotolog在21台服务器上部署了51个memcached实例，总计有254G缓存空间可用，缓存了多达175G的内容，这个数量比很多网站的数据库都要大的多，原文是A Bunch of Great Strategies for Using Memcached and MySQL Better Together，我这里还是选择性的翻译以及按照我的理解补充，感谢Todd Hoff，总能给我们一些学习的案例，从这里也能看出国外技术的开放态度，不似我们，其实就那么点小九九还藏着掖着，好了，进入正题。
一、关于memcached
　　还不知道这个？那你去面试的时候要吃亏了，赶紧去官方网站看一下[http://www.danga.com/memcached/](http://www.danga.com/memcached/)，另外google一下用法，硬盘总是太慢，把数据存在内存里面吧，如果你只有一台服务器，推荐用一下APC(Facebook在用)或者eaccelerator或者Xcache(国人开发的)，这些产品单机效果更好，如果你需要分布式的缓存方案，那么用memcached吧。
二、memcached如何与mysql并肩作战？
◦通过数据库分片来解决数据库写扩展的问题把数据库分片，部署到不同的服务器上，免得只有一个主服务器，写操作成为瓶颈以及可能有的“单点故障”，一般的数据库分片主要是按照业务来分，尽可能的拆分业务，不相干的都独立起来做成服务也好
◦前端mysql和一堆memcached服务器来应付读的问题应用程序首先从memcached中获取数据，获取不到再从数据库中获得并保存在memcached中，以前看过一篇文章说好的应用95％的数据从memcache的中获得，3％的数据从mysql的query cache中获得，剩下2％才去查表，对比一下你的应用，差距有多远？
◦通过mysql复制（master-slave）来解决读的问题
首先mysql数据库通过master-slave读写分离，多个slave来应对应用程序读的操作。 
三、为什么不用mysql的query cache？
　　我们都知道mysql有个query cache，可以缓存上次查询的结果，可实际上帮不上太多的忙，下面是mysql quety cache的不足：
◦只能有一个实例
意味着你能存储内容的上限就是你服务器的可用内存，一台服务器能有多少内存？你又能存多少呢？ 
◦只要有写操作，mysql的query cache就失效
只要数据库内容稍有改变，那怕改变的是其他行，mysql的query cache也会失效 
◦mysql的query cache只能缓存数据库数据行
意味着其他内容都不行，比如数组，比如对象，而memcached理论上可以缓存任何内容，甚至文件^_^ 
四、Fotolog的缓存技术
◦非确定性缓存你不确定你要的数据缓存中有没有，你也不知道是不是过期了，于是你就试探性的问memcached，我要的什么什么数据你那有吗？我可不要过期的数据啊，memcached告诉你说有并且给你，你就开心了，如果没有呢，你就要从数据库或者别的地方去获取了，这是memcached典型的应用。主要应用在：1.复杂的数据需要多次读取，你的数据库做了分片处理，从多个数据库中获取数据并组合起来是一个非常大的开销，你大可以把这些数据取出来之后存到memcached中
2.mysql query cache的一个好的替代方案，这样数据库其他部门改变了，只要自己没改变就没问题（注意数据库更新的问题，后面会提到）
3.把关系或者列表缓存起来，比如某个栏目下的多篇文章列表
4.被多个页面调用并且获取起来很慢的数据，或者是更新很慢的数据，比如文章浏览排行榜
5.如果cache的开销超过重新获取的开销，那么不要缓存它吧
6.标签云和自动建议(类似google sugest)
例如：当一个用户上传一个图片，这个用户的好友页面上都要列出这张图片来，那么把它缓存起来吧。
潜在问题：
memcached消耗的主要是服务器内存，对CPU消耗很小，所以Fotolog把memcached部署在他们的应用服务器上（貌似我们也是这样），他们遇到了CPU搞到90％的使用率（怎么会那么高？哪出问题了吧）、内存回收（这是个大问题）等等问题。
◦状态缓存把应用服务的当前状态存在memcached中主要应用在：1.“昂贵”的操作，开销大的操作 
2.sessions会话，Flickr把session存在数据库中，个人感觉还是存memcached比较“便宜”些，如果memecached服务器down掉了，那么重新登录吧。
3.记录用户在线信息(我们也是这样做的) 
◦确定性缓存对于某些特定数据库的全部内容，都缓存到memcached，有一个专门的应用服务来保障你要的数据都在memcached中，其他应用服务直接从memcached中获取数据而不去取数据库，因为数据库已经全部保存到memcached中并保持同步。主要应用在：1.读取伸展，所有的读取都从memcached中获得，数据库没有负载
2.”永不过期“(相对的)的数据，比如行政规划数据，变动很小吧
3.经常调用的内容
4.用户的认证信息
5.用户的概要信息
6.用户的参数设置
7.用户当前常用的媒体文件列表，比如用户的图片
8.用户登录，不走数据库，只走memcached（个人觉得这个不太好，登录信息还是需要持久化的，用类似BDB这样效果也不错）
使用方式：
1.多个专门的缓存池而不是一个大的缓存服务器，多个缓存池保障了高可用性，一个缓存实例挂掉了走其他的缓存实例，所有的缓存实例挂掉了，走数据库（估计数据库抗不住^_^）
2.所有的缓存池都用程序来维护，比如数据库有更新时，程序自动把更新后的内容同步到多个缓存实例中
3.服务器重启之后，缓存要比网站先启动，这就意味着当网站已经启动了，所有的缓存都可用
4.读取的请求可以负载均衡到多个缓存实例中去，高性能，高可靠性
潜在的问题：
1.你需要足够多的内存来存储那么多的数据
2.数据以行记录数据，而memcached以对象来存储数据，你的逻辑要把行列的数据转换成缓存对象
3.要维护多个缓存实例非常麻烦,Fotolog用Java/Hibernate，他们自己写了个客户端来轮询
4.管理多个缓存实例会增加应用程序的许多开销，但这些开销相对于多个缓存得到的好处来说算不了什么 
◦主动缓存数据魔法般的出现在缓存中，当数据库中有更新的时候，缓存立马填充，更新的数据被调用的可能性更高（比如一篇新文章，看的的人当然多），是非确定性缓存的一种变形(原文是It’s non-deterministic caching with a twist.我觉得这样翻译怪怪的)。主要应用在：1.预填充缓存：让memcached尽可能的少调用mysql如果内容不展现的话。
2.“预热”缓存：当你需要跨数据中心复制的时候
使用步骤：
1.解析数据库更新的二进制日志，发现数据库更新时对memcached也进行同样的更新
2.执行用户自定义函数，设置触发器调用UDF更新，具体参考[http://tangent.org/586/Memcached_Functions_for_MySQL.html](http://tangent.org/586/Memcached_Functions_for_MySQL.html)
3.使用BLACKHOLE策略，传说中Facebook也用mysql的Blackhole存储引擎来填充缓存，写到Blackhole的数据复制到缓存中，Facebook用这来设置数据作废以及跨国界的复制，好处是数据库的复制不走mysql，这就意味着没有二进制日志以及对CPU使用不那么多（啊？难道通过memcached存储二进制日志，然后复制到不同的数据库？有经验的同志在这个话题上可以补充。）
◦文件系统缓存把文件直接缓存在memcached中，哇，够BT的，减轻NFS的负担，估计只缓存那些过于热门的图片吧。 
◦部分页面内容缓存如果页面的某些部分获取起来非常费劲，以其缓存页面的原始数据还不如把页面的部分内容直接缓存起来直接调用 
◦应用程序级别的复制通过API来更新缓存，API的执行细节如下：1.一个应用把数据写到某个缓存实例，这个缓存实例把内容复制到其他缓存实例（memcached同步）2.自动获得缓存池地址以及实例个数
3.同时对多个缓存实例更新
4.如果某个缓存实例down掉了，跳到下一个实例，直到更新成功
整个过程非常高效以及低开销 
◦其他技巧1.多节点以应对”单点故障”2.使用热备技术，当某个节点down掉了，另外一台服务自动替换成它的IP，这样客户端不用更新memcached的IP地址3.memcached可以通过TCP/UDP访问，持续连接可以减轻负载，系统设计成可同时承受1000个连接
4.不同的应用服务，不同的缓存服务器群
5.检查一下你的数据大小是否匹配你分配的缓存，更多请参考[http://download.tangent.org/talks/Memcached%20Study.pdf](http://download.tangent.org/talks/Memcached%20Study.pdf)
6.不要考虑数据行缓存，缓存复杂的对象
7.不要在你的数据库服务器上跑memcached，两个都是吃内存的怪兽
8.不要被TCP延迟困扰，本地的TCP/IP对内存复制是做了优化的
9.尽可能的并行处理数据
10.并不是所有的memcached的客户端都是一样的，仔细研究你用的语言所对应的（好像php和memcached配合的不错）
11.尽可能的是数据过期而不是使数据无效，memcached可以设定过期时间
12.选择一个好的缓存标识key，比如更新的时候加上版本号
13.把版本号存储在memcached中 
　　作者最后的感言我就不翻译了，貌似mysql proxy正在做一个项目，自动同步mysql以及memcached，更多参考[http://jan.kneschke.de/2008/5/18/mysql-proxy-replicating-into-memcache](http://jan.kneschke.de/2008/5/18/mysql-proxy-replicating-into-memcache)
 ---★ 本文转摘自『IT学习者』→ [http://www.itlearner.com/article/4325](http://www.itlearner.com/article/4325)
[Memcached 集群架构方面的问题](http://kb.cnblogs.com/page/69074/)
这里收集了经常被问到的关于memcached的问题 
- [memcached是怎么工作的？](http://kb.cnblogs.com/page/69074/#how%20does%20memcached%20work)
- [memcached最大的优势是什么？](http://kb.cnblogs.com/page/69074/#what%27s%20the%20big%20benefit%20for%20all%20this)
- [memcached和MySQL的query cache相比，有什么优缺点？](http://kb.cnblogs.com/page/69074/#how%20does%20it%20compare%20to%20mysql%27s%20query%20cache)
- [memcached和服务器的local cache（比如PHP的APC、mmap文件等）相比，有什么优缺点？](http://kb.cnblogs.com/page/69074/#how%20does%20it%20compare%20to%20a%20server%20local%20cache)
- [memcached的cache机制是怎样的？](http://kb.cnblogs.com/page/69074/#What%20is%20memcached%27s%20cache)
- [memcached如何实现冗余机制？](http://kb.cnblogs.com/page/69074/#how%20is%20memcached%20redundant)
- [memcached如何处理容错的？](http://kb.cnblogs.com/page/69074/#how%20does%20memcached%20handle%20failover)
- [如何将memcached中item批量导入导出？](http://kb.cnblogs.com/page/69074/#how%20can%20you%20dump%20data%20from%20or%20load%20data%20into%20memcached)
- [但是我确实需要把memcached中的item都dump出来，确实需要把数据load到memcached中，怎么办？](http://kb.cnblogs.com/page/69074/#but%20I%20really%20need%20to%20dump%20and%20load%20data%20from%20memcached)
- [memcached是如何做身份验证的？](http://kb.cnblogs.com/page/69074/#how%20does%20memcached%27s%20authentication%20mechanisms%20work)
- [如何使用memcached的多线程是什么？如何使用它们？](http://kb.cnblogs.com/page/69074/#what%20are%20memcached%20threads)
- [memcached能接受的key的最大长度是多少？（250bytes）](http://kb.cnblogs.com/page/69074/#what%20is%20the%20maxiumum%20key%20length)
- [memcached对item的过期时间有什么限制？（为什么有30天的限制？）](http://kb.cnblogs.com/page/69074/#what%20are%20the%20limits%20on%20setting%20expire%20time)
- [memcached最大能存储多大的单个item？（1M byte）](http://kb.cnblogs.com/page/69074/#what%20is%20the%20maximum%20data%20size%20you%20can%20store)
- [为什么单个item的大小被限制在1M byte之内？](http://kb.cnblogs.com/page/69074/#why%20are%20items%20limited%20to%201%20megabyte%20in%20size)
- [为了让memcached更有效地使用服务器的内存，可以在各个服务器上配置大小不等的缓存空间吗？](http://kb.cnblogs.com/page/69074/#can%20I%20use%20different%20size%20caches%20across%20servers%20and%20will%20memcached%20use%20the%20servers%20with%20more%20memory%20efficiently)
- [什么是binary协议？它值得关注吗？](http://kb.cnblogs.com/page/69074/#what%20is%20the%20binary%20protocol)
- [memcached是如何分配内存的？为什么不用malloc/free！？究竟为什么使用slab呢？](http://kb.cnblogs.com/page/69074/#how%20does%20memcached%27s%20memory%20allocation%20work)
- [memcached能保证数据存储的原子性吗？](http://kb.cnblogs.com/page/69074/#is%20memcached%20atomic)
### [使用Memcache缓存MySQL查询](http://blog.csdn.net/kangqing2003/article/details/6202733)
分类： [Memcache](http://blog.csdn.net/kangqing2003/article/category/788124)
      实际应用中，尤其是在大规模访问的Web项目中，Memcache作为一种优秀的缓存解决方案，被广泛使用。其灵活性和可扩展性受到了广大开发者的青睐。为了提高响应速度，使用Memcache来减少数据查询运算是一种不错的选择。
      关于Memcache的安装和配置，网上有很多资料，这里就不再赘述。本文以笔者在实际项目中的应用为例，来说明如何使用Memcache缓存MySQL数据，从而达到高效数据响应的目的。
       需求背景：网站有一个图片放映功能，页面加载时需从数据库中查询管理员所提交放映的图片信息，图片放映区共有9张图片，对应在数据库表中有9条记录。每次用户访问网站时，需查询数据库，并显示图片。当承受大规模用户同时访问时，数据库的压力可想而之。
       为了降低数据库压力，提高查询速度，我采用了Memcache来将图片记录信息缓存到Memcache服务器上。这样就将数据库的查询压力分散到了各缓存服务器，提高了网站访问的整体效率。
     一、Memcache 服务器配置(内部集群模拟)
**[php]**[view
 plain](http://blog.csdn.net/kangqing2003/article/details/6202733#)[copy](http://blog.csdn.net/kangqing2003/article/details/6202733#)
- $MEMCACHE_SERVERS[] = '192.168.1.78:11211';  
- $MEMCACHE_SERVERS[] = '192.168.1.78:11212';  
- $MEMCACHE_SERVERS[] = '192.168.1.252:11211';  
- $MEMCACHE_SERVERS[] = '192.168.1.252:11212';  
- //如果要添加Memcache节点，就在此处添加即可。
- 
- $MEMCACHE_LIFETIME = 6000;   // 缓存数据生命周期
    我采用了两台机器作为Memcache Server，每台开启两个端口提供Memcache服务，每个服务开启服务内存为1G。这样，缓存空间总的有4G。
   二、php连接Memcache服务器
   php中和Memcache的交互有两种：1、采用php下的memcache扩展；2、采用libmemcache组件。两者的优劣在此先不作对比。
**[php]**[view
 plain](http://blog.csdn.net/kangqing2003/article/details/6202733#)[copy](http://blog.csdn.net/kangqing2003/article/details/6202733#)
- $_SGLOBAL['memcache'] = new memcache;  
- global$MEMCACHE_SERVERS;  
- $result = array();  
- foreach ($MEMCACHE_SERVERSas$server){  
- $serverstr = explode(':',$server);  
- $host = $serverstr[0];  
- $port = $serverstr[1];  
- 
- $_SGLOBAL['memcache']->addServer($host,$port);  
- }   
   这段程序从刚刚的memcache服务器配置文件中读取所有的server列表，并创立连接。
   三、php实现memcache和mysql的缓存查询
   实现原理其实很简单，在我的数据库操作类中新增一个函数query_memcache($sql,$type)，以$sql的MD5值作为Key，从 Memcache服务器上查询是否存在该key的值，如果存在，则直接返回；如果不存在，则从MySQL数据库中查询，并将结果写入Memcache。
**[php]**[view
 plain](http://blog.csdn.net/kangqing2003/article/details/6202733#)[copy](http://blog.csdn.net/kangqing2003/article/details/6202733#)
- function query_memcache($sql,$type=''){  
- $key = md5($sql);  
- 
- if(!($value = $_SGLOBAL['memcache']->get($key))){ //Cache中没有,则从My SQL中查询
- $query = $this->query($sql,$type);  
- while($item = $this->fetch_array($query)){  
- $result[] = $item;  
-             }  
- $value = $result;  
- 
- //将Key和Value写入MemCache
- $_SGLOBAL['memcache']->set($key,$result,0,$MEMCACHE_LIFETIME);  
-         }  
- return$value;  
-     }  
   在页面加载的过程中，将图片检索的数据库查询调用改成用query_memcache()函数即可。
   到此，我们就实现了用Memcache缓存首页数据信息，经测试，在大规模的并发访问下，首页加载速度快了许多。
   用计时器测试单台客户端访问响应时间如下：
![](http://hi.csdn.net/attachment/201102/23/0_12984561032bI1.gif)
  (红线框内的是第一次从数据库中读取数据的耗时，当数据存入Memcache后，读取数据耗时更短【蓝线框的数据】)
