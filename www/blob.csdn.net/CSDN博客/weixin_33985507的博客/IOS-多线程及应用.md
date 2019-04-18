# IOS-多线程及应用 - weixin_33985507的博客 - CSDN博客
2016年11月16日 23:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
今天有个安卓的大神问我多线程的知识，因为长时间只使用那几个方法，所以该忘得都忘了，所以被问得很尴尬，所以今天就把多线程的GCD和NSOperation相关的全部拾起来，有空多拿出来看看。
# NSOperation
几个关键点：
## 0. GCD创建线程是遵守FIFO。
## 1. GCD是面向队列和任务编程
GCD是苹果极力推荐使用的多线程实现方案，相对于NSThread，GCD已经完全没有线程的概念，是面向队列和任务的。
## 2. 使用GCD的步骤：（两个步骤）
- 创建队列   2. 将任务添加到队列中
## 3. 并发队列和串行队列、同步任务和异步任务
##### 同步任务和异步任务
- 异步函数（异步任务）：具有开线程能力，在当前线程任务完成之后，再去新开线程去执行队列中的任务。
```
dispatch_async(dispatch_queue_t , <#^(void)block#>)
```
- 同步函数（同步任务）：是在当前线程中立即执行任务，不具有开线程能力；在当前线程中立马执行队列中的任务。
```
dispatch_sync(dispatch_queue_t , <#^(void)block#>)
```
##### 并发队列和串行队列。
- 并发队列：允许同时开启多条线程来执行任务
> 
注意点：
- 让队列中的任务同时执行，那就可能需要开启多个线程来同时执行任务。
- 开多少条线程要看CPU的使用状况。
- 并发队列其实不具有开线程能力，异步函数才具有。
创建并发队列方式：
- 全局并发队列
```
dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
// 或者
  dispatch_queue_t queue = dispatch_get_global_queue(0, 0);
```
- 自定义并发队列
```
dispatch_queue_t queue = dispatch_queue_create("com.wangwei", DISPATCH_QUEUE_CONCURRENT);
```
- 串行队列：将队列中的任务一件一件完成。（如果在同步函数下，串行队列中的任务都是在当前线程下执行的，如果在异步函数下，队列中的任务都是在新开的线程中被执行的）
（2）主队列（在主线程中执行的队列）
```
dispatch_queue_t queue= dispatch_get_main_queue();
```
（2）自定义串行队列
```
dispatch_queue_t queue = dispatch_queue_create("com.wangwei", DISPATCH_QUEUE_SERIAL);
```
## 4. 上代码
其实最常使用的是 （1）异步全局并发队列（2）异步同步队列
（并发队列和串行队列、同步函数和异步函数、主队列 排列组合获得）
```
#pragma mark - GCD的同步函数和异步函数，并发队列和串行队列
/// 1 异步函数+并发 (开启多个线程同时执行队列中的多个任务)
- (void)asyncConCurrent {
    /// 创建队列
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    
    dispatch_async(queue, ^{
        [self work2];
    });
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [self work3];
    });
    
    NSLog(@"看看是否先执行");
}
/// 2 异步函数+串行 (开启新的线程，串行执行队列中的多个任务)
- (void)asyncSerial {
    
    /// 创建串行队列
    dispatch_queue_t queue = dispatch_queue_create("com.wangwei", DISPATCH_QUEUE_SERIAL);
    
    
    dispatch_async(queue, ^{
        [self work2];
    });
    dispatch_async(queue, ^{
        [self work3];
    });
    
    NSLog(@"看看是否先执行");
}
/// 3 同步函数+并发 (在当前线程中线程串行执行队列中的多个任务)
- (void)syncConCurrent {
    /// 创建队列
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    
    dispatch_sync(queue, ^{
        [self work2];
    });
    dispatch_sync(queue, ^{
        [self work3];
    });
    
    NSLog(@"看看是否先执行");
}
/// 4 同步函数+串行 (在当前线程，串行执行队列中的多个任务)
- (void)syncSerial {
    
    /// 创建串行队列
    dispatch_queue_t queue = dispatch_queue_create("com.wangwei", DISPATCH_QUEUE_SERIAL);
    
    dispatch_sync(queue, ^{
        [self work2];
    });
    dispatch_sync(queue, ^{
        [self work3];
    });
    
    NSLog(@"看看是否先执行");
}
/// 5 异步函数+主队列 (在主线程中执行队列中的多个任务)
- (void)asyncMain {
 
    /// 队列
    dispatch_queue_t queue = dispatch_get_main_queue();
    
    /// 添加任务到队列中
    dispatch_async(queue, ^{
        [self work2];
    });
    dispatch_async(queue, ^{
        [self work3];
    });
    
    NSLog(@"看看是否先执行");
}
```
## 5. 常用函数
### （1）dispatch_barrier_async
作用：可以将队列中的任务通过barrier来分割执行
注意：不能使用全局并发队列，只能通过自定义并发队列，才能达到分割效果；在一个队列中可以多次使用barrier进行分割。
```
#pragma barrier
- (void)barrierThread {
    /// 自定义一个并发队列
    dispatch_queue_t queue = dispatch_queue_create("wangwei.com", DISPATCH_QUEUE_CONCURRENT);
    
    dispatch_async(queue, ^{
        [self work2];
    });
    
   dispatch_barrier_async(queue, ^{
        NSLog(@"我是barrier，当前线程：%@",[NSThread currentThread]);
    });
    dispatch_async(queue, ^{
        [self work3];
    });
    
    dispatch_barrier_async(queue, ^{
        NSLog(@"我是barrier，当前线程：%@",[NSThread currentThread]);
    });
    
    dispatch_async(queue, ^{
        [self work7];
    });
    dispatch_async(queue, ^{
        [self work8];
    });
}
```
### （2）线程中通信（其实就是在子线程中执行耗时操作，在主线程中刷新UI）(其实通畅就是使用异步函数并发队列)
```
#pragma mark - 线程之间通信
- (void)comebackThread {
    
   dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        
        /// 网络获取图片
        
        /// 返回主队列显示图片
        dispatch_async(dispatch_get_main_queue(), ^{
            
            /// 刷新界面
        });
    });
    
}
```
### （3）几种延时方式（可能不是很全面）
```
/// 延时1 GCD
    NSLog(@"现在开始延时");
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        [self work2];
    });
/// 延时2 NSTimer
    [NSTimer scheduledTimerWithTimeInterval:3 target:self selector:@selector(work2) userInfo:nil repeats:NO];
    
/// 延时3 perform
     [self performSelector:@selector(work2) withObject:nil afterDelay:2];
```
### （4）快速迭代 apply （可以快速将数组的内容快速遍历）
结果可以表示：使用快速迭代的方式可能创建了多个线程，所以遍历数组的时间更短、效率更高。（推荐在遍历数组的时候使用，这样在主线程中计算和分解数组时，可以减少在主线程的执行时间，很多耗时的计算都在子线程中进行，进而界面就会更加流畅）
```
#pragma apply
- (void)apply {
    /// 将数组中的各个元素快速使用（其实就是针对数组的操作，多开了写线程）
    NSArray *arr = @[@1,@2,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3,@3];
   
    CFTimeInterval begin = CFAbsoluteTimeGetCurrent();
    /// 将数组中的元素加一
    dispatch_apply(arr.count, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(size_t index) {
        NSLog(@"%@",arr[index]);
    });
    
    CFTimeInterval end = CFAbsoluteTimeGetCurrent();
    NSLog(@"-----%f",end - begin);
    /************************华丽分割线**********************************/
    CFTimeInterval begin1 = CFAbsoluteTimeGetCurrent();
    for (int i=0; i<arr.count; i++) {
        NSLog(@"%@",arr[i]);
    }
    CFTimeInterval end1 = CFAbsoluteTimeGetCurrent();
    NSLog(@"-----%f",end1 - begin1);
}
```
### （5）一次性代码
(程序执行一次，之后这段代码就像消失了一样，也就是不会再执行，这和懒加载有很大的区别）
```
/// 都知道在dispatch_once中使用，可用于写单利，在这里不再赘述
```
### CGD信号量和队列组
[参考博客](https://www.jianshu.com/p/a8322aa8bd23)
一般情况下，我们会将显示界面的数据一次性获取到之后再去进行布局操作，而当一个界面存在多个网络接口, 比如: 在首页界面中的banner、菜单目录、滚动头条、商品、价格等。说明下我没有使用GCD和队列组时候的用法，虽然实现了功能但是是一种很low的方法（例如有三个请求，在第一个请求返回中进行第二个请求，在第二个请求返回中进行第三次请求），而现在使用CGD信号量和队列组，那么代码可读性就变高很多了，编码也更加规范。
#### （1）队列组
基本概念：
思路：GCD中的队列组, 将请求操作添加队列组, 最后在dispatch_group_notify中刷新UI。
```
//首页中提供了五个网络接口
 
    /*
     1. 在队列组中进行多个网络请求
     2. 在全部的网络请求完成之后再进行UI刷新
     3. 其队列请求是随机的
     */
    
    // URL
    NSString *str1 = @"http://sonydata.csmc-cloud.com/layout/kv/v2/kv.json";
    NSString *str2 = @"http://sonydata.csmc-cloud.com/layout/menu/v2/menu_body.json";
    NSString *str3 = @"http://sonydata.csmc-cloud.com/layout/news/v1/news_body.json";
    NSString *str4 = @"http://sonydata.csmc-cloud.com/layout/new_product/v1/new_product_head.json";
    NSString *str5 = @"http://sonydata.csmc-cloud.com/layout/new_product/v1/new_product_body.json";
    
    // 创建队列组
    dispatch_group_t group = dispatch_group_create();
  // 创建队列组的block操作
    dispatch_group_async(group, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
         NSLog(@"子线程上的第1个请求");
        // 网络请求1
        [self getMessageWithURLString:str1 messageBlock:^(NSDictionary *dic) {
            NSLog(@"完成1");
        }];
    });
    
    dispatch_group_async(group, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        NSLog(@"子线程上的第2个请求");
        // 网络请求2
        [self getMessageWithURLString:str2 messageBlock:^(NSDictionary *dic) {
            NSLog(@"完成2");
        }];
        
    });
    
    dispatch_group_async(group, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        NSLog(@"子线程上的第3个请求");
        // 网络请求3
        [self getMessageWithURLString:str3 messageBlock:^(NSDictionary *dic) {
            NSLog(@"完成3");
        }];
    });
    
    dispatch_group_async(group, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        NSLog(@"子线程上的第4个请求");
        // 网络请求4
        [self getMessageWithURLString:str4 messageBlock:^(NSDictionary *dic) {
            NSLog(@"完成4");
        }];
    });
    
    dispatch_group_async(group, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        NSLog(@"子线程上的第5个请求");
        // 网络请求5
        [self getMessageWithURLString:str5 messageBlock:^(NSDictionary *dic) {
            NSLog(@"完成5");
        }];
    });
    
    // 这五个请求都完成之后，执行完成后的通知
    dispatch_group_notify(group, dispatch_get_main_queue(), ^{
        // 返回主线程刷新UI
        NSLog(@"完成请求后，刷新UI");
    });
    
/// 使用AFN进行网络请求
- (void)getMessageWithURLString:(NSString *)string messageBlock:(myBlock)messageBlock {
    /// 请求管理者
    AFHTTPSessionManager *manager =[AFHTTPSessionManager manager];
    manager.securityPolicy.allowInvalidCertificates = YES;
    manager.securityPolicy.validatesDomainName = NO;
    manager.responseSerializer = [AFHTTPResponseSerializer serializer];
    manager.requestSerializer = [AFJSONRequestSerializer serializer];
    
    //如果报接受类型不一致请替换一致text/html或别的
    manager.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json", @"text/json", @"text/javascript",@"text/plain", @"text/html" , nil];
    [manager GET:string parameters:nil progress:^(NSProgress * _Nonnull downloadProgress) {
    } success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
        messageBlock(responseObject);
        
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
        messageBlock(nil);
    }];
}
```
其打印的结果如下：
![3011588-9b8fd3ef8fc00e8c.png](https://upload-images.jianshu.io/upload_images/3011588-9b8fd3ef8fc00e8c.png)
屏幕快照 2017-02-28 下午1.37.37.png
然而这并不能够满足请求完数据后再刷新界面的要求。在GCD中提供了一种信号机制，可以解决资源抢占问题（和同步锁的机制并不一样）。GCD中信号量是dispatch_semaphore_t类型，支持信号通知和信号等待。每当发送一个信号通知，则信号量+1；每当发送一个等待信号时信号量-1；如果信号量为0则信号会处于等待状态，直到信号量大于0开始执行。根据这个原理我们可以初始化一个信号量变量，默认信号量设置为1，每当有线程进入“加锁代码”之后就调用信号等待命令（此时信号量为0）开始等待，此时其他线程无法进入，执行完后发送信号通知（此时信号量为1），其他线程开始进入执行，如此一来就达到了线程同步目的。
### GCD提供了两种方式来支持dispatch队列的同步,就是dispatch组(队列组)和信号量(dispatch_semaphore)
- 信号量
将“AFN进行网络请求”的方法添加信号量
```
/// 使用AFN进行网络请求
- (void)getMessageWithURLString:(NSString *)string messageBlock:(myBlock)messageBlock {
    
    //创建信号量并设置计数默认为0 (创建信号量，可以设置信号量的资源数。0表示没有资源，调用dispatch_semaphore_wait会立即等待)
    dispatch_semaphore_t sema = dispatch_semaphore_create(0);
    
    /// 请求管理者
    AFHTTPSessionManager *manager =[AFHTTPSessionManager manager];
    manager.securityPolicy.allowInvalidCertificates = YES;
    manager.securityPolicy.validatesDomainName = NO;
    manager.responseSerializer = [AFHTTPResponseSerializer serializer];
    manager.requestSerializer = [AFJSONRequestSerializer serializer];
    
    //如果报接受类型不一致请替换一致text/html或别的
    manager.responseSerializer.acceptableContentTypes = [NSSet setWithObjects:@"application/json", @"text/json", @"text/javascript",@"text/plain", @"text/html" , nil];
    
    [manager GET:string parameters:nil progress:^(NSProgress * _Nonnull downloadProgress) {
    } success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
        
        
        // 计数+1操作（ 通知信号，如果等待线程被唤醒则返回非0，否则返回0）
        dispatch_semaphore_signal(sema);
        messageBlock(responseObject);
        
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
        //计数+1操作（ 通知信号，如果等待线程被唤醒则返回非0，否则返回0）
        dispatch_semaphore_signal(sema);
        messageBlock(nil);
    }];
    // 等待group所对应的block执行完毕（等待信号，可以设置超时参数。该函数返回0表示得到通知，非0表示超时）
    dispatch_semaphore_wait(sema, DISPATCH_TIME_FOREVER);
    
    
}
```
其运行结果为
![3011588-68902faadf32728d.png](https://upload-images.jianshu.io/upload_images/3011588-68902faadf32728d.png)
屏幕快照 2017-02-28 下午1.51.52.png
也就是数据请求完之后再进行UI的刷新操作。
信号量的几个概念和操作：
```
1. dispatch_semaphore(即: GCD中的信号量), 通过GCD中的信号量实现线程同步。
2. dispatch_semaphore概念: 信号量是基于计数器的一种多线程同步机制, 主要是用于解决多个线程访问共有的资源时.造成数据紊乱的问题。
3. dispatch_semaphore基本原理: 比如说网络请求成功或者失败之后需要将dispatch_semaphore计数器 +1, 请求网络操作完成之后需要将dispatch_semaphore计数器 -1. 如果dispatch_semaphore计数器等于0表示等待。
加1操作: dispatch_semaphore_signal(semaphore)
等待操作: dispatch_semaphore_wait(sema, DISPATCH_TIME_FOREVER)
```
### GCD中提供了函数, 可以指定操作的执行顺序
在GCD中的任务添加依赖关系
```
/*
     如果我们要指定网络操作的执行顺序的话, 直接使用GCD中的队列, 然后添加依赖即可
     */
    
    //1.任务一：
    NSBlockOperation *operation1 = [NSBlockOperation blockOperationWithBlock:^{
        
        // 请求1
        NSString *str1 = @"http://sonydata.csmc-cloud.com/layout/kv/v2/kv.json";
        [self getMessageWithURLString:str1 messageBlock:^(NSDictionary *dic) {
            NSLog(@"完成1");
        }];
    }];
    
    //2.任务二：
    NSBlockOperation *operation2 = [NSBlockOperation blockOperationWithBlock:^{
        
        // 请求2
        NSString *str2 = @"http://sonydata.csmc-cloud.com/layout/menu/v2/menu_body.json";
        [self getMessageWithURLString:str2 messageBlock:^(NSDictionary *dic) {
            NSLog(@"完成2");
        }];
    }];
    
    //3.任务三：
    NSBlockOperation *operation3 = [NSBlockOperation blockOperationWithBlock:^{
        
        // 请求3
        NSString *str3 = @"http://sonydata.csmc-cloud.com/layout/news/v1/news_body.json";
        [self getMessageWithURLString:str3 messageBlock:^(NSDictionary *dic) {
            NSLog(@"完成3");
        }];
    }];
    
    //4.任务四：
    NSBlockOperation *operation4 = [NSBlockOperation blockOperationWithBlock:^{
        
        // 请求4
        NSString *str4 = @"http://sonydata.csmc-cloud.com/layout/new_product/v1/new_product_head.json";
        [self getMessageWithURLString:str4 messageBlock:^(NSDictionary *dic) {
            NSLog(@"完成4");
        }];
    }];
    
    //5.任务五：
    NSBlockOperation *operation5 = [NSBlockOperation blockOperationWithBlock:^{
        
        // 请求5
        NSString *str5 = @"http://sonydata.csmc-cloud.com/layout/new_product/v1/new_product_body.json";
        [self getMessageWithURLString:str5 messageBlock:^(NSDictionary *dic) {
            NSLog(@"完成5");
        }];
    }];
    
    //4.设置依赖
    [operation2 addDependency:operation1];      //任务二依赖任务一
    [operation3 addDependency:operation2];      //任务三依赖任务二
    [operation4 addDependency:operation3];      //任务三依赖任务二
    [operation5 addDependency:operation4];      //任务三依赖任务二
    
    //5.创建队列并加入任务
    NSOperationQueue *queue = [[NSOperationQueue alloc] init];
    [queue addOperations:@[operation3, operation2, operation1, operation4, operation5] waitUntilFinished:NO];
```
log：
![3011588-4b189be6b1859d12.png](https://upload-images.jianshu.io/upload_images/3011588-4b189be6b1859d12.png)
屏幕快照 2017-02-28 下午2.43.41.png
注：
//  我们也可以管理队列组的运行状态或者计数, 使用下面两个函数的时候需要注意的一点就是, 进入或者退出, 他们的次数必须要匹配。
```
dispatch_group_enter(group); // 进入
dispatch_group_leave(group);    // 退出
// 所以，我们也可以利用dispatch_group_enter、 dispatch_group_leave和dispatch_group_wait来实现同步
```
[信号量和队列组的demo](https://link.jianshu.com?t=https://github.com/WangWei1993/GCD-.git)
