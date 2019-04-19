# DockerFile - LC900730的博客 - CSDN博客
2017年11月20日 21:36:57[lc900730](https://me.csdn.net/LC900730)阅读数：146标签：[docker](https://so.csdn.net/so/search/s.do?q=docker&t=blog)
个人分类：[docker](https://blog.csdn.net/LC900730/article/category/7160342)
## 格式
1.FROM
```
FROM <image>或者FROM <image>:<tag>
第一条指令必须为FROM指令。如果在同一个Dockerfile中创建多个镜像时，可以使用多个FROM指令
```
2.MAINTAINER
```xml
MAINTAINER <name>，指定维护者信息
```
3.RUN
```
RUN <command>，或者RUN["executable","param1","param2"]
前者在shell终端中执行 即bin/sh -c 
后者使用exec执行，如 RUN  ["/bin/bash","-c","echo hello"]
每条RUN指令将在当前镜像基础上执行指定命令，并提交为新镜像。命令较长时使用\换行。
```
4.CMD
```
CMD ["executable","param1","param2"]使用exec，推荐
CMD command param1,param2在/bin/sh中执行
CMD ["param1","param2"]提供给ENTRYPOINT的默认参数
指定容器启动时候执行的命令，每个Dockerfile只能有一条CMD命令。如果指定多条，那么只有最后一条会被执行。
如果铜壶启动容器时候指定了运行命令，则会覆盖CMD指定的命令
5.EXPOSE
```XML
EXPOSE <port> [<port>...]
如expose 22 80 8443
告诉docker服务器暴露的端口号
启动容器时候需要指定-P(随机映射宿主端口)或者-p(指定端口映射)
<div class="se-preview-section-delimiter"></div>
```
5.EXPOSE
```
EXPOSE <port> [<port>...]
如expose 22 80 8443
告诉docker服务器暴露的端口号
启动容器时候需要指定-P(随机映射宿主端口)或者-p(指定端口映射)
```
```
6.ENV
```
XML 
ENV  value> 
指定环境变量，会被后续RUN指令使用，并在容器运行时候保持 
ENV PG_MAJOR9.3 
RUN curl -sL [http://example.com/postgres-$PG_VERSION.tar.xz|tar](http://example.com/postgres-$PG_VERSION.tar.xz| rel=) -xJc /usr/src/postgress &&… 
ENV PATH /usr/local/postgres-$PG_MAJOR/bin:$PATH
```xml
7.ADD
```XML
ADD <src> <dest>
该命令将复制指定的src到容器中dest，其中src可以是Dockerfile所在目录的一个相对路径(文件或者目录)，也可以是一个url。还可以是一个tar
指定环境变量，会被后续RUN指令使用，并在容器运行时候保持
<div class="se-preview-section-delimiter"></div>
```
7.ADD
```xml
ADD <src> <dest>
该命令将复制指定的src到容器中dest，其中src可以是Dockerfile所在目录的一个相对路径(文件或者目录)，也可以是一个url。还可以是一个tar
指定环境变量，会被后续RUN指令使用，并在容器运行时候保持
```
```xml
8.COPY
```XML
COPY <src> <dest>
复制本地主机的src到容器中dest，路径不存在会自动创建
使用本地目录为源目录时候，推荐COPY
<div class="se-preview-section-delimiter"></div>
```
8.COPY
```xml
COPY <src> <dest>
复制本地主机的src到容器中dest，路径不存在会自动创建
使用本地目录为源目录时候，推荐COPY
```
```
9.ENTRYPOINT
```XML
ENTRYPOINT ["executable","param1","param2"]
ENTRYPOINT command param1 param2(shell中执行)
配置容器启动后执行的命令，并且不可被docker run提供的参数覆盖
每个Dockerfile中只能有一个ENTRYPOINT，指定多个时最后一个生效
<div class="se-preview-section-delimiter"></div>
```
10.VOLUME 
“` 
VOLUME [‘/data’]
```
10.VOLUME
```
XML 
VOLUME [‘/data’] 
创建一个可以从本地或其他容器挂载的挂载点，一般用来存放数据库和需要保持的数据。
```
11.USER
```
XML 
USER  daemon 
指定运行容器的时候用户名或者UID，后续的RUN也会使用指定用户。 
并且可以在之前创建所需要的用户 
RUN groupadd -r postgres && useradd -r -g postgres postgres.。 
临时获取管理员权限使用gosu，而不推荐sudo
```
12.WORKDIR
```XML
WORKDIR  /path/to/workdir
为后续的RUN CMD ENTRYPOINT指令配置工作目录
可以使用多个WORKDIR指令，后续指令如果参数是相对路径，会基于之前命令指定的路径
WORKDIR /a
WORKDIR b
WORKDIR c
RUN pwd
则最终路径是/a/b/c
<div class="se-preview-section-delimiter"></div>
```
12.WORKDIR
```
WORKDIR  /path/to/workdir
为后续的RUN CMD ENTRYPOINT指令配置工作目录
可以使用多个WORKDIR指令，后续指令如果参数是相对路径，会基于之前命令指定的路径
WORKDIR /a
WORKDIR b
WORKDIR c
RUN pwd
则最终路径是/a/b/c
```
```
13.ONBUILD
```XML
ONBUILD  [INSTRUCTION]
配置当所创建的镜像作为其他新创建镜像的基础镜像时候，所执行的操作指令。如Dockerfile使用如下的内容创建了镜像image-A
[...]
ONBUILD ADD . /app/src
ONBUILD RUN  /usr/local/bin/python-build --dir /app/src
[...]
如果基于image-A指定新镜像，新的Dockerfile使用FROM image-A指定基础镜像时，会自动执行ONBUILD指令内容，相当于在后面添加了两条指令。
FROM image-A
ADD . /app/src
RUN  /usr/local/bin/python-build --dir /app/src
[...]
```
## 创建镜像
docker  buil [选项] 路径。可以使用-t指定标签
