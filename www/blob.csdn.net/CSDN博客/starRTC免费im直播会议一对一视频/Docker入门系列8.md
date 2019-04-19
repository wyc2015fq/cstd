# Docker入门系列8 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年03月09日 12:57:48[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：200
## commit
docker commit -m "Added json gem" -a "Docker Newbee" 0b2616b0e5a8 ouruser/sinatra:v2
-m 来指定提交的说明信息，跟我们使用的版本控制工具一样；-a 可以指定更新的用户信息；之后是用来创建镜像的容器的ID；最后指定目标镜像的仓库名和 tag 信息。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%978&action=edit&section=2)]导出与导入镜像
docker save -o ubuntu_14.04.tar ubuntu:14.04
可以使用 docker load 从导出的本地文件中再导入到本地镜像库，例如
docker load < ubuntu_14.04.tar
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%978&action=edit&section=3)]删除
docker rmi 删除镜像。注： docker rm 命令是删除容器。
在删除镜像之前要先用 docker rm 删掉依赖于这个镜像的所有容器。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%978&action=edit&section=4)]启动容器
docker run命令中的-t选项让Docker分配一个伪终端（pseudo-tty）并绑定到容器的标准输入上， -i 则让容器的标准输入保持打开。
创建容器时，Docker 会在后台：
- 分配一个文件系统，并在只读的镜像层外面挂载一层可读写层
- 从宿主主机配置的网桥接口中桥接一个虚拟接口到容器中去
容器的核心为所执行的应用程序，所需要的资源都是应用程序运行所必需的。除此之外，并没有其它的资源。可以在伪终端中利用 ps 或 top 来查看进程信息。
bash-4.1# ps
 PID TTY          TIME CMD
   1 ?        00:00:00 bash
  12 ?        00:00:00 ps
可见，容器中仅运行了指定的 bash 应用。这种特点使得 Docker 对资源的利用率极高，是货真价实的轻量级虚拟化。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%978&action=edit&section=5)]后台运行
可以通过添加 -d 参数来实现。
 docker run -d ubuntu:14.04 /bin/sh -c "while true; do echo hello world; sleep 1; done"
查看输出信息，可以通过 docker logs [容器]命令。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%978&action=edit&section=6)]终止容器
可以使用 docker stop 来终止一个运行中的容器。
此外，当Docker容器中指定的应用终结时，容器也自动终止。 如对于只启动了一个终端的容器，用户通过 exit 命令或 Ctrl+d 来退出终端时，所创建的容器立刻终止。
docker restart 命令会将一个运行态的容器终止，然后再重新启动它。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%978&action=edit&section=7)]艺搜参考
[http://yeasy.gitbooks.io/docker_practice/content/](http://yeasy.gitbooks.io/docker_practice/content/)
