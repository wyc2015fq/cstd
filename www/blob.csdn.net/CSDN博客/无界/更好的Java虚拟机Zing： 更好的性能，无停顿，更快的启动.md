
# 更好的Java虚拟机Zing： 更好的性能，无停顿，更快的启动 - 无界 - CSDN博客

2019年03月19日 17:57:52[21aspnet](https://me.csdn.net/21aspnet)阅读数：300


[Zing虚拟机文档Understanding Java Garbage Collection(了解Java垃圾收集)](https://download.csdn.net/download/21aspnet/11038153)
首先说明这个Zing是收费的，但是他也是优秀的，我觉得我们可以研究下他的一些思想对于怎么提高JVM，以及目前的JVM有什么缺陷是非常有帮助的。
![](https://img-blog.csdnimg.cn/20190319213620276.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
中文版简介：[https://cn.azul.com/](https://cn.azul.com/)
![](https://img-blog.csdnimg.cn/20190319172834840.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)

## Zing的优势：
1）**更好的性能**
支持 Zing 的 Falcon JIT 编译器是使用 LLVM 技术构建的，其可提供深入的优化。
2）**无停顿执行**
Azul 解决了Java 的垃圾回收问题。停止和应用程序超时、GC 停顿、停滞和抖动周旋。
3）**更快速启动**
凭借 ReadyNow!® 技术，Java 应用程序启动速度快，且能保持高速运行。

## [Zing：虚拟机](https://www.azul.com/products/zing/virtual-machine/)
![](https://img-blog.csdnimg.cn/20190319181203774.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
Zing FAQ：
[https://www.azul.com/products/zing/zinqfaq/](https://www.azul.com/products/zing/zinqfaq/)
下面是我结合谷歌翻译后再适当修改了下的FAQ，删掉了购买等和技术无关的部分：
**什么是Zing®？**
Zing是一个兼容并符合Java SE规范的JVM。Zing针对Linux和x86进行了优化。Zing专为需要**大内存，高事务率，一致响应时间或高持续吞吐量**的任意组合的企业应用程序和工作负载而设计，是唯一能够提供**与堆大小无关**的可预测响应时间的JVM。
**Zing与传统JVM在部署Java应用程序方面有何不同？**
Zing是一个高度优化的 JVM和弹性运行时，它打破了传统的Java规模障碍，并为Java应用程序，规模和吞吐量提供了数量级的改进。Zing发行版包括一个**非侵入式**，生产时**应用程序可见性工具**，称为Zing Vision（ZVision）。
借助ZVision，IT组织首次拥有零开销，细粒度的可见性工具，可帮助发现问题并加速解决难以捉摸的生产问题。
**Zing是另一个JVM吗？**
Zing是一个更好的 JVM，它利用先进技术为主流Java应用程序提供更高的性能。我们的大多数客户不需要巨大的Java堆或超低延迟 - 他们只是希望他们的Java应用程序可靠地运行。Zing通过**Azul C4**提供这些优势   (Continuously Concurrent Compacting Collector) （**连续并发压缩收集器**）消除了限制所有传统JVM可扩展性的“stop-the-world”(“全局停顿”)。由于**单实例**可以扩展到几千兆字节到多达**8TB内存**，因此Zing可以显着减少支持企业应用程序或**多租户**SaaS / PaaS解决方案所需的实例总数，从而简化部署，显着提高响应时间一致性并降低运营支出和资本支出。Zing还附带ReadyNow！解决Java预热问题的技术，允许您的应用程序快速启动并保持快速。
**Zing如何提供比其他JVM更好的性能？**
Zing**消除了“stop-the-world”(“全局停顿”)式的垃圾收集**，其他JVM限制了这样的可扩展性，因此每个Zing实例可以扩展到8TB的堆内存。Zing还使用高度优化的**即时（JIT）编译器**，默认使用**分层编译**。
传统的Java热身问题由Zing的ReadyNow解决！允许应用程序**快速启动并保持快速运行**的技术，允许运营团队在运行之间**保存和重用累积的编译器优化**，并**避免**在工作**负载或条件发生变化时**可能**减慢处理速度**的去**优化停顿**。
**什么是Zing Vision？**
Zing Vision是一个安装在Zing里的零开销，永远在线的线上监控，诊断和调整工具。
Zing总是收集数据作为其必须完成的工作的辅助任务，这使得系统管理员可以通过Zing Vision利用这些信息，而不必担心影响正在运行的应用程序。
**Zing如何解决Java热身问题（Java warm up）？**
Zing使用Azul创新的**ReadyNow！™**技术来解决热身问题。ReadyNow专为**低延迟系统**而设计！允许Java应用程序在**启动时实现最佳性能和一致性**，并最大限度地减少**负载条件发生变化时可能发生的破坏优化**。
为什么要介绍Zing的名为**“Falcon”的新编译器**？
Falcon JIT编译器的开发是为了确保Java开发人员和基于Java的企业能够从当今的服务器硬件中**获取最大性能**。
**Falcon JIT编译器基于什么技术？**
Falcon JIT编译器**基于LLVM技术**，这是一个受欢迎的编译器基础设施项目，得到了一流大学和包括Adobe，Apple，Google，NVIDIA和Intel在内的数十家企业贡献者的积极参与。
Falcon比C2更快吗？
是。
Falcon支持哪些版本的Java？
Falcon适用于使用Java SE 7和Java SE 8构建的应用程序。作为功能预览，Falcon还可以与Java SE 10和11一起使用（即将推出）
**Zing如何支持大堆大小以及它是如何弹性的？**
Zing允许Java应用程序利用他们需要执行的任何数量的内存，**仅受系统中物理内存或虚拟机管理程序可识别的数量的限制**。
由于Zing独特的垃圾收集器，基于Azul的开创性C4 （连续并发压缩收集器）技术，GC暂停与JVM堆的大小无关，并且不限制应用程序可伸缩性。
此外，Zing还包括一项专利创新的**“内存池”（即内存储备）**，允许JVM根据实时需求动态增加内存占用量; 当需求减少时，Zing将此内存返回给系统。
此内存预留不仅可确保在高负载下保持一致的性能，而且可以充当因错误或编码错误而遇到“内存泄漏”的JVM的紧急内存。
**什么是Zing ReadyNow！™？**
ReadyNow！是Zing运行时内置的技术，可以很好地解决Java热身问题。
他有两个主要特点：
首先，它使运营团队能够跨运行保存和重用编译器优化。
第二，ReadyNow！为开发人员提供强大的API和编译器指令，以预编译常用的代码或必须快速的方法，即使它们不经常被调用。
**Zing如何提供无间断操作？**
Zing默认使用**Azul C4垃圾收集器**。无论堆大小如何，C4收集器都在不断地进行压缩，并且**永远不会使用“stop-the-world”**(“全局停顿”)的暂停来进行垃圾收集。
**什么是C4垃圾收集器？**
该C4（连续并行压缩收集器）  是一个更新的代执行Azul Pauseless GC算法，是Zing的默认垃圾收集器。
Zing能帮助解决数据库争用吗？
是。使用较少的应用程序实例，您的数据库可以看到更少的连接和更少的争用
Zing是否已经使用各种Java应用程序进行了测试？
是。对于每个版本，我们测试各种应用程序堆栈（包括Wildfly，Active MQ，Cassandra，WebLogic Server，WebSphere Application Server，JBoss和Tomcat），以及其他开源软件和第三方应用程序。
**企业主为什么要关心Zing？**
Java应用程序通常是关键业务和关键任务。影响收入和使用的性能和可伸缩性问题通常不是由应用程序，数据库或网络引起的，而是通常与JVM的选择有关。
通过选择Zing，您可以**消除意外长时间的用户等待时间**和**内存不足崩溃**，从而捕获收入和客户的损失，并在需求突然出现时提供一致的用户体验。

## [Azul C4垃圾收集器](https://www.azul.com/resources/azul-technology/azul-c4-garbage-collector/)
大多数垃圾收集器的主要缺点是需要长时间的应用程序暂停。这些暂停是不可避免的要求压缩堆以释放空间的结果。收集器使用不同的策略来延迟这些事件，但是除了使用连续并发压缩收集器的Azul C4垃圾收集器之外，所有商业可用收集器都不可避免地压缩。
C4（连续并发压缩收集器）是Azul Pauseless GC算法是Zing®的默认收集器。 C4通过支持同时生成并发来区别于其他世代垃圾收集器：使用可以同时且独立活动的并发（非停顿）机制收集不同代。与其他算法不同，它**不是“大部分”并发**，而是**完全并发**，所以它永远不会回到 stop-the-world 的压缩。
垃圾收集器摘要如下。有关术语的更多信息以及有关收集器的更多详细信息，请阅读Understanding Java GC白皮书。
![](https://img-blog.csdnimg.cn/20190319182527882.PNG)
垃圾收集（GC）是Java平台上应用程序行为的一个组成部分。Java开发人员可以通过了解GC的工作原理以及更好地选择垃圾收集器来提高应用程序性能，可伸缩性和可靠性。
更多资源：
关于Azul C4收集器的技术白皮书»
*请注意，使用IBM的J9和Oracle的JRockit，您可以选择使用单代或2代垃圾收集器
## [ReadyNow！® - 启动更快，保持快速](https://www.azul.com/products/zing/readynow-technology-for-zing/)
**解决Java热身问题**
专为基于Java的应用程序而设计，必须满足特定的服务级别
帮助开发人员管理Java的运行时去优化
减少CPU资源消耗
允许保存和重复使用累积的编译器优化配置文件
为开发人员提供对Java编译的更多控制
减少因合成测试或“假”数据的需要而导致的运行预热风险
在市场开放等关键时刻确保一致的峰值性能
允许Java快速启动并保持快速
要了解有关ReadyNow的更多信息！®特性和功能，下载 数据表
了解更多有关Azul的ReadyNow的信息！®技术在 InfoQ采访阿祖尔首席技术:[Azul ReadyNow！寻求消除JVM热身](https://www.infoq.com/news/2014/03/Azul-ReadyNow-Eliminates-Warmup)
![](https://img-blog.csdnimg.cn/2019031918373277.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
**Java“热身”问题的背后:**
基于Java的系统在运行编译和优化代码时可提供出色的性能。但是，JVM需要时间来“预热”或优化常用代码，因此应用程序可以以最快的速度运行。
为什么会这样？ Java旨在快速启动，然后根据实际使用情况提高性能。JVM的即时（JIT）编译器（如Zing的Falcon编译器）依赖于描述应用程序的哪些部分被称为最多（“热”代码）的配置文件数据。
JIT编译允许JVM优化性能，但这可能需要时间。在资本市场等用例中，通常系统会“热身”以提供最佳性能。虽然Java应用程序通常需要一段时间才能启动，但是当打开铃声响起时，它必须准备好进行全面优化。
**当前的Java预热策略:**
需要最佳Java性能和一致性的公司（如金融服务公司）已经尝试了多种方法来加热JVM，例如模拟测试数据，“虚假”交易，甚至是市场开放时的小型实时交易。
在其他问题中，这些策略可能会引入操作风险，即“虚假”数据可能会泄漏到“真实”交易日，或者实际情况可能与用于预热JVM的方案不同。有效的策略需要复制真正的端到端行为。
如果条件发生变化，Java会恢复为已解释的代码，这种情况称为“去优化”，这会使性能降低到爬行速度，直到重新编译和重新优化关键方法。Azul的ReadyNow！技术提供两个关键功能。首先是运营团队能够跨运行保存和重用累积的优化配置文件。第二个是一组强大的API和编译器指令，使开发人员能够更好地控制JVM去优化的时间和影响。
**解决方案：ReadyNow！®AzulSystems的技术:**
好了！®是用于Java的Zing运行时内置的技术。它允许基本系统在交易日开始时实现最佳性能和一致性。常见的热身技术有时会针对错误的条件进行优化，Zing的ReadyNow！
当“真实”交易与用于预热的配置文件不同时，技术可以防止大多数去优化。有了ReadyNow！运营团队可以从一天或一组市场条件中节省累积的优化，以便以后重复使用。
此外，开发人员可以更好地控制Java编译，包括API，配置指令，策略控制以及对“未实现”代码处理的改进。ReadyNow专为低延迟系统而设计！技术使Java应用程序从开幕式开始就能快速上市 - 并且它们保持快速。

## [垃圾收集（GC）调整](https://www.azul.com/resources/azul-technology/garbage-collection-gc-tuning/)
**为什么垃圾收集（GC）调整？**
大多数垃圾收集器必须停止应用程序处理 (“stop-the-world”)以确定哪些对象仍在使用中，这些对象可以被垃圾收集以及对内存堆进行碎片整理和压缩。暂停时间随内存堆大小线性增加，因此大堆可能导致用户明显延迟和应用程序性能不佳。
**GC调整 - 一些基础知识**
调整垃圾收集所花费的大部分时间都是为了延迟压缩。由于大多数物体都很年轻，因此可以在年轻一代上进行垃圾收集一段时间，但最终需要回收旧的没用的对象。可以通过跟踪旧的gen对象被删除的位置并使用释放的内存来进一步延迟完整的GC。但是，在某些时候，这个空间将变得支离破碎，需要回收。您可以尝试通过调整尽可能地延迟垃圾收集，但实际情况是您不能永远延迟它。
**消除垃圾收集暂停和GC调整的需要**
Zing虚拟机使用 Azul创新的C4收集器，可提高应用程序性能，无需进行大多数GC调整。使用Zing，应用程序可在堆和GC设置的广泛，平稳操作范围内始终如一地执行。另一个关键的好处是“你测试的是你得到的东西”，因此生产中的性能与测试的性能相匹配。
C4是一种高度并发，一致的算法，能够同时压缩Java堆，并允许应用程序在执行内存重映射时继续执行。其他JVM需要“停止世界”暂停才能完全压缩堆。请参阅下面的Zing GPGC和HotSpot JVM CMS的比较。
HotSpot JVM CMS与Zing Pauseless垃圾收集的比较：
![](https://img-blog.csdnimg.cn/20190319184503511.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
**Hotspot JVM  CMS**
**年轻代**：会停顿。
**年老代**：大部分是并发的，非压缩的
├主要是并发标记
├在mutator运行时同时标记，
├跟踪卡片标记中的突变，
├重新访问变异卡片（根据需要重复），
└停顿Stop-the-world以赶上突变，ref处理等。
├并发打扫
└不压缩（维护空闲列表，不移动对象）
**FGC**：停顿Stop-the-world
而Azul  PGC 总之是**并发，压缩，且没有停顿**的！
## [Java堆大小](https://www.azul.com/resources/azul-technology/java-heap-size/)
Java堆是分配给在JVM中运行的应用程序的内存量。堆内存中的对象可以在线程之间共享。
由于垃圾收集暂停，**传统JVM**中Java堆大小的实际限制**通常约为2-8 GB**。许多类型的应用程序都可以从非常大的堆中受益，例如内存计算，NoSQL数据库，大数据 应用程序，分析，Web个性化和电子商务。
一个**100GB的堆不会在典型的JVM上崩溃**，它会**一次定期暂停几分钟**。这限制了应用程序性能和可伸缩性，并阻止Java应用程序使用当今商用服务器的全部资源。
如果可以消除相关的性能问题，**非常大的堆大小通常非常实用**。Azul的Zing®是解决Java的垃圾回收问题，并允许堆多达8TB没有性能损失的第一个JVM。
**更大的Java内存堆：**
允许创建更多对象；
填充需要更长时间；
允许应用程序在垃圾收集（GC）事件之间运行更长时间。
**较小的Java内存堆：**
保留较少的对象；
填写更快；
是否更频繁地收集垃圾（但暂停时间较短）；
可能导致内存不足错误。
2-8GB的内存堆是否足以满足大多数Java应用程序的需求？
我们已经找到了大量证据表明对更多堆的需求被压抑了：
机器内“横向尺度”的常见用途
使用“外部”内存和不断增长的数据集（更大的数据库以及使用外部数据缓存，如memcached，JCache和JavaSpaces）
持续研究永无止境的分配问题
问题出在软件堆栈中，它会对每个实例的内存施加人为约束。
GC暂停时间是实例大小的唯一限制因素，正如我们在实践中发现的那样，即使是广泛的垃圾收集（GC）调整也不会让它消失。
一旦你解决了GC，你就解决了这个问题。Azul的创新C4垃圾收集算法是完全并发的，消除了非常大堆的性能影响。

## JVM调优
JVM调优：主要指**内存堆大小**和**垃圾收集（GC）调优**。
如果您正在寻找有关Zing调整标志的特定信息，请查看[ZVM命令行选项](https://docs.azul.com/zing/UseZVM_CommandLineOptionsZVM.htm)下的[在线Zing文档](https://docs.azul.com/zing)。
**内存堆大小**
对于大多数JVM，调整堆大小需要分析应用程序如何使用内存，更改参数和重新分析。应用程序需要尽可能靠近生产负载运行。这是特别难以做到的，因为在实验室中不会看到一些现实中出现的负载。大多数JVM使您能够设置初始堆大小（-Xms）和最大堆大小（-Xmx），但有些也允许其他参数，如大页。如果**堆大小设置得太大**，**垃圾收集暂停会变得太长**; 如果**堆大小设置得太小**，**应用程序将抛出内存不足错误**并可能崩溃。因此，内存和GC调优是影响应用程序性能的密切相关主题。
Zing®消除了大部分JVM调整的需要。Zing支持堆内存的“银行系统” ( ‘banking system’ )，允许**运行时实例根据需要可靠地增长和缩小内存堆**。Zing可以以多个GB /秒的速度在实例之间转移物理资源。不必将内存堆调整“完全正确”可以节省大量时间。您可以专注于为业务增加更多价值并启动新计划，而不是花时间调整和重新调整Java基础架构以修复生产故障。
此外，该平台还包括Zing Vision，一种零开销，始终在线监控，调整和可视性工具。使用Zing Vision，您可以快速启动应用程序，从而加快产品上市速度并随时调整生产。下面是传统JVM和Zing的实际JVM调整参数的示例。
![](https://img-blog.csdnimg.cn/20190319190950936.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[介绍Falcon JIT编译器](https://cn.azul.com/products/zing/falcon-jit-compiler/)
**Zing的Falcon JIT编译器**
在将近二十年之后，Java有了一个新的实时（JIT）编译器，它只能在Zing运行时中用于Java。 Azul Systems构建了Falcon JIT编译器，以确保Java开发人员和基于Java的企业能够从当今的服务器硬件中获取最大性能。
Falcon JIT编译器与Azul的无暂停垃圾收集技术以及ReadyNow完全集成！技术，解决了延迟敏感应用程序中Java的预热问题。
**LLVM  - Falcon JIT背后的技术**
Falcon JIT编译器**基于LLVM技术**，这是一个受欢迎的编译器基础设施项目，得到了一流大学和包括Adobe，Apple，Google，NVIDIA和Intel在内的数十家企业贡献者的积极参与。
今天，LLVM被用于Java和JVM之外的各种编程语言和框架，包括Swift和Rust等新平台以及许多其他平台，包括C / C ++，Objective-C，Swift，Clang，OpenCL，CUDA等等。
LLVM被认为是**最先进的编译器技术**，LLVM社区不断采用新的优化技术并支持新的处理器指令集和硬件平台。
**使用Zing的新Falcon JIT编译器运行时，Java工作负载的性能优势具有显着的业务优势：**
更好的应用程序服务级别指标：减少延迟，减少超时，提高一致性;
更好的客户体验：即使在不可预测的负载下也能可靠地实现客户期望，提高客户满意度;
降低运营成本：减少内存占用要求，提高云/ AWS部署中类似大小的实例的承载能力;
降低开发成本：实现所需性能，一致性和规模所需的工程时间更短.
问：Zing什么时候和Falcon一起发货？
答：Falcon JIT已与Zing一起运行了几个月。从版本17.03开始，Falcon是Zing的默认JIT编译器，取代了传统的C2编译器。
问：Falcon JIT比C2更快吗？
答：是的。
问：Zing还支持C2吗？
答：是的，可以通过在JVM启动时指定标志来使用C2。
问：我是否需要重新编译现有的应用程序以使用Falcon？
答：不用。
问：Falcon支持哪些版本的Java？
答：Falcon适用于使用Java SE 7和Java SE 8构建的应用程序。
问：Falcon会支持Java 9吗？
答：是的，在Zing的未来版本中。

