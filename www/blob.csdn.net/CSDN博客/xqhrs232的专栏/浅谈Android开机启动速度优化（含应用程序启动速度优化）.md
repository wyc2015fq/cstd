# 浅谈Android开机启动速度优化（含应用程序启动速度优化） - xqhrs232的专栏 - CSDN博客
2012年02月29日 15:02:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：686标签：[android																[优化																[google																[图形																[list																[ebook](https://so.csdn.net/so/search/s.do?q=ebook&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=图形&t=blog)](https://so.csdn.net/so/search/s.do?q=google&t=blog)](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/jackyu613/article/details/6044297](http://blog.csdn.net/jackyu613/article/details/6044297)
众所周知Android开机启动速度较慢，于是如何加快启动速度便成为一个值得讨论的问题。
在查阅过许多资料后（特别是Google Group的android-platform），我整理总结出下面几点基本看法。
Android开机启动耗时较多的部分有2个，分别是preload classes和scan packages。
这里又数preload classes最为耗时，在我的机子上一般需要13秒左右。关于preload classes的优化，可以参见[http://www.eoeandroid.com/thread-29953-1-1.html](http://www.eoeandroid.com/thread-29953-1-1.html)。这篇帖子并没有给出如何优化preloaded-classes list的具体取舍。实际上，在看过google
 group众多关于preload class的主题后，基本可以确定以下事实：  
- preloaded-classes list中预加载的类位于dalvik zygote进程的heap中。在zygote衍生一个新的dalvik进程后，新进程只需加载heap中没有预加载的类（这些后加载进来的类成为该进程所private独有的），这样便加快了应用程序的启动速度。实际上这是一种以空间换时间的办法，因为几乎没有一个应用程序能够使用到所有的预加载类，必定有很多类对于该应用程序来说是冗余的。但是也正如Google所说，智能手机开机远没有启动应用程序频繁——用户开机一次，但直到下次再开机之前可能要运行多个应用程序。因此牺牲一点启动时间来换取应用程序加载时的较快速度是合算的。
- preloaded-classes list已经是Google Android工程师使用众多测试工具分析，加以手动微调后形成的最优化预加载列表，涵盖了智能机上最长见的应用类型所需要的各种类。很难想象我们自己能够有什么手段能够获得比这样更优的一个预加载列表。所以，除非你的Android系统是被移植到非智能手机设备上使用（例如MID、EBOOK，可以不需要Telephony相关的类），不建议去“优化”preloaded-classes list。
- 在zygote中单起一个线程来做preload，是否可行？答案是否定的。首先在zygote中不可以新开线程，其次，就算新开一个线程，在目前智能机硬件条件下（单核CPU），除非有频繁大量的存储IO，否则我们不能看到我们期望加速启动效果。
关于scan packages的问题。同样参考上面提到的那篇帖子，我们从中可以知道一个事实：越少的apk安装，越短的启动时间。事实上确实如此，apk安装的多少的确影响开机速度，但相比而言，scan packages所花费的时间远没有preload classe多。似乎这里没有多少油水可榨，但起码我们知道了：尽量减少产品中预置的apk数量可以提升启动速度（哪怕精简到极致也许只节省了2s）。
最后，关于那篇帖子中提到的start services阶段，我认为虽然此阶段确实需要消耗可观的时间，但是正如文中提到的那样，优化这些services其实就是剔除我们不需要的一些services，而且不仅仅是修改SystemServer.java的问题，任何使用到被优化剔除掉的服务的代码都必须加以修改，否则系统肯定是起不来的。这样工作量大，而且难度也不小，并且有一定风险。因此对这些services的优化要慎之又慎。
那么加快启动速度是不是就没有办法了呢？也不是。除了硬件上的改动，在软件上使用BLCR技术也可以解决这个问题。在[http://blog.csdn.net/shuaiff/archive/2010/09/19/5894646.aspx](http://blog.csdn.net/shuaiff/archive/2010/09/19/5894646.aspx)这篇文章中比较详细的介绍了BLCR技术在Android上的应用情况。个人认为应用BLCR不复杂，值得我们尝试。
在此我认为同时有必要提一下应用程序启动速度加速的问题。用过Android的都会发现，第一次启动某个应用程序时比较慢，但只要不关机重启，大部分情况下以后再次启动就明显的要快许多。因此我们很容易想到一种办法，即“预加载”我们的应用程序一次，那么下次用户再次启动我们时不就快了吗？
我们首先明确一点：任何“预加载”的想法都是不切实际的。先不讨论实施在技术上的可能性，我们只要看一下Android的Activity生命周期管理就应该明白，就算你通过某种方式“预加载”了你的某个Activity，你也不能确保在用户真正要求开始运行它的时候，你所“预加载”的Activity还存在，因为Android很可能在你为“预加载”第一次启动Activity后的不久就将它gc掉了。依靠一个不可靠的技术，显然是不明智的。
那么还有没有别的办法呢？答案是有的，但是只在少数情况下才有一定意义。在源码的frameworks/base/core/res/res/values/arrays.xml中，我们可以看到有名为“preloaded_drawables”的项，其中列出的是Android在启动时预加载的图形资源，这样在某个应用程序需要这些图形资源时就不必再加载了。如果我们某个应用程序包含大量的图形资源，那么我们可以将其加入到这个preloaded_drawables项中以加快我们应用程序的启动速度。但是这样有一个显而易见的弊端：同preload
 classes一样，不是每个应用程序都需要所有预加载的图形资源，这些冗余的资源反而占据了应用程序进程的内存空间。因此，这种技术实际应用的局限性较大，仅限于这样一种情况：某个设备只运行固定的几个应用程序，而且这些应用程序包含大量的图形资源需要加载。但这样会是一个什么设备呢？
好了，到此基本上把我这两天研究的心得写出来了。限于认识水平有限，如果文中有误或者哪位能有更好的想法，欢迎在下面留言：）如果以后我又有心得，会再更新此文。
