# Django之你的第一个Django应用程序（1） - miner_zhu的博客 - CSDN博客





2018年07月20日 20:25:11[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：122








官方文档：[https://docs.djangoproject.com/en/2.0/](https://docs.djangoproject.com/en/2.0/)

## 创建项目

如果这是你第一次使用Django，你将不得不处理一些初始设置。也就是说，您需要自动生成一些建立Django [项目的](https://docs.djangoproject.com/en/2.0/glossary/#term-project)代码- Django实例的设置集合，包括数据库配置，Django特定选项和特定于应用程序的设置。

从命令行`cd`进入要存储代码的目录，然后运行以下命令：

```
$ django-admin startproject mysite
```

这将`mysite`在当前目录中创建一个目录。

```
mysite/
    manage.py
    mysite/
        __init__.py
        settings.py
        urls.py
        wsgi.py
```

这些文件是：
- 外部`mysite/`根目录只是项目的容器。它的名字对Django来说无关紧要; 你可以将它重命名为你喜欢的任何东西。
- `manage.py`：一个命令行实用程序，允许您以各种方式与此Django项目进行交互。您可以`manage.py`在[django-admin和manage.py中](https://docs.djangoproject.com/en/2.0/ref/django-admin/)阅读有关的所有详细信息 。
- 内部`mysite/`目录是项目的实际Python包。它的名称是您需要用来导入其中任何内容的Python包名称（例如`mysite.urls`）。
- `mysite/__init__.py`：一个空文件，告诉Python该目录应该被视为Python包。如果您是Python初学者，请阅读官方Python文档中[有关包的更多信息](https://docs.python.org/3/tutorial/modules.html#tut-packages)。
- `mysite/settings.py`：此Django项目的设置/配置。 [Django设置](https://docs.djangoproject.com/en/2.0/topics/settings/)将告诉您有关设置如何工作的所有信息。
- `mysite/urls.py`：这个Django项目的URL声明; 您的Django支持的站点的“目录”。您可以在[URL调度](https://docs.djangoproject.com/en/2.0/topics/http/urls/)程序中阅读有关URL的更多信息。
- `mysite/wsgi.py`：与WSGI兼容的Web服务器的入口点，用于为您的项目提供服务。有关更多详细信息，请参阅[如何使用WSGI](https://docs.djangoproject.com/en/2.0/howto/deployment/wsgi/)进行[部署](https://docs.djangoproject.com/en/2.0/howto/deployment/wsgi/)。

## 开发服务器

让我们验证您的Django项目是否有效。`mysite`如果尚未更改到外部目录，请运行以下命令：

```
$ python manage.py runserver
```

您将在命令行中看到以下输出：

```
Performing system checks...

System check identified no issues (0 silenced).

You have unapplied migrations; your app may not work properly until they are applied.
Run 'python manage.py migrate' to apply them.

July 06, 2018 - 15:50:53
Django version 2.0, using settings 'mysite.settings'
Starting development server at http://127.0.0.1:8000/
Quit the server with CONTROL-C.
```

**注意**

暂时忽略有关未应用数据库迁移的警告; 我们很快就会处理数据库。

您已经启动了Django开发服务器，这是一个纯粹用Python编写的轻量级Web服务器。我们已经将它包含在Django中，因此您可以快速开发，而无需处理配置生产服务器（如Apache），直到您准备好进行生产。

现在是时候注意了：**不要**在类似生产环境的任何地方使用这个服务器。它仅用于开发时使用。（我们的业务是制作Web框架，而不是Web服务器。）

现在服务器正在运行，请使用Web浏览器访问[http://127.0.0.1:8000/](http://127.0.0.1:8000/)。你会看到一个“Congratulations!” page, with a rocket taking off. It worked!

## 创建polls应用

既然你的环境 - 一个“项目” - 已经建立起来，你就可以开始工作了。

您在Django中编写的每个应用程序都包含一个遵循特定约定的Python包。Django附带了一个实用程序，可以自动生成应用程序的基本目录结构，因此您可以专注于编写代码而不是创建目录。

**项目与应用**

项目和应用程序之间有什么区别？应用程序是执行某些操作的Web应用程序 - 例如，Weblog系统，公共记录数据库或简单的轮询应用程序。项目是特定网站的配置和应用程序的集合。项目可以包含多个应用程序。一个应用程序可以在多个项目中。

您的应用程序可以存在于[Python路径的](https://docs.python.org/3/tutorial/modules.html#tut-searchpath)任何位置。在本教程中，我们将在您的`manage.py` 文件旁边创建我们的民意调查应用程序，以便可以将其导入为自己的顶级模块，而不是子模块`mysite`。

要创建应用程序，请确保您与该目录位于同一目录中`manage.py` 并键入以下命令：

```
$ python manage.py startapp polls
```

那将创建一个目录`polls`，其布局如下：

```
polls/
    __init__.py
    admin.py
    apps.py
    migrations/
        __init__.py
    models.py
    tests.py
    views.py
```

此目录结构将容纳轮询应用程序。

## 写下你的第一个视图

我们来写第一个视图。打开文件`polls/views.py` 并在其中放入以下Python代码：

polls/views.py

```
from django.http import HttpResponse


def index(request):
    return HttpResponse("Hello, world. You're at the polls index.")
```

这是Django中最简单的视图。要调用视图，我们需要将其映射到URL - 为此我们需要一个URLconf。

要在polls目录中创建URLconf，请创建一个名为的文件`urls.py`。您的app目录现在应该如下所示：

```
polls/
    __init__.py
    admin.py
    apps.py
    migrations/
        __init__.py
    models.py
    tests.py
    urls.py
    views.py
```

在该`polls/urls.py`文件中包含以下代码：

polls/urls.py

```
from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='index'),
]
```

下一步是将根URLconf指向`polls.urls`模块。在 `mysite/urls.py`，添加导入`django.urls.include`并[`include()`](https://docs.djangoproject.com/en/2.0/ref/urls/#django.urls.include)在`urlpatterns`列表中插入 ，所以你有：

mysite/urls.py

```
from django.contrib import admin
from django.urls import include, path

urlpatterns = [
    path('polls/', include('polls.urls')),
    path('admin/', admin.site.urls),
]
```

该[`include()`](https://docs.djangoproject.com/en/2.0/ref/urls/#django.urls.include)函数允许引用其他URLconf。每当Django遇到时[`include()`](https://docs.djangoproject.com/en/2.0/ref/urls/#django.urls.include)，它都会删除与该点匹配的URL的任何部分，并将剩余的字符串发送到包含的URLconf以进行进一步处理。

背后的想法[`include()`](https://docs.djangoproject.com/en/2.0/ref/urls/#django.urls.include)是使即插即用的URL变得容易。由于民意调查位于他们自己的URLconf（`polls/urls.py`）中，因此可以将它们放在“/ polls /”下，或“/ fun_polls /”下，或“/ content / polls /”下，或任何其他路径根目录下，并且应用仍会工作。

**什么时候用 [`include()`](https://docs.djangoproject.com/en/2.0/ref/urls/#django.urls.include)**

`include()`当您包含其他URL模式时，应始终使用。 `admin.site.urls`是唯一的例外。

您现在已将`index`视图连接到URLconf。让我们验证它是否正常工作，运行以下命令：

```
$ python manage.py runserver
```

在浏览器中转到[http：// localhost：8000 / polls /](http://localhost:8000/polls/)，您应该看到文本"Hello, world. You’re at the polls index.“，您在`index`视图中定义的 。

[**url**](https://docs.djangoproject.com/en/1.11/ref/urls/#django.conf.urls.url) 函数有四个参数，两个必需参数：**regex** 正则和 **view** 视图；两个选项参数：**kwargs**字典和 **name** 名字。在这点上，值得再看下这些参数到底是干什么的。

### [url](https://docs.djangoproject.com/en/1.11/ref/urls/#django.conf.urls.url)参数：regex

术语 “regex” 是正则表达式 “regular expression” 的缩写，是匹配字符串的一段语法，像这里例子的是 url 匹配模式。Django 从列表的第一个正则表达式开始，按顺序匹配请求的 URL，直到找到与之匹配的。

注意，这些正则表达式不会去匹配 GET 和 POST 请求的参数值，或者域名。比如 **https://www.example.com/myapp/** 这个请求，URLconf 会找 **myapp/**；**https://www.example.com/myapp/?page=3** 这个请求，URLconf 同样只会找 **mysqpp/**。

如果你需要正则表达式的帮助，可以看 [**Wikipedia’s entry**](https://en.wikipedia.org/wiki/Regular_expression) 和 关于 [**re**](https://docs.python.org/3/library/re.html#module-re) 模块的文档。还有，由 Jeffrey Friedl 写的书 《掌握正则表达式》 也是很棒的。实际上，你并不需要成为正则表达式方面的专家，你真正要会的是如何使用简单捕获模式。因为复杂的正则可能会有不尽人意的查找性能，所以你不太应该全依赖于正则匹配。

最后，一个性能注意点：这些正则表达式在 URLconf 模块加载后的第一时间就被编译了。它们都是非常快的（只要查找的不是特别复杂就像上面举例的）。

### [url](https://docs.djangoproject.com/en/1.11/ref/urls/#django.conf.urls.url) 参数：view

当 Django 发现一个正则表达式匹配时，Django 就会调用指定的视图函数，[**HttpRequest**](https://docs.djangoproject.com/en/1.11/ref/request-response/#django.http.HttpRequest) 对象作为第一个参数，正则表达式捕获的值作为其他参数。如果正则使用简单捕获，值会作为位置参数传递；如果使用命名捕获，值会作为关键字传递。我们稍后会给出一个例子。

### [url](https://docs.djangoproject.com/en/1.11/ref/urls/#django.conf.urls.url) 参数：kwargs

任意的关键字参数都可以作为字典传递到目标视图。但我们不准备在本教程里使用 Django 的这个特性。

### [url](https://docs.djangoproject.com/en/1.11/ref/urls/#django.conf.urls.url) 参数：name

命名你的 URL 可以让你在 Django 的别处明白引用的是什么，特别是在模版里。这个强大的特性允许你在项目里对一个文件操作就能对 URL 模式做全局改变。



