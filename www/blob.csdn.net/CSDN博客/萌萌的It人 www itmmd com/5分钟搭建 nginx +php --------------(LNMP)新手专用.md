
# 5分钟搭建 nginx +php --------------(LNMP)新手专用 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月14日 16:48:22[Jlins](https://me.csdn.net/dyllove98)阅读数：110869


这里要先声明一下，针对Nginx的php安装和针对apache的php安装是有区别的，因为Nginx中的php是以fastcgi的方式结合nginx的，可以理解为nginx代理了php的fastcgi，而apache是把php作为自己的模块来调用的。
先把php 和php-fpm编译安装吧 请看[http://heilinux.blog.51cto.com/6123663/1575582](http://heilinux.blog.51cto.com/6123663/1575582)
修改配置文件
|1
|cd|php-5.6.2
|

cp php.ini-production /usr/local/php/etc/php.ini
cp /usr/local/php/etc/php-fpm.conf.default.conf  php-fpm.conf保存配置文件后，检验配置是否正确的方法为:
/usr/local/php/sbin/php-fpm -t如果出现诸如 “test is successful” 字样，说明配置没有问题。
启动php-fpm
cp /usr/local/src/php-5.3.27/sapi/fpm/init.d.php-fpm /etc/init.d/php-fpm
chmod 755 /etc/init.d/php-fpm
service php-fpm start如果想让它开机启动，执行:
chkconfig php-fpm on检测是否启动:
ps aux |grep php-fpmnetstat -ant |grep 9000看看是不是有很多个进程（大概20多个）还有你的9000端口开启了么？。
》》》》》》》》》》》》》》》》华华丽丽的分隔符《《《《《《《《《《《《《《《《《《《

## 安装nginx
开始安装nginx吧
|1
|2
|wget
|http:|//nginx|.org|/download/nginx-1|.6.2.|tar|.gz|//|最新稳定版哦
|


解压nginx
tar zxvf nginx-1.6.2.tar.gz配置编译参数
cd nginx-1.6.2
./configure \
--prefix=/usr/local/nginx \
--with-http_realip_module \
--with-http_sub_module \
--with-http_gzip_static_module \
--with-http_stub_status_module  \
--with-pcre编译nginx
make安装nginx
make install因为nginx比较小，所以很快就会安装完，而且也不会出什么错误，如果出错了，yum安装pcre-devel（嘿嘿，我是这个问题了，至于你出别的问题问度娘吧。![j_0058.gif](http://img.baidu.com/hi/jx2/j_0058.gif)）
》》》》》》》》》》》》》》》》》》华丽的分隔符《《《《《《《《《《《《《《《《《《《《《
添加一个nginx主程序的符号链接
|1
|2
|ln|-sf|/usr/local/nginx/sbin/nginx|/usr/sbin
|

|1
|nginx -t      测试一下吧，成功了吧，是不是很有成就感，别着急 咱们继续！
|

更改nginx配置
首先把原来的配置文件清空:
> /usr/local/nginx/conf/nginx.conf“>” 这个符号之前阿铭介绍过，为重定向的意思，单独用它，可以把一个文本文档快速清空。
vim /usr/local/nginx/conf/nginx.conf写入如下内容:
user nobody nobody;
worker_processes 2;
error_log /usr/local/nginx/logs/nginx_error.log crit;
pid /usr/local/nginx/logs/nginx.pid;
worker_rlimit_nofile 51200;
events
{
    use epoll;
    worker_connections 6000;
}
http
{
    include mime.types;
    default_type application/octet-stream;
    server_names_hash_bucket_size 3526;
    server_names_hash_max_size 4096;
    log_format combined_realip '$remote_addr $http_x_forwarded_for [$time_local]'
    '$host "$request_uri" $status'
    '"$http_referer" "$http_user_agent"';
    sendfile on;
    tcp_nopush on;
    keepalive_timeout 30;
    client_header_timeout 3m;
    client_body_timeout 3m;
    send_timeout 3m;
    connection_pool_size 256;
    client_header_buffer_size 1k;
    large_client_header_buffers 8 4k;
    request_pool_size 4k;
    output_buffers 4 32k;
    postpone_output 1460;
    client_max_body_size 10m;
    client_body_buffer_size 256k;
    client_body_temp_path /usr/local/nginx/client_body_temp;
    proxy_temp_path /usr/local/nginx/proxy_temp;
    fastcgi_temp_path /usr/local/nginx/fastcgi_temp;
    fastcgi_intercept_errors on;
    tcp_nodelay on;
    gzip on;
    gzip_min_length 1k;
    gzip_buffers 4 8k;
    gzip_comp_level 5;
    gzip_http_version 1.1;
    gzip_types text/plain application/x-javascript text/css text/htm application/xml;
server
{
    listen 80;
    server_name localhost;
    index index.html index.htm index.php;
    root /usr/local/nginx/html;
    location ~ \.php$ {
        include fastcgi_params;
        fastcgi_pass 127.0.0.1：9000;
        fastcgi_index index.php;
        fastcgi_param SCRIPT_FILENAME /usr/local/nginx/html$fastcgi_script_name;
    }
}
}》》》》》》》》》》》》》》》》你相信么，你已经搞定了LNMP了《《《《《《《《《《《《《《
## 测试是否解析php文件
创建测试文件:
vim /usr/local/nginx/html/2.php内容如下:
<?php
    echo "测试php是否解析";
?>测试:
curl localhost/2.php看到什么了。。不要怀疑自己的眼睛，不要热泪盈眶。你已经成功了。
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

