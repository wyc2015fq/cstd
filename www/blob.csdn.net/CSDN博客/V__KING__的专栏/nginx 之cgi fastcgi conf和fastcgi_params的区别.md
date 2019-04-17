# nginx 之cgi fastcgi.conf和fastcgi_params的区别 - V__KING__的专栏 - CSDN博客





2015年11月23日 11:00:19[v__king__](https://me.csdn.net/V__KING__)阅读数：6870









# nginx 之cgi



## fastcgi.conf和fastcgi_params的区别：

只有一行的差别

> 
$: diff fastcgi_params fastcgi.conf 

2d1 

< fastcgi_param SCRIPT_FILENAME fastcgi_script_name;


不再建议大家使用以下方式（搜了一下，网上大量的文章，并且nginx.conf的默认配置也是使用这种方式）：



```
location ~ \.php$ {
        root            /var/www/wordpress;
        fastcgi_pass   127.0.0.1:9000;
        fastcgi_index  index.php;
        #fastcgi_param  SCRIPT_FILENAME  $document_root$fastcgi_script_name;
        #include        fastcgi_params;
        include        fastcgi.conf;#只用包含fastcgi.conf就行了，上面就可以省去了
    }
```


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



