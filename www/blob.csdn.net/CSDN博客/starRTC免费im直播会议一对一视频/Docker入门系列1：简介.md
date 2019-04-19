# Docker入门系列1：简介 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年03月09日 12:48:46[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：226
可以实现快速部署。
比如一台 16 核 32G 内存的虚拟机上，需要跑 500+ 个用户的应用（每个应用的功能可以认为是一个网站 + 一系列的 RESTful API），有两个事情很重要：
- 资源隔离：比如限制应用最大内存使用量，或者资源加载隔离等。
- 低消耗：虚拟化本身带来的损耗需要尽量的低。
我们不可能在一台机器上开 500 个虚拟机，虽然可以在资源隔离方面做的很好，但这种虚拟化本身带来的资源消耗太严重。
另一个方面，我们可以考虑使用语言级别沙箱，虽然这种「虚拟化」本身的消耗可以低到忽略不计，但是资源隔离方面绝对是噩梦，比如你打算在一个 JVM 里隔离内存的使用。
而 Docker 很好的权衡了两者，即拥有不错的资源隔离能力，又有很低的虚拟化开销。
做PaaS（平台即服务）的也有，把docker当中一个小型虚拟机器使用。
简单得来说，Docker是一个由GO语言写的程序运行的“容器”(Linux containers， LXCs); 目前云服务的基石是操作系统级别的隔离，在同一台物理服务器上虚拟出多个主机。Docker则实现了一种应用程序级别的隔离; 它改变我们基本的开发、操作单元，由直接操作虚拟主机(VM),转换到操作程序运行的“容器”上来。
Docker相对于VM虚拟机的优势十分明显，那就是轻量和高性能和便捷性。
快：管理操作(启动，停止，开始，重启等等) 都是以秒或毫秒为单位的。
Docker是一个云计算平台，它利用Linux的LXC、AUFU、Go语言、cgroup实现了资源的独立，可以很轻松的实现文件、资源、网络等隔离，其最终的目标是实现类似PaaS平台的应用隔离。
文件系统隔离：每个进程容器运行在一个完全独立的根文件系统里。
资源隔离：系统资源，像CPU和内存等可以分配到不同的容器中，使用cgroup。
网络隔离：每个进程容器运行在自己的网络空间，虚拟接口和IP地址。
它把应用程序及所有程序的依赖环境打包到一个虚拟容器中，这个虚拟容器可以运行在任何一种 Linux 服务器上。这大大地提高了程序运行的灵活性和可移植性，无论需不需要许可、是在公共云还是私密云、是不是裸机环境等等。
Docker 由下面这些组成：
- Docker 服务器守护程序（server daemon），用于管理所有的容器。
- Docker 命令行客户端，用于控制服务器守护程序。
- Docker 镜像：查找和浏览 docker 容器镜像。它也访问这里得到：[https://index.docker.io/](https://index.docker.io/)
Docker 之所以有用，是因为把代码从一个机器迁移到另一个机器经常是困难的。它尝试去使得软件迁移的过程变得更加可信和自动化。Docker 容器可以移植到所有支持运行 Docker 的操作系统上。
Docker 通过不仅仅打包应用程序，也打包应用程序的依赖环境来解决这个问题。
到现在为止，要把程序可靠地移植的唯一选择是虚拟机（Virtual Machines，VMs）。虚拟机现在已经很常见了，但虚拟机是非常低级，它提供的是完整的操作系统环境。虚拟机的问题是，迁移的时候太大了。它们包含了大量类似硬件驱动、虚拟处理器、网络接口等等并不需要的信息。 虚拟机也需要比较长时间的启动，同时也会消耗大量的内存、CPU 资源。
Docker 相比起来就非常轻量级了。运行起来就和一个常规程序差不多。这个容器不仅仅运行快，创建一个镜像和制作文件系统快照也很快。它可以在 EC2, RackSpace VMs 那样的虚拟环境中运行。事实上，在 Mac 和 Windows 系统上使用 Docker 的更好方式是使用 Vagrant。Docker 的初衷其实是发挥类似 VM 的作用，但它启动得更快和需要更少的资源。
Docker 比起 Vagrant 来说，运行起来会更加省资源。Vagrant 提供的环境其实是基于 Virtual Box 提供的虚拟机。
Docker是一个开源的引擎，可以轻松的为任何应用创建一个轻量级的、可移植的、自给自足的容器。开发者在笔记本上编译测试通过的容器可以批量地在生产环境中部署，包括VMs（虚拟机）、bare metal、OpenStack 集群和其他的基础应用平台。
还可以在Windows或Mac OS X中，使用Vagrant在VirtualBox的虚拟机里安装。
## 艺搜参考
[https://github.com/docker/docker](https://github.com/docker/docker)
[什么是Docker?](http://www.docker.org.cn/book/docker/16_what-is-docker.html)
[30天学习30种新技术之Docker入门](http://segmentfault.com/a/1190000000366923)
[自动化软件部署](http://www.infoq.com/cn/news/2013/04/Docker)
[Linux容器运行时Docker](http://www.csdn.net/article/2013-03-28/2814679-Linux-Container-Runtime-Docker)
[Docker 的应用场景在哪里？](http://www.zhihu.com/question/22969309)
[Docker究竟是什么，它的优点和缺陷有哪些？](http://code.csdn.net/news/2821898)
[linux里面的docker到底是什么？](http://www.zhihu.com/question/23540634)
