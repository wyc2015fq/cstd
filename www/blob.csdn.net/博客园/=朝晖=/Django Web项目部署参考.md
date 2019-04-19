# Django Web项目部署参考 - =朝晖= - 博客园
# [Django Web项目部署参考](https://www.cnblogs.com/dhcn/p/7116104.html)
环境准备：
1、Python 2.7.*]
2.pip
3.sudo pip install django
4、sudo aptitude show python-setuptools
5、sudo aptitude install python-psycopg2
6、#Ckeditor已完全下线，故此条作废#Django-Ckedior的子安装项pillow依赖于gcc和python-dev
7、pillow的依赖项PIL编译过程中缺乏jpeg support的解决办法：http://jj.isgeek.net/2011/09/install-pil-with-jpeg-support-on-ubuntu-oneiric-64bits/  备份地址 ：http://blog.csdn.net/dipolar/article/details/20059357
8、aptitude install lxml 
9、pip install uwsgi 
环境准备的总结：环境贮备主要是安装依赖项，这些依赖项主要分以下3中：1、系统aptitude可安装的、2、pip可安装的;3、自己下载包install。本地程序优先aptitude安装，但是如果系统源的包实现质量有问题或者安装过程中需要的一些编译质量要求无法达到，则选择pip install, python包优先Pip安装，对于编译质量的问题，某些编译依赖项需要特殊处理，比如上面的pillow安装。最后的自己下包安装只适用于一些个人开发的小包，一般这种包尽量打进项目代码中
10、PostgreSQL的准备:1.登录用户必须用create user指令创建,2、用pg_hba.conf来设置可连接IP和密码方式，3、主配置文件中设置服务器的绑定IP；然后syncdb，数据模式，然后灌预备数据，比如城市和工种
-------------------------------------------------------------------------------------------
项目部署过程：
1、应用准备:确保应用使用runserver启动没问题，且功能正常可用。
2、nginx+uywsgi的部署：
先说说uwsgi的启动，uwsgi是最容易出问题的，出问题的原因主要以下两者：1、uwsgi启动应用的时候加载过程比上面的runserver深入全面的多，所以如果系统有编码问题，会在启动的时候就发现，导致uwsgi启动失败，一般来讲这个问题可以通过看uwsgi的log解决.2、uwsgi本身的启动问题，这种本身的问题主要原因是uwsgi的设置文件的问题，这个问题的日志输出不是那么明显，可以通过把配置项全面命令行化看错误输出来判断，命令行的方法看uwsgi的help和这个Django文档：https://docs.djangoproject.com/en/1.5/howto/deployment/wsgi/uwsgi/ 
说说nginx的问题:昨天部署的时候nginx的问题比较诡异，主要问题如下：1、nginx的配置规则是后向优先，也就是后面的优先于前面的，这个规则之前文档里看过，昨天部署时受django配置的影响，规则顺序有误。2、就是alias命令出现异常，最后暂时用root解决了问题.
最后说说nginx与uwsgi的通信，这个简直就是无方法解的问题，因为出了错误，日志基本没什么用，一般的容易出的问题是uwsgi和http协议问题，一般来讲，这方面问题都会归在uwsgi server的问题。]
ubuntu本地wusgi的使用需要[Python](http://lib.csdn.net/base/python)-plugin: http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/dipolar/article/details/37562103
3、nginx静态文件部署：昨天发生就是alias指令访问index diretory forbidden的问题，最后通过目录调整和root指令暂时解决了问题。
4、uwsgi启动日志出现“No module named uwsgi”这个错误不要理会，不影响系统的正常启动和运行，具体解释见： http://www.cnblogs.com/lazyboy/archive/2013/06/03/3115451.html

