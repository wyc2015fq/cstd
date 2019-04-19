# Docker入门系列6 如何打开多个终端进入Docker容器 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年03月09日 12:56:56[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：3046
Docker容器运行后，如何进入容器进行操作呢？起初我是用SSH。如果只启动一个容器，用SSH还能应付，只需要将容器的22端口映射到本机的一个端口即可。当我启动了五个容器后，每个容器默认是没有配置SSH Server的，安装配置SSHD，映射容器SSH端口，实在是麻烦。 我发现很多Docker镜像都是没有安装SSHD服务的，难道有其他方法进入Docker容器？
有很多种方法，包括使用 docker attach 命令或 nsenter 工具等。
使用 attach 命令有时候并不方便。当多个窗口同时 attach 到同一个容器的时候，所有窗口都会同步显示。
nsenter 可以访问另一个进程的名字空间。
为了连接到容器，你还需要找到容器的第一个进程的 PID，可以通过下面的命令获取。
PID=$(docker inspect --format "{{ .State.Pid }}" <container>) //将<container>换成你的容器id
通过这个 PID，就可以连接到这个容器：
$ nsenter --target $PID --mount --uts --ipc --net --pid
更简单的，建议大家下载[.bashrc_docker](https://github.com/yeasy/docker_practice/raw/master/_local/.bashrc_docker)，并将内容放到
 .bashrc 中。
$ wget -P ~ [https://github.com/yeasy/docker_practice/raw/master/_local/.bashrc_docker](https://github.com/yeasy/docker_practice/raw/master/_local/.bashrc_docker);
$ echo "[ -f ~/.bashrc_docker ] && . ~/.bashrc_docker" >> ~/.bashrc; source ~/.bashrc
这个文件中定义了很多方便使用 Docker 的命令，例如 docker-pid 可以获取某个容器的 PID；而 docker-enter 可以进入容器或直接在容器内执行命令。
$ echo $(docker-pid <container>)
$ docker-enter <container>
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%976:%E5%A6%82%E4%BD%95%E6%89%93%E5%BC%80%E5%A4%9A%E4%B8%AA%E7%BB%88%E7%AB%AF%E8%BF%9B%E5%85%A5Docker%E5%AE%B9%E5%99%A8&action=edit&section=1)]附
.bashrc_docker文件内容：
*# Some useful commands to use docker.**# Author: yeasy@github**# Created:2014-09-25***alias** docker-pid="sudo docker inspect --format '{{.State.Pid}}'"**alias** docker-ip="sudo docker inspect --format '{{ .NetworkSettings.IPAddress }}'"*#the implementation refs from https://github.com/jpetazzo/nsenter/blob/master/docker-enter***function** docker-enter**(****)****{****if****[**-e $**(****dirname**"$0"**)****/**nsenter **]**; **then***# with boot2docker, nsenter is not in the PATH but it is in the same folder*NSENTER=$**(****dirname**"$0"**)****/**nsenter
    **else**NSENTER=nsenter
    **fi****[**-z"$NSENTER"**]****&&****echo**"WARN Cannot find nsenter"**&&****return****if****[**-z"$1"**]**; **then****echo**"Usage: `basename "$0"` CONTAINER [COMMAND [ARG]...]"**echo**""**echo**"Enters the Docker CONTAINER and executes the specified COMMAND."**echo**"If COMMAND is not specified, runs an interactive shell in CONTAINER."**else**PID=$**(****sudo** docker inspect --format"{{.State.Pid}}""$1"**)****if****[**-z"$PID"**]**; **then****echo**"WARN Cannot find the given container"**return****fi****shift**OPTS="--target $PID --mount --uts --ipc --net --pid"**if****[**-z"$1"**]**; **then***# No command given.**# Use su to clear all host environment variables except for TERM,**# initialize the environment variables HOME, SHELL, USER, LOGNAME, PATH,**# and start a login shell.**#sudo $NSENTER "$OPTS" su - root***sudo**$NSENTER--target$PID--mount--uts--ipc--net--pid**su** - root
        **else***# Use env to clear all host environment variables.***sudo**$NSENTER--target$PID--mount--uts--ipc--net--pid**env**-i $**@****fi****fi****}**
## 艺搜参考
[http://www.hjue.me/post/docker-nsenter](http://www.hjue.me/post/docker-nsenter)
[http://yeasy.gitbooks.io/docker_practice/content/container/enter.html](http://yeasy.gitbooks.io/docker_practice/content/container/enter.html)
