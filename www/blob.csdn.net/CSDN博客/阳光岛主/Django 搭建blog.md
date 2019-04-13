
# Django 搭建blog - 阳光岛主 - CSDN博客

2013年11月25日 22:15:40[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：6316所属专栏：[Python 学习入门](https://blog.csdn.net/column/details/python-learning.html)




## 1.新建Django项目

eclipse ——》File——》New ——》PyDev ——》PyDev Django Project
![](https://img-blog.csdn.net/20131125131117546)

如果提示无法找到了Django，请添加Django到Python编译环境中，步骤：
eclipse ——》 window ——》 Preferences ——》 PyDev ——》Interpreter - Python，在弹出对话框的下一个对话框，点击”New Folder“——》 引入解压的Django目录，如： /opt/Django-1.6/django/
如果eclipse无法安装PyDev，或者安装完了找不到PyDev，请安装[aptana](http://www.aptana.com/products/studio3)，它里面含有PyDev

选择sqlite数据库
![](https://img-blog.csdn.net/20131125131248687)

如果没有安装，Ubuntu下安装命令：sudo apt-get install sqlite3

## 2.创建博客模块app
右键项目mysite ——》 Django ——》 Create application （manage.py startup），弹出对话框创建 blog，如下图：

![](https://img-blog.csdn.net/20131125131717625)


## 3.测试新建的模块是否正常
右键项目mysite ——》 Run As ——》 PyDev：Django，控制台下打印出：
Validating models...
0 errors found
November 25, 2013 - 05:20:34
Django version 1.6, using settings 'mysite.settings'
Starting development server at http://127.0.0.1:8000/
Quit the server with CONTROL-C.


依照上面提示，在浏览器输入：[http://127.0.0.1:8000/](http://127.0.0.1:8000/)，出现如下测试网页：
![](https://img-blog.csdn.net/20131125132043828)


## 4.编辑代码
4.1　打开项目mysite/src/blog，修改 blog.models.py

```python
from django.db import models
from django.contrib import admin
# Create your models here.
class BlogPost(models.Model):
    title = models.CharField(max_length=150)
    body = models.TextField()
    timestamp = models.DateTimeField()
    
class BlogPostAdmin(admin.ModelAdmin):
    list_display = ('title', 'body', 'timestamp')
    
admin.site.register(BlogPost, BlogPostAdmin)
```

4.2打开项目mysite/src/blog，修改 blog.views.py

```python
from django.shortcuts import render
from django.http import HttpResponse
from django.template import loader, Context
from blog.models import BlogPost
# Create your views here.
def archive(request):
    posts = BlogPost.objects.all()
    t = loader.get_template('archive.html')
    c = Context({'posts':posts})
    return HttpResponse(t.render(c))
```

4.3打开项目mysite/src/mysite，修改mysite.setting.py

```python
# Application definition
INSTALLED_APPS = (
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
    'blog',
)
```

4.4打开项目mysite/src/mysite，修改mysite.urls.py

```python
from django.conf.urls import patterns, include, url
from django.contrib import admin
from blog.views import *
admin.autodiscover()
urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'mysite.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),
    url(r'^admin/', include(admin.site.urls)),
    url(r'^blog/$', archive),
)
```

## 5. 建立样式网页模板
在项目mysite/blog目录下，添加templates文件夹
5.1  在文件夹templates下，添加并编辑archive.html

```python
{% extends "base.html" %}
{% block content %}
{% for post in posts %}
<h1>{{ post.title}}</h1>
<!--<p>{{ post.timestamp|date:"Y-m-d f:s"}}</p>-->
<p>{{ post.timestamp|date:"Y-m-d H:i:s"}}</p>
<p>{{ post.body }}</p>
{% endfor %}
{% endblock %}
```

5.2在文件夹templates下，添加并编辑base.html

```python
<html>
<style type="text/css">
body { color: #000000; background: #ffffff; padding: 0 5em; margin:0 }
h1 { padding: 2em lem; background:#f5f5f5 }
h2 { color: #bf8; border-top: 1px dotted #fff; margin-top: 2em }
p { margin: lem 0 }
</style>
<body>
<h1>mysite.example.com</h1>
{% block content %}
{% endblock %}
</body>
</html>
```
至此，blog项目文件建立完毕，项目结构图如下：
![](https://img-blog.csdn.net/20131125134427062)


## 6.同步数据库
右键项目mysite ——》 Django ——》 Sync DB（Manage.py syncdb)，出现如下界面：

![](https://img-blog.csdn.net/20131125132915500)

根据上图提示，依次输入：yes ——》 blog的用户名“admin” ——》 邮箱 ——》 blog用户的密码“123456” ——》 重复密码 “123456”，创建完毕
这里的用户名和密码，在blog管理台需要用到。

## 7.运行成功
登陆blog管理台，右键项目mysite ——》 Run As ——》 PyDev:Django，启动项目后，在浏览器输入：http://127.0.0.1:8000/admin
![](https://img-blog.csdn.net/20131125133437968)

登陆用户名和密码是初始化数据库的时候设定的，即步骤6："admin"和"123456"
![](https://img-blog.csdn.net/20131125133936859)

**8. 发布博客**
博客平台搭建完毕，登录到了管理后台，就可以发布博客了，点击上图的“Blog posts” ——》 “Add” 按钮，就可以编辑自己的博客，如下图：
![](https://img-blog.csdn.net/20131125134910437)

点击“Save”保存按钮后，就发布了自己的博客，然后在浏览器输入：[http://127.0.0.1:8000/blog/](http://127.0.0.1:8000/blog/)，就可以看自己刚才发布的博客了，如下图：
![](https://img-blog.csdn.net/20131125135120109)

一个基于django的简易博客就搭建起来了，很赞吧，O(∩_∩)O


**博客之星评选，请投我一票：**
**http://vote.blog.csdn.net/blogstaritem/blogstar2013/sunboy_2050**


