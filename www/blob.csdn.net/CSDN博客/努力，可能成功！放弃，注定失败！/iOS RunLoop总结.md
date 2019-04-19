# iOS RunLoop总结 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月04日 23:52:23[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：286标签：[ios开发																[iOS																[runloop																[runloop总结](https://so.csdn.net/so/search/s.do?q=runloop总结&t=blog)](https://so.csdn.net/so/search/s.do?q=runloop&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS&t=blog)](https://so.csdn.net/so/search/s.do?q=ios开发&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
# 一、简介
- 
什么是RunLoop
- 运行循环
- 基本作用：
- 保持程序的持续运行，如果没有RunLoop，程序执行完main函数就结束了。
- 处理App中的各种事件（比如触摸事件、定时器事件、Selector事件）
- 节省CPU资源，提高程序性能：该做事时做事，该休息时休息
- ......
- 
main函数中的RunLoop
- 在UIApplicationMain函数内部就启动了一个RunLoop，所以UIApplicationMain函数一直没有返回，保持了程序的持续运行
- 这个默认启动的`RunLoop是跟主线程相关联的`
```
#import <UIKit/UIKit.h>
#import "AppDelegate.h"
int main(int argc, char * argv[]) {
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class])); 
    }
}
```
# 二、RunLoop与线程
- 每条线程都有唯一的一个与之对应的RunLoop对象
- 主线程的RunLoop已经自动创建好了，子线程的RunLoop需要主动创建
- RunLoop在第一次获取时创建，在线程结束时销毁
> 
获得RunLoop对象
- 
iOS中有2套API来访问和使用RunLoop
- 1.Foundation : NSRunLoop
- 2.CoreFoundation : CFRunLoopRef
- NSRunLoop和CFRunLoopRef都代表着RunLoop对象
- 
Foundation
```
[NSRunLoop currentRunLoop];// 获得当前线程的RunLoop对象
[NSRunLoop mainRunLoop];// 获得主线程的RunLoop对象
```
- CoreFoundation
```
CFRunLoopGetCurrent();// 获得当前线程的RunLoop对象
CFRunLoopGetMain();// 获得主线程的RunLoop对象
```
# 三、RunLoop相关资料
- 
苹果官方文档
[https://developer.apple.com/library/mac/documentation/Cocoa/Conceptual/Multithreading/RunLoopManagement/RunLoopManagement.html](https://developer.apple.com/library/mac/documentation/Cocoa/Conceptual/Multithreading/RunLoopManagement/RunLoopManagement.html)
- 
CFRunLoopRef是开源的
[http://opensource.apple.com/source/CF/CF-1151.16/](http://opensource.apple.com/source/CF/CF-1151.16/)
# 四、RunLoop相关类
- CoreFoundation中关于RunLoop的5个类
- CFRunLoopRef
- CFRunLoopModeRef
- CFRunLoopSourceRef
- CFRunLoopTimerRef
- CFRunLoopObserverRef
![](http://upload-images.jianshu.io/upload_images/831339-b392a61e568c7bad.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
runLoop相关类.png
> 
CFRunLoopModeRef
- CFRunLoopModeRef代表RunLoop的运行模式
- 一个 RunLoop包含若干个 Mode，每个Mode又包含若干个Source/Timer/Observer
- 每次RunLoop启动时，只能指定其中一个 Mode，这个Mode被称作 CurrentMode
- 如果需要切换Mode，只能退出Loop，再重新指定一个Mode进入
- 
这样做主要是为了分隔开不同组的Source/Timer/Observer，让其互不影响
- 
系统默认注册了5个Mode:
- 
NSDefaultRunLoopMode：App的默认Mode，通常主线程是在这个Mode下运行
- 
UITrackingRunLoopMode：界面跟踪 Mode，用于 ScrollView追踪触摸滑动，保证界面滑动时不受其他Mode 影响
- UIInitializationRunLoopMode: 在刚启动 App 时第进入的第一个 Mode，启动完成后就不再使用
- GSEventReceiveRunLoopMode: 接受系统事件的内部 Mode，通常用不到
- NSRunLoopCommonModes: 这是一个占位用的Mode，不是一种真正的Mode
> 
CFRunLoopSourceRef
- CFRunLoopTimerRef是基于时间的触发器
- CFRunLoopTimerRef基本上说的就是NSTimer，它受RunLoop的Mode影响
- 注意： `GCD的定时器不受RunLoop的Mode影响`
> 
CFRunLoopSourceRef
- CFRunLoopSourceRef是事件源（输入源）
- 
按照官方文档，Source的分类
- Port-BasedSources
- Custom InputSources
- Cocoa PerformSelector Sources
- 
按照函数调用栈，Source的分类
- Source0：非基于Port的, 用于用户主动触发事件
- Source1：基于Port的，通过内核和其他线程相互发送消息
> 
CFRunLoopObserverRef
- 
CFRunLoopObserverRef是观察者， `能够监听RunLoop的状态改变`
- 
可以监听的时间点有以下几个
![](http://upload-images.jianshu.io/upload_images/831339-2a880bf22c00addb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
监听时间点.png
- 添加Observer
```
//创建observer
CFRunLoopObserverRefobserver = CFRunLoopObserverCreateWithHandler(CFAllocatorGetDefault(),kCFRunLoopAllActivities,YES,0,^(CFRunLoopObserverRefobserver, CFRunLoopActivityactivity) {
    NSLog(@"----监听到RunLoop状态发生改变---%zd",activity);
});
//添加观察者：监听RunLoop的状态
CFRunLoopAddObserver(CFRunLoopGetCurrent(),observer, kCFRunLoopDefaultMode);
//释放Observer
CFRelease(observer);
```
