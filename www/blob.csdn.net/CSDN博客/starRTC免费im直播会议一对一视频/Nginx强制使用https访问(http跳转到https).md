# Nginx强制使用https访问(http跳转到https) - starRTC免费im直播会议一对一视频 - CSDN博客
2018年02月01日 09:51:13[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：205
index.html
<html><meta http-equiv="refresh" content="0;url=https://www.starRTC.com/"></html>
server {    listen 80;    server_name www.starRTC.com;    location / {                #index.html放在虚拟主机监听的根目录下        root /www;    }        #将404的页面重定向到https的首页    error_page  404 https://www.starRTC.com/;}
