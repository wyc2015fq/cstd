# Nginx实战（一） 虚拟主机 - 菜鸟路上的小白 - CSDN博客





2016年10月11日 12:28:02[somnus_小凯](https://me.csdn.net/u012486840)阅读数：7777








- 利用虚拟主机，不同为每个要运行的网站提供一台单独的Nginx服务器或单独运行一组Nginx进程。虚拟主机提供了在同一台服务器、同一组Nginx进程上运行多个网站的功能。


在Nginx配置文件nginx.conf中，一个最简化的虚拟主机配置代码如下
- server {
-   listen       80;
-   server_name  localhost;
-   access_log  logs/host.access.log  main;
-   location /{
-      root   html;
-      index  index.html index.htm;
- }
- }

## 基于IP的虚拟主机

可以在一块物理网卡上绑定多个lP地址。这样就能够在使用单一网卡的同一个服务器上运行多个基于IP的虚拟主机。设置IP别名也非常容易，只须配置系统上的网络接口，让它监听额外的lP地址。
- server {
-     listen      192.168.204.131:80;
-     server_name example.org www.example.lianggzone.com;
-     root /data/www;
- }
- 
- server {
-     listen      192.168.204.132:80;
-     server_name example.net www.example.lianggzone.com;
-     root /data/bbs;
- }

## 基于域名的虚拟主机

基于域名的虚拟主机是最常见的一种虚拟主机。只需配置你的DNS服务器，将每个主机名映射到正确的lP地址，然后配置Nginx服务器，令其识别不同的主机名就可以了。这种虚拟主机技术，使很多虚拟主机可以共享同一个lP地址，有效解决了lP地址不足的问题。所以，如果没有特殊要求使你必须用一个基于lP的虚拟主机，最好还是使用基于域名的虚拟主机。

编辑/etc/hosts加入虚拟域名以便解析。
- cat /etc/hosts

编辑内容如下
- 127.0.0.1 www.blog.lianggzone.com www.bbs.lianggzone.com

修改Nginx配置文件nginx.conf，添加虚拟域名支持
- server {
-     listen      80;
-     server_name www.blog.lianggzone.com;
-     location /{
-       root   /usr/local/dev/nginx/page;
-       index  index.html;
- }
- }
- 
- server {
-     listen      80;
-     server_name www.bbs.lianggzone.com;
-     location /{
-       root   /usr/local/dev/nginx/page;
-       index  index2.html;
- }
- }

## 基于端口的虚拟主机

基于端口的虚拟主机配置，使用端口来区分，浏览器使用域名或ip地址:端口号访问。
- server {
-     listen 8080;
-     server_name www.blog.lianggzone.com;
-     root /usr/local/dev/nginx/page;
- }
- 
- server {
-     listen 9090;
-     server_name www.bbs.lianggzone.com;
-     root /usr/local/dev/nginx/page;
- }



