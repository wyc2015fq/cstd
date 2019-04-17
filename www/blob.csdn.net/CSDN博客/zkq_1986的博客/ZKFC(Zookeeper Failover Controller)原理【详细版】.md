# ZKFC(Zookeeper Failover Controller)原理【详细版】 - zkq_1986的博客 - CSDN博客





2017年02月09日 19:06:20[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：9066








ZKFC设计文档（ZK Failover Controller Design）






- 设计- 组件化设计，ZK-based的automatic Failover主要由三个组件组成：- HealthMonitor：用于监控NN是否unavailable或者处于unhealth状态
- ActiveStandbyElector：用于监控NN在zk中的状态
- ZKFailoverController：从HealthMonitor和ActiveStandbyElector中订阅事件并管理NN的状态，另外ZKFC还需要负责fencing。
- 现阶段，上述三个组件都在跑在一个JVM中，这个JVM与NN的JVM在同一个机器上。但是是两个独立的进程。一个典型的HA集群，有两个NN组成，每个NN都有自己的ZKFC进程。- - ![](https://img-my.csdn.net/uploads/201208/31/1346378241_3680.png)

- 部署示意图



- HealthMonitor设计- HealthMonitor由HADOOP-7788完成提交，它由一个loop循环的调用一个monitorHealth rpc来检视本地的NN的健康性。如果NN返回的状态信息发生变化，那么它将经由callback的方式向ZKFC发送message。HealthMonitor具有一下状态：
- - INITIALIZING：HealthMonitor已经初始化好，但是仍未与NN进行联通
- SERVICE NOT RESPONDING：rpc调用要么timeout，要么返回值未定义。
- SERVICE HEALTHY：RPC调用返回成功
- SERVICE UNHEALTHY：RPC放好事先已经定义好的失败类型
- HEALTH MONITOR FAILED：HealthMonitor由于未捕获的异常导致失败。


- ActiveStandbyElector设计- ActiveStandbyElector (committed in HADOOP-7992 and improved in HADOOP-8163, HADOOP-8212)主要负责凭借ZK进行协调，和ZKFC主要进行以下两个方面的交互：- joinElection()--通知ASE，本地的NN可以被选为活动NN
- quitElection()--通知ASE，本地的NN不能被选为活动NN

- 一旦ZKFC调用了joinElection，那么ASE将试图获取ZK中的lock（an ephemeral znode，automatically deleted when ZKFC crash or lost connection），如果ASE成功的创建了该lock，那么它向ZKFC调用becomeActive()。否则调用becameStandby()并且开始监控这个lock（其他NN创建的）
- 如果当前lock-holder失败了，另一个监控在这个lock上的ZKFC将被触发，然后试图获取这个lock。如果成功，ASE将同样的调用becomeActive方法来通知ZKFC
- 如果ZK的session过期，那么ASE将在本地NN上调用enterNeutralMode而不是调用becomeStandby。因为他没法知道是否有另一个NN已经准备好接管了。这种情况下，将本地NN转移到Standby状态是由fencing机制来完成（详见下文）。

- ZKFC设计- ZKFC本身非常简单，它运行以下进程：
- - 启动时，通知HealthMonitor去监控本地NN。然后使用配置好的ZK去初始化ASE。但是绝不能立即参加Election。
- 当HealthMonitor的状态改变时，ZKFC相应的做出如下反应：- SERVICE_HEALTHY---通知elector去join Election，如果还没有的话。
- HEALTH MONITOR FAILED---中断所有的ZKFC进程，因为ZKFC也没法工作了。
- INITIALIZING---这个情况一般是NN刚刚重启还没准备好进行服务。ZKFC会退出Election，并且通知fencing是没必要进行的。因为NN总是以Standby来开始。
- Other states---退出Election，如果当前是在Election状态。

- 当ActiveStandbyElector发布一个改变的时候，ZKFC做出如下反应：- becomeActive() - ZKFC将在本地NN上调用transitionToActive()。如果失败了，将退出Election然后sleep一段时间，重新进入Election。Sleep是为了让其他准备好了的NN也有机会成为ActiveNN。这种情况下退出Election并不会删除breadcrumb node {这是为了确保了无论谁成为了ActiveNN之后可以fencing这个NN，尽管这个情况的失败可能导致这个NN可能会进入partially-active状态。｝
- becomeStandby() - 在本地NN上调用transitionToStandby() ，如果失败，另一个NN将毫不犹疑的将这个NN进行fencing。（详见fencing）
- enterNeutralMode() - 当前没有反应，因为目前的设计中，不会进入此状态。
- fenceOldActive(...) - 详见fencing
- notifyFatalError(...) - 中断ZKFC，因为已经没法正常工作了。

- 所有的调用都是在ZKFC上进行同步，这样确保了串行化所有事件的顺序确保其逻辑的正确性。


- Fencing设计- HADOOP-8163对ASE进行了增强，主要是通过增加了fencing的回调机制，详细如下：
- - 在获取了ActiveLock之后，通知本地NN成为了Active之前，检查breadcrumb znode的存在性- 面包屑Znode存在的话，调用fenceOldActive(data)从那个NN上传入data数据，如果成功了，删除面包屑Znode
- 如果fencing失败，log一个error，扔掉lock，sleep一会，重新进行Election。这样也给其他NN有机会成为ActiveNN
- 使用本地NN的标识数据，创建一个新的breadcrumb node。

- 当退出Election的时候，quiting的NN能够自己判定是否需要fencing。如果需要，将删除breadcrumb node，然后关闭ZK session。


- ZKFC的状态机图- ![](https://img-my.csdn.net/uploads/201208/31/1346378267_6862.png)


- 尽管有了Automatic-Failover，但是能够进行手工的Failover对管理员来说也是个很好的选择，特别是在有计划的升级软硬件的时候和想要指定NN为Active的时候。- 开始的时候，只能通过Failover只能通过fail来触发。所以要达到想要的结果只能手工模拟fail的情况，比如kill掉一个NN以便让另一个NN成为ActiveNN。这有如下缺点：- 没有对standby机器的预先检查机制，如果standby不是ready的话，这将导致service不可用。
- 如果standby在成为Active的时候失败，将不能很快切回去，因为另一个NN已经killed
- 考虑到之前的ActiveNN被un-gracefully失效，这将导致fencing的执行，有可能导致STONITH等其它fencing，这对于管理员只想做个小改动来说too heavy。

- 因此通过以下的改变，带来了手动的failover：- 为每个namespace加入一个dfs.ha.auto-failover.enabled配置项。如果为true，将导致以下行为：
- - haadmin -failover命令不再直接对NN进行Rpc调用，而是采用以下描述的方式来代替
- NN进入到一个只能接受mutative HAServiceProtocol RPCs从ZKFCs
- start-dfs.sh也可以自动在所有的NN机器上启动ZKFC


- 手动Failover的操作- 当管理员执行Failover时候，首先执行haadmin -failover -to <namenode>，我们会自动将其转化为no-automatic的设置语法 haadmin -failover -from<namenode> -to <namenode>。以下操作将被执行
- - 检查被选NN的健康状况，如果不是healthy，将抛出异常
- 检查是否已经成为Active，如果是，返回成功。
- 发送一个concedeLock() RPC给当前的活动ZKFC，这个ZKFC将会执行：- 发送一个transitionToStandby() RPC给本地NN
- 如果成功，那么删除其留下的breadcrumb node，否则不动。
- 退出Election，设置一个定时器，在下一个5-10秒内将不会再次参加Election。

- 当这个RPC返回时，希望lock已经被其他NN给drop了，等待5-10秒在验证这个NN是否已经成功获取了该lock。等到本地NN要么成功成为了ActiveNN，要么失效，然后返回给client这个结果。

- 目前仅仅假定只有两个NN在参加Election，将来我们将支持多个standbyNN，这需要添加新的znode来标识需要成为Active的NN，然后更改Election的过程，在这个NN想获取lock之前先检查这个NN。



- 示例场景- ActiveNN产生JVM crash- 一旦这种情况发生，HealthMonitor在调用monitorHealth()将失效。然后HM将向ZKFC调用enterState(SERVICE_NOT_RESPONDING)，本地ZKFC将退出Election，另外一个ZKFC获取active lock，执行fencing，变成active。

- ActiveNN JVM freeze(e.g sigstop)- 如果JVM freeze了但是没有crash掉，这与上面情况一直，monitorHealth会由于timeout而引发上述过程。FUTURE-WORK：使用JVMTI来判断NN是否在进行gc，如此可以使用另一个timeout来为gc进行failover。

- ActiveNN machine crash- 当整个机器crash了，ASE在zk的session将会过期，另一个ZKFC将会获取这个事件，引发failover。

- Active ZKFC crash- 尽管ZKFC设计简单，但是仍然有可能会crash掉，在这个情况下，failover将会被错误的触发。另一个NN的ZKFC将会对ActiveNN调用transitionToStandby让它放弃active lock，然后进行aggressive fencing。尽管会成功，但是会导致进行了一次没有必要的failover。

- Zookeeper crash- 当zk集群crash了，那么所有的ZKFC将收到DISCONNECTED事件。然后ZKFC在本地NN调用enterNeutralMode，除此之外不做任何改变。系统除了不能执行failover之外，与其他情况无异。
- 当zk恢复了，clients立马能够重连。而zk能够将之前的session信息重新被各个client进行获取（在启动的一个timeou时间内）。所以所有的nodes将会重新获取session，不需要进行无必要的failover。
- FutureWork：breadcrumb znode在这个情况下可以优先的给予到ActiveNN，在ZK挂掉之前。


- 待发掘的细节- 和手动failover的集成- 如前所述，尽管有了强大的automatic failover，但是手动的failover在某些场合下仍是不二选择。
- 加入一个简单的quiesceActiveState() RPC接口到ZKFC，这个rpc通知NN退出Election，并等待StandbyNN发起failover，如果等待超时仍未有failover发起，那么这个NN重新获取Active lock。并向client汇报错误。


- Future work- 优先节点- 某些情况，可能希望为将指定的NN成为ActiveNN。现阶段是通过公平竞争来获取Active lock从而变成ActiveNN。可以通过两个方式来达成这个目的：一个是延迟非优选节点加入Election。另一个是提供failback将非优选节点从ActiveNN变成StandbyNN。

- 自我fencing- 当HM通知本地NN变成unhealthy时候，在退出Election之前，ZKFC能够执行自我fencing。例如，它能进行fuse -k -9 <ipcport>来强制击杀本地NN。这个方法能够避免很多复杂的fencing机制

- 管理Process- 当前的设计认为ZKFC process和本地NN独立的运行。NN挂掉了，ZKFC也不会视图去重启它，只是继续监控IPC端口直到NN被另外方式重启了。
- 当然如果ZKFC来负责NN的进程管理，这样使得部署要简单些，但是同时也增加了ZKFC本身的复杂度。因为java中的进程管理就是一坨屎。尽管如此当前的模块化设计足以使得以后如要进行这个工作很直接









**转载自：**[http://blog.csdn.net/chenpingbupt/article/details/7922042](http://blog.csdn.net/chenpingbupt)





