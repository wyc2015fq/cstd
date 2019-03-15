# MySQL环境搭建



INSTALL_MySQL:

　　所有平台的MySQL下载地址：<http://www.mysql.com/downloads>。

简单介绍MySQL安装

Linux上安装MySQL的两种方式：源码安装、RPM包安装

（因为是Linux基础知识，就不啰嗦啦）---主要讲述使用rpm包安装MySQL 5.7

 

MySQL的各RPM包：

　　①MySQL：MySQL服务器

　　②MySQL-client：MySQL客户端程序，用于连接并操作Mysql服务器

　　③MySQL-devel：库和包含文件，若想编译其它MySQL客户端(例如Perl模块)，则需要安装该RPM包

　　④MySQL-shared：包含某些语言和应用程序需要动态装载的共享库(libmysqlclient.so*)

　　⑤MySQL-bench：数据库服务器的基准和性能测试工具

 

RPM包安装MySQL：

①MySQL官网下载版本tar包，上传到linux系统

　　mysql-5.7.14-1.el6.x86_64.rpm-bundle.tar

②新建/software目录，将tar包mv到该目录下，tar开

　　tar xf mysql-5.7.14-1.el6.x86_64.rpm-bundle.tar

③利用yum安装(推荐)

　　yum localinstall /software/*.rpm

④MySQL初始化、配置/etc/my.conf、关闭防火墙和selinux

 

1、初始化启动MySQL服务：

[root@mysqlserver ~]# service mysqld start

netstat -an |grep :3306

mysql的初始密码在 /var/log/mysqld.log 里面

A temporary password is generated for root@localhost: a?tMa%FsE7a. 密码

vim /etc/my.cnf  添加：validate_password=off

[root@mysqlserver ~]# service mysqld restart

用初始密码登录进去之后：set password=password('123');

------

如果安装的是5.6版本的MySQL，在安装成功后，默认的root用户密码为空，命令行创建root用户的密码：

[root@mysqlserver ~]# mysqladmin -u root password 123

------

 

2、连接到MySQL服务器：

```
[root@mysqlserver ~]# mysql -uroot -p -S /var/lib/mysql/mysql.sock
Enter password：***
mysql> status　　查看数据库状态
--------------
mysql Ver 14.14 Distrib 5.7.14, for Linux (x86_64) using EditLine wrapper

Connection id:    13
Current database:    db1
Current user:    root@localhost
SSL:    Not in use
Current pager:    stdout
Using outfile:    ''
Using delimiter:    ;
Server version:    5.7.14 Source distribution
Protocol version:    10
Connection:    Localhost via UNIX socket
Server characterset:    latin1
Db characterset:    latin1
Client characterset:    utf8
Conn. characterset:    utf8
UNIX socket:    /var/lib/mysql/mysql.sock
Uptime:    2 days 13 hours 25 min 42 sec

Threads: 2 Questions: 10793065 Slow queries: 0 Opens: 646 Flush tables: 1 Open tables: 426 Queries per second avg: 48.806
--------------

mysql> show databases;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| mysql              |
| performance_schema |
| test               |
+--------------------+
4 rows in set (0.04 sec)

mysql> quit　　退出MySQL
Bye
[root@mysqlserver ~]#
```

 

3、MySQL的配置文件/etc/my.cnf

```
[root@mysqlserver ~]# cat /etc/my.cnf
[mysqld]
datadir=/mydata
socket=/var/lib/mysql/mysql.sock
symbolic-links=0

[mysqld_safe]
log-error=/var/log/mysqld.log
pid-file=/mydata/mysqld.pid
[root@mysqlserver ~]#
```

注意：MySQL在windows的配置文件是my.ini