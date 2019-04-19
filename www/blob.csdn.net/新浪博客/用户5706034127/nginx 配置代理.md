# nginx 配置代理_用户5706034127_新浪博客
||分类：[web](http://blog.sina.com.cn/s/articlelist_5706034127_7_1.html)|
```
location / {
        proxy_pass       http://127.0.0.1:9000;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header Host $host;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
    }
```
```
```
由于win下不能按章 uwsgi，就直接使用nginx + django了。proxy_pass表示被代理的服务器的地址和端口。
nginx是反向代理服务器
