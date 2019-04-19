# 五步教你实现使用Nginx+uWSGI+Django方法部署Django程序(下) - 宇宙浪子的专栏 - CSDN博客
2016年03月29日 11:47:53[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：2018
转自：[http://www.django-china.cn/topic/124/](http://www.django-china.cn/topic/124/)
在上一篇文章[《五步教你实现使用Nginx+uWSGI+Django方法部署Django程序(上)》](http://django-china.cn/topic/101/)中，阐述了如何只使用uWSGI来部署Django程序。
当然，单单只有uWSGI是不够的，在实际的部署环境中，Nginx是必不可少的工具。
在本篇文章中，我将一直延用“N步法”的风格来阐述如何将uWSGI与Nginx做连接来部署Django程序。并在最后，会较为完整的阐述本社区的部署方法。
**本文大纲：**
- 环境介绍
- 配置uWSGI
- 配置Nginx
- Nginx+uWSGI+Django的实现方式
- 一些建议
## 环境介绍
- Ubuntu 12.04.1 LTS
- django 1.4.2
- nginx/1.2.6
- uWSGI 1.4.4
关于uWSGI的安装可参见上一篇文章 上一篇文章[《五步教你实现使用Nginx+uWSGI+Django方法部署Django程序(上)》](http://django-china.cn/topic/101/)
我们假定你已经安装好Nginx了。
## 配置uWSGI
在上一篇文章[《五步教你实现使用Nginx+uWSGI+Django方法部署Django程序(上)》](http://django-china.cn/topic/101/)中,我们是直接使用命令行来启动uWSGI，在实际部署环境中，我们常用的是配置文件的方式，而非命令行的方式。我的一般做法是用命令行来测试是否uWSGI安装成功，然后用配置文件来真正部署。
另外，为了实现Nginx与uWSGI的连接，两者之间将采用soket来通讯方式。
在本节中，我们将使用uWSGI配置文件的方式来改进uWSGI的启动方式。
假定你的程序目录是 /home/work/src/sites/testdjango1/testdjango/mysite
我们将要让Nginx采用8077端口与uWSGI通讯，请确保此端口没有被其它程序采用。
注意，请确定你在上一节[《五步教你实现使用Nginx+uWSGI+Django方法部署Django程序(上)》](http://django-china.cn/topic/101/)中的django_wsgi.py文件已经存在了。
新建一个XML文件：
djangochina_socket.xml，将它放在 /home/work/src/sites/testdjango1/testdjango/mysite 目录下：
<uwsgi><socket>:8077</socket><chdir>/home/work/src/sites/testdjango1/testdjango/mysite</chdir><module>django_wsgi</module><processes>4</processes><!-- 进程数 --><daemonize>uwsgi.log</daemonize></uwsgi>
在上面的配置中，我们使用 uwsgi.log 来记录日志，开启4个进程来处理请求。
这样，我们就配置好uWSGI了。
## 配置Nginx
我们假设你将会把Nginx程序日志放到你的目录/home/work/var/test/logs/下，请确保该目录存在。
我们假设你的Django的static目录是/home/work/src/sites/testdjango1/testdjango/collectedstatic/ ， media目录是/home/work/src/sites/testdjango1/testdjango/public/media/，请确保这些目录存在。
我们假设你的域名是 [www.you.com](http://www.you.com/) （在调试时你可以设置成你的机器IP）
我们假设你的域名端口是 80（在调试时你可以设置一些特殊端口如 8070）
基于上面的假设，我们为conf/nginx.conf添加以下配置
server{listen80;server_namewww.you.com;access_log/home/work/var/test/logs/access.log;error_log/home/work/var/test/logs/error.log;#charsetkoi8-r;#access_loglogs/host.access.logmain;location/{includeuwsgi_params;uwsgi_pass127.0.0.1:8077;}#error_page404/404.html;#redirectservererrorpagestothestaticpage/50x.html#error_page500502503504/50x.html;location=/50x.html{roothtml;}location/static/{alias/home/work/src/sites/testdjango1/testdjango/collectedstatic/;indexindex.htmlindex.htm;}location/media/{alias/home/work/src/sites/testdjango1/testdjango/public/media/;}}
在上面的设置后，可以让Nginx来处理静态文件(/static/ 和 /media/ ）。非静态文件请求Nginx会发给 socket 8077，然后让uWSGI来进行处理。
## Nginx+uWSGI+Django的实现方式
在完成上面配置后，需要按以下步骤来做：
- 
重启Nginx服务器，以使Nginx的配置生效。
nginx -s  reload
重启后检查Nginx日志是否有异常。
- 
启动uWSGI服务器
cd /home/work/src/sites/testdjango1/testdjango/mysite
uwsgi -x djangochina_socket.xml
检查日志 uwsgi.log 是否有异常发现。
- 
访问服务
基于上面的假设你的域名是[www.you.com](http://www.you.com/)
因此，我们访问 [www.you.com](http://www.you.com/)，如果发现程序与 单独使用Django启动的程序一模一样时，就说明成功啦！
- 
关闭服务的方法
将uWSGi进程杀死即可。
## 一些建议
- 
uWSG配置文件的进程数，可以根据实际情况分配。不要开得太大，否则机器可能会内存耗用太高。一般来说，对于一个小社区来说，4个进程已经足够了。
- 
一般情况下，可以编写一下 stop.sh 脚本 来关闭uWSGI。
## 最后
最后面，请大家要支持[Django中国社区](http://django-china.cn/)哦，单靠一两个人是不行的，一起推广一下，让Django社区更有力量哈！更有人气哈！
推广链接： [http://django-china.cn/](http://django-china.cn/)
