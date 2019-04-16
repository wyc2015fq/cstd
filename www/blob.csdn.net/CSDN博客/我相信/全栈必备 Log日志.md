# 全栈必备 Log日志 - 我相信...... - CSDN博客





2017年06月12日 08:14:59[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：19210
所属专栏：[全栈之路](https://blog.csdn.net/column/details/12818.html)









Log日志，不论对开发者自身，还是对软件系统乃至产品服务都是非常重要的事情。每个开发者都接触过日志，以至于每个人对日志的了解都会有所不同。

![这里写图片描述](https://img-blog.csdn.net/20170611150100457?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### 什么是日志

日志是什么呢？老码农看来，日志是带有明确时间标记的信息数据记录。

根据不同的使用场景和视角，日志的分类也多种多样。针对生成日志的主体，如果是开发者，可以有工作或学习日志，如果是代码，可以是程序日志。根据应用程序的使用领域，可以有系统日志和应用日志，还可以进一步细分，例如消息日志，事件日志，数据库日志等等。 

### 记日志的方式

对于开发者而言，随手记日志（笔记）是一个良好的工作习惯。将工作或学习中遇到的问题，解决方案，新的体会随时记下来，日积月累，就可以逐渐形成自己的知识体系。作为一个Evernote 的深度用户，老码农推荐使用Evernote，当然有道云笔记之类的其他工具同样有效。开发者自身的日志一般都是给自己看的，记录工作或学习日志的方式因人而异，适合自己就好。

而程序代码中的日志，多用于应用中相关信息的跟踪记录，要便于其他开发者或者其他应用程序使用，一般是由日志标准的。广泛使用的日志记录标准是RFC 5424（[https://tools.ietf.org/html/rfc5424](https://tools.ietf.org/html/rfc5424)）中定义的syslog。 syslog标准能够标准化系统生成，过滤，记录和分析日志消息。例如，其中定义的日志重要等级是这样的：

```
0       Emergency: system is unusable
          1       Alert: action must be taken immediately
          2       Critical: critical conditions
          3       Error: error conditions
          4       Warning: warning conditions
          5       Notice: normal but significant condition
          6       Informational: informational messages
          7       Debug: debug-level messages
```

在实际应用中，根据应用场景，有可能不完全遵从RFC 5424， 但依然可以从各种各样日志工具库中看到他们的影子，参考《全栈的技术栈设想》，对于全栈必备的几种编程语言而言，Java中的Log4j，Python 中的Logging， JavaScript 中的log4JS，遗憾的是，老码农没有在Objective－c 中使用过较好的日志库，还在用将nslog 写入文件的方式记日志，在C/C++中到是曾经用过log4CXX以及zlog。 

在python 的世界里，有着这样相对偏执的说法，“以日志调试为荣，以单步跟踪为耻”。虽然有矫枉过正的嫌疑，但充分说明了记log日志的重要性。

### 常见应用中的日志

成熟的系统中都有着自己的日志子系统，例如windows的系统日志，Linux的syslog，docker的容器日志等。这里简要回顾一些常见后端服务中的日志：Nginx日志，mysql 日志 和 tomcat 日志。

Nginx日志主要分为两种：访问日志和错误日志。访问日志主要记录客户端访问Nginx的每一个请求，格式可以自定义。通过访问日志，可以得到用户地域来源、跳转来源、使用终端、某个URL访问量等相关信息。错误日志主要记录客户端访问Nginx出错时的日志，格式不支持自定义。通过错误日志，可以得到系统某个服务或server的性能瓶颈等。在分析nginx 日志时， 老码农目前比较喜欢的工具是goaccess.

Tomcat下相关的日志文件有Cataline引擎的日志文件，文件名catalina.日期.log；Tomcat下内部代码丢出的日志，文件名localhost.日期.log；Tomcat下默认manager应用日志，文件名manager.日期.log ；控制台输出的日志，Linux下默认重定向到catalina.out ；通过Servlet.xml配置的Access日志，应用程序以log4j.properties：${catalina.base}/logs/probe.log重定向的日志等等。 就分析工具而言，老码农目前觉得Awstats是个不错的工具。

MySQL有以下几种日志：
- 错误日志：记录启动、运行或停止时出现的问题，一般也会记录警告信息。 
- 一般查询日志：记录建立的客户端连接和执行的语句。
- 慢查询日志：记录所有执行时间超过long_query_time秒的所有查询或不使用索引的查询，可以帮我们定位服务器性能问题。
- 二进制日志：任何引起或可能引起数据库变化的操作，主要用于复制和即时点恢复。
- 中继日志：从主服务器的二进制日志文件中复制而来的事件，并保存为的日志文件。
- 事务日志：记录InnoDB等支持事务的存储引擎执行事务时产生的日志。

MySQL 中的日志不仅仅是跟踪信息，而且成为了业务系统的一部分。就日志分析而言，老码农最喜欢的就是percona-toolkit了，尤其是其中的慢查询分析工具pt-query-degist.

根据应用中的日志，可以获知系统中更有价值的信息，而且可以为自己的日志系统提供资粮。

### 日志分析平台

对单应用实体的日志分析，可以有一些非常不错的工具，例如上面提到的pt，goaccess等等。但是，对于整个应用系统而言，在工作流或任务链上的每个服务组件都会产生日志，那么如何分析整个业务系统的日志呢？

老码农觉得ELK是日志分析平台的一个上佳选择。ELK由Elasticsearch、Logstash和Kibana三部分组件组成：Elasticsearch是个开源分布式搜索引擎，它的特点是分布式，零配置，自动发现，索引自动分片，索引副本机制，restful风格接口，多数据源，自动搜索负载等。Kibana 是一个开源和免费的工具，可以为 Logstash 和 ElasticSearch 提供日志分析结果的Web可视化界面，汇总、分析和搜索重要数据日志。Logstash是一个完全开源的工具，可以对各种日志进行收集、分析，并将其存储供,logstash 的工作示意图如下： 
![这里写图片描述](https://img-blog.csdn.net/20170611150140067?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在ELK方案中，logstash 进行日志采集和预处理，ES 完成分布式检索，Kibaba 做最终的分析结果可视化。随之需求的变化， 基于ELK 之上还有很多变种，例如引进Kafka消息队列，增加Storm 实时分析等等，都使得基于日志的应用实现更大的价值化。
更进一步，会形成统一日志平台，逐渐形成业务系统的一部分，例如支持数据的最终一致性等等。连自己都没想到，日志居然如此的丰富多彩，还有很多有趣的问题，例如日志的动态开关配置，日志传输中的准实时性，写日志的性能损耗等等。 
![这里写图片描述](https://img-blog.csdn.net/20170611150222411?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lyZWxlc3NfY29t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

综上，log日志的纪录和分析，不仅仅是一种全栈必备的技能，更是一种开发者的基本素养。




