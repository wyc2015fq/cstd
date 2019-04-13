
# IBM Cloud Private (ICP)： Overiew - Wesley's Blog - CSDN博客


2018年07月17日 11:19:08[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：399


[IBM® Cloud Private overview](#ibm-cloud-private-overview)
[IBM® Cloud Private bundles](#ibm-cloud-private-bundles)[IBM Could Private Cloud Native](#ibm-could-private-cloud-native)
[IBM Could Private Enterprise](#ibm-could-private-enterprise)
[其他可选组件](#其他可选组件)
[IBM® Cloud Private charts](#ibm-cloud-private-charts)[核心服务](#核心服务)
[开源软件](#开源软件)
[Architecture](#architecture)[Boot node](#boot-node)
[Master node](#master-node)
[Worker node](#worker-node)
[Proxy node](#proxy-node)
[Management node](#management-node)
[VA node](#va-node)
[etcd node](#etcd-node)
[IBM® Cloud Private components](#ibm-cloud-private-components)


# IBM® Cloud Private overview
在使用ICP之前，最好先熟悉**Kubernetes, Docker, Helm**的工作原理：
[Helm Document](https://docs.helm.sh/)
[Docker Document](https://docs.docker.com/get-started/)
[Kubernetes Document](https://kubernetes.io/docs/tutorials/kubernetes-basics/)
ICP v2.1.0.3 具有以下功能特点：
简易安装
ICP管理console
私有Docker镜像管理
容器化软件和服务目录
隔离共享网络
稳定监控和logging
# IBM® Cloud Private bundles
所有的IBM私有云bundles包含核心ICP plateform和免费的可通过Catalog获取的apps。 每个bundle包含不同的可以分别安装的软件
## IBM Could Private Cloud Native
Could Automation Manager: 用于在多个云上部署云架构的工具
IBM WebSphere Application Server Liberty: 集成了丰富的中间件，例如数据库，缓存管理，信息方案
Microservice Builder: 提供部署和管理容器化apps的服务
## IBM Could Private Enterprise
Could Automation Manager: 用于在多个云上部署云架构的工具
IBM API Connect Professional: 提供综合的解决方案去管理你的整个API生命周期，从创建到管理
IBM MQ(Message Queue) Advanced：是一个稳定的消息传递中间件
IBM WebSphere Application Server Liberty: 集成了丰富的中间件，例如数据库，缓存管理，信息方案
Microservice Builder: 提供部署和管理容器化apps的服务
## 其他可选组件
IBM Cloud Private Cloud Foundry
Data Science Experience Local
IBM DB2 Direct Advanced
IBM UrbanCode Deploy
# IBM® Cloud Private charts
IBM charts repository:[GitHub地址](https://github.com/IBM/charts/tree/master/stable)
## 核心服务
|Services|Components|
|---|---|
|Monitoring|
|Prometheus|
|Logging|
|ELK (Elasticsearch, Logstash, Kibana)|
|Metering(测量)|
|Product Insights|
|Security|
|Identity and access|
|Management|
|Vulnerability Advisor|
## 开源软件
|Services|Components|
|---|---|
|Toolchains and runtimes|
|Jenkins|
|Open Liberty|
|Swift runtime|
|Node.js runtime|
|Messaging|
|RabbitMQ|
|Data services|
|MongoDB|
|PostgreSQL|
|Redis|
|MariaDB|
|Galera for mariaDB|
|HTTP servers|
|Nginx|
|Tools|
|Web terminal|
|Skydive|
# Architecture
ICP集群主要有四种节点：boot, master, worker 和 proxy
可以在集群中选择性地确定 management, Vulnerability Advisor(VA), etcd 节点
必须在ICP集群安装之前确定整体架构，在安装之后，就只能在集群中移除或增加worker, proxy, management, VA nodes节点了。
## Boot node
Boot node 用于安装，配置，升级集群，控制节点。一个集群只有一个boot node，可以多个集群共享一个boot node， boot node和master node可以同一个节点
## Master node
用于在集群中管理服务和worker nodes。 Master node 主持用于管理资源分配，状态管理，调度和监控的进程。 因为高可用(HA)环境包含多个master nodes， 如果运行主节点崩坏，将自动替换。
## Worker node
Worker node用于提供运行task的容器环境，至少需要有一个worker节点
## Proxy node
代理节点是用于传输外部request到集群内部服务的节点。因为高可用性(HA)环境通常包含多个proxy node（类似于主节点）。
![这里写图片描述](https://img-blog.csdn.net/20180717153714810?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## Management node
Management node是一个专门用于管理服务（monitoring, metering, logging)的可选节点。配置管理节点，你可以防止主节点超载。
如果使用了管理节点。
## VA node
VA(Vulnerability Advisor) node 是一个可选的用于运行Vulnerability Advisor服务的可选节点
## etcd node
etcd node 是一个用于运行 etcd 分布式key-value存储的节点， 需要在hosts file中配置etcd node。
# IBM® Cloud Private components
ICP有两个主要的组件：一个容器管理器（Docker）和一个容器协调器（Kubernetes)
ICP的其他组件围绕这两个主要组件提供服务，例如：验证，存储，网络，日志，监控等。也提供了一个集群管理console。
具体组件参考链接：[ICP Conponent](https://www.ibm.com/support/knowledgecenter/SSBS6K_2.1.0.3/getting_started/components.html)

