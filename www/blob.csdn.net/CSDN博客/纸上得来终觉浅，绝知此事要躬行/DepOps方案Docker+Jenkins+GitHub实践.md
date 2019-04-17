# DepOps方案Docker+Jenkins+GitHub实践 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年07月12日 11:19:13[boonya](https://me.csdn.net/boonya)阅读数：960








要实现—“DepOps方案：Docker+Jenkins+GitHub实践”必须实现三个步骤：（1）构建自己的Docker镜像容器 （2）Jenkins CI持续集成工具管理 (3)Github仓库代码管理。Docker可以根据任一一个镜像的基础上来构建自己的镜像，也就是Docker允许继承并自我定义和发展自己的镜像。使用镜像有很多好处，最直接的就是可以省却复杂环境的再次构建。


## Dockerfile文件配置

maven下载地址：[http://maven.apache.org/download.cgi](http://maven.apache.org/download.cgi)


jenkins下载地址：[http://updates.jenkins-ci.org/download/war/](http://updates.jenkins-ci.org/download/war/)




```
FROM boonyadocker/tomcat-allow-remote:latest
MAINTAINER boonya <boonya@sina.com> 

# ************now add java and tomcat support in the container ******
ADD apache-maven-3.3.9-bin.tar.gz /usr/local/ 
ADD jenkins.war /usr/local/apache-tomcat-8.5.16/webapps/

# ************configuration of java and tomcat ENV **********

ENV MAVEN_HOME /usr/local/apache-maven-3.3.9

ENV PATH $PATH:$MAVEN_HOME/bin 

# ************container listener port **************
EXPOSE 8080 
# startup web application services by self 
CMD /usr/local/apache-tomcat-8.5.16/bin/catalina.sh run
```

注：以上是基于我的boonyadocker/tomcat-allow-remote:latest镜像构建一个Jenkins的配置。



## Dockerfile镜像构建

构建命令： docker build -t boonyadocker/jenkins .




```
root@default:/usr/local/docker#  docker build -t boonyadocker/jenkins .
Sending build context to Docker daemon  81.23MB
Step 1/8 : FROM boonyadocker/tomcat-allow-remote:latest
 ---> 6137f64875dd
Step 2/8 : MAINTAINER boonya <boonya@sina.com>
 ---> Running in de845d9c160d
 ---> 4b0af4f26dd4
Removing intermediate container de845d9c160d
Step 3/8 : ADD apache-maven-3.3.9-bin.tar.gz /usr/local/
 ---> 27d8a3eee9c0
Removing intermediate container d35afcbae453
Step 4/8 : ADD jenkins.war /usr/local/apache-tomcat-8.5.16/webapps/
 ---> 13fe633c62df
Removing intermediate container 5c22fe45e3ec
Step 5/8 : ENV MAVEN_HOME /usr/local/apache-maven-3.3.9
 ---> Running in 470098cd48c6
 ---> 8de0d55b34b8
Removing intermediate container 470098cd48c6
Step 6/8 : ENV PATH $PATH:$MAVEN_HOME/bin
 ---> Running in b8aeda93a0ec
 ---> 4516bb4c1bd3
Removing intermediate container b8aeda93a0ec
Step 7/8 : EXPOSE 8080
 ---> Running in 318ccf3dd328
 ---> ffa7621607f4
Removing intermediate container 318ccf3dd328
Step 8/8 : CMD /usr/local/apache-tomcat-8.5.16/bin/catalina.sh run
 ---> Running in b685a0a9f8a7
 ---> 7cd7e633a573
Removing intermediate container b685a0a9f8a7
Successfully built 7cd7e633a573
Successfully tagged boonyadocker/jenkins:latest
```



## Jenkins镜像的运行

首先查看构建的镜像列表：



```
root@default:/usr/local/docker# docker images
REPOSITORY                         TAG                 IMAGE ID            CREATED             SIZE
boonyadocker/jenkins               latest              7cd7e633a573        11 minutes ago      654MB
boonyadocker/tomcat-allow-remote   latest              6137f64875dd        2 days ago          571MB
boonyadocker/tomcat-web            latest              09677d05b579        5 days ago          571MB
apptomcat                          1.0                 249dc3f5dfb0        6 days ago          334MB
tomcat                             8                   a2fbbcebd67e        11 days ago         334MB
jenkins                            latest              0b4d4d677a26        2 weeks ago         681MB
ubuntu                             14.04               4a2820e686c4        3 weeks ago         188MB
wurstmeister/kafka                 latest              12453f4efa7b        6 weeks ago         265MB
root@default:/usr/local/docker#
```




运行镜像命令： 



`docker run  -t -p 8080:8080 boonyadocker/jenkins:latest`


运行完成后，打开f服务访问地址，如：http://192.168.99.100:8080/，如果看到如下界面说明启动成功：

![](https://img-blog.csdn.net/20170712105005750)

## Jenkins 基础安装和设置

### 用户授权

启动后可以访问Jenkins：

![](https://img-blog.csdn.net/20170712105439844)

密码可以通过上面提示的方式去获取，如Docker的是在：[https://wiki.jenkins.io/display/JENKINS/Logging#Logging-Docker](https://wiki.jenkins.io/display/JENKINS/Logging#Logging-Docker)

因为我的Jenkins 输入docker logs containerId并不能看到tomcat启动的日志，所以只好关闭容器重启，重启后Jenkins部署过程中仍然会提示Administrator paasword请注意查看启动日志，及时copy:



```
12-Jul-2017 03:07:23.100 INFO [Jenkins initialization thread] jenkins.install.SetupWizard.init 

*************************************************************
*************************************************************
*************************************************************

Jenkins initial setup is required. An admin user has been created and a password generated.
Please use the following password to proceed to installation:

173c5b840b074186ae63eb98e3bbaebc

This may also be found at: /root/.jenkins/secrets/initialAdminPassword

*************************************************************
*************************************************************
*************************************************************
```

上面173c5b840b074186ae63eb98e3bbaebc就是我们所需要继续安装的密码，不知道你拿到这个密码是否可用，你可以试下。


### 插件安装

![](https://img-blog.csdn.net/20170712111358190)


选择推荐安装的插件就可以了。

![](https://img-blog.csdn.net/20170712111445061)

### 创建用户

![](https://img-blog.csdn.net/20170712111506468)

看到如下界面表示基础的安装已经完成：


![](https://img-blog.csdn.net/20170712111545645)

下次你再次启动的时候，这些过程都不会再出现了。

## 创建一个项目Job并构建GitHub源码

### 全局工具设置

![](https://img-blog.csdn.net/20170712120037130)


### 设置插件管理

![](https://img-blog.csdn.net/20170712115732669)


### 创建项目

![](https://img-blog.csdn.net/20170712115655220)


### 配置项目

![](https://img-blog.csdn.net/20170712120221975)

### Credentials设置

![](https://img-blog.csdn.net/20170712120255973)

### 构建触发器 

采用SCM从Github上定时拉取代码，一旦有代码更新就进行构建和部署：


![](https://img-blog.csdn.net/20170717191734213)


### 构建项目

![](https://img-blog.csdn.net/20170712120431339)

点击立即构建，首次构建maven回去下载包，所以比较慢，请耐心等待。我们来看第二次构建的控制台日志输出：
![](https://img-blog.csdn.net/20170712120746385)

至此项目构建环境搭建成功。


### 构建后自动部署

上面的的Deploy war/ear to container配置问题修正（名字误差问题可以用*表示）：![](https://img-blog.csdn.net/20170717172631700)

注：这里的Tomcat URL是我的boonyadocker/tomcat-allow-remote:latest 镜像运行地址。


通过Jenkins立即构建，maven输出日志：



```
Started by user Manlan Peng
Building in workspace /root/.jenkins/workspace/docker-jenkins
Fetching changes from the remote Git repository
Checking out Revision 62825b065e8a496cadff651060f6a1757b4000bd (refs/remotes/origin/master)
Commit message: "Go back to the codes!"
[docker-jenkins] $ /usr/local/apache-maven-3.3.9/bin/mvn -f pom.xml clean install
[INFO] Scanning for projects...
[INFO]                                                                         
[INFO] ------------------------------------------------------------------------
[INFO] Building jenkins-docker 0.0.1-SNAPSHOT
[INFO] ------------------------------------------------------------------------
[INFO] 
[INFO] --- maven-clean-plugin:2.5:clean (default-clean) @ jenkins-docker ---
[INFO] Deleting /root/.jenkins/workspace/docker-jenkins/target
[INFO] 
[INFO] --- maven-resources-plugin:2.6:resources (default-resources) @ jenkins-docker ---

[WARNING] Using platform encoding (ANSI_X3.4-1968 actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory /root/.jenkins/workspace/docker-jenkins/src/main/resources
[INFO] 
[INFO] --- maven-compiler-plugin:3.1:compile (default-compile) @ jenkins-docker ---

[INFO] Changes detected - recompiling the module!
[WARNING] File encoding has not been set, using platform encoding ANSI_X3.4-1968, i.e. build is platform dependent!
[INFO] Compiling 1 source file to /root/.jenkins/workspace/docker-jenkins/target/classes

[INFO] 
[INFO] --- maven-resources-plugin:2.6:testResources (default-testResources) @ jenkins-docker ---
[WARNING] Using platform encoding (ANSI_X3.4-1968 actually) to copy filtered resources, i.e. build is platform dependent!
[INFO] skip non existing resourceDirectory /root/.jenkins/workspace/docker-jenkins/src/test/resources
[INFO] 
[INFO] --- maven-compiler-plugin:3.1:testCompile (default-testCompile) @ jenkins-docker ---
[INFO] No sources to compile
[INFO] 
[INFO] --- maven-surefire-plugin:2.12.4:test (default-test) @ jenkins-docker ---
[INFO] No tests to run.
[INFO] 
[INFO] --- maven-war-plugin:2.2:war (default-war) @ jenkins-docker ---

[INFO] Packaging webapp
[INFO] Assembling webapp [jenkins-docker] in [/root/.jenkins/workspace/docker-jenkins/target/jenkins-docker]
[INFO] Processing war project
[INFO] Copying webapp resources [/root/.jenkins/workspace/docker-jenkins/src/main/webapp]
[INFO] Webapp assembled in [70 msecs]
[INFO] Building war: /root/.jenkins/workspace/docker-jenkins/target/jenkins-docker.war
[INFO] WEB-INF/web.xml already added, skipping
[INFO] 
[INFO] --- maven-install-plugin:2.4:install (default-install) @ jenkins-docker ---
[INFO] Installing /root/.jenkins/workspace/docker-jenkins/target/jenkins-docker.war to /root/.m2/repository/com/boonya/jenkins/docker/jenkins-docker/0.0.1-SNAPSHOT/jenkins-docker-0.0.1-SNAPSHOT.war
[INFO] Installing /root/.jenkins/workspace/docker-jenkins/pom.xml to /root/.m2/repository/com/boonya/jenkins/docker/jenkins-docker/0.0.1-SNAPSHOT/jenkins-docker-0.0.1-SNAPSHOT.pom
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------
[INFO] Total time: 7.037 s
[INFO] Finished at: 2017-07-17T09:17:09+00:00

[INFO] Final Memory: 14M/35M
[INFO] ------------------------------------------------------------------------
Deploying /root/.jenkins/workspace/docker-jenkins/target/jenkins-docker.war to container Tomcat 7.x Remote
  [/root/.jenkins/workspace/docker-jenkins/target/jenkins-docker.war] is not deployed. Doing a fresh deployment.
  Deploying [/root/.jenkins/workspace/docker-jenkins/target/jenkins-docker.war]

Finished: SUCCESS
```

上面的Deploying 表示部署日志输出，通过浏览器访问能看到下图：


![](https://img-blog.csdn.net/20170717173052651)

说明应用已实现构建和自动部署功能。


## Jenkins构建和部署流程示意图

![](https://img-blog.csdn.net/20170717180017108)


========================说明======================================

Docker容器：

- jenkins是持续集成管理工具：可以实现代码构建、发布、自动化测试、代码质量等。
- Tomcat用于接收部署程序的war包，独立运行于不同的环境。

源代码管理：
-   常用的代码管理工具Git、GitHub、Subversion。

下一篇内容将着重讲解jenkins与Docker通过SSH持续集成。







