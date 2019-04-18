# MSMQ,Enterprise Service, DotNet Remoting,Web Service 的优缺点 - 深之JohnChen的专栏 - CSDN博客

2010年01月23日 16:59:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1330


对于送耦合的引用，有一下四种选项。

1.MSMQ

从windows nt 开始微软就开始提供msmq 的支持，一直到现在的3.0，主要提供一下几个特性的支持。
 可靠的消息传递，类似mail 系统，有脱机支持
可设置消息的优先级，Label的各种额外的标示
事务支持
通过DC,IC的灵活应用，有好的缩放性

对于客户端，要求必须是windows 系统，从windowsce 到windows .net 2003 都作支持。可以通过连接器跟其他的非微软技术集成.NET 有一个专门的封装 System.Messaing Namespace.

2.Enterprise Service

.NET 中其实通过托管的Enterprise Service 跟 COM+ 应用架构交互。

从windows 2000 开始有这个组件，目前到windows 2003 版本是1.1

我认为有几个特性值得一用
对象池，对于对象构造特别慢，而又没有状态的应用特别适合。很类似我们 ADO.NET 中的连接吃。可以跟JIT去结合。
分布式事务协调，加上事务补偿。这是一大优点
另外一点就是送耦合事件，这一点对于plug and play 的订阅式应用很有帮助。

当然直接的客户端也必须是 windows 2000 以及以上的OS

3.DotNet Remoting 
这个我用的最多，感觉也最深；）

dotnet remoing 的中文翻译时远程处理，其实是一种.NET 平台下面很好的一种远程处理调用，提供了开发的架构。灵活的通讯传输协议，当然也可以自己去扩展。远程对象的调用提供了多种方式，可以使有状态的，也可以使无状态。对于开发人员，感觉根本地调用一样方便。这一点主要却别于Web Service。

其实这种应用类似一个相对耦合的应用。客户端和服务端丰富的通讯模型是基于.NET 平台。也就是说如果客户端不一定是.NET 平台的话，remoting 就显得不是很适合。一个简单的例子就是对象的传递

从remoting 服务端传递一个对象给客户端，可以是对象的远程的一个远程引用，也可以使一个对象的copy，就是我们通常说的mbr 或者mbv

当然，web 服务是无法实现对象的引用传递，web 服务只能是一个mbv，而web 服务这里的mbv 作的就不够彻底了。我在[http://dotnet.mblogger.cn/montaque/posts/2094.aspx](http://dotnet.mblogger.cn/montaque/posts/2094.aspx) 提到他们走的不同的序列化方式。对于web 服务，只是一个很浅的copy。也就是说对象在传递到服务端的时候，并没有把 100% 的状态传递过去。

而remoting 传递的对象就比较地道。或许这就是一个.NET Remoting 相对于web 服务比较更贴近本地调用的一个体现

缺点前面提到了，就是丰富的特性是基于。NET 这个平台。.NET 中通过 System.Runtime.Remoting.Dll

4. web 服务。
这是个标准的东西，我的意见是标准的东西不见的都是最好的。在保证标准的同时，丢失了很多特性。

