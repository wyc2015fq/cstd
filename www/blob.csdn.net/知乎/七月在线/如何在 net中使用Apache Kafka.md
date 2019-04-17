# 如何在.net中使用Apache Kafka - 知乎
# 



Apache Kafka是一个开源、分布式、分区的、高性能的发布-订阅消息平台。对于能够处理大量数据的系统来说，使用Apache Kafka是一个很好的选择。在本文中，我们将研究如何用c#语言在Kafka中创建生产者和消费者应用程序。

要开始使用Kafka，你应该先下载Kafka和ZooKeeper，然后将它们安装到你的Windows系统上。当你完成了必要的特定的设置之后，运行ZooKeeper和Kafka，然后再返回继续看这篇文章。




**Apache Kafka架构**

在本节中，我们将研究Kafka中的架构组件和相关术语。基本上，Kafka由以下组件组成：

（1）Kafka cluster—一一个或多个服务器的集合，每一台服务器称为broker。

（2）生产者——用于发布消息的组件。

（3）消费者—用于检索或消费消息的组件。

（4）ZooKeeper——一个用于在分布式环境中维护跨集群节点配置信息的集中协调服务。

Kafka的基本数据单元是消息，在Kafka中的消息被表示为键值对，Kafka将所有的消息转换成数组。应该注意的是，在Kafka中，生产者、消费者和集群之间的通信使用TCP协议。Kafka集群中的每个服务器都称为broker。通过向集群中添加额外的服务器，你可以水平地伸缩Kafka。

下图展示了Kafka的架构组件——高级视图
![](https://pic3.zhimg.com/v2-281b79513cc8e439905cd5cc59a7cfee_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='557' height='275'></svg>)



Kafka中的主题代表了一个消息的逻辑集合，你可以将其看作是生产者可以发布的消息所属的类别。顺便说一下，Kafkabroker包含一个或多个主题，这些主题依次被划分为一个或多个分区，一个分区被定义为一个有序的消息序列。分区是Kafka 能够动态伸缩的关键，因为分区分布在多个服务器之间。

在任何给定的时间点，你可以用一个或多个生产者将消息推送到集群中。Kafka的生产者将消息发布为一个特定的主题，消费者通过订阅这个主题来接收消息。




**在Kafka和RabbitMQ之间进行选择**

Kafka和RabbitMQ都是流行的开源消息系统，在相当长的一段时间内都被广泛使用。什么时候你应该选择Kafka而不是RabbitMQ ?这个取决于以下几个因素。

RabbitMQ是流行的开源消息队列系统，用Erlang语言开发。它强大的原因是丰富的路由功能和提供消息确认的能力。RabbitMQ还提供了一个用户友好的web界面，你可以使用这个界面来监视RabbitMQ服务器。

然而，当涉及到支持大型部署时，Kafka比RabbitMQ要好得多，因为你需要做的就是添加更多的分区。还应该注意到，RabbitMQ集群不能支持网络分区。你应当采用联合的方法使RabbitMQ服务器结合在一起。

Kafka在性能上也明显超过了RabbitMQ，因为一个Kafka实例每秒可以处理100K消息，而RabbitMQ每秒可以处理近20K消息。Kafka支持离线和在线消息消费，当你想要以低延迟的状态发送消息以支持一批消费者时，Kafka是一个不错的选择。




**构造Kafka的生产者和Kafka的消费者**

在本节中，我们将研究如何构造在Kafka中一起使用的生产者和消费者。为此，我们将在Visual Studio中构造两个控制台应用程序——其中一个代表生产者，一个代表消费者。我们需要安装一个适合于.net的Kafka应用程序。

顺便说一下，有许多的关于Kafka的应用程序，但在这篇文章中，我们将使用Kafka - net作为Apache Kafka的本地c#客户端，你可以在Visual Studio内通过NuGet包管理器安装Kafka - net。

下面是构造Kafka的生产者的主要方法:
![](https://pic3.zhimg.com/v2-3cffa0f585df4e3207c72b1510a2f086_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='557' height='241'></svg>)



下面是构造Kafka的消费者的主要方法:
![](https://pic2.zhimg.com/v2-0a06eb671948751519e85075016910c1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='558' height='272'></svg>)



注意，你应该在生产者和消费者的应用程序中加入Kafka的命名空间，如下图所示：
![](https://pic3.zhimg.com/v2-75b8031d457a646036fbb0a9c6a84496_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='524' height='78'></svg>)



最后，先运行生产者，再运行消费者。你就可以看到“欢迎来到Kafka!”显示在消费者控制台窗口。

虽然我们有很多的消息传递系统以供选择，如：RabbitMQ、MSMQ、IBM MQ系列，但Kafka在处理大量数据流方面领先于其他系统。Kafka通常用于IoT应用程序，日志聚合以及其他需要低延迟和强大的消息传递的保证的情况。

如果你的应用程序需要一个快速且可扩展的服务平台，Kafka是一个很好的选择。


