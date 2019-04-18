# 在线实时大数据平台Storm并行和通信机制理解 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年08月08日 10:53:55[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3794
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









1、storm系统角色和应用组件基本理解：

![](https://img-blog.csdn.net/20160808102342080?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       和Hadoop一起理解，清晰点。

      1）物理节点Nimubus，负责资源分配和任务调度；

      2）物理节点Supervisor负责接受nimbus分配的任务，启动和停止属于自己管理的worker进程；

      3）系统角色Worker运行具体处理组件逻辑的进程；

      4）系统爵色Task是worker中每一个spout/bolt的线程称为一个task，storm0.8之后的版本，task不再与物理线程对应，同一个spout/bolt的task可能会共享一个物理线程，该线程称为executor。

      节点（supervisor）-进程（worker）-线程（executor）-任务（task）和应用组件的关系呢？

2、Storm应用组件关键的基本理解

![](https://img-blog.csdn.net/20160808103042057?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       这个图对这个关系梳理的还是比较清楚。通俗地理解下：

       1）提交有一个topology（就是一个程序）给集群，集群分配到不同worker执行（可能分布在不同节点），就是有多少个进程在同时进行这个topology，而进程可能在同一个节点上也可能在不同节点上；

       2）每个topology运行在多个worker上，每个worker又分出多个executor，就是进程内有多个线程来执行；

      3）每个executor又可以有多个具体任务来执行。

       一个topology可以在多个supervisor上执行，一个supervisor也可以执行多个topology；一个worker只执行某个topology，一个topology由多个worker来执行。

       一个executor可以执行一个component中的多个task。 一个executor默认对应一个task，一个worker中包含多个executor。

      现在问题是：不同topolopy之间如何通信？同一topology的不同进程(worker）之间如何通信？同一worker的不同线程（executor）之间如何通信？这三层分别涉及到应用间、进程间、线程间的通信？




3、Storm通信机制

       有这么一个场景：从一个实时生产的文件列表中取出文件，然后统计具体id的次数，这如果应用storm平台，涉及到文件资源读取会不会重复？具体id的统计如何汇聚？在分布式情况下，storm是如何控制topology不会重复读取文件内容，同时又能汇聚id的次数。先看看storm的通信机制。

       1）同一worker间消息的发送使用的是LMAX Disruptor，它负责同一节点（同一进程内）上线程间的通信；

              Disruptor使用了一个RingBuffer替代队列，用生产者消费者指针替代锁。

             生产者消费者指针使用CPU支持的整数自增，无需加锁并且速度很快。Java的实现在Unsafe package中。

       2） 不同worker间通信使用ZeroMQ（0.8）或Netty（0.9.0）；

       3） 不同topologey之间的通信，Storm不负责，需要自己实现，例如使用kafka等；

       先不考虑不同topologey之间的通信（除了kafa，我想还可以用nosql的redis来保存一些需要共享的数据资源），同一topology的worker之间用netty通信和同一worker之间用LMAX Disruptor通信，这进程和线程的通信工具能否实现对数据资源读取的排斥性，这个storm平台应该是能实现，但个人理解上需要通过代码来加深。




4、Storm并行机制

      基于storm的通信机制，我想storm是可以实现并行分布来实现任务。

![](https://img-blog.csdn.net/20160808104358001?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

       这张storm官方的图，很清晰地给出了各组件之间的并行度。代码如下：



```java
Config conf = new Config();
conf.setNumWorkers(2); // use two worker processes

topologyBuilder.setSpout("blue-spout", new BlueSpout(), 2); // set parallelism hint to 2

topologyBuilder.setBolt("green-bolt", new GreenBolt(), 2)
               .setNumTasks(4)
               .shuffleGrouping("blue-spout");

topologyBuilder.setBolt("yellow-bolt", new YellowBolt(), 6)
               .shuffleGrouping("green-bolt");

StormSubmitter.submitTopology(
        "mytopology",
        conf,
        topologyBuilder.createTopology()
    );
```
    该topology：1）包含3个component，1个spout，2个bolt；2）包含2个worker process，10个executor thread，12个task。PS：同颜色属于同一个component。




5、Storm的worker并行配置

![](https://img-blog.csdn.net/20160808104802019?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

    Storm官网给出的Worker进程内部的结构图。每一个worker进程都有一个单独的线程来监听该worker的端口号，并接收发送到该端口的数据，它将通过网络发送过来的数据放到worker的接收队列里面。监听的端口号是通过supervisor.slots.ports定义（conf/storm.yaml中配置），每个节点配置几个端口就可以有几个worker。

6、storm的executor并行配置

     除在代码中配置外，还可以通过storm rebalance来调整。
storm rebalance mytopology -n 5 -e blue-spout=3 -e yellow-bolt=10




总结：如何理解storm进程间和线程间的通信，是通过storm平台高性能解决实时数据处理的关键，还需要通过实际例子来理解netty和LMAX Disruptor，有了这两个才有并行分布的意义。这个道理很清晰，就是你要实现分布、并行，首要就是要解决不同任务之间的通信问题，才能确保并行分布的任务对数据的共享不会产生冲突。



