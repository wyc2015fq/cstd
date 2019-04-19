# Hive分布式安装 - BlueSky - CSDN博客
2016年03月12日 18:42:46[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：2522
## 1、mysql安装
主要是通过在线安装，首先通过下面命令来查看mysql相关的版本
```
[hadoop1@master ~]$ yum list | grep mysql
mysql-libs.x86_64                      5.1.71-1.el6                      @anaconda-CentOS-201311272149.x86_64/6.5
apr-util-mysql.x86_64                  1.3.9-3.el6_0.1                   base   
bacula-director-mysql.x86_64           5.0.0-13.el6                      base   
bacula-storage-mysql.x86_64            5.0.0-13.el6                      base   
dovecot-mysql.x86_64                   1:2.0.9-19.el6_7.2                updates
freeradius-mysql.x86_64                2.2.6-6.el6_7                     updates
libdbi-dbd-mysql.x86_64                0.8.3-5.1.el6                     base   
mod_auth_mysql.x86_64                  1:3.0.0-11.el6_0.1                base   
mysql.x86_64                           5.1.73-5.el6_6                    base   
mysql-bench.x86_64                     5.1.73-5.el6_6                    base   
mysql-connector-java.noarch            1:5.1.17-6.el6                    base   
mysql-connector-odbc.x86_64            5.1.5r1144-7.el6                  base   
mysql-devel.i686                       5.1.73-5.el6_6                    base   
mysql-devel.x86_64                     5.1.73-5.el6_6                    base   
mysql-embedded.i686                    5.1.73-5.el6_6                    base   
mysql-embedded.x86_64                  5.1.73-5.el6_6                    base   
mysql-embedded-devel.i686              5.1.73-5.el6_6                    base   
mysql-embedded-devel.x86_64            5.1.73-5.el6_6                    base   
mysql-libs.i686                        5.1.73-5.el6_6                    base   
mysql-libs.x86_64                      5.1.73-5.el6_6                    base   
mysql-server.x86_64                    5.1.73-5.el6_6                    base   
mysql-test.x86_64                      5.1.73-5.el6_6                    base   
php-mysql.x86_64                       5.3.3-46.el6_7.1                  updates
qt-mysql.i686                          1:4.6.2-28.el6_5                  base   
qt-mysql.x86_64                        1:4.6.2-28.el6_5                  base   
rsyslog-mysql.x86_64                   5.8.10-10.el6_6                   base   
rsyslog7-mysql.x86_64                  7.4.10-3.el6_7.1                  updates
```
在root用户下输入下面命令，进行在线安装，看到下图则表示安装成功：`yum install -y mysql-server mysql mysql-devel`![](https://img-blog.csdn.net/20160312171133105?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们在安装完mysql数据库以后，会发现会多出一个mysqld的服务，这个就是咱们的数据库服务，我们通过输入 service mysqld start 命令就可以启动我们的mysql服务。
注意：如果我们是第一次启动mysql服务，mysql服务器首先会进行初始化的配置，如：
```
[root@xiaoluo ~]# service mysqld start
初始化 MySQL 数据库： WARNING: The host 'xiaoluo' could not be looked up with resolveip.
This probably means that your libc libraries are not 100 % compatible
with this binary MySQL version. The MySQL daemon, mysqld, should work
normally with the exception that host name resolving will not work.
This means that you should use IP addresses instead of hostnames
when specifying MySQL privileges !
Installing MySQL system tables...
OK
Filling help tables...
OK
To start mysqld at boot time you have to copy
support-files/mysql.server to the right place for your system
PLEASE REMEMBER TO SET A PASSWORD FOR THE MySQL root USER !
To do so, start the server, then issue the following commands:
/usr/bin/mysqladmin -u root password 'new-password'
/usr/bin/mysqladmin -u root -h xiaoluo password 'new-password'
Alternatively you can run:
/usr/bin/mysql_secure_installation
which will also give you the option of removing the test
databases and anonymous user created by default.  This is
strongly recommended for production servers.
See the manual for more instructions.
You can start the MySQL daemon with:
cd /usr ; /usr/bin/mysqld_safe &
You can test the MySQL daemon with mysql-test-run.pl
cd /usr/mysql-test ; perl mysql-test-run.pl
Please report any problems with the /usr/bin/mysqlbug script!
                                                           [确定]
正在启动 mysqld：                                            [确定]
```
这时我们会看到第一次启动mysql服务器以后会提示非常多的信息，目的就是对mysql数据库进行初始化操作，当我们再次重新启动mysql服务时，就不会提示这么多信息了，如：
```
[root@xiaoluo ~]# service mysqld restart
停止 mysqld：                                             [确定]
正在启动 mysqld：                                          [确定]
```
我们在使用mysql数据库时，都得首先启动mysqld服务，我们可以 通过  chkconfig --list | grep mysqld 命令来查看mysql服务是不是开机自动启动，如：
```
[root@xiaoluo ~]# chkconfig --list | grep mysqld
mysqld             0:关闭    1:关闭    2:关闭    3:关闭    4:关闭    5:关闭    6:关闭
```
我们发现mysqld服务并没有开机自动启动，我们当然可以通过 chkconfig mysqld on 命令来将其设置成开机启动，这样就不用每次都去手动启动了
```
[root@xiaoluo ~]# chkconfig mysqld on
[root@xiaoluo ~]# chkconfig --list | grep mysql
mysqld             0:关闭    1:关闭    2:启用    3:启用    4:启用    5:启用    6:关闭
```
mysql数据库安装完以后只会有一个root管理员账号，但是此时的root账号还并没有为其设置密码，在第一次启动mysql服务时，会进行数据库的一些初始化工作，在输出的一大串信息中，我们看到有这样一行信息 ：
```
/usr/bin/mysqladmin -u root password 'new-password'　　// 为root账号设置密码
```
所以我们可以通过 该命令来给我们的root账号设置密码(注意：这个root账号是mysql的root账号，非Linux的root账号)
```
[root@xiaoluo ~]# mysqladmin -u root password 'root'　　// 通过该命令给root账号设置密码为 root
```
此时我们就可以通过 mysql -u root -p 命令来登录我们的mysql数据库了
![](http://images.cnitblog.com/blog/432441/201304/07003612-b31a56759e64417c84d85be6dfca5fbb.jpg)
五、mysql数据库的主要配置文件
1./etc/my.cnf 这是mysql的主配置文件
我们可以查看一下这个文件的一些信息
```
[root@xiaoluo etc]# ls my.cnf 
my.cnf
[root@xiaoluo etc]# cat my.cnf 
[mysqld]
datadir=/var/lib/mysql
socket=/var/lib/mysql/mysql.sock
user=mysql
# Disabling symbolic-links is recommended to prevent assorted security risks
symbolic-links=0
[mysqld_safe]
log-error=/var/log/mysqld.log
pid-file=/var/run/mysqld/mysqld.pid
```
2./var/lib/mysql   mysql数据库的数据库文件存放位置
我们的mysql数据库的数据库文件通常是存放在了/ver/lib/mysql这个目录下
```
[root@xiaoluo ~]# cd /var/lib/mysql/
[root@xiaoluo mysql]# ls -l
总用量 20488
-rw-rw----. 1 mysql mysql 10485760 4月   6 22:01 ibdata1
-rw-rw----. 1 mysql mysql  5242880 4月   6 22:01 ib_logfile0
-rw-rw----. 1 mysql mysql  5242880 4月   6 21:59 ib_logfile1
drwx------. 2 mysql mysql     4096 4月   6 21:59 mysql　　// 这两个是mysql数据库安装时默认的两个数据库文件
srwxrwxrwx. 1 mysql mysql        0 4月   6 22:01 mysql.sock
drwx------. 2 mysql mysql     4096 4月   6 21:59 test　　// 这两个是mysql数据库安装时默认的两个数据库文件
```
我们可以自己创建一个数据库，来验证一下该数据库文件的存放位置
```
创建一个我们自己的数据库：
mysql> create database xiaoluo;
Query OK, 1 row affected (0.00 sec)
[root@xiaoluo mysql]# ls -l
总用量 20492
-rw-rw----. 1 mysql mysql 10485760 4月   6 22:01 ibdata1
-rw-rw----. 1 mysql mysql  5242880 4月   6 22:01 ib_logfile0
-rw-rw----. 1 mysql mysql  5242880 4月   6 21:59 ib_logfile1
drwx------. 2 mysql mysql     4096 4月   6 21:59 mysql
srwxrwxrwx. 1 mysql mysql        0 4月   6 22:01 mysql.sock
drwx------. 2 mysql mysql     4096 4月   6 21:59 test
drwx------. 2 mysql mysql     4096 4月   6 22:15 xiaoluo　　// 这个就是我们刚自己创建的xiaoluo数据库
[root@xiaoluo mysql]# cd xiaoluo/
[root@xiaoluo xiaoluo]# ls
db.opt
```
3./var/log mysql数据库的日志输出存放位置
我们的mysql数据库的一些日志输出存放位置都是在/var/log这个目录下
```
[root@xiaoluo xiaoluo]# cd 
[root@xiaoluo ~]# cd /var/log
[root@xiaoluo log]# ls
amanda                cron           maillog-20130331   spice-vdagent.log
anaconda.ifcfg.log    cron-20130331  mcelog             spooler
anaconda.log          cups           messages           spooler-20130331
anaconda.program.log  dirsrv         messages-20130331  sssd
anaconda.storage.log  dmesg          mysqld.log         tallylog
anaconda.syslog       dmesg.old      ntpstats           tomcat6
anaconda.xlog         dracut.log     piranha            wpa_supplicant.log
anaconda.yum.log      gdm            pm-powersave.log   wtmp
audit                 httpd          ppp                Xorg.0.log
boot.log              ibacm.log      prelink            Xorg.0.log.old
btmp                  lastlog        sa                 Xorg.1.log
btmp-20130401         libvirt        samba              Xorg.2.log
cluster               luci           secure             Xorg.9.log
ConsoleKit            maillog        secure-20130331    yum.log
```
其中mysqld.log 这个文件就是我们存放我们跟mysql数据库进行操作而产生的一些日志信息，通过查看该日志文件，我们可以从中获得很多信息
因为我们的mysql数据库是可以通过网络访问的，并不是一个单机版数据库，其中使用的协议是 tcp/ip 协议，我们都知道mysql数据库绑定的端口号是 3306 ，所以我们可以通过 netstat -anp命令来查看一下，Linux系统是否在监听 3306 这个端口号：
![](http://images.cnitblog.com/blog/432441/201304/07004518-a8b1e585e3e546c1a068d2a38d37b535.jpg)
结果如上所示，Linux系统监听的3306端口号就是我们的mysql数据库！！！！
## 2、设置mysql远程连接
输入命令：mysql -u root -p   登陆mysql，
```
mysql> use mysql;
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A
Database changed
mysql> select host,user,password from user;
+-----------+------+-------------------------------------------+
| host      | user | password                                  |
+-----------+------+-------------------------------------------+
| localhost | root | *81F5E21E35407D884A6CD4A731AEBFB6AF209E1B |
| master    | root |                                           |
| 127.0.0.1 | root |                                           |
| localhost |      |                                           |
| master    |      |                                           |
+-----------+------+-------------------------------------------+
5 rows in set (0.00 sec)
mysql> grant all privileges  on *.* to root@'%' identified by "root";
Query OK, 0 rows affected (0.00 sec)
mysql> select host,user,password from user;
+-----------+------+-------------------------------------------+
| host      | user | password                                  |
+-----------+------+-------------------------------------------+
| localhost | root | *81F5E21E35407D884A6CD4A731AEBFB6AF209E1B |
| master    | root |                                           |
| 127.0.0.1 | root |                                           |
| localhost |      |                                           |
| master    |      |                                           |
| %         | root | *81F5E21E35407D884A6CD4A731AEBFB6AF209E1B |
+-----------+------+-------------------------------------------+
6 rows in set (0.00 sec)
```
通过以上命令，就是对任何客户端都允许通过，root用户来进行登陆查看。
## 二、安装Hive
Root用户下操作
将apache-hive-1.2.1-bin.tar.gz拿U盘复制到/home/hadoop目录下
在/home/hadoop目录下，解压安装
tar -zxvf apache-hive-1.2.1-bin.tar.gz -C/usr/local
修改权限
chown -R hadoop:hadoop/usr/local/apache-hive-1.2.1-bin/
三、配置Hive
1、在~/.bashrc或.bash_profile文件中增加以下设置，
在/etc/profile文件中增加以下设置：
# set hive environment
exportHIVE_HOME=/usr/local/apache-hive-1.2.1-bin/
export PATH=$HIVE_HOME/bin:$PATH
2、新建hive-site.xml、hive-env.sh等文件
打开目录
cd /usr/local/apache-hive-1.2.1-bin/conf/
拷贝
cp hive-default.xml.templatehive-default.xml
cp hive-default.xml.template hive-site.xml
cp hive-env.sh.template hive-env.sh
cp hive-log4j.properties.templatehive-log4j.properties
cp hive-exec-log4j.properties.templatehive-exec-log4j.properties
3、第一次修改hive-site.xml
使用vim命令打开
vim hive-site.xml
使用/**查找需要修改的内容
如下：
Server2连接的主机名，Hive安装在Master机上
 <property>
 <name>hive.server2.thrift.bind.host</name>
 <value>Master</value>
 </property>
所连接的MySQL数据库实例
 <property>
 <name>javax.jdo.option.ConnectionURL</name>
 <value>jdbc:mysql://localhost:3306/hive?createDatabaseIfNotExist=true</value>
 <description>JDBC connect string for a JDBCmetastore</descriptioni
</property>
连接的MySQL数据库驱动
<property>
 <name>javax.jdo.option.ConnectionDriverName</name>
 <value>com.mysql.jdbc.Driver</value>
 <description>Driver class name for a JDBCmetastore</description>
</property>
连接的MySQL数据库用户名
<property>
 <name>javax.jdo.option.ConnectionUserName</name>
 <value>hive</value>
 <description>username to use against metastoredatabase</description>
</property>
连接的MySQL数据库密码
<property>
 <name>javax.jdo.option.ConnectionPassword</name>
 <value>hive</value>
 <description>password to use against metastoredatabase</description>
</property>
4、第二次修改hive-site.xml
在/usr/local/apache-hive-1.2.1-bin/目录下新建iotmp文件夹
mkdir iotmp
将含有hive-site.xml中含有"system:java.io.tmpdir"的配置项的值修改为
/usr/local/apache-hive-1.2.1-bin/iotmp
修改方法和上一步骤相同
如果不进行修改，会报一下错误：
Relative path in absoluteURI:${system:java.io.tmpdir%7D/$%7Bsystem:user.name%7D
5、下载mysql jdbc jar包
下载地址：
[http://dev.mysql.com/downloads/connector/j/](http://dev.mysql.com/downloads/connector/j/)
和
[http://cdn.mysql.com//Downloads/Connector-J/mysql-connector-java-5.1.37.tar.gz](http://cdn.mysql.com/Downloads/Connector-J/mysql-connector-java-5.1.37.tar.gz)
下载完成后，拿U盘复制到/home/hadoop目录下
在/home/hadoop目录下，解压安装
tar -zxvfmysql-connector-java-5.1.37.tar.gz
拷贝到Hive的lib目录下
cpmysql-connector-java-5.1.37/mysql-connector-java-5.1.37-bin.jar $HIVE_HOME/lib
其中，$HIVE_HOME/lib即/usr/local/apache-hive-1.2.1-bin/lib
如果不进行这一步，或操作过程中出错，启动Hive时，会报错
Unable to instantiateorg.apache.hadoop.hive.metastore.HiveMetaStoreClient
和
The specified datastore driver("com.mysql.jdbc.Driver") was not found in the CLASSPATH
6、修改hive-env.sh
这个文件在conf目录下
加入以下内容
# set java environment
exportJAVA_HOME=/usr/local/java/jdk1.8.0_65
exportJRE_HOME=/usr/local/java/jdk1.8.0_65/jre
exportPATH=$JAVA_HOME/bin:$JAVA_HOME/jre/bin:$PATH
exportCLASSPATH=$CLASSPATH:.:$JAVA_HOME/lib:$JAVA_HOME/jre/lib
# set hadoop path
export HADOOP_HOME=/usr/local/hadoop
export PATH=$PATH:$HADOOP_HOME/bin:$PATH
# set hive environment
exportHIVE_HOME=/usr/local/apache-hive-1.2.1-bin/
export PATH=$HIVE_HOME/bin:$PATH
export HIVE_AUX_JARS_PATH=$HIVE_HOME/bin
export HIVE_CONF_DIR=$HIVE_HOME/conf
export HADOOP_USER_CLASSPATH_FIRST=true
其中，最后一句很重要，如果不加入，启动Hive时，会报以下错误
[ERROR] Terminal initialization failed;falling back to unsupported  
java.lang.IncompatibleClassChangeError:Found class jline.Terminal, but interface was expected 
四、启动Hive
前提条件：启动Hive前，需要启动Hadoop集群和MySQL
1、测试能否正常启动
cd /usr/local/apache-hive-1.2.1-bin/././
bin/hive
成功启动的提示：
[hadoop@Master apache-hive-1.2.1-bin]$bin/hive
Logging initialized using configuration in 
file:/usr/local/apache-hive-1.2.1-bin/conf/hive-log4j.properties
hive>
2、基本操作
展现TABLES，初次操作时，只返回OK和操作所用时间
hive> SHOW TABLES;
建立表records
hive> CREATE TABLE records (year STRING,temperature INT, quality INT) ROW FORMAT DELIMITED FIELDS TERMINATED BY '\t';
显示存在的表，多了一个records
hive> SHOW TABLES;           
查看表records的定义
hive> DESCRIBE records;
向表records导入数据，数据在sample.txt中
hive> LOAD DATA INPATH'/home/hadoop/sample.txt' INTO TABLE records; 
查看表records的数据
hive> SELECT * FROM records;
一个普通操作，例如：计算records中每一年的最高温度
hive> SELECT year, MAX(temperature) FROMrecords GROUP BY year;
3、启动hiveserver2
[hadoop@Master apache-hive-1.2.1-bin]$ **nohup hive--service hiveserver2 &**
[1] 22539
[hadoop@Master apache-hive-1.2.1-bin]$nohup: ignoring input and appending output to `nohup.out'
**beeline**
Beeline version 1.2.1 by Apache Hive
beeline>** !connect jdbc:hive2://Master:10000**
Connecting to jdbc:hive2://Master:10000
Enter username forjdbc:hive2://Master:10000: **hive**
Enter password forjdbc:hive2://Master:10000: ****
Connected to: Apache Hive (version 1.2.1)
Driver: Hive JDBC (version 1.2.1)
Transaction isolation: TRANSACTION_REPEATABLE_READ
0: jdbc:hive2://Master:10000> **show tables;**
+-----------+--+
| tab_name |
+-----------+--+
| records  |
+-----------+--+
1 row selected (3.509 seconds)
0: jdbc:hive2://Master:10000> 
