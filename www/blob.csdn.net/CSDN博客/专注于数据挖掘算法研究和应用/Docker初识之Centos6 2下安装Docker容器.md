# Docker初识之Centos6.2下安装Docker容器 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月05日 10:56:31[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2383








                
1、Docker简单介绍

    Docker 扩展了 Linux 容器（Linux Containers），或着说 LXC，通过一个高层次的 API 为进程单独提供了一个轻量级的虚拟环境。Docker 利用了 LXC， cgroups 和 Linux 自己的内核。和传统的虚拟机不同的是，一个 Docker 容器并不包含一个单独的操作系统，而是基于已有的基础设施中操作系统提供的功能来运行的。

    Docker类似虚拟机的概念，但是与虚拟化技术的不同点在于下面几点：

    1）虚拟化技术依赖物理CPU和内存，是硬件级别的；而docker构建在操作系统上，利用操作系统的containerization技术，所以docker甚至可以在虚拟机上运行。

    2）虚拟化系统一般都是指操作系统镜像，比较复杂，称为“系统”；而docker开源而且轻量，称为“容器”，单个容器适合部署少量应用，比如部署一个redis、一个memcached。

    3）传统的虚拟化技术使用快照来保存状态；而docker在保存状态上不仅更为轻便和低成本，而且引入了类似源代码管理机制，将容器的快照历史版本一一记录，切换成本很低。

    4）传统的虚拟化技术在构建系统的时候较为复杂，需要大量的人力；而docker可以通过Dockfile来构建整个容器，重启和构建速度很快。更重要的是Dockfile可以手动编写，这样应用程序开发人员可以通过发布Dockfile来指导系统环境和依赖，这样对于持续交付十分有利。

    5）Dockerfile可以基于已经构建好的容器镜像，创建新容器。Dockerfile可以通过社区分享和下载，有利于该技术的推广。

    Docker 会像一个可移植的容器引擎那样工作。它把应用程序及所有程序的依赖环境打包到一个虚拟容器中，这个虚拟容器可以运行在任何一种 Linux 服务器上。这大大地提高了程序运行的灵活性和可移植性，无论需不需要许可、是在公共云还是私密云、是不是裸机环境等等。

    Docker也是一个云计算平台，它利用Linux的LXC、AUFU、Go语言、cgroup实现了资源的独立，可以很轻松的实现文件、资源、网络等隔离，其最终的目标是实现类似PaaS平台的应用隔离。

    Docker 由下面这些组成：

    1）Docker 服务器守护程序（server daemon），用于管理所有的容器。

    2）Docker 命令行客户端，用于控制服务器守护程序。

    3）Docker 镜像：查找和浏览 docker 容器镜像。



2、Docker安装：受外网限制，只有通过rpm安装docker

   1）rpm源：https://yum.dockerproject.org/repo/main/centos/6/Packages/

      下载：docker-engine-1.7.1-1.el6.x86_64.rpm

   2）依赖组件cgroup下载：https://pkgs.org/

      #rpm -ivh libcgroup-0.40.rc1-17.el6_7.x86_64.rpm  

      #rpm -ivh libcgroup-pam-0.40.rc1-17.el6_7.x86_64.rpm

      #rpm -ivh libcgroup-devel-0.40.rc1-17.el6_7.x86_64.rpm

   3）内核组件下载：https://pkgs.org/

      #yum install kernel-lt-3.10.104-1.el6.elrepo.x86_64.rpm 

      #reboot   重启

      #uname -r  查看内核版本

   4）Docker安装：

      #rpm -ivh docker-engine-1.7.1-1.el6.x86_64.rpm

      # service docker start    启动docker服务

      # docker -v               查看docker版本



3、Docker使用：

   docker入门教程：http://dockone.io/article/111

   具体使用过程也暴露出安装过程中的一些问题，前面的安装是有不足之处。

   具体使用待后续使用时再深入，相信在运维上有很多帮助。
            


