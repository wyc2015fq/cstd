# rtmp与hls流媒体服务器搭建：ubuntu下Nginx搭建初探与rtmp-module的添加 - DoubleLi - 博客园






**关键词：Nignx（http服务器）；rtmp，hls（流媒体服务）**



前言：感谢开源，感谢战斗民族。现在在做流媒体服务的一些工作，流媒体服务器搭建的网上教程多入牛毛，但是细细查看，发现很多同志贴上来的教程都有些许问题，还有一些是爬别人的数据得来的，排版上也有问题，让新手无所适从。本文也是通过翻看一些优质的博文得到的这篇分享。以下分享的网址就是引用的博文，安利一下。

1. http://redstarofsleep.iteye.com/blog/2123752

2. http://blog.csdn.net/cjsafty/article/details/9108587

3. http://blog.chinaunix.net/uid-20639775-id-154556.html



教程：

    1. 下载nginx, openssl, pcre, zlib到一个目录下，如（/home/user），用wget命令，具体可查看下面的各种版本。

```
nginx-1.8.0.tar.gz
openssl-1.0.1i.tar.gz
pcre-8.37.tar.gz
zlib-1.2.8.tar.gz
```

      2. tar解压。

```
tar -zxvf nginx-1.8.0.tar.gz
tar -zxvf openssl-1.0.1i.tar.gz
tar -zxvf pcre-8.37.tar.gz
tar -zxvf zlib-1.2.8.tar.gz
```

    3. git下nginx-rtmp-module（https://github.com/arut/nginx-rtmp-module），具体略。

    4. 进入nginx内，配置好，再make和make install。

   （这里强调一下，/home/user里的user就是你的用户名，然后是configure的时候最好加入--with-debug）

```java
./configure --prefix=/usr/local/nginx --with-debug --with-pcre=/home/user/pcre/pcre-8.37 --with-zlib=/home/user/zlib/zlib-1.2.8 --with-openssl=/home/user/openssl/openssl-1.0.1i  --add-module=/home/user/nginx-rtmp-module
make
make install
```

    5. 进到（/usr/local/nginx/conf），配置好conf，下面来个简单完整版的

```
#user  nobody;
worker_processes  1;

#这里可以通过加入debug模式来查看error.log，内容更加丰富
error_log  logs/error.log debug;
#error_log  logs/error.log  notice;
#error_log  logs/error.log  info;

#pid        logs/nginx.pid;


events {
    worker_connections  1024;
}
-------------------------------------@@@
rtmp {
    server {
        listen 1935;

        application myapp {
            live on;
        }
        application hls {
            live on;
            hls on;
            hls_path /tmp/hls;
        }
    }
}
---------------------------------------@@@

http {
    include       mime.types;
    default_type  application/octet-stream;

    sendfile        on;
    keepalive_timeout  65;

    server {
        listen       80;
        server_name  localhost;

        --------------------------------------------@@@
        location /hls {
            types {
                application/vnd.apple.mpegurl m3u8;
                video/mp2t ts;
            }
            root /tmp;
            add_header Cache-Control no-cache;
       }
       --------------------------------------------@@@
        location / {
            root   html;
            index  index.html index.htm;
        }

}
```

    加的部分已经在上面标出来了。

    6. 进入（/usr/local/nginx/sbin）运行
`./ngnix`
    7. 可以查看端口占用，80,1935是不是在监听
`netstat -ltn`
    8. 更多测试

        1）把rtmp流发给服务器
`rtmp://ip/myapp/test`
    myapp是ngnix.conf里application设置的，stream为test则是自定义的。

        2）接收hls流
`http://ip/hls/test.m3u8`
    hls是http内的location设置的，访问的是Linux下的（/tmp/hls），test则是按照上面rtmp的test，如果上面是haha，下面也是haha。

        3）ngnix主页
`http://ip`


后言：有教程说明在./configure时可以不需要zlib和pcre，这个我没有试过，真心不需要这两个模块的童鞋们可以不添加。









