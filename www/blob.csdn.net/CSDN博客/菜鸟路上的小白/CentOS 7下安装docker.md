# CentOS 7下安装docker - 菜鸟路上的小白 - CSDN博客





2016年11月21日 17:28:24[somnus_小凯](https://me.csdn.net/u012486840)阅读数：733








## 使用 yum 安装


Docker 要求 CentOS 系统的内核版本高于 3.10 ，查看本页面的前提条件来验证你的CentOS 版本是否支持 Docker 。
通过 uname
 -r 命令查看你当前的内核版本[root@runoob ~]# uname -r 3.10.0-327.el7.x86_64
![](https://img-blog.csdn.net/20161121172657522?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### 安装 Docker


Docker 软件包和依赖包已经包含在默认的 CentOS-Extras 软件源里，安装命令如下：
[root@runoob ~]# yum -y install docker
![](https://img-blog.csdn.net/20161121172710194?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


安装完成。

![](https://img-blog.csdn.net/20161121172722913?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


启动 Docker 后台服务
[root@runoob ~]# service docker start
![](https://img-blog.csdn.net/20161121172733788?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


测试运行 hello-world
[root@runoob ~]#docker run hello-world
![](https://img-blog.csdn.net/20161121172744913?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


由于本地没有hello-world这个镜像，所以会下载一个hello-world的镜像，并在容器内运行。

## 使用脚本安装 Docker


1、使用 `sudo` 或 `root` 权限登录 Centos。


2、确保 yum 包更新到最新。
$ sudo yum update

3、执行 Docker 安装脚本。
$ curl -fsSL https://get.docker.com/ | sh

执行这个脚本会添加 `docker.repo` 源并安装 Docker。


4、启动 Docker 进程。
$ sudo service docker start

5、验证 `docker` 是否安装成功并在容器中执行一个测试的镜像。
$ sudo docker run hello-world

到此，docker 在 CentOS 系统的安装完成。



