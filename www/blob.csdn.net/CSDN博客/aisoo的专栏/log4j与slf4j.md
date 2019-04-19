# log4j与slf4j - aisoo的专栏 - CSDN博客
2012年11月20日 15:11:08[aisoo](https://me.csdn.net/aisoo)阅读数：450
log4j 和 commons-logging 在 2007 年相继停止了更新，对于得到如此广泛应用的框架来说，这是个让人不安的事实。幸运的是，log4j 的作者 Ceki Gülcü 这几年并没有闲着，而是带给了我们 slf4j 和 logback。尽管名字改变了，也不再有 Apache 的光环笼罩着，但任何一个使用过
 log4j 的开发者对 slf4j 的 API 和 logback 的配置方式都不会感到陌生，甚至，你可能会认为这就是新版本的 log4j 。 
除了解决 commons-logging 被人诟病已久的 ClassLoader 问题，slf4j 和 logback 还做出了一些细节上的改进。对那些已经受够了 commons-logging 和 log4j 的小毛病折磨的开发者来说，这些改进提供了相当充分的理由让我们来考虑“升级”到 slf4j
 和 logback 。 
Parameterized Logging
这个是 slf4j 最讨人喜欢的改进了，不必再为了避免性能损失而到处做 if ( logger.isDebugEnabled() ) 这样的检查，也不必为了插入几个变量而把一句话拆成很多段来写，在很大程度上挽救了开发者的审美情趣。 
Default Logging Functionality
当无法找到配置文件时，log4j 只会输出几行错误信息，然后就直接罢-工了，而 logback 则能够优雅地切换到内置的缺省配置，输出 Debug 或更高级别的 Log 到 Console 。对于应用程序的开发来说，log 的配置文件很可能不是从默认位置读取的，因此 logback 的处理方式使得开发者更容易诊断应用程序初始化阶段出现的问题。 
Variable Substitution
虽然 log4j 也支持这个特性，但查找范围仅限于 Java System Property，而 logback 则像很多其它框架一样，支持载入外部 .properties 文件，所以开发应用程序的时候，可以把运行环境相关的配置集中在一个 .properties 文件中，提供给各个框架解析，不需要再单独为了
 logging 进行设置。 
slf4j 提供了丰富的 bridge module 与 commons-logging，log4j 和 java.util.logging 进行协作，迁移到 slf4j 的技术风险被降到了最低。最大的决定因素来自于对已有代码进行修改的工作量，尽管 slf4j 提供了一个移植工具来帮助修改源代码，但只能进行一些基本的
 Class 替换，转变到 Parameterized Logging 仍然必须通过手工修改源代码。 
采用 logback 要考虑的情况稍微复杂一些。目前只有 slf4j 支持 logback，但对于应用程序而言，仍然有大量的第三方的 lib 使用的是 commons-logging API， 为此，slf4j 提供了山寨版的 jcl-over-slf4j.jar 在集成阶段替换 commons-logging.jar
 （也有对应 log4j 和 java.util.logging 的山寨版），从而将所有的 Logging 都集中交给 slf4j 处理。如果应用程序使用了 Maven 进行依赖管理的话，由于第三方 lib 对于 commons-logging.jar 的依赖具有传递性，对其逐个添加 exclusion 显然是很繁琐的，用釜底抽薪的方法更简单些，相应的零件可以在 JBoss Repository 中找到。最后，还得留意某些应用服务器的陷阱，例如 IBM Websphere Application Server
 6.0 内置了 commons-logging 并且会将其暴露给部署的应用程序并且默认的 ClassLoader 策略是 Parent First……
