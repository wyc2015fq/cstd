# Hadoop Metrics体系分析之一：认识Metrics - aisoo的专栏 - CSDN博客
2012年12月12日 15:17:31[aisoo](https://me.csdn.net/aisoo)阅读数：1190
对于分布式软件系统来说Metrics已经成为了不可缺少的组成部分，通过它我们可以了解系统的运行状况、健康状况、性能状况，通过对历史数据的分析，也可以帮助我们发现系统缺陷以及避免系统不稳定的发生。比如通过JVM数据我们可以优化系统GC策略，通过TPS和latency数据可以获知系统的压力和性能表现。
Hadoop作为目前流行的分布式计算系统，提供了一套简单有效的metrics框架。接下来通过对这个框架的分析和学习，可以帮助我们在软件中构建自己的metrics体系。
**Hadoop支持的metrics方式**hadoop支持两种metrics方式，其中一种是JMX方式。通过将系统内建立的多个metrics 数据转化为dynamic mbeans注册到JVM的JMX Mbean服务器中，就可以利用JMX框架来获取以及发布这些数据。对于我们客户端来说可以使用JConsole来连接指定的JMX服务器。
从客户端%java_home%/bin下面启动jconsole
![](http://www.kuqin.com/upimg/allimg/111221/1S4514557-0.jpg)
服务器metrics数据监控界面
![](http://www.kuqin.com/upimg/allimg/111221/1S4516249-1.jpg)
从上图可以看出利用JMX方式监控与展示数据非常方便，但是受到很多的限制，用起来也不太直观，尤其是集群状况下的成百上千台服务器数据的收集、展示、计算、预警等。因此hadoop支持另外一种方式，通过一个简单的框架，实现数据的收集与发布。发布方式上我们可以选择写文件、ganglia甚至是自定义的方式来对外发布数据，后面会详细介绍这个框架的实现和我们所能做的扩展应用。
**Hadoop metrics与ganglia**hadoop metrics框架只能做到本机数据的收集与发布，在集群环境下成百上千台服务器数据的收集汇总和展示还需要第三方的工具来实现。我们常用的就是ganglia，下图展示的是ganglia的系统结构。Hadoop将metrics数据发送给集群每个节点运行的gmond程序，然后gmetad周期性的轮询gmond收集到的信息，并存入rrd数据库，通过web服务器可以对其进行查询展示。
![](http://www.kuqin.com/upimg/allimg/111221/1S451M35-2.jpg)
Ganglia上展示的metrics数据
![](http://www.kuqin.com/upimg/allimg/111221/1S4515P5-3.jpg)
**基于 metrics的应用**
看到这里，大家对hadoop的metrics应该有大致的了解了，他的作用就是收集、发送数据，但是这些还远远不够，数据如何存储、分析以及预警才是我们最终的目的。而这些系统加在一起才能发挥巨大的作用。后面我们会仔细介绍hadoop metrics框架，通过它来构建我们自己的metrics体系。
