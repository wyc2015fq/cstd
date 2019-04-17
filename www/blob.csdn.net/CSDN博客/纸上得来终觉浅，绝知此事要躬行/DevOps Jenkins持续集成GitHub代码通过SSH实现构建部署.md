# DevOps Jenkins持续集成GitHub代码通过SSH实现构建部署 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年07月17日 22:01:43[boonya](https://me.csdn.net/boonya)阅读数：2288








最近在虚拟环境中使用安装Jenkins持续集成环境，中间遇到些问题，以致于浪费了不少时间。此篇通过SSH Server方式来实现github源码通过Jenkin持续集成到Docker容器的实例。so，遇到问题多从自身出发，咨询他人未必能解决，还是自身要注意细节问题。


## Jenkins 安装

Jenkins命令行执行安装：$ java -jar jenkins.war



```
boonya@ubuntu:/usr/local/docker/docker-jenkins$ java -jar jenkins.war
Running from: /usr/local/docker/docker-jenkins/jenkins.war
webroot: $user.home/.jenkins
Jul 17, 2017 6:48:59 AM Main deleteWinstoneTempContents
WARNING: Failed to delete the temporary Winstone file /tmp/winstone/jenkins.war
Jul 17, 2017 6:49:02 AM org.eclipse.jetty.util.log.Log initialized
INFO: Logging initialized @11418ms to org.eclipse.jetty.util.log.JavaUtilLog
Jul 17, 2017 6:49:05 AM winstone.Logger logInternal
INFO: Beginning extraction from war file
Jul 17, 2017 6:49:18 AM org.eclipse.jetty.server.handler.ContextHandler setContextPath
WARNING: Empty contextPath
Jul 17, 2017 6:49:19 AM org.eclipse.jetty.server.Server doStart
INFO: jetty-9.4.z-SNAPSHOT
Jul 17, 2017 6:49:21 AM org.eclipse.jetty.webapp.StandardDescriptorProcessor visitServlet
INFO: NO JSP Support for /, did not find org.eclipse.jetty.jsp.JettyJspServlet
Jul 17, 2017 6:49:22 AM org.eclipse.jetty.server.session.DefaultSessionIdManager doStart
INFO: DefaultSessionIdManager workerName=node0
Jul 17, 2017 6:49:22 AM org.eclipse.jetty.server.session.DefaultSessionIdManager doStart
INFO: No SessionScavenger set, using defaults
Jul 17, 2017 6:49:22 AM org.eclipse.jetty.server.session.HouseKeeper startScavenging
INFO: Scavenging every 600000ms
Jenkins home directory: /home/boonya/.jenkins found at: $user.home/.jenkins
Jul 17, 2017 6:49:26 AM org.eclipse.jetty.server.handler.ContextHandler doStart
INFO: Started w.@50eca7c6{/,file:///home/boonya/.jenkins/war/,AVAILABLE}{/home/boonya/.jenkins/war}
Jul 17, 2017 6:49:26 AM org.eclipse.jetty.server.AbstractConnector doStart
INFO: Started ServerConnector@419a20a6{HTTP/1.1,[http/1.1]}{0.0.0.0:8080}
Jul 17, 2017 6:49:26 AM org.eclipse.jetty.server.Server doStart
INFO: Started @35512ms
Jul 17, 2017 6:49:26 AM winstone.Logger logInternal
INFO: Winstone Servlet Engine v4.0 running: controlPort=disabled
Jul 17, 2017 6:49:29 AM jenkins.InitReactorRunner$1 onAttained
INFO: Started initialization
Jul 17, 2017 6:49:29 AM jenkins.InitReactorRunner$1 onAttained
INFO: Listed all plugins
Jul 17, 2017 6:49:37 AM jenkins.InitReactorRunner$1 onAttained
INFO: Prepared all plugins
Jul 17, 2017 6:49:37 AM jenkins.InitReactorRunner$1 onAttained
INFO: Started all plugins
Jul 17, 2017 6:49:37 AM jenkins.InitReactorRunner$1 onAttained
INFO: Augmented all extensions
Jul 17, 2017 6:49:45 AM jenkins.InitReactorRunner$1 onAttained
INFO: Loaded all jobs
Jul 17, 2017 6:49:48 AM hudson.model.AsyncPeriodicWork$1 run
INFO: Started Download metadata
Jul 17, 2017 6:49:50 AM jenkins.InitReactorRunner$1 onAttained
INFO: Completed initialization
Jul 17, 2017 6:49:52 AM org.springframework.context.support.AbstractApplicationContext prepareRefresh
INFO: Refreshing org.springframework.web.context.support.StaticWebApplicationContext@15930e35: display name [Root WebApplicationContext]; startup date [Mon Jul 17 06:49:52 PDT 2017]; root of context hierarchy
Jul 17, 2017 6:49:52 AM org.springframework.context.support.AbstractApplicationContext obtainFreshBeanFactory
INFO: Bean factory for application context [org.springframework.web.context.support.StaticWebApplicationContext@15930e35]: org.springframework.beans.factory.support.DefaultListableBeanFactory@df9e3ea
Jul 17, 2017 6:49:52 AM org.springframework.beans.factory.support.DefaultListableBeanFactory preInstantiateSingletons
INFO: Pre-instantiating singletons in org.springframework.beans.factory.support.DefaultListableBeanFactory@df9e3ea: defining beans [authenticationManager]; root of factory hierarchy
Jul 17, 2017 6:49:52 AM org.springframework.context.support.AbstractApplicationContext prepareRefresh
INFO: Refreshing org.springframework.web.context.support.StaticWebApplicationContext@4752f4f: display name [Root WebApplicationContext]; startup date [Mon Jul 17 06:49:52 PDT 2017]; root of context hierarchy
Jul 17, 2017 6:49:52 AM org.springframework.context.support.AbstractApplicationContext obtainFreshBeanFactory
INFO: Bean factory for application context [org.springframework.web.context.support.StaticWebApplicationContext@4752f4f]: org.springframework.beans.factory.support.DefaultListableBeanFactory@14144e84
Jul 17, 2017 6:49:52 AM org.springframework.beans.factory.support.DefaultListableBeanFactory preInstantiateSingletons
INFO: Pre-instantiating singletons in org.springframework.beans.factory.support.DefaultListableBeanFactory@14144e84: defining beans [filter,legacy]; root of factory hierarchy
Jul 17, 2017 6:49:54 AM jenkins.install.SetupWizard init
INFO: 

*************************************************************
*************************************************************
*************************************************************

Jenkins initial setup is required. An admin user has been created and a password generated.
Please use the following password to proceed to installation:

f6337157bda8440f9e66dde6f4d79a6d

This may also be found at: /home/boonya/.jenkins/secrets/initialAdminPassword

*************************************************************
*************************************************************
*************************************************************

Jul 17, 2017 6:50:08 AM hudson.model.UpdateSite updateData
INFO: Obtained the latest update center data file for UpdateSource default
Jul 17, 2017 6:50:08 AM hudson.model.UpdateSite updateData
INFO: Obtained the latest update center data file for UpdateSource default
Jul 17, 2017 6:50:08 AM hudson.WebAppMain$3 run
INFO: Jenkins is fully up and running
Jul 17, 2017 6:50:11 AM hudson.model.DownloadService$Downloadable load
INFO: Obtained the updated data file for hudson.tasks.Maven.MavenInstaller
Jul 17, 2017 6:51:43 AM hudson.model.DownloadService$Downloadable load
INFO: Obtained the updated data file for hudson.tools.JDKInstaller
Jul 17, 2017 6:51:43 AM hudson.model.AsyncPeriodicWork$1 run
INFO: Finished Download metadata. 115,667 ms
Jul 17, 2017 6:53:34 AM org.eclipse.jetty.server.session.Session beginInvalidate
INFO: Session node0f9fhxe6uq0n2lim0qta6dtt10 already being invalidated
```

注意：这种方式停服后重新启动不能记住以前的数据。


### 安装Jenkins服务

参考官文：[https://wiki.jenkins-ci.org/display/JENKINS/Installing+Jenkins+on+Ubuntu](https://wiki.jenkins-ci.org/display/JENKINS/Installing+Jenkins+on+Ubuntu)


建议采用下面的方式执行进行安装，如果要安装jenkins为服务需要执行下面四步：





```
wget -q -O - https://pkg.jenkins.io/debian/jenkins-ci.org.key | sudo apt-key add - 
sudo sh -c 'echo deb http://pkg.jenkins.io/debian-stable binary/ > /etc/apt/sources.list.d/jenkins.list' 
sudo apt-get update 
sudo apt-get install jenkins
```



```
root@ubuntu:/usr/local/docker/docker-jenkins# sudo apt-get install jenkins
Reading package lists... Done
Building dependency tree       
Reading state information... Done
The following extra packages will be installed:
  daemon openjdk-8-jre-headless
Suggested packages:
  openjdk-8-jre-jamvm libnss-mdns fonts-ipafont-gothic fonts-ipafont-mincho ttf-wqy-microhei ttf-wqy-zenhei fonts-indic
The following NEW packages will be installed:
  daemon jenkins openjdk-8-jre-headless
0 upgraded, 3 newly installed, 0 to remove and 87 not upgraded.
Need to get 133 MB of archives.
After this operation, 346 MB of additional disk space will be used.
Do you want to continue? [Y/n] Y
Get:1 http://us.archive.ubuntu.com/ubuntu/ vivid/universe openjdk-8-jre-headless amd64 8u45-b14-1 [63.1 MB]
Get:2 http://pkg.jenkins-ci.org/debian-stable/ binary/ jenkins 2.60.1-1 [70.3 MB]
Get:3 http://us.archive.ubuntu.com/ubuntu/ vivid/universe openjdk-8-jre-headless amd64 8u45-b14-1 [63.1 MB]                                                                                   
Get:4 http://us.archive.ubuntu.com/ubuntu/ vivid/universe openjdk-8-jre-headless amd64 8u45-b14-1 [63.1 MB]                                                                                   
53% [4 openjdk-8-jre-headless 243 kB/63.1 MB 0%]
Get:5 http://us.archive.ubuntu.com/ubuntu/ vivid/universe openjdk-8-jre-headless amd64 8u45-b14-1 [63.1 MB]                                                                                   
Get:6 http://us.archive.ubuntu.com/ubuntu/ vivid/universe openjdk-8-jre-headless amd64 8u45-b14-1 [63.1 MB]                                                                                   
Get:7 http://us.archive.ubuntu.com/ubuntu/ vivid/universe openjdk-8-jre-headless amd64 8u45-b14-1 [63.1 MB]                                                                                   
Get:8 http://us.archive.ubuntu.com/ubuntu/ vivid/universe openjdk-8-jre-headless amd64 8u45-b14-1 [63.1 MB]                                                                                   
Get:9 http://us.archive.ubuntu.com/ubuntu/ vivid/universe openjdk-8-jre-headless amd64 8u45-b14-1 [63.1 MB]                                                                                   
Get:10 http://us.archive.ubuntu.com/ubuntu/ vivid/universe daemon amd64 0.6.4-1 [98.2 kB]                                                                                                     
Fetched 132 MB in 2h 15min 55s (16.2 kB/s)                                                                                                                                                    
Selecting previously unselected package openjdk-8-jre-headless:amd64.
(Reading database ... 77335 files and directories currently installed.)
Preparing to unpack .../openjdk-8-jre-headless_8u45-b14-1_amd64.deb ...
Unpacking openjdk-8-jre-headless:amd64 (8u45-b14-1) ...
Selecting previously unselected package daemon.
Preparing to unpack .../daemon_0.6.4-1_amd64.deb ...
Unpacking daemon (0.6.4-1) ...
Selecting previously unselected package jenkins.
Preparing to unpack .../jenkins_2.60.1-1_all.deb ...
Unpacking jenkins (2.60.1-1) ...
Processing triggers for man-db (2.7.0.2-5) ...
Processing triggers for ureadahead (0.100.0-19) ...
Processing triggers for systemd (219-7ubuntu3) ...
Setting up openjdk-8-jre-headless:amd64 (8u45-b14-1) ...
update-alternatives: using /usr/lib/jvm/java-8-openjdk-amd64/jre/bin/rmid to provide /usr/bin/rmid (rmid) in auto mode
update-alternatives: using /usr/lib/jvm/java-8-openjdk-amd64/jre/bin/java to provide /usr/bin/java (java) in auto mode
update-alternatives: using /usr/lib/jvm/java-8-openjdk-amd64/jre/bin/keytool to provide /usr/bin/keytool (keytool) in auto mode
update-alternatives: using /usr/lib/jvm/java-8-openjdk-amd64/jre/bin/jjs to provide /usr/bin/jjs (jjs) in auto mode
update-alternatives: using /usr/lib/jvm/java-8-openjdk-amd64/jre/bin/pack200 to provide /usr/bin/pack200 (pack200) in auto mode
update-alternatives: using /usr/lib/jvm/java-8-openjdk-amd64/jre/bin/rmiregistry to provide /usr/bin/rmiregistry (rmiregistry) in auto mode
update-alternatives: using /usr/lib/jvm/java-8-openjdk-amd64/jre/bin/unpack200 to provide /usr/bin/unpack200 (unpack200) in auto mode
update-alternatives: using /usr/lib/jvm/java-8-openjdk-amd64/jre/bin/orbd to provide /usr/bin/orbd (orbd) in auto mode
update-alternatives: using /usr/lib/jvm/java-8-openjdk-amd64/jre/bin/servertool to provide /usr/bin/servertool (servertool) in auto mode
update-alternatives: using /usr/lib/jvm/java-8-openjdk-amd64/jre/bin/tnameserv to provide /usr/bin/tnameserv (tnameserv) in auto mode
update-alternatives: using /usr/lib/jvm/java-8-openjdk-amd64/jre/lib/jexec to provide /usr/bin/jexec (jexec) in auto mode
Setting up daemon (0.6.4-1) ...
Setting up jenkins (2.60.1-1) ...
insserv: script mysql.server.sh: service mysql already provided!
Processing triggers for ureadahead (0.100.0-19) ...
Processing triggers for systemd (219-7ubuntu3) ...
```

注：管理密码在/var/lib/jenkins/secrets/initialAdminPassword文件里。

查看jenkins服务进程：




`ps -ef | grep jenkins`

访问路径不需要跟应用上下文，直接访问：[http://10.0.0.128:8080/](http://10.0.0.128:8080/)，当然你也可以放在Tomcat等容器里面运行。

Jenkins升级命令：



```
sudo apt-get update 
sudo apt-get install jenkins
```





注：jenkins升级可以直接替换jenkins.war为最新即可。

## Jenkins需要的SSH处理

### 生成公钥私钥

一般用到SSH方式时，一般Jenkins和Tomcat并不是同一台服务器，所以才需要用到SSH进行文件传输。假设10.0.0.128是Jenkins服务器(Docker容器)，而10.0.129是Tomcat服务器（Docker容器 ）。在Jenkins服务器执行，命令：

$ ssh-keygen -t rsa -P ''
注：‘’表示密码为空。




```
boonya@ubuntu:~$ ssh-keygen -t rsa -P ''
Generating public/private rsa key pair.
Enter file in which to save the key (/home/boonya/.ssh/id_rsa): 
Created directory '/home/boonya/.ssh'.
Your identification has been saved in /home/boonya/.ssh/id_rsa.
Your public key has been saved in /home/boonya/.ssh/id_rsa.pub.
The key fingerprint is:
7f:a6:95:3d:7d:d5:b1:78:2b:7f:3f:58:92:61:7c:64 boonya@ubuntu
The key's randomart image is:
+---[RSA 2048]----+
|                 |
|               E |
|            . o. |
|             +..+|
|        S   ..+oo|
|         .   =.oo|
|          . =.*.o|
|           = .ooo|
|          .    .=|
+-----------------+
boonya@ubuntu:~$
```

上面输出了公钥（/home/boonya/.ssh/id_rsa.pub）和私钥（/home/boonya/.ssh/id_rsa）：公钥认证，是使用一对加密字符串，一个称为公钥(public key)， 任何人都可以看到其内容，用于加密；另一个称为密钥(private key)，只有拥有者才能看到，用于解密。



### 发送公钥

使用如下命令进行公钥发送：

$ ssh-copy-id -i    /home/boonya/.ssh/id_rsa.pub    <receiver IP>




```
boonya@ubuntu:~$ ssh-copy-id -i    /home/boonya/.ssh/id_rsa.pub 10.0.0.129
The authenticity of host '10.0.0.129 (10.0.0.129)' can't be established.
ECDSA key fingerprint is 3a:72:d1:d6:c3:c7:48:c5:5c:f8:a3:c8:8a:f5:36:19.
Are you sure you want to continue connecting (yes/no)? yes
/usr/bin/ssh-copy-id: INFO: attempting to log in with the new key(s), to filter out any that are already installed
/usr/bin/ssh-copy-id: INFO: 1 key(s) remain to be installed -- if you are prompted now it is to install the new keys
boonya@10.0.0.129's password: 

Number of key(s) added: 1

Now try logging into the machine, with:   "ssh '10.0.0.129'"
and check to make sure that only the key(s) you wanted were added.

boonya@ubuntu:~$
```

通过SSH去验证上次登录：




```
boonya@ubuntu:~$ ssh 10.0.0.129
Welcome to Ubuntu 15.04 (GNU/Linux 3.19.0-15-generic x86_64)

 * Documentation:  https://help.ubuntu.com/
Your Ubuntu release is not supported anymore.
For upgrade information, please visit:
http://www.ubuntu.com/releaseendoflife

New release '15.10' available.
Run 'do-release-upgrade' to upgrade to it.

Last login: Mon Jul 17 08:29:58 2017 from 10.0.0.129
boonya@ubuntu:~$
```

此时表明我们的Jenkins可以通过SSH去部署项目到Tomcat的服务上了。


### Jenkins安装SSH插件

![](https://img-blog.csdn.net/20170717235244065)

### Jenkins系统配置SSH插件

![](https://img-blog.csdn.net/20170717235416158)


## Jenkins全局工具设置

### JDK-Git-Maven

![](https://img-blog.csdn.net/20170717235918959)


## 通过编写Shell脚本部署应用










后续继续补充................


## 参考资料




[Linux配置SSH公钥认证与Jenkins远程登录进行自动发布](http://www.cnblogs.com/jager/p/5986563.html)















