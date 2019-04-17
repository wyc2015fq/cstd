# Nginx前后端分离配置实践-微服务模式 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年06月27日 12:46:18[boonya](https://me.csdn.net/boonya)阅读数：7827








Nginx前后端分离配置实践：本文主要记录前端通过Nginx与后端实现交互的配置。通过Nginx对外提供反向访问前端页面和后台接口，只需要在Nginx作简单的配置就可以对外发布应用服务（后端分模块开发，分别按照规范提供不同的业务接口-授权登录统一即可，实际上就是提供微服务）。同一个Nginx可以实现多个平台的服务配置，也就实现了微服务模式。

## Nginx配置

主要关注 server（监控平台）的配置：




```
worker_processes  1;
events {
    worker_connections  128;
}
http {
    include       mime.types;
    default_type  application/octet-stream;
    sendfile        on;
    keepalive_timeout  65;

 
	
  # 监控平台
  server {
        #配置HTTP服务器监听的端口号，这里默认写为8880
        listen       8881;
        server_name  localhost;
        location / {
            #替换下面的地址为您网页目录地址
            root   E:/work/cvnaviHtml/Monitor;
            index  index.html index.htm;
        }
        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }
        #下面配置Tomcat或WAS服务器的地址，服务程序会自动进行代理转发
        location /monitoryt {
            proxy_pass   http://10.10.11.112:8081/cvnavi-monitoryt;
        }
    }
}
```





## 静态页面

主要关注静态文件的路径:E:\work\cvnaviHtml\Monitor


![](https://img-blog.csdn.net/20170627123423457)

![](https://img-blog.csdn.net/20170627123541749)

Nginx配置的时候可以找到index.html页面。


## 后台接口

后台接口与前端是分离的，可以从Nginx代理到Tomcat服务


![](https://img-blog.csdn.net/20170627123943588)

通过Nginx代理访问前端页面

![](https://img-blog.csdn.net/20170627124353408)

sosoapi管理Nginx反向代理的接口

![](https://img-blog.csdn.net/20170627124509138)

接口测试正常：


![](https://img-blog.csdn.net/20170627124524845)




