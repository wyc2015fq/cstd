# 源码安装LAMP环境 - nosmatch的专栏 - CSDN博客
2011年07月29日 17:03:43[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：729标签：[mysql																[apache																[phpmyadmin																[documentation																[module																[centos](https://so.csdn.net/so/search/s.do?q=centos&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=documentation&t=blog)](https://so.csdn.net/so/search/s.do?q=phpmyadmin&t=blog)](https://so.csdn.net/so/search/s.do?q=apache&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)
                
        本文讲了从源码简单安装apache+mysql+php+phpMyAdmin，安装步骤基本按照源码本身提供的安装文档并结合系统实际情况进行的。
安装过程中，出现缺少其他软件包的情况，我们将使用yum安装，如果你的RHEL未付费无法使用yum，请配置CentOS的yum源，或者下载rpm包安装。
安装之前，防火墙与selinux均设置无效。
1.下载软件
[?](http://litvip.com/2011/02/18/26#)
```
```bash
[root@RHEL src]
```
```bash
# cd /usr/local/src/
```
```bash
[root@RHEL src]
```
```bash
# wget http://dev.mysql.com/get/Downloads/MySQL-5.1/mysql-5.1.50.tar.gz/from/http://ftp.iij.ad.jp/pub/db/mysql/
```
```bash
[root@RHEL src]
```
```bash
# wget http://jp.php.net/get/php-5.2.14.tar.bz2/from/this/mirror
```
```bash
[root@RHEL src]
```
```bash
# wget http://ftp.riken.jp/net/apache//httpd/httpd-2.2.16.tar.gz
```
```bash
[root@RHEL src]
```
```bash
# wget http://downloads.sourceforge.net/project/phpmyadmin/phpMyAdmin/3.3.7/phpMyAdmin-3.3.7-all-languages.tar.bz2
```
```
2.安装配置mysql
[?](http://litvip.com/2011/02/18/26#)
```
```bash
2.1.编译安装
```
```bash
[root@RHEL src]
```
```bash
# tar -xzvf mysql-5.1.50.tar.gz
```
```bash
[root@RHEL src]
```
```bash
# cd mysql-5.1.50
```
```bash
[root@RHEL mysql-5.1.50]
```
```bash
# more INSTALL-SOURCE
```
```bash
```
```bash
2.3.1. Source Installation Overview
```
```bash
```
```bash
The basic commands that you must execute to
```
```bash
install
```
```bash
a MySQL
```
```bash
source
```
```bash
```
```bash
distribution are:
```
```bash
```
```bash
shell> groupadd mysql
```
```bash
```
```bash
shell>
```
```bash
useradd
```
```bash
-g mysql mysql                     （一会我们修改一下 不让mysql用户能登录系统）
```
```bash
```
```bash
shell> gunzip < mysql-VERSION.
```
```bash
tar
```
```bash
.gz |
```
```bash
tar
```
```bash
-xvf - (我已经解压了 这步省略）
```
```bash
```
```bash
shell>
```
```bash
cd
```
```bash
mysql-VERSION                          （我们已经进入解压目录了）
```
```bash
```
```bash
shell> .
```
```bash
/configure
```
```bash
--prefix=
```
```bash
/usr/local/mysql
```
```bash
```
```bash
shell>
```
```bash
make
```
```bash
```
```bash
shell>
```
```bash
make
```
```bash
install
```
```bash
```
```bash
shell>
```
```bash
cp
```
```bash
support-files
```
```bash
/my-medium
```
```bash
.cnf
```
```bash
/etc/my
```
```bash
.cnf
```
```bash
```
```bash
shell>
```
```bash
cd
```
```bash
/usr/local/mysql
```
```bash
```
```bash
shell>
```
```bash
chown
```
```bash
-R mysql .
```
```bash
```
```bash
shell>
```
```bash
chgrp
```
```bash
-R mysql .
```
```bash
```
```bash
shell> bin
```
```bash
/mysql_install_db
```
```bash
--user=mysql
```
```bash
```
```bash
shell>
```
```bash
chown
```
```bash
-R root .
```
```bash
```
```bash
shell>
```
```bash
chown
```
```bash
-R mysql var
```
```bash
```
```bash
shell> bin
```
```bash
/mysqld_safe
```
```bash
--user=mysql &
```
```bash
[root@RHEL mysql-5.1.50]
```
```bash
# groupadd mysql
```
```bash
[root@RHEL mysql-5.1.50]
```
```bash
# useradd -g mysql -s /bin/false mysql
```
```bash
[root@RHEL mysql-5.1.50]
```
```bash
# ./configure --prefix=/usr/local/mysql
```
```bash
```
```bash
......
```
```bash
```
```bash
checking
```
```bash
for
```
```bash
termcap functions library... configure: error: No curses
```
```bash
/termcap
```
```bash
library found
```
```bash
```
```bash
......
```
```bash
```
```bash
OOPS!出错了！
```
```bash
[root@RHEL mysql-5.1.50]
```
```bash
# yum -y install ncurses-devel
```
```bash
（rpm下载地址http:
```
```bash
//ftp
```
```bash
.sjtu.edu.cn
```
```bash
/centos/5
```
```bash
.5
```
```bash
/os/i386/CentOS/ncurses-devel-5
```
```bash
.5-24.20060715.i386.rpm）
```
```bash
[root@RHEL mysql-5.1.50]
```
```bash
# ./configure --prefix=/usr/local/mysql
```
```bash
[root@RHEL mysql-5.1.50]
```
```bash
# make
```
```bash
[root@RHEL mysql-5.1.50]
```
```bash
# make install
```
```bash
[root@RHEL mysql-5.1.50]
```
```bash
# cd /usr/local/mysql
```
```bash
[root@RHEL mysql]
```
```bash
# chown -R mysql .
```
```bash
[root@RHEL mysql]
```
```bash
# chgrp -R mysql .
```
```bash
[root@RHEL mysql]
```
```bash
# bin/mysql_install_db --user=mysql
```
```bash
[root@RHEL mysql]
```
```bash
# chown -R root .
```
```bash
[root@RHEL mysql]
```
```bash
# chown -R mysql var
```
```bash
[root@RHEL mysql]
```
```bash
# bin/mysqld_safe --user=mysql &
```
```bash
2.2.确认mysql状态 并修改密码
```
```bash
[root@RHEL mysql]
```
```bash
# ps -ef | grep mysql
```
```bash
[root@RHEL mysql]
```
```bash
# /usr/local/mysql/bin/mysqladmin -u root password 'mysql'
```
```bash
[root@RHEL mysql]
```
```bash
# /usr/local/mysql/bin/mysql -uroot -p
```
```bash
Enter password:
```
```bash
Welcome to the MySQL monitor.  Commands end with ; or \g.
```
```bash
Your MySQL connection
```
```bash
id
```
```bash
is 2
```
```bash
Server version: 5.1.50 Source distribution
```
```bash
Copyright (c) 2000, 2010, Oracle and
```
```bash
/or
```
```bash
its affiliates. All rights reserved.
```
```bash
This software comes with ABSOLUTELY NO WARRANTY. This is
```
```bash
free
```
```bash
software,
```
```bash
and you are welcome to modify and redistribute it under the GPL v2 license
```
```bash
Type
```
```bash
'help;'
```
```bash
or
```
```bash
'\h'
```
```bash
for
```
```bash
help. Type
```
```bash
'\c'
```
```bash
to
```
```bash
clear
```
```bash
the current input statement.
```
```bash
mysql>
```
```bash
2.3.使用官方提供的启动脚本并添加开机启动
```
```bash
[root@RHEL mysql]
```
```bash
# cp share/mysql/mysql.server /etc/init.d/mysqld
```
```bash
[root@RHEL mysql]
```
```bash
# /etc/init.d/mysqld stop
```
```bash
Shutting down MySQL100909 20:06:48 mysqld_safe mysqld from pid
```
```bash
file
```
```bash
/usr/local/mysql/var/RHEL
```
```bash
.pid ended
```
```bash
```
```bash
[确定]
```
```bash
[1]+  Done                    bin
```
```bash
/mysqld_safe
```
```bash
--user=mysql
```
```bash
[root@RHEL mysql]
```
```bash
# /etc/init.d/mysqld start
```
```bash
Starting MySQL.                                            [确定]
```
```bash
[root@RHEL mysql]
```
```bash
# /sbin/chkconfig --add mysqld
```
```bash
[root@RHEL mysql]
```
```bash
# /sbin/chkconfig mysqld on
```
```
3.安装apache2.2
[?](http://litvip.com/2011/02/18/26#)
```
```bash
3.1.编译安装
```
```bash
[root@RHEL mysql]
```
```bash
# cd /usr/local/src/
```
```bash
[root@RHEL src]
```
```bash
# tar -xzvf httpd-2.2.16.tar.gz
```
```bash
[root@RHEL src]
```
```bash
# cd httpd-2.2.16
```
```bash
[root@RHEL httpd-2.2.16]
```
```bash
# more INSTALL
```
```bash
Quick Start - Unix
```
```bash
```
```bash
------------------
```
```bash
```
```bash
For complete installation documentation, see [ht]docs
```
```bash
/manual/install
```
```bash
.html or
```
```bash
```
```bash
http:
```
```bash
//httpd
```
```bash
.apache.org
```
```bash
/docs/2
```
```bash
.2
```
```bash
/install
```
```bash
.html
```
```bash
```
```bash
```
```bash
$ .
```
```bash
/configure
```
```bash
--prefix=PREFIX
```
```bash
```
```bash
$
```
```bash
make
```
```bash
```
```bash
$
```
```bash
make
```
```bash
install
```
```bash
```
```bash
$ PREFIX
```
```bash
/bin/apachectl
```
```bash
start
```
```bash
```
```bash
NOTES: * Replace PREFIX with the filesystem path under
```
```bash
which
```
```bash
```
```bash
Apache should be installed.  A typical installation
```
```bash
```
```bash
might use
```
```bash
"/usr/local/apache2"
```
```bash
for
```
```bash
PREFIX (without the
```
```bash
```
```bash
quotes).
```
```bash
*在他说的最简单的安装方式上 我们加2个编译参数
```
```bash
*--
```
```bash
enable
```
```bash
-rewrite        rule based URL manipulation
```
```bash
*--
```
```bash
enable
```
```bash
-so             DSO capability
```
```bash
[root@RHEL httpd-2.2.16]
```
```bash
# ./configure --prefix=/usr/local/apache2 --enable-rewrite --enable-so
```
```bash
[root@RHEL httpd-2.2.16]
```
```bash
# make
```
```bash
[root@RHEL httpd-2.2.16]
```
```bash
# make install
```
```bash
3.2.建立apache专用用户
```
```bash
[root@RHEL httpd-2.2.16]
```
```bash
# groupadd apache
```
```bash
[root@RHEL httpd-2.2.16]
```
```bash
# useradd -g apache -M -s /sbin/nologin apache
```
```bash
[root@RHEL httpd-2.2.16]
```
```bash
# vi /usr/local/apache2/conf/httpd.conf
```
```bash
```
```bash
找到如下2句 并在行头加
```
```bash
#号注释掉
```
```bash
```
```bash
User daemon
```
```bash
```
```bash
Group daemon
```
```bash
```
```bash
追加如下2句
```
```bash
```
```bash
User apache
```
```bash
```
```bash
Group apache
```
```bash
3.3.启动状态确认
```
```bash
[root@RHEL httpd-2.2.16]
```
```bash
# /usr/local/apache2/bin/apachectl start
```
```bash
[root@RHEL httpd-2.2.16]
```
```bash
# lsof -i:80
```
```bash
COMMAND   PID   USER   FD   TYPE DEVICE SIZE NODE NAME
```
```bash
httpd   14125   root    3u  IPv6 194322       TCP *:http (LISTEN)
```
```bash
httpd   14354 apache    3u  IPv6 194322       TCP *:http (LISTEN)
```
```bash
httpd   14355 apache    3u  IPv6 194322       TCP *:http (LISTEN)
```
```bash
httpd   14356 apache    3u  IPv6 194322       TCP *:http (LISTEN)
```
```bash
httpd   14357 apache    3u  IPv6 194322       TCP *:http (LISTEN)
```
```bash
httpd   14358 apache    3u  IPv6 194322       TCP *:http (LISTEN)
```
```bash
[root@RHEL httpd-2.2.16]
```
```bash
# curl http://localhost （你可以在浏览器里输入http://ip测试)
```
```bash
<html><body><h1>It works!<
```
```bash
/h1
```
```bash
><
```
```bash
/body
```
```bash
><
```
```bash
/html
```
```bash
>
```
```
*通过apachectl就可以方便的启动关闭apache了 我就不做启动脚本了
*如果你需要你可以上网搜一下 或者参照/etc/init.d/下的其他脚本 在apachectl的基础上修改一个
*或者 你可以再/etc/rc.local里追加一句来让apache开机启动/usr/local/apache2/bin/apachectl start
4.源码编译安装php
[?](http://litvip.com/2011/02/18/26#)
```
```bash
4.1.编译安装
```
```bash
root@RHEL httpd-2.2.16]
```
```bash
# cd /usr/local/src/
```
```bash
[root@RHEL src]
```
```bash
# tar -jxvf php-5.2.14.tar.bz2
```
```bash
[root@RHEL src]
```
```bash
# cd php-5.2.14
```
```bash
[root@RHEL php-5.2.14]
```
```bash
# more INSTALL
```
```bash
```
```bash
Apache 2.0 on Unix systems
```
```bash
```
```bash
·.......
```
```bash
```
```bash
You are highly encouraged to take a
```
```bash
look
```
```bash
at the Apache Documentation to
```
```bash
```
```bash
get a basic understanding of the Apache 2.0 Server.
```
```bash
```
```bash
......
```
```bash
```
```bash
Example 2-4. Installation Instructions (Apache 2 Shared Module Version)
```
```bash
```
```bash
1.
```
```bash
gzip
```
```bash
-d httpd-2_0_NN.
```
```bash
tar
```
```bash
.gz
```
```bash
```
```bash
2.
```
```bash
tar
```
```bash
xvf httpd-2_0_NN.
```
```bash
tar
```
```bash
```
```bash
3.  gunzip php-NN.
```
```bash
tar
```
```bash
.gz
```
```bash
```
```bash
4.
```
```bash
tar
```
```bash
-xvf php-NN.
```
```bash
tar
```
```bash
```
```bash
5.
```
```bash
cd
```
```bash
httpd-2_0_NN
```
```bash
```
```bash
6.  .
```
```bash
/configure
```
```bash
--
```
```bash
enable
```
```bash
-so
```
```bash
```
```bash
7.
```
```bash
make
```
```bash
```
```bash
8.
```
```bash
make
```
```bash
install
```
```bash
```
```bash
Now you have Apache 2.0.NN available under
```
```bash
/usr/local/apache2
```
```bash
,
```
```bash
```
```bash
configured with loadable module support and the standard MPM prefork.
```
```bash
```
```bash
To
```
```bash
test
```
```bash
the installation use your normal procedure
```
```bash
for
```
```bash
starting
```
```bash
```
```bash
the Apache server, e.g.:
```
```bash
```
```bash
/usr/local/apache2/bin/apachectl
```
```bash
start
```
```bash
```
```bash
and stop the server to go on with the configuration
```
```bash
for
```
```bash
PHP:
```
```bash
```
```bash
/usr/local/apache2/bin/apachectl
```
```bash
stop.
```
```bash
```
```bash
9.
```
```bash
cd
```
```bash
..
```
```bash
/php-NN
```
```bash
```
```bash
10. .
```
```bash
/configure
```
```bash
--with-apxs2=
```
```bash
/usr/local/apache2/bin/apxs
```
```bash
--with-mysql （一会我们指定一下mysql的安装路径)
```
```bash
```
```bash
11.
```
```bash
make
```
```bash
```
```bash
12.
```
```bash
make
```
```bash
install
```
```bash
```
```bash
.......
```
```bash
```
```bash
13. Setup your php.ini
```
```bash
```
```bash
cp
```
```bash
php.ini-dist
```
```bash
/usr/local/lib/php
```
```bash
.ini
```
```bash
```
```bash
.......
```
```bash
```
```bash
14. Edit your httpd.conf to load the PHP module.
```
```bash
```
```bash
......
```
```bash
```
```bash
For PHP 5:
```
```bash
```
```bash
LoadModule php5_module modules
```
```bash
/libphp5
```
```bash
.so
```
```bash
```
```bash
15. Tell Apache to parse certain extensions as PHP.
```
```bash
```
```bash
......
```
```bash
```
```bash
AddType application
```
```bash
/x-httpd-php
```
```bash
.php .phtml
```
```bash
```
```bash
It's also common to setup the .phps extension to show highlighted PHP
```
```bash
```
```bash
source
```
```bash
, this can be
```
```bash
done
```
```bash
with:
```
```bash
```
```bash
AddType application
```
```bash
/x-httpd-php-source
```
```bash
.phps
```
```bash
```
```bash
16. Use your normal procedure
```
```bash
for
```
```bash
starting the Apache server, e.g.:
```
```bash
```
```bash
/usr/local/apache2/bin/apachectl
```
```bash
start
```
```bash
```
```bash
......
```
```bash
[root@RHEL php-5.2.14]
```
```bash
# ./configure --with-apxs2=/usr/local/apache2/bin/apxs --with-mysql=/usr/local/mysql
```
```bash
```
```bash
......
```
```bash
```
```bash
configure: error: xml2-config not found. Please check your libxml2 installation.
```
```bash
```
```bash
......
```
```bash
```
```bash
OOPS!出错了！
```
```bash
[root@RHEL php-5.2.14]
```
```bash
# yum -y install libxml2-devel
```
```bash
（我的情况 安装libxml2-devel的时候 依赖于zlib-devel)
```
```bash
```
```bash
(rpm下载地址 http:
```
```bash
//ftp
```
```bash
.sjtu.edu.cn
```
```bash
/centos/5
```
```bash
.5
```
```bash
/os/i386/CentOS/zlib-devel-1
```
```bash
.2.3-3.i386.rpm)
```
```bash
```
```bash
(rpm下载地址 http:
```
```bash
//ftp
```
```bash
.sjtu.edu.cn
```
```bash
/centos/5
```
```bash
.5
```
```bash
/os/i386/CentOS/libxml2-devel-2
```
```bash
.6.26-2.1.2.8.i386.rpm）
```
```bash
[root@RHEL php-5.2.14]
```
```bash
# ./configure --with-apxs2=/usr/local/apache2/bin/apxs --with-mysql=/usr/local/mysql
```
```bash
[root@RHEL php-5.2.14]
```
```bash
# make
```
```bash
[root@RHEL php-5.2.14]
```
```bash
# make install
```
```bash
[root@RHEL php-5.2.14]
```
```bash
# cp php.ini-dist /usr/local/lib/php.ini
```
```bash
[root@RHEL php-5.2.14]
```
```bash
# vi /usr/local/apache2/conf/httpd.conf
```
```bash
```
```bash
在内容里搜一下是否有libphp5.so
```
```bash
```
```bash
我的情况 已经有了 如果没有追加下面那句前面不带
```
```bash
#的
```
```bash
```
```bash
# LoadModule foo_module modules/mod_foo.so
```
```bash
```
```bash
LoadModule php5_module        modules
```
```bash
/libphp5
```
```bash
.so
```
```bash
```
```bash
#
```
```bash
```
```bash
再搜索AddType关键字 在那附近追加如下一句
```
```bash
```
```bash
AddType application
```
```bash
/x-httpd-php
```
```bash
.php .phtml
```
```bash
```
```bash
再搜索下面一段 把第二句DirectoryIndex的后面追加index.php
```
```bash
```
```bash
<IfModule dir_module>
```
```bash
```
```bash
DirectoryIndex index.html index.php
```
```bash
```
```bash
<
```
```bash
/IfModule
```
```bash
>
```
```bash
4.1.2测试php网页
```
```bash
[root@RHEL php-5.2.14]
```
```bash
# cd /usr/local/apache2/htdocs/
```
```bash
[root@RHEL htdocs]
```
```bash
# vi index.php
```
```bash
```
```bash
输入内容
```
```bash
```
```bash
<?php
```
```bash
```
```bash
phpinfo();
```
```bash
```
```bash
?>
```
```bash
[root@RHEL php-5.2.14]
```
```bash
# /usr/local/apache2/bin/apachectl restart
```
```bash
[root@RHEL mysql]
```
```bash
# curl -I http://localhost/index.php
```
```bash
```
```bash
或者你在你的浏览器里输入http:
```
```bash
//yourip/index
```
```bash
.php
```
```bash
4.1.3测试php和mysql连接
```
```bash
[root@RHEL htdocs]
```
```bash
# vi phpmysql.php
```
```bash
```
```bash
输入内容
```
```bash
```
```bash
<?php
```
```bash
```
```bash
mysql_connect(
```
```bash
"localhost"
```
```bash
,
```
```bash
"root"
```
```bash
,
```
```bash
"mysql"
```
```bash
)
 or die(
```
```bash
"MySQL First Test:Failed"
```
```bash
);
```
```bash
```
```bash
echo
```
```bash
"MySQL First Test:Success"
```
```bash
;
```
```bash
```
```bash
?>
```
```bash
```
```bash
因为我们编译php的时候没有加--with-mysqli选项 所以不支持mysqli扩展
```
```bash
[root@RHEL htdocs]
```
```bash
# curl http://localhost/phpmysql.php
```
```bash
```
```bash
MySQL First Test:Success
```
```bash
```
```bash
好了 我这里成功了
```
```bash
```
```bash
你可以再浏览器里输入http:
```
```bash
//ip/phpmysql
```
```bash
.php
```
```
5.安装配置phpMyAdmin
[?](http://litvip.com/2011/02/18/26#)
```
```bash
5.1.配置phpMyAdmin
```
```bash
[root@RHEL mysql]
```
```bash
# cd /usr/local/src/
```
```bash
[root@RHEL src]
```
```bash
# tar -jxvf phpMyAdmin-3.3.7-all-languages.tar.bz2
```
```bash
我们把phpMyAdmin配置到
```
```bash
/var/www/
```
```bash
下 命名为phpMyAdmin
```
```bash
[root@RHEL src]
```
```bash
# test -d /var/www || mkdir -p /var/www
```
```bash
[root@RHEL src]
```
```bash
# mv phpMyAdmin-3.3.7-all-languages /var/www/phpMyAdmin
```
```bash
[root@RHEL src]
```
```bash
# cp /var/www/phpMyAdmin/config.sample.inc.php /var/www/phpMyAdmin/config.inc.php
```
```bash
[root@RHEL src]
```
```bash
# chmod 660 /var/www/phpMyAdmin/config.inc.php
```
```bash
[root@RHEL src]
```
```bash
# vi /var/www/phpMyAdmin/config.inc.php
```
```bash
```
```bash
追加$cfg[
```
```bash
'Lang'
```
```bash
] =
```
```bash
'zh-utf-8'
```
```bash
;
```
```bash
----------可选设置 为phpMyAdmin设置50位的短语密码---------
```
```bash
------推荐你设置 但是要使用mkpasswd命令 你需要安装expect包
```
```bash
[root@RHEL src]
```
```bash
# mkpasswd -l 50
```
```bash
clecXjgn7Hg%whkyajmmq8zisgzceinvcuqnwhbxlwapnzemwm
```
```bash
复制产生的短语密码
```
```bash
[root@RHEL src]
```
```bash
# vi /var/www/phpMyAdmin/config.inc.php
```
```bash
```
```bash
找到如下参数
```
```bash
```
```bash
$cfg[
```
```bash
'blowfish_secret'
```
```bash
] =
```
```bash
'clecXjgn7Hg%whkyajmmq8zisgzceinvcuqnwhbxlwapnzemwm'
```
```bash
----------------------可选设置 结束-----------------------
```
```bash
[root@RHEL src]
```
```bash
# chown -R root.apache /var/www/phpMyAdmin
```
```bash
[root@RHEL src]
```
```bash
# cd /usr/local/apache2/conf/
```
```bash
[root@RHEL conf]
```
```bash
# vi extra/phpmyadmin.conf
```
```bash
```
```bash
加入内容
```
```bash
```
```bash
Alias
```
```bash
/phpMyAdmin
```
```bash
/var/www/phpMyAdmin
```
```bash
```
```bash
<Location
```
```bash
/phpMyAdmin
```
```bash
>
```
```bash
```
```bash
Order deny,allow
```
```bash
```
```bash
Deny from all
```
```bash
```
```bash
Allow from 127.0.0.1
```
```bash
```
```bash
Allow from 10.110.108.55
```
```bash
//
```
```bash
这里改成你实际允许访问的ip
```
```bash
```
```bash
<
```
```bash
/Location
```
```bash
>
```
```bash
[root@RHEL conf]
```
```bash
# vi httpd.conf
```
```bash
```
```bash
追加
```
```bash
```
```bash
Include conf
```
```bash
/extra/phpmyadmin
```
```bash
.conf
```
```bash
[root@RHEL conf]
```
```bash
# /usr/local/apache2/bin/apachectl restart
```
```bash
5.2.测试 在浏览器输入下面的网址
```
```bash
```
```bash
http:
```
```bash
//ip/phpMyAdmin/
```
```bash
```
```bash
输入我们前面设置的mysql用户名root 和密码mysql
```
```bash
Enjoy!
```
```
注：
本文测试环境RHEL5.5，同样适合于CentOS5.5。
本文适合新手学习安装，不适合生产环境。
