# Android性能优化的方方面面 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年02月14日 15:32:25[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：260标签：[android																[性能优化](https://so.csdn.net/so/search/s.do?q=性能优化&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/qq_30513483/article/category/6189211)
![](http://upload-images.jianshu.io/upload_images/1836169-5d91e23bf8ef7a92.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
本文来自简书，原文地址:[http://www.jianshu.com/p/b3b09fa29f65](http://www.jianshu.com/p/b3b09fa29f65)
通常项目比较大的APP都面临着如下性能问题，APP启动慢、界面跳转慢、事件相应慢、滑动和动画卡顿、展现内容慢等，有的公司处理的很好，有的还有很大的优化空间，对于性能优化，怎么去做，我总结了一下性能优化如何入手，Android性能优化这个系列目前已经完成11篇，涉及到常用内存优化工具的使用，比如Memory Monitor、MAT、Allaction Tracing、TraceView 等，也涉及到如何提高应用的启动速度以及页面的渲染速度，还有流畅度的检测。学会这些，基本上可以处理一般的性能问题了，这段时间需要研究插件化和算法，实在抽不出来时间去总结学习性能优化的相关知识了，日后会在此页面中更新。
#### 1、[Android性能优化第（一）篇---基本概念](http://www.jianshu.com/p/c0e5c13d5ecb)[http://www.jianshu.com/p/c0e5c13d5ecb](http://www.jianshu.com/p/c0e5c13d5ecb)
第一篇简单回顾java的内存区域划分、引用、内存泄露以及场景，还有垃圾回收
#### 2、[Android性能优化第（二）篇---Memory Monitor检测内存泄露](http://www.jianshu.com/p/ef9081050f5c)[http://www.jianshu.com/p/ef9081050f5c](http://www.jianshu.com/p/ef9081050f5c)
第二篇主要是使用过Android Studio自带工具Memory Monitor 检测内存泄露
#### 3、[Android性能优化第（三）篇---MAT比Menmery Monitor更强大](http://www.jianshu.com/p/2d47d1cf5ccf)[http://www.jianshu.com/p/2d47d1cf5ccf](http://www.jianshu.com/p/2d47d1cf5ccf)
接上一篇，介绍另外一种内存泄露的检测工具MAT
#### 4、[Android性能优化第（四）篇---Android渲染机制](http://www.jianshu.com/p/9ac245657127)[http://www.jianshu.com/p/9ac245657127](http://www.jianshu.com/p/9ac245657127)
学习一下Android渲染机制，为什么16ms没完成绘制就会卡顿？
#### 5、[Android性能优化第（五）篇---Allaction Tracing追踪内存分配的轨迹](http://www.jianshu.com/p/d9e426c10581)[http://www.jianshu.com/p/d9e426c10581](http://www.jianshu.com/p/d9e426c10581)
Allaction Tracing是追踪内存分配的工具，可以很直观的看到某个操作是如何一步步分配的
#### 6、[Android性能优化第（六）篇---TraceView 分析图怎么看](http://www.jianshu.com/p/388c693c1b58)[http://www.jianshu.com/p/388c693c1b58](http://www.jianshu.com/p/388c693c1b58)
TraceView 是 Android 平台特有的数据采集和分析工具，主要用做热点分析，找出最需要优化的点。我就是靠它将我公司的项目的启动速度明显的提高了。
#### 7、[Android性能优化第（七）篇---StrickMode严苛模式检测耗时与内存问题](http://www.jianshu.com/p/2ebc9363ea16)[http://www.jianshu.com/p/2ebc9363ea16](http://www.jianshu.com/p/2ebc9363ea16)
StrictMode意思为严格模式，是用来检测程序中违例情况的开发者工具。使用一般是场景是检测主线程中本地磁盘和网络读写等耗时的操作。注意这个StrictMode是在Anroid2.3以后引入的。严格模式主要检测两大问题，一个是线程策略，即TreadPolicy，另一个是VM策略，即VmPolicy。
#### 8、[Android性能优化第（八）篇---App启动速度优化之耗时检测处理](http://www.jianshu.com/p/a0e242d57360)[http://www.jianshu.com/p/a0e242d57360](http://www.jianshu.com/p/a0e242d57360)
介绍如何统计应用的启动时间和性能检查项，并且用TraceView逐个修复
#### 9、[Android性能优化第（九）篇---App启动速度优化之启动页设计](http://www.jianshu.com/p/777fe2f71039)[http://www.jianshu.com/p/777fe2f71039](http://www.jianshu.com/p/777fe2f71039)
因为启动页一般View的数量比较少，业务逻辑比较简单，启动不太费劲，但是该怎么写启动页面让应用的启动速度更快呢？
#### 10、[Android性能优化第（十）篇---布局优化](http://www.jianshu.com/p/c0e0cca14162)[http://www.jianshu.com/p/c0e0cca14162](http://www.jianshu.com/p/c0e0cca14162)
Android系统每隔16ms就重新绘制一次Activity，如果没有完成就会有丢帧的现象。为了减轻UI绘制的负担，有必要把Layout编写的一些注意事项总结一下
#### 11、[Android性能优化第（十 一）篇---卡顿分析，正确评测流畅度](http://www.jianshu.com/p/d126640eccb1)[http://www.jianshu.com/p/d126640eccb1](http://www.jianshu.com/p/d126640eccb1)
说到应用的流畅度，都会想到FPS，FPS评测应用流畅度不准确，系统获取FPS的原理是：手机屏幕显示的内容是通过Android系统的SurfaceFLinger类，把当前系统里所有进程需要显示的信息合成一帧，然后提交到屏幕上进行显示，FPS就是1秒内SurfaceFLinger提交到屏幕的帧数。但是用FPS来评测一个应用是否真的卡顿存在两个问题，我们可以使用Choreographer帧率检测。
还有几个想写的，过两个月抽出来时间去总结，对低性能say no,希望朋友们来个mark，知识共享。未来性能优化还有四篇，如下
#### 12、[Android性能优化第（十二）篇---耗内存大户Bitmap](http://www.jianshu.com/p/b3b09fa29f65)
#### 13、[Android性能优化第（十三）篇---安装包性能瘦身](http://www.jianshu.com/p/b3b09fa29f65)
#### 14、[Android性能优化第（十四）篇---MultiDex初次启动优化](http://www.jianshu.com/p/b3b09fa29f65)
#### 15、[Android性能优化第（十 五）篇---Systrace的使用](http://www.jianshu.com/p/b3b09fa29f65)
