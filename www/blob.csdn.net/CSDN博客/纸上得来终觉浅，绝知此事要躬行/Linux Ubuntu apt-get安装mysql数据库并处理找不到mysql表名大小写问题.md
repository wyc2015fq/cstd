# Linux Ubuntu apt-get安装mysql数据库并处理找不到mysql表名大小写问题 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年12月28日 14:33:41[boonya](https://me.csdn.net/boonya)阅读数：2539








网上找了很多安装方式，最多的就是mysql-*.rpm和mysql-*.tar.gz的安装。我原本使用的是mysql-*.tar.gz方式，但是执行到最后scripts/mysql_install_db --user=mysql找不到@bindir 。很早参考文章：[http://blog.csdn.net/lizuqingblog/article/details/18423751](http://blog.csdn.net/lizuqingblog/article/details/18423751)进行配置安装，但是可能用户权限不够没有安装成功，最后使用root用户就可以了。

## 安装前验证是否安装mysql：


`sudo netstat -tap | grep mysql`

注：如果没有信息输出说明系统未安装mysql。

## mysql安装SHELL操作

关键命令：sudo apt-get install mysql-server mysql-client



```
root@ubuntu:/usr/local# sudo apt-get install mysql-server mysql-client
Reading package lists... Done
Building dependency tree       
Reading state information... Done
The following extra packages will be installed:
  libdbd-mysql-perl libdbi-perl libhtml-template-perl libmysqlclient18 libterm-readkey-perl mysql-client-5.6 mysql-client-core-5.6 mysql-common mysql-server-5.6 mysql-server-core-5.6
Suggested packages:
  libclone-perl libmldbm-perl libnet-daemon-perl libsql-statement-perl libipc-sharedcache-perl mailx tinyca
The following NEW packages will be installed:
  libdbd-mysql-perl libdbi-perl libhtml-template-perl libmysqlclient18 libterm-readkey-perl mysql-client mysql-client-5.6 mysql-client-core-5.6 mysql-common mysql-server mysql-server-5.6 mysql-server-core-5.6
0 upgraded, 12 newly installed, 0 to remove and 90 not upgraded.
Need to get 21.6 MB of archives.
After this operation, 155 MB of additional disk space will be used.
Do you want to continue? [Y/n] y
Get:1 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main mysql-common all 5.6.28-0ubuntu0.15.04.1 [14.6 kB]
Get:2 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main libmysqlclient18 amd64 5.6.28-0ubuntu0.15.04.1 [699 kB]
Get:3 http://us.archive.ubuntu.com/ubuntu/ vivid/main libdbi-perl amd64 1.631-3build1 [770 kB]                                                                                                                                                                               
Get:4 http://us.archive.ubuntu.com/ubuntu/ vivid/main libdbd-mysql-perl amd64 4.028-2 [88.6 kB]                                                                                                                                                                              
Get:5 http://us.archive.ubuntu.com/ubuntu/ vivid/main libterm-readkey-perl amd64 2.32-1build1 [25.0 kB]                                                                                                                                                                      
Get:6 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main mysql-client-core-5.6 amd64 5.6.28-0ubuntu0.15.04.1 [4,102 kB]                                                                                                                                                 
Get:7 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main mysql-client-5.6 amd64 5.6.28-0ubuntu0.15.04.1 [5,360 kB]                                                                                                                                                      
Get:8 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main mysql-server-core-5.6 amd64 5.6.28-0ubuntu0.15.04.1 [4,670 kB]                                                                                                                                                 
Get:9 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main mysql-server-5.6 amd64 5.6.28-0ubuntu0.15.04.1 [5,779 kB]                                                                                                                                                      
Get:10 http://us.archive.ubuntu.com/ubuntu/ vivid/main libhtml-template-perl all 2.95-1 [65.5 kB]                                                                                                                                                                            
Get:11 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main mysql-client all 5.6.28-0ubuntu0.15.04.1 [8,760 B]                                                                                                                                                            
Get:12 http://us.archive.ubuntu.com/ubuntu/ vivid-updates/main mysql-server all 5.6.28-0ubuntu0.15.04.1 [8,882 B]                                                                                                                                                            
Fetched 21.6 MB in 11min 4s (32.5 kB/s)                                                                                                                                                                                                                                      
Preconfiguring packages ...
Selecting previously unselected package mysql-common.
(Reading database ... 57492 files and directories currently installed.)
Preparing to unpack .../mysql-common_5.6.28-0ubuntu0.15.04.1_all.deb ...
Unpacking mysql-common (5.6.28-0ubuntu0.15.04.1) ...
Selecting previously unselected package libmysqlclient18:amd64.
Preparing to unpack .../libmysqlclient18_5.6.28-0ubuntu0.15.04.1_amd64.deb ...
Unpacking libmysqlclient18:amd64 (5.6.28-0ubuntu0.15.04.1) ...
Selecting previously unselected package libdbi-perl.
Preparing to unpack .../libdbi-perl_1.631-3build1_amd64.deb ...
Unpacking libdbi-perl (1.631-3build1) ...
Selecting previously unselected package libdbd-mysql-perl.
Preparing to unpack .../libdbd-mysql-perl_4.028-2_amd64.deb ...
Unpacking libdbd-mysql-perl (4.028-2) ...
Selecting previously unselected package libterm-readkey-perl.
Preparing to unpack .../libterm-readkey-perl_2.32-1build1_amd64.deb ...
Unpacking libterm-readkey-perl (2.32-1build1) ...
Selecting previously unselected package mysql-client-core-5.6.
Preparing to unpack .../mysql-client-core-5.6_5.6.28-0ubuntu0.15.04.1_amd64.deb ...
Unpacking mysql-client-core-5.6 (5.6.28-0ubuntu0.15.04.1) ...
Selecting previously unselected package mysql-client-5.6.
Preparing to unpack .../mysql-client-5.6_5.6.28-0ubuntu0.15.04.1_amd64.deb ...
Unpacking mysql-client-5.6 (5.6.28-0ubuntu0.15.04.1) ...
Selecting previously unselected package mysql-server-core-5.6.
Preparing to unpack .../mysql-server-core-5.6_5.6.28-0ubuntu0.15.04.1_amd64.deb ...
Unpacking mysql-server-core-5.6 (5.6.28-0ubuntu0.15.04.1) ...
Processing triggers for man-db (2.7.0.2-5) ...
Setting up mysql-common (5.6.28-0ubuntu0.15.04.1) ...
update-alternatives: using /etc/mysql/my.cnf.fallback to provide /etc/mysql/my.cnf (my.cnf) in auto mode
Selecting previously unselected package mysql-server-5.6.
(Reading database ... 57855 files and directories currently installed.)
Preparing to unpack .../mysql-server-5.6_5.6.28-0ubuntu0.15.04.1_amd64.deb ...
Unpacking mysql-server-5.6 (5.6.28-0ubuntu0.15.04.1) ...
Selecting previously unselected package libhtml-template-perl.
Preparing to unpack .../libhtml-template-perl_2.95-1_all.deb ...
Unpacking libhtml-template-perl (2.95-1) ...
Selecting previously unselected package mysql-client.
Preparing to unpack .../mysql-client_5.6.28-0ubuntu0.15.04.1_all.deb ...
Unpacking mysql-client (5.6.28-0ubuntu0.15.04.1) ...
Selecting previously unselected package mysql-server.
Preparing to unpack .../mysql-server_5.6.28-0ubuntu0.15.04.1_all.deb ...
Unpacking mysql-server (5.6.28-0ubuntu0.15.04.1) ...
Processing triggers for man-db (2.7.0.2-5) ...
Processing triggers for ureadahead (0.100.0-19) ...
Processing triggers for systemd (219-7ubuntu3) ...
Setting up libmysqlclient18:amd64 (5.6.28-0ubuntu0.15.04.1) ...
Setting up libdbi-perl (1.631-3build1) ...
Setting up libdbd-mysql-perl (4.028-2) ...
Setting up libterm-readkey-perl (2.32-1build1) ...
Setting up mysql-client-core-5.6 (5.6.28-0ubuntu0.15.04.1) ...
Setting up mysql-client-5.6 (5.6.28-0ubuntu0.15.04.1) ...
Setting up mysql-server-core-5.6 (5.6.28-0ubuntu0.15.04.1) ...
Setting up mysql-server-5.6 (5.6.28-0ubuntu0.15.04.1) ...

Configuration file '/etc/init.d/mysql'
 ==> File on system created by you or by a script.
 ==> File also in package provided by package maintainer.
   What would you like to do about it ?  Your options are:
    Y or I  : install the package maintainer's version
    N or O  : keep your currently-installed version
      D     : show the differences between the versions
      Z     : start a shell to examine the situation
 The default action is to keep your current version.
*** mysql (Y/I/N/O/D/Z) [default=N] ? y
Installing new version of config file /etc/init.d/mysql ...
update-alternatives: using /etc/mysql/mysql.cnf to provide /etc/mysql/my.cnf (my.cnf) in auto mode
2016-12-27 22:03:09 0 [Warning] TIMESTAMP with implicit DEFAULT value is deprecated. Please use --explicit_defaults_for_timestamp server option (see documentation for more details).
2016-12-27 22:03:09 0 [Note] /usr/sbin/mysqld (mysqld 5.6.28-0ubuntu0.15.04.1) starting as process 2542 ...
Setting up libhtml-template-perl (2.95-1) ...
Setting up mysql-client (5.6.28-0ubuntu0.15.04.1) ...
Setting up mysql-server (5.6.28-0ubuntu0.15.04.1) ...
Processing triggers for libc-bin (2.21-0ubuntu4) ...
Processing triggers for ureadahead (0.100.0-19) ...
Processing triggers for systemd (219-7ubuntu3) ...
root@ubuntu:/usr/local# sudo netstat -tap | grep mysql
tcp        0      0 localhost:mysql         *:*                     LISTEN      3026/mysqld
```



操作过程中会要求输入数据库的密码：

![](https://img-blog.csdn.net/20161228141416897)

输入确认密码：
![](https://img-blog.csdn.net/20161228142149511)

## 登录Mysql验证


`mysql -u root -p`输入对应的数据库root用户密码，进入mysql。进入成功会有以下输出：





```
root@ubuntu:~# mysql -u root -p
Enter password: 
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 3
Server version: 5.6.28-0ubuntu0.15.04.1 (Ubuntu)

Copyright (c) 2000, 2015, Oracle and/or its affiliates. All rights reserved.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

mysql>
```


## Mysql服务操作

查看所有系统服务:  service --status-all




```
root@ubuntu:~# service
Usage: service < option > | --status-all | [ service_name [ command | --full-restart ] ]
root@ubuntu:~# service --status-all
 [ + ]  apparmor
 [ - ]  bootmisc.sh
 [ - ]  checkfs.sh
 [ - ]  checkroot-bootclean.sh
 [ - ]  checkroot.sh
 [ - ]  console-setup
 [ + ]  cron
 [ + ]  dbus
 [ + ]  dns-clean
 [ + ]  grub-common
 [ - ]  hostname.sh
 [ - ]  hwclock.sh
 [ + ]  irqbalance
 [ - ]  killprocs
 [ + ]  kmod
 [ - ]  lvm2
 [ - ]  mountall-bootclean.sh
 [ - ]  mountall.sh
 [ - ]  mountdevsubfs.sh
 [ - ]  mountkernfs.sh
 [ - ]  mountnfs-bootclean.sh
 [ - ]  mountnfs.sh
 [ + ]  mysql
 [ + ]  networking
 [ + ]  nginx
 [ + ]  ondemand
 [ - ]  pppd-dns
 [ + ]  procps
 [ - ]  rc.local
 [ + ]  resolvconf
 [ - ]  rsync
 [ + ]  rsyslog
 [ - ]  sendsigs
 [ + ]  ssh
 [ - ]  sudo
 [ + ]  udev
 [ + ]  udev-finish
 [ + ]  ufw
 [ - ]  umountfs
 [ - ]  umountnfs.sh
 [ - ]  umountroot
 [ + ]  urandom
 [ - ]  uuidd
root@ubuntu:~# service mysql 
Usage: /etc/init.d/mysql start|stop|restart|reload|force-reload|status
```



mysql服务的操作方式是：service mysql    start|stop|restart|reload|force-reload|status


注：service mysql 后续命令操作选项之一。




验证数据库命令：



```
root@ubuntu:~# service mysql start
root@ubuntu:~# service mysql stop
root@ubuntu:~# mysql -u root -p
Enter password: 
ERROR 2002 (HY000): Can't connect to local MySQL server through socket '/var/run/mysqld/mysqld.sock' (2)
root@ubuntu:~# service mysql start
root@ubuntu:~# mysql -u root -p
Enter password:
```
从上面看出命令操作是没有问题的。

## 查询Mysql服务端口

命令：netstat -nap | grep mysql




```
root@ubuntu:~# netstat -nap | grep mysql
tcp        0      0 127.0.0.1:3306          0.0.0.0:*               LISTEN      4809/mysqld     
unix  2      [ ACC ]     STREAM     LISTENING     43458    4809/mysqld         /var/run/mysqld/mysqld.sock
root@ubuntu:~#
```


## 开启远程访问

查询Mysql服务状态：



```
service mysql status
?.mysql.service - MySQL Community Server
   Loaded: loaded (/lib/systemd/system/mysql.service; enabled; vendor preset: enabled)
   Active: active (running) since Wed 2016-12-28 00:28:24 PST; 29s ago
  Process: 589 ExecStartPost=/usr/share/mysql/mysql-systemd-start post (code=exited, status=0/SUCCESS)
  Process: 575 ExecStartPre=/usr/share/mysql/mysql-systemd-start pre (code=exited, status=0/SUCCESS)
 Main PID: 588 (mysqld_safe)
   CGroup: /system.slice/mysql.service
           ?..588 /bin/sh /usr/bin/mysqld_safe
           ?..972 /usr/sbin/mysqld --basedir=/usr --datadir=/var/lib/mysql --plugin-dir=/usr/lib/mysql/plugin --log-error=/var/log/mysql/error.log --pid-

file=/var/run/mysqld/mysqld.pid --socket=/var/run/mysqld/mysqld.sock --port=3306

Dec 28 00:28:23 ubuntu systemd[1]: Starting MySQL Community Server...
Dec 28 00:28:24 ubuntu mysqld_safe[588]: 161228 00:28:24 mysqld_safe Can't log to error log and syslog at the same time.  Remove all --log-error configuration options for --

syslog to take effect.
Dec 28 00:28:24 ubuntu mysqld_safe[588]: 161228 00:28:24 mysqld_safe Logging to '/var/log/mysql/error.log'.
Dec 28 00:28:24 ubuntu mysqld_safe[588]: 161228 00:28:24 mysqld_safe Starting mysqld daemon with databases from /var/lib/mysql
Dec 28 00:28:24 ubuntu systemd[1]: Started MySQL Community Server.
root@ubuntu:~# whereis mysql.cnf
mysql: /usr/bin/mysql /usr/lib/mysql /etc/mysql /usr/share/mysql /usr/share/man/man1/mysql.1.gz
root@ubuntu:~# whereis *.cnf
*:
```


修改：/etc/mysql/mysql.conf.d/mysqld.cnf文件中的bind-address 127.0.0.1这一行。



命令：vi /etc/mysql/mysql.conf.d/mysqld.cnf

修改完成后，需要重启mysql：/etc/init.d/mysql restart，也可以通过Navicat客户端进行远程测试。




## 处理Mysql表名大小写敏感问题

**非Ubuntu：**




vi /etc/my.cnf [mysqld]放开大小写敏感：lower_case_table_names=1

默认为0，大小写敏感。

设置1，大小写不敏感。

```
[mysqld]
datadir=/var/lib/mysql
socket=/var/lib/mysql/mysql.sock
user=mysql
# Disabling symbolic-links is recommended to prevent assorted security risks
symbolic-links=0
lower_case_table_names=1

[mysqld_safe]
log-error=/var/log/mysqld.log
pid-file=/var/run/mysqld/mysqld.pid
```

**在Ubuntu下：**

vi /etc/mysql/mysql.conf.d/mysqld.cnf  [mysqld]放开大小写敏感（在文件最后加入）：lower_case_table_names=1



```
#
# The MySQL database server configuration file.
#
# You can copy this to one of:
# - "/etc/mysql/my.cnf" to set global options,
# - "~/.my.cnf" to set user-specific options.
# 
# One can use all long options that the program supports.
# Run program with --help to get a list of available options and with
# --print-defaults to see which it would actually understand and use.
#
# For explanations see
# http://dev.mysql.com/doc/mysql/en/server-system-variables.html

# This will be passed to all mysql clients
# It has been reported that passwords should be enclosed with ticks/quotes
# escpecially if they contain "#" chars...
# Remember to edit /etc/mysql/debian.cnf when changing the socket location.

# Here is entries for some specific programs
# The following values assume you have at least 32M ram

[mysqld_safe]
socket		= /var/run/mysqld/mysqld.sock
nice		= 0

[mysqld]
#
# * Basic Settings
#
user		= mysql
pid-file	= /var/run/mysqld/mysqld.pid
socket		= /var/run/mysqld/mysqld.sock
port		= 3306
basedir		= /usr
datadir		= /var/lib/mysql
tmpdir		= /tmp
lc-messages-dir	= /usr/share/mysql
skip-external-locking
#
# Instead of skip-networking the default is now to listen only on
# localhost which is more compatible and is not less secure.
#bind-address		= 127.0.0.1
#
# * Fine Tuning
#
key_buffer_size		= 16M
max_allowed_packet	= 16M
thread_stack		= 192K
thread_cache_size       = 8
# This replaces the startup script and checks MyISAM tables if needed
# the first time they are touched
myisam-recover         = BACKUP
#max_connections        = 100
#table_cache            = 64
#thread_concurrency     = 10
#
# * Query Cache Configuration
#
query_cache_limit	= 1M
query_cache_size        = 16M
#
# * Logging and Replication
#
# Both location gets rotated by the cronjob.
# Be aware that this log type is a performance killer.
# As of 5.1 you can enable the log at runtime!
#general_log_file        = /var/log/mysql/mysql.log
#general_log             = 1
#
# Error log - should be very few entries.
#
log_error = /var/log/mysql/error.log
#
# Here you can see queries with especially long duration
#log_slow_queries	= /var/log/mysql/mysql-slow.log
#long_query_time = 2
#log-queries-not-using-indexes
#
# The following can be used as easy to replay backup logs or for replication.
# note: if you are setting up a replication slave, see README.Debian about
#       other settings you may need to change.
#server-id		= 1
#log_bin			= /var/log/mysql/mysql-bin.log
expire_logs_days	= 10
max_binlog_size   = 100M
#binlog_do_db		= include_database_name
#binlog_ignore_db	= include_database_name
#
# * InnoDB
#
# InnoDB is enabled by default with a 10MB datafile in /var/lib/mysql/.
# Read the manual for more InnoDB related options. There are many!
#
# * Security Features
#
# Read the manual, too, if you want chroot!
# chroot = /var/lib/mysql/
#
# For generating SSL certificates I recommend the OpenSSL GUI "tinyca".
#
# ssl-ca=/etc/mysql/cacert.pem
# ssl-cert=/etc/mysql/server-cert.pem
# ssl-key=/etc/mysql/server-key.pem
lower_case_table_names=1
```
修改完成后，需要重启mysql：/etc/init.d/mysql restart，然后启动Tomcat就OK了！









