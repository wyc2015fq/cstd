# python快速开发Web之Django - _天枢 - 博客园
## [python快速开发Web之Django](https://www.cnblogs.com/yhleng/p/7484880.html)
2017-09-06 15:08 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7484880)
**以前写测试框架，要么是纯python代码驱动的，要么是WinForm界面的框架，多人操作并不适合。**
**就想用python写个Web版的，于是想到了Web快速开发的框架Flask和Django两个**
**个人觉得Django更快一些，后台管理，用户管理都不需要自己写了，虽然相较于Flask来说Django限制要多些，但是没关系**
**Django足够满足需求了。果断选Django**
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
**环境：python2.7**
**安装：pip install Django**
**成功后在Pthon\Scripts下会有django-admin.py等**
**确认环境变量PATH可以执行django-admin(不设置那么，每次要进入所在路径执行)**
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
ok,准备功作完毕，开始创建第一个项目
**1、创建项目**
**django-admin.exe startproject mysite**
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170906135748616-201502681.png)
执行完成，命令行下，没有返回消息，可以到命令行所在目录看下项目创建成功没
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170906140054522-1710206964.png)
**startproject命令创建的项目下，包括了三个文件**
**manage.py文件是同django一起工作的工具。**
**settings.py文件包含项目位置，数据库信息，调试标志，和一些其它重要信息。**
**urls.py文件它是url映射到你应用程序里的配置文件。是django里非常强大的一个特性。**
2、启动web服务
**manage.py runserver**
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170906140621647-1108154042.png)
**如果出现start develoment server at http://127.0.0.1:8000这说明启动成功了。**
**可以通过浏览器访问这个ip会看到'It worked'页面我们就可以进行下边开发了。**
3、创建app应用
**命令行到testProject目录下执行**
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170906140855804-1213910501.png)
**manage.py startapp blog**
**blog是应用名字，自己决定**
*![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170906140927616-596086252.png)*
***－－－－－－－－－－－－－到这里，项目创建完成，下面可以进行开发了－－－－－－－－－－－－－－－－－－－***
** 这里使用的python开发环境pycharm**
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170906141127601-1317660970.png)
** 1、将应用加到项目中来Settings.py**
**INSTALLED_APPS设置来告诉Django app也是项目的一部分。**
```
INSTALLED_APPS = [
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
    'blog', #将应用加入
    'bootstrap3', #这个是html样式pip可以安装
]
```
**setting.py中配置数据库，sqlite3小型数据库，类似Access数据库**
**默认也是这个。如果使用sqlite3数据库不需要修改**
```
DATABASES = {
    'default': {
        'ENGINE': 'django.db.backends.sqlite3',
        'NAME': os.path.join(BASE_DIR, 'db.sqlite3'),
    }
}
```
**2、设计Model**
**models.py这是定义数据结构的地方；是Django操作数据库的地方**
```
# -*- coding: utf-8 -*-
from __future__ import unicode_literals
from django.db import models
from django.contrib import admin
# Create your models here.
class blogPost(models.Model):
    title = models.CharField(max_length=150)
    body = models.TextField()
    timestamp = models.DateTimeField()
class blogPostAdmin(admin.ModelAdmin):
    list_display = ('title','timestamp')
admin.site.register(blogPost,blogPostAdmin)
#存储测试用例表
class caseInfo(models.Model):
    step = models.AutoField  #步骤序号
    desc = models.CharField(max_length=150) #描述
    url = models.URLField(max_length=200) #url
    method = models.CharField(max_length=8) #方法 post or get
    header = models.TextField() #头信息
    data = models.TextField() #数据
    date = models.DateTimeField() #日期时间
    exetime = models.DateTimeField() #执行时间，执行时动态更新
    exeresult = models.CharField(max_length=8) #执行结果
    exeresponse = models.TextField() #影响信息
#后台，显示的字段，也就是/admin/登录的后台
class caseAdmin(admin.ModelAdmin):
    list_display = ('desc','url','method','header','data','exetime','exeresult','exeresponse')
admin.site.register(caseInfo,caseAdmin) #将表信息注册到后台
```
**在命令行执行，创建测试用例信息表**
**manage.py syncdb**
***如果你使用syncdb报错“未知命令”不要着急，这是由于Django 1.9版本以上命令换成了migrate***
**要先同步数据库：manage.py makemigrations 要不然数据库中不会有表**
**manage.py migrate创建成功，会有很多的ok**
**可以下载工具查看数据库：SQLiteSpy.exe**
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170906143316397-1648898052.png)
**如果models.py中有更改，比如增加了新字段，都需要再次执行上边的两个命令makemigrations和*migrate***
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
**创建好model后，要登录后台去录入数据，那么我们需要创建个超级管理员帐户才行**
**创建超级用户：**
**manage.py createsuperuser**
**设置用户名，密码**
**然后登录：http://127.0.0.1:8000/admin/**
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170906144039429-762329724.png)
**Add增加信息**
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170906144123866-266544800.png)
**增加后，在数据库中可以查到增加信息。**
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
**要实现如下界面：**
![](https://images2017.cnblogs.com/blog/1149221/201709/1149221-20170906144410366-1566333330.png)
**1、首先在app下建一个模版文件夹migrations  这个名子在Django是固定的不能改**
**     在这下边创建html页面**
```
<html>
    <style type="text/css">
        #box-table-a
        {
            font-family: "Lucida Sans Unicode", "Lucida Grande", Sans-Serif;
            font-size: 12px;
            margin: 45px;
            width: 1240px;
            text-align: left;
            border-collapse: collapse;
        }
        #box-table-a th
        {
            font-size: 13px;
            font-weight: normal;
            padding: 8px;
            background: #b9c9fe;
            border-top: 4px solid #aabcfe;
            border-bottom: 1px solid #fff;
            color: #039;
        }
        #box-table-a td
        {
            padding: 8px;
            background: #e8edff;
            border-bottom: 1px solid #fff;
            color: #669;
            border-top: 1px solid transparent;
        }
        #box-table-a tr:hover td
        {
            background: #d0dafd;
            color: #339;
        }
    </style>
    <head>
        {% load bootstrap3 %}
        {% bootstrap_css %}
        {% bootstrap_javascript %}
        <title>IABS online</title>
    </head>
    <body>
        <!-- 导航栏-->
        <nav class="navbar navbar-inverse navbar-fixed-top">
            <div class="container">
                <div class="navbar-header">
                    <a class="navbar-brand" href="/event_manage/">IATS接口自动化测试系统</a>
                </div>
                <div id="navbar" class="collapse navbar-collapse">
                    <ul class="nav navbar-nav">
                        <li class="active"><a href="#">用例管理</a></li>
                        <li><a href="/guest_manage/">测试报告</a></li>
                    </ul>
                    <ul class="nav navbar-nav navbar-right">
                        <li><a href="#">{{user}}</a></li>
                        <li><a href="/logout/">退出</a></li>
                    </ul>
                </div>
            </div>
        </nav>
        </br>
        <!--table -->
        <table id="box-table-a" summary="Test Case List">
            <thead>
                <tr>
                    <th scope="col">Step</th>
                    <th scope="col">Description</th>
                    <th scope="col">Url</th>
                    <th scope="col">Method</th>
                    <th scope="col">Headers</th>
                    <th scope="col">Data</th>
                    <!--
                    <th scope="col">Response</th>
                    -->
                    <th scope="col">Results</th>
                    <th scope="col">Execute Time</th>
                    <th scope="col">Execute</th>
                </tr>
            </thead>
            <tbody>
                {% for post in posts %}
                    <tr>
                        <td><p>{{post.id}}</p></td>
                        <td><p>{{post.desc}}</p></td>
                        <td><p>{{post.url}}</p></td>
                        <td><p>{{post.method}}</p></td>
                        <td><p>{{post.header}}</p></td>
                        <td><p>{{post.data}}</p></td>
                        <!--
                        <td><p>{{post.exeresponse}}</p></td>
                        -->
                        <td><p>{{post.exeresult}}</p></td>
                        <td><p>{{post.exetime}}</td>
                        <td>
                            <form method="post">
                                <input name="id" type="hidden" value="{{post.id}}" />
                                <input name="url" type="hidden" value="{{post.url}}" />
                                <input name="method" type="hidden" value="{{post.method}}" />
                                <input name="header" type="hidden" value="{{post.header}}" />
                                <input name="data" type="hidden" value="{{post.data}}" />
                                <input name='btn{{post.id}}' type="submit" value="执  行"/>
                                {% csrf_token %}
                            </form>
                        </td>
                    </tr>
                {% endfor %}
            </tbody>
        </table>
    </body>
</html>
View Code
```
**views.py**
**接收页面请求，验证响应信息code**
```
# -*- coding: utf-8 -*-
from __future__ import unicode_literals
from django.shortcuts import render
from django.http import HttpResponse,request,HttpResponseRedirect
from models import blogPost
from models import caseInfo,statusInfo
from requests import Session
import json
# Create your views here.
def index(request):
    #posts = blogPost.objects.all()
    posts = caseInfo.objects.all()
    status = statusInfo.objects.all()
    if request.method=="POST":   #执行代码
        pid = request.POST.get('id')
        url = request.POST.get('url')
        method = request.POST.get('method')
        header = request.POST.get('header')
        postdata = request.POST.get('data')
        reStr = Session().post(url,data=eval(postdata))
        if reStr.json()['code'] =='7011':
            caseInfo.objects.filter(id=pid).update(exeresult='FAIL')
        else:
            caseInfo.objects.filter(id=pid).update(exeresult='PASS')
        return render(request,"index.html",{"posts":posts,})
    elif request.method =="GET":
        return render(request,"index.html",{"posts":posts,})
```
**url.py**
```
from django.conf.urls import url
from django.contrib import admin
from blog import views
urlpatterns = [
    url(r'^admin/', admin.site.urls),
    url(r'^index/$', views.index),
]
```
** 代码到这基本完成了；当然还需完善；**
**点击，执行会将当前行的数据，传到后台，进行发送请求做处理…………**
**后续完善在更新…………**
