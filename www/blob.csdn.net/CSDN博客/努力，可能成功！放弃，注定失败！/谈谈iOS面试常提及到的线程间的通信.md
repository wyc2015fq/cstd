# 谈谈iOS面试常提及到的线程间的通信 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月01日 09:27:29[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：6089
![](http://upload-images.jianshu.io/upload_images/680076-e0cc3209e1b04ae8.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
程序猿思考问题时的样子
> 
我们看图片只是乐呵一下，程序猿思考问题差不多就是这个样子，
今天同事在线程通信这一块有点疑问，我们下面来分析一下，系统都提供给我们那些，其实我们都知道，但是很少去关注这些API，也正是这些API，来回在APP中去执行各种不同的线程和队列
##### 一、常见的线程间通信 GCD
我们先来看一个系统的例子：
```
//开启一个全局队列的子线程
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
            //1. 开始请求数据
            //...
            // 2. 数据请求完毕
            //我们知道UI的更新必须在主线程操作，所以我们要从子线程回调到主线程
        dispatch_async(dispatch_get_main_queue(), ^{
                //我已经回到主线程更新
        });
    });
```
如上所述，我们下面进行一个测试：
```
//自定义队列，开启新的子线程
    dispatch_queue_t    custom_queue = dispatch_queue_create("concurrentqueue", DISPATCH_QUEUE_CONCURRENT);
    for (int i=0; i<10; i++) {
        dispatch_async(custom_queue, ^{
            NSLog(@"## 并行队列 %d ##",i);
                //数据更新完毕回调主线程 线程之间的通信
            dispatch_async(dispatch_get_main_queue(), ^{
                NSLog(@"### 我在主线程 通信 ##");
            });
        });
    }
```
- 线程中延迟调用某个方法
```
//线程延迟调用 通信
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(5.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        NSLog(@"## 在主线程延迟5秒调用 ##");
    });
```
- 
线程休眠几秒的方法
`sleep(6);` : 这里是休眠6秒
- 
常用的线程通信方法有以下几种：（GCD）
- 
需要更新UI操作的时候使用下面这个GCD的block方法
```
//回到主线程更新UI操作
dispatch_async(dispatch_get_main_queue(), ^{
     //数据执行完毕回调到主线程操作UI更新数据
});
```
- 
有时候省去麻烦，我们使用系统的全局队列：一般用这个处理遍历大数据查询操作
```
DISPATCH_QUEUE_PRIORITY_HIGH  全局队列高优先级
DISPATCH_QUEUE_PRIORITY_LOW 全局队列低优先级
DISPATCH_QUEUE_PRIORITY_BACKGROUND  全局队里后台执行队列
// 全局并发队列执行处理大量逻辑时使用   
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
});
```
- 
当在开发中遇到一些数据需要单线程访问的时候，我们可以采取同步线程的做法，来保证数据的正常执行
```cpp
```cpp
//当我们需要执行一些数据安全操作写入的时候，需要同步操作，后面所有的任务要等待当前线程的执行
dispatch_sync(dispatch_get_global_queue(0, 0), ^{
   //同步线程操作可以保证数据的安全完整性
});
```
```
#### 二、了解一下NSObject中的对象线程访问模式
- 
我们介绍简单的perfermselecter选择器实现线程通信
```
//数据请求完毕回调到主线程，更新UI资源信息  waitUntilDone    设置YES ，代表等待当前线程执行完毕
 [self performSelectorOnMainThread:@selector(dothing:) withObject:@[@"1"] waitUntilDone:YES];
```
- 
如果需要执行到后台线程，则直接前往后台线程执行即可
```
//将当前的逻辑转到后台线程去执行
[self performSelectorInBackground:@selector(dothing:) withObject:@[@"2"]];
```
- 自己定义线程，将当前数据转移到指定的线程内去通信操作
```
//支持自定义线程通信执行相应的操作
    NSThread * thread = [[NSThread alloc]init];
    [thread start];
        //当我们需要在特定的线程内去执行某一些数据的时候，我们需要指定某一个线程操作
    [self performSelector:@selector(dothing:) onThread:thread withObject:nil waitUntilDone:YES];
    //支持自定义线程通信执行相应的操作
    NSThread * thread = [[NSThread alloc]initWithTarget:self selector:@selector(testThread) object:nil];
    [thread start];
       //当我们需要在特定的线程内去执行某一些数据的时候，我们需要指定某一个线程操作
    [self performSelector:@selector(dothing:) onThread:thread withObject:nil waitUntilDone:YES];
```
- 上面几种方法就是我们常用的对象调用常用的线程间通信，我们可以根据不同的情况，采取不同的线程执行状态.
##### 增加一个特殊的线程常驻RunLoop 的做法
- 需求： 我们经常要用到常驻线程来请求数据，但是请求有时候在线程会退出，这个时候我们需要用一下方法：
```
//有时候需要线程单独跑一个RunLoop 来保证我们的请求对象存在，不至于会被系统释放掉
    NSThread * runLoopThread = [[NSThread alloc]initWithTarget:self selector:@selector(entryThreadPoint) object:nil];
    [runLoopThread start];
    [self performSelector:@selector(handleMutiData) onThread:runLoopThread withObject:nil waitUntilDone:YES];
    //给线程增加一个run loop 来保持对象的引用
- (void)entryThreadPoint{
    @autoreleasepool {
        [NSThread currentThread].name = @"自定义线程名字";
        NSRunLoop * runloop = [NSRunLoop currentRunLoop];
        [runloop addPort:[NSPort port] forMode:NSDefaultRunLoopMode];
        [runloop run];
    }
}
- (void)handleMutiData{
    NSLog(@"## 我是跑在runloop的线程 ##");
}
```
最后测试截图如下，看下我们的线程是不是已经加入runloop 了
![](http://upload-images.jianshu.io/upload_images/680076-1b93a55de8dbee86.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这个就是最后的执行了
本文暂时先介绍到这里，如果还想了解用信号量控制线程可以参考另外一篇博客：
[iOS 关于dispatch_semaphore_t 和 dispatch_group_t 的简单实用，用于多网络异步回调通知](http://www.jianshu.com/p/a4ea43179870)
文／Raybon_lee（简书作者）
原文链接：http://www.jianshu.com/p/aacd3409f14b
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
