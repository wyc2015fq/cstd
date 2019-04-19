# RunLoop知识点总结 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年02月28日 19:42:50[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：412
本文来自简书，原文地址:[http://www.jianshu.com/p/108c6255962c](http://www.jianshu.com/p/108c6255962c)
RunLoop是多线程的难点. 在实际开发中我们如何使用RunLoop呢? 且容我一一道来, 不当之处, 敬请斧正.
##### 先浏览一下RunLoop知识点的大致框架, 这也是本文即将要说明的:
![](http://upload-images.jianshu.io/upload_images/2868984-9713d58f368646ec.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
RunLoop知识点的大致框架
### RunLoop的概念和作用
RunLoop被称为运行循环, 你可以把RunLoop理解为一个死循环, 看一下CFRunLoop的源码就知道了:
```
void CFRunLoopRun(void) {    /* DOES CALLOUT */
    int32_t result;
    do {
        result = CFRunLoopRunSpecific(CFRunLoopGetCurrent(), kCFRunLoopDefaultMode, 1.0e10, false);
        CHECK_FOR_FORK();
    } while (kCFRunLoopRunStopped != result && kCFRunLoopRunFinished != result);
}
```
它的基本作用是:
- 保持程序的持续运行(保证程序不退出)
- 处理App中的各种事件(触摸事件, 定时器事件, Selector事件)
##### 但这个死循环是一个很特殊的死循环, 它能够在该做事情的时候做事情, 没事情做的时候休息待命, 以节省CPU资源, 提高程序性能.
在应用程序的入口main函数中, 如下所示
```
int main(int argc, char * argv[]) {
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
```
##### 有一个UIApplicationMain(,,_,_)函数, 这个函数内部就启动了一个RunLoop运行循环
UIApplicationMain(,,_,_)函数是有一个int类型的返回值的, 但是一直不会返回,保持程序的持续运行, 这个默认启动的RunLoop运行循环是跟主线程相关联的.
### 获取RunLoop对象
苹果官方文档告诉我们:
> 
Both Cocoa and Core Foundation provide run loop objects to help you configure and manage your thread’s run loop
- 
Cocoa框架: `NSRunLoop`
- 
Core Foundation框架:`CFRunLoopRef`
NSRunLoop的底层其实还是调用的CFRunLoopRef的API, 是对其进行了OC层面上的封装
##### 线程和RunLoop的关系
- 一条线程对应一个RunLoop,是一一对应的关系
- 主线程的RunLoop是默认存在的,已经创建好了, 子线程的RunLoop需要手动创建
- RunLoop的生命周期: 在第一次获取得时候创建, 线程结束时销毁
##### 获取RunLoop对象
- Cocoa框架: 
```
[NSRunLoop
 currentRunLoop]
```
(获得当前线程的RunLoop对象)
`[NSRunLoop mainRunLoop]`(获得主线程的RunLoop对象)
- Core Foundation框架:`CFRunLoopGetCurrent()`(获得当前线程的RunLoop对象)
`CFRunLoopGetMain()`(获得主线程的RunLoop对象)
##### 我们来看一下CFRunLoop的源码:
```
CF_EXPORT CFRunLoopRef _CFRunLoopGet0(pthread_t t) {
    if (pthread_equal(t, kNilPthreadT)) {
    t = pthread_main_thread_np();
    }
    __CFLock(&loopsLock);
    if (!__CFRunLoops) {
        __CFUnlock(&loopsLock);
    CFMutableDictionaryRef dict = CFDictionaryCreateMutable(kCFAllocatorSystemDefault, 0, NULL, &kCFTypeDictionaryValueCallBacks);
    CFRunLoopRef mainLoop = __CFRunLoopCreate(pthread_main_thread_np());
    CFDictionarySetValue(dict, pthreadPointer(pthread_main_thread_np()), mainLoop);
    if (!OSAtomicCompareAndSwapPtrBarrier(NULL, dict, (void * volatile *)&__CFRunLoops)) {
        CFRelease(dict);
    }
    CFRelease(mainLoop);
        __CFLock(&loopsLock);
    }
    CFRunLoopRef loop = (CFRunLoopRef)CFDictionaryGetValue(__CFRunLoops, pthreadPointer(t));
    __CFUnlock(&loopsLock);
    if (!loop) {
    CFRunLoopRef newLoop = __CFRunLoopCreate(t);
        __CFLock(&loopsLock);
    loop = (CFRunLoopRef)CFDictionaryGetValue(__CFRunLoops, pthreadPointer(t));
    if (!loop) {
        CFDictionarySetValue(__CFRunLoops, pthreadPointer(t), newLoop);
        loop = newLoop;
    }
        // don't release run loops inside the loopsLock, because CFRunLoopDeallocate may end up taking it
        __CFUnlock(&loopsLock);
    CFRelease(newLoop);
    }
    if (pthread_equal(t, pthread_self())) {
        _CFSetTSD(__CFTSDKeyRunLoop, (void *)loop, NULL);
        if (0 == _CFGetTSD(__CFTSDKeyRunLoopCntr)) {
            _CFSetTSD(__CFTSDKeyRunLoopCntr, (void *)(PTHREAD_DESTRUCTOR_ITERATIONS-1), (void (*)(void *))__CFFinalizeRunLoop);
        }
    }
    return loop;
}
```
上面的代码告诉我们: 线程和RunLoop是一一对应的关系, 当线程是主线程时,会创建一个mainRunLoop,然后保存在字典中:
```
CFRunLoopRef mainLoop = __CFRunLoopCreate(pthread_main_thread_np());
CFDictionarySetValue(dict, pthreadPointer(pthread_main_thread_np()), mainLoop);
```
当线程是子线程时, 会先看一下当前线程为key的对应的Value中有没有RunLoop,如果没有, 会先创建一个RunLoop, 然后再将其保存在字典中:
```
CFRunLoopRef loop = (CFRunLoopRef)CFDictionaryGetValue(__CFRunLoops, pthreadPointer(t));
    __CFUnlock(&loopsLock);
    if (!loop) {
    CFRunLoopRef newLoop = __CFRunLoopCreate(t);
        __CFLock(&loopsLock);
    loop = (CFRunLoopRef)CFDictionaryGetValue(__CFRunLoops, pthreadPointer(t));
    if (!loop) {
        CFDictionarySetValue(__CFRunLoops, pthreadPointer(t), newLoop);
        loop = newLoop;
    }
```
### RunLoop的相关类
##### Core Foundation中关于RunLoop的关键类
- `CFRunLoopRef`
- `CFRunLoopModeRef`
- `CFRunLoopSourceRef`
- `CFRunLoopTimerRef`
- `CFRunLoopObserverRef`
##### 这5个类之间的关系可以用下图来表示:
![](http://upload-images.jianshu.io/upload_images/2868984-25bb04eeef22d649.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
RunLoop的相关类之间的关系
这个关系图其实就是说: 在一个RunLoop中可以存在两个或者两个以上的模式,但RunLoop每次只能选择一个模式运行, 就像空调有制冷和制热模式, 但是运行的时候只能选择制冷或者制热模式, 要保证运行循环RunLoop不退出, 每个模式里面至少存在一个Source或者 一个Timer,Observer可以有也可以没有, 只是监听RunLoop的运行状态.
##### RunLoop运行模式(一共有5种)
- Default
`NSDefaultRunLoopMode` (Cocoa)
`kCFRunLoopDefaultMode` (Core Foundation)
- Event tracking
`NSEventTrackingRunLoopMode` (Cocoa)
- Common modes
`NSRunLoopCommonModes` (Cocoa)
`kCFRunLoopCommonModes` (Core Foundation)
- Connection
`NSConnectionReplyMode` (Cocoa)
- Modal
`NSModalPanelRunLoopMode` (Cocoa)
后面两种运行模式就不做过多解读了, 我们需要关注的是前面三种模式
- 默认模式
```
- (void)timer1 {
    NSTimer *timer = [NSTimer timerWithTimeInterval:1 repeats:YES block:^(NSTimer * _Nonnull timer) {
        NSLog(@"run1----------%@",[NSRunLoop currentRunLoop].currentMode);
    }];
    [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
}
```
当定时器以默认模式被添加到运行循环中时, 如果发生例如scrollView拖拽事件等, 运行循环将有默认模式自动切换到事件追踪模式, 这时候, 默认模式中的定时器就暂时不工作了, 当拖拽事件结束之后, 运行循环中的运行模式又由追踪模式切换到默认模式.
- 事件追踪模式
```
- (void)timer2 {
    NSTimer *timer = [NSTimer timerWithTimeInterval:1 repeats:YES block:^(NSTimer * _Nonnull timer) {
        NSLog(@"run2----------%@",[NSRunLoop currentRunLoop].currentMode);
    }];
    [[NSRunLoop currentRunLoop] addTimer:timer forMode:UITrackingRunLoopMode];
}
```
跟上面的模式刚好相反, 如果发生例如scrollView拖拽事件等, 事件追踪模式中的定时器就开始工作, 当拖拽事件结束之后, 运行循环中的运行模式又由事件追踪模式切换到默认模式, 那么在事件追踪模式中的定时器就不工作了.
那么问题来了, 如果需要定时器不管添加到哪种模式下都正常工作应该怎么做呢? 有一种方法是, 同时把定时器添加到两种运行模式中, 不过这种方法太笨了, 正确的做法是:
- 通用模式
```
- (void)timer3 {
    NSTimer *timer = [NSTimer timerWithTimeInterval:1 repeats:YES block:^(NSTimer * _Nonnull timer) {
        NSLog(@"run3----------%@",[NSRunLoop currentRunLoop].currentMode);
    }];
    [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];
}
```
这种模式其实不是真正的一种运行模式, 而是将两种模式都打上CommonModes的标签,那么就相当于timer可以在只要是带有CommonModes标签的运行模式下工作.
```
+ (NSTimer *)scheduledTimerWithTimeInterval:(NSTimeInterval)interval
 repeats:(BOOL)repeats block:(void (^)(NSTimer *timer))block
```
类似于这种scheduledTimer开头的方法, 是自动将定时器添加到默认的运行循环模式了, 如果要添加到事件追踪模式和通用模式, 还是需要手动添加.
如果我在子线程创建了定时器, 那么需要创建子线程中的runLoop对象, 然后把timer添加到子线程的runLoop中:
```
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    [NSThread detachNewThreadWithBlock:^{
        NSTimer *timer = [NSTimer timerWithTimeInterval:1 repeats:YES block:^(NSTimer * _Nonnull timer) {
            NSLog(@"run2----------%@",[NSRunLoop currentRunLoop].currentMode);
        }];
        NSLog(@"%@",[NSThread currentThread]);
        NSRunLoop *currentRunLoop = [NSRunLoop currentRunLoop];
        [currentRunLoop addTimer:timer forMode:NSRunLoopCommonModes];
        [currentRunLoop run];
    }];
}
```
需要记住的是:
- 创建子线程的RunLoop直接调用
```
[NSRunLoop
 currentRunLoop];
```
, 这个方法是懒加载的
- 一定要让子线程的runLoop跑起来, 不然的话, 子线程一结束, 运行循环立马销毁, 即时添加了定时器也没个X用. 
```
[currentRunLoop
 run];
```
这句代码不能少!
### GCD定时器
插播另外一条知识点, 我们在使用timer的时候, 除了NSTimer, 还有其他的选择吗? 答案是有, GCD定时器就是一个很好的选择, xcode甚至为我们提供了默认的定时器代码块, 方便你的使用:
```
- (void)gcdTimer {
    /* 
     参数1 : 需要创建的源的种类, timer 也是一种数据源
     参数2,参数3: 在你创建timer的时候直接传0就可以了
     参数4: timer触发的代码运行的队列
     */
    dispatch_source_t timer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0));
    /* 
     参数1 : timer定时器
     参数2 : 从什么时间开始触发定时器, DISPATCH_TIME_NOW代表现在
     参数3 : 时间间隔
     参数4 : 表示精度, 传0表示绝对精准
     */
    dispatch_source_set_timer(timer, DISPATCH_TIME_NOW, 2.0 * NSEC_PER_SEC, 0 * NSEC_PER_SEC);
    /* 
     封装timer需要触发的操作
     */
    dispatch_source_set_event_handler(timer, ^{
        NSLog(@"GCDTimer-----%@",[NSThread currentThread]);
        NSLog(@"%@",[NSRunLoop currentRunLoop].currentMode);
    });
    dispatch_resume(timer);
    /* 
     用强指针引用, 防止timer释放
     */
    self.timer = timer;
}
```
### RunLoop输入源
`CFRunLoopSourceRef`是输入源,
 输入源的种类有两种分类方法:
- 
以前的分法:
1.Port-Based Sources
2.Custom Input Sources
3.Cocoa Perform Selector Sources
- 
现在的分法:
1.Source0:非基于端口的
2.Source1:基于端口的
那么根据现在的分法, 什么是基于端口的呢? 就是系统默认的, 非基于端口的就是用户主动触发的事件, 比如用户点击了一个按钮等. 假设现在用户点击了一个按钮, 我们来观察函数调用栈的情况:
![](http://upload-images.jianshu.io/upload_images/2868984-0e0a727600ab564f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
点击按钮时的函数调用栈
我们可以看出, 点击按钮时, 运行循环RunLoop处理的输入源是source0
### RunLoop Observer
`CFRunLoopObserverRef`是观察者, 是用来监听RunLoop状态改变的
> 
typedef CF_OPTIONS(CFOptionFlags, CFRunLoopActivity) {
kCFRunLoopEntry = (1UL << 0), 即将进入运行循环
kCFRunLoopBeforeTimers = (1UL << 1), 即将处理定时器事件
kCFRunLoopBeforeSources = (1UL << 2), 即将处理输入源事件
kCFRunLoopBeforeWaiting = (1UL << 5), 即将进入休眠
kCFRunLoopAfterWaiting = (1UL << 6), 刚从休眠中唤醒
kCFRunLoopExit = (1UL << 7), 退出运行循环
kCFRunLoopAllActivities = 0x0FFFFFFFU 运行循环所有活动
};
添加观察者到运行循环的代码:
```
- (void)observer {
    /* 
     参数1 :分配内存空间的方式, 传默认
     参数2 :RunLoop的运行状态
     参数3 :是否持续观察
     参数4 :优先级, 传0
     参数5 :观察者观测到状态改变时触发的方法
     */
    CFRunLoopObserverRef observer = CFRunLoopObserverCreateWithHandler(CFAllocatorGetDefault(), kCFRunLoopAllActivities, YES, 0, ^(CFRunLoopObserverRef observer, CFRunLoopActivity activity) {
        /* 
         typedef CF_OPTIONS(CFOptionFlags, CFRunLoopActivity) {
         kCFRunLoopEntry = (1UL << 0),
         kCFRunLoopBeforeTimers = (1UL << 1),
         kCFRunLoopBeforeSources = (1UL << 2),
         kCFRunLoopBeforeWaiting = (1UL << 5),
         kCFRunLoopAfterWaiting = (1UL << 6),
         kCFRunLoopExit = (1UL << 7),
         kCFRunLoopAllActivities = 0x0FFFFFFFU
         };
         */
        switch (activity) {
            case kCFRunLoopEntry:
                NSLog(@"即将被唤醒");
                break;
            case kCFRunLoopBeforeTimers:
                NSLog(@"即将处理定时器事件");
                break;
            case kCFRunLoopBeforeSources:
                NSLog(@"即将处理输入源事件");
                break;
            case kCFRunLoopBeforeWaiting:
                NSLog(@"即将进入休眠");
                break;
            case kCFRunLoopAfterWaiting:
                NSLog(@"休眠结束");
                break;
            case kCFRunLoopExit:
                NSLog(@"运行循环退出");
                break;
            default:
                break;
        }
    });
    /* 
     参数1 :运行循环, 传入当前的运行循环
     参数2 :观察者, 观察运行循环的各种状态
     参数3 :运行循环的模式
     */
    CFRunLoopAddObserver(CFRunLoopGetCurrent(),observer, kCFRunLoopDefaultMode);
}
```
重要的函数就两个
- 
`CFRunLoopObserverCreateWithHandler(_,_,_,_,_)`创建观察者(包括block回调)
- 
`CFRunLoopAddObserver(_,_,_)`添加观察者到运行循环
### RunLoop运行流程
##### 我们先看一下苹果官方文档的示意图:
![](http://upload-images.jianshu.io/upload_images/2868984-50b230241e4c91ee.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
RunLoop运行流程
从图中可以看出的是: 当线程中的运行循环开启之后, 将循环处理两大source事件:
- Input Sources
- 端口事件
2.用户输入源事件
3.performSelector事件
- Timer Sources
定时器事件
然后运行循环就循环处理这些事件, 如果没有, 就休眠并等待被唤醒
详细的官方文档步骤见下图(感谢网友提供):
![](http://upload-images.jianshu.io/upload_images/2868984-70fae89e2a14b985.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
RunLoop运行流程
### RunLoop的应用:常驻线程
开启常驻线程的作用: 让一个子线程不被销毁, 等待其他线程发来消息, 处理事件
```
#import "ViewController.h"
@interface ViewController ()
/**
 thread
 */
@property (nonatomic,strong)NSThread *thread;
@end
@implementation ViewController
- (IBAction)creatNewThread:(UIButton *)sender {
    self.thread = [[NSThread alloc] initWithTarget:self selector:@selector(task1) object:nil];
    [self.thread start];
}
- (IBAction)stableThread:(UIButton *)sender {
     /* 在子线程中实现方法 */
    [self performSelector:@selector(task2) onThread:self.thread withObject:nil waitUntilDone:YES];
}
- (void)task1 {
    NSLog(@"创建了一个新的线程----%@",[NSThread currentThread]);
    /* 获取当前线程 */
    NSRunLoop *runLoop = [NSRunLoop currentRunLoop];
     /* 添加端口, 保证运行循环不退出 */
    [runLoop addPort:[NSPort port] forMode:NSDefaultRunLoopMode];
    [runLoop run];
}
- (void)task2 {
    NSLog(@"%s",__func__);
}
@end
```
最重要的就是添加端口到当前runLoop中,因为当前的runLoop就是子线程的runLoop, 添加端口的目的就是保证运行循环不退出, 当然你也可以添加timer到运行循环中, 但timer本身起不到任何用, 反而浪费资源
如上图所示, 我先创建了一个子线程, 在子线程里我创建了子线程的runLoop,然后在子线程中添加端口保证其不退出, 然后调用`run`方法让runLoop跑起来,
 这样, 我不管在程序运行的任何阶段在这个子线程中调用方法, 该子线程都可以响应, 这就是常驻线程.
以上就是RunLoop的相关知识点, 再补充一个小的知识点, 即自动释放池的创建和销毁
- 在启动RunLoop的时候, 会第一次创建自动释放池
- RunLoop退出的时候, 会销毁自动释放池
- 其他创建和销毁自动释放池的时机:
- 当RunLoop即将进入睡眠的时候, 说明之前的事件都已经全部处理完毕, 那么之前的变量就没有必要再留着, 这时候会销毁自动释放池
- 同时, 再创建一个新的自动释放池
##### 关于RunLoop的更多知识, 请查看苹果的相关文档
### RunLoop的相关文档
[苹果官方文档](https://developer.apple.com/library/mac/documentation/Cocoa/Conceptual/Multithreading/RunLoopManagement/RunLoopManagement.html)
[CFRunLoop源码](http://opensource.apple.com/source/CF/CF-1151.16/)
