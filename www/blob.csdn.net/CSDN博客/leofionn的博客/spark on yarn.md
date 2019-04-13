
# spark on yarn - leofionn的博客 - CSDN博客


2018年03月22日 14:24:05[leofionn](https://me.csdn.net/qq_36142114)阅读数：244


spark on yarn 主要是借助于yarn良好的资源管理机制。将Spark Application部署到yarn种，有如下优点：
（1）部署Application和服务更加方便
只需要yarn服务，包括Spark，Storm在内的多种应用程序不要要自带服务，它们经由客户端提交后，由yarn提供的分布式缓存机制分发到各个计算节点上
（2）资源隔离机制
yarn只负责资源的管理和调度，完全由用户和自己决定在yarn集群上运行哪种服务和Applicatioin，所以在yarn上有可能同时运行多个同类的服务和Application。Yarn利用Cgroups实现资源的隔离，用户在开发新的服务或者Application时，不用担心资源隔离方面的问题。
（3）资源弹性管理
Yarn可以通过队列的方式，管理同时运行在yarn集群种的多个服务，可根据不同类型的应用程序压力情况，调整对应的资源使用量，实现资源弹性管理。
yarn运行流程图
![](//img-blog.csdn.net/2018032214235342)

Spark on Yarn模式又由Cluster和Client模式
![](//img-blog.csdn.net/20180322142120260)

在Yarn-cluster模式下，driver运行在Appliaction Master上，Appliaction Master进程同时负责驱动Application和从Yarn中申请资源，该进程运行在Yarn container内，所以启动Application Master的client可以立即关闭而不必持续到Application的生命周期

Yarn-cluster模式下作业执行流程：
1. 客户端生成作业信息提交给ResourceManager(RM)
2. RM在某一个NodeManager(由Yarn决定)启动container并将Application Master(AM)分配给该NodeManager(NM)
3. NM接收到RM的分配，启动Application Master并初始化作业，此时这个NM就称为Driver
4. Application向RM申请资源，分配资源同时通知其他NodeManager启动相应的Executor
5. Executor向NM上的Application Master注册汇报并完成相应的任务

Yarn-client
在Yarn-client中，Application Master仅仅从Yarn中申请资源给Executor，之后client会跟container通信进行作业的调度，下图是Yarn-client模式
![](https://images2015.cnblogs.com/blog/776259/201609/776259-20160909165804926-17816733.png)
Yarn-client模式下作业执行流程：
1. 客户端生成作业信息提交给ResourceManager(RM)
2. RM在本地NodeManager启动container并将Application Master(AM)分配给该NodeManager(NM)
3. NM接收到RM的分配，启动Application Master并初始化作业，此时这个NM就称为Driver
4. Application向RM申请资源，分配资源同时通知其他NodeManager启动相应的Executor
5. Executor向本地启动的Application Master注册汇报并完成相应的任务

下表是Spark Standalone与Spark On Yarn模式下的比较
![](https://images2015.cnblogs.com/blog/776259/201609/776259-20160909165822723-1513641104.png)

【来自@若泽大数据】

