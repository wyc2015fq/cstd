# iOS-Runloop常驻线程／性能优化 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年10月11日 09:09:46[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：13标签：[iOS-Runloop常驻线程／性能优化																[Runloop](https://so.csdn.net/so/search/s.do?q=Runloop&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS-Runloop常驻线程／性能优化&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
![](https://upload-images.jianshu.io/upload_images/1940927-867ff0da6a7d2bdb.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/440/format/webp)
懵逼
## 主要聊聊以下内容
> 
1 Runloop基本概要
2 Runloop与定时器
3 Runloop常驻线程
4 Runloop性能优化
## Runloop基本概要
- Runloop就是一个do…while 循环
```
function loop() {
   initialize();
   do {
       var message = get_next_message();
       process_message(message);
   } while (message != quit);
}
```
- 只有主线程默认打开了Runloop，子线程需要手动打开
- Runloop一共有5个`mode`，其中给我们使用的有三个
```
NSDefaultRunLoopMode  默认模式
UITrackingRunLoopMode scrollView进入拖拽的时候会进入的模式
NSRunLoopCommonModes  占位模式
```
- 内部实现原理（图片网上找的）
![](https://upload-images.jianshu.io/upload_images/1940927-992f6f38e2c08abd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/417/format/webp)
内部实现原理
- Runloop会在一次循环绘制屏幕上所有的点
## Runloop-定时器
不多说，直接上代码，分析问题。
```
NSTimer * timer = [NSTimer timerWithTimeInterval:1.f repeats:YES block:^(NSTimer * _Nonnull timer) {
        NSLog(@"%s",__func__);
    }];
    [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
```
- 小知识：
```
[NSRunLoop currentRunLoop]
[NSRunLoop mainRunLoop]
在主线程中上面获取的Runloop是同一个，在子线程中不同。
[NSRunLoop mainRunLoop] 表示获取主线程的Runloop对象
[NSRunLoop currentRunLoop] 表示获取当前线程的Runloop对象
```
运行图：
![](https://upload-images.jianshu.io/upload_images/1940927-983d608684d03bbe.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/454/format/webp)
mode问题
通过上面的图得知Runloop在scrollView进行拖拽的时候会进入`UITrackingRunLoopMode`模式，退出拖拽的时候则会回到`NSDefaultRunLoopMode`模式，如何解决上面的问题呢？
需要用到`NSRunLoopCommonModes`占位模式，切换模式后，在看图。
```
NSTimer * timer = [NSTimer timerWithTimeInterval:1.f repeats:YES block:^(NSTimer * _Nonnull timer) {
        static int count = 0;
        NSLog(@"%s - %d",__func__,count++);
    }];
    [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];
```
运行图：
![](https://upload-images.jianshu.io/upload_images/1940927-955581180ebd5fc4.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/454/format/webp)
占位模式
## Runloop常驻线程
在项目开发中我们通常把耗时操作方在子线程中运行，我们就接着上面的那个代码中模拟耗时操作。
```
NSTimer * timer = [NSTimer timerWithTimeInterval:1.f repeats:YES block:^(NSTimer * _Nonnull timer) {
        static int count = 0;
        [NSThread sleepForTimeInterval:1];
        //休息一秒钟，模拟耗时操作
        NSLog(@"%s - %d",__func__,count++);
    }];
    [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];
```
- 这个时候我们是没有放在子线程里面进行处理的，通过实验效果得知，会卡顿。
	效果图：
![](https://upload-images.jianshu.io/upload_images/1940927-2f630dcf08e04a17.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/454/format/webp)
模拟耗时操作
然后此时的你，可能就会写出这样子的代码。
```
dispatch_async(dispatch_get_global_queue(0, 0), ^{
        NSTimer * timer = [NSTimer timerWithTimeInterval:1.f repeats:YES block:^(NSTimer * _Nonnull timer) {
            static int count = 0;
            [NSThread sleepForTimeInterval:1];
            //休息一秒钟，模拟耗时操作
            NSLog(@"%s - %d",__func__,count++);
        }];
        [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];
    });
```
然后你就会发现，啥都没有啊，根本没运行……
分析问题：
> 
当进入block的时候，先创建了timer，并且也把timer也把timer加入到runloop中，但是很重要的一点`子线程中Runloop不会自动运行，需要手动运行`，因为这里没有运行Runloop，所以timer就被释放掉了，所以导致了啥都没有。
既然得知原因是没有运行Runloop，那我们自己运行不就可以了吗？
```
dispatch_async(dispatch_get_global_queue(0, 0), ^{
        NSTimer * timer = [NSTimer timerWithTimeInterval:1.f repeats:YES block:^(NSTimer * _Nonnull timer) {
            static int count = 0;
            [NSThread sleepForTimeInterval:1];
            //休息一秒钟，模拟耗时操作
            NSLog(@"%s - %d",__func__,count++);
        }];
        [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];
        
        //子线程需要手动开启Runloop
        [[NSRunLoop currentRunLoop] run];
    });
```
- 小知识
```
[[NSRunLoop currentRunLoop] run]; 
运行runloop
    [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];  
指定runloop在指定模式下，设置开始时间,开启成功会返回YES
    [[NSRunLoop currentRunLoop] runUntilDate:[NSDate distantFuture]]; 
运行runloop直到一个时间
```
运行效果：
![](https://upload-images.jianshu.io/upload_images/1940927-9ae86337b7345e9a.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/454/format/webp)
runloop-timer4.gif
这样子我们就可以实现在子线程处理耗时操作，并且常驻线程了。
## Runloop性能优化
案例：tableView的Cell中有多个ImageView，同时加载大图，导致UI卡顿。
解决思路：使用Runloop每次循环址添加一张图片。
工具：这里我们需要使用到CFRunloop
实现过程：
> 
1、把加载图片等代码保存起来，先不执行 （保存一段代码，block）
2、监听Runloop循环（CFRunloopObserver）
3、每次都从任务数组中取出一个加载图片等代码执行（执行block代码）
### 监听Runloop
```
//添加runloop监听者
- (void)addRunloopObserver{
    
    //    获取 当前的Runloop ref - 指针
    CFRunLoopRef current =  CFRunLoopGetCurrent();
    
    //定义一个RunloopObserver
    CFRunLoopObserverRef defaultModeObserver;
    
    //上下文
    /*
     typedef struct {
        CFIndex version; //版本号 long
        void *  info;    //这里我们要填写对象（self或者传进来的对象）
        const void *(*retain)(const void *info);        //填写&CFRetain
        void    (*release)(const void *info);           //填写&CGFRelease
        CFStringRef (*copyDescription)(const void *info); //NULL
     } CFRunLoopObserverContext;
     */
    CFRunLoopObserverContext context = {
        0,
        (__bridge void *)(self),
        &CFRetain,
        &CFRelease,
        NULL
    };
    
    /*
     1 NULL空指针 nil空对象 这里填写NULL
     2 模式
        kCFRunLoopEntry = (1UL << 0),
        kCFRunLoopBeforeTimers = (1UL << 1),
        kCFRunLoopBeforeSources = (1UL << 2),
        kCFRunLoopBeforeWaiting = (1UL << 5),
        kCFRunLoopAfterWaiting = (1UL << 6),
        kCFRunLoopExit = (1UL << 7),
        kCFRunLoopAllActivities = 0x0FFFFFFFU
     3 是否重复 - YES
     4 nil 或者 NSIntegerMax - 999
     5 回调
     6 上下文
     */
    //    创建观察者
    defaultModeObserver = CFRunLoopObserverCreate(NULL,
                                                  kCFRunLoopBeforeWaiting, YES,
                                                  NSIntegerMax - 999,
                                                  &Callback,
                                                  &context);
    
    //添加当前runloop的观察着
    CFRunLoopAddObserver(current, defaultModeObserver, kCFRunLoopDefaultMode);
    
    //释放
    CFRelease(defaultModeObserver);
}
```
### 我们要实现回调方法
```
static void Callback(CFRunLoopObserverRef observer, CFRunLoopActivity activity, void *info){
    
    //通过info桥接为当前的对象
    PQRunloop * runloop = (__bridge PQRunloop *)info;
    
    //如果没有任务，就直接返回
    if (runloop.tasks.count == 0) {
        return;
    }
    
    BOOL result = NO;
    while (result == NO && runloop.tasks.count) {
        
        //取出任务
        RunloopBlock unit = runloop.tasks.firstObject;
        
        //执行任务
        result = unit();
        
        //删除任务
        [runloop.tasks removeObjectAtIndex:0];
    }
}
因为在C方法中没有办法调用OC对象，所以context中有一个void * info，为的就是解决这个问题，把OC的对象传入到回调方法中。
```
- 通过上面的两个方法我们可以做到监听Runloop循环，以及每次循环需要处理的事情，这个时候我们只需要对外提供一个添加任务的方法，用数组保存起来。
```
//add task 添加任务
- (void)addTask:(RunloopBlock)unit withId:(id)key{
    //添加任务到数组
    [self.tasks addObject:unit];
    [self.taskKeys addObject:key];
    
    //为了保证加载到图片最大数是18所以要删除
    if (self.tasks.count > self.maxQueue) {
        [self.tasks removeObjectAtIndex:0];
        [self.taskKeys removeObjectAtIndex:0];
    }
}
```
- 如果你一切就这样子结束了，那么你就错了，Runloop在执行后如果没有唤醒操作，就会进入睡眠状态，也就是歇菜了，啥都不干了，所以为了能让Runloop能一直跑，我们需要创建一个定时器，但是最好不要在定时器中作操作（1 耗时，2 耗电）
	所以我们还需要：
```
self.timer = [NSTimer scheduledTimerWithTimeInterval:0.001 repeats:YES block:^(NSTimer * _Nonnull timer) { }];
```
- 小知识
```
NULL 空指针
nil  空对象
```
最后送上运行图：
- 优化后
优化后
- 优化前
优化前
### 结论
- 通过上面两个很明显的可以看出，优化后明显的流畅度得到提高，并且在滑动的时候内存会减，而未优化的明显卡顿，内存消耗也不会在滑动的时候得到减少。
# [Demo地址](https://github.com/codepgq/RunloopDemo)
如果对你有一点点帮助，请用star砸死我，也欢迎交流。
![](https://upload-images.jianshu.io/upload_images/1940927-56d74d5f957d6f17.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/560/format/webp)
滑稽
作者：Codepgq
链接：https://www.jianshu.com/p/f3079ea36775
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
