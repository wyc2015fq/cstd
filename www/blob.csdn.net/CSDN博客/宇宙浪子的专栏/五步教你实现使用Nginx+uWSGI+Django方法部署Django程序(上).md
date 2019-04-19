# 五步教你实现使用Nginx+uWSGI+Django方法部署Django程序(上) - 宇宙浪子的专栏 - CSDN博客
2016年03月29日 11:46:16[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：3274
转自：[http://www.django-china.cn/topic/101/](http://www.django-china.cn/topic/101/)
Django的部署可以有很多方式，采用nginx+uwsgi的方式是其中比较常见的一种方式。
![](http://www.th7.cn/Article/UploadFiles/200808/2008081305323204.jpg)
在这种方式中，我们的通常做法是，将nginx作为服务器最前端，它将接收WEB的所有请求，统一管理请求。nginx把所有静态请求自己来处理（这是NGINX的强项）。然后，NGINX将所有非静态请求通过uwsgi传递给Django，由Django来进行处理，从而完成一次WEB请求。
可见，uwsgi的作用就类似一个桥接器。起到桥梁的作用。
> 
NOTE：不使用nginx，只使用uwsgi+django也是可以实现WEB服务的。uwsgi也可以直接处理WEB请求。
为了完成上述的方式部署，我将分成两篇文章来分别进行阐述。
- 
第一步先解决uwsgi与django的桥接。解决在没有nginx的情况下，如何使用uwsgi+DJANGO来实现一个简单的WEB服务器。
- 
第二步解决uwsgi与Nginx的桥接。通过nginx与uwsgi的桥接，打通nginx与django的连通，从而比较完美的实现django的部署。
本文将分成五步来详细阐述uwsgi+django的部署方式。nginx+uwsgi+django的部署将在下一篇 文章中阐述。
本文大纲：
- 环境介绍
- 安装uwsgi
- 测试uwsgi
- 配置django
- 连接django和uwsgi，实现简单的WEB服务器。
## 环境介绍
- Ubuntu 12.04.1 LTS
- django 1.4.2
## 安装uwsgi
1.安装pip
可以参考这篇文章：[http://www.jsxubar.info/install-pip.html](http://www.jsxubar.info/install-pip.html)
2.安装uwsgi
$ export LDFLAGS="-Xlinker --no-as-needed"$ pip install uwsgi
## 测试uwsgi
在你的机器上写一个test.py
# test.py
def application(env, start_response):
    start_response('200 OK', [('Content-Type','text/html')])
    return "Hello World"
然后执行shell命令：
uwsgi --http :8001 --wsgi-file test.py
访问网页：
[http://127.0.0.1:8001/](http://127.0.0.1:8001/)
看在网页上是否有Hello World
## 配置django
> 
NOTE：
请保证你的django项目是正常使用的。可以使用
python manage.py runserver 0.0.0.0:8002
来测试一下你的django项目是否能正常跑起来。
请保证你的django程序已经关闭。
编写django_wsgi.py文件，将其放在与文件manage.py同一个目录下。
> 
注意： 编写文件时需要注意语句os.environ.setdefault。比如，如果你的项目为mysite，则你的语句应该是 
```
os.environ.setdefault("DJANGO_SETTINGS_MODULE",
 "mysite.settings")
```
|1 2 3 4 5 6 7 8 91011121314|#!/usr/bin/env python# coding: utf-8importosimportsys# 将系统的编码设置为UTF8reload(sys)sys.setdefaultencoding('utf8')os.environ.setdefault("DJANGO_SETTINGS_MODULE","mysite.settings")fromdjango.core.handlers.wsgiimportWSGIHandlerapplication=WSGIHandler()|
## 连接django和uwsgi，实现简单的WEB服务器。
我们假设你的Django项目的地址是/home/work/src/sites/testdjango1/testdjango/mysite，
然后，就可以执行以下命令：
uwsgi --http :8000 --chdir /home/work/src/sites/testdjango1/testdjango/mysite --module django_wsgi
这样，你就可以在浏览器中访问你的Django程序了。所有的请求都是经过uwsgi传递给Django程序的。
## 最后：
关于如何将uwsgi与Nginx连接，可以期待下篇文章 《五步教你实现使用Nginx+Uwsgi+Django方法部署Django程序(下)》
最后面，请大家要支持[Django中国社区](http://django-china.cn/)哦，单靠一两个人是不行的，一起推广一下，让Django社区更有力量哈！更有人气哈！
推广链接： [http://django-china.cn/](http://django-china.cn/)
## 参考、解释及其它
- 
wsgi: WSGI是一种Web服务器网关接口。它是一个Web服务器（如nginx）与应用服务器（如uWSGI服务器）通信的一种规范。
关于WSGI协议看这里：[WSGI](http://wsgi.readthedocs.org/en/latest/)
- 
uWSGI: [http://uwsgi-docs.readthedocs.org/en/latest/index.html](http://uwsgi-docs.readthedocs.org/en/latest/index.html) uWSGI是一个Web服务器，它实现了WSGI协议、uwsgi、http等协议。
 Nginx中HttpUwsgiModule的作用是与uWSGI服务器进行交换。
- 
uwsgi: uwsgi同WSGI一样是一种通信协议，而uWSGI是实现了uwsgi和WSGI两种协议的Web服务器。
uwsgi协议是一个uWSGI服务器自有的协议，它用于定义传输信息的类型（type of information），每一个uwsgi packet前4byte为传输信息类型描述，它与WSGI相比是两样东西。
关于uwsgi协议看这里：[The uwsgi protocol](http://projects.unbit.it/uwsgi/wiki/uwsgiProtocol)
- 
有了uWSGI为什么还需要nginx？
nginx具备优秀的静态内容处理能力，然后将动态内容转发给uWSGI服务器，这样可以达到很好的客户端响应。
- 
参考文献：[http://heipark.iteye.com/blog/1750970](http://heipark.iteye.com/blog/1750970)
## END
*Last edited by Django中国社区 (2013-05-12 23:35)*
