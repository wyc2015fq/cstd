
# Django之“学习笔记”网站开发3 - OraYang的博客 - CSDN博客

2018年01月19日 15:59:09[OraYang](https://me.csdn.net/u010665216)阅读数：825所属专栏：[python科学计算、游戏开发、后台开发](https://blog.csdn.net/column/details/18109.html)



# 引言
前面我们讲述了基本的Web应用程序开发流程，现在我们就来谈谈如何设置应用程序的样式并对其进行部署。
当前咱们这个”学习笔记“项目功能已齐备，但未设置样式，也只是在本地计算机上运行。在这次分享中我们将以简单而专业的方式设置这个项目的样式，再将其部署到一台服务器上，让所有人都能建立账户。
为了设置样式，我们将使用Bootstrap库，这是一组工具，用于为Web应用程序设置样式，使其在现代设备上都看起来很专业，无论是大型的平板显示器还是智能手机
# 设置项目”学习笔记”的样式
前面我们一直专注与项目学习笔记的功能，所以外观很丑，这里我们将简单地介绍应用程序django-bootstrap3，并演示如何将其继承到项目中，为部署项目做好准备。
## 安装django-bootstrap3
在虚拟环境中输入如下命令：
```python
$
```
```python
pip install django-bootstrap3
```
然后再settings.py文件中输入添加应用‘bootstrap3’，并添加jQuery库。
## 使用Bootstrap来设置项目“学习笔记”的样式
先在浏览器地址栏输入地址：
> [http://getbootstrap.com/docs/4.0/examples/](http://getbootstrap.com/docs/4.0/examples/)
然后点击Get Start并找到Example中的Navbars。
## 修改base.html
首先定义HTML头部
在base.html输入如下代码：
```python
{% load bootstrap3 %}
```
```python
<!DOCTYPE html>
```
```python
<
```
```python
html
```
```python
lang
```
```python
=
```
```python
"en"
```
```python
>
```
```python
<
```
```python
head
```
```python
>
```
```python
<
```
```python
meta
```
```python
charset
```
```python
=
```
```python
"utf-8"
```
```python
>
```
```python
<
```
```python
meta
```
```python
http-equiv
```
```python
=
```
```python
"X-UA-Compatible"
```
```python
content
```
```python
=
```
```python
"IE=edge"
```
```python
>
```
```python
<
```
```python
meta
```
```python
name
```
```python
=
```
```python
"viewport"
```
```python
content
```
```python
=
```
```python
"width=device-width, initial-scale=1"
```
```python
>
```
```python
<
```
```python
title
```
```python
>
```
```python
Learning Log
```
```python
</
```
```python
title
```
```python
>
```
```python
{% bootstrap_css %}
    {% bootstrap_javascript %}
```
```python
</
```
```python
head
```
```python
>
```
接下来定义导航栏、设置各个页面的样式，最终结果如下：
![这里写图片描述](https://img-blog.csdn.net/20180119120932693?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这里采用模板的套路是：
> 将找到的模板复制到base.html中，并修改包含实际内容的元素，以使用该模板来显示项目的信息；然后，使用Bootstrap的样式设置工具来设置各个页面的内容的样式。

# 部署“学习笔记”
## 建立Heroku账户
访问：[https://www.heroku.com/](https://www.heroku.com/)并注册账户：
![这里写图片描述](https://img-blog.csdn.net/20180119124018238?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 安装Heroku Toolbelt
在本地安装Heroku CLI ，是为了接下来能够将本地的项目部署到服务器上。
安装方法：[传送门](https://devcenter.heroku.com/articles/heroku-cli#download-and-install)
我是用的是如下的方法，下载，安装
![这里写图片描述](https://img-blog.csdn.net/20180119144344190?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 安装必要的包
输入以下命令安装必要的包：
```python
$
```
```python
pip install dj-database-url
```
```python
$
```
```python
pip install dj-static
```
```python
$
```
```python
pip install static3
```
```python
$
```
```python
pip install gunicorn
```
## 接下来安装软件依赖
这部分安装方法，直接参考：[传送门](https://devcenter.heroku.com/articles/deploying-python#declare-app-dependencies)
## 剩余需要修改的一些文件
还有一些要修改的文件runtime.txt，settings.py，Procfile，wsgi.py等等，大家可以去github上对比看，这里就不列出来了。
## push to heroku
采用下面的方法：
```python
git add .
$ git commit -m
```
```python
"Added a Procfile."
```
```python
$ heroku login
Enter your Heroku credentials.
```
```python
...
```
```python
$ heroku create
Creating intense-falls-
```
```python
9163.
```
```python
.. done, stack is cedar
http://intense-falls-
```
```python
9163.
```
```python
herokuapp.com/ | git@heroku.com:intense-falls-
```
```python
9163.
```
```python
git
Git remote heroku added
$ git push heroku master
```
```python
...
```
```python
-----> Python app detected
```
```python
...
```
```python
-----> Launching... done, v7
       https://intense-falls-
```
```python
9163.
```
```python
herokuapp.com/ deployed to Heroku
```
如果在push过程中，发生了remote error错误，可以输入下面的命令增大文件的大小：
```python
$
```
```python
git
```
```python
config
```
```python
-
```
```python
-
```
```python
global
```
```python
http
```
```python
.
```
```python
postBuffer
```
```python
838860800
```
# 总结
在本次分享中，我们简要讲述了如何利用Bootstrap模板来修改样式，以及如何去将web应用程序部署到Heroku网站上。
# 项目源码
GitHub：[传送门](https://github.com/shawshany/Django-Web3)

