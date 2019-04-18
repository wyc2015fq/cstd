# Runloop - weixin_33985507的博客 - CSDN博客
2019年01月23日 19:09:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
# 1 什么是Runloop
Runloop就像他的名字一样，是线程中的循环。它用来接收循环中的各种事件和安排线程工作，并且在没有工作时，不让线程退出，而是进入睡眠。
运行循环，在程序运行过程中循环做一些事情，如果没有Runloop程序执行完毕就会立即退出，如果有Runloop程序会一直运行，并且时时刻刻在等待用户的输入操作。RunLoop可以在需要的时候自己跑起来运行，在没有操作的时候就停下来休息。充分节省CPU资源，提高程序性能
- 驱动程序一直运行，并响应输入
- 决定程序响应的Event
- 节约CPU的能耗
- Message Queue（调用解耦，产生Message的对象，不能产生之后，一直阻塞在处理该事件的过程中，不能再次产生Message）
#### Runloop和线程
- 每条线程都有唯一的一个与之对应的RunLoop对象
- RunLoop保存在一个全局的Dictionary里，线程作为key，RunLoop作为value
- 线程刚创建时并没有RunLoop对象，RunLoop会在第一次获取它时创建
- RunLoop会在线程结束时销毁
- 主线程的RunLoop已经自动获取（创建），子线程默认没有开启RunLoop
# RunLoop的五个类
![143264-437d72bab5b8d8ff.png](https://upload-images.jianshu.io/upload_images/143264-437d72bab5b8d8ff.png)
Runloop.png
![143264-269b73f04730d111.png](https://upload-images.jianshu.io/upload_images/143264-269b73f04730d111.png)
Runloop的关系.png
CFRunLoopRef
CFRunLoopModeRef
CFRunLoopSourceRef
CFRunLoopTimerRef
CFRunLoopObserverRef
虽然runloop包含了五个类，但是公开的类只有图中的三个
# 2 Mode
一个Runloop 包含若干个Mode，每个Mode包含若干个Source/Timer/Observer
从上图可以看到一个runloop可以包含多个model，每个model都是独立的，而且runloop只能选择一个model运行，也就是currentModel。如果需要切换 Mode，在Runloop内部进行切换，再重新指定一个 Mode 进入。这样做主要是为了分隔开不同组的 Source/Timer/Observer，让其互不影响。
![143264-097208b4804bd9a9.png](https://upload-images.jianshu.io/upload_images/143264-097208b4804bd9a9.png)
Mode的种类.png
系统默认注册了5个Mode:
- NSDefaultRunLoopMode：App的默认Mode，通常主线程是在这个Mode下运行
- UITrackingRunLoopMode：界面跟踪 Mode，用于 ScrollView 追踪触摸滑动，保证界面滑动时不受其他 Mode 影响
- UIInitializationRunLoopMode: 在刚启动 App 时第进入的第一个 Mode，启动完成后就不再使用
- GSEventReceiveRunLoopMode: 接受系统事件的内部 Mode，通常用不到
- NSRunLoopCommonModes: 这是一个占位用的Mode，不是一种真正的Mode。主线程的 RunLoop 里有两个预置的 Mode：kCFRunLoopDefaultMode 和 UITrackingRunLoopMode。
根据Mr Peak的文章，`GSEventReceiveRunLoopMode`和`UIInitializationRunLoopMode`，属于`private mode`，其实还有其他的`private mode`，比如`_kCFStreamBlockingOpenMode`。
mode的切换，目前无法得知系统是如何进行切换的。从源码中发现的事实，切换的时候，会加锁保证多线程安全，并且有三处切换：
- sleep 之前，runloop 可能一觉醒来，发现 mode 已经物是人非。
- doMainQueue 之前，执行完 GCD main queue 中的任务后，mode 也能会发生变化。
- 在 CFRunLoopRunSpecific 函数，也就是 runloop exit 之后。
### Source的种类
###### 官方文档的分类
- Port-Base Sources
- Custom Input Sources
- Cocoa Perform Selector Sources
###### 按照函数调用栈，Source的分类：
- 
Source0
- 非基于Port的。只包含了一个回调（函数指针），它并不能主动触发事件。使用时，你需要先调用 CFRunLoopSourceSignal(source)，将这个 Source 标记为待处理，然后手动调用 CFRunLoopWakeUp(runloop) 来唤醒 RunLoop，让其处理这个事件。
- 触摸事件处理，比如Touch事件
- performSelector:onThread:
- 
Source1
- 基于Port的，通过内核和其他线程通信，接收、分发系统事件。这种 Source 能主动唤醒 RunLoop 的线程。后面讲到的创建常驻线程就是在线程中添加一个NSport来实现的。
- 捕捉系统事件
### Timer
Timer是在哪个Mode下的问题
- CFRunLoopTimerRef是基于时间的触发器
- CFRunLoopTimerRef基本上说的就是NSTimer，它受RunLoop的Mode影响
- GCD的定时器不受RunLoop的Mode影响
- 当其加入到 RunLoop 时，RunLoop会注册对应的时间点，当时间点到时，RunLoop会被唤醒以执行那个回调
- performSelector:withObject:afterDelay:
### Observer
向外部报告Runloop当前状态的更改。每个 Observer 都包含了一个回调（函数指针），当 RunLoop 的状态发生变化时，观察者就能通过回调接受到这个变化。可以观测的时间点有以下几个
```
typedef CF_OPTIONS(CFOptionFlags, CFRunLoopActivity) {
    kCFRunLoopEntry = (1UL << 0),          //即将进入Loop
    kCFRunLoopBeforeTimers = (1UL << 1),     //即将处理Timer
    kCFRunLoopBeforeSources = (1UL << 2),     //即将处理source
    kCFRunLoopBeforeWaiting = (1UL << 5),     //即将进入休眠
    kCFRunLoopAfterWaiting = (1UL << 6),     //从休眠中唤醒
    kCFRunLoopExit = (1UL << 7),
    kCFRunLoopAllActivities = 0x0FFFFFFFU
};
```
- 用于监听RunLoop的状态
- UI刷新（BeforeWaiting）
- Autorelease pool（BeforeWaiting）
#### 处理流程
- 线程进入休眠，是通过从用户态切换到内核态，由内核进行休眠；当有消息时，内核就把线程唤醒 。唤醒只有三种：
- Timer
- dispatch 到Main Queue
- Source1
- 
GCD开启线程相关的事情，是不依赖于Runloop；只有回到主线程依赖
![143264-cde66880ee680de3.png](https://upload-images.jianshu.io/upload_images/143264-cde66880ee680de3.png)
Runloop流程.png
# 系统应用
### 1. AutoreleasePool
App启动后，苹果在主线程 RunLoop 里注册了两个 Observer，其回调都是 _wrapRunLoopWithAutoreleasePoolHandler()。
- kCFRunLoopEntry，进入runloop之前，创建一个自动释放池
第二个Observer做的事情
- kCFRunLoopBeforeWaiting， 休眠之前，销毁自动释放池，创建一个新的自动释放池
- kCFRunLoopExit，退出runloop之前，销毁自动释放池
### 2. 事件响应
苹果注册了一个 Source1 (基于 mach port 的) 用来接收系统事件，其回调函数为 __IOHIDEventSystemClientQueueCallback()。
当一个硬件事件(触摸/锁屏/摇晃等)发生后，首先由 IOKit.framework 生成一个 IOHIDEvent 事件并由 SpringBoard 接收。这个过程的详细情况可以参考这里。SpringBoard 只接收按键(锁屏/静音等)，触摸，加速，接近传感器等几种 Event，随后用 mach port 转发给需要的App进程。随后苹果注册的那个 Source1 就会触发回调，并调用 _UIApplicationHandleEventQueue() 进行应用内部的分发。
_UIApplicationHandleEventQueue() 会把 IOHIDEvent 处理并包装成 UIEvent 进行处理或分发，其中包括识别 UIGesture/处理屏幕旋转/发送给 UIWindow 等。通常事件比如 UIButton 点击、touchesBegin/Move/End/Cancel 事件都是在这个回调中完成的。
### 3. 手势识别
当上面的 _UIApplicationHandleEventQueue() 识别了一个手势时，其首先会调用 Cancel 将当前的 touchesBegin/Move/End 系列回调打断。随后系统将对应的 UIGestureRecognizer 标记为待处理。
苹果注册了一个 Observer 监测 BeforeWaiting (Loop即将进入休眠) 事件，这个Observer的回调函数是 _UIGestureRecognizerUpdateObserver()，其内部会获取所有刚被标记为待处理的 GestureRecognizer，并执行GestureRecognizer的回调。
当有 UIGestureRecognizer 的变化(创建/销毁/状态改变)时，这个回调都会进行相应处理。
### 4. 界面更新
当在操作 UI 时，比如改变了 Frame、更新了 UIView/CALayer 的层次时，或者手动调用了 UIView/CALayer 的 setNeedsLayout/setNeedsDisplay方法后，这个 UIView/CALayer 就被标记为待处理，并被提交到一个全局的容器去。
苹果注册了一个 Observer 监听 BeforeWaiting(即将进入休眠) 和 Exit (即将退出Loop) 事件，回调去执行一个很长的函数：
_ZN2CA11Transaction17observer_callbackEP19__CFRunLoopObservermPv()。这个函数里会遍历所有待处理的 UIView/CAlayer 以执行实际的绘制和调整，并更新 UI 界面。
### 5. 定时器
NSTimer 其实就是 CFRunLoopTimerRef，他们之间是 toll-free bridged 的。一个 NSTimer 注册到 RunLoop 后，RunLoop 会为其重复的时间点注册好事件。例如 10:00, 10:10, 10:20 这几个时间点。RunLoop为了节省资源，并不会在非常准确的时间点回调这个Timer。Timer 有个属性叫做 Tolerance (宽容度)，标示了当时间点到后，容许有多少最大误差。
```
// NSDefaultRunLoopMode、UITrackingRunLoopMode才是真正存在的模式
    // NSRunLoopCommonModes并不是一个真的模式，它只是一个标记
    // timer能在_commonModes数组中存放的模式下工作
    [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];
```
如果某个时间点被错过了，例如执行了一个很长的任务，则那个时间点的回调也会跳过去，不会延后执行。就比如等公交，如果 10:10 时我忙着玩手机错过了那个点的公交，那我只能等 10:20 这一趟了。
CADisplayLink 是一个和屏幕刷新率一致的定时器（但实际实现原理更复杂，和 NSTimer 并不一样，其内部实际是操作了一个 Source）。如果在两次屏幕刷新之间执行了一个长任务，那其中就会有一帧被跳过去（和 NSTimer 相似），造成界面卡顿的感觉。在快速滑动TableView时，即使一帧的卡顿也会让用户有所察觉。Facebook 开源的 AsyncDisplayLink 就是为了解决界面卡顿的问题，其内部也用到了 RunLoop
### 6. PerformSelecter
当调用 NSObject 的 performSelecter:afterDelay: 后，实际上其内部会创建一个 Timer 并添加到当前线程的 RunLoop 中。所以如果当前线程没有 RunLoop，则这个方法会失效。`run`方法在子线程执行，必须打开`runloop`才能成功执行`test`方法
```
- (void)run {
    NSLog(@"current thread: %@", [NSThread currentThread]);
    
    [self performSelector:@selector(test) withObject:nil afterDelay:0];
    
    [[NSRunLoop currentRunLoop] run];
}
```
当调用 performSelector:onThread: 时，实际上其会创建一个 Timer 加到对应的线程去，同样的，如果对应线程没有 RunLoop 该方法也会失效。
- 如果是`waitUntilDone:NO`才需要Runloop；如果为YES，`performSelector`会阻塞地执行`test`方法，就不需要Runloop了
```
[self performSelector:@selector(test) onThread:[NSThread currentThread] withObject:nil waitUntilDone:NO];
[[NSRunLoop currentRunLoop] run];
```
### 7. 关于GCD
实际上 RunLoop 底层也会用到 GCD 的东西，~~比如 RunLoop 是用 dispatch_source_t 实现的 Timer~~（评论中有人提醒，NSTimer 是用了 XNU 内核的 mk_timer，我也仔细调试了一下，发现 NSTimer 确实是由 mk_timer 驱动，而非 GCD 驱动的）。但同时 GCD 提供的某些接口也用到了 RunLoop， 例如 dispatch_async()。
当调用 dispatch_async(dispatch_get_main_queue(), block) 时，libDispatch 会向主线程的 RunLoop 发送消息，RunLoop会被唤醒，并从消息中取得这个 block，并在回调 **CFRUNLOOP_IS_SERVICING_THE_MAIN_DISPATCH_QUEUE**() 里执行这个 block。但这个逻辑仅限于 dispatch 到主线程，dispatch 到其他线程仍然是由 libDispatch 处理的。
### 8. 关于网络请求
iOS 中，关于网络请求的接口自下至上有如下几层:
1 CFSocket 是最底层的接口，只负责 socket 通信。
2 CFNetwork 是基于 CFSocket 等接口的上层封装，ASIHttpRequest 工作于这一层。
3 NSURLConnection 是基于 CFNetwork 的更高层的封装，提供面向对象的接口，AFNetworking 工作于这一层。
4 NSURLSession 是 iOS7 中新增的接口，表面上是和 NSURLConnection 并列的，但底层仍然用到了 NSURLConnection 的部分功能 (比如 com.apple.NSURLConnectionLoader 线程)，AFNetworking2 和 Alamofire 工作于这一层。
下面主要介绍下 NSURLConnection 的工作过程。
通常使用 NSURLConnection 时，你会传入一个 Delegate，当调用了 [connection start] 后，这个 Delegate 就会不停收到事件回调。实际上，start 这个函数的内部会会获取 CurrentRunLoop，然后在其中的 DefaultMode 添加了4个 Source0 (即需要手动触发的Source)。CFMultiplexerSource 是负责各种 Delegate 回调的，CFHTTPCookieStorage 是处理各种 Cookie 的。
当开始网络传输时，我们可以看到 NSURLConnection 创建了两个新线程：com.apple.NSURLConnectionLoader 和 com.apple.CFSocket.private。其中 CFSocket 线程是处理底层 socket 连接的。NSURLConnectionLoader 这个线程内部会使用 RunLoop 来接收底层 socket 的事件，并通过之前添加的 Source0 通知到上层的 Delegate。
# 实践
### 1. 滚动scrollview导致定时器失效
在界面上有一个UIscrollview控件（tableview，collectionview等），如果此时还有一个定时器在执行一个事件，你会发现当你滚动scrollview的时候，定时器会失效。
解决方案
- 把定时器的runloop的model改为NSRunLoopCommonModes 模式，这个模式是一种占位mode，并不是真正可以运行的mode，它是用来标记一个mode的。默认情况下default和tracking这两种mode 都会被标记上NSRunLoopCommonModes 标签。
- 使用GCD创建定时器，GCD创建的定时器不会受runloop的影响
### 2. 常驻线程
需要创建一个在后台一直存在的程序，来做一些需要频繁处理的任务。比如检测网络状态，AFNetworking。
默认情况一个线程创建出来，运行完要做的事情，线程就会消亡。而程序启动的时候，就创建的主线程已经加入到runloop，所以主线程不会消亡。
这个时候我们就需要把自己创建的线程加到runloop中来，就可以实现线程常驻后台。
解决方案
- 添加NSPort。
- 添加NSTimer
需要注意的是：
- 如果需要停止Runloop，不能用`run`方法启动，这个是无限循环，不能暂停，可以通过以下代码中的方式暂停
- 一定要使用判断`weakSelf`是否为空，因为会在`weakSelf`的`dealloc`去stop的话，runLoop停止之后，又会去判断while条件，这个时候`weakSelf`为nil的话，如果不判断nil，`!weakSelf.isStoped`就是YES，那Runloop就停不了了
- 在`dealloc`方法，暂停子线程，一定要保证在子线程的暂停方法，必须阻塞地执行完毕，即`waitUntilDone:YES]`；要是`waitUntilDone:NO]`，self已经销毁了，子线程还有访问self的方法，那就`EXC_BAD_ACCESS`
```
[[NSRunLoop currentRunLoop] addPort:[[NSPort alloc] init] forMode:NSDefaultRunLoopMode];
    
 while (weakSelf && !weakSelf.isStoped) {
            [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
        }
```
```
// 用于停止子线程的RunLoop
- (IBAction)stop {
    if (!self.thread) return;
    
    // 在子线程调用stop（waitUntilDone设置为YES，代表子线程的代码执行完毕后，这个方法才会往下走）
    [self performSelector:@selector(stopThread) onThread:self.thread withObject:nil waitUntilDone:YES];
}
- (void)stopThread
{
    // 设置标记为YES
    self.stopped = YES;
    
    // 停止RunLoop
    CFRunLoopStop(CFRunLoopGetCurrent());
    NSLog(@"%s %@", __func__, [NSThread currentThread]);
    
    // 清空线程
    self.thread = nil;
}
- (void)dealloc
{
    NSLog(@"%s", __func__);
    
    [self stop];
}
```
### 3. 在所有UI相应操作之前处理任务
比如我们点击了一个按钮，在ui关联的事件开始执行之前，我们需要执行一些其他任务，可以在observer中实现。AsyncDisplayKit也是这种思路处理的
# 面试题
#### 讲讲 RunLoop，项目中有用到吗？
- 什么是Runloop
Runloop就像他的名字一样，是线程中的循环。它用来接收循环中的各种事件和安排线程工作，并且在没有工作时，不让线程退出，而是进入睡眠。
- Runloop和线程之间的关系
- 介绍mode，mode中的Source0，Source1，Timer，Observers
- 从源码入手，介绍整个流程
- 应用
- 线程保活
- Timer
- 在定时器、GCD、performSelector、事件响应、手势识别、界面刷新、网络请求、AutoreleasePool等等很多地方用
#### runloop内部实现逻辑？
#### runloop和线程的关系？
每条线程都有唯一的一个与之对应的RunLoop对象
RunLoop保存在一个全局的Dictionary里，线程作为key，RunLoop作为value
线程刚创建时并没有RunLoop对象，RunLoop会在第一次获取它时创建
RunLoop会在线程结束时销毁
主线程的RunLoop已经自动获取（创建），子线程默认没有开启RunLoop
#### timer 与 runloop 的关系？
Runloop里面有多种mode，每种mode包含多个Timer，Source，Observer，一般timer属于common mode，它是一种标记，包含default和Tracking两种mode。
#### 程序中添加每3秒响应一次的NSTimer，当拖动tableview时timer可能无法响应要怎么解决？
将timer加入到commonModes
#### runloop 是怎么响应用户操作的， 具体流程是什么样的？
Source1捕捉用户操作，然后把这个事件包装成事件队列EventQueue，然后放到source0中处理
#### 说说runLoop的几种状态
```
typedef CF_OPTIONS(CFOptionFlags, CFRunLoopActivity) {
    kCFRunLoopEntry = (1UL << 0),          //即将进入Loop
    kCFRunLoopBeforeTimers = (1UL << 1),     //即将处理Timer
    kCFRunLoopBeforeSources = (1UL << 2),     //即将处理source
    kCFRunLoopBeforeWaiting = (1UL << 5),     //即将进入休眠
    kCFRunLoopAfterWaiting = (1UL << 6),     //从休眠中唤醒
    kCFRunLoopExit = (1UL << 7),
    kCFRunLoopAllActivities = 0x0FFFFFFFU
};
```
#### runloop的mode作用是什么？
RunLoop启动时只能选择其中一个Mode，作为currentMode
隔离不同的Source，Timer，和Observer，互不影响，在该模式下工作就会很流畅。
# 参考文章
[http://blog.ibireme.com/2015/05/18/runloop/](http://blog.ibireme.com/2015/05/18/runloop/)
[http://www.jianshu.com/p/3105b7f9fb8c](https://www.jianshu.com/p/3105b7f9fb8c)
Mr Peak:[http://mrpeak.cn/blog/ios-runloop/](http://mrpeak.cn/blog/ios-runloop/)
#### 面试题
[https://www.jianshu.com/p/dd07f431a8c7](https://www.jianshu.com/p/dd07f431a8c7)
[https://www.jianshu.com/p/de752066d0ad](https://www.jianshu.com/p/de752066d0ad)
