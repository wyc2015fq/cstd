# aspectj之一 ----- 简介 - z69183787的专栏 - CSDN博客
2015年07月05日 11:55:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：743
[aop](http://www.csdn.net/tag/aop)[spring](http://www.csdn.net/tag/spring)[protocols](http://www.csdn.net/tag/protocols)[debugging](http://www.csdn.net/tag/debugging)[正则表达式](http://www.csdn.net/tag/%E6%AD%A3%E5%88%99%E8%A1%A8%E8%BE%BE%E5%BC%8F)[extension](http://www.csdn.net/tag/extension)
**一、为什么写这个系列的博客**
　　　　   Aspectj一个易用的、功能强大的aop编程语言。其官网地址是：http://www.eclipse.org/aspectj/，目前最新版本为：1.7.0 RC1。但关于他使用的中文资料少之又少，几乎没有比较完整的中文资料对其做介绍。笔者用Aspectj有段时间了，对其语法还是有些了解。故把自己的经验写出来与大家分享。学习过程主要是从官方网站学习，由于笔者英文水平有限（４级没有过），有理解不到位的地方也请大家多多指正。
**二、Aspectj是什么**
　　官方网站的的介绍是这样的：
- a seamless aspect-oriented extension to the Javatm programming language（一种基于Java平台的面向切面编程的语言）
- Java platform compatible（兼容Java平台，可以无缝扩展）
- easy to learn and use（易学易用）
** 三、Aspectj能做什么**
        clean modularization of crosscutting concerns, such as error checking and handling, synchronization, context-sensitive behavior, performance optimizations, monitoring and logging, debugging support, and multi-object protocols。
         大意是说：干净的模块化横切关注点（也就是说单纯，基本上无侵入），如错误检查和处理，同步，上下文敏感的行为，性能优化，监控和记录，调试支持，多目标的协议。
**四、还有那些常用的Aop**，**以及他们的区别**
- Jboss Aop：我基本上没有用过，所以没有发言权
- Spring Aop：Spring自己原生的Aop，只能用一个词来形容：难用。　你需要实现大量的接口，继承大量的类，所以spring aop一度被千夫所指。这于他的无侵入，低耦合完全冲突。不过Spring对开源的优秀框架，组建向来是采用兼容，并入的态度。所以，后来的Spring 就提供了Aspectj支持，也就是我们后来所说的基于纯POJO的Aop。
　　　区别：Spring Aop采用的动态织入，而Aspectj是静态织入。静态织入：指在编译时期就织入，即：编译出来的class文件，字节码就已经被织入了。动态织入又分静动两种，静则指织入过程只在第一次调用时执行；动则指根据代码动态运行的中间状态来决定如何操作，每次调用Target的时候都执行。有不清楚的同学，可以自己补下基础的代理知识。
五、**Aop术语解释**
　　　基本上每篇Aop的文章都必讲的内容，不过笔者所将可能与他们不同，笔者不会照本宣科。
　　　pointcut：　是一个（组）基于正则表达式的表达式，有点绕，就是说他本身是一个表达式，但是他是基于正则语法的。通常一个pointcut，会选取程序中的某些我们感兴趣的执行点，或者说是程序执行点的集合。
　　　joinPoint：　通过pointcut选取出来的集合中的具体的一个执行点，我们就叫JoinPoint.
　　　Advice：　在选取出来的JoinPoint上要执行的操作、逻辑。关于５种类型，我不多说，不懂的同学自己补基础。
　　　aspect：　就是我们关注点的模块化。这个关注点可能会横切多个对象和模块，事务管理是横切关注点的很好的例子。它是一个抽象的概念，从软件的角度来说是指在应用程序不同模块中的某一个领域或方面。又pointcut 和　 advice组成。
　　　Target：被aspectj横切的对象。我们所说的joinPoint就是Target的某一行，如方法开始执行的地方、方法类调用某个其他方法的代码。
