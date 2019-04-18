# nginx反向代理及缓存清理 - weixin_33985507的博客 - CSDN博客
2017年12月19日 11:49:22[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：18
#下载安装包
wget [http://nginx.org/download/nginx-1.13.7.tar.gz](http://nginx.org/download/nginx-1.13.7.tar.gz)
wget [http://labs.frickle.com/files/ngx_cache_purge-2.3.tar.gz](http://labs.frickle.com/files/ngx_cache_purge-2.3.tar.gz)
wget [https://nchc.dl.sourceforge.net/project/pcre/pcre/8.40/pcre-8.41.tar.bz2](https://nchc.dl.sourceforge.net/project/pcre/pcre/8.40/pcre-8.41.tar.bz2)
#--------------- 在未安装nginx的情况下安装ngx_cache_purge ---------------#
cd nginx-1.13.7
./configure --prefix=/usr/local/nginx \
--with-pcre=/opt/pcre-8.41 \
--user=www \
--group=www \
--with-http_stub_status_module \
--with-http_ssl_module \
--with-http_flv_module \
--with-http_gzip_static_module
--add-module=/opt/ngx_cache_purge-2.3
make
make install
#--------------- 在已安装nginx情况下安装ngx_cache_purge ---------------#
cd nginx-1.13.7
./configure --prefix=/usr/local/nginx \
--with-pcre=/opt/pcre-8.41 \
--user=www \
--group=www \
--with-http_stub_status_module \
--with-http_ssl_module \
--with-http_flv_module \
--with-http_gzip_static_module
--add-module=/opt/ngx_cache_purge-2.3
make  //千万不要make install，不然就真的覆盖了
/etc/init.d/nginx stop
mv /usr/local/nginx/sbin/nginx /usr/local/nginx/sbin/nginx.bak
cp objs/nginx /usr/local/nginx/sbin/nginx
#nginx反向代理及缓存清理
#user  nobody;
worker_processes  8;
#error_log  logs/error.log;
#error_log  logs/error.log  notice;
#error_log  logs/error.log  info;
#pid        logs/nginx.pid;
events {
worker_connections  65535;
}
http {
include       mime.types;
default_type  application/octet-stream;
```
#log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
#                  '$status $body_bytes_sent "$http_referer" '
#                  '"$http_user_agent" "$http_x_forwarded_for"';
#access_log  logs/access.log  main;
sendfile        off;
tcp_nopush     on;
keepalive_timeout  300;
#nginx跟后端服务器连接超时时间(代理连接超时)
proxy_connect_timeout 300s; 
#连接成功后，后端服务器响应时间(代理接收超时)
proxy_read_timeout 300s; 
proxy_send_timeout 300s; 
#设置代理服务器（nginx）保存用户头信息的缓冲区大小
proxy_buffer_size 64k;
#proxy_buffers缓冲区，网页平均在32k以下的话，这样设置
proxy_buffers 4 32k;
#高负荷下缓冲大小（proxy_buffers*2）
proxy_busy_buffers_size 64k; 
#设定缓存文件夹大小，大于这个值，将从upstream服务器传递请求，而不缓冲到磁盘
proxy_temp_file_write_size 64k; 
#不允许代理端主动关闭连接
proxy_ignore_client_abort on;
proxy_cache_path /data/cache1 levels=1:2 keys_zone=cache_blufly:20m inactive=1d max_size=1g;
fastcgi_connect_timeout 300;
fastcgi_send_timeout 300;
fastcgi_read_timeout 300;
fastcgi_buffer_size 64k;
fastcgi_buffers 4 64k;
fastcgi_busy_buffers_size 128k;
fastcgi_temp_file_write_size 128k;
gzip on;
gzip_min_length  1k;
gzip_buffers     4 16k;
gzip_http_version 1.0;
gzip_comp_level 2;
gzip_types       text/plain application/x-javascript text/css application/xml;
gzip_vary on;
#禁止非指定域名访问
server {
listen 80 default;
server_name  _;
return 500;
}
#www.blufly.com前端缓存
server
{
listen  80;
server_name blufly.com www.blufly.com;
add_header X-Via $server_addr;
add_header X-Cache $upstream_cache_status;
#301重定向设置
if ($host != 'www.blufly.com' ) 
{
    rewrite ^/(.*)$ http://www.blufly.com/$1 permanent;
}
location /
{
    proxy_cache cache_blufly;
    proxy_cache_valid 200 302 3h;
    proxy_cache_key $uri$is_args$args;
    proxy_set_header Host www.blufly.com;
    proxy_set_header X-Forwarded-For $remote_addr;
    proxy_pass http://192.168.5.31:80/;
    expires      1d;
}
#使用ngx_cache_purge清理缓存
    location ~ /purge(/.*) {
    allow              127.0.0.1;
    allow              192.168.5.0/24;
    deny               all;
    proxy_cache_purge  cache_blufly $1$is_args$args;
    }
access_log off;
}
```
}
#---------------- nginx缓存清理脚本 ------------------------#
cat atuo_nginx_cache_clean.sh
#!/bin/bash
mfile="$*"
cache_dir=/data/cache1
echo $mfile
if [ "$#" -eq 0 ]
then
echo "please input scripts, if not, it will exit"
sleep 2 && exit
fi
echo "what you put $mfile will delete, please wait..."
for i in `echo $mfile | sed 's/ /\n/g'`
do
grep -ira $i $cache_dir | awk -F ':' '{print $1}' > /tmp/cache_list.txt
for j in `cat /tmp/cache_list.txt`
do
rm -rf $j
echo "$i $j is delete Success!"
done
done
#脚本用法
./atuo_nginx_cache_clean.sh aa.jpg bb.js cc.html
![nginx反向代理及缓存清理](http://i2.51cto.com/images/blog/201712/19/bae95e24a3018c31b869f74b7618e779.jpg?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![nginx反向代理及缓存清理](http://i2.51cto.com/images/blog/201712/19/8440050563565a4c12d2f23aa965de91.jpg?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
