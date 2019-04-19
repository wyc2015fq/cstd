# Django uwsgi nginx pypy 环境搭建 - walkabc - 博客园
# [Django uwsgi nginx pypy 环境搭建](https://www.cnblogs.com/hitfire/articles/4508118.html)
　　最近比较感兴趣使用Python进行Web开发。之前比较关注Pypy，所以这次就使用作为Web应用的运行环境。
　　下面说一下整体环境的搭建；
　　1.安装Pypy
　　Pypy的官网地址是[http://pypy.org/download.html](http://pypy.org/download.html)，下载完之后，解压缩到指定的文件夹。一般我会把所有的开发软件放在一个tools的文件夹下。
　　解压缩Pypy之后，目录结构是tools/pypy。
　　2.安装pip，pip是Pypy的包管理工具，方便我们安装第三方模块。
　　pip的官方地址是[https://pip.pypa.io/en/stable/installing.html](https://pip.pypa.io/en/stable/installing.html)，下载完之后，执行命令，
　　pypy get-pip.py。
　　执行完之后，pip就已经安装完成了,pip命令会出现在pypy命令同一个目录下。
　　3.使用pip安装uwsgi
　　执行命令pip install uwsgi，执行完之后uwsgi就安装完成了,uwsgi命令也会出现在pypy命令同一个目录下
　　4.使用pip安装django
　　pip install django
　　5.安装nginx，我使用的是Ubuntu14.04，所以直接通过apt-get来安装
　　apt-get install nginx
　　需要的软件都已经安装完成了。
　　接着接合他们一起工作。
　　1.使用django创建一个工程。
　　django-admin.py startproject mysite，这个命令执行后，mysite文件夹里面还有一个mysite文件夹，为了方便描述，我们先称前一个mysite为mysite1，后面的mysite2
　　想让uwsgi和nginx，还需要一个uwsgi_params文件，这个文件是给nginx使用，里面定义了一些变量，这些变量都是通过nginx传递给uwsgi的。
　　到[https://github.com/nginx/nginx/blob/master/conf/uwsgi_params](https://github.com/nginx/nginx/blob/master/conf/uwsgi_params)下载好之后，把这个文件拷贝到/etc/nginx文件夹下
　　编写uwsgi配置文件mysite.ini
```
# mysite_uwsgi.ini file
[uwsgi]
# Django-related settings
# the base directory (full path)
chdir        = /path/mysite
# the virtualenv (full path)
pypy-home    = /path/pypy
# Django's wsgi file
pypy-wsgi-file    = mysite/wsgi.py
# process-related settings
# master
master          = true
# maximum number of worker processes
processes       = 10
# the socket (use the full path to be safe
socket          = 127.0.0.1:8000
# ... with appropriate permissions - may be needed
# chmod-socket    = 664
# clear environment on exit
#vacuum          = true
```
　　需要特别注意的是：这个ini文件要放在mysite1文件夹下
　　然后执行uwsgi --ini mysite.ini,这样uwsgi进程就已经启动了。监听的端口是8000
　　下面开始配置nginx
```
# mysite_nginx.conf
# the upstream component nginx needs to connect to
upstream django {
    # server unix:///path/to/your/mysite/mysite.sock; # for a file socket
    server 127.0.0.1:8000; # for a web port socket (we'll use this first)
}
# configuration of the server
server {
    # the port your site will be served on
    listen      80;
    # the domain name it will serve for
    server_name www.mysite.dev; # substitute your machine's IP address or FQDN
    charset     utf-8;
    # max upload size
    client_max_body_size 75M;   # adjust to taste
    # Django media
    location /media  {
        alias /path/to/your/mysite/media;  # your Django project's media files - amend as required
    }
    location /static {
        alias /path/to/your/mysite/static; # your Django project's static files - amend as required
    }
    # Finally, send all non-media requests to the Django server.
    location / {
        uwsgi_pass  django;
        include     uwsgi_params; # the uwsgi_params file you installed
    }
}
```
　　接下来就可以通过www.mysite.dev/admin/就可以正常访问了。
　　参考地址：[http://uwsgi-docs.readthedocs.org/en/latest/tutorials/Django_and_nginx.html](http://uwsgi-docs.readthedocs.org/en/latest/tutorials/Django_and_nginx.html)

