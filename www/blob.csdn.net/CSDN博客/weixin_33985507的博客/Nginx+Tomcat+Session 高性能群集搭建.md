# Nginx+Tomcat+Session 高性能群集搭建 - weixin_33985507的博客 - CSDN博客
2011年03月18日 20:08:22[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3

## 【Nginx+Tomcat+Session 高性能群集搭建】
随着IT行业的发展，linux服务器在企业中应用广泛，人们对linux上的应用服务要求也越来越高，早先的apache服务器、apache有优点也 有不足，apache渐渐不能满足人们的要求，目前nginx被大量采用做高性能web服务器，无论是个人网站还是大型门户网站都在使用nginx作为 web搭建web服务器的首选。
接下来我们亲自来搭建一个属于自己的高性能web服务器，并且带故障自动转移的群集、和负载均衡服务器。
一、Nginx+Tomcat+Session 高性能群集服务搭建
- 本文采用目前主流版本搭建，搭建环境及版本如下列表：  
- 系统版本：Centos 5.3 64位系统  
- Nginx版本为：nginx-1.0.5.tar.gz  
- Jdk版本为：1.6.0_18_64  
- Tomcat版本为：Apache Tomcat/6.0.30  
- 其他相似版本也可以！安装服务所需的包如下下载路径：本网站集成部分下载：  
- 
- jdk请到以下网站下载：请选择自己系统版本的jdk。  
- 
- https://cds.sun.com/is-bin/INTERSHOP.enfinity/WFS/CDS-CDS_Developer-Site/en_US/-/USD/ViewFilteredProducts-SingleVariationTypeFilter  
- 
- 其他版本请到官网下载 
二、首先安装Nginx：安装nginx之前需要安装pcre包和zlib以支持重写,正则以及网页压缩等等】
把所需的包下载到/usr/src下【根据自己的习惯，路径可以改变】
1、首先安装pcre：
- cd /usr/src &&tar xzf pcre-8.01.tar.gz &&cd pcre-8.01 && ./configure --prefix=/usr/local/pcre &&make &&make install
2、然后再安装nginx ：【给nginx安装jvmroute模块】
- cd /usr/src &&useradd www &&wget http://friendly.sinaapp.com//LinuxSoft/nginx-upstream-jvm-route-0.1.tar.gz && tar xzf nginx-upstream-jvm-route-0.1.tar.gz && tar xzf nginx-1.0.5.tar.gz &&cd nginx-1.0.5&& patch -p0 <../nginx_upstream_jvm_route/jvm_route.patch && ./configure --prefix=/usr/local/nginx --with-http_stub_status_module --with-openssl=/usr/ --with-pcre=/usr/src/pcre-8.01 --add-module=../nginx_upstream_jvm_route/  --user=www--group=www &&make &&make install  
- 【nginx注意* –with-pcre=/usr/src/pcre-8.01指向的是源码包解压的路径，而不是安装的路径，否则会报  
- 
- make[1]: *** [/usr/local/pcre/Makefile] Error 127 错误
Nginx 安装完毕！我们先不着急nginx配置文件的配置，先来修改tomcat配置文件：
三、Tomcat安装配置：
1、将所需的包下载至/usr/src 目录；首先安装jdk：
- chmod o+x jdk* && ./jdk* ;程序包会提示按回车键，我们根据提示安装即可，解压完毕，会在当前目录生成 jdk1.6.0_18 这个文件夹，  
- mkdir -p /usr/java && mv jdk1.6.0_18 /usr/java/下  
- 
- vi /etc/profile 最后面加入以下语句：  
- export JAVA_HOME=/usr/java/jdk1.6.0_18  
- export CLASSPATH=$CLASSPATH:$JAVA_HOME/lib:$JAVA_HOME/jre/lib  
- export PATH=$JAVA_HOME/bin:$JAVA_HOME/jre/bin:$PATH:$HOMR/bin  
- source /etc/profile //使环境变量马上生效  
- java –version //查看java版本，显示版本是1.6.0_18，并且是64位的证明安装成功！  
- 【 Java HotSpot(TM) 64-Bit Server VM (build 16.0-b10, mixed mode) 】  
- 
- cd /usr/src && tar xzf apache-tomcat-6.0.30.tar.gz  
- 解压完成执行：  
- mv apache-tomcat-6.0.30 /usr/local/tomcat_1 && cp /usr/local/tomcat_1 /usr/local/tomcat_2 -r  
- 【移动到/usr/local下并重命名为tomcat_1文件夹,并cp一个tomcat_2】  
- 部署两个Tomcat群集做负载均衡用
2、设置tomcat的java变量
- cd /usr/local/tomcat_1/bin/ && vi setenv.sh 创建一个setenv.sh脚本文件，添加如下语句即可  
- JAVA_HOME=/usr/java/jdk1.6.0_18  
- JAVA_JRE=/usr/java/jdk1.6.0_18/jre  
- 并且拷贝setenv.sh文件到tomcat_2/bin/下 ：  
- 
- cp -p /usr/local/tomcat_1/bin/setenv.sh /usr/local/tomcat_2/bin 下即可！  
- 保存退出  
- 检查两个tomcat下bin目录的*.sh 文件是否有可执行权限，如果没有则进入到相应的目录执行 chmod o+x *.sh 即可！  
- 
- 给两个tomcat分别创建不同的目录如下：mkdir -p /usr/webapps/{www_1,www_2}
3、设置Tomcat的 server.xml文件：
【以下是我的tomcat配置文件具体内容，本想已附件的形式发的，本博客目前还不支持附件功能，重要修改的地方供大家参考，如果把所有配置都粘贴在这里，显示有问题，所有用了两个web链接！这样一来部分解释就产生了影响。不明白的欢迎留言】
这个是完整的server.xml文件下载地址：
[http://chinaapp.sinaapp.com/download/server.xml.tgz](http://chinaapp.sinaapp.com/download/server.xml.tgz)
- 两个Tomcat配置的cluster里面的端口分别为4000、4001 ，必须设置成不同的端口；而且得分别设置两个jvmroute名称，一会在nginx中会用到！  
- 
-  配置Tomcat session会话复制：  
- 分别在两个tomcat的conf下web.xml文件里面加入如下一行即可:   
- 
- <distributable/>
-  这个是加入tomcat的session复制的，做tomcat集群必须需要这一步，否则用户的session就无法正常使用 <distributable/>直接加在</web-app>之前就可以了   
- 
- 配置完毕后，我们测试一下广播  
- java -cp tomcat-replication.jar MCaster 224.0.0.1 45564 Terminal1  
- java -cp tomcat-replication.jar MCaster 224.0.0.1 45564 Terminal2  
- 
- 如果不报错则能正常广播  
- tomcat-replication.jar 下载:http://cvs.apache.org/~fhanik/tomcat-replication.jar  
- 如果是两台机器的话，可以用tcpdump 抓包看看！ 
Tomcat配置完毕，启动两个tomcat，先启动tomcat1，tomcat_1启动日志如下：
- 信息: Initializing Coyote HTTP/1.1 on http-8080  
- 2011-3-18 19:56:21 org.apache.catalina.startup.Catalina load  
- 信息: Initialization processed in 1088 ms  
- 2011-3-18 19:56:21 org.apache.catalina.core.StandardService start  
- 信息: Starting service Catalina  
- 2011-3-18 19:56:21 org.apache.catalina.core.StandardEngine start  
- 信息: Starting Servlet Engine: Apache Tomcat/6.0.18  
- 2011-3-18 19:56:21 org.apache.catalina.ha.tcp.SimpleTcpCluster start  
- 信息: Cluster is about to start  
- 2011-3-18 19:56:21 org.apache.catalina.tribes.transport.ReceiverBase bind  
- 信息: Receiver Server Socket bound to:/192.168.2.79:4000  
- 2011-3-18 19:56:21 org.apache.catalina.tribes.membership.McastServiceImpl setupSocket  
- 信息: Setting cluster mcast soTimeout to 500  
- 2011-3-18 19:56:21 org.apache.catalina.tribes.membership.McastServiceImpl waitForMembers  
- 信息: Sleeping for 1000 milliseconds to establish cluster membership, start level:4  
- 2011-3-18 19:56:22 org.apache.catalina.tribes.membership.McastServiceImpl waitForMembers  
- 信息: Done sleeping, membership established, start level:4  
- 2011-3-18 19:56:22 org.apache.catalina.tribes.membership.McastServiceImpl waitForMembers  
- 信息: Sleeping for 1000 milliseconds to establish cluster membership, start level:8  
- 2011-3-18 19:56:23 org.apache.catalina.tribes.membership.McastServiceImpl waitForMembers  
- 信息: Done sleeping, membership established, start level:8  
- 2011-3-18 19:56:23 org.apache.catalina.ha.deploy.FarmWarDeployer start  
- 严重: FarmWarDeployer can only work as host cluster subelement!  
- 2011-3-18 19:56:23 org.apache.catalina.ha.session.DeltaManager start  
- 信息: Register manager to cluster element Engine with name Catalina  
- 2011-3-18 19:56:23 org.apache.catalina.ha.session.DeltaManager start  
- 信息: Starting clustering manager at  
- 2011-3-18 19:56:23 org.apache.catalina.ha.session.DeltaManager getAllClusterSessions  
- 信息: Manager [localhost#]: skipping state transfer. No members active in cluster group.  
- 2011-3-18 19:56:23 org.apache.catalina.ha.session.JvmRouteBinderValve start  
- 信息: JvmRouteBinderValve started  
- 2011-3-18 19:56:23 org.apache.coyote.http11.Http11Protocol start  
- 信息: Starting Coyote HTTP/1.1 on http-8080  
- 2011-3-18 19:56:24 org.apache.jk.common.ChannelSocket init  
- 信息: JK: ajp13 listening on /0.0.0.0:8411  
- 2011-3-18 19:56:24 org.apache.jk.server.JkMain start  
- 信息: Jk running ID=0time=0/42 config=null
- 2011-3-18 19:56:24 org.apache.catalina.startup.Catalina start  
- 信息: Server startup in 2676 ms 
Tomcat_2的日志如下：
- 000,{-64, -88, 2, 79},4000, alive=86901,id={78 42 25 -19 -102 3 64 10 -76 93 69 -43 45 76 95 -112 }, payload={}, command={}, domain={}, ]. This operation will timeout if no session state has been received within 60 seconds.  
- 2011-3-18 19:57:48 org.apache.catalina.tribes.group.interceptors.ThroughputInterceptor report  
- 信息: ThroughputInterceptor Report[  
- Tx Msg:1 messages  
- Sent:0.00 MB (total)  
- Sent:0.00 MB (application)  
- Time:0.01 seconds  
- Tx Speed:0.04 MB/sec (total)  
- TxSpeed:0.04 MB/sec (application)  
- Error Msg:0  
- Rx Msg:1 messages  
- Rx Speed:0.00 MB/sec (since 1st msg)  
- Received:0.00 MB]  
- 
- 2011-3-18 19:57:48 org.apache.catalina.ha.session.DeltaManager waitForSendAllSessions  
- 信息: Manager [localhost#]; session state send at 11-3-18 下午7:57 received in 125 ms.  
- 2011-3-18 19:57:48 org.apache.catalina.tribes.group.interceptors.ThroughputInterceptor report  
- 信息: ThroughputInterceptor Report[  
- Tx Msg:2 messages  
- Sent:0.00 MB (total)  
- Sent:0.00 MB (application)  
- Time:0.05 seconds  
- Tx Speed:0.02 MB/sec (total)  
- TxSpeed:0.02 MB/sec (application)  
- Error Msg:0  
- Rx Msg:2 messages  
- Rx Speed:0.00 MB/sec (since 1st msg)  
- Received:0.00 MB]  
- 
- 2011-3-18 19:57:49 org.apache.catalina.ha.session.JvmRouteBinderValve start  
- 信息: JvmRouteBinderValve started  
- 2011-3-18 19:57:49 org.apache.coyote.http11.Http11Protocol start  
- 信息: Starting Coyote HTTP/1.1 on http-8081  
- 2011-3-18 19:57:49 org.apache.jk.common.ChannelSocket init  
- 信息: JK: ajp13 listening on /0.0.0.0:8412  
- 2011-3-18 19:57:49 org.apache.jk.server.JkMain start  
- 信息: Jk running ID=0time=0/34 config=null
- 2011-3-18 19:57:49 org.apache.catalina.startup.Catalina start  
- 信息: Server startup in 2962 ms 
 果有报错或者请查看原因！
- 如  
- skipping state transfer. No members active in cluster group.”  
- mcastBindAddress 没有添加或添加得不对   
- 
- 【如上报错，如果启动其中一个报的错，而且已经修改了mcastBindAddress，此时还是有报错，我们可以不用管，在启动另一个 tomcat看看，第二个tomcat没有No members active in cluster group，则表示两个tomcat通信正常。】  
- 
- 四、最后配置Nginx：【贴出nginx.conf配置文件的内容，下载地址如下：】  
- http://friendly.sinaapp.com/LinuxSoft/nginx.conf.tgz   
- 
- 配置文件需要注意的地方：  
- http  
- {  
- upstream backend {  
- server 192.168.2.79:8080 srun_id=tomcat1;  
- #【tomcat1是我tomcat_1 server.xml里面配置的jvmroute=tomcat1】  
- server 192.168.2.79:8081 srun_id=tomcat2;  
- #【tomcat2是我tomcat_2 server.xml里面配置的jvmroute=tomcat2】  
- jvm_route $cookie_JSESSIONID|sessionid reverse;  
- #【配置jvmroute 、session会话】  
- }  
- 配置完毕后，新建/usr/webapps/www_1/index.jsp测试页面  
- <%@ page language="java"import="java.util.*"pageEncoding="UTF-8"%>
- <%  
- %>
- http://blog.mgcrazy.com  
- 
- <%out.print(request.getSession()) ;%>
- 
- <%out.println(request.getHeader("Cookie")); %>
- 
- tomcat_2的发布目录index.jsp内容为：  
- 
- <%@ page language="java"import="java.util.*"pageEncoding="UTF-8"%>
- <%  
- %>
- http://wgkgood.gicp.net  
- 
- <%out.print(request.getSession()) ;%>
- 
- <%out.println(request.getHeader("Cookie")); %>
- 
- 启动nginx即可！  
- 
- 最后测试！在同一个浏览器窗口下 ctrl+f5刷新，都是同一个tomcat内容，它不会跳转到另一tomcat  
- 
- http://192.168.2.79:81 结果如下：  
- http://wgkgood.gicp.net  
- org.apache.catalina.session.StandardSessionFacade@2d7aece8  
- JSESSIONID=FEB19E24719ED666D1C9C50A7A09A2BA.tomcat2  
- 
- 另一个浏览器窗口为：  
- http://blog.mgcrazy.com  
- org.apache.catalina.session.StandardSessionFacade@259e215b  
- rtime=0; ltime=1298014951125; cnzz_eid=85744900-1298010049-; Hm_lvt_0e74098a87be059a433e5a74f1b5fecf=1298014951312; Hm_lvt_b7c035fd79b2c378053e06c86493de76=1300439158373; JSESSIONID=B2574ABB491F50F37162479BD3FE761C.tomcat1 
无论你怎么刷新同一个窗口下都不会变！而且当你down掉一个tomcat后，会自动转移会话到另一个tomcat上！用户是感觉不到服务挂掉的！当两个服务正常的时候，可以负载均衡、当一个服务down掉时，可以自动剔除它，并复制它的会话！
