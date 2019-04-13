
# Kubernetes的未来是虚拟机？ - 喜欢打酱油的老鸟 - CSDN博客


2019年01月01日 20:48:21[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：146


[https://www.toutiao.com/a6639707449186058755/](https://www.toutiao.com/a6639707449186058755/)
2018-12-28 00:15:59
Kubernetes作为一项技术对我今年的职业生涯非常重要，明年也会如此。随着2018年即将结束，斗胆的预测一下， Kubernetes的未来是虚拟机，而不是容器。
**Kubernetes的未来是虚拟机，而不是容器**
2018年按照中国的十二生肖叫做狗年，技术上是Kubernetes的一年。现在有很多人都在学习Kubernetes，各地的CIO们都在努力制定“Kubernetes战略”，一些组织已经在把一些线上服务跑在Kubernetes的环境下。
换句话说，就像Gartner Hype Cycle说描述。任何一项新技术都是启动、高峰、低谷阶段，Kubernetes也延着这种趋势在发展。
![Kubernetes的未来是虚拟机？](https://img-blog.csdnimg.cn/201812290858258)
我是容器的忠实粉丝，我不会试图暗示容器已经死了。 Docker在2013以linux容器的概念闯入我们工作中， 他们向我们展示了一种构建，打包，共享和部署应用程序的令人惊喜的新方法，而这个时期正是我们认真考虑如何持续交付的时候。 他们的模型非常适合现代交付管道以及PaaS和后来的CaaS平台的出现。
![Kubernetes的未来是虚拟机？](https://img-blog.csdnimg.cn/2018122908582551)
在Google工作的工程师看到技术社区终于为容器做好了准备。 Google已经在很长一段时间内使用（或多或少发明）容器，他们开始构建Kubernetes，它构建在前身Borg平台基础上，同时为社区贡献力量。
没过多久，大的公有云厂商就提供了基于Kubernetes的平台（GKE，AKS，EKS），私有云也很快建立了基于Kubernetes的平台（Pivotal Container Service，Openshift等）。
**柔性多租户**
有一个棘手的问题需要解决：多租户，这个能否解决的好，将决定容器的未来。
Linux容器并非构建为安全的隔离沙箱（如Solaris Zones或FreeBSD Jails）。相反，它们建立在共享内核模型的基础上，该模型利用内核功能提供基本的进程隔离。正如Jessie Frazelle所说：“容器不是一切”。
更复杂的是，大多数Kubernetes组件也绝非都是安全隔离的。 当然你有命名空间和Pod安全策略，但API本身不具备。 内部组件像kubelet或kube-proxy也不具备， 这导致Kubernetes只能适合“软租赁”模型。
![Kubernetes的未来是虚拟机？](https://img-blog.csdnimg.cn/2018122908582595)
劣势，建立在容器之上的平台将继承容器的许多软多租户特效，而在硬件多租户虚拟机之上构建的平台都继承了硬性多租户特性（VMWare，Amazon Web Services，Openstack等）。
Kubernetes集群本身就是“硬性多租户”，这导致了“许多集群”而不是“一个大共享”集群的新兴模式。看到Google GKE服务的客户为多个团队部署了数十个Kubernetes集群并不罕见。通常每个开发人员都有自己的集群，这种行为会导致令人震惊的Kubesprawl数量。
**“这种行为会导致令人震惊的Kubesprawl”**
通常，您获得的最小集群是4台计算机（或VM）。 Kubernetes Master的一个（或3个HA），Kubernetes Worker的三个。这中系统性价比很低，大部分系统资源被闲置浪费。
所以我们需要以某种方式将Kubernetes转移到硬性多租户模型。 Kubernetes社区非常了解这一需求，并拥有一个多租户工作组。这个小组一直在努力解决这个问题，他们有几个建议的模型和建议如何解决每个模型。
**只需制作针对速度优化的小型虚拟机**......
Kata Containers是一个开源项目和社区，致力于构建轻量级虚拟机（VM）的标准实现，感知和执行类似容器，但提供VM的工作负载隔离和安全优势。
Jessie建议使用VM容器技术，例如Kata Containers。 Kata Containers结合了虚拟机级别隔离，可以像容器一样执行。这允许Kubernetes在嵌套的VM容器（在底层IaaS提供的VM内运行的VM容器）中运行的每个租户（我们假定每个命名空间的租户）提供自己的一组Kubernetes系统服务。
![Kubernetes的未来是虚拟机？](https://img-blog.csdnimg.cn/20181229085825223)
这是Kubernetes多租户的优雅解决方案。 她的建议甚至进一步表明Kubernetes使用嵌套的容器虚拟机来运行Kubernetes上的工作负载（Pod），从而大大提高了资源利用率。
我们在这里至少还有一个优化。 为底层IaaS或云提供商构建合适的管理平台。 如果VM容器是IaaS提供的第一级抽象，那么我们甚至可以进一步提高资源利用率。 运行Kubernetes集群所需的最小VM数量下降到一个（或三个HA）以承载暴露给“超级用户”的Kubernetes控制平面。
**资源（成本）优化多租户**
具有两个名称空间的Kubernetes部署都运行了许多应用程序，看起来像这样。
![Kubernetes的未来是虚拟机？](https://img-blog.csdnimg.cn/20181229085825269)
最初，部署到云的基础设施为零，因此超级用户的成本为零。
超级用户从云端请求Kubernetes集群。云提供商为运行主Kubernetes API和系统服务创建单个Container VM（或3个用于HA）。超级用户可以选择在系统命名空间中部署pod，或者创建新的命名空间以委派对其他用户的访问权限。
超级用户创建两个命名空间foo和bar。 Kubernetes为每个命名空间的控制平面（Kubernetes API和系统服务）从云中请求两个VM容器。超级用户将访问这些命名空间的用户委派给每个部署一些pod的用户，他们各自的控制平面为每个pod载请求VM容器。
在此的所有阶段，超级用户仅支付实际消耗的资源。云提供商拥有云的任何用户可用的容量。
**我实际上并没有在这里说任何新的东西**......
云提供商已经在努力实现这一未来。您可以通过观察开源社区中发生的事情来看到这种情况。 （可以说亚马逊已经与Fargate已经私下再做了这件事）。
第一个提示是Virtual Kubelet，它是一个开源工具，旨在伪装成一个kubelet。它将Kubernetes连接到其他API。这将允许Kubernetes从Cloud的Container VM调度程序请求Container VM。
其他提示包括新兴VM容器技术的数量，已经提到的Kata容器，还有来自亚马逊的Firecracker和来自Google的gvisor。
**结论**
与Kubernetes硬性多租户模式相结合，您将获得Kubernetes的成功密钥。完全隔离Kubernetes工作负载和纯每个Pod消耗成本模型，以在Kubernetes上运行工作负载。
对于那些不在公共云上的人，我们没有获得与基础设施提供商（在这种情况下是您自己）的容量负担相同的消费模型。您仍然可以获得更高资源利用率的好处，这可以降低容量需求。
让我们希望VMWare和Openstack正在关注并为我们带来基于轻量级VM容器技术的虚拟机管理程序和适当的Virtual Kubelet实现。

