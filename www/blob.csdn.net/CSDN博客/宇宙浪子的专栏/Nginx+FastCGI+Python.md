# Nginx+FastCGI+Python - 宇宙浪子的专栏 - CSDN博客
2014年09月05日 15:46:56[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：565
网上更多文章是用Django、webpy等框架的前提下，由于我只是用其CGI而不是搞纯web开发，所以暂时不打算用这类框架，直接用flup包（其实大多数框架WSGI也是封装的flup）。
废话不多说，进入正题，先看下需要的环境和工具：
> 
1、Linux2.6（废话- -！）
2、Nginx（需包含fastcgi模块）
3、Spawn-fcgi （官方下载：[http://redmine.lighttpd.net/projects/spawn-fcgi/wiki](http://redmine.lighttpd.net/projects/spawn-fcgi/wiki)）
4、Python
5、flup（官方下载：[http://trac.saddi.com/flup](http://trac.saddi.com/flup)）
安装都是些常规方法，就不一一介绍了
nginx的fastcgi配置照搬php的即可。
先上张简单画的原理图
![](http://hi.csdn.net/attachment/201009/8/0_12839166559zGP.gif)
spawn-fcgi是个工具，用来以fastcgi方式开启多个WSGIServer进程。
例：
> 
spawn-fcgi -f /data/WSGIServer.py -a 127.0.0.1 -p 5678 -u www -F 5
-f WSGIServer脚本的决定路径
-a fastcgi绑定的ip
-p fastcgi绑定的端口
-u 已什么身份运行
-F 要启动的进程数（很多人以为是-C，其实那是PHP专用的，这里要用-F）
需要编码的部分就是WSGIServer这里了，贴个简单测试代码
- #!/usr/bin/env python
- # -*- coding: utf-8 -*-
- import flup.server.fcgi as flups  
- 
- def application(environ, start_response):  
- """该函数可以写成模块import导入"""
-     ret = ""  
- try:  
-         uri = environ['PATH_INFO']  
- if uri[-1] == "/":  
-             uri = uri[:-1]  
- if uri == "":  
-             ret = str(environ)  
- elif uri == "/sleep":  
- import time  
-             time.sleep(5)  
-             ret = "sleep: 5 secends"
- else:  
-             ret = uri  
- except Exception, e:  
-             ret = str(e)  
-             status = '200 OK'
-     response_headers = [('Content-type','text/plain')]  
-     start_response(status, response_headers)  
- return [ret]  
- 
- if __name__ == "__main__":  
- #直接用python运行
- #flups.WSGIServer(application, multithreaded=True, multiprocess=False, bindAddress=('127.0.0.1', 5678)).run()
- #fastcgi方式运行
-     flups.WSGIServer(application).run()  
- 
