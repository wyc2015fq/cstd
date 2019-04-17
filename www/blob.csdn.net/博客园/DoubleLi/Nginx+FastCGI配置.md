# Nginx+FastCGI配置 - DoubleLi - 博客园






安装过程中参考：张宴 [http://blog.s135.com/nginx_php_v5/](http://blog.s135.com/nginx_php_v5/)
nginx对我来说是一个新的内容，安装的包比较多，在经过了一段时间对nginx的了解之后，才真正的配置成功，本文在经过第二轮的nginx配置学习过程中，不断的完善，不断的理解。对以后再次配置是一个很好的总结。

**1.1 nginx概述nginx简介**
Nginx是俄罗斯人编写的十分轻量级的HTTP服务器,Nginx，它的发音为“engine X”， 是一个高性能的HTTP和反向代理服务器，同时也是一个IMAP/POP3/SMTP 代理服务器．Nginx是由俄罗斯人 Igor Sysoev为俄罗斯访问量第二的 Rambler.ru站点开发的，它已经在该站点运行超过两年半了。Igor Sysoev在建立的项目时,使用基于BSD许可。
在高并发连接的情况下，Nginx是Apache服务器不错的替代品。Nginx同时也可以作为7层负载均衡服务器来使用。

**1.2 nginx安装准备 1.2.1 nginx环境准备**
**1．系统要求：Linux 2.6+ 内核，本文中的Linux操作系统为redhat AS 5.3。2．获取程序并安装**
RedHat等其他Linux发行版可从安装光盘中找到这些程序库的RPM包RedHat可以直接利用CentOS的RPM包安装。
**可以用rpm安装以下包，如有关联包，安装时一起安装。**
gcc gcc-c++ autoconf libjpeg libjpeg-devel libpng libpng-devel freetype freetype-devel libxml2 libxml2-devel zlib zlib-devel glibc glibc-devel glib2 glib2-devel bzip2 bzip2-devel ncurses ncurses-devel curl curl-devel e2fsprogs e2fsprogs-devel krb5 krb5-devel libidn libidn-devel openssl openssl-devel openldap openldap-devel nss_ldap openldap-clients openldap-servers
以上包如果安装了的话，不需要再安装了。

**3．RPM包搜索网站**
[http://rpm.pbone.net/](http://rpm.pbone.net/)
[http://www.rpmfind.net/](http://www.rpmfind.net/)

**4．其它准备工作**
如果采用半RPM安装的话，PHP的一些支持包，之前应该事先安装好，全部采用tar包安装的话，中间出现问题的可能性比较大。
所以应事先安装好dg库的支持包，也可以采用tar安装。

**1.2.2 nginx软件包准备**
Nginx所需要的软件包可以从下面位置获取，也可以记住名字在google中搜索。
nginx-0.7.61.tar.gz
php-5.2.10.tar.gz
php-5.2.10-fpm-0.5.11.diff.gz
mysql-5.1.35.tar.gz
libiconv-1.13.tar.gz
libmcrypt-2.5.8.tar.gz
mcrypt-2.6.8.tar.gz
memcache-2.2.5.tgz
mhash-0.9.9.9.tar.gz
pcre-7.9.tar.gz
eaccelerator-0.9.5.3.tar.bz2
PDO_MYSQL-1.0.2.tgz
ImageMagick.tar.gz
imagick-2.2.2.tgz

**1.3 FastCGI安装**
Fastcgi可以安装到其它的电脑上，可以实现分步式处理，需要修改相关配置文件来实现，本章内容讲的是一台电脑上实现所有功能。
**1.3.1 PHP支持环境准备**
编译安装PHP 5.2.10所需的支持库：
**1.安装libiconv**
对文本进行编码间的转换，用它来处理中文各种编码之间的转换。
#tar zxvf libiconv-1.13.tar.gz
#cd libiconv-1.13/
#./configure --prefix=/usr/local
#make
#make install
cd ../

**2.安装libmcrypt 实现加密功能的库。**
# tar zxvf libmcrypt-2.5.8.tar.gz 
# cd libmcrypt-2.5.8/
# ./configure
# make
# make install
# /sbin/ldconfig
**# 注：这里不要退出去了。**
# cd libltdl/
# ./configure --enable-ltdl-install
# make
# make install
# cd ../../

**3. 安装mhash(哈稀函数库)**
# tar zxvf mhash-0.9.9.9.tar.gz
# cd mhash-0.9.9.9/
# ./configure
# make
# make install
# cd ../

ln -s /usr/local/lib/libmcrypt.la /usr/lib/libmcrypt.la
ln -s /usr/local/lib/libmcrypt.so /usr/lib/libmcrypt.so
ln -s /usr/local/lib/libmcrypt.so.4 /usr/lib/libmcrypt.so.4
ln -s /usr/local/lib/libmcrypt.so.4.4.8 /usr/lib/libmcrypt.so.4.4.8
ln -s /usr/local/lib/libmhash.a /usr/lib/libmhash.a
ln -s /usr/local/lib/libmhash.la /usr/lib/libmhash.la
ln -s /usr/local/lib/libmhash.so /usr/lib/libmhash.so
ln -s /usr/local/lib/libmhash.so.2 /usr/lib/libmhash.so.2
ln -s /usr/local/lib/libmhash.so.2.0.1 /usr/lib/libmhash.so.2.0.1

**4. 安装mcrypt**
# tar zxvf mcrypt-2.6.8.tar.gz
# cd mcrypt-2.6.8/
# /sbin/ldconfig
#./configure
# make
# make install
# cd ../

**注：DG库所需要安装包，可以采用rpm包来安装，减少时间，由于php已经集成GD库，但前提支持包应事先安装，如zlib,png,jpeg,freetype等。如果完全想采用tar包安装的话，请参考lamp相关内容。**

**1.3.2 MySql环境准备**
建立mysql组，建立mysql用户并且加入到mysql组中
# groupadd mysql
# useradd mysql -g mysql
# tar zxvf mysql-5.1.24-rc.tar.gz
# cd mysql-5.1.24-rc
# ./configure --prefix=/usr/local/mysql
--without-debug 
--with-extra-charsets=gbk
--with-extra-charsets=all 
--enable-assembler 
--with-pthread
--enable-thread-safe-client 
--with-mysqld-ldflags=-all-static  /*不带共享库的形式编译mysqld*/
--with-client-ldflags=-all-static
--with-big-tables 
--with-readline    /*要采用rpm方式安装ncurses或tar包安装*/
--with-ssl            /*要采用rpm方式安装openssl*/
--with-embedded-server 
--enable-local-infile 
--with-plugins=innobase
# make && make install

# /usr/local/mysql/bin/mysql_install_db --user=mysql 
#以mysql身份初始化数据库
# cp ./support-files/mysql.server /etc/init.d/mysql 
#复制Mysql启动服务至系统
# cp ./support-files/my-medium.cnf /etc/my.cnf
# chmod 755 /etc/init.d/mysql

# cd /usr/local/mysql/   #切换到cd /usr/local/mysql/目录下
# chown -R root .        #改变当前目录下的所有者为mysql用户
# chown -R mysql var     #修改数据库目录的权限
# chgrp -R mysql .       #改变当前目录下的mysql用户的文件为mysql组

# /usr/local/mysql/bin/mysqld_safe --user=mysql& 
# /usr/local/mysql/bin/mysqladmin -u root password 'admin' #设置管理员密码
# /usr/local/mysql/bin/mysql -u root -p   #测试密码输入
Enter password: 
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 6
Server version: 5.1.36-log Source distribution

Type 'help;' or '\h' for help.

mysql> \q       /*退出mysql*/

# chkconfig --add mysql  #添加mysqld服务到系统
# chkconfig mysql on     #打开myslqd服务
# service mysql start    #启动Mysql
#/usr/local/mysql/bin/mysqladmin shutdown  #关闭数据库

#查看mysql端口的打开情况
# netstat -tunlp 
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address   Foreign Address  State   PID/P name 
tcp        0      0 0.0.0.0:3306    0.0.0.0:*        LISTEN  2936/    
#查看是否启动：
#ps -ef ｜ grep mysql
注：在配置过程中，整体的配置思路从上往下，其中的一些功能可以用在实际的配置过程中，主要用于测试环节中。
Mysql的优化部分这里没有给出，需要参考其它资料。

**1.3.3 PHP安装支持FastCGI模式1．编译安装PHP（FastCGI模式）**
# tar zxvf php-5.2.10.tar.gz
# gzip -cd php-5.2.10-fpm-0.5.11.diff.gz | patch -d php-5.2.10 -p1
# cd php-5.2.10/
# ./configure --prefix=/usr/local/php 
--with-config-file-path=/usr/local/php/etc 
--with-mysql=/usr/local/mysql 
--with-mysqli=/usr/local/mysql/bin/mysql_config 
--with-iconv-dir=/usr/local 
--with-freetype-dir 
--with-jpeg-dir 
--with-png-dir 
--with-zlib 
--with-gd 
--enable-gd-native-ttf 
--with-libxml-dir=/usr 
--enable-xml 
--disable-rpath 
--enable-discard-path 
--enable-safe-mode 
--enable-bcmath 
--enable-shmop 
--enable-sysvsem 
--enable-inline-optimization 
--with-curl 
--with-curlwrappers 
--enable-mbregex 
**--enable-fastcgi --enable-fpm --enable-force-cgi-redirect**
--enable-mbstring 
--with-mcrypt 
--with-openssl 
--with-mhash 
--enable-pcntl 
--enable-sockets 
--with-ldap 
--with-ldap-sasl 
--with-xmlrpc 
--enable-zip 
--enable-soap 
--without-pear

**#注：make的时候一定要加上后面的参数，才能成功。**
# make ZEND_EXTRA_LIBS='-liconv'
# make install
# cp php.ini-dist /usr/local/php/etc/php.ini
# cd ../

注：在安装过程中采用了tar包与rpm混合安装的情况，对于库的指定确实出现了很大的麻烦。如果采用rpm安装的话，不需要指定支持包的位置就可以了，tar安装的话，需要指定安装位置。

**1.3.4编译安装PHP5扩展模块1.安装memcache**
# tar zxvf memcache-2.2.5.tgz
# cd memcache-2.2.5/
# /usr/local/php/bin/phpize
# ./configure --with-php-config=/usr/local/php/bin/php-config
# make
# make install
# 说明：memcache库的位置
Installing shared extensions:
/usr/local/php/lib/php/extensions/no-debug-non-zts-20060613/
# cd ..

**2.安装eaccelerator php加速**
# tar jxvf eaccelerator-0.9.5.3.tar.bz2
# cd eaccelerator-0.9.5.3/
# /usr/local/php/bin/phpize
# ./configure --enable-eaccelerator=shared  --with-php-config=/usr/local/php/bin/php-config
# make
# make install
Installing shared extensions: 
/usr/local/php/lib/php/extensions/no-debug-non-zts-20060613/
# cd ../

**3.安装PDO_MYSQL(数据库连接的支持)**
# tar zxvf PDO_MYSQL-1.0.2.tgz
# cd PDO_MYSQL-1.0.2/
# /usr/local/php/bin/phpize
#./configure --with-php-config=/usr/local/php/bin/php-config
--with-pdo-mysql=/usr/local/mysql
# make
# make install
Installing shared extensions:
/usr/local/php/lib/php/extensions/no-debug-non-zts-20060613/
# cd ../

**4.安装ImageMagick是Linux下非常强大的图象处理函数与GD类似.**
# tar zxvf ImageMagick.tar.gz
# cd ImageMagick-6.5.1-2/
#./configure
# make
# make install
# cd ../

**5.安装imagick(连接PHP和ImageMagick的通道)**
# tar zxvf imagick-2.2.2.tgz
# cd imagick-2.2.2/
# /usr/local/php/bin/phpize
# ./configure --with-php-config=/usr/local/php/bin/php-config
# make
# make install
Installing shared extensions:
/usr/local/php/lib/php/extensions/no-debug-non-zts-20060613/
# cd ../

**6.修改php.ini文件,已使php支持扩展的功能**
vi /usr/local/php/etc/php.ini
查找
extension_dir = "./"
修改为
extension_dir="/usr/local/php/lib/php/extensions/no-debug-non-zts-20060613/"
并在此行后增加以下几行，然后保存：
extension = "memcache.so"
extension = "pdo_mysql.so"
extension = "imagick.so"

再查找output_buffering = Off
修改为output_buffering = On

**7.配置eAccelerator加速PHP：**
mkdir -p /usr/local/eaccelerator_cache
vi /usr/local/php/etc/php.ini

到配置文件的最末尾，粘上以下内容:
[eaccelerator]
zend_extension="/usr/local/php/lib/php/extensions/no-debug-non-zts-20060613/eaccelerator.so"
eaccelerator.shm_size="64"
eaccelerator.cache_dir="/usr/local/eaccelerator_cache"
eaccelerator.enable="1"
eaccelerator.optimizer="1"
eaccelerator.check_mtime="1"
eaccelerator.debug="0"
eaccelerator.filter=""
eaccelerator.shm_max="0"
eaccelerator.shm_ttl="3600"
eaccelerator.shm_prune_period="3600"
eaccelerator.shm_only="0"
eaccelerator.compress="1"
eaccelerator.compress_level="9"

**1.3.5 PHP-fpm配置1. 创建php-fpm配置文件**
php-fpm是为PHP打的一个FastCGI管理补丁，可以平滑变更php.ini配置而无需重启php-cgi:
在/usr/local/php/etc/目录中创建php-fpm.conf文件，也可以在原有的基础上进行修改。
如果您安装 Nginx + PHP 用于程序调试
请将以下的
<value name="display_errors">0</value>改为
<value name="display_errors">1</value>，以便显示PHP错误信息，否则，Nginx 会报状态为500的空白错误页。
说明：创建www用户与组，这里创建了下面就不用创建了。

#/usr/sbin/groupadd www -g 48 
#/usr/sbin/useradd -u 48 -g www www

rm -f /usr/local/php/etc/php-fpm.conf
vi /usr/local/php/etc/php-fpm.conf
###############################
#输入或者是修改为以下内容:
###############################
<?xml version="1.0" ?> 
<configuration> 
<section name="global_options"> 
<value name="pid_file">/usr/local/php/logs/php-fpm.pid</value> 
<value name="error_log">/usr/local/php/logs/php-fpm.log</value> 
<value name="log_level">notice</value> 
<value name="emergency_restart_threshold">10</value> 
<value name="emergency_restart_interval">1m</value> 
<value name="process_control_timeout">5s</value> 
<value name="daemonize">yes</value> 
</section> 
<workers> 
<section name="pool"> 
<value name="name">default</value> 
<value name="listen_address">127.0.0.1:9000</value> 
<value name="listen_options"> 
<value name="backlog">-1</value> 
<value name="owner"></value> 
<value name="group"></value> 
<value name="mode">0666</value> 
</value> 
<value name="php_defines"> 
<value name="sendmail_path">/usr/sbin/sendmail -t -i</value> 
<value name="display_errors">1</value> 
</value> 
<value name="user">www</value> 
<value name="group">www</value> 
<value name="pm"> 
<value name="style">static</value> 
<value name="max_children">128</value> 
<value name="apache_like"> 
<value name="StartServers">20</value> 
<value name="MinSpareServers">5</value> 
<value name="MaxSpareServers">35</value> 
</value> 
</value> 
<value name="request_terminate_timeout">0s</value> 
<value name="request_slowlog_timeout">0s</value> 
<value name="slowlog">logs/slow.log</value> 
<value name="rlimit_files">51200</value> 
<value name="rlimit_core">0</value> 
<value name="chroot"></value> 
<value name="chdir"></value> 
<value name="catch_workers_output">yes</value> 
<value name="max_requests">500</value> 
<value name="allowed_clients">127.0.0.1</value> 
<value name="environment"> 
<value name="HOSTNAME">$HOSTNAME</value> 
<value name="PATH">/usr/local/bin:/usr/bin:/bin</value> 
<value name="TMP">/tmp</value> 
<value name="TMPDIR">/tmp</value> 
<value name="TEMP">/tmp</value> 
<value name="OSTYPE">$OSTYPE</value> 
<value name="MACHTYPE">$MACHTYPE</value> 
<value name="MALLOC_CHECK_">2</value> 
</value> 
</section> 
</workers> 
</configuration>

**2.php-fpm启动与管理**
/usr/local/php/sbin/php-fpm start
注：/usr/local/php/sbin/php-fpm还有其他参数，包括：
start|stop|quit|restart|reload|logrotate，修改php.ini后不重启php-cgi，重新加载配置文件使用reload，就保持了在php的fastcgi进程持续运行的状态下，又重新加载了php.ini。

**1.4 Nginx安装 1.4.1 nginx安装**
Nginx只是web服务器，配合php技术实现的fastcgi来提高性能。
**1、安装rewrite模块支持包pcre库：**# tar zxvf pcre-7.8.tar.gz
# cd pcre-7.8/
# ./configure
# make && make install
cd ../

**2、安装Nginx**
说明:创建www用户组及www用户，如果之前php-fpm没有创建，这里要创建。
# /usr/sbin/groupadd www
# /usr/sbin/useradd -g www www

# tar zxvf nginx-0.7.59.tar.gz
# cd nginx-0.7.59/
#./configure --user=www --group=www --prefix=/usr/local/nginx --with-http_stub_status_module --with-http_ssl_module

nginx path prefix: "/usr/local/nginx"
  nginx binary file: "/usr/local/nginx/sbin/nginx"
  nginx configuration prefix: "/usr/local/nginx/conf"
  nginx configuration file: "/usr/local/nginx/conf/nginx.conf"
  nginx pid file: "/usr/local/nginx/logs/nginx.pid"
  nginx error log file: "/usr/local/nginx/logs/error.log"
  nginx http access log file: "/usr/local/nginx/logs/access.log"
  nginx http client request body temporary files: "client_body_temp"
  nginx http proxy temporary files: "proxy_temp"
  nginx http fastcgi temporary files: "fastcgi_temp"


# make && make install
# cd ../

**3．Nginx 安装后只有一个程序文件，本身并不提供各种管理程序，它是使用参数和系统信号机制对 Nginx 进程本身进行控制的。**
Nginx 的参数包括有如下几个：
-c <path_to_config>：使用指定的配置文件而不是 conf 目录下的 nginx.conf 。
-t：测试配置文件是否正确，在运行时需要重新加载配置的时候，此命令非常重要，用来检测所修改的配置文件是否有语法错误。
-v：显示 nginx 版本号。
-V：显示 nginx 的版本号以及编译环境信息以及编译时的参数。
例如我们要测试某个配置文件是否书写正确，我们可以使用以下命令
sbin/nginx -t -c conf/nginx.conf

**1.4.2 nginx配置**
** 1.在/usr/local/nginx/conf/目录中创建nginx.conf文件：**rm -f /usr/local/nginx/conf/nginx.conf
vi /usr/local/nginx/conf/nginx.conf
=======================================
nginx.conf才是nginx web服务器的配置文件
=======================================
user  www www;        /*启动nginx服务的用户与组*/
worker_processes 8;   /*启动nginx服务的工作进程*/
error_log  logs/nginx_error.log  crit; /*错误日志，以及等级*/
pid        /usr/local/nginx/nginx.pid;  /*nginx服务进程PID*/

worker_rlimit_nofile 51200; 

events 
{
 use epoll;                /*工作模式*/
 worker_connections 51200; /*每进程允许最大的同时连接数*/
}

http 
{
 include       mime.types;
 default_type  application/octet-stream;

 #charset  gb2312;
 server_names_hash_bucket_size 128;
 client_header_buffer_size 32k;
 large_client_header_buffers 4 32k;

 sendfile on;
 tcp_nopush     on;
 keepalive_timeout 60;
 tcp_nodelay on;

 fastcgi_connect_timeout 300;
 fastcgi_send_timeout 300;
 fastcgi_read_timeout 300;
 fastcgi_buffer_size 64k;
 fastcgi_buffers 4 64k;
 fastcgi_busy_buffers_size 128k;
 fastcgi_temp_file_write_size 128k;

 gzip on;
 gzip_min_length  1k;
 gzip_buffers     4 16k;
 gzip_http_version 1.0;
 gzip_comp_level 2;
 gzip_types text/plain application/x-javascript text/css application/xml;
 gzip_vary on;

 #limit_zone  crawler  $binary_remote_addr  10m;

 server
 {
   listen       80;               /*监听端口*/
   server_name  localhost;        /*服务器名称*/
   index index.html index.htm index.php;  /*缺省主页名称*/
   root  /usr/local/nginx/html;   /*网站根目录，也可以采用下面内容*/
#也可以采用相对路径,下面注释部分*/
   #location / {
   #         root   html;
   #         index  index.html index.htm;
   # }

   #limit_conn   crawler  20;    

   #通过FastCGI方式支持PHP，php页面由fastcgi代理处理，这也是反向代理的一个应用，这里可以是jsp/asp等脚本。
   location ~ .*\.(php|php5)?$
   {      
     #fastcgi_pass  unix:/tmp/php-cgi.sock;
     fastcgi_pass  127.0.0.1:9000;   /*fastcgi监听端口*/
     fastcgi_index index.php;
     include fcgi.conf;       /*fastcgi配置文件，修改为以下内容*/
   }

   #对于某一类型的文件，设置过期时间，静态的页面通常设置长一点。
   #静态文件，nginx自己处理
   location ~ .*\.(gif|jpg|jpeg|png|bmp|swf|js|css)$
   {
     expires      30d;
   }

#日志的格式
   log_format  access  '$remote_addr - $remote_user [$time_local] "$request" '
             '$status $body_bytes_sent "$http_referer" '
             '"$http_user_agent" $http_x_forwarded_for';
   access_log  logs/access.log  access;
     }
}
说明:以上配置文件只是基本配置文件，要实现其它功能的话，需要在此基础上进行修改。

**2.在/usr/local/nginx/conf/目录中创建fcgi.conf文件：**
说明：可以直接粘贴以下内容。
vi /usr/local/nginx/conf/fcgi.conf
fastcgi_param  GATEWAY_INTERFACE  CGI/1.1;
fastcgi_param  SERVER_SOFTWARE    nginx;

fastcgi_param  QUERY_STRING       $query_string;
fastcgi_param  REQUEST_METHOD     $request_method;
fastcgi_param  CONTENT_TYPE       $content_type;
fastcgi_param  CONTENT_LENGTH     $content_length;

fastcgi_param  SCRIPT_FILENAME    $document_root$fastcgi_script_name;
fastcgi_param  SCRIPT_NAME        $fastcgi_script_name;
fastcgi_param  REQUEST_URI        $request_uri;
fastcgi_param  DOCUMENT_URI       $document_uri;
fastcgi_param  DOCUMENT_ROOT      $document_root;
fastcgi_param  SERVER_PROTOCOL    $server_protocol;

fastcgi_param  REMOTE_ADDR        $remote_addr;
fastcgi_param  REMOTE_PORT        $remote_port;
fastcgi_param  SERVER_ADDR        $server_addr;
fastcgi_param  SERVER_PORT        $server_port;
fastcgi_param  SERVER_NAME        $server_name;

# PHP only, required if PHP was built with --enable-force-cgi-redirect
fastcgi_param  REDIRECT_STATUS    200;

**1.4.3 nginx启动与管理1．启动nginx**
/usr/local/nginx/sbin/nginx

**2．测试nginx配置文件**
修改/usr/local/nginx/conf/nginx.conf配置文件后，请执行以下命令检查配置文件是否正确：
# /usr/local/nginx/sbin/nginx -t
如果屏幕显示以下两行信息，说明配置文件正确：
the configuration file /usr/local/nginx/conf/nginx.conf syntax is ok
the configuration file /usr/local/nginx/conf/nginx.conf was tested successfully

**3．查看Nginx主进程号**
ps -ef | grep "nginx: master process" | grep -v "grep" | awk -F ' ' '{print $2}'
屏幕显示的即为Nginx主进程号，例如：
6302
这时，执行以下命令即可使修改过的Nginx配置文件生效：
kill -HUP 6302
或者无需这么麻烦，找到Nginx的Pid文件：
kill -HUP `cat /usr/local/nginx/logs/nginx.pid`

**4．配置开机自动启动Nginx + PHP**
vi /etc/rc.local
加入以下内容:
ulimit -SHn 51200
/usr/local/php/sbin/php-fpm start
/usr/local/nginx/sbin/nginx

**5.测试nginx**
应事先准备
vi /usr/local/nginx/html/test.php
<?
phpinfo();
?>
(1)http://192.168.129.21
(2)http://192.168.129.21/test.php

**1.4.4 nginx配置总结**
nginx对我来说是一个新的内容，安装的包比较多，在经过了一段时间对nginx的了解之后，才真正的配置成功，本文只是实现了一个最基本的功能，对于nginx高性能的使用在配置文件中没有实现。在以后的学习过程中还需要更深入的去了解，比如：反向代理，以及nginx+fastcgi分步式的环境配置。

**1.5 Nginx基本配置管理1.5.1 nginx配置基础1、正则表达式匹配**
 ~  区分大小写匹配
 ~* 不区分大小写匹配
 !~和!~*分别为区分大小写不匹配及不区分大小写不匹配
 ^  以什么开头的匹配
 $  以什么结尾的匹配
 \  转义字符。可以转. * ?等
 *  代表任意字符

**2、文件及目录匹配**
 -f和!-f用来判断是否存在文件
 -d和!-d用来判断是否存在目录
 -e和!-e用来判断是否存在文件或目录
 -x和!-x用来判断文件是否可执行
例:
location = /
#匹配任何查询，因为所有请求都已 / 开头。但是正则表达式规则和长的块规则将被优先和查询匹配
location ^~ /images/ {
# 匹配任何已/images/开头的任何查询并且停止搜索。任何正则表达式将不会被测试。
location ~* \.(gif|jpg|jpeg)$ {
# 匹配任何已.gif、.jpg 或 .jpeg 结尾的请求

**3、一些可用的全局变量**
$args
$content_length
$content_type
$document_root
$document_uri
$host
$http_user_agent
$http_cookie
$limit_rate
$request_body_file
$request_method
$remote_addr
$remote_port
$remote_user
$request_filename
$request_uri
$query_string
$scheme
$server_protocol
$server_addr
$server_name
$server_port
$uri

**1.5.2 nginx切割日志1、创建脚本/usr/local/nginx/sbin/cut_nginx_log.sh，输入以下内容：**
#!/bin/bash
# This script run at 00:00

# The Nginx logs path
logs_path="/usr/local/nginx/logs/"
mkdir -p ${logs_path}$(date -d "yesterday" +"%Y")/$(date -d "yesterday" +"%m")/
mv ${logs_path}access.log ${logs_path}$(date -d "yesterday" +"%Y")/$(date -d "yesterday" +"%m")/access_$(date -d "yesterday" +"%Y%m%d").log
kill -USR1 `cat /usr/local/nginx/logs/nginx.pid`

**2、设置crontab，每天凌晨00:00切割nginx访问日志**
crontab -e
输入以下内容：
00 00 * * * /bin/bash  /usr/local/nginx/sbin/cut_nginx_log.sh

**1.5.3 nginx状态监控**
在nginx.conf根目录下加入以下内容，过程如下：
**1.修改配置文件**
vi /usr/local/nginx/conf/nginx.conf
    #以下面加入以下内容
location /NginxStatus {
   stub_status             on;
   access_log              on;
   auth_basic              "NginxStatus";
}    

**2．重启nginx服务**
(1)kill -HUP `cat /usr/local/nginx/logs/nginx.pid`
(2)如果不行的话，先杀，再启动

**3．测试**
[http://192.168.129.21/NginxStatus](http://192.168.129.21/NginxStatus)
Active connections: 1 
server accepts handled requests
#nginx 总共处理了3个连接, 成功创建3 次握手, 总共处理了14个请求
3 3 14 
Reading: 0 Writing: 1 Waiting: 0

**1.5.4 nginx目录浏览**
在nginx.conf根目录下加入以下内容，过程如下：
**1.修改配置文件**
vi /usr/local/nginx/conf/nginx.conf
    #以下面加入以下内容
#里面有要这一句就可以了，其它可以注释
#注：与 { 要有空格，否则会出错
location / {
autoindex on;
}

**2．重启nginx服务**
(1)kill -HUP `cat /usr/local/nginx/logs/nginx.pid`
(2)如果不行的话，先杀，再启动

**3．测试**
[http://192.168.129.21](http://192.168.129.21/)
![Nginx+FastCGI配置 - koumm - koumm的博客](http://img.bimg.126.net/photo/asYd9TdSiV6ZRfI4oGvH-Q==/3417106217268651116.jpg)

**1.5.5 nginx禁止访问某类文件**
在nginx.conf根目录下加入以下内容，过程如下：
修改配置文件如下
vi /usr/local/nginx/conf/nginx.conf
#以下面加入以下内容
#里面有要这一句就可以了，其它可以注释
#注：与 { 要有空格，否则会出错
**方法一:**
location ~* \.(txt|doc)$ {
if (-f $request_filename) {
root /usr/local/nginx/html/test;
break;
}
}
**方法二**
location ~* \.(txt|doc)$ {
root /usr/local/nginx/html/test;
deny all;
}
说明：用正则表达式进行匹配，来执行相应的处理。

**1.5.6 nginx静态文件处理**
第一种方法:根据文件类型expires
location ~* \.(js|css|jpg|jpeg|gif|png|swf)$ {
if (-f $request_filename) {
root /usr/local/nginx/html/;
expires 1d;
}
break;
}
**第二种方法:根据判断某个目录**
location ~ ^/(images|javascript|js|css|flash|media|static)/ {
root /usr/local/nginx/html/;
expires 30d;
}

**1.5.7 nginx动态文件处理**
nginx只是一个web服务器，本身没有apache那么强大的功能，通常支持各种模块来实现支持多种功能。Nginx本身只能处理静态页的解析，如果要解析动态页面的话，需要用到反向代理的功能。就是说，把php等动态页面请求转发给后端其它web主机处理(例如：tomcat/iis/apache)。如果访问量比较大的情况下。需要用到nginx负载均衡的架构。
转发给后端有两种情况：(php页面的情况)
**(1)反向代理(2)fastcgi1、反向代理配置**
**示例1:**
反向代理的后端是web服务器，fastcgi是实现php的解析程序。它们的配置代码如下：
server {    
   listen       80;   
   server_name  koumm.com [www.koumm.com](http://www.koumm.com/);   
   access_log   logs/domain2.access.log  main;   
   #静态文件，nginx自己处理   
   location ~ ^/(images|javascript|js|css|flash|media|static)/  {   
       root    /var/www/virtual/big.server.com/htdocs;   
       expires 30d; 
   }   

   #把请求转发给后台web服务器 
   location / {   
       #这里也可以由内网段其它主机提供，也可以由本地服务提供
proxy_pass      [http://127.0.0.1:8080](http://127.0.0.1:8080/);   
include /usr/local/nginx/conf/proxy.conf; /*需要包含支持*/
   }   
}  

**示例2:**
反向代理负载均衡的应用架构
#upstream的负载均衡，weight是权重，可以根据机器配置定义权重。据说nginx可以根据后台响应时间调整。后台需要多个web服务器。

相关配置文件如下：
upstream www_koumm_com {   
   server 127.0.0.1:8000 down;
   server 127.0.0.1:7070 backup;
   server 192.168.129.22:8000 weight=5;   
   server 192.168.129.23:8000;
}   

server {   
   listen          80;   
   server_name     [www.koumm.com](http://www.koumm.com/);   
   access_log      logs/www.koumm.access.log main;   
   location / {   
       proxy_pass  [http://www_koumm_com](http://www_koumm_com/);   
   }   
} 

nginx的upstream目前支持4种方式的分配
1)、轮询（默认）
每个请求按时间顺序逐一分配到不同的后端服务器，如果后端服务器down掉，能自动剔除。
2)、weight
指定轮询几率，weight和访问比率成正比，用于后端服务器性能不均的情况。
2)、ip_hash
每个请求按访问ip的hash结果分配，这样每个访客固定访问一个后端服务器，可以解决session的问题。
3)、fair（第三方）
按后端服务器的响应时间来分配请求，响应时间短的优先

每个设备的状态设置为:
(1)down 表示单前的server暂时不参与负载
(2)weight 默认为1.weight越大，负载的权重就越大。
(3)max_fails ：允许请求失败的次数默认为1.当超过最大次数时，返回proxy_next_upstream 模块定义的错误
(4)fail_timeout:max_fails次失败后，暂停的时间。
(5)backup：其它所有的非backup机器down或者忙的时候，请求backup机器。所以这台机器压力会最轻。nginx支持同时设置多组的负载均衡，用来给不用的server来使用。

**2、fastcgi支持示例1:分布式fastcgi的架构的应用**
![Nginx+FastCGI配置 - koumm - koumm的博客](http://img.bimg.126.net/photo/X0vpQw4XDqFX2JwOxEvjdQ==/3417106217268651344.jpg)
注：配置文件中有相关默认示例，只需要打开相关的注释就可以实现php页面的解析了。以下示例只是应用于nginx与fastcgi安装在同一台电脑的情况，
location ~ \.php$ {
   root           html;
   fastcgi_pass   127.0.0.1:9000;
   fastcgi_index  index.php;
   #fastcgi_param  SCRIPT_FILENAME  /scripts$fastcgi_script_name;
   include        fcgi.conf;
}
如果fastcgi安装在其它主机上实现有是分布是的架构，如上图，对于php页面的处理需要修改，一共需要修改几个地方：
**(1)修改nginx.conf文件如下：**
location ~ \.php$ {
   root           html;
   fastcgi_pass   192.168.129.22:9000;
   fastcgi_index  index.php;
   fastcgi_param  SCRIPT_FILENAME  /opt/www/$fastcgi_script_name;
   include        fcgi.conf;
}
两处说明：一个是访问nginx，nginx把请求转发到fastcgi处理，但是php页面又是在fastcgi那台机器上的/opt/www目录中，所以需要修改以上两个地方才能实现php页面的解析。
**(2)修改/usr/local/php/etc/php-fpm.conf文件**
注：在安装fastcgi主机上：
<value name="listen_address">192.168.129.22:9000</value> 
<value name="allowed_clients">192.168.129.21</value>
说明：
第一处监听本机的9000端口，默认是127.0.0.1,由于是分布式应用，需要修改。
第二处只允许nginx发来的请求给予解析，也需要修改。

**1.5.8 nginx防盗链**
一般的防盗链如下：
location ~* .(gif|jpg|png|swf|flv)$ {
valid_referers none blocked server_names *.koumm.com [www.koumm.com](http://www.koumm.com/);
if ($invalid_referer) {
rewrite ^/ [http://www.koumm.com/403.html](http://www.koumm.com/403.html);
#return 404;
}
}

**1.5.9 nginx虚拟目录**
Nginx的目录如果是在网站根下建立的话，就可以直接当成目录访问，实现虚拟目录的效果，如果另外指一个目录建立网站的虚拟目录的话，需要用到以下方法来实现。
nginx虚拟目录配置有两种方法alias与root。虚拟目录的概念与其它的Web应用一样。
最基本的区别：alias指定的目录是准确的，root是指定目录的上级目录，并且该上级目录要含有location指定名称的同名目录。另外，根据前文所述，使用alias标签的目录块中不能使用rewrite的break。
示例1:
location /abc/ {
alias /home/html/abc/;  
}
在这段配置下，[http://test/abc/a.html](http://test/abc/a.html)就指定的是/home/html/abc/a.html。

示例2:
这段配置亦可改成
location /abc/ {
root /home/html/;   /*是abc的上级目录*/
}

示例3:
把alias的配置改成：
location /abc/ {
alias /home/html/def/;
}
那么nginx将会从/home/html/def/取数据，这段配置还不能直接使用root配置，如果非要配置，只有在/home/html/下建立一个 def->abc的软link（快捷方式）了。
说明：
**1、使用alias时目录名后面一定要加 /2、nginx、resin当虚拟目录名与真实目录同名时，虚拟目录比真实目录优先级要高。**

**1.5.10 nginx虚拟主机**
虚拟主机通常有三种情况：
一、多ip地址
二、单ip多端口实现
三、单ip一个端口多主机头(或者是多域名实现)
在这里只是实现后两种虚拟主机的配置方法：
方法一：
1.修改配置文件
vi /usr/local/nginx/conf/nginx.conf
    #注释原有server内容，加入以下内容，相关内容省略，如日志出错提示等。
server {
    listen          99;
    server_name     localhost;
    location / {
    index index.html;
    root  /usr/local/www1/htdocs;
    }
}
server {
    listen          88;
    server_name     localhost;
    location / {
    index index.html;
    root  /usr/local/www2/htdocs;
    }
}

**2．其它准备工作**
mkdir  -p  /usr/local/www1/htdocs
cd  /usr/local/www1/htdocs
echo "web server 1" >index.html

mkdir -p /usr/local/www2/htdocs
cd  /usr/local/www2/htdocs
echo  "web server 2">index.html

3．重启nginx服务
(1)kill -HUP `cat /usr/local/nginx/logs/nginx.pid`
(2)如果不行的话，先杀，再启动

4．测试
[http://192.168.129.21:88](http://192.168.129.21:88/)
[http://192.168.129.21:99](http://192.168.129.21:99/)

方法二：
方法二采用的是比较常用的虚拟主机配置，如单IP，80端口，通过域名来实现区分不同的网站主页。在方法一的基础上修改为以下配置。
注：需要有dns的支持，有域名，在下面的配置中，只是实现了静态页面实现，如果要实现动态的功能，参考相头内容。
server {
    listen          80;
    server_name     [www.koumm.com](http://www.koumm.com/);
    access_log      logs/koumm.access.log main;
    location / {
    index index.html;
    root  /var/www/koumm.com/htdocs;
    }
}
server {
    listen          80;
    server_name     [www.abc.com](http://www.abc.com/);
    access_log      logs/abc.access.log main;
    location / {
    index index.html;
    root  /var/www/abc.com/htdocs;
    }
}









