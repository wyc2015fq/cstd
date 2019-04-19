# MapReduce 运行原理详解 （Hadoop 2.6） - Simple 专栏 - CSDN博客
2015年12月30日 09:51:23[Simple_Zz](https://me.csdn.net/love284969214)阅读数：239
所属专栏：[Hadoop 浅谈](https://blog.csdn.net/column/details/28029.html)
### 通过提交jar包，进行MapReduce处理，那么整个运行过程分为五个环节：
- 向 client 端提交 MapReduce job。
- 随后 yarn 的 ResourceManager 进行资源的分配。
- 由 NodeManager 进行加载与监控containers。
- 通过 applicationMaster 与 ResourceManager 进行资源的申请及状态的交互，由 NodeManagers 进行 MapReduce 运行时 job 的管理。
- 通过 hdfs 进行 job 配置文件、jar包的各节点分发。
- 
![](https://img-blog.csdn.net/20180920162825137?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### Job 提交过程
job的提交通过**调用 submit() 方法**创建一个 **JobSubmitter **实例，并**调用 submitJobInternal() 方法**。
    整个job的运行过程如下：
- 向 ResourceManager 申请 application ID，此 ID 为该 MapReduce 的 jobId 。
- 检查 output 的路径是否正确，是否已经被创建。
- 计算 input 的 splits 。
- 拷贝运行 job 需要的 jar 包、配置文件以及计算 input 的 split 到各个节点。
- 在 ResourceManager 中调用 submitAppliction() 方法，执行 job 。
### Job 初始化过程
- 当 resourceManager 收到了 submitApplication() 方法的调用通知后，scheduler 开始分配 container ，随之ResouceManager 发送 applicationMaster 进程，告知每个 nodeManager 管理器。
- **由 applicationMaster 决定**如何运行 tasks，如果 job 数据量比较小，applicationMaster 便选择**将 tasks 运行在一个JVM中**。那么如何判别这个 job 是大是小呢？当一个 job 的 **mappers 数量小于10个**，**只有一个reducer或者读取的文件大小要小于一个HDFS block时。**可通过修改配置项 **mapreduce.job.ubertask.maxmaps**、**mapreduce.job.ubertask.maxreduces**、**mapreduce.job.ubertask.maxbytes** 进行调整。
- 在运行 tasks 之前，applicationMaster 将会**调用 setupJob() 方法**，随之创建 output 的输出路径（这就能够解释，不管你的 mapreduce 一开始是否报错，输出路径都会创建）。
### Task 任务分配
- 接下来 applicationMaster 向 ResourceManager 请求 containers 用于执行 map 与 reduce 的 tasks（step 8)，这里 map task 的优先级要高于 reduce task，当所有的 map tasks 结束后，随之进行 sort （这里是shuffle过程后面再说）,最后进行reduce task的开始。（这里有一点，当 map tasks 执行了百分之 5% 的时候，将会请求 reduce，具体下面再总结）。
- 运行tasks的是需要消耗内存与CPU资源的，**默认情况下，map 和 reduce 的 task 资源分配为 1024MB 与一个核**，（可修改运行的最小与最大参数配置 **mapreduce.map.memory.mb**、**mapreduce.reduce.memory.mb**、**mapreduce.map.cpu.vcores**、**mapreduce.reduce.reduce.cpu.vcores **）
### **Task 任务执行**
- 这时一个 task 已经被 ResourceManager 分配到一个 container 中，由 applicationMaster 告知 nodemanager 启动container，这个 task 将会被一个**主函数为 YarnChild **的 java application 运行，但在运行 task 之前，**首先定位 task 需要的 jar 包、配置文件以及加载在缓存中的文件**。
- YarnChild 运行于一个专属的 JVM 中，所以**任何一个 map 或 reduce 任务出现问题，都不会影响整个 nodemanager 的 crash 或者 hang **。
- 每个 task 都可以在相同的 JVM task 中完成，随之将完成的处理数据写入临时文件中。
### **运行进度与状态更新**
- MapReduce 是一个较长运行时间的批处理过程，可以是一小时、几小时甚至几天，那么 Job 的运行状态监控就非常重要。每个 job 以及**每个 task 都有一个包含 job（running,successfully completed,failed）的状态**，以及 value 的计数器，状态信息及描述信息（描述信息一般都是在代码中加的打印信息），那么这些信息是如何与客户端进行通信的呢？
- 当一个 task 开始执行，它将会保持运行记录，记录 task 完成的比例，对于 map 的任务，将会记录其运行的百分比，对于 reduce 来说可能复杂点，但系统依旧会估计 reduce 的完成比例。当一个 map 或 reduce 任务执行时，**子进程会持续每三秒钟与 applicationMaster 进行交互**。
### **Job 完成**
 　　最终，applicationMaster会收到一个job完成的通知，随后改变job的状态为successful。最终，applicationMaster与task containers被清空。
### Shuffle与Sort
　　从map到reduce的过程，被称之为shuffle过程，MapReduce使到reduce的数据一定是经过key的排序的，那么shuffle是如何运作的呢？
　　当map任务将数据output时，**不仅仅是将结果输出到磁盘，它是将其写入内存缓冲区域，并进行一些预分类**。
![](https://img-blog.csdn.net/20180920162922988?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**　　1、The Map Side**
　　首先map任务的**output过程是一个环状的内存缓冲区，缓冲区的大小默认为100MB**（可通过修改配置项mpareduce.task.io.sort.mb进行修改），当写入内存的大小到达一定比例**，默认为80%**（可通过mapreduce.map.sort.spill.percent配置项修改）,便开始写入磁盘。
　　在写入磁盘之前，线程将会指定数据写入与reduce相应的patitions中，最终传送给reduce.在每个partition中**，后台线程将会在内存中进行Key的排序**，（**如果代码中有combiner方法，则会在output时就进行sort排序**，这里，如果只有少于3个写入磁盘的文件，combiner将会在outputfile前启动，如果只有一个或两个，那么将不会调用）
　 这里**将map输出的结果进行压缩会大大减少磁盘IO与网络传输的开销**（配置参数mapreduce.map .output.compress 设置为true,如果使用第三方压缩jar，可通过mapreduce.map.output.compress.codec进行设置)
　  随后这些paritions输出文件将会通过HTTP发送至reducers，传送的最大启动线程通过mapreduce.shuffle.max.threads进行配置。
**　　2、The Reduce Side**
　　首先上面每个节点的map都将结果写入了本地磁盘中，现在reduce需要将map的结果通过集群拉取过来，这里要注意的是，**需要等到所有map任务结束后,reduce才会对map的结果进行拷贝**，由于reduce函数有少数几个复制线程，以至于它**可以同时拉取多个map的输出结果。默认的为5个线程**（可通过修改配置mapreduce.reduce.shuffle.parallelcopies来修改其个数）
　　这里有个问题，那么reducers怎么知道从哪些机器拉取数据呢？ 
　　当所有map的任务结束后，**applicationMaster通过心跳机制（heartbeat mechanism)，由它知道mapping的输出结果与机器host**,所以**reducer会定时的通过一个线程访问applicationmaster请求map的输出结果**。
　　Map的结果将会被拷贝到reduce task的JVM的内存中（内存大小可在mapreduce.reduce.shuffle.input.buffer.percent中设置）如果不够用，则会写入磁盘。当内存缓冲区的大小到达一定比例时（可通过mapreduce.reduce.shuffle.merge.percent设置)或map的输出结果文件过多时（可通过配置mapreduce.reduce.merge.inmen.threshold)，将会除法合并(merged)随之写入磁盘。　　这时要注意，**所有的map结果这时都是被压缩过的，需要先在内存中进行解压缩，以便后续合并它们**。（合并最终文件的数量可通过mapreduce.task.io.sort.factor进行配置） 最终reduce进行运算进行输出。
