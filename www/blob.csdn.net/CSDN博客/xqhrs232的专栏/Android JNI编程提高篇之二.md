# Android JNI编程提高篇之二 - xqhrs232的专栏 - CSDN博客
2010年12月22日 14:36:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1002标签：[jni																[android																[虚拟机																[java																[多线程																[interface](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=虚拟机&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://my.unix-center.net/~Simon_fu/?p=856](http://my.unix-center.net/~Simon_fu/?p=856)
上文《[Android JNI编程提高篇之一](http://my.unix-center.net/~Simon_fu/?p=849)》中我和大家分享了局部引用的一些理解，本文我将继续和大家全局引用和弱全局引用的一些理解。这也可以作为我学习《[Java
 Native Interface: Programmer’s Guide and Specification](http://java.sun.com/docs/books/jni/)》一个总结，欢迎大家对本文的理解拍砖。
### 全局引用
    在所有引用中，我觉得全局引用是最好理解的一个了。为什么呢？主要和C语言的全局变量非常相近。
    上文《[Android JNI编程提高篇之一](http://my.unix-center.net/~Simon_fu/?p=849)》我已经提到局部引用大部分是通过JNI API返回而创建的，而全局引用必须要在Native代码中显示的调用JNI
 API [NewGlobalRef](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#NewGlobalRef)来创建，创建之后将一直有效，直到显示的调用[DeleteGlobalRef](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#DeleteGlobalRef)来删除这个全局引用。请注意[NewGlobalRef](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#NewGlobalRef)的第二个参数，既可以用一个局部引用，也可以用全局引用生成一个全局引用，当然也可以用弱全局引用生成一个全局引用，但是这中情况有特殊的用途，后文会介绍。
    全局引用和局部引用一样，可以防止其指向的对象被Java虚拟机垃圾回收。与局部引用只在当前线程有效不同的是全局引用可以在多线程之间共享（如果是多线程编程需要注意同步问题）。
### 弱全局引用
    弱全局引用是非常让人迷惑的一个，主要是它的用法太怪异了。
    弱全局引用和全局引用一样，需要显示的创建和销毁。创建调用[NewWeakGlobalRef](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#NewWeakGlobalRef)，销毁调用[DeleteWeakGlobalRef](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#DeleteWeakGlobalRef)。
    与全局引用和局部引用能够阻止Java虚拟机垃圾回收其指向的对象不同，弱全局引用指向的对象随时都可以被Java虚拟机垃圾回收，所以使用弱全局变量的时候，要时刻记着：它所指向的对象可能已经被垃圾回收了。JNI API提供了引用比较函数[IsSameObject](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#wp16514)，用弱全局引用和NULL进行比较，如果返回JNI_TRUE，则说明弱全局引用指向的对象已经被释放。需要重新初始化弱全局引用。根据上面的介绍你可能会写出如下的代码：
static jobject weak_global_ref = NULL;
if((*env)->IsSameObject(env, weak_global_ref, NULL) == JNI_TRUE)
{
    /* Init week global referrence again */
    weak_global_ref = NewWeakGlobalRef(...);
}
/* Process weak_global_ref */
.....
    上面这段代码表面上没有什么错误，但是我们忘了一点儿，Java虚拟机的垃圾回收随时都可能发生。假设如下情形：
> 
1、通过引用比较函数[IsSameObj](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#wp16514)[ect](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#wp16514)判断弱全局引用是否有效的时候，返回JNI_FALSE，证明其指向对象有效。
2、这时Java虚拟机进行了垃圾回收，回收了弱全局引用指向的对象。 
3、这样如果我们后面访问弱全局引用指向的对象，将会引发程序崩溃，因为弱全局引用指向对象已经被Java虚拟机回收了。
    根据JNI标准手册《[Weak Global References](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#weak)》中的介绍，我们可以有这样一个使用弱全局引用的方案。在使用全局引用之前，我们先通过[NewLocalRef](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#new_local_ref)函数创建一个局部引用，然后使用这个局部引用来访问该对象进行处理，当完成处理之后，删除局部引用。局部引用可以阻止Java虚拟机回收其指向的对象，这样可以保证在处理期间弱全局引用和局部引用指向的对象不会被Java虚拟机回收。假如弱全局引用指向对象已经被Java虚拟机回收，则[NewLocalRef](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#new_local_ref)函数将会返回NULL，则创建局部引用失败，这个返回值有助于我们判断是否需要重新初始化弱全局引用。我们可以写出如下的代码：
static jobject weak_global_ref = NULL;
jobject local_ref;
/* We ensure create local_ref success */、
while ( week_global_ref == NULL
    || (local_ref = NewLocalRef(env, weak_global_ref)) == NULL )
{
    /* Init week global referrence again */
    weak_global_ref = NewWeakGlobalRef(...);
}
/* Process local_ref */
.....
(*env)->DeleteLocalRef(env, local_ref);
    注意在《[Java Native Interface: Programmer’s Guide and Specification](http://java.sun.com/docs/books/jni/)》的例子中，有很多不是按照如上的代码实现的，那些代码是有潜在风险的，请各位朋友注意。
    弱全局引用的有效期和全局引用一样，创建之后除非调用[DeleteWeakGlobalRef](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#DeleteWeakGlobalRef)显示的删除，否则将一直有效。并且弱全局引用可以在多个线程之间共享。
     你可能会问弱全局引用这么难用，为什么还要存在呢？Simon也觉得这个东西不好用，但是”存在即合理“，弱全局引用还是有他的用途的。弱全局引用是可以用来缓存jclass对象，那么你可能会问能否用全局引用来缓存jclass对象呢？答案是不可以。这里需要简单介绍一下Native的共享库的卸载。当Class Loader释放完所有的class后，然后Class Loader会卸载Native的共享库。如果我们用全局引用来缓存jclass对象的话，根据前面对全局引用对Java虚拟机垃圾回收机制的影响，将会阻止Java虚拟机回收该对象。则Class
 Loader也将不能释放这个jclass对象，进而造成Class Loader不能卸载Native的共享库（永远无法释放）。如果用弱全局引用来缓存将不会有这个问题，Java虚拟机随时都可以释放它指向的对象。
### 总结
    至此我们把JNI规范中的三种引用都进行了一个简单的介绍，在此我对以上内容做一个简单总结：
1、局部引用是Native代码中最常用的引用。大部分局部引用都是通过JNI API返回来创建，也可以通过调用[NewLocalRef](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#new_local_ref)来创建。另外强烈建议Native函数返回值为局部引用。局部引用只在当前调用上下文中有效，所以局部引用不能用Native代码中的静态变量和全局变量来保存。另外时刻要记着Java虚拟机局部引用的个数是有限的，编程的时候强烈建议调用[EnsureLocalCapacity](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#ensure_local_capacity)和[PushLocalFrame](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#push_local_frame)来确保Native代码能够获得足够的局部引用数量。
2、全局变量必须要通过[NewGlobalRef](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#NewGlobalRef)创建，通过[DeleteGlobalRef](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#DeleteGlobalRef)删除。主要用来缓存Field
 ID和Method ID。全局引用可以在多线程之间共享其指向的对象。在C语言中以静态变量和全局变量来保存。
3、全局引用和局部引用可以阻止Java虚拟机回收其指向的对象。
4、弱全局引用必须要通过[NewWeakGlobalRef](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#NewWeakGlobalRef)创建，通过[DeleteWeakGlobalRef](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/functions.html#DeleteWeakGlobalRef)销毁。可以在多线程之间共享其指向的对象。在C语言中通过静态变量和全局变量来保持弱全局引用。弱全局引用指向的对象随时都可能会被Java虚拟机回收，所以使用的时候需要时刻注意检查其有效性。弱全局引用经常用来缓存jclass对象。
5、全局引用和弱全局引用可以在多线程中共享其指向对象，但是在多线程编程中需要注意多线程同步。强烈建议在[JNI_OnLoad](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/invocation.html#JNI_OnLoad)初始化全局引用和弱全局引用，然后在多线程中进行读全局引用和弱全局引用，这样不需要对全局引用和弱全局引用同步（只有读操作不会出现不一致情况）。
    本文和前文《[Android JNI编程提高篇之一](http://my.unix-center.net/~Simon_fu/?p=849)》是我对《[Java
 Native Interface: Programmer’s Guide and Specification](http://java.sun.com/docs/books/jni/)》学习的一个总结。只包括了我认为最难的引用部分，JNI编程还有很多其他方面需要学习的方面还很多，大家可以根据《[Android JNI开发入门之一](http://my.unix-center.net/~Simon_fu/?p=833)》介绍更深入的学习。
//==================
相关资源
  1》[Java Native Interface: Programmer’s Guide and Specification](http://java.sun.com/docs/books/jni/)
  2》[Java Native Interface Specification](http://download.oracle.com/javase/1.5.0/docs/guide/jni/spec/jniTOC.html)
