# nginx+tomcat+redis/Memcached实现集群 session共享 - 零度的博客专栏 - CSDN博客
2017年11月16日 17:48:40[零度anngle](https://me.csdn.net/zmx729618)阅读数：451
nginx安装：[http://blog.csdn.net/grhlove123/article/details/47834673](http://blog.csdn.net/grhlove123/article/details/47834673)
redis安装：[http://blog.csdn.net/grhlove123/article/details/47783471](http://blog.csdn.net/grhlove123/article/details/47783471)
准备两个tomcat，修改相应的端口
|名称|IP|端口|tomcat版本|JDK|
|----|----|----|----|----|
|tomcat1|10.10.49.23|8080|7.0.40|1.7.0_25|
|tomcat2|10.10.49.15|8081|7.0.40|1.7.0_25|
修改nginx.conf加上：
**[html]**[view
 plain](http://blog.csdn.net/grhlove123/article/details/48047735#)[copy](http://blog.csdn.net/grhlove123/article/details/48047735#)
- upstream backend {  
-     server 10.10.49.23:8080 max_fails=1fail_timeout=10s;  
-     server 10.10.49.15:8081 max_fails=1fail_timeout=10s;  
- }  
修改nginx.conf的location成
**[html]**[view
 plain](http://blog.csdn.net/grhlove123/article/details/48047735#)[copy](http://blog.csdn.net/grhlove123/article/details/48047735#)
- location / {  
-     root   html;  
-     index  index.html index.htm;  
-     proxy_pass http://backend;  
-  }  
启动nginx。
下载tomcat-redis-session-manager相应的jar包，主要有三个：
wget https://github.com/downloads/jcoleman/tomcat-redis-session-manager/tomcat-redis-session-manager-1.2-tomcat-7-java-7.jar
wget http://central.maven.org/maven2/redis/clients/jedis/2.5.2/jedis-2.5.2.jar
wget http://central.maven.org/maven2/org/apache/commons/commons-pool2/2.0/commons-pool2-2.0.jar
下载完成后拷贝到$TOMCAT_HOME/lib中
修改两tomcat的context.xml:
**[html]**[view
 plain](http://blog.csdn.net/grhlove123/article/details/48047735#)[copy](http://blog.csdn.net/grhlove123/article/details/48047735#)
- <Context>
- 
- <!-- Default set of monitored resources -->
- <WatchedResource>WEB-INF/web.xml</WatchedResource>
- 
- <!-- Uncomment this to disable session persistence across Tomcat restarts -->
- <!--
-     <Manager pathname="" />
-     -->
- 
-     <!-- Uncomment this to enable Comet connection tacking (provides events  
-          on session expiration as well as webapp lifecycle) -->
- <!--
-     <Valve className="org.apache.catalina.valves.CometConnectionManagerValve" />
-     -->
- 
- <ValveclassName="com.orangefunction.tomcat.redissessions.RedisSessionHandlerValve"/>
- <ManagerclassName="com.orangefunction.tomcat.redissessions.RedisSessionManager"
- host="10.10.49.20"
- port="6379"
- database="0"
- maxInactiveInterval="60"/>
- </Context>
在tomcat/webapps/test放一个index.jsp
**[html]**[view
 plain](http://blog.csdn.net/grhlove123/article/details/48047735#)[copy](http://blog.csdn.net/grhlove123/article/details/48047735#)
- <%@ page language="java" %>
- <html>
- <head><title>TomcatA</title></head>
- <body>
- 
- <tablealign="centre"border="1">
- <tr>
- <td>Session ID</td>
- <td><%= session.getId() %></td>
- </tr>
- <tr>
- <td>Created on</td>
- <td><%= session.getCreationTime() %></td>
- </tr>
- </table>
- </body>
- </html>
- sessionID:<%=session.getId()%>
- <br>
- SessionIP:<%=request.getServerName()%>
- <br>
- SessionPort:<%=request.getServerPort()%>
- <%   
- //为了区分，第二个可以是222  
- out.println("This is Tomcat Server 1111");   
- %>
 通过访问http://10.10.49.20/test/
![](https://img-blog.csdn.net/20150828103648215)
刷新：
![](https://img-blog.csdn.net/20150828103659291)
可以看到虽然Server从1111变为2222，但session的创建时间没有变化，这就完成了session共享。
二、[Nginx+Tomcat+Memcached集群Session共享](http://blog.csdn.net/shimiso/article/details/8979044)
cookie是怎样工作的？ 
例如，我们创建了一个名字为login的Cookie来包含访问者的信息，创建Cookie时，服务器端的Header如下面所示，这里假设访问者的注册名是“Michael Jordan”，同时还对所创建的Cookie的属性如path、domain、expires等进行了指定。 
Set-Cookie:login=Michael Jordan;path=/;domain=msn.com; 
expires=Monday,01-Mar-99 00:00:01 GMT 
上面这个Header会自动在浏览器端计算机的Cookie文件中添加一条记录。浏览器将变量名为“login”的Cookie赋值为“Michael Jordon”。注意，在实际传递过程中这个Cookie的值是经过了URLEncode方法的URL编码操作的。 
这个含有Cookie值的HTTP Header被保存到浏览器的Cookie文件后，Header就通知浏览器将Cookie通过请求以忽略路径的方式返回到服务器，完成浏览器的认证操作。 
此外，我们使用了Cookie的一些属性来限定该Cookie的使用。例如Domain属性能够在浏览器端对Cookie发送进行限定，具体到上面的例子，该Cookie只能传到指定的服务器上，而决不会跑到其他的如www.hp.com的Web站点上去。Expires属性则指定了该Cookie保存的时间期限，例如上面的Cookie在浏览器上只保存到1999年3月1日1秒。当然，如果浏览器上Cookie太多，超过了系统所允许的范围，浏览器将自动对它进行删除。至于属性Path，用来指定Cookie将被发送到服务器的哪一个目录路径下。 
说明：浏览器创建了一个Cookie后，对于每一个针对该网站的请求，都会在Header中带着这个Cookie；不过，对于其他网站的请求Cookie是绝对不会跟着发送的。而且浏览器会这样一直发送，直到Cookie过期为止。
session又是如何工作的？ 
    由于http是无状态的协议，你访问了页面A，然后在访问B，http无法确定这2个访问来自一个人，因此要用cookie或session来跟踪用户，根据授权和用户身份来显示不同的页面。比如用户A登陆了，那么能看到自己的个人信息，而B没登陆，无法看到个人信息。还有A可能在购物，把商品放入购物车，此时B也有这个过程，你无法确定A，B的身份和购物信息，所以需要一个session ID来维持这个过程。
   cookie是服务器发给客户端，并且保持在客户端的一个文件，里面包含了用户的访问信息（账户密码等），可以手动删除或设置有效期，在下次访问的时候，会返给服务器。注意：cookie可以被禁用，所以要想其他办法，这就是session。比如：你去商场购物，商场会给你办一张会员卡，下次你来出示该卡，会有打折优惠。该卡可以自己保存（cookie），或是商场代为保管，由于会员太多，个人需要保存卡号信息（session ID)。
为什么要持久化SESSION？
在客户端每个用户的Session对象存在Servlet容器中，如果Tomcat服务器重起/当机的话该session就会丢失，而客户端的操作应为session的丢失而造成数据丢失，而且当前用户访问量巨大，每个用户的Session里存放大量的数据的话，那么就很占用服务器大量的内存，从而是服务器的性能受到影响。
有关集群几种session同步方案请参考 [http://blog.csdn.net/shimiso/article/details/8978922](http://blog.csdn.net/shimiso/article/details/8978922)
有关Nginx 介绍和使用请参考 [http://blog.csdn.net/shimiso/article/details/8690897](http://blog.csdn.net/shimiso/article/details/8690897)
Nginx+Tomcat+Memcached实现tomcat集群和session共享
一、Nginx部分
1.1、Nginx介绍
　　Nginx是俄罗斯人编写的十分轻量级的HTTP服务器,Nginx，它的发音为“engine X”， 是一个高性能的HTTP和反向代理服务器，同时也是一个IMAP/POP3/SMTP 代理服务器。
　　Nginx是由俄罗斯人 Igor Sysoev为俄罗斯访问量第二的 Rambler.ru站点开发的，它已经在该站点运行超过两年半了。Igor Sysoev在建立的项目时,使用基于BSD许可。
　　据说他当初是F5的成员之一，英文主页：http://nginx.net 。
1.2、Nginx优点
　　Nginx 是一个很牛的高性能Web和反向代理服务器, 它具有有很多非常优越的特性: 
　　在高连接并发的情况下，Nginx是Apache服务器不错的替代品: Nginx在美国是做虚拟主机生意的老板们经常选择的软件平台之一. 能够支持高达 50,000 个并发连接数的响应, 感谢Nginx为我们选择了 epoll and kqueue作为开发模型. 
　　Nginx作为负载均衡服务器: Nginx 既可以在内部直接支持 Rails 和 PHP 程序对外进行服务, 也可以支持作为 HTTP代理服务器对外进行服务. Nginx采用C进行编写, 不论是系统资源开销还是CPU使用效率都比 Perlbal 要好很多. 
　　作为邮件代理服务器: Nginx 同时也是一个非常优秀的邮件代理服务器（最早开发这个产品的目的之一也是作为邮件代理服务器）, Last.fm 描述了成功并且美妙的使用经验. 
　　Nginx 是一个 安装非常的简单 , 配置文件 非常简洁（还能够支持perl语法）, Bugs非常少的服务器: Nginx 启动特别容易, 并且几乎可以做到7*24不间断运行，即使运行数个月也不需要重新启动. 你还能够 不间断服务的情况下进行软件版本的升级.
1.3、Nginx安装
　　到官网http://nginx.org/en/download.html下载最新的稳定版本，目前是0.7.67。
**[html]**[view
 plain](http://blog.csdn.net/shimiso/article/details/8979044#)[copy](http://blog.csdn.net/shimiso/article/details/8979044#)
- [root@localhost ~]# wget http://nginx.org/download/nginx-0.7.67.tar.gz            
- --2010-09-24 14:48:12--  http://nginx.org/download/nginx-0.7.67.tar.gz            
- Resolving nginx.org... 81.19.68.137                                               
- Connecting to nginx.org|81.19.68.137|:80... connected.                            
- HTTP request sent, awaiting response... 200 OK                                    
- Length: 608462 (594K) [application/octet-stream]                                  
- Saving to: `nginx-0.7.67.tar.gz'                                                  
- 100%[<========================================>] 608,462     44.5K/s   in 18s       
- 2010-09-24 14:48:32 (32.8 KB/s) - `nginx-0.7.67.tar.gz' saved [608462/608462]     
- 
- [root@localhost ~]# tar -zxvf nginx-0.7.67.tar.gz    /解压压缩文件                
- [root@localhost ~]# cd nginx-0.7.67                  /进入安装文件目录            
- [root@localhost nginx-0.7.67]# ./configure –prefix=/usr/local/nginx      /指定安装到/usr/local/nginx目录下，可用./configure –help查看需要哪些编译参数         
- [root@localhost nginx-0.7.67]#make                  /make                         
- [root@localhost nginx-0.7.67]#make install          /安装                         
- [root@localhost nginx-0.7.67]# ll /usr/local/nginx/      /查看是否安装成功        
- drwxr-xr-x 2 root root 4096 Sep 24 15:12 conf                                     
- drwxr-xr-x 2 root root 4096 Sep 24 15:12 html                                     
- drwxr-xr-x 2 root root 4096 Sep 24 15:12 logs                                     
- drwxr-xr-x 2 root root 4096 Sep 24 15:12 sbin                                     
- [root@localhost nginx-0.7.67]#/usr/local/nginx/sbin/nginx –t     /测试Nginx配置文件是否正确                                                                    
- [root@localhost nginx-0.7.67]#/usr/local/nginx/sbin/nginx         /启动Nginx     
下面验证Nginx是否正常启动，浏览器中输入nginx服务器地址，出现如下图说明nginx正常工作。
![](https://img-blog.csdn.net/20130527112239861)
二、利用memcached配置Nginx、Tomcat实现负载均衡和session共享
Tomcat、memcached安装这里就不讲了，主要介绍如何配置Nginx实现Tomcat的负载均衡和利用memcached实现session共享。
配置Nginx+Tomcat集群实现负载均衡
这里主要修改nginx的配置文件nginx.conf
**[html]**[view
 plain](http://blog.csdn.net/shimiso/article/details/8979044#)[copy](http://blog.csdn.net/shimiso/article/details/8979044#)
- [root@localhost ~]# vi /usr/local/nginx/conf/nginx.conf       /编辑nginx配置文件，内容如下  
- user  nobody;  
- worker_processes  4;  
- error_log  logs/error.log;  
- events {  
-     worker_connections  1024;  
- }  
- 
- 
- http {  
-     include       mime.types;  
-     default_type  application/octet-stream;  
-     sendfile        on;  
-     keepalive_timeout  65;  
-     gzip  on;  
-     upstream  www.huiyi99.com   {  
-               server   192.168.1.11:8080;  
-               server   192.168.1.101:8080;  
-     }  
-     server {  
-         listen       80;  
-         server_name  www.huiyi99.com;  
-         charset utf-8;  
-         location / {  
-             root   html;  
-             index  index.html index.htm;  
-             proxy_pass        http://www.huiyi99.com;  
-             proxy_set_header  X-Real-IP  $remote_addr;  
-             client_max_body_size  100m;  
-         }  
- 
- 
-         location ~ ^/(WEB-INF)/ {   
-             deny all;   
-         }   
- 
- 
-         error_page   500 502 503 504  /50x.html;  
- location = /50x.html {  
-             root   html;  
-         }  
- 
- 
-     }  
- }  
注意：红色字体部分是需要修改的地方。
www.huiyi99.com的域名更换为自己需要的域名。
server   192.168.1.11:8080
server   192.168.1.10:8080     
更换为后端Tomcat的ip的ip和端口，如果有多个按照该格式添加即可。
1.2、Tomcat配置
　　使用memcached-session-manager这个开源项目（http://code.google.com/p/memcached-session-manager，下面简称msm）配置Tomcat和memcached实现session共享。
首先将下面的包下载到Tomcat的lib目录下，这些包都是msm所依赖的包。
**[html]**[view
 plain](http://blog.csdn.net/shimiso/article/details/8979044#)[copy](http://blog.csdn.net/shimiso/article/details/8979044#)
- [root@localhost ~]# cd /usr/local/tomcat/lib/  
- [root@localhost lib]# wget http://memcached-session-manager.googlecode.com/files/memcached-session-manager-1.3.0.jar   
- [root@localhost lib]# wget http://memcached-session-manager.googlecode.com/files/msm-javolution-serializer-jodatime-1.3.0.jar   
- [root@localhost lib]# wget http://memcached-session-manager.googlecode.com/files/msm-javolution-serializer-cglib-1.3.0.jar   
- [root@localhost lib]# wget http://spymemcached.googlecode.com/files/memcached-2.4.2.jar   
- [root@localhost lib]# wget http://memcached-session-manager.googlecode.com/files/javolution-5.4.3.1.jar  
　　下面修改Tomcat配置文件，让Tomcat结合memcached实现session共享
[root@localhost lib]# vi /usr/local/tomcat/conf/server.xml
　　在配置文件中加入如下内容
**[html]**[view
 plain](http://blog.csdn.net/shimiso/article/details/8979044#)[copy](http://blog.csdn.net/shimiso/article/details/8979044#)
- <ContextdocBase="/var/www/html"path=""reloadable="true">
- <ManagerclassName="de.javakaffee.web.msm.MemcachedBackupSessionManager"
- memcachedNodes="n1:localhost:11211"
- requestUriIgnorePattern=".*\.(png|gif|jpg|css|js)$"
- sessionBackupAsync="false"
- sessionBackupTimeout="100"
- transcoderFactoryClass="de.javakaffee.web.msm.serializer.javolution.JavolutionTranscoderFactory"
- copyCollectionsForSerialization="false"/>
- </Context>
说明：
这里的memcachedNodes是填写memcached节点,多个节点时可以以空隔分开，如: 
 n1:localhost:11211 n2:localhost:11212     /localhost改为安装memcached的服务器的IP
sessionBackupTimeout的单位为分钟
/var/www/html改为Tomcat服务器web根目录的路径
修改后重启两个TOMCAT和nginx即可,这个时候已经解决SESSION的共享问题.
测试Nginx和Tomcat
　　测试Nginx是否实现负载均衡已经Tomcat能否共享session信息，分别在两个Tomcat服务器的/var/www/html目录下新建两个test.jsp页面
Tomcat1
**[html]**[view
 plain](http://blog.csdn.net/shimiso/article/details/8979044#)[copy](http://blog.csdn.net/shimiso/article/details/8979044#)
- [root@localhost lib]# vi /var/www/html/test.jsp     /输入如下内容  
- SessionID:<%=session.getId()%>
- <BR>
- SessionIP:<%=request.getServerName()%>
- <BR>
- SessionPort:<%=request.getServerPort()%>
- <%  
- out.println("This is Tomcat Server 111111！");  
- %>
Tomcat2
**[html]**[view
 plain](http://blog.csdn.net/shimiso/article/details/8979044#)[copy](http://blog.csdn.net/shimiso/article/details/8979044#)
- [root@localhost lib]# vi /var/www/html/test.jsp     /输入如下内容  
- SessionID:<%=session.getId()%>
- <BR>
- SessionIP:<%=request.getServerName()%>
- <BR>
- SessionPort:<%=request.getServerPort()%>
- <%  
- out.println("This is Tomcat Server 222222！");  
- %>
通过浏览器访问Nginx服务器，如下图将访问地址改成Nginx所在服务器。
反复刷新浏览器，如果SessionID一直不变，下面的SessionPort的内容在不断变化则说明配置成功。
