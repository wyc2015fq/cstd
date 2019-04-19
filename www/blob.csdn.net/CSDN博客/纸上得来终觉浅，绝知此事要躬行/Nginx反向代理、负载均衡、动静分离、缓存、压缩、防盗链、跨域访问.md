# Nginx反向代理、负载均衡、动静分离、缓存、压缩、防盗链、跨域访问 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客
2019年03月12日 09:07:58[boonya](https://me.csdn.net/boonya)阅读数：148
原文地址：[http://www.bubuko.com/infodetail-2839821.html](http://www.bubuko.com/infodetail-2839821.html)
**目录**
[一、反向代理](#%E4%B8%80%E3%80%81%E5%8F%8D%E5%90%91%E4%BB%A3%E7%90%86)
[二、负载均衡](#%E4%BA%8C%E3%80%81%E8%B4%9F%E8%BD%BD%E5%9D%87%E8%A1%A1)
[三、动静分离](#%E4%B8%89%E3%80%81%E5%8A%A8%E9%9D%99%E5%88%86%E7%A6%BB)
[缓存](#%C2%A0%E7%BC%93%E5%AD%98)
[压缩](#%E5%8E%8B%E7%BC%A9)
[四、防盗链](#%E5%9B%9B%E3%80%81%E9%98%B2%E7%9B%97%E9%93%BE)
[五、跨域请求](#%E4%BA%94%E3%80%81%E8%B7%A8%E5%9F%9F%E8%AF%B7%E6%B1%82)
# 一、反向代理
1、在192.168.189.130机器启动tomcat服务，http://192.168.189.130:8080/ 访问服务正常
2、在192.168.189.131机器配置nginx
server {
        listen       80;
        server_name  localhost;
        #charset koi8-r;
        #access_log  logs/host.access.log  main;
        location / {
            **proxy_pass  http://192.168.189.130:8080;**
            root   html;
            index  index.html index.htm;
        }
        #error_page  404              /404.html;
        # redirect server error pages to the static page /50x.html
        #
        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }
3、启动nginx服务，http://192.168.189.131/ 能够访问，这就完成了一个最简单的反向代理
# 二、负载均衡
1、负载均衡使用的是nginx的http upstream模块，语法：server address；
2、负载均衡策略或算法：默认使用轮询算法，可以使用ip_hash 或者 权重轮询
3、为了配置清晰，在nginx配置文件中http模块引入额外配置
http {
    include       mime.types;
    **include      extra/*.conf;**
    default_type  application/octet-stream;
    #log_format  main  ‘$remote_addr - $remote_user [$time_local] "$request" ‘
    #                  ‘$status $body_bytes_sent "$http_referer" ‘
    #                  ‘"$http_user_agent" "$http_x_forwarded_for"‘;
    #access_log  logs/access.log  main;
    sendfile        on;
    #tcp_nopush     on;
    #keepalive_timeout  0;
    keepalive_timeout  65;
    #gzip  on;
    # server {
     #   listen       80;
     #   server_name  localhost;
        #charset koi8-r;
        #access_log  logs/host.access.log  main;
目录：
/root/data/program/nginx/conf/extra
配置文件proxy_demo.conf 
#负载均衡模块
upstream tomcat {
    #最大失败2次，失败后在60秒内不再转发到该机器
    server 192.168.189.130:8080 max_fails=2 fail_timeout=60s;
    server 192.168.189.132:8080 max_fails=2 fail_timeout=60s;
}
server {
    listen 80;
    server_name localhost;
    location / {
        proxy_pass http://tomcat;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        #性能优化相关
        #例：130机器 error、超时、500、503的时候请求转到132
        proxy_next_upstream error timeout http_500 http_503;
        #连接超时时间
        proxy_connect_timeout 60s;
        proxy_send_timeout 60s;
        proxy_read_timeout 60s;
    }
}
4、分别在192.168.189.130，192.168.189.132机器上启动tomcat，
然后在/root/data/program/apache-tomcat-8.5.34/webapps/ROOT 下index.jsp页面添加一些标识区分，启动tomcat服务
访问http://192.168.189.131/，可以看到130、132两个服务轮询处理请求 
# 三、动静分离
1、132机器/root/data/program/apache-tomcat-8.5.34/webapps/ROOT目录新建文件夹bak，把除index.jsp外的其他文件（静态资源）移动到bak文件夹
![技术分享图片](http://image.bubuko.com/info/201811/20181108234850500601.png)
此时访问http://192.168.189.132:8080/发现首页样式没了
![技术分享图片](http://image.bubuko.com/info/201811/20181108234851016245.png)
2、配置nginx，使用正则表达式，当请求静态资源时从**static-resource**获取
#负载均衡模块
upstream tomcat {
    #最大失败2次，失败后在60秒内不再转发到该机器
    #server 192.168.189.130:8080 max_fails=2 fail_timeout=60s;
    server 192.168.189.132:8080 max_fails=2 fail_timeout=60s;
}
server {
    listen 80;
    server_name localhost;
    location / {
        proxy_pass http://tomcat;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        #性能优化相关
        #例：130机器 error、超时、500、503的时候请求转到132
        proxy_next_upstream error timeout http_500 http_503;
        #连接超时时间
        proxy_connect_timeout 60s;
        proxy_send_timeout 60s;
        proxy_read_timeout 60s;
    }
    **location ~ .*\.(js|css|png|svg|ico|jpg)$ {         root static-resource;    }**
3、在nginx根目录下创建文件夹static-resource,把tomcat目录下的静态资源文件拷贝到此目录（静态资源文件，不包含jsp）
tomcat静态资源文件地址：/root/data/program/apache-tomcat-8.5.34/webapps/ROOT
![技术分享图片](http://image.bubuko.com/info/201811/20181108234851739906.png)
4、./nginx -s reload  nginx重新加载，此时访问http://192.168.189.131/可以看到当请求转发到192.168.189.132:8080上时，首页样式能够显示
![技术分享图片](http://image.bubuko.com/info/201811/20181108234851964524.png)
##  缓存
nginx可以通过expires设置缓存，比如我们可以针对图片做缓存。
在location中设置expires
格式： expires 30s|m|h|d
#负载均衡模块
upstream tomcat {
    #最大失败2次，失败后在60秒内不再转发到该机器
    # server 192.168.189.130:8080 max_fails=2 fail_timeout=60s;
    server 192.168.189.132:8080 max_fails=2 fail_timeout=60s;
}
server {
    listen 80;
    server_name localhost;
    location / {
        proxy_pass http://tomcat;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        #性能优化相关
        #例：130机器 error、超时、500、503的时候请求转到132
        proxy_next_upstream error timeout http_500 http_503;
        #连接超时时间
        proxy_connect_timeout 60s;
        proxy_send_timeout 60s;
        proxy_read_timeout 60s;
    }
    location ~ .*\.(js|css|png|svg|ico|jpg)$ {
         root static-resource;
         **expires 1d;**
    }
}
对比设置缓存前，浏览器请求静态资源Response Headers
![技术分享图片](http://image.bubuko.com/info/201811/20181108234852180353.png)
设置expires 1d;后浏览器多了Expires
![技术分享图片](http://image.bubuko.com/info/201811/20181108234852440128.png)
## 压缩
Nginx中提供了一种Gzip的压缩优化手段，可以对后端的文件进行压缩传输，压缩以后的好处在于能够降低文件的大小来提高传输效率 
配置信息
Gzip on|off 是否开启gzip压缩
Gzip_buffers 4 16k #设置gzip申请内存的大小，作用是按指定大小的倍数申请内存空间。4 16k代表按照原始数据大小以16k为单位的4倍申请内存。
Gzip_comp_level[1-9] 压缩级别， 级别越高，压缩越小，但是会占用CPU资源
Gzip_disable #正则匹配UA 表示什么样的浏览器不进行gzip
Gzip_min_length #开始压缩的最小长度（小于多少就不做压缩），可以指定单位，比如 1k
Gzip_http_version 1.0|1.1 表示开始压缩的http协议版本
Gzip_proxied （nginx 做前端代理时启用该选项，表示无论后端服务器的headers头返回什么信息，都无条件启用压缩）
Gzip_type text/pliain,application/xml 对那些类型的文件做压缩 （conf/mime.conf）
Gzip_vary on|off 是否传输gzip压缩标识; 启用应答头"Vary: Accept-Encoding";给代理服务器用的，有的浏览器支持压缩，有的不支持，所以避免浪费不支持的也压缩，所以根据客户端的HTTP头来判断，是否需要压缩 
user  root;
#user  nobody;
worker_processes  1;
#error_log  logs/error.log;
#error_log  logs/error.log  notice;
#error_log  logs/error.log  info;
#pid        logs/nginx.pid;
events {
    worker_connections  1024;
}
http {
    include       mime.types;
    include      extra/*.conf;
    default_type  application/octet-stream;
    #配置nginx压缩
    **gzip on;    gzip_min_length 5k;    gzip_comp_level 3;    gzip_types application/javascript image/jpeg image/svg+xml;    gzip_buffers 4 32k;    gzip_vary on;**
压缩前：
![技术分享图片](http://image.bubuko.com/info/201811/20181108234853061246.png)
压缩后：
![技术分享图片](http://image.bubuko.com/info/201811/20181108234853306372.png)
![技术分享图片](http://image.bubuko.com/info/201811/20181108234853640370.png)
# 四、防盗链
1、设置静态资源只能192.168.189.132访问，其他ip访问返回404
#负载均衡模块
upstream tomcat {
    #最大失败2次，失败后在60秒内不再转发到该机器
    # server 192.168.189.130:8080 max_fails=2 fail_timeout=60s;
    server 192.168.189.132:8080 max_fails=2 fail_timeout=60s;
}
server {
    listen 80;
    server_name localhost;
    location / {
        proxy_pass http://tomcat;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        #性能优化相关
        #例：130机器 error、超时、500、503的时候请求转到132
        proxy_next_upstream error timeout http_500 http_503;
        #连接超时时间
        proxy_connect_timeout 60s;
        proxy_send_timeout 60s;
        proxy_read_timeout 60s;
    }
    location ~ .*\.(js|css|png|svg|ico|jpg)$ {
         #防盗链
         #设置静态资源只能192.168.189.132访问，其他ip访问返回404
         **valid_referers none blocked 192.168.189.132;         if ($invalid_referer) {             return 404;         }**
         root static-resource;
         expires 1d;
    }
}
![技术分享图片](http://image.bubuko.com/info/201811/20181108234854017337.png)
“Referer”请求头为指定值时，内嵌变量$invalid_referer被设置为空字符串，否则这个变量会被置成“1”。
查找匹配时不区分大小写，其中none表示缺少referer请求头、blocked表示请求头存在，但是它的值被防火墙或者代理服务器删除、server_names表示referer请求头包含指定的虚拟主机名 
# 五、跨域请求
环境说明：两台tomcat服务130，132；一台nginx服务131，131nginx代理132tomcat服务，130tomcat服务器请求131上一个json
1、130tomcat   index.jsp页面请求json（注意添加jquery.js）
![技术分享图片](http://image.bubuko.com/info/201811/20181108234854546654.png)
![技术分享图片](http://image.bubuko.com/info/201811/20181108234854703887.png)
2、132服务添加demo.json
![技术分享图片](http://image.bubuko.com/info/201811/20181108234854862096.png)
{
"hello":"world"
}
3、No ‘Access-Control-Allow-Origin‘ header is present on the requested resource.
![技术分享图片](http://image.bubuko.com/info/201811/20181108234855007610.png)
4、nginx配置
upstream tomcat {
    #最大失败2次，失败后在60秒内不再转发到该机器
    #server 192.168.189.130:8080 max_fails=2 fail_timeout=60s;
    server 192.168.189.132:8080 max_fails=2 fail_timeout=60s;
}
server {
    listen 80;
    server_name localhost;
    location / {
        # 注释最原始的代理，使用负载均衡
        # proxy_pass http://192.168.189.132:8080;
        proxy_pass http://tomcat;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        #性能优化相关
        #例：130机器 error、超时、500、503的时候请求转到132
        proxy_next_upstream error timeout http_500 http_503;
        #连接超时时间
        proxy_connect_timeout 60s;
        proxy_send_timeout 60s;
        proxy_read_timeout 60s;
        #跨域请求，注释掉130，只代理132
        add_header ‘Access-Control-Allow-Origin‘ ‘*‘;  #允许来自所有的访问地址
        add_header ‘Access-Control-Allow-Methods‘ ‘GET,PUT,POST,DELETE,OPTIONS‘; #支持的请求方式
        add_header ‘Access-Control-Allow-Header‘ ‘Content-Type,*‘; #支持的媒体类型
    }
    location ~ .*\.(js|css|png|svg|ico|jpg)$ {
        #防盗链
        #设置静态资源只能192.168.189.132访问，其他ip访问返回404
        valid_referers none blocked 192.168.189.132;
        if ($invalid_referer) {
            return 404;
        }
        #130、132两台机器ROOT下静态资源改到bak下，创建static-resource存静态资源
        root static-resource;
        #nginx缓存配置，Response Headers多了一个Expires
        expires 1d;
    }
}
5、效果
![技术分享图片](http://image.bubuko.com/info/201811/20181108234855228321.png)
