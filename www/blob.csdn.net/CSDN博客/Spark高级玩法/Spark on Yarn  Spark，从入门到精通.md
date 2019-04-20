# Spark on Yarn | Spark，从入门到精通 - Spark高级玩法 - CSDN博客
2018年09月10日 00:02:44[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：141

** / 为什么需要 Yarn？ / **
Yarn 的全称是 Yet Anther Resource Negotiator（另一种资源协商者）。它作为 Hadoop 的一个组件，官方对它的定义是**一个工作调度和集群资源管理的框架**。
Yarn 最早出现于 Hadoop 0.23 分支中，0.23 分支是一个实验性分支，之后经过了几次迭代，最后发布于 2014 年 6 月的 0.23.11 版本(该分支的最后一个版本)。在 0.23.0 发布后不久的 2011 年 12 月，Hadoop 的 0.20 分支发展成了 Hadoop1.0，一直到 1.0 的最后一个版本 1.2.1-stable 都没有出现 Yarn 的身影，而在 Hadoop2.0 的第一个版本 2.0.0-alpha，Yarn 已经作为一个正式组件加入。在 2.0.2-alpha 版本，它已经支持了 2k 台机器的集群，接着在 2.0.3-alpha 版本中已经可以支持 30k 台机器的集群。在 2.0.3-alpha 版本中同时还支持了多种资源，如 cpu&memory 的调度和 ResourceManager restart。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/iaicWqh8rjVrwENSewC9P7icUN62IYvTkcNQXoKIpIYBO0fcy9YfTia870G7WL9P7zmvaiaAfnwvj6yBaicX6K48RibAA/640)
图 1，via https://blog.csdn.net/suifeng3051/article/details/49364677
如图 1 所示， Hadoop1.0 的运作流程如下：
1.客户端提交任务给集群；
2.JobTracker 接收 Job 请求；
3.JobTracker 根据 Job 的输入参数向 NameNode 请求包含这些文件数据块的 DataNode 节点列表；
4.JobTracker 确定 Job 的执行计划：确认 Map、Reduce 的 Task 数量，并分配 Task 到离数据块最近的节点上执行。
最初，Hadoop1.0 能够很好地支撑大数据计算，但是随着计算规模的扩大和计算模型的多样化，它逐渐力不从心。众所周知当集群性能不足的时候可以简单粗暴地加机器，但 JobTracker 同时部署多个时只有一个是处于 active 状态，因此受限于这个 active JobTracker 的负载上限，整个集群能够容纳的机器也有限，有数据显示整个集群的管理上限约为 4k 台机器。同时应用程序相关和资源管理相关的逻辑全部放在 JobTracker中，当集群规模扩大的时候，会存在一个瓶颈。除此之外，Map-Reduce 计算模型与 JobTracker 的耦合过高，其他计算模型难以在 Hadoop1.0 上运行。
Yarn 是 Hadoop 基于这些问题的一个解决方案，接下来通过了解 Yarn 的组件、架构以及运作机制来分析 Yarn 是如何解决这些问题的。
** / Yarn 是什么？ / **
**Yarn 的组件&基本架构**
如图 2 所示 Yarn 采用 Master/Slave 结构，整体采用双层调度架构。在第一层的调度是 ResourceManager 和 NodeManager：ResourceManager 是 Master 节点，相当于 JobTracker，包含 Scheduler 和App Manager 两个组件，分管资源调度和应用管理；NodeManager 是 Slave 节点，可以部署在独立的机器上，用于管理机器上的资源。NodeManager 会向 ResourceManager 报告它的资源数量、使用情况，并接受 ResourceManager 的资源调度。
*ResourceManager 同 JobTracker 一样可以多机部署，并且只有一台处于 active 状态。但是在 ResourceManager 中将调度管理和应用管理作了拆分，两个组件的功能更专一。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/iaicWqh8rjVrzD95icoEUPZVjLur0WmEKHmOyFpLy0hQGXc9l8eC5QxguM0P5TNqswcCaBkfSNPxWSNCQvJt3jpcQ/640)
图 2
第二层的调度指的是 NodeManager 和 Container。NodeManager 会将 Cpu&内存等资源抽象成一个个的 Container，并管理它们的生命周期。
通过采用双层调度结构将 Scheduler 管理的资源由细粒度的 Cpu&内存变成了粗粒度的 Container，降低了负载。在 App Manager 组件中也只需要管理 App Master，不需要管理任务调度执行的完整信息，同样降低了负载。通过降低 ResourceManager 的负载，变相地提高了集群的扩展性。
**Yarn 运作流程**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/iaicWqh8rjVrzD95icoEUPZVjLur0WmEKHmJKk7le3Blz8ZM4V8UF888EicxMXqk5c68gLYSGpmDiaNEySVKhRLcpHQ/640)
图 3，via https://blog.csdn.net/suifeng3051/article/details/49486927
如图 3 所示 Yarn 的运作流程如下：
1.客户端向 ResourceManager 的 App Manager 提交应用并请求一个 AppMaster 实例；
2.ResourceManager 找到可以运行一个 Container 的 NodeManager，并在这个 Container 中启动 AppMaster 实例；
3.App Master 向 ResourceManager 注册，注册之后，客户端就可以查询 ResourceManager 获得自己 App Master 的详情以及直接和 App Master 交互；
4.接着 App Master 向 Resource Manager 请求资源，即 Container；
5.获得 Container 后，App Master 启动 Container，并执行 Task；
6.Container 执行过程中会把运行进度和状态等信息发送给 AppMaster；
7.客户端主动和 App Master 交流应用的运行状态、进度更新等信息；
8.所有工作完成 App Master 向 RM 取消注册然后关闭，同时所有的 Container 也归还给系统。
通过这个 Job 的处理过程可以看到 App Master 是作为 Job 的驱动角色，它驱动了 Job 任务的调度执行。在这个运作流程中，App Manager 只需要管理 App Master 的生命周期以及保存它的内部状态，而 App Master 这个角色的抽象使得每种类型的应用都可以定制自己的 App Master，这样其他的计算模型就可以相对容易地运行在 Yarn 集群上。
**Yarn HA（容灾备援）**
接下来介绍的是 Yarn 集群高可用中关于容错备援的设计。根据图 3 所示的 Yarn 架构图，假如 Container 故障 Resource Manager 可以分配其他的 Container 继续执行，当运行 App Master 的 Container 故障后也将分配新的 Container，App Master 可以从 App Manager 获取信息恢复。当 NodeManager 故障的时候系统可以先把这个节点移除，在其他 NodeManager 重启再继续任务。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/iaicWqh8rjVrwENSewC9P7icUN62IYvTkcN7sianlkianNJ5nFhZztrCuBZHOjWhfsiaqTTnf8sr9VBRdYN91LnPlJng/640)
图 4，via https://www.cnblogs.com/sodawoods-blogs/p/8715231.html
那么当 ResourceManager 故障的时候呢？如上文所说的在 Yarn 集群中，ResourceManager 可以启动多台，只有其中一台是 active 状态的，其他都处于待命状态。这台 active 状态的 ResourceManager 执行的时候会向 ZooKeeper 集群写入它的状态，当它故障的时候这些 RM 首先选举出另外一台 leader 变为 active 状态，然后从 ZooKeeper 集群加载 ResourceManager 的状态。在转移的过程中它不接收新的 Job，转移完成后才接收新 Job。
** / Sparkon Yarn / **
首先介绍 Spark 的资源管理架构。Spark 集群考虑到了未来对接一些更强大的资源管理系统（如 Yarn、Mesos 等）没有在资源管理的设计上对外封闭，所以Spark 架构设计时将资源管理抽象出了一层，通过这种抽象能够构建一种插件式的资源管理模块。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/iaicWqh8rjVrzD95icoEUPZVjLur0WmEKHm4x5aUjNZoD2jLsSr9icqyGoYzju6kpB2U4KLF41xxTWkD7vrvoXo2aA/640)
图 5，via http://shiyanjun.cn/archives/1545.html
如图 5 所示是 Spark 的资源管理架构图。Master 是 Spark 的 主控节点，在实际的生产环境中会有多个 Master，只有一个 Master 处于 active 状态。Worker 是 Spark 的工作节点，向 Master 汇报自身的资源、Executeor 执行状态的改变，并接受 Master 的命令启动 Executor 或 Driver。Driver 是应用程序的驱动程序，每个应用包括许多小任务，Driver 负责推动这些小任务的有序执行。Executor 是 Spark 的工作进程，由 Worker 监管，负责具体任务的执行。
以上就是 Spark 在资源管理上的抽象出来的架构，这个架构跟 Yarn 的架构十分相似，因此 Spark 很容易地构建于 Yarn 之上。在 Spark 和 Yarn 两边的角色对比中：Master 和 ResourceManager 对应，Worker 和 NodeManager 对应，Driver 和 App Master 对应，Executor 和 Container 对应。
根据 Spark 部署模式的不同资源管理架构也会有不同的形态。Spark 大致包括四种部署模式：
- 
**Local 模式**：部署在同一个进程上，只有 Driver 角色。接受任务后创建 Driver 负责应用的调度执行，不涉及 Master 和 Worker；
- 
**Local-Cluster 模式**：部署在同一个进程上，存在 Master 和 Worker 角色，它们作为独立线程存在于这个进程内；
- 
**Standalone 模式**：Spark 真正的集群模式，在这个模式下 Master 和 Worker 是独立的进程；
- 
**第三方部署模式**：构建于 Yarn 或 Mesos 之上，由它们提供资源管理。
接着看看 Spark on Yarn 对 Job 的处理过程。客户端提交一个任务给 Yarn ResourceManager 后，App Manager 接受任务并找到一个 Container 创建App Master，此时 App Master 上运行的是 Spark Driver。之后 App Master 申请 Container 并启动，Spark Driver 在 Container 上启动 Spark Executor，并调度 Spark Task 在 Spark Executor 上运行，等到所有的任务执行完毕后，向 App Manager 取消注册并释放资源。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/iaicWqh8rjVrzD95icoEUPZVjLur0WmEKHmg9Q3PExQaic0JGDh9Gwy6vGYxcbgScsicCqoj6H3ht2DIhJoZYx2mqmA/640)
图 6，via https://www.iteblog.com/archives/1223.html
可以看出这个执行流程和 Yarn 对一个任务的处理过程几乎一致，不同的是在 Spark on Yarn 的 Job 处理过程中 App Master、Container 是交由 Spark 相对应的角色去处理的。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/iaicWqh8rjVrzD95icoEUPZVjLur0WmEKHmicKDEYTNkXYeicgIzJmSmTAuSfL0KkqDlK5ib61CwOQkypdnQpSrZIJfw/640)
图 7，via https://www.iteblog.com/archives/1223.html
Spark on Yarn 还有另外一种运行模式：Spark on Yarn-Client。不同于上述的 Spark on Yarn-Cluster，Spark on Yarn-Client 的客户端在提交完任务之后不会将 Spark Driver 托管给 Yarn，而是在客户端运行。App Master 申请完 Container 之后同样也是由 Spark Driver 去启动 Spark Executor，执行任务。
那为什么使用 Yarn 作为 Spark 的资源管理呢？我们来对比 Spark 集群模式 Standalone 和 Spark on Yarn 在资源调度能力上的区别：Spark 的 Standalone 模式只支持 FIFO 调度器，单用户串行，默认所有节点的所有资源对应用都是可用的；而 Yarn 不止支持 FIFO 的资源调度，还提供了弹性和公平的资源分配方式。
Yarn 是通过将资源分配给 queue 来进行资源分配的，每个 queue 可以设置它的资源分配方式，接着展开介绍 Yarn 的三种资源分配方式。
**FIFOScheduler**
如果没有配置策略的话，所有的任务都提交到一个 default 队列，根据它们的提交顺序执行。富裕资源就执行任务，若资源不富裕就等待前面的任务执行完毕后释放资源，这就是 FIFO Scheduler 先入先出的分配方式。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/iaicWqh8rjVrzD95icoEUPZVjLur0WmEKHmjXHqMjyU8A0V33cf1mCrlUmkGXyfBZlibibbrT3KGCwaKhDTNbnic4JJg/640)
图 8，via https://blog.csdn.net/suifeng3051/article/details/49508261
如图 8 所示，在 Job1 提交时占用了所有的资源，不久后 Job2提交了，但是此时系统中已经没有资源可以分配给它了。加入 Job1 是一个大任务，那么 Job2 就只能等待一段很长的时间才能获得执行的资源。所以先入先出的分配方式存在一个问题就是大任务会占用很多资源，造成后面的小任务等待时间太长而饿死，因此一般不使用这个默认配置。
**Capacity Scheduler**
Capacity Scheduler 是一种多租户、弹性的分配方式。每个租户一个队列，每个队列可以配置能使用的资源上限与下限（譬如 50%，达到这个上限后即使其他的资源空置着，也不可使用），通过配置可以令队列至少有资源下限配置的资源可使用。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/iaicWqh8rjVrzD95icoEUPZVjLur0WmEKHmeUaRgtGnatcKRB7TMOEwKe50yRGBHyaaQJVDfgBibxdxJ8sexvxm6DQ/640)
图 9，via https://blog.csdn.net/suifeng3051/article/details/49508261
图 9 中队列 A 和队列 B 分配了相互独立的资源。Job1 提交给队列 A 执行，它只能使用队列 A 的资源。接着 Job2 提交给了队列B 就不必等待 Job1 释放资源了。这样就可以将大任务和小任务分配在两个队列中，这两个队列的资源相互独立，就不会造成小任务饿死的情况了。
**Fair Scheduler**
Fair Scheduler 是一种公平的分配方式，所谓的公平就是集群会**尽可能地按配置****的比例****分配**资源给队列。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/iaicWqh8rjVrzD95icoEUPZVjLur0WmEKHmrukZhUdkbYSLHjdbnB7nm63VB2JMG4M5gGaMLaGpuMlpCtaibHWlR4Q/640)
图 10，via https://blog.csdn.net/suifeng3051/article/details/49508261
图 10 中 Job1 提交给队列 A，它占用了集群的所有资源。接着 Job2 提交给了队列 B，这时 Job1 就需要释放它的一半的资源给队列 A 中的 Job2 使用。接着 Job3 也提交给了队列 B，这个时候 Job2 如果还未执行完毕的话也必须释放一半的资源给 Job3。这就是公平的分配方式，在队列范围内所有任务享用到的资源都是均分的。
** / the Future of  Spark / **
Mesos 的资源调度和 Yarn 类似，但是它提供了粗粒度和细粒度的两种模式。所谓的粗粒度和细粒度的差别在于：Executor 申请的资源是在执行前申请，还是在执行过程中按需申请。集群资源紧张时可能有一个 Executor 申请的资源在当时处于闲置状态，如果处于粗粒度模式下，这些资源在当时就浪费了。但是在细粒度模式下，Executor 执行时所需的资源是按照它的需求分配的，这样就不存在资源闲置的情况了。
因为 Mesos 的 Executor 是可以动态调整，而 Yarn 使用的 Container 是不可以动态调整的，所以目前 Yarn 是不支持细粒度的调度模式的，但 Yarn 已经有计划支持细粒度的资源管理方式。
除此之外在 Hadoop3.1.0 中 Yarn 提供了对 gpu 资源的支持，目前只支持 Nvidia gpu。期待 Spark 在其他方面的更多探索，下一篇我们将具体介绍 RDD，欢迎持续关注。
**转载声明**：本文转载自「美图数据技术团队」，搜索「美图数据技术团队」即可关注。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
