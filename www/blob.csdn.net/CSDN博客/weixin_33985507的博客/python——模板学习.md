# python——模板学习 - weixin_33985507的博客 - CSDN博客
2017年04月19日 14:40:19[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
- 
模板
- 
创建项目
- 
初始化项目
```
django-admin startproject tmpl
cd tmpl
python manage.py startapp learn
```
- 
修改tmpl/settings.py
```
ALLOWED_HOSTS = [
  '虚拟机ip地址',
  'www.mysite.com'
  ]
  
INSTALLED_APPS = (
'django.contrib.admin',
'django.contrib.auth',
'django.contrib.contenttypes',
'django.contrib.sessions',
'django.contrib.messages',
'django.contrib.staticfiles',
 
'learn',
)
```
- 
在learn目录下新建templates文件夹
```
cd templates
touch index.html
```
index.html内容如下：
```
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>TMPL Index</title>
</head>
<body>
    <div>TMPL  Index</div>
</body>
</html>
```
- 
修改learn/views.py  
修改如下：
```
from django.shortcuts import render
 
 
def home(request):
    return render(request, 'index.html')
```
- 
修改tmpl/urls.py  
修改如下：
```
from django.conf.urls import url
from django.contrib import admin
from learn import views as learn_views
 
 
    urlpatterns = [
        url(r'$', learn_views.home, name='home'),  # 新增
        url(r'^admin/', admin.site.urls),
    ]
```
备注：如果有多个应用时，可以在templates文件夹下新建一个learn文件夹，把模板文件index.html移动到templates/learn/下，views.py中的home方法也需要作出相应的修改：
```
def home(request):
    return render(request, 'learn/index.html')
```
这样做的目的是，让Django能够正确查找到home.html
- 
模板继承  
在templates/learn新建base.html,内容如下：
```
<!DOCTYPE html>
    <html>
    <head>
        <meta charset="utf-8">
        <title>{% block title %} 默认标题{% endblock %}</title>
    </head>
    <body>
        {% block content %}
        <div>这里是默认内容区</div>
        {% endblock %}
    </body>
    </html>
```
修改index.html, 修改如下：
```
{% extends 'learn/base.html' %}
{% block title %} 首页 {% endblock %}
{% block content %}
    <div> 这里是首页 </div>
{% endblock %}
```
- 
运行
`python manage.py runserver 0.0.0.0:8000`
主机访问：www.pyl.com:8000 ，输出：这里是首页
- 
感觉    
写习惯了前端，觉得模板继承写起来好麻烦，还要加{% %}和标记开头和结束，jade模板语法很好看，要是Python模板的语法也那样就好了
