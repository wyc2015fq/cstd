# 在线实时大数据平台Storm集群组件学习 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年07月11日 15:58:47[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2417
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









Hadoop常用于离线的复杂的大数据处理，Spark常用于离线的快速(轻量级)的大数据处理， Storm常用于在线的实时的大数据处理；这句话一定程度上反应了三套大数据平台的鲜明特征。Storm是一套实时、在线、分布式的大数据处理平台。

**1）Nimbus和Supervisor**

Storm集群中包含两类节点：主控节点（Master Node）和工作节点（Work Node），角色定位如下：

主控节点（MasterNode）上运行一个被称为Nimbus的后台程序，它负责在Storm集群内分发代码，分配任务给工作机器，并且负责监控集群运行状态。Nimbus的作用类似于Hadoop中JobTracker的角色。

每个工作节点（WorkNode）上运行一个被称为Supervisor的后台程序。Supervisor负责监听从Nimbus分配给它执行的任务，据此启动或停止执行任务的工作进程。每一个工作进程执行一个Topology的子集；一个运行中的Topology由分布在不同工作节点上的多个工作进程组成。

![](https://img-blog.csdn.net/20160711155721875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




Nimbus和Supervisor节点之间所有的协调工作是通过Zookeeper集群来实现的。此外，Nimbus和Supervisor进程都是快速失败（fail-fast)和无状态（stateless）的；Storm集群所有的状态要么在Zookeeper集群中，要么存储在本地磁盘上。这意味着你可以用kill -9来杀死Nimbus和Supervisor进程，它们在重启后可以继续工作。这个设计使得Storm集群拥有不可思议的稳定性。

**2）Topology**

一个topology是spouts和bolts组成的图， 通过streamgroupings将图中的spouts和bolts连接起来，如下图：

![](https://img-blog.csdn.net/20160711155750692?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




一个topology会一直运行直到你手动kill掉，Storm自动重新分配执行失败的任务， 并且Storm可以保证你不会有数据丢失（如果开启了高可靠性的话）。如果一些机器意外停机它上面的所有任务会被转移到其他机器上。

运行一个topology很简单。首先，把你所有的代码以及所依赖的jar打进一个jar包。然后运行类似下面的这个命令：

storm jarall-my-code.jar backtype.storm.MyTopology arg1 arg2

这个命令会运行主类: backtype.strom.MyTopology, 参数是arg1, arg2。这个类的main函数定义这个topology并且把它提交给Nimbus。storm jar负责连接到Nimbus并且上传jar包。Topology的定义是一个Thrift结构，并且Nimbus就是一个Thrift服务，可以提交由任何语言创建的topology。

**3）Stream**

消息流stream是storm里的关键抽象。一个消息流是一个没有边界的tuple序列， 而这些tuple序列会以一种分布式的方式并行地创建和处理。通过对stream中tuple序列中每个字段命名来定义stream。在默认的情况下，tuple的字段类型可以是：integer，long，short， byte，string，double，float，boolean和bytearray。你也可以自定义类型（只要实现相应的序列化器）。

每个消息流在定义的时候会被分配给一个id，因为单向消息流使用的相当普遍， OutputFieldsDeclarer定义了一些方法让你可以定义一个stream而不用指定这个id。在这种情况下这个stream会分配个值为‘default’默认的id 。

Storm提供的最基本的处理stream的原语是spout和bolt。你可以实现spout和bolt提供的接口来处理你的业务逻辑。

**4）Spouts**

消息源spout是Storm里面一个topology里面的消息生产者。一般来说消息源会从一个外部源读取数据并且向topology里面发出消息：tuple。Spout可以是可靠的也可以是不可靠的。如果这个tuple没有被storm成功处理，可靠的消息源spouts可以重新发射一个tuple， 但是不可靠的消息源spouts一旦发出一个tuple就不能重发了。

消息源可以发射多条消息流stream。使用OutputFieldsDeclarer.declareStream来定义多个stream，然后使用SpoutOutputCollector来发射指定的stream。

Spout类里面最重要的方法是nextTuple。要么发射一个新的tuple到topology里面或者简单的返回如果已经没有新的tuple。要注意的是nextTuple方法不能阻塞，因为storm在同一个线程上面调用所有消息源spout的方法。

另外两个比较重要的spout方法是ack和fail。storm在检测到一个tuple被整个topology成功处理的时候调用ack，否则调用fail。storm只对可靠的spout调用ack和fail。

**5）Bolts**

所有的消息处理逻辑被封装在bolts里面。Bolts可以做很多事情：过滤，聚合，查询数据库等等。

Bolts可以简单的做消息流的传递。复杂的消息流处理往往需要很多步骤，从而也就需要经过很多bolts。比如算出一堆图片里面被转发最多的图片就至少需要两步：第一步算出每个图片的转发数量。第二步找出转发最多的前10个图片。（如果要把这个过程做得更具有扩展性那么可能需要更多的步骤）。

Bolts可以发射多条消息流， 使用OutputFieldsDeclarer.declareStream定义stream，使用OutputCollector.emit来选择要发射的stream。

Bolts的主要方法是execute,它以一个tuple作为输入，bolts使用OutputCollector来发射tuple，bolts必须要为它处理的每一个tuple调用OutputCollector的ack方法，以通知Storm这个tuple被处理完成了，从而通知这个tuple的发射者spouts。 一般的流程是： bolts处理一个输入tuple,  发射0个或者多个tuple, 然后调用ack通知storm自己已经处理过这个tuple了。storm提供了一个IBasicBolt会自动调用ack。

**6）Stream groupings**

义一个topology的其中一步是定义每个bolt接收什么样的流作为输入。stream grouping就是用来定义一个stream应该如何分配数据给bolts上面的多个tasks。

Storm里面有7种类型的stream grouping

ü   ShuffleGrouping: 随机分组，随机派发stream里面的tuple，保证每个bolt接收到的tuple数目大致相同。

ü   FieldsGrouping：按字段分组，比如按userid来分组， 具有同样userid的tuple会被分到相同的Bolts里的一个task， 而不同的userid则会被分配到不同的bolts里的task。

ü   All Grouping：广播发送，对于每一个tuple，所有的bolts都会收到。

ü   GlobalGrouping：全局分组，这个tuple被分配到storm中的一个bolt的其中一个task。再具体一点就是分配给id值最低的那个task。

ü   Non Grouping：不分组，这个分组的意思是说stream不关心到底谁会收到它的tuple。目前这种分组和Shuffle grouping是一样的效果， 有一点不同的是storm会把这个bolt放到这个bolt的订阅者同一个线程里面去执行。

ü   DirectGrouping： 直接分组，这是一种比较特别的分组方法，用这种分组意味着消息的发送者指定由消息接收者的哪个task处理这个消息。 只有被声明为Direct Stream的消息流可以声明这种分组方法。而且这种消息tuple必须使用emitDirect方法来发射。消息处理者可以通过TopologyContext来获取处理它的消息的task的id （OutputCollector.emit方法也会返回task的id）。

ü   Local orshuffle grouping：如果目标bolt有一个或者多个task在同一个工作进程中，tuple将会被随机发生给这些tasks。否则，和普通的Shuffle Grouping行为一致。

**7）Tasks和Workers**

每一个spout和bolt会被当作很多task在整个集群里执行。每一个executor对应到一个线程，在这个线程上运行多个task，而stream grouping则是定义怎么从一堆task发射tuple到另外一堆task。你可以调用TopologyBuilder类的setSpout和setBolt来设置并行度（也就是有多少个task）。

一个topology可能会在一个或者多个worker（工作进程）里面执行，每个worker是一个物理JVM并且执行整个topology的一部分。比如，对于并行度是300的topology来说，如果我们使用50个工作进程来执行，那么每个工作进程会处理其中的6个tasks。Storm会尽量均匀的工作分配给所有的worker。

**8）Configuration**

Storm里面有一堆参数可以配置来调整Nimbus, Supervisor以及正在运行的topology的行为，一些配置是系统级别的，一些配置是topology级别的。default.yaml里面有所有的默认配置。你可以通过定义个storm.yaml在你的classpath里来覆盖这些默认配置。并且你也可以在代码里面设置一些topology相关的配置信息（使用StormSubmitter）。



