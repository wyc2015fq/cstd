# 服务器数据库系列 - Web server调研分析 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月21日 10:32:05[initphp](https://me.csdn.net/initphp)阅读数：1021








来源：[http://stblog.baidu-tech.com/?p=755](http://stblog.baidu-tech.com/?p=755)

****

**摘要 **

简单可依赖的架构首先需要有一个简单可依赖的前端WebServer集群。本文通过深入调研当前主流的异步web服务器Lighttpd和Nginx，从业界使用情况、架构原理、扩展开发、功能对比、性能对比等多个方面进行分析。

### 调研分析

#### 业界相关

从业界使用情况来看，最新Web Server使用情况的数据如下：Nginx的使用率是6.6%，Lighttpd的使用率是0.51%。

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=11307081845.jpg&type=image%2Fjpeg&width=558&height=159)

从文档来看，nginx中文相关文档越来越多。来自最新的百度搜索数据显示，nginx的网页数量是lighttpd的10倍。目前国内对于Nignx内核深入研究的人越来越多，有淘宝、sina、腾讯等许多大公司的技术人员参与研究，并进行相关的技术交流。对于Lighttpd的研究，目前主要是公司内部和一些学生。

从业界的点评来看，国内外基本上结论如下：
- 两者都是异步WebServer，都采用了状态机。本质上是相同的。
- Nginx稳定度高于Lighttpd。Lighttpd一直存在一定的内存泄漏。
- 性能上两者都非常优秀，Nginx有一定优势。
- Nginx在综合性能上更加优秀，更有可能成为未来的apache。

从社区活跃度来看，Nginx每月2到3个三位版本发布。Lighttpd3位版本更新较慢，目前1.5的版本基本上没有更新过。同时Nginx有丰富的第三方库类。

#### 架构原理

**代码层次**

Nginx的代码量10W行，Lighttpd是5W左右。相对来说，Nginx的代码层次结构更加分明，具体代码结构如下：

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=21307081895.jpg&type=image%2Fjpeg&width=202&height=197)

Nginx和lighttpd都是采用C语言编写的，对于基础数据结构都有一定的处理。Nginx中关于数据结构的代码主要放在core文件夹里面。

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=31307081938.jpg&type=image%2Fjpeg&width=558&height=72)

Nginx主要的基础代码有：array、string、buf、file、hash、md5、内存池、队列、红黑树、time、共享锁等。这些数据结构对于扩展开发都非常有帮助。Lighttpd有一定的基础数据封装，但相对没有那么明显的设计。目前观察到的基础代码有：bitset、buffer等。

内存管理是所有C程序中非常值得关注的一点。Lighttpd在内存管理上没有做特殊的考虑，基本上都是采用系统内存管理函数，比如malloc/calloc等。在扩展开发中的内存也需要扩展模块自己考虑。Nginx在内存管理上提供了两种方式：1、原生malloc等的二次封装。2、内存池。在nginx内部大量的使用内存池。在扩展开发中也能直接调用内存池进行内存管理。此外，nginx还内置了对tcmalloc的支持。把内存优化做到极致。

**架构层次**

从总体架构来看，Nginx/Lighttpd都属于master+worker的工作模型。

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=41307081979.jpg&type=image%2Fjpeg&width=384&height=300)

但nginx相比lighttpd，在细节上处理的更加优化，具体可以从几个方面来谈

1、  配置文件热加载。Nginx从设计开始就支持配置文件热加载，甚至程序的热加载。Lighttpd本身不支持，目前业界有采用扩展方式，比如说lua，可以实现部分配置热加载。

2、  强大的master进程，实现worker进程和master进程的各司其职。Lighttpd的master进程在fork完worker进程后，就单独的等待worker进程的结束（或者在worker进程结束后再创建新的worker子进程）。Nginx的master进程负责对worker子进程的管理，并通过socket pair通信方式实现热配置文件升级、优雅重启、热应用程序升级等功能。

3、  线程锁。Lighttpd在进程之外还启动了线程进行相关方面的工作，这会对lighttpd的性能带来一定影响。Nginx内部虽然提供了对线程模式的支持，但在主推的进程模式中不会出现额外的线程。

4、  多核机器优化，cpu affinity。Nginx设计中考虑了对多核机器的优化方案，能降低进程在不同cpu之间的切换次数，从而提升性能。

**超时处理**

Lighttpd的超时处理原理非常简单：通过alarm信号量来实现的。每1s出发一个alarm信号，从而切换到超时处理函数。该函数也非常简单：循环简单当前所有连接的读和写，如果事件超时了，则直接close掉。这会有几个问题：

1、  连接非常多的时候。超时处理还是会非常耗事件的。随着连接数而递增。

2、  在不可重入的函数中出发alarm的时候，有可能出现意想不到的问题。

3、  需要轮询。

Nginx在超时处理上实现的巧妙的多：采用数据结构和巧妙的策略来实现。

1、  使用红黑树来存放定时器的相关数据。红黑树的重要特点是：插入删除都会在O（logN）完成，同时具有优秀的查找性能。所以很多C++的库（map）等都用到了它。

2、  通过红黑树计算出当前节点的超时时间差，使用这个时间差作为调用多路复用I/O操作的参数，当函数返回，只可能是I/O事件被触发，或者超时。

3、  处理完I/O事件之后，得到处理前后的时间差，根据这个时间差依次查看红黑树中哪些定时器可以被处理。

这也是在高压力下，Nginx更优于Lighttpd的一个重要原因。

**Accept处理**

Lighttpd中对于Accept的处理有几个特点：1、不加锁。2、连接处理达到0.9的时候会禁止接收新的连接。3、1次性accpet 100个。这样有一个好处, 假如服务器监听fd是每次触发只接收一个新的连接, 那么效率是比较低的,不如每次被触发的时候”尽力”的去接收, 一直到接收了100个新的连接或者没有可接收的连接之后才返回。4、提供了fdwaitqueue。在fd不够用的时候备用。

Nginx：1、进程加锁，避免惊群，同时控制了获取accpet的概率，一定程度上控制各个子进程之间的请求数目。2、7/8阀值。连接数目达到最大连接数的7/8的时候，该进程将获取不到对应的accept锁。从而进入安全控制阶段。3、提供了multi_accept指令，在开启的情况下也和lighttpd一样尽可能的多accept。

**状态机**

Lighttpd和nginx都是状态机驱动模型，两者之间主要体现在细节的差异性上。
- Nginx对整个状态进行了分类，分成预处理、状态机、filter流程三个明显的阶段。
- Lighttpd的状态机相对简单固定。Nginx则相对灵活。
- Nginx的大部分处理状态都是可以扩展的并且可中断的。Lighttpd在部分状态中也可以扩展的。
- 耦合程度。nginx状态处理函数之间的耦合紧密，状态切换时的下一步处理由状态处理函数来决。而lighttpd将状态切换的动作放在状态机里，各个状态处理函数不关心下一步需要做什么，状态之间的耦合小。但同时会对扩展性带来一些问题，比如说subrequest的实现。

**后端处理**

Nginx针对不同的后端处理方式进行了封装，提供upstream来支持不同的协议（HTTP/FASTCGI/Memcache），提供扩展来支持不同的负载均衡算法。同样的Lighttpd在新版中也对不同的后端协议进行了封装，并提供了不同可供选择的负载均衡算法。

从原理层次来看，两者在后端处理上的思路是基本一致的。更多的对比需要从功能和性能上来对比。

**扩展开发**

Nginx和Lighttpd都支持扩展，Lighttpd是通过预留系统钩子来实现的，相对来说不够灵活，如果有一些特殊的修改则不得不修改源码。Nginx则通过预留系统钩子和控制反转结合，从而能够实现更多的功能。所以，nginx扩展的灵活性高于Lighttpd。

总结如下：

1、  nginx不支持动态扩展模块。

2、  扩展开发上，nginx更加灵活。提供了多种扩展切入方式。

3、  Nginx提供了丰富的类库，方便扩展开发。

#### 功能对比

**反向代理**

对比分析如下：

1、  性能。
- 同等压力下，nginx的cpu消耗要低于lighttpd。但整体差别不大。
- 极限压力下，nginx处理能力高于lighttpd。原因未知。

2、  功能。
|功能点|Lighttpd|Nginx|备注|
|----|----|----|----|
|灵活的反向代理方式|支持|支持|都非常好|
|正则|支持|支持||
|自定义header头|**部分支持**|支持|目前gm有库支持IP的传递|
|负载均衡|支持|支持||
|超时处理|**支持**|支持连接、读写等||
|故障处理|支持|支持||
|Cache|**不支持**|支持||
|文件上传|**未知**|支持，可配置，有优化|Transmit不支持|
|输出过滤|**不支持**|支持|头部过滤和内容过滤。|

结论：

1、功能上，nignx和lighttpd都具有完整的反向代理功能。但nginx在这方面明显优于lighttpd，更加完整的细节考虑和优化。主要体现在超时处理、文件上传、输入输出的过滤、cache等等。

2、性能上，Nginx稍优于lighttpd。

**Fastcgi支持**

Nginx和lighttpd在Fastcgi方面功能上基本上相同，主要调研是从性能上对比。

![](http://stblog.baidu-tech.com/wp-content/uploads/wp-display-data.php?filename=51307082030.jpg&type=image%2Fjpeg&width=351&height=180)

**10k的php请求**
|前端压力|Lighttpd|Nginx|备注|
|----|----|----|----|
|1000QPS|96% 处理1000QPS|98%||
|2000QPS|91%|96%||
|4000QPS|81%|92%||
|8000QPS|65%|85%||

**20k的PHP请求**
|前端压力|Lighttpd|Nginx|备注|
|----|----|----|----|
|1000QPS|95% 处理1000QPS|98%||
|2000QPS|90%|95%||
|4000QPS|80%|90%||
|**8000QPS**|**63%实际处理5588 QPS**|**86%。实际处理5220QPS**|****|

从性能数据来看，2000QPS以内，两者性能差别不大，但高压力下，两者性能差别非常大。甚至有可能达到20%cpu差别。

**页面Cache和运维**

Lighttpd目前暂无页面Cache的支持。Nginx从设计之初就考虑了更改Cache。甚至有单独的Cache管理进程。

从功能上来看，目前Nginx已经支持proxy cache和ssl filter，并且实现了对esi cache的支持。

从运维上来看，Nginx支持配置热加载，支持程序热加载。更适合完成24*365的全天候不间断服务。

#### 总结

对比点汇总整理后如下
|对比点|Nginx|Lighttpd|备注|
|----|----|----|----|
|市场占有率|6.6%|0.5%||
|文档|||百度文档10：1Google文档 1：1国内研究人员nginx>lighttpd|
|业界点评|||更加看好Nginx|
|代码量|10W|5W|Nginx的代码结构层次较好。|
|**基础数据结构**|array、string、buf、file、hash、md5、内存池、队列、红黑树、time、共享锁|bitset、buffer|丰富的库类对扩展开发有很大帮助|
|**内存管理**|原生malloc、内存池、支持tcmalloc|原生malloc||
|配置文件热加载|支持|不支持||
|**进程模型**|Master负责管理，worker负责处理请求，各司其职。|Master简单。Worker复杂||
|**进程额外线程**|无|有|存在线程锁|
|多核机器优化|支持|不支持||
|连接管理|静态数组+单链表|动态数组，key交互|Nginx更加稳定高效。|
|**超时处理**|红黑树+巧妙的策略|Alarm+for循环||
|Accept处理|锁+7/8阀值，支持mult  accept|0.9策略，一次性aceept100个。||
|状态机|- Nginx对整个状态进行了分类，分成预处理、状态机、filter流程三个明显的阶段。- Lighttpd的状态机相对简单固定。Nginx则相对灵活。- Nginx的每一个状态都是可以扩展的并且可中断的。Lighttpd在部分状态中也可以扩展的。- 耦合程度。nginx状态处理函数之间的耦合紧密，状态切换时的下一步处理由状态处理函数来决定ligty将状态切换的动作放在状态机里，各个状态处理函数不关心下一步需要做什么，状态之间的耦合小。| | |
|后端处理|都支持多种协议，并且方便扩展，都支持负载均衡算法扩展。| | |
|扩展开发|1、  预定义钩子2、  控制反转3、  丰富库类|预定义钩子||
|反向代理|1、功能上，nignx和lighttpd都具有完整的反向代理功能。但nginx在这方面明显优于lighttpd，更加完整的细节考虑和优化。主要体现在超时处理、文件上传、输入输出的过滤、cache等等。2、性能上，Nginx稍优于lighttpd。| | |
|fastcgi|功能上两者差别不大，主要体现在性能上。在性能上，2000QPS以内，两者性能差别不大，但高压力下，两者性能差别非常大。| | |
|页面Cache|支持proxy_cache。支持esi页面cache|不支持，需要额外开发。||
|运维相关|支持配置文件热加载支持应用程序热加载|支持有限的配置文件热加载||

通过上述对比分析，可以得出如下结论：

“lighttpd和nginx一样具有非常好的架构，但在数据结构、内存管理都多个细节方面处理nginx考虑更加完善。如果说lighttpd是异步web server的先驱，那么nginx则是对lighttpd做了整体的优化的。而这些优化是全面的，根本性质的。无法简单的通过升级lighttpd来实现。因为nginx从一开始设计就希望做成一个完美的异步web server。nginx从event、跨平台、基础数据结构都很多细节方面进行了考虑和优化。应该来说，nginx必定是未来的apache，未来的主流。”



