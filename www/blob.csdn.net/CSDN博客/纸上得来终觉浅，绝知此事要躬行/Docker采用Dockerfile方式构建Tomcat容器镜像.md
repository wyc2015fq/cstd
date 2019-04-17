# Docker采用Dockerfile方式构建Tomcat容器镜像 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年07月07日 09:51:13[boonya](https://me.csdn.net/boonya)阅读数：8030








## 开启Tomcat远程部署设置

tomcat-users.xml

```xml
<role rolename="manager-gui"/> 
<role rolename="manager-script"/> 
<user username="tomcat" password="password" roles="manager-gui, manager-script"/>
```

## Dockerfile配置

![文件路径](https://img-blog.csdn.net/20170707094959452?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
FROM ubuntu:14.04
MAINTAINER boonya <boonya@sina.com> 
# now add java and tomcat support in the container 
ADD jdk-8u121-linux-x64.tar.gz /usr/local/ 
ADD apache-tomcat-8.5.16.tar.gz /usr/local/ 
ADD tomcat-users.xml /usr/local/apache-tomcat-8.5.16/conf/tomcat-users.xml
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

## Dockerfile构建

docker build -t boonya/tomcat-web .

*注意：”.”表示当前路径，也可以写Dockerfile的绝对路径。*

```
root@default:/usr/local/docker# docker build -t boonya/tomcat-web .
Sending build context to Docker daemon  192.7MB
Step 1/12 : FROM ubuntu:14.04
 ---> 4a2820e686c4
Step 2/12 : MAINTAINER boonya <boonya@sina.com>
 ---> Using cache
 ---> ffb09e7abb8a
Step 3/12 : ADD jdk-8u121-linux-x64.tar.gz /usr/local/
 ---> 776b93471ed0
Removing intermediate container c1341cc694b6
Step 4/12 : ADD apache-tomcat-8.5.16.tar.gz /usr/local/
 ---> f8bedc6d1f75
Removing intermediate container 49bc9c2a0eb6
Step 5/12 : ADD tomcat-users.xml /usr/local/apache-tomcat-8.5.16/conf/tomcat-users.xml
 ---> b906a8642f1a
Removing intermediate container 28b1399fa84d
Step 6/12 : ENV JAVA_HOME /usr/local/jdk1.8.0_121
 ---> Running in f17b6168acd5
 ---> 8a347fbfc46e
Removing intermediate container f17b6168acd5
Step 7/12 : ENV CLASSPATH $JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar
 ---> Running in b60488fb048f
 ---> dfae16b2b559
Removing intermediate container b60488fb048f
Step 8/12 : ENV CATALINA_HOME /usr/local/apache-tomcat-8.5.16
 ---> Running in 3b6e4c12391d
 ---> 6e1ce15492c3
Removing intermediate container 3b6e4c12391d
Step 9/12 : ENV CATALINA_BASE /usr/local/apache-tomcat-8.5.16
 ---> Running in d27dbd0eb6ce
 ---> d3ff8cacd1a2
Removing intermediate container d27dbd0eb6ce
Step 10/12 : ENV PATH $PATH:$JAVA_HOME/bin:$CATALINA_HOME/lib:$CATALINA_HOME/bin
 ---> Running in ebbbcae14f6e
 ---> a3380e6627cc
Removing intermediate container ebbbcae14f6e
Step 11/12 : EXPOSE 8080
 ---> Running in fcbbdccc369c
 ---> 2985c218b2e6
Removing intermediate container fcbbdccc369c
Step 12/12 : CMD /usr/local/apache-tomcat-8.5.16/bin/catalina.sh run
 ---> Running in 7fba86d7a6dd
 ---> 09677d05b579
Removing intermediate container 7fba86d7a6dd
Successfully built 09677d05b579
Successfully tagged boonya/tomcat-web:latest
root@default:/usr/local/docker#
```

## 启动Tomcat镜像
`docker run -p 8080:8080 boonya/tomcat-web:latest`
启动成功后访问[http://host:8080](http://host:8080)可以看到如下界面：

![Tomcat管理页面](https://img-blog.csdn.net/20170707123621492?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

查看是否启动：

```
docker@default:~$ docker ps 
CONTAINER ID        IMAGE                      COMMAND                  CREATED             STATUS              PORTS                    NAMES
875187ffb37d        boonya/tomcat-web:latest   "/bin/sh -c '/usr/..."   32 minutes ago      Up 32 minutes       0.0.0.0:8080->8080/tcp   cocky_goodall
docker@default:~$
```

## 自动部署到Tomcat镜像配置

maven settings.xml

```xml
<servers>
   <server> 
     <id>TomcatServer</id>
     <username>tomcat</username> 
     <password>password</password> 
    </server>
</servers>
```

项目 pom.xml配置

```xml
<build>
        <finalName>webtest</finalName>
        <plugins>
            <plugin>
                <groupId>org.apache.tomcat.maven</groupId>
                <artifactId>tomcat7-maven-plugin</artifactId>
                <version>2.1</version>
                <configuration>
                    <url>http://192.168.99.100:8080/manager/text</url>
                    <server>TomcatServer</server>
                    <path>/webtest</path>
                </configuration>
            </plugin>
        </plugins>
    </build>
```

远程部署403问题：

```xml
[INFO] Deploying war to http://192.168.99.100:8080/webtest  
Uploading: http://192.168.99.100:8080/manager/text/deploy?path=%2Fwebtest
2/4 KB   
4/4 KB   
Uploaded: http://192.168.99.100:8080/manager/text/deploy?path=%2Fwebtest (4 KB at 140.3 KB/sec)

[INFO] tomcatManager status code:403, ReasonPhrase:
[INFO] <!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
[INFO] <html>
[INFO]  <head>
[INFO]   <title>403 Access Denied</title>
[INFO]   <style type="text/css">
[INFO]     <!--
[INFO]     BODY {font-family:Tahoma,Arial,sans-serif;color:black;background-color:white;font-size:12px;}
[INFO]     H1 {font-family:Tahoma,Arial,sans-serif;color:white;background-color:#525D76;font-size:22px;}
[INFO]     PRE, TT {border: 1px dotted #525D76}
[INFO]     A {color : black;}A.name {color : black;}
[INFO]     -->
[INFO]   </style>
[INFO]  </head>
[INFO]  <body>
[INFO]    <h1>403 Access Denied</h1>
[INFO]    <p>
[INFO]     You are not authorized to view this page.
[INFO]    </p>
[INFO]    <p>
[INFO]     By default the Manager is only accessible from a browser running on the
[INFO]     same machine as Tomcat. If you wish to modify this restriction, you'll need
[INFO]     to edit the Manager's <tt>context.xml</tt> file.
[INFO]    </p>
[INFO]    <p>
[INFO]     If you have already configured the Manager application to allow access and
[INFO]     you have used your browsers back button, used a saved book-mark or similar
[INFO]     then you may have triggered the cross-site request forgery (CSRF) protection
[INFO]     that has been enabled for the HTML interface of the Manager application. You
[INFO]     will need to reset this protection by returning to the
[INFO]     <a href="/manager/html">main Manager page</a>. Once you
[INFO]     return to this page, you will be able to continue using the Manager
[INFO]     application's HTML interface normally. If you continue to see this access
[INFO]     denied message, check that you have the necessary permissions to access this
[INFO]     application.
[INFO]    </p>
[INFO]    <p>
[INFO]     If you have not changed
[INFO]     any configuration files, please examine the file
[INFO]     <tt>conf/tomcat-users.xml</tt> in your installation. That
[INFO]     file must contain the credentials to let you use this webapp.
[INFO]    </p>
[INFO]    <p>
[INFO]     For example, to add the <tt>manager-gui</tt> role to a user named
[INFO]     <tt>tomcat</tt> with a password of <tt>s3cret</tt>, add the following to the
[INFO]     config file listed above.
[INFO]    </p>
[INFO] <pre>
[INFO] <role rolename="manager-gui"/>
[INFO] <user username="tomcat" password="s3cret" roles="manager-gui"/>
[INFO] </pre>
[INFO]    <p>
[INFO]     Note that for Tomcat 7 onwards, the roles required to use the manager
[INFO]     application were changed from the single <tt>manager</tt> role to the
[INFO]     following four roles. You will need to assign the role(s) required for
[INFO]     the functionality you wish to access.
[INFO]    </p>
[INFO]     <ul>
[INFO]       <li><tt>manager-gui</tt> - allows access to the HTML GUI and the status
[INFO]           pages</li>
[INFO]       <li><tt>manager-script</tt> - allows access to the text interface and the
[INFO]           status pages</li>
[INFO]       <li><tt>manager-jmx</tt> - allows access to the JMX proxy and the status
[INFO]           pages</li>
[INFO]       <li><tt>manager-status</tt> - allows access to the status pages only</li>
[INFO]     </ul>
[INFO]    <p>
[INFO]     The HTML interface is protected against CSRF but the text and JMX interfaces
[INFO]     are not. To maintain the CSRF protection:
[INFO]    </p>
[INFO]    <ul>
[INFO]     <li>Users with the <tt>manager-gui</tt> role should not be granted either
[INFO]         the <tt>manager-script</tt> or <tt>manager-jmx</tt> roles.</li>
[INFO]     <li>If the text or jmx interfaces are accessed through a browser (e.g. for
[INFO]         testing since these interfaces are intended for tools not humans) then
[INFO]         the browser must be closed afterwards to terminate the session.</li>
[INFO]    </ul>
[INFO]    <p>
[INFO]     For more information - please see the
[INFO]     <a href="/docs/manager-howto.html">Manager App HOW-TO</a>.
[INFO]    </p>
[INFO]  </body>
[INFO] </html>
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time: 35.674 s
[INFO] Finished at: 2017-07-07T14:26:34+08:00
[INFO] Final Memory: 26M/204M
[INFO] ------------------------------------------------------------------------
```

*注：远程部署的错误请参考[Docker开启Tomcat8远程服务端管理角色权限构建tomcat远程部署镜像](http://blog.csdn.net/boonya/article/details/74890680)*

## 参考资料

[Docker自动部署Apache Tomcat](http://www.open-open.com/lib/view/open1428473806229.html)

[Docker使用之Tomcat容器环境设置](http://blog.csdn.net/qq_30114557/article/details/69391961)

[Docker化tomcat 并且使用maven热部署](http://www.cnblogs.com/bianqi/p/6965244.html)

[Docker修炼第一招： 先从Dockerfile开始](https://www.oschina.net/question/584116_2209819)

[Tomcat | Docker : Tomcat image doesn’t start on docker when providing tomcat-users.xml](https://stackoverflow.com/questions/28216424/tomcat-docker-tomcat-image-doesnt-start-on-docker-when-providing-tomcat-use)



