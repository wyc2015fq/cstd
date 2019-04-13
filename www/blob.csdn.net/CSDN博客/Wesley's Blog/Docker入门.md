
# Docker入门 - Wesley's Blog - CSDN博客


2018年08月03日 12:01:06[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：55标签：[Docker																](https://so.csdn.net/so/search/s.do?q=Docker&t=blog)个人分类：[云技术																](https://blog.csdn.net/yinanmo5569/article/category/7808067)



# Introduction
Docker 是一个开源的应用容器引擎，让开发者可以打包他们的应用以及依赖包到一个可移植的容器中，然后发布到任何流行的 Linux 机器上，也可以实现虚拟化。容器是完全使用沙箱机制，相互之间不会有任何接口
# Docker 组成
一个完整的Docker有以下几个部分组成：
docker Client客户端
Docker Daemon守护进程
Docker Image镜像
Docker Container容器
# Docker Hello World
通过docker run来在容器中运行一个应用程序。
`runoob@runoob:~$ docker run ubuntu:15.10 /bin/echo "Hello world"
Hello world`其中：
ubuntu:15.10为指定的要运行的镜像
/bin/echo “Hello world”为要运行的指令
通过`-t -i`让docker运行的容器实现对话能力
`runoob@runoob:~$ docker run -i -t ubuntu:15.10 /bin/bash
root@dc0050c79503:/#`参数解析：
-t:在新容器内指定一个伪终端或终端。
-i:允许你对容器内的标准输入 (STDIN) 进行交互。
-d:后台模式启动
-P:将容器内部使用的网络端口映射到我们使用的主机上。
其他重要指令：
`~$ docker ps
~$ docker logs <Container ID or NAMES>
~$ docker stop <Container ID or NAMES>
~$ docker start <Container ID or NAMES>
~$ docker rm <Container ID or NAMES>
~$ docker port <Container ID or NAMES>
~$ docker top <Container ID or NAMES>
~$ docker inspect <Container ID or NAMES>`
# Docker 镜像使用
docker镜像分为本地镜像和远端镜像（Docker Hub）
通过`docker images`列出本地镜像
`runoob@runoob:~$ docker images           
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
ubuntu              14.04               90d5884b1ee0        5 days ago          188 MB
php                 5.6                 f40e9e0f10c8        9 days ago          444.8 MB`通过`docker pull`获取新镜像：
`Crunoob@runoob:~$ docker pull ubuntu:13.10
13.10: Pulling from library/ubuntu`通过`docker search`查找镜像：
runoob@runoob:~$  docker search httpd
通过Dockerfile创建镜像：[Dockerfile定制镜像](https://yeasy.gitbooks.io/docker_practice/image/build.html)
通过`docker commit`提交容器副本,作为镜像
`runoob@runoob:~$ docker commit -m="has update" -a="runoob" e218edb10161 runoob/ubuntu:v2
sha256:70bf1840fd7c0d2d8ef0a42a817eb29f854c1af8f7c59fc03ac7bdee9545aff8`通过`docker tag`设置容器标签
`runoob@runoob:~$ docker tag 860c279d2fec runoob/centos:dev`
# Docker 容器连接
`-P -p`网络端口映射
-P :是容器内部端口随机映射到主机的高端口。
-p : 是容器内部端口绑定到指定的主机端口。
runoob@runoob:~$ docker run -d -P training/webapp python app.py
fce072cc88cee71b1cdceb57c2821d054a4a59f67da6b416fceb5593f059fc6d
runoob@runoob:~$ docker run -d -p 5000:5000 training/webapp python app.py
33e4523d30aaf0258915c368e66e03b49535de0ef20317d3f639d40222ba6bc0
# Docker 导入导出
使用`docker export`导出
`$ docker export 7691a814370e > ubuntu.tar`使用`docker import`导入
`$ cat ubuntu.tar | docker import - test/ubuntu:v1.0`注：用户既可以使用`docker load`来导入镜像存储文件到本地镜像库，也可以使用`docker import`来导入一个容器快照到本地镜像库。这两者的区别在于容器快照文件将丢弃所有的历史记录和元数据信息（即仅保存容器当时的快照状态），而镜像存储文件将保存完整记录，体积也要大。此外，从容器快照文件导入时可以重新指定标签等元数据信息。
# Docker 镜像制作
利用`Dockerfile`来制作镜像， 举个例子：
`FROM debian:jessie
RUN buildDeps='gcc libc6-dev make' \
    && apt-get update \
    && apt-get install -y $buildDeps \
    && wget -O redis.tar.gz "http://download.redis.io/releases/redis-3.2.5.tar.gz" \
    && mkdir -p /usr/src/redis \
    && tar -xzf redis.tar.gz -C /usr/src/redis --strip-components=1 \
    && make -C /usr/src/redis \
    && make -C /usr/src/redis install \
    && rm -rf /var/lib/apt/lists/* \
    && rm redis.tar.gz \
    && rm -r /usr/src/redis \
    && apt-get purge -y --auto-remove $buildDeps`然后在`Dockerfile`所在的目录执行：
`$ docker build -t nginx:v3 .
Sending build context to Docker daemon 2.048 kB
Step 1 : FROM nginx
 ---> e43d811ce2f4
Step 2 : RUN echo '<h1>Hello, Docker!</h1>' > /usr/share/nginx/html/index.html
 ---> Running in 9cdc27646c7b
 ---> 44aa4490ce2c
Removing intermediate container 9cdc27646c7b
Successfully built 44aa4490ce2c`其中`.`指的是**上下文**，会把上下文的内容都传给docker engine。
另外，还可以直接使用Git Repo构建：
`$ docker build https://github.com/twang2218/gitlab-ce-zh.git#:8.14`默认指定`master`分支，构建目录为`/8.14/`
关于Dockerfile编写，具体可以参考博文：[Dockerfile编写](https://yeasy.gitbooks.io/docker_practice/image/dockerfile/copy.html)
主要有一下指令:
RUN: 最基本的指令。注意，每一个RUN指令的执行环境根本不同，前一条RUN指令不会影响后一条RUN指令。所以单纯的RUN cd /app是没有用的，这是对Dockerfile构建分层存储的概念不了解导致的错误。
COPY: COPY 指令将从构建上下文目录中 <源路径> 的文件/目录复制到新的一层的镜像内的 <目标路径> 位置。
ADD: 与COPY类似，推荐使用COPY，ADD对于自动解压缩比较有用，如 zip, gzip, tar.gz 等文件
CMD: CMD与RUN类似，CMD指令就是用于指定默认的容器主进程的启动命令的。推荐使用格式：CMD [“可执行文件”, “参数1”, “参数2”…]。另外，Docker 不是虚拟机，容器中的应用都应该以前台执行，而不是像虚拟机、物理机里面那样，用upstart/systemd去启动后台服务，容器内没有后台服务的概念。
ENV: 设置环境变量，ENV <key1>=<value1> <key2>=<value2>..，环境变量也可以在Dockerfile的ADD, RUN, COPY等命令中展开
VOLUME:定义匿名卷，VOLUME ["<路径1>", "<路径2>"...]，容器运行时应该尽量保持容器存储层不发生写操作，在运行时再通过 -v mydata:/data 来将命名挂在卷挂载到dockerfile中定义的匿名的挂载位置
EXPOSE: 声明运行时容器提供服务端口。没有实际用途，单纯方便镜像使用者理解。还是需要通过运行时-p, -P来打开端口
WORKDIR: 用来指定工作目录，如果需要改变各层的工作目录位置，就是用这条指令。
USER: 指定当前用户，和WORKDIR类似，改变各层工作状态
HEALTHCHECK: 检查容器是否健康，进入死锁状态。与CMD, ENTRYPOINT一样，只能出现一次。
ONBUILD: 用于制作基础镜像的时候使用。作为基础镜像的时候，ONBUILD的语句会被忽略
# Docker 网络
外部访问容器，通过`-p`,`-P`来实现端口映射
`$ docker run -d -P training/webapp python app.py
$ docker container ls -l
CONTAINER ID  IMAGE                   COMMAND       CREATED        STATUS        PORTS                    NAMES
bc533791f3f5  training/webapp:latest  python app.py 5 seconds ago  Up 2 seconds  0.0.0.0:49155->5000/tcp  nostalgic_morse`映射到指定地址的指定端口，`ip:hostPort:containerPort`
`$ docker run -d -p 127.0.0.1:5000:5000 training/webapp python app.py`映射到指定地址的任意端口
`$ docker run -d -p 127.0.0.1::5000 training/webapp python app.py`查看映射端口配置
`$ docker port nostalgic_morse 5000
127.0.0.1:49155.`
# 常用命令最佳实践
在后台创建容器，并且指定名字，注意端口需要在最开始的时候打开：
`# docker run -tid --name Operation -p 8000:8000 operationtrident`查看还在运行着的容器：
`# docker ps // -a 查看所有，包括死了的`登陆容器：
`# docker exec -ti Operation bash`容器主机间复制文件：
`# docker cp Dockerfile Operation:/`删除所有未运行的容器（已经挂掉的）：
`# sudo docker rm $(sudo docker ps -a -q)`制作容器，并指定tag：
`# docker build -t operationtrident:1.0 https://github.com/ZezhongWang/OperationTrident-BackEnd.git`

