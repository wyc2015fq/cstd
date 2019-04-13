
# Nginx配置Https - 致林 - 博客园






# [Nginx配置Https](https://www.cnblogs.com/bincoding/p/6118270.html)
**1.申请证书：**
[https://console.qcloud.com/ssl?utm_source=yingyongbao&utm_medium=ssl&utm_campaign=qcloud](https://console.qcloud.com/ssl?utm_source=yingyongbao&utm_medium=ssl&utm_campaign=qcloud)
**2.将证书导入服务器，xftp或者FileZilla都可以:**
![](https://images2015.cnblogs.com/blog/771778/201611/771778-20161130160633006-1658635390.png)
**3.修改Nginx，我的Nginx位于/etc/nginx/ :**
![](https://images2015.cnblogs.com/blog/771778/201611/771778-20161130161102881-1434375029.png)
Nginx自带ssl配置，删掉注释，修改证书位置就好了。
![](https://images2015.cnblogs.com/blog/771778/201611/771778-20161130161108365-1331111520.png)
80端口配置：
server {
        listen80default_server;
        listen       [::]:80default_server;
        server_name  _;
        \#root/usr/share/nginx/html;
\#注意添加下面这行代码，用于重定向
        rewrite^(.*) https://$server_name$1 permanent;\# Load configuration filesforthe default server block.
        include/etc/nginx/default.d/*.conf;
        location / {
        }
        error_page 404 /404.html;
            location = /40x.html {
        }
        error_page 500 502 503 504 /50x.html;
            location = /50x.html {
        }
   }
443端口设置:
server {
        listen443ssl http2 default_server;
        listen       [::]:443ssl http2 default_server;
        server_name  _;
        root/usr/share/nginx/html;
\#        ssl_certificate"/etc/pki/nginx/server.crt";
\#        ssl_certificate_key"/etc/pki/nginx/private/server.key";
        ssl_certificate/etc/nginx/conf.d/1_www.bincoding.cn_bundle.crt;
        ssl_certificate_key/etc/nginx/conf.d/2_www.bincoding.cn.key;
        ssl_session_cache shared:SSL:1m;
        ssl_session_timeout  10m;
        ssl_ciphers HIGH:!aNULL:!MD5;
        ssl_prefer_server_ciphers on;
        \# Load configuration filesforthe default server block.
        include/etc/nginx/default.d/*.conf;
        location / {
        }
        error_page 404 /404.html;
            location = /40x.html {
        }
        error_page 500 502 503 504 /50x.html;
            location = /50x.html {
        }
    }
最后记得重启Nginx :  service nginx restart
参考：
nginx配置https   https://www.v2ex.com/t/309382
常用服务器配置[https://mozilla.github.io/server-side-tls/ssl-config-generator/](https://mozilla.github.io/server-side-tls/ssl-config-generator/)





