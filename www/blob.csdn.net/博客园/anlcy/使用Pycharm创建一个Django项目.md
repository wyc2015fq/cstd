
# 使用Pycharm创建一个Django项目 - anlcy - 博客园






# [使用Pycharm创建一个Django项目](https://www.cnblogs.com/camilla/p/8857022.html)
在使用python写脚本一段时间后，想尝试使用Django来编写一个python项目，现做以下记录备忘：
## 1、创建项目
![](https://segmentfault.com/img/bVWINZ?w=973&h=614)
如果本地没有安装与所选python版本对应Django版本，pycharm会自动下载相应的版本：
![](https://segmentfault.com/img/bVWIPu?w=612&h=152)
创建后运行项目，默认页面为[http://127.0.0.1:8000/](http://127.0.0.1:8000/)，打开后：
![](https://segmentfault.com/img/bVWIX3?w=1177&h=282)
出现上面的页面，则正面项目创建成功
目录结构：
![](https://segmentfault.com/img/bVWI0N?w=466&h=429)
#### migrations/：用于记录 models 中数据的变更。admin.py：映射 models 中的数据到 Django 自带的 admin 后台。 apps.py：在新的 Django 版本中新增，用于应用程序的配置。 models.py：创建应用程序数据表模型（对应数据库的相关操作）。 tests.py：创建 Django 测试。 views.py：控制向前端显示哪些数据。
## 2、创建APP
在Django项目中可以包含多个APP，相当于一个大型项目中的分系统、子模块、功能部件等，相互之间比较独立，但也有联系，所有APP共享项目资源
![](https://segmentfault.com/img/bVWI2s?w=822&h=329)
输入：python manage.py startapp myapp
生成myapp文件夹
## 3、视图和url配置
myapp/views.py文件代码：
`from django.http``import HttpResponse``#需要导入HttpResponse模块``def``hello``(request):``#request参数必须有，名字类似self的默认规则，可以修改，它封装了用户请求的所有内容``return HttpResponse(``"Hello world ! ")``#不能直接字符串，必须是由这个类封装，此为Django规则`testDjango/urls.py文件代码：
`from myapp``import views``#首先需要导入对应APP的views
urlpatterns = [
    url(``r'^admin/', admin.site.urls),``#admin后台路由
    url(``r'^hello$', views.hello),``#你定义的路由，第一个参数为引号中的正则表达式，第二个参数业务逻辑函数（当前为views中的hello函数）
]`运行项目：
命令行的方式是：python manage.py runserver 127.0.0.1:8000
但是在pycharm中可以使用如下方法：
![](https://segmentfault.com/img/bVWJjJ?w=469&h=148)
![](https://segmentfault.com/img/bVWJj2?w=885&h=288)
![](https://segmentfault.com/img/bVWJkj?w=1283&h=404)
![](https://segmentfault.com/img/bVWJky?w=766&h=157)
## 4、Django模板
修改views文件：
`def``hello``(request):``# return HttpResponse("Hello world ! ")
    context = {}
    context[``'hello'] =``'Hello World!'``#数据绑定``return render(request,``'hello.html', context)``#将绑定的数据传入前台`被继承的模板：
`{#base.html#}``<!DOCTYPE html>``<``html>``<``head>``<``meta``charset=``"utf-8">``<``title>模板测试``</``title>``</``head>``<``body>``<``h1>``{{ hello }}``</``h1>``<``p>Django模板测试``</``p>``{%``block mainbody %}``<``p>original``</``p>``{%``endblock %}``</``body>``</``html>`hello.html 中继承 base.html，并替换特定 block，hello.html 修改后的代码如下：
`{#hello.html#}``{%``extends "base.html" %}``{%``block mainbody %}``<``p>继承了 base.html 文件``</``p>``{%``endblock %}`重新访问地址[http://127.0.0.1:8000/hello](http://127.0.0.1:8000/hello)，输出结果如下：
![](https://segmentfault.com/img/bVWJqN?w=638&h=249)
## 5、引入静态文件
需要将一些静态资源引入项目，新建一个static目录，可以将js、css等文件放入这个目录中：
![](https://segmentfault.com/img/bVWJzh?w=495&h=127)
需要让Django找到这个目录，需要在setting文件中进行配置：
![](https://segmentfault.com/img/bVWJA6?w=857&h=240)
在html文件中引入静态资源：
`{%``load staticfiles %}``{#base.html#}``<!DOCTYPE html>``<``html>``<``head>``<``meta``charset=``"utf-8">``<``link``rel=``"stylesheet"``href=``"``{%``static 'css/mypage.css' %}``">``<``script``src=``"``{%``static 'js/jquery-1.11.1.js' %}``">``</``script>``<``title>模板测试``</``title>``</``head>``<``body>``<``h1>``{{ hello }}``</``h1>``<``p>Django模板测试``</``p>``{%``block mainbody %}``<``p>original``</``p>``{%``endblock %}``</``body>``</``html>`第一行引入静态文件路径{% load staticfiles %}，在<head>...</head>里加入CSS网链和js文件





