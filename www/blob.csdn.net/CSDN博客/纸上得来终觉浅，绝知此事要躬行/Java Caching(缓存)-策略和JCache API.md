# Java Caching(缓存)-策略和JCache API - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年01月20日 17:26:34[boonya](https://me.csdn.net/boonya)阅读数：12901








This Article Is From：[https://dzone.com/refcardz/java-caching](https://dzone.com/refcardz/java-caching)





Written by![](https://secure.gravatar.com/avatar/eb0d2c5bf9426d7718efc6f9b087efb5?d=identicon&r=PG)[Abhishek
 Gupta](https://dzone.com/users/456870/abhirockzz.html)Senior Product Manager, Oracle——Oracle高级产品经理。





建议先看英文再看翻译：翻译使用的是Google翻译。






**第一部分**

# 介绍

## 什么是缓存？

一般来说，缓存是一种技术，其中应用程序中的对象存储在称为缓存的临时存储区域中。缓存本身可以被认为是内存中的数据结构。缓存对象可以是从昂贵和耗时的操作的结果到静态网页或后端数据库的内容的任何东西。缓存使应用程序免于获取相同信息（从任何数据源）或通过在需要时存储到中间高速缓存并从中获取时，重复执行相同的昂贵计算。

缓存提供了几个好处，这就是为什么它在需要快速访问数据的使用情况下大量利用的原因。正确的缓存可以提高性能，减少资源使用，并提高响应速度。

通常，在企业应用程序中，高速缓存充当中间的低延迟数据源，并位于应用程序和后端系统之间。

注意：这是一个相当简单的表示，让你开始。我们将在后面详细介绍更多涉及的结构拓扑。

![](https://img-blog.csdn.net/20170120162333420)


**第二部分**

# Java缓存景观（Landscape）

Java生态系统已经为缓存解决方案提供了坚实的支持，其中有几个产品和框架可供选择。一些广泛使用的解决方案是：


- Hazelcast 
- Oracle Coherence 
- Infinispan 
- Ehcache 
- Apache Ignite 
- GridGain 
- GemFire 



为了创建一组标准的API来抽象这些异构缓存解决方案，JSR 107：Java临时缓存API启动。




**第三部分**




# JCache API (JSR 107)

## 什么是JCache



JCache是用于缓存的Java API。它提供了一组通用接口和类，可用于将Java对象临时存储在内存中。它是JSR 107所代表的JCP标准。它是JCP历史上运行时间最长的JSR之一 - 从2001年开始，到2014年3月最终发布。

JCache API由不同的供应商（a.k.a.缓存提供程序）实现，但是客户端代码访问它的方式是通过它暴露的一组标准接口和类。


## 为什么使用JCache?

缓存不是Java世界中的一个新概念 - 甚至一般来说。
如前所述，有几种企业级缓存解决方案已经可用 - 因此，为什么要在您的应用程序中已经使用的特定供应商/产品上选择JCache？

**有两个主要好处：**

可移植性：JCache允许您在客户端应用程序中使用标准API，将其与实际的供应商实现分离。这反过来使您的应用程序在各种JCache实现之间可移植。

开发人员生产力：开发人员可以利用通用的JCache API和接口集，而不必考虑供应商特定的细节。这确保了学习曲线更小，因为它被限制为作为标准的JCache的知识，而不是供应商实现的细节。


## 关于供应商特定功能？

JCache允许使用与特定JCache接口/类相对应的等效供应商API。这将在后续章节中详细讨论。




**第四部分**


# JCache深潜(Deep dive)

## 基本构件

这里是一个简单的代码段，让你开始。详情将在下一节中探讨。



```java
CachingProvider cachingProvider = Caching. getCachingProvider();

CacheManager cacheManager = cachingProvider. getCacheManager();

MutableConfiguration<String, String> config = new MutableConfiguration();

Cache<String, String> cache = cacheManager. createCache("JDKCodeNames",config);

cache.put("JDK1.5","Tiger");

cache.put("JDK1.6","Mustang");

cache.put("JDK1.7","Dolphin");

String jdk7CodeName = cache.get("JDK1.7");
```



## Cache

javax.cache.Cache是一种类型安全的数据结构，允许应用程序临时存储数据（作为键值对）。它的语义类似于java.util.Map对象，但它与Map不完全相同。Cache接口通过其API暴露了几个特性，但是下面列出了一些基本特性：

![](https://img-blog.csdn.net/20170120163642597)

## Entry

如其名称所示，javax.cache.Entry表示高速缓存中的键值对。如前所述，在高速缓存中可以有多个这样的条目。条目是一个暴露最小操作的简单实体，例如分别通过getKey和getValue方法获取键和值。

## CacheManager

javax.cache.CacheManager接口帮助处理缓存对象，并执行诸如缓存创建，销毁和内省（获取有关其自身的相关详细信息）之类的操作。让我们来看一些常见的操作：

![](https://img-blog.csdn.net/20170120163653144)



## CachingProvider 
作为JCache SPI的一部分，javax.cache.spi.CachingProvider提供了允许应用程序管理CacheManager实例的方法。


![](https://img-blog.csdn.net/20170120164415132)

## Caching

此类用于通过getCachingProvider及其重载版本获取javax.cache.CacheProvider对象的句柄。注意：还有其他方法可以获得CachingProvider。除此之外，缓存类还提供了下表中指出的其他功能：

![](https://img-blog.csdn.net/20170120164027909)

**第五部分**

# API概述

将JCache视为一组模块是很好的，每个模块都提供了一个特定的功能。JCache API是这样分拆，它是很容易和直观的，以这种方式探索它。

![](https://img-blog.csdn.net/20170120164328460)

## 核心(Core)

基本构建块部分中介绍的API构成了我们可以称为JCache核心的大部分，因为这些类/接口（以及它们各自的提供者实现）是API的基础。

## 配置（Configuration）

JCache API提供了一组标准的接口和实现，通过它们可以以编程方式配置缓存。javax.cache.configuration.MutableConfiguration及其构建器类API有助于配置。可以配置以下高速缓存特性：

![](https://img-blog.csdn.net/20170120164616324)



```java
MutableConfiguration<String,String> config = new MutableConfiguration();

config.setReadThrough(true);
```




## 到期政策（Expiration Policies）

顾名思义，可以在缓存上实施到期策略，这将根据策略语义确定自动逐出或从缓存中删除条目。

注意：默认情况下，javax.cache.Cache中的条目不会过期。

JCache API提供的到期策略围绕javax.cache.expiry.ExpiryPolicy接口及其即用型实现。

![](https://img-blog.csdn.net/20170120164854562)



```java
MutableConfiguration<String,String> config = new MutableConfiguration();

config.setExpiryPolicyFactory(CreatedExpiryPolicy.factoryOf(Duration.ONE_MINUTE));
```




## 监听器和过滤器(Listeners and Filters)



缓存事件侦听器允许客户端代码针对他们可能感兴趣的缓存事件注册回调。javax.cache.event.CacheEntryListener充当提供合同（方法）的其他接口的（父）标记接口，可以按顺序实现以对高速缓存中的特定事件作出反应。这些通常是单一的抽象方法，这使得它们非常适合Java 8 lambda风格的实现。

JCache还有过滤器，这有助于确定是否调用缓存侦听器。当你想根据某些条件选择性地调用对高速缓存监听器的调用时，这很方便。

![](https://img-blog.csdn.net/20170120165057791)



```java
CacheEntryCreatedListener<Long,TicketDetails> newTicketCreationListener = (cacheEntries) -> {

for(CacheEntryEvent ticketCreatedEvent : cacheEntries){

System.out.println(“Ticket ID: “ + ticketCreatedEvent.getKey());

System.out.println(“Ticket Details: “ + ticketCreatedEvent.getValue().toString());

}

};

CacheEntryEventFilter<Long,TicketDetails> entryEventFilter = (event) -> event.getSource().getName().equals(“TicketsCache”);
```



## 外部资源集成(External Resource Integration)

JCache API支持缓存加载器和缓存写入器，这有助于将缓存与外部资源集成。读取操作是在javax.cache的帮助下完成的。CacheLoader实现（如果在缓存中找不到键，则会自动调用）从外部源检索相应键的值。同样，一个javax。cache.CacheWriter实现同步外部源以响应缓存中的更新和删除条目。

![](https://img-blog.csdn.net/20170120165326302)



```java
public class TicketCacheLoaderWriter implements CacheLoader<Long, TicketDetails>, CacheWriter<Long, TicketDetails>{

  @Override

public TicketDetails load(Long ticketID) throws CacheLoaderException {

return getTicketDetails(ticketID); }

  @Override

public Map<Long, TicketDetails> loadAll(Iterable<? extends Long> ticketIDs) throws CacheLoaderException {

Map<Long, TicketDetails> tickets = new HashMap<>();

for(Long ticketID : ticketIDs){

tickets.put(ticketID, getTicketDetails(ticketID));

}

return Collections.unmodifiableMap(tickets); }

  private TicketDetails getTicketDetails(Long ticketID){

TicketDetails details = null;

//business logic to fetch ticket information

return details; }

  @Override

public void write(Cache.Entry<? extends Long, ? extends TicketDetails> ticketEntry) throws CacheWriterException{

writeTicketDetails(ticketEntry.getKey(), ticketEntry.getValue());}

  @Override

public void writeAll(Collection<Cache.Entry<? extends Long, ? extends TicketDetails>> ticketEntries) throws CacheWriterException {

for(Cache.Entry ticketEntry : ticketEntries){

          writeTicketDetails((Long) ticketEntry.getKey(), (TicketDetails) ticketEntry.getValue());}

}

@Override

public void delete(Object ticketID) throws CacheWriterException {

deleteTicket((Long) ticketID); }

  @Override

public void deleteAll(Collection<?> ticketIds) throws CacheWriterException {

for(Object ticketID : ticketIds){

deleteTicket((Long) ticketID); }

}

private void writeTicketDetails(Long ticketID, TicketDetails ticketDetails){

//business logic to delete ticket information

}

  private void deleteTicket(Long ticketID){

}

}
```




## 缓存条目处理(Cache Entry Processing)

当缓存在多个节点（JVM）上分布式（通常是这种情况）时，入口处理器特别有用。为了更新缓存中的现有条目，可以遵循默认的一组步骤：


- 从缓存获取值
- 突变/更新它
- 将更新的值放回缓存中


虽然这是完全正常的，但在性能方面（特别是当缓存值很大时）效率不高。缓存提供程序必须将缓存值从众多节点中的一个解串行到客户端，然后将更新的（和序列化的）版本发送回缓存。如果连续进行多次这样的调用，则问题放大。条目处理器允许客户端通过将其发送到缓存节点而对缓存条目应用变换，而不是从缓存获取条目，然后在本地进行变换。这显着减少了网络流量以及序列化/反序列化费用。所有你需要做的是定义/实现由javax.cache.EntryProcessor接口表示的入口处理器，并在Cache.invoke或Cache.invokeAll方法中指定相同的入口处理器。

![](https://img-blog.csdn.net/20170120165511465)


## 注解(Annotations)

JCache注释（在javax.cache.annotation包中）帮助您将缓存操作作为方面（从面向方面的范例角度）。这些注释允许客户端代码以声明方式指定高速缓存需求。注意，这些注释的功效取决于外部框架（如CDI，Guice等），它可以处理这些注册并执行所需的操作。

默认情况下，JCache通过注释公开有限数量的缓存操作，但它们仍然有用。

![](https://img-blog.csdn.net/20170120165817279)


注意：上述注释适用于一个类（本质上是对该类中的所有方法强制执行）或一个或多个方法。

还有三个辅助注释值得一提：

![](https://img-blog.csdn.net/20170120165847795)



```java
@CacheDefaults(cacheName=”TicketsCache”)

public class TicketingService{

@CachePut

public void persistTicket(long ticketID, @CacheValue TicketDetails details){

//domain logic to persist ticket information

}

@CacheResult

public TicketDetails findTicket(@CacheKey long ticketID){

TicketDetails details = null;

//execute domain logic to find ticket information

return details;

}

@CacheRemove

public void deleteTicket(@CacheKey long ticketID){

//domain logic to delete ticket information

}

@CacheRemoveAll

public void deleteAllTickets(){

    //domain logic to delete ticket information

} 

}
```




## 管理(Management)

JCache提供了MBean接口，其实现公开了缓存配置和运行时性能监视相关的统计信息。可以通过任何JMX客户端或通过javax.management.MBeanServer API（程序化）跟踪这些统计信息。

![](https://img-blog.csdn.net/20170120170049437)




```java
MutableConfiguration<String,String> config = new MutableConfiguration();

config.setManagementEnabled(true);

config.setStatisticsEnabled(true);
```




## SPI/扩展(Extensions)

javax.cache.spi包由单个接口组成：CachingProvider。我们已经看过这个类的具体细节，但让我们从JCache供应商的角度来理解它。

JCache提供程序实现了此接口，为了使其可自动发现，在META-INF / services / javax.cache.spi.CachingProvider中声明了具体类的完全限定类名，它必须在类路径中可用。一般来说，这是包装在供应商实现JAR本身。因此，您可以将此接口视为JCache提供程序的网关。




```java
CacheManager cacheManager = Caching. getCachingProvider(“com.hazelcast.cache.impl.HazelcastCachingProvider”).getCacheManager();

CacheManager cacheManager = Caching. getCachingProvider(“com.tangosol.coherence.jcache.CoherenceBasedCachingProvider”).getCacheManager();
```




## 两个世界最佳：与JCache一起使用供应商特定的功能

现在我相信你理解JCache（JSR 107）只是一个标准的API，由不同的供应商实现。标准的JCache API为您提供了一个钩子，以利用具体的供应商特定的实现本身 - 你可以使用解包方法。让我们来看下表中的细节。

注意：如果真正的CachingProvider可移植性是您正在寻找的，这显然不推荐，因为您的应用程序将直接耦合到供应商特定的API。

![](https://img-blog.csdn.net/20170120170437923)



```java
//Hazelcast specific example

ICache<String,String> hazelcastICache = cache. unwrap(ICache.class);
```

JCache的Hazelcast ICache扩展提供了许多增值功能。不可能详细讨论所有这些，但以下是一些重要的：

异步操作：ICache扩展暴露了大多数JCache操作的异步等效，如get，put，putIfAbsent等。

近缓存：此功能允许Hazelcast客户端（通过显式配置）在本地存储数据，而不是伸出到远程Hazelcast集群，从而减少延迟和网络流量。





**第六部分**

# 缓存策略

缓存策略是可以采用的方法，同时将缓存解决方案实现为应用程序的一部分。缓存层需求背后的驱动程序/用例因应用程序的需求而异。

让我们来看看一些通常采用的缓存策略以及JCache如何适应这种情况。下表提供了快速快照，后面是一些详细信息：

## 缓存拓扑（Cache Topology）

哪个缓存拓扑/设置最适合您的应用程序？您的应用程序是否需要单节点高速缓存或具有多个节点的协作高速缓存？

**策略/选项**

从规范的角度来看，JCache不包括关于缓存拓扑的任何细节或假设。

**独立(Standalone)：**顾名思义，此设置由包含所有缓存数据的单个节点组成。它相当于单节点集群，不与其他正在运行的实例协作。

**分布式(Distributed)：**数据分布在高速缓存中的多个节点上，因此只有一个节点负责读取特定条目。这是可能的通过分布/分区的集群
（即，所有节点具有相同数量的条目并且因此被负载平衡）。故障切换通过每个节点上的可配置备份进行处理。

**复制(Replicated)：**数据分布在高速缓存中的多个节点上，使得每个节点由完整的高速缓存数据组成，因为每个群集节点包含所有数据;故障转移不是一个问题。


## 缓存模式(Cache Modes)

您希望高速缓存在与应用程序相同的进程中运行，还是希望高速缓存独立存在（即服务模式）并在客户端 - 服务器模式下执行？

**策略/选项**

JCache不要求任何特定的缓存模式。它通过提供以高速缓存模式无关方式设计的灵活API来包含这些原则。

以下模式通常在高速缓存中是通用的：

**嵌入模式(Embedded mode)：**当缓存和应用程序在同一个JVM中共存时，缓存可以说是以嵌入模式运行。缓存与应用程序JVM一起工作和死亡。此策略应在以下情况下使用：


- 您的应用程序和缓存之间的紧密耦合不是一个问题
- 应用主机具有足够的容量（存储器）以适应高速缓存的需求


**客户端 - 服务器模式(Client-server mode)：**在此设置中，应用程序充当独立（远程）高速缓存层的客户端。这应该在以下情况下使用：


- 缓存基础架构和应用程序需要独立演进
- 多个应用程序使用统一的缓存层，可以在不影响客户端应用程序的情况下扩展

## 从多个组合中选择

不同的缓存模式和拓扑使得可以有多个选项可供选择，具体取决于具体要求。

**独立嵌入式缓存：**与应用程序本身位于同一JVM中的单个缓存节点

![](https://img-blog.csdn.net/20170120171530193)





**分布式嵌入式缓存：**多个缓存（集群）节点，每个节点共同位于应用程序JVM中，并且仅负责特定的缓存条目

**复制嵌入式缓存：**多个缓存（集群）节点，每个节点位于应用程序JVM内;这里缓存的数据被复制到所有节点

![](https://img-blog.csdn.net/20170120171545784)


**独立客户端 - 服务器缓存：**作为与应用程序不同的进程运行的单个缓存节点


![](https://img-blog.csdn.net/20170120171735006)


**分布式客户端 - 服务器缓存：**多个缓存（集群）节点，以分布式方式协作并与客户端应用程序隔离执行

**复制的客户端 - 服务器缓存：**多个缓存（集群）节点，其中整个缓存数据副本存在于每个节点上，并且缓存本身作为与应用程序不同的进程运行


![](https://img-blog.csdn.net/20170120171747538)


## 透明缓存访问

您正在设计异构应用程序的通用缓存层。理想情况下，您希望应用程序以透明方式访问高速缓存，而不会通过高速缓存实现的细节污染其核心逻辑。

**策略/选项**

如JCache
 Deep Dive部分中所述，通过CacheLoader和CacheWriter机制已经提取了与外部系统（如数据库，文件存储库，LDAP等）的集成，这些机制分别帮助您实现直接读写和直写策略。

**读取通过(Read-Through)：**从集成后端存储获取丢失的缓存条目的过程。

**直写(Write-Through)：**将缓存条目（创建，更新，删除）的更改推送到后端数据存储中的过程。

重要的是要注意，用于特定高速缓存的直读和直写操作的业务逻辑被限制在高速缓存层本身内。因此，您的应用程序与缓存的详细信息及其备份系统记录保持隔离。
其他策略（非JCache）

**后写(Write-Behind)：**此策略利用更有效的方法，其中高速缓存更新批量（排队）并异步写入后端存储，而不是写入通过策略采用的急切和同步策略。例如，Hazelcast通过其com.hazelcast.core支持Write-Behind策略。MapStore接口，当write-delay-seconds配置属性大于0.请注意，这只是一个Hazelcast
 Map功能，不支持通过ICache扩展。

**刷新提前(Fresh-Ahead)：**这是另一个有趣的策略，其中缓存实现允许您根据特定因素从后端存储刷新缓存数据，这可以用条目的到期时间表示。重新加载过程本质上是异步的。例如，Oracle
 Coherence支持此策略，该策略由称为刷新提前因子的配置元素驱动，这是高速缓存条目的过期时间的百分比。


## 缓存数据质量

缓存到期/逐出策略从缓存数据质量角度是至关重要的。如果缓存未调整到使其内容过期，则它不会获得刷新/重新加载/与其主仓库/系统记录同步的机会，并且可能最终返回过时或过时的数据。

您需要确保缓存实施考虑到后备数据存储（缓存后）中的数据波动性，并有效地调整缓存以维护质量数据。

**策略/选项**

在JCache Deep Dive部分中，您遇到了JCache-AccessedExpiryPolicy，CreatedExpiryPolicy，EternalExpiryPolicy，ModifiedExpiryPolicy和TouchedExpiryPolicy中提供的默认到期策略。除了这些策略，JCache允许您通过实现javax.cache.expiry.ExpiryPolicy接口来实现自定义驱逐策略。

**Flexi-Eviction（非JCache）**

JCache API允许您在特定高速缓存上强制实施到期策略;因此，它适用于该缓存中的所有条目。使用Hazelcast
 JCache实现，您可以通过提供在缓存中为每个条目指定ExpiryPolicy的功能进一步微调。这由com.hazelcast.cache.ICache接口提供支持。



```java
ICache<String,String> hazelcastICache = cache. unwrap(ICache.class);

hazelcastICache.put(“key2”, “value2”, new TouchedExpiryPolicy(Duration.FIVE_MINUTES));
```








