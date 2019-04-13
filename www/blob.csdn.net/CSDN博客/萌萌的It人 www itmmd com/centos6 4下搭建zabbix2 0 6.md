
# centos6.4下搭建zabbix2.0.6 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月22日 20:51:09[Jlins](https://me.csdn.net/dyllove98)阅读数：1228标签：[centos6.4																](https://so.csdn.net/so/search/s.do?q=centos6.4&t=blog)[zabbix2.0.6																](https://so.csdn.net/so/search/s.do?q=zabbix2.0.6&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=centos6.4&t=blog)


一服务端配置
1.配置前先关闭iptables和SELINUX，避免安装过程中报错。
[root@zabbix~]\# service
 iptables stop
[root@zabbix~]\# chkconfig
 iptables off
[root@zabbix~]\# getenforce
Disabled
[root@zabbix~]\#setenforce
 0
2.安装LAMP环境
[root@zabbix~]\# yum install
 -y httpd mysql mysql-server mysql-devel php php-mysql php-common php-mbstring php-gd php-odbc php-xml php-pear
[root@zabbix~]\# service
 httpd start
[root@zabbix~]\# service
 mysqld start
3.下载zabbix-2.0.6.tar.gz
root@zabbix~]\# wgethttp://sourceforge.net/projects/
 ... 0.6.tar.gz/download
4.安装zabbix所需的组件（server,agent)
[root@zabbix~]\# yum install
 -y curl curl-devel net-snmp net-snmp-devel perl-DBI
5.创建用户账号(server,agent)
[root@zabbix~]\# useradd
 zabbix
[root@zabbix~]\# usermod
 -s /sbin/nologin zabbix
6.创建zabbix数据库并导入zabbix数据库（server)
[root@zabbix~]\#tar zxf
 zabbix-2.0.6.tar.gz
[root@zabbix~]\# mysqladmin
 -u root password 123456
[root@zabbix~]\# mysql –uroot
 –p123456
mysql> create database zabbix;
mysql> grant all on zabbix.* to zabbix@localhost identified by '123456';
mysql> use zabbix;
mysql> source /root/zabbix-2.0.6/database/mysql/schema.sql
mysql> source /root/zabbix-2.0.6/database/mysql/images.sql
mysql> source /root/zabbix-2.0.6/database/mysql/data.sql
mysql> exit
7.编译安装，如果是客户端安装选择--enable-agent就行了。
[root@zabbix~]\#cd /root/zabbix-2.0.6
[root@zabbixzabbix-2.0.6]\#
 ./configure --enable-server --enable-agent --with-mysql --with-net-snmp --with-libcurl（可以用--prefix=/usr/local/指定安装目录）
报错：checking for main in -lmysqlclient... no
configure: error: Not found mysqlclient library
解决：[root@zabbixzabbix-2.0.6]\#
 ln -s /usr/lib64/mysql/libmysqlclient.so.16 /usr/lib64/mysql/libmysqlclient.so
***********************************************************
*            Now run 'make install'                       *
*            Thank you for using Zabbix!                  *
*              <http://www.zabbix.com>             
       *
***********************************************************
make install
8.编辑配置文件
[root@zabbixzabbix-2.0.6]\#
 grep -vE '(^\#|" "|^)' /usr/local/etc/zabbix_server.conf >> /usr/local/etc/zabbix_server.conf.bak
[root@zabbixzabbix-2.0.6]\#
 cd /usr/local/etc/
[root@zabbixetc]\# cp zabbix_server.conf.bak
 zabbix_server.conf
[root@zabbixetc]\# grep
 -vE '(^\#|" "|^$)' zabbix_agentd.conf >> zabbix_agentd.conf.bak
[root@zabbixetc]\# cp zabbix_agentd.conf.bak
 zabbix_agentd.conf
cp: overwrite `zabbix_agentd.conf'? y
[root@zabbixetc]\# more
 zabbix_server.conf \#服务端的配置
LogFile=/var/log/zabbix_server.log
DBHost=localhost
DBName=zabbix
DBUser=zabbix
DBPassword=123456
我这查看的时候是空的，可能前面操作有问题吧，如果是空的，直接用vim编辑，把上面几行复制进去就行了
[root@zabbixetc]\# more
 zabbix_agentd.conf \#客户端配置
LogFile=/tmp/zabbix_agentd.log
Server=127.0.0.1
ServerActive=127.0.0.1
Hostname=Zabbix server
[root@zabbixetc]\# touch
 /var/log/zabbix_server.log
[root@zabbixetc]\# touch
 /var/log/zabbix_agentd.log
[root@zabbixetc]\# chmod
 777 /var/log/zabbix_*
9.启动服务（server,agent）
[root@zabbixzabbix-2.0.6]\#
 cp misc/init.d/tru64/zabbix_server /etc/init.d/
[root@zabbixzabbix-2.0.6]\#
 cp misc/init.d/tru64/zabbix_agentd /etc/init.d/
[root@zabbixzabbix-2.0.6]\#
 chmod +x /etc/init.d/zabbix_*
在文件头部\#!/bin/bash下面分别添加两行
\#!/bin/sh
\#chkconfig: 35 95 95
\#description:zabbix Agent server
添加服务
[root@zabbixzabbix-2.0.6]\#
 chkconfig --add zabbix_server
[root@zabbixzabbix-2.0.6]\#
 chkconfig --add zabbix_agentd
设置服务器开机自动启动
[root@zabbix~]\# chkconfig
 zabbix_server on
[root@zabbix~]\# chkconfig
 zabbix_agentd on
[root@zabbix~]\# /etc/init.d/zabbix_server
 start
Zabbix server started.
[root@zabbix~]\# /etc/init.d/zabbix_agentd
 start
Zabbix agent started.
[root@zabbix~]\# ps -ef
 |grep zabbix
zabbix    1803     1  0 08:21 ?        00:00:00 /usr/local/sbin/zabbix_agentd
zabbix    1805  1803  0 08:21 ?        00:00:00 /usr/local/sbin/zabbix_agentd
zabbix    1806  1803  0 08:21 ?        00:00:00 /usr/local/sbin/zabbix_agentd
zabbix    1807  1803  0 08:21 ?        00:00:00 /usr/local/sbin/zabbix_agentd
zabbix    1808  1803  0 08:21 ?        00:00:00 /usr/local/sbin/zabbix_agentd
zabbix    1814     1  0 08:21 ?        00:00:00 /usr/local/sbin/zabbix_server
root      2043  2006  0 08:23 pts/0    00:00:00 grep zabbix
[root@zabbix~]\# netstat
 -lantp |grep 10050
tcp        0      0 0.0.0.0:10050               0.0.0.0:*                   LISTEN      1803/zabbix_agentd
[root@zabbix~]\# netstat
 -lantp |grep 10051
tcp        0      0 0.0.0.0:10051               0.0.0.0:*                   LISTEN      2308/zabbix_server
10.安装zabbix web界面（server)
复制php文件，zabbix的终端程序是用php写的，因此需要一个支持php脚本解析的web服务器。然后将frontends/php下面的php文件拷贝到web服务器html文件目录下面。
[root@zabbixzabbix-2.0.6]\#
 cp -r frontends/php /var/www/html/zabbix
[root@zabbixhtml]\#service
 httpd restart
Stopping httpd: [  OK  ]
Starting httpd: [  OK  ]
11.安装图示
![wKioL1RwRj7ToFTcAAK9ZKPNkII432.gif](http://s3.51cto.com/wyfs02/M01/53/CB/wKioL1RwRj7ToFTcAAK9ZKPNkII432.gif)

![wKioL1RwRj7ARRV_AAERs92FstM190.gif](http://s3.51cto.com/wyfs02/M02/53/CB/wKioL1RwRj7ARRV_AAERs92FstM190.gif)
报错：根据提示修改/etc/php.ini文件里的参数
[root@zabbixzabbix]\# yum
 install -y php-bcmath
[root@zabbixzabbix]\# /etc/init.d/httpd
 restart
Stopping httpd: [  OK  ]
Starting httpd: [  OK  ]


![wKiom1RwRcPySQAoAADuzMVRZro348.gif](http://s3.51cto.com/wyfs02/M00/53/CD/wKiom1RwRcPySQAoAADuzMVRZro348.gif)

![wKiom1RwRcSBhAmPAADEb-k8ZEw366.gif](http://s3.51cto.com/wyfs02/M00/53/CD/wKiom1RwRcSBhAmPAADEb-k8ZEw366.gif)

![wKioL1RwRj-DduldAADf6BTb3CQ215.gif](http://s3.51cto.com/wyfs02/M02/53/CB/wKioL1RwRj-DduldAADf6BTb3CQ215.gif)

![wKioL1RwRkCTLB83AADU3-hJATU223.gif](http://s3.51cto.com/wyfs02/M00/53/CB/wKioL1RwRkCTLB83AADU3-hJATU223.gif)

如果提示创建文件失败，更改目录权限就行了
![wKioL1RwRkDj3AR4AAFYGDw8vQY231.jpg](http://s3.51cto.com/wyfs02/M01/53/CB/wKioL1RwRkDj3AR4AAFYGDw8vQY231.jpg)
初始用户名admin密码zabbix

![wKioL1RwRkHT76VBAALmj-3-an4890.jpg](http://s3.51cto.com/wyfs02/M02/53/CB/wKioL1RwRkHT76VBAALmj-3-an4890.jpg)
![wKiom1RwRcfQqZrEAAEoVnITTEg213.gif](http://s3.51cto.com/wyfs02/M02/53/CD/wKiom1RwRcfQqZrEAAEoVnITTEg213.gif)
二.WINDOWS客户端安装
1.官网上下载windows客户端zabbix

2.拷贝到windows服务器上
解压后，如果是64位系统，则把\bin\win64\下面的三个文件拷贝到c:\zabbix目录下。如果是32位则同理
3.在c:\zabbix目录下新建zabbix_agentd.conf文件，添加以下内容
LogFile=c:\zabbix\zabbix_agentd.log \#日志文件
Server=172.16.21.119      \#服务端的IP
ListenIP=10.6.1.126      \#客户端IP
Hostname=61      \#客户端主机名
4.安装zabbix客户端
![wKiom1RwRcjBqRULAAAVro0_R_8909.gif](http://s3.51cto.com/wyfs02/M00/53/CD/wKiom1RwRcjBqRULAAAVro0_R_8909.gif)
5.启动zabbix客户端

![wKiom1RwRciAzxxeAAANk3Camvk770.gif](http://s3.51cto.com/wyfs02/M01/53/CD/wKiom1RwRciAzxxeAAANk3Camvk770.gif)
6.查看端口是否监听
![wKioL1RwRkTBHlh7AAAL1jtPyqM373.gif](http://s3.51cto.com/wyfs02/M02/53/CB/wKioL1RwRkTBHlh7AAAL1jtPyqM373.gif)

三.服务端测试
[root@10-6-8-200 ~]\# zabbix_get  -s 10.6.2.227 -k agent.ping
1
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

