
# Django项目的创建及基本使用 - anlcy - 博客园






# [Django项目的创建及基本使用](https://www.cnblogs.com/camilla/p/7356766.html)
**安装步骤**
Django是[Python](http://lib.csdn.net/base/python)进行Web开发的框架，目前应用比较广泛。使用[python](http://lib.csdn.net/base/python)进行Web开发，能够很快的搭建所需的项目，可以运用于原型开发，也可以部署到实际的应用环境。
使用Django开发，首先要安装Django模块，安装过Python的系统，启动命令行，cd 到python pip的安装目录，执行
pip install django ---此处为pip安装方法，其他设备安装方式参考菜鸟教程http://www.runoob.com/django/django-install.html
安装好Django,需要将安装目录配置到环境变量path中，例如C:\Python33\Lib\site-packages\Django，django自动安装在python下面的lib\site-packages里面
如果安装时候碰到错误：
pip安装三方库的时候提示 Could not fetch URL https://pypi.python.org/simple/virtualenv/: There was a prob
将pip install 改成：pip --trusted-host pypi.python.org  install  三方库名
检查是否安装成功，可以在dos下进入Django-1.6.2目录查看Django版本。
1.输入python 2.输入import django3.输入django.get_version()
卸载django：pip uninstall django
**创建一个网站：**
首先创建项目，用cmd命令在希望创建的目录执行：
django-admin.py startproject hello
我们创建了项目名为hello的项目，此时hello目录结构如下：
.
├── hello
│   ├── __init__.py
│   ├── settings.py
│   ├── urls.py
│   └── wsgi.py
├── manage.py
我们执行
python manage.py runserver
此时，浏览器访问http://localhost:8000/，应该可以看到我们的服务器已经启动了。显示了一个Welcome to Django的页面。
Django提倡基于应用作为单位进行开发，我们创建自己的应用，使用命令
python manage.py startapp say_hello
这样，就创建了say_hello应用，当前目录结构为：
.
├── hello
│   ├── __init__.py
│   ├── settings.py
│   ├── urls.py
│   └── wsgi.py
├── manage.py
└── say_hello
├── admin.py
├── apps.py
├── __init__.py
├── migrations
│   └── __init__.py
├── models.py
├── tests.py
└── views.py
可以看到，多出了一个say_hello目录。下面开始我们的开发，首先在say_hello目录下，新建一个templates目录，其中创建say_hello.html页面。
<!DOCTYPE html>
<html>
<head>
<metacharset="UTF-8">
<title>Hello</title>
</head>
<body>
<h1>Hello {{hello_words}}</h1>
</body>
</html>
页面很简单，我们使用了Django提供的模板，取hello_words参数渲染视图，后面会看到如何赋值这个参数。
现在有了页面，我们需要做我们的控制器，在say_hello目录下的views.py中添加函数
from django.shortcutsimport render

def hello_page(request):
return render(
request,
'say_hello.html',
{'hello_words': request.GET.get('hello_words', "")}
)
使用了render函数，该函数会返回一个response对象，该对象中有经过渲染的html，我们这里基于say_hello.html模板进行渲染，渲染的参数由字典表示，参数名为hello_words，内容是GET请求的hello_words的参数值，如果没有参数值，则hello_words的值为""。
完成了控制器，设置一下url映射，打开hello目录下的urls.py，添加映射
from django.conf.urlsimport url
from django.contribimport admin
from say_helloimport views
urlpatterns = [
url(r'^admin/', admin.site.urls),
url(r'^$', views.hello_page, name='hello_page'),
]
使用正则表达式^$，将发送到根路径下的请求转交给home_page处理，就是上面实现的控制器，这里Django使用视图来表示控制器，实际上Django也是MVC架构，只是M对应的是实体类，V对应的是模板，也就是html文件，控制器对应了views.py的定义。
现在运行程序，还是无法运行的，会报错找不到say_hello.html，这是因为我们还需要注册一下我们的应用，打开settings.py文件，在INSTALLED_APPS列表中添加一项"say_hello"，如下
\# Application definition
INSTALLED_APPS = [
'django.contrib.admin',
'django.contrib.auth',
'django.contrib.contenttypes',
'django.contrib.sessions',
'django.contrib.messages',
'django.contrib.staticfiles',
'say_hello',
]
现在运行服务器，浏览器输入localhost:8000可以看到Hello，如果是使用http://localhost:8000/?hello_words=World，则可以看到Hello World.





