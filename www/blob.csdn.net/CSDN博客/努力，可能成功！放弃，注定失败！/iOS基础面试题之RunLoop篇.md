# iOS基础面试题之RunLoop篇 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年11月28日 09:33:56[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：3721
### RunLoop是什么，有什么作用，如何获取？
- 定义
- RunLoop的实质是一个死循环，用于保证程序的持续运行，只有当程序退出的时候才会结束（由main函数开启主线程的RunLoop）
- 作用
- 保持程序的持续运行
- 处理App中的各种事件（触摸、定时器、Selector事件）
- 节省CPU资源，提高程序性能（该做事做事，没事做休息）
- 获取方法
- 使用NSRunLoop（面向对象）或者CFRunLoopRef（底层C语言）
### RunLoop的原理
- RunLoop开启一个循环事件，并接受输入事件，接受的事件来自两种不同的来源：
- 输入源（input source）(传递异步事件)
- 定时源（timer source）(传递同步事件)
- RunLoop接收到消息后采用handlePort、customSrc、mySelector和timerFired等四个方法处理对应的事件
- 当RunLoop没有接收到消息时，则进入休眠状态，以保持程序持续运行
![](http://upload-images.jianshu.io/upload_images/3502943-9f3a27349a28b9f9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
RunLoop的原理
### RunLoop接收几种输入源，系统默认定义了几种模式？
- 输入源有两种
- 基于端口的输入源（port）
- 自定义的输入源（custom）
- 系统定义的RunLoop模式有五种，最常用的有三种，如下所示：
- NSDefaultRunLoopMode
- 默认模式，主线程中默认是NSDefaultRunLoopMode
- UITrackingRunLoopMode
- 视图滚动模式，RunLoop会处于该模式下
- NSRunLoopCommonModes
- 并不是真正意义上的Mode，是一个占位用的“Mode”，默认包含了NSDefaultRunLoopMode和UITrackingRunLoopMode两种模式
### RunLoop模式的原理和使用注意点？
- 原理和注意点
- 一个RunLoop包含若干个Mode，每个Mode又包含若干个Source、Observer、Timer（如下图所示）
- 每次RunLoop启动，只能指定一个Mode，这个Mode被称为CurrentMode
- 如果需要切换Mode，只能退出Loop，再重新指定一个Mode进入， 以使不同组之间的Source、Observer、Timer互不受影响
![](http://upload-images.jianshu.io/upload_images/3502943-dcc8c40d2e4ede41.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
RunLoopMode
### RunLoop和线程有什么关系
- RunLoop与线程是一一对应的
- 程序启动时，主线程默认会自己创建RunLoop，并设置为Default模式
- 创建子线程时，必须获取当前线程的RunLoop并启动它
```
NSRunLoop *loop = [NSRunLoop currentRunLoop];
[loop run];
```
### NSTimer和RunLoop的关系？
- NSTimer需要添加到Runloop中， 才能执行的情况
```
NSTimer *timer = [NSTimer timerWithTimeInterval:1.f target:self selector:@selector(update) userInfo:nil repeats:YES];
    [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
```
- NSTimer默认被添加到Runloop中， 直接执行的情况
`[NSTimer scheduledTimerWithTimeInterval:1.f target:self selector:@selector(update) userInfo:nil repeats:YES];`
### NSTimer准确吗，如果不准确，如何设计一个准确的timer？
- 不准确
- 准确的Timer应该和当前线程的RunLoopMode保持一致
### TableView/ScrollView/CollectionView滚动时为什么NSTimer会停止？
- 一个RunLoop不能同时共存两个mode
- 当滚动视图滚动时，当前RunLoop处于UITrackingRunLoopMode，
- NSTimer的RunLoopMode和当前线程的RunLoopMode不一致，所以会停止
- 解决方法：将timer的runloopMode改为UITrackingRunLoopMode或者NSRunLoopCommonModes
### 如果NSTimer在分线程中创建，会发生什么，应该注意什么？
- NSTimer没有启动
- 在主线程中，系统默认创建并启动主线程的runloop
- 在分线程中，系统不会自动启动runloop，需要手动启动
- 解决方法：
- 启动分线程的runLoop 
### 在异步线程中下载很多图片,如果失败了,该如何处理?请结合RunLoop来谈谈解决方案
- 在异步线程中启动一个RunLoop重新发送网络请求,下载图片
### 如果程序启动就需要执行一个耗时操作，你会怎么做？
- 开启一个异步的子线程，并启动它的RunLoop来执行该耗时操作
### runloop与autoreleasepool的关系
### 如果在分线程中启动一个异步请求，会有什么问题？
判断其是否请求结束，如果未结束，要保持当前线程一直启动，直到结束
```
while(!isFinish)
     {
       [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
     }
```
### 程序启动时，runloop是如何工作的？如果程序启动就需要执行一个耗时操作，你会怎么做？
程序启动时，系统默认创建并启动主线程的runloop，runloop会默认创建两个Observe来进行监听runloop的进出和睡眠，有事情的时候就去做，没事的休眠
(线程(创建)-->runloop将进入-->最高优先级OB创建释放池-->runloop将睡-->最低优先级OB销毁旧池创建新池-->runloop将退出-->最低优先级OB销毁新池-->线程(销毁))
线程刚创建时并没有runloop，如果你不主动去获取，那么一直都不会有。
耗时操作可以放在分线程中进行，结束后回到主线程
文／Stars木木（简书作者）
原文链接：http://www.jianshu.com/p/848ea3817502
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
