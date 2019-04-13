
# Linux下使用Cacti监控流量状况 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月16日 20:39:07[Jlins](https://me.csdn.net/dyllove98)阅读数：2055


Cacti 在英文中的意思是仙人掌的意思，Cacti是一套基于PHP,MySQL,SNMP及RRDTool开发的网络流量监测图形分析工具，使用PHP开发，它是由一些PHP页面组成软件，主要针对企业的大量网络设备、服务器主机***能监测服务，可提供对CPU负载、内存占用、运行进程数、磁盘空间、网卡流量等各种数据信息的监测，cacti注重的是状态监控，报警和服务恢复功能比较差。

官方站点：
[http://www.cacti.net](http://www.cacti.net/)
下载页面：
[http://www.cacti.net/downloads/cacti-0.8.8b.tar.gz](http://www.cacti.net/downloads/cacti-0.8.8b.tar.gz)

![wKiom1RXnxPRRpLMAAT4JaswXNQ738.jpg](http://s3.51cto.com/wyfs02/M00/4D/B4/wKiom1RXnxPRRpLMAAT4JaswXNQ738.jpg)

开始Cacti安装部署
机器IP地址：192.168.1219.129

软件下载：
[root@rhel64 ~]\#wget http://www.cacti.net/downloads/cacti-0.8.8a.tar.gz
[root@rhel64 ~]\#wget http://oss.oetiker.ch/rrdtool/pub/rrdtool-1.4.5.tar.gz
[root@rhel64 ~]\#wget http://www.cacti.net/downloads/spine/cacti-spine-0.8.8a.tar.gz

安装lamp环境：
[root@rhel64 ~]\#yum install -y httpd mysql  mysql-server php php-mysql php-json php-pdo

安装相关的lib库：
[root@rhel64 ~]\#yum install -y cairo-devel libxml2-devel pango pango-devel

安装编译器：
[root@rhel64 ~]\#yum -y install gcc make gcc-c++

解压编译软件：
[root@rhel64 ~]\#tar -xzf rrdtool-1.4.5.tar.gz
[root@rhel64 ~]\#cd rrdtool-1.4.5
[root@rhel64 rrdtool-1.4.5]\#./configure --prefix=/usr/local/rrdtool/
[root@rhel64 rrdtool-1.4.5]\#make && make install
[root@rhel64 rrdtool-1.4.5]\#ln -s /usr/local/rrdtool/bin/* /usr/local/bin/

安装snmp服务：
[root@rhel64 rrdtool-1.4.5]\#yum -y install net-snmp net-snmp-utils

安装Cacti主程序：
[root@rhel64 ~]\#tar xzf cacti-0.8.8a.tar.gz && mv cacti-0.8.8a /var/www/html/cacti

创建Cacti数据库：
[root@rhel64 ~]\#service mysqld start
[root@rhel64 ~]\#/usr/bin/mysqladmin -u root -h rhel64 password '123456'
[root@rhel64 ~]\#/usr/bin/mysqladmin -u root password '123456'
[root@rhel64 ~]\#mysql -uroot -p
mysql>create database cacti;
Query OK, 1 row affected (0.00 sec)
mysql>grant all on cacti.* to cacti@'localhost' identified by "123456";
Query OK, 0 rows affected (0.00 sec)
mysql>flush privileges;
Query OK, 0 rows affected (0.00 sec)
mysql>


把Cacti数据导入Cacti数据库：
[root@rhel64 ~]\#mysql -ucacti -p123456 cacti</var/www/html/cacti/cacti.sql
[root@rhel64 ~]\#chmod -R 777 /var/www/html/cacti/rra
[root@rhel64 ~]\#chmod -R 777 /var/www/html/cacti/log
[root@rhel64 ~]\#vi /var/www/html/cacti/include/config.php
<?php
/*
+-------------------------------------------------------------------------+
| Copyright (C) 2004-2012 The Cacti Group                                 |
|                                                                         |
| This program is free software; you can redistribute it and/or           |
| modify it under the terms of the GNU General Public License             |
| as published by the Free Software Foundation; either version 2          |
| of the License, or (at your option) any later version.                  |
|                                                                         |
| This program is distributed in the hope that it will be useful,         |
| but WITHOUT ANY WARRANTY; without even the implied warranty of          |
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           |
| GNU General Public License for more details.                            |
+-------------------------------------------------------------------------+
| Cacti: The Complete RRDTool-based Graphing Solution                     |
+-------------------------------------------------------------------------+
| This code is designed, written, and maintained by the Cacti Group. See  |
| about.php and/or the AUTHORS file for specific developer information.   |
+-------------------------------------------------------------------------+
| http://www.cacti.net/                                                   |
+-------------------------------------------------------------------------+
*/

/* make sure these values refect your actual database/host/user/password */
$database_type = "mysql";
$database_default = "cacti";
$database_hostname = "localhost";
$database_username = "cacti";
$database_password = "123456";
$database_port = "3306";
$database_ssl = false;

/*
Edit this to point to the default URL of your Cacti install
ex: if your cacti install as at http://serverip/cacti/ this
would be set to /cacti/
*/
//$url_path = "/cacti/";

/* Default session name - Session name must contain alpha characters */
//$cacti_session_name = "Cacti";

?>

[root@rhel64 ~]\#setenforce 0
[root@rhel64 ~]\#crontab -e
*/5  * * * * /usr/bin/php /var/www/html/cacti/poller.php >>/tmp/cacti_rrdtool.log 2>&1

http://192.168.1219.129/cacti/

一直点击next下去即可，用户名和密码默认是admin.admin，第一次登陆需要修改密码。

![wKiom1RoC_OS2okFAAH3sVv6v2E614.jpg](http://s3.51cto.com/wyfs02/M00/53/76/wKiom1RoC_OS2okFAAH3sVv6v2E614.jpg)

![wKioL1RoDInCSq8YAADMxZAAmtg434.jpg](http://s3.51cto.com/wyfs02/M02/53/74/wKioL1RoDInCSq8YAADMxZAAmtg434.jpg)

![wKiom1RoDDfQKYlFAAGOu2xxR-I955.jpg](http://s3.51cto.com/wyfs02/M02/53/76/wKiom1RoDDfQKYlFAAGOu2xxR-I955.jpg)


![wKioL1RoDPjxtdaBAAC7LAGaJVE970.jpg](http://s3.51cto.com/wyfs02/M00/53/74/wKioL1RoDPjxtdaBAAC7LAGaJVE970.jpg)

![wKioL1RoDR6BI7HpAADm5H-QcHo223.jpg](http://s3.51cto.com/wyfs02/M01/53/74/wKioL1RoDR6BI7HpAADm5H-QcHo223.jpg)


![wKiom1RoDMzixi0MAAMjWrGAzGM275.jpg](http://s3.51cto.com/wyfs02/M01/53/76/wKiom1RoDMzixi0MAAMjWrGAzGM275.jpg)


定义监控哪些设备：
控制面版-->建立设备-->定义设备-->为设备生成图像-->在图像树中添加新设备
console-->Devices-->add(左上角)-->define devices-->Create Graphs for this Host-->create
在被监控客户机上要安装net-snmp,41行那里要填写监控端的IP地址。
[root@rhel64 ~]\# yum -y install net-snmp
[root@rhel64 ~]\# vim /etc/snmp/snmpd.conf
62 access  notConfigGroup ""      any       noauth    exact  all none none
41 com2sec notConfigUser  192.168.1219.129       public
85 view all    included  .1                               80

[root@rhel64 ~]\# service snmpd start

待续...
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

