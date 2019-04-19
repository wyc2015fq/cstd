# Docker入门系列2 安装 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年03月09日 12:49:14[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：262
可以从 Docker 社区直接下载可用的模版或镜像。
Docker容器的启动可以在秒级实现，这相比传统的虚拟机方式要快得多。 其次，Docker对系统资源的利用率很高，一台主机上可以同时运行数千个Docker容器。
对开发和运维人员来说，最希望的就是一次创建或配置，可以在任意地方正常运行。
开发者可以使用一个标准的镜像来构建一套开发容器，开发完成之后，运维人员可以直接使用这个容器来部署代码。 Docker可以快速创建容器，快速迭代应用程序，并让整个过程全程可见，使团队中的其他成员更容易理解应用程序是如何创建和工作的。 Docker容器很轻很快！容器的启动时间是秒级的，大量地节约开发、测试、部署的时间。
Docker容器几乎可以在任意的平台上运行，包括物理机、虚拟机、公有云、私有云、个人电脑、服务器等。 这种兼容性可以让用户把一个应用程序从一个平台直接迁移到另外一个。
Docker包括三个基本概念
- 镜像（Image）
- 容器（Container）
- 仓库（Repository）
## 目录
 [[隐藏](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972%EF%BC%9A%E5%AE%89%E8%A3%85#)] 
- [1 Docker镜像](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972%EF%BC%9A%E5%AE%89%E8%A3%85#Docker.E9.95.9C.E5.83.8F)
- [2 Docker容器](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972%EF%BC%9A%E5%AE%89%E8%A3%85#Docker.E5.AE.B9.E5.99.A8)
- [3 Docker仓库](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972%EF%BC%9A%E5%AE%89%E8%A3%85#Docker.E4.BB.93.E5.BA.93)
- [4 安装](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972%EF%BC%9A%E5%AE%89%E8%A3%85#.E5.AE.89.E8.A3.85)
- [5 艺搜参考](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972%EF%BC%9A%E5%AE%89%E8%A3%85#.E8.89.BA.E6.90.9C.E5.8F.82.E8.80.83)
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972%EF%BC%9A%E5%AE%89%E8%A3%85&action=edit&section=1)]Docker镜像
Docker 镜像就是一个只读的模板。
例如：一个镜像可以包含一个完整的ubuntu的操作系统环境，里面仅安装了Apache或用户需要的其它应用程序。
镜像可以用来创建Docker容器。
Docker提供了一个很简单的机制来创建镜像或者更新现有的镜像，用户甚至可以直接从其他人那里下载一个已经做好的镜像来直接使用。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972%EF%BC%9A%E5%AE%89%E8%A3%85&action=edit&section=2)]Docker容器
Docker利用容器来运行应用。
容器是从镜像创建的运行实例。它可以被启动、开始、停止、删除。每个容器都是相互隔离的、保证安全的平台。可以把容器看做是一个应用程序。
注：镜像是只读的，容器在启动的时候创建一层可写层作为最上层。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972%EF%BC%9A%E5%AE%89%E8%A3%85&action=edit&section=3)]Docker仓库
仓库是集中存放镜像文件的场所。有时候会把仓库和仓库注册服务器（Registry）混为一谈，并不严格区分。实际上，仓库注册服务器上往往存放着多个仓库，每个仓库中又包含了多个镜像。
仓库分为公开仓库（Public）和私有仓库（Private）两种形式。
最大的公开仓库是[Docker Hub](https://hub.docker.com/)，存放了数量庞大的镜像供用户下载。 国内的公开仓库包括[Docker
 Pool](http://www.dockerpool.com/)等，可以提供大陆用户更稳定快速的访问。
当然，用户也可以在本地网络内创建一个私有仓库。
当用户创建了自己的镜像之后就可以使用push命令将它上传到公有或者私有仓库，这样下次在另外一台机器上使用这个镜像时候，只需要从仓库上pull下来就可以了。
注：Docker仓库的概念跟Git的类似。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972%EF%BC%9A%E5%AE%89%E8%A3%85&action=edit&section=4)]安装
官方网站上有各种环境下的[安装指南](https://docs.docker.com/installation/#installation)，这里主要介绍下CentOS系列的安装。
Docker支持CentOS6及以后的版本。
CentOS6:
 
$ **sudo****yum install** http:**//**dl.fedoraproject.org**/**pub**/**epel**/**6**/**x86_64**/**epel-release-6-8.noarch.rpm      
或者     
$ **sudo****yum install** http:**//**mirrors.yun-idc.com**/**epel**/**6**/**x86_64**/**epel-release-6-8.noarch.rpm  **//**64位
$ **sudo****yum install** docker-io
centos 7直接运行yum install docker
安装之后启动Docker服务，并让它随系统启动自动加载。
$ **sudo** service docker start
$ **sudo** chkconfig docker on
检查安装情况:
docker -h
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Docker%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972%EF%BC%9A%E5%AE%89%E8%A3%85&action=edit&section=5)]艺搜参考
[http://www.liquidweb.com/kb/how-to-install-docker-on-centos-6/](http://www.liquidweb.com/kb/how-to-install-docker-on-centos-6/)
[https://docs.docker.com/installation/centos/#installing-docker-centos-6](https://docs.docker.com/installation/centos/#installing-docker-centos-6)
[http://yeasy.gitbooks.io/docker_practice/introduction/README.html](http://yeasy.gitbooks.io/docker_practice/introduction/README.html)
[http://tech.uc.cn/?p=2726](http://tech.uc.cn/?p=2726)
[http://www.docker.org.cn/book/install.html](http://www.docker.org.cn/book/install.html)
[在 CentOS 6.4(64位) 安装 docker.io](http://www.oschina.net/translate/nstalling-dockerio-on-centos-64-64-bit)
[在Redhat/Centos下安装Docker（不升级内核）](http://speakingbaicai.blog.51cto.com/5667326/1353183)
[https://github.com/yeasy/docker_practice](https://github.com/yeasy/docker_practice)
[INSTALLING DOCKER ON UBUNTU 14.04](http://www.jamescoyle.net/how-to/1499-installing-docker-on-ubuntu-14-04)
[http://cn.soulmachine.me/blog/20131025/](http://cn.soulmachine.me/blog/20131025/)
