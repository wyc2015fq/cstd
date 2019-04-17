# nginx php mysql 安装 - V__KING__的专栏 - CSDN博客





2015年11月23日 10:57:48[v__king__](https://me.csdn.net/V__KING__)阅读数：429标签：[nginx](https://so.csdn.net/so/search/s.do?q=nginx&t=blog)
个人分类：[linux](https://blog.csdn.net/V__KING__/article/category/1733387)










# nginx php mysql 安装



## 安装
- 
1.安装nginx

获取nginx，在[http://nginx.org/en/download.html](http://nginx.org/en/download.html)上可以获取当前最新的版本。


解压缩nginx-xx.tar.gz包。 

进入解压缩目录，执行./configure 

make & make install

- 
安装php5： sudo apt-get install php5 php5-fpm

- 
安装mysql： sudo apt-get install mysql;




### 配置
- nginx配置： /etc/nginx/conf.d/pi.conf配置



```
server {
    listen       80;
    server_name  localhost;
    location / {
        root   /var/www/html;
        index  index.html index.htm index.php;
    }
    error_page   500 502 503 504  /50x.html;
    location = /50x.html {
        root   /var/www/html;
    }
    location ~ \.php$ {
        fastcgi_pass 127.0.0.1:9000;
        fastcgi_index index.php;
        include fastcgi.conf;
    }
    location ~ \.cgi$ {
        fastcgi_pass 127.0.0.1:8080;
        fastcgi_index index.cgi;
        fastcgi_param SCRIPT_FILENAME fcgi$fastcgi_script_name;
        include fastcgi_params;
    }
}
```
- php 配置 

配置/etc/php5/fpm/pool.d/www.conf 

添加： listen = 127.0.0.1:9000




