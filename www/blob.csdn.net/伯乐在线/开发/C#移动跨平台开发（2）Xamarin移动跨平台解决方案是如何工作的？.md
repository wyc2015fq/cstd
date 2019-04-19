# C#移动跨平台开发（2）Xamarin移动跨平台解决方案是如何工作的？ - 文章 - 伯乐在线
原文出处： [腾飞（Jesse)（博客园）](http://www.cnblogs.com/jesse2013/p/how-xamarin-works.html)
## 概述
上一篇 [C#移动跨平台开发（1）环境准备](http://blog.jobbole.com/100470/)发布之后不久，无独有偶，微软宣布了[开放.NET框架源代码](http://news.cnblogs.com/n/508410/)并且会为Windows、Mac和Linux开发一个核心运行时（Core CLR），这也是开源的！IT媒体网站纷纷转载，博客园的C#开发者们热泪盈眶（泥煤都等这一天好久了！）
与此同时VS2015预览版更是直接集成了Android模拟器，但是其实里面并没有说集成IOS模拟器，我不知道大家是怎么得出可以直接用VS来开发Android和IOS应用的。不管怎么说，这都是个好消息。那么问题来了，C#如何来开发Android和IOS应用？微软会怎么做我们不确定，但是我们倒是可以来看看Xamarin是如何做的。
## Android系统架构
我想下面这张图做Android开发的同学应该很熟悉，下面我们就通过来了解Android系统的架构入门来看看Xamarin会怎么样去做？
![](http://jbcdn2.b0.upaiyun.com/2016/05/500308356fa063af9542318f53fafc4b.png)
- Linux Kernel 操作系统层
- Libraries And Android Runtime 各种库和Android 运行环境
- Application Framework 应用框架层 （由Java编写）
- Applications 应用程序层（由Java编写并且在Dalvk虚拟机来运行）
现在做Android开发的同学只要熟悉这些应用框架层的一些接口和类库就可以给方便的来实现自己的Android应用程序。
### 关于Dalvk虚拟机与Java运行环境的区别
- Dalvik主要是完成对象生命周期管理，堆栈管理，线程管理，安全和异常管理，以及垃圾回收等等重要功能。
- Dalvik负责进程隔离和线程管理，每一个Android应用在底层都会对应一个独立的Dalvik虚拟机实例，其代码在虚拟机的解释下得以执行。
- 不同于Java虚拟机运行java字节码，Dalvik虚拟机运行的是其专有的文件格式
- Dex文件格式可以减少整体文件尺寸，提高I/o操作的类查找速度。
- 是为了在运行过程中进一步提高性能，对dex文件的进一步优化。
- 所有的Android应用的线程都对应一个Linux线程，虚拟机因而可以更多的依赖操作系统的线程调度和管理机制
- 有一个特殊的虚拟机进程Zygote，他是虚拟机实例的孵化器。它在系统启动的时候就会产生，它会完成虚拟机的初始化，库的加载，预制类库和初始化的操作。如果系统需要一个新的虚拟机实例，它会迅速复制自身，以最快的数据提供给系统。对于一些只读的系统库，所有虚拟机实例都和Zygote共享一块内存区域。
大家注意第2点和第7点有助于我们理解Xamarin.Android的工作机制。
## Xamarin.Android 架构
![](http://jbcdn2.b0.upaiyun.com/2016/05/3f63ec4a65d420f4197937ce02aae27c.png)
Java编写的Android应用程序通过调用 Android.* 和 Java.* 这些命名空间下的类来实现一些系统的功能包括：声音、显示、OpenGl等一些通过Java API不能实现的功能或者说是与硬件、系统平台相关的功能。那这里的问题是当我们用C#来编写的时候，这些功能怎么去调用？C#写的Android 应用程序又是如何初始化的？
### Android Callable Wrappers (ACW)
当一个C#开发的Android程序运行的时候，除了一个Dalvik的虚拟机实例，还有一个Mono的虚拟机实例在运行。那个Dalvik虚拟机实体就像一个宿主，我们的APP在宿主上运行，而我们所有用C#写的方法都会以ACW的形式被调用。在Java代码中以native的式式invoke，就像invoke其它C或者C++的代码一样。
Momodroid.exe 在编译阶段会为我们的C#类生成对应的ACW。

```
using System;
  using Android.App;
  using Android.OS;
  
  namespace Mono.Samples.HelloWorld
  {
          public class HelloAndroid : Activity
          {
                 protected override void OnCreate (Bundle savedInstanceState)
                 {
                         base.OnCreate (savedInstanceState);
                         SetContentView (R.layout.main);
                 }
         }
 }
```
对应生成的ACW代码：

```
package mono.samples.helloWorld;
  
  public class HelloAndroid
          extends android.app.Activity
  {
          static final String __md_methods;
          static {
                  __md_methods = 
                          "n_onCreate:(Landroid/os/Bundle;)V:GetOnCreate_Landroid_os_Bundle_Handlern" +
                         "";
                 mono.android.Runtime.register ("Mono.Samples.HelloWorld.HelloAndroid, HelloWorld, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null", HelloAndroid.class, __md_methods);
         }
 
         public HelloAndroid ()
         {
                 super ();
                 if (getClass () == HelloAndroid.class)
                         mono.android.TypeManager.Activate ("Mono.Samples.HelloWorld.HelloAndroid, HelloWorld, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null", "", this, new java.lang.Object[] {  });
         }
 
         @Override
         public void onCreate (android.os.Bundle p0)
         {
                 n_onCreate (p0);
         }
 
         private native void n_onCreate (android.os.Bundle p0);
 }
```
大家可以看到上面那个 native的n_onCreate方法最后就会调用那个通过mono虚拟机注册的Mono.Samples.HelloWorld.OnCreate 方法。
### Managed Callable Wrappers (MCW)
上面我们讲Dalvik 虚拟机是我们C#开发的Android应用程序的宿主，它通过ACW来调用我们用C#写的方法。如果我们的这个C#里面的方法涉及到一些声音或者系统本身，或者说现在已经有很多成熟的用Java写的类库，我们想调用怎么办？ 答案是我们再通过MCW 调回去。在C#运行时中去调用Java 称之为MCW。
Xamarin.Android Framework中有很多就是MCW，关于MCW的创建以及C#与Java集成开发的东西我们后面再细讲，包括一个C#开发的Android应用程序是如何被启动的，我们后面再细讲。对于C#开发的Android应用程序来说，性能上面肯定是会有影响，但是影响有多大，我并没有做过具体测试，所以就不详述了。后面我们再细细的去对比。
## Xamarin.IOS 架构
对于开发者来说，Xamarin.IOS相对于Xamarin.Android就要简单很多了，我们用C#开发的ios应用程序在被编译成IL代码之后，然后转交给Apple complier直接编译成IPhonee的本地机器码。也就是说C#写的IPhone应用程序和objective-c 写的是一样的。并且Xamarin团队保证在每一个IOS系统更新的时间同步更新Xamarin.IOS这样我们就不用担心被滞后了。
## 小结
这仅仅是一个粗略的介绍，Xamarin移动跨平台方案是如何来工作的，还有太多我们没有涉及。 Xamarn.Forms为多种移动平台提供了统一的UI（虽然目前还有些比较特别的没有实现，官方也没有力推它，用来做一些原型，或者概念验证可以试一样，另外如果你的UI确实很简单，那倒是没有问题。），当然在Xamarin.Forms不能够满足你的要求的时候，可以使用Xamarin.Android和Xamarin.IOS的UI控件，这些也是Native的，也就是说在UI方面不会有性能的影响。
就目前来看C#来开发IOS应用更具有优越性，我们要在VS中开发Iphone的应用程序需要配一台Mac来做编译。相对而言，开发Android的应用程序就会显得繁琐一些，底下还有一些隐藏的东西我们没有具体的讲，包括一个app的创建过程，C#与Java集成在一起开发时候的交互等等，这些问题我们就留在后面慢慢来解决。
###  参考阅读
http://developer.xamarin.com/guides/android/under_the_hood/architecture/
http://en.wikipedia.org/wiki/Java_Native_Interface
http://developer.xamarin.com/guides/android/advanced_topics/java_integration_overview/
