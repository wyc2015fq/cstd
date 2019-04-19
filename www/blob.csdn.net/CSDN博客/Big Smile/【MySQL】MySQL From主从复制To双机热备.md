# 【MySQL】MySQL From主从复制To双机热备 - Big Smile - CSDN博客
2018年07月31日 16:33:39[王啸tr1912](https://me.csdn.net/tr1912)阅读数：524
所属专栏：[mysql系列](https://blog.csdn.net/column/details/26482.html)
        安装完mysql之后，有了新的需求，就是需要有一台服务器用来做数据备份使用，以达到容灾的目的，这样需要怎么做呢？还有我们需要进行多点访问来减小单点压力的时候又要怎么办呢？这个时候我们就引入了主从复制和双机热备的概念了。
        其实从实现上来讲，双机热备就是我们做了两个机器的主从互备。下面我们从原理上开始说，这次试用的mysql版本依旧是5.7.23，在网上看配置方法后，感觉和5.5以来的版本似乎都没有什么差别，在此提示。
# 一、主从复制原理
         主从复制的根本操作就是把从主服务器上所执行过的sql在从机上再执行一遍，只要两个机器的数据库**初态（数据库结构，数据，配置）**是一样的，那么我们开启主从复制之后就能保证他们一直都是一样的状态。这些都是mysql自己实现的，我们就配置一下就可以了。
首先看下面一张图：
![](https://img-blog.csdn.net/2018073114531832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
         从图中我们可以看出来了，主服务器需要做的事情非常简单的，就只是把执行的sql语句存储到二进制文件binary-log中，而从服务器需要做的事情比较多，而且还得使用两个线程进行整个事件的监听和处理。
我们的主从复制一共分为三步：
- master将改变记录到二进制日志（binary log）。这些记录过程叫做二进制日志事件，binary log events。
- slave将master的binary log events拷贝到它的中继日志（relay log）。
- slave重做中继日志中的事件，将改变应用到自己的数据库中。 MySQL复制是异步的且串行化的。
**基本原则**
   由于主从复制的一些特性，为了保证数据一一致，所以这里有些原则需要遵守：
- 每个slave只有一个master
- 每个slave只能有一个唯一的服务器ID
- 每个master可以有多个salve
# **二、搭建主从服务**
**前提：**
       1、两台装有相同版本mysql的服务器，分别是——M：192.168.65.135，S：192.168.65.136。
配置教程：[https://blog.csdn.net/tr1912/article/details/81270406](https://blog.csdn.net/tr1912/article/details/81270406)
       2、都没有配置过主从的集群。
       3、防火墙3306端口开放（使用的默认端口）。
       4、两台服务器的数据表结构，配置都必须是一样的。
**（一）、主服务器设置**
**1、创建一个从服务器访问需要使用的用户，并授权。**
  我们登陆到主服务器的mysql上，并执行如下语句即可：
```
CREATE USER 'slave'@'192.168.65.%' IDENTIFIED BY 'slave@123.com';
GRANT REPLICATION SLAVE ON *.* TO 'repl'@'192.168.65.%';
```
注意，这里创建用户的时候可能会遇到弱密码提示，请参照：
[https://blog.csdn.net/tr1912/article/details/81299360](https://blog.csdn.net/tr1912/article/details/81299360)
**2、修改my.cnf配置文件**
```
[mysqld]
log-bin=mysql-bin
server-id=1
```
**说明：**
     1. log-bin:开启二进制日志，该日志是在事务提交时写日志文件的。默认大小是1G，后面加001,002这样的后缀顺加。 
     2. server-id，唯一标识主机，mysql主从每个mysql实例配置都不一样就行。这个值默认是0，如果是0，主服务器拒绝任何从服务器的连接。 
**其他配置（不是必须配置的）：**
     1、binlog-do-db=db_001       #配置需要同步的数据库，可以配置多个，如果没有此配置项则同步全部。
     2、binlog-ignore-db=mysql    #配置不同步的数据库，可以配置多个。
     3、binlog_format = mixed     #配置binlog的格式
     4、read-only = 0                   #配置是否只读  0代表不只读，1代表只读
     5、auto-increament-increment = 10    #用于设定双主情况下自增列的ID冲突使用的，主要用来设置自增步长
     6、auto-increment-offset = 1               #表示这台服务器的序号，从1开始，不超过auto-increament-increment
**3、同步数据（非必须）**
       我们做主从同步，在最开始的时候，必须保证主从上的数据表的结构和数据都是一样的，所以如果有数据不一样的情况，我们需要进行这一步。
       首先执行master的数据dump备份：
`mysqldump --all-databases --master-data -uroot -p > /tmp/dbdump.db`
      然后我们在设定的目录下拿到dump备份后，复制到从机上进行还原：
`mysql -uroot -p < /tmp/dbdump.db `
这样就搞定了主机的配置以及数据的同步。
**（二）、从机配置**
         从机的配置也很简单。
**      1、配置my.cnf**
```
[mysqld]
server-id=2
binlog-do-db=db_001
binlog-ignore-db=mysql
skip-slave-start=true
```
这样我们的从机就已经配置好了。
**2、修改UUID（非必须）**
      如果你使用的是虚拟机，而且从机是从master主机克隆出来的，那么你就需要进行这一步的操作了。 
     首先到我们mysql的数据库目录下：
`cd /var/lib/mysql`
     然后我们会找到auto.cnf这个配置文件：
![](https://img-blog.csdn.net/20180731160314702?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
 修改里面的uuid为一个不同的即可：
![](https://img-blog.csdn.net/20180731161009206?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**（三）、配置运行**
由于我们的主从都已经修改过配置文件了，所以都需要重新启动一下：
`service mysqld restart`
然后我们才能进行接下来的配置。
**  1、获取binlog的信息**
我们先到主（master）服务器上获取binlog的信息，在mysql的命令界面输入：
`show master status;`
![](https://img-blog.csdn.net/20180731161526920?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
        这里展示的就是我们当前主服务器使用的binlog的文件名，其中position是文件中偏移量，我们之后配置slave需要用到这些信息。
**2、配置从服务器认主**
我们需要在从服务器上配置需要连接的主服务器的信息，执行如下sql命令：
```
CHANGE MASTER TO
MASTER_HOST='192.168.65.135',
MASTER_USER='slave',
MASTER_PASSWORD='slave@123.com',
MASTER_LOG_FILE='mysql-bin.000003',
MASTER_LOG_POS=419;
```
         这里user和password就是我们第一步在主服务器上创建的用户名和密码，然后MASTER_LOG_FILE 就是我们上一步查看到的master在使用的binlog文件（这个文件在每次主服务器状态变化后都不同），MASTER_LOG_POS   就是binlog的偏移量，用于同步扫描使用。
**3、启动从服务**
        最后我们在从服务器的mysql命令中输入：
`start slave;`
就可以启动我们的从服务器进行同步了。我们可以查看状态：
`show slave status\G;`
![](https://img-blog.csdn.net/20180731162400827?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
看到图中的两个yes，就表示我们的从服务器已经在完全运行了。
现在我们可以在主服务器的数据库上做一个修改数据看看是否同步到从服务器的数据库上来证明可用性。
# **三、双机热备实现**
        我们实现双机热备，原理其实就是做两个机器的互相主从，我们把上述步骤主从对调，然后做一遍就能实现了双机热备了，实现图为：
![](https://img-blog.csdn.net/20180731163118697?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
我们的两个节点互为主从，然后都可以接收读写的操作。
如果只是单点的读的话，我们还需要设置Slave节点为readonly，即slave节点的 my.cnf 设置 read-only=1
