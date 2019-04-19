# Nginx+Tomcat+Memcached负载均衡+session共享集群服务搭建(Linux) - 零度的博客专栏 - CSDN博客
2016年10月28日 12:44:48[零度anngle](https://me.csdn.net/zmx729618)阅读数：887
# Nginx+Tomcat+Memcached负载均衡集群服务搭建
       操作系统：CentOS6.5
       本文档主要讲解，如何在CentOS6.5下搭建Nginx+Tomcat+Memcached负载均衡集群服务器，Nginx负责负载均衡，Tomcat负责实际服务，Memcached负责同步Tomcat的Session，达到Session共享的目的。
**       本文文档下载地址**：[http://download.csdn.net/detail/zht666/7744157](http://download.csdn.net/detail/zht666/7744157)
       Tomcat与Memcached配置官方文档：[https://code.google.com/p/memcached-session-manager/wiki/SetupAndConfiguration](https://code.google.com/p/memcached-session-manager/wiki/SetupAndConfiguration)
**       注意文档中修正Tomcat配置部分：**
       打开server.xml文件，在<Engine>节点的中添加jvmRoute="tomcatRoute1"，这个是用于指定Tomcat集群的路由。Tomcat2设置成jvmRoute="tomcatRoute2"，Tomcat3设置成jvmRoute="tomcatRoute3"，依次类推。
# 1.安装Nginx
Nginx官网：[http://nginx.org/](http://nginx.org/)
下载最新稳定版本。在安装Nginx之前，需要先安装gcc、 openssl、 pcre和zlib软件库。
1.1安装gcc、gcc-c++
安装命令：
#sudo yum install gcc
# sudo yum install gcc-c++
1.2安装openssl
openssl官网：[http://www.openssl.org/](http://www.openssl.org/)
安装版本：openssl-1.0.1i.tar.gz
安装命令：
#tar -zxvf openssl-1.0.1i.tar.gz
#cd openssl-1.0.1i
#sudo ./config --prefix=/usr/local/openssl-1.0.1i    #prefix指定安装目录
#sudo make
#sudo make install
【注意】：此处使用的是config命令，而不是平常的configure命令
安装完成后，到/usr/local/下查看是否安装成功。如果安装出错，需要重新加压缩，重新安装。
1.3安装pcre
pcre官网：[http://www.pcre.org/](http://www.pcre.org/)
安装版本：pcre-8.35.tar.gz
安装命令：
#tar -zxvf pcre-8.35.tar.gz
#cd pcre-8.35
#sudo ./configure --prefix=/usr/local/pcre-8.35    #prefix指定安装目录
#sudo make
#sudo make install
安装完成后，到/usr/local/下查看是否安装成功。如果安装出错，需要重新加压缩，重新安装。
【注意】：如果没有安装c++编译器，这个软件的安装会报错！
1.4安装zlib
zlib官网：[http://www.zlib.net/](http://www.zlib.net/)
安装版本：zlib-1.2.8.tar.gz
安装命令：
#tar -zxvf zlib-1.2.8.tar.gz
#cd zlib-1.2.8
#sudo ./configure --prefix=/usr/local/zlib-1.2.8    #prefix指定安装目录
#sudo make
#sudo make install
安装完成后，到/usr/local/下查看是否安装成功。如果安装出错，需要重新加压缩，重新安装。
1.5安装Nginx
安装版本：nginx-1.6.1.tar.gz
安装命令：
#tar -zxvf nginx-1.6.1.tar.gz
#cd nginx-1.6.1
#sudo ./configure 
--prefix=/usr/local/nginx-1.6.1                #prefix指定安装目录
--with-openssl=/home/zht/src/openssl-1.0.1i    #指的是openssl源码路径
--with-pcre=/home/zht/src/pcre-8.3.5          #指的是pcre的源码路径
--with-zlib=/home/zht/src/zlib-1.2.8           #指的是zlib 的源码路径
--with-http_ssl_module
#sudo make
#make install
安装完成后，到/usr/local/下查看是否安装成功。如果安装出错，需要重新加压缩，重新安装。
![](https://img-blog.csdn.net/20140812143703296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh0NjY2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
1.5.1 配置Nginx
配置文件目录：**/usr/local/nginx-1.6.1/conf/nginx.conf**
# cd /usr/local/nginx-1.6.1/conf
# sudo vi nginx.conf
【修改后的配置文件如下】：
#创建进程的用户和用户组
user      zht zht;
#服务进程数量，一般等于CPU数量
worker_processes 1;
#全局错误日志定义，建议开启error级别日志.[ debug | info | notice | warn | error | crit ]
error_log logs/error.log error;
#error_log logs/error.log  notice;
#error_log logs/error.log  info;
#记录进程ID的文件
#pid       logs/nginx.pid;
events {
    #epoll是多路复用IO(I/O Multiplexing)中的一种方式,但是仅用于linux2.6以上内核,可以大大提高nginx的性能.Linux建议使用epoll，FreeBSD建议使用kqueue.
    useepoll;
    #一个worker_processe允许的最近并发连接数量
   worker_connections  1024;
}
http {
   include       mime.types;
   default_type application/octet-stream;
   #log_format  main  '$remote_addr - $remote_user [$time_local]"$request" '
    #                  '$status $body_bytes_sent"$http_referer" '
    #                  '"$http_user_agent""$http_x_forwarded_for"';
    #access_log  logs/access.log  main;
   sendfile        on;
   #tcp_nopush     on;
    #http连接的持续时间
   keepalive_timeout  65;
    #gzip压缩设置
    gzip  on;           #开启gzip
    gzip_min_length 1k;  #最小压缩文件大小
    gzip_buffers 4 16k;  #压缩缓冲区
    #http的协议版本(1.0/1.1),默认1.1，前端如果是squid2.5请使用1.0
    gzip_http_version 1.1;
    #gzip压缩比，1压缩比最小处理速度最快，9压缩比最大但处理速度最慢(传输快但比较消耗cpu)
    gzip_comp_level 2;    
    #和http头有关系，加个vary头，给代理服务器用的，有的浏览器支持压缩，有的不支持，所以避免浪费不支持的也压缩，所以根据客户端的HTTP头来判断，是否需要压缩
    gzip_varyon;
    #gzip压缩类型，不用添加text/html，否则会有警告信息
    gzip_types text/plain text/javascript text/css application/xmlapplication/x-javascript application/json;
    #设定负载均衡的服务器列表，可以设置多个upstream，但mysvr名字要区分
    upstreammyClusterServer1 {
       #weigth参数表示权值，权值越高被分配到的几率越大
        #本机上的Squid开启3128端口
        server 127.0.0.1:8081  weight=5;
        server 127.0.0.1:8082  weight=5;
        server 127.0.0.1:8083  weight=5;
    }
    server {
        #nginx监听的端口号
        listen       80;
        #域名可以有多个，用空格隔开
       server_name  127.0.0.1;
        #字符编码方式
       charset utf-8;
        #设定本虚拟主机的访问日志。关闭日志可以减少IO，提高性能。
       #access_log logs/host.access.log  main;
        #默认请求
       location / {
            #定义服务器的默认网站根目录位置
            root   html;
            #定义首页索引文件的名称
            index  index.html index.htmindex.jsp;
            #请求转向mysvr 定义的服务器列表
            proxy_pass    http://myClusterServer1;
            proxy_redirect default;
            #跟代理服务器连接的超时时间，必须留意这个time out时间不能超过75秒，当一台服务器当掉时，过10秒转发到另外一台服务器。
            proxy_connect_timeout 10;
        }
       #error_page  404              /404.html;
        #redirect server error pages to the static page /50x.html
        #
       error_page   500 502 503 504  /50x.html;
       location = /50x.html {
           root   html;
        }
        #proxy the PHP scripts to Apache listening on 127.0.0.1:80
        #
        #location ~ \.php$ {
        #   proxy_pass   http://127.0.0.1;
        #}
        #pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000
        #
        #location ~ \.php$ {
        #    root           html;
        #    fastcgi_pass   127.0.0.1:9000;
        #    fastcgi_index  index.php;
        #    fastcgi_param  SCRIPT_FILENAME  /scripts$fastcgi_script_name;
        #    include        fastcgi_params;
        #}
        #deny access to .htaccess files, if Apache's document root
        #concurs with nginx's one
        #
        #location ~ /\.ht {
        #    deny  all;
        #}
    }
    # anothervirtual host using mix of IP-, name-, and port-based configuration
    #
    #server {
    #    listen      8000;
    #    listen      somename:8080;
    #    server_name somename  alias  another.alias;
    #    location / {
    #        root  html;
    #        index index.html index.htm;
    #    }
    #}
    # HTTPSserver
    #
    #server {
    #    listen      443 ssl;
    #    server_name localhost;
    #    ssl_certificate      cert.pem;
    #    ssl_certificate_key  cert.key;
    #    ssl_session_cache    shared:SSL:1m;
    #    ssl_session_timeout  5m;
    #    ssl_ciphers  HIGH:!aNULL:!MD5;
    #    ssl_prefer_server_ciphers  on;
    #    location / {
    #        root  html;
    #        index index.html index.htm;
    #    }
    #}
}
1.5.2启动与关闭Nginx
(1)启动
# sudo  /usr/local/nginx-1.6.1/sbin/nginx
确保系统的 80 端口没被其他程序占用
重启：
# sudo  /usr/local/nginx-1.6.1/sbin/nginx  -s  reload
(2)关闭：
ps -ef | grep nginx
找到主进程ID，然后kill即可，如：
# kill -9 [进程号]
![](https://img-blog.csdn.net/20140812143809671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh0NjY2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
(3)检查是否启动成功：
netstat -ano | grep80 有结果输入说明启动成功
打开浏览器访问此机器的 IP，如果浏览器出现Welcome to nginx! 则表示 Nginx 已经安装并运行成功。如果已经配置了负载均衡服务器，则会看Tomcat中的网站页面，如下图：
![](https://img-blog.csdn.net/20140812143841640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh0NjY2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 2. Memcache安装
Memcached官网：[http://memcached.org/](http://memcached.org/)
安装memcached需要先安装libevent，libevent官网：[http://libevent.org/](http://libevent.org/)
本次安装版本：
memcached-1.4.20.tar.gz
libevent-2.0.21-stable.tar.gz
2.1安装livevent
查看是否已安装：# rpm qa | grep libevent
如果已安装且版本低于1.3，则先通过：
# rpm -e libevent --nodeps 进行卸载。
# tar zxvf libevent-2.0.21-stable.tar.gz
# cd libevent-2.0.21-stable
# sudo ./configure --prefix=/usr/local/libevent-2.0.21-stable      #prefix指定安装路径
# sudo make
# sudo make install
安装完成后，到prefix指定的目录下，看看是否存在libevent-2.0.21-stable目录，如下图所示。
![](https://img-blog.csdn.net/20140812144126548?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh0NjY2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2.2安装Memcached
# tar zxvf memcached-1.4.20.tar.gz
# cd memcached-1.4.20
# sudo ./configure 
--prefix=/usr/local/memcached-1.4.20
--with-libevent=/usr/local/libevent-2.0.21-stable
# sudo make
# sudo make install
2.2.1检查看装情况
安装完成后，到prefix指定的目录下查看是否有memcached-1.4.20目录，如下图所示。
![](https://img-blog.csdn.net/20140812144247107?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh0NjY2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2.2.2查看memcached和libevent版本信息
首先定位到Memcached的bin目录下：
# cd /usr/local/memcached-1.4.20/bin
执行命令：
# sudo ./memcached -i
2.2.3启动memcached
# sudo ./memcached -d -v -p 12000 -m 512 -u zht
解释：-d表示以守护进程方式运行memcached；-v表示输出浸膏和错误信息；-p指定监听的端口号；-m指定能使用的最大内存，单位MB；-u指定运行memcached的账户，非root用户。
使用# ps -ef | grep memcached查看进程。
![](https://img-blog.csdn.net/20140812144329648?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh0NjY2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
|**关注基本选项：**|**说明**|
|----|----|
|-p <num>|监听的TCP端口 (缺省: 11211)|
|-d|以守护进程方式运行memcached|
|-u <username>|运行memcached的账户，非root用户|
|-m <num>|最大的内存使用，单位是MB，缺省是 64 MB|
|-c <num>|软连接数量，缺省是1024（最大并发连接数）|
|-v|输出警告和错误信息|
|-vv|打印客户端的请求和返回信息|
|-h|打印帮助信息|
|-i|打印memcached和libevent的版权信息|
2.2.4使用telnet验证服务是否可用
Win7安装telnet：控制面板-> 程序和功能->打开或安装Windows功能->勾选Telnet服务端、Telnet客户端。
![](https://img-blog.csdn.net/20140812144416698?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh0NjY2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
CentOS安装telnet客户端：# sudo yum install telnet
Windows命令：
![](https://img-blog.csdn.net/20140812144456961?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh0NjY2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
CentOS（Linux）下命令：
# telnet 127.0.0.1 12000
Trying 127.0.0.1...
Connected to localhost (127.0.0.1).
Escape character is '^]'.
连接成功后，手动输入命令：stats
即可看到如下信息：
![](https://img-blog.csdn.net/20140812144559861?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh0NjY2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
手动输入：quit   //退出
Connection closed by foreign host
2.2.5停止memcached服务
# ps -ef | grep memcached
找到memcached进程ID号，然后强制杀死：
kill - 9 2439
# 3.安装Tomcat+配置memcached
Tomcat官网：[http://tomcat.apache.org/](http://tomcat.apache.org/)
3.1安装Tomcat
本次使用Tomcat版本：apache-tomcat-7.0.55.tar.gz
将Tomcat解压到任意目录下。在同一台机器上安装，多个Tomcat时，端口号需要设置成不一样的，编辑三个Tomcat的server.xml将端口号都改成不一样的，我的三个Tomcat端口号分别是8081,8082,8083。Tomcat8081的server.xml文件内容如下（去掉了注释部分，其它两个做类似的修改）：
![](https://img-blog.csdn.net/20140817100505777?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh0NjY2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20140812144451000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh0NjY2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3.2为Tomcat配置memcached
3.2.1为Tomcat添加库文件
Tomcat要支持memcached管理Session，需要调用一些jar库文件：
![](https://img-blog.csdn.net/20140825150832437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh0NjY2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
msm1.6.5依赖了Couchbase，需要添加couchbase-client的jar包，否则启动会报：java.lang.NoClassDefFoundError:com/couchbase/client/CouchbaseClient。【注意】：
Tomcat6和Tomcat7使用不同msm支持包：memcached-session-manager-tc6-1.6.5.jar和memcached-session-manager-tc7-1.6.5.jar，只可选一，否则启动报错。
msm源码中的lib包版本太低：spymemcached需要使用2.10.2，否则启动tomcat报错：
java.lang.NoSuchMethodError:net.spy.memcached.MemcachedClient.set(Ljava/lang/String;ILjava/lang/Object;)Lnet/spy/memcached/internal/OperationFuture;
atde.javakaffee.web.msm.BackupSessionTask.storeSessionInMemcached(BackupSessionTask.java:227)
kryo-serializers需要使用0.10版本，否则报错：
Caused by:java.lang.ClassNotFoundException: de.javakaffee.kryoserializers.DateSerializer
部分文件下载地址：[http://code.google.com/p/memcached-session-manager/downloads/list](http://code.google.com/p/memcached-session-manager/downloads/list)
其他的文件自己找。
下载后，将这些库文件放到**tomcat\lib**目录下。
3.2.2为Tomcat配置memcached
配置文件目录：
**tomcat\conf\context.xml**
**tomcat\conf\server.xml**
打开context.xml文件，在<Context>...</Context>节点中添加如下内容：
<ManagerclassName="de.javakaffee.web.msm.MemcachedBackupSessionManager"
        memcachedNodes="n1:127.0.0.1:12000"
        sticky="false"
        requestUriIgnorePattern=".*\.(png|gif|jpg|css|js|ico|jpeg)$"
        sessionBackupAsync="false"
        sessionBackupTimeout="100"
        transcoderFactoryClass="de.javakaffee.web.msm.serializer.kryo.KryoTranscoderFactory"/>
【参数说明】：
docBase：与<Host>中的appBase一致，网站部署目录。
memcachedNodes：memcached服务器信息，多个服务器时，使用空格分开，如：
n1:127.0.0.1:12001 n2:127.0.0.1:12002  n3:127.0.0.1:12003
打开server.xml文件，在<Engine>节点的中添加jvmRoute="tomcatRoute1"，这个是用于指定Tomcat集群的路由。Tomcat2设置成jvmRoute="tomcatRoute2"，Tomcat3设置成jvmRoute="tomcatRoute3"，依次类推。
3.2.3测试Session共享
测试JSP代码如下：index.jsp
<html> 
< head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>Tomcat+memcached共享session测试</title> 
< /head>
< body>
SessionID:<%=session.getId()%>
<BR>  
SessionIP:<%=request.getServerName()%>
<BR>  
SessionPort:<%=request.getServerPort()%>
      <BR>
<%  
out.println("This is Tomcat Server 8081.");
%>
< /body> 
< /html>
同时启动多个Tomcat，我部署了3个，打开浏览器去访问第一个Tomcat，然后在访问第二个和第三个Tomcat，页面Session信息如下：
![](https://img-blog.csdn.net/20140812145544472?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh0NjY2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
由图可以看到，三个Tomcat的SessionID都是一样的：5FBF6D6B6F37BE8248ED965536427005-n1，只要不关闭浏览器，不管怎么刷新，SessionID都是不变了。由此可以，三个Tomcat通过memcached实现了Session信息共享。
**3.2.3 Manager标签属性说明**
className：此属性是必须的。
memcachedNodes：此属性是必须的。这个属性必须包含你所有运行的memcached节点。每个节点的定义格式为<id>:<host>:<port>。多个之间用空格或半角逗号隔开（如：memcachedNodes="n1:localhost:11211,n2:localhost:11212")。如果你设置单个memcache节点<id>是可选的，所以它允许设置为<host>:<port>（memcachedNodes="localhost:11211"）。
failoverNodes：可选项，属性只能用在非粘连Session机制中。此属性必须包含memcached节点的Id，此节点是Tomcat作为备份使用。多个之间用空格或逗号隔开。
memcachedProtocol：可选项，默认为text。出属性指明memcached使用的存储协议。只支持text或者binary。
sticky：可选项，默认为true。指定使用粘性的还是非粘性的Session机制。
lockingMode：可选项，此属性只对非粘性Session有用，默认为none。指定非粘性Session的锁定策略。它的值的只有：
              (1) none：从来不加锁
              (2) all：当请求时对Session锁定，直到请求结束
              (3) auto：对只读的request不加锁，对非只读的request加锁
              (4) uriPattern:<regexp>：使用正则表达式来比较requestRUI + "?" + queryString来决定是否加锁，
requestUriIgnorePattern：可选项，此属性是那些不能改备份Session的请求的正则表达式。如果像css、javascript、图片等静态文件被同一个Tomcat和同一个应用上下文来提供，这些请求也会通过memcached-session-manager。但是这些请求在一个http会话中几乎没什么改变，所以他们没必要触发Session备份。所以那些静态文件没必要触发Session备份，你就可以使用此属性定义。此属性必须符合java
 regex正则规范。
sessionBackupAsync：可选项，默认true。指定Session是否应该被异步保存到Memcached中。如果被设置为true，backupThreadCount设置起作用，如果设置false，通过sessionBackupTimeout设置的过期时间起作用。
backupThreadCount：可选项，默认为CPU内核数。用来异步保存Session的线程数(如果sessionBackupAsync="true")。
sessionBackupTimeout：可选项，默认100，单位毫秒。设置备份一个Session所用的时间，如果操作超过时间那么保存失败。此属性只在sessionBackupAsync="false"是起作用。默认100毫秒。
sessionAttributeFilter：可选项从1.5.0版本有。此属性是用来控制Session中的那个属性值保存到Memcached中的正则表达式。正则表达式被用来匹配Session中属性名称。如sessionAttributeFilter="^(userName|sessionHistory)$" 指定了只有"userName"和"sessionHistory"属性保存到Memcached中。依赖于选择的序列化策略。
transcoderFactoryClass：可选，默认为de.javakaffee.web.msm.JavaSerializationTranscoderFactory，此属性值是创建序列化和反序列化保存到Memcached中的Session的编码转换器的工厂类名。这个指定的类必须实现了de.javakaffee.web.msm.TranscoderFactory和提供一个无参的构造方法。例如其他的有效的实现在其他packages/jars中提供如：msm-kryo-serializer,msm-
 xstrea-serializer和msm-javolution-serializer.
copyCollectionsForSerialization：可选项，默认false。
customConverter：可选项
enableStatistics：可选项，默认true用来指定是否进行统计。
enabled：可选项，默认true。指定Session保存到Memcached中是否可用和是否可以通过JMX进行改变。只用于粘性Session。
# 4.安装Samba共享文件服务
查看是否已经安装samba：
# rpm -qa | grep samba
4.1安装samba
使用网络安装快速方便，命令如下：
# sudo yum install samba samba-client
4.2配置共享文件夹
(1)修改配置文件：
# sudo vi /etc/samba/smb.conf
[SharedFolder]
path = /home/zht/SharedFolder          #共享目录的路径
writeable = yes                       #是否允许写
browsable = yes                      #是否允许以浏览目录内容
valid user = zht                       #访问目录的账户名
(2)添加账户
添加zht账户：# sudosmbpasswd -a zht
按照提示输出访问密码。
4.3开启端口号
Port 137 (UDP)  --NetBIOS name server and nmbd
Port 138 (UDP) --NetBIOS datagram service
Port 139 (TCP) --File and printer sharing and smbd
Port 389 (TCP) --for LDAP(Active Directory Mode)
Port 445 (TCP) --NetBIOS was moved to 445 after 2000 and beyond(CIFS)
Port 901 (TCP) --for SWAT
(1) CentOS使用图形化添加端口号：sudo setup；
(2) 使用以下命令开启：
# iptables -I INPUT -p udp --dport [端口号] -j ACCEPT
# iptables -I INPUT -p tcp --dport [端口号] -j ACCEPT
保存：
# sudo service iptables save
4.4关闭SELinux
查看状态：# getenforce
关闭： # setenforce 0
SELinux几种状态：
enforcing：强制模式，代表SELinux运行中且正确限制；
permissive：宽容模式，代表SELinux运行中，不过仅有警告信息，并不实际限制；
disable：关闭，SELinux未运行。
永久关闭方法：
# sudo vi /etc/selinux/config
# SELINUX = enforcing          注释掉
# SELINUXTYPE = targeted      注释掉
在末尾添加一行：
SELINUX = disable
保存关闭：:wq
重启系统
4.5启动与停止服务
（1）查看运行状态：
# sudo service smb status
（2）启动和停止服务：
# sudo /etc/init.d/smb   start/stop/restart
或者
# sudo service smb    start/stop/restart
（3）设置开启启动：
# sudo chkconfig --level 35 smb on
在3,5级别上自动运行smb服务
（4）验证服务
# smbclient -L //192.168.101.249 -U rxyy
或者
# smbclient //192.168.101.249/sharedFolder -U rxyy
# 5安装配置JDK
本次使用JDK版本为：jdk-7u60-linux-x64.tar.gz
5.1卸载已安装的JDK
查看已经安装的JDK
# rpm -qa | grep jdk*
卸载JDK：
# sudo yum -y remove [包名]
如下图所示：
![](https://img-blog.csdn.net/20140812145654999?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh0NjY2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
安装新的JDK：将jdk-7u60-linux-x64.tar.gz解压缩到指定目录下即可。
5.2配置Java环境变量
# sudo vi /etc/profile
在末尾添加：
#JDK
exportJAVA_HOME=/home/zht/BalanceServer/Java/jdk1.7.0_60
export JRE_HOME=$JAVA_HOME/jre
export CLASSPATH=$CLASSPATH:.:$JRE_HOME/lib:$JAVA_HOME/lib
export PATH=$PATH:$JAVA_HOME/bin:$JRE_HOME/bin
# 6安装MySQL数据库
6.1卸载已安装的[MySQL](http://lib.csdn.net/base/14)
查看已经安装的mysql数据库：
# rpm -qa | grep mysql
卸载已安装的mysql数据库：
# rpm -e [包名]                //普通卸载，可能因为依赖无法卸载
# rpm -e --nodeps [包名]         //强制卸载，有依赖也能卸载
或者
# yum -y remove [包名]         //卸载时自动处理依赖
6.2安装mysql
    安装MySQL数据库需要安装4个组件：mysql--shared-compat 、mysql-server、mysql-client和mysql-workbench，安装方法如下。
（1）安装mysql共享库：
# rpm -ivh MySQL-shared-compat-5.6.20-1.el6.x86_64.rpm
（2）安装mysql服务端：：
# rpm -ivh MySQL-server-5.6.20-1.el6.x86_64.rpm
（3）安装mysql客户端：
# rpm -ivh MySQL-client-5.6.20-1.el6.x86_64.rpm
（4）安装mysql-workbench工具：
# rpm -ivh mysql-workbench-community-6.1.7-1.el6.x86_64.rpm
【说明】：RPM安装包，在CentOS下可以直接双击运行安装，系统会自动处理依赖。
另附：yum安装方法
查看yum上可安装的版本：# yumlist | grep mysql
安装：# yum install -y mysql-server mysql mysql-devel
6.3配置mysql
配置文件目录：/usr/my.cnf或者/etc/mysql/my.cnf
打开文件后，在[mysqld]后面添加如下配置：
#字符编码
character-set-server=utf-8
#存储引擎
default-storage-engine=INNODB
innodb_flush_log_at_trx_commit=2
#缓冲池
innodb_additional_mem_pool_size=16M
innodb_buffer_pool_size=1024M
#最大连接数
max_connections=1000
#允许的最大包大小（例如一个Blob字段）
max_allowed_packet=16M
![](https://img-blog.csdn.net/20140812145756214?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh0NjY2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
6.4启动mysql
（1）启动mysql：
# sudo service mysql  start/stop/restart
或
# sudo /etc/rc.d/init.d/mysql  start/stop/restart
（或 # sudo /etc/rc.d/init.d/mysqld  start/stop/restart）
启动mysql出错时，请到/var/liv/mysql/xxx.err查看错误信息。
![](https://img-blog.csdn.net/20140812145841766?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh0NjY2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（2）登录MySQL
# mysql -u root -p
# 输入密码
# mysql> show databases;
# mysql> show variables like '%character_set%'
# mysql> ...
（3）创建远程连接用户
启用sxzl用户在本地（localhost）的登录权限，密码hymmldr.
# mysql> grant all privileges on *.* tosxzl@localhost identified by 'hymmldr.' with grant option;
启用sxzl用户远程（%）登录权限，密码hymmldr.
# mysql> grant all privileges on *.* to sxzl@"%"identified by 'hymmldr.' with grant option;
刷新权限
# mysql> flush privileges;
第一次安装需要重设mysql密码，详见如下。
6.5重设mysql密码（官网方法）
启动mysql，然后执行如下命令：
# ps -ef | grep -i mysql
查看mysqld_safe的路径，例如：/usr/bin/mysqld_safe
![](https://img-blog.csdn.net/20140812145919378?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh0NjY2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
停止mysql，然后以安全方式启动mysql：
**# sudo  /usr/bin/mysqld_safe  --skip-grant-tables  >/dev/null 2>&1  &**
5秒钟后执行：
# sudo /usr/bin/mysql -u root mysql
重设root用户的密码：
# mysql> update user SET PASSWORD=PASSWORD('root')where user='root'
刷新权限
# mysql> flush privileges;
# mysql> exit;
接着再次登录mysql：
# mysql -u root -p
# 输入密码
然后执行：
# mysql> show databases;
报错：You must SET PASSWORD before executing thisstatement.
解决办法，重设一次密码：
# mysql> SET PASSWORD=PASSWORD('root');
# mysql> flush privileges;
设置完毕后，就可以打开mysql-workbench连接到数据库了。
# 7命令说明：yum
yum安装选项说明：
yum -y install 包名（支持*） ：自动选择y，全自动
yum install 包名（支持*） ：手动选择y or n
yum -y remove [package name] ：自动处理依赖
yum remove 包名（不支持*）
rpm -ivh 包名（支持*）：安装rpm包
rpm -e 包名（不支持*）：卸载rpm包
本文文档下载地址：[http://download.csdn.net/detail/zht666/7744157](http://download.csdn.net/detail/zht666/7744157)
