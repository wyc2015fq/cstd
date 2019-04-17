# 让你的 Nginx 的 RTMP 直播具有统计某频道在线观看用户数量的功能 - DoubleLi - 博客园






你的 Nginx 已经有了 RTMP 直播功能的话，如果你还想统计某直播频道当前观看用户量的话，可以加入 with-http_xslt_module 模块。具体步骤如下：
        1.查看原来的参数
        /usr/local/nginx/sbin/nginx -V
        输出中可以得到原来编译时带有的参数，比如作者得到：
        --user=nginx --group=nginx --with-http_stub_status_module --with-http_gzip_static_module --with-http_ssl_module --with-http_flv_module --with-http_perl_module --with-mail
        这些参数在我们安装新模块时仍然有用。
        2.下载 nginx-rtmp-module 安装包
        nginx-rtmp-module-master.zip，最新下载地址：[https://github.com/arut/nginx-rtmp-module](https://github.com/arut/nginx-rtmp-module)。
        下载后将其解压缩得到 nginx-rtmp-module-master 目录。
        3.下载 nginx-1.3.8.tar.gz 包
        可以在 [http://nginx.org/download/](http://nginx.org/download/) 找你需要的版本。
        下载后解压缩得到 nginx-1.3.8 目录。
        4.关闭 nginx
        ps - ef | grep nginx
        在进程列表里找到 master 进程，这是 nginx 的主进程号。
        kill -TERM 主进程号
        nginx 被关闭。
        5.安装其他依赖包
        yum install pcre-devel
        yum install openssl-devel
        yum install perl-devel perl-ExtUtils-Embed
        yum install gcc
        yum install libxml2 libxml2-devel libxslt libxslt-devel
        6.编译 with-http_xslt_module 模块
        在步骤一得到的一系列参数后增加以下参数：
        --with-http_xslt_module --add-module=/home/defonds/nginx-rtmp-module-master
        其中 /home/defonds/nginx-rtmp-module-master 是步骤二得到的目录。
        切换进入步骤三得到的 nginx-1.3.8 目录，使用新组合得到的参数列表重新配置：
        ./configure --prefix=/usr/local/nginx --user=nginx --group=nginx --with-http_stub_status_module --with-http_gzip_static_module --with-http_ssl_module --with-http_flv_module --with-http_perl_module --with-mail --with-http_xslt_module --add-module=/home/defonds/nginx-rtmp-module-master
        然后编译：
        make
        最后替换掉原来的二进制执行文件：
        cp /usr/local/nginx/sbin/nginx /usr/local/nginx/sbin/nginx.bak
        cp ./objs/nginx /usr/local/nginx/sbin/
        7.修改 nginx 配置文件
        创建一个简单地 xls 表格文件 nclients.xsl 用于提取观看当前频道的用户数量，编辑其内容如下：



**[html]**[view plain](https://blog.csdn.net/defonds/article/details/9065591#)[copy](https://blog.csdn.net/defonds/article/details/9065591#)



- <xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">  
- 
- <xsl:output method="html"/>  
- 
- <xsl:param name="app"/>  
- <xsl:param name="name"/>  
- 
- <xsl:template match="/">  
- <xsl:value-of select="count(//application[name=$app]/live/stream[name=$name]/client[not(publishing) and flashver])"/>  
- </xsl:template>  
- 
- </xsl:stylesheet>  



        然后将其放在一个目录中，比如 /home/www。
        修改 nginx 主配置文件 nginx.conf，添加以下内容：



**[html]**[view plain](https://blog.csdn.net/defonds/article/details/9065591#)[copy](https://blog.csdn.net/defonds/article/details/9065591#)



- location /stat {  
-     rtmp_stat all;  
-     allow 127.0.0.1;  
- }  
- location /nclients {  
-     proxy_pass http://127.0.0.1/stat;  
-     xslt_stylesheet /home/www/nclients.xsl app='$arg_app' name='$arg_name';  
-     add_header Refresh "3; $request_uri";  
- }  



        8.重启 nginx
        /usr/local/nginx/sbin/nginx
        No news is good news，终端没有输出证明启动成功。否则参照终端给的异常信息检查主配置文件。
        根据直播频道访问以下地址：
        http://直播服务器IP/nclients?app=app应用名&name=频道名
        有返回结果表示 with-http_xslt_module 模块安装成功。返回结果就是当前频道的观看人数。

参考资料：[Nginx RTMP 模块 nginx-rtmp-module 指令详解](http://blog.csdn.net/defonds/article/details/9274479)；
[https://github.com/arut/nginx-rtmp-module/wiki/Getting-number-of-subscribers](https://github.com/arut/nginx-rtmp-module/wiki/Getting-number-of-subscribers)。









