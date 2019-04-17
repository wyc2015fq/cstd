# Docker软件应用容器平台 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年05月24日 11:32:40[boonya](https://me.csdn.net/boonya)阅读数：1364








Docker是世界领先的软件容器平台。开发人员使用Docker来消除与同事的代码协作时的“我机器上的工作”问题。操作员使用Docker在隔离的容器中并行运行和管理应用程序，以获得更好的计算密度。企业使用Docker构建灵活的软件传送管道，可以更快，更安全地运行新功能，并且对于Linux和Windows Server应用程序都有信心。


Docker 是一个开源的应用容器引擎，让开发者可以打包他们的应用以及依赖包到一个可移植的容器中，然后发布到任何流行的 Linux 机器上，也可以实现虚拟化。容器是完全使用沙箱机制，相互之间不会有任何接口。


![](https://img-blog.csdn.net/20170524111112967)




## 项目介绍：



Docker是dotCloud开源的、可以将任何应用包装在Linux Container中运行的工具，2013年3月发布首个版本。当应用被打包成Docker Image后，部署和运维就变得极其简单。可以使用统一的方式下载、启动、扩展、删除、迁移。Docker可以用来：
- 自动化打包和部署任何应用
- 创建一个轻量级私有PaaS云
- 搭建开发测试环境
- 部署可扩展的Web应用

著名的轻量PaaS平台CoreOS就是使用了Docker作为部署应用的工具。

**开发语言：** Go

## License：

Apache License 2.0 

## 链接：
- 
**项目主页：**[http://docker.io](http://docker.io)

- 
**项目托管地址：**
- **GitRepository:** git://github.com/dotcloud/docker.git 
- **Github项目地址:**[https://github.com/dotcloud/docker](https://github.com/dotcloud/docker)
- Github Docker 源码：[https://github.com/docker/docker](https://github.com/docker/docker)


- 
**其他链接：**
- **维基百科:**[http://en.wikipedia.org/wiki/Docker_%28Linux_container_engine%29](http://en.wikipedia.org/wiki/Docker_%28Linux_container_engine%29)
- **docs.docker.io:**[http://docs.docker.io](http://docs.docker.io)
- **Docker Index:**[https://index.docker.io](https://index.docker.io)
- **Docker Issues:**[https://github.com/dotcloud/docker/issues](https://github.com/dotcloud/docker/issues)
- **Docker Club:**[https://groups.google.com/forum/#!forum/docker-club](https://groups.google.com/forum/#%21forum/docker-club)
- **Docker Community:**[http://www.docker.io/community/](http://www.docker.io/community/)
- **Freecode地址:**[http://freecode.com/projects/docker](http://freecode.com/projects/docker)


## 什么是容器

使用容器，使一件软件运行所需的一切都被打包成隔离的容器。与VM不同，容器不捆绑完整的操作系统 - 只需要使软件工作所需的库和设置。这使得高效，轻便，自包含的系统，并保证软件将始终运行相同，无论它在哪里部署。

ps:后面单独对容器用一篇文章来介绍它。


## Docker主要服务对象

主要对服务对象：开发者、产品运维和企业而言。开发和运维的下载地址：[https://www.docker.com/get-docker](https://www.docker.com/get-docker)，企业版需要下载特定版本。


### 开发者

Docker自动执行设置和配置开发环境的重复任务，以便开发人员可以专注于重要的事情：构建出优秀的软件。

![](https://img-blog.csdn.net/20170524112414421)


使用Docker的开发人员不必安装和配置复杂数据库，也不用担心在不兼容的语言工具链版本之间切换。当应用程序停泊时，这种复杂性被推入容易构建，共享和运行的容器中。将同事加入新的代码库不再意味着安装软件和解释安装程序的时间。Dockerfiles随附的代码更简单：依赖关系被拉为整齐的Docker映像，任何具有Docker和编辑器的人都可以在几分钟内构建和调试应用程序。

### 运维

Docker简化了软件交付。开发和部署缺陷修复程序和新功能，而不会有路障。实时缩放应用程序。

Docker是开发人员和IT运营团队无处不在的秘密武器，允许他们自动，安全，可移植地构建，发布，测试和部署应用程序，而不会有任何惊喜。没有更多的维基，自述文件，长的运行手册文档和后期的注释与陈旧的信息。使用Docker的团队知道他们的图像在开发，分期和生产中都是一样的。新功能和修补程序可以快速获得客户，而无需麻烦，惊喜或停机。

### 企业

Docker是现代应用程序平台的核心，桥接开发人员和IT，Linux和Windows。Docker在云端工作以及内部部署;并支持传统和微型服务架构。使用Docker来构建，网络，安全和安排容器，并将其从开发到生产管理。Docker通过使所有应用程序能够以最佳成本进行敏捷，云准备和安全化，将企业置于数字化转型之路。




## Docker的应用场景
- 
Web 应用的自动化打包和发布。

- 
自动化测试和持续集成、发布。

- 
在服务型环境中部署和调整数据库或其他的后台应用。

- 从头编译或者扩展现有的OpenShift或Cloud Foundry平台来搭建自己的PaaS环境。

## Docker 的优点
- 
**简化程序：**
Docker 让开发者可以打包他们的应用以及依赖包到一个可移植的容器中，然后发布到任何流行的  Linux 机器上，便可以实现虚拟化。Docker改变了虚拟化的方式，使开发者可以直接将自己的成果放入Docker中进行管理。方便快捷已经是 Docker的最大优势，过去需要用数天乃至数周的	任务，在Docker容器的处理下，只需要数秒就能完成。

- 
**避免选择恐惧症：**
如果你有选择恐惧症，还是资深患者。Docker 帮你	打包你的纠结！比如 Docker 镜像；Docker 镜像中包含了运行环境和配置，所以 Docker 可以简化部署多种应用实例工作。比如 Web 应用、后台应用、数据库应用、大数据应用比如 Hadoop 集群、消息队列等等都可以打包成一个镜像部署。 

- 
**节省开支：**
一方面，云计算时代到来，使开发者不必为了追求效果而配置高额的硬件，Docker 改变了高性能必然高价格的思维定势。Docker 与云的结合，让云空间得到更充分的利用。不仅解决了硬件管理的问题，也改变了虚拟化的方式。





## 更多官方介绍

官网：[https://www.docker.com/](https://www.docker.com/)




- [What is Docker](https://www.docker.com/what-docker)
- [What is a Container](https://www.docker.com/what-container)
- [Use Cases](https://www.docker.com/use-cases)
- [Customers](https://www.docker.com/customers)
- [Partners](https://www.docker.com/partners/partner-program)
- [For Government](https://www.docker.com/industry-government)
- [About Docker](https://www.docker.com/company)
- [Management](https://www.docker.com/company/management)
- [Press & News](https://www.docker.com/company/news-and-press)
- [Careers](https://www.docker.com/careers)


- [Product](https://www.docker.com/get-docker)
- [Pricing](https://www.docker.com/pricing)
- [Community Edition](https://www.docker.com/community-edition)
- [Enterprise Edition](https://www.docker.com/enterprise-edition)
- [Docker Datacenter](https://www.docker.com/enterprise-edition#container_management)
- [Docker Cloud](https://cloud.docker.com/)
- [Docker Store](https://store.docker.com/)


- [Docker for Mac](https://www.docker.com/docker-mac)
- [Docker for Windows(PC)](https://www.docker.com/docker-windows)
- [Docker for AWS](https://www.docker.com/docker-aws)
- [Docker for Azure](https://www.docker.com/docker-azure)
- [Docker for Windows Server](https://www.docker.com/docker-windows-server)
- [Docker for the CentOS distribution](https://www.docker.com/docker-centos-distribution)
- [Docker for Debian](https://www.docker.com/docker-debian)
- [Docker for Fedora®](https://www.docker.com/docker-fedora)
- [Docker for Oracle Linux](https://www.docker.com/docker-oracle-linux)
- [Docker for RHEL](https://www.docker.com/docker-red-hat-enterprise-linux-rhel)
- [Docker for SLES](https://www.docker.com/docker-suse-linux-enterprise-server-sles)
- [Docker for Ubuntu](https://www.docker.com/docker-ubuntu)


- [Documentation](https://docs.docker.com/)
- [Blog](https://blog.docker.com/)
- [RSS Feed](https://blog.docker.com/feed/)
- [Training](https://training.docker.com/)
- [Knowledge Base](https://success.docker.com/kbase)
- [Resources](https://www.docker.com/products/resources)


- [Community](https://www.docker.com/docker-community)
- [Open Source](https://www.docker.com/technologies/overview)
- [Events](https://www.docker.com/community/events)
- [Forums](https://forums.docker.com/)
- [Docker Captains](https://www.docker.com/community/docker-captains)
- [Scholarships](https://www.docker.com/docker-community/scholarships)
- [Community News](https://blog.docker.com/curated/)


开始：[https://docs.docker.com/get-started/](https://docs.docker.com/get-started/)





