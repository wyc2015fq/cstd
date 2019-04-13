
# 腾讯云实验室：搭建 LNMP 环境 - 阳光岛主 - CSDN博客

2017年11月10日 21:37:31[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：1199


LNMP，表示Linux、Nginx、MySQL、PHP的集合服务器环境，它是基于CentOS/Debian编写的Nginx、PHP、MySQL、phpMyAdmin、eAccelerator一键安装包。可以在云服务器、VPS、独立主机上轻松的安装LNMP生产环境。
Nginx是俄罗斯人编写的十分轻量级的HTTP服务器，它的发音为 “engine X”， 是一个高性能的HTTP和反向代理服务器。在高并发连接的情况下，Nginx是Apache服务器的替代品。Nginx同时也可以作为7层负载均衡服务器来使用。Nginx 0.8.46 + PHP 5.2.14 (FastCGI) 可以承受3万以上的并发连接数，相当于同等环境下Apache的10倍。
**本文原文转自米扑博客：****腾讯云实验室：搭建 LNMP 环境**
**准备工作**
**操作系统环境**
系统：CentOS release 6.8 (Final)
内核：Linux VM_124_53_centos 2.6.32-642.6.2.el6.x86_64
CPU：单核
内存：1GB
**系统参数**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|\# uname -a
|Linux VM_124_53_centos 2.6.32-642.6.2.el6.x86_64|\#1 SMP Wed Oct 26 06:52:09 UTC 2016 x86_64 x86_64 x86_64 GNU/Linux
|\# free
|total       used|free|shared    buffers     cached
|Mem:       1020128     763204     256924        388      43516     537620
|-/+ buffers|/cache|:     182068     838060
|Swap:            0          0          0
|\# whoami
|root
|

**安装 lsb_release**
yum -y install redhat-lsb
**查看系统版本**
\# lsb_release -a
LSB Version:    :base-4.0-amd64:base-4.0-noarch:core-4.0-amd64:core-4.0-noarch:graphics-4.0-amd64:graphics-4.0-noarch:printing-4.0-amd64:printing-4.0-noarch
Distributor ID: CentOS
Description:    CentOS release 6.8 (Final)
Release:        6.8
Codename:       Final
**top 命令**
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|top|- 10:48:57 up  1:26,  1 user,  load average: 0.29, 0.45, 0.24
|Tasks:  94 total,   1 running,  93 sleeping,   0 stopped,   0 zombie
|Cpu(s):  0.0%us,  0.3%sy,  0.0%ni, 99.7%|id|,  0.0%wa,  0.0%hi,  0.0%si,  0.0%stMem:   1020128k total,   763180k used,   256948k|free|,    43548k buffers
|Swap:        0k total,        0k used,        0k|free|,   537724k cached
|PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND
|1 root      20   0 19364 1544 1228 S  0.0  0.2   0:00.69 init
|2 root      20   0     0    0    0 S  0.0  0.0   0:00.00 kthreadd
|3 root      RT   0     0    0    0 S  0.0  0.0   0:00.00 migration|/0
|4 root      20   0     0    0    0 S  0.0  0.0   0:00.02 ksoftirqd|/0
|5 root      RT   0     0    0    0 S  0.0  0.0   0:00.00 stopper|/0
|6 root      RT   0     0    0    0 S  0.0  0.0   0:00.00 watchdog|/0
|7 root      20   0     0    0    0 S  0.0  0.0   0:00.16 events|/0
|8 root      20   0     0    0    0 S  0.0  0.0   0:00.00 events|/0
|9 root      20   0     0    0    0 S  0.0  0.0   0:00.00 events_long|/0
|10 root      20   0     0    0    0 S  0.0  0.0   0:00.00 events_power_ef
|11 root      20   0     0    0    0 S  0.0  0.0   0:00.00 cgroup
|12 root      20   0     0    0    0 S  0.0  0.0   0:00.00 khelper
|


**一、搭建 Nginx 静态服务器**
1. 安装 Nginx
yum install nginx -y
2. 修改/etc/nginx/conf.d/default.conf
去除对 IPv6 地址的监听，可参考下面的代码示例：
vim/etc/nginx/conf.d/default.conf
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|server {
|listen       80 default_server;
|\# listen       [::]:80 default_server;
|server_name  _;
|root|/usr/share/nginx/html|;
|\# Load configuration files for the default server block.
|include|/etc/nginx/default|.d/*.conf;
|location / {
|}
|error_page 404|/404|.html;
|location =|/40x|.html {
|}
|error_page 500 502 503 504|/50x|.html;
|location =|/50x|.html {
|}
|}
|

3. 启动 Nginx
nginx
4. 查看网页
可访问实验机器外网 HTTP 服务（http://119.29.116.97）来确认是否已经安装成功。
![teng-xun-yun-shi-yan-shi-da-jian-lnmp-huan-jing-01](https://blog.mimvp.com/wp-content/uploads/2017/09/teng-xun-yun-shi-yan-shi-da-jian-lnmp-huan-jing-01.png)
5. 将 Nginx 设置为开机启动
chkconfig nginx on
6. 查看开机启动的所有进程（chkconfig）
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|\# chkconfig
|abrt-ccpp       0:off   1:off   2:off   3:on    4:off   5:on    6:off
|abrtd           0:off   1:off   2:off   3:on    4:off   5:on    6:off
|acpid           0:off   1:off   2:on    3:on    4:on    5:on    6:off
|atd             0:off   1:off   2:off   3:on    4:on    5:on    6:off
|auditd          0:off   1:off   2:on    3:on    4:on    5:on    6:off
|blk-availability        0:off   1:on    2:on    3:on    4:on    5:on    6:off
|bootlocal       0:off   1:off   2:off   3:on    4:off   5:off   6:off
|crond           0:off   1:off   2:on    3:on    4:on    5:on    6:off
|cups            0:off   1:off   2:on    3:on    4:on    5:on    6:off
|htcacheclean    0:off   1:off   2:off   3:off   4:off   5:off   6:off
|httpd           0:off   1:off   2:off   3:off   4:off   5:off   6:off
|ip6tables       0:off   1:off   2:off   3:off   4:off   5:off   6:off
|iptables        0:off   1:off   2:on    3:on    4:on    5:on    6:off
|irqbalance      0:off   1:off   2:off   3:off   4:off   5:off   6:off
|kdump           0:off   1:off   2:on    3:on    4:on    5:on    6:off
|lvm2-monitor    0:off   1:on    2:on    3:on    4:on    5:on    6:off
|mdmonitor       0:off   1:off   2:on    3:on    4:on    5:on    6:off
|messagebus      0:off   1:off   2:on    3:on    4:on    5:on    6:off
|mysqld          0:off   1:off   2:on    3:on    4:on    5:on    6:off
|netconsole      0:off   1:off   2:off   3:off   4:off   5:off   6:off
|netfs           0:off   1:off   2:off   3:on    4:on    5:on    6:off
|network         0:off   1:off   2:on    3:on    4:on    5:on    6:off
|nfs-rdma        0:off   1:off   2:off   3:off   4:off   5:off   6:off
|nginx           0:off   1:off   2:off   3:off   4:off   5:off   6:off
|ntpd            0:off   1:off   2:off   3:off   4:off   5:off   6:off
|ntpdate         0:off   1:off   2:off   3:off   4:off   5:off   6:off
|php-fpm         0:off   1:off   2:on    3:on    4:on    5:on    6:off
|portreserve     0:off   1:off   2:on    3:on    4:on    5:on    6:off
|postfix         0:off   1:off   2:on    3:on    4:on    5:on    6:off
|psacct          0:off   1:off   2:off   3:off   4:off   5:off   6:off
|quota_nld       0:off   1:off   2:off   3:off   4:off   5:off   6:off
|rdisc           0:off   1:off   2:off   3:off   4:off   5:off   6:off
|rdma            0:off   1:off   2:off   3:off   4:off   5:off   6:off
|restorecond     0:off   1:off   2:off   3:off   4:off   5:off   6:off
|rsyslog         0:off   1:off   2:on    3:on    4:on    5:on    6:off
|saslauthd       0:off   1:off   2:off   3:off   4:off   5:off   6:off
|sshd            0:off   1:off   2:on    3:on    4:on    5:on    6:off
|udev-post       0:off   1:on    2:on    3:on    4:on    5:on    6:off
|


**二、安装 MySQL 数据库服务**
1. 安装 MySQL
yum -y install mysql-server
[CentOS 安装配置MariaDB](https://blog.mimvp.com/2015/02/centos-installation-configuring-mariadb/)
MariaDB 官方安装教程：[MariaDB 10.2 CentOS repository list](https://downloads.mariadb.org/mariadb/repositories/#mirror=neusoft&distro=CentOS&distro_release=centos7-amd64--centos7&version=10.2)
**CentOS 7**vim /etc/yum.repos.d/MariaDB-10.2.repo
|1
|2
|3
|4
|5
|[mariadb]
|name = MariaDB
|baseurl = http:|//yum|.mariadb.org|/10|.2|/centos7-amd64
|gpgkey=https:|//yum|.mariadb.org|/RPM-GPG-KEY-MariaDB
|gpgcheck=1
|

安装命令：
\# yum remove MariaDB-server MariaDB-client
yum -y install MariaDB-client MariaDB-server MariaDB-devel
2. 启动 MySQL 服务
service mysqld restart
3. 设置 MySQL 账户 root 密码
/usr/bin/mysqladmin -u root password 'mimvp-demo'
4. 登录MySQL查看数据库
mysql -u root -p'mimvp-demo'
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|\# mysql -u root -p'mimvp-demo'
|Welcome to the MySQL monitor.  Commands end with ; or \g.
|Your MySQL connection|id|is 40
|Server version: 5.1.73 Source distribution
|Copyright (c) 2000, 2013, Oracle and|/or|its affiliates. All rights reserved.
|Oracle is a registered trademark of Oracle Corporation and|/or|its
|affiliates. Other names may be trademarks of their respective
|owners.
|Type|'help;'|or|'\h'|for|help. Type|'\c'|to|clear|the current input statement.
|mysql>
|mysql>
|mysql> show databases;
|+--------------------+
|| Database           |
|+--------------------+
|| information_schema |
|| mysql              |
|||test||
|+--------------------+
|3 rows|in|set|(0.28 sec)
|

5. 将 MySQL 设置为开机自动启动：
chkconfig mysqld on

**三、搭建 PHP 环境**
1. 安装 PHP
yum -y install php php-fpm php-mysql
注：CentOS 6 默认已经安装了 PHP-FPM 及 PHP-MYSQL，下面命令执行的可能会提示已经安装。
2. 安装之后，启动 PHP-FPM 进程
service php-fpm start
3. 查看启动的端口号
netstat -nlpt | grep php-fpm
启动之后，可以使用下面的命令查看 PHP-FPM 进程监听哪个端口（默认监听9000端口）
netstat -nlpt | grep php-fpm|1
|2
|\# netstat -nlpt | grep php-fpm
|tcp        0      0 127.0.0.1:9000              0.0.0.0:*                   LISTEN      13894|/php-fpm
|

4. 把 PHP-FPM 也设置成开机自动启动：
chkconfig php-fpm on

**四、配置 Nginx 并运行 PHP 程序**
1. 配置 Nginx
在/etc/nginx/conf.d目录中新建一个名为 php.conf 的文件，并配置 Nginx 端口 ，配置示例如下：
vim /etc/nginx/conf.d/php.conf
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|server {
|listen 8000;
|\# pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000
|location ~ .php$ {
|root|/usr/share/php|;
|fastcgi_pass   127.0.0.1:9000;
|fastcgi_index  index.php;
|fastcgi_param  SCRIPT_FILENAME  $document_root$fastcgi_script_name;
|include        fastcgi_params;
|}
|}
|

2. 重启 nginx 服务
service nginx restart
这时候，我们就可以在/usr/share/php目录下新建一个 info.php 文件来检查 php 是否安装成功了，文件内容参考如下：
vim /usr/share/php/info.php
|1
|2
|3
|4
|\# cat /usr/share/php/info.php
|<?php
|echo|phpinfo();
|?>
|

3. 访问网页
访问http://119.29.116.97:8000/info.php可浏览到我们刚刚创建的 info.php 页面了
![teng-xun-yun-shi-yan-shi-da-jian-lnmp-huan-jing-02](https://blog.mimvp.com/wp-content/uploads/2017/09/teng-xun-yun-shi-yan-shi-da-jian-lnmp-huan-jing-02.png)
![teng-xun-yun-shi-yan-shi-da-jian-lnmp-huan-jing-03](https://blog.mimvp.com/wp-content/uploads/2017/09/teng-xun-yun-shi-yan-shi-da-jian-lnmp-huan-jing-03.png)

**五、Nginx 应用实例**
米扑科技的大多产品，建立在 LNMP 和 LAMP 服务器框架上，示例如下：
米扑科技：[http://mimvp.com](http://mimvp.com/)（LNMP + LAMP）
米扑代理：[http://proxy.mimvp.com](http://proxy.mimvp.com/)（LNMP）
米扑支付：[http://pay.mimvp.com](http://pay.mimvp.com/)（LNMP）
米扑财富：[http://money.mimvp.com](http://money.mimvp.com/)（LNMP）
米扑博客：[https://blog.mimvp.com](https://blog.mimvp.com/)（LAMP）
米扑论坛：[http://forum.mimvp.com](http://forum.mimvp.com/)（LAMP）


