
# 某公司平台部署 linx平台完整过程 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月18日 17:35:19[Jlins](https://me.csdn.net/dyllove98)阅读数：2697


# 部署网站平台
## 安装MySQL
### 编译安装MySQL-5.1
\# ./configure --prefix=/usr/local/mysql--with-charset=utf8 --enable-assembler --with-extra-charsets=all--with-plugins=all
|1
|2
|3
|4
|5
|6
|7
|8
|9
|\# make -j8        //多核心编译
|\# make install
|\# cp support-files/mysql.server/etc/init.d/mysqld            //为mysql提供SysV启动脚本；
|\# chmod +x /etc/init.d/mysqld
|\# chkconfig --add mysqld
|\# useradd –r mysql
|\# chown -R mysql /usr/local/mysql
|\# ./mysql_install_db --user=mysql                             //初始化；
|\# cat /etc/my.cnf
|

|1
|\# vim /etc/my.cnf
|


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
|40
|41
|42
|43
|[client]
|port              = 10861
|socket          = /tmp/mysql.sock
|[mysqld]
|port              = 10861
|socket          = /tmp/mysql.sock
|\#skip-locking
|key_buffer = 384M
|max_allowed_packet = 1M
|table_cache = 512
|sort_buffer_size = 2M
|read_buffer_size = 2M
|read_rnd_buffer_size =8M
|myisam_sort_buffer_size= 64M
|thread_cache_size = 8
|query_cache_size = 32M
|thread_concurrency = 8
|max_connect_errors =2000
|max_user_connections =2000
|max_error_count = 2000
|wait_timeout    = 300
|max_connections = 2000
|skip-federated
|log-bin=mysql-bin
|server-id       = 1
|[mysqldump]
|quick
|max_allowed_packet = 16M
|[mysql]
|no-auto-rehash
|\#safe-updates
|[isamchk]
|key_buffer = 256M
|sort_buffer_size = 256M
|read_buffer = 2M
|write_buffer = 2M
|[myisamchk]
|key_buffer = 256M
|sort_buffer_size = 256M
|read_buffer = 2M
|write_buffer = 2M
|[mysqlhotcopy]
|interactive-timeout
|


### 安装MySQL5.6
|1
|2
|3
|4
|5
|6
|7
|8
|9
|\# tar xf mysql-5.6.10-linux-glibc2.5-x86_64.tar.gz-C /usr/local/
|\# ln -s/usr/local/mysql-5.6.10-linux-glibc2.5-x86_64/ /usr/local/mysql
|\# useradd -r mysql
|\# cd /usr/local/mysql
|\# chown -R mysql .
|\# mkdir /data/db
|\# chown -R mysql.mysql /data/db/
|\# scripts/mysql_install_db --user=mysql--datadir=/data/db/             //初始化；
|\# grep-E -v "^( )*\#|^$" /usr/local/mysql/my.cnf
|


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
|[mysqld]
|server_id           = 1
|port                    = 3306
|socket                = /tmp/mysql.sock
|datadir                     = /data/db
|skip-name-resolve
|max_user_connections   = 2000
|max_connections      = 2000
|log-bin               = mysql-bin
|expire_logs_days  = 7
|binlog_format        = mixed
|sql_mode=NO_ENGINE_SUBSTITUTION,STRICT_TRANS_TABLES
|

|1
|2
|3
|\# cp support-files/mysql.server/etc/init.d/mysqld
|\# chmod +x /etc/init.d/mysqld
|\# chkconfig --add mysqld
|



### 安装Apache-2.4.6
#### 安装apr-1.4.6
|1
|2
|3
|4
|\# tar xf apr-1.4.6.tar.gz
|\# cd apr-1.4.6
|\# ./configure --prefix=/usr/local/apr
|\# make && make install
|


#### 安装apr-util-1.4.1
|1
|2
|3
|\# tar xf apr-util-1.4.1.tar.gz
|\# ./configure --prefix=/usr/local/apr-util--with-apr=/usr/local/apr/
|\# make && make install
|


#### 安装pcre-8.32
|1
|2
|3
|4
|\# tar xf pcre-8.32.tar.gz
|\# cd pcre-8.32
|\# ./configure --prefix=/usr/local/pcre
|\# make && make install
|


#### 安装httpd-2.4.6
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
|\# tar xf httpd-2.4.6.tar.gz
|\# cd httpd-2.4.6
|\# ./configure \
|--prefix=|/usr/local/apache|\
|--sysconfdir=|/etc/httpd/|\
|--|enable|-so \
|--|enable|-ssl \
|--|enable|-rewrite \
|--with-zlib \
|--with-pcre=|/usr/local/pcre/|\
|--with-apr=|/usr/local/apr|\
|--with-apr-util=|/usr/local/apr-util/|\
|--|enable|-modules=most \
|--|enable|-mpms-shared=all \
|--with-mpm=event
|\# cp build/rpm/httpd.init /etc/init.d/httpd
|\# chmod +x /etc/init.d/httpd
|\# vim /etc.init.d/httpd
|


|1
|2
|3
|CONFFILE=/etc/httpd/httpd.conf
|httpd=${HTTPD-/usr/local/apache/bin/httpd}
|pidfile=${PIDFILE-/usr/local/apache/logs/${prog}.pid}
|


|1
|2
|\# service httpd start
|Starting httpd:                                           [  OK  ]
|


### 安装Nginx-1.4.7
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
|\# tar xf nginx-1.4.7.tar.gz
|\# ./configure \
|--prefix=|/usr|\
|--sbin-path=|/usr/sbin/nginx|\
|--conf-path=|/etc/nginx/nginx|.conf\
|--error-log-path=|/var/log/nginx/error|.log\
|--http-log-path=|/var/log/nginx/access|.log\
|--pid-path=|/var/run/nginx/nginx|.pid\
|--lock-path=|/var/lock/nginx|.lock\
|--user=www\
|--group=www\
|--with-http_ssl_module\
|--with-http_flv_module\
|--with-http_stub_status_module\
|--with-http_gzip_static_module\
|--http-client-body-temp-path=|/var/tmp/nginx/client/|\
|--http-proxy-temp-path=|/var/tmp/nginx/proxy/|\
|--http-fastcgi-temp-path=|/var/tmp/nginx/fcgi/|\
|--with-pcre
|\# mkdir –p /var/tmp/nginx/{client,proxy,fcgi}
|\# chown –R www.www/var/tmp/nginx
|\# make && make install
|\# vim /etc/init.d/nginx
|


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
|40
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|52
|53
|54
|55
|56
|57
|58
|59
|60
|61
|62
|63
|64
|65
|66
|67
|68
|69
|70
|71
|72
|73
|74
|75
|76
|77
|78
|79
|80
|81
|82
|83
|84
|85
|86
|87
|88
|89
|90
|91
|92
|93
|94
|95
|96
|97
|98
|99
|100
|101
|102
|103
|104
|105
|106
|107
|\#!/bin/sh
|\# nginx- this script starts and stops the nginx daemon
|\#chkconfig:   - 85 15
|\#description:  Nginx is an HTTP(S) server,HTTP(S) reverse \
|\#               proxy and IMAP/POP3 proxy server
|\#processname: nginx
|\#config:      /etc/nginx/nginx.conf
|\#config:      /etc/sysconfig/nginx
|\#pidfile:     /var/run/nginx.pid
|\#Source function library.
|./etc/rc.d/init.d/functions
|\#Source networking configuration.
|./etc/sysconfig/network
|\# Checkthat networking is up.
|["$NETWORKING" = "no" ] && exit 0
|nginx="/usr/sbin/nginx"
|prog=$(basename$nginx)
|NGINX_CONF_FILE="/etc/nginx/nginx.conf"
|[ -f/etc/sysconfig/nginx ] && . /etc/sysconfig/nginx
|lockfile=/var/lock/subsys/nginx
|make_dirs(){
|\# make required directories
|user=`nginx-V 2>&1 | grep "configure arguments:" | sed's/[^*]*--user=\([^ ]*\).*/\1/g' -`
|options=`$nginx-V 2>&1 | grep 'configure arguments:'`
|foropt in $options; do
|if[ `echo $opt | grep '.*-temp-path'` ]; then
|value=`echo$opt | cut -d "=" -f 2`
|if[ ! -d "$value" ]; then
|\#echo "creating" $value
|mkdir-p $value && chown -R $user $value
|fi
|fi
|done
|}
|start(){
|[-x $nginx ] || exit 5
|[-f $NGINX_CONF_FILE ] || exit 6
|make_dirs
|echo-n $"Starting $prog: "
|daemon$nginx -c $NGINX_CONF_FILE
|retval=$?
|echo
|[$retval -eq 0 ] && touch $lockfile
|return$retval
|}
|stop(){
|echo-n $"Stopping $prog: "
|killproc$prog -QUIT
|retval=$?
|echo
|[$retval -eq 0 ] && rm -f $lockfile
|return$retval
|}
|restart(){
|configtest|| return $?
|stop
|sleep1
|start
|}
|reload(){
|configtest|| return $?
|echo-n $"Reloading $prog: "
|killproc$nginx -HUP
|RETVAL=$?
|echo
|}
|force_reload(){
|restart
|}
|configtest(){
|$nginx-t -c $NGINX_CONF_FILE
|}
|rh_status(){
|status$prog
|}
|rh_status_q(){
|rh_status>/dev/null 2>&1
|}
|case"$1" in
|start)
|rh_status_q&& exit 0
|$1
|;;
|stop)
|rh_status_q|| exit 0
|$1
|;;
|restart|configtest)
|$1
|;;
|reload)
|rh_status_q|| exit 7
|$1
|;;
|force-reload)
|force_reload
|;;
|status)
|rh_status
|;;
|condrestart|try-restart)
|rh_status_q|| exit 0
|;;
|*)
|echo$"Usage: $0{start|stop|status|restart|condrestart|try-restart|reload|force-reload|configtest}"
|exit 2
|esac
|


|1
|2
|\# chmod +x /etc/init.d/nginx
|\# chkconfig --add nginx
|


### 安装PHP
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
|\# ./configure \
|--prefix=|/usr/local/php|\
|--with-openssl \
|--|enable|-mbstring \
|--with-freetype-|dir|\
|--with-jpeg-|dir|\
|--with-png-|dir|\
|--with-zlib \
|--with-libxml-|dir|=|/usr|\
|--|enable|-xml \
|--|enable|-sockets \
|--with-mcrypt \
|--with-config-|file|-path=|/etc|\
|--with-config-|file|-scan-|dir|=|/etc/php|.d \
|--with-bz2 \
|--|enable|-maintainer-zts--with-mysql=mysqlnd \
|--with-pdo-mysql=mysqlnd \
|--with-mysqli=mysqlnd \
|--|enable|-fpm \
|--with-gd
|


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
|\# make && make install
|\# cp php.ini-production /etc/php.ini
|\# cp sapi/fpm/init.d.php-fpm /etc/init.d/php-fpm
|\# chmod +x /etc/init.d/php-fpm
|\# chkconfig --add php-fpm
|\# cp /usr/local/php/etc/php-fpm.conf.default/usr/local/php/etc/php-fpm.conf
|\# vim /usr/local/php/etc/php-fpm.conf
|\# service php-fpm start
|Starting php-fpm|done
|\# tar xf xcache-3.0.4.tar.gz
|


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
|\# ./configure --enable-xcache--enable-xcache-coverager --enable-xcache-optimizer--with-php-config=/usr/local/php/bin/php-config
|\# 注意：
|--|enable|-xcache|\# 启用xcache
|--|enable|-xcache-vocerager|\# 附加特性
|--|enable|-xcache-optimizer|\# 启用操作码优化
|\# make && make install
|\# tar zxvf ImageMagick.tar.gz
|\# cd ImageMagick-6.5.1-2/
|\# make clean
|\# ./configure
|\# make && make install
|


### 整合Apache与PHP
|1
|\# vim /etc/httpd/httpd.conf
|


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
|40
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|52
|53
|54
|55
|56
|57
|58
|59
|60
|61
|62
|63
|64
|65
|66
|67
|68
|69
|70
|71
|72
|73
|74
|75
|76
|77
|78
|79
|80
|81
|82
|83
|84
|85
|86
|87
|88
|89
|90
|91
|92
|93
|94
|95
|96
|97
|98
|99
|100
|101
|102
|103
|104
|105
|106
|107
|108
|109
|110
|111
|112
|113
|114
|115
|116
|117
|118
|119
|120
|121
|122
|123
|124
|125
|126
|127
|128
|ServerRoot"/usr/local/apache"
|Listen80
|LoadModuleauthn_file_module modules/mod_authn_file.so
|LoadModuleauthn_core_module modules/mod_authn_core.so
|LoadModuleauthz_host_module modules/mod_authz_host.so
|LoadModuleauthz_groupfile_module modules/mod_authz_groupfile.so
|LoadModuleauthz_user_module modules/mod_authz_user.so
|LoadModuleauthz_core_module modules/mod_authz_core.so
|LoadModuleaccess_compat_module modules/mod_access_compat.so
|LoadModuleauth_basic_module modules/mod_auth_basic.so
|LoadModulereqtimeout_module modules/mod_reqtimeout.so
|LoadModulefilter_module modules/mod_filter.so
|LoadModulemime_module modules/mod_mime.so
|LoadModulelog_config_module modules/mod_log_config.so
|LoadModuleenv_module modules/mod_env.so
|LoadModuleheaders_module modules/mod_headers.so
|LoadModulesetenvif_module modules/mod_setenvif.so
|LoadModuleversion_module modules/mod_version.so
|LoadModule proxy_module modules/mod_proxy.so
|LoadModule proxy_fcgi_module modules/mod_proxy_fcgi.so
|LoadModulempm_event_module modules/mod_mpm_event.so
|LoadModuleunixd_module modules/mod_unixd.so
|LoadModulestatus_module modules/mod_status.so
|LoadModuleautoindex_module modules/mod_autoindex.so
|LoadModuledir_module modules/mod_dir.so
|LoadModulealias_module modules/mod_alias.so
|LoadModule rewrite_module modules/mod_rewrite.so
|<IfModuleunixd_module>
|Userwww
|Groupwww
|</IfModule>
|ServerAdminyou@example.com
|ServerName127.0.0.1
|<Directory/>
|AllowOverride none
|Require all denied
|</Directory>
|<Directory"/usr/local/apache/htdocs">
|Options Indexes FollowSymLinks
|AllowOverride None
|Require all granted
|</Directory>
|<IfModuledir_module>
|DirectoryIndex index.phpindex.html index.htm
|</IfModule>
|<Files".ht*">
|Require all denied
|</Files>
|ErrorLog"logs/error_log"
|LogLevelwarn
|<IfModulelog_config_module>
|LogFormat "%h %l %u %t\"%r\" %>s %b \"%{Referer}i\"\"%{User-Agent}i\"" combined
|LogFormat "%h %l %u %t\"%r\" %>s %b" common
|<IfModule logio_module>
|LogFormat "%h %l %u %t\"%r\" %>s %b \"%{Referer}i\"\"%{User-Agent}i\" %I %O" combinedio
|</IfModule>
|CustomLog "logs/access_log"common
|</IfModule>
|<IfModulealias_module>
|ScriptAlias /cgi-bin/"/usr/local/apache/cgi-bin/"
|</IfModule>
|<IfModulecgid_module>
|</IfModule>
|<Directory"/usr/local/apache/cgi-bin">
|AllowOverride None
|Options None
|Require all granted
|</Directory>
|<IfModulemime_module>
|TypesConfig /etc/httpd/mime.types
|AddType application/x-compress .Z
|AddType application/x-gzip .gz .tgz
|AddTypeapplication/x-httpd-php .php
|AddType application/x-httpd-php-source.phps
|</IfModule>
|Include /etc/httpd/extra/httpd-vhosts.conf
|<IfModuleproxy_html_module>
|Include/etc/httpd/extra/proxy-html.conf
|</IfModule>
|<IfModulessl_module>
|SSLRandomSeedstartup builtin
|SSLRandomSeedconnect builtin
|</IfModule>
|\# vim /etc/httpd/extra/httpd-vhost.conf
|<VirtualHost*:80>
|DocumentRoot /data/htdocs/xxx_bbs
|ServerName bbs.xxx.com
|ErrorLog"/data/logs/apache/bbs.xxx.com.error"
|SetEnvIf Request_URI "^/ok\.htm$"dontlog
|<Directory />
|AllowOverride FileInfo
|AllowOverride All
|Options FollowSymlinks
|Require all granted
|</Directory>
|ProxyRequests off
|ProxyPassMatch ^/(.*)\.php$fcgi://127.0.0.1:9000/data/htdocs/xxx/$1.php
|</VirtualHost>
|\# vim /data/htdocs/xxx_bbs/.htaccess
|RewriteEngineOn
|RewriteBase/
|RewriteCond%{QUERY_STRING} ^(.*)$
|RewriteRule^topic-(.+)\.html$ portal.php?mod=topic&topic=$1&%1
|RewriteCond%{QUERY_STRING} ^(.*)$
|RewriteRule^article-([0-9]+)-([0-9]+)\.html$portal.php?mod=view&aid=$1&page=$2&%1
|RewriteCond%{QUERY_STRING} ^(.*)$
|RewriteRule^forum-(\w+)-([0-9]+)\.html$forum.php?mod=forumdisplay&fid=$1&page=$2&%1
|RewriteCond%{QUERY_STRING} ^(.*)$
|RewriteRule^thread-([0-9]+)-([0-9]+)-([0-9]+)\.html$forum.php?mod=viewthread&tid=$1&extra=page\%3D$3&page=$2&%1
|RewriteCond%{QUERY_STRING} ^(.*)$
|RewriteRule^group-([0-9]+)-([0-9]+)\.html$forum.php?mod=group&fid=$1&page=$2&%1
|RewriteCond%{QUERY_STRING} ^(.*)$
|RewriteRule^space-(username|uid)-(.+)\.html$ home.php?mod=space&$1=$2&%1
|RewriteCond%{QUERY_STRING} ^(.*)$
|RewriteRule^blog-([0-9]+)-([0-9]+)\.html$home.php?mod=space&uid=$1&do=blog&id=$2&%1
|RewriteCond%{QUERY_STRING} ^(.*)$
|RewriteRule^archiver/(fid|tid)-([0-9]+)\.html$ archiver/index.php?action=$1&value=$2&%1
|RewriteCond%{QUERY_STRING} ^(.*)$
|RewriteRule^([a-z]+[a-z0-9_]*)-([a-z0-9_\-]+)\.html$ plugin.php?id=$1:$2&%1
|RewriteCond%{http_host} ^xxx.com [NC]
|RewriteRule^(.*)$  [L,R=301]
|ErrorDocument404 /404.shtml
|RewriteCond%{HTTP_REFERER} !^$ [NC]
|RewriteCond%{HTTP_REFERER} !baidu.com [NC]
|RewriteCond%{HTTP_REFERER} !google.com [NC]
|RewriteCond%{HTTP_REFERER} !google.com.hk [NC]
|RewriteCond%{HTTP_REFERER} !xxx.com [NC]
|RewriteRule.*\.(gif|jpg)$  [R,NC,L]
|


### 整合Nginx与PHPgame
\# vim /etc/nginx/nginx.conf
worker_processes4;
events{
worker_connections65535;
}
http {
includemime.types;
default_typeapplication/octet-stream;
\#log_formatmain'$remote_addr
 - $remote_user[$time_local] "$request" '
\#'$status
 $body_bytes_sent"$http_referer" '
\#'"$http_user_agent""$http_x_forwarded_for"';
server_names_hash_bucket_size
 128;
client_header_buffer_size
 32k;
large_client_header_buffers
 4 32k;
client_max_body_size
 8m;
sendfile
 on;
tcp_nopushon;
keepalive_timeout
 60;
tcp_nodelay
 on;
fastcgi_connect_timeout
 300;
fastcgi_send_timeout
 300;
fastcgi_read_timeout
 300;
fastcgi_buffer_size
 64k;
fastcgi_buffers
 4 64k;
fastcgi_busy_buffers_size
 128k;
fastcgi_temp_file_write_size
 128k;
gzip
 on;
gzip_min_length1k;
gzip_buffers4
 16k;
gzip_http_version
 1.0;
gzip_comp_level
 2;
gzip_typestext/plain
 application/x-javascripttext/css application/xml;
gzip_vary
 on;
server {
listen       80;
server_name  bbs.xxx.com;
\#charset koi8-r;
\#access_log  logs/host.access.log  main;
location / {
root   /data/htdocs/xxx_bbs;
index  index.html index.htm;
location ~* \.php$ {
root             /data/htdocs/xxx_bbs;
fastcgi_pass     127.0.0.1:9000;
include              fastcgi_params;
}
include /etc/nginx/rewrite.conf;
}
error_page500
 502 503 504/50x.html;
location
 = /50x.html {
roothtml;
}
}
}
|1
|\# vim /etc/nginx/fastcgi_params
|


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
|fastcgi_param  GATEWAY_INTERFACE  CGI/1.1;
|fastcgi_param  SERVER_SOFTWARE    nginx;
|fastcgi_param  QUERY_STRING       $query_string;
|fastcgi_param  REQUEST_METHOD     $request_method;
|fastcgi_param  CONTENT_TYPE       $content_type;
|fastcgi_param  CONTENT_LENGTH     $content_length;
|fastcgi_param  SCRIPT_FILENAME    $document_root$fastcgi_script_name;
|fastcgi_param  SCRIPT_NAME        $fastcgi_script_name;
|fastcgi_param  REQUEST_URI        $request_uri;
|fastcgi_param  DOCUMENT_URI       $document_uri;
|fastcgi_param  DOCUMENT_ROOT      $document_root;
|fastcgi_param  SERVER_PROTOCOL    $server_protocol;
|fastcgi_param  REMOTE_ADDR        $remote_addr;
|fastcgi_param  REMOTE_PORT        $remote_port;
|fastcgi_param  SERVER_ADDR        $server_addr;
|fastcgi_param  SERVER_PORT        $server_port;
|fastcgi_param SERVER_NAME        $server_name;
|


|1
|\# vim /etc/nginx/rewrite.conf
|


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
|rewrite^([^\.]*)/topic-(.+)\.html$ $1/portal.php?mod=topic&topic=$2 last;
|rewrite^([^\.]*)/article-([0-9]+)-([0-9]+)\.html
$$
1/portal.php?mod=view&aid=$2&page=$3 last;
|rewrite^([^\.]*)/forum-(\w+)-([0-9]+)\.html
$$
1/forum.php?mod=forumdisplay&fid=$2&page=$3 last;
|rewrite^([^\.]*)/thread-([0-9]+)-([0-9]+)-([0-9]+)\.html
$$
1/forum.php?mod=viewthread&tid=$2&extra=page%3D$4&page=$3 last;
|rewrite^([^\.]*)/group-([0-9]+)-([0-9]+)\.html
$$
1/forum.php?mod=group&fid=$2&page=$3 last;
|rewrite^([^\.]*)/space-(username|uid)-(.+)\.html$ $1/home.php?mod=space&$2=$3last;
|rewrite^([^\.]*)/blog-([0-9]+)-([0-9]+)\.html
$$
1/home.php?mod=space&uid=$2&do=blog&id=$3 last;
|rewrite^([^\.]*)/(fid|tid)-([0-9]+)\.html$ $1/index.php?action=$2&value=$3 last;
|rewrite^([^\.]*)/([a-z]+[a-z0-9_]*)-([a-z0-9_\-]+)\.html$ $1/plugin.php?id=$2:$3 last;
|if (!-e$request_filename) {
|return 404;
|}
|


![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

