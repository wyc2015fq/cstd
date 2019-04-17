# Jenkins Centos部署过程中异常No route to host问题解决 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年08月14日 20:25:24[boonya](https://me.csdn.net/boonya)阅读数：5425








Jenkins在centos6.9（Final）下遇到如题所示没法路由主机问题，此问题是在部署阶段报出。我用Oracle VirtualBox通过Boot2Docker.iso实现的时候直接用的宿主IP 这里在centos直接自己安装的docker，所以出现差异导致找不到主机路由。

## 没法路由主机问题详细日志


```
Started by user cvnavi
Building in workspace /root/.jenkins/workspace/webtest-centos
Fetching changes from the remote Git repository
Checking out Revision 957ce0e60c3abcd970c025b6b92bc2ef356a8bf9 (refs/remotes/origin/master)
Commit message: "Update Hello.java"
[webtest-centos] $ /root/.jenkins/tools/hudson.tasks.Maven_MavenInstallation/apache-maven-3.3.9/bin/mvn -f pom.xml clean install
[INFO] Scanning for projects...
[INFO]                                                                         
[INFO] ------------------------------------------------------------------------
[INFO] Building webtest 0.0.1-SNAPSHOT
[INFO] ------------------------------------------------------------------------
[INFO] 
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ webtest ---
[INFO] Deleting /root/.jenkins/workspace/webtest-centos/target
[INFO] 
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ webtest ---
[WARNING] Using platform encoding (ANSI_X3.4-1968 actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory /root/.jenkins/workspace/webtest-centos/src/main/resources
[INFO] 
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ webtest ---
[INFO] Changes detected - recompiling the module!
[WARNING] File encoding has not been set, using platform encoding ANSI_X3.4-1968, i.e. build is platform dependent!
[INFO] Compiling 1 source file to /root/.jenkins/workspace/webtest-centos/target/classes
[INFO] 
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ webtest ---
[WARNING] Using platform encoding (ANSI_X3.4-1968 actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory /root/.jenkins/workspace/webtest-centos/src/test/resources
[INFO] 
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ webtest ---
[INFO] No sources to compile
[INFO] 
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ webtest ---
[INFO] No tests to run.
[INFO] 
[INFO] --- maven-war-plugin:2.2:war (default-war) @ webtest ---
[INFO] Packaging webapp
[INFO] Assembling webapp [webtest] in [/root/.jenkins/workspace/webtest-centos/target/webtest]
[INFO] Processing war project
[INFO] Copying webapp resources [/root/.jenkins/workspace/webtest-centos/src/main/webapp]
[INFO] Webapp assembled in [25 msecs]
[INFO] Building war: /root/.jenkins/workspace/webtest-centos/target/webtest.war
[INFO] WEB-INF/web.xml already added, skipping
[INFO] 
[INFO] --- maven-install-plugin:2.4:install (default-install) @ webtest ---
[INFO] Installing /root/.jenkins/workspace/webtest-centos/target/webtest.war to /root/.m2/repository/com/boonya/app/webtest/0.0.1-SNAPSHOT/webtest-0.0.1-SNAPSHOT.war
[INFO] Installing /root/.jenkins/workspace/webtest-centos/pom.xml to /root/.m2/repository/com/boonya/app/webtest/0.0.1-SNAPSHOT/webtest-0.0.1-SNAPSHOT.pom
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time: 1.870 s
[INFO] Finished at: 2017-08-14T12:12:33+00:00
[INFO] Final Memory: 20M/476M
[INFO] ------------------------------------------------------------------------
Deploying /root/.jenkins/workspace/webtest-centos/target/webtest.war to container Tomcat 8.x Remote with context webtest
ERROR: Build step failed with exception
org.codehaus.cargo.container.ContainerException: Failed to redeploy [/root/.jenkins/workspace/webtest-centos/target/webtest.war]
	at org.codehaus.cargo.container.tomcat.internal.AbstractTomcatManagerDeployer.redeploy(AbstractTomcatManagerDeployer.java:192)
	at hudson.plugins.deploy.CargoContainerAdapter.deploy(CargoContainerAdapter.java:77)
	at hudson.plugins.deploy.CargoContainerAdapter$DeployCallable.invoke(CargoContainerAdapter.java:147)
	at hudson.plugins.deploy.CargoContainerAdapter$DeployCallable.invoke(CargoContainerAdapter.java:117)
	at hudson.FilePath.act(FilePath.java:998)
	at hudson.FilePath.act(FilePath.java:976)
	at hudson.plugins.deploy.CargoContainerAdapter.redeploy(CargoContainerAdapter.java:114)
	at hudson.plugins.deploy.PasswordProtectedAdapterCargo.redeploy(PasswordProtectedAdapterCargo.java:93)
	at hudson.plugins.deploy.DeployPublisher.perform(DeployPublisher.java:64)
	at hudson.tasks.BuildStepMonitor$3.perform(BuildStepMonitor.java:45)
	at hudson.model.AbstractBuild$AbstractBuildExecution.perform(AbstractBuild.java:730)
	at hudson.model.AbstractBuild$AbstractBuildExecution.performAllBuildSteps(AbstractBuild.java:676)
	at hudson.model.Build$BuildExecution.post2(Build.java:186)
	at hudson.model.AbstractBuild$AbstractBuildExecution.post(AbstractBuild.java:621)
	at hudson.model.Run.execute(Run.java:1760)
	at hudson.model.FreeStyleBuild.run(FreeStyleBuild.java:43)
	at hudson.model.ResourceController.execute(ResourceController.java:97)
	at hudson.model.Executor.run(Executor.java:415)
Caused by: java.net.NoRouteToHostException: No route to host (Host unreachable)
	at java.net.PlainSocketImpl.socketConnect(Native Method)
	at java.net.AbstractPlainSocketImpl.doConnect(AbstractPlainSocketImpl.java:350)
	at java.net.AbstractPlainSocketImpl.connectToAddress(AbstractPlainSocketImpl.java:206)
	at java.net.AbstractPlainSocketImpl.connect(AbstractPlainSocketImpl.java:188)
	at java.net.SocksSocketImpl.connect(SocksSocketImpl.java:392)
	at java.net.Socket.connect(Socket.java:589)
	at java.net.Socket.connect(Socket.java:538)
	at sun.net.NetworkClient.doConnect(NetworkClient.java:180)
	at sun.net.www.http.HttpClient.openServer(HttpClient.java:432)
	at sun.net.www.http.HttpClient.openServer(HttpClient.java:527)
	at sun.net.www.http.HttpClient.<init>(HttpClient.java:211)
	at sun.net.www.http.HttpClient.New(HttpClient.java:308)
	at sun.net.www.http.HttpClient.New(HttpClient.java:326)
	at sun.net.www.protocol.http.HttpURLConnection.getNewHttpClient(HttpURLConnection.java:1202)
	at sun.net.www.protocol.http.HttpURLConnection.plainConnect0(HttpURLConnection.java:1138)
	at sun.net.www.protocol.http.HttpURLConnection.plainConnect(HttpURLConnection.java:1032)
	at sun.net.www.protocol.http.HttpURLConnection.connect(HttpURLConnection.java:966)
	at org.codehaus.cargo.container.tomcat.internal.TomcatManager.invoke(TomcatManager.java:561)
	at org.codehaus.cargo.container.tomcat.internal.TomcatManager.list(TomcatManager.java:876)
	at org.codehaus.cargo.container.tomcat.internal.TomcatManager.getStatus(TomcatManager.java:889)
	at org.codehaus.cargo.container.tomcat.internal.AbstractTomcatManagerDeployer.redeploy(AbstractTomcatManagerDeployer.java:173)
	... 17 more
java.net.NoRouteToHostException: No route to host (Host unreachable)
	at java.net.PlainSocketImpl.socketConnect(Native Method)
	at java.net.AbstractPlainSocketImpl.doConnect(AbstractPlainSocketImpl.java:350)
	at java.net.AbstractPlainSocketImpl.connectToAddress(AbstractPlainSocketImpl.java:206)
	at java.net.AbstractPlainSocketImpl.connect(AbstractPlainSocketImpl.java:188)
	at java.net.SocksSocketImpl.connect(SocksSocketImpl.java:392)
	at java.net.Socket.connect(Socket.java:589)
	at java.net.Socket.connect(Socket.java:538)
	at sun.net.NetworkClient.doConnect(NetworkClient.java:180)
	at sun.net.www.http.HttpClient.openServer(HttpClient.java:432)
	at sun.net.www.http.HttpClient.openServer(HttpClient.java:527)
	at sun.net.www.http.HttpClient.<init>(HttpClient.java:211)
	at sun.net.www.http.HttpClient.New(HttpClient.java:308)
	at sun.net.www.http.HttpClient.New(HttpClient.java:326)
	at sun.net.www.protocol.http.HttpURLConnection.getNewHttpClient(HttpURLConnection.java:1202)
	at sun.net.www.protocol.http.HttpURLConnection.plainConnect0(HttpURLConnection.java:1138)
	at sun.net.www.protocol.http.HttpURLConnection.plainConnect(HttpURLConnection.java:1032)
	at sun.net.www.protocol.http.HttpURLConnection.connect(HttpURLConnection.java:966)
	at org.codehaus.cargo.container.tomcat.internal.TomcatManager.invoke(TomcatManager.java:561)
	at org.codehaus.cargo.container.tomcat.internal.TomcatManager.list(TomcatManager.java:876)
	at org.codehaus.cargo.container.tomcat.internal.TomcatManager.getStatus(TomcatManager.java:889)
	at org.codehaus.cargo.container.tomcat.internal.AbstractTomcatManagerDeployer.redeploy(AbstractTomcatManagerDeployer.java:173)
	at hudson.plugins.deploy.CargoContainerAdapter.deploy(CargoContainerAdapter.java:77)
	at hudson.plugins.deploy.CargoContainerAdapter$DeployCallable.invoke(CargoContainerAdapter.java:147)
	at hudson.plugins.deploy.CargoContainerAdapter$DeployCallable.invoke(CargoContainerAdapter.java:117)
	at hudson.FilePath.act(FilePath.java:998)
	at hudson.FilePath.act(FilePath.java:976)
	at hudson.plugins.deploy.CargoContainerAdapter.redeploy(CargoContainerAdapter.java:114)
	at hudson.plugins.deploy.PasswordProtectedAdapterCargo.redeploy(PasswordProtectedAdapterCargo.java:93)
	at hudson.plugins.deploy.DeployPublisher.perform(DeployPublisher.java:64)
	at hudson.tasks.BuildStepMonitor$3.perform(BuildStepMonitor.java:45)
	at hudson.model.AbstractBuild$AbstractBuildExecution.perform(AbstractBuild.java:730)
	at hudson.model.AbstractBuild$AbstractBuildExecution.performAllBuildSteps(AbstractBuild.java:676)
	at hudson.model.Build$BuildExecution.post2(Build.java:186)
	at hudson.model.AbstractBuild$AbstractBuildExecution.post(AbstractBuild.java:621)
	at hudson.model.Run.execute(Run.java:1760)
	at hudson.model.FreeStyleBuild.run(FreeStyleBuild.java:43)
	at hudson.model.ResourceController.execute(ResourceController.java:97)
	at hudson.model.Executor.run(Executor.java:415)
Build step 'Deploy war/ear to a container' marked build as failure
Finished: FAILURE
```

注：此问题与镜像的核心系统是centos还是ubuntu无关，两种镜像都拉取测试过都是此问题。


## 查看iptables状态



```
[root@mysqlserver ~]# service iptables status
Table: nat
Chain PREROUTING (policy ACCEPT)
num  target     prot opt source               destination         
1    DOCKER     all  --  0.0.0.0/0            0.0.0.0/0           ADDRTYPE match dst-type LOCAL 

Chain POSTROUTING (policy ACCEPT)
num  target     prot opt source               destination         
1    MASQUERADE  all  --  172.17.0.0/16        0.0.0.0/0           
2    MASQUERADE  tcp  --  172.17.0.1           172.17.0.1          tcp dpt:8080 
3    MASQUERADE  tcp  --  172.17.0.2           172.17.0.2          tcp dpt:8080 

Chain OUTPUT (policy ACCEPT)
num  target     prot opt source               destination         
1    DOCKER     all  --  0.0.0.0/0           !127.0.0.0/8         ADDRTYPE match dst-type LOCAL 

Chain DOCKER (2 references)
num  target     prot opt source               destination         
1    DNAT       tcp  --  0.0.0.0/0            0.0.0.0/0           tcp dpt:8091 to:172.17.0.1:8080 
2    DNAT       tcp  --  0.0.0.0/0            0.0.0.0/0           tcp dpt:18091 to:172.17.0.2:8080 

Table: filter
Chain INPUT (policy ACCEPT)
num  target     prot opt source               destination         
1    ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0           tcp dpt:10050 state NEW,ESTABLISHED 
2    ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0           tcp dpt:21 state NEW,ESTABLISHED 
3    ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0           tcp dpt:3306 state NEW,ESTABLISHED 
4    ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0           tcp dpt:3036 
5    ACCEPT     all  --  0.0.0.0/0            0.0.0.0/0           state RELATED,ESTABLISHED 
6    ACCEPT     icmp --  0.0.0.0/0            0.0.0.0/0           
7    ACCEPT     all  --  0.0.0.0/0            0.0.0.0/0           
8    ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0           state NEW tcp dpt:22 
9    REJECT     all  --  0.0.0.0/0            0.0.0.0/0           reject-with icmp-host-prohibited 

Chain FORWARD (policy ACCEPT)
num  target     prot opt source               destination         
1    DOCKER     all  --  0.0.0.0/0            0.0.0.0/0           
2    ACCEPT     all  --  0.0.0.0/0            0.0.0.0/0           ctstate RELATED,ESTABLISHED 
3    ACCEPT     all  --  0.0.0.0/0            0.0.0.0/0           
4    REJECT     all  --  0.0.0.0/0            0.0.0.0/0           reject-with icmp-host-prohibited 
5    ACCEPT     all  --  0.0.0.0/0            0.0.0.0/0           

Chain OUTPUT (policy ACCEPT)
num  target     prot opt source               destination         
1    ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0           tcp spt:10050 state ESTABLISHED 
2    ACCEPT     tcp  --  0.0.0.0/0            0.0.0.0/0           tcp spt:3306 state ESTABLISHED 

Chain DOCKER (1 references)
num  target     prot opt source               destination         
1    ACCEPT     tcp  --  0.0.0.0/0            172.17.0.1          tcp dpt:8080 
2    ACCEPT     tcp  --  0.0.0.0/0            172.17.0.2          tcp dpt:8080 

[root@mysqlserver ~]#
```




## 查看Tomcat管理运行IP地址

Tomcat 管理地址：http://10.10.11.218:18091/manager/html



|Server Information| | | | | | | |
|----|----|----|----|----|----|----|----|
|Tomcat Version|JVM Version|JVM Vendor|OS Name|OS Version|OS Architecture|Hostname|IP Address|
|Apache Tomcat/8.5.16|1.8.0_121-b13|Oracle Corporation|Linux|2.6.32-504.el6.x86_64|amd64|a74e9c3227c0|172.17.0.2|




所有映射地址都对上了，但是确仍然报无法路由到主机。

## 推测错误原因



```
docker -d 
INFO[0000] Listening for HTTP on unix (/var/run/docker.sock) 
WARN[0000] You are running linux kernel version 2.6.32-504.el6.x86_64, which might be unstable running docker. Please upgrade your kernel to 3.10.0. 
INFO[0000] [graphdriver] using prior storage driver "devicemapper" 
WARN[0000] Running modprobe bridge nf_nat failed with message: , error: exit status 1 
INFO[0000] Loading containers: start.                   
............
INFO[0000] Loading containers: done.                    
INFO[0000] Daemon has completed initialization          
INFO[0000] Docker daemon                                 commit=786b29d/1.7.1 execdriver=native-0.2 graphdriver=devicemapper version=1.7.1
INFO[0470] GET /v1.19/containers/json?all=1             
INFO[0484] POST /v1.19/containers/cvnavi-centos-jenkins-20170814/start 
INFO[0492] POST /v1.19/containers/cvnavi-centos-tomcat-20170814/start
```

注：很有可能是Linux内核版本过低导致的奇葩问题。




## 路由问题解决


原因：此路由问题是因为部署设置的tomcat 主机没有设置为Docker tomcat的容器IP。

![](https://img-blog.csdn.net/20170817144810425)


![](https://img-blog.csdn.net/20170817144757503)


设置之后部署成功：

![](https://img-blog.csdn.net/20170817144944897)

Docker容器查看：

![](https://img-blog.csdn.net/20170817145304223)


注意：我用Oracle VM通过Boot2Docker.iso实现的时候直接用的宿主IP 这里在centos直接自己安装的docker，所以出现差异导致找不到主机路由。














