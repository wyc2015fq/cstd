# Docker在不同平台上的安装 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年05月24日 13:19:41[boonya](https://me.csdn.net/boonya)阅读数：1165








本篇文章摘录w3cschool的Docker在不同平台上的安装，包括Ubuntu、CentOS、Windows。




# Ubuntu Docker 安装





Docker 支持以下的 Ubuntu 版本：
- Ubuntu Precise 12.04 (LTS)
- Ubuntu Trusty 14.04 (LTS)
- Ubuntu Wily 15.10
- 其他更新的版本……



## 前提条件



Docker 要求 Ubuntu 系统的内核版本高于 3.10 ，查看本页面的前提条件来验证你的 Ubuntu 版本是否支持 Docker。

通过 uname -r 命令查看你当前的内核版本
w3cschool@w3cschool:~$ uname -r

![](http://img.w3cschool.cn/attachments/day_160902/201609021349399637.png)


## 使用脚本安装 Docker

### 1、获取最新版本的 Docker 安装包


```
w3cschool@w3cschool:~$ sudo apt-get update
```



![](http://img.w3cschool.cn/attachments/day_160902/201609021743155432.png)


```
w3cschool@w3cschool:~$ sudo apt-get install -y docker.io
```
![](http://img.w3cschool.cn/attachments/day_160902/201609021742292521.png)




### 2、启动docker 后台服务
w3cschool@w3cschool:~$ sudo service docker start

![](http://img.w3cschool.cn/attachments/day_160902/201609021750267729.png)




# CentOS Docker 安装

Docker支持以下的CentOS版本：
- CentOS 7 (64-bit)
- CentOS 6.5 (64-bit) 或更高的版本

## 前提条件

目前，CentOS 仅发行版本中的内核支持 Docker。

Docker 运行在 CentOS 7 上，要求系统为64位、系统内核版本为 3.10 以上。

Docker 运行在 CentOS-6.5 或更高的版本的 CentOS 上，要求系统为64位、系统内核版本为 2.6.32-431 或者更高版本。

## 使用 yum 安装（CentOS 7下）

Docker 要求 CentOS 系统的内核版本高于 3.10 ，查看本页面的前提条件来验证你的CentOS 版本是否支持 Docker 。
通过 **uname -r **命令查看你当前的内核版本[root@w3cschool ~]# uname -r 3.10.0-327.el7.x86_64

![](http://img.w3cschool.cn/attachments/image/20160929/1475121914749431.jpg)


### 安装 Docker

Docker 软件包和依赖包已经包含在默认的 CentOS-Extras 软件源里，安装命令如下：
[root@w3cschool ~]# yum -y install docker

![](http://img.w3cschool.cn/attachments/image/20160929/1475122023117353.jpg)


安装完成。

![](http://img.w3cschool.cn/attachments/image/docker10.png)

启动 Docker 后台服务
[root@w3cschool ~]# service docker start

![](http://img.w3cschool.cn/attachments/image/20160929/1475122123942838.jpg)


测试运行 hello-world
[root@w3cschool ~]#docker run hello-world

![](http://img.w3cschool.cn/attachments/image/20160929/1475122214879794.jpg)


由于本地没有hello-world这个镜像，所以会下载一个hello-world的镜像，并在容器内运行。

## 使用脚本安装 Docker

1、使用 `sudo` 或 `root` 权限登录 Centos。

2、确保 yum 包更新到最新。
$ sudo yum update

3、执行 Docker 安装脚本。
$ curl -fsSL https://get.docker.com/ | sh

执行这个脚本会添加 `docker.repo` 源并安装 Docker。

4、启动 Docker 进程。
$ sudo service docker start

5、验证 `docker` 是否安装成功并在容器中执行一个测试的镜像。
$ sudo docker run hello-world

到此，docker 在 CentOS 系统的安装完成。







# Windows Docker 安装

Docker 引擎使用的是 Linux 内核特性，所以我们需要在 Windows 上使用一个轻量级的虚拟机 (VM) 来运行 Docker。

我们通过 Boot2Docker 来安装虚拟机和运行 Docker

## 安装

### 1、安装Boot2Docker 

最新版 Boot2Docker 下载地址：[https://github.com/boot2docker/windows-installer/releases/latest](https://github.com/boot2docker/windows-installer/releases/latest)

目前最新版为v1.8.0, 下载地址为：[https://github.com/boot2docker/windows-installer/releases/download/v1.8.0/docker-install.exe](https://github.com/boot2docker/windows-installer/releases/download/v1.8.0/docker-install.exe)

### 2、运行安装文件

运行安装文件，它将会安装 virtualbox、MSYS-git boot2docker Linux 镜像和 Boot2Docker 的管理工具。

![](http://img.w3cschool.cn/attachments/image/docker13.png)

![](http://img.w3cschool.cn/attachments/image/docker14.png)

![](http://img.w3cschool.cn/attachments/image/docker15.png)

接着连续点击"Next"，进到如下界面

![](http://img.w3cschool.cn/attachments/image/docker16.png)

点击"Install"开始安装。

![](http://img.w3cschool.cn/attachments/image/docker17.png)

安装完成。

![](http://img.w3cschool.cn/attachments/image/docker18.png)

从桌面上或者 Program Files 中运行 Boot2Docker Start。

Boot2Docker Start 将启动一个 Unix shell 来配置和管理运行在虚拟主机中的 Docker，我们可以通过运行 **docker version** 来查看它是否正常工作。

![](http://img.w3cschool.cn/attachments/image/docker36.png)

## 运行 Docker

使用boot2docker.exe ssh 连接到虚拟主机上，然后执行docker run hello-world



![](http://img.w3cschool.cn/attachments/image/docker37.png)

![](http://img.w3cschool.cn/attachments/image/docker38.jpg)

到此，docker在Windows系统的安装完成。







