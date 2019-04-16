# 服务器数据库系列 - Nginx基础配置说明 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月20日 15:55:18[initphp](https://me.csdn.net/initphp)阅读数：1142








```
user  nfsnobody nfsnobody;  #使用的用户和组
worker_processes 8;  #指定工作衍生进程数
error_log /usr/local/web/nginx.0.8.15/nginx_error.log  crit; #日志记录的文件地址
pid  /usr/local/web/nginx.0.8.15/nginx.pid; #进行PID存放的路劲
worker_rlimit_nofile 65535;  #最大文件描述符
events
{
  use epoll; #使用事件模型，一般Linux2.6+使用epoll模型
  worker_connections 65535; #最大链接数
}
http
{
  include      mime.types;   
  default_type application/octet-stream;
  server_names_hash_bucket_size 128; #hash_table
  client_header_buffer_size 32k; #请求头部的缓冲大小
  large_client_header_buffers 4 32k; #最大的请求头部缓冲大小
  client_max_body_size 8m; #客户端最大请求 8M
  sendfile on; #传输文件
  tcp_nopush    on;
  keepalive_timeout 60; #60秒超时
  tcp_nodelay on;
  fastcgi_connect_timeout 300;#以下fastcgi模块配置
  fastcgi_send_timeout 300;
  fastcgi_read_timeout 300;
  fastcgi_buffer_size 64k;
  fastcgi_buffers 4 64k;
  fastcgi_busy_buffers_size 128k;
  fastcgi_temp_file_write_size 128k;
  gzip on; #以下gzip模块配置
  gzip_min_length  1k;
  gzip_buffers     4 16k;
  gzip_http_version 1.0;
  gzip_comp_level 2;
  gzip_types       text/plain application/x-javascript text/css application/xml;
  gzip_vary on;
  server
  {
    listen      80;  #监听的端口
    server_name www.test2.com; #host名称
    index index.html index.htm index.php; #默认访问的首页
root  /usr/local/web/www/; #文件目录
error_page 404 403  http://admin.test.com/404.htm; #默认404页面
    location ~ .*\.(php|php5)?$   #php cgi模块
    {
      #fastcgi_pass  unix:/tmp/php-cgi.sock;
      fastcgi_pass  127.0.0.1:9000;
      fastcgi_index index.php;
      include fcgi.conf;
    }
location ~ .*\.(gif|jpg|jpeg|png|bmp|swf)$
{
      expires      30d;
    }
   location /       #URL重写模块
    {
      index index.php;
      if (!-f $request_filename){
              rewrite (.*) /admin.php;
          }
      try_files $uri $uri/ /index.html;
    }
    location ~ .*\.(js|css)?$
    {
      expires      1h;
    }
    #以下是log模块-记录nginx日志
    log_format access  '$remote_addr - $remote_user [$time_local] "$request" '
              '$status $body_bytes_sent "$http_referer" '
              '"$http_user_agent" $http_x_forwarded_for';
    access_log /usr/local/web/nginx.0.8.15/logs/access.log  access;
      }
}
```





