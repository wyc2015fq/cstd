# Tomcat多实例单应用部署方案 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [fullstackyang](http://www.jobbole.com/members/2653yy) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
## 一、Tomcat部署的场景分析
通常，我们对tomcat部署需求可以分为几种：单实例单应用，单实例多应用，多实例单应用，多实例多应用。
对于第一种场景，如果不要求周期性地维护tomcat版本，一般的做法是把打好的war包丢到webapps目录下，然后执行startup.sh脚本，并且可以在浏览器里访问就行了。
对于第二种场景，是把多个应用程序的war包放在同一个tomcat的webapps目录，这样一来，关闭和启动tomcat会影响所有项目。
对于第三种场景，各个tomcat都运行同一个应用程序，对应地需要修改不同的监听端口，这种方式通常会和apache httpd或者nginx整合使用，做一些负载均衡的处理。
对于第四种场景相当于第一种场景的复数形式，除了修改不同的监听端口，没有本质区别。
一般来说，多实例部署tomcat，可以充分利用系统资源，不过这种方式，也有几个方面需要考虑：
- 多实例tomcat的更新维护，例如对tomcat进行升级等操作，我们需要考虑如何能“优雅”地对所有实例进行升级
- 尽量不要影响应用程序，在更新tomcat时，一不小心就把conf目录等全部覆盖，所以尽量要把配置文件和安装目录隔离
- 对于单应用来说，如果将war包分别置于各个tomcat的webapps目录，那么在发布新版本的war时，可能会出现某个实例更新失败，导致用户在访问时可能会访问到不同版本的web app，因此，比较好的方式就是所有tomcat实例都统一指向同一个应用程序
本文重点阐述多实例应用的部署方案，但是为了解决上述几个问题，我们需要先来了解一下tomcat的一些基本情况
## 二、基本要点
### 2.1 分离目录
首先来看一下tomcat的目录结构，一个刚解压出来的tomcat打包文件应该有以下几个目录
|bin|主要存放脚本文件，例如比较常用的windows和linux系统中启动和关闭脚本|
|----|----|
|conf|主要存放配置文件，其中最重要的两个配置文件是server.xml和web.xml|
|lib|主要存放tomcat运行所依赖的包|
|logs|主要存放运行时产生的日志文件，例如catalina.{date}.log等|
|temp|存放tomcat运行时产生的临时文件，例如开启了hibernate缓存的应用程序，会在该目录下生成一些文件|
|webapps|部署web应用程序的默认目录|
|work|主要存放由JSP文件生成的servlet（java文件以及最终编译生成的class文件）|
再介绍两个tomcat中比较重要的概念（通常也是两个系统变量）——CATALINA_HOME和CATALINA_BASE：
- CATALINA_HOME：即指向Tomcat安装路径的系统变量
- CATALINA_BASE：即指向活跃配置路径的系统变量
通过设置这两个变量，就可以将tomcat的安装目录和工作目录分离，从而实现tomcat多实例的部署。
Tomcat官方文档指出，CATALINA_HOME路径的路径下只需要包含bin和lib目录，这也就是支持tomcat软件运行的目录，而CATALINA_BASE设置的路径可以包括上述所有目录，不过其中bin和lib目录并不是必需的，缺省时会使用CATALINA_HOME中的bin和conf。如此，我们就可以使用一个tomcat安装目录部署多个tomcat实例，这样的好处在于方便升级，就可以在不影响tomcat实例的前提下，替换掉CATALINA_HOME指定的tomcat安装目录。
![tomcat-dirs](http://jbcdn2.b0.upaiyun.com/2016/12/4724fb98341af2815fa1a87ccde8370e.png)
### 2.2 修改server.xml
这里不详细分析server.xml中每一个配置项，网上也有很多这方面的文档。下面主要说明监听端口和Host的配置内容。
在server.xml中配置了四个监听端口，分别是：
- Server Port：该端口用于监听关闭tomcat的shutdown命令，默认为8005
- Connector Port：该端口用于监听HTTP的请求，默认为8080
- AJP Port：该端口用于监听AJP（ Apache JServ Protocol ）协议上的请求，通常用于整合Apache Server等其他HTTP服务器，默认为8009
- Redirect Port：重定向端口，出现在Connector配置中，如果该Connector仅支持非SSL的普通http请求，那么该端口会把https的请求转发到这个Redirect Port指定的端口，默认为8443
可见，如果不是使用AJP协议连接tomcat，只需要保证多实例中的Server Port和Connect Port不同即可。
再来说Host配置，Host就是所谓的虚拟主机，对应包含了一个或者多个web应用程序，默认的Host配置如下


```
<Host name="localhost"  appBase="webapps" unpackWARs="true" autoDeploy="true">
```
其中：
- name： 虚拟主机的名称，一台主机表示了完全限定的域名或IP地址，默认为localhost，同时也是唯一的host，进入tomcat的所有http请求都会映射到该主机上
- appBase：web应用程序目录的路径，可以是CATALINA_HOME的相对路径，也可以写成绝对路径，默认情况下为$CATALINA_HOME/webapps
- unpackWARs： 表示是否自动解压war包
- autoDeploy：所谓的热部署，即在tomcat正在运行的情况下，如果有新的war加入，则会立即执行部署操作
- 另外再介绍一个Host中的属性—deployOnStartup：表示tomcat启动时是否自动部署appBase目录下所有的Web应用程序，默认为true。这个属性和autoDeploy会产生两次部署的“副作用”：一次是tomcat启动时就开始部署，第二次就是autoDeploy引起的热部署。因此最好将autoDeploy置为false
在部署多实例单应用的时候，默认的$CATALINA/webapps会因为tomcat安装目录升级产生不必要的麻烦，我们考虑将appBase的目录统一到另外的路径下。
最后再说明一下Context的配置，它出现在Host配置内，一个Context的配置就代表了一个web应用程序，如果配置多应用程序，就需要在Host下配置多个Context，一个简单的Context配置如下


```
<Context path="/some" docBase="someapp.war" >
```
其中:
- path：表示访问入口，例如，path=”/abc”，则访问localhost:8080/abc时，就可以访问该Context对应的应用程序。如果path=””，则直接用localhost:8080就可以访问
- docBase：表示应用程序的解包目录或者war文件路径，是Host的appBase配置目录的相对路径，也可以是直接写成绝对路径，但是不要将appBase的值，作为docBase配置路径的前缀，例如appBase=”somedir”，docBase=”somedir-someapp.war”，这样的配置会导致部署错误
通过配置Host的appBase和Context的docBase两个属性，可以将应用程序的文件和tomcat相关的目录进行分离，这样webapps目录也就没有作用了。
最终我们可以得到多实例单应用部署方案的整体框架：
![tomcat-instances](http://jbcdn2.b0.upaiyun.com/2016/12/c0292730deb862c384300b94e74f7895.png)
## 三、方案实施
现在假设我们有一台已经配置好Java环境的服务器：


```
# export JAVA_HOME=/application/jdk
# export PATH=$JAVA_HOME/bin:$JAVA_HOME/jre/bin:$PATH
# export CLASSPATH=.$CLASSPATH:$JAVA_HOME/lib:$JAVA_HOME/jre/lib:$JAVA_HOME/lib/tools.jar
 
# java -version
java version "1.8.0_101"
Java(TM) SE Runtime Environment (build 1.8.0_101-b13)
Java HotSpot(TM) 64-Bit Server VM (build 25.101-b13, mixed mode)
```
另外假设一个完好的可以运行的web应用程序已经打包成some.war，放在/data/www目录下，其中包含一个简单的index.jsp
XHTML


```
<html>
<body>
<h2>Hello World!</h2>
</body>
</html>
```
### 3.1 下载并解压tomcat


```
# cd /usr/local
# wget -q http://mirrors.cnnic.cn/apache/tomcat/tomcat-7/v7.0.72/bin/apache-tomcat-7.0.72.tar.gz
# tar zxf apache-tomcat-7.0.72.tar.gz
# ls apache-tomcat-7.0.72
bin  conf  lib  LICENSE  logs  NOTICE  RELEASE-NOTES  RUNNING.txt  temp  webapps  work
```
### 3.2 配置tomcat多实例
可以用一小段脚本来完成批量创建的工作


```
for ((i=1;i<=3;i++))
do
    mkdir /application/tomcat7-$i/{work,temp,conf,logs} -p
    cp -a /application/tomcat7-$i/application/tomcat7-$i &&
    sed -i 22s@8005@80${i}5@ /application/tomcat7-$i/conf/server.xml // 修改tomcat关闭端口
    sed -i 71s@8080@808${i}@ /application/tomcat7-$i/conf/server.xml // 修改Http监听端口
    sed -i 93s@8009@80${i}9@ /application/tomcat7-$i/conf/server.xml // 修改AJP端口
    sed -i '125s@appBase="webapps"@appBase="/data/www"@' /application/tomcat7-$i/conf/server.xml //统一web应用程序的路径
    sed -i '126s@autoDeploy="true"@autoDeploy="false"@' /application/tomcat7-$i/conf/server.xml //关闭自动部署
    sed -i '126a ' /application/tomcat7-$i/conf/server.xml //配置Context信息，指向some.war的路径
done
```
### 3.3 创建启停脚本

```
#!/bin/sh
 
CUR_DIR=`dirname $BASH_SOURCE`
export JAVA_HOME="/application/jdk"
export CATALINA_BASE=`readlink -f $CUR_DIR`
export CATALINA_HOME="/usr/local/apache-tomcat-7.0.72"
 
case $1 in
start)
[-f$CATALINA_HOME/bin/startup.sh]&&$CATALINA_HOME/bin/startup.sh
;;
stop)
[-f$CATALINA_HOME/bin/shutdown.sh]&&$CATALINA_HOME/bin/shutdown.sh
;;
restart)
[-f$CATALINA_HOME/bin/shutdown.sh]&&$CATALINA_HOME/bin/shutdown.sh&&
[-f$CATALINA_HOME/bin/startup.sh]&&$CATALINA_HOME/bin/startup.sh
;;
*)
echo"usage : $0 { start | stop | restart }"
return1;
;;
esac
 
return 0
```
把该脚本放在tomcat7-1，tomcat7-2和tomcat7-3目录下
现在我们的目录结构如下

```
# ls tomcat7-1 tomcat7-2 tomcat7-3
tomcat7-1:
conf  logs  temp  tomcat.sh  work
 
tomcat7-2:
conf  logs  temp  tomcat.sh  work
 
tomcat7-3:
conf  logs  temp  tomcat.sh  work
```
### 3.4 启动tomcats


```
# cd /application
# . tomcat7-1/tomcat.sh start
# . tomcat7-2/tomcat.sh start
# . tomcat7-3/tomcat.sh start
 
# netstat -tunlp
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address               Foreign Address             State       PID/Program name
tcp        0      0 0.0.0.0:22                  0.0.0.0:*                   LISTEN      25757/sshd
tcp        0      0 :::8029                     :::*                        LISTEN      21225/java
tcp        0      0 ::ffff:127.0.0.1:8035       :::*                        LISTEN      21186/java
tcp        0      0 :::8039                     :::*                        LISTEN      21186/java
tcp        0      0 ::ffff:127.0.0.1:8015       :::*                        LISTEN      21002/java
tcp        0      0 :::8081                     :::*                        LISTEN      21002/java
tcp        0      0 :::8082                     :::*                        LISTEN      21225/java
tcp        0      0 :::8083                     :::*                        LISTEN      21186/java
tcp        0      0 :::8019                     :::*                        LISTEN      21002/java
tcp        0      0 :::22                       :::*                        LISTEN      25757/sshd
tcp        0      0 ::ffff:127.0.0.1:8025       :::*                        LISTEN      21225/java
```
可以看到8081，8082，8083等其他端口都已经启动，现在可以使用curl命令进行访问


```
# curl localhost:8081
<html>
<body>
<h2>Hello World!</h2>
</body>
</html>
 
# curl localhost:8082
<html>
<body>
<h2>Hello World!</h2>
</body>
</html>
 
# curl localhost:8083
<html>
<body>
<h2>Hello World!</h2>
</body>
</html>
```
完成！
## 四、参考文献
- (美)布里泰恩, (美)达尔文, 吴豪. Tomcat权威指南[M]. 中国电力出版社, 2009.
- tomcat官方文档：https://tomcat.apache.org/tomcat-7.0-doc/RUNNING.txt
