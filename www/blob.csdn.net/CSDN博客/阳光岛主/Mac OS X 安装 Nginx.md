
# Mac OS X 安装 Nginx - 阳光岛主 - CSDN博客

2017年04月29日 16:40:55[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：5660



**brew 安装 Nginx**
1、brew search nginx
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
|$ brew search nginx
|nginx
|homebrew|/nginx/accept-language-nginx-module|homebrew|/nginx/mogilefs-nginx-module
|homebrew|/nginx/accesskey-nginx-module|homebrew|/nginx/mp4-h264-nginx-module
|homebrew|/nginx/ajp-nginx-module|homebrew|/nginx/mruby-nginx-module
|homebrew|/nginx/anti-ddos-nginx-module|homebrew|/nginx/naxsi-nginx-module
|homebrew|/nginx/array-var-nginx-module|homebrew|/nginx/nchan-nginx-module
|homebrew|/nginx/auth-digest-nginx-module|homebrew|/nginx/nginx-full
|homebrew|/nginx/auth-ldap-nginx-module|homebrew|/nginx/notice-nginx-module
|homebrew|/nginx/auth-pam-nginx-module|homebrew|/nginx/php-session-nginx-module
|homebrew|/nginx/auto-keepalive-nginx-module|homebrew|/nginx/push-stream-nginx-module
|homebrew|/nginx/autols-nginx-module|homebrew|/nginx/realtime-req-nginx-module
|homebrew|/nginx/cache-purge-nginx-module|homebrew|/nginx/redis-nginx-module
|homebrew|/nginx/captcha-nginx-module|homebrew|/nginx/redis2-nginx-module
|homebrew|/nginx/counter-zone-nginx-module|homebrew|/nginx/rtmp-nginx-module
|homebrew|/nginx/ctpp2-nginx-module|homebrew|/nginx/set-misc-nginx-module
|homebrew|/nginx/dav-ext-nginx-module|homebrew|/nginx/small-light-nginx-module
|homebrew|/nginx/dosdetector-nginx-module|homebrew|/nginx/stream-lua-nginx-module
|homebrew|/nginx/echo-nginx-module|homebrew|/nginx/subs-filter-nginx-module
|homebrew|/nginx/eval-nginx-module|homebrew|/nginx/tcp-proxy-nginx-module
|homebrew|/nginx/extended-status-nginx-module|homebrew|/nginx/txid-nginx-module
|homebrew|/nginx/fancyindex-nginx-module|homebrew|/nginx/unzip-nginx-module
|homebrew|/nginx/geoip2-nginx-module|homebrew|/nginx/upload-nginx-module
|homebrew|/nginx/headers-more-nginx-module|homebrew|/nginx/upload-progress-nginx-module
|homebrew|/nginx/healthcheck-nginx-module|homebrew|/nginx/upstream-order-nginx-module
|homebrew|/nginx/http-accounting-nginx-module|homebrew|/nginx/ustats-nginx-module
|homebrew|/nginx/http-flood-detector-nginx-module|homebrew|/nginx/var-req-speed-nginx-module
|homebrew|/nginx/http-remote-passwd-nginx-module|homebrew|/nginx/vod-nginx-module
|homebrew|/nginx/log-if-nginx-module|homebrew|/nginx/websockify-nginx-module
|homebrew|/nginx/lua-nginx-module|homebrew|/nginx/xsltproc-nginx-module
|homebrew|/nginx/mod-zip-nginx-module
|

2、brew install nginx
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
|$ brew|install|nginx
|Updating Homebrew...
|Auto-updated Homebrew!
|Updated 1 tap (homebrew|/core|).
|New Formulae
|qmmp
|Updated Formulae
|vaulted
|Error: Could not link:
|/usr/local/share/doc/homebrew
|Please delete these paths and run `brew update`.
|Installing dependencies|for|nginx: pcre, openssl@1.1
|Installing nginx dependency: pcre
|Downloading https:|//homebrew|.bintray.com|/bottles/pcre-8|.40.sierra.bottle.|tar|.gz
|\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\# 100.0%
|Pouring pcre-8.40.sierra.bottle.|tar|.gz
|/usr/local/Cellar/pcre/8|.40: 204 files, 5.4MB
|Installing nginx dependency: openssl@1.1
|Downloading https:|//homebrew|.bintray.com|/bottles/openssl|@1.1-1.1.0e.sierra.bottle.|tar|.gz
|\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\# 100.0%
|Pouring openssl@1.1-1.1.0e.sierra.bottle.|tar|.gz
|Using the sandbox
|Caveats
|A CA|file|has been bootstrapped using certificates from the system
|keychain. To add additional certificates, place .pem files|in
|/usr/local/etc/openssl|@1.1|/certs
|and run
|/usr/local/opt/openssl|@1.1|/bin/c_rehash
|This formula is keg-only,|which|means it was not symlinked into|/usr/local|.
|This is an alternate version of another formula.
|If you need to have this software first|in|your PATH run:
|echo|'export PATH="/usr/local/opt/openssl@1.1/bin:$PATH"'|>> ~/.bash_profile
|For compilers to|find|this software you may need to|set|:
|LDFLAGS:  -L|/usr/local/opt/openssl|@1.1|/lib
|CPPFLAGS: -I|/usr/local/opt/openssl|@1.1|/include
|Summary
|/usr/local/Cellar/openssl|@1.1|/1|.1.0e: 6,303 files, 15.4MB
|Installing nginx
|Downloading https:|//homebrew|.bintray.com|/bottles/nginx-1|.12.0.sierra.bottle.|tar|.gz
|\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\# 100.0%
|Pouring nginx-1.12.0.sierra.bottle.|tar|.gz
|Caveats
|Docroot is:|/usr/local/var/www
|The default port has been|set|in|/usr/local/etc/nginx/nginx|.conf to 8080 so that
|nginx can run without|sudo|.
|nginx will load all files|in|/usr/local/etc/nginx/servers/|.
|To have launchd start nginx now and restart at login:
|brew services start nginx
|Or,|if|you don't want|/need|a background service you can just run:
|nginx
|Summary
|/usr/local/Cellar/nginx/1|.12.0: 8 files, 1002.3KB
|

**启动 Nginx**
sudo nginx
**访问 Nginx**
[http://localhost:8080](http://localhost:8080/)
![mac-os-x-install-nginx-01](http://blog.mimvp.com/wp-content/uploads/2017/11/mac-os-x-install-nginx-01.png)
出现nginx的欢迎页面了，nginx安装成功！
**命令软链接**
|1
|2
|3
|4
|5
|$|which|nginx
|/usr/local/bin/nginx
|\# /usr/bin/nginx 提示没权限，直接用 /usr/local/bin/nginx
|$|ln|-s|/usr/local/sbin/nginx|/usr/local/bin/nginx
|

**Nginx 常用指令**
nginx -V 查看版本，以及配置文件地址
nginx -v 查看版本
nginx -c filename 指定配置文件
nginx -h 帮助
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
|$ nginx -V
|nginx version: nginx|/1|.12.0
|built by clang 8.1.0 (clang-802.0.42)
|built with OpenSSL 1.1.0e  16 Feb 2017
|TLS SNI support enabled
|configure arguments: --prefix=|/usr/local/Cellar/nginx/1|.12.0 --with-http_ssl_module --with-pcre --sbin-path=|/usr/local/Cellar/nginx/1|.12.0|/bin/nginx|--with-cc-opt=|'-I/usr/local/opt/pcre/include -I/usr/local/opt/openssl@1.1/include'|--with-ld-opt=|'-L/usr/local/opt/pcre/lib -L/usr/local/opt/openssl@1.1/lib'|--conf-path=|/usr/local/etc/nginx/nginx|.conf --pid-path=|/usr/local/var/run/nginx|.pid --lock-path=|/usr/local/var/run/nginx|.lock --http-client-body-temp-path=|/usr/local/var/run/nginx/client_body_temp|--http-proxy-temp-path=|/usr/local/var/run/nginx/proxy_temp|--http-fastcgi-temp-path=|/usr/local/var/run/nginx/fastcgi_temp|--http-uwsgi-temp-path=|/usr/local/var/run/nginx/uwsgi_temp|--http-scgi-temp-path=|/usr/local/var/run/nginx/scgi_temp|--http-log-path=|/usr/local/var/log/nginx/access|.log --error-log-path=|/usr/local/var/log/nginx/error|.log --with-http_gzip_static_module --with-ipv6
|$
|$ nginx -|v
|nginx version: nginx|/1|.12.0
|$
|$ nginx -h
|nginx version: nginx|/1|.12.0
|Usage: nginx [-?hvVtTq] [-s signal] [-c filename] [-p prefix] [-g directives]
|Options:
|-?,-h         : this help
|-|v|: show version and|exit
|-V            : show version and configure options|then|exit
|-t            :|test|configuration and|exit
|-T            :|test|configuration, dump it and|exit
|-q            : suppress non-error messages during configuration testing
|-s signal     : send signal to a master process: stop, quit, reopen, reload
|-p prefix     :|set|prefix path (default:|/usr/local/Cellar/nginx/1|.12.0/)
|-c filename   :|set|configuration|file|(default:|/usr/local/etc/nginx/nginx|.conf)
|-g directives :|set|global directives out of configuration|file
|

**Nginx 重启关闭**
\#重新加载配置|重启|停止|退出 nginx
nginx -s reload|reopen|stop|quit
\#打开 nginx
sudo nginx
nginx 配置文件路径
vim /usr/local/etc/nginx/nginx.conf
\#测试配置是否有语法错误
nginx -t
|1
|2
|3
|$ nginx -t
|nginx: the configuration|file|/usr/local/etc/nginx/nginx|.conf syntax is ok
|nginx: configuration|file|/usr/local/etc/nginx/nginx|.conf|test|is successful
|

**Nginx 隐藏版本号**
nginx 配置文件里增加server_tokens off;
server_tokens作用域是http server location语句块
server_tokens默认值是on，表示显示版本信息，设置server_tokens值是off，就可以在所有地方隐藏nginx的版本信息。
vim /usr/local/etc/nginx/nginx.conf
![mac-os-x-install-nginx-02](http://blog.mimvp.com/wp-content/uploads/2017/11/mac-os-x-install-nginx-02.png)
**隐藏了 Nginx 版本号后，重启Nginx**
nginx -s reload
或
nginx -s stop
nginx
![mac-os-x-install-nginx-03](http://blog.mimvp.com/wp-content/uploads/2017/11/mac-os-x-install-nginx-03.jpg)

**Mac 安装 brew**
**brew 官方网站**：[https://brew.sh](https://brew.sh/)
**brew 安装方法：**
在Mac中打开Termal，输入命令：
`/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
![install-software-mac-brew-01](http://blog.mimvp.com/wp-content/uploads/2017/08/install-software-mac-brew-01.png)
当brew安装成功后，就可以随意安装自己想要的软件了，
例如wget，命令如下：
sudo brew install wget
卸载命令如下：
sudo brew uninstall wget
查看安装软件的话，命令如下：
sudo brew search /apache*/
注意/apache*/是使用的正则表达式，用/分割。



