# 如何生成唯一的server Id，server_id为何不能重复？ - z69183787的专栏 - CSDN博客
2018年07月23日 16:12:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：613
[https://www.cnblogs.com/wajika/p/6710657.html](https://www.cnblogs.com/wajika/p/6710657.html?utm_source=itdadao&utm_medium=referral)
我们都知道MySQL用server-id来唯一的标识某个数据库实例，并在链式或双主复制结构中用它来避免sql语句的无限循环。这篇文章分享下我对server-id的理解，然后比较和权衡生成唯一server-id的几种方式。
## server_id的用途
简单说来，server_id有两个用途： 
1. 用来标记binlog event的源产地，就是SQL语句最开始源自于哪里。 
2. 用于IO_thread对主库binlog的过滤。如果没有设置 [replicate-same-server-id=1](http://dev.mysql.com/doc/refman/5.6/en/replication-options-slave.html#option_mysqld_replicate-same-server-id) ，那么当从库的io_thread发现event的源与自己的server-id相同时，就会跳过该event，不把该event写入到relay log中。从库的sql_thread自然就不会执行该event。这在链式或双主结构中可以避免sql语句的无限循环。
注意：相同server-id的event在io_thread这一层就过滤了；而对于replicate-(do|ignore)-等规则，则是在sql_thread这一层过滤的。io_thread和sql_thread都有过滤的功能。
## server_id为何不能重复
在同一个集群中，server-id一旦重复，可能引发一些诡异问题。 
看看下面两种情况：![](https://images2015.cnblogs.com/blog/43136/201704/43136-20170414200051408-1606370165.png)
图1：主库与从库的server-id不同，但是两个或多个从库的server-id相同
这种情况下复制会左右摇摆。当两个从库的server-id相同时，如果从库1已经连接上主库，此时从库2也需要连接到主库，发现之前有server-id相同的连接，就会先注销该连接，然后重新注册。 
参考下面的代码片段：
repl_failsafe (register_slave) [download](http://nettedfish.sinaapp.com/downloads/code/register_slave)
|1234567891011121314151617|```int register_slave(THD* thd, uchar* packet, uint packet_length){  int res;  SLAVE_INFO *si;...  if (!(si->master_id= uint4korr(p)))    si->master_id= server_id;  si->thd= thd;  pthread_mutex_lock(&LOCK_slave_list);/* 先注销相同server-id的连接*/  unregister_slave(thd,0,0); /* 重新注册*/  res= my_hash_insert(&slave_list, (uchar*) si);  pthread_mutex_unlock(&LOCK_slave_list);  return res;...}```|
两台从库不停的注册，不停的注销，会产生很多relay log文件，查看从库状态会看到relay log文件名不停改变，从库的复制状态一会是yes一会是正在连接中。
图2：链式或双主结构中，主库与从库的server-id相同
从库1同时又是relay数据库，它能正确同步，然后把relay-log内容重写到自己的binlog中。当server-id为100的从库2 io线程获取binlog时，发现所有内容都是源自于自己，就会丢弃这些event。因此从库2无法正确同步主库的数据。只有直接写relay server的event能正确同步到从库2。 
上面两种情况可以看到，在同一个replication set中，保持server-id的唯一性非常重要。
## server_id的动态修改
无意中发现 [server-id](http://dev.mysql.com/doc/refman/5.6/en/replication-options.html#option_mysqld_server-id) 竟然是可以动态修改的，可别高兴的太早。好处是，上面图1的情况下，直接修改其中一个从库的server-id就可以解决server-id冲突的问题。坏处很隐蔽，如下图的结构： 
![](https://images2015.cnblogs.com/blog/43136/201704/43136-20170414200410236-635818646.png)
现在假设active-master因为某种原因与passive-master的同步断开后，passive-master上进行了一些ddl变更。然后某dba突发奇想把passive-master的server-id修改为400。当双master的复制启动后，那些之前在passive-master上执行的server-id为200的ddl变更，会从此陷入死循环。如果是 `alter table t engine=innodb` ，它会一直不停，可能你会发现。但是像 `update a=a+1；` 这样的sql，你很难发现。当然这种场景只是我的杜撰，这儿有个更真实的例子 [主备备的两个备机转为双master时出现的诡异slave lag问题](http://hatemysql.com/2010/10/15/%E4%B8%BB%E5%A4%87%E5%A4%87%E7%9A%84%E4%B8%A4%E4%B8%AA%E5%A4%87%E6%9C%BA%E8%BD%AC%E4%B8%BA%E5%8F%8Cmaster%E6%97%B6%E5%87%BA%E7%8E%B0%E7%9A%84%E8%AF%A1%E5%BC%82slave-lag%E9%97%AE%E9%A2%98/) 。 
举这两个例子只是想说明修改server-id有点危险，最好不要去修改，那么能一步到位生成它吗？
## 如何生成唯一的server_id
常用的方法有如下几种：
### 1. 采用随机数
mysql的server-id是4字节整数，范围从0-4294967295，因此采用该范围内的随机数来作为server-id产生冲突的可能性是非常小的。
### 2. 采用时间戳
直接用date +%s来生成server-id。一天86400秒来计算，往后计算50年，最大的server-id也才使用到86400*365*50，完全在server-id范围内。
### 3. 采用ip地址+端口
这是我们经常采用的方法。例如ip为192.168.122.23，端口为3309，那么server-id可以写为122233309。产生冲突的可能性比较小：遇到*.*.122.23 或者*.*.12.223，而且搭建了同一个replication set的3309才会出现。
### 4. 采用集中的发号器
在管理服务器上采用自增的id来统一分配server-id。这可以保证不冲突，但是需要维护中心节点。
### 5. 分开管理每个replication set
在每个replication set中为mysql库增加一个管理表，保证每个从库的server-id不冲突。
## 哪一种更好
上面的几种方法都不赖，但是：
1. 方法4加了维护负担，而且开发环境、测试环境、线上环境都维护一套发号器的话，有点麻烦，混在一起又可能遇到网段隔离的风险，还有发号器数据库权限的问题难于控制。所以不推荐。
2. 方法5实现了自治，但是管理成本有点高。从库要能够写主库的server-id表，复杂。
3. 5种方法都存在的问题是，使用冷备的数据来扩容，server-id需要手动去修改，否则就与冷备源的server-id冲突。而且，当mysql启动的时候，你无法判断该mysql是刚通过备份扩容的，还是之前一直正常运行的。所以你不知道这个server-id到底要不要改。而我希望server-id对dba完全透明，又绝不产生冲突，即可彻底屏蔽这个讨厌的东西。
## 建议的方法
其实很简单。ipv4是4字节的整数，与server-id的范围完全一样。我们认为只有ip地址+端口才能唯一的确定一个mysql实例，所以总是希望把ip信息和端口信息都集成到server-id中。但是别忘了，一个ip上不能同时启动两个一样的端口。所以，server-id只需采用ip地址的整数形式！自定义的mysql启动脚本强制对server-id进行检查，发现server-id不对就进行纠正，然后启动。上面所有的问题，都会迎刃而解。
