# nginx + echo模块编译安装 - walkabc - 博客园
# [nginx + echo模块编译安装](https://www.cnblogs.com/hitfire/articles/4563069.html)
　　1:在编译nginx之前要把编译依赖的库和头文件安装好，所需的库就是下面几个openssl,pcre,zlib几个库
　　2:下载nginx和nginx-echo源代码。
　　３：
```
./configure \
  --prefix=/home/m/workspace/build/nginx \
  --sbin-path=/home/m/workspace/build/nginx/nginx \
  --conf-path=/home/m/workspace/build/nginx/nginx.conf \
  --error-log-path=/home/m/workspace/build/nginx/log/error.log \
  --http-log-path=/home/m/workspace/build/nginx/log/access.log \
  --pid-path=/home/m/workspace/build/nginx/nginx.pid  \
  --lock-path=/home/m/workspace/build/nginx/nginx.lock \
  --user=mnginx \
  --group=mnginx \
  --with-http_ssl_module \
  --with-http_flv_module \
  --with-http_stub_status_module \
  --with-http_gzip_static_module \
  --http-client-body-temp-path=/home/m/workspace/build/nginx/temp/client/ \
  --http-proxy-temp-path=/home/m/workspace/build/nginx/proxy/ \
--http-fastcgi-temp-path=/home/m/workspace/build/nginx/fcgi/ \
  --add-module=/home/aron/workspace/build/echo-nginx-module-0.57 \
```

