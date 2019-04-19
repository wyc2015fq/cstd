# 通过QML Profiler分析程序性能问题 - xqhrs232的专栏 - CSDN博客
2017年07月26日 11:08:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：344
原文地址::[https://zhuanlan.zhihu.com/p/25769561](https://zhuanlan.zhihu.com/p/25769561)
相关文章
1、[Qt和QML如何实现国际化](https://segmentfault.com/a/1190000002978863)----[https://segmentfault.com/a/1190000002978863](https://segmentfault.com/a/1190000002978863)
对于一个程序的开发，性能优化是开发中的一个重要步骤。
我们肯定不希望开发出来的程序表现出卡顿，最好是处处流畅，丝滑般的体验。
对于C++程序，我们有很多方法可以做性能优化，例如Visual Studio Profiler。
而对于QML（QtQuick）程序，我们可以选择QML Profiler，这是QtCreator的一个功能。
那么QML Profiler是什么呢，官方的描述如下：
> 
You can use the QML Profiler to find causes for typical performance problems in your applications, such as slowness and unresponsive, stuttering user interfaces. Typical causes include executing too much JavaScript in too few frames. All JavaScript must return
 before the GUI thread can proceed, and frames are delayed or dropped if the GUI thread is not ready.
也就是说，QML Profiler主要功能就是帮助我们去解决程序中典型的性能问题，说简单就是帮助我们做性能优化。
注意：这个性能优化，仅指QML这里，一般来说就是界面，可能还包含点界面逻辑代码（JS），而C++这块，QML Profiler几乎帮不上忙，最多是能给在QML中调用的槽函数记个耗时。
注意2：使用QML Profiler是优化QtQuick性能的基本功，之后我会再发几篇和QtQuick性能优化相关的文章，里面都会用到QML Profiler。本文只介绍QML Profiler，尽量不涉及具体的优化点。
## 正文开始：
借助于QML Profiler，我们快速的了解程序运行中的主要情况和耗时细则（可以精确到微秒），其中包括但不限于：
- 
图片缓存使用情况
- 
渲染耗时
- 
内存使用情况
- 
输入事件
- 
动画帧率
- 
编译耗时
- 
创建耗时
- 
绑定耗时
- 
信号处理耗时
- 
JS代码耗时
那么对于QML Profiler这样的工具，使用起来是否非常复杂呢？
答案是：使用非常方便，甚至不需要额外的配置，连很多分析都是可视化的。使用步骤如下：
- 
准备Qt环境，我建议是Qt5.7或者更高版本。这是因为以前QML Profiler结果中很多数据是企业版的QtCreator才看得到的，有的数据非常重要，例如渲染的耗时。后来才向开源版开放。是5.6还是5.7有点记不清了，我用的是Qt5.8.0配合QtCreator4.2.1。
- 
打开QtCreator
- 
打开一个QML（QtQuick）工程，我这里使用的是默认的工程![](https://pic2.zhimg.com/v2-18f44c8e40a8375df653f219565a9dfd_b.png)
- 
选择debug模式![](https://pic1.zhimg.com/v2-abaa668b7f9c55ebbe3900718aadd980_b.png)
- 
在QtCreator上边菜单中选择 Analyze 中的 QML 分析器![](https://pic3.zhimg.com/v2-c9b7b9a95d6c61f344883b56b6cbd16e_b.png)
- 
等待程序运行起来，并且运行一段时间
- 
在QtCreator左边菜单中选择 Debug ，然后点击 Stop 按钮，停止QMLProfiler，并且开始处理数据![](https://pic4.zhimg.com/v2-c0153d2d36a82397f24f506be4ce29f3_b.png)
至此，QML Profiler应当已经成功运行，并且收集到了数据，如图。
![](https://pic4.zhimg.com/v2-98aaf460a486332172eafdeac5ea696b_b.png)
图中，我们可以看到如下几个主要的数据区：
- 
时间轴视图
- 
统计信息视图
- 
火焰视图
----------
分别介绍一下3个视图
## 时间轴视图
![](https://pic1.zhimg.com/v2-7d71e91e14dcd416ece3d49b8612be54_b.png)
在这里，我们可以以时间轴角度，查看各个细节的耗时。时间轴的起点，就是QQmlApplication实例化的时间。我们可能看不到零点，因为在QQmlApplication被实例化到第一个元素被开始处理，时间可能会有其他的耗时。
在视图中，从左到右，就是QML Profiler从开始到停止的所有记录了。越小的块表示时间越短，反之越大的块，表示时间越长。这里的方块具有一定的嵌套关系，下面的方块对象隶属于上面的对象。比如说 Windows { } 里面还可能会有一个 Item { } 这样的嵌套关系。
如果在你的程序中，发现一个非常巨大的块，那么恭喜你，你可能已经找到了程序性能卡顿的所在。之后就是分析和优化了。
例如上面截图中的数据，我们发现TextEdit的创建非常耗时，创建方块（蓝色那个）非常大，几乎已经是一大半的时间都消耗在这里了。这也是导致了程序在启动时，比较慢（至少没有一瞬间起来）的罪魁祸首。
目前我知道这个是程序第一个用到字的地方（Text、TextEdit等），就会有这么慢，有这么几百毫秒，可能是在加载字体库？但是这个现象我目前只在macOS系统上看到，没有什么代表性。所以，我们暂时跳过这个不管他。
我这里略讲下时间轴视图的常用操作：
- 
详细信息查看
通过点击色块，我们可以得知这个色块的基本信息，例如下图中：
![](https://pic3.zhimg.com/v2-8e8962069c9e918fa190e0cc7f304282_b.png)
我们可以在这里看出这个时间开销是来自于TextEdit的创建，总共耗时了580毫秒，使用到这个TextEdit的地方是MainForm.ui.qml的15行。打开源码，可以看到确实在这里。
![](https://pic2.zhimg.com/v2-b24dd286145be520bf0b5e0b18ad28ad_b.png)- 
根据事件类型展开
在左侧不同类型的事件中，我们可以点击那个展开按钮，这样我们就可以看到展开的详细数据，这样看数据对应关系时会更加的清楚，但是当数据很多的时候也会更加的凌乱，所以酌情使用。
![](https://pic4.zhimg.com/v2-9830c0c91a70e416ab24c4559cdb2623_b.png)
- 
缩放按钮
在左侧有一个放大镜，可以缩放视图的比例，这对于分析一段比较长的QML Profiler或者想看某一个细节点的数据会非常有用
![](https://pic3.zhimg.com/v2-419bc2c001fe5ea3904962b94f0f0c46_b.png)
----------
## 统计信息视图
![](https://pic2.zhimg.com/v2-cb3fa77e2c6a9aca23a0c08f3795ce2d_b.png)
在这里，我们可以看到每个细则，例如编译、创建、绑定、JavaScript或者信号处理的次数以及它们所消耗的时间。
除了在时间轴那里，通过肉眼观察，我们在这里，通过对百分比的排序，也可以迅速的看出哪个东西最耗时。
如图，前面提到的TextEidt，通过排序后，就排到了第一位。
----------
## 火焰视图
![](https://pic2.zhimg.com/v2-0bf0359b02e7aedd2f55346b197c9551_b.png)
在这里，我们可以看到更加简洁的QML和JS统计。其中也直观的告诉了我们一些嵌套关系。比如对于之前提到的TextEdit，我们可以很清楚的看到它是Window里面的MainForm里面的Rectangle里面的TextEdit。嵌套关系一目了然。同时我们也可以在这里看出，TextEdit的创建开销，总共占据了整个程序的74%，看来优化Window的创建主要就是去掉它了。
综上，这是最基本的3个功能区，构成了QML Profiler。我们程序的性能分析，主要也围绕着这三点展开。
## 主体介绍完毕，介绍细节
讲完3个基本视图，现在再来详细说下时间轴中数据的意义，这对于我们数据分析是比较重要的一块：
- 
Pixmap Cache![](https://pic2.zhimg.com/v2-9209aa45cb13058c288925dcd7021d09_b.png)
在QML中使用的Image，默认是开缓存的。而所有缓存的图片，都会在这里显示，包括用了多少像素的缓存，还包括了图片的加载耗时、文件名等信息。（没有缓存的图片也会显示，但不会记入到缓存的阶梯里）
- 
Scene Graph![](https://pic2.zhimg.com/v2-e77499a041a179e681b53851d51f0e6d_b.png)
这里显示渲染时各个阶段的耗时，如果我们发现程序的动画有卡顿，除了一些函数的阻塞导致的卡顿外，还可以分析一下渲染的耗时开销，看看是不是渲染的量太大导致的卡顿。
这里我们主要关注Render Render这个数据，这个数据表示将OpenGL数据发送到GPU的过程。看到一个Render Render的结束，基本表示这一帧已经结束渲染，并且即将显示出来了。
另外还有Glyph Upload这个数据，这个数据表示字形纹路上传。如果你的程序是嵌入式，并且有很多的字，那么Glyph Upload有可能会带来一定的性能开销。减少这个开销的方式基本就是减少字，比如说用图片（Image）代替文字（Text或者Label）。
- 
Memory Usage![](https://pic3.zhimg.com/v2-aa8b6692e10490b11e809fefde6b8322_b.png)
显示内存使用情况，如果这里有大块的内存增长，看看是不是这里在初始化很多东西，或者是有很多不必要的组件被创建出来了。
- 
Input Events![](https://pic4.zhimg.com/v2-a6a6529528a1502eac94eb18eb2f0e8b_b.png)
显示用户输入事件，例如鼠标和键盘事件
- 
Debug Mssages![](https://pic4.zhimg.com/v2-aba151067eca8296c2193b586549a2cf_b.png)
显示调试输出的时间点，如果你需要对照Debug输出和对应的QML事件，那么这会很有帮助
- 
Animations![](https://pic2.zhimg.com/v2-353aa681677403905367a166cf3f9d79_b.png)
显示是否有动画在执行，以及动画的FPS，在多线程渲染时还会显示多线程的信息。如果我们发现FPS低于18，那么视觉上可能就会有明显的卡顿了。而30到60的FPS，一般就可以认为是流畅的。
- 
Compiling![](https://pic4.zhimg.com/v2-87da0aeae0c98ea92fc2c1f0ad567a77_b.png)
显示编译的耗时。这里要说下，从Qt5.8开始，QtQuick引入了qmlc机制，让编译时间大幅度缩减，基本上是从几百毫秒，缩减到几十甚至十毫秒以内。之前在csdn发过文章讲这个，这里再放下链接：
[Qt：5.8新特性，qmlc使用以及效果 - Jason’s
 home - 博客频道 - CSDN.NET](http://link.zhihu.com/?target=http%3A//blog.csdn.net/wsj18808050/article/details/54710485)
- 
Creating![](https://pic3.zhimg.com/v2-a5689a0f5e7065d0c368254f4f4adf2e_b.png)
显示创建的耗时，一般也是启动优化的主要部分。
- 
Binding![](https://pic3.zhimg.com/v2-1620985ad51dbf0acf808b4d6b227e5e_b.png)
显示绑定的耗时
- 
Handling Signal![](https://pic2.zhimg.com/v2-6bba539b061e7c74f8673205ff5f4395_b.png)
显示信号处理的耗时
- 
JavaScript![](https://pic3.zhimg.com/v2-21fb799edc4294268ad84b81e9d4f886_b.png)
显示JS执行的耗时。如果在QML里调用了一个C++的槽，那么这里也会有计时，但是也只有槽函数的总耗时，C++那里的运行情况这里看不出来。
那么，这么多的数据，我们要如何优化呢？
我的建议是这样的
如果程序有明显的加载慢问题，那么可以先去看创建，找大块，去延后加载或者异步加载。让首界面先显示出来。尤其是图片，图片的加载比较慢，尽量选择合适分辨率的图片，不要过大。对于不会再第一时间显示的东西，尽量不要在第一时间加载。
如果程序有明显卡顿问题，那么可以看渲染那里，是不是渲染的东西太多了，例如用了过多的clip。或者有很多在视觉上看不到的元素，例如xy为-1000这样的Item，没有被隐藏，这些Item照样会渲染，照样会有性能开销，对于这些元素可以将visible设置为false，直接影藏掉，这就不会有渲染耗时了。例外值得一提的是，对于有动画的场景，建议把每帧时间控制在16ms以内，以维持60FPS的流畅界面。
关于性能优化进一步的细节点，这里不展开，以后单独发文章讲，本文只讲QML Profiler的基础。更多关于QML Profiler的信息，可以前往官网查看：
[Profiling QML Applications](http://link.zhihu.com/?target=http%3A//doc.qt.io/qtcreator/creator-qml-performance-monitor.html)
在保留出处（也就是本篇知乎文章的链接）情况下，本文可以任意转载。

