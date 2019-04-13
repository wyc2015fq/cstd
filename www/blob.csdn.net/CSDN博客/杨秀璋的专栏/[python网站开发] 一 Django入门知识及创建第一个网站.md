
# [python网站开发] 一.Django入门知识及创建第一个网站 - 杨秀璋的专栏 - CSDN博客

2017年06月27日 00:05:51[Eastmount](https://me.csdn.net/Eastmount)阅读数：4447


随着Python应用越来越广泛，学习Python相关知识越来越多的人选择。最新研究，Python已成为美国top高校最受欢迎的语言，同时国内用Python开发的网站越来越多，比较知名的网站如知乎、豆瓣、果壳等，再加上之前自己数据分析和网络爬虫一直使用的是Python，对它比较熟悉，所以这系列文章将讲述Python搭建网站，后面会结合数据分析可视化进行深入研究。
Python有许多款不同的Web框架，包括：Pylons、Tornado、Django、Bottle、Flask等，而Django是最有代表性的一个，也可认为是Python框架中最好的、文档最完善、市场占有率最高的框架，许多成功的网站和APP都基于Django技术的。
下面从入门开始讲解，同时Django相关资料也比较多，写得比我好的很多，而我这系列主要结合实例和自己的体验进行讲解，后面数据分析相关知识的加入也是这篇文章的升华，希望对你有所帮助，如果文章中存在错误或不足之处，还请海涵。

## 一. Django入门知识
Django是一个开放源代码的Web应用框架，由Python写成，Django采用了MVC的软件设计模式，即模型M，视图V和控制器C。
**百度百科-MVC**
MVC全名是Model View Controller，是模型(model)－视图(view)－控制器(controller)的缩写，一种软件设计典范，用一种业务逻辑、数据、界面显示分离的方法组织代码，将业务逻辑聚集到一个部件里面，在改进和个性化定制界面及用户交互的同时，不需要重新编写业务逻辑。MVC被独特的发展起来用于映射传统的输入、处理和输出功能在一个逻辑的图形化用户界面的结构中。常见框架：Struts、Spring等。

![](https://img-blog.csdn.net/20170626234843798)

**Django-MTV**
这部分内容直接引用博客园feixuelove大神的叙述，讲解得真的很好，强烈推荐阅读。
[简约而不简单的Django新手图文教程 - feixuelove](http://www.cnblogs.com/feixuelove1009/p/5823135.html)

MVC其实就是把不同类型的文件放到不同的目录下的一种方法，然后取了个高大上的名字。当然，它带来的好处有很多，比如前后端分离，松耦合等等。
1.模型(model)：定义数据库相关的内容，一般放在models.py文件中。
2.视图(view)：定义HTML等静态网页文件相关，那些html、css、js等前端的东西。
3.控制器(controller)：定义业务逻辑相关，就是你的主要代码。
MTV: 有些WEB框架觉得MVC的字面意思很别扭，就给它改了一下。view不再是HTML相关，而是主业务逻辑了，相当于控制器。html被放在Templates中，称作模板，于是MVC就变成了MTV。这其实就是一个文字游戏，和MVC本质上是一样的，换了个名字和叫法而已，换汤不换药。
目录分开，就必须有机制将他们在内里进行耦合。在Django中，urls、orm、static、settings等起着重要的作用。一个典型的业务流程是如下图所示：

![](https://img-blog.csdn.net/20170626235438468)

PS：这部分内容本来想自己写的，但是feixuelove大神写得更好，同时自己体会不是很深，所以直接引用了。相信自己做得多以后，能用自己的理解进行总结，如果该部分侵权，立刻删除。

## 二. 安装及配置Django
**PIP指令**
easy_insall的作用和perl中的cpan，ruby中的gem类似，提供了在线一键安装模块的傻瓜方便方式，而pip是easy_install的改进版，提供更好的提示信息，删除package等功能。常见的具体用法如下：

```python
easy_install的用法：  
1） 安装一个包  
 $ easy_install <package_name>  
 $ easy_install "<package_name>==<version>"  
2) 升级一个包  
 $ easy_install -U "<package_name>>=<version>"  
  
pip的用法  
1) 安装一个包  
 $ pip install <package_name>  
 $ pip install <package_name>==<version>  
2) 升级一个包 (如果不提供version号，升级到最新版本）  
 $ pip install --upgrade <package_name>>=<version>  
3）删除一个包  
 $ pip uninstall <package_name>
```
也在官网下载，下载地址：[https://www.djangoproject.com/](https://www.djangoproject.com/)
作者安装的版本：(pip list指令查看)
Python 2.7.8
Django 1.11.2

**安装Django**
通过cd去到Python的Scripts文件夹下，然后调用pip指定进行安装。

```python
pip install django
```
运行结果如下图所示，它在CMD环境下自动下载安装。
![](https://img-blog.csdn.net/20170626222917630)

安装成功后，在Scripts目录下存在django-admin.py的文件。

![](https://img-blog.csdn.net/20170626224040370)
在Scripts文件夹下输入: django-admin.py，查看命令的介绍。
![](https://img-blog.csdn.net/20170626224742108)
下面部分讲解如何创建一个Python网站项目。


## 三. 创建第一个项目及详解
**第一步 创建项目HelloWorld**
通过调用django-admin.py文件中的startproject命令进行安装。

```python
django-admin.py startproject HelloWorld
```
![](https://img-blog.csdn.net/20170626225801191)
然后在Scripts文件夹下生成项目HelloWorld，如下图所示。
![](https://img-blog.csdn.net/20170626230231927)
![](https://img-blog.csdn.net/20170626230314478)
![](https://img-blog.csdn.net/20170626230341441)

CMD中cd命令去到文件夹，dir命令显示目录下文件，tree命令显示路径。
![](https://img-blog.csdn.net/20170626230604341)

**第二步 runserver启动项目服务器**
接下来进入HelloWorld项目，并输入如下命令，启动项目服务器。
```python
python manage.py runserver 9090
```
其中9090为端口号，如Java网站的8080端口等。
![](https://img-blog.csdn.net/20170626231103261)
然后通过浏览器方位127.0.0.1:9090，显示It worked表示配置成功。
![](https://img-blog.csdn.net/20170626231212768)
**第三步 介绍项目的各个文件及作用**
参考RUNOOB网站对Django几个文件的进行介绍，目录如下：
```python
|-- HelloWorld
|   |-- __init__.py
|   |-- settings.py
|   |-- urls.py
|   `-- wsgi.py
|
`-- db.sqlite3
`-- manage.py
```
说明如下：
HelloWorld: 项目的容器。
manage.py:Django管理主程序。一个实用的命令行工具，可让你以各种方式与该 Django 项目进行交互。
__init__.py: 空文件。告诉Python该目录是一个Python包。
settings.py:主配置文件。该Django项目的设置/配置。
urls.py:URL路由文件。Django项目的URL声明，一份由Django驱动的网站"目录"。
wsgi.py:网络通信接口。一个WSGI兼容的Web服务器的入口，以便运行你的项目。
用Sublime打开如下所示：
![](https://img-blog.csdn.net/20170626232057607)

**第四步 创建视图views.py文件**
在HelloWorld目录下创建一个views.py文件，用于书写业务处理逻辑。
```python
#-*- coding:utf-8 -*-
from django.http import HttpResponse
 
def hello(request):
    return HttpResponse("Hello world, My name is Eastmount！")
```
自定义hello方法，参数request固定的，用于返回HttpResponse。
![](https://img-blog.csdn.net/20170626233237513)
**第五步 配置URL**
修改urls.py文件内容，将访问的内容改为views.py文件中的hello函数。
如下所示：

```python
"""HelloWorld URL Configuration
The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/1.11/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  url(r'^$', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  url(r'^$', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.conf.urls import url, include
    2. Add a URL to urlpatterns:  url(r'^blog/', include('blog.urls'))
"""
from django.conf.urls import url
from views import *
urlpatterns = [
    url(r'^$', hello),
]
```
代码需要导入views文件内容，才能引用hello方法。views.py视图文件在编译后会产生对应的views.pyc文件。然后启动Django服务器，在浏览器中访问即可，如下图所示：
![](https://img-blog.csdn.net/20170626234355293)

推荐资料：
[简约而不简单的Django新手图文教程 - feixuelove（强烈推荐）](http://www.cnblogs.com/feixuelove1009/p/5823135.html)
[Django创建第一个项目 - Runoob](http://www.runoob.com/django/django-first-app.html)
[https://www.djangoproject.com/](https://www.djangoproject.com/)
[Python Django入门教程 - 百度文库](https://wenku.baidu.com/view/14f45112f18583d0496459c3.html)

希望文章对你有所帮助，如果文章存在错误或不足之处，还请海涵。
加油，秀璋。绿妖，晚安！
(By:Eastmount 2017-06-27 晚上12点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))


