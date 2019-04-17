# 【推荐】Nginx基础知识之————多模块(非覆盖安装、RTMP在线人数实例安装测试) - DoubleLi - 博客园






**说明：**
已经安装好的nginx，需要添加一个未被编译安装的模块，需要怎么弄呢？

**具体：**这里以安装第三方nginx-rtmp-module和nginx-accesskey-2.0.3模块为例，nginx的模块是需要重新编译nginx，而不是像apache一样配置文件引用.so

1. 下载第三方扩展模块nginx-rtmp-module

```
sudo git clone https://github.com/arut/nginx-rtmp-module.git
```

2. 下载第三方扩展模块nginx-accesskey-2.0.3

[http://www.filewatcher.com/m/nginx-accesskey-2.0.3.tar.gz.2632-0.html](http://www.filewatcher.com/m/nginx-accesskey-2.0.3.tar.gz.2632-0.html)   下载后解压即可

```
[tar.gz](http://www.filewatcher.com/m/nginx-accesskey-2.0.3.tar.gz.2632-0.html) 这种格式是我使用得最多的压缩格式。它在压缩时不会占用太多CPU的，而且可以得到一个非常理想的压缩率。使用下面这种格式去压缩一个目录：
# tar -zcvf archive_name.tar.gz directory_to_compress
解压缩：
# tar -zxvf archive_name.tar.gz
```

3、最后下载的结果如下所示，git上下载的是不需用解压的，[nginx-accesskey-2.0.3](http://www.filewatcher.com/m/nginx-accesskey-2.0.3.tar.gz.2632-0.html) 下载下来需要解压的

![](https://images2015.cnblogs.com/blog/1021265/201610/1021265-20161021103423592-1877266296.png)

4.cp [nginx-accesskey-2.0.3](http://www.filewatcher.com/m/nginx-accesskey-2.0.3.tar.gz.2632-0.html) 到Nginx 安装目录下去 nginx-1.8.1

![](https://images2015.cnblogs.com/blog/1021265/201610/1021265-20161021103504779-15628534.png)

修改 nginx-accesskey-2.0.3 文件下面的配置文件

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

vim nginx-accesskey-2.0.3/config
#修改$HTTP_ACCESSKEY_MODULE为ngx_http_accesskey_module
USE_MD5=YES
USE_SHA1=YES
ngx_addon_name=ngx_http_accesskey_module
HTTP_MODULES="$HTTP_MODULES ngx_http_accesskey_module"
NGX_ADDON_SRCS="$NGX_ADDON_SRCS $ngx_addon_dir/ngx_http_accesskey_module.c"

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

5、提示安装错误：

```
./configure: error: the HTTP rewrite module requires the PCRE library.
You can either disable the module by using --without-http_rewrite_module
option, or install the PCRE library into the system, or build the PCRE library
statically from the source with nginx by using --with-pcre=<path> option.
```

大概意思是:HTTP重写模块需要PCRE库。您可以使用--without-http_rewrite_module禁用模块选项，或将PCRE库安装到系统中，或构建PCRE库通过使用--with-pcre = <path>选项从源与nginx静态。

解决办法：

需要安装一下两个库文件：(nginx pcre 安装)

sudo apt-get install libpcre3 libpcre3-dev

apt-get install openssl libssl-dev

apt-get install git



6、在Nginx安装目录下面配置文件

```
./configure --user=www --group=www --prefix=/usr/local/nginx --with-http_stub_status_module --with-http_ssl_module --with-http_realip_module --add-module=./nginx-accesskey-2.0.3 --add-module=../nginx-rtmp-module/
```

![](https://images2015.cnblogs.com/blog/1021265/201610/1021265-20161021103927592-1393415068.png)

注意：安装时候的两个模块是在不同的目录下面的，所以在配置文件的时候一定要记住这个文件路径必须的合适，否则安装不成功,提示一下错误：

![](https://images2015.cnblogs.com/blog/1021265/201703/1021265-20170307131657953-1307480610.png)



7、编译:

make

#不要make install，否则就是覆盖安装

8、 替换nginx二进制文件:

```
root@iZ231gvwxe7Z:/home/www/nginx-1.8.1# cd objs/
root@iZ231gvwxe7Z:/home/www/nginx-1.8.1/objs# ls
addon autoconf.err Makefile nginx nginx.8 ngx_auto_config.h ngx_auto_headers.h ngx_modules.c ngx_modules.o src
root@iZ231gvwxe7Z:/home/www/nginx-1.8.1/objs# cp nginx /usr/local/nginx/sbin/nginx
```

 9、 查看已经安装好的模块和重启Nginx:

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

www@iZ231gvwxe7Z:/tmp/hls$ /usr/local/nginx/sbin/nginx -V
nginx version: nginx/1.8.1
built by gcc 4.8.2 (Ubuntu 4.8.2-19ubuntu1) 
built with OpenSSL 1.0.1f 6 Jan 2014
TLS SNI support enabled
configure arguments: --user=www --group=www --prefix=/usr/local/nginx --with-http_stub_status_module 
--with-http_ssl_module --with-http_realip_module 
--add-module=./nginx-accesskey-2.0.3 
--add-module=../nginx-rtmp-module/

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

配置文件：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

user www www;
worker_processes  1;

#error_log  logs/error.log;
#error_log  logs/error.log  notice;
error_log  logs/warn.log  warn;

#pid        logs/nginx.pid;


events {
    worker_connections  65525;
}


http {
    include       mime.types;
    default_type  application/octet-stream;

    log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
                      '$status $body_bytes_sent "$http_referer" '
                      '"$http_user_agent" "$http_x_forwarded_for"';

    #access_log  logs/access.log  main;

    sendfile        on;
    #tcp_nopush     on;

    #keepalive_timeout  0;
    keepalive_timeout  65;

    #gzip  on;

    server {
        listen       80;
        server_name  localhost;

        #charset koi8-r;

        #access_log  logs/host.access.log  main;

        location / {
            root   html;
            index  index.html index.htm;
        }

        #error_page  404              /404.html;

        # redirect server error pages to the static page /50x.html
        #
        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }
    }    
}

rtmp {
    server {
            listen 1935;
            chunk_size 4096;

            application live {
                    live on;
                    record off;
            exec /home/www/bin/deviceToUrlStreamName.sh $name;
            exec_kill_signal term;
                   
            }
            application live360p {
                    live on;
                    record off;
           }
    }
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

10、 重启Nginx:

```
service nginx restart
```

 11.增加一个RTMP在线统计人数模块：

```
./configure --user=www --group=www --prefix=/usr/local/nginx --with-http_stub_status_module --with-http_gzip_static_module --with-http_ssl_module --with-http_flv_module --with-http_perl_module --with-http_xslt_module --add-module=../nginx-rtmp-module/
```

配置 --with-http_xslt_module 时提示 the HTTP XSLT module requires the libxml2/libxslt libraries

提示错误:./configure: error: the HTTP XSLT module requires the libxml2/libxslt

```
sudo apt-get install libxml2 libxml2-dev libxslt-dev
sudo apt-get install libgd2-xpm libgd2-xpm-dev
```

 修改 nginx 配置文件,增加以下两个代码：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

location /stat {
    rtmp_stat all;
    allow 127.0.0.1;
}

location /nclients {
    proxy_pass http://127.0.0.1/stat;
    xslt_stylesheet /www/nclients.xsl app='$arg_app' name='$arg_name';
    add_header Refresh "3; $request_uri";
}

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

创建一个简单地 xls 表格文件 nclients.xsl 用于提取观看当前频道的用户数量，编辑其内容如下：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="html"/>

<xsl:param name="app"/>
<xsl:param name="name"/>

<xsl:template match="/">
    <xsl:value-of select="count(//application[name=$app]/live/stream[name=$name]/client[not(publishing) and flashver])"/>
</xsl:template>

</xsl:stylesheet>

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

使用HTTP请求http://myserver.com/nclients?app=myapp&name=mystream获取流订阅者的数量。 在浏览器或iframe中打开时，此编号将每3秒自动刷新一次。

重启Nginx服务：

```
root@iZ231gvwxe7Z:/home/www/nginx-1.8.1/objs# service nginx restart
 * Stopping Nginx Server...                                                                                                                                                           [fail] 
 * Starting Nginx Server...                                                                                                                                                           [ OK ] 
root@iZ231gvwxe7Z:/home/www/nginx-1.8.1/objs#
```

使用OBS推流该Nginx服务器的RTMP模块既可以

例如我测试的结果：

![](https://images2015.cnblogs.com/blog/1021265/201612/1021265-20161222113103323-1535903117.png)

使用VLC播放该流

测试在线观看人数结果（只有我一个人播放的，就是一个人哦！）：

![](https://images2015.cnblogs.com/blog/1021265/201612/1021265-20161222113244464-1155341824.png)







参考文献：

[https://github.com/arut/nginx-rtmp-module/wiki/Getting-number-of-subscribers](https://github.com/arut/nginx-rtmp-module/wiki/Getting-number-of-subscribers)

http://blog.csdn.net/defonds/article/details/9065591

http://www.cnblogs.com/terrysun/archive/2012/11/22/2782472.html

[如何升级Nginx到最新稳定版](http://www.cnblogs.com/terrysun/archive/2012/11/22/2782472.html)：*[http://www.cnblogs.com/terrysun/archive/2012/11/22/2782472.html](http://www.cnblogs.com/terrysun/archive/2012/11/22/2782472.html)*



*from:https://www.cnblogs.com/tinywan/p/5983694.html*









