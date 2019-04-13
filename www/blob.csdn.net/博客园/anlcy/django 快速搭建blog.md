
# django 快速搭建blog - anlcy - 博客园






# [django 快速搭建blog](https://www.cnblogs.com/camilla/p/7457221.html)
Django 自称是“最适合开发有限期的完美WEB框架”。本文参考《Django web开发指南》，快速搭建一个blog 出来，在中间涉及诸多知识点，这里不会详细说明，如果你是第一次接触Django ，本文会让你在感性上对Django有个认识，完成本文操作后会让你有兴趣阅读的相关书籍和文档。
废话少说，come on!!
本操作的环境：
===================
Windows 7/10
python 2.7
Django 1.8.2
===================

**创建工程**
创建mysite工程项目：
D:/djpy>django-admin.py startproject mysite
工程目录结构：
![](https://images0.cnblogs.com/i/311516/201405/192314479813569.png)
manage.py ----- Django项目里面的工具，通过它可以调用django shell和数据库等。
settings.py ---- 包含了项目的默认设置，包括数据库信息，调试标志以及其他一些工作的变量。
urls.py ----- 负责把URL模式映射到应用程序。

**创建blog应用**
在mysite目录下创建blog应用
D:/pydj>cd mysiteD:/djpy/mysite$python manage.py startapp blog
目录结构：
![](https://images2015.cnblogs.com/blog/311516/201510/311516-20151004204021183-131182059.jpg)

**初始化admin后台数据库**
python 自带SQLite数据库，Django支持各种主流的数据库，这里为了方便推荐使用SQLite，如果使用其它数据库请在settings.py文件中设置。
切换到mysite创建数据库：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
D:/djpy/mysite$python manage.py syncdbC:\Python27\lib\site-packages\django\core\management\commands\syncdb.py:24: RemovedInDjango19Warning: The syncdb command will be removed in Django 1.9
warnings.warn("The syncdb command will be removed in Django 1.9", RemovedInDjango19Warning)
Operations to perform:
Synchronize unmigrated apps: staticfiles, messages
Apply all migrations: admin, contenttypes, auth, sessions
Synchronizing apps without migrations:
Creating tables...
Running deferred SQL...
Installing custom SQL...
Running migrations:
Rendering model states... DONE
Applying contenttypes.0001_initial... OK
Applying auth.0001_initial... OK
Applying admin.0001_initial... OK
Applying contenttypes.0002_remove_content_type_name... OK
Applying auth.0002_alter_permission_name_max_length... OK
Applying auth.0003_alter_user_email_max_length... OK
Applying auth.0004_alter_user_username_opts... OK
Applying auth.0005_alter_user_last_login_null... OK
Applying auth.0006_require_contenttypes_0002... OK
Applying sessions.0001_initial... OK
You have installed Django's auth system, and don't have any superusers defined.
Would you like to create one now? (yes/no): yes
Username (leave blank to use 'fnngj'):    用户名（默认当前系统用户名）
Email address: fnngj@126.com     邮箱地址
Password:     密码
Password (again):    重复密码
Superuser created successfully.
![复制代码](https://common.cnblogs.com/images/copycode.gif)

**设置admin应用**
admin 是Django 自带的一个后台管理系统。
1、添加blog应用，打开mysite/mysite/settings.py 文件：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
\#Application definitionINSTALLED_APPS =('django.contrib.admin','django.contrib.auth','django.contrib.contenttypes','django.contrib.sessions','django.contrib.messages','django.contrib.staticfiles','blog',
)![复制代码](https://common.cnblogs.com/images/copycode.gif)
在列表末尾，添加blog 应用
2、在我们创建django项目时，admin就已经创建，打开mysite/mysite/urls.py文件：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
from django.conf.urlsimportinclude, urlfrom django.contribimportadmin
urlpatterns =[
    url(r'^admin/', include(admin.site.urls)),
]![复制代码](https://common.cnblogs.com/images/copycode.gif)

3、启动django容器
![复制代码](https://common.cnblogs.com/images/copycode.gif)
D:\pydj\mysite>python manage.py runserver
Performing system checks...
System check identified no issues (0 silenced).
October 04, 2015 - 20:56:45Django version 1.8.2, using settings'mysite.settings'Starting development server at http://127.0.0.1:8000/Quit the server with CTRL-BREAK.![复制代码](https://common.cnblogs.com/images/copycode.gif)

4、访问后台应用
http://127.0.0.1:8000/admin![](https://images0.cnblogs.com/i/311516/201405/192322321374972.png)
输入用户、密码，用户名密码为第一次创建数据库时创建的。回想“设置数据库”时的设置。

**设计Model(即设计数据库表)**
1、设计model
现在我们打开blog目录下的models.py文件，这是我们定义blog数据结构的地方。打开mysite/blog/models.py 文件进行修改：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
from django.dbimportmodelsfrom django.contribimportadmin\#Create your models here.classBlogsPost(models.Model):
    title = models.CharField(max_length = 150)
    body =models.TextField()
    timestamp =models.DateTimeField()
admin.site.register(BlogsPost)![复制代码](https://common.cnblogs.com/images/copycode.gif)

2、 再次初始化数据库
![复制代码](https://common.cnblogs.com/images/copycode.gif)
D:\pydj\mysite>python manage.py makemigrations blog
Migrationsfor 'blog':
  0001_initial.py:
    -Create model BlogsPost
D:\pydj\mysite>python manage.py syncdb
C:\Python27\lib\site-packages\django\core\management\commands\syncdb.py:24: RemovedInDjango19Warning: The syncdb command will be removedin Django 1.9warnings.warn("The syncdb command will be removed in Django 1.9", RemovedInDjango19Warning)
Operations to perform:
  Synchronize unmigrated apps: staticfiles, messages
  Apply all migrations: admin, blog, contenttypes, auth, sessions
Synchronizing apps without migrations:
  Creating tables...
    Running deferred SQL...
  Installing custom SQL...
Running migrations:
  Rendering model states... DONE
  Applying blog.0001_initial... OK![复制代码](https://common.cnblogs.com/images/copycode.gif)

3、再次runserver启动服务，访问admin后台，创建文章。
登陆成功选择add创建博客
![](https://images0.cnblogs.com/i/311516/201405/192323093871885.png)
输入博客标题，正文、日期时间、点击save创建博客。
![](https://images0.cnblogs.com/i/311516/201405/192323272935956.png)

**设置admin 的BlogsPost界面**
打开mysite/blog/models.py 文件，做如下修改：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
from django.dbimportmodelsfrom django.contribimportadmin\#Create your models here.classBlogsPost(models.Model):
    title = models.CharField(max_length = 150)
    body =models.TextField()
    timestamp =models.DateTimeField()classBlogPostAdmin(admin.ModelAdmin):
    list_display = ('title','timestamp')
    
admin.site.register(BlogsPost,BlogPostAdmin)![复制代码](https://common.cnblogs.com/images/copycode.gif)

创建BlogPostAdmin类，继承admin.ModelAdmin父类，以列表的形式显示BlogPost的标题和时间。
![](https://images0.cnblogs.com/i/311516/201405/192324392313866.png)

**创建blog的公共部分**
从Django的角度看，一个页面具有三个典型的组件：
一个模板（template）：模板负责把传递进来的信息显示出来。
一个视图（view）：视图负责从数据库获取需要显示的信息。
一个URL模式：它负责把收到的请求和你的试图函数匹配，有时候也会向视图传递一些参数。

**创建模板**
在blog项目下创建templates目录（mysite/blog/templates/）,在目录下创建模板文件index.html，内容如下：
{% for post in posts %}<h2>{{ post.title }}</h2><p>{{ post.timestamp }}</p><p>{{ post.body }}</p>{% endfor%}

**创建视图函数**
打开mysite/blog/views.py文件：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
\#coding=utf-8from django.shortcutsimportrenderfrom blog.modelsimportBlogsPostfrom django.shortcutsimportrender_to_response\#Create your views here.defindex(request):
    blog_list =BlogsPost.objects.all()return render_to_response('index.html',{'blog_list':blog_list})![复制代码](https://common.cnblogs.com/images/copycode.gif)
blog_list = BlogPost.objects.all() ：获取数据库里面所拥有BlogPost对象
render_to_response()返回一个页面(index.html)，顺带把数据库中查询出来的所有博客内容（blog_list）也一并返回。

**创建blog的URL模式**
在mysite/urls.py文件里添加blog的url：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
\#coding=utf-8from django.conf.urlsimportpatterns, include, urlfrom django.contribimportadmin
urlpatterns = patterns('',
    url(r'^admin/', include(admin.site.urls)),
    url(r'^index/$','blog.views.index'),
)![复制代码](https://common.cnblogs.com/images/copycode.gif)

再次启动服务（$python manage.py runserver），访问blog应用（*http://127.0.0.1:8000/**index/*）下图有错，仅供参考。
页面如下：
![](https://images0.cnblogs.com/i/311516/201405/192328433714620.png)
当然，读者可以继续到admin后台添加blog，从而刷新这个页是否显示新添加的blog。

**添加样式**
创建基础模板
在mysite/blog/templates目录里创建base.html的模板：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<html><styletype="text/css">body{color:\#efd;background:\#453;padding:0 5em;margin:0}h1{padding:2em 1em;background:\#675}h2{color:\#bf8;border-top:1px dotted \#fff;margin-top:2em}p{margin:1em 0}</style><body><h1>虫师blog</h1><h3>大人不华，君子务实</h3>{% block content %}
        {% endblock %}</body></html>![复制代码](https://common.cnblogs.com/images/copycode.gif)

修改index.html模板，让它引用base.html模板和它的“content”块。
![复制代码](https://common.cnblogs.com/images/copycode.gif)
{% extends "base.html" %}
  {% block content %}
      {% for post in posts %}<h2>{{  post.title }}</h2><p>{{ post.timestamp | date:"1,F jS"}}</p><p>{{ post.body }}</p>{% endfor %}
  {% endblock %}![复制代码](https://common.cnblogs.com/images/copycode.gif)
再次刷新博客页面：
http://127.0.0.1/index/   下图有错，仅供参考。
![](https://images0.cnblogs.com/i/311516/201405/192330068567523.png)
请系统的学习django web框架，然后在此基础上做更多的扩展，开发自己真正的blog。






