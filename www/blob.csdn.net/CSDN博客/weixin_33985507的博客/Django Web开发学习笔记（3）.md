# Django Web开发学习笔记（3） - weixin_33985507的博客 - CSDN博客
2014年05月14日 06:50:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
## 1、创建一个简单视图
这章是按照DgangoBook的说明。在我们创建的工程目录下面DjangoE_1(这是我为自己的工程命名的名字）新建一个view.py的文件，并在该文件下添加如下代码
```
from django.http import HttpResponse
def hello(request):
    return HttpResponse("hello world")
```
 熟悉HTTP的话知道HttpResponse是服务器到返回到客户端的应答，而request是客户端到服务器的请求。
我们添加了视图的显示信息，可是Django怎样映射到我们的视图上？我们在生成的工程目录下面发现了那个叫urls.py的文件。这就是视图和控制的映射文件。打开后我们看到默认生成的代码：
```
from django.conf.urls import patterns, include, url
from django.contrib import admin
admin.autodiscover()
urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'DjangoE_1.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),
    url(r'^admin/', include(admin.site.urls)),
)
```
默认的代码是添加管理员权限的东西，我们暂且先不管这些东西，你也可以先注释掉这些内容。我们在其中添加这样两句代码：
from DjangoE_1.view import hello
 (r'^hello/$',hello),
于是我们的代码变成了这个样子
```
from django.conf.urls import patterns, include, url
from django.contrib import admin
admin.autodiscover()
from DjangoE_1.view import hello
urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'DjangoE_1.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),
    url(r'^admin/', include(admin.site.urls)),
    (r'^hello/$',hello),
)
```
需要注意的是erlpatterns是一个元组类型的数据，因此我们在末尾最好添加一个逗号，刷新一下服务器，重新登录一下
[http://localhost:8000/hello/](http://localhost:8000/hello/)
于是我们在我们的页面中看到了输出了
Hello world
熟悉Web的应该已经明白基本上的操作流程了，下面是一个稍微复杂一点的操作！
假如我们希望显示一个动态的内容，那我们该怎么办呢？也就是我们怎样想view.py的视图文件中传入参数？
```
#view.py
def hello(request):
    return HttpResponse("hello");
def showTime(request):
    now = datetime.datetime.now()
    html = """<html><body>It is now %s.
    <div id='mybtn'><input type='button' value='click me'></div></body></html>""" % now
    return HttpResponse(html)
def showCurrentTime(request):
    now = datetime.datetime.now();
    html="""<html><body><div id="timeArea"></div></body><script>function
    getTime(){document.getElementById('timeArea').innerText='%s';}setInterval("getTime()",1000);</script></html>""" %now;
    return HttpResponse(html)
```
```
#urls.py
from django.conf.urls import patterns, include, url
from django.contrib import admin
admin.autodiscover()
import DjangoE_1.view
urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'DjangoE_1.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),
    url(r'^admin/', include(admin.site.urls)),
    
    (r'^hello/$',DjangoE_1.view.hello),
    (r'^show/$', DjangoE_1.view.showTime),
    (r'^getnow/$', DjangoE_1.view.showCurrentTime),
)
```
　　可以发现，我们完全可以以字符串的形式传递数据。但是要求有严格的数据格式，因为在Python里面如果空格没有的话有时候都会报错。这里只是练习，实际开发中是不会有这样的代码的。另外，注意getnow是不会动态显示时间的，因为我们的now只是在连接的时候获取一次，因此它的值实际是不会变化的。下一节我们将应用模板来进行代码分离，目的是将Python和HTML的代码能区别开来。
