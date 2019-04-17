# 开启Tomcat8管理角色权限构建Docker tomcat远程部署镜像 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年07月09日 16:18:03[boonya](https://me.csdn.net/boonya)阅读数：2128








在“[Docker采用Dockerfile方式构建Tomcat容器镜像](http://blog.csdn.net/boonya/article/details/74626431)”一文中，Docker镜像的构建是成功了的，但是通过Maven发布项目到Tomcat8却遇到了服务器拒绝访问403的错误，因此才有了本文记述解决Tomcat访问问题的文章。主要是要配置${TOMCAT_HOME}/conf/tomcat-users.xml和${TOMCAT_HOME}/webapps/manager/META-INF/context.xml两个文件。

## 基础操作说明

因为下载的linux tomcat8没有开放server用户管理和远程权限，所以需要编辑${TOMCAT_HOME}/conf/tomcat-users.xml和${TOMCAT_HOME}/webapps/manager/META-INF/context.xml两个文件。我将所有操作文件都放于/usr/local/docker下：

![](https://img-blog.csdn.net/20170709160522021)

Tomcat 包重新编辑 打包的步骤如下：


1、进入/usr/lcoal/docker解压apache-tomcat-8.5.16.tar.gz


`tar -zxvf apache-tomcat-8.5.16.tar.gz`


2、编辑tomcat-users.xml和context.xml(下面有)

3、重新打包apache-tomcat-8.5.16.tar.gz



`tar czvf apache-tomcat-8.5.16.tar.gz apache-tomcat-8.5.16`


## Dockerfile配置



```
FROM ubuntu:14.04
MAINTAINER boonya <boonya@sina.com> 
# now add java and tomcat support in the container 
ADD jdk-8u121-linux-x64.tar.gz /usr/local/ 
ADD apache-tomcat-8.5.16.tar.gz /usr/local/ 
# ADD tomcat-users.xml /usr/local/apache-tomcat-8.5.16/conf/tomcat-users.xml
# configuration of java and tomcat ENV 
ENV JAVA_HOME /usr/local/jdk1.8.0_121 
ENV CLASSPATH $JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar 
ENV CATALINA_HOME /usr/local/apache-tomcat-8.5.16 
ENV CATALINA_BASE /usr/local/apache-tomcat-8.5.16 
ENV PATH $PATH:$JAVA_HOME/bin:$CATALINA_HOME/lib:$CATALINA_HOME/bin 
# container listener port 
EXPOSE 8080 
# startup web application services by self 
CMD /usr/local/apache-tomcat-8.5.16/bin/catalina.sh run
```

注：此处注释掉了此前配置的ADD tomcat-users.xml的配置。


## Tomcat用户角色配置

编辑${TOMCAT_HOME}/conf/tomcat-users.xml：



```
<?xml version="1.0" encoding="UTF-8"?>

<tomcat-users xmlns="http://tomcat.apache.org/xml"
              xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
              xsi:schemaLocation="http://tomcat.apache.org/xml tomcat-users.xsd"
              version="1.0">
<role rolename="manager"/>
<role rolename="manager-gui"/>
<role rolename="admin"/>
<role rolename="admin-gui"/>
<role rolename="manager-script"/>
<user username="tomcat" password="password" roles="admin-gui,admin,manager-gui,manager,manager-script"/>
</tomcat-users>
```




## Manager管理权限过滤配置

编辑${TOMCAT_HOME}/webapps/manager/META-INF/context.xml：



```
<?xml version="1.0" encoding="UTF-8"?>
<Context antiResourceLocking="false" privileged="true" docBase="${catalina.home}/webapps/manager" >
  <Valve className="org.apache.catalina.valves.RemoteAddrValve"    allow="^.*$" />
  <Manager sessionAttributeValueClassNameFilter="java\.lang\.(?:Boolean|Integer|Long|Number|String)|org\.apache\.catalina\.filters\.CsrfPreventionFilter\$LruCache(?:\$1)?|java\.util\.(?:Linked)?HashMap"/>
</Context>
```
原始文件只支持本地管理：




```
<Context antiResourceLocking="false" privileged="true" >
  <Valve className="org.apache.catalina.valves.RemoteAddrValve"
         allow="127\.\d+\.\d+\.\d+|::1|0:0:0:0:0:0:0:1" />
  <Manager sessionAttributeValueClassNameFilter="java\.lang\.(?:Boolean|Integer|Long|Number|String)|org\.apache\.catalina\.filters\.CsrfPreventionFilter\$LruCache(?:\$1)?|java\.util\.(?:Linked)?HashMap"/>
</Context>
```




## Docker tomcat镜像构建和运行

以上文件编辑之后就可以开始构建Tomcat的镜像了：

构建：


`docker build -t boonya/tomcat-allow-remote .`
查看镜像：




```
boonya/tomcat-allow-remote   latest              6137f64875dd        34 minutes ago      571MB
boonya/tomcat-web            latest              09677d05b579        2 days ago          571MB
<none>                       <none>              e1471ab1845a        2 days ago          188MB
<none>                       <none>              cf240e31edfb        2 days ago          188MB
<none>                       <none>              717961796ae2        2 days ago          334MB
apptomcat                    1.0                 249dc3f5dfb0        3 days ago          334MB
tomcat                       8                   a2fbbcebd67e        8 days ago          334MB
ubuntu                       14.04               4a2820e686c4        2 weeks ago         188MB
wurstmeister/kafka           latest              12453f4efa7b        6 weeks ago         265MB
docker@default:~$
```

运行：`docker run -p 8080:8080 boonya/tomcat-allow-remote:latest`


## 验证Tomcat服务器远程管理

进入默认端口主页：



![](https://img-blog.csdn.net/20170709161242709)



验证是否可登录：



![](https://img-blog.csdn.net/20170709161338532)



注：镜像是基于Tomcat8的远程管理角色权限，管理账号和密码是：tomcat/password。


至此，Tomcat server端成功开放管理权限。


## 验证Maven远程部署项目

还是以文中开头提到的文章为例，maven的webtest项目相关的配置这里就补贴出来了。下面run as Configuration:

![](https://img-blog.csdn.net/20170709162622838)


实际上的命令是：mvn clean tomcat7:deploy  后台输出日志如下：



```
[INFO] Scanning for projects...
[INFO] 
[INFO] ------------------------------------------------------------------------
[INFO] Building webtest 0.0.1-SNAPSHOT
[INFO] ------------------------------------------------------------------------
[INFO] 
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ webtest ---
[INFO] Deleting D:\workspace-sts-3.8.4.RELEASE\webtest\target
[INFO] 
[INFO] >>> tomcat7-maven-plugin:2.1:deploy (default-cli) > package @ webtest >>>
[INFO] 
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ webtest ---
[WARNING] Using platform encoding (UTF-8 actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] Copying 0 resource
[INFO] 
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ webtest ---
[INFO] Changes detected - recompiling the module!
[WARNING] File encoding has not been set, using platform encoding UTF-8, i.e. build is platform dependent!
[INFO] Compiling 1 source file to D:\workspace-sts-3.8.4.RELEASE\webtest\target\classes
[INFO] 
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ webtest ---
[INFO] Not copying test resources
[INFO] 
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ webtest ---
[INFO] Not compiling test sources
[INFO] 
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ webtest ---
[INFO] Tests are skipped.
[INFO] 
[INFO] --- maven-war-plugin:2.2:war (default-war) @ webtest ---
[INFO] Packaging webapp
[INFO] Assembling webapp [webtest] in [D:\workspace-sts-3.8.4.RELEASE\webtest\target\webtest]
[INFO] Processing war project
[INFO] Copying webapp resources [D:\workspace-sts-3.8.4.RELEASE\webtest\src\main\webapp]
[INFO] Webapp assembled in [243 msecs]
[INFO] Building war: D:\workspace-sts-3.8.4.RELEASE\webtest\target\webtest.war
[INFO] WEB-INF\web.xml already added, skipping
[INFO] 
[INFO] <<< tomcat7-maven-plugin:2.1:deploy (default-cli) < package @ webtest <<<
[INFO] 
[INFO] 
[INFO] --- tomcat7-maven-plugin:2.1:deploy (default-cli) @ webtest ---
[INFO] Deploying war to http://192.168.99.100:8080/webtest  
Uploading: http://192.168.99.100:8080/manager/text/deploy?path=%2Fwebtest
2/4 KB   
4/4 KB   
Uploaded: http://192.168.99.100:8080/manager/text/deploy?path=%2Fwebtest (4 KB at 90.8 KB/sec)

[INFO] tomcatManager status code:200, ReasonPhrase:
[INFO] OK - Deployed application at context path [/webtest]
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time: 26.821 s
[INFO] Finished at: 2017-07-09T16:22:11+08:00
[INFO] Final Memory: 20M/115M
[INFO] ------------------------------------------------------------------------
```
看似成功了，还是要去验证一下。打开服务端管理，看到如下图：





![](https://img-blog.csdn.net/20170709162831820)

webtest项目赫然在列，100%成功了。本文Docker相关的文件可以到我的百度云盘进行下载：[http://pan.baidu.com/s/1qXZN4Wc](http://pan.baidu.com/s/1qXZN4Wc)


## 参考资料

[Tomcat8访问管理页面localhost出现：403 Access Denied](http://blog.csdn.net/u012167045/article/details/61624226)



[tomcat7, tomcat8的进入manager, tomcat_user.xm配,以及如何配置远程访问manager](http://blog.csdn.net/u012167045/article/details/61624226)


[tomcat8配置管理员后仍然报403](http://www.cnblogs.com/ianduin/articles/6506645.html)







