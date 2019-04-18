# iOS 多线程 - weixin_33985507的博客 - CSDN博客
2017年11月21日 11:14:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
参考资料：
[iOS多线程](https://www.jianshu.com/p/0b0d9b1f1f19)
[iOS GCD](https://www.jianshu.com/p/2d57c72016c6)
多线程问题在iOS中目前有`4`套多线程方案，他们分别是：
> - Pthreads
- NSthread
- GCD
- NSOperation
## Pthreads
这是一套在很多操作系统通用的多线程API，不过这是基于c语言的框架，在iOS实际开发中很少使用。
- 包含头文件：
`#import <pthread.h>`
- 创建线程，并执行任务：
```
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    // 创建一个新的线程并自动执行
    pthread_t thread;
    pthread_create(&thread, NULL, pthreadStart, NULL);
}
void *pthreadStart() {
    NSLog(@"%@", [NSThread currentThread]);
    return NULL;
}
```
- 输出打印：
`<NSThread: 0x6040002719c0>{number = 3, name = (null)}`
## NSthread
这套方案是苹果封装的多线程解决方案。可以直接操控线程对象。但是，它的生命周期还是需要我们手动管理，所以这套方案使用频率不高，下面来看看它的一些用法。
- 创建线程并启动
```
NSThread *thread = [[NSThread alloc] initWithTarget:self selector:@selector(threadStart) object:nil];
[thread start];
```
- 创建线程并自动启动
```
[NSThread detachNewThreadSelector:@selector(threadStart) toTarget:self withObject:nil];
```
- 使用NSObject的方法创建线程并自动启动
```
[self performSelectorInBackground:@selector(threadStart) withObject:nil];
```
- 其他常用方法
```
//取消线程
- (void)cancel;
//启动线程
- (void)start;
//判断某个线程的状态的属性
@property (readonly, getter=isExecuting) BOOL executing;
@property (readonly, getter=isFinished) BOOL finished;
@property (readonly, getter=isCancelled) BOOL cancelled;
//设置和获取线程名字
-(void)setName:(NSString *)n;
-(NSString *)name;
//获取当前线程信息
+ (NSThread *)currentThread;
//获取主线程信息
+ (NSThread *)mainThread;
//使当前线程暂停一段时间，或者暂停到某个时刻
+ (void)sleepForTimeInterval:(NSTimeInterval)time;
+ (void)sleepUntilDate:(NSDate *)date;
```
## GCD
`Grand Central Dispatch`，它是苹果为多核的并行运算提出的解决方案，所以会自动合理地利用更多的CPU内核（比如双核、四核），最重要的是它会自动管理线程的生命周期（创建线程、调度任务、销毁线程），完全不需要我们管理，我们只需要告诉干什么就行。同时它使用的也是 c语言，不过由于使用了`Block`（Swift里叫做闭包），使得使用起来更加方便，而且灵活。
#### 1. 任务和队列
在`GCD`中，加入了两个非常重要的概念：**任务**和**队列**。
- 任务：即操作，你想要干什么，说白了就是一段代码，在 GCD 中就是一个 Block，所以添加任务十分方便。任务有两种执行方式： 同步执行 和 异步执行，他们之间的区别是 **是否会创建新的线程**。
> 
同步（sync） 和 异步（async） 的主要区别在于会不会阻塞当前线程，直到 Block 中的任务执行完毕！
如果是 同步（sync） 操作，它会阻塞当前线程并等待 Block 中的任务执行完毕，然后当前线程才会继续往下运行。
如果是 异步（async）操作，当前线程会直接往下执行，它不会阻塞当前线程。
- 队列：用于存放任务。一共有两种队列， 串行队列 和 并行队列。
**串行队列** 中的任务，GCD 会 FIFO（先进先出） 地取出来一个，执行一个，然后取下一个，这样一个一个的执行。
**并行队列** 中的任务，GCD 也会 FIFO的取出来，但不同的是，它取出来一个就会放到别的线程，然后再取出来一个又放到另一个的线程。这样由于取的动作很快，忽略不计，看起来，所有的任务都是一起执行的。不过需要注意，GCD 会根据系统资源控制并行的数量，所以如果任务很多，它并不会让所有任务同时执行。
#### 2. 创建队列
- **主队列**：这是一个特殊的 串行队列。什么是主队列，大家都知道吧，它用于刷新 UI，任何需要刷新 UI 的工作都要在主队列执行，所以一般耗时的任务都要放到别的线程执行。
```
//OBJECTIVE-C
dispatch_queue_t queue = ispatch_get_main_queue();
```
- **自己创建的队列**：自己可以创建 `串行队列`, 也可以创建 `并行队列`。看下面的代码（代码已更新），它有两个参数，第一个上面已经说了，第二个才是最重要的。
第二个参数用来表示创建的队列是串行的还是并行的，传入 `DISPATCH_QUEUE_SERIAL` 或 `NULL` 表示创建串行队列。传入 `DISPATCH_QUEUE_CONCURRENT` 表示创建并行队列。
```
//串行队列
dispatch_queue_t queue = dispatch_queue_create("tk.bourne.testQueue", NULL);
dispatch_queue_t queue = dispatch_queue_create("tk.bourne.testQueue", DISPATCH_QUEUE_SERIAL);
//并行队列
dispatch_queue_t queue = dispatch_queue_create("tk.bourne.testQueue", DISPATCH_QUEUE_CONCURRENT);
```
- **全局并行队列**：只要是并行任务一般都加入到这个队列。这是系统提供的一个并发队列。
```
dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
```
#### 3. 创建任务
- 同步任务：会阻塞当前线程 (SYNC)
```
dispatch_sync(<#queue#>, ^{
    //code here
    NSLog(@"%@", [NSThread currentThread]);
});
```
- 异步任务：不会阻塞当前线程 (ASYNC)
```
dispatch_async(<#queue#>, ^{
    //code here
    NSLog(@"%@", [NSThread currentThread]);
});
```
#### 4. 队列组
队列组可以将很多队列添加到一个组里，这样做的好处是，当这个组里所有的任务都执行完了，队列组会通过一个方法通知我们。下面是使用方法，这是一个很实用的功能。
```
//1.创建队列组
dispatch_group_t group = dispatch_group_create();
//2.创建队列
dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
//3.多次使用队列组的方法执行任务, 只有异步方法
//3.1.执行3次循环
dispatch_group_async(group, queue, ^{
    for (NSInteger i = 0; i < 3; i++) {
        NSLog(@"group-01 - %@", [NSThread currentThread]);
    }
});
//3.2.主队列执行8次循环
dispatch_group_async(group, dispatch_get_main_queue(), ^{
    for (NSInteger i = 0; i < 8; i++) {
        NSLog(@"group-02 - %@", [NSThread currentThread]);
    }
});
//3.3.执行5次循环
dispatch_group_async(group, queue, ^{
    for (NSInteger i = 0; i < 5; i++) {
        NSLog(@"group-03 - %@", [NSThread currentThread]);
    }
});
//4.都完成后会自动通知
dispatch_group_notify(group, dispatch_get_main_queue(), ^{
    NSLog(@"完成 - %@", [NSThread currentThread]);
});
```
#### 5. 栅栏方法
- 我们有时需要异步执行两组操作，而且第一组操作执行完之后，才能开始执行第二组操作。这样我们就需要一个相当于栅栏一样的一个方法将两组异步执行的操作组给分割起来，当然这里的操作组里可以包含一个或多个任务。这就需要用到`dispatch_barrier_async`方法在两个操作组间形成栅栏。
```
dispatch_queue_t queue = dispatch_queue_create("12312312", DISPATCH_QUEUE_CONCURRENT);
dispatch_async(queue, ^{
    NSLog(@"----1-----%@", [NSThread currentThread]);
});
dispatch_async(queue, ^{
    NSLog(@"----2-----%@", [NSThread currentThread]);
});
dispatch_barrier_async(queue, ^{
    NSLog(@"----barrier-----%@", [NSThread currentThread]);
});
dispatch_async(queue, ^{
    NSLog(@"----3-----%@", [NSThread currentThread]);
});
dispatch_async(queue, ^{
    NSLog(@"----4-----%@", [NSThread currentThread]);
});
```
#### 6. 延时执行
- 当我们需要延迟执行一段代码时，就需要用到GCD的`dispatch_after`方法。
```
dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2.0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
    // 2秒后异步执行这里的代码...
   NSLog(@"run-----");
});
```
#### 7. 一次性代码
- 我们在创建单例、或者有整个程序运行过程中只执行一次的代码时，我们就用到了GCD的`dispatch_once`方法。使用`dispatch_once`函数能保证某段代码在程序运行过程中只被执行1次。
```
static dispatch_once_t onceToken;
dispatch_once(&onceToken, ^{
    // 只执行1次的代码(这里面默认是线程安全的)
});
```
#### 8. 快速迭代方法
- 通常我们会用for循环遍历，但是GCD给我们提供了快速迭代的方法`dispatch_apply`，使我们可以同时遍历。比如说遍历0~5这6个数字，for循环的做法是每次取出一个元素，逐个遍历。`dispatch_apply`可以同时遍历多个数字。
```
dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
dispatch_apply(6, queue, ^(size_t index) {
    NSLog(@"%zd------%@",index, [NSThread currentThread]);
});
```
## NSOperation
NSOperation 是苹果公司对 GCD 的封装，完全面向对象，所以使用起来更好理解。 大家可以看到`NSOperation`和`NSOperationQueue`分别对应 GCD 的`任务`和`队列`。操作步骤如下：
- 将要执行的任务封装到一个`NSOperation`对象中。
- 将此任务添加到一个`NSOperationQueue`对象中。
注：
`NSOperation`只是一个抽象类，所以不能封装任务。但它有 2 个子类用于封装任务。分别是：`NSInvocationOperation`和`NSBlockOperation`。创建一个`Operation`后，需要调用`start`方法来启动任务，它会 默认在当前队列同步执行。当然你也可以在中途取消一个任务，只需要调用其`cancel`方法即可。
- NSInvocationOperation：
```
//1.创建NSInvocationOperation对象
NSInvocationOperation *operation = [[NSInvocationOperation alloc] initWithTarget:self selector:@selector(run) object:nil];
//2.开始执行
[operation start];
```
- NSBlockOperation
```
//1.创建NSBlockOperation对象
NSBlockOperation *operation = [NSBlockOperation blockOperationWithBlock:^{
    NSLog(@"%@", [NSThread currentThread]);
}];
//2.开始任务
[operation start];
```
`NSBlockOperation`默认会在当前线程执行。但它还有一个方法：`addExecutionBlock:`，通过这个方法可以给`Operation`添加多个执行 Block。这样 Operation 中的任务 会并发执行，它会在主线程和其它的多个线程 执行这些任务。
```
NSBlockOperation *operation = [NSBlockOperation blockOperationWithBlock:^{
    NSLog(@"%@", [NSThread currentThread]);
}];
for (int i = 0; i < 5; i++) {
    [operation addExecutionBlock:^{
        NSLog(@"execution - %@", [NSThread currentThread]);
    }];
}
[operation start];
```
输出打印：
```
<NSThread: 0x60000046c880>{number = 9, name = (null)}
execution - <NSThread: 0x604000078ac0>{number = 1, name = main}
execution - <NSThread: 0x600000469280>{number = 11, name = (null)}
execution - <NSThread: 0x60400026cb40>{number = 10, name = (null)}
execution - <NSThread: 0x60000046c880>{number = 9, name = (null)}
execution - <NSThread: 0x604000078ac0>{number = 1, name = main}
```
- 自定义Operation
除了上面的两种`Operation`以外，我们还可以自定义`Operation`。自定义`Operation`需要继承`NSOperation`类，并实现其`main()`方法，因为在调用`start()`方法的时候，内部会调用`main()`方法完成相关逻辑。所以如果以上的两个类无法满足你的欲望的时候，你就需要自定义了。你想要实现什么功能都可以写在里面。除此之外，你还需要实现`cancel()`在内的各种方法。
#### 创建队列
看过上面的内容就知道，我们可以调用一个`NSOperation`对象的`start()`方法来启动这个任务，但是这样做他们默认是 同步执行 的。就算是`addExecutionBlock`方法，也会在 当前线程和其他线程 中执行，也就是说还是会占用当前线程。这是就要用到队列`NSOperationQueue`了。而且，按类型来说的话一共有两种类型：主队列、其他队列。只要添加到队列，会自动调用任务的`start()`方法。
- 主队列
细心的同学就会发现，每套多线程方案都会有一个主线程（当然啦，说的是iOS中，像`pthread`这种多系统的方案并没有，因为*UI线程*理论需要每种操作系统自己定制）。这是一个特殊的线程，必须串行。所以添加到主队列的任务都会一个接一个地排着队在主线程处理。
```
NSOperationQueue *queue = [NSOperationQueue mainQueue];
```
- 其他队列
因为主队列比较特殊，所以会单独有一个类方法来获得主队列。那么通过初始化产生的队列就是其他队列了，因为只有这两种队列，除了主队列，其他队列就不需要名字了。
注意：其他队列的任务会在其他线程并行执行。
```
//1.创建一个其他队列    
NSOperationQueue *queue = [[NSOperationQueue alloc] init];
//2.创建NSBlockOperation对象
NSBlockOperation *operation = [NSBlockOperation blockOperationWithBlock:^{
    NSLog(@"%@", [NSThread currentThread]);
}];
//3.添加多个Block
for (NSInteger i = 0; i < 5; i++) {
    [operation addExecutionBlock:^{
        NSLog(@"第%ld次：%@", i, [NSThread currentThread]);
    }];
}
//4.队列添加任务
[queue addOperation:operation];
```
打印输出
> 
2015-07-28 20:26:28.463 test[18622:4443534] <NSThread: 0x7fd022c3ac10>{number = 5, name = (null)}
2015-07-28 20:26:28.463 test[18622:4443536] 第2次 - <NSThread: 0x7fd022e36d50>{number = 2, name = (null)}
2015-07-28 20:26:28.463 test[18622:4443535] 第0次 - <NSThread: 0x7fd022f237f0>{number = 4, name = (null)}
2015-07-28 20:26:28.463 test[18622:4443533] 第1次 - <NSThread: 0x7fd022d372b0>{number = 3, name = (null)}
2015-07-28 20:26:28.463 test[18622:4443534] 第3次 - <NSThread: 0x7fd022c3ac10>{number = 5, name = (null)}
2015-07-28 20:26:28.463 test[18622:4443536] 第4次 - <NSThread: 0x7fd022e36d50>{number = 2, name = (null)}
OK, 这时应该发问了，大家将`NSOperationQueue`与`GCD`的队列 相比较就会发现，这里没有串行队列，那如果我想要10个任务在其他线程串行的执行怎么办？
这就是苹果封装的妙处，你不用管串行、并行、同步、异步这些名词。`NSOperationQueue`有一个参数`maxConcurrentOperationCount`最大并发数，用来设置最多可以让多少个任务同时执行。当你把它设置为 1 的时候，他不就是串行了嘛！
`NSOperationQueue`还有一个添加任务的方法，`- (void)addOperationWithBlock:(void (^)(void))block;`，这是不是和`GCD`差不多？这样就可以添加一个任务到队列中了，十分方便。
`NSOperation`有一个非常实用的功能，那就是添加依赖。比如有 3 个任务：A: 从服务器上下载一张图片，B：给这张图片加个水印，C：把图片返回给服务器。这时就可以用到依赖了:
```
//1.任务一：下载图片
NSBlockOperation *operation1 = [NSBlockOperation blockOperationWithBlock:^{
    NSLog(@"下载图片 - %@", [NSThread currentThread]);
    [NSThread sleepForTimeInterval:3.0];
}];
//2.任务二：打水印
NSBlockOperation *operation2 = [NSBlockOperation blockOperationWithBlock:^{
    NSLog(@"打水印   - %@", [NSThread currentThread]);
    [NSThread sleepForTimeInterval:1.0];
}];
//3.任务三：上传图片
NSBlockOperation *operation3 = [NSBlockOperation blockOperationWithBlock:^{
    NSLog(@"上传图片 - %@", [NSThread currentThread]);
    [NSThread sleepForTimeInterval:1.0];
}];
//4.设置依赖
[operation1 addDependency:operation3];      //任务二依赖任务一
[operation3 addDependency:operation2];      //任务三依赖任务二
//5.创建队列并加入任务
NSOperationQueue *queue = [[NSOperationQueue alloc] init];
[queue addOperations:@[operation3, operation2, operation1] waitUntilFinished:NO];
```
打印结果
```
打水印   - <NSThread: 0x600000276900>{number = 3, name = (null)}
上传图片 - <NSThread: 0x604000462400>{number = 4, name = (null)}
下载图片 - <NSThread: 0x600000276900>{number = 3, name = (null)}
```
- 注意：不能添加相互依赖，会死锁，比如 A依赖B，B依赖A。
- 可以使用`removeDependency`来解除依赖关系。
- 可以在不同的队列之间依赖，反正就是这个依赖是添加到任务身上的，和队列没关系。
