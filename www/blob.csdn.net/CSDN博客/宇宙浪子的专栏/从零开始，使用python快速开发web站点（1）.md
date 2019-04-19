# 从零开始，使用python快速开发web站点（1） - 宇宙浪子的专栏 - CSDN博客
2014年09月05日 15:47:21[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：508
环境：ubuntu 12.04
python版本:  2.73
ok,首先，既然是从零开始，我们需要的是一台可以运行的python的计算机环境，并且假设你已经安装好了python,
(ubuntu 或者debian的linux用户，安装python的过程可以参考：[http://blog.csdn.net/i7788/article/details/9886127](http://blog.csdn.net/i7788/article/details/9886127))
然后，既然是快速开发，必不可少的需要用到框架，python的web框架很多，我这里用到的是django,很适合做内容站点，而且也是python众多框架里最流行的～～～之一。
下载 django
**[html]**[view
 plain](http://blog.csdn.net/i7788/article/details/10306595#)[copy](http://blog.csdn.net/i7788/article/details/10306595#)
- sudo apt-get install python-django  
![](https://img-blog.csdn.net/20130825184216890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaTc3ODg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
当出现上面的情况，说明django 安装完毕。
接下来。直接开发。在这里我多罗嗦关于mvc的东西。我相信大家肯定明白了。但是有一点需要注意，django 和 thinkphp 之类的框架类似的一点是：
**一个完整的web应用由多个django应用组成**
废话不多说，直接建立新的项目，windows 下面是django-admin.py. linux下面是 django-admin命令。
以ubuntu为例：
django-admin startproject(参数，表明建立一个新的应用) dqq0(项目名称)
然后 一个新的python web应用框架已经建立成功。接下来就是添加内容
文件目录如下：
![](https://img-blog.csdn.net/20130825185517546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaTc3ODg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
等等，还有一点，就是python 用的并不一定是apache服务器，我这里用的是自带的web 服务器，开启web 服务很简单.
打开新建的应用文件夹:
**[html]**[view
 plain](http://blog.csdn.net/i7788/article/details/10306595#)[copy](http://blog.csdn.net/i7788/article/details/10306595#)
- python manage.py runserver  
然后，我相信你可以看得到，服务器已经启动的信息提示。
（默认端口是8000，）
至此，python web 应用的环境搭建完毕，接下来是添加数据模型，url分发。以及django特有的界面管理模块。
书接上文。[http://blog.csdn.net/i7788/article/details/10306595](http://blog.csdn.net/i7788/article/details/10306595)
[](http://blog.csdn.net/i7788/article/details/10306595)
首先是数据库的搭建，这里的django的数据模型搭建十分easy. 
no sql。just config file
打开应用目录，下的setting.py 文件。找到
![](https://img-blog.csdn.net/20130825191121906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaTc3ODg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在这里配置你的数据库信息。（这里使用的是sqlite3）
配置完毕，使用自带的命令，生成数据库文件:
python manage.py syncdb
（我相信你能看懂提示成功）
接下来创建一个应用，之前说过一个完整的网站由多个django应用构成。每一个django对于一个类型的操作，比如现在我们要做一个blog,
那么先来建立一个 文章应用:
**[html]**[view
 plain](http://blog.csdn.net/i7788/article/details/10306947#)[copy](http://blog.csdn.net/i7788/article/details/10306947#)
- python manage.py startapp Article  
然后，你的web 应用目录下会多了一个Article的文件夹，这里存放这你对文章的所有操作
首先是编辑Article的属性以及与数据库的字段映射，需要编辑的是models.py
![](https://img-blog.csdn.net/20130825194237140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaTc3ODg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后配置 项目的setting.py 
添加app 
**[html]**[view
 plain](http://blog.csdn.net/i7788/article/details/10306947#)[copy](http://blog.csdn.net/i7788/article/details/10306947#)
- INSTALLED_APPS = (  
-     'django.contrib.auth',  
-     'django.contrib.contenttypes',  
-     'django.contrib.sessions',  
-     'django.contrib.sites',  
-     'django.contrib.messages',  
-     'django.contrib.staticfiles',  
-     'dqq0.Article',  
-     # Uncomment the next line to enable the admin:  
-     # 'django.contrib.admin',  
-     # Uncomment the next line to enable admin documentation:  
-     # 'django.contrib.admindocs',  
- )  
然后 更改数据库信息
python manage.py syncdb
ok，一个model 创建完成
URL分发部分其实跟其他一样类似。首先是django的url分发指向应用目录下的 urls,py文件。然后，现在我们有两个选择
在urls.py目录里面，直接将url替换成我们需要的字符串。或者，继续分发到下面。
