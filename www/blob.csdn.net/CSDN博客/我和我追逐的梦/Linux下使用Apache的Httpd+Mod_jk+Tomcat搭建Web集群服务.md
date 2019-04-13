
# Linux下使用Apache的Httpd+Mod_jk+Tomcat搭建Web集群服务 - 我和我追逐的梦~~~ - CSDN博客


置顶2016年05月08日 01:22:54[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：12490标签：[apache																](https://so.csdn.net/so/search/s.do?q=apache&t=blog)[tomcat																](https://so.csdn.net/so/search/s.do?q=tomcat&t=blog)[集群																](https://so.csdn.net/so/search/s.do?q=集群&t=blog)[linux																](https://so.csdn.net/so/search/s.do?q=linux&t=blog)[web																](https://so.csdn.net/so/search/s.do?q=web&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=linux&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=集群&t=blog)个人分类：[web服务器																](https://blog.csdn.net/heyongluoyao8/article/category/6218683)
[
																								](https://so.csdn.net/so/search/s.do?q=集群&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=tomcat&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=tomcat&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=apache&t=blog)

### Linux下使用Apache的Httpd+Mod_jk+Tomcat搭建Web集群服务
#### 目的
使用多个tomcat服务器来对请求进行分流，防止单个服务器压力过重。这里为了简单，只使用两个tomcat。
#### 软件
apache httpd-2.2.31(下载地址：[https://httpd.apache.org/download.cgi](https://httpd.apache.org/download.cgi))
apache tomcat-7.0.69(下载地址：[https://tomcat.apache.org/download-70.cgi](https://tomcat.apache.org/download-70.cgi))
tomcat-connectors-1.2.41(即mod_jk，下载地址：[https://tomcat.apache.org/download-connectors.cgi](https://tomcat.apache.org/download-connectors.cgi))
软件都在/home/hey/apache-src 下
单机版(垂直集群)
即apache与几个tomcat都部署在一个机器上，实际上分布式集群应该是apache部署在主节点上，多个tomcat分别在不同的机器上. 如果tomcat都在一个机器上，那么使用不同的端口。
建立一个apache目录
cd /home/hey
mkdir apache
安装httdp
解压httpd并进入目录
tar -zxvf httpd-2.2.31.tar.gz
mv httpd-2.2.31 httpd
cd httpd
进行编译参数配置
./configure –prefix=/home/hey/apache/apache –enable-mods-shared=all
编译安装
make
make install
如果过程中报Cannot use an external APR with the bundled APR-util，那么还需要安装apr和apr-util，如果httpd是2.4+版本，那么还需要先下载apr和apr-util，详细参见：[http://blog.csdn.net/bluishglc/article/details/6867358](http://blog.csdn.net/bluishglc/article/details/6867358)。
安装结束，测试启动
启动
cd /home/hey/apache/apache/bin/apachectl start
停止
cd /home/hey/apache/apache/bin/apachectl stop
启动后，你在机器上输入这台机器的ip，显示It works 则表明配置成功.
为了直接使用apacheclt命令，配置~/.bash_profile
vi ~/.bash_profile
HTTPD_HOME=/home/hey/apache/apache
PATH=PATH:HTTPD_HOME/bin
export HTTPD_HOME
安装tomcat-connectors-1.2.41
解压源文件
tar -zxvf tomcat-connectors-1.2.41-src.tar.gz
进入native目录
cd tomcat-connectors-1.2.41-src/native
进行编译参数配置
./buildconf.sh
./configure –with-apx=/home/hey/apache/apache/bin/apxs \
–with-java-home=$JAVA_HOME –with-java-platform=2\
–enable-jni
编译安装
make
make install
是否成功
如果成功，则在httpd安装目录下/home/hey/apache/apache/modules可以看到mod_jk.so文件
安装读个tomcat(已两个为例)
解压tomcat
tar -zxvf apache-tomcat-7.0.69.tar.gz
mv apache-tomcat-7.0.69 tomcat1
cp -r tomcat1 ../apache/tomcat1
cp -r ../apache/tomcat1 ../apache/tomcat2
集群配置
配置tomcat
修改端口
tomcat的配置主要是对tomcat/conf/server.xml文件的配置.
因为，两个tomcat都部署在一台机器上，即一台机器上运行两个tomcat的实例，因此，要配置不同的端口，即要对server.xml中配置的所有端口进行修改，避免端口被占用。这里对所有的端口在默认端口上加上1000。
如下：
修改tomcat1的server.xml的SHUTDOWN端口8005为9005，tomcat2的8005为10005，如下：
<Serverport="8005"shutdown="SHUTDOWN">改为：
<Serverport="9005"shutdown="SHUTDOWN">同理，tomcat2的改为10005。
分别修改tomcat1与tomcat2的HTTP Connector端口，从默认的8080分别改为9080与10080，如下：
<Connectorport="8080"protocol="HTTP/1.1"connectionTimeout="20000"redirectPort="8443"/>改为：
<Connectorport="9080"protocol="HTTP/1.1"connectionTimeout="20000"redirectPort="8443"/>同理，tomcat2的改为10080。
分别修改tomcat1与tomcat2的AJP Connector端口，从默认的8009分别改为9009与10009，如下：
<Connectorport="8009"protocol="AJP/1.3"redirectPort="8443"/>改为：
<Connectorport="9009"protocol="AJP/1.3"redirectPort="8443"/>同理，tomcat2的改为10009。
3. tomcat集群配置
需要再在server.xml文件改动两处：
为<Engine>元素设置jvmRoute属性 引用[1]:
在server.xml文件中可以找到注释掉的带jvmRoute的<Engine>配置，如下图：
![这里写图片描述](https://img-blog.csdn.net/20160508005848634)
参考这个配置就可以了，在为被注释掉的<Engine>加上jvmRoute属性，如下：
<Enginename="Catalina"defaultHost="localhost"jvmRoute="tomcat1">tomcat2的jvmRoute设置为tomcat2.
对于jvmRoute的取值要特别注意：其值必须于后面要提到的mod_jk的workers.properties文件中结点名相一致！由于那个文件中结点名为tomcat1和tomcat2,因此，此处我们必须设定jvmRoute的值为这两个值之一。关于这一点在tomcat官方关于Apache Tomcat Connector的文档[http://tomcat.apache.org/connectors-doc/webserver_howto/apache.html](http://tomcat.apache.org/connectors-doc/webserver_howto/apache.html)也有明确提及。jvmRoute值会出现在由该结点创建的session id中，例如：在非集群环境下，一个sessionid可能是 “xxxxxxxxx” 的格式，而在集群环境下，如果当前结点的jvmRtomat1oute被配置为tomcat1,那由该结点生成的sessionid将变成“xxxxxxxxx.tomat1”格式，而mod_jk正是依赖于这个节点后缀实现sticky session的，也就是把所有后缀是tomat1的请求都发送给tomat1结点进行处理。
注意：个人在实验中发现有问题，如果tomcat配置了jvmRoute，并且httpd的conf下的workers.properties(下文会讲到)的worker.controller.sticky_session=1的话，会发现多次请求后，都是固定的一台tomcat(或者tomcat1或者tomcat2)在处理，刷新浏览器窗口总是在某一个tomcat控制台输出形如(上面的请求都是一台机器上的浏览器请求，如果ip不变，那么可以认为是同一个客户的请求)：
要么一直是：
SessionID:154678FA6D4D0ABD57658B750E7A3532.tomcat1  (在tomcat1窗口)
要么一直是：
SessionID:3800571A532AECEA7280F45361861AD4.tomcat2  (在tomcat2窗口)
由控制台打印的结果可以看出，SessionID在哪个tomcat上产生，那么后续该客户端的请求将总是会这个tomcat来处理。
其实worker.controller.sticky_session=1表示某一 client的session创建之后，后续由该客户端发起的请求，也就是这个session的所有请求都始终由第一次处理该请求的结点负责处理（除非该结点挂掉），0则相反。
(1) 如果你需要设置相同的客户端一直在同一个tomcat服务器上（除非这台挂掉乐)，那么需要设置tomcat的jvmRoute与worker.controller.sticky_session=1。
(2) 如果你需要设置相同的客户端可以随机分配到不同的tomcat服务器上，那么需要设置tomcat的jvmRoute与worker.controller.sticky_session=0，或者不设置tomcat的jvmRoute。
具体参见后面的截图。
在<Engine>元素中添加机器配置信息
在tomcat官网中的集群配置文档：[http://tomcat.apache.org/tomcat-6.0-doc/cluster-howto.html](http://tomcat.apache.org/tomcat-6.0-doc/cluster-howto.html)中，有一份默认配置：
<ClusterclassName="org.apache.catalina.ha.tcp.SimpleTcpCluster"channelSendOptions="8"><ManagerclassName="org.apache.catalina.ha.session.DeltaManager"expireSessionsOnShutdown="false"notifyListenersOnReplication="true"/><ChannelclassName="org.apache.catalina.tribes.group.GroupChannel"><MembershipclassName="org.apache.catalina.tribes.membership.McastService"address="228.0.0.4"port="45564"frequency="500"dropTime="3000"/><ReceiverclassName="org.apache.catalina.tribes.transport.nio.NioReceiver"address="auto"port="4000"autoBind="100"selectorTimeout="5000"maxThreads="6"/><SenderclassName="org.apache.catalina.tribes.transport.ReplicationTransmitter"><TransportclassName="org.apache.catalina.tribes.transport.nio.PooledParallelSender"/></Sender><InterceptorclassName="org.apache.catalina.tribes.group.interceptors.TcpFailureDetector"/><InterceptorclassName="org.apache.catalina.tribes.group.interceptors.MessageDispatch15Interceptor"/></Channel><ValveclassName="org.apache.catalina.ha.tcp.ReplicationValve"filter=""/><ValveclassName="org.apache.catalina.ha.session.JvmRouteBinderValve"/><DeployerclassName="org.apache.catalina.ha.deploy.FarmWarDeployer"tempDir="/tmp/war-temp/"deployDir="/tmp/war-deploy/"watchDir="/tmp/war-listen/"watchEnabled="false"/><ClusterListenerclassName="org.apache.catalina.ha.session.JvmRouteSessionIDBinderListener"/><ClusterListenerclassName="org.apache.catalina.ha.session.ClusterSessionListener"/></Cluster>这个默认配置一般可满足要求，需要直接将上面的配置复制到<Engine>元素中便可，这有tomcat便配置完毕。
4. 配置Apache httpd的配置
mod_jk配置项
编辑在apache httpd安装目录的conf目录下(此处是/home/hey/apache/apache/conf)的httpd.conf文件，在文件最后追加以下内容：
\# Load mod_jk moduleLoadModule jk_module modules/mod_jk.so\# Specify jk log file.JkLogFile /var/log/mod_jk.log\# Specify jk log level [debug/error/info]JkLogLevel info\# Specify workers.properties, this file tell jk:\# how many nodes and where they are.JkWorkersFile conf/workers.properties\# Specify which requests should handled by which node.JkMount/* controller关于mod_jk配置项的详细内容，可参考：[http://tomcat.apache.org/connectors-doc/webserver_howto/apache.html](http://tomcat.apache.org/connectors-doc/webserver_howto/apache.html)
上述配置中：JkWorkersFile conf/workers.properties 指明由一个workers.properties文件来描述集群结点的情况，因此，我们需要创建这个workers.properties文件，并放置于conf文件夹下。
配置workers.properties
在conf目录下创建该文件，并且写入以下内容：
\#所有节点列表，其中controller是一个逻辑结点，负责负载均衡控制，\#如果JkMount中的URL指定给了controller就表示这个请求会被自动散列到某个物理节点上。\#注意：真正负责处理请求的tomcat的名称（这里就是tomcat1,tomcat2)必须于它们在conf/server.xml\#文件中配置的jvmRout的属性值是一致的！worker.list= controller,tomcat1,tomcat2\#========tomcat1========worker.tomcat1.port=9009\#ajp13 端口号，在tomcat1下server.xml配置,默认8009,必须要与修改后的一致worker.tomcat1.host=localhost\#tomcat1的主机地址，如不为本机，请填写ip地址worker.tomcat1.type=ajp13
worker.tomcat1.lbfactor=1\#tomcat1 server的加权比重，值越高，分得的请求越多\#========tomcat2========worker.tomcat2.port=10009\#ajp13 端口号，在tomcat2下server.xml配置,默认8009,必须要与修改后的一致worker.tomcat2.host=localhost\#tomcat2的主机地址，如不为本机，请填写ip地址worker.tomcat2.type=ajp13
worker.tomcat2.lbfactor=1\#tomcat2 server的加权比重，值越高，分得的请求越多\#========controller,负载均衡控制器========worker.controller.type=lb
worker.controller.balance_workers=tomcat1,tomcat2\#指定分担请求的tomcat，旧版本中的balanced_workers,已不再推荐使用！worker.controller.sticky_session=1\#sticky_session为1表示，\#当某一 client的session创建之后，后续由该客户端发起的请求，也就是这个session的所有请求都始终由第一次处理该请求的结点\#负责处理（除非该结点挂掉）到此，所有配置均已完成。
5. 测试
创建web应用程序
使用Eclipse创建Maven webapp程序ClusterTest，在WEB-INF的web.xml中加上<distributable/>元素，要放在servlet元素前面(不然不报错），然后打成war包分别拷贝到tomcat1与tomcat2的webapps下。
也就是在需要集群的应用的web.xml中加上属性distributable，表明该应用可多应用分流处理，能进行Session的复制
多刷新几次浏览器窗口，你将会看到在两个Tomcat窗口都打印出相同的SessionID及其中的值，并且每次刷新后打印的结果都一样的。
如果不为应用的web.xml加上 ，同样测试index.jsp页面，每次刷新分流到不同的tomcat上都会产生不一样的SessionID,在同一个tomcat上也是间隔出现不同的sessionID。
即如果不加上<distributable/>元素，那么同一个客户的多次访问sessionID是不会复制的，也就是相同客户会一直改变。
启动两个tomcat和apache httpd
(1) 启动apache httpd
cd ~/apache/apache/bin
sudo ./apachectl start
(2) 启动tomcat1
cd ~/apache/tomcat1/bin
chmod 755 *.sh
startup.sh
(3) 启动tomcat2
cd ~/apache/tomcat2/bin
chmod 755 *.sh
startup.sh
多次访问localhost/ClusterTest(远程使用ip/ClusterTest, httpd 默认是80端口)
验证
分别进入~/apache/tomcat1/logs与~/apache/tomcat2/logs，查看localhost_access_log.yyyy-MM-dd.txt，可以看出一些访问请求信息，说明请求被随机分配给了两个tomcat交替执行。
另外一种验证方法
在应用程序中的index.jsp中写入内容为：
<%@ page language="java"import="java.util.*"pageEncoding="UTF-8"%><%@ page import="java.text.SimpleDateFormat"%><!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"><html><head><title>Tomcat集群测试</title></head><body>服务器信息:<%Stringdtm =newSimpleDateFormat("yyyy-MM-dd hh:mm:ss").format(newDate());
      System.out.println("["+request.getLocalAddr()+":"+request.getLocalPort()+"]"+ dtm);
      out.println("<br>["+request.getLocalAddr()+":"+request.getLocalPort()+"]"+ dtm+"<br>"); 
    %>session分发:<%
        session.setAttribute("name","dennisit");
        System.out.println("[session分发] session id:"+session.getId());
        out.println("<br>[session分发] session id： "+ session.getId()+"<br>");
    %>绝对路径<%StringabsPath =newjava.io.File(application.getRealPath(request.getRequestURI())).getParent();
    System.out.println("Abs Path:"+absPath);
    out.println("Abs Path:"+absPath);
    %></body></html>那么多次在浏览器中输入http://机器的ip/ClusterTest/ 会打印出不同的绝对路径(tomcat1与tomcat2):
应用程序的web.xml没有配置distributable元素，那么：
![这里写图片描述](https://img-blog.csdn.net/20160508010136967)[ ](https://img-blog.csdn.net/20160508010136967)
![这里写图片描述](https://img-blog.csdn.net/20160508010151551)[ ](https://img-blog.csdn.net/20160508010151551)
在不同的tomcat上，sessionID会不一样，因为不会进行sessionID拷贝。上面的截图是tomcat没有Engine元素配置jvmRoute属性下的。
下面四种情况是应用程序的web.xml配置了distributable元素
tomcat的Engine元素配置了jvmRoute, worker.controller.sticky_session=1：
![这里写图片描述](https://img-blog.csdn.net/20160508143802381)[ ](https://img-blog.csdn.net/20160508143802381)
![这里写图片描述](https://img-blog.csdn.net/20160508143838003)[ ](https://img-blog.csdn.net/20160508143838003)
说明相同的客户一直在同一个tomcat上，并且sessionID一致。
tomcat的Engine元素配置了jvmRoute并且worker.controller.sticky_session=0:
![这里写图片描述](https://img-blog.csdn.net/20160508143949538)[ ](https://img-blog.csdn.net/20160508143949538)
![这里写图片描述](https://img-blog.csdn.net/20160508144006208)[ ](https://img-blog.csdn.net/20160508144006208)
说明相同的客户会交替在不同的tomcat上，并且sessionID一致。
tomcat的Engine元素没有配置jvmRoute, worker.controller.sticky_session=1：
![这里写图片描述](https://img-blog.csdn.net/20160508144202617)[ ](https://img-blog.csdn.net/20160508144202617)
![这里写图片描述](https://img-blog.csdn.net/20160508144223539)[ ](https://img-blog.csdn.net/20160508144223539)
说明相同的客户会交替在不同的tomcat上，并且sessionID一致，只是后面不会sessionID不会出现.tomcat1或者.tomcat2。
tomcat的Engine元素没有配置jvmRoute并且worker.controller.sticky_session=0:
![这里写图片描述](https://img-blog.csdn.net/20160508144449511)[ ](https://img-blog.csdn.net/20160508144449511)
![这里写图片描述](https://img-blog.csdn.net/20160508144505699)[ ](https://img-blog.csdn.net/20160508144505699)
说明相同的客户会交替在不同的tomcat上，并且sessionID一致，只是后面不会sessionID不会出现.tomcat1或者.tomcat2。
上述便说明了：
(1) 如果你需要设置相同的客户端一直在同一个tomcat服务器上（除非这台挂掉乐)，那么需要设置tomcat的jvmRoute与worker.controller.sticky_session=1。
(2) 如果你需要设置相同的客户端可以随机分配到不同的tomcat服务器上，那么需要设置tomcat的jvmRoute与worker.controller.sticky_session=0，或者不设置tomcat的jvmRoute。
因此根据不同的需要配置不同。
如果在不同的机器上访问，上述的四种配置都会进行分流，因为是不同的客户。
完全集群(水平集群)
tomcat分别部署在多台机器上，步骤与单机情况差不多。其中不同机器上的tomcat的server.xml各端口可以一样，不过在<Engine>元素的jvmRoute属性必须配置不一样的名字，如tomcat1、tomcat2 …, 并且在workers.properties文件中配置正确：
worker.controller.balance_workers=tomcat1,tomcat2…
以及在每个tomcat的属性配置正确，如：
worker.tomcat1.port=自己的ajp13端口号
worker.tomcat1.host=自己的ip
问题
在启动httpd时 ./apachectl start 报(13)Permission denied: make_sock: could not bind to address 0.0.0.0:80
no listening sockets available, shutting down
Unable to open logs
解决: 需要管理员权限 使用sudo 运行该命令
参考资料
[1][http://blog.csdn.net/bluishglc/article/details/6867358](http://blog.csdn.net/bluishglc/article/details/6867358)
[2] tomcat官方关于tomcat集群配置的文档：
[http://tomcat.apache.org/tomcat-6.0-doc/cluster-howto.html](http://tomcat.apache.org/tomcat-6.0-doc/cluster-howto.html)
[3] tomcat官方关于Apache Tomcat Connector的文档
[http://tomcat.apache.org/connectors-doc/webserver_howto/apache.html](http://tomcat.apache.org/connectors-doc/webserver_howto/apache.html)
[4] apache官方关于apache的安装文档
[http://httpd.apache.org/docs/2.2/en/install.html](http://httpd.apache.org/docs/2.2/en/install.html)
[5] apache官方关于configure参数的文档
[http://httpd.apache.org/docs/2.2/en/programs/configure.html\#installationdirectories](http://httpd.apache.org/docs/2.2/en/programs/configure.html#installationdirectories)

