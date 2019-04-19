# storm简介 - 三少GG - CSDN博客
2013年12月10日 14:10:44[三少GG](https://me.csdn.net/scut1135)阅读数：650
个人分类：[云计算/大数据](https://blog.csdn.net/scut1135/article/category/1449467)
# [storm简介](http://www.searchtb.com/2012/09/introduction-to-storm.html)
[悟时](http://www.searchtb.com/author/%E6%82%9F%E6%97%B6)
- 场景
伴随着信息科技日新月异的发展，信息呈现出爆发式的膨胀，人们获取信息的途径也更加多样、更加便捷，同时对于信息的时效性要求也越来越高。举个搜索场景中的例子，当一个卖家发布了一条宝贝信息时，他希望的当然是这个宝贝马上就可以被卖家搜索出来、点击、购买啦，相反，如果这个宝贝要等到第二天或者更久才可以被搜出来，估计这个大哥就要骂娘了。再举一个推荐的例子，如果用户昨天在淘宝上买了一双袜子，今天想买一副泳镜去游泳，但是却发现系统在不遗余力地给他推荐袜子、鞋子，根本对他今天寻找泳镜的行为视而不见，估计这哥们心里就会想推荐你妹呀。其实稍微了解点背景知识的码农们都知道，这是因为后台系统做的是每天一次的全量处理，而且大多是在夜深人静之时做的，那么你今天白天做的事情当然要明天才能反映出来啦。
- 实现一个实时计算系统
全量数据处理使用的大多是鼎鼎大名的hadoop或者hive，作为一个批处理系统，hadoop以其吞吐量大、自动容错等优点，在海量数据处理上得到了广泛的使用。但是，hadoop不擅长实时计算，因为它天然就是为批处理而生的，这也是业界一致的共识。否则最近这两年也不会有s4,storm,puma这些实时计算系统如雨后春笋般冒出来啦。先抛开s4,storm,puma这些系统不谈，我们首先来看一下，如果让我们自己设计一个实时计算系统，我们要解决哪些问题。
- 
低延迟。都说了是实时计算系统了，延迟是一定要低的。
- 
高性能。性能不高就是浪费机器，浪费机器是要受批评的哦。
- 
分布式。系统都是为应用场景而生的，如果你的应用场景、你的数据和计算单机就能搞定，那么不用考虑这些复杂的问题了。我们所说的是单机搞不定的情况。
- 
可扩展。伴随着业务的发展，我们的数据量、计算量可能会越来越大，所以希望这个系统是可扩展的。
- 
容错。这是分布式系统中通用问题。一个节点挂了不能影响我的应用。
好，如果仅仅需要解决这5个问题，可能会有无数种方案，而且各有千秋，随便举一种方案，使用消息队列+分布在各个机器上的工作进程就ok啦。我们再继续往下看。
- 
容易在上面开发应用程序。亲，你设计的系统需要应用程序开发人员考虑各个处理组件的分布、消息的传递吗？如果是，那有点麻烦啊，开发人员可能会用不好，也不会想去用。
- 
消息不丢失。用户发布的一个宝贝消息不能在实时处理的时候给丢了，对吧？更严格一点，如果是一个精确数据统计的应用，那么它处理的消息要不多不少才行。这个要求有点高哦。
- 
消息严格有序。有些消息之间是有强相关性的，比如同一个宝贝的更新和删除操作消息，如果处理时搞乱顺序完全是不一样的效果了。
不知道大家对这些问题是否都有了自己的答案，下面让我们带着这些问题，一起来看一看storm的解决方案吧。
- Storm是什么
如果只用一句话来描述storm的话，可能会是这样：分布式实时计算系统。按照storm作者的说法，storm对于实时计算的意义类似于hadoop对于批处理的意义。我们都知道，根据google mapreduce来实现的hadoop为我们提供了map, reduce原语，使我们的批处理程序变得非常地简单和优美。同样，storm也为实时计算提供了一些简单优美的原语。我们会在第三节中详细介绍。
我们来看一下storm的适用场景。
- 
流数据处理。Storm可以用来处理源源不断流进来的消息，处理之后将结果写入到某个存储中去。
- 
分布式rpc。由于storm的处理组件是分布式的，而且处理延迟极低，所以可以作为一个通用的分布式rpc框架来使用。当然，其实我们的搜索引擎本身也是一个分布式rpc系统。
说了半天，好像都是很玄乎的东西，下面我们开始具体讲解storm的基本概念和它内部的一些实现原理吧。
- Storm的基本概念
首先我们通过一个 storm 和hadoop的对比来了解storm中的基本概念。
||Hadoop|Storm|
|----|----|----|
|系统角色|JobTracker|Nimbus|
|TaskTracker|Supervisor| |
|Child|Worker| |
|应用名称|Job|Topology|
|组件接口|Mapper/Reducer|Spout/Bolt|
表3-1
接下来我们再来具体看一下这些概念。
- 
Nimbus：负责资源分配和任务调度。
- 
Supervisor：负责接受nimbus分配的任务，启动和停止属于自己管理的worker进程。
- 
Worker：运行具体处理组件逻辑的进程。
- 
Task：worker中每一个spout/bolt的线程称为一个task. 在storm0.8之后，task不再与物理线程对应，同一个spout/bolt的task可能会共享一个物理线程，该线程称为executor。
下面这个图描述了以上几个角色之间的关系。
![系统结构](http://www.searchtb.com/wp-content/uploads/2012/08/deploy0.jpg)
图3-1
- 
Topology：storm中运行的一个实时应用程序，因为各个组件间的消息流动形成逻辑上的一个拓扑结构。
- 
Spout：在一个topology中产生源数据流的组件。通常情况下spout会从外部数据源中读取数据，然后转换为topology内部的源数据。Spout是一个主动的角色，其接口中有个nextTuple()函数，storm框架会不停地调用此函数，用户只要在其中生成源数据即可。
- 
Bolt：在一个topology中接受数据然后执行处理的组件。Bolt可以执行过滤、函数操作、合并、写数据库等任何操作。Bolt是一个被动的角色，其接口中有个execute(Tuple input)函数,在接受到消息后会调用此函数，用户可以在其中执行自己想要的操作。
- 
Tuple：一次消息传递的基本单元。本来应该是一个key-value的map，但是由于各个组件间传递的tuple的字段名称已经事先定义好，所以tuple中只要按序填入各个value就行了，所以就是一个value list.
- 
Stream：源源不断传递的tuple就组成了stream。
10.  stream grouping：即消息的partition方法。Storm中提供若干种实用的grouping方式，包括shuffle, fields hash, all, global, none, direct和localOrShuffle等
相比于s4, puma等其他实时计算系统，storm最大的亮点在于其记录级容错和能够保证消息精确处理的事务功能。下面就重点来看一下这两个亮点的实现原理。
- Storm记录级容错的基本原理
首先来看一下什么叫做记录级容错？storm允许用户在spout中发射一个新的源tuple时为其指定一个message id, 这个message id可以是任意的object对象。多个源tuple可以共用一个message id，表示这多个源 tuple对用户来说是同一个消息单元。storm中记录级容错的意思是说，storm会告知用户每一个消息单元是否在指定时间内被完全处理了。那什么叫做完全处理呢，就是该message id绑定的源tuple及由该源tuple后续生成的tuple经过了topology中每一个应该到达的bolt的处理。举个例子。在图4-1中，在spout由message
 1绑定的tuple1和tuple2经过了bolt1和bolt2的处理生成两个新的tuple，并最终都流向了bolt3。当这个过程完成处理完时，称message 1被完全处理了。
![消息传递](http://www.searchtb.com/wp-content/uploads/2012/08/message10.jpg)
图4-1
在storm的topology中有一个系统级组件，叫做acker。这个acker的任务就是追踪从spout中流出来的每一个message id绑定的若干tuple的处理路径，如果在用户设置的最大超时时间内这些tuple没有被完全处理，那么acker就会告知spout该消息处理失败了，相反则会告知spout该消息处理成功了。在刚才的描述中，我们提到了”记录tuple的处理路径”，如果曾经尝试过这么做的同学可以仔细地思考一下这件事的复杂程度。但是storm中却是使用了一种非常巧妙的方法做到了。在说明这个方法之前，我们来复习一个数学定理。
A xor A = 0.
A xor B…xor B xor A = 0，其中每一个操作数出现且仅出现两次。
storm中使用的巧妙方法就是基于这个定理。具体过程是这样的：在spout中系统会为用户指定的message id生成一个对应的64位整数，作为一个root id。root id会传递给acker及后续的bolt作为该消息单元的唯一标识。同时无论是spout还是bolt每次新生成一个tuple的时候，都会赋予该tuple一个64位的整数的id。Spout发射完某个message id对应的源tuple之后，会告知acker自己发射的root id及生成的那些源tuple的id。而bolt呢，每次接受到一个输入tuple处理完之后，也会告知acker自己处理的输入tuple的id及新生成的那些tuple的id。Acker只需要对这些id做一个简单的异或运算，就能判断出该root
 id对应的消息单元是否处理完成了。下面通过一个图示来说明这个过程。
![](http://www.searchtb.com/wp-content/uploads/2012/08/ack10.jpg)
图4-1 spout中绑定message 1生成了两个源tuple，id分别是0010和1011.
![](http://www.searchtb.com/wp-content/uploads/2012/08/ack20.jpg)
图4-2 bolt1处理tuple 0010时生成了一个新的tuple，id为0110.
![](http://www.searchtb.com/wp-content/uploads/2012/08/ack30.jpg)
图4-3 bolt2处理tuple 1011时生成了一个新的tuple，id为0111.
![](http://www.searchtb.com/wp-content/uploads/2012/08/ack40.jpg)
图4-4 bolt3中接收到tuple 0110和tuple 0111，没有生成新的tuple.
可能有些细心的同学会发现，容错过程存在一个可能出错的地方，那就是，如果生成的tuple id并不是完全各异的，acker可能会在消息单元完全处理完成之前就错误的计算为0。这个错误在理论上的确是存在的，但是在实际中其概率是极低极低的，完全可以忽略。
- Storm的事务拓扑
事务拓扑(transactional topology)是storm0.7引入的特性，在最近发布的0.8版本中已经被封装为Trident，提供了更加便利和直观的接口。因为篇幅所限，在此对事务拓扑做一个简单的介绍。
事务拓扑的目的是为了满足对消息处理有着极其严格要求的场景，例如实时计算某个用户的成交笔数，要求结果完全精确，不能多也不能少。Storm的事务拓扑是完全基于它底层的spout/bolt/acker原语实现的，通过一层巧妙的封装得出一个优雅的实现。个人觉得这也是storm最大的魅力之一。
事务拓扑简单来说就是将消息分为一个个的批(batch)，同一批内的消息以及批与批之间的消息可以并行处理，另一方面，用户可以设置某些bolt为committer，storm可以保证committer的finishBatch()操作是按严格不降序的顺序执行的。用户可以利用这个特性通过简单的编程技巧实现消息处理的精确。
- Storm在淘宝
由于storm的内核是clojure编写的(不过大部分的拓展工作都是java编写的)，为我们理解它的实现带来了一定的困难，好在大部分情况下storm都比较稳定，当然我们也在尽力熟悉clojure的世界。我们在使用storm时通常都是选择java语言开发应用程序。
在淘宝，storm被广泛用来进行实时日志处理，出现在实时统计、实时风控、实时推荐等场景中。一般来说，我们从类kafka的metaQ或者基于hbase的timetunnel中读取实时日志消息，经过一系列处理，最终将处理结果写入到一个分布式存储中，提供给应用程序访问。我们每天的实时消息量从几百万到几十亿不等，数据总量达到TB级。对于我们来说，storm往往会配合分布式存储服务一起使用。在我们正在进行的个性化搜索实时分析项目中，就使用了timetunnel + hbase + storm + ups的架构，每天处理几十亿的用户日志信息，从用户行为发生到完成分析延迟在秒级。
- Storm的未来
Storm0.7系列的版本已经在各大公司得到了广泛使用，最近发布的0.8版本中引入了State，使得其从一个纯计算框架演变成了一个包含存储和计算的实时计算新利器，还有刚才提到的Trident，提供更加友好的接口，同时可定制scheduler的特性也为其针对不同的应用场景做优化提供了更便利的手段，也有人已经在基于storm的实时ql(query language)上迈出了脚本。在服务化方面，storm一直在朝着融入mesos框架的方向努力。同时，storm也在实现细节上不断地优化，使用很多优秀的开源产品，包括kryo,
 Disruptor, curator等等。可以想象，当storm发展到1.0版本时，一定是一款无比杰出的产品，让我们拭目以待，当然，最好还是参与到其中去吧，同学们。
- 参考文献
[1]storm官方wiki及code. [https://github.com/nathanmarz/storm](https://github.com/nathanmarz/storm)
