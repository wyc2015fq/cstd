# 配置Tomcat使用https协议 - z69183787的专栏 - CSDN博客
2017年09月13日 16:22:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：914
[http://www.cnblogs.com/wanghaoyuhappy/p/5267702.html](http://www.cnblogs.com/wanghaoyuhappy/p/5267702.html)
一.  创建tomcat证书
这里使用JDK自带的keytool工具来生成证书：
1. 在jdk的安装目录\bin\keytool.exe下打开keytool.exe
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160311223812929-1202820817.png)
2. 在命令行中输入以下命令:
```
keytool -genkeypair -alias "tomcat" -keyalg "RSA" -keystore "g:\tomcat.keystore"
```
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160311232032257-547664440.png)
以上命令将生产一对非对称密钥和自我签名的证书g:\tomcat.keystore
注意：“名字与姓氏”应该是域名，输成了姓名，和真正运行的时候域名不符，会出问题
这里我输入的密码是123456,  域名是以tomcat为例,  省市以广东深圳为例
二. 配置tomcat服务器
 定位到tomcat服务器的安装目录, 找到conf下的server.xml文件
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160311225843554-970722103.png)
找到如下已经被注释的代码：
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160311230054272-1556652290.png)
```
1 <!--
2     <Connector port="8443" protocol="HTTP/1.1" SSLEnabled="true"
3                maxThreads="150" scheme="https" secure="true"
4                clientAuth="false" sslProtocol="TLS" />
5     -->
```
去掉注释，修改为：
```
1 <Connector port="8443" protocol="HTTP/1.1" SSLEnabled="true"  
2               maxThreads="150" scheme="https" secure="true"  
3               clientAuth="false" sslProtocol="TLS"   
4        keystoreFile="g:\tomcat.keystore"  
5        keystorePass="123456" />
```
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160311230535585-185952816.png)
在互联网上, http协议的默认端口是80, https的默认端口是443, 这里将端口改为了443
三. 启动tomcat服务器
在IE浏览器中输入: https://localhost
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160311233551991-1048623893.png)
选择继续浏览此网站
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160311233642757-1558077937.png)
搞定了!!!!
遇到的问题:
我在配置的过程中问题，当我修改了server.xml的配置后，启动tomcat报错
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 org.apache.catalina.core.StandardService initInternal
 2 严重: Failed to initialize connector [Connector[HTTP/1.1-443]]
 3 org.apache.catalina.LifecycleException: Failed to initialize component [Connector[HTTP/1.1-443]]
 4 at org.apache.catalina.util.LifecycleBase.init(LifecycleBase.java:106)
 5 at org.apache.catalina.core.StandardService.initInternal(StandardService.java:559)
 6 at org.apache.catalina.util.LifecycleBase.init(LifecycleBase.java:102)
 7 at org.apache.catalina.core.StandardServer.initInternal(StandardServer.java:814)
 8 at org.apache.catalina.util.LifecycleBase.init(LifecycleBase.java:102)
 9 at org.apache.catalina.startup.Catalina.load(Catalina.java:633)
10 at org.apache.catalina.startup.Catalina.load(Catalina.java:658)
11 at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
12 at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:57)
13 at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
14 at java.lang.reflect.Method.invoke(Method.java:606)
15 at org.apache.catalina.startup.Bootstrap.load(Bootstrap.java:281)
16 at org.apache.catalina.startup.Bootstrap.main(Bootstrap.java:455)
17 Caused by: org.apache.catalina.LifecycleException: Protocol handler initialization failed
18 at org.apache.catalina.connector.Connector.initInternal(Connector.java:983)
19 at org.apache.catalina.util.LifecycleBase.init(LifecycleBase.java:102)
20 ... 12 more
21 Caused by: java.lang.Exception: Connector attribute SSLCertificateFile must be defined when using SSL with APR
22 at org.apache.tomcat.util.net.AprEndpoint.bind(AprEndpoint.java:507)
23 at org.apache.tomcat.util.net.AbstractEndpoint.init(AbstractEndpoint.java:610)
24 at org.apache.coyote.AbstractProtocol.init(AbstractProtocol.java:429)
25 at org.apache.catalina.connector.Connector.initInternal(Connector.java:981)
26 ... 13 more
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
网上找了下，可以将protocol="HTTP/1.1"替换为protocol="org.apache.coyote.http11.Http11Protocol"，问题可以得以解决
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160311233956038-287731525.png)
四. 安装tomcat的证书在浏览器中
这里浏览器显示证书错误, 有红色警告
在浏览器中右击选择属性, 查看不到证书
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160311234753335-1949891765.png)
在这里我为了能导出证书, 在tomcat的根目录下新建了一个hello.html的页面
下面可以访问到
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160311235028960-1686722083.png)
将证书导出来
注意:这个地方有部分电脑的"复制文件"的按钮是不能点的, 导不出来
可以尝试装个虚拟机试一下
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160311235126835-1191467250.png)
下面就下一步就行
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160311235348929-474166615.png)
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160311235359007-1593805711.png)
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160311235414116-177487282.png)
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160311235427444-517141823.png)
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160311235436335-2094022908.png)
我将证书导出到了桌面上,
得到了一个cer的证书文件
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160311235603554-90786013.png)
下面打开浏览器的Internet选项  ----> 内容  ----> 证书
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160311235748444-831221867.png)
选择"受信任的根证书颁发机构"  ----> 导入证书
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160311235850007-70181176.png)
将生成的证书导入进来
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160312000051725-762746178.png)
其余的步骤直接下一步即可
![](http://images2015.cnblogs.com/blog/370507/201603/370507-20160312000213929-263762697.png)
重启浏览器,发现------没有红色警告了!!!
搞定!!!
