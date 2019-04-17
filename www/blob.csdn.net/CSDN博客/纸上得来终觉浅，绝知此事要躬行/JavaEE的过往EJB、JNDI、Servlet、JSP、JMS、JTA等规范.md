# JavaEE的过往EJB、JNDI、Servlet、JSP、JMS、JTA等规范 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年11月25日 16:09:31[boonya](https://me.csdn.net/boonya)阅读数：679








文章来源：[http://blog.csdn.net/jiaoly/article/details/52993256](http://blog.csdn.net/jiaoly/article/details/52993256)


JavaEE规范涉及的内容很多，这里只对EJB、JNDI、Servlet、JSP、JMS、JTA等规范做一个对比说明。




**1、 从EJB这个失败的规范理解微服务的后端服务**

说起JavaEE规范，要先从EJB（Enterprise [Java](http://lib.csdn.net/base/java)Bean），他是一种用Java实现后端服务的规范。本来EJB是JavaEE中最重要的规范，但EJB出现后，人们一直诟病他过于复杂的使用方式，在[spring](http://lib.csdn.net/base/javaee)出现后，大家其实抛弃了EJB，虽然他自身做了很多改革，以至于EJB
 3.0 后和Spring非常类似，然并卵… … 其实，EJB的设想还是很好的，他把后端服务分为会话Bean（Session Beans）、实体Bean（Entity Beans）、消息驱动Bean（Message Driven Beans）三种模式，前者又分为 无状态会话Bean（Stateless Session Beans）、有状态会话Bean（Stateful Session Beans），最初EJB完全是使用远程调用的，后来由于性能的原因，又加上了本地模式，上述四种EJB 都可以采用本地调用。结合微服务架构，我们来回顾一下：

**首先服务应该被分为本地和远程两种方式**，我一向反对这两种服务处理的透明化，原因是这两种调用在应用开发上差别太大，例如远程调用应该采用异步回调模式，设置明确的超时时间，事务处理不能依赖[数据库](http://lib.csdn.net/base/mysql)事务，数据传递需要序列化，需要传递上下文等等，而本地调用可以简单的多。EJB开始时把所有的东西都做成远程模式，后来由试图两者都支持，结果本来复杂的事情没简单下来，简单的事情反而复杂了，所以我在微服务架构中，把本地和远程服务显示分开，采用不同的API进行调用，对于远程服务需要采用异步模式调用，配置超时时间、数据一致性声明、通讯报文定义等等，不去幻想用一种透明方式进行动态切换，其实把本地服务变成远程服务的工作量是远大于这几行代码开发的，所以本地/远程调用透明化只是一个看起来很美，这一点上EJB是失败的。

**其次，EJB把服务分成无状态和有状态两种**，无状态服务没什么好说的，大家都在做无状态化，以便有利于横向伸缩和弹性。无状态虽好，但是业务其实是有状态的，但Servlet规范中有Session，常见的客户登录信息等状态都维护在Session中，再者还有很多业务状态也可以在客户端维护，例如翻页时的计数器，在客户端保存，每次提交到服务端，这样有状态服务使用的场景在业务上反而少了。但移动设备出现后，多屏融合的需求让我们无法在客户端维护状态了，例如在PC上做一个操作，在手机上做下一步，就只有服务端维护状态才行。服务端维护状态也不是说一定要用有状态服务，因为这些信息可以维护在数据库中，即使考虑性能因素，也可以维护在集中缓存中，服务还是无状态的。上面说了很多，是说明为什么有状态服务使用比较少，但物联网出现后，有状态服务重新有抬头的趋势，例如在读取设备信息时，必须在服务端维护状态，但由于数据量比较大，集中在缓存的方式导致缓存过大，不容易维护，于是就要分而治之，有状态服务就是一个好的选择了。其实，有状态服务经常默默的为我们服务，例如客户端获得一个数据库连接，以后对这个数据库连接做操作时，数据库本身就是维护了一系列的有状态服务，服务状态包括登录信息、缓存数据等上下文信息，每次根据客户端的标识找到这个服务，提供服务。在微服务架构的实现中，需要考虑有状态的模式，可以参考EJB的设计，把远程服务分为无状态和有状态两种。

**实体Bean（Entity Beans）是含有持久化状态的分布式对象。**这个持久化状态的管理既可以交给Bean自身（Bean-Managed Persistence，BMP），也可以托付于外部机制（[Container](http://lib.csdn.net/base/docker)-Managed
 Persistence，CMP）。如果说会话Bean出现的早期还有很多应用，实体Bean一出现就让人感到没法用，分布式对象这玩意，还是太复杂了。我也仅仅是做过Demo而已，从来没有实际的应用，不过在我看来，IBatis和[hibernate](http://lib.csdn.net/base/javaee)应该是BMP和CMP最好的实践了，而IBatis和Hibernate都不是面向分布式应用的，他们都迎合了当时巨石应用的架构模式，以至于EJB
 3.0 中和Hibernate已经非常类似了。在微服务架构下，数据必然是分布式的，而数据的存储方式也从关系数据库拓展到缓存、NoSQL、图等数据存储方式，实体Bean实在是分布式数据的早期探索之一，只不过这个尝试失败了。

消息驱动Bean（Message Driven Beans）是基于JMS事件驱动方式触发后端服务的模式，无非是在EJB之上加一个事件驱动的外壳。微服务架构下，也支持事件驱动的方式，以后再详细论述。

EJB规范的目的在于为企业及应用开发人员实现后台业务提供一个标准方式，自动处理了诸如数据持久化、事务处理、并发控制、基于JMS的事件驱动、基于JNDI的名字和空间管理、基于JCE和JAAS的安全管理、应用服务器端的软件组件部署、使用RMI-IIOP协议的远程过程调用、将业务方法暴露为Web服务、以及如何将EJB部署至EJB容器当中，虽然这是一个不成功的尝试，但这些都是微服务架构需要考虑的问题。



**2、JNDI  Java版的服务发现**

Java命名和目录接口（Java Naming and Directory Interface），是Java的一个目录服务API，它提供一个目录系统，并将服务名称与对象关联起来，从而使得开发人员在开发过程中可以使用名称来访问对象，这个规范是JavaSE的一部分，而JavaEE建立在JavaSE之上，JNDI也是JavaEE一个重要的基石。上面的解释比较拗口，其实解决的是服务注册、发现和配置集中管理问题。看看JNDI的示例：




示例：服务查找

```java
Context ctx = ...  
    Object obj =ctx.lookup("/MyDataSource");
```


示例：注册监听器，可用于监听配置的改变




```java
NamespaceChangeListener listener = ...;  
    src.addNamingListener("x",SUBTREE_SCOPE, listener);
```





示例：监听器被触发，获得变更前状态




```java
evt.getEventContext()== src；  
    evt.getOldBinding().getName().equals("x/y")
```




JNDI规范虽好，但我们最常用就是lookup一个DataSource，之所以这样我认为有几个原因：a) JavaEE虽然号称是面向分布式应用，但实际情况绝大多数不是分布式应用，对服务注册、发现的需求很低；b)每个应用服务器的实现差距很大，尤其是命名方式和服务绑定（bind）上，以至于后来bind的接口主要用于应用服务器内部实现了，一个难以做服务注册的服务发现自然难有太大的用处；c）从命名服务中查找得到的是对象，对象一般都需要实例化的，如果是远程对象又涉及到方法的调用问题，加大了复杂度。d）非Java的环境无法使用。这几个问题其实也是其他规范难以推广的原因，所以在微服务架构下，Zookeeper、etcd解决的就是上述问题，重读JNDI对规范化服务发现、注册、配置集中管理的接口有很大的帮助。



**3、Servlet：Java API网关**

Servlet是用Java编写服务端程序的接口，在J2EE出现之前，服务端程序一般都是用CGI实现的， Servlet的出现让Java的服务端程序有了统一的模式。早期我们会把每一个响应请求的类都实现Servlet的接口，后来在很多框架中都把Servlet做成统一的入口，由框架进行分发，编程的时候就看不到Servlet了。Servlet一直在被广泛使用，在微服务架构中也可以被做为前端接入存在。



**4、JSP：成功的服务端模板技术**

JSP是一种把Java语言嵌入到静态页面，动态生成HTML或其他格式Web网页的技术标准，他解决了Servlet 生成Web网页比较麻烦的问题。JSP促进了很多框架的产生，不过在Axaj模式出现后，JSP的使用方式也发生了很大变化，前端更加趋向于客户端的渲染，而不是在服务端生成全部Web页面。微服务架构下前端有很多实现方式，JSP只是选择之一。我们曾经在全国产平台上做过[测试](http://lib.csdn.net/base/softwaretest)（龙芯、麒麟等组合），由于国产芯片的计算能力不足，造成浏览器上的渲染速度不够，这时候前端动态渲染的效果很不好，反倒是传统JSP
 在服务端生成Web页面的模式体验更佳。



**5、JTA：分布式事务的尝试**

Java事务API（Java Transaction API） 是在Java环境中，允许完成跨越多个XA资源的分布式事务。JTA的接口比较简单，但是实现起来却比较复杂，事实上很少有人尝试使用基于XA 资源的分布式事务，JTA往往被框架（例如Spring）做为底层的本地事务接口，实现业务逻辑的事务一致性声明。在事务声明方面EJB作出了很大贡献，是他率先将Required、RequiresNew、Mandatory、NotSupported、Supports、Never这样的事务声明引入到Java的体系中，后来在
 Spring 中被广泛大家使用了，而JTA正是这一实现的支撑。在微服务架构中，本地事务还应该是这种方式，麻烦的是远程服务的事务。对分布式事务的实现方式，请参考我的同事田向阳《[微服务架构下的数据一致性保证（一）](http://mp.weixin.qq.com/s?__biz=MzI5MDEzMzg5Nw==&mid=2660392782&idx=1&sn=d28e43bf6f7cf140eed9fffcf2f29e86&scene=21#wechat_redirect)》和刘相《[分布式事务：不过是在一致性、吞吐量和复杂度之间，做一个选择](http://mp.weixin.qq.com/s?__biz=MzI5MDEzMzg5Nw==&mid=2660393519&idx=1&sn=089bed7c005338bc8379fd847f2f26a8&scene=21#wechat_redirect)》的文章，在此之上，我们也应该参考EJB或者Spring的方式，用事务声明的方式维护数据的一致性，当然这个声明会远远复杂于本地事务。



**6、JMS：通过JMS 看成功的JavaEE规范**

Java消息服务（JavaMessage Service）是一个Java平台中关于面向消息中间件（MOM）的API，用于在两个应用程序之间，或分布式系统中发送消息，进行异步通信。应该说JMS规范的使用还是很多的，是JavaEE中比较成功的一个规范，除应用服务器之外的很多消息系统也都支持这个规范（例如kafka）。既然是一个很受欢迎的规范，对这个技术本身我没什么可说的，继续保持发扬吧，我想说的是，这个规范为什么做的好，受欢迎。究其原因是当时人们对于消息编程的理解比面向对象编程更加深刻，面向批处理编程、面向消息编程是在面向对象编程之前的状态，JavaEE规范的出现正是人们基于Java这样面向对象的语言实现企业应用的过程，这也证明严格的面向对象方式实现企业分布式应用并不是好的选择，在JavaEE规范中使用比较好的JSP、Servlet、JDBC、JMS等都不是面向对象的编程模式，JSP是模板式的、Servlet是请求响应式的、JDBC是面向结果集的、JMS是面向消息的。



**7、从JavaEE部署规范看[Docker](http://lib.csdn.net/base/docker)与微服务架构的关系**

JavaEE规范中，EAR、WAR、JAR的部署模式是大家最常见的方式，按照JavaEE的设想，每一个模块都是一个独立的可部署单元，前端界面、后端服务都是可以独立部署的，而应用服务器对多个模块进行统一管理。但是，由于JavaSE天然的缺陷，应用之间难以实现隔离，而应用对第三方类库的依赖也让多应用的管理变得难以接受。而Docker的到来既实现了应用的隔离，也加快了应用的部署。所以，我把Docker用在应用的部署上，用于解决JavaEE没有解决好的问题上。在我看来，Docker不是一个轻量级的虚拟机，他不应该仅仅用在替换虚拟机上，而是把他们之间的区分使用，把资源的分配、安全等问题交给虚拟机，而Docker象EAR一样做应用的打包工具，这样，把Docker部署在虚机上就不显的奇怪了，谁会对EAR部署在虚机上感到奇怪呢？

最后，我要说的是JavaEE规范建立在三层/多层应用架构体系之上（如下图左），但在数字化时代应用程序必须支持多个客户端渠道（例如，桌面，移动，社交），并且这些前端应用程序与后端服务交付（如下图右）。未来SOA仍然是企业架构的核心，但SOA的实现将从JavaEE的三层架构向轻量级的微服务架构演进，API是服务的接口，微服务架构则代表了提高[敏捷](http://lib.csdn.net/base/agile)性和可伸缩性的范例。我们提供的微服务应用平台，其实就是实现新一代的应用服务器：将中间件微服务化，将微服务工程化。

![](https://img-blog.csdn.net/20161102145800893?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




在下图上，我把微服务架构中与JavaEE规范对应的部分画出来，供大家在实现微服务时做参考：

![](https://img-blog.csdn.net/20161102152428362?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




