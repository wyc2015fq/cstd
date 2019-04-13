
# Kubernetes基础与架构 - 喜欢打酱油的老鸟 - CSDN博客


2019年01月16日 08:09:41[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：25标签：[Kubernetes																](https://so.csdn.net/so/search/s.do?q=Kubernetes&t=blog)个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)


[https://www.toutiao.com/a6645132278605283848/](https://www.toutiao.com/a6645132278605283848/)
2019-01-12 09:00:00
```python
作者：Forezp；
来源：方志朋
```
Kubernetes是一个可移植，可扩展，强大的的开源平台，用于管理容器化工作负载和服务，有助于声明性配置和自动化。 它拥有庞大，快速发展的生态系统。 Kubernetes服务，支持和工具广泛可用。
谷歌在2014年开放了Kubernetes项目.Kubernetes建立在谷歌大规模运行生产工作量的十五年经验基础上，结合了社区中最佳的创意和实践。
K8s架构图
![Kubernetes基础与架构](http://p1.pstatp.com/large/pgc-image/638365b1d2f649d9b9f4eb48f3496e11)

Kubernetes是由一组节点组成，这些节点可以是物理主机，也可以是虚拟机。Kubernetes平台运行这些节点之上，构成了集群。Kubernetes集群分为两种Node，一种Master Node和用于实际部署的Pod的工作Node。一个集群往往由多个Master Node，这几个Master Node控制着整个集群的所有Node。
master节点包含以下的组件：
etcd 用于存储集群中所有对象产生的数据
api server 唯一可以和etcd 数据库交互的组件，通过通过kubectl调用api server 的api。
controller manager 管理资源对象
schedualer 调度Node，发布Pod
Node节点包含以下的组件
kubelete 用于和Master节点交互，运行了node上的Service进程
kube-proxy 负责网络相关，用于服务的负载
Master组件
Master节点上集群的大脑，是集群逻辑上的控制中心。
Master组件起到控制整个集群的作用。 Master组件做出关于群集的全局决策（例如调度），以及检测和响应群集事件（当rc控制器的'副本'字段不满足时启动新的pod）。
Master组件可以在群集中的任何计算机上运行。 但是，为简单起见，通常Mater组件在同一台计算机上运行。
下图是K8s的master节点的架构图，从图中可以看出，master节点爆款四个组件，分别为Etcd、Api Server、kube-scheduler、Controller。它们之间互相协作，共同控制着k8s集群。
![Kubernetes基础与架构](http://p1.pstatp.com/large/pgc-image/e88bf51c191c44e192ffb72377b16588)

etcd
etcd是Coreos开源的（Apache 2.0协议）一个分布式键值存储数据库，它提供了一种在一组机器上存储数据的可靠方法。 etcd采用了master-slave架构，在网络分区期间优雅地处理Leader选举，并且可以容忍机器故障，包括Leader。
CoreOs另外开源了CoreDns组件，也是一个对于K8s非常的重要的组件，这在后续文章会详细介绍。
etcd是一致性高和高可用的键值存储，用作Kubernetes的所有群集数据的存储，包括各种K8s对象和k8s状态等数据。
Api Server
Api Server 是唯一可以操作etcd 数据库的组件，并提供了认证、授权等机制。它严格遵守了REST规范，去操作这些资源，具有CRUD特性。它是Kubernetes控制的前端工程，它能够水平扩展，可以通过部署多个实例来达到高可用的目的。
kube-scheduler
通过API Server的watch接口监听新建Pod副本信息，并通过算法为该pod选择一个合适的node。调度器可用选择合适的策略，策略的考虑包括个人和集体资源的情况，软件、硬件条件的影响，亲和性和反亲和性的规范等因素。调度器是可插拔的，并且我们期待支持多集群的调度，未来甚至希望可以支持用户自定义的调度器。
controller-manager
集群内各种资源controller 的核心管理者。针对于每一种具体的资源，都有相应的Controller，保证其下管理的每个Controller所对应的资源始终处于“期望的状态”。
从逻辑上讲，每个控制器都是一个单独的过程，但为了降低复杂性，它们都被编译成单个二进制文件并在单个进程中运行。
这些控制器包括：
Node controller：负责在节点出现故障时注意和响应。
Replication Controller：负责为系统中的Replication Controller对象维护正确数量的pod。
Endpoint Controller：填充Endpoint Controller象（即，连接服务和窗格）。
Service Account & Token Controllers：为新的命名空间创建默认帐户和API访问令牌。
Node组件
Node是K8s集群中真正的工作负载节点，K8s集群由多个Node共同承担工作负载，pod被分配到某个具体的node上执行。k8s通过node controller对node资源进行管理。支持动态在集群中添加或删除node。
kubelet
它位于集群中每个 node上非容器形式的服务进程组件，是Master和node之间的桥梁。处理Master下发到本Node上的pod创建、启停等管理任务，向API server注册Node信息。监控本Node上容器和节点资源情况，并定期向Master汇报节点资源占用情况。它还会向Master汇报容器运行的健康情况。
Kube-proxy
Service抽象概念的实现，将到service的请求按策略（负载均衡算法分发到后端的pod ）上，默认使用iptables mode实现；支持nodeport模式，实现从外部访问集群内的service。
总结
kubernetes组件由两类节点组成，分别为master和node。
其中master组件是控制整个集群大脑，它包括apiserver、scheduler、controller manager、etcd组件。apiserver为外部控制集群提供了REST API，并提供了安全机制；etcd是k8s记录各种资源的唯一数据库，只有api server能够直接访问;controller manager用来管理controller，保证了每种资源的正常工作；schedula是用来调度pod 到具体的节点。
而node节点包含了2个组件，分别为kubelete和kubeproxy，kubelete是pod管家，管理着pod的生命周期，与此同时与api server保持联系，实时上报pod的数据；kubeproxy组件实现了 service的抽象，使应用能够被访问。
由此可见,kubernetes集群是由众多的节点组成，每一个节点又有k8s的组件，这些组件相互协作，共同维护，保证集群的正常运行。
参考资料
书籍《k8s权威指南》
https://kubernetes.io/docs/concepts/overview/components/\#master-components
https://www.kubernetes.org.cn/kubernetes%E8%AE%BE%E8%AE%A1%E6%9E%B6%E6%9E%84
https://www.imooc.com/learn/978

