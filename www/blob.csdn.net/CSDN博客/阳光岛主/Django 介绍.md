
# Django 介绍 - 阳光岛主 - CSDN博客

2013年11月22日 00:06:07[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：4917所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)



**[Django](https://www.djangoproject.com/)**，是一个开放源代码的Web应用框架，由Python写成，采用了MVC的软件设计模式，即模型M，视图V，控制器C。它最初是被开发来用于管理劳伦斯出版集团旗下的一些以新闻内容为主的网站的，并于2005年7月在BSD许可证下发布。这套框架是以比利时的吉普赛爵士吉他手DjangoReinhardt来命名的。
Django的主要目标是使得开发复杂的、数据库驱动的网站变得简单，注重组件的重用性和“可插拔性”，敏捷开发和DRY法则（Don't Repeat Yourself）。在Django中Python被普遍使用，甚至包括配置文件和数据模型，于2008年6月17日正式成立基金会。

**核心组件**
Django框架的核心包括：
一个面向对象的映射器，用作数据模型（以Python类的形式定义）和关联性数据库间的媒介；
一个基于正则表达式的URL分发器；
一个视图系统，用于处理请求；
一个模板系统。
核心框架中还包括：
一个轻量级的、独立的Web服务器，用于开发和测试。
一个表单序列化及验证系统，用于HTML表单和适于数据库存储的数据之间的转换。
一个缓存框架，并有几种缓存方式可供选择。
中间件支持，允许对请求处理的各个阶段进行干涉。
内置的分发系统允许应用程序中的组件采用预定义的信号进行相互间的通信。
一个序列化系统，能够生成或读取采用XML或JSON表示的Django模型实例。
一个用于扩展模板引擎的能力的系统。
**内置应用**
Django 包含了很多应用在它的"contrib"包中，这些包括：
一个可扩展的认证系统
动态站点管理页面
一组产生RSS和Atom的工具
一个灵活的评论系统
产生Google站点地图（Google Sitemaps）的工具
防止跨站请求伪造（cross-site request forgery）的工具
一套支持轻量级标记语言（Textile和Markdown）的模板库
一套协助创建地理信息系统（GIS）的基础框架
**服务器部署**
Django可以运行在启用了mod python的Apache 2上，或是任何WSGI兼容的Web服务器。Django也有启动FastCGI服务的能力，因此能够应用于任何支持FastCGI的机器上，Django1.0已经可以利用Jython运行在任何J2EE服务器。
下列数据库引擎被Django官方支持：
PostgreSQL
MySQL
SQLite
Oracle
Microsoft SQL Server的适配器正在开发中，处于试验阶段。（注：SQL Server的支持在1.0版本中已经被完全去除）

**Django 安装**
Django is available open-source under the[BSD license](https://github.com/django/django/blob/master/LICENSE). It requires Python version 2.6.5 or higher, but it has no dependencies on other Python libraries. There are several ways you can get it:
## Option 1. Get the latest official version
The latest official version is 1.6.
You can install it with[pip](http://www.pip-installer.org/en/latest/):
`pip install Django==1.6`You can also get it by direct download:
First, download[Django-1.6.tar.gz](https://www.djangoproject.com/download/1.6/tarball/)([checksums](https://www.djangoproject.com/download/1.6/checksum/)). Then:
`tar xzvf Django-1.6.tar.gz
cd Django-1.6
sudo python setup.py install`
## Option 2. Get the latest development version
The latest and greatest Django version is the one that's in our Git repository (our revision-control system). Get it using this shell command, which requires[Git](http://git-scm.com/):
`git clone https://github.com/django/django.git`
You can also download[a zipped archive](https://github.com/django/django/zipball/master)of the development version.
## After you get it
See the[installation guide](https://docs.djangoproject.com/en/stable/intro/install/)for further instructions. Make sure you read the documentation that corresponds to the version of Django you've just installed.
And be sure to sign up for the[django-users mailing list](https://www.djangoproject.com/r/django-users/), where other Django users and the Django developers themselves all hang out to help each other.

**Django 安装验证：**
安装完毕Django后，验证是否成功：
root@ubuntu:/\# python
>>> import django
>>> django.get_version()
'1.6'

**Django 创建工程**
\#django-admin.py startproject mysite
\#tree mysite/
mysite/
├── manage.py
└── mysite
├── __init__.py
├── settings.py
├── urls.py
└── wsgi.py

**运行 mysite**
\#python manage.py runserver
Validating models...
0 errors found
November 21, 2013 - 12:22:12
Django version 1.6, using settings 'mysite.settings'
Starting development server athttp://127.0.0.1:8000/
Quit the server with CONTROL-C.

访问[http://127.0.0.1:8000/](http://127.0.0.1:8000/)，浏览器如下图：
![](https://img-blog.csdn.net/20131121202400171)



**参考推荐：**
[Django 官网](https://www.djangoproject.com/)
[Django 源码](https://github.com/django/django)（github）
[Django 博客专栏](http://blog.csdn.net/column/details/hellodjango.html)（推荐）


