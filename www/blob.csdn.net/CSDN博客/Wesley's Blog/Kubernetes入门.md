
# Kubernetes入门 - Wesley's Blog - CSDN博客


2018年07月17日 22:03:38[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：66标签：[IBM																](https://so.csdn.net/so/search/s.do?q=IBM&t=blog)[云技术																](https://so.csdn.net/so/search/s.do?q=云技术&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=IBM&t=blog)个人分类：[云技术																](https://blog.csdn.net/yinanmo5569/article/category/7808067)



# 简介
是一个全新的基于容器技术的分布式架构领先方案。是Google Borg的开源版本。Google Borg是谷歌内部使用的**大规模集群管理系统，它基于容器技术，实现资源管理自动化，以及跨多个数据中心的资源利用率的最大化**
Kubernetes(简称k8s)简化了负载均衡，部署实施的问题，不必再引入或者自己开发一个复杂的服务治理框架。
K8s是一个完备的分布式系统支撑平台，有完备的集群管理能力，也提供了完善的管理工具，包含了开发，部署测试，运维监控在内的各个环节。k8s是一个一站式的完备的分布式系统开发和支撑平台。
# Why Kubernetes?
“轻装上阵”开发复杂系统
全面拥抱微服务架构：微服务架构核心是将一个巨大的单体应用分解为很多小的互相连接的微服务
易于“搬迁”到公有云上
超强的横向扩容能力
# Kubernetes基本概念&术语
## Master
负责整个的管理和控制，运行着一下一组关键进程：
Kubernetes API Server (kube-apiserver): k8s所有资源的增删改查唯一入口
Kubernetes Controller Manager (kube-controller-manager): 所有资源对象的自动化控制中心
Kubernetes Scheduler (kube-scheduler): Pod调度进程
## Node
集群中的工作负载节点，运行着以下一组关键进程：
kubelet: 负责Pod对应容器的创建、启停，与Master密切协作
kube-proxy：实现通信与负载均衡的重要组件
Docker Engine：负责容器创建和管理
## Pod
Pod运行在节点（Node）上，节点可以是物理机也可以是虚拟机，通常一个节点有几百个Pod；每个Pod中运行着一个Pause容器，其他容器则是业务容器，这些业务容器共享Pause容器的网络栈和Volume挂载卷，它们之间的通信和数据交换更为高效。
Pod是k8s管理的最小运行单元。
Pod是k8s最重要最基本的概念。**Pod由一个Pause容器（根容器）和一个或多个用户业务容器组成**。为什么会设计出一个全新的Pod概念?
容器作为一个单元的情况下，难以对“整体”简单地进行判断及有效进行行动。比如，一个容器死亡算整体死亡吗？
Pod里的多个业务容器共享Pause容器的IP，共享Pause容器挂在的Volume，简化了关系密切的业务容器之间的通信问题和文件共享问题。
每个Pod分配了唯一的IP地址，称为PodIP，**一个Pod里的容器可以直接与另外一台主机上的Pod容器直接通信。**
Pod分为普通Pod和Static Pod，后者不存放在k8s的etcd中，而存放在Node某个具体文件上，并且只在此Node上启动运行。
默认情况下，当Pod里的某个容器停止时，会自动检测到这个问题并且重新启动这个Pod，如果Pod所在的Node宕机，则会将这个Node上所有的Pod重新调度到其他Node上。
## Replication Controller (RC)
声明某种Pod的副本数量在任意时刻都符合某个预期值，所以RC的定义包括以下几个部分：
Pod期待的副本数
用于筛选目标Pod的Label Selector
当Pod副本数小于预期时，用于创建新Pod的Pod模板（template）
RC的特性与作用：
大多数情况下，通过定义一个RC实现Pod的创建过程及副本数量的自动控制
RC里包括完整的Pod定义模板
RC通过Label Selector机制实现对Pod副本的自动控制
可以实现Pod的扩容缩容，滚动升级等功能
## Deployment
Deployment和Replica Set时RC的一次升级。
典型的使用场景：
创建一个Deployment对象来生成对应的Replica Set并完成Pod副本创建过程
检查Deployment状态查看部署是否完成
更新Deployment以创建新的Pod
回滚，扩展，查看状态等
## Horizontal Pod Autoscaler
HPA与RC， Deployment一样，也是一个k8s资源对象。通过监视分析RC控制Pod的负载变化情况。来自动扩容的工具
## Service
在k8s中，Service是分布式集群架构的核心，一个服务具有以下关键特征：
唯一指定的名字（例如mysql-server）
拥有一个虚拟IP（Cluster IP， Service IP或VIP）和端口号
能够提供某种远程服务能力
被映射到提供这种服务能力的一组容器应用上
Kubernetes由“三种IP”：
Node IP: Node的IP地址(每个节点物理网卡的IP地址)
Pod IP: Pod的IP地址(Docker Engine根据docker0网桥的IP地址段进行分配的）
Cluster IP: Service的IP地址
如果外部应用或用户需要使用集群的服务，可以采用NodePort解决这个问题。 可在Service定义里做出扩展，定义nodePort，这样k8s会自动分配一个可用的端口。但是这个没有解决外部访问的所有问题，但是可以通过将type=NodePort改为type=LoadBalancer，此时k8s会自动创建一个Load Balancer实力。
## Volume（存储卷）
Volume是Pod中能被多个容器访问的共享目录。k8s的Volume与Docker的Volume比较类似。
k8s由以下几种Volume类型：
emptyDir
hostPath
gcePersistentDisk
awsElastiBlockStore
其他类型的Volume
## Persistent Volume
PV可以理解成k8s中某个网络存储中对应的一块存储，与Volume类似，但是有以下区别：
PV只能是网络存储，不属于任何Node
PV并不是定义在Pod上的，而是独立于Pod之外定义

