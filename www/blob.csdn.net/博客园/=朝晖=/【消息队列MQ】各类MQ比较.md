# 【消息队列MQ】各类MQ比较 - =朝晖= - 博客园
# [【消息队列MQ】各类MQ比较](https://www.cnblogs.com/dhcn/p/7120648.html)
http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/sunxinhere/article/details/7968886
目前业界有很多MQ产品，我们作如下对比：
## RabbitMQ
是使用Erlang编写的一个开源的消息队列，本身支持很多的协议：AMQP，XMPP, SMTP, STOMP，也正是如此，使的它变的非常重量级，更适合于企业级的开发。同时实现了一个经纪人(Broker)构架，这意味着消息在发送给客户端时先在中心队列排队。对路由(Routing)，负载均衡(Load balance)或者数据持久化都有很好的支持。
## Redis
是一个Key-Value的NoSQL[数据库](http://lib.csdn.net/base/mysql)，开发维护很活跃，虽然它是一个Key-Value数据库存储系统，但它本身支持MQ功能，所以完全可以当做一个轻量级的队列服务来使用。对于RabbitMQ和[Redis](http://lib.csdn.net/base/redis)的入队和出队操作，各执行100万次，每10万次记录一次执行时间。[测试](http://lib.csdn.net/base/softwaretest)数据分为128Bytes、512Bytes、1K和10K四个不同大小的数据。实验表明：入队时，当数据比较小时[redis](http://lib.csdn.net/base/redis)的性能要高于RabbitMQ，而如果数据大小超过了10K，Redis则慢的无法忍受；出队时，无论数据大小，Redis都表现出非常好的性能，而RabbitMQ的出队性能则远低于Redis。
||入队|出队| | | | | | |
|----|----|----|----|----|----|----|----|----|
||128B|512B|1K|10K|128B|512B|1K|10K|
|Redis|16088|15961|17094|25|15955|20449|18098|9355|
|RabbitMQ|10627|9916|9370|2366|3219|3174|2982|1588|
## ZeroMQ
号称最快的消息队列系统，尤其针对大吞吐量的需求场景。ZMQ能够实现RabbitMQ不擅长的高级/复杂的队列，但是开发人员需要自己组合多种技术框架，技术上的复杂度是对这MQ能够应用成功的挑战。ZeroMQ具有一个独特的非中间件的模式，你不需要安装和运行一个消息服务器或中间件，因为你的应用程序将扮演了这个服务角色。你只需要简单的引用ZeroMQ程序库，可以使用NuGet安装，然后你就可以愉快的在应用程序之间发送消息了。但是ZeroMQ仅提供非持久性的队列，也就是说如果down机，数据将会丢失。其中，Twitter的Storm中使用ZeroMQ作为数据流的传输。
## ActiveMQ
是Apache下的一个子项目。 类似于ZeroMQ，它能够以代理人和点对点的技术实现队列。同时类似于RabbitMQ，它少量代码就可以高效地实现高级应用场景。RabbitMQ、ZeroMQ、ActiveMQ均支持常用的多种语言客户端 C++、[Java](http://lib.csdn.net/base/java)、.Net,、[Python](http://lib.csdn.net/base/python)、 [PHP](http://lib.csdn.net/base/php)、 Ruby等。
## Jafka/Kafka
Kafka是Apache下的一个子项目，是一个高性能跨语言分布式Publish/Subscribe消息队列系统，而Jafka是在Kafka之上孵化而来的，即Kafka的一个升级版。具有以下特性：快速持久化，可以在O(1)的系统开销下进行消息持久化；高吞吐，在一台普通的服务器上既可以达到10W/s的吞吐速率；完全的分布式系统，Broker、Producer、Consumer都原生自动支持分布式，自动实现复杂均衡；支持[Hadoop](http://lib.csdn.net/base/hadoop)数据并行加载，对于像[hadoop](http://lib.csdn.net/base/hadoop)的一样的日志数据和离线分析系统，但又要求实时处理的限制，这是一个可行的解决方案。Kafka通过Hadoop的并行加载机制来统一了在线和离线的消息处理，这一点也是本课题所研究系统所看重的。Apache Kafka相对于ActiveMQ是一个非常轻量级的消息系统，除了性能非常好之外，还是一个工作良好的分布式系统。
其他一些队列列表HornetQ、Apache Qpid、Sparrow、Starling、Kestrel、Beanstalkd、Amazon SQS就不再一一分析。

