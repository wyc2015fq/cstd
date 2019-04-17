# Nginx+Tomcat服务器负载均衡实践方案 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年07月13日 23:51:56[boonya](https://me.csdn.net/boonya)阅读数：5920











# 1.    为何选择Nginx+Tomcat做负载均衡？

## 1.1. Nginx是什么？

 Nginx（发音同 engine x）是一款轻量级的[Web](http://baike.baidu.com/view/3912.htm) 服务器/反向代理服务器及电子邮件（IMAP/POP3）代理服务器，并在一个BSD-like 协议下发行。由俄罗斯的程序设计师Igor
 Sysoev所开发，供俄国大型的入口网站及搜索引擎Rambler（俄文：Рамблер）使用。其特点是占有[内存](http://baike.baidu.com/view/1082.htm)少，[并发](http://baike.baidu.com/view/684757.htm)能力强，事实上nginx的并发能力确实在同类型的网页服务器中表现较好，中国大陆使用nginx网站用户有：[新浪](http://baike.baidu.com/view/2410.htm)、[网易](http://baike.baidu.com/view/6043.htm)、 [腾讯](http://baike.baidu.com/view/1591.htm)等。

## 1.2. Nginx特点

     Nginx是一款面向性能设计的HTTP服务器，相较于[Apache](http://zh.wikipedia.org/wiki/Apache_HTTP_Server)、[lighttpd](http://zh.wikipedia.org/wiki/Lighttpd)具有占有[内存](http://zh.wikipedia.org/wiki/%E5%86%85%E5%AD%98)少，稳定性高等优势。与旧版本（<=2.2）的[Apache](http://zh.wikipedia.org/wiki/Apache_HTTP_Server)不
 同，nginx不采用每客户机一线程的设计模型，而是充分使用异步逻辑，削减了上下文调度开销，所以并发服务能力更强。整体采用模块化设计，有丰富的模块 库和第三方模块库，配置灵活。  在Linux操作系统下，nginx使用epoll事件模型，得益于此，nginx在Linux操作系统下效率相当高。同时Nginx在OpenBSD或FreeBSD操作系统上采用类似于epoll的高效事件模型kqueue。

## 1.3. Nginx可大量平行处理

    Nginx在官方测试的结果中，能够支持五万个平行连接，而在实际的运作中，是可以支持二万至四万个平行链接。

## 1.4. nginx 的模块

整体采用模块化设计是nginx的一个重大特点，甚至http服务器核心功能也是一个模块。要注意的是：nginx的模块是静态的，添加和删除模块都要对nginx进行重新编译，这一点与Apache的动态模块完全不同。

## 1.5. Nginx与PHP的集成

自PHP-5.3.3起，PHP-FPM加入到了PHP核心，编译时加上--enable-fpm即可提供支持。  PHP-FPM以守护进程在后台运行，Nginx响应请求后，自行处理静态请求，PHP请求则经过fastcgi_pass交由PHP-FPM处理，处理 完毕后返回。 Nginx和PHP-FPM的组合，是一种稳定、高效的PHP运行方式，效率要比传统的Apache和mod_php高出不少。

    PHP-FPM不支持Windows平台，由于没有fastcgi进程管理器管理php-cgi.exe，因此一旦php-cgi.exe崩溃退 出，前端将失去响应，这时Nginx会返回“The page you are looking for is temporarily  unavailable. Please try again
 later.”的错误信息。  因此在Windows上用Nginx和php-cgi.exe组合来运行PHP的方式并不可靠，稳定性有待提高。

## 1.6. Tomcat与Nginx性能对比

  Tomcat 服务器作为一个 Web 服务器，其并发数在 300-500 之间，如果有超过 500的并发数便会出现 Tomcat 不能响应新的请求的情况，严重影响网站的运行。另外，在访问量大的情况下，Tomcat 的线程数会不断增加。由于Tomcat 自身对内存的占用有控制，当对内存的占用达到最大值时便会出现内存溢出，对网站的访问严重超时等现象，这时便需要重新启动 Tomcat 以释放占用的内存，这样做便会阻断网站运行。所以对 Tomcat 做负载均衡便很有必要。目前可以和 Tomcat 做负载均衡的主流服务器是 Apache ，但是 Nginx 由于功能多、配置简单等优点逐渐成为很多负载均衡服务器的首选。 Nginx 的并发数可达到50000 ，所以理论上可以和Tomcat 以 1:100 的比例来配置，这样便可以很好的解决网站并发瓶颈问题。



# 2.    Nginx+Tomcat负载均衡方案实施

## 2.1. 环境及软件安装工具准备

### 2.1.1.下载JDK7

在官网（http://www.oracle.com/us/sun/index.htm）上下载JDK即可，下载地址：

[http://java.com/zh_CN/download/index.jsp](http://java.com/zh_CN/download/index.jsp)

### 2.1.2.下载Nginx

根据自己的需要下载合适的系统版本，目前官方对Windows系列平台只提供Windows 32bit的下载，下载地址：[http://nginx.org/en/download.html](http://nginx.org/en/download.html)

### 2.1.3.下载Tomcat7

 Tomcat需要JDK运行环境支持，由于我们下载的是JDK7所以选择对应的Tomcat也是7。建议下载最新的Tomcat7版本，下载地址：[http://tomcat.apache.org/download-70.cgi](http://tomcat.apache.org/download-70.cgi)

注：下载Binary版本，没有必要下载Resource版本。

## 2.2. 安装JDK并设置Tomcat和Nginx

### 2.2.1.安装JDK

下载完成后，系统会自动弹出安装文件提示用户安装。直接点击“同意协议”，然后直接默认安装JDK路径，点“下一步”知道完成为止（对于JDK安装这个没多大的技术难度，JDK怎么安装已经在多处文档中写过了，故不再详述），记得验证安装是否成功。  参见文档：《WEB应用打包部署》。

### 2.2.2.解压Tomcat

将下载的Tomcat服务器包解压到任意路径下，如：D:\BALANCE_TOMCAT__NGINX由于我已经下载Tomcat7，就用我本机的apache-tomcat-7.0.47作为示例。解压两个Tomcat分别命名为apache-tomcat-7.0.47-1何apache-tomcat-7.0.47-2.解压后如图所示：

![](http://p53-4.yunpan.360.cn/8b612d9faae67b6fa0c57ca44b198376985fc6d9/f5db2c42e553ca23cc46c319dcd15d8a.files/image001.png)

### 2.2.3.启动Nginx

之所以不以安装为名是因为Nginx本身是控制台程序而并未做出Windows服务；将下载的Nginx1.6.0解压至与Tomcat相同的D:\BALANCE_TOMCAT__NGINX路径下。切换文件夹路径找到D:\BALANCE_TOMCAT__NGINX\nginx-1.6.0：

![](http://p53-4.yunpan.360.cn/8b612d9faae67b6fa0c57ca44b198376985fc6d9/f5db2c42e553ca23cc46c319dcd15d8a.files/image002.png)

在控制台“cmd”，执行“start nginx”命令启动上图中的exe程序。

![](http://p53-4.yunpan.360.cn/8b612d9faae67b6fa0c57ca44b198376985fc6d9/f5db2c42e553ca23cc46c319dcd15d8a.files/image004.jpg)

启动后会看到控制台闪一下；这时查看系统进程中有两个nginx.exe的进程，说明启动完成。

![](http://p53-4.yunpan.360.cn/8b612d9faae67b6fa0c57ca44b198376985fc6d9/f5db2c42e553ca23cc46c319dcd15d8a.files/image005.png)

### 2.2.4.负载均衡设置

#### 2.2.4.1.       修改Tomcat设置

这里需要修改两个Tomcat的server.xml文件：



**apache-tomcat-7.0.47-1配置：**



1、<Server port="18080" shutdown="SHUTDOWN">



2、 <Connector port="18080" protocol="HTTP/1.1"

               connectionTimeout="20000"

               redirectPort="18443" />



3、 <Connector port="18009" protocol="AJP/1.3" redirectPort="18443" />



4、  <Engine name="Catalina" defaultHost="localhost" jvmRoute="tomcat1">

5、 另外<Host>中添加<Content>设置:

  <Context path="/nginx" reloadable="false" docBase="D:/DEVELOPER_WORKSPACE_AJX/workspace/nginx/WebRoot"> </Context>

注：docBase是项目物理路径。



**apache-tomcat-7.0.47-2配置：**



2、<Server port="28080" shutdown="SHUTDOWN">



2、 <Connector port="28080" protocol="HTTP/1.1"

               connectionTimeout="20000"

               redirectPort="28443" />



6、 <Connector port="28009" protocol="AJP/1.3" redirectPort="28443" />



7、  <Engine name="Catalina" defaultHost="localhost" jvmRoute="tomcat2">

8、 另外<Host>中添加<Content>设置:

  <Context path="/nginx" reloadable="false" docBase="D:/DEVELOPER_WORKSPACE_AJX/workspace/nginx/WebRoot"> </Context>

注：docBase是项目物理路径。



#### 2.2.4.2.       修改Nginx设置

打开Nginx1.6.0/conf/nginx.conf进行修改，nginx.conf内容如下：



```
#user  nobody;
#此参数修改为与CPU个数一致
worker_processes  1;
#error_log  logs/error.log;
#error_log  logs/error.log  notice;
#error_log  logs/error.log  info;
 
pid        logs/nginx.pid;
 
#后添的
worker_rlimit_nofile 51200;
 
events {
    worker_connections  51200;
}
 
http {
    include       mime.types;
    default_type  application/octet-stream;
 
    #log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
    #                  '$status $body_bytes_sent "$http_referer" '
    #                  '"$http_user_agent" "$http_x_forwarded_for"';
 
    #access_log  logs/access.log  main;
 
    server_names_hash_bucket_size 128;
    client_header_buffer_size 32k;     
    large_client_header_buffers 4 32k;
 
    sendfile        on;
    #tcp_nopush     on;
 
    #keepalive_timeout  0;
    keepalive_timeout  60;
   
    tcp_nodelay on;
 
    #gzip  on;
   
    upstream abc.tomcats.com {
       #ip_hash;
       server 192.168.1.147:18080 weight=1;
       server 192.168.1.147:28080 weight=2;
       }
 
    server {
        listen       80;
        server_name  abc.tomcats.com;
 
proxy_redirect off;
 
access_log logs/abc.tomcats.com.log combined;
        #charset koi8-r;
        #access_log  logs/host.access.log  main;
 
#充许客户端请求的最大单个文件字节数
client_max_body_size 10m;
 
client_body_buffer_size 128k;
 
#跟后端服务器连接的超时时间
proxy_connect_timeout 600;
 
#连接成功后等候后端服务器响应时间
proxy_read_timeout 600;
 
#后端服务器数据回传时间
proxy_send_timeout 600;
 
#代理请求缓存区
proxy_buffer_size 8k;
 
#同上，保存用几个buffer每个最大空间是多少
proxy_buffers 4 32k;
 
#如果系统很忙时可以申请更大的proxy_buffers，官方推荐*2
proxy_busy_buffers_size 64k;
 
#缓存临时文件的大小
proxy_temp_file_write_size 64k;
 
        location / {
            root   html;
            index  index.html index.htm;
            proxy_pass   http://abc.tomcats.com;
            proxy_set_header Host $host;
            proxy_set_header X-Real-Ip $remote_addr;
            proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        }
 
        location /status{
            stub_status on;
            access_log off;
            auth_basic "NginxStatus";
            auth_basic_user_file htpasswd;
              }
              location ~ \.jsp$ {
            proxy_pass  http://abc.tomcats.com;
              }              
 
        location ~ \.(html|js|css|png|gif)$ {
            root html;
                     proxy_pass  http://abc.tomcats.com;
        }
 
        #error_page  404              /404.html;
 
        # redirect server error pages to the static page /50x.html
        #
        error_page   500 502 503 504  /50x.html;
        location = /50x.html {
            root   html;
        }
 
        # proxy the PHP scripts to Apache listening on 127.0.0.1:80
        #
        #location ~ \.php$ {
        #    proxy_pass   http://127.0.0.1;
        #}
 
        # pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000
        #
        #location ~ \.php$ {
        #    root           html;
        #    fastcgi_pass   127.0.0.1:9000;
        #    fastcgi_index  index.php;
        #    fastcgi_param  SCRIPT_FILENAME /scripts$fastcgi_script_name;
        #    include        fastcgi_params;
        #}
 
        # deny access to .htaccess files, if Apache's document root
        # concurs with nginx's one
        #
        #location ~ /\.ht {
        #    deny  all;
        #}
    }
 
    # another virtual host using mix of IP-, name-, and port-based configuration
    #
    #server {
    #    listen       8000;
    #    listen       somename:8080;
    #    server_name  somename  alias  another.alias;
 
    #    location / {
    #        root   html;
    #        index  index.html index.htm;
    #    }
    #}
 
 
    # HTTPS server
    #
    #server {
    #    listen       443;
    #    server_name  localhost;
 
    #    ssl                  on;
    #    ssl_certificate      cert.pem;
    #    ssl_certificate_key  cert.key;
 
    #    ssl_session_timeout  5m;
 
    #    ssl_protocols  SSLv2 SSLv3 TLSv1;
    #    ssl_ciphers ALL:!ADH:!EXPORT56:RC4+RSA:+HIGH:+MEDIUM:+LOW:+SSLv2:+EXP;
    #    ssl_prefer_server_ciphers   on;
 
    #    location / {
    #        root   html;
    #        index  index.html index.htm;
    #    }
    #}
 
}
```




然后使用“Nginx -s reload”命令重新加载Nginx的设置。

![](http://p53-4.yunpan.360.cn/8b612d9faae67b6fa0c57ca44b198376985fc6d9/f5db2c42e553ca23cc46c319dcd15d8a.files/image007.jpg)

### 2.2.5.项目部署及启动

#### 2.2.5.1.       部署web项目

将J2EE项目打包成war包分别放置到apache-tomcat-7.0.47-1和apache-tomcat-7.0.47-2的webapps目录下，然后分别启动Tomcat就行了。

注：webapps下放置的war版本要一致，这是两个相同的程序服务器部署。

#### 2.2.5.2.       启动Tomcat和Nginx

2个Tomcat随便启动无先后顺序，并且Nginx与Tomcat启动也无先后顺序。如果修改了nginx.conf只需要执行“Nginx -s reload”命令重新加载Nginx的设置。而如果修改了Tomcat的配置则需要重新启动Tomcat和Nginx。



## 2.3. 启动测试负载均衡实施效果

### 2.3.1.导航到服务器默认界面

在浏览器输入输入localhost即可看到如下界面：

![](http://p53-4.yunpan.360.cn/8b612d9faae67b6fa0c57ca44b198376985fc6d9/f5db2c42e553ca23cc46c319dcd15d8a.files/image009.jpg)

### 2.3.2.访问静态html文件

![](http://p53-4.yunpan.360.cn/8b612d9faae67b6fa0c57ca44b198376985fc6d9/f5db2c42e553ca23cc46c319dcd15d8a.files/image011.jpg)

![](http://p53-4.yunpan.360.cn/8b612d9faae67b6fa0c57ca44b198376985fc6d9/f5db2c42e553ca23cc46c319dcd15d8a.files/image013.jpg)

注：不一定用localhost，你也可以通过ip地址访问。

### 2.3.3.访问JSP动态页面

![](http://p53-4.yunpan.360.cn/8b612d9faae67b6fa0c57ca44b198376985fc6d9/f5db2c42e553ca23cc46c319dcd15d8a.files/image015.jpg)

## 2.4. 配置Memcached 实现session共享

### 2.4.1.下载Memcached

你可以从这里下载对应系统版本的Memcached：[http://pecl.php.net/package/memcache](http://pecl.php.net/package/memcache)

Memcached与Nginx相似，安装这里就不再说了。

### 2.4.2.Windows Memcached常用命令

以下是 memcached 在 Windows 系统下的 x86/x64 版本的安装方法。命令提示符下运行如下，

-----------------------------------------安装--------------------：

memcached -d install

memcached -d start

net start "Memcached Server"



----------------------------------------卸载-------------------：

memcached -d stop

memcached -d remove

sc delete "Memcached Server"



-------------------------------memcached的基本设置：

-p 监听的端口

-l 连接的IP地址, 默认是本机

-d start 启动memcached服务

-d restart 重起memcached服务

-d stop|shutdown 关闭正在运行的memcached服务

-d install 安装memcached服务

-d uninstall 卸载memcached服务

-u 以的身份运行 (仅在以root运行的时候有效)

-m 最大内存使用，单位MB。默认64MB

-M 内存耗尽时返回错误，而不是删除项

-c 最大同时连接数，默认是1024

-f 块大小增长因子，默认是1.25

-n 最小分配空间，key+value+flags默认是48

-h 显示帮助

### 2.4.3.设置Tomcat的Memcached

下载如下jar包 （这些jar不能缺失，否则会出现问题）![](http://p53-4.yunpan.360.cn/8b612d9faae67b6fa0c57ca44b198376985fc6d9/f5db2c42e553ca23cc46c319dcd15d8a.files/image016.jpg)全部拷贝进TOMCAT/lib中全部的tomcat：然后修改 conf/context.xml添加如下代码：   



```
<Manager  className="de.javakaffee.web.msm.MemcachedBackupSessionManager"  sticky="false"  
     memcachedNodes="n1:localhost:11211"   
     failoverNodes=""  
     lockingMode="all"   
     requestUriIgnorePattern=".*\.(png|gif|jpg|css|js|ico)$"   
     sessionBackupAsync="false"
     sessionBackupTimeout="100"         
     transcoderFactoryClass="de.javakaffee.web.msm.serializer.kryo.KryoTranscoderFactory"        />
```



因为支持memcached分布式  如果有多台memcached在memcachedNodes="nx:IP:port" 即可。

注意：这里的端口号要和启动的设置的端口一致

打开cmd 进入nginx 的解压目录 输入 nginx -s -stop  输入 nginx start  重新启动nginx依次启动三台tomcat。在TOMCAT \webapps\ROOT 下新增一张页面demo.jsp：



```
<%@ page contentType="text/html; charset=UTF-8" %>   
<%@ page import="java.util.*" %>
<html>
<head><title>Cluster Test</title></head>   
<body>  
 <%    
 HttpSession sess = request.getSession(true);    
 System.out.println(sess.getId());  
 out.println("<br> SESSION ID:" + sess.getId()+"<br>");    
 // 如果有新的请求，则添加session属性  
 String name = request.getParameter("name");     
if (name != null && name.length() > 0) {   
      String value = request.getParameter("value");     
      sess.setAttribute(name, value);  
}        
out.print("<b>Session List:</b>");     
Enumeration<String> names = sess.getAttributeNames();      
while (names.hasMoreElements()) {     
     String sname = names.nextElement();      
      String value = sess.getAttribute(sname).toString();       
      out.println( sname + " = " + value+"<br>");      
      System.out.println( sname + " = " + value);  
} 
  %>jvm1
</body>  
 </html>
```



JVM 分别是 1 2 3 分别放进 t1 t2 t3 中 然后打开浏览器http://localhost/demo.jsp 多次刷新看到不同的jvm 相同的session 表示成功nginx + tomcat + memcached 环境就搭建好了。







