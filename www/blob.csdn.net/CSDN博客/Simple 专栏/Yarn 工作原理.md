# Yarn 工作原理 - Simple 专栏 - CSDN博客
2016年04月19日 10:00:43[Simple_Zz](https://me.csdn.net/love284969214)阅读数：214
个人分类：[hadoop生态圈](https://blog.csdn.net/love284969214/article/category/7975457)
所属专栏：[Hadoop 浅谈](https://blog.csdn.net/column/details/28029.html)
由于Yarn良好的兼容性和扩展性，目前可以支持大部分数据引擎，所以了解Yarn的资源调度原理很有必要，Yarn主要由四个重要角色组成：
ResourceManager：顾名思义资源管理器，主要负责资源管理和调度，ResourceManager主要由两个组件构成：ApplicationManager,主要负责两类工作：
- 管理监控各个系统的应用，包括启动Applicaiton Master，监控Applicaiton Master运行状态(为了容错)
- 跟踪分配给Application Master的进度和状态。Scheduler，主要负责分配Container给Applicaiton Master，分配算法有多种(如公平调度等等)可以根据需求不同选择适合的调度策略。
NodeManager：节点管理器，主要负责维护本节点的资源情况和任务管理。首先NodeManager需要定期向ResourceManager汇报本节点资源使用情况，以便ResourceManager，根据资源使用情况，来分配资源给Application Master，其次，需要管理Applicaiton Master提交来的task，比如接收Applicaiton Master 启动或停止task的请求(启动和停止有NodeManager的组件ContainersLanuncher完成)。
ApplicaitonMaster：用户提交的每个program都会对应一个ApplicationMaster，主要负责监控应用，任务容错(重启失败的task)等。它同时会和ResourceManager和NodeManager有交互，向ResourceManager申请资源，请求NodeManager启动或提示task。
Container：容器是资源调度的单位，它是内存、cpu、磁盘、和IO的集合。Application Master会给task分配Container，task只能只用分配给它的Container的资源。
分配流程为Resource Manager ->Application Master -> task
![](https://img-blog.csdn.net/20181006212546410?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Yarn调度?主要分为8个步骤如上图所示：
- 有YarnClient提交program信息打拼ResourceManager，包括(应用代码和应用需要的一切参数和环境信息)。
- ResourceManager收到请求之后，调用ApplicationMasterManager向NodeManager发送请求，申请一个资源(Container)，并且要求Container启动ApplicationMaster。
- ApplicationMaster启动之后，首先注册自己到ResourceManager，然后为自己的Task申请Container，这个过程是轮训的，循环申请资源，ResourceManager收到请求之后，会要求NodeManager分配资源。
- 资源分配完毕之后，Application Master发送请求到NodeManager，启动任务。
- NodeManager设置Container的运行时环境(jar包，环境变量，任务启动脚本)，NodeManager会通过脚本启动任务。
- 启动的过程是由NodeManager的ContainerLauncher负责的，ContainerLauncher完成启动任务的工作。
- 这一步是在作业执行过程中持续发生的，我用虚线代表，主要包括两类交互，第一，task和Application Master的交互，task会向AM汇报任务状态和进度信息，比如任务启动，停止，状态更新。Applicaiton Master利用这些信息监控task整个执行过程。第二，是NodeManager和ResourceManager的交互，这个过程和任务没有关系，主要是两者之间保持的心跳信息(状态的变化等等)。
- Application Master在检测到作业运行完毕之后，Application Master想Resource Manager 删除自己，并且停止自己执行。
**说明：**
- 图中蓝色的部分，是NodeManager和ResourceManager的组件，主要负责控制整个流程。
- 图中绿色的部分，核心资源分配流程，不属于yarn，是由具体的数据引擎实现的(如mr，spark，strom等)，由此可见Yarn采用了资源双层调度模型，第一层是Yarn分配资源(Container)给Applicaiton Master，这是由Yarn实现的 ，第二层，是Applicaiton Master分配资源给task，这是有具体工作引擎实现，这样的好处是将，yarn的调度和应用分隔开，便于yarn和应用实现、优化或升级自己的调度策略。
上述内容是Yarn调度的基本过程，从调度过程可以看到，Yarn将资源封装抽象为Container，将应用抽象为Application Master,两个关键模型的抽象，实现了对资源和应用的统一管理，进而实现了调度平台和执行引擎的解耦。至于Application怎么样把Container分配给task，怎么样监控task的执行过程，完全是由执行引擎根据自身特性实现的。充分解耦的好处是，Yarn可以同时部署不同的执行引擎，集群不受限制，并且当一个执行引擎升级的时候，不会影响到别的引擎，这对于生产是至关重要的。并且Yarn提供了多种资源调度模式，以满足不同的生产环境。
