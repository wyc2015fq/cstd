# window7环境下配置nginx+tomcat+memcached集群 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年05月25日 14:19:09[boonya](https://me.csdn.net/boonya)阅读数：2026









**From：[http://blog.csdn.net/luckykapok918/article/details/39500545](http://blog.csdn.net/luckykapok918/article/details/39500545)**

**首先整个安装软件说明  ：windows7 ， nginx-1.7.4  ，memcache1.4.13 ，分别在本机创建了2个服务memcached1 （端口 11211 ）和  memcached2（端口 11212 ）.**


`1.首先整个安装软件说明`


`  windows7``  nginx-1.7.4`** tomcat分别为apache-tomcat-6.0.41 （端口 18081 ）和apache-tomcat-7.0.54 （端口 18080），主要是测试不同版本的tomcat的配置**


1.1 首先安装nginx，本人使用的版本是nginx-1.7.4，并且把nginx作为系统服务，具体操作见下面：

[http://blog.csdn.net/luckykapok918/article/details/39501259](http://blog.csdn.net/luckykapok918/article/details/39501259)


![](https://img-blog.csdn.net/20140923170725187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


修改nginx的conf文件下的nginx.txt配置文件





**[plain]**[view plain](http://blog.csdn.net/luckykapok918/article/details/39500545#)[copy](http://blog.csdn.net/luckykapok918/article/details/39500545#)[print](http://blog.csdn.net/luckykapok918/article/details/39500545#)[?](http://blog.csdn.net/luckykapok918/article/details/39500545#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- #user  nobody;  
- worker_processes  1;  
- 
- #error_log  logs/error.log;  
- #error_log  logs/error.log  notice;  
- #error_log  logs/error.log  info;  
- 
- #pid        logs/nginx.pid;  
- 
- 
- events {  
-     worker_connections  1024;  
- }  
- 
- 
- http {  
-     include       mime.types;  
-     default_type  application/octet-stream;  
- 
-     #log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '  
-     #                  '$status $body_bytes_sent "$http_referer" '  
-     #                  '"$http_user_agent" "$http_x_forwarded_for"';  
- 
-     #access_log  logs/access.log  main;  
- 
-     sendfile        on;  
-     #tcp_nopush     on;  
- 
-     #keepalive_timeout  0;  
-     keepalive_timeout  65;  
- 
-     #gzip  on;  
-     upstream localhost {  
-         #ip_hash  
-       #ip_hash;  
-       server localhost:18081;  
-       server localhost:18080;  


```
#user  nobody;
worker_processes  1;

#error_log  logs/error.log;
#error_log  logs/error.log  notice;
#error_log  logs/error.log  info;

#pid        logs/nginx.pid;


events {
    worker_connections  1024;
}


http {
    include       mime.types;
    default_type  application/octet-stream;

    #log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
    #                  '$status $body_bytes_sent "$http_referer" '
    #                  '"$http_user_agent" "$http_x_forwarded_for"';

    #access_log  logs/access.log  main;

    sendfile        on;
    #tcp_nopush     on;

    #keepalive_timeout  0;
    keepalive_timeout  65;

    #gzip  on;
   	upstream localhost {
   		#ip_hash
      #ip_hash;
      server localhost:18081;
      server localhost:18080;
```



**[plain]**[view plain](http://blog.csdn.net/luckykapok918/article/details/39500545#)[copy](http://blog.csdn.net/luckykapok918/article/details/39500545#)[print](http://blog.csdn.net/luckykapok918/article/details/39500545#)[?](http://blog.csdn.net/luckykapok918/article/details/39500545#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

-       server 192.168.80.128:8080;  
-      }  
- 
-     server {  
-         listen       80;  
-         server_name  localhost;  
- 
- 
-         location / {  
-                     proxy_connect_timeout   3;  
-                     proxy_send_timeout      30;  
-                     proxy_read_timeout      30;  
-                 proxy_pass http://localhost;  
-             }  
- 
-         #charset koi8-r;  
- 
-         #access_log  logs/host.access.log  main;  
- 
- 
-         #error_page  404              /404.html;  
- 
-         # redirect server error pages to the static page /50x.html  
-         #  
-         error_page   500 502 503 504  /50x.html;  
-         location = /50x.html {  
-             root   html;  
-         }  
- 
-         # proxy the PHP scripts to Apache listening on 127.0.0.1:80  
-         #  
-         #location ~ \.php$ {  
-         #    proxy_pass   http://127.0.0.1;  
-         #}  
- 
-         # pass the PHP scripts to FastCGI server listening on 127.0.0.1:9000  
-         #  
-         #location ~ \.php$ {  
-         #    root           html;  
-         #    fastcgi_pass   127.0.0.1:9000;  
-         #    fastcgi_index  index.php;  
-         #    fastcgi_param  SCRIPT_FILENAME  /scripts$fastcgi_script_name;  
-         #    include        fastcgi_params;  
-         #}  
- 
-         # deny access to .htaccess files, if Apache's document root  
-         # concurs with nginx's one  
-         #  
-         #location ~ /\.ht {  
-         #    deny  all;  
-         #}  
-     }  
- 
- 
-     # another virtual host using mix of IP-, name-, and port-based configuration  
-     #  
-     #server {  
-     #    listen       8000;  
-     #    listen       somename:8080;  
-     #    server_name  somename  alias  another.alias;  
- 
-     #    location / {  
-     #        root   html;  
-     #        index  index.html index.htm;  
-     #    }  
-     #}  
- 
- 
-     # HTTPS server  
-     #  
-     #server {  
-     #    listen       443 ssl;  
-     #    server_name  localhost;  
- 
-     #    ssl_certificate      cert.pem;  
-     #    ssl_certificate_key  cert.key;  
- 
-     #    ssl_session_cache    shared:SSL:1m;  
-     #    ssl_session_timeout  5m;  
- 
-     #    ssl_ciphers  HIGH:!aNULL:!MD5;  
-     #    ssl_prefer_server_ciphers  on;  
- 
-     #    location / {  
-     #        root   html;  
-     #        index  index.html index.htm;  
-     #    }  
-     #}  
- 
- }  


```
server 192.168.80.128:8080;
     }
	 
    server {
        listen       80;
        server_name  localhost;
		
		
		location / {
            		proxy_connect_timeout   3;
            		proxy_send_timeout      30;
            		proxy_read_timeout      30;
                proxy_pass http://localhost;
            }

        #charset koi8-r;

        #access_log  logs/host.access.log  main;


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
        #    fastcgi_param  SCRIPT_FILENAME  /scripts$fastcgi_script_name;
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
    #    listen       443 ssl;
    #    server_name  localhost;

    #    ssl_certificate      cert.pem;
    #    ssl_certificate_key  cert.key;

    #    ssl_session_cache    shared:SSL:1m;
    #    ssl_session_timeout  5m;

    #    ssl_ciphers  HIGH:!aNULL:!MD5;
    #    ssl_prefer_server_ciphers  on;

    #    location / {
    #        root   html;
    #        index  index.html index.htm;
    #    }
    #}

}
```







1.2.安装memcache，具体操作见下面：

[http://blog.csdn.net/luckykapok918/article/details/10005583](http://blog.csdn.net/luckykapok918/article/details/10005583)

![](https://img-blog.csdn.net/20140923170920750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





1.3.解压tomcat免安装压缩包到，具体如下图：





![](https://img-blog.csdn.net/20140923170457500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)










![](https://img-blog.csdn.net/20140923170613562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


         因为是在一台机器上做测试，所以还需对tomcat的配置文件端口、jvm等做修改，以满足一台机器测试所用：




     a. tomcat6需要配置的文件，如下：



    server.xml文件中4处修改（如果是安装在不同机器上则不需要）




    第一处端口修改：



Xml代码  ![收藏代码](http://ari.iteye.com/images/icon_star.png)

- <!--  修改原port端口：8005 俩个tomcat不能重复，端口随意，别太小-->
- <Serverport="18006"shutdown="SHUTDOWN">




   第二处端口修改：



Xml代码  ![收藏代码](http://ari.iteye.com/images/icon_star.png)

- <!-- 原port="8080" tomcat监听端口，随意设置，别太小 -->
- <Connectorport="18081"protocol="HTTP/1.1"
- connectionTimeout="20000"
- redirectPort="8443"/>
- 
- 





   第三处端口修改：



Java代码  ![收藏代码](http://ari.iteye.com/images/icon_star.png)

- <Connector port="8009" protocol="AJP/1.3" redirectPort="8443" />  





   第四处修改：Engine元素增加jvmRoute属性：



Xml代码  ![收藏代码](http://ari.iteye.com/images/icon_star.png)

- <Enginename="Catalina"defaultHost="localhost"jvmRoute="tomcat1">




     b. tomcat7需要配置的文件，如下：

    server.xml文件中4处修改（如果是安装在不同机器上则不需要）




    第一处端口修改：



Xml代码  ![收藏代码](http://ari.iteye.com/images/icon_star.png)

- <!--  修改原port端口：8005
 俩个tomcat不能重复，端口随意，别太小-->
- <Serverport="18007"shutdown="SHUTDOWN">




   第二处端口修改：



Xml代码  ![收藏代码](http://ari.iteye.com/images/icon_star.png)

- <!-- 原port="8080" tomcat监听端口，随意设置，别太小 -->
- <Connectorport="18080"protocol="HTTP/1.1"
- connectionTimeout="20000"
- redirectPort="8443"/>
- 
- 





   第三处端口修改：



Java代码  ![收藏代码](http://ari.iteye.com/images/icon_star.png)

- <Connector port="8010" protocol="AJP/1.3" redirectPort="8443" />  





第四处修改：Engine元素增加jvmRoute属性：



Xml代码  ![收藏代码](http://ari.iteye.com/images/icon_star.png)

- <Enginename="Catalina"defaultHost="localhost"jvmRoute="tomcat2">







   c. tomcat6和tomcat7都需要修改的：

     context.xml文件中 增加

  kryo策略模式（一般来说采用kryo模式下的Non-sticky Session方式，性能最好）





**[html]**[view plain](http://blog.csdn.net/luckykapok918/article/details/39500545#)[copy](http://blog.csdn.net/luckykapok918/article/details/39500545#)[print](http://blog.csdn.net/luckykapok918/article/details/39500545#)[?](http://blog.csdn.net/luckykapok918/article/details/39500545#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- <ManagerclassName="de.javakaffee.web.msm.MemcachedBackupSessionManager"
- memcachedNodes="n1:localhost:11211,n2:localhost:11212"
- sticky="false"
- sessionBackupAsync="false"
- lockingMode="uriPattern:/path1|/path2"
- requestUriIgnorePattern=".*\.(ico|png|gif|jpg|css|js)$"
- transcoderFactoryClass="de.javakaffee.web.msm.serializer.kryo.KryoTranscoderFactory"
- />


```
<Manager className="de.javakaffee.web.msm.MemcachedBackupSessionManager"
    memcachedNodes="n1:localhost:11211,n2:localhost:11212"
    sticky="false"
    sessionBackupAsync="false"
    lockingMode="uriPattern:/path1|/path2"
    requestUriIgnorePattern=".*\.(ico|png|gif|jpg|css|js)$"
    transcoderFactoryClass="de.javakaffee.web.msm.serializer.kryo.KryoTranscoderFactory"
    />
```



**[html]**[view plain](http://blog.csdn.net/luckykapok918/article/details/39500545#)[copy](http://blog.csdn.net/luckykapok918/article/details/39500545#)[print](http://blog.csdn.net/luckykapok918/article/details/39500545#)[?](http://blog.csdn.net/luckykapok918/article/details/39500545#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- <spanstyle="color:#000099;"> javolution策略模式</span>

`   <span style="color:#000099;"> javolution策略模式</span>`


**[html]**[view plain](http://blog.csdn.net/luckykapok918/article/details/39500545#)[copy](http://blog.csdn.net/luckykapok918/article/details/39500545#)[print](http://blog.csdn.net/luckykapok918/article/details/39500545#)[?](http://blog.csdn.net/luckykapok918/article/details/39500545#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- <prename="code"class="html"><ManagerclassName="de.javakaffee.web.msm.MemcachedBackupSessionManager"
- memcachedNodes="n1:localhost:11211,n2:localhost:11212"
- sticky="false"
- sessionBackupAsync="false"
- lockingMode="uriPattern:/path1|/path2"
- requestUriIgnorePattern=".*\.(ico|png|gif|jpg|css|js)$"
- transcoderFactoryClass="de.javakaffee.web.msm.serializer.javolution.JavolutionTranscoderFactory"
- />


```
<pre name="code" class="html">	<Manager className="de.javakaffee.web.msm.MemcachedBackupSessionManager"
    memcachedNodes="n1:localhost:11211,n2:localhost:11212"
    sticky="false"
    sessionBackupAsync="false"
    lockingMode="uriPattern:/path1|/path2"
    requestUriIgnorePattern=".*\.(ico|png|gif|jpg|css|js)$"
    transcoderFactoryClass="de.javakaffee.web.msm.serializer.javolution.JavolutionTranscoderFactory"
    />
```

 d. tomcat6和tomcat7 lib文件夹下添加的MSM(memcached-session-manager) 相关jar：
kryo策略模式所需jar包 





**[html]**[view plain](http://blog.csdn.net/luckykapok918/article/details/39500545#)[copy](http://blog.csdn.net/luckykapok918/article/details/39500545#)[print](http://blog.csdn.net/luckykapok918/article/details/39500545#)[?](http://blog.csdn.net/luckykapok918/article/details/39500545#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- <imgsrc="https://img-blog.csdn.net/20140923223300359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast"alt=""/>


```
<img src="https://img-blog.csdn.net/20140923223300359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast" alt="" />
```



**[html]**[view plain](http://blog.csdn.net/luckykapok918/article/details/39500545#)[copy](http://blog.csdn.net/luckykapok918/article/details/39500545#)[print](http://blog.csdn.net/luckykapok918/article/details/39500545#)[?](http://blog.csdn.net/luckykapok918/article/details/39500545#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- <spanstyle="color:#000099;">javolution策略模式所需jar包</span>

`<span style="color:#000099;">javolution策略模式所需jar包</span>`
![](https://img-blog.csdn.net/20140923224005187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







1.4.测试web工程ssm，只需要一个index.jsp页面就可以了





**[html]**[view plain](http://blog.csdn.net/luckykapok918/article/details/39500545#)[copy](http://blog.csdn.net/luckykapok918/article/details/39500545#)[print](http://blog.csdn.net/luckykapok918/article/details/39500545#)[?](http://blog.csdn.net/luckykapok918/article/details/39500545#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- <%@ page contentType="text/html; charset=GBK" %>
- <%@ page import="java.util.*" %>
- <html><head><title>Cluster App Test</title></head>
- <body>
-    Server Info:   
- <%   
- out.println(request.getLocalAddr() + ":" + request.getLocalPort());  
- %>
- <br><br>
- <%    
- out.println("<br> ID " + session.getId()+"<br>");    
- String dataName = request.getParameter("dataName");  
- if (dataName != null && dataName.length() > 0) {    
- String dataValue = request.getParameter("dataValue") ;   
- session.setAttribute(dataName, dataValue);    
- }    
- out.print("<b>Session list</b><br>");    
- Enumeration e = session.getAttributeNames();    
- while (e.hasMoreElements()) {    
- String name = (String)e.nextElement();  
- String value = session.getAttribute(name).toString();    
- out.println( name + " = " + value+"<br>");   
- System.out.println( name + " = " + value);   
- }   
- %>
- 
- <formaction="index.jsp"method="POST">
-           name:<inputtype=textsize=20name="dataName"><br>
-           key:<inputtype=textsize=20name="dataValue"><br>
- <inputtype=submit>
- </form>
- </body>
- </html>


```
<%@ page contentType="text/html; charset=GBK" %> 
<%@ page import="java.util.*" %> 
<html><head><title>Cluster App Test</title></head> 
<body> 
   Server Info: 
   <% 
out.println(request.getLocalAddr() + ":" + request.getLocalPort());
%> 
<br><br>
    <%  
out.println("<br> ID " + session.getId()+"<br>");  
String dataName = request.getParameter("dataName");
if (dataName != null && dataName.length() > 0) {  
String dataValue = request.getParameter("dataValue") ; 
session.setAttribute(dataName, dataValue);  
}  
out.print("<b>Session list</b> <br>");  
Enumeration e = session.getAttributeNames();  
while (e.hasMoreElements()) {  
String name = (String)e.nextElement();
String value = session.getAttribute(name).toString();  
out.println( name + " = " + value+"<br>"); 
System.out.println( name + " = " + value); 
} 
%>
                
<form action="index.jsp" method="POST"> 
          name:<input type=text size=20 name="dataName"> <br> 
          key:<input type=text size=20 name="dataValue"> <br> 
          <input type=submit> 
    </form> 
  </body> 
</html>
```




所有软件安装配置好后，先启动nginx，memcached，最好分别启动tomcat6，tomcat7，都没报错，则配置成功。

2.tomcat集群session共享验证测试

 2.1 ，验证tomcat6启动成功：

![](https://img-blog.csdn.net/20140923230346712?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

 2.2 ，验证tomcat7启动成功：

![](https://img-blog.csdn.net/20140923230455648?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


 2.3，验证nginx转发成功：

![](https://img-blog.csdn.net/20140923230926028?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20140923231032484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


  2.4，验证tomcat 
session共享成功：


kryo策略：

![](https://img-blog.csdn.net/20140923232233486?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



首先登陆的是tomcat6的服务器，现在关掉tomcat6服务器，然后再刷新页面, 如果ID的值相同且Session List中值也存在，说明Session是被共享的。

kryo策略：


![](https://img-blog.csdn.net/20140923232925171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)








javolution策略：


![](https://img-blog.csdn.net/20140924000856500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





 首先登陆的是tomcat7的服务器，现在关掉tomcat7服务器，然后再刷新页面,如果ID的值相同且Session List中值也存在，说明Session是被共享的。

javolution策略：


![](https://img-blog.csdn.net/20140924001203653?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVja3lrYXBvazkxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

﻿﻿



