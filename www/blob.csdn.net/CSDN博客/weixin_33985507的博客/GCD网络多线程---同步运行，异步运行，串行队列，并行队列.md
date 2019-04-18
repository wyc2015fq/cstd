# GCD网络多线程---同步运行，异步运行，串行队列，并行队列 - weixin_33985507的博客 - CSDN博客
2017年08月14日 11:39:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4

总结:同步(无论是串行还是并行)----不又一次开辟子线程
异步(无论是串行还是并行)----开辟子线程
    GCD：
        dispatch queue
主线程的main queue
并行队列 global dispatch queue
串行队列serial queues
一般用于按顺序同步訪问

#pragma mark - 载入多线程
- (void) _loadMutil
{
//GCD基于C语言
//1.主对列:(串行队列)
dispatch_queue_t mainQueue=dispatch_get_main_queue();
//2.全局并行队列
dispatch_queue_t concu=dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT,0);
//3.创建串行队列
dispatch_queue_t queueSerial=dispatch_queue_create("jrqueue1",DISPATCH_QUEUE_SERIAL);
//4.创建并行队列
dispatch_queue_t queueConcu=dispatch_queue_create("jrqueue2",DISPATCH_QUEUE_CONCURRENT);
//同步运行+串行队列
/*
     dispatch_sync(queueSerial, ^{
     [NSThread sleepForTimeInterval:2];
     NSLog(@"同步串行队列1-----%@",[NSThread currentThread]);
     });
     dispatch_sync(queueSerial, ^{
     [NSThread sleepForTimeInterval:2];
     NSLog(@"同步串行队列2-----%@",[NSThread currentThread]);
     });
     */
//同步运行+并行队列
/*
     dispatch_sync(queueConcu, ^{
     [NSThread sleepForTimeInterval:2];
     NSLog(@"同步并行队列1-----%@",[NSThread currentThread]);
     });
     dispatch_sync(queueConcu, ^{
     [NSThread sleepForTimeInterval:2];
     NSLog(@"同步并行队列2-----%@",[NSThread currentThread]);
     });
     */
//异步运行+串行队列-----开启一个子线程，且顺序运行
/*
     dispatch_async(queueSerial, ^{
     [NSThread sleepForTimeInterval:2];
     NSLog(@"异步串行队列1-----%@",[NSThread currentThread]);
     });
     dispatch_async(queueSerial, ^{
     [NSThread sleepForTimeInterval:2];
     NSLog(@"异步串行队列2-----%@",[NSThread currentThread]);
     });
     dispatch_async(queueSerial, ^{
     [NSThread sleepForTimeInterval:2];
     NSLog(@"异步串行队列3-----%@",[NSThread currentThread]);
     });
     */
//异步运行+并行队列----开启多个线程，且并发运行(无序)
/*
     dispatch_async(queueConcu, ^{
     [NSThread sleepForTimeInterval:2];
     NSLog(@"异步并行队列1-----%@",[NSThread currentThread]);
     });
     dispatch_async(queueConcu, ^{
     [NSThread sleepForTimeInterval:2];
     NSLog(@"异步并行队列2-----%@",[NSThread currentThread]);
     });
     dispatch_async(queueConcu, ^{
     [NSThread sleepForTimeInterval:2];
     NSLog(@"异步并行队列3-----%@",[NSThread currentThread]);
     });
     */
//主对列+同步运行-----死锁（将下面两个加入到主队列，等待前面的运行完毕（loadView。
loadData之类的），可是当运行到这一步时，形成死循环）
/*
     dispatch_sync(mainQueue, ^{
     [NSThread sleepForTimeInterval:2];
     NSLog(@"同步主队列1-----%@",[NSThread currentThread]);
     });
     dispatch_sync(mainQueue, ^{
     [NSThread sleepForTimeInterval:2];
     NSLog(@"同步主队列2-----%@",[NSThread currentThread]);
     });
     */
}
@end
将以上的方法，在viewDidLoad 中调用一下。看看效果怎样~
