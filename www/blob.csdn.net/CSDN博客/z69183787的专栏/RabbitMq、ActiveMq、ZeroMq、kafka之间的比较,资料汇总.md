# RabbitMq、ActiveMq、ZeroMq、kafka之间的比较,资料汇总 - z69183787的专栏 - CSDN博客
2016年09月19日 14:17:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1965
MQ框架非常之多，比较流行的有RabbitMq、ActiveMq、ZeroMq、kafka。这几种MQ到底应该选择哪个？要根据自己项目的业务场景和需求。下面我列出这些MQ之间的对比数据和资料。
第一部分：RabbitMQ,ActiveMq,ZeroMq比较
1、 TPS比较
 一
ZeroMq 最好，RabbitMq 次之， ActiveMq 最差。这个结论来自于以下这篇文章。
[http://blog.x-aeon.com/2013/04/10/a-quick-message-queue-benchmark-activemq-rabbitmq-hornetq-qpid-apollo/](http://blog.x-aeon.com/2013/04/10/a-quick-message-queue-benchmark-activemq-rabbitmq-hornetq-qpid-apollo/)
测试环境：
     Model: Dell Studio 1749
     CPU: Intel Core i3 @ 2.40 GHz
     RAM: 4 Gb
     OS: Windows 7 64 bits
其中包括持久化消息和瞬时消息的测试。注意这篇文章里面提到的MQ，都是采用默认配置的，并无调优。
![](https://img-blog.csdn.net/20150819152603760?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
更多的统计图请参看我提供的文章url。
2、TPS比较二
ZeroMq 最好，RabbitMq次之， ActiveMq最差。这个结论来自于一下这篇文章。[http://www.cnblogs.com/amityat/archive/2011/08/31/2160293.html](http://www.cnblogs.com/amityat/archive/2011/08/31/2160293.html)
显示的是发送和接受的每秒钟的消息数。整个过程共产生1百万条1K的消息。测试的执行是在一个Windows Vista上进行的。
![](https://img-blog.csdn.net/20150819152931287?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3、持久化消息比较
zeroMq不支持，activeMq和rabbitMq都支持。持久化消息主要是指：MQ
 down或者MQ所在的服务器down了，消息不会丢失的机制。
4、技术点：可靠性、灵活的路由、集群、事务、高可用的队列、消息排序、问题追踪、可视化管理工具、插件系统、社区
      RabbitMq最好，ActiveMq次之，ZeroMq最差。当然ZeroMq也可以做到，不过自己必须手动写代码实现，代码量不小。尤其是可靠性中的：持久性、投递确认、发布者证实和高可用性。
所以在可靠性和可用性上，RabbitMQ是首选，虽然ActiveMQ也具备，但是它性能不及RabbitMQ。
 5、高并发
从实现语言来看，RabbitMQ最高，原因是它的实现语言是天生具备高并发高可用的erlang语言。
总结：
按照目前网络上的资料，RabbitMQ、activeM、zeroMQ三者中，综合来看，RabbitMQ是首选。下面提供一篇文章，是淘宝使用RabbitMQ的心得，可以参看一些业务场景。
[http://www.docin.com/p-462677246.html](http://www.docin.com/p-462677246.html)
第二部分：kafka和RabbitMQ的比较
关于这两种MQ的比较，网上的资料并不多，最权威的的是kafka的提交者写一篇文章。[http://www.quora.com/What-are-the-differences-between-Apache-Kafka-and-RabbitMQ](http://www.quora.com/What-are-the-differences-between-Apache-Kafka-and-RabbitMQ)
里面提到的要点：
1、  RabbitMq比kafka成熟，在可用性上，稳定性上，可靠性上，RabbitMq超过kafka
2、  Kafka设计的初衷就是处理日志的，可以看做是一个日志系统，针对性很强，所以它并没有具备一个成熟MQ应该具备的特性
3、  Kafka的性能（吞吐量、tps）比RabbitMq要强，这篇文章的作者认为，两者在这方面没有可比性。
这里在附上两篇文章，也是关于kafka和RabbitMq之间的比较的：
1、http://www.mrhaoting.com/?p=139
2、http://www.liaoqiqi.com/post/227
总结：
两者对比后，我仍然是选择RabbitMq，性能其实是很强劲的，同时具备了一个成熟的MQ应该具有的特性，我们无需重新发明轮子。
好资料推荐：
            1、最全最给力的kafka博客：http://blog.csdn.net/lizhitao/article/category/2194509
           2、淘宝对rabbitmq的使用：http://www.docin.com/p-462677246.html
