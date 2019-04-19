# Java EE的未来 - =朝晖= - 博客园
# [Java EE的未来](https://www.cnblogs.com/dhcn/p/7130848.html)
http://www.infoq.com/cn/articles/enterprise-[Java](http://lib.csdn.net/base/java)-opinion
作为InfoQ下一年编辑关注点审核工作的一部分，我们挑选了Java作为深入探讨的主题。我们借用了Geoffrey Moore在他的经典书籍《跨越鸿沟》里的所描述的模型，这个模型贴切地概括了技术的生命周期，它包含五个阶段：创新者（Innovators）、早期采用者（Early Adopters）、早期大众（Early Majority）、后期大众（Late Majority）和落后者（Laggards）。Moore的模型在早期采用者和早期大众两个阶段之间引入了一个大缺口，早期采用者是真正愿意冒着风险使用新技术的人，而早期大众在确认新技术能够提升生产效率之前是不会采取行动的。
从下图可以看出Java技术的整体趋势：
![](http://cdn2.infoqstatic.com/statics_s1_20170411-0445/resource/articles/enterprise-java-opinion/zh/resources/000.jpg)
我们可以看到，[Java](http://lib.csdn.net/base/java)SE仍然保持着它的健康度，它仍然是一个被广泛使用的企业计算编程语言。[java](http://lib.csdn.net/base/java)9有望在今年夏天发布，届时将带来Jigsaw和JShell REPL。而Java 10的开发工作已经在进行当中。我们相信Java仍然是构建大规模企业应用的一个很好的选择，最起码现在人们不希望Java走出生产环境。
说到基于JVM的编程语言，不得不提到[Scala](http://lib.csdn.net/base/scala)和Clojure。不过从[scala](http://lib.csdn.net/base/scala)的受欢迎程度来看，它的采用率或许已经达到了最顶峰。而在Java 8推出Lambda功能时，Scala的受欢迎程度有轻微的下降。我们认为Scala还未能跨越Moore模型里的“鸿沟”，所以它仍然处在早期采用者阶段。我们不认为它会达到大众阶段。
Clojure一如既往的好，作为[Python](https://www.infoq.com/articles/data-science-abstraction)的有力竞争者，面向数据的特性让它处在一个很有利的位置上，不过它不像是一门会被广泛采用的语言。
作为Java之外的选择，Groovy仍然是很受欢迎的脚本语言，同时，JetBrain的[Kotlin](https://kotlinlang.org/)似乎在[Android](http://lib.csdn.net/base/android)开发方面为自己找到了一个稳固的位置，不过这两门语言都无法动摇Java的统治地位。
可以说，[Java SE](http://lib.csdn.net/base/javase)形势一片大好，但对于[Java EE](http://lib.csdn.net/base/javaee)来说却不是如此。我们在考虑要不要把[java ee](http://lib.csdn.net/base/javaee)放入“落后者”这一类，不过最近的一些迹象表明，它应该可以被放在后期大众里，这要归功于Java EE底层的那些技术。jClarity的联合创始人Ben Evans，同时也是InfoQ的编辑，他说：
> 
我不得不说，Java EE是一个正在消褪的品牌，不过它的底层核心技术让情况看上去没有那么糟糕。
另一位编辑Chris Swan，同时也是CSC的CTO，他说：
> 
Java EE有一些重要的规范，它们很有用。EJB作为Java EE的心脏，它把Java应用服务器推向了企业。[spring](http://lib.csdn.net/base/javaee)去掉了EJB的复杂性，“EE”变成了“在Java应用服务器上运行Spring”的代名词。传统的应用服务器市场（由几个大厂商所统治）正在被PaaS所代替（因为你不需要整套臃肿的应用服务器就能运行Spring应用），而企业对服务器厂商的“绑架”行为深感厌恶，同时PaaS提供商提供了更具竞争力的运营能力，从而加剧了这种情况的发生。
InfoQ之前[报道](https://www.infoq.com/news/2016/12/Gartner-downgrades-Java-EE?utm_source=infoq&utm_campaign=user_page&utm_medium=link)过Gartner对技术走势的研究，Gartner在这方面比我们做得更为深入，而最近Gartner再次发表其观点，认为“Java EE和其它三层[架构](http://lib.csdn.net/base/architecture)的框架，比如ASP[.NET](http://lib.csdn.net/base/dotnet)”正在走向衰退：
> 
数字信息业务要求应用平台提供新的特性和能力，而Java EE没能赶上这趟班车。
负责应用基础设施现代化的领导者们应该想办法应对Java EE的衰退问题。
到2019年，只有不到35%的新应用会被部署在Java EE应用服务器上。
不到35%，也就是差不多三分之一，不过我们认为尽管Java EE应用服务器还会存在，但只会被用于运行遗留的系统。
不过对于Java来说，还是有一些好消息的。一些强有力的框架为构建[微服务](http://lib.csdn.net/base/microservice)提供了更多的选择，Pivotal在这方面一路领先，它为微服务提供了一整个技术栈。[Spring Boot](https://projects.spring.io/spring-boot/)内嵌了Tomcat、Jetty或Undertow，可以方便快速地用它来构建独立运行的Spring应用。[Spring Cloud](http://projects.spring.io/spring-cloud/)由美国的Netflix和中国的阿里巴巴共同支持，为Spring Boot加入了很多强大的云组件，比如服务发现、外部配置、回路断路器和负载均衡。[Pivotal Cloud Foundry](https://pivotal.io/platform)、IBM的[Bluemix](https://www.ibm.com/cloud-computing/bluemix/)和SAP的[HANA](http://www.sap.com/uk/product/technology-platform/hana.html)为此提供了商业支持。
除此之外，还有很多其他的可选方案。其中有三个值得关注，它们分别是Lightbend的[Lagom](https://www.lightbend.com/lagom)、Eclipse [Vert.x](http://vertx.io/)和[Ratpack](https://ratpack.io/)，其中Lagom是基于Play和Akka的。而Akka所使用的actor模型也非常有意思。
我们注意到，[Oracle](http://lib.csdn.net/base/oracle)曾经声称要在今年的Java EE 8里将Java EE带入云端，并具备基本的微服务能力，不过[oracle](http://lib.csdn.net/base/oracle)并未透露更多相关细节。Java EE 9有望在2019发布，不知道在未来的这一两年，Java世界能否发生一些不一样的变化。
[MicroProfile.io](http://microprofile.io/)最近加入了[Eclipse Foundation](https://www.infoq.com/news/2017/02/microprofile-eclipse-foundation)，广大的Java EE社区把工作转向了MicroProfile.io。
以上观点并不代表我们将对Java EE开发失去兴趣，或者说它将会消失。我们只是认为，对于构建现代化的系统，我们总是会有更好的选择。

