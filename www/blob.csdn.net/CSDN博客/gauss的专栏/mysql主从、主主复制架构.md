# mysql主从、主主复制架构 - gauss的专栏 - CSDN博客
2015年05月04日 00:01:33[gauss](https://me.csdn.net/mathlmx)阅读数：371
转载：[http://rain1118.blog.51cto.com/4350097/834046](http://rain1118.blog.51cto.com/4350097/834046)
mysql主从复制的概述：
mysql内建的复制功能是构建大型，高性能应用程序的基础。将mysql的数据分布多个系统上，这种复制机制是通过将mysql的一台主机（master）的数据复制到其他主机（slaves）上，并重新执行一遍来实现。
mysql支持单行、异步复制。复制时对表的更新须在主服务器上进行，避免发生更新冲突
主从复制的好处：
  1：数据分布，稳定性提升，主服务器出现故障，还可以用从服务器支撑。
  2：读写分离，可以实现负载均衡，提升数据处理效率。
  3：高可用性和容错性。
  4：便于实现数据库的冗灾、备份、恢复等操作。
  5：测试mysql升级。
复制功能：
  1：支持一株多从机制。数据通过主服务器复制到从服务器上。
  2：支持多级结构。主从，从从，主主（互为主从）。
  3：支持过滤功能（可以只复制主服务器上的部分数据，而非全部）。
mysql主从复制类型：
mysql复制类型:
  1、基于语句的复制：在主服务器上执行的SQL语句，在从服务器上执行同样的SQL语句。mysql默认采用基于语句的复制，效率比较高。
  2、基于行的复制：把改变的内容复制过去，而不是把命令在从服务器上执行一遍（mysql5.0开始支持）。
  3、混合类型的复制：默认采用基于语句的复制。发现基于语句无法精确复制时，就会采用基于行的复制
相应的二进制日志：
  1、STATEMENT
  2、ROW
  3、MIXED
注主从服务器架构要求：
  1、主从服务器的表可以使用不同的表类型，另外：一台主服务器同时带多台从服务器，
会影响其性能，可以拿出一台服务器作为从发服务器代理，使用BLOCKHOLE表类型，只记录日志，不写数据，由它带多台服务器，从而提升性能。
  2、主从服务器中的表可以使用不同的字段类型。
  3、主从服务器中的表可以使用不同的索引。主服务器主要用来写操作，所以除了逐渐和唯一索引等保证数据关系的索引一般都可以不加；从服务器一般用来读操作，所以可以针对查询特征设置索引，甚至：不同的从服务器可以针对不同的查询设置不同的索引。
复制流程：
  1、master服务器将改变记录到二进制日志文件（binary log）中，这些记录叫做二进制日志时间（binary log events）
  2、slave服务器master的binary log events拷贝到它的中继日志（relay log）
  3、slave重做中继日志的事件，将改变反映到他自己的数据。
![](http://img1.51cto.com/attachment/201204/195611848.jpg)
主从服务器的配置：
首先安装mysql：
1、准备数据存放的文件系统
新建一个逻辑卷，并将其挂载至特定目录即可，过程如下：
fdisk /dev/sda 建一个分区，大概2G 即可，类型要是8e的。如 /dev/sda5
     # pvcreate /dev/sda5先创建物理卷
     # vgcreate myvg /dev/sda5再创建物理卷组
     # lvcreate -L 1G -n lv1 /dev/myvg   创建逻辑卷并指定大小为1G，名字为lv1 
     # mke2fs -j /dev/myvg/lv1格式化
其逻辑卷的挂载目录为/mydata，则# mkdir /mydata 
                                       # mount /dev/myvg/lv1 /mydata 如果想开机自启动，则可在/etc/fstab内添加内容。 
而后需要创建/mydata/data目录做为mysql数据的存放目录。
    # mkdir /mydata/data
2、新建用户以安全方式运行进程：
# groupadd -r mysql
# useradd -g mysql -r -s /sbin/nologin -M -d /mydata/data mysql
# chown -R mysql:mysql /mydata/data
3、安装并初始化mysql-5.5.19
首先下载对应的mysql版本至本地，这里是32位平台，因此，选择的为mysql-5.5.19-linux2.6-i686.tar.gz
# tar xf mysql-5.5.19-linux2.6-i686.tar.gz -C /usr/local
# cd /usr/local/
# ln -sv mysql-5.5.19-linux2.6-i686  mysql  创建链接
# cd mysql 
# chown -R mysql:mysql  .
# ./scripts/mysql_install_db --user=mysql --datadir=/mydata/data 初始化mysql
# chown -R root  .
# chown -R mysql data/
4、为mysql提供主配置文件：
# cd /usr/local/mysql
# cp support-files/my-large.cnf  /etc/my.cnf
# vim /etc/my.cnf
修改此文件中thread_concurrency的值为你的CPU个数乘以2，比如这里使用如下行：
thread_concurrency = 2
另外还需要添加如下行指定mysql数据文件的存放位置：
datadir = /mydata/data
5、为mysql提供sysv服务脚本：
# cd /usr/local/mysql
# cp support-files/mysql.server  /etc/rc.d/init.d/mysqld
并查看是否有执行权限，若无 则加上执行权限
添加至服务列表：
# chkconfig --add mysqld
# chkconfig mysqld on 实现开机启动
而后就可以启动服务测试使用了（service mysql start）。
为了使用mysql的安装符合系统使用规范，并将其开发组件导出给系统使用，这里还需要进行如下步骤：
6、输出mysql的man手册至man命令的查找路径：
编辑/etc/man.config，添加如下行即可：
MANPATH  /usr/local/mysql/man
7、输出mysql的头文件至系统头文件路径/usr/include：
这可以通过简单的创建链接实现：
# ln -sv /usr/local/mysql/include  /usr/include/mysql
8、输出mysql的库文件给系统库查找路径：
# echo '/usr/local/mysql/lib' > /etc/ld.so.conf.d/mysql.conf
而后让系统重新载入系统库：
# ldconfig
若无mysql库文件则：
vim /etc/ld.co.conf.d/mysql.conf
添加/usr/local/mysql/bin
则再次重新载入系统库即可
9、修改PATH环境变量，让系统可以直接使用mysql的相关命令。
步骤如下
# Vim /etc/profile
增加：PATH=$PATP:/usr/local/mysql/bin
#source /etc/profile让内核重读一下配置文件，使其立即生效
至此mysql源码编译安装完成
这里我们使用ip为172.16.5.1主机为主服务器；ip为172.16.5.2的主机为从服务器
主服务器的配置过程：
1、编辑配置文件/etc/my.cnf
service-id=1                      #此处id号主从服务器不得一样
启动log-bin=mysql-bin
在[mysqld]下面添加：
relay-log=relay-bin
relay-log-index=relay-bin.index
service mysqld reload       #重新载入配置文件使其生效
2、创建用户让其有复制权限：
mysql > GRANT REPLICATION CLIENT, REPLICATION SLAVE ON *.* TO repl@'172.16.%.%' IDENTIFIED BY '12345';
mysql > FLUSH PRIVILEGES;      # 刷新授权列表，使授权账户立即生效
mysql > GRANTS FOR repl@'172.16.%.%';
从服务器的配置过程：
1、编辑主配置文件/etc/my.cnf
service-id=11          #这里id号不得与主服务器的id号相同
注释掉 log-bin=mysql-bin
在[mysqld]下面添加relay-log=relay-bin      #确保中继日志开启
relay-log-index=relay-bin.index
service mysqld reload   重新载入配置文件，使配置生效
mysql > CHANGE MASTER TO
      > MASTER_HOST='172.16.5.1',
      > MASTER_USER='repl',
      > MASTER_PASSWORD='12345';
mysql> stop slave;
mysql> start slave;
mysql> show slave status\G;
如果出现以下两行，则证明从服务器正常；
Slave_IO_Running: Yes
Slave_SQL_Running: Yes
至此主从服务器已配置完成
主主服务器架构实现：
mysql主主服务器的思路和主从差不多，本质就是让多台mysql服务器间互为主从。这里可以在主从服务器的基础上进行配置主主服务器实现主主服务器架构。主主架构不能降低写操作能力
设置主主复制：
这里我们只需要在172.16.5.2上建立一个具有复制权限的用户：
mysql > stop slave;
mysql > GRANT REPLICATION CLIENT, REPLICATION SLAVTE ON *.* TO repl@172.16.%.%' IDENTIFIED BY '12345';
mysql > FLUSH PRIVILEGES;
mysql > GRANTS FOR repl@'172.16.%.%';
编辑主配置文件 /etc/my.cnf
在[mysqld]下添加：
auto-increment-increment = 2
auto-increment-offset = 2
service mysqld reload              #重新载入配置文件使其生效
在172.16.5.1主机上配置：
mysql > change master to
      > master_host="172.16.5.2",
      > master_user='repl',
      > master_password='12345';
编辑配置文件/etc/my.cnf
在[mysqld]下面添加：
auto-increment-increment = 2
auto-increment-offset = 1
service mysqld reload         #重新在入配置文件使其生效
在172.16.5.1和172.16.5.2服务器上分别查看从服务进程的运行状态：
mysql> show slave status\G
如果出现如下两行，则说明工作正常：
Slave_IO_Running: Yes
Slave_SQL_Running: Yes
