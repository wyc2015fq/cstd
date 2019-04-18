# 使用nginx作为css,js,image静态文件的服务器 - z69183787的专栏 - CSDN博客
2017年09月18日 15:04:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：901
## [http://ju.outofmemory.cn/entry/146795](http://ju.outofmemory.cn/entry/146795)
## 概述
最近web项目将图片,css,js等一些静态文件都移出去用svn统一管理了。所以在开发web项目时需要自己导入静态文件，或者配置一个HTTP服务来访问svn下的静态资源。当然不会每次都自己拷贝静态文件到项目了，nginx是一个不错的选择。
# 一些命令
`tasklist /fi "imagename eq nginx.exe"`
显示nginx在系统中的进程
![显示nginx进程](http://blog-tdoly-com.u.qiniudn.com/@/image/blog/body//nginx01.png)
其中一个是主进程，另一个是工作进程。
其他命令:
- nginx -s stop 快速退出
- nginx -s quit 优雅退出
- nginx -s reload 更换配置，启动新的工作进程，优雅的关闭以往的工作进程
- nginx -s reopen 重新打开日志文件
## 简单的配置
静态资源本地路径：E:\svn\web及文件如下:
![本地文件列表](http://blog-tdoly-com.u.qiniudn.com/@/image/blog/body//nginx02.png)
PS：习惯用linux下的ls命令，而在windows里面不能识别这个命令。为了能在windows里面使用ls命令，可以在C:\Windows 中加入一个文件ls.bat，内容如下
```
@echo off
dir
```
这样就能在windows下用ls了。
配置文件conf/nginx.conf
```
#user  nobody;
worker_processes  1;#推荐worker数为cpu核数，避免cpu不必要的上下文切换
events {
    #表示每个worker进程所能建立连接的最大值
    #一个nginx最大的连接数max=worker_connections*worker_processes;
    #对于http请求本地资源最大并发数量为max
    #如果http作为反向代理，最大并发数为max/2。因为每个并发会建立与客户端的连接和与后端服务的连接，会占用两个连接。
    worker_connections  1024;
}
http {
    include       mime.types;
    default_type  application/octet-stream;
    log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
                      '$status $body_bytes_sent "$http_referer" '
                      '"$http_user_agent" "$http_x_forwarded_for"';
    #access_log  logs/access.log  main;
    sendfile        on;
    keepalive_timeout  65;
    server {
        listen       80;
        server_name  localhost;
        #charset koi8-r;
        access_log  logs/host.access.log  main;
        location / {
            root   html;
            index  index.html index.htm;
        }
        # serve static files(css|js|image..)
        #
        location ~ ^/(images|javascript|js|css|flash|media|static)/  {
          root        E:\svn\web;
          access_log  off;
          expires     30d;
        }
        #error_page  404              /404.html;
        # redirect server error pages to the static page /50x.html
        #
        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }
    }
}
```
主要是在server监控的80端口下新增一个location来指定静态文件路径，location 这个指令允许根据URI进行不同的配置。它可以使用字符串和正则表达式进行配置。如果使用正则，就必须使用前缀。
- “~” 匹配时区分大小写
- “~*” 匹配时不区分大小写
- “=” 精确匹配字符和字符串
- 
“^~” 例如： ^~ /images/ 匹配到任何以images开头的，便停止搜索。
location ~ ^/(images|javascript|js|css|flash|media|static)/ { #请求的根文档 root E:\svn\web; #过期时间 expires 30d; }
这里的location匹配以images等开头的路径。如果文件路径不存在，会提示404错误。例如:http://localhost:80/css/detail.css。会找到E:\svn\web\css\detail.css
![本地文件列表](http://blog-tdoly-com.u.qiniudn.com/@/image/blog/body//nginx03.png)可以正常访问到文件。
http://localhost:80/test/test.css。会提示404，虽然存在这个E:\svn\web\test\test.css文件。
![本地文件列表](http://blog-tdoly-com.u.qiniudn.com/@/image/blog/body//nginx04.png)
