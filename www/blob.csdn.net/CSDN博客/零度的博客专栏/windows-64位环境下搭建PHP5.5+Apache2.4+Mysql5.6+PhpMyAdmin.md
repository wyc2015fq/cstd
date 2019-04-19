# windows-64位环境下搭建PHP5.5+Apache2.4+Mysql5.6+PhpMyAdmin - 零度的博客专栏 - CSDN博客
2016年05月24日 17:34:21[零度anngle](https://me.csdn.net/zmx729618)阅读数：1328
# 一、软件准备
下载64位的Apache2.4.7： http://yun.baidu.com/s/1o6rwXh4
VC++ 2012：http://pan.baidu.com/s/1eQrDofK
php-5.5.6-Win32-VC11-x64（也可以去官方下载最新的） ：http://pan.baidu.com/s/1c0inUQO
在官网下载Mysql 5.6.16 64位版本： http://dev.mysql.com/downloads/mysql/
在官网点击链接下载PhpMyAdmin：http://www.phpmyadmin.net/home_page/index.php
# 二、解压安装Apache
解压Apache24到c:/Apache24（这是配 置里的服务器根目录，即ServerRoot的定义）。你的项目根目录为c:/Apache24/htdocs即DocumentRoot的定义。
如果解 压到其他目录下，需要对httpd.conf做出一些改动，包括ServerRoot、DocumentRoot、 Directories，ScriptAlias。需要改为你的Apache目录的地址。
开启一个dos窗口（cmd）：
切换到你的Apache 的bin目录下（dos下如何切换目录请自行百度）：执行httpd.exe 。
好吧！我知道你有可能跟我一样遇到问题了，报了一个错误：
![](https://img-blog.csdn.net/20140306164959343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmlzaW5nc3VuMDAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如果出现了这个问题，不要紧张，将第一步中下载好的VC++ 2012 安装完成，重新输入httpd.exe（注：如果不装VC++2012库，在安装PHP的时候也会出现问题哦。）。
如果httpd.conf配置正确的话，输入httpd回车后是没有任何提示的。不要关闭命令窗口（关闭命令窗口就是关闭Apache），本地浏览器访问。出现“It works”那么就说明apache已经正确安装了。
把Apache加入系统服务
关闭httpd命令窗口，不然会报错的。
开始 --- 运行，输入cmd，再打开一个命令提示符。分别输入如下命令（每行回车）
cd C:\Apache24\bin\httpd.exe -k install -n "servicename"              \\加入服务。servicename是服务里面的名字，可以自定义。
如果要卸载这个服务的话，先要停止这个服务，然后输入：httpd.exe -k uninstall -n "servicename" 卸载这个服务。
如果安装成功，就可以去你的apache目录下的bin里双击ApacheMonitor.exe管理apache服务了。
启动Apache后，在浏览器中输入： http://localhost/   ，回车后如果打印出It Works! 则代表安装apache 成功！ 
# 三、载入PHP模块
在windows下配置Apache+PHP，在配置PHP时，需要下载相应的版本，下载时要注意non-thread-safe和thread-safe这两个版本。
non-thread-safe是非安全线程主要与IIS搭配环境。
thread-safe安全线程与Apache搭配环境。
注意thread-safe版本中会有php5apache2_4.dll的动态链接库，与apache搭配。
如果解压到c盘，建议将php-5.5.16-Win32-VC11-x64文件夹名称改下，如：php5。
一、将该文件中的php.ini-development文件名改成：php.ini，并做如下修改：
      1.   extension_dir改为你的php下/ext目录，如改为extension_dir ="C:/php5/ext。并把前面的注释分号去掉。
      2.   修改时区date.timezone = PRC，基本配置完成。
二、在Apache的目录下修改  conf / httpd.conf
      1. 配置PHP：
             在<IfModulemime_module></IfModule>之间加入：AddType application/x-httpd-php .php
      2. 在尾部加上php（注意路径）：
              LoadModule php5_module "c:/php-5.5.16-x64/php5apache2_4.dll"
              PHPIniDir "c:/php-5.5.16-x64"
      3. 修改默认后缀，在原来index.html 的前面添加 index.php，如下：
              <IfModule dir_module>
                       DirectoryIndex index.php index.html index.htm
              < /IfModule>
重启Apache，在apache/htdocs 目录下新建一个index.php，内容为<?php phpinfo(); ?>保存，访问出现php的信息就说明php已经成功安装。
# 四、安装MySQL5.6
1. 将下载好的MySQL 5.6解压到你想要安装的目录，比如D:\mysql-5.6.10-winx64，这是MySQL的程序目录；
2. 将解压目录（D:\mysql-5.6.10-winx64）下的my-simple.ini 修改成my.ini
3. 编写my.ini文件(下面给出一个简单的配置例子，后期有需要可以再加)
[html] view plaincopyprint?
- <span style="font-size: 14px;">[client]   
- no-beep   
- 
- # pipe   
- # socket=mysql   
- port=3306   
- 
- [mysql]   
- 
- default-character-set=utf8   
- 
- # For advice on how to change settings please see  
- # http://dev.mysql.com/doc/refman/5.6/en/server-configuration-defaults.html  
- # *** DO NOT EDIT THIS FILE. It's a template which will be copied to the  
- # *** default location during install, and will be replaced if you  
- # *** upgrade to a newer version of MySQL.   
- 
- [mysqld]   
- 
- explicit_defaults_for_timestamp = TRUE   
- 
- # Remove leading # and set to the amount of RAM for the most important data  
- # cache in MySQL. Start at 70% of total RAM for dedicated server, else 10%.  
- innodb_buffer_pool_size = 2G   
- 
- # Remove leading # to turn on a very important data integrity option: logging  
- # changes to the binary log between backups.   
- # log_bin   
- 
- # These are commonly set, remove the # and set as required.  
- 
- basedir="D:\wamp\mysql5.6.16\"   
- datadir="D:\wamp\mysql5.6.16\data\"   
- 
- port=3306   
- server_id=1   
- 
- general-log=0   
- general_log_file="mysql_general.log"   
- slow-query-log=1   
- slow_query_log_file="mysql_slow_query.log"   
- long_query_time=10   
- 
- log-error="mysql_error_log.err"   
- 
- 
- # default-storage-engine=INNODB   
- # max_connections=1024   
- # query_cache_size=128M   
- # key_buffer_size=128M   
- # innodb_flush_log_at_trx_commit=1   
- # innodb_thread_concurrency=128   
- # innodb_autoextend_increment=128M   
- # tmp_table_size=128M   
- 
- 
- # Remove leading # to set options mainly useful for reporting servers.  
- # The server defaults are faster for transactions and fast SELECTs.  
- # Adjust sizes as needed, experiment to find the optimal values.  
- # join_buffer_size = 128M   
- # sort_buffer_size = 2M   
- # read_rnd_buffer_size = 2M    
- 
- #sql_mode=NO_ENGINE_SUBSTITUTION,STRICT_TRANS_TABLES   
- sql-mode="STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION"  
- character-set-server=utf8   
- </span>   
其中，下面的路径得修改成自己mysql的相关路径
[html] view plaincopyprint?
- <span style="font-size: 14px;">basedir="D:\wamp\mysql5.6.16\"  
- datadir="D:\wamp\mysql5.6.16\data\"</span>   
4. 输入mysqld --install 命令安装MySQL，将MySQL作为win系统服务，如果出错或想重命名服务名，可以mysqld --remove，具体可以看http://dev.mysql.com/doc/refman/5.1/en/windows-start-service.html；
5. 设置环境变量（方便命令行无需切换目录即可执行）
在Path的最后面加上D:\wamp\mysql5.6.16\bin  保存
6. 输入net start mysql启动服务；
OK，现在就可以通过mysql  -P3306 -uroot -p来跑数据库了。
默认root密码为空。
7. 在PHP中加入相应的扩展
打开php.ini 文件：
将下面3个之前的分号（；）去掉
extension=php_mbstring.dll
extension=php_mysql.dll
extension=php_mysqli.dll
# 五、安装PhpMyAdmin
1. 将下载的PhpMyAdmin解压到htdocs下。
2. 将config.sample.inc.php文件名修改为config.inc.php
3. 如果你的root的密码是空，将$cfg['Servers'][$i]['AllowNoPassword'] = false;  中的false修改为true即可
打开http://localhost/phpmyadmin/ 输入用户名密码登陆成功则证明登陆成功了。
