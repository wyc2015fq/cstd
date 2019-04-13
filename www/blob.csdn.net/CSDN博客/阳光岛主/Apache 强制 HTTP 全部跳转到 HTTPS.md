
# Apache 强制 HTTP 全部跳转到 HTTPS - 阳光岛主 - CSDN博客

2018年01月06日 01:14:42[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：17806


米扑博客最新写了一篇博客《[Apache 强制 HTTP 全部跳转到 HTTPS](https://blog.mimvp.com/article/20403.html)》，分享出来
更多经典技术博客，请见我的米扑博客：**https://blog.mimvp.com**

.htaccess 在每一层独立服务根目录下都存在，例如：
全部网站根目录为/var/www/html/.htaccess
米扑博客根目录位/var/www/html/mimvp-wordpress/.htaccess
米扑论坛根目录位/var/www/html/mimvp-discuz/.htaccess
米扑学习根目录位/var/www/html/mimvp-study/.htaccess
**HTTP 80 强制转 HTTPS**
全站采用https协议访问，所以需要http重定向到https，只需要在.htaccess加入下面规则
在相应的网站根目录新建**.htaccess**
**例如，在米扑博客的网站根目录下，新建**
vim/var/www/html/mimvp-wordpress/.htaccess
|1
|2
|3
|RewriteEngine On
|RewriteCond %{SERVER_PORT} 80
|RewriteRule ^(.*)$ https:|//|%{HTTP_HOST}/$1 [R,L]
|

或者
|1
|2
|3
|RewriteEngine On
|RewriteCond %{HTTPS} !=on
|RewriteRule ^(.*) https:|//|%{SERVER_NAME}/$1 [R,L]
|


**强制301重定向 HTTPS**
<IfModule mod_rewrite.c>
RewriteEngine on
RewriteBase /
RewriteCond %{SERVER_PORT} !^443$
RewriteRule (.*) https://%{SERVER_NAME}/$1 [R=301,L]
</IfModule>
**站点绑定多个域名**
只允许www.gworg.com 跳转
RewriteEngine On
RewriteCond %{SERVER_PORT} 80
RewriteCond %{HTTP_HOST} ^example.com [NC,OR]
RewriteCond %{HTTP_HOST} ^www.example.com [NC]
RewriteRule ^(.*)$ https://%{HTTP_HOST}/$1 [R,L]\#\#\#把网址更改为自己的\#\#\#
**高级用法 (可选）**
RewriteEngine on
\# 强制HTTPS
RewriteCond %{HTTPS} !=on [OR]
RewriteCond %{SERVER_PORT} 80
\# 某些页面强制
RewriteCond %{REQUEST_URI} ^something_secure [OR]
RewriteCond %{REQUEST_URI} ^something_else_secure
RewriteRule .* https://%{SERVER_NAME}%{REQUEST_URI} [R=301,L]
\# 强制HTTP
RewriteCond %{HTTPS} =on [OR]
RewriteCond %{SERVER_PORT} 443
\# 某些页面强制
RewriteCond %{REQUEST_URI} ^something_public [OR]
RewriteCond %{REQUEST_URI} ^something_else_public
RewriteRule .* http://%{SERVER_NAME}%{REQUEST_URI} [R=301,L]
**Apache mod_rewrite实现HTTP和HTTPS重定向跳转**
当你的站点使用了HTTPS之后，你可能会想把所有的HTTP请求（即端口80的请求），全部都重定向至HTTPS（即端口443）。这时候你可以用以下的方式来做到：（Apache mod_rewrite）
把这段代码放在.htaccess文件，即可实现HTTP到HTTPS的重定向。
|1
|2
|3
|4
|5
|6
|<IfModule mod_rewrite.c>
|RewriteEngine On
|RewriteBase /
|RewriteCond %{SERVER_PORT} 80
|RewriteRule ^(.*)$ https:|//blog|.mimvp.com/$1 [R=301,L]
|<|/IfModule|>
|

而当你又想用回HTTP的时候，反过来就可以了：
|1
|2
|3
|4
|5
|6
|<IfModule mod_rewrite.c>
|RewriteEngine On
|RewriteBase /
|RewriteCond %{SERVER_PORT} 443
|RewriteRule ^(.*)$ https:|//blog|.mimvp.com/$1 [R=301,L]
|<|/IfModule|>
|

其中R=301表示Moved Permanently，即告诉搜索引擎或者浏览器下去直接访问后者的地址，
如果只是试验性地重定向，可以使用R=302(Found)，临时跳转
更多30x状态，请见米扑博客：[HTTP协议中POST、GET、HEAD、PUT等请求方法总结](https://blog.mimvp.com/2014/04/http-protocol-in-the-post-get-head-put-and-other-request-methods/)
**VirtualHost 添加重定向**
实测以上方法，对于我的需求场景，都无效
我的项目场景：
1. 在我的根目录下 /var/www/htmp/
2. 配置有多个网站，如米扑博客（/var/www/htmp/mimvp-blog/）、米扑论坛（/var/www/htmp/mimvp-forum/）、米扑学习（/var/www/htmp/mimvp-study/）等
3. 对于米扑博客的http请求，全部定向到https博客；对于米扑论坛的http请求，全部定向到https论坛；
最后，解决方案是在 VirtualHost 节点里，添加如下配置：
RewriteEngine on
RewriteCond   %{HTTPS} !=on
RewriteRule   ^(.*)  https://%{SERVER_NAME}$1 [L,R]
完整配置参数如下：
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
|13
|14
|\# blog
|<VirtualHost *:80>
|ServerAdmin yanggang_2050@163.com
|DocumentRoot|/var/www/html/wordpress
|ServerName blog.mimvp.com
|RewriteEngine on
|RewriteCond   %{HTTPS} !=on
|RewriteRule   ^(.*)  https:|//|%{SERVER_NAME}$1 [L,R]
|DirectoryIndex index.php
|ErrorLog|/var/log/blog|.mimvp.com-error_log
|CustomLog|/var/log/blog|.mimvp.com-access_log common
|<|/VirtualHost|>
|

在米扑论坛、米扑学习等 VirtualHost 节点里，都添加如上配置，问题解决。
米扑博客效果，全部自动跳转到 https ：
[https://blog.mimvp.com](https://blog.mimvp.com/)
[https://blog.mimvp.com/about/](https://blog.mimvp.com/about/)


