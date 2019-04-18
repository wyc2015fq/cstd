# nginx静态资源分离部署 - z69183787的专栏 - CSDN博客
2017年09月18日 15:20:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1421
修改nginx.conf文件，用于nginx处理静态资源。
主要配置如下(在server配置中加入location配置即可)：
server {
    listen       80;
    server_name  123.57.162.75;
    charset utf-8;
    index index.html index.htm index.jsp index.do;
    root /opt/nginx-1.4.7/html/resources;
    
    #配置Nginx动静分离，定义的静态页面直接从Nginx发布目录读取。
    location ~ .*\.(html|htm|gif|jpg|jpeg|bmp|png|ico|txt|js|css)$ 
    { 
        root /opt/nginx-1.4.7/html/resources;
        #expires定义用户浏览器缓存的时间为7天，如果静态页面不常更新，可以设置更长，这样可以节省带宽和缓解服务器的压力
        expires      7d; 
    } 
}
upstream blog.ha97.com {
#upstream的负载均衡，weight是权重，可以根据机器配置定义权重。weigth参数表示权值，权值越
高被分配到的几率越大。
server 192.168.80.121:80 weight=3;
server 192.168.80.122:80 weight=2;
server 192.168.80.123:80 weight=3;
}
#本地动静分离反向代理配置
#所有jsp的页面均交由tomcat或resin处理
location ~ .(jsp|jspx|do)?$ {
proxy_set_header Host $host;
proxy_set_header X-Real-IP $remote_addr;
proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
proxy_pass [http://127.0.0.1:8080](http://127.0.0.1:8080/);
}
#所有静态文件由nginx直接读取不经过tomcat或resin
location ~ .*.(htm|html|gif|jpg|jpeg|png|bmp|swf|ioc|rar|zip|txt|flv|mid|doc|ppt|
pdf|xls|mp3|wma)$
{ expires 15d; }
location ~ .*.(js|css)?$
{ expires 1h; }
}
}
