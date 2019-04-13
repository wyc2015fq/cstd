
# Nginx 重定向所有子域名到www - 阳光岛主 - CSDN博客

2016年07月08日 08:49:49[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：6538


Apache 配置重定向
vim .htaccess或vim  /var/www/html/.htaccess
|1
|2
|3
|RewriteEngine On
|RewriteCond %{HTTP_HOST} !^www\.yourdomain\.com
|RewriteRule (.*) http:|//www|.yourdomain.com$1 [R=301,L]
|

**Nginx 配置重定向**
vim /usr/local/nginx/conf/nginx.conf
|1
|2
|3
|if|( $http_host !~|"^www\.yourdomain\.com$"|) {
|rewrite  ^(.*)    http:|//www|.youdomain.com$1 permanent;
|}
|

**ReWrite语法**：
last – 基本上都用这个Flag。
break – 中止Rewirte，不在继续匹配
redirect – 返回临时重定向的HTTP状态302
permanent – 返回永久重定向的HTTP状态301
1、if 和location 匹配判断
~ 为区分大小写匹配;
!~ 为区分大小写不匹配
~* 为不区分大小写匹配；
!~* 为不区分大小写不匹配
2、if 判断的表达式：
-f和!-f用来判断是否存在文件
-d和!-d用来判断是否存在目录
-e和!-e用来判断是否存在文件或目录
-x和!-x用来判断文件是否可执行
3、用作判断的全局变量
例：http://mimvp.com:88/test1/test2/test.php
$host：mimvp.com
$server_port：88
$request_uri：http://mimvp.com:88/test1/test2/test.php
$document_uri：/test1/test2/test.php
$document_root：/usr/local/nginx/html/
$request_filename：D:\nginx/html/test1/test2/test.php
用法示例：
1.  nginx在文件和目录不存在的时候重定向：
if (!-e $request_filename) {
proxy_pass http://mimvp.com;
}
2.  设定nginx在用户使用ie的使用重定向到/nginx-ie目录下：
if ($http_user_agent ~ MSIE) {
rewrite ^(.*)$ /nginx-ie/$1 break;
}
3.  设置nginx防盗链：
location ~* \.(gif|jpg|png|swf|flv|swf)$ {
valid_referers none blocked www.mimvp.com  mimvp.com;
if ( $invalid_referer ) {
rewrite ^/ http://$host/logo.png;
}
}
4.  根据文件类型设置过期时间
location ~* \.(js|css|jpg|jpeg|gif|png|swf)$ {
if ( -f $request_filename ) {
expires    1h;
break;
}
}
5.  禁止访问某个目录
location ~* \.(txt|doc)${
root /data/www/wwwroot/linuxtone/test;
deny all;
}
**Redirect语法**
server {
listen 80;
server_name www.mimvp.com;
index index.html index.php;
root html;
if ($http_host !~ “^www\.mimvp\.com$”  {
rewrite ^(.*)http://www.mimvp.com$1 redirect;
}
}
**nginx: [emerg] unknown directive “if($host” in**
修改nginx.conf文件后，重启出错如下：
Starting nginx: nginx: [emerg] unknown directive “if($host” in /usr/local/nginx/conf/nginx_mimvp.com.conf:14[FAILED]
**示例：**把全部 xxx.mimvp.com 都重定向到 www.mimvp.com
|1
|2
|3
|4
|5
|6
|7
|8
|server {
|listen  80;
|server_name mimvp.com www.mimvp.com;
|root|/home/web|;
|if|($host !~|"www\.mimvp\.com$"|) {
|rewrite ^(.*) http:|//www|.mimvp.com$1 permanent;
|}
|}
|

重启Nginx服务：/etc/init.d/nginx restart
报错信息如下：nginx: [emerg] unknown directive “if($host” in…
原因分析：对照手册检查了很久，真的没有写错，可为什么报错？睡了一觉，做了一梦，终于找到原因了，原来Nginx对语法的格式还是比较严格的，”if($host”之间需要空格隔开（”if  (  $host “，也就是括号前后字符，都要至少留一空格）
**正确写法**（注意代码第5行空格的变化）
|1
|2
|3
|4
|5
|6
|7
|8
|server {
|listen  80;
|server_name mimvp.com www.mimvp.com;
|root|/home/web|;
|if|( $host !~|"www\.mimvp\.com$"|) {
|rewrite ^(.*) http:|//www|.mimvp.com$1 permanent;
|}
|}
|

多个域名，指向同一个域名，米扑科技：[mimvp.com](http://mimvp.com/)
**域名解析配置：**
***  表示所有二级域名，例如： abc.mimvp.com,   blog.mimvp.com,  www.mimvp.com**
**@ 表示根域名，例如： mimvp.com**
**www 也是二级域名，同 abc.mimvp.com,   blog.mimvp.com 类似**
**mimvp.com 域名解析**
![nginx-redirect-redirecting-all-subdomains-to-the-www-01](http://cdn-blog.mimvp.com/wp-content/uploads/2016/07/nginx-redirect-redirecting-all-subdomains-to-the-www-01.png)
**ithomer.net 域名解析**
![nginx-redirect-redirecting-all-subdomains-to-the-www-02](http://cdn-blog.mimvp.com/wp-content/uploads/2016/07/nginx-redirect-redirecting-all-subdomains-to-the-www-02.png)
**mimvp.net 域名解析**
![nginx-redirect-redirecting-all-subdomains-to-the-www-03](http://cdn-blog.mimvp.com/wp-content/uploads/2016/07/nginx-redirect-redirecting-all-subdomains-to-the-www-03.png)
**Nginx 配置文件**：vim /etc/nginx/conf/nginx.conf
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|server {
|add_header Vary mimvp-home;
|listen       80;
|server_name  mimvp.com ithomer.net mimvp.net;
|if|( $host ~|"ithomer.net"|) {
|rewrite  ^(.*)    http:|//mimvp|.com permanent;
|}
|if|( $host ~|"mimvp.net"|) {
|rewrite  ^(.*)    http:|//mimvp|.com permanent;
|}
|....
|}
|

实例请见米扑科技，三个域名，都统一指向 mimvp.com
[http://mimvp.com](http://mimvp.com/)
[http://mimvp.net](http://mimvp.net/)
[http://ithomer.net](http://ithomer.net/)


