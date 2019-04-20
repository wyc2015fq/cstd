# 干货|kafka流量监控的原理及实现 - Spark高级玩法 - CSDN博客
2018年10月09日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：762
**工程能力**
作为一个优秀的开发人员，项目开发的过程中监控告警系统的可靠性是可以体现出一个人的工程管理能力的。优秀的监控告警系统可以免去很多精力消耗，比如维护，故障预判，故障及时准确通知，故障定位排查等。
可以想像项目上线后，假如没有监控告警系统，这么一个暗箱是多么可怕。
对于大数据项目，数据一般需要先入消息队列，如kafka，然后分离线和实时将数据进行解耦分流，用于实时处理和离线处理。消息队列存在的好处：
- 
消息队列的订阅者可以根据需要随时扩展，可以很好的扩展数据的使用者。
- 
消息队列的横向扩展，增加吞吐量，做起来还是很简单的。这个用传统数据库，分库分表还是很麻烦的。
- 
由于消息队列的存在，也可以帮助我们抗高峰，避免高峰时期后端处理压力过大导致整个业务处理宕机。
kafka在大数据项目中作用至关重要，那么对其的监控告警就至关重要了，我们这里主要是讲针对kafka流量的监控告警，其目的也是很明显的便于我们了解数据的整体情况及波动情况，以调整处理后端，如spark streaming，flume等。
kafka 监控工具很多，常见的有kafka manager，KafkaOffsetMonitor，kafka eagle，kafka tools等，浪尖最经常使用的是kafka manager，也建议大家使用该工具，其不仅有监控功能还有管理功能。具体使用方法可以参看：
[kafka管理神器-kafkamanager](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485370&idx=2&sn=406e4e799928460f9cfbafe0665b4f58&chksm=9f38e492a84f6d849fcb40eaf504cfda61daecd3f14129b4f939391bfea237b9a763cdf0db57&scene=21#wechat_redirect)
**监控指标**
kafka的指标服务器和客户端都有的。具体指标内容，可以参看kafka官网：
http://kafka.apache.org/0102/documentation.html#monitoring
查看可用指标的最简单方法是启动jconsole并将其指向正在运行的kafka客户端或服务器; 这将允许使用JMX浏览所有指标。
对于熟悉kafka manager的朋友都应该看过broker相关信息，比如每秒钟的流入的消息条数，每秒钟的流入的消息大小，流出的消息大小等。
使用kafka manager可以很方便的查看。但是，这其实不能让我们及时的发现数据流量波动，或者说我们想画个曲线的详细对比历史流量，它是做不到的。所以，我们要想办法去获取出来这些指标，然后做我们自己的展示。还有一点就是，流量波动告警。
浪尖这里只做了图中几个指标的接口：
defgetBytesInPerSec(kafkaVersion: KafkaVersion, mbsc: MBeanServerConnection, topicOption: Option[String] = None) = {
    getBrokerTopicMeterMetrics(kafkaVersion, mbsc, "BytesInPerSec", topicOption)
  }
defgetBytesOutPerSec(kafkaVersion: KafkaVersion, mbsc: MBeanServerConnection, topicOption: Option[String] = None) = {
    getBrokerTopicMeterMetrics(kafkaVersion, mbsc, "BytesOutPerSec", topicOption)
  }
defgetBytesRejectedPerSec(kafkaVersion: KafkaVersion, mbsc: MBeanServerConnection, topicOption: Option[String] = None) = {
    getBrokerTopicMeterMetrics(kafkaVersion, mbsc, "BytesRejectedPerSec", topicOption)
  }
defgetFailedFetchRequestsPerSec(kafkaVersion: KafkaVersion, mbsc: MBeanServerConnection, topicOption: Option[String] = None) = {
    getBrokerTopicMeterMetrics(kafkaVersion, mbsc, "FailedFetchRequestsPerSec", topicOption)
  }
defgetFailedProduceRequestsPerSec(kafkaVersion: KafkaVersion, mbsc: MBeanServerConnection, topicOption: Option[String] = None) = {
    getBrokerTopicMeterMetrics(kafkaVersion, mbsc, "FailedProduceRequestsPerSec", topicOption)
  }
defgetMessagesInPerSec(kafkaVersion: KafkaVersion, mbsc: MBeanServerConnection, topicOption: Option[String] = None) = {
    getBrokerTopicMeterMetrics(kafkaVersion, mbsc, "MessagesInPerSec", topicOption)
  }
**jmx客户端**
连接jmx的server是可以使用jconsole，但是满足不了我们的需求。所以，我们使用JMXConnectorFactory 方式连接jmx。使用JMXConnectorFactory 链接jmx时，JMXServiceURL 的参数 url 必须使用 service:jmx 方式进行连接，具体链接创建方式很简单，几行代码而已，如下：
**val **jmxHost = "hostname"
**val **jmxPort = 9999
**val **urlString = s"service:jmx:rmi:///jndi/rmi://**$**jmxHost:**$**jmxPort/jmxrmi"
**val **url = **new **JMXServiceURL(urlString)
**val **jmxc = JMXConnectorFactory.*connect*(url )
**val **mbsc = jmxc.getMBeanServerConnection;
*println*(KafkaMetrics.*getMessagesInPerSec*(Kafka_0_10_2_1,mbsc,*Some*("test")).fifteenMinuteRate)
jmxc.close()
**开启kafka的jmx端口**
kafka的jmx服务默认时关闭的，开启的话很简单，只需要在kafka server的启动脚本kafka-server-start.sh里增加一行代码即可，内容export JMX_PORT="9999"，增加位置如下：
if [ "x$KAFKA_HEAP_OPTS" = "x"]; then
export KAFKA_HEAP_OPTS="-Xmx1G -Xms1G"
export JMX_PORT="9999"
fi
**测试**
我这里测试就比较简单了，主要是将消息条数打出来，大家可以根据需要自行调整，比如均值大于阈值发短信告警等。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWutRWiaJP6pJLicbAdU9zDmX1PvNWbDD9YmMu8BRib29OdNtzicVPpmYFMFD2McpSa8mNwZ5GG11Udnw/640)
本文是自己实现kafka 监控系统的第二篇文章，前面有篇文章讲到了从kafka broker获取消费者已经提交的offset，具体可以阅读：
[如何获取kafka的broker保存的消费者信息？](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485462&idx=1&sn=4cf0c42c5091f16a89294971c9cbd840&chksm=9f38eb3ea84f62281f6881007a3b76acd001bd61331420d7e12a11b0b7837cceaca6d17ef88c&scene=21#wechat_redirect)
一套完整的kafka监控，包括：
- 
消费者监控，主要是存活告警，消费滞后告警。
- 
生产者监控，主要是存活告警，生产者消费上游数据能力告警。
- 
broker监控，主要是存活告警，流量告警，isr列表，topic异常告警，control变换告警。
内容颇多，后面陆续出文章实现，当然整个项目最终会放到星球里的。
本文源码，关注本公众号(bigdatatip)，输入 **1027** 获取。
微信技术交流群，扫描微信二维码，拉入群聊，也可以搜索微信 158570986。
[Spark SQL从入门到精通](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485504&idx=1&sn=e19460f43aa2fe388140a2241e9ea219&chksm=9f38eb68a84f627ee0b6dac115f0684dddba573be04ae7e4e54d17c084cf1946282f81b1fd43&scene=21#wechat_redirect)
[最常见的Kafka面试题及答案](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485515&idx=1&sn=d1a33b86adfef9305fa22d4528f184fe&chksm=9f38eb63a84f6275b476193d011e708f7ec2a976bc1c0180606e0627e580c965d5c42f70fec2&scene=21#wechat_redirect)
[Flume NG：Flume 发展史上的第一次革命](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485570&idx=1&sn=ca7e8fc27f76ee156ba7ef297ca15272&chksm=9f38ebaaa84f62bcfb379629c792a07ba0258877a45dee0579004999daa25b6fea3ca3c6d709&scene=21#wechat_redirect)
更多干货加入浪尖知识星球，179价格最后一天，明天开始199.
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWM4BwonEs9Uh3zFE9Hwh3ib1FwCRsxEXL0rT4xtfyqaaxZA6yjO1EiatC3iaqibyt2fkFy04iaxqa40ZQ/640)
微信交流群，可以扫浪尖微信。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWutRWiaJP6pJLicbAdU9zDmXR7Udmo7TGaR7V7XkSO57VHaKkxyJdqay8UkwyMoibw7J1KDJHgKkM5Q/640)
