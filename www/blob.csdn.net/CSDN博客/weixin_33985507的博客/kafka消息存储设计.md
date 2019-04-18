# kafka消息存储设计 - weixin_33985507的博客 - CSDN博客
2017年09月23日 18:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：17
消息队列的引入，什么时候使用MQ？
MQ（Message Queue），一种跨进程的通信机制，用于上下游传递消息。能达到解耦、异步、消峰限流的作用。举几个对应的适用例子。
**解耦**
1. 比如定时任务依赖的场景，晚上需要跑一些定时统计任务，任务2依赖任务1的结果，任务3依赖任务2的接口，一般开发人员会在每个定时任务之间，预留一些时间buffer处理。但是，当某一天其中一个任务超出常规时间，任务就跑乱套了，第二天肯定就有人来找到你了。这个场景就很适合用MQ去解耦，当任务1完成后，通知到任务2，任务二通过订阅消息去实现触发。
2. 比如统一充值网关服务，某个产品接入统一充值服务的微信渠道充值。充值成功后，微信服务端会通知到统一充值服务端，因为这些是异步的调用，且是公网接口，时间会相对长一些，业务上产品接入方会有需求想知道，到账的结果。这个到账通知就很适合MQ去实现。如果，这里由充值网关服务调用上游来通知结果的话，每次新增调用方，充值网关服务都需要修改代码发布，依赖反转了。充值网关+MQ的方式，业务调用方去订阅消息实现解耦。
**异步**
1.场景上游不关心执行结果。异步，rpc框架异步调用也可以。区别就是MQ消息会落地，并且消息中间件都会有HA的涉及，能保证消息语义的实现（至少一次、至少一次、至多一次）。rpc异步请求本身也会有本地内存队列，所以数据不是要求很重要的场景，差不多。只是在工程上，有一点，使用rpc处理这种业务，经验上要单拉出一个服务去调用下游，因为依赖倒置了。每增加接入方或者业务有修改，都要提供服务的工程去修改发布（有经验的同学应该深有体会），作为基础服务的话，应该减少这种依赖倒置的发布，独立出来一个服务处理的话，会减少风险。
业务的话，处理推荐日志，处理app埋点的统计数据
**消峰限流**
这个应该是MQ另一个最主要的作用之一。做活动访问量陡增，下游处理不过来的时候，使用消息队列达到限流的作用。工程上有个C10K问题（虽然现在已经有现在我们早已经突破了C10K这个瓶颈，具体的思路就是通过单个进程或线程服务于多个客户端请求，通过异步编程和事件触发机制替换轮训，IO 采用非阻塞的方式（reactor模型），减少不必要的性能损耗，等等）。但是这个要求下游的服务包括存储和依赖，都要做到这点，哪个环节弱都不行。可能还会浪费资源，平时的量用不着那么多服务器。
相应的，那些调用方需要被调用方立刻返回结果的需求，就不适用于MQ，需要根据业务去考虑，脱离了业务去引入新技术就是耍流氓。
**业务上有对消息组件的需求后，市面上陆续出现了很多成熟的消息中间件**
IBM webSphere MQ*、*Apache ActiveMQ、LinkedIn Kafka、阿里 rocketMQ、
java社区肯定要跟着一起玩的，社区也定义jms规范（[JMS规范百度词条](https://link.jianshu.com?t=https://baike.baidu.com/item/JMS/2836691?fr=aladdin)）***maid***。这些消息组件，前两个是基于jms规范实现的，后两个没有，rocketMQ开始是kafka的java版实现，现在已经从Apache社区正式毕业（17-0925），成为Apache顶级项目。在原有设计的基础上，比如提供事务消息等一些功能，kafka0.11版开始也提供可事务的支持，还没发布太久，效果还有待观察；但是都用别的方式实现了jms定义的一些功能，比如发布订阅，点对点通信。
**如何设计实现一个消息队列：**
实现一个消息组件不可避免的要处理如下问题（[消息中间件精要设计](https://link.jianshu.com?t=https://tech.meituan.com/mq-design.html)）：
1.通信协议的选择
2.消息的分布式存储设计，关系型数据库 磁盘 kv存储
3.如何分布式设计生产者和消费者保证高吞吐
4.如何实现顺序消费
5.如何保证HA，在高吞吐和HA上做平衡
6.事务消息的支持
7.push还是pull
8.单播、广播、订阅发布的实现
9.性能的优化，同步异步、批量等
以上问题有交叉，以这些问题出发，看看kafka是如何设计实现的。
**一.kafka基本概念的介绍**
1.**topics 主题**，队列的逻辑概念。可以有多个producer生产消息往topic发送，有个consumer从topic消费消息。
![6630851-46739e8650c1da09.png](https://upload-images.jianshu.io/upload_images/6630851-46739e8650c1da09.png)
topic结构图
topic有partition组成，partition个数有server.property配置文件指定，partition的服务器的分布，会由算法均分到不同的服务器上。每个partition上的消息是有序的不可变的。
![6630851-7343aa2fe5dea5da.png](https://upload-images.jianshu.io/upload_images/6630851-7343aa2fe5dea5da.png)
partition上生产消费情况
生产的消息，是以log的文件格式存储在服务器端。具体格式下边会单独再说下。每条消息有一个位移信息offset，生产者在队列尾添加消息，offset+1。生产者消费消息的进度位置也是用offset标记，消费后，消息是不删除的，所以可以指定offset重新消费。**offset的值的提交存储是放到客户端完成的**，**所以服务端是无消费状态的**。
offset的存储位置老版本是放到zookeeper上，考虑到集群topic很多的话，zookeeper的读写操作很频繁，zookeeper是不适合有大量写入操作的。所以新版本把offset存储到服务器端一个单独的topic下__consumer-offsets。这个topic默认50个分区
![6630851-00950be49643dd29.png](https://upload-images.jianshu.io/upload_images/6630851-00950be49643dd29.png)
offset存储位置__consumer-offsets
**2.producer**
生产者往指定的topic发消息，生产者发消息到不同的partition上算法有根据key值hash、轮训和新版本最新的算法，也可以自己实现指定partition的策略。
**生产者如何指定发送的partition？如何指定发送的策略？**
**3.consumer and consumer group**
![6630851-91a6ca80519cbc1a.png](https://upload-images.jianshu.io/upload_images/6630851-91a6ca80519cbc1a.png)
consumer group，consumer，partition的关系
> 
1.消费者消费topic，必须指定consumer group，其中配置文件group.id唯一标识一个consumer group
2.topic上可以有多个consumer group去订阅，kafka使用这个概念实现JMS规范里边发布订阅功能，即不同的接入方想实现订阅功能，只需要指定不同的consumer group即可。
3.consumer group上可以有多个消费者，并且一个partition只有consumer group的其中一个consumer在消费。一个consumer可以消费多个partition
看到后会想到的问题：**consumer group里的consumer如何分配消费partition的关系？**
4.replica 副本
HA设计的概念，**副本对应的是partition的概念**。每个partition的副本个数，在配置文件有指定。如果有3机器，3副本能保证（3-1）个server fail的情况下，不丢消息。[见kafka的HA设计](https://www.jianshu.com/p/8ba38874fcb4)
**问题：kafka是如何利用replica概念设计HA的？**
**二.服务端消息log存储设计**
> 
选择的是磁盘文件的持久化方式，没有提供不持久化的选择。
![6630851-efd8fc4e6875c62a.png](https://upload-images.jianshu.io/upload_images/6630851-efd8fc4e6875c62a.png)
3个partition文件夹
可以看到kafka10-topic-20170924有三个文件夹，每个文件夹代表一个分区，每个分区下的存储由segment组成，一个segment包括index索引文件，时间戳index索引文件和实际存储数据的log文件组成。
每个log文件的大小默认是10M（可配置），超过10M，新建文件，文件的名字是第一个消息的offset值。
**为什么这样设计？好处是什么？**
![6630851-345254f2581efb03.png](https://upload-images.jianshu.io/upload_images/6630851-345254f2581efb03.png)
segment结构
![6630851-2e6f6ac93b9c982a.png](https://upload-images.jianshu.io/upload_images/6630851-2e6f6ac93b9c982a.png)
segment结构
![6630851-3bdfcd08a47edbe7.png](https://upload-images.jianshu.io/upload_images/6630851-3bdfcd08a47edbe7.png)
索引文件和log文件关系
索引文件的结构是一个map，key是当前segment的offset的偏移量，从0开始。value是对应的log文件中消息开始位置的实际物理位置偏移量。索引文index file采取稀疏索引存储方式，它减少索引文件大小，通过mmap可以直接内存操作，稀疏索引为数据文件的每个对应message设置一个元数据指针,它比稠密索引节省了更多的存储空间，但查找起来需要消耗更多的时间。
举个🌰，消息的查找过程，比如offset的值是368772，如何查找消费对应消息内容。
> 
1.根据offset找到所在的segment，根据二分查找，找到消息所在的log文件0000000000000368769.log和索引文件0000000000000368769.index
2.计算下差368772-368769=3，在索引文件中也是二分查找，定位到是<3,497>记录，即对应的物理位置是497，从而找到消息
3.根据物理位置497在0000000000000368769.log文件找到消息。
**问题：如果稀疏索引没有找到怎么办？**
如果是索引文件没有命中怎么办。这就要继续在看下每条log的消息格式：
![6630851-bcc7d0e0dc33abff.png](https://upload-images.jianshu.io/upload_images/6630851-bcc7d0e0dc33abff.png)
每条消息日志的协议格式
字段解释：
> 
offset：8bytes长度的偏移量，唯一表示一条消息
message length ：消息长度
crc：CRC32校验消息
magic value：标示是否允许格式化改变
attributes：bit 0～2：压缩方法，0没有压缩 1gzip 2 snappy 3 lz4；bit 3：时间戳类型，0创建时间 1日志的追加时间；bit 4～7预留
timestamp：当时magic value的值是1是，有效。表示时间戳。这个新版本引入的字段
key length：消息key的长度
key：key的值
value length：消息内容的长度
value：消息的具体内容
所以没有索引到的查找，就先根据二分找到最近的一条内容，**然后根据每条消息的格式，知道消息的长度。依次计算出下一条消息的位置，直到找到offset相等的那条记录。**
服务器上的日志文件是二进制的，kafka提供很多方便的脚本工具，可以使用kafka的工具类DumpLogSegments类解析查看一下结构如图
![6630851-ac4cf9a9d488ec54.png](https://upload-images.jianshu.io/upload_images/6630851-ac4cf9a9d488ec54.png)
log二进制文件转换后的结构输出
转换脚本指令，要加上--print-data-log参数，不加的话，默认不输出key值和value值。其中图中payload是value值。
> 
./kafka-run-class.sh  kafka.tools.DumpLogSegments --print-data-log --files /tmp/kafka-logs/kafka10-topic-20170924-0/000000000000000000.log
同样的看下offset索引文件和时间戳索引文件，嗯，跟官网文档描述的一样，就放心了。
![6630851-9587a4a0902f63a9.png](https://upload-images.jianshu.io/upload_images/6630851-9587a4a0902f63a9.png)
offset索引文件
![6630851-196eb9c0628e217f.png](https://upload-images.jianshu.io/upload_images/6630851-196eb9c0628e217f.png)
时间戳索引文件
