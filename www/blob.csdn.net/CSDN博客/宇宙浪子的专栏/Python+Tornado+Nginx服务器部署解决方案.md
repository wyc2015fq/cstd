# Python+Tornado+Nginx服务器部署解决方案 - 宇宙浪子的专栏 - CSDN博客
2017年08月16日 09:38:23[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：211
个人分类：[Python																[tornado](https://blog.csdn.net/bluehawksky/article/category/7102850)](https://blog.csdn.net/bluehawksky/article/category/2542577)
转自：[http://blog.csdn.net/sc4599/article/details/50069667](http://blog.csdn.net/sc4599/article/details/50069667)
目标：反向代理+Tornado框架实现高性能web服务器
原则：生产环境部署，提升服务端性能。 
适用岗位：开发人员、运维人员
实施步骤： 
1、 选定[Python](http://lib.csdn.net/base/python)版本2.7.8 32位； 
(1)下载Python2.7.8 ： 
[https://www.python.org/downloads/release/python-278/](https://www.python.org/downloads/release/python-278/)
(2)安装python_2.7.8.msi： 
一路下一步，但是到下图步骤的时候，记得吧添加到环境变量点上 
（把红色叉叉变成硬盘！） 
2、 安装easy_install easy_install是由PEAK([python](http://lib.csdn.net/base/python) Enterprise Application Kit)开发的setuptools包里带的一个命令，所以使用easy_install实际上是在调用setuptools来完成安装模块的工作。； 
(1)下载地址： 
[https://pypi.python.org/pypi/setuptools](https://pypi.python.org/pypi/setuptools)
下载好后解压，使用cmd进入相应目录下执行 
(2)配置环境变量 
默认easy_install 会安装在c:/python2.7/Script/文件加下（根据自己实际情况配置），所以可以把此文件夹加入到环境变量中
3、 安装pip：pip 是一个安装和管理 Python 包的工具，python安装包的工具有easy_install, setuptools, pip，distribute。使用这些工具都能下载并安装django。，而pip是easy_install的替代品。在CPython解释器，pypy解释器，可以很好地工作 ； 
(1)通过cmd打开命令窗口，执行如下命令 
4、 使用pip安装安装Tornado： 
5、 安装MySQLdb模块： 
pip installMySQL-python 
6、 如果上面安装出现错误: 
那么你需要安装[MySQL](http://lib.csdn.net/base/mysql)-python-1.2.5.win32-py2.7.exe 
网盘地址：[http://pan.baidu.com/s/1mgIVNlu](http://pan.baidu.com/s/1mgIVNlu)
7、 安装了以上模块还是出错： 
此时需要安装VCForPython27.msi 此处将会提供网盘地址： 
[http://pan.baidu.com/s/1i30KYUD](http://pan.baidu.com/s/1i30KYUD)
8、 安装Nginx反向代理服务器： 
下载网址：[http://nginx.org/download/nginx-1.8.0.zip](http://nginx.org/download/nginx-1.8.0.zip)
(此处建议下载稳定版，根据当时官网提供的最高版本安装即可) 
（1） nginx.conf配置详解
```
（2）     #运行用户
user www-data;    
#启动进程,通常设置成和cpu的数量相等
worker_processes  1;
（3）     #全局错误日志及PID文件
error_log  /var/log/nginx/error.log;
pid        /var/run/nginx.pid;
（4）     #工作模式及连接数上限
events {
    use  epoll;            #epoll是多路复用IO(I/OMultiplexing)中的一种方式,但是仅用于linux2.6以上内核,可以大大提高nginx的性能
    worker_connections  1024;#单个后台worker process进程的最大并发链接数
    # multi_accept on; 
}
（5）     #设定http服务器，利用它的反向代理功能提供负载均衡支持
http {
     #设定mime类型,类型由mime.type文件定义
    include      /etc/nginx/mime.types;
    default_type  application/octet-stream;
    #设定日志格式
    access_log    /var/log/nginx/access.log;
（6）         #sendfile 指令指定 nginx 是否调用 sendfile 函数（zero copy 方式）来输出文件，对于普通应用，
    #必须设为 on,如果用来进行下载等应用磁盘IO重负载应用，可设置为 off，以平衡磁盘与网络I/O处理速度，降低系统的uptime.
    sendfile        on;
    #tcp_nopush     on;
（7）         #连接超时时间
    #keepalive_timeout  0;
    keepalive_timeout  65;
    tcp_nodelay        on;
    #开启gzip压缩
    gzip  on;
    gzip_disable "MSIE [1-6]\.(?!.*SV1)";
（8）         #设定请求缓冲
    client_header_buffer_size    1k;
    large_client_header_buffers  4 4k;
（9）         include /etc/nginx/conf.d/*.conf;
    include /etc/nginx/sites-enabled/*;
（10）      #设定负载均衡的服务器列表
     upstream mysvr {
    #weigth参数表示权值，权值越高被分配到的几率越大
    #本机上的Squid开启3128端口
    server 192.168.8.1:3128 weight=5;
    server 192.168.8.2:80  weight=1;
    server 192.168.8.3:80  weight=6;
    }
（11）  
   server {
    #侦听80端口
       listen       80;
        #定义使用www.xx.com访问
        server_name  www.xx.com;
（12）          #设定本虚拟主机的访问日志
        access_log logs/www.xx.com.access.log  main;
（13）      #默认请求
    location / {
          root  /root;      #定义服务器的默认网站根目录位置
          index index.phpindex.html index.htm;  #定义首页索引文件的名称
#请求转向定义的服务器列表 
proxy_pass  http://mysvr ;
（14）           fastcgi_pass  www.xx.com;
         fastcgi_param SCRIPT_FILENAME  $document_root/$fastcgi_script_name; 
          include/etc/nginx/fastcgi_params;
        }
（15）      # 定义错误提示页面
    error_page   500 502 503 504 /50x.html;  
        location = /50x.html {
        root   /root;
    }
（16）      #静态文件，nginx自己处理
    location ~ ^/(images|javascript|js|css|flash|media|static)/{
        root /var/www/virtual/htdocs;
        #过期30天，静态文件不怎么更新，过期可以设大一点，如果频繁更新，则可以设置得小一点。
        expires 30d;
    }
    #PHP 脚本请求全部转发到 FastCGI处理. 使用FastCGI默认配置.
    location ~ \.php$ {
        root /root;
        fastcgi_pass 127.0.0.1:9000;
        fastcgi_index index.php;
        fastcgi_param SCRIPT_FILENAME/home/www/www$fastcgi_script_name;
        include fastcgi_params;
    }
    #设定查看Nginx状态的地址
    location /NginxStatus {
       stub_status           on;
       access_log             on;
       auth_basic             "NginxStatus";
        auth_basic_user_file conf/htpasswd;
    }
    #禁止访问 .htxxx 文件
    location ~ /\.ht {
        deny all;
    }
     }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
（17）我们要配置的主要参数是：11中的侦听端口,13中的请求转向定义的服务器地址 
（18）配置好nginx.conf后 可以在nginx中使用 start nginx.exe来开启nginx服务器 
（19）现在可以通过[http://127.0.0.1:80](http://127.0.0.1/)(或者是根据你自己配置的端口)来访问你的服务器啦 
9、 安装tornado： 
使用pip install tornado 
10、安装好后[测试](http://lib.csdn.net/base/softwaretest)你的tornado WEB服务器是否能够正常工作，并将nginx的proxy_pass 指向自己tornado的地址。
