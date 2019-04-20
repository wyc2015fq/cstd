# Java 11正式发布，新特性解读 - 纯洁的微笑 - CSDN博客
2018年09月27日 08:08:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：392
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/YriaiaJPb26VNOLeQJ3VQ1xKfIKsuVtaX3fiaWB0hQAYicdZ1TWHspHdSMXVwZqETCasbKzbsdD17ObPiawJ480HEwQ/640?wx_fmt=jpeg)
		作者 | 杨晓峰	
		编辑 | 何潇、小智	
北京时间 9 月 26 日，Oracle 官方宣布 Java 11 正式发布。这是 Java 大版本周期变化后的第一个长期支持版本，非常值得关注。你可以点击以下地址即刻下载：
https://www.oracle.com/technetwork/java/javase/downloads/index.html
最新发布的 Java11 将带来 ZGC、Http Client 等重要特性，一共包含 17 个 JEP（JDK Enhancement Proposals，JDK 增强提案）。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/C7RIabesibqYaj3KP6LCjJ2sWGkK22R3SPYmSgkadianARLj3vllfwx6lUqyibV5Vk7tWgxqReuc7SymL72c6bCMA/640?wx_fmt=jpeg)
除了这些重要特性以外，Java 11 还有哪些值得关注的点呢？Java 现在更新那么频繁，是否要考虑升级呢？工程师们还应该继续学，学得动吗？一起来看！
不知不觉 JDK 11 已经发布了，从 9 开始，JDK 进入了让人学不动的更新节奏，对于广大 Java 工程师来说，真是又爱又恨，Java 演进快速意味着它仍将能够保持企业核心技术平台的地位，我们对 Java 的投入和饭碗是安全的，但同时也带来了学习、选择的困惑。
所以，今天我们不准备做个流水账的介绍，一起来看看工程师甚至是 IT 决策者最关心的问题：
- 
JDK 更新如此频繁，我是否要考虑升级? 新的发布模式下，企业的 IT 策略需要做出什么样的调整？
- 
除了耍酷，JDK 11，或者说最近的 JDK 版本，有什么真正值得生产环境中应用的特性？工程师要跟进吗？
对于第一个问题，本人十分确信 JDK 11 将是一个 **企业不可忽视** 的版本。
首先，从时间节点来看，JDK 11 的发布正好处在 JDK 8 免费更新到期的前夕，同时 JDK 9、10 也陆续成为“**历史版本**”，请看下面的 Oracle JDK 支持路线图：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YriaiaJPb26VNOLeQJ3VQ1xKfIKsuVtaX39Epic3KFjVKy6WeFlSfp2NOO3iacOZsHHANTds1u7MSficodiciaetnV58w/640?wx_fmt=png)
**JDK 更新很重要吗？非常重要**，在过去的很多年中，Oracle 和 OpenJDK 社区提供了接近免费的午餐，导致人们忽略了其背后的海量工作和价值，这其中包括但不仅仅限于：
- 
最新的安全更新，如，安全协议等基础设施的升级和维护，安全漏洞的及时修补，这是 Java 成为企业核心设施的基础之一。安全专家清楚，即使开发后台服务，而不是前端可直接接触，编程语言的安全性仍然是重中之重。
- 
大量的新特性、Bug 修复，例如，容器环境支持，GC 等基础领域的增强。很多生产开发中的 Hack，其实升级 JDK 就能解决了。
- 
不断改进的 JVM，提供接近零成本的性能优化
- 
…
“Easy is cheap”? Java 的进步虽然“容易”获得，但莫忽略其价值，这得益于厂商和 OpenJDK 社区背后的默默付出。
**第二，JDK 11 是一个长期支持版本（LTS, Long-Term-Support）**
对于企业来说，选择 11 将意味着长期的、可靠的、可预测的技术路线图。有人说，那是对于付费订阅客户，不订阅是不是就不用考虑 11 了呢？
不，请放心，11 确定将得到 OpenJDK 社区的长期支持，目前 Oracle 提供了 OpenJDK build，虽然后续计划未定，但是承诺“至少维护到明年”。即使是停止发布后续 JDK11 更新，Andrew Haley 等社区专家也已经明确保证，会组建并领导“JDK-11-updates”项目，并且保证：
> 
“please let me assure you of one thing: whether by Oracle orRed Hat or someone else, JDK LTS releases will continue to be supported. We all have a lot invested in Java, and we won't let itfall.”
所以，LTS 版本将是企业 IT 决策者可以放心选择的版本。
回到第二个问题，我们一起来看看 JDK 11 的有哪些能力上的突破，能够让我们觉得升级到 JDK 11 是超值的。
从 JVM GC 的角度，JDK 11 引入了两种新的 GC，其中包括也许是划时代意义的 ZGC，虽然其目前还是实验特性，但是从能力上来看，这是 OpenJDK 的一个巨大突破，为特定生产环境的苛刻需求提供了一个可能的选择。例如，对部分企业核心存储等产品，如果能够保证不超过 10ms 的 GC 暂停，可靠性会上一个大的台阶，这是过去我们进行 GC 调优几乎做不到的，是能与不能的问题。相信你对下面的数据已经不再陌生。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/YriaiaJPb26VNOLeQJ3VQ1xKfIKsuVtaX3MEibzQXyLyBXpjrxuDGpmUmA44ial8g7zgkZrvmVRSkALMiaO6EgtPaTQ/640?wx_fmt=png)
关于 ZGC 特性的解读已经非常多，本文不再重复。我在这里主要介绍那些不起眼，但更具生产系统价值的部分。例如，对于 G1 GC，相比于 JDK 8，升级到 JDK 11 即可免费享受到：并行的 Full GC，快速的 CardTable 扫描，自适应的堆占用比例调整（IHOP），在并发标记阶段的类型卸载等等。这些都是针对 G1 的不断增强，其中串行 Full GC 等甚至是曾经被广泛诟病的短板，你会发现 GC 配置和调优在 JDK11 中越来越方便。
**云计算时代的监控、诊断和 Profiling 能力**，这是个人认为比 ZGC 更具生产实践意义的特性。
不知道你有没有注意到，不知不觉中 Java 的应用场景发生了天翻地覆的变化，从单机长时间运行的 Java 应用，发展成为分布式、大的单体应用或小的 function、瞬时或长时间运行等，应用场景非常复杂。
我们还用什么工具诊断 Java 应用？ JConsole，JProfiler，还是“自研”的各种工具？目前的工具大多是从对单个 Java 应用的诊断视角出发，试想如果我们的集群中有几百、数千台机器或容器，每台机器有几个或者几十个 Java 进程，那么：
- 
怎么在不干扰生产系统的情况下，高效地跟踪海量的 Java 进程，准确定位可以优化的性能空间？
- 
如何保证“随机出现”的 JVM 问题，不需要进行额外的、令人头痛的“重现”？
JDK 11 悄悄地为我们提供了更加强大的基础能力，主要是两部分：
**JEP 328: Flight Recorder（JFR）** 是 Oracle 刚刚开源的强大特性。我们知道在生产系统进行不同角度的 Profiling，有各种工具、框架，但是能力范围、可靠性、开销等，大都差强人意，要么能力不全面，要么开销太大，甚至不可靠可能导致 Java 应用进程宕机。
而 JFR 是一套集成进入 JDK、JVM 内部的事件机制框架，通过良好架构和设计的框架，硬件层面的极致优化，生产环境的广泛验证，它可以做到极致的可靠和低开销。在 SPECjbb2015 等基准测试中，JFR 的性能开销最大不超过 1%，所以，工程师可以基本没有心理负担地在大规模分布式的生产系统使用，这意味着，我们既可以随时主动开启 JFR 进行特定诊断，也可以让系统长期运行 JFR，用以在复杂环境中进行“After-the-fact”分析。还需要苦恼重现随机问题吗？JFR 让问题简化了很多哦。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/YriaiaJPb26VNOLeQJ3VQ1xKfIKsuVtaX38ezzhd1Y9rQJPGwGGT3VPSneDUXkzpZib7Ix5jJqTniaNiaia2BAheLYVQ/640?wx_fmt=jpeg)
在保证低开销的基础上，JFR 提供的能力也令人眼前一亮，例如：
- 
我们无需 BCI 就可以进行 Object Allocation Profiling，终于不用担心 BTrace 之类把进程搞挂了。
- 
对锁竞争、阻塞、延迟，JVM GC、SafePoint 等领域，进行非常细粒度分析。
- 
甚至深入 JIT Compiler 内部，全面把握热点方法、内联、逆优化等等。
- 
JFR 提供了标准的 Java、C++ 等扩展 API，可以与各种层面的应用进行定制、集成，为复杂的企业应用栈或者复杂的分布式应用，提供 All-in-One 解决方案。
而这一切都是内建在 JDK 和 JVM 内部的，并不需要额外的依赖，开箱即用。
另一方面，就是同样不起眼的 **JEP 331: Low-Overhead Heap Profiling**。我们知道，高效地了解在 Java 堆上都进行了哪些对象分配，是诊断内存问题的基本出发点之一。 JEP 331 来源于 Google 等业界前沿厂商的一线实践，通过获取对象分配的 Call-site，为 JDK 补足了对象分配诊断方面的一些短板，工程师可以通过 JVMTI 使用这个能力增强自身的工具。
从 Java 类库发展的角度来看，JDK 11 最大的进步也是两个方面：
首先，是难得的现代 HTTP/2 Client API，Java 工程师终于可以摆脱老旧的 HttpURLConnection 了。新的 HTTP API 提供了对 HTTP/2 等业界前沿标准的支持，精简而又友好的 API 接口，与主流开源 API（如，Apache HttpClient， Jetty， OkHttp 等）对等甚至更高的性能。与此同时它是 JDK 在 Reactive-Stream 方面的第一个生产实践，广泛使用了 Java Flow API 等，终于让 Java 标准 HTTP 类库在扩展能力等方面，满足了现代互联网的需求。
第二，就是安全类库、标准等方面的大范围升级，其中特别是 JEP 332: Transport Layer Security (TLS) 1.3，除了在安全领域的重要价值，它还是中国安全专家范学雷所领导的 JDK 项目，完全不同于以往的修修补补，是个非常大规模的工程。
除此之外，JDK 还在逐渐进行瘦身工作，或者偿还 JVM、Java 规范等历史欠账，例如
- 
181: Nest-Based Access Control
http://openjdk.java.net/jeps/181
- 
309: Dynamic Class-File Constants
http://openjdk.java.net/jeps/309
- 
320: Remove the Java EE and CORBA Modules
http://openjdk.java.net/jeps/320
- 
330: Launch Single-File Source-Code Programs
http://openjdk.java.net/jeps/330
- 
335: Deprecate the Nashorn JavaScript Engine
http://openjdk.java.net/jeps/335
- 
336: Deprecate the Pack200 Tools and API
http://openjdk.java.net/jeps/336
其中最值得关注的是 JEP 335，它进一步明确了 Graal 很有可能将成为 JVM 向前演进的核心选择，Java-on-Java 正在一步步的成为现实。
JDK 11 还有什么遗憾吗？很多 Valhalla、Loom、Panama 等项目中继续补齐的短板，如协程、Value Type 等，目前还是可望而不可即，也许令人欣慰地是，我们能看到 Java 能够正视自身存在的不足，不断飞速发展。
从 1995 年第一个版本发布到现在，Java 语言已经在跌宕起伏中走过了 23 年。这 23 年，既有 Java 连续霸榜多年的风头无两，也有近两年 Java 不可忽视的颓势。Java 是最好的语言吗？不是，每个领域都有更合适的编程语言，没有无所不能的存在。
Java 语言到底有什么优势可以占据排行榜第一的位置呢？
其一，语法比较简单，学过计算机编程的开发者都能快速上手，JVM 也为开发者屏蔽了大量复杂的细节。其二，能力过硬，在若干个领域的竞争力都非常强，比如服务端编程，高性能网络程序，企业软件事务处理，大数据，分布式计算，移动、嵌入终端应用开发等等。最重要的一点是其吸收了了业界领先的工程实践，构建从嵌入式设备到超大规模软件系统的能力，充分得到了实践验证。所有这些都使得 Java 成为企业软件公司的首选，也得到很多互联网公司的青睐。时移世易，Java 正在也必须改变。
Java 改为每半年发版一次以后，在对合并关键特性、快速得到开发者反馈等方面，做得越来越好，我们明显能够看到各厂商和社区对 Java 投入的提高。最新发布的 JDK 11 虽然谈不上划时代的进步，但一定是 JDK 发展历程中的一个重要版本，升级 JDK 就可以获得的性能等各种提高，基础能力的全面进步和突破，这一切无不说明，是时候开始评估并开始计划升级到 JDK 11 了。
END
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnrnzUNX2KRVxAAzQcdevxOMnrGvPJ9mMeUR4XiatKR6F3auSmDICibxseIKoNLfo3fhiam8icfbSl4FTw/640?wx_fmt=jpeg)
