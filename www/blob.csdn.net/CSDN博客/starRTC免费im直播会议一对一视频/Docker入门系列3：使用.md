# Docker入门系列3：使用 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年03月09日 12:49:52[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：400
## 入门
首先强烈建议玩一遍官方的入门教程，[Interactive commandline tutorial](http://www.docker.io/gettingstarted/)，下面是答案：
查看版本：docker version
搜索Image：docker search tutorial
下载Image：docker pull learn/tutorial
创建容器并输出"hello world"：docker run learn/tutorial echo "hello world"
在容器里安装ping：docker run learn/tutorial apt-get install -y ping
保存刚才的状态：首先用docker ps -l找到安装ping的容器ID，
ID                  IMAGE               COMMAND                CREATED             STATUS              PORTS
6982a9948422        ubuntu:12.04**apt-get install****ping**1 minute ago        Exit 0
然后保存为 'learn/ping'：docker commit 698 learn/ping （注：id不用全部写出，一般写前面3到4个字符就可以了），此命令会返回一个新的id
运行：docker run learn/ping ping google.com
查看运行的容器：docker ps或者“docker inspect 容器id前3或4个字符”
上传image到Docker Hub Registry：
首先运行“docker images” will show you which images are currently on your host.
然后docker push learn/ping即可。
自己的库：[https://hub.docker.com/](https://hub.docker.com/)
[https://registry.hub.docker.com/](https://registry.hub.docker.com/) 网站上有一系列创建好的操作系统和应用程序映像。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%973%EF%BC%9A%E4%BD%BF%E7%94%A8&action=edit&section=2)]使用
我们来下载标准的 ubuntu 14.04 镜像：
docker pull ubuntu:14.04
如果报错：dial unix /var/run/docker.sock: no such file or directory
解决：service docker start
使用 docker images 命令列出可用来创建 Docker 容器的本地镜像。
注意结果中的镜像 ID ，我们后面可以通过这个 ID 来创建容器。
下一步是创建容器并做相关操作。创建一个 Docker 容器的方法是使用 docker run 命令
docker run -i-t 6b4e8a7373fe **/**bin**/****bash**
退出时运行**exit**即可
注：将6b4e8a7373fe替换成你自己的镜像 ID 报错：Cannot start container 32d57c8b3dbaec42ef7e787b1726b3af01996ae7dc177ec4a8aa5eca00250437: write /cgroup/devices/docker/32d57c8b3dbaec42ef7e787b1726b3af01996ae7dc177ec4a8aa5eca00250437/cgroup.procs: invalid argument
原因: Your kernel is too old. 换成centos7，也可以升级内核，docker要求内核至少到3.8.0,参见[使用yum快速升级CentOS
 6.x内核到3.10.28](http://192.168.1.100/elesos_com/index.php?title=%E4%BD%BF%E7%94%A8yum%E5%BF%AB%E9%80%9F%E5%8D%87%E7%BA%A7CentOS_6.x%E5%86%85%E6%A0%B8%E5%88%B03.10.28)
现在你已经通过镜像 6b4e8a7373fe创建了一个 Docker 容器，并在其中运行了 bash 。
通过 docker ps -a 命令查看本地 Docker 环境中存在多少容器(列出所有的container)。
docker run 6b4e8a7373fe **/**bin**/****echo** hello docker
可以再次执行一遍：docker start container_id
不过这次看不到”hello world”了，只能看到ID，用logs命令才能看得到，
docker logs container_id
可以看到两个”hello world”，因为这个container运行了两次。
daemon方式，让软件作为长时间服务运行，这就是SAAS！
CONTAINER_ID=$**(**docker run -d 6b4e8a7373fe **/**bin**/****sh**-c"while true; do echo hello world; sleep 1; done"**)**
在container外面查看它的输出:
docker logs $CONTAINER_ID
或者连接上容器实时查看
docker attach $CONTAINER_ID
终止容器:docker stop $CONTAINER_ID
用sudo docker ps(列出当前所有正在运行的container)要看一下，发现该容器已经没有了。
还有一种方式可以让程序在daemon模式下运行，就是在Dockerfile里设置USER为daemon，见[Dockerfile tutorial Level2](http://www.docker.io/learn/dockerfile/level2/)。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%973%EF%BC%9A%E4%BD%BF%E7%94%A8&action=edit&section=3)]删除容器
每一行命令都会产生一个新的容器（无论是在sudo docker run -i -t ubuntu /bin/bash 模式下，还是Dockerfile里的RUN命令），玩了一会儿后，sudo docker ps -a 会看到很多容器，很是干扰视线，可以用一行命令删除所有容器：
docker **rm****`**docker **ps**-a -q**`**
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%973%EF%BC%9A%E4%BD%BF%E7%94%A8&action=edit&section=4)]创建image
有两种用方式，
- 写一个Dockerfile，然后用docker build创建一个image
- 在容器里交互式地（例如docker run -i -t ubuntu /bin/bash）进行一系列操作，然后docker commit固化成一个image。
image相当于编程语言里的类，container相当于实例，不过可以动态给实例安装新软件，然后把这个container用commit命令固化成一个image。
使用dockerfile，创建一个image：
docker build -t yourusername**/**ubuntu:latest - **<** test.dockerfile
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%973%EF%BC%9A%E4%BD%BF%E7%94%A8&action=edit&section=5)]下载image
[https://index.docker.io/](https://index.docker.io/) 是官方的image仓库，也可以用[Docker-Registry](https://github.com/dotcloud/docker-registry)创建自己的仓库，这就好比git，[https://index.docker.io/](https://index.docker.io/) 相当于Github，也可以自己DIY搭建一个git服务器，把自己的代码托管到私有服务器上。
docker pull ubuntu 是从 [https://index.docker.io/_/ubuntu/](https://index.docker.io/_/ubuntu/) 下载名为 ubuntu 的repo，里面包含了几个tag，默认使用latest这个tag。这个repo是docker官方的。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%973%EF%BC%9A%E4%BD%BF%E7%94%A8&action=edit&section=6)]上传并共享image
首先，要去 [https://index.docker.io/](https://index.docker.io/) 注册一个账号
build一个image，build命令格式如下：
docker build -t username**/**repo:tag - **<** Dockerfile
如果没有tag，则默认为 latest。也可以用commit方式生成images
登陆:docker login
输入自己的用户名和密码。
push 到 Docker index：
docker push username**/**repo
如：docker push elesos**/****test****(**注：生成images时要加elesos,即"docker commit 9576cb8dfbcd  elesos/test"，不然push时会报"No such id"错误**)**
这条命令会把一个repo下面的所有tag都push到[https://index.docker.io/](https://index.docker.io/)
注意repo的名字要小于31个字符，不然会push失败。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%973%EF%BC%9A%E4%BD%BF%E7%94%A8&action=edit&section=7)]删除images
停止所有的container，这样才能够删除其中的images：
docker stop $**(**docker **ps**-a -q**)**
删除images，通过image的id(IMAGE ID)来指定删除谁
docker rmi **<**image **id****>**
要删除全部image的话:docker rmi $(docker images -q)
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%973%EF%BC%9A%E4%BD%BF%E7%94%A8&action=edit&section=8)]Dockerfile
怎么自己定制一个 image，例如把常用的软件装好后打包 ? 这时候该 Dockfile 登场了。Dockerfile 实质上是一个脚本文件，用于自动化创建image。
我们在后面的系列中介绍Dockfile。
Docker QQ交流群【群名称是Docker从入门到精通】：293948287
我的docker库：[https://registry.hub.docker.com/repos/elesos/](https://registry.hub.docker.com/repos/elesos/) ，里面有很多新技术的测试实例。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%973%EF%BC%9A%E4%BD%BF%E7%94%A8&action=edit&section=9)]艺搜参考
[官方Hello World](http://docs.docker.com/userguide/dockerizing/#running-examples)
[Docker 快速入门](http://cn.soulmachine.me/blog/20131026/)
[利用Docker构建开发环境 | UC技术博客](http://tech.uc.cn/?p=2726)
[创建你的首个 Docker 容器](https://www.dockboard.org/create-your-first-docker-container/)
[Docker：利用Linux容器实现可移植的应用部署](http://www.infoq.com/cn/articles/docker-containers)
[https://github.com/docker/docker/issues/6670](https://github.com/docker/docker/issues/6670)
[http://www.linuxidc.com/Linux/2014-09/106322.htm](http://www.linuxidc.com/Linux/2014-09/106322.htm)
