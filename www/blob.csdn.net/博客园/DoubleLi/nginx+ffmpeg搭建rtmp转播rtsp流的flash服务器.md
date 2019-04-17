# nginx+ffmpeg搭建rtmp转播rtsp流的flash服务器 - DoubleLi - 博客园






# 本文概要：

        nginx是非常优秀的开源服务器，用它来做hls或者rtmp流媒体服务器是非常不错的选择。本文介绍了一种简易方法快速搭建rtmp流媒体服务器，也叫rtsp转播，数据源不是读取文件，而是采用获取rtspNal流后使用ffmpeg转播。csdn固本培元：leoluopy@gmail.com

开发环境：centos6.4 （主流的linux服务器选择，30%以上的市场占有率）

需要这几个包。

版本号：nginx 1.7.3  openssl 1.0.2   rtmp-module（版本未知）



资源：这些都是公网下载的资源，有需要的朋友，可以给我email或者评论留言。email：leoluopy@gmail.com



注意：先编译openssl 然后编译nginx

编译选项：

./configure --add-module=../nginx-rtmp-module-master --without-http_rewrite_module --with-openssl=../openssl-1.0.2-beta2



一路通过



启动时可能出现错误：

Starting nginx: nginx: [emerg] unknown directive "rtmp" in /etc/nginx/nginx.conf:9

没有识别rtmp模块。

另外一种可能，原有centos从源安装了nginx，它不带rtmp模块，需要卸载。



安装编译好的nginx后：

进入：

/usr/local/nginx/sbin

./nginx -c /usr/local/nginx/conf/nginx.conf

（注意：后面的需要是绝对地址）





ffmpeg推流 （如果没有flv文件，可以用264，ts这些文件转换，ffmpeg的常用命令本博客另一篇文章也有简要介绍）



ffmpeg -re -i IpCam.flv -f flv rtmp://localhost/myapp/test1

ffmpeg -re -i IpCam.flv -f flv -b 20000000 rtmp://localhost/myapp/test1



实时rtsp转rtmp直播流（其实转了rtmp之后会多几秒钟的延迟，不同客户端还不一定一致,我们这里测试的是JWplayer 3秒左右（后面用了个flex的方案，时间延迟提升到2s），稳定时可以在1秒左右，vlc播放器更长了10秒 ~_~!  ，客户端的话还是就用rtsp吧，这样很快，一般延迟可以忽略不计，rtmp本来就是为web端设计的）

rtsp流的获取方法可以参考另外一篇文章：http://blog.csdn.net/gubenpeiyuan/article/details/19072223



ffmpeg -re -i RealStream.fifo -f flv -b 20000000 rtmp://localhost/myapp/test1



RealStream.fifo  是创建的fifo。 命令： mkfifo RealStream.fifo

启动rtsp获取Nal添加sps和pps位，之后打开fifo实时写入即可。



使用vlc测试如下：

![](http://img.blog.csdn.net/20140725111410265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3ViZW5wZWl5dWFu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



此后可以将实时流推送到rmp了。

我的方案是live555接收流，串流方式推送至fifo，做rtmp的转发。可能有些延迟，哪位朋友有更好的方案，欢迎交流评论。

其框架如下：非常简单，并容易集成入大流媒体框架。



![](http://img.blog.csdn.net/20140728141617341?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3ViZW5wZWl5dWFu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



Flex方案截图：



![](http://img.blog.csdn.net/20140731151709395?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3ViZW5wZWl5dWFu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



附录：

RTMP配置如下：





**[plain]**[view plain](http://blog.csdn.net/gubenpeiyuan/article/details/38089013)[copy](http://blog.csdn.net/gubenpeiyuan/article/details/38089013)



- #user  nobody;  
- worker_processes  1;  
- 
- #error_log  logs/error.log;  
- #error_log  logs/error.log  notice;  
- #error_log  logs/error.log  info;  
- 
- #pid        logs/nginx.pid;  
- 
- 
- events {  
-     worker_connections  1024;  
- }  
- 
- rtmp {    
-     server {    
-         listen 1935;    
- 
-         application myapp {    
-             live on;    
- 
-             #record keyframes;    
-             #record_path /tmp;    
-             #record_max_size 128K;    
-             #record_interval 30s;    
-             #record_suffix .this.is.flv;    
- 
-             #on_publish http://localhost:8080/publish;    
-             #on_play http://localhost:8080/play;    
-             #on_record_done http://localhost:8080/record_done;    
- 
-        }    
-        application hls {    
-              live on;    
-              hls on;    
-              hls_path /tmp/app;    
-              hls_fragment 5s;    
- 
- 
-        }    
-     }    
- }  
- 
- http {  
-     include       mime.types;  
-     default_type  application/octet-stream;  
- 
-     #log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '  
-     #                  '$status $body_bytes_sent "$http_referer" '  
-     #                  '"$http_user_agent" "$http_x_forwarded_for"';  
- 
-     #access_log  logs/access.log  main;  
- 
-     sendfile        on;  
-     #tcp_nopush     on;  
- 
-     #keepalive_timeout  0;  
-     keepalive_timeout  65;  
- 
-     #gzip  on;  
- 
-     server {  
-         listen       80;  
-         server_name  localhost;  
- 
-         #charset koi8-r;  
- 
-         #access_log  logs/host.access.log  main;  
- 
-         location / {  
-             root   html;  
-             index  index.html index.htm;  
-         }  
- 
-         #error_page  404              /404.html;  
- 
-         # redirect server error pages to the static page /50x.html  
-         #  
-         error_page   500 502 503 504  /50x.html;  
-         location = /50x.html {  
-             root   html;  
-         }  
- 
-         # proxy the PHP scripts to Apache listening on 127.0.0.1:80  
-         #  
-         #location ~ \.php$ {  
-         #    proxy_pass   http://127.0.0.1;  
-         #}  
- 
-         # pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000  
-         #  
-         #location ~ \.php$ {  
-         #    root           html;  
-         #    fastcgi_pass   127.0.0.1:9000;  
-         #    fastcgi_index  index.php;  
-         #    fastcgi_param  SCRIPT_FILENAME  /scripts$fastcgi_script_name;  
-         #    include        fastcgi_params;  
-         #}  
- 
-         # deny access to .htaccess files, if Apache's document root  
-         # concurs with nginx's one  
-         #  
-         #location ~ /\.ht {  
-         #    deny  all;  
-         #}  
-     }  
- 
- 
-     # another virtual host using mix of IP-, name-, and port-based configuration  
-     #  
-     #server {  
-     #    listen       8000;  
-     #    listen       somename:8080;  
-     #    server_name  somename  alias  another.alias;  
- 
-     #    location / {  
-     #        root   html;  
-     #        index  index.html index.htm;  
-     #    }  
-     #}  
- 
- 
-     # HTTPS server  
-     #  
-     #server {  
-     #    listen       443 ssl;  
-     #    server_name  localhost;  
- 
-     #    ssl_certificate      cert.pem;  
-     #    ssl_certificate_key  cert.key;  
- 
-     #    ssl_session_cache    shared:SSL:1m;  
-     #    ssl_session_timeout  5m;  
- 
-     #    ssl_ciphers  HIGH:!aNULL:!MD5;  
-     #    ssl_prefer_server_ciphers  on;  
- 
-     #    location / {  
-     #        root   html;  
-     #        index  index.html index.htm;  
-     #    }  
-     #}  
- 
- }  










参考文章：

nginx的RTMP协议服务器

[http://www.cnblogs.com/aHuner/p/3247068.html](http://www.cnblogs.com/aHuner/p/3247068.html)

用开源nginx-rtmp-module搭建flash直播环境

[http://www.rosoo.net/a/201307/16696.html](http://www.rosoo.net/a/201307/16696.html)

[nginx上搭建HLS流媒体服务器](http://blog.csdn.net/cjsafty/article/details/7922849)

[http://blog.csdn.net/cjsafty/article/details/7922849](http://blog.csdn.net/cjsafty/article/details/7922849)

[基于nginx的hls直播系统](http://blog.csdn.net/cjsafty/article/details/9108587)（记录了rtmp的问题和解决方法）

[http://blog.csdn.net/cjsafty/article/details/9108587](http://blog.csdn.net/cjsafty/article/details/9108587)

C++实现RTMP协议发送H.264编码及AAC编码的音视频

[http://www.cnblogs.com/haibindev/archive/2011/12/29/2305712.html](http://www.cnblogs.com/haibindev/archive/2011/12/29/2305712.html)

Centos安装FLASHPlayer插件

http://www.linuxidc.com/Linux/2013-03/81073.htm






ffmpeg推流

ffmpeg -re -i ~/2012.flv -f flv rtmp://192.168.1.102/myapp/test1



ffmpeg -re -i IpCam.h264 -vcodec copy -f flv -b 20000000 rtmp://localhost/myapp/test1






from:http://blog.csdn.net/gubenpeiyuan/article/details/38089013









