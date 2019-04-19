# Django mod_wsgi 配置 - 宇宙浪子的专栏 - CSDN博客
2016年03月29日 10:26:18[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：1026
原文：[http://www.cnblogs.com/JeffreySun/archive/2010/01/11/1612155.html](http://www.cnblogs.com/JeffreySun/archive/2010/01/11/1612155.html)
这几天花了点时间学习把Django开发好的项目部署到Apache上，感觉官方的一些文档说得很不清楚，花了我比较多的时间，这里把我自己配置的过程说一下。注意一下，我这里的配置的环境是Windows，版本情况：Apache 2.2，Django 1.1，Mod_wsgi 3.1，Python 2.6。不同版本的配置方法大同小异。
　　一、演示环境
　　这里假设您的Django和Apache已经安装好，并且Django的工程已经开发好。下面是我自己的相关目录位置，您需要参照您的实际目录。
- 我这里Apache安装在 C:\Program Files\Apache Software Foundation\Apache2.2 。
- 我的工程的目录是 D:\WorkSpace\DjangoProject\FirstProject，工程名称叫 FirstProject 。
　　二、下载mod_wsgi文件
　　mod_wsgi可以去[http://code.google.com/p/modwsgi/downloads/list](http://code.google.com/p/modwsgi/downloads/list)这里下载，选择您需要的版本，下载下来后，是一个名字为mod_wsgi*****.os的一个文件。把这个文件重命名为mod_wsgi.os，然后放到Apache的moduls目录下目录下，对于我这里的情况，就是放到
 C:\Program Files\Apache Software Foundation\Apache2.2\modules 这个目录下。放到这个目录下后，就算安装完毕了。
```
注意，不同版本的的mod_wsgi对于python的版本是有要求的，比如这里的mode_wsgi 3.1 就需要python 2.6+的相关版本。如果用python 2.5及以下的版本，就算配置好了，也无法启动Apache，而在EventViewer里面可以查看Apache给出的错误信息，大概如下：
　　*"httpd.exe: Syntax error on line 128 of C:/data/apache/conf/httpd.conf: Cannot load C:/data/apache/modules/mod_wsgi.so into server: The specified module could not be found.     . "*
```
　　三、配置
　　首先需要去更改Apache的httpd.conf(C:\Program Files\Apache Software Foundation\Apache2.2\conf目录下)配置文件：
　　1)在配置文件中LoadModule的那一段，添加一行如下：
LoadModule wsgi_module modules/mod_wsgi.so
　　2)然后再添加一行如下：
WSGIScriptAlias / "D:/WorkSpace/DjangoProject/FirstProject/django.wsgi"
　　注意上面的目录D:/WorkSpace/DjangoProject/FirstProject，这个就是您的工程的目录，需要根据的实际情况做修改。后面的django.wsgi这个文件是需要新建的，后面我再来说这个如何新建这个文件。
　　3)找到Directory的配置区域，添加如下的一段配置
　　　　<Directory "D:/WorkSpace/DjangoProject/FirstProject">
　　　　　　Order Deny,Allow
　　　　　　Allow from all
　　　　</Directory>
　　注意，上面的目录，这个同上面的一样，是您的工程的目录，根据您的实际情况做修改。
　　4)新建一个wsgi文件。
　　　　在工程的目录下新建一个django.wsgi，用记事本打开，输入以下的内容并保存：　
import os
　　　　　　import sys
　　　　　　import django.core.handlers.wsgi
　　　　　　sys.path.append(r'D:\WorkSpace\DjangoProject')
　　　　　　os.environ['DJANGO_SETTINGS_MODULE'] = 'FirstProject.settings'
　　　　　　application = django.core.handlers.wsgi.WSGIHandler()
　　　　解释一下第4、5两行:
　　　　第四行中的 D:/WorkSpace/DjangoProject 是您的工程目录D:/WorkSpace/DjangoProject/FirstProject 的上一层目录。根据您工程的目录情况做修改。
　　　　第五行 os.environ['DJANGO_SETTINGS_MODULE'] = 'FirstProject.settings' ，这个FirstProject.settings就是指您的工程目录下的setting文件，根据您的工程目录名称的不同需要做修改。
　　　　最后，这个django.wsgi文件名可以随便取，例如test.wsgi、app.wsgi等等，但是一定要与2) 中的那行配置保持一致。如果您这里新建的文件名不是django.wsgi而是test.wsgi，那么2) 中的那行配置就应该修改为：WSGIScriptAlias / "D:/WorkSpace/DjangoProject/FirstProject/django.wsgi"。
　　5)最后修改项目的setting.py文件。
　　找到您工程目录下的setting.py，对于我这里就是D:\WorkSpace\DjangoProject\FirstProject\setting.py。找到其中的TEMPLATE_DIRS，默认为：
TEMPLATE_DIRS = ("Template",)
　　修改为如下：
TEMPLATE_DIRS = ("D:\WorkSpace\DjangoProject\FirstProject",)
 　　这个D:\WorkSpace\DjangoProject\FirstProject就是您的工程目录，根据您的实际情况做相应的修改。到这里，整个配置就基本完成了。
　　四、其它信息
　　　　这个是关于mod_wsgi实现的一些配置命令的详细解释：[http://code.google.com/p/modwsgi/wiki/ConfigurationDirectives](http://code.google.com/p/modwsgi/wiki/ConfigurationDirectives)，例如我们之前配置中的WSGIScriptAlias到底是一个什么意思等，需要的时候可以做一个参考。
