# 七、带你进入神奇的symfony世界 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:23:22[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：619












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_104_default_big.png)



一步一步手把手讲解symfony2的安装和配置，并快速搭建网站第一个页面。

## 安装symfony2



我们默认你使用的是centos7操作系统并已经安装好了php和nginx（如果还没有请回过头看前面几篇）。

首先，我们来安装symfony安装器。假设我们要安装到/usr/local/bin下，那么执行

```
sudo curl -LsS https://symfony.com/installer -o /usr/local/bin/symfony
sudo chmod a+x /usr/local/bin/symfony
```

执行一下symfony命令看看是否正常显示帮助信息

下面找到一个放置自己网站的目录，比如/data/httpdir/，创建好目录后，在httpdir下执行如下命令下载并初始化网站目录
`[root@centos7vm httpdir]# symfony new mywebsite 2.8`
这里的2.8是symfony2的版本，现在已经发布到3了，但一些流行的组件兼容性可能不好，所以依然用最稳定的2.8版



《《《《《《《《《《《《《《《《《《《《《《《《《《《

创建mywebsite时你可能会遇到这样的警告
`PHP Warning:  date(): It is not safe to rely on the system's timezone settings. You are *required* to use the date.timezone setting or the date_default_timezone_set() function. blublu...`
这是因为你的php.ini文件没有指定时区，修改/etc/php.ini文件，把
`;date.timezone =`
改成
`date.timezone = Asia/Shanghai`
注意：要去掉前面的";"

重启php-fpm
`[root@centos7vm httpdir]# service php-fpm restart`
》》》》》》》》》》》》》》》》》》》》》》》》》》



有没有发现在httpdir目录下出现了mywebsite目录？走进去瞧瞧symfony2已经为你做了什么

```
[root@centos7vm mywebsite]# ls
app  bin  composer.json  composer.lock  README.md  src  vendor  web
```

web目录是网站的入口，假设你的网站叫做www.shareditor.com，那么www.shareditor.com/test.php一般会通过nginx配置为对应web/test.php文件（当然这都需要你的ngixn配置文件配合，如果你非要配置成web目录的上一级也可以，只不过网站不能用而已，呵呵）。一般会把可以通过url访问的文件放到这里，比如css、js、图片、静态html文件等

app目录是全局应用的运行相关文件（包括入口函数、组件加载、全局配置、log文件、cache文件等），这里面还有一个重要二进制文件console，用来方便我们做自动化操作，后面会讲到

src是你自己开发php代码的地方，一般以组件形式整理，symfony2里叫做bundle，一会再说bundle是什么

vendor是symfony2自带组件放置的地方，也是最庞大的地方，一般不需要我们修改里面的内容，使用里面哪个功能就可以在app目录下的配置文件里配置，不使用也无妨，缺什么组件也可以手工安装组件到vendor下，安装方法马上便知

bin目录里是symfony2提供给我们的一些二进制工具

composer.json是这个网站工程里用到symfony2中组件的配置和版本等信息



好！什么都不用改，我们现在去改下nginx的配置，让网站指向web目录，这里需要看一下[如何配置nginx实现虚拟主机](http://www.shareditor.com/blogshow/11)

nginx.conf如下：

```
user  nobody;
worker_processes  1;

events {
    worker_connections  1024;
}

http {
    include       /etc/nginx/mime.types;
    default_type  application/octet-stream;
    sendfile        on;
    keepalive_timeout  65;
    client_max_body_size 20M;
    include /etc/nginx/conf.d/*.conf;
}
```

mywebsite.conf如下：

```
log_format logformat '$remote_addr - $remote_user [$time_local] "$request" '
'$status $body_bytes_sent "$http_referer" '
'"$http_user_agent" "$http_x_forwarded_for"';

server {
    listen       80;
    server_name  172.16.142.130;
    index app.php;
    root /data/httpdir/mywebsite/web;
    location / {
        if (!-e $request_filename){
            rewrite ^/(.+)$ /app.php/$1 last;
        }
        client_max_body_size 20M;
    }
    location ~ ^/(app|app_dev)\.php(/|$) {
        fastcgi_pass   127.0.0.1:9000;
        fastcgi_split_path_info ^(.+\.php)(/.*)$;
        include fastcgi_params;
        fastcgi_param  SCRIPT_FILENAME    $document_root$fastcgi_script_name;
        fastcgi_param  HTTPS              off;
        client_max_body_size 20M;
    }
    location ~ .*\.(gif|jpg|jpeg|png|bmp|swf)$
    {
        expires 1h;
        client_max_body_size 20M;
    }

    access_log /data/httpdir/logs/mywebsite.log logformat;
    error_log /data/httpdir/logs/mywebsite.error_log;
}
```

因为系统自带的service命令起动服务有些无法理解的现象，所以我们以后都采取直接启动nginx，强制杀掉所有nginx进程后，执行
`[root@centos7vm mywebsite]# nginx`
以后如果想重启nginx可以用命令
`[root@centos7vm mywebsite]# nginx -s restart`
其实如果只是改配置，可以调用reload命令
`[root@centos7vm mywebsite]# nginx -s reload`


为了简单，把php-fpm的子进程用户也改到nobody，修改/etc/php-fpm.d/www.conf，把里面的

```
; RPM: apache Choosed to be able to access some dir as httpd
user = apache
; RPM: Keep a group allowed to write in log dir.
group = apache
```

改成

```
; RPM: apache Choosed to be able to access some dir as httpd
user = nobody
; RPM: Keep a group allowed to write in log dir.
group = nobody
```

因为系统自带的service命令起动服务有些无法理解的现象，所以我们以后都采取直接启动php-fpm，强制杀掉所有php-fpm进程后，执行
`[root@centos7vm mywebsite]# php-fpm`
以后如果想重启php-fpm可以用命令：
`[root@centos7vm mywebsite]# kill -USR2 `cat /var/run/php-fpm/php-fpm.pid``


OK，现在可以打开http://127.0.0.1/啦，看到了什么？

![](http://www.shareditor.com/uploads/media/my-context/0001/01/772fdf8e0ca0acaa2d0c2077c79425f4e5d71d5c.png)



恭喜你，你已经进入了神奇的symfony2的世界




