# 基于markdown的blog系统调研1：typecho - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月13日 18:35:45[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：200
[http://typecho.org/download](http://typecho.org/download)
[https://github.com/typecho/typecho](https://github.com/typecho/typecho)
A PHP Blogging Platform. 
7 张数据表
不足 400KB 的代码
**安装环境要求**
- PHP 5.2 以上
- MySQL, PostgreSQL, SQLite 任意一种数据库支持，并在 PHP 中安装了相关扩展
- CURL 扩展支持
- mbstring 或 iconv 扩展支持
安装文档
[http://docs.typecho.org/doku.php](http://docs.typecho.org/doku.php)
安装注意事项：
chmod -R 777 typecho/ 需要写config.ini.php
然后数据库要自行先建立好
首页：
[http://demo.elesos.com/typecho/index.php](http://demo.elesos.com/typecho/index.php)
Nginx服务器无法登录后台，点击前台链接或者后台登录时出现"404, not found"
垃圾一个，配置半天用不了！！！
最后换成httpd了
非80端口：
vim typecho/var/Typecho/Request.php
**getUrlPrefix方法中**
array(80, 443,
8080))
