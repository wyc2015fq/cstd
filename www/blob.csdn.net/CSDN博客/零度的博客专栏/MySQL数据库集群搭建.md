# MySQL数据库集群搭建 - 零度的博客专栏 - CSDN博客
2016年04月28日 11:14:33[零度anngle](https://me.csdn.net/zmx729618)阅读数：510
## MySQL Cluster简介
      MySQL cluster 和 Oracle RAC 完全不同，它采用 无共享架构Shared nothing（shared nothing architecture）。整个集群由*管理节点(ndb_mgmd)，处理节点(mysqld)和存储节点(*ndbd)组 成，不存在一个共享的存储设备。MySQL cluster 主要利用了 NDB 存储引擎来实现，NDB 存储引擎是一个内存式存储引擎，要求数据必须全部加载到内存之中。数据被自动分布在集群中的不同存 储节点上，每个存储节点只保存完整数据的一个分片(fragment)。同时，用户可以设置同一份数据保存在多个不同的存储节点上，以保证单点故障不会造
 成数据丢失。MySQL cluster 主要由 3 各部分组成：
- *SQL 服务器节点*
- *NDB 数据存储节点*
- *监控和管理节点*
这样的分层也是与 MySQL 本身把 SQL 处理和存储分开的架构相关系的。MySQL cluster 的优点在于其是一个分布式的数据库集群，处理节点和存储节点都可以线性增加，整个集群没有单点故障，可用性和扩展性都可以做到很高，更适合 OLTP 应用。但是它的问题在于：
- *NDB（“NDB” 是一种“内存中”的[存储引擎](http://baike.baidu.com/view/1238930.htm)，它具有可用性高和[数据一致性](http://baike.baidu.com/view/4138648.htm)好的特点。） 存储引擎必须要求数据全部加载到内存之中，限制比较大，但是目前 NDB 新版本对此做了改进，允许只在内存中加 载索引数据，数据可以保存在磁盘上。*
- * 目前的 MySQL cluster 的性能还不理想，因为数据是按照主键 hash 分布到不同的存储节点上，如果应用不是通过主键去获取数据的话，必须在所有的存储节点上扫描， 返回结果到处理节点上去处理。而且，写操作需要同时写多份数据到不同的存储节点上， 对 节点间的网络要求很高。*
虽然 MySQL cluster 目前性能还不理想，但是 share nothing 的架构一定是未来的趋势，Oracle 接手 MySQL之后，也在大力发展 MySQL cluster，我对 MySQL cluster 的前景抱有很大的期待。
[回到顶部](http://www.cnblogs.com/baiboy/p/4646849.html#_labelTop)
## mysql-cluster构造
安装mysql-cluster首先要了解cluster的构造,Cluster大体上是由管理节点（mgmd）、数据节点（ndbd）、sql节点（mysqld）组成的。
![](http://images0.cnblogs.com/blog2015/380252/201507/142226009858889.png)
[回到顶部](http://www.cnblogs.com/baiboy/p/4646849.html#_labelTop)
## 虚拟机安装centos6.6
**系统要求：**
*内存：4G（最少4G）*
*CPU：需要支持VT*
*如果你是笔记本用户那么cpu虚拟化可能是关闭的，请在BIOS下设置virtualization 为enable*
新建虚拟机，选择典型安装，点击下一步
![](http://images0.cnblogs.com/blog2015/380252/201507/142226126101638.png)
点击下一步
![](http://images0.cnblogs.com/blog2015/380252/201507/142228312353521.png)
这里用户名最好是mysql因为后面要用到。设置成其他用户也可以。
下一步
![](http://images0.cnblogs.com/blog2015/380252/201507/142228408458086.png)
位置就是保存这个虚拟centos系统的位置
下一步
![](http://images0.cnblogs.com/blog2015/380252/201507/142228571888058.png)
选择如图所示，下一步
![](http://images0.cnblogs.com/blog2015/380252/201507/142229062981710.png)
选择自定义硬件，建议最好每个虚拟机分配1G的内存。每个cent最少分配512M内存。
点击完成就可以了。
虚拟机安装完成后，用root权限进入。密码和mysql账户的密码相同。
[回到顶部](http://www.cnblogs.com/baiboy/p/4646849.html#_labelTop)
## mysql-cluster下载
[www.mysql.com/downloads/cluster](http://www.mysql.com/downloads/cluster%20%20%E9%80%89%E6%8B%A9linux-generic%E4%B8%8B%E7%9A%84%E7%89%88%E6%9C%AC%20%E6%A0%B9%E6%8D%AE%E4%BD%A0%E7%9A%84cent%E7%9A%84%E7%B3%BB%E7%BB%9F%E9%80%89%E6%8B%A932or64) 选择linux-generic下的版本 根据你的cent的系统选择32or64
[回到顶部](http://www.cnblogs.com/baiboy/p/4646849.html#_labelTop)
## centos6.6搭建环境配置
首先进入linux终端，右键桌面如图所示
![](http://images0.cnblogs.com/blog2015/380252/201507/142229329702862.png)
Open in terminal（终端）
安装mysql-cluster前首先要卸载原有系统自带的mysql
联网卸载（终端下执行命令）
#yum remove mysql
手动清理（终端下执行命令）
#rpm –qa|grep mysql* （这条是查询系统中存在的mysql）
#rpm –e –nodeps mysql-libs-5.1.71-1.el6.x86_64 (不同版本的centos这个版本也不同)
[回到顶部](http://www.cnblogs.com/baiboy/p/4646849.html#_labelTop)
## mysql-cluster安装包导入cent中
首先查询需要导入的cent的ip地址，终端下输入
#ifconfig
![](http://images0.cnblogs.com/blog2015/380252/201507/142229450015552.png)
如图所示其中inet addr：192.168.191.129就是这个cent的ip地址。
下载软件winscp
![](http://images0.cnblogs.com/blog2015/380252/201507/142229552514288.png)
打开软件
![](http://images0.cnblogs.com/blog2015/380252/201507/142230014703955.png)
新建站点，主机名就是cent 的ip ，用户名使用root
点击登录，输入密码，连接成功
![](http://images0.cnblogs.com/blog2015/380252/201507/142230157045772.png)
左边是windows右边是cent
Cent下进入目录/usr/local/src 把你的mysql-cluster安装文件放在这个目录下
打开cent终端
添加mysql用户和组（如果前面安装虚拟机的时候使用的用户是mysql则跳过此步骤
添加mysql用户
1.      # groupadd mysql 
2.      # useradd mysql -g mysql
安装mysql-cluster 7.4.4-linux
1.      # cd /usr/local/src/（已下载好集群版）  
2.      # tar -xvf mysql-cluster-gpl-7.4.4-linux-glibc2.5-x86_64.tar.gz  
3.      # mv mysql-cluster-gpl-7.4.4-linux-glibc2.5-x86_64 ../mysql 
4.      # cd .. 
5.      # chown -R mysql:mysql mysql/ 
6.      # cd mysql 
7.      # scripts/mysql_install_db --user=mysql  
![](http://images0.cnblogs.com/blog2015/380252/201507/142233508139254.png)
当出现这个页面则mysql-cluster安装成功
以上步骤是安装mysql-cluster必须步骤，所有机器都要进行以上的步骤
[回到顶部](http://www.cnblogs.com/baiboy/p/4646849.html#_labelTop)
## 集群配置
### 首先进行管理节点的配置(终端输入)
# vi  /var/lib/mysql-cluster/config.ini （目录和文件没有请新建，添加以下内容）
```
1.      [NDBD DEFAULT] 
2.      NoOfReplicas=2 
3.      [TCP DEFAULT] 
4.      portnumber=3306 
5.       
6.      [NDB_MGMD] 
7.      #设置管理节点服务器  
8.      nodeid=1 
9.      HostName=192.168.191.132 （管理节点服务器ip地址）
10.   DataDir=/var/mysql/data 
11.    
12.   [NDBD] 
13.   nodeid=2 
14.   HostName=192.168.191.130 （ndbd点ip地址）
15.   DataDir=/var/mysql/data 
16.    
17.   [MYSQLD] 
18.   nodeid=3 
19.   HostName=192.168.191.131  (mysqld节点ip地址)
20.   [MYSQLD] 
21.   nodeid=4 
22.   HostName=192.168.191.133  (mysqld节点ip地址)
23.    
24.   #必须有空的mysqld节点，不然数据节点断开后启动有报错 
25.   [MYSQLD] 
26.   nodeid=5 
27.   [mysqld] 
28.   nodeid=6
```
以上内容输入完毕后，按ESC，输入：wq即可保存退出
![](http://images0.cnblogs.com/blog2015/380252/201507/142234025011218.png)
**拷贝ndb_mgm、ndb_mgmd、config.ini到bin目录。**
# cd /usr/local/mysql/bin 
# cp ./ndb_mgm /usr/local/bin/ 
# cp ./ndb_mgmd /usr/local/bin/
# cp /var/lib/mysql-cluster/config.ini /usr/local/bin/
![](http://images0.cnblogs.com/blog2015/380252/201507/142234187351691.png)
### 数据节点配置（终端输入）
# vi /etc/my.cnf （添加以下内容）
[mysqld] 
datadir=/var/mysql/data 
socket=/var/mysql/mysql.sock 
user=mysql 
# Disabling symbolic-links is recommended to prevent assorted security risks 
symbolic-links=0 
#运行NDB存储引擎 
ndbcluster  
#指定管理节点 
ndb-connectstring=192.168.191.132  (管理节点ip)
[MYSQL_CLUSTER] 
ndb-connectstring=192.168.191.132   (管理节点ip)
[NDB_MGM] 
connect-string=192.168.191.132   (管理节点ip)
[mysqld_safe] 
log-error=/var/mysql/log/mysqld.log 
pid-file=/var/run/mysqld/mysqld.pid
### Sql节点配置(终端输入)
# cd /usr/local/mysql/
设置mysql服务为开机自启动
# cp support-files/mysql.server /etc/rc.d/init.d/mysqld 
# chmod +x /etc/rc.d/init.d/mysqld 
# chkconfig --add mysqld
# vi /etc/my.cnf （添加以下内容）
[mysqld]    
datadir=/var/mysql/data 
socket=/var/mysql/mysql.sock 
user=mysql 
# Disabling symbolic-links is recommended to prevent assorted security risks 
symbolic-links=0 
log-bin = /var/mysql/log/mysql-bin.log 
max_connections=1000 
#以下为mysql 主主模式的配置文件 
# 忽略mysql数据库复制 
binlog-ignore-db=mysql 
# 每次增长2 
auto-increment-increment=2 
# 设置自动增长的字段的偏移量，即初始值为2 
auto-increment-offset=1 
ndbcluster 
ndb-connectstring=192.168.191.132 （管理节点ip）
[MYSQL_CLUSTER] 
ndb-connectstring=192.168.191.132  （管理节点ip）
[NDB_MGM] 
connect-string=192.168.191.132  （管理节点ip）
[mysqld_safe] 
log-error=/var/mysql/log/mysqld.log 
pid-file=/var/run/mysqld/mysqld.pid 
这里要注意，在var目录下新建mysql目录，在新建的mysql目录下建立log和data目录
另外要给这两个目录授权代码如下
# cd /var
# chown –R mysql:mysql mysql/
下面两个命令是对mysql命令的一个link，否则无法使用mysql命令
**# Ln –s /usr/local/mysql/bin/mysql /usr/bin**
**# Ln –s /var/mysql/mysql.sock /tmp/mysql.sock**
[回到顶部](http://www.cnblogs.com/baiboy/p/4646849.html#_labelTop)
## 运行mysql-cluster
**mysql集群的启动顺序为：管理节点->数据节点->SQL节点**
**mysql集群的关闭顺序为，管理节点->数据节点->SQL节点**
打开服务的时候注意防火墙问题
注意把管理节点的防火墙关闭 命令如下：
# /etc/init.d/iptables stop
首先打开管理节点服务
# ndb_mgmd -f /var/lib/mysql-cluster/config.ini --initial（注意第一次或者对config文件进行修改后，打开服务一定要加上initial否则新增加的内容不会被使用）
然后打开ndbd节点服务
# /usr/local/mysql/bin/ndbd --initial（此处同上）
出现一下信息表示ndbd节点启动成功.
2012-03-28 02:01:38 [ndbd] INFO -- Angel connected to '10.32.33.120:1186'
2012-03-28 02:01:38 [ndbd] INFO -- Angel allocated nodeid: 36
最后打开两个sql节点131和133
#service mysqld start
出现mysql success表示服务启动成功
打开管理节点，终端输入
# ndb_mgm
#ndb_mgm> show
显示以下信息，则mysql-cluster安装部署成功
Cluster Configuration
---------------------
[ndbd(NDB)] 2 node(s)
id=2 @192.168.191.130 (mysql-5.5.20 ndb-7.4.4, Nodegroup: 0)
[ndb_mgmd(MGM)] 1 node(s)
id=1 @192.168.191.132 (mysql-5.5.20 ndb-7.4.4)
[mysqld(API)] 4 node(s)
id=3 @192.168.191.131 (mysql-5.5.20 ndb-7.4.4)
id=4 @192.168.191.133 (mysql-5.5.20 ndb-7.4.4)
id=5 (not connected, accepting connect from any host)
id=6 (not connected, accepting connect from any host)
可以看到各个节点已经连接上了。
**以下是关于mgm相关的命令**
**# ndb_mgm**
**# ndb_mgm> show 查看各节点情况。   **
**# ndb_mgm> all report memory 查看各数据节点使用情况   **
**# ndb_mgm>create nodegroup 3创建数据节点分组    **
**# mysql> alter online table data_house reorganize partition; 调整分区数据**
**# ndb_mgm>shutdown关闭节点**
**# ndb_mgm>exit退出**
[回到顶部](http://www.cnblogs.com/baiboy/p/4646849.html#_labelTop)
## 常见问题
### Mysqld端服务无法启动
[root@localhost Desktop]#service mysqld start
Starting mysql.the service quit without updating PID file (/[FAILED]/var/mysql/log/localhost.localdomain.pid).
先去查看日志
#cd /var/mysql/data
#less localhost.localdomain.err
如果错误是
Fatal error：can’t open and lock privilege tables:Table’mysql.user’ doesn’t exist
解决方法：
终端输入
到mysql目录下/usr/local/mysql
输入
Scripts/mysql_install_db –user=mysql(初始化数据库即可)
其他问题请注意查看问题所在的文件夹权限问题
[回到顶部](http://www.cnblogs.com/baiboy/p/4646849.html#_labelTop)
## 测试
1、从SQL节点131登录，创建数据库和表，进行简单测试。
mysql> create database tltest ;
mysql> use tltest;
Database changed
mysql> create table test1(id int,name varchar(10)) engine=ndb ;
mysql> insert into test1 values(1,'tl');
mysql> select * from test1 ;
+------+---------+
| id   | name  |
+------+---------+
|  1 |    tl   |
+------+---------+
登陆133节点，查看效果，库，表和数据已经同步。
从133节点插入一条数据，同样登陆131，也能看到数据已经同步。
