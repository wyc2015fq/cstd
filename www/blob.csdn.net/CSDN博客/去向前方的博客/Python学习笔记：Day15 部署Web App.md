# Python学习笔记：Day15 部署Web App - 去向前方的博客 - CSDN博客





2018年07月10日 09:53:13[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：173








# 前言

最近在学习深度学习，已经跑出了几个模型，但Pyhton的基础不够扎实，因此，开始补习Python了，大家都推荐廖雪峰的课程，因此，开始了学习，但光学有没有用，还要和大家讨论一下，因此，写下这些帖子，廖雪峰的课程连接在这里：[廖雪峰](https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000)

Python的相关介绍，以及它的历史故事和运行机制，可以参见这篇：[python介绍](https://blog.csdn.net/lvsehaiyang1993/article/details/80644237)

Python的安装可以参见这篇：[Python安装](https://blog.csdn.net/lvsehaiyang1993/article/details/80644321)

Python的运行模式以及输入输出可以参见这篇：[Python IO](https://blog.csdn.net/lvsehaiyang1993/article/details/80644827)

Python的基础概念介绍，可以参见这篇：[Python 基础](https://blog.csdn.net/lvsehaiyang1993/article/details/80647010)

Python字符串和编码的介绍，可以参见这篇：[Python字符串与编码](https://blog.csdn.net/lvsehaiyang1993/article/details/80648947)

Python基本数据结构：list和tuple介绍，可以参见这篇：[Python list和tuple](https://blog.csdn.net/lvsehaiyang1993/article/details/80649384)

Python控制语句介绍：ifelse，可以参见这篇：[Python 条件判断](https://blog.csdn.net/lvsehaiyang1993/article/details/80649552)

Python控制语句介绍：循环实现，可以参见这篇：[Python循环语句](https://blog.csdn.net/lvsehaiyang1993/article/details/80651808)

Python数据结构：dict和set介绍[Python数据结构dict和set](https://blog.csdn.net/lvsehaiyang1993/article/details/80652307)

Python函数相关：[Python函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80654173)

Python高阶特性：[Python高级特性](https://blog.csdn.net/lvsehaiyang1993/article/details/80661973)

Python高阶函数：[Python高阶函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80665892)

Python匿名函数：[Python匿名函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80667629)

Python装饰器：[Python装饰器](https://blog.csdn.net/lvsehaiyang1993/article/details/80667177)

Python偏函数：[Python偏函数](https://blog.csdn.net/lvsehaiyang1993/article/details/80673522)

Python模块：[Python模块](https://blog.csdn.net/lvsehaiyang1993/article/details/80673944)

Python面向对象编程（1）：[Python面向对象](https://blog.csdn.net/lvsehaiyang1993/article/details/80674239)

Python面向对象编程（2）：[Python面向对象（2）](https://blog.csdn.net/lvsehaiyang1993/article/details/80675404)

Python面向对象编程（3）：[Python面向对象（3）](https://blog.csdn.net/lvsehaiyang1993/article/details/80675873)

Python面向对象编程（4）：[Pyhton面向对象（4）](https://blog.csdn.net/lvsehaiyang1993/article/details/80676298)

Python面向对象高级编程（上）：[Python面向对象高级编程（上）](https://blog.csdn.net/lvsehaiyang1993/article/details/80688026)

Python面向对象高级编程（中上）：[Python面向对象高级编程（中上）](https://blog.csdn.net/lvsehaiyang1993/article/details/80689908)

Python面向对象高级编程（中下）：[Python面向对象高级编程（中下）](https://blog.csdn.net/lvsehaiyang1993/article/details/80691702)

Python面向对象高级编程（完）：[Python面向对象高级编程（完）](https://blog.csdn.net/lvsehaiyang1993/article/details/80692002)

Python错误调试（起）：[Python调试：起](https://blog.csdn.net/lvsehaiyang1993/article/details/80694444)

Python错误调试（承）：[Python调试：承](https://blog.csdn.net/lvsehaiyang1993/article/details/80694819)

Python错误调试（转）：[Python调试：转](https://blog.csdn.net/lvsehaiyang1993/article/details/80695280)

Python错误调试（合）：[python调试：合](https://blog.csdn.net/lvsehaiyang1993/article/details/80697743)

Python文件IO编程：[Python文件IO](https://blog.csdn.net/lvsehaiyang1993/article/details/80700751)

Python文件IO编程2：[Python文件IO2](https://blog.csdn.net/lvsehaiyang1993/article/details/80707102)

Python文件IO编程3：[PYthon文件IO3](https://blog.csdn.net/lvsehaiyang1993/article/details/80707554)

Python进程和线程（起）：[Python进程和线程起](https://blog.csdn.net/lvsehaiyang1993/article/details/80709180)

Python进程和线程（承）：[Python进程和线程承](https://blog.csdn.net/lvsehaiyang1993/article/details/80709260)

Python进程和线程（转）：[Python进程和线程转](https://blog.csdn.net/lvsehaiyang1993/article/details/80709353)

Python进程和线程（合）：[Python进程和线程合](https://blog.csdn.net/lvsehaiyang1993/article/details/80709405)

Python正则表达式：[Python正则表达式](https://blog.csdn.net/lvsehaiyang1993/article/details/80711331)

Python学习笔记:常用内建模块1：[Python学习笔记：常用内建模块1](https://blog.csdn.net/lvsehaiyang1993/article/details/80712912)

Python学习笔记:常用内建模块2：[Python学习笔记：常用内建模块2](https://blog.csdn.net/lvsehaiyang1993/article/details/80713218)

Python学习笔记:常用内建模块3：[Python学习笔记：常用内建模块3](https://blog.csdn.net/lvsehaiyang1993/article/details/80713747)

Python学习笔记:常用内建模块4：[Python学习笔记:   常用内建模块4](https://blog.csdn.net/lvsehaiyang1993/article/details/80716665)

Python学习笔记:常用内建模块5：[Python学习笔记:   常用内建模块5](https://blog.csdn.net/lvsehaiyang1993/article/details/80716912)

Python学习笔记:常用内建模块6:[Python学习笔记：常用内建模块6](https://blog.csdn.net/lvsehaiyang1993/article/details/80717102)

Python学习笔记:第三方模块1：[Python常用第三方模块](https://blog.csdn.net/lvsehaiyang1993/article/details/80726046)

Python学习笔记:第三方模块2：[Python常用第三方模块](https://blog.csdn.net/lvsehaiyang1993/article/details/80726301)

Python学习笔记:第三方模块3：[Python常用第三方模块](https://blog.csdn.net/lvsehaiyang1993/article/details/80726378)

Pytho学习笔记：网络编程：[Python网络编程](https://blog.csdn.net/lvsehaiyang1993/article/details/80729939)

Python学习笔记：电子邮件：[Python电子邮件1](https://blog.csdn.net/lvsehaiyang1993/article/details/80731086)

Python学习笔记：SMTP服务器：[PythonSMTP服务器](https://blog.csdn.net/lvsehaiyang1993/article/details/80731573)

Python学习笔记：POP3服务器：[PythonPOP3服务器](https://blog.csdn.net/lvsehaiyang1993/article/details/80731697)

Python学习笔记：Python数据库 [Python数据库1](https://blog.csdn.net/lvsehaiyang1993/article/details/80742388)

Python学习笔记：Python数据库2[Python数据库2](https://blog.csdn.net/lvsehaiyang1993/article/details/80747424)

Python学习笔记：web开发1[Python学习笔记：web开发1](https://blog.csdn.net/lvsehaiyang1993/article/details/80765955)

Python学习笔记：web开发2[Python学习笔记: web开发2](https://blog.csdn.net/lvsehaiyang1993/article/details/80778507)

Python学习笔记:  web开发3[Python学习笔记: web开发3](https://blog.csdn.net/lvsehaiyang1993/article/details/80779521)

Python学习笔记：异步IO（1）[Python学习笔记：异步IO（1）](https://blog.csdn.net/lvsehaiyang1993/article/details/80789370)

Python学习笔记：异步IO（2）[Python学习笔记：异步IO（2）](https://blog.csdn.net/lvsehaiyang1993/article/details/80789574)

Python学习笔记：异步IO（3）[Python学习笔记：异步IO（3）](https://blog.csdn.net/lvsehaiyang1993/article/details/80795526)

Python学习笔记：Day 1-2开发[Python学习笔记：Day1-2开发](https://blog.csdn.net/lvsehaiyang1993/article/details/80807565)

Python学习笔记：Day 3 ORM[Python学习笔记：Day3 ORM](https://blog.csdn.net/lvsehaiyang1993/article/details/80842307)

Python学习笔记：Day 4 Model[Python学习笔记：Day4Model](https://blog.csdn.net/lvsehaiyang1993/article/details/80845475)

Python学习笔记：Day 5 web框架[PYTHON学习笔记：DAy5](https://blog.csdn.net/lvsehaiyang1993/article/details/80866237)

Python学习笔记：Day 6 配置文件[Python学习笔记：Day 6 配置文件](https://blog.csdn.net/lvsehaiyang1993/article/details/80875144)

Python学习笔记：Day 7 编写MVC[Python学习笔记：Day 7编写MVC](https://blog.csdn.net/lvsehaiyang1993/article/details/80880125)

Python学习笔记：Day 8 构建前端[Python学习笔记:Day 8 构建前端](https://blog.csdn.net/lvsehaiyang1993/article/details/80899296)

Python学习笔记：Day 9 编写API[Python 9编写API](https://blog.csdn.net/lvsehaiyang1993/article/details/80914717)

Python学习笔记：Day 10 用户注册和登陆[Python 10用户注册和登陆](https://blog.csdn.net/lvsehaiyang1993/article/details/80921837)

Python学习笔记：Day11 编写日志创建页[day11 编写目录](https://blog.csdn.net/lvsehaiyang1993/article/details/80935496)

Python学习笔记：Day12 编写日志列表页[day12 编写日志](https://blog.csdn.net/lvsehaiyang1993/article/details/80949143)

Python学习笔记：Day13 提升开发效率[开发效率](https://blog.csdn.net/lvsehaiyang1993/article/details/80956748)

Python学习笔记：Day14 完成Web App[完成web app](https://blog.csdn.net/lvsehaiyang1993/article/details/80972844)
# 目录


- [前言](#前言)
- [目录](#目录)
- [部署web app](#部署web-app)
- [搭建Linux服务器](#搭建linux服务器)
- [部署方式](#部署方式)
- [部署](#部署)
- [配置Supervisor](#配置supervisor)
- [配置Nginx](#配置nginx)



# 部署web app

作为一个合格的开发者，在本地环境下完成开发还远远不够，我们需要把Web App部署到远程服务器上，这样，广大用户才能访问到网站。

很多做开发的同学把部署这件事情看成是运维同学的工作，这种看法是完全错误的。首先，最近流行[devOps理念](https://zh.wikipedia.org/wiki/DevOps)理念，就是说，开发和运维要变成一个整体。其次，运维的难度，其实跟开发质量有很大的关系。代码写得垃圾，运维再好也架不住天天挂掉。最后，DevOps理念需要把运维、监控等功能融入到开发中。你想服务器升级时不中断用户服务？那就得在开发时考虑到这一点。

下面，我们就来把awesome-python3-webapp部署到Linux服务器。

# 搭建Linux服务器

要部署到Linux，首先得有一台Linux服务器。要在公网上体验的同学，可以在Amazon的[AWZ](https://aws.amazon.com/cn/)申请一台EC2虚拟机（免费使用1年），或者使用国内的一些云服务器，一般都提供Ubuntu Server的镜像。想在本地部署的同学，请安装虚拟机，推荐使用VirtualBox[file](https://www.virtualbox.org/)。

我们选择的Linux服务器版本是[Ubuntu server 14.04 LTS](https://www.ubuntu.com/download/server)，原因是apt太简单了。如果你准备使用其他Linux版本，也没有问题。

Linux安装完成后，请确保ssh服务正在运行，否则，需要通过apt安装：

```
$ sudo apt-get install openssh-server
```

有了ssh服务，就可以从本地连接到服务器上。建议把公钥复制到服务器端用户的.ssh/authorized_keys中，这样，就可以通过证书实现无密码连接。

# 部署方式

利用Python自带的asyncio，我们已经编写了一个异步高性能服务器。但是，我们还需要一个高性能的Web服务器，这里选择Nginx，它可以处理静态资源，同时作为反向代理把动态请求交给Python代码处理。这个模型如下：

![这里写图片描述](https://img-blog.csdn.net/20180710093520829?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Nginx负责分发请求：

![这里写图片描述](https://img-blog.csdn.net/20180710093557350?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在服务器端，我们需要定义好部署的目录结构：

```
/
+- srv/
   +- awesome/       <-- Web App根目录
      +- www/        <-- 存放Python源码
      |  +- static/  <-- 存放静态资源文件
      +- log/        <-- 存放log
```

在服务器上部署，要考虑到新版本如果运行不正常，需要回退到旧版本时怎么办。每次用新的代码覆盖掉旧的文件是不行的，需要一个类似版本控制的机制。由于Linux系统提供了软链接功能，所以，我们把www作为一个软链接，它指向哪个目录，哪个目录就是当前运行的版本：

![这里写图片描述](https://img-blog.csdn.net/20180710093633440?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

而Nginx和python代码的配置文件只需要指向www目录即可。

Nginx可以作为服务进程直接启动，但app.py还不行，所以，[可以随系统启动而启动](http://supervisord.org/)登场！Supervisor是一个管理进程的工具，可以随系统启动而启动服务，它还时刻监控服务进程，如果服务进程意外退出，Supervisor可以自动重启服务。

总结一下我们需要用到的服务有：

•Nginx：高性能Web服务器+负责反向代理；

•Supervisor：监控服务进程的工具；

•MySQL：数据库服务。

在Linux服务器上用apt可以直接安装上述服务：

```
$ sudo apt-get install nginx supervisor python3 mysql-server
```

然后，再把我们自己的Web App用到的Python库安装了：

```bash
$ sudo pip3 install jinja2 aiomysql aiohttp
```

在服务器上创建目录/srv/awesome/以及相应的子目录。

在服务器上初始化MySQL数据库，把数据库初始化脚本schema.sql复制到服务器上执行：

```
$ mysql -u root -p < schema.sql
```

服务器端准备就绪。

# 部署

用FTP还是SCP还是rsync复制文件？如果你需要手动复制，用一次两次还行，一天如果部署50次不但慢、效率低，而且容易出错。

正确的部署方式是使用工具配合脚本完成自动化部署。[Fabric](http://www.fabfile.org/)就是一个自动化部署工具。由于Fabric是用Python 2.x开发的，所以，部署脚本要用Python 2.7来编写，本机还必须安装Python 2.7版本。

要用Fabric部署，需要在本机（是开发机器，不是Linux服务器）安装Fabric：

```
$ easy_install fabric
```

Linux服务器上不需要安装Fabric，Fabric使用SSH直接登录服务器并执行部署命令。

下一步是编写部署脚本。Fabric的部署脚本叫fabfile.py，我们把它放到awesome-python-webapp的目录下，与www目录平级：

```
awesome-python-webapp/
+- fabfile.py
+- www/
+- ...

Fabric的脚本编写很简单，首先导入Fabric的API，设置部署时的变量：

# fabfile.py
import os, re
from datetime import datetime

# 导入Fabric API:
from fabric.api import *

# 服务器登录用户名:
env.user = 'michael'
# sudo用户为root:
env.sudo_user = 'root'
# 服务器地址，可以有多个，依次部署:
env.hosts = ['192.168.0.3']

# 服务器MySQL用户名和口令:
db_user = 'www-data'
db_password = 'www-data'
```

然后，每个Python函数都是一个任务。我们先编写一个打包的任务：

```python
_TAR_FILE = 'dist-awesome.tar.gz'

def build():
    includes = ['static', 'templates', 'transwarp', 'favicon.ico', '*.py']
    excludes = ['test', '.*', '*.pyc', '*.pyo']
    local('rm -f dist/%s' % _TAR_FILE)
    with lcd(os.path.join(os.path.abspath('.'), 'www')):
        cmd = ['tar', '--dereference', '-czvf', '../dist/%s' % _TAR_FILE]
        cmd.extend(['--exclude=\'%s\'' % ex for ex in excludes])
        cmd.extend(includes)
        local(' '.join(cmd))
```

Fabric提供local(‘…’)来运行本地命令，with lcd(path)可以把当前命令的目录设定为lcd()指定的目录，注意Fabric只能运行命令行命令，Windows下可能需要Cgywin环境。

在awesome-python-webapp目录下运行：

```
$ fab build
```

看看是否在dist目录下创建了dist-awesome.tar.gz的文件。

打包后，我们就可以继续编写deploy任务，把打包文件上传至服务器，解压，重置www软链接，重启相关服务：

```python
_REMOTE_TMP_TAR = '/tmp/%s' % _TAR_FILE
_REMOTE_BASE_DIR = '/srv/awesome'

def deploy():
    newdir = 'www-%s' % datetime.now().strftime('%y-%m-%d_%H.%M.%S')
    # 删除已有的tar文件:
    run('rm -f %s' % _REMOTE_TMP_TAR)
    # 上传新的tar文件:
    put('dist/%s' % _TAR_FILE, _REMOTE_TMP_TAR)
    # 创建新目录:
    with cd(_REMOTE_BASE_DIR):
        sudo('mkdir %s' % newdir)
    # 解压到新目录:
    with cd('%s/%s' % (_REMOTE_BASE_DIR, newdir)):
        sudo('tar -xzvf %s' % _REMOTE_TMP_TAR)
    # 重置软链接:
    with cd(_REMOTE_BASE_DIR):
        sudo('rm -f www')
        sudo('ln -s %s www' % newdir)
        sudo('chown www-data:www-data www')
        sudo('chown -R www-data:www-data %s' % newdir)
    # 重启Python服务和nginx服务器:
    with settings(warn_only=True):
        sudo('supervisorctl stop awesome')
        sudo('supervisorctl start awesome')
        sudo('/etc/init.d/nginx reload')
```

注意run()函数执行的命令是在服务器上运行，with cd(path)和with lcd(path)类似，把当前目录在服务器端设置为cd()指定的目录。如果一个命令需要sudo权限，就不能用run()，而是用sudo()来执行。

# 配置Supervisor

上面让Supervisor重启awesome的命令会失败，因为我们还没有配置Supervisor呢。

编写一个Supervisor的配置文件awesome.conf，存放到/etc/supervisor/conf.d/目录下：

```
[program:awesome]

command     = /srv/awesome/www/app.py
directory   = /srv/awesome/www
user        = www-data
startsecs   = 3

redirect_stderr         = true
stdout_logfile_maxbytes = 50MB
stdout_logfile_backups  = 10
stdout_logfile          = /srv/awesome/log/app.log
```

配置文件通过[program:awesome]指定服务名为awesome，command指定启动app.py。

然后重启Supervisor后，就可以随时启动和停止Supervisor管理的服务了：

```bash
$ sudo supervisorctl reload
$ sudo supervisorctl start awesome
$ sudo supervisorctl status
awesome                RUNNING    pid 1401, uptime 5:01:34
```

# 配置Nginx

Supervisor只负责运行app.py，我们还需要配置Nginx。把配置文件awesome放到/etc/nginx/sites-available/目录下：

```bash
server {
    listen      80; # 监听80端口

    root       /srv/awesome/www;
    access_log /srv/awesome/log/access_log;
    error_log  /srv/awesome/log/error_log;

    # server_name awesome.liaoxuefeng.com; # 配置域名

    # 处理静态文件/favicon.ico:
    location /favicon.ico {
        root /srv/awesome/www;
    }

    # 处理静态资源:
    location ~ ^\/static\/.*$ {
        root /srv/awesome/www;
    }

    # 动态请求转发到9000端口:
    location / {
        proxy_pass       http://127.0.0.1:9000;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header Host $host;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
    }
}
```

然后在/etc/nginx/sites-enabled/目录下创建软链接：

```bash
$ pwd
/etc/nginx/sites-enabled
$ sudo ln -s /etc/nginx/sites-available/awesome .
```

让Nginx重新加载配置文件，不出意外，我们的awesome-python3-webapp应该正常运行：

```bash
$ sudo /etc/init.d/nginx reload
```

如果有任何错误，都可以在/srv/awesome/log下查找Nginx

和App本身的log。如果Supervisor启动时报错，可以在/var/log/supervisor下查看Supervisor的 
![这里写图片描述](https://img-blog.csdn.net/20180710094354430?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)og。
如果一切顺利，你可以在浏览器中访问Linux服务器上的awesome-python3-webapp了：

如果在开发环境更新了代码，只需要在命令行执行：

```
$ fab build
$ fab deploy
```

自动部署完成！刷新浏览器就可以看到服务器代码更新后的效果。

友情链接 

嫌国外网速慢的童鞋请移步网易和搜狐的镜像站点：

[http://mirrors.163.com/](http://mirrors.163.com/)

[http://mirrors.sohu.com/](http://mirrors.sohu.com/)





