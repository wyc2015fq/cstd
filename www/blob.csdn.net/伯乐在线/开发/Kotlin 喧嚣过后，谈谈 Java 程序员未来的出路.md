# Kotlin 喧嚣过后，谈谈 Java 程序员未来的出路 - 文章 - 伯乐在线
原文出处： [世相微语](https://mp.weixin.qq.com/s/NIn2hOUZDAq4sdA4TsSNYg)
[自从谷歌 I/O 2017宣布将支持 Kotlin 作为 Android 开发的 First-Class 语言](http://blog.jobbole.com/111024/)，各种 Kotlin 能否取代 Java 的文章、争论层出不穷。 作为一名老 Java 程序员，虽然类似场面见过不少，但心态是比较复杂的。
早在 2009 年，就有了 Scala 是否是 Java 后继者的争论。2009年7月 InfoQ 发表的一篇文章：「Roundup: Scala as the long term replacement for Java」，还谈到了 Java 之父 James Gosling 也偏爱 Scala 。（参考下图）
![](http://jbcdn2.b0.upaiyun.com/2017/06/077c6c331d941ed20a062ac2efd822d2.png)
尽管 Scala 在业界的口碑不错，还受到不少自命极客的程序猿的拥趸。在 Java 程序员当中，也掀起了一阵学习 Scala 的风潮。但时至今日，取代 Java，还远远谈不上。
Java 从诞生到现在，已有22年的历史。Java 在业界的霸主地位，虽然这些年一直受到 C/C++ 、Node.js 、GO 、Python 等语言的挑战，但一直稳如泰山。在众多领域的普及率、Java 不是榜首，也是名列前茅。下图是 2017年5月 最新的 TIOBE 编程语言排行榜
![](http://jbcdn2.b0.upaiyun.com/2017/06/18c7a545e04c4d27f284116a3ae94702.png)
Java 依然是遥遥领先，不过相比较去年同期，Java 下降了 6.32% 。Java 的普及率从 2016年年初以来一直呈下降趋势，Java 程序员的未来之路该如何走，还是值得思考一下。
**懂得居安思危、未雨绸缪，未来的运气才不会太差。**
## **Java 生态圈**
Java 的生态环境开放、自由，在Sun/Oracle、Google、Apache、Eclipse基金会等各大厂商，还有技术大牛的共同努力下，Java 的生态圈异常繁荣，各种优秀的开源框架层出不穷。
**Spring Boot**
Spring Boot 是 Pivotal 团队推出的一个支持快速开发的框架，伴随 Spring4.0 而生，继承了 Spring 的优秀特质，简化了使用 Spring 编码、配置、部署的过程，使项目的开发变得简单、敏捷。
**Spring Cloud**
Spring Cloud 是基于 Spring Boot 的一整套分布式系统下的微服务构建框架，包含了众多的子项目，如 Spring Cloud Config 、Spring Cloud Stream 等。
具体可参考：[http://projects.spring.io/spring-cloud/](http://projects.spring.io/spring-cloud/)
**Hadoop/Spark**
Hadoop 是首个获得极大应用的大数据框架，是大数据领域标志性的解决方案。
Spark 通过完善的内存计算和处理优化，极大的提升了速度，是具备流处理能力的下一代批处理框架。
Spark 体系还包括一系列附加库，如Spark Streaming 、Spark MLlib、Spark GraphX、SparkNet、CaffeOnSpark等。
**Kafka**
Kafka 是 LinkedIn 使用 Scala 开发的一个分布式消息中间件，可以实现不同应用之间的松耦合，由于其可扩展、高吞吐、低延迟、高可靠等特性而被广泛使用。
具体可参考：[https://kafka.apache.org/intro](https://kafka.apache.org/intro)
**ElasticSearch**
ElasticSearch 是基于 Lucene 的实时分布式搜索引擎，由于其搜索稳定、可靠，速度快、安装方便等特点，是使用最广泛的开源搜索引擎之一。
具体可参考《Elasticsearch: 权威指南》：[https://www.elastic.co/guide/cn/elasticsearch/guide/current/index.html](https://www.elastic.co/guide/cn/elasticsearch/guide/current/index.html)
**Nutch**
Nutch 是 Apache 旗下的高度可扩展、可伸缩、可插拔的开源网络爬虫框架，功能完整。
当然爬出框架还有很多：Heritrix、Crawler4j、WebCollector、WebMagic、SeimiCrawler、HtmlUnit 等，可根据实际项目需要选择。
在爬虫领域，Python 可能使用的更多一些，入门也简单。
爬虫的难点不在于语言的选择，无论 Java 、Python 都可以胜任，关键还是反反爬策略的制定，以及各种实战的积累。
**ethereumj**
ethereumj 是以太坊协议的纯 Java 实现。不过在区块链领域，Go 语言的使用率可能更高一些。开发人员可以酌情选择。
代码可参考：[https://github.com/ethereum/ethereumj](https://github.com/ethereum/ethereumj)
**Deeplearning4J**
DeepLearning4J（DL4J）是基于Java语言的分布式深度学习框架，在机器学习领域应用广泛。
入门可以参考《DL4J快速入门》：[https://deeplearning4j.org/cn/quickstart](https://deeplearning4j.org/cn/quickstart)
不过由于谷歌的 TensorFlow 提供的 java API 也会日趋完善，目前不少 java 程序员也会选择 TensorFlow 作为机器学习的首选框架。
以上只是个人正在使用或练手，并且比较看好的一些开源框架。如果想了解更多，可以去 Github 上搜索。
譬如，在 GitHub 的 Search 框，输入 java stars:>10000，可以搜索到星数最多、最热门的一些 Java 开源框架，如图：
![](http://jbcdn2.b0.upaiyun.com/2017/06/3e48861d07ac2cf1d75cc91ed59c710f.png)
如果想发掘 Java 前沿的开源项目，可以选择 Explore，点击 Trending 按钮，就可以看到近期热门的一些开源项目。
可以选择 today「当天热门」、this week「一周热门」和 this month「一月热门」，还可以在右侧选择语言来查看，比如选择 Java 语言，如图：
![](http://jbcdn2.b0.upaiyun.com/2017/06/12a66f81788e8cbed51d8c2703199a8b.png)
*注：觉得好的，记得点击 star，大神贡献代码也需要鼓励  :)*
可以看到，得益于最近被谷歌钦点为 Android 的官方语言，JetBrains 的 Kotlin 这一周成了最大的热门。
Java 的生态圈如此繁盛，Java 离陨落还尚需时日吧。
普通的 Java 程序员能更多的学习、使用这些优秀的框架，有能力的还可贡献一些代码，融入这个繁荣的生态，在我看来是上上策。
## **JVM 语系**
Java 功能强大、生态丰富，已无需多言。但类型、语法与编程范式方面，存在一些局限性，也长期为人所诟病。
所以，有不少开发人员试图去寻求其他语言，既可以突破 Java 的这些局限，又可以与 Java 和谐共处。
Java 的祖师爷们也是非常明智，一开始就将编程语言 Java 与运行时环境 JVM 分离。
JVM 负责与操作系统的交互，屏蔽了具体操作系统的细节，制订了一整套生成字节码的规范，使得前面这一构想得以顺利实施。
如今有众多语言运行在 JVM 上，Groovy、Scala、Kotlin、Clojure 可能算得上是其中的佼佼者。
**Groovy**
Groovy 是类似 Ruby 的动态语言，适合编写脚本、插件、测试例，DSL (领域特定语言)等。知名的自动化建构工具 Gradle 主要就基于 Groovy 。
**Scala**
Scala 的设计初衷，就是在 JVM 上整合面向对象与函数式编程的各种特性。
大名鼎鼎的大数据处理框架 Spark、消息中间件 Kafka、用于构建可容错，可扩展、高并发应用的Akka，都是基于 Scala 语言编写。
**Kotlin**
前几日刚升级为 Android 官方开发语言的 Kotlin ，近期是一个热门。从前面讲到的 Github 上的 star 指数也可窥见一斑。
基于 Kotlin 的库和框架、官方工具的支持、教学视频、文档等，也肯定会层出不穷。
**Clojure**
Clojure 被认为是基于 JVM 的 Lisp 方言，是函数式编程的铁杆。知名的大数据处理框架 Storm 主要就是由 Clojure 语言编写而成。
**Ceylon**
Ceylon 是面向企业级应用设计开发的强静态类型编程语言，支持 Java 和 JavaScript 虚拟机。
具体可参考：[https://ceylon-lang.org/documentation/current/](https://ceylon-lang.org/documentation/current/)
下图是 2017 年的 JVM 语言使用率调查：
![](http://jbcdn2.b0.upaiyun.com/2017/06/8dd51871fd25a9253ff965370e29c889.png)
*来源：Baeldung，链接：**[http://www.baeldung.com/java-in-2017](http://www.baeldung.com/java-in-2017)*
注：Groovy 达到了将近40％的份额，有点出乎我的意料。另外，得益于谷歌这么推一把，Kotlin 后续的比例有望来个大跃进。
2008 年开始，Oracle主办的 JVM 语言峰会，为 JVM 语系的架构师、设计师、工程师，提供了非常好的开放式合作的机遇。
顺便说一下，今年的 JVM 语言峰会，时间：7月31日~8月2日，地点：Oracle Santa Clara Campus，有兴趣的童鞋可以关注一下。
作为一名 Java 程序员，如果要选择第二门编程语言、或想来个多语言编程的话，选择 JVM 系的语言显然是一个非常不错的选择。
选择 JVM 系语言的一大优势是，前期可以使用熟悉的 Java 搭架子，局部功能优先尝试一下新语言，如此风险最小，与 Java 可以实现无缝切换。
至于具体如何选择，就要看你在功能性、实用性、舒适性、有趣性之间，如何取舍了。
## **结语**
**Java 自身在性能、语法、编程范式等方面，也是在不断地提高改进。**
譬如 Java 8 引入了 Lambda 表达式、Optional、Streams、Date/Time API 等，Java 9 带来了 Jigsaw（模块化）等。
Java 程序员可以适时的与时俱进。
不过，谷歌等其他厂商由于对 Java 不具备掌控力，还有吃官司的风险，寻求 Java 的替代方案，动机是存在的。
所以，作为 Java 程序员，过于依赖 Java，也是比较有风险的。
**适时的主动去改变，不要让自己哪天陷于被动。**
**除了学习第二、第三编程语言，程序员可以把更多的重心，放在算法上。**
以后的生活无处不算法，从娱乐影视推荐、新闻资讯过滤，到出行购物、住宿交友等等，我们都离不开算法。
尽管许多优秀的算法，包括最前沿的深度学习学法，都会有大牛奉献开源实现，但只有其中的原理深刻理解了，才能掌握、并利用好这些算法。
## **后记**
**技术的更迭，有时候会来的很突然，很猛烈。但系统的更迭，往往会慢得多。**
譬如 Cobol 语言，尽管语言本身陨落已超过 20 年，依然能在银行、证券、保险、电信等行业领域里见到 Cobol 的身影。
即使几年之后，Java 因各种阴差阳错、机缘巧合，开始走向陨落，Java 程序员们再混个 20 年，也是毫无压力。
**因为 Java 常年占据编程语言前列（榜首），目前有太多的平台使用 Java 了。**
Amazon、Google、eBay、阿里、京东、银行、证券、医疗、移动、电信、ERP等等。
各个领域，各行各业，Java 程序员吃老本也能再吃个 20 年……
当然，这只是 Java 抽中了下下签，您又恰巧选择了下下策。但愿不会如此。
**最后，推荐一幅比较全面、而又有趣的 JVM 语系的海盗图**：Pirates of the JVM
链接：*[https://jaxenter.com/wp-content/uploads/2017/03/Pirates-of-the-JVM-JAXenter-768×4245.png](https://jaxenter.com/wp-content/uploads/2017/03/Pirates-of-the-JVM-JAXenter-768x4245.png)*
