# yarn初识 - Soul Joy Hub - CSDN博客

2016年07月17日 20:55:38[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：599


> 
用自己的话表述YARN和MR之间的区别和联系

YARN是一个资源管理和作业框架，MR是计算框架 

但。MR1中，JobTracker作为核心，管理集群中的每一台机器以及所有的job分配，需要很大的资源消耗，并存在单点故障。MR2以YARN作为资源和作业管理系统，把JobTracker所做的工作拆成两部分，一个是资源管理器ResourceManager，负责所有任务的资源管理和分配，一个是任务调度器ApplicationMaster，负责应用程序的管理和监控。这样将任务和资源分离，大大减少了MR1中JobTracker的资源消耗，同时，对任务的监控交由ApplicationMaster，这样可以分布式化，避免了单点故障问题。

> 
名词解释： RM、NM、AM

- 
RM

ResourceManager，承担资源分配功能，为不同的任务分配资源，决定资源分配策略等。

- 
NM

NodeManager,主要管理单个计算节点，与ResourceManager通信。

- 
AM

ApplicationMaster，承担作业控制功能，向调度器申请资源，并对作业进行跟踪监控。

当客户向RM提交 作业时，由AM负责向RM提出资源申请，和向NM提出task执行 。在这个过程中，RM负责资源调度，AM 负责任务调度。RM负责整个集群的资源管理与调度；NM负责单个节点的资源管理与调度；NM定时的通过心跳的形式与RM进行通信，报告节点的健康状态与内存使用情况；AM通过与RM交互获取资源，然后然后通过与NM交互，启动计算任务。

> 
简述TDH产品中InceptorServer在YARN上的默认资源分配策略（以8180 TranswarpManager界面上的InceptorSQL组件里面的“资源配置”列举的默认配置为例介绍）

Application Master内存:512MB

Executor资源 Ratio

Memory : core比例: 0.18229167

core百分比:0.501

inceptor.yarn.executor.cores=3，core百分比=0.501，得每个executor运行占1.5个core,整为2。

内存=3×0.18229167=0.54687501，分配默认为1GB

每个worker上只运行一个executor； 

每个worker上都运行executor； 

每个worker节点都运行一个executor。

