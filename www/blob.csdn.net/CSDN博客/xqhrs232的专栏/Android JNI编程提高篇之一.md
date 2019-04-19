# Android JNI编程提高篇之一 - xqhrs232的专栏 - CSDN博客
2010年12月22日 14:33:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：885标签：[jni																[android																[虚拟机																[java																[reference																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=虚拟机&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://my.unix-center.net/~Simon_fu/?p=849](http://my.unix-center.net/~Simon_fu/?p=849)
Simon在拙文《[Android JNI开发入门之二](http://my.unix-center.net/~Simon_fu/?p=836)》中提到了，Android JNI开发的深入学习的一些资料和方法。最近Simon在学习《[Java
 Native Interface: Programmer’s Guide and Specification](http://java.sun.com/docs/books/jni/)》，在学习过程中发现在JNI规范中关于引用部分是最难理解的，恕Simon愚钝，看了两遍才对其思想略有理解，在此和大家分享我对JNI规范中引用部分的理解，如果有错误之处，希望能和各位网友一起探讨。
### 什么是JNI规范中对象引用
    在JNI编程中，Native代码不能对Java虚拟机中对象的内存分布有任何假设。因为Java虚拟机可以根据自己的策略定义自己对象的内存布局。这就要求JNI规范有如下要求：
> 
1、如果要在Native代码中生成Java对象，则必须调用Java虚拟机的JNI接口来生成。 
2、对Java对象的操作也需要调用Java虚拟机的JNI接口来进行。 
3、在Native代码中，通过引用来访问Java对象。对象引用有Java虚拟机的JNI接口返回，比如：NewStringUTF函数，用utf8的字符串创建一个Java字符串对象，在Native代码中以jstring类型的引用来访问它。
    JNI规范中的引用和Java中应用有类比关系，大家可以相互对照。
### JNI引用的分类
    JNI规范中定义了三种引用——局部引用（Local reference），全局引用（Global reference），弱全局引用（Weak global reference）。
    这算三种引用的生存期是不同的，全局引用和弱全局引用的生存期为创建之后，到程序员显示的释放他们。局部引用会被Java虚拟机在当前上下文（可以理解成Java程序调用Native代码的过程）结束之后自动释放。
    全局引用和局部引用将会迫使Java虚拟机不会垃圾回收其指向的对象。而弱全局引用指向的对象可以被Java虚拟机垃圾回收。
    每种引用都有自己的用途，比如当Native函数返回的时候，需要返回局部引用（和C语言的局部变量要区分开）。全局引用和弱全局引用在多线程之间共享其指向的对象等。
### 局部引用
    这里重点要强调一下局部引用的有效期，很多有C语言背景的程序员会认为当Native函数结束之后局部引用就无效了，和C语言的局部变量对应。实际上JNI中的局部引用和C语言中局部变量是不同的，他的有效期是当前Native函数被调用的上下文中。我理解的调用上下文，为Java虚拟机的调用流程。Native函数是被Java虚拟机调用的，Native函数执行完成之后，控制流程将继续返回给Java虚拟机。局部变量在Native函数中，由Native代码调用Java虚拟机的JNI接口创建，秉着谁创建谁销毁的原则（软件设计一个常用规则），当Native函数执行完成之后，如果局部引用没有被Native代码显示删除，那么局部引用在Java虚拟机中还是有效的。Java虚拟机来决定在什么时候来删除这个对象。这和C语言的局部变量概念是不同的。这也可以解释为什么Natvie函数能够以一个局部引用为返回值了。
    JNI编程中，局部引用是最常被用到的概念。比如：大部分JNI函数都返回一个局部引用，所以局部引用概念理解正确非常重要。
    局部引用在Native代码显示释放非常重要。你可能会问，既然Java虚拟机会自动释放局部变量为什么还需要我在Native代码中显示释放呢？原因有以下几点：
> 
1、Java虚拟机默认为Native引用分配的局部引用数量是有限的，大部分的Java虚拟机实现默认分配16个局部引用。当然Java虚拟机也提供API（[PushLocalFrame](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#push_local_frame)，[EnsureLocalCapacity](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#ensure_local_capacity)）让你申请更多的局部引用数量（Java虚拟机不保证你一定能申请到）。有限的资源当然要省着点用，否则将会被Java虚拟机无情抛弃（程序崩溃）。JNI编程中，实现Native代码时强烈建议调用[PushLocalFrame](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#push_local_frame)，[EnsureLocalCapacity](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#ensure_local_capacity)来确保Java虚拟机为你准备好了局部变量空间。
2、如果你实现的Native函数是工具函数，会被频繁的调用。如果你在Native函数中没有显示删除局部引用，那么每次调用该函数Java虚拟机都会创建一个新的局部引用，造成局部引用过多。尤其是该函数在Native代码中被频繁调用，代码的控制权没有交还给Java虚拟机，所以Java虚拟机根本没有机会释放这些局部变量。退一步讲，就算该函数直接返回给Java虚拟机，也不能保证没有问题，我们不能假设Native函数返回Java虚拟机之后，Java虚拟机马上就会回收Native函数中创建的局部引用，依赖于Java虚拟机实现。所以我们在实现Native函数时一定要记着删除不必要的局部引用，否则你的程序就有潜在的风险，不知道什么时候就会爆发。
3、如果你Native函数根本就不返回。比如消息循环函数——死循环等待消息，处理消息。如果你不显示删除局部引用，很快将会造成Java虚拟机的局部引用内存溢出。
    在JNI中显示释放局部引用的函数为[DeleteLocalRef](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#DeleteLocalRef)，大家可以查看手册来了解调用方法。
    在JDK1.2中为了方便管理局部引用，引入了三个函数——[EnsureLocalCapacity](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#ensure_local_capacity)、[PushLocalFrame](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#push_local_frame)、[PopLocalFrame](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#pop_local_frame)。这里介绍一下[PushLocalFrame](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#push_local_frame)和[PushLocalFrame](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#push_local_frame)函数。这两个函数是成对使用的，先调用[PushLocalFrame](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#push_local_frame)，然后创建局部引用，并对其进行处理，最后调用[PushLocalFrame](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#push_local_frame)释放局部引用，这时Java虚拟机也可以对其指向的对象进行垃圾回收。可以用C语言的栈来理解这对JNI
 API，调用[PushLocalFrame](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#push_local_frame)之后Native代码创建的所有局部引用全部入栈，当调用[PopLocalFrame](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#pop_local_frame)之后，入栈的局部引用除了需要返回的局部引用（[PushLocalFrame](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#push_local_frame)和[PopLocalFrame](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#pop_local_frame)这对函数可以返回一个局部引用给外部）之外，全部出栈，Java虚拟机这时可以释放他们指向的对象。具体的用法可以参考手册。这两个函数使JNI的局部引用由于和C语言的局部变量用法类似，所以强烈推荐使用。
    当创建局部变量之后，将迫使Java虚拟机不对其指向的对象进行垃圾回收，直到Native代码显示调用了[DeleteLocalRef](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#DeleteLocalRef)删除局部引用。Native代码调用[DeleteLocalRef](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#DeleteLocalRef)显示删除局部引用之后，Java虚拟机就可以对局部引用指向的对象垃圾回收了。当Native代码创建了局部引用，但未显示调用[DeleteLocalRef](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#DeleteLocalRef)删除局部引用，并返回Java虚拟机的话，那么由虚拟机来决定什么时候删除该局部引用，然后对其指向的对象垃圾回收。程序员不能对java虚拟机删除局部引用的时机进行假设。
    局部引用仅仅对于java虚拟机当前调用上下文有效，不能够在多次调用上下文中共享局部引用。这句话也可以这样理解：局部引用只对当前线程有效，多个线程之间不能共享局部引用。局部引用不能用C语言的静态变量或者全局变量来保存，否则第二次调用的时候，将会产生崩溃。。
    本文叙述了JNI规范中局部引用的理解，后续文章《[Android JNI编程提高篇之二](http://my.unix-center.net/~Simon_fu/?p=856)》我们将会继续关注全局变量和弱全局变量。

