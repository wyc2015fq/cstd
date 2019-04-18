# 【Python】后端 - YZXnuaa的博客 - CSDN博客
2018年02月23日 16:35:45[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：515
1.代码
import os, re, urllib, requests
import urllib.request as urllib2
from tornado import httpserver, web, ioloop
class MainPageHandler(web.RequestHandler):
    def get(self, *args, **kwargs):
        #self.write('欢迎！')
self.render('index.html')
# 登录模块
class LoginHandler(web.RequestHandler):
    def get(self, *args, **kwargs):
        self.render('login.html')
settings = {
    # 设置模板路径
'templates_path': 'templates',
# 设置静态文件
'static_path': 'static',
}
# 域名的分机号，内部的URL
application = web.Application([
    (r'/index', MainPageHandler),
(r'/login', LoginHandler),
], **settings)  # 关联settings设置
if __name__ == '__main__':
    # socket 前台
http_server = httpserver.HTTPServer(application)
    http_server.listen(8080)
    ioloop.IOLoop.current().start()
