# Java 12 / JDK 12 正式发布 - 纯洁的微笑 - CSDN博客
2019年03月22日 09:09:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：46
自 2 月 7 日开始，Java 12 / JDK 12 就进入了 RC 阶段。按照发布周期，美国当地时间 3 月 19 日，也就是今天——Java 12 正式发布了！
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Pn4Sm0RsAuia2bxehXM2KZk80VXMdD6AJrDG1UIXppztcavh4nUOmq99PsRCxrMoAg9ibn1MMUvxVEGHpchicqosg/640?wx_fmt=jpeg)
**Java 12 ****新特性**
Java 12 带来了一系列新特性：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/dkwuWwLoRKicmwgIEPy2ibIUGx7UanmvJz15a1YfebbI5oZZZuoZic5ib3JURaCguy9nv9bs8hxNz9QcnUySs49Sdw/640?wx_fmt=png)
下边分别来看看：
**JEP 189: Shenandoah: A Low-Pause-Time Garbage Collector (Experimental)**
**Shenandoah：低暂停时间的 GC（实验性功能）**
新增了一个名为 Shenandoah 的 GC 算法，通过与正在运行的 Java 线程同时进行 evacuation 工作来减少 GC 暂停时间。使用 Shenandoah 的暂停时间与堆大小无关，这意味着无论堆是 200MB 还是 200GB，都将具有相同的暂停时间。
**JEP 230: Microbenchmark Suite**
**微基准测试套件**
JDK 源码中新增了一套微基准测试套件，开发人员可通过它轻松运行已有的微基准测试并创建新的基准测试。
**JEP 325: Switch Expressions (Preview)**
**Switch 表达式（预览功能）**
对 switch 语句进行了扩展，使其不仅可以作为语句（statement），还可以作为表达式（expression），并且两种写法都可以使用传统的 switch 语法，或者使用简化的 "case L ->" 模式匹配语法作用于不同范围并控制执行流。这些更改将简化日常编码工作，并为 switch 中的模式匹配（JEP 305）做好准备。
**JEP 334: JVM Constants API**
**JVM 常量 API**
引入 API 来对关键类文件（key class-file）和运行时工件（run-time artifacts）的名义描述（nominal descriptions）进行建模，特别是可从常量池加载的常量。
在新的 java.lang.invoke.constant 包中定义了一系列基于值的符号引用（JVMS 5.1）类型，它们能够描述每种可加载常量。
符号引用以纯 nominal 形式描述可加载常量，与类加载或可访问性上下文区分开。有些类可以作为自己的符号引用（例如 String），而对于可链接常量，定义了一系列符号引用类型（ClassDesc、MethodTypeDesc、MethodHandleDesc 和 DynamicConstantDesc），它们包含描述这些常量的 nominal 信息。
**JEP 340: One AArch64 Port, Not Two**
**只保留一个 AArch64 实现**
在保留 32 位 ARM 实现和 64 位 aarch64 实现的同时，删除与 arm64 实现相关的所有源码。
JDK 中存在两套 64 位 ARM 实现，主要存在于 src/hotspot/cpu/arm 和 open/src/hotspot/cpu/aarch64 目录。两者都实现了 aarch64，现在将只保留后者，删除由 Oracle 提供的 arm64。这将使贡献者将他们的精力集中在单个 64 位 ARM 实现上，并消除维护两套实现所需的重复工作。
**JEP 341: Default CDS Archives**
**默认类数据共享归档文件**
针对 64 位平台，使用默认类列表增强 JDK 构建过程，以生成类数据共享（class data-sharing，CDS）归档。
**JEP 344: Abortable Mixed Collections for G1**
**可中止的 G1 混合 GC**
如果混合 GC 的 G1 存在超出暂停目标的可能性，则使其可中止。
**JEP 346: Promptly Return Unused Committed Memory from G1**
**G1 及时返回未使用的已分配内存**
增强 G1 GC，以便在空闲时自动将 Java 堆内存返回给操作系统。
为了实现向操作系统返回最大内存量的目标，G1 将在应用程序不活动期间定期执行或触发并发周期以确定整体 Java 堆使用情况。这将导致它自动将 Java 堆的未使用部分返回给操作系统。而在用户控制下，可以可选地执行完整的 GC，以使返回的内存量最大化。
上述每一个新特性都有详细的说明文档，点击「阅读原文」可以跳转查看。
**JDK 13 还会远吗？**
根据 JDK 13 的迭代计划，预计其将于 2019 年 9 月发布 —— Java 现在采取“半年发布一次版本”的模式。
对于 JDK 13 的新特性，Oracle Java 平台组首席架构师 Mark Reinhold 做出了两个预测：
- 
**原生字符串文字（Raw String Literals）**，它可以跨多行源码而不对转义字符进行转义。目标是简化 Java 的开发，比如开发者可以可读形式表示字符序列，而不会掺杂一些 Java 指示符，或者提供针对 Java 以外的语法的字符串。在 JDK 13 中进行预期的测试运行后，该功能可以在随后的 JDK 14 版本中跟进。
- 
**可用于生产环境的 switch 表达式**，JDK 13 中将带来一个 beta 版本实现。switch 表达式扩展了 switch 语句，使其不仅可以作为语句（statement），还可以作为表达式（expression），并且两种写法都可以使用传统的 switch 语法，或者使用简化的“case L ->”模式匹配语法作用于不同范围并控制执行流。这些更改将简化日常编码工作，并为 switch 中的模式匹配（JEP 305）做好准备。
可访问 http://openjdk.java.net/projects/jdk/13/ 查看 JDK 13 的相关信息。
**讨论：开发者如何看待 Java 12？**
留意到不少开发者对 Raw String Literals 特性情有独钟，该特性类似于 JavaScript ES6 语法中的模板字符串，使用它基本可以告别丑陋的字符串拼接（特性详见 http://openjdk.java.net/jeps/326）。
该特性原计划于 JDK 12 发布，可惜最后还是被取消了（不过可能会在 JDK 13 发布），详见：http://mail.openjdk.java.net/pipermail/jdk-dev/2018-December/002402.html。
可能是因为业界呼声太高，最近委员会又把这个特性拿出来重新讨论了：https://mail.openjdk.java.net/pipermail/amber-spec-experts/2019-January/000931.html。
你最想要的 JDK 特性是什么？欢迎在留言区和大家分享！
-END-
**纯洁的微笑**
**一个有故事的程序员**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnqjV7GOKB2htgfZjgMjqxftxfmmdrLiaMKpyicTmLLX5fUjb6YxA6Z5Bhcozb3p0uMV7wqdKED89HZA/640?wx_fmt=jpeg)
