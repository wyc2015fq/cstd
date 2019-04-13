
# Kubernetes控制流分析 - Wesley's Blog - CSDN博客


2018年07月25日 08:50:10[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：1470


# 以部署一个应用为例
例如执行以下命令：
`kubectl run httpd-app --image=httpd --replicas=2`等待一段时间，应用部署完成.Kubernetes部署了deployment httpd-app，有两个副本Pod，分别运行在k8s-node1和k8s-node2。
整个部署过程分析：
![这里写图片描述](https://img-blog.csdn.net/20180725084909865?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
补充两点：
应用的配置和当前状态信息保存在etcd中，执行kubectl get pod时API Server会从etcd中读取这些数据。
flannel会为每个Pod都分配IP。因为没有创建service，所以目前kube-proxy还没有参与进来
![这里写图片描述](https://img-blog.csdn.net/20180725084922938?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# Kubernetes API Server
核心功能是提供了K8s各类资源对象（如Pod，RC,Service）的REST接口的增删改查
是集群管理的API入口
是资源配额控制的入口
提供了完备的集群安全机制
### 集群功能模块之间的通信
集群内的各个功能模块通过API Server将信息存入etcd，当需要获取和操作这些数据时，则通过API Server提供的REST接口（用GET， LIST或WATCH方法）来实现，从而实现各个模块之间的交互。
kubelet与API Server交互。每个Node节点上的kubelet每隔一个时间周期会调用API Server的REST接口报告自身状态，API Server收到这些信息后，将节点信息更新到etcd中。 kubelet也通过API Server的Watch接口监听Pod信息（例如是否需要创建，删除）
Controller-Manager与Api Server的交互。 API Server提供的Watch接口，实时监控Node信息
Scheduler与API Server交互，通过Watch接口监听到新建的Pod副本信息后，执行Pod调度
# Contoller Manager
Controller Manager的主要类别如下，每种Controller都负责一种控制流程：
![这里写图片描述](https://img-blog.csdn.net/20180725084942487?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Replication Controller
保证任何时候集群中一个RC所关联的Pod副本书来给你保持预设值
NodeConroller
kubelet进程启动时通过API Server注册自身节点信息，定时向API Server汇报状态信息，API Server接收到这些信息后，将信息更新到etcd中。
Node Controller通过API Server实时获取Node的相关信息，实现管理和监控集群中各个Node节点相关控制功能
ResourceQuota Controller
ResourceQuota Controller（资源配额管理）确保指定的资源对象在任何时候都不会超量占用系统物理资源
Namespace Controller
用户可通过API Server创建新的Namespace并保存在etcd中，Namespace Controller定时通过API Server读取这些Namespace信息。
Service Controller
Service Controller是属于k8s集群与外部云平台之间的一个接口控制器，它监听Service的变化，如果是一个LoadBalance类型的Service，则Service Controller确保外部云平台上该Service对应的LoadBalancer实例被相应地创建删除及及时更新路由转发表
Endpoint Controller
Endpoints表示一个Service对应的所有Pod副本的访问地址，Endpoint Controller就是负责生成和维护所有Endpoints对象的控制器。Endpoint在每个Node上的kube-proxy进程被使用，kube-proxy进程获取每个Service的Endpoints，实现了Service的负载均衡功能。
Scheduler
主要承担了“承上启下”地功能，“承上”是指它负责接收Controller Manager创建的新Pod，并安排目标Node，“启下”是指安置工作完成后，目标Node上的kubelet服务进程接管后继工作。并将绑定信息写入etcd中
kubelet运行机制
节点管理：向API Server注册时自己
Pod管理：获取自身Node上要运行Pod清单，做对应的操作
容器健康检查
cAdvisor资源监控

