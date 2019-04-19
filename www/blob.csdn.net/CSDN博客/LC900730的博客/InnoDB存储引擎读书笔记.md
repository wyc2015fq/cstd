# InnoDB存储引擎读书笔记 - LC900730的博客 - CSDN博客
2017年05月18日 21:42:42[lc900730](https://me.csdn.net/LC900730)阅读数：181
./mysql –help| grep my.cnf 
查看mysql实例启动时候，它会在哪些地方查找配置文件 
/etc/my.cnf /etc/mysql/my.cnf /usr/local/mysql/etc/my.cnf ~/.my.cnf 
配置文件中datadir该参数指定了数据库所在的路径；
### 数据库：
从概念上来讲：数据库是文件的集合，是依照某种数据模型组织起来并且存放在二级存储中的数据集合(一个个文件，一般来说是二进制文件)
### 数据库实例：
是应用程序，是用户与操作系统之间的一层数据管理软件，用户对数据库数据的任何操作，包括数据库定义、数据查询、数据维护、数据库控制运行等都是在数据库实例下进行的；应用程序只能通过数据库实例才能和数据库打交道
### 连接池组件
### 管理服务和工具组件
### SQL接口组件
### 查询分析器组件
### 优化器组件
### 缓冲组件
### 插件式存储引擎
：mysql特有
### 物理文件
其中存储引擎是基于表的，而不是数据库。
### InnoDB存储引擎
将数据放在逻辑表空间，这个表空间像黑盒一样，由innodb管理。每张表的存储按主键顺序存放，如果没有显示在表定义时候知道主键，InnoDB存储引擎会为每一行生成一个6字节的ROWID作为主键
### 连接mysql
连接mysql操作是连接进程和Mysql数据库实例进行通信。从本质上来讲，是进程间通信。 
- TCP/IP
是 mysql在任何平台上都提供的连接方式，也是网络上使用最多一种方式，这种方式是TCP/IP上建立一个基于网络的连接； 
如mysql -h192.168.0.101 -u root -p; 
在通过TCP/IP连接，MySQL会检查一张权限试图，用来判断发起请求的客户端IP是否允许连接到MySQL实例
### Unix套接字
Unix域套接字不是网络协议，所以只能在MySQL客户端和数据库实例在一台服务器上使用。通过配置文件的套接字文件路径，如-socket=/tmp/mysql.sock.当数据库实例启动之后，可以通过 
show variables like ‘socket’来查找Unix域套接字； 
可以使用mysql -uroot -S /tmp/mysql.sock
### InnoDB体系
- 维护所有进程和线程需要访问的多个内部数据结构
- 缓存磁盘上数据，方便快速读取，并且对磁盘文件的数据进行修改之前在这里缓存
- 重做日志缓冲
### 后台线程
后台线程的主要作用是负责刷新内存池中的数据，保证缓冲池中的内存缓存的是最近的数据。将已经修改的数据文件刷新到磁盘文件，同时保证数据库发生异常情况下InnoDB能恢复到正常运行状态 
默认情况下，InnoDB存储引擎后台线程有7个—-4个IO 1个master 
1个锁监控线程 1个错误监控线程 
IOthread的数量由配置文件中的innodb_file_io_threads参数控制，默认是4； 
- insert buffer thread
- log thread
- read thread
- write thread
### 内存
InnoDB存储引擎内存由以下几个部分组成：缓冲池(buffer bool)、重做日志缓冲池(redo log buffer)以及额外的内存池(additional memory pool),分别由配置文件中的参数innodb_buffer_pool_size和innodb_log_buffer_size的大小决定。 
缓冲池是占最大内存的部分，用来存放各种数据的缓存；因为InnoDB的存储引擎工作方式是将数据库文件按照页（每页16K）读取到缓冲池，然后按照最近最少使用算法来保留在缓冲池中的缓存数据（LRU）。如果数据需要修改，总是修改在缓存池中的页（发生修改后，该页为脏页），然后按照一定频率将缓冲池中脏页刷新(flush)到文件。 
通过命令SHOW ENGINE INNODB STATUS 
查看innodb_buffer_pool使用情况； 
在BUFFER POOL AND MEMORY里可以看到InnoDB存储引缓冲的使用情况；buffer pool size表面了一共多少缓冲帧(buffer frame),每个buffer frane为16K,单位是字节。 
Free buffers表示当前空闲的缓冲帧 
Database pages表示已经使用的缓存帧 
Modified db pages表示脏页的数量
缓存池中的数据页类型
- 索引页
- 数据页
- undo页
- 插入缓重(insert buffer)
- 自适应哈希(adaptive hash index)
- InnoDB存储的锁信息（lock info）
- 数据字典信息(data dictionary)
缓冲池中数据页和索引页占很大部分 
其中：
### 日志缓冲
将日志信息放入这个缓冲区，然后按照一定频率将其刷新到重做日志文件。一般是每秒刷新一次；
### 额外内存池
InnoDB对内存的管理是通过内存堆(heap)的方式进行的。 
在对一些数据结构本身分配内存时，需要从额外内存池中申请，当该区域不够，从缓冲池中申请。
## master thread
InnoDB存储引擎的主要工作是在单独的一个线程中后台线程master thread中完成的； 
优先级最高，其内部由几个循环(loop)组成：主循环loop、后台循环background loop、刷新循环flush loop 、暂停循环suspend loop。master thread会根据数据库的状态在loop background loop flush loop和suspend loop中进行切换。
### 主循环loop
大多数操作在这个循环中，有2大部分，每秒钟的操作和每10秒的操作 
void master_thread(){ 
    loop: 
    for(int i=0;i<10;i++){ 
        do thing once per second 
        sleep 1 second if necessary 
    } 
    do thing once per 10 second 
    goto loop; 
} 
每秒一次的操作包括： 
- 日志缓冲刷新到磁盘，即使没有提交(总是)
- 合并插入缓冲(可能)
- 至多刷新100个InnoDB的缓冲池中脏页到磁盘(磁盘)
- 如果当前没有用户活动，切换到backgroup loop(可能)
每10秒一次的操作包括： 
- 刷新100个脏页到磁盘(可能)
- 合并至多5个插入缓冲(可能)
- 将日志缓冲刷新到磁盘(总是)
- 刷新无用的Undo页(总是)
- 刷新100个或者10个脏页到磁盘(总是)
- 产生1个检查点(总是)
在上述过程中，InnoDB存储引擎会先判断过去10秒之内磁盘的IO操作是否小于200次，如果是InnoDB任务当前有足够的IO能力，因此将100个脏页刷到磁盘。接着InnoDB存储引擎会合并插入缓冲。不同于每1秒操作时候可能发生的合并插入缓冲操作，这次的合并插入缓冲操作总是在这个阶段进行。之后InnoDB存储引擎会在执行一次将日志刷新到磁盘的操作。 
        接着InnoDB存储引擎会执行一步full purge操作，即删除无用的Undo页。对表执行update dalete这类操作，原先行被标记删除，但是因为一致性读(consitent read)的关系，需要保留这些行的信息。
## 关键特性
包括插入缓冲、两次写、自适应哈希索引 
- 插入缓冲：InnoDB缓冲池中有Insert Buffer信息固然不错，但是Insert Buffer和数据页一样，也是物理页的一个组成部分。
主键是行唯一的标识符，在应用程序中行记录的插入顺序是按照主键递增的顺序进行插入的，因此插入聚集索引一般是顺序的，不需要磁盘随机读写。 
在进行插入操作，数据页的存放还是按主键id的执行顺序存放，但是对于非聚集索引，叶子节点的插入不是顺序的了。
## MySQL文件
- >参数文件：告诉MySQL实例启动的时候在哪里可以找到数据库文件，并且指定某些初始化参数，这些参数定义了某种内存结构的大小等设置
- 日志文件：用来记录MySQL实例对某种条件做出响应时候写入的文件。如错误日志文件，二进制日志文件，慢查询日志文件，查询日志文件
- socket文件：当用Unix域套接字方式进行连接时候需要的文件
- pid文件：MySQL实例的进程id文件
- MySQL表结构文件：用来存放MySQL表结构定义文件
- 存储引擎文件：因为MySQL表存储引擎的关系，每个存储引擎都会有自己的文件来保存数据。这些存储引擎真正存储了数据和索引等数据
### 3.1参数文件
当MySQL实例启动的时候，会读一个配置参数文件，用来寻找数据库的各种文件所在的位置以及指定某些初始化参数，这些参数通常定义了某种内存结构有多大等设置。 
MySQL参数文件是文本的；
#### 参数
可以当做键值对。
### 日志文件
常见有错误日志、二进制日志、慢查询日志、查询日志
#### 错误日志
默认情况下是err结尾； 
show variables like ‘log_error’ 
log_error /usr/local/mysql/data/stargazer.err. 
默认情况下错误文件名为服务器的主机名；
#### 慢查询日志
可以设置一个阈值，将运行时间超哥该值的所有SQL语句都记录到慢查询日志文件中。该阈值可以通过参数long_query_time来设置，默认是10，代表10秒。默认情况下mysql数据库不启动慢查询日志。 
另一个和慢查询日志有关的是log_queries_not_using_indexes,如果运行的语句没有使用索引，则Mysql会将这条SQL语句记录到慢查询日志文件。 
首先确认打开log_queries_not_using_indexes
### mysqldumpslow命令
### 5.1开始，可以将慢查询的日志记录放入一张表中，名为slow_log
参数log_output指定了慢查询输出的格式，默认是FILE，你可以设置为TABLE
### 查询日志
记录索引对MySQL数据库请求的信息，不论这些请求是否得到正确执行。可以修改配置文件放入general_log表
### 二进制日志
记录了对数据库执行更改的所有操作，但是不包括select show，此外二进制还包括了执行数据库更改操作的时间和执行时间等。作用有 
恢复：某些数据恢复需要二进制日志，如当一个数据库全备文件恢复后，我们可以通过二进制日志执行point-in-time的恢复 
复制：原理与恢复类似。 
show variables like ‘datadir’ 
bin_log.000001:指定了名字，所以没有使用默认名称 
bin_log.index:二进制的索引文件，用来存储过往产生的二进制日志序号。 
默认情况下，二进制日志文件没有启动。 
以下文件参数影响着二进制日志记录的信息和行为 
- max_binlog_size
system ls -lh /usr/local/mysql/data/* 
查看数据库的所有库和某些文件如ib_logfile 
max-binlog-size指定了单个二进制日志文件的大小，如果超过该值，产生新的二进制日志文件，后缀名+1，并记录到.index文件。 
使用事务的表存储引擎(如innodb)，所有未提交的二进制日志会被记录到一个缓存中，等该事务提交时直接将缓冲中的二进制日志写入二进制文件，而该缓冲的大小由binlog-cache-size决定，默认大小32KB。此外binlog_cache_size是基于会话 的，也就是说当一个线程开始时候，MySQL会自动分配一个大小为binlog-cahe——size缓存，因此该值需要小心被设置当一个事务的记录大于设定的binlog——cache——size时候，MySQL会将缓冲中的日志写入一个临时文件中，因此该值不能被设置的太小。 
SHOW GLOBAL STSTUS 命令查看： 
binlog_cache_use ：记录了使用缓冲写二进制的次数； binlog_cache_disk_use：记录了使用临时文件写二进制日志的次数； 
的状态，可以判断当前binlog_cache_size的值设置的是否合理。 
默认情况下，二进制日志并不是在每次写的时候同步到磁盘(我们可以理解为缓冲写)。当数据库所在 操作系统发生宕机，可能会有最后一部分数据没有写入二进制日志文件中，给恢复和复制带来问题。参数sync_binlog=[N]表示每写缓冲多少次就同步到磁盘。如果N=1那么表示采用同步写磁盘的方式来写二进制日志。 
问题：一个事务commit之前，由于设置为1，因此写入二进制到磁盘，但是提交还没有发生发生了宕机，那么MySQL下次启动，因为没有commit所以会回滚，但是二进制日志记录了该事务信息，不能被回滚。这个问题通过 innodb_support_xa设置为1来解决 
如果当前数据库是复制中的slave角色，那么它不会从master取得并执行的二进制日志写入自己的二进制日志文件中。 
- binlog_cache_size
- sync_binlog
- binlog_do_db
- binlog_ignore_db
- log_slave_update
- binlog_format
### B+树
##### 索引并不能找到一个给定键值的具体行。B+树索引能找到的只是被查数据的行所在的页。然后数据库通过把页读入内存，再在内存中进行查找，最后找到查找的数据。
