# iOS RunLoop学习笔记 - weixin_33985507的博客 - CSDN博客
2017年03月06日 22:17:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
# 1.什么是RunLoop？
- 从字面意思来看，runloop就是一个运行循环，不断的在跑圈。
# 2.RunLoop的作用？
- RunLoop的基本作用：保持程序的持续运行。
- 处理App中的各种事件(触摸、定时器、Selector等)。
- 节省cpu资源，提高程序性能。该做事的时候做事，该休息的时候休息。
# 3有和没有RunLoop会怎么样？
### 有RunLoop
- 原来在main.m的函数是这样的，UIApplicationMain函数内部就启动了一个RunLoop，所以UIApplicationMain函数一直没有返回，保持了程序的持续运行
- 这个默认启动的RunLoop是跟主线程相关联的
- 有RunLoop的情况下,由于main函数里面启动了个RunLoop，所以程序并不会马上退出，保持持续运行状态
```
int main(int argc, char * argv[]) {
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
```
### 没有RunLoop
- 如果去掉Runloop，那么在main.m中的主函数相当于变成了：
```
int main(int argc, char * argv[]) {
    NSLog(@"execute main function");
    return 0;
}
```
- 以上程序往下走了以后，第3行后程序就结束了；
# 4.RunLoop对象
- 
iOS中有2套API来访问和使用RunLoop
Foundation
NSRunLoop
Core Foundation
CFRunLoopRef
- 
NSRunLoop和CFRunLoopRef都代表着RunLoop对象
- 
NSRunLoop是基于CFRunLoopRef的一层OC包装，所以要了解RunLoop内部结构，需要多研究CFRunLoopRef层面的API（Core Foundation层面）
# 5.关于RunLoop的一些资料
- 
[苹果官方文档](https://link.jianshu.com?t=https://developer.apple.com/library/mac/documentation/Cocoa/Conceptual/Multithreading/RunLoopManagement/RunLoopManagement.html)
- 
[CFRunLoopRef](https://link.jianshu.com?t=http://opensource.apple.com/source/CF/CF-1151.16/)
# 6.RunLoop与线程
- 
每条线程都有唯一的一个与之对应的RunLoop对象
- 
主线程的RunLoop已经自动创建好了，子线程的RunLoop需要主动创建
- 
RunLoop在第一次获取时创建，在线程结束时销毁
### 获得RunLoop对象
- 
Foundation
`[NSRunLoop currentRunLoop]; // 获得当前线程的RunLoop对象`
`[NSRunLoop mainRunLoop]; // 获得主线程的RunLoop对象`
- 
Core Foundation
`CFRunLoopGetCurrent(); // 获得当前线程的RunLoop对象`
`CFRunLoopGetMain(); // 获得主线程的RunLoop对象`
# 7.RunLoop相关类
- Core Foundation中关于RunLoop的5个类
- CFRunLoopRef
- CFRunLoopModeRef
- CFRunLoopSourceRef
- CFRunLoopTimerRef
- CFRunLoopObserverRef
#### CFRunLoopModeRef
- CFRunLoopModeRef代表RunLoop的运行模式
- 一个 RunLoop 包含若干个 Mode，每个Mode又包含若干个Source/Timer/Observer
- 每次RunLoop启动时，只能指定其中一个 Mode，这个Mode被称作 CurrentMode
- 如果需要切换Mode，只能退出Loop，再重新指定一个Mode进入
- 这样做主要是为了分隔开不同组的Source/Timer/Observer，让其互不影响
###### Run Loop Modes 系统默认注册了5个Mode:
- 
kCFRunLoopDefaultMode：App的默认Mode，通常主线程是在这个Mode下运行
- 
UITrackingRunLoopMode：界面跟踪 Mode，用于 ScrollView 追踪触摸滑动，保证界面滑动时不受其他 Mode 影响
- 
UIInitializationRunLoopMode: 在刚启动 App 时第进入的第一个 Mode，启动完成后就不再使用
- 
GSEventReceiveRunLoopMode: 接受系统事件的内部 Mode，通常用不到
- 
kCFRunLoopCommonModes: 这是一个占位用的Mode，不是一种真正的Mode
#### CFRunLoopSourceRef
- 
CFRunLoopSourceRef是事件源（输入源）
- 
以前的分法
Port-Based Sources
Custom Input Sources
Cocoa Perform Selector Sources
- 
现在的分法
Source0：非基于Port的
Source1：基于Port的
#### CFRunLoopTimerRef
- 
CFRunLoopTimerRef是基于时间的触发器
- 
基本上说的就是NSTimer
#### CFRunLoopObserverRef
- 
CFRunLoopObserverRef是观察者，能够监听RunLoop的状态改变
- 
可以监听的时间点有以下几个
![2763148-80b947cb403bf518.png](https://upload-images.jianshu.io/upload_images/2763148-80b947cb403bf518.png)
observer.png
# 8.RunLoop处理逻辑-官方版
![2763148-77c09f4e45dea841.png](https://upload-images.jianshu.io/upload_images/2763148-77c09f4e45dea841.png)
RunLoop_official.png
![2763148-5d09e5a3b8010fff.png](https://upload-images.jianshu.io/upload_images/2763148-5d09e5a3b8010fff.png)
RunLoop_official2.png
# 9.RunLoop处理逻辑-网友整理版
![2763148-082ecc6028d98840.png](https://upload-images.jianshu.io/upload_images/2763148-082ecc6028d98840.png)
RunLoop_Nets.png
# 10.RunLoop开发中的使用
- 
当实例化一个时钟timer的时候，添加到运行循环中。
注意:一定需要销毁该时钟 否则会产生循环应用
- 
socket开发中 使用RunLoop 能够监听网络端口数据的接受与发送的情况
学习笔记暂时记录到这里 后续继续记录 :)
