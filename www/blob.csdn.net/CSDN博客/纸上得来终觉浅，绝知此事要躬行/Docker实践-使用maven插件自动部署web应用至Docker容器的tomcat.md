# Docker实践-使用maven插件自动部署web应用至Docker容器的tomcat - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年05月24日 16:46:23[boonya](https://me.csdn.net/boonya)阅读数：1594








此前在一篇文章有讲到将maven项目部署至tomcat：[使用maven的tomcat插件实现webapp的自动部署](http://blog.csdn.net/massivestars/article/details/54318149)


本文就是将 *maven-tomcat-plugins* 和 [Docker](http://lib.csdn.net/base/docker) 结合起来，将web应用部署至Docker容器中正在运行的tomcat。

原文地址：[http://blog.csdn.net/massivestars/article/details/54357293](http://blog.csdn.net/massivestars/article/details/54357293)


# 配置maven

### 在pom.xml加入

```xml
<plugins>
        <plugin>
            <groupId>org.apache.tomcat.maven</groupId>
            <artifactId>tomcat7-maven-plugin</artifactId>
            <version>2.2</version>
            <configuration>
                <url>http://192.168.1.106:8081/manager/text</url>
                <server>tomcat</server>
                <username>deploy</username>
                <password>deploy</password>
            </configuration>
        </plugin>
    </plugins>
```

### 在maven的settings.xml加入

```xml
<server>
      <id>tomcat</id>
      <username>deploy</username>
      <password>deploy</password>
  </server>
```

> 
maven的settings.xml在$MAVEN_HOME/conf目录


# 下载docker的tomcat镜像

### 1、搜索Docker Hub里的tomcat镜像
`docker search tomcat`- 部分搜索结果如下

```
NAME                        DESCRIPTION                                     STARS     OFFICIAL   AUTOMATED
tomcat                      Apache Tomcat is an open source implementa...   1132      [OK]
dordoka/tomcat              Ubuntu 14.04, Oracle JDK 8 and Tomcat 8 ba...   29                   [OK]
cloudesire/tomcat           Tomcat server, 6/7/8                            12                   [OK]
davidcaste/alpine-tomcat    Apache Tomcat 7/8 using Oracle Java 7/8 wi...   11                   [OK]
andreptb/tomcat             Debian Jessie based image with Apache Tomc...   6                    [OK]
```

> 
可以看到，星数最高的是官方的tomcat，有关官方tomcat的镜像可以访问 
[https://hub.docker.com/r/library/tomcat/](https://hub.docker.com/r/library/tomcat/)


![这里写图片描述](https://img-blog.csdn.net/20170112093919424?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTWFzc2l2ZVN0YXJz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上面 “7.0.73-jre7, 7.0-jre7, 7-jre7, 7.0.73, 7.0, 7”等等 是这个tomcat库支持的tag（标签），这里我们选用的是 “7” 这个标签

### 2、拉取Docker Hub里的镜像
`docker pull tomcat:7`
### 3、拉取完成后查看本地的镜像

```bash
docker images #所有镜像
docker image tomcat:7  #查看REPOSITORY为tomcat:7的镜像
```

# 创建个人的Docker镜像

### 创建Dockerfile文件

```
mkdir -p /usr/local/dockerfile/massive
cd /usr/local/dockerfile/massive
touch Dockerfile
```

### 编辑Dockerfile

```
from tomcat:7
MAINTAINER massive
ADD tomcat-users.xml /usr/local/tomcat/conf/
```

> 
*注：tomcat-users.xml可以在tomcat/conf目录找到，拷贝一个到此目录*


### 在tomcat-users.xml加入以下内容

```xml
<role rolename="manager-gui"/>
<role rolename="manager-script"/>
<user username="deploy" password="deploy" roles="manager-gui, manager-script"/>
```

### build这个镜像
`docker build -t massive:tomcat `
# 运行个人定制的Docker镜像
`docker run -p 8081:8080  massive:tomcat`
# 将web应用部署至容器里运行的tomcat
`mvn tomcat7:deploy `
> 
web应用会通过tomcat的部署机制拷贝至容器的 /usr/local/tomcat/webapps/${project} 下，当Docker容器关闭后，容器内的改动不会保存至镜像，也就是说拷贝至容器的web应用会在容器关闭后被删除。




