# ndk-build C++优化 - 三少GG - CSDN博客
2013年06月11日 21:55:29[三少GG](https://me.csdn.net/scut1135)阅读数：1933
[ndk-build C++优化](http://blog.csdn.net/wangchenggggdn/article/details/7551197)
随着[Android
 NDK](http://developer.android.com/sdk/ndk/index.html)（本机开发套件）披露了许多的新设备和新功能，现在我们可以充分利用这些ARM设备了。下面列举了一些快速提示，希望对您有所帮助。
1 - 关注目标
最新的设备一般是[ARMv7](http://en.wikipedia.org/wiki/ARM_architecture#ARM_cores)，这意味着它可以使用v7版本和功能。最新版的NDK增加了ARMv7和NEON代码支持，可以实现关键循环操作和媒体操作优化，远超其它方法。NDK提供小型静态库，可以帮助您识别运行时的选项。有关如何使用这些功能的示例，请参见NDK样本目录中的[hello-neon](http://android.git.kernel.org/?p=platform/ndk.git;a=blob;f=apps/hello-neon/project/jni/helloneon.c;h=ee19e0873ac4cc88ff94dc3089ed04f9766175e6;hb=HEAD)示例项目。
v6是较旧的设备，但是NDK未提供专门支持，您需要自己选择是安全地构建v5TE还是冒险尝试可能是v5TE的设备。如果您对速度无甚要求，并清楚目标硬件是什么，则可以尝试构建v6。尽管无法避免狂热爱好者将新版的OS装入旧硬件上，但事实是支持Android
 2.0及以上版本的最新设备通常是基于ARMv7的。如果有选择的话，最好专注于采用NEON实施方案的ARMv7。
2 - 不要仓促优化
除非您计划移植现有的C++应用程序，否则不要仓促编译本机代码。首先确保应用程序正确运行并仔细地profiling[配置](http://developer.android.com/guide/developing/tools/traceview.html)。成为瓶颈的要素通常是您意想不到的。近期的手持设备都具有两级缓存，有时还有用于节省电量的时钟调度，再加上Google为了提高OS性能而付出的长期努力，我们将会看到更多惊喜。
3 - 优化，悄无声息
不留痕迹。为了得到额外的运行周期会诱使您打破设计，但如非特殊情况，应避免这样。执行优化时，轻轻进入，迅速将应用程序关键部分转为本机或汇编代码，然后不留痕迹地退出。这样，您不用牺牲可维护性和调试便利性。当然，如果您确定重新设计可以带来足够的速度且值得付出代价，则可放手大干。
4 - 围绕优化实施重构
设计完成后，务必重新排列代码以使其更加适于优化，但一旦本机代码开始工作，则要避免修补过度。Java代码更易重新排列和调试。通常应先在Java层面优化，以便让逻辑块接受本机代码。在移植本机代码时，在Java层面改变算法或数据结构更具优势且更易处理。
5 - 维护Java的异常处理
执行不支持的本机代码是个糟糕的主意；即使理想状态下，它也可能造成应用程序意外退出。开发人员可以大胆推测本机代码是否与运行程序的设备兼容，只要有Java实施方案，您可以随时退回Java。这就是技巧4中优化Java版本的好处所在。
6 - 小心的分配内存
不论何时，在Java中尽可能用Java来分配，而不要拖延到以后调用C函数来解围。这样才能最大程度地减少泄漏并使Java代码保持简洁。如果有长期操作，则应在本地分配暂存空间，但请注意您的指针 -
 编写大量Java代码时，围绕指针准则的旧C语言习惯可能会忘得很快。
7 - 谨慎采用多线程
已经有好几个采用多ARM内核的开发平台，客户使用它们的速度可能比您想像的更快。这样的想法会诱使您将所有任务都分割为多线程。总的来说这是个好主意，但切记，加重系统负载尽管能提升速度，也会牺牲用户体验。当然，巧妙使用线程非常地有效。Android系统自身会创建几条线程去运行一个应用程序，然而它们一般都在等待事件，所以如果您自己创建太多活动执行线程，可能会削弱它们的运行能力，适得其反地降低整体用户体验。
8 - Java层的线程
如果将逻辑分为多个线程，应尽可能通过Java执行，而不要采用Pthreads。使用Java VM管理访问可以降低风险，且有更多的语言层工具。如果您担心设置成本，采用现有的小型工人池应该不会太困难。切记将长期任务的更新发回到主UI线程，以确保用户知悉。另外请牢记一条重要的Android
 NDK规则：不要在UI线程上运行本机代码。事实上，最好不在UI线程上运行任何Java代码；Android有相应的API（请查看Handler类）可在非UI线程上经济运行任务。
9 - 懂得何时使用汇编程序
C或C++可以立即带来速度提升，而不需要编写比Java要难以维护的代码。也可通过将代码关键部分转换为汇编程序来实现进一步提升，但需要在耗费的时间、精力与最终受益之间做出权衡。最好先发布工作代码，在以后发布快速升级，而不是永无休止的调试。尽管如此，如果您十分清楚想要做什么，也许可以获得更多性能。
切记要检查发布出来的GCC，这一点至关重要。[objdump](http://en.wikipedia.org/wiki/Objdump)是一款非常有用的工具，谷歌NDK下载目录位于build/prebuilt/linux-x86/arm-eabi-4.4.0/arm-eabi/bin下提供了一种版本。
10 - 懂得如何使用NEON
NEON是一个非常强大的工具，在理想环境中，可给优化汇编程序提供8倍的性能提升，但并不适用于所有环境。要了解如何充分利用NEON，请阅读本博客中的NEON指南。
- [NEON加载和存储](http://blogs.arm.com/software-enablement/coding-for-neon-part-1-load-and-stores/)
- [处理遗留工作](http://blogs.arm.com/software-enablement/coding-for-neon-part-2-dealing-with-leftovers/)
[View
 post in English.](http://blogs.arm.com/software-enablement/238-10-android-ndk-tips/)
Dave，ARM首席工程师，致力于虚拟机的维护、优化、检查至少十年，为硬件工程师解答有关虚拟机的问题。在Java领域经营多年后，他得以抽出时间研究工具，现在正着力于研究并理解Android。
示例：
# Android ffmpeg的x86编译和优化
基于FFMPEG的Android-ARM项目很多，最具有代表性的有2个：
havlenapetr ：最早期的完全基于FFMEPG的android开源项目，整个项目比较简单，音视频同步做的很初步，音视频的显示是直接在android工程内添加hack，直接从NDK层调用系统的音视频输出。该项目比较简单，但是整个工程的编译写的非常好，适合初学者学习和移植ffmpeg，项目地址：[https://github.com/havlenapetr](https://github.com/havlenapetr)
VideoLAN：（简称 VLC）：整合了FFMEPG和众多开源项目，全平台全格式，且有网络支持，VLC大而全，功能完善，所有功能都实现为独立的module，且各个module可以动态加载。VLC也支持android全平台，视频输出使用系统库，只是VLC相当复杂，不太好移植项目地址：[http://git.videolan.org/](http://git.videolan.org/)
