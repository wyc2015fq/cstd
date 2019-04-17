# NGINX 添加MP4、FLV视频支持模块 - DoubleLi - 博客园






由于公司网站需要放置视频，但是默认的服务器环境是没有编译这个支持的模块，视频文件只能缓冲完了在播放，非常麻烦。



之前呢也安装了一个nginx_mod_h264_streaming来支持，效果很不错，但是服务器最近系统更新后出了点问题，只好从新编译，今天在nginx官网看到了nginx在新版本中已经支持了--with-http_mp4_module --with-http_flv_module这2个模块。所以就来自己编译吧。



编译之前我们需要看看之前编译的参数，使用nginx -V查看得到如下参数



configure arguments: --user=www --group=www --prefix=/usr/local/nginx --with-http_stub_status_module --with-http_ssl_module --with-http_gzip_static_module



接下来就进入之前安装的目录，我用的是lnmp1.0的脚本安装的，文件地址在/root/lnmp1.0-full



当然我的nginx是我后来更新过的，版本是1.4.3，再次进入nginx-1.4.3这个目录



备份之前的程序配置





mv /usr/local/nginx/sbin/nginx /usr/local/nginx/sbin/nginx.old



./configure --user=www --group=www --prefix=/usr/local/nginx --with-http_stub_status_module --with-http_ssl_module --with-http_gzip_static_module --with-http_mp4_module --with-http_flv_module

等待结束，查看是否报错（应该不会报错，因为用的都是nginx默认支持的模块，不是外挂的）正常文件如



Configuration summary

+ using system PCRE library

+ using system OpenSSL library

+ md5: using OpenSSL library

+ sha1: using OpenSSL library

+ using system zlib library



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

nginx http uwsgi temporary files: "uwsgi_temp"

nginx http scgi temporary files: "scgi_temp"

编译





make

nginx -t

如果提示如下命令就证明没有问题，就可以升级了





nginx: the configuration file /usr/local/nginx/conf/nginx.conf syntax is ok

nginx: configuration file /usr/local/nginx/conf/nginx.conf test is successful



make install



nginx version: nginx/1.4.3

built by gcc 4.4.7 20120313 (Red Hat 4.4.7-3) (GCC)

TLS SNI support enabled

configure arguments: --user=www --group=www --prefix=/usr/local/nginx --with-http_stub_status_module --with-http_ssl_module --with-http_gzip_static_module --with-http_mp4_module --with-http_flv_module









