# docker 入门准备step1 - 三少GG - CSDN博客
2015年09月05日 23:07:36[三少GG](https://me.csdn.net/scut1135)阅读数：915
## 更改存储路径解决方案20170501：    [http://www.jianshu.com/p/e98ef6d6cfb4](http://www.jianshu.com/p/e98ef6d6cfb4)
`**sudo service docker stop**`
然后移动整个`/var/lib/docker`目录到目的路径：
`sudo mv /var/lib/docker /data/dockerpath```
## Docker 配置文件位置Ｄocker 的配置文件可以设置大部分的后台进程参数，在各个操作系统中的存放位置不一致在 ubuntu 中的位置是：/etc/default/docker
## 在 centos 中的位置是：/etc/sysconfig/docker
```
DOCKER_OPTS="-g /data/dockerpath"
```
最后重新启动，
`**sudo service docker start**`
Docker 的路径就改成 /data/dockerpath 了。

*******************
1.进入root权限
sudo su
2.停止所有的container，这样才能够删除其中的images：
docker stop $(docker ps -a -q)
如果想要删除所有container的话再加一个指令：
docker rm $(docker ps -a -q)
3.查看当前有些什么images
docker images
4.删除images，通过image的id来指定删除谁
docker rmi <image id>
想要删除untagged images，也就是那些id为<None>的image的话可以用
docker rmi $(docker images | grep "^<none>" | awk "{print $2}")
要删除全部image的话
docker rmi $(docker images -q)
【非常优秀的文章：[http://www.open-open.com/lib/view/open1423703640748.html](http://www.open-open.com/lib/view/open1423703640748.html)】
### 8.4 删除 Volumes
Volume 只有在下列情况下才能被删除：
- docker rm -v删除容器时添加了-v选项
- docker run --rm运行容器时添加了--rm选项
否则，会在/var/lib/docker/vfs/dir目录中遗留很多不明目录。
****************************************************************************************************************************
# 如何免 sudo 使用 docker
用daocloud脚本按照后：
If you would like to use Docker as a non-root user, you should now consider
adding your user to the "docker" group with something like:
**sudo usermod -aG docker ubuntu**
Remember that you will have to log out and back in for this to take effect!
### 其他情况：
默认安装完 docker 后，每次执行 docker 都需要运行 sudo 命令，非常浪费时间影响效率。如果不跟 sudo，直接执行 `docker images` 命令会有如下问题：
> 
FATA[0000] Get http:///var/run/docker.sock/v1.18/images/json: dial unix /var/run/docker.sock: permission denied. Are you trying to connect to a TLS-enabled daemon without TLS?
于是考虑如何免 sudo 使用 docker，经过查找资料，发现只要把用户加入 docker 用户组即可，具体用法如下。
### 免 sudo 使用 docker
- 
如果还没有 docker group 就添加一个：
```
```
- 
将用户加入该 group 内。然后退出并重新登录就生效啦。
``
- 
重启 docker 服务
```
```
- 
切换当前会话到新 group 或者重启 X 会话
``
注意，最后一步是必须的，否则因为 `groups` 命令获取到的是缓存的组信息，刚添加的组信息未能生效，所以 
```
docker
 images
```
 执行时同样有错。
### 原因分析
- 
因为 `/var/run/docker.sock` 所属 docker 组具有 setuid 权限
```
```
Docker 如何进入Container? 
【http://www.tuicool.com/articles/vyyaYv】
 run命令的"-d"选项表示容器将以daemon的形式运行，我们在前台无法看到容器的输出。那么我们怎么查看容器的输出呢？我们可以通过 docker logs + 容器id的方式查看容器内应用的标准输出或标准错误。我们也可以进入容器来查看。
进入容器有多种方法，比如用sudo docker attach daf3ae88fec2。attach后，就好比将daemon方式运行的容器 拿到了前台，你可以Ctrl + C一下，可以看到如下dockerapp1的输出:
^Chandle signal: interrupt
另外一种方式是利用nsenter工具进入我们容器的namespace空间。ubuntu 14.04下可以通过如下方式安装该工具：
```
$ wget [https://www.kernel.org/pub/linux/utils/util-linux/v2.24/util-linux-2.24.tar.gz](https://www.kernel.org/pub/linux/utils/util-linux/v2.24/util-linux-2.24.tar.gz); tar xzvf util-linux-2.24.tar.gz
$ cd util-linux-2.24
$ ./configure –without-ncurses && make nsenter
$ sudo cp nsenter /usr/local/bin
```
安装后，我们通过如下方式即可进入上面的容器：
```
$ sudo docker inspect -–format "{{ .State.Pid }}" daf3ae88fec2
5494
$ sudo nsenter –-target 5494 –-mount –-uts –-ipc –-net –-pid
-bash-4.1# ps -ef
UID        PID  PPID  C STIME TTY          TIME CMD
root         1     0  0 09:20 ?        00:00:00 /bin/dockerapp1
root        16     0  0 09:32 ?        00:00:00 -bash
root        27    16  0 09:32 ?        00:00:00 ps -ef
-bash-4.1#
```
进入容器后通过ps命令可以看到正在运行的dockerapp1程序。
[附录] 几种方法： http://www.lupaworld.com/article-241785-1.html
**牛人脚本：[http://www.hjue.me/post/docker-nsenter](http://www.hjue.me/post/docker-nsenter)**
使用shell脚本 docker-enter，将如下代码保存为docker-enter, `chomod +x docker-enter`
```
#!/bin/sh
  if [ -e $(dirname "$0")/nsenter ]; then
    # with boot2docker, nsenter is not in the PATH but it is in the same folder
    NSENTER=$(dirname "$0")/nsenter
  else
    NSENTER=nsenter
  fi
  if [ -z "$1" ]; then
    echo "Usage: `basename "$0"` CONTAINER [COMMAND [ARG]...]"
    echo ""
    echo "Enters the Docker CONTAINER and executes the specified COMMAND."
    echo "If COMMAND is not specified, runs an interactive shell in CONTAINER."
  else
    PID=$(docker inspect --format "{{.State.Pid}}" "$1")
    if [ -z "$PID" ]; then
      exit 1
    fi
    shift
    OPTS="--target $PID --mount --uts --ipc --net --pid --"
    if [ -z "$1" ]; then
      # No command given.
      # Use su to clear all host environment variables except for TERM,
      # initialize the environment variables HOME, SHELL, USER, LOGNAME, PATH,
      # and start a login shell.
      "$NSENTER" $OPTS su - root
    else
      # Use env to clear all host environment variables.
      "$NSENTER" $OPTS env --ignore-environment -- "$@"
    fi
  fi
```
运行 `docker-enter <container id>` ，这样就进入到指定的容器中
**sudo ./docker-enter <container id>**
####Update 20160806
https://github.com/jpetazzo/nsenter
# Looking to start a shell inside a Docker container?
Starting from Docker 1.3 you can use [Docker exec](https://docs.docker.com/reference/commandline/cli/#exec) to enter a Docker container.
 Example:
```
docker exec -it CONTAINER_NAME /bin/bash
```
There are differences between nsenter and docker exec; namely, nsenter doesn't enter the cgroups, and therefore evades resource limitations. The potential benefit of this would be debugging and external audit, but for remote access, docker
 exec is the current recommended approach.
