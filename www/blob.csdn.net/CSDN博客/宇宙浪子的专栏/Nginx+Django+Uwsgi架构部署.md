# Nginx+Django+Uwsgi架构部署 - 宇宙浪子的专栏 - CSDN博客
2016年03月29日 18:34:55[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：634
一、Django环境部署
官网下载django版本，目前最新版为1.7版本，https://www.djangoproject.com/
然后解压安装，我这里安装完后，新建的app主目录为：/root/django/django/bin/command
[root@Linux_wugk_SA command]# pwd
/root/django/django/bin/command
root@Linux_wugk_SA command]# ls
command  manage.py 
[root@Linux_wugk_SA command]#
二、Uwsgi安装部署
下载uwsgi安装包：wget http://projects.unbit.it/downloads/uwsgi-2.0.6.tar.gz
解压安装：
tar xzfuwsgi-2.0.6.tar.gz ;
cd uwsgi-2.0.6/ ;
python setup.py install
即可。
安装完毕后，在/root/django/django/bin/command目录(与manage.py同级目录)新建如下文件：vi  django_wsgi.py
#!/usr/bin/env python
# coding: utf-8
import os
import sys
reload(sys)
sys.setdefaultencoding('utf8')
os.environ.setdefault("DJANGO_SETTINGS_MODULE","command.settings")
from django.core.handlers.wsgiimport WSGIHandler
application = WSGIHandler()
然后新建uwsgi配置文件,内容如下：vi uwsgi_socket.xml
<uwsgi>
  <socket>127.0.0.1:199</socket>
        <chdir>/root/django/django/bin/command</chdir>
          <module>django_wsgi</module>
              <processes>4</processes>
  <daemonize>uwsgi.log</daemonize>
</uwsgi>
注* chdir目录写当前目录，即/root/django/django/bin/command
三、Nginx部署配置
Nginx安装大家都非常熟悉，这里直接上Server段配置代码：
server {
        listen 80;
        server_name localhost;
        index index.php index.html;
        location / {
            include    uwsgi_params;
            uwsgi_pass  127.0.0.1:199;
            uwsgi_param UWSGI_CHDIR  /root/django/django/bin/command;
            uwsgi_param UWSGI_SCRIPTdjango_wsgi;
            access_log/usr/local/nginx/logs/access.log;
            }
        location /static {
          root /root/django/django/bin/command/command;
        }
    }
四、部署测试
分别启动uwsgi和nginx即可，然后通过浏览器直接访问Nginx 80端口即可。
最终截图测试如下：
![Nginx+Django+Uwsgi架构部署](http://www.linuxidc.com/upload/2014_09/14091918589700.jpg)
[Ubuntu](http://www.linuxidc.com/topicnews.aspx?tid=2) 12.04 下部署 Nginx+uWSGI+Flask [http://www.linuxidc.com/Linux/2014-01/96007.htm](http://www.linuxidc.com/Linux/2014-01/96007.htm)
你应该使用 Nginx + uWSGI [http://www.linuxidc.com/Linux/2013-07/87286.htm](http://www.linuxidc.com/Linux/2013-07/87286.htm)
uWSGI + Nginx 部署 Flask Web 应用 [http://www.linuxidc.com/Linux/2013-06/85828.htm](http://www.linuxidc.com/Linux/2013-06/85828.htm)
Django+Nginx+uWSGI 部署 [http://www.linuxidc.com/Linux/2013-02/79862.htm](http://www.linuxidc.com/Linux/2013-02/79862.htm)
Linux下Nginx+uWSGI部署Python应用 [http://www.linuxidc.com/Linux/2012-10/72443.htm](http://www.linuxidc.com/Linux/2012-10/72443.htm)
Ubuntu Server 12.04 安装Nginx+uWSGI+Django环境 [http://www.linuxidc.com/Linux/2012-05/60639.htm](http://www.linuxidc.com/Linux/2012-05/60639.htm)
[CentOS](http://www.linuxidc.com/topicnews.aspx?tid=14) 5.5 + Nginx 0.8.50 + uWSGI + Django 1.2.3 部署Django项目 [http://www.linuxidc.com/Linux/2011-05/36399.htm](http://www.linuxidc.com/Linux/2011-05/36399.htm)
**Nginx 的详细介绍**：[请点这里](http://www.linuxidc.com/Linux/2012-03/56786.htm)
**Nginx 的下载地址**：[请点这里](http://www.linuxidc.com/down.aspx?id=342)
**本文永久更新链接地址**：[http://www.linuxidc.com/Linux/2014-09/106928.htm](http://www.linuxidc.com/Linux/2014-09/106928.htm)
