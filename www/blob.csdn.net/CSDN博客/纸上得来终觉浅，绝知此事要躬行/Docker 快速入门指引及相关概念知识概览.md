# Docker 快速入门指引及相关概念知识概览 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年07月06日 13:54:24[boonya](https://me.csdn.net/boonya)阅读数：369标签：[docker](https://so.csdn.net/so/search/s.do?q=docker&t=blog)
个人分类：[Docker](https://blog.csdn.net/boonya/article/category/6559498)









原文地址：[https://yuanxuxu.com/2016/06/14/docker-kuai-su-ru-men-zhi-yin/](https://yuanxuxu.com/2016/06/14/docker-kuai-su-ru-men-zhi-yin/)


## Docker是什么？
Docker是 Docker.Inc 公司开源的一个基于 LXC技术之上构建的Container容器引擎，基于Go语言并遵从Apache2.0协议开源。
![](https://img-blog.csdn.net/20170706133544072)

开发者可以搭建他们的应用仅仅一次，就能保证让这个应用保持一致的跑在任何地方。运营人员可以将他们的服务器配置一遍，就能跑任何应用。

### What is Docker’s architecture?
> Docker uses a client-server architecture. The Docker client talks to the Dockerdaemon, which does the heavy lifting of building, running, and distributing your Docker containers. Both the Docker client and the daemon can run on the same system, or you can connect a Docker client to a remote Docker daemon. The Docker client and daemon communicate via sockets or through a RESTful API.




![](https://img-blog.csdn.net/20170706133617790)




## 和传统虚拟主机的区别
传统虚拟机通过硬件虚拟化来创造一整个虚拟系统。每一个虚拟机内的应用不仅仅包含这个应用的一些类库代码,而且还包含一整个操作系统。
![](https://img-blog.csdn.net/20170706133711130)

Docker所有的容器分享一个操作系统，他们显然会比虚拟机更小一些，使得他们可以存在100多个虚拟的系统在一个主机上（而不像一个严格限制数量的虚拟机）。

## 优点和特点

-     标准化应用发布，Docker容器包含了运行环境和可执行程序，可以跨平台和主机使用；
-     快速部署和启动，VM启动一般是分钟级，Docker容器启动是秒级，即启即用；
-     方便构建基于SOA架构或微服务架构的系统，通过服务编排，更好的松耦合；
-     轻量低成本，占有更少的磁盘空间，一台主机可以启动上千个容器；
-     方便持续集成，通过与代码进行关联使持续集成非常方便；
-     安全隔离的执行环境，每个运行的容器互不影响；
-         文件系统分离，每一个进程容器跑在完全分离的root权限的文件系统下；
-         资源分离，系统资源（像CPU、内存）能被指定的分配给每一个进程容器，使用Cgroups；
-         网络分离，使用一个虚拟的接口和IP地址，每一个进程容器跑在它自己的网络命名空间；
-     丰富的镜像资源，用户可以方便的在此基础上构建自己的容器运行；
    ......



## 一些概念和名称

-     Docker Client 是用户界面，它支持用户与Docker Daemon之间通信；
-     Docker Daemon Docker最核心的后台进程，运行于主机上，处理服务请求；
-     Docker Index是中央registry，支持拥有公有与私有访问权限的Docker容器镜像的备份；
-     Docker Containers负责应用程序的运行，包括操作系统、用户添加的文件以及元数据；
-     Docker Images是一个只读模板，用来运行Docker容器；
-     DockerFile是文件指令集，用来说明如何自动创建Docker镜像。


## 安装
-     现在已经支持Windows、Mac OS X、以及大多数Linux版本 -> Install Docker Engine
-     DaoCloud的安装脚本


```
#Ubuntu
curl -sSL https://get.daocloud.io/docker | sh
#CentOS
curl -sSL https://get.daocloud.io/docker | sh
sudo chkconfig docker on
sudo systemctl start docker
```


## 仓库
仓库（Repository）是集中存放镜像文件的场所，仓库注册服务器（Registry）上往往存放着多个仓库，每个仓库中又包含了多个镜像，每个镜像有不同的标签（tag），和仓库（Repository）严格来讲不是同一个概念。

仓库分为公开仓库（Public）和私有仓库（Private），官方的[Docker Hub](https://hub.docker.com/)提供大量镜像提供下载，但是访问非常的慢，可以使用国内的公开仓库[时速云](https://hub.tenxcloud.com/) 、[网易云](https://c.163.com/hub) 、[阿里云](https://dev.aliyun.com/search.html?spm=5176.775974865.0.0.Blezik) 、DaoCloud加速器等。

用户也可以在本地网络内创建一个私有仓库，创建了自己的镜像之后就可以使用 push 命令将它上传到公有或者私有仓库，其他机器上使用只需要从仓库上 pull 下来就可以了。
镜像
镜像就是一堆文件的集合，并不是像VM那样的是一个操作系统。镜像可以简单到只有一个程序文件。如果你写一个hello world静态编译后放到一个空的Image中，那么整个image的大小，就是你编译后的二进制文件的大小。一个ubuntu:14.04的镜像，提供了一个基本的ubuntu:14.04的发行版，镜像是不包含操作系统Linux内核。

如果在Debian镜像中安装MySQL 5.6，就成了mysql:5.6镜像。底层一个Debian操作系统镜像，上面叠加一个 MySQL层，就完成了一个MySQL镜像的构建。

Dockerfile构建出Docker镜像，通过Docker镜像运行Docker容器。Docker镜像是Docker容器运行的基础，没有Docker镜像，就不可能有Docker容器，这也是Docker的设计原则之一。

## 常用命令

-     docker images 显示本地已有镜像；
-     docker info 显示docker系统信息；
-     docker commit -m -a 提交更新后的镜像；
-     docker build 通过Dockerfile来构建镜像；
-     docker import 本地导入镜像；
-     docker search 查找仓库中镜像；
-     docker push 将镜像推送到仓库；
-     docker pull 将仓库中镜像下载到本地；
-     docker save -o mysql_5.6.tar mysql:5.6 导出镜像到本地；
-     docker load < mysql_5.6.tar 载入镜像；
-     docker rmi 移除镜像；
-     docker attach 运行中容器的stdin，进行命令执行的动作；
-     docker history 显示镜像的历史；



## Dockerfile
Dockerfile是Docker用来构建镜像的文本文件，包含自定义的指令和格式, 可以通过docker build命令从Dockerfile中构建镜像。
![](https://img-blog.csdn.net/20170706134258137)

**    FROM**

    FROM <image>[:<tag>]
    设置要制作的镜像基于哪个镜像，FROM指令必须是整个Dockerfile的第一个指令，如果指定的镜像不存在默认会自动从Docker Hub上下载。

** MAINTAINER**

    MAINTAINER <name>
    MAINTAINER指令允许你给将要制作的镜像设置作者信息

**   RUN**

    RUN <command>        #将会调用/bin/sh -c <command>
    RUN ["executable", "param1", "param2"]   #将会调用exec执行，以避免有些时候shell方式执行时的传递参数问题，而且有些基础镜像可能不包含/bin/sh
    RUN指令会在一个新的容器中执行任何命令，然后把执行后的改变提交到当前镜像，提交后的镜像会被用于Dockerfile中定义的下一步操作，RUN中定义的命令会按顺序执行并提交，这正是Docker廉价的提交和可以基于镜像的任何一个历史点创建容器的好处，就像版本控制工具一样。

** CMD**

    CMD ["executable", "param1", "param2"]    #将会调用exec执行，首选方式
    CMD ["param1", "param2"]        #当使用ENTRYPOINT指令时，为该指令传递默认参数
    CMD <command> [ <param1>|<param2> ]        #将会调用/bin/sh -c执行
    CMD指令中指定的命令会在镜像运行时执行，在Dockerfile中只能存在一个，如果使用了多个CMD指令，则只有最后一个CMD指令有效。
    当出现ENTRYPOINT指令时，CMD中定义的内容会作为ENTRYPOINT指令的默认参数，也就是说可以使用CMD指令给ENTRYPOINT传递参数。
    RUN和CMD都是执行命令，他们的差异在于RUN中定义的命令会在执行docker build命令创建镜像时执行，而CMD中定义的命令会在执行docker run命令运行镜像时执行，另外使用第一种语法也就是调用exec执行时，命令必须为绝对路径。

**   EXPOSE**
    EXPOSE <port> [ ...]
    EXPOSE指令用来告诉Docker这个容器在运行时会监听哪些端口，Docker在连接不同的容器(使用–link参数)时使用这些信息。

** ENV**

    ENV <key> <value>
    ENV指令用于设置环境变量，在Dockerfile中这些设置的环境变量也会影响到RUN指令，当运行生成的镜像时这些环境变量依然有效，如果需要在运行时更改这些环境变量可以在运行docker run时添加–env <key>=<value>参数来修改。
    最好不要定义那些可能和系统预定义的环境变量冲突的名字，否则可能会产生意想不到的结果。

**    ADD**

    ADD <src> <dest>
    ADD指令用于从指定路径拷贝一个文件或目录到容器的指定路径中，<src>是一个文件或目录的路径，也可以是一个url，路径是相对于该Dockerfile文件所在位置的相对路径，<dest>是目标容器的一个绝对路径
    例如/home/yooke/Docker/Dockerfile这个文件中定义的，那么ADD /data.txt /db/指令将会尝试拷贝文件从/home/yooke/Docker/data.txt到将要生成的容器的/db/data.txt，且文件或目录的属组和属主分别为uid和gid为0的用户和组，如果是通过url方式获取的文件，则权限是600。
    注意： ①如果执行docker build – < somefile即通过标准输入来创建时，ADD指令只支持url方式，另外如果url需要认证，则可以通过RUN wget …或RUN curl …来完成，ADD指令不支持认证。
    ②<src>路径必须与Dockerfile在同级目录或子目录中，例如不能使用ADD ../somepath，因为在执行docker build时首先做的就是把Dockerfile所在目录包含子目录发送给docker的守护进程。
    ③如果<src>是一个url且<dest>不是以”/“结尾，则会下载文件并重命名为<dest>。
    ④如果<src>是一个url且<dest>以“/”结尾，则会下载文件到<dest>/<filename>，url必须是一个正常的路径形式，“http://example.com”像这样的url是不能正常工作的。
    ⑤如果<src>是一个本地的压缩包且<dest>是以“/”结尾的目录，则会调用“tar -x”命令解压缩，如果<dest>有同名文件则覆盖，但<src>是一个url时不会执行解压缩。

**COPY**

    COPY <src> <dest>
    用法与ADD相同，<src>不支持使用url

**    ENTRYPOINT**
    ENTRYPOINT ["executable", "param1", "param2"]        #将会调用exec执行，首选方式
    ENTRYPOINT command param1 param2             #将会调用/bin/sh -c执行
    ENTRYPOINT指令中指定的命令会在镜像运行时执行，在Dockerfile中只能存在一个，如果使用了多个则只有最后一个指令有效。
    ENTRYPOINT指令中指定的命令(exec执行的方式)可以通过docker run来传递参数，例如docker run <images> -l启动的容器将会把-l参数传递给ENTRYPOINT指令定义的命令并会覆盖CMD指令中定义的默认参数(如果有的话)，但不会覆盖该指令定义的参数，例如ENTRYPOINT ["ls","-a"]，CMD ["/etc"],当通过docker run <image>启动容器时该容器会运行ls -a /etc命令，当使用docker run <image> -l启动时该容器会运行ls -a -l命令，-l参数会覆盖CMD指令中定义的/etc参数。
    当使用ENTRYPOINT指令时生成的镜像运行时只会执行该指令指定的命令。
    当出现ENTRYPOINT指令时CMD指令只可能(当ENTRYPOINT指令使用exec方式执行时)被当做ENTRYPOINT指令的参数使用，其他情况则会被忽略。

**    VOLUME**

    VOLUME ["samepath"]
    VOLUME指令用来设置一个挂载点，可以用来让其他容器挂载以实现数据共享或对容器数据的备份、恢复或迁移。

**    USER**

    USER [username|uid]
    USER指令用于设置用户或uid来运行生成的镜像和执行RUN指令。

**    WORKDIR**

    WORKDIR /path/to/workdir
    WORKDIR指令用于设置Dockerfile中的RUN、CMD和ENTRYPOINT指令执行命令的工作目录(默认为/目录)，该指令在Dockerfile文件中可以出现多次，如果使用相对路径则为相对于WORKDIR上一次的值，例如WORKDIR /data，WORKDIR logs，RUN pwd最终输出的当前目录是/data/logs。
**    ONBUILD**

    ONBUILD [INSTRUCTION]
    ONBUILD指令用来设置一些触发的指令，用于在当该镜像被作为基础镜像来创建其他镜像时(也就是Dockerfile中的FROM为当前镜像时)执行一些操作，ONBUILD中定义的指令会在用于生成其他镜像的Dockerfile文件的FROM指令之后被执行，上述介绍的任何一个指令都可以用于ONBUILD指令，可以用来执行一些因为环境而变化的操作，使镜像更加通用。
    注意： ①ONBUILD中定义的指令在当前镜像的build中不会被执行。
    ②可以通过查看docker inspeat <image>命令执行结果的OnBuild键来查看某个镜像ONBUILD指令定义的内容。
    ③ONBUILD中定义的指令会当做引用该镜像的Dockerfile文件的FROM指令的一部分来执行，执行顺序会按ONBUILD定义的先后顺序执行，如果ONBUILD中定义的任何一个指令运行失败，则会使FROM指令中断并导致整个build失败，当所有的ONBUILD中定义的指令成功完成后，会按正常顺序继续执行build。
    ④ONBUILD中定义的指令不会继承到当前引用的镜像中，也就是当引用ONBUILD的镜像创建完成后将会清除所有引用的ONBUILD指令。
    ⑤ONBUILD指令不允许嵌套，例如ONBUILD ONBUILD ADD . /data是不允许的。
    ⑥ONBUILD指令不会执行其定义的FROM或MAINTAINER指令。



## 容器
容器 = 镜像 + 可读层

Docker容器就像是一个文件夹，它包含了一个应用程序运行所需要的所有内容。每个容器都是基于Docker镜像构建。我们可以运行、开始、停止、迁移或者是删除Docker容器。每个容器均是一个隔离的、安全的应用平台。Docker容器是Docker的运行组件。

### 常用命令

-     docker run 新建并启动容器
        -d 容器运行在后台，此时不能使用--rm选项
        -i -t 和容器进行交互式操作
        --name 命名容器，没有该参数Docker deamon会生产UUID来标识
        --cidfile 将容器ID输入到指定文件中
        --add-host 添加一行到/etc/hosts
        --mac-address 设置MAC地址
        --dns 覆盖容器DNS设置
        --rm 退出容器时自动清除数据
        -m 调整容器的内存使用
        -c 调整容器的CPU优先级
        -e 设定环境变量
        --expose 运行时暴露端口，不创建和宿主机的映射
        -p 创建映射规则，将一个或者一组端口从容器里绑定到宿主机上,可多次使用
        ip:hostPort:containerPort
        ip::containerPort
        hostPort:containerPort
        containerPort
        -P 将Dockfile中暴露的端口映射动态映射到宿主机
        --link 容器互联 --link name:alias
        -v 创建数据卷挂载到容器，一次run中可多次使用
        可覆盖Dockfile参数
        CMD
        ENTRYPOINT
        EXPOSE
        ENV
        VOLUME
        USER
        WORKDIR
-     docker stop 停止运行中容器
-     docker stop $(docker ps -qa) 停止所有运行中的容器
-     docker restart 重启容器
-     docker ps -a 查看所有容器
-     docker rm 移除处于终止状态的容器
                       * `docker rm $(docker ps -qa)` 移除处于终止状态的容器
-     docker logs 从容器中去日志
-     docker diff 列出容器中被改变的文件或者目录
-     docker top 显示运行容器的进程信息
-     docker cp 从容器中拷贝文件或者目录到本地
-     docker inspect 查看容器详细信息



### Volume和Volume Containers
数据卷类似于 Linux 下对目录或文件进行 mount，本质是容器中一个特殊的文件或目录(挂载点)。在容器的创建过程中，这个挂载点会被挂载一个宿主机上的指定的目录 (一个以volumeID为名称的目录 或者指定的宿主机目录)。它的设计用来持久化数据的，生命周期独立于容器。

-     docker run -i -t -v /data debian:jessie /bin/sh
    将宿主的/var/lib/docker/volumes/volume_id/_data 绑定挂载到 /data
-     docker run -i -t -v /var/www/:/data debian:jessie /bin/sh
    ​将宿主机的/var/www/ 绑定挂载到 /data
-     docker run -i -t -v /var/www/:/data:ro debian:jessie /bin/sh
    ​挂载为只读


数据卷容器，用来提供数据卷供其它容器挂载的正常容器。

-     创建一个数据卷容器dbdata

    docker run -d -v /var/www/:/dbdata --name dbdata debian:jessie

-     其他容器挂载 dbdata容器中的数据卷

    docker run -d --volumes-from dbdata --name db1 debian:jessie

-     也可以从其他已经挂载了数据卷的容器来级联挂载数据卷

    docker run -d --name db2 --volumes-from db1 debian:jessie


如果删除了挂载的容器，数据卷并不会被自动删除。如果要删除一个数据卷，必须在删除最后一个还挂载着它的容器时使用 docker rm -v 命令来指定同时删除关联的容器。

-     备份数据卷，使用了 tar 命令来将 dbdata 卷备份为容器中 /backup/backup.tar 文件，也就是主机当前目录下的名为 backup.tar 的文件。

`docker run --volumes-from dbdata -v $(pwd):/backup debian:jessie tar cvf /backup/backup.tar /dbdata`-     恢复数据卷


```
# 创建一个带有空数据卷的容器
    docker run -v /dbdata --name dbdata2 debian:jessie /bin/bash
    创建另一个容器，挂载 dbdata2 容器卷中的数据卷，并解压备份文件到挂载的容器卷中。
    docker run --volumes-from dbdata2 -v $(pwd):/backup debian:jessie tar xvf /backup/backup.tar
    再启动一个容器挂载同样的容器卷来查看恢复的数据
    docker run --volumes-from dbdata2 debian:jessie /bin/ls /dbdata
```


## 参考
[Docker Docs](https://docs.docker.com/)[Docker命令](http://www.runoob.com/docker/docker-command-manual.html)

[Docker入门与实战](https://www.gitbook.com/star/book/hujb2000/docker-flow-evolution)



