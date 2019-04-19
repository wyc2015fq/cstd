# Ubuntu 14.04下安装Docker图文教程(一) - 零度的博客专栏 - CSDN博客
2017年06月08日 14:22:34[零度anngle](https://me.csdn.net/zmx729618)阅读数：1086
当前，完全硬件虚拟化技术（KVM、Xen、Hyper-V 等）能在一个物理主机上很好地运行多个互相独立的操作系统，但这也带来一些问题：性能不佳，资源浪费，系统反应迟缓等。有时候对用户来说，完全的硬件虚拟化并不是最好的选择。
一种替代方案是使用轻量级虚拟化技术 —— 所谓的 LinuX Container 容器 (LXC)，它提供的是系统级虚拟化。与跑虚拟机相比，LXC 可以在一个轻量级沙箱容器里面跑多个 Linux 操作系统。当你需要设置一些易于克隆的开发环境、测试环境，或想在安全沙盒里安装应用时，LXC 就非常有用了。
[Docker](http://lib.csdn.net/base/4)是一个开源工具，可以让用户方便地布署 Linux Container 容器。Docker 很快变成了 container 技术的非官方标准，从而被 Ubuntu 和 Red Hat等众多发行版吸收进去。
![](https://img-blog.csdn.net/20150515155828951?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3VuaHVhcWlhbmcx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20150515155907016?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3VuaHVhcWlhbmcx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
本教程中我会向你们演示如何在 Ubuntu 14.04 中使用 Docker 来管理 LXC。需要注意的是，本教程的一些内容可能会与其他 Ubuntu 版本下的操作会稍微有些出入。
当前Ubuntu 系统下的 Docker 的安装包只提供64位。如果你想运行32位的 Docker，你需要使用源码编译32位 Docker 工具。
继续进行分布式开发，下面开始在Ubuntu14.04下安装Docker。
注：Ubuntu上可用的Docker程序包只支持64位系统。想在32位机器上运行它，你就要利用源代码构建32位版本的Docker,建议使用64位的。
Docker利用Linux容器(LXC)虚拟化技术提供一份部署环境。Docker目的是创建可移植,可分发给任何的Docker环境中运行。Docker由于是OpenVZ的作品,对内核有一些要求。
1.安装
安装Docker使用apt-get命令（若为非root用户，需在命令前添加sudo）:
 apt-get install docker.io
![](https://img-blog.csdn.net/20150515155956046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3VuaHVhcWlhbmcx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
启动服务和守护进程
 service docker.io status 
 service docker.io start 
创建软连接，以便在外壳上使用起来更容易。
 ln -sf /usr/bin/docker.io /usr/local/bin/docker 
如没有提示错误则说明你已经在Ubuntu14.04上面快速安装Docker成功了。
读者也可通过下面的方法从Docker 官方源安装最新版本。首先需要安装apt-transporthttps，
并添加Docker 官方源：
 sudo apt-get install apt-transport-https 
sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 36A1D7
869245C8950F966E92D8576A8BA88D21E9
sudo bash -c "echo deb https://get.docker.io/ubuntu docker main > /etc/apt/
sources.list.d/docker.list"
sudo apt-get update
之后，可以通过下面的命令来安装最新版本的Docker：
sudo apt-get install -y lxc-docker
在安装了Docker 官方软件源后，若需要更新Docker 软件版本，只需要执行以下命令即可升级：
sudo apt-get update -y lxc-docker
如果使用的是较低版本的Ubuntu 系统，则需要先进行内核更新并重启系统后再进行安装：
sudo apt-get update
sudo apt-get install -y linux-image-generic-lts-raring linux-headers-genericlts-
Raring
sudo reboot
重启后，重复在Ubuntu 14.04 系统的安装步骤即可。
注：在安装过程中应尽量做到将以上脚本命令整理好放在一个脚本中，以便以后进行一键式部署安装。
2.准备外壳环境
现在，我将把我的user=shq添加到docker组：
sudo usermod -a -G docker shq
或者使用：
sudo usermod -a -G docker $USER 
这里，我会退出当前会话，然后再重新登录。现在，我将添加docker配置文件，以便向系统通知其位置。
sudo vi /etc/default/docker.io 
DOCKER="/usr/bin/docker.io" 
现在重启服务，如下所示：
sudo service docker.io restart 
3.管理docker容器
3.1.下载ubuntu docker容器
安装完成后，可以使用下面的命令确定安装的版本和基本的信息。由于docker使用go语言进行编写的，所以要依赖于GO的编译工具和库文件。若要查看Docker版本信息，可输入如下命令：
sudo docker version
以下为提示信息：
![](https://img-blog.csdn.net/20150515160051692?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3VuaHVhcWlhbmcx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
镜像是Docker 运行容器的前提。
官方安装方式docker pull imagename从docker的索引中心下载，imagename是镜像名称，例如docker pull ubuntu就是下载base ubuntu并且tag是latest。pull命令用于从注册中心（registry）拉取映像或软件库。(使用pull 命令从Docker Hub仓库中下载镜像到本地)
sudo docker pull ubuntu:14.04
以下为提示信息：
Pulling repository ubuntu
c4ff7513909d: Download complete
511136ea3c5a: Download complete
1c9383292a8f: Download complete
9942dd43ff21: Download complete
d92c3c92fa73: Download complete
0ea0d582fd90: Download complete
cc58e55aa5a5: Download complete
上面的命令即从默认的注册服务器registry.hub.docker.com 中的ubuntu 仓库来下载标记为latest 的镜像。
用户也可以选择从其他注册服务器的仓库下载。此时，需要在仓库名称前指定完整的仓
库注册服务器地址。例如从DockerPool 社区的镜像源dl.dockerpool.com 下载最新的
Ubuntu 镜像。
 sudo docker pull dl.dockerpool.com:5000/ubuntu 
下载完成后，你也可以使用docker images 命令可以列出本地主机上已有的镜像。
 sudo docker images 
以下为提示信息：
![](https://img-blog.csdn.net/20150515160224574?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3VuaHVhcWlhbmcx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
使用docker inspect 命令可以获取该镜像的详细信息。
 sudo docker inspect 5506de2b643b 
docker inspect 命令返回的是一个JSON 格式的消息，如果我们只要其中一项内容
时，可以使用-f 参数来指定，例如，获取镜像的Architecture 信息：
 sudo docker inspect -f {{".Architecture"}} 550 amd64
在指定镜像ID 的时候，通常使用该ID 的前若干个字符组成的可区分字串来替代完整的 ID。
使用docker ps -a 命令可以看到本机上存在的所有容器：
 sudo docker ps -a 
列出当前运行的容器， -a 会列出所有，包括已停止的， -q 只列出容器 ID，使用如下命令：
 docker ps -a -q 
删除所有未运行的容器，使用如下命令：
 docker ps -a -q | xargs docker rm 
4.启动docker容器
注：容器依赖于镜像。
下面的命令启动 Docker 里面的 Ubuntu 系统,进入交互的Shell中，最后的参数“/bin/bash”表示一旦容器启动，首先会执行简单的 bash。登录进入到Ubuntu容器的bash外壳：
 sudo docker run -i -t ubuntu:14.04 /bin/bash 
查看当前镜像信息，输入如下命令：
cat /etc/lsb-release
显示如下信息：
DISTRIB_ID=Ubuntu 
DISTRIB_RELEASE=12.04  
DISTRIB_CODENAME=precise 
DISTRIB_DESCRIPTION="Ubuntu 12.04.4 LTS" 
