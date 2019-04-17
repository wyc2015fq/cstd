# 流媒体技术学习笔记之（三）Nginx-Rtmp-Module统计某频道在线观看流的客户数 - DoubleLi - 博客园






获得订阅者人数，可以方便地显示观看流的客户数。

查看已经安装好的模块

```
/usr/local/nginx/sbin/nginx -V
```

安装从源编译Nginx和Nginx-RTMP所需的工具

```
sudo apt-get install build-essential libpcre3 libpcre3-dev libssl-dev
```

下载Nginx和Nginx-RTMP源码

```
wget http://nginx.org/download/nginx-1.7.5.tar.gz
wget https://github.com/arut/nginx-rtmp-module/archive/master.zip
```

提取Nginx和Nginx-RTMP源

```
tar -zxvf nginx-1.7.5.tar.gz
unzip master.zip
```

切换到Nginx目录

```
cd nginx-1.7.5
```

添加Nginx将编译的模块,包括Nginx-RTMP

```
./configure --prefix=/usr/local/nginx --user=www --group=www --with-http_ssl_module --with-http_xslt_module --with-http_flv_module --with-debug --with-http_gzip_static_module --add-module=../nginx-rtmp-module-master
```

提示错误：

```
./configure: error: the HTTP XSLT module requires the libxml2/libxslt
libraries. You can either do not enable the module or install the libraries.
```

配置 --with-http_xslt_module 时提示 the HTTP XSLT module requires the libxml2/libxslt libraries：安装一下扩展

```
sudo apt-get install libxml2 libxml2-dev libxslt-dev
```

编译和安装Nginx与Nginx-RTMP。

```
make
sudo make install
```

安装Nginx初始化脚本

```
sudo wget https://raw.github.com/JasonGiedymin/nginx-init-ubuntu/master/nginx -O /etc/init.d/nginx
sudo chmod +x /etc/init.d/nginx
sudo update-rc.d nginx defaults
```

启动和停止Nginx生成配置文件

```
sudo service nginx start
sudo service nginx stop
```

观看安装是否成功：

```
/usr/local/nginx/sbin/nginx -V
```

安装结果

```
configure arguments: --prefix=/usr/local/nginx --user=www --group=www --with-http_ssl_module --with-http_xslt_module --with-http_flv_module --with-debug --with-http_gzip_static_module --add-module=../nginx-rtmp-module-master
```

修改Nginx 配置文件添加一下信息，在位置设置统计信息页面

```
location /stat {
    rtmp_stat all;
    allow 127.0.0.1;
}
```

创建一个简单的xsl样式表nclients.xsl提取流用户数

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="html"/>

<xsl:param name="app"/>
<xsl:param name="name"/>

<xsl:template match="/">
    <xsl:value-of select="count(//application[name=$app]/live/stream[name=$name]/client[not(publishing) and flashver])"/>
</xsl:template>

</xsl:stylesheet>

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

设置一个返回订阅者数量的位置

```
location /nclients {
    proxy_pass http://127.0.0.1/stat;
    xslt_stylesheet /home/www/nclients.xsl app='$arg_app' name='$arg_name';
    add_header Refresh "3; $request_uri";
}
```

Nginx.config 完整配置：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

user www www;
worker_processes  1;

error_log  logs/error.log  debug;

#pid        logs/nginx.pid;

events {
    worker_connections  65535;
}


http {
    include       mime.types;
    default_type  application/octet-stream;

    log_format  main  '[$time_local][$remote_addr][$http_x_forwarded_for] $status "$request" "$http_referer" "$http_user_agent"';

    access_log  logs/access.log  main;

    sendfile        on;
    keepalive_timeout  65;

    server {
        
        set  $wwwroot  /home/www/node/html;

        listen       80;
        server_name  localhost;
        index        index.html;
        root         $wwwroot;
        access_log   logs/node.access.log  main;
        error_log    logs/error.log debug;
        
        
        location /rtmp/stat {
            rtmp_stat all;
            rtmp_stat_stylesheet rtmpstat.xsl;
        }
        
        location /rtmpstat.xsl {
        }
        
        location /rtmp/control {
            rtmp_control all;
        }
    
       ** location ****/stat {                rtmp_stat all;                allow 127.0.0.1;        }        location /nclients {            proxy_pass http://127.0.0.1/stat;            xslt_stylesheet /home/www/nclients.xsl app='$arg_app' name='$arg_name';            add_header Refresh "3; $request_uri"****;        }**
        
        location ~* /hls/.*\.m3u8$ {
            types {
                application/vnd.apple.mpegurl m3u8;
            }
            root /tmp;
            add_header Cache-Control no-cache;
            add_header Access-Control-Allow-Origin *;
        }

        location ~* /hls/.*\.ts$ {
            types {
                video/mp2t ts;
            }
            root /tmp;
            expires    1m;
            add_header Cache-Control public;
            add_header Access-Control-Allow-Origin *;
        }

        #error_page  404              /404.html;

        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }
  
        location / {
            
        }
        
    }

}

rtmp {
    server {
        listen 1935;
        chunk_size 4096;
        application live {
        live on;
        record off;    
    　　}    
    }
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

重启Nginx

```
root@iZ239kcyg8rZ:/usr/local/nginx/conf# service nginx restart
 * Stopping Nginx Server...                                                                                                                                                           [ OK ] 
 * Starting Nginx Server...                                                                                                                                                           [ OK ] 
root@iZ239kcyg8rZ:/usr/local/nginx/conf#
```

根据直播频道访问以下地址：

```
http://你的推流服务器IP/nclients?app=live&name=4001482820358
```

客户端打开三个播放测试：

![](https://images2015.cnblogs.com/blog/1021265/201612/1021265-20161227161158023-86210660.png)

打开2个VLC播放流：

![](https://images2015.cnblogs.com/blog/1021265/201612/1021265-20161227161306554-1836685009.png)



 查看在线人数：

![](https://images2015.cnblogs.com/blog/1021265/201612/1021265-20161227161403414-683151737.png)


` `


 结束。



from:https://www.cnblogs.com/tinywan/p/6226320.html









