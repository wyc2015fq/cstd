
# 深入理解JAVA垃圾回收机制 - lin_tuer的博客 - CSDN博客


2017年09月28日 16:48:44[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：163


[JVM 相关–java垃圾回收作为其特色之一，面试中还是经常会被问到的]
# 一、简述垃圾回收机制
java的一个显著特点，就是引入了垃圾回收机制。使Java程序员在编写程序时不需要考虑内存管理；
由于有垃圾回收机制，Java中的对象不再有“作用域”的概念，只有对象的引用才有“作用域”；
垃圾回收机制在不可预知的情况下对内存堆中已经死亡或者长时间没有使用的对象进行清楚和回收。
垃圾回收机制可以有效防止内存泄漏，有效使用空闲的内存。
程序员不能实时调用垃圾回收，可以手动执行System.gc()，来通知Java虚拟机执行GC,但是java不保证GC一定会执行
（ps:内存泄漏：☞该内存空间使用完毕未回收。在不涉及复杂数据结构的一般情况下，java 内存泄漏表现为，一个内存对象的生命周期超出了程序需要它的时间长度。）
**涉及到的问题一般有 where when and how?**
# 二、about How–垃圾回收机制中的算法
## 1. 引用计数法（Reference counting collector）
概述：垃圾回收的早期策略。在此方法中，堆中每个对象实例都有一个引用计数器，当对象被创建，且该对象实例被分配给一个变量，变量计数器就+1。任何其他变量被赋值为这个对象的引用时，计数器+1；当该对象实例的某个引用超过了生命周期或者被设置为一个新值，计数器-1。任何计数器为0的对象实例可以被当作垃圾收集。当一个对象实例被垃圾机制收集时，它引用的任何对象实例的计算器-1。
优点：快；交织在程序中运行。对程序需要不被长时间打断的实时环境比较有利。
缺点：无法检测出循环引用。eg, 父对象有一个对子对象的引用，子对象反过来引用父对象，这样，他们的引用计数器永远不可能为0.
Example:
publicclassMain {publicstaticvoidmain(String[] args) {
        MyObject object1 =newMyObject();
        MyObject object2 =newMyObject();
        object1.object= object2;
        object2.object= object1;
        object1 =null;
        object2 =null;
    }
}最后面两句将object1和object2赋值为null，也就是说object1和object2指向的对象已经不可能再被访问，但是由于它们互相引用对方，导致它们的引用计数器都不为0，那么垃圾收集器就永远不会回收它们。
2. tracing 算法（Tracing collection）或 标记-清楚算法(mark and sweep)

