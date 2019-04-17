# Spring-cloud微服务学习入门教程 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年09月04日 18:29:54[boonya](https://me.csdn.net/boonya)阅读数：4174








学习教程地址：[https://github.com/eacdy/spring-cloud-book](https://github.com/eacdy/spring-cloud-book)[](https://github.com/APIGatewayOfSunflowers/spring-cloud-book)

学习教程源码：[https://github.com/eacdy/spring-cloud-study](https://github.com/eacdy/spring-cloud-study)


## Spring-cloud微服务学习内容



Spring-cloud微服务学习入门教程内容包括以下几点：
- 服务发现（Eureka）
- 服务提供者（Provider）
- 服务消费者(Consumer)
- 客户端负载均衡Ribbon
- 简化的Http客户端Feign
- 熔断器（Hystrix、Hystrix监控界面Hystrix Dashboard、Hystrix集群监控工具Turbine）
- 配置中心
- API Gateway

## Spring-cloud微服务项目运行测试

将项目加入IDE目录：

![](https://img-blog.csdn.net/20170904175820232)


项目主机端口分配：


|项目名称|端口|描述|URL|
|----|----|----|----|
|microservice-api-gateway|8050|API Gateway|详见文章|
|microservice-config-client|8041|配置服务的客户端|详见文章|
|microservice-config-server|8040|配置服务|详见文章|
|microservice-consumer-movie-feign|8020|Feign Demo|/feign/1|
|microservice-consumer-movie-feign-with-hystrix|8021|Feign Hystrix Demo|/feign/1|
|microservice-consumer-movie-feign-with-hystrix-stream|8022|Hystrix Dashboard Demo|/feign/1|
|microservice-consumer-movie-ribbon|8010|Ribbon Demo|/ribbon/1|
|microservice-consumer-movie-ribbon-with-hystrix|8011|Ribbon Hystrix Demo|/ribbon/1|
|microservice-discovery-eureka|8761|注册中心|/|
|microservice-hystrix-dashboard|8030|hystrix监控|/hystrix.stream|
|microservice-hystrix-turbine|8031|turbine|/turbine.stream|
|microservice-provider-user|8000|服务提供者|/1|

运行示例步骤：


步骤（0）：设置主机IP名称映射（必要，示例demo需要通过主机名访问）：



```
# Copyright (c) 1993-2009 Microsoft Corp.
#
# This is a sample HOSTS file used by Microsoft TCP/IP for Windows.
#
# This file contains the mappings of IP addresses to host names. Each
# entry should be kept on an individual line. The IP address should
# be placed in the first column followed by the corresponding host name.
# The IP address and the host name should be separated by at least one
# space.
#
# Additionally, comments (such as these) may be inserted on individual
# lines or following the machine name denoted by a '#' symbol.
#
# For example:
#
#      102.54.94.97     rhino.acme.com          # source server
#       38.25.63.10     x.acme.com              # x client host

# localhost name resolution is handled within DNS itself.
#	127.0.0.1       localhost
#	::1             localhost


0.0.0.1	mssplus.mcafee.com


127.0.0.1       localhost discovery peer1 peer2 ribbon  config-server gateway
```

步骤（1）：启动microservice-discovery-eureka 主函数。


步骤（2）：启动microservice-provider-user 主函数。

步骤（3）：启动你要测试的其它任意demo。




## Spring-cloud核心知识点拨

以下只记录本人比较关注的点和内容，ribbon、hystrix、feign、turbine和配置服务请到原文去了解。


### spring-cloud



Spring Cloud是在Spring Boot的基础上构建的，用于简化分布式系统构建的工具集，为开发人员提供快速建立分布式系统中的一些常见的模式。

> 
例如：配置管理（configuration management），服务发现（service discovery），断路器（circuit breakers），智能路由（ intelligent routing），微代理（micro-proxy），控制总线（control bus），一次性令牌（ one-time tokens），全局锁（global locks），领导选举（leadership election），分布式会话（distributed sessions），集群状态（cluster state）。


Spring Cloud 包含了多个子项目：

> 
例如：Spring Cloud Config、Spring Cloud Netflix等 Spring Cloud 项目主页：[http://projects.spring.io/spring-cloud/](http://projects.spring.io/spring-cloud/)


### 微服务发现



在微服务架构中，服务发现（Service Discovery）是关键原则之一。手动配置每个客户端或某种形式的约定是很难做的，并且很脆弱。Spring Cloud提供了多种服务发现的实现方式，例如：Eureka、Consul、Zookeeper。

Spring Cloud支持得最好的是Eureka，其次是Consul，最次是Zookeeper。


### 服务提供者和消费者


|名词|概念|
|----|----|
|服务提供者|服务的被调用方（即：为其他服务提供服务的服务，如 microservice-provider-user）|
|服务消费者|服务的调用方（即：依赖其他服务的服务，如：microservice-consumer-movie-*）|


### API Gateway

此处文字来自：[http://dockone.io/article/482](http://dockone.io/article/482)


通常来说，一个更好的解决办法是采用API Gateway的方式，可以理解为它是微服务接口的上层代理(Proxy)。API Gateway是一个服务器，也可以说是进入系统的唯一节点。这跟面向对象设计模式中的Facade模式很像。API Gateway封装内部系统的架构，并且提供API给各个客户端。它还可能有其他功能，如授权、监控、负载均衡、缓存、请求分片和管理、静态响应处理等。

下图展示了一个适应当前架构的API Gateway。
![](https://img-blog.csdn.net/20170904181023780)

API Gateway负责请求转发、合成和协议转换。所有来自客户端的请求都要先经过API Gateway，然后路由这些请求到对应的微服务。API Gateway将经常通过调用多个微服务来处理一个请求以及聚合多个服务的结果。它可以在web协议与内部使用的非Web友好型协议间进行转换，如HTTP协议、WebSocket协议。

API Gateway可以提供给客户端一个定制化的API。它暴露一个粗粒度API给移动客户端。以产品最终页这个使用场景为例。API Gateway提供一个服务提供点（/productdetails?productid=xxx）使得移动客户端可以在一个请求中检索到产品最终页的全部数据。API Gateway通过调用多个服务来处理这一个请求并返回结果，涉及产品信息、推荐、评论等。

一个很好的API Gateway例子是Netfix API Gateway。Netflix流服务提供数百个不同的微服务，包括电视、机顶盒、智能手机、游戏系统、平板电脑等。起初，Netflix视图提供一个适用全场景的API。但是，他们发现这种形式不好用，因为涉及到各式各样的设备以及它们独特的需求。现在，他们采用一个API Gateway来提供容错性高的API，针对不同类型设备有相应代码。事实上，一个适配器处理一个请求平均要调用6到8个后端服务。Netflix API Gateway每天处理数十亿的请求。




**API Gateway的优点和缺点**


如你所料，采用API Gateway也是优缺点并存的。API Gateway的一个最大好处是封装应用内部结构。相比起来调用指定的服务，客户端直接跟gatway交互更简单点。API Gateway提供给每一个客户端一个特定API，这样减少了客户端与服务器端的通信次数，也简化了客户端代码。

API Gateway也有一些缺点。它是一个高可用的组件，必须要开发、部署和管理。还有一个问题，它可能成为开发的一个瓶颈。开发者必须更新API Gateway来提供新服务提供点来支持新暴露的微服务。更新API Gateway时必须越轻量级越好。否则，开发者将因为更新Gateway而排队列。但是，除了这些缺点，对于大部分的应用，采用API Gateway的方式都是有效的。




**实现一个API Gateway**


既然我们已经知道了采用API Gateway的动机和优缺点，下面来看在设计它时需要考虑哪些事情。
**(1)性能和可扩展性**
只有少数公司需要处理像Netflix那样的规模，每天需要处理数十亿的请求。但是，对于大多数应用，API Gateway的性能和可扩展性也是非常重要的。因此，创建一个支持同步、非阻塞I/O的API Gateway是有意义的。已经有不同的技术可以用来实现一个可扩展的API Gateway。在JVM上，采用基于NIO技术的框架，如Netty，Vertx，Spring Reactor或者JBoss Undertow。Node.js是一个非JVM的流行平台，它是一个在Chrome的JavaScript引擎基础上建立的平台。一个可选的方案是NGINX Plus。NGINX Plus提供一个成熟的、可扩展的、高性能web服务器和反向代理，它们均容易部署、配置和二次开发。NGINX Plus可以管理授权、权限控制、负载均衡、缓存并提供应用健康检查和监控。
**(2)采用反应性编程模型**
对于有些请求，API Gateway可以通过直接路由请求到对应的后端服务上的方式来处理。对于另外一些请求，它需要调用多个后端服务并合并结果来处理。对于一些请求，例如产品最终页面请求，发给后端服务的请求是相互独立的。为了最小化响应时间，API Gateway应该并发的处理相互独立的请求。但是，有时候请求之间是有依赖的。API Gateway可能需要先通过授权服务来验证请求，然后在路由到后端服务。类似的，为了获得客户的产品愿望清单，需要先获取该用户的资料，然后返回清单上产品的信息。这样的一个API 组件是Netflix Video Grid。

利用传统的同步回调方法来实现API合并的代码会使得你进入回调函数的噩梦中。这种代码将非常难度且难以维护。一个优雅的解决方案是采用反应性编程模式来实现。类似的反应抽象实现有Scala的Future，Java8的CompletableFuture和JavaScript的Promise。基于微软.Net平台的有Reactive Extensions(Rx)。Netflix为JVM环境创建了RxJava来使用他们的API Gateway。同样地，JavaScript平台有RxJS，可以在浏览器和Node.js平台上运行。采用反应编程方法可以帮助快速实现一个高效的API Gateway代码。
**(3)服务调用**
一个基于微服务的应用是一个分布式系统，并且必须采用线程间通信的机制。有两种线程间通信的方法。一种是采用异步机制，基于消息的方法。这类的实现方法有JMS和AMQP。另外的，例如Zeromq属于服务间直接通信。还有一种线程间通信采用同步机制，例如Thrift和HTTP。事实上一个系统会同时采用同步和异步两种机制。由于它的实现方式有很多种，因此API Gateway就需要支持多种通信方式。
**(4)服务发现**
API Gateway需要知道每一个微服务的IP和端口。在传统应用中，你可能会硬编码这些地址，但是在现在云基础的微服务应用中，这将是个简单的问题。基础服务通常会采用静态地址，可以采用操作系统环境变量来指定。但是，探测应用服务的地址就没那么容易了。应用服务通常动态分配地址和端口。同样的，由于扩展或者升级，服务的实例也会动态的改变。因此，API Gateway需要采用系统的服务发现机制，要么采用服务端发现，要么是客户端发现。后续的一篇文章将会更详细的介绍这部分。如果采用客户端发现服务，API Gateway必须要去查询服务注册处，也就是微服务实例地址的数据库。
**(5)处理部分失败**
在实现API Gateway过程中，另外一个需要考虑的问题就是部分失败。这个问题发生在分布式系统中当一个服务调用另外一个服务超时或者不可用的情况。API Gateway不应该被阻断并处于无限期等待下游服务的状态。但是，如何处理这种失败依赖于特定的场景和具体服务。例如，如果是在产品详情页的推荐服务模块无响应，那么API Gateway应该返回剩下的其他信息给用户，因为这些信息也是有用的。推荐部分可以返回空，也可以返回固定的顶部10个给用户。但是，如果是产品信息服务无响应，那么API Gateway就应该给客户端返回一个错误。

在缓存有效的时候，API Gateway应该能够返回缓存。例如，由于产品价格变化并不频繁，API Gateway在价格服务不可用时应该返回缓存中的数值。这类数据可以由API Gateway自身来缓存，也可以由Redis或Memcached这类外部缓存实现。通过返回缓存数据或者默认数据，API Gateway来确保系统错误不影响到用户体验。

Netflix Hystrix对于实现远程服务调用代码来说是一个非常好用的库。Hystrix记录那些超过预设定的极限值的调用。它实现了circuit break模式，使得可以将客户端从无响应服务的无尽等待中停止。如果一个服务的错误率超过预设值，Hystrix将中断服务，并且在一段时间内所有请求立刻失效。Hystrix可以为请求失败定义一个fallback操作，例如读取缓存或者返回默认值。如果你在用JVM，就应该考虑使用Hystrix。如果你采用的非JVM环境，那么应该考虑采用类似功能的库。






