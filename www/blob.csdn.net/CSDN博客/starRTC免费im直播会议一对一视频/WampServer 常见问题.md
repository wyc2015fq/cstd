# WampServer 常见问题 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年08月22日 13:44:07[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：48
Wamp就是Windows 下的Apache Mysql PHP集成环境。
支持phpmyadmin
注意：安装前请先安装VC 2010运行库（vcredist2010.exe ），否则会提示没有找到MSVCR100.dll而导致安装失败；
1，打开localhost时
Forbidden
You don't have permission to access / on this server.
修改httpd.conf
将<Directory "c:/wamp/www/"> 的
 Deny from all 
改为
  Allow from all
参考
[官方网站](http://www.wampserver.com/)
