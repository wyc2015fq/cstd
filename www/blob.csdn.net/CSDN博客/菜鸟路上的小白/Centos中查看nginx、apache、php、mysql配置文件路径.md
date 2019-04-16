# Centos中查看nginx、apache、php、mysql配置文件路径 - 菜鸟路上的小白 - CSDN博客





2016年09月06日 15:02:01[somnus_小凯](https://me.csdn.net/u012486840)阅读数：10190








很多朋友都可能会碰到安装好环境之后不知道怎么查看配置环境的文件路径了，下面我就来介绍centos中nginx、apache、php、mysql配置文件路径查看方法吧。





1、判断[apache](http://www.111cn.net/list-121/)

首先执行命令找到httpd路径
[ps](http://www.111cn.net/fw/photo.html) aux | grep httpd

如httpd路径为 /usr/local/apache/bin/httpd

然后执行以下命令

/usr/local/apache/bin/httpd -V | grep “SERVER_CONFIG_FILE”

即可找到编译时加载的配置文件路径 httpd.conf

-V 参数可以看到编译时配置的参数

2、判断[nginx](http://www.111cn.net/list-195/)

首先执行命令找到nginx路径

ps aux | grep nginx

如nginx路径为

/usr/local/nginx/sbin/nginx

然后执行以下命令

/usr/local/nginx/sbin/nginx -V

默认放在 安装目录下 conf/nginx.conf

3、判断mysql

首先执行命令找到mysql路径

ps aux | grep mysqld

如mysqld路径为

/usr/bin/mysql

然后执行以下命令

/usr/bin/mysql –verbose –help | grep -A 1 ‘Default options’

或

/usr/bin/mysql –print-defaults

4、判断php加载的配置文件路径

（1）、可通过php函数phpinfo来查看，写个文件，然后用网址访问一下，查找“Loaded Configuration File”对应的值即为php加载的配置文件

（2）、如果是nginx+[php配置](http://www.111cn.net/tags.php/php%C5%E4%D6%C3/)，也可以通过查找php执行路径

ps aux | grep php

如，路径为 /usr/local/nginx/sbin/php-fpm

然后执行以下命令

/usr/local/nginx/sbin/php-fpm -i | grep “Loaded Configuration File”

即可看到php加载的配置文件

(3)、如果是apache+mod_php配置，也可以在[apache配置](http://www.111cn.net/tags.php/apache%C5%E4%D6%C3/)文件中查看加载的php.ini路径。如 PHPIniDir “/usr/local/apache/conf/php.ini”

当然也有简单的方法，就是通过find来搜索

如

find / -name nginx.conf

find / -name php.ini

find / -name my.cnf

find / -name httpd.conf



