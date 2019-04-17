# Ubuntu Wiki指导安装Nginx +mysql+php - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年10月28日 16:13:13[boonya](https://me.csdn.net/boonya)阅读数：793标签：[ubuntu																[nginx																[mysql																[php](https://so.csdn.net/so/search/s.do?q=php&t=blog)
个人分类：[Linux																[Nginx](https://blog.csdn.net/boonya/article/category/2126311)](https://blog.csdn.net/boonya/article/category/1740759)





原文地址：[http://wiki.ubuntu.org.cn/Nginx](http://wiki.ubuntu.org.cn/Nginx)

## 安装nginx
sudo apt-get install nginx
Ubuntu安装之后的文件结构大致为：
- 所有的配置文件都在/etc/nginx下，并且每个虚拟主机已经安排在了/etc/nginx/sites-available下
- 程序文件在/usr/sbin/nginx
- 日志放在了/var/log/nginx中
- 并已经在/etc/init.d/下创建了启动脚本nginx
- 默认的虚拟主机的目录设置在了/var/www/nginx-default (有的版本 默认的虚拟主机的目录设置在了/var/www, 请参考/etc/nginx/sites-available里的配置)

## 启动nginx
sudo /etc/init.d/nginx start

然后就可以访问了，[http://localhost/](http://localhost/) ， 一切正常！如果不能访问，先不要继续，看看是什么原因，解决之后再继续。启动时候若显示端口80被占用：Starting nginx: [emerg]: bind() to 0.0.0.0:80 failed (98: Address
 already in use)修改文件：/etc/nginx/sites-available/default,去掉 listen 前面的 # 号 , # 号在该文件里是注释的意思 , 并且把 listen 后面的 80 端口号改为自己的端口，访问是需要添加端口号。

（安装完后如出现403错误，那可能是nginx配置文件里的网站路径不正确）
|![](http://wiki.ubuntu.org.cn/images/thumb/f/f0/Nginxdefault.png/500px-Nginxdefault.png)![](http://wiki.ubuntu.org.cn/skins/common/images/magnify-clip.png)nginx默认页面|
|----|

## 配置php和mysql

### 安装Php和mysql

安装php和MySQL:
sudo apt-get install php5-cli php5-cgi mysql-server php5-mysql

### 安装FastCgi

/usr/bin/spawn-fcgi这个文件来管理 FastCgi，它原属于lighttpd这个包里面，但 9.10 后，spawn-fcgi 被分离出来单独成包：
sudo apt-get install spawn-fcgi

### 配置 nginx

修改nginx的配置文件：/etc/nginx/sites-available/default修改主机名
 server_name localhost;

修改index的一行修改为：
index index.php index.html index.htm;

去掉下面部分的注释用于支持 php 脚本：
location ~ \.php$ {
include /etc/nginx/fastcgi_params; #需放在第一行，否则会出错
fastcgi_pass 127.0.0.1:9000;
fastcgi_index index.php;
fastcgi_param SCRIPT_FILENAME /var/www/nginx-default$fastcgi_script_name;
}

重新启动nginx:
/etc/init.d/nginx stop
/etc/init.d/nginx start

启动fastcgi php:
spawn-fcgi -a 127.0.0.1 -p 9000 -C 10 -u www-data -f /usr/bin/php-cgi

为了让php-cgi开机自启动：Ubuntu开机之后会执行/etc/rc.local文件中的脚本所以我们可以直接在/etc/rc.local中添加启动脚本。spawn-fcgi -a 127.0.0.1 -p 9000 -C 10 -u www-data -f /usr/bin/php-cgi添加到语句：exit 0 前面才行

创建、测试phpinfo：
sudo vi /var/www/nginx-default/info.php<?php phpinfo(); ?>
打开 [http://localhost/info.php](http://localhost/info.php) 。
|![](http://wiki.ubuntu.org.cn/images/thumb/7/76/Nginxphpinfo.png/600px-Nginxphpinfo.png)![](http://wiki.ubuntu.org.cn/skins/common/images/magnify-clip.png)Nginx phpinfo页面|
|----|

## 安装nginx +uwsgi+ Django

这次采用 uwsgi 1.2+django1.4+nginx1.1.19+ubuntu12.04组合进行安装

首先注意 要安装比较新的软件 如果你是ubuntu11.04和11.10 因为内核不同 所以安装软件版本不同

1.首先 安装nginx：sudo apt-get install nginx2.然后：进行uwsgi的安装之前 要安装其他几个：sudo apt-get install mysql-server libxml2 libxml2-dev python-dev libpcre3 libpcre3-dev python-MySQLdb 3.进行uwsgi的安装：可能由于软件版本不够新吧 ppa里头安装后出现502错误 所以 用软件包安装：
- wget [http://projects.unbit.it/downloads/uwsgi-1.2.tar.gz](http://projects.unbit.it/downloads/uwsgi-1.2.tar.gz)
- tar zxvf uwsgi-1.2.tar.gz
- cd uwsgi-1.2
- make
- cp uwsgi /usr/bin

注：如果在MAKE 过程中出现如：（Python.h：没有那个文件或目录） 
       解决方法：sudo apt-get install python-dev 

4.安装django之前我用ubuntu源安装.由于admin管理的静态问题.所以我还是改用下载安装比较好.google下django然后进入下载 [https://www.djangoproject.com/download/](https://www.djangoproject.com/download/)用chrome下载默认/home/helios/下载 然后cd然后cd 下载接着 tar xzvf Django-1.4.tar.gzcd Django-1.4sudo python setup.py install这是最新版本.copy官网的安装方法.接着 sudo cp django/bin/django-admin.py /usr/bin/接下来 sudo mv /usr/bin/django-admin.py /usr/bin/django-admin这是为了和源安装在操作上保持一致
 但是不知道有没有问题.暂时没有发现问题.有问题大家可以上来补教程.可以了.安装好了.

建立项目------------------------

1.cd2.django-admin startproject mysite

|-mysite|--mysite,manage.py|---__init__.py ,settings.py, urls.py

现在 就是刚刚说的admin静态文件的问题.我现在把刚刚下载的django安装包的解压文件夹里头 /home/helios/下载/Django-1.4/django/contrib/admin下static文件夹直接复制到mysite下



这样保持localhost/admin可以出现正常页面不再是没有样式的了.

另一种解决admin静态方法问题--------

sudo chmod -R 777 文件夹路径.这个是给予文件夹下所有子文件子文件夹权限.

那么可以从源安装django了sudo apt-get install python-django然后mkdir /home/helios/mysite/static/admincp -rf /usr/lib/python2.7/dist-packages/django/contrib/admin/media /home/helios/mysite/static/adminsudo chmod -R 777 /home/helios/mysite/static/admin这样就ok了

## 配置 nginx uwsgi django 

交代下环境：
 我的电脑用户名：helios；
 #sudo vim /etc/nginx/sites-available/default

出现nginx代码：(序列号不要抄) 

1.
 listen   80;

2.
 root     /home/helios/mysite;

3.
 server_name    youdomain.com#域名 或者当地localhost；

4.
 location / {
               uwsgi_pass    127.0.0.1:9000;
               include uwsgi_params;
               access_log  off;
 }

5.
 location /static/ {
               alias /home/helios/mysite/static;
               autoindex on;
               allow 127.0.0.1;
               deny all;
 }

6.
 location /static/admin/ {
               alias /home/helios/mysite/static/admin;
               autoindex on;
               allow 127.0.0.1;
               deny all;
 }

7.
 location ~ ^.+\.(gif|jpg|png|ico|jpeg)$ {
               expires 3d;
 }

8.
 location ~ ^.+\.(css|js)$ {
               expires 12h;
 }



然后
 #vim /etc/nginx/nginx.conf

1.
 user  helios;

2. 
 把所有gzip开头的前面#号都去掉,除了Gzip setting

现在 把nginx配置都配好了

##  uwsgi配置 
 #cd /home/helios/mysite
 #vim wsgi.py

写上
 import os
 import sys from django.core.handlers.wsgi 
 import WSGIHandler
 
 sys.path.append("/home/helios/mysite")
 os.environ["DJANGO_SETTINGS_MODULE"] = "mysite.settings"
 application = WSGIHandler()

完了以后
 #vim wsgi.xml

写上
 <uwsgi>
   <socket>127.0.0.1:9000</socket>
   <listen>80</listen>
   <master>true</master>
   <pidfile>/var/run/nginx.pid</pidfile>
   <processes>8</processes>
   <pythonpath>/home/helios/mysite</pythonpath>
   <pythonpath>/home/helios/</pythonpath>
   <module>wsgi</module>
   <profiler>true</profiler>
   <memory-report>true</memory-report>
   <enable-threads>true</enable-threads>
   <logdate>true</logdate>
   #这个是程序占用大小，不能太小，太大暂时还不知道 
   #在测试过程中，我的电脑300以下目测不行。
   #也可以删除这段，因为在测试过程中，如果这段设置太小
   #会出现问题libgcc_s.so.1 must be installed for pthread_cancel to work 
   #只有在 ubuntu 12.04测试过，其他系统还不清楚
   #为了不容易出错可以删掉，但是这个提示给有需要的朋友
   <limit-as>300</limit-as>
   #自己建立文件夹 然后创建空文件django.log这个灵活的
   <daemonize>/home/helios/mysite/log/django.log</daemonize>
 </uwsgi>

## 配置django 

刚刚配置nginx里头有看到这两段把：




location /static/ {
               root /home/helios/mysite;
               access_log  off;

}location /static/admin/ {
               root /home/helios/mysite/static;
               access_log off;

}好了 现在终端：
- cd /home/helios/mysite
- vim settings.py

看到 ADMIN_MEDIA_PREFIXSTATIC_URL这两个参数 就是指向这里 如果还有问题 我的参数不是这俩 另外的 你可以参考：MEDIA_ROOT = '/home/helios/mysite/static'MEDIA_URL = '/static/'ADMIN_MEDIA_PREFIX = '/static/admin/'然后 数据库问题：我的是mysql数据库：终端
- cd /home/helios/mysite
- mysql -h localhost -u root -p

Enter password******>create database mysite default character set utf8;(因为要记录中文 所以默认utf8)>quit
- vim settings.py

DATABASES = {
   'default': {
       'ENGINE': 'django.db.backends.mysql', # Add 'postgresql_psycopg2', 'postgresql', 'mysql', 'sqlite3' or 'oracle'.
       'NAME': 'mysite',                      # Or path to database file if using sqlite3.
       'USER': 'root',                      # Not used with sqlite3.
       'PASSWORD': '*******',                  # Not used with sqlite3.
       'HOST': 'localhost',                      # Set to empty string for localhost. Not used with sqlite3.
       'PORT': '3306',                      # Set to empty string for default. Not used with sqlite3.
   }

最后 还有一些：语言：LANGUAGE_CODE = 'zh-cn'时间：TIME_ZONE = 'Asia/Shanghai'

其他关于django的配置 朋友们可以去网络搜索django教程或者官网去看看 我的博客上面有个类似懒人安装的脚本 有兴趣可以去看看更详细的教程 [http://sishengbang.tk](http://sishengbang.tk)如果我的这些步骤还有问题可以mail我：helio_s@163.com或者yuyanget@gmail.com或者我的博客www.younguy.tkqq也可以加我 不过要说明nginx+ubuntu+django：845262968新建一个github项目：一个tornado+django+php+nginx+mysql的部署并且包括源代码[https://github.com/yuyangit/BMProject](https://github.com/yuyangit/BMProject)

## no input file specified错误
sudo vi /etc/nginx/sites-available/default
其中这个字段
location ~ \.php$ {
root html;
fastcgi_pass 127.0.0.1:9000;
fastcgi_index index.php;
fastcgi_param SCRIPT_FILENAME /var/www/nginx-default$fastcgi_script_name;
include fastcgi_params;
注意
fastcgi_param SCRIPT_FILENAME /var/www/nginx-default$fastcgi_script_name;
/var/www/nginx-default 改为你的网站根目录，一般就是改成这个。server 字段下root 目录和网站根目录保持一致

## 安装Zend Optimizer

要求PHP版本为5.2，不支持Ubuntu10.04的PHP5.3，请参照[PHP5.2](http://wiki.ubuntu.org.cn/PHP5.2)。（Zend 5.5支持PHP5.3，下载地址：[http://downloads.zend.com/guard/5.5.0/ZendGuardLoader-php-5.3-linux-glibc23-i386.tar.gz）](http://downloads.zend.com/guard/5.5.0/ZendGuardLoader-php-5.3-linux-glibc23-i386.tar.gz%EF%BC%89)

下载 [Zend Optimizer](http://www.zend.com/en/products/guard/downloads)。直接贴下载地址，参考版本号改（这是32位的），不然主页要注册才能下
 http://downloads.zend.com/optimizer/3.3.9/ZendOptimizer-3.3.9-linux-glibc23-i386.tar.gz tar zxvf ZendOptimizer-3.3.9-linux-glibc23-i386.tar.gzcd ZendOptimizer-3.3.9-linux-glibc23-i386/data/5_2_x_comp
sudo mkdir /usr/local/zend
sudo cp ZendOptimizer.so /usr/local/zend 
编辑php.ini
sudo gedit /etc/php5/cgi/php.ini
开头加入，注意标点符号要英文。
[Zend Optimizer]
zend_optimizer.optimization_level=1  
zend_extension="/usr/local/zend/ZendOptimizer.so"
关闭php-cgi
sudo killall -HUP php-cgi
重启php-cgi
spawn-fcgi -a 127.0.0.1 -p 9000 -C 10 -u www-data -f /usr/bin/php-cgi
不需要重启nginx

还是上面那个phpinfo文件，要能看到如下信息
This program makes use of the Zend Scripting Language Engine:
Zend Engine v2.2.0, Copyright (c) 1998-2009 Zend Technologies
    with Zend Optimizer v3.3.9, Copyright (c) 1998-2009, by Zend Technologies
## 安裝XCache
sudo apt-get install php5-xcacheroot@ubuntu:/home/qii# dpkg -l | grep xcach
ii  php5-xcache                          1.2.2-5                                         Fast, stable PHP opcode cacher
xcache配置文件路径是
/etc/php5/conf.d/xcache.ini
编辑php.ini
sudo gedit /etc/php5/cgi/php.ini
把xcache.ini的内容加入到php.ini。

关闭php-cgi
sudo killall -HUP php-cgi
重启php-cgi
spawn-fcgi -a 127.0.0.1 -p 9000 -C 10 -u www-data -f /usr/bin/php-cgi
不需要重启nginx检查安装是否成功
root@ubuntu:/home/qii# php -v
PHP 5.2.10-2ubuntu6 with Suhosin-Patch 0.9.7 (cli) (built: Oct 23 2009 16:30:10) 
Copyright (c) 1997-2009 The PHP Group
Zend Engine v2.2.0, Copyright (c) 1998-2009 Zend Technologies
    with XCache v1.2.2, Copyright (c) 2005-2007, by mOo
还有前面info.php页应该有XCache模块
|![](http://wiki.ubuntu.org.cn/images/thumb/2/28/Apachexcache.png/500px-Apachexcache.png)![](http://wiki.ubuntu.org.cn/skins/common/images/magnify-clip.png)info页面的XCache模块|
|----|

这里有点奇怪的是，如果不把xcache.ini的内容加入php.ini，apache也能载入XCache，但info.php上没XCache模块。

## 安装eAccelerator
sudo apt-get install php5-dev
下载 [eAccelerator](http://www.eaccelerator.net/)
wget http://sourceforge.net/projects/eaccelerator/files/eaccelerator/eAccelerator%200.9.6.1/eaccelerator-0.9.6.1.tar.bz2tar jxvf eaccelerator-0.9.6.1.tar.bz2cd eaccelerator-0.9.6.1 phpize
sudo ./configure -enable-eaccelerator=shared
sudo makeqii@ubuntu:~/tmp/eaccelerator-0.9.6.1$ sudo make install
Installing shared extensions:     /usr/lib/php5/20090626+lfs/

修改php.ini文件，安装为Zend扩展，最好放在开头，放到[zend]之前，免的出莫名其妙的问题：
sudo vi /etc/php5/cgi/php.ini[eaccelerator]
zend_extension="/usr/lib/php5/20090626+lfs/eaccelerator.so"  
eaccelerator.shm_size="16" 
eaccelerator.cache_dir="/tmp/eaccelerator" 
eaccelerator.enable="1" 
eaccelerator.optimizer="1" 
eaccelerator.check_mtime="1" 
eaccelerator.debug="0" 
eaccelerator.filter="" 
eaccelerator.shm_max="0" 
eaccelerator.shm_ttl="0" 
eaccelerator.shm_prune_period="0" 
eaccelerator.shm_only="0" 
eaccelerator.compress="1" 
eaccelerator.compress_level="9" 
eaccelerator.allowed_admin_path="/var/www/nginx-default/control.php"
创建cache缓存目录
eaccelerator.cache_dir="/var/cache/eaccelerator"   这里定义cache路径
默认值是/tmp/eaccelerator，这非常简单因为任何人都对该目录可写，但是并不明智，因为重启后系统会自动清理该目录。一个更好的地方是/var/cache/eaccelerator。创建该目录并确保它对eAccelerator的使用者可写（通常该用户是你的网络服务器运行者，可能是www-data）。使用默认值的话这样继续：
mkdir /tmp/eacceleratorchmod 777 /tmp/eaccelerator
改成 /var/cache/eaccelerator的话这样继续，先改php.ini
eaccelerator.cache_dir="/var/cache/eaccelerator" sudo mkdir /var/cache/eaccelerator
sudo chown root:www-data /var/cache/eaccelerator
sudo chmod u=rwx,g=rwx,o= /var/cache/eaccelerator
复制控制文件control.php到网站根目录
sudo cp control.php /var/www/nginx-default/
修改control.php的$user和$pw，默认是admin和eAccelerator
sudo vi /var/www/nginx-default/control.php 
关闭php-cgi
sudo killall -HUP php-cgi
重启php-cgi
spawn-fcgi -a 127.0.0.1 -p 9000 -C 10 -u www-data -f /usr/bin/php-cgi
不需要重启nginx打开 [http://localhost/control.php](http://localhost/control.php)
|![](http://wiki.ubuntu.org.cn/images/thumb/2/28/NginxeAcceleratorcontrol.png/500px-NginxeAcceleratorcontrol.png)![](http://wiki.ubuntu.org.cn/skins/common/images/magnify-clip.png)eAccelerator control.php页面|
|----|

查看之前的info.php页面，有下列字段：
This program makes use of the Zend Scripting Language Engine:
Zend Engine v2.2.0, Copyright (c) 1998-2009 Zend Technologies
    with eAccelerator v0.9.6.1, Copyright (c) 2004-2010 eAccelerator, by eAccelerator
    
## 屏蔽迅雷

新建
/etc/nginx/agent.confif ($http_user_agent ~ "Mozilla/4.0\ \(compatible;\ MSIE\ 6.0;\ Windows\ NT\ 5.1;\ SV1;\ .NET\ CLR\ 1.1.4322;\ .NET\ CLR\ 2.0.50727\)") { return 404; }

注意的是，空格和括弧需要使用“\”进行转义。

然后site配置中
include /etc/nginx/agent.conf;
迅雷usera-gent和这种做法失效的情况见[Apache#屏蔽迅雷](http://wiki.ubuntu.org.cn/Apache#.E5.B1.8F.E8.94.BD.E8.BF.85.E9.9B.B7)

## 参考

[nginx 禁止某个 User_Agent 的方法](http://luy.li/2009/10/31/nginx-block-user_agent/)

## 了解更多
- Nginx 的中文维基：[http://wiki.codemongers.com/NginxChs](http://wiki.codemongers.com/NginxChs)
- Nginx 的中文站： [http://www.nginx.cn](http://www.nginx.cn)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)](https://so.csdn.net/so/search/s.do?q=nginx&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)




