
# Nginx 利用代理转发请求实例 - 阳光岛主 - CSDN博客

2018年03月09日 18:51:58[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：2654


我们[米扑科技](http://mimvp.com/)的业务遍布全球，有时国外的客户无法访问我们搭建在国内的服务，这就要求我们在国外服务器上搭建一个代理转发服务，用户请求国外服务器的域名，然后代理转发到国内，处理请求后返回结果给国外的客户。Nginx 不仅可以实现负载均衡，还可以实现反向代理转发，非常适合我们的使用场景，因此我们[米扑科技](https://mimvp.com/)采用 Nginx 利用代理转发请求实例。
米扑博客原文：[Nginx 利用代理转发请求实例](https://blog.mimvp.com/article/22927.html)
**Nginx 利用代理转发请求实例**
nginx 作为目前最流行的web服务器之一，可以很方便地实现反向代理。
nginx 反向代理官方文档:[NGINX REVERSE PROXY](https://www.nginx.com/resources/admin-guide/reverse-proxy/)
当在一台主机上部署了多个不同的web服务器，并且需要能在80和443端口同时访问这些web服务器时，可以使用 nginx 的反向代理功能，用 nginx 在80端口监听所有请求，并转发给443端口，再由443端口统一依据转发规则转发到对应的国内web服务器上。
**示例：**
[proxy.mimvp.com](http://proxy.mimvp.com/)域名在阿里云备案，部署在国内的北京阿里云服务器
[proxy.mimgu.cn](http://proxy.mimgu.cn/)域名在腾讯云备案，部署在国外的新加坡腾讯云服务器（国外域名可不备案）
**1. 国内北京阿里云服务器**（[proxy.mimvp.com](http://proxy.mimvp.com/)）
**vim mimvp_proxy.conf**
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
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|52
|53
|54
|55
|56
|57
|58
|59
|60
|61
|62
|63
|64
|65
|66
|67
|68
|69
|70
|71
|72
|73
|74
|75
|76
|77
|78
|79
|80
|81
|82
|83
|84
|85
|86
|87
|88
|89
|90
|91
|92
|93
|94
|95
|96
|97
|98
|99
|server {
|listen|80|;
|server_name   proxy.mimvp.com;|\# 域名可以有多个，用空格隔开
|root      www|/|mimvp_proxy;
|rewrite ^(.|*|)$  https:|/|/|$host$|1|permanent;
|location|/|{
|root        www|/|mimvp_proxy;
|index       index.php index.html index.htm;
|}
|location ~ \.php$ {
|root                www|/|mimvp_proxy;
|fastcgi_pass|127.0|.|0.1|:|9000|;
|fastcgi_index   index.php;
|fastcgi_param   SCRIPT_FILENAME|/|scripts$fastcgi_script_name;
|include         fastcgi.conf;
|}
|location ~|*|^|/|(images|img|javascript|js|css|blog|flash|media|static)|/|{
|root        www|/|mimvp_proxy;
|expires|30d|;
|}
|location ~|*|^|/|(robots\.txt) {
|root        www|/|mimvp_proxy;
|expires|365d|;
|}
|location ~|*|^|/|favicon\.ico {
|root        www|/|mimvp_proxy;
|expires|365d|;
|}
|location ~|*|^|/|img|/|logo\.png {
|root        www|/|mimvp_proxy;
|expires|365d|;
|}
|location ~|/|\.ht {
|deny|all|;
|}
|}

|server {
|listen|443|ssl http2;
|server_name     proxy.mimvp.com;|\# 域名可以有多个，用空格隔开
|root            www|/|mimvp_proxy;
|ssl             on;
|ssl_certificate      proxy.mimvp.com.crt;
|ssl_certificate_key  proxy.mimvp.com.key;
|ssl_session_cache    shared:SSL:|1m|;
|ssl_session_timeout|10m|;
|ssl_protocols TLSv1 TLSv1.|1|TLSv1.|2|;
|ssl_ciphers ECDHE|-|RSA|-|AES128|-|GCM|-|SHA256:HIGH:!aNULL:!MD5:!RC4:!DHE;
|ssl_prefer_server_ciphers on;
|location|/|{
|root        www|/|mimvp_proxy;
|index       index.php index.html index.htm;
|}
|location ~ \.php$ {
|root                www|/|mimvp_proxy;
|fastcgi_pass|127.0|.|0.1|:|9000|;
|fastcgi_index   index.php;
|fastcgi_param   SCRIPT_FILENAME|/|scripts$fastcgi_script_name;
|include         fastcgi.conf;
|}
|location ~|*|^|/|(images|img|javascript|js|css|blog|flash|media|static)|/|{
|root        www|/|mimvp_proxy;
|expires|30d|;
|}
|location ~|*|^|/|(robots\.txt) {
|root        www|/|mimvp_proxy;
|expires|365d|;
|}
|location ~|*|^|/|favicon\.ico {
|root        www|/|mimvp_proxy;
|expires|365d|;
|}
|location ~|*|^|/|img|/|logo\.png {
|root        www|/|mimvp_proxy;
|expires|365d|;
|}
|location ~|/|\.ht {
|deny|all|;
|}
|}
|


**2. 国外新加坡腾讯云服务器**（[proxy.mimgu.cn](http://proxy.mimgu.cn/)）
**vim mimvp_proxy2.conf**
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
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|52
|53
|server {
|listen|80|;
|server_name   proxy.mimgu.cn;
|root      www|/|mimvp_proxy;
|rewrite ^(.|*|)$  https:|/|/|$host$|1|permanent;|\#\# 全部请求强制跳转到 https
|}

|server {
|listen|443|ssl http2;
|server_name     proxy.mimgu.cn;
|root            www|/|mimvp_proxy;
|ssl             on;
|ssl_certificate      proxy.mimgu.cn.crt;
|ssl_certificate_key  proxy.mimgu.cn.key;
|ssl_session_cache    shared:SSL:|1m|;
|ssl_session_timeout|10m|;
|ssl_protocols TLSv1 TLSv1.|1|TLSv1.|2|;
|ssl_ciphers ECDHE|-|RSA|-|AES128|-|GCM|-|SHA256:HIGH:!aNULL:!MD5:!RC4:!DHE;
|ssl_prefer_server_ciphers on;
|location|/|{
|proxy_pass          https:|/|/|proxy.mimvp.com;|\#\# 全部请求强制跳转到国内服务器 https://proxy.mimvp.com
|}
|location ~ \.php$ {
|proxy_pass          https:|/|/|proxy.mimvp.com;
|}
|location ~|*|^|/|(images|img|javascript|js|css|blog|flash|media|static)|/|{
|proxy_pass          https:|/|/|proxy.mimvp.com;|\#\# css,js,img 等全部要强制转发，否则可能排版错乱
|}
|location ~|*|^|/|(robots\.txt) {
|proxy_pass          https:|/|/|proxy.mimvp.com;
|}
|location ~|*|^|/|favicon\.ico {
|proxy_pass          https:|/|/|proxy.mimvp.com;
|}
|location ~|*|^|/|img|/|logo\.png {
|proxy_pass          https:|/|/|proxy.mimvp.com;
|}
|location ~|/|\.ht {
|deny|all|;
|}
|}
|

**3. 查看效果**
[http://proxy.mimvp.com](http://proxy.mimvp.com/)// 强制跳转到  https://proxy.mimvp.com
[https://proxy.mimvp.com](https://proxy.mimvp.com/)// 对外显示的唯一域名网址
[http://proxy.mimgu.cn](http://proxy.mimgu.cn/)// 强制跳转到  https://proxy.mimgu.cn，然后继续强制跳转到 https://proxy.mimvp.com
[https://proxy.mimgu.cn](https://proxy.mimgu.cn/)// 强制跳转到  https://proxy.mimvp.com
**小结**
1. 四个域名，统一跳转指向唯一的服务[https://proxy.mimvp.com](https://proxy.mimvp.com/)
2. http 实现了强制跳转到 https ，整个网站使用ssl加密
3. 国外的腾讯云域名（[proxy.mimgu.cn](https://proxy.mimgu.cn/)），nginx代理转发到了国内阿里云域名（[proxy.mimvp.com](https://proxy.mimvp.com/)）
4. 上面nginx配置文件，是一个经典的配置，信息量大，值得认真学习[米扑科技](https://mimvp.com/)的匠心服务

**参考推荐：**
[Nginx Redirect重定向所有子域名到www](https://blog.mimvp.com/article/13312.html)
[Nginx 配置文件禁止访问目录或文件](https://blog.mimvp.com/article/18435.html)
[Nginx屏蔽访问过于频繁的IP](https://blog.mimvp.com/article/12970.html)
[Mac OS X 安装 Nginx](https://blog.mimvp.com/article/17317.html)


