# Docker实践-安装Docker并在容器里运行tomcat - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年05月24日 16:40:48[boonya](https://me.csdn.net/boonya)阅读数：3317








随着[微服务](http://lib.csdn.net/base/microservice)的流行，[Docker](http://lib.csdn.net/base/docker)越来越流行，正如它的理念”Build, Ship, and Run Any App, Anywhere”一样，Docker提供的容器隔离技术使得开发人员不用再去理清server里的各种环境配置，轻松把应用运行起来。我们只需把运行环境的配置和应用封装在Docker的镜像(image)，然后使用Docker运行这个镜像即可。Docker可以说是给所有开发人员的一个福利包，学习和使用Docker是所有开发人员的标配技能。
文章来源：[http://blog.csdn.net/massivestars/article/details/54357293](http://blog.csdn.net/massivestars/article/details/54357293)


# 安装Docker
`yum install docker`
> 
本文使用的系统是centos7,ubuntu使用以下命令 
  sudo apt-get update 
  sudo apt-get install docker-engine


如果报了以下错误，是因为yum被其它进程使用了

```
Another app is currently holding the yum lock; waiting for it to exit...
  The other application is: PackageKit
    Memory :  12 M RSS (924 MB VSZ)
    Started: Mon Jan  2 17:22:13 2017 - 1 day(s) 1:06:13 ago
    State  : Sleeping, pid: 16208
```

查看正在yum使用的进程
`ps -ef|grep yum`
kill掉它即可
`kill -9 16208`
安装完成，查看安装是否成功

```
docker info        #查看docker的情况 
docker --version   #查看docker的版本
```

# 启动Docker服务
`service docker start`
# 启动Docker的hello-world

### 从Docker Hub下载一个hello-world镜像
`docker pull hello-world`
### 运行hello-world镜像
`docker run hello-world`
### 输出以下信息

```
Hello from Docker!
This message shows that your installation appears to be working correctly.

To generate this message, Docker took the following steps:
 1. The Docker client contacted the Docker daemon.
 2. The Docker daemon pulled the "hello-world" image from the Docker Hub.
 3. The Docker daemon created a new container from that image which runs the
    executable that produces the output you are currently reading.
 4. The Docker daemon streamed that output to the Docker client, which sent it
    to your terminal.
```

至此，我们已成功运行起第一个Docker容器

# tomcat运行环境

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

> 可以看到，星数最高的是官方的tomcat，有关官方tomcat的镜像可以访问 

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

### 4、运行tomcat镜像
`docker run tomcat:7`
> 
可以访问 [http://ip:8080](http://ip:8080) 确认容器的tomcat已启动成功

- 使用以下命令来查看正在运行的容器
`docker ps`- 若端口被占用，可以指定容器和主机的映射端口
`docker run -p 8081:8080 tomcat:7 `- 启动后，访问地址是[http://ip:8081](http://ip:8081)

### 5、运行我们的web应用

假设我们应用是www,目录位置在/app/deploy/www
`docker run --privileged=true -v /app/deploy/www:/usr/local/tomcat/webapps/www  -p 8081:8080 tomcat:7 `
> 
-v /app/deploy/www:/usr/local/tomcat/webapps/www 是把/app/deploy/www的目录挂载至容器的/usr/local/tomcat/webapps/www。 
  –privileged=true是授予docker挂载的权限


至此，已成功把web应用部署在Docker容器运行

# 常用命令

```
# 查看所有镜像
docker images

# 正在运行容器
docker ps

# 查看docker容器
docker ps -a

# 启动tomcat:7镜像
docker run -p 8080:8080 tomcat:7

# 以后台守护进程的方式启动
docker run -d tomcat:7

# 停止一个容器
docker stop b840db1d182b

# 进入一个容器
docker attach d48b21a7e439

# 进入正在运行容器并以命令行交互
docker exec -it e9410ee182bd /bin/sh

# 以交互的方式运行
docker run -i -t -p 8081:8080 tomcat:7 /bin/bash
```




