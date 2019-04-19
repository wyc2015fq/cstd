# 60 个 DevOps 开源工具，你在用哪些？ - 文章 - 伯乐在线
原文出处： [Mrina Natarajan](https://elasticbox.com/blog/devops-open-source-tools/)   译文出处：[开源中国](http://www.oschina.net/question/2012764_246208?sort=time)
![](http://jbcdn2.b0.upaiyun.com/2016/03/c2fa90f8f6d9f781cac31d47420c64b0.jpg)
你喜欢免费的东西吗?获得开发者社区支持的自动化，开源的工具是大家梦寐以求的。这里列举了 60 多款最棒的开源工具，可以帮助你很好的实行 DevOps。
![](http://jbcdn2.b0.upaiyun.com/2016/03/2fa91b8f4d8aa34387168912dd408ec8.jpg)
**一、开发工具**
版本控制&协作开发
**1.版本控制系统 Git**
Git是一个开源的分布式版本控制系统，用以有效、高速的处理从很小到非常大的项目版本管理。
**2.代码托管平台 GitLab**
GitLab是一个利用Ruby on Rails开发的开源应用程序，实现一个自托管的Git项目仓库，可通过Web界面进行访问公开的或者私人项目。
**3.代码评审工具 Gerrit**
Gerrit是一个免费、开放源代码的代码审查软件，使用网页界面。利用网页浏览器，同一个团队的软件程序员，可以相互审阅彼此修改后的程序代码，决定是否能够提交，退回或者继续修改。它使用Git作为底层版本控制系统。
**4.版本控制系统 Mercurial**
Mercurial是一种轻量级分布式版本控制系统，采用 Python 语言实现，易于学习和使用，扩展性强。
**5.版本控制系统 Subversion**
Subversion 是一个版本控制系统，相对于的RCS、CVS，采用了分支管理系统，它的设计目标就是取代CVS。互联网上免费的版本控制服务多基于Subversion。
**6.版本控制系统 Bazaar**
Bazaar 是一个分布式的版本控制系统，它发布在 GPL 许可协议之下，并可用于 Windows、GNU/Linux、Unix 以及 Mac OS 系统。
**二、自动化构建和测试**
**1.Apache Ant**
Apache Ant是一个将软件编译、测试、部署等步骤联系在一起加以自动化的一个工具，大多用于Java环境中的软件开发。
**2.Maven**
Maven 除了以程序构建能力为特色之外，还提供 Ant 所缺少的高级项目管理工具。由于 Maven 的缺省构建规则有较高的可重用性，所以常常用两三行 Maven 构建脚本就可以构建简单的项目，而使用 Ant 则需要十几行。事实上，由于 Maven 的面向项目的方法，许多 Apache Jakarta 项目现在使用 Maven，而且公司项目采用 Maven 的比例在持续增长。
**3.Selenium (SeleniumHQ)**
thoughtworks公司的一个集成测试的强大工具。
**4.PyUnit**
Python单元测试框架(The Python unit testing framework)，简称为PyUnit， 是Kent Beck和Erich Gamma这两位聪明的家伙所设计的 JUnit 的Python版本。
**5.QUnit**
QUnit 是 jQuery 的单元测试框架。
**6.JMeter**
JMeter 是 Apache 组织的开放源代码项目，它是功能和性能测试的工具，100% 的用 java 实现。
**7.Gradle**
Gradle 就是可以使用 Groovy 来书写构建脚本的构建系统，支持依赖管理和多项目，类似 Maven，但比之简单轻便。
**8.PHPUnit**
PHPUnit 是一个轻量级的PHP测试框架。它是在PHP5下面对JUnit3系列版本的完整移植，是xUnit测试框架家族的一员(它们都基于模式先锋Kent Beck的设计)。
**三、持续集成&交付**
**1.Jenkins**
Jenkins 的前身是 Hudson，它是一个可扩展的持续集成引擎。
**2.Capistrano**
Capistrano 是一个用来并行的在多台机器上执行相同命令的工具，使用用来安装一整批机器。它最初是被开发用来发布 Rails 应用的。
**3.BuildBot**
BuildBot 是一个系统的自动化编译/测试周期最需要的软件，以验证代码的变化。通过自动重建和测试每次发生了变化的东西，在建设迅速查明之前，减少不必要的失败。
**4.Fabric**
fabric8 是开源 Java Containers(JVMs) 深度管理集成平台。有了 fabric8 可以非常方便的从 UI 和 UX 一致的中央位置进行自动操作，配置和管理。fabric8 同时提供一些非功能性需求，比如配置管理，服务发现故障转移，集中化监控，自动化等等。
**Tinderbox**
**5.Travis CI**
Travis CI 是一个基于云的持续集成项目， 目前已经支持大部分主流语言了，比如：C，PHP，Ruby，Python，Nodejs等等。
**6.Continuum**
Apache Continuum 是最新的 CI 服务器之一，也是值得关注的一个新进入者。基于 Web 的界面使得配置项目很容易。而且，还不需要安装 Web 服务器，因为 Continuum 内置了 Jetty Web 服务器。并且，Continuum 可以作为 Windows 服务运行，还在应用程序的某些部分嵌入了上下文敏感的文档，从而提供了很多帮助。
**7.LuntBuild**
LuntBuild 是一个强大自动构建的工具。通过一个简洁的web接口就可以很容易地进行系统的持续构建。
**8.CruiseControl**
CruiseControl 是一个针对持续构建程序(项目持续集成)的框架，它包括一个email通知的插件，Ant和各种各样的CVS工具。CruiseControl提供了一个Web接口，可随时查看当前的编译状况和历史状况。
**9.Integrity**
Integrity 是 Ruby 开发的持续集成服务器。
**10.Gump**
Gump 是 Apache 的整合工具。它以 Python 写成、完全支持 Apache Ant、Apache Maven 等等软件组建工具。
**11.Go**
Go 是 Google 开发的一种编译型，并发型，并具有垃圾回收功能的编程语言。
**四、部署工具**
**(一)容器平台**
**1.Docker**
Docker 是一个开源的应用容器引擎，让开发者可以打包他们的应用以及依赖包到一个可移植的容器中，然后发布到任何流行的 Linux 机器上，也可以实现虚拟化。
**2.Rocket**
Rocket (也叫 rkt)是 CoreOS 推出的一款容器引擎，和 Docker 类似，帮助开发者打包应用和依赖包到可移植容器中，简化搭环境等部署工作。
**3.Ubuntu(LXC)**
LXD 是 ubuntu 基于 LXC 技术的重构，容器天然支持非特权和分布式。LXD 与 Docker 的思路不同，Docker 是 PAAS，LXD 是 IAAS。LXC 项目由一个 Linux 内核补丁和一些 userspace 工具组成。这些 userspace 工具使用由补丁增加的内核新特性，提供一套简化的工具来维护容器。
**(二)配置管理**
**1.Chef**
Chef 是一个系统集成框架，为整个架构提供配置管理功能。
**2.Puppet**
Puppet，您可以集中管理每一个重要方面，您的系统使用的是跨平台的规范语言，管理所有的单独的元素通常聚集在不同的文件，如用户， CRON作业，和主机一起显然离散元素，如包装，服务和文件。
**3.CFengine**
Cfengine(配置引擎)是一种 Unix 管理工具，其目的是使简单的管理的任务自动化，使困难的任务变得较容易。Cfengine 适用于管理各种环境，从一台主机到上万台主机的机群均可使用。
**4.Bash**
Bash 是大多数Linux系统以及Mac OS X v10.4默认的shell，它能运行于大多数Unix风格的操作系统之上，甚至被移植到了Microsoft Windows上的Cygwin系统中，以实现windows的POSIX虚拟接口。此外，它也被DJGPP项目移植到了MS-DOS上。
**5.Rudder**
Rudder 已改名为Flannel，为每个使用 Kubernetes 的机器提供一个子网。也就是说 Kubernetes 集群中的每个主机都有自己一个完整的子网，例如机器 A 和 B 可以有 10.0.1.0/24 和 10.0.2.0/24 子网。
**Powershell  **
**6.RunDeck**
RunDeck 是用 Java/Grails 写的开源工具，帮助用户在数据中心或者云环境中自动化各种操作和流程。通过命令行或者web界面，用户可以对任意数量的服务器进行操作，大大降低了对服务器自动化的门槛。
**7.Saltstack**
Saltstack 可以看做是func的增强版+Puppet的弱化版。使用Python编写。非常好用，快速可以基于EPEL部署。Salt 是一个开源的工具用来管理你的基础架构，可轻松管理成千上万台服务器。
**8.Ansible**
Ansible 提供一种最简单的方式用于发布、管理和编排计算机系统的工具，你可在数分钟内搞定。Ansible 是一个模型驱动的配置管理器，支持多节点发布、远程任务执行。默认使用 SSH 进行远程连接。无需在被管理节点上安装附加软件，可使用各种编程语言进行扩展。
**(三)微服务平台**
**1.OpenShift**
OpenShift 是由红帽推出的一款面向开源开发人员开放的平台即服务(PaaS)。 OpenShift通过为开发人员提供在语言、框架和云上的更多的选择，使开发人员可以构建、测试、运行和管理他们的应用。
**2.Cloud Foundry**
Cloud Foundry 是VMware于2011年4月12日推出的业界第一个开源PaaS云平台，它支持多种框架、语言、运行时环境、云平台及应用服务，使开发人员能够在几秒钟内进行应用程序的部署和扩展，无需担心任何基础架构的问题。
**3.Kubernetes**
Kubernetes 是来自 Google 云平台的开源容器集群管理系统。基于 Docker 构建一个容器的调度服务。该系统可以自动在一个容器集群中选择一个工作容器供使用。其核心概念是 Container Pod。
**4.Mesosphere**
Apache Mesos 是一个集群管理器，提供了有效的、跨分布式应用或框架的资源隔离和共享，可以运行Hadoop、MPI、Hypertable、Spark。
**(四)服务开通**
**1.Puppet**
Puppet，您可以集中管理每一个重要方面，您的系统使用的是跨平台的规范语言，管理所有的单独的元素通常聚集在不同的文件，如用户， CRON作业，和主机一起显然离散元素，如包装，服务和文件。
**Razor**
**2.Docker Swarm**
Docker Swarm 是一个Dockerized化的分布式应用程序的本地集群，它是在Machine所提供的功能的基础上优化主机资源的利用率和容错服务。具体来说，Docker Swarm支持用户创建可运行Docker Daemon的主机资源池，然后在资源池中运行Docker容器。Docker Swarm可以管理工作负载并维护集群状态。
**3.Vagrant**
Vagrant 是一个基于 Ruby 的工具，用于创建和部署虚拟化开发环境。它使用 Oracle 的开源 VirtualBox 虚拟化系统，使用 Chef 创建自动化虚拟环境。
**4.Powershell**
**5.OpenStack Heat**
**五、维护**
日志记录
**1.Logstash**
Logstash 是一个应用程序日志、事件的传输、处理、管理和搜索的平台。你可以用它来统一对应用程序日志进行收集管理，提供 Web 接口用于查询和统计。
**2.CollectD**
collectd 是一个守护(daemon)进程，用来收集系统性能和提供各种存储方式来存储不同值的机制。比如以RRD 文件形式。
**3.StatsD**
StatsD 是一个简单的网络守护进程，基于 Node.js 平台，通过 UDP 或者 TCP 方式侦听各种统计信息，包括计数器和定时器，并发送聚合信息到后端服务，例如 Graphite。
**六、监控，警告&分析**
**1.Nagios**
Nagios 是一个监视系统运行状态和网络信息的监视系统。Nagios能监视所指定的本地或远程主机以及服务，同时提供异常通知功能等。
**2.Ganglia**
Ganglia 是一个跨平台可扩展的，高性能计算系统下的分布式监控系统，如集群和网格。它是基于分层设计，它使用广泛的技术，如XML数据代表，便携数据传输，RRDtool用于数据存储和可视化。
**3.Sensu**
Sensu 是开源的监控框架。主要特性：高度可组合;提供一个监控代理，一个事件处理器和文档 APIs;为云而设计;Sensu 的现代化架构允许监控大规模的动态基础设施，能够通过复杂的公共网络监控几千个全球分布式的机器和服务;热情的社区。
**4.zabbix**
zabbix 是一个基于Web界面的提供分布式系统监视以及网络监视功能的企业级的开源解决方案。
**5.ICINGA**
ICINGA 项目是 由Michael Luebben、HendrikB?cker和JoergLinge等人发起的，他们都是现有的Nagios项目社区委员会的成员，他们承诺，新的开源项目将完全兼容以前的Nagios应用程序及扩展功能。
**6.Graphite**
Graphite 是一个用于采集网站实时信息并进行统计的开源项目，可用于采集多种网站服务运行状态信息。Graphite服务平均每分钟有4800次更新操作。
**7.Kibana**
Kibana 是一个为 Logstash 和 ElasticSearch 提供的日志分析的 Web 接口。可使用它对日志进行高效的搜索、可视化、分析等各种操作。
以上，如果有其他补充可以在评论中跟大家分享哦！
