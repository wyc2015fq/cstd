# iOS多线程的基本使用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年01月26日 10:42:20[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：391
一、NSThread：
程序就是一段代码,是静态的概念
进程是运行起来的程序，是动态的概念,进程需要占内存空间
线程是进程的基本单位,一个进程至少有一个线程,iOS程序默认有一个主线程，用来显示和操作UI，主线程由系统自动创建，有系统管理。如果主线程不能满足我们的需求，可以手动创建线程，手动创建的线程需要我们自己管理。
1.例：让一个线程延迟10秒运行
- (void)viewDidLoad {
    [superviewDidLoad];
// Do any additional setup after loading the view, typically from a nib.
//按钮一
UIButton *btn1 = [MyUtilcreateBtnFrame:CGRectMake(100,100,
80,40)
title:@"按钮一"bgImageName:niltarget:selfaction:@selector(clickBtnOne:)];
    [self.viewaddSubview:btn1];
//按钮二
UIButton *btn2 = [MyUtilcreateBtnFrame:CGRectMake(100,200,
80,40)
title:@"按钮二"bgImageName:niltarget:selfaction:@selector(clickBtnTwo:)];
    [self.viewaddSubview:btn2];
}
- (void)clickBtnOne:(id)sender
{
//模拟耗时较长的操作
//例如实际中的网络请求或者数据库的操作
//让线程睡眠十秒
    [NSThreadsleepForTimeInterval:10];
//如果不使用多线程，会有界面假死的现象
//用户体验不好
//所有在这种情况下我们需要使用多线程
}
- (void)clickBtnTwo:(id)sender
{
NSLog(@"%s",__func__);
}
![](https://img-blog.csdn.net/20160126104604403?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2.线程的优先级：
//用线程的这个属性来修改线程的优先级，属性值越大，执行的次数越多
   t1.threadPriority =0;
- (void)viewDidLoad {
    [superviewDidLoad];
// Do any additional setup after loading the view, typically from a nib.
//线程一
NSThread *t1 = [[NSThreadalloc]
initWithTarget:selfselector:@selector(threadOne)object:nil];
//修改优先级
//优先级的值在0-1之间,优先级越高，执行的次数相对来说会越多
    t1.threadPriority =0;
    [t1 start];
//线程二
NSThread *t2 = [[NSThreadalloc]
initWithTarget:selfselector:@selector(threadTwo)object:nil];
//修改优先级，线和二大于线程一，线程二先执行完成
    t2.threadPriority =1;
    [t2 start];
}
- (void)threadOne
{
for (int i=0; i<100;
 i++) {
NSLog(@"线程一:%d", i);
    }
}
- (void)threadTwo
{
for (int i=0; i<100;
 i++) {
NSLog(@"线程二:%d", i);
    }
}
3. 线程锁：
第一种方法用：@synchronized加线程锁
-(void)withDraw:(float)money
{
//多个线程修改同一块资源的时候，会出现问题
//我们需要用线程锁的方式解决
@synchronized(self){
if (_money >= money) {
//模拟取钱的时间
            [NSThreadsleepForTimeInterval:1];
_money -= money;
NSLog(@"%@取了%f元",[NSThreadcurrentThread].name,money);
        }else{
NSLog(@"余额不足");
        }
    }
}
@end

第二种方法：
用NSLock 来实现加锁和解锁
@implementation MyAccount
{
//线程锁
NSLock *_myLock;
}
-(instancetype)init
{
if (self = [superinit])
 {
_myLock = [[NSLockalloc]
init];
    }
returnself;
}
-(void)withDraw:(float)money
{
//多个线程修改同一块资源的时候，会出现问题
//我们需要用线程锁的方式解决
//加锁
    [_myLocklock];
if (_money >= money) {
//模拟取钱的时间
        [NSThreadsleepForTimeInterval:1];
_money -= money;
NSLog(@"%@取了%f元",[NSThreadcurrentThread].name,money);
    }else{
NSLog(@"余额不足");
    }
//解锁
    [_myLockunlock];
}
@end
二：NSOpreation:
1.用NSOpreation实现在线下载图片
#import "ViewController.h"
#import "ImageOperation.h"
@interfaceViewController ()
{
NSOperationQueue *_queue;
}
@end
@implementation ViewController
- (void)viewDidLoad {
    [superviewDidLoad];
// Do any additional setup after loading the view, typically from a nib.
//创建图片视图
UIImageView *myImageView = [[UIImageViewalloc]
initWithFrame:CGRectMake(40,100,
300,300)];
    [self.viewaddSubview:myImageView];
//使用NSOperatio自定制类型实现多线程
//1.创建一个队列
_queue = [[NSOperationQueuealloc]
init];
//队列里面同时执行的线程最大数量
//例如往队列里面加了20个线程，只会执行10个，其他的线程要等这10个中的某个执行完成之后，才能执行
_queue.maxConcurrentOperationCount =10;
//2.创建ImageOperation对象
ImageOperation *op = [[ImageOperationalloc]
init];
//设置下载链接
    op.urlString =@"http://g.hiphotos.baidu.com/image/pic/item/4034970a304e251fb3145e6ca586c9177e3e5346.jpg";
//设置显示的视图
    op.imgView = myImageView;
//添加到队列
    [_queueaddOperation:op];
//ASIHttpRequest: NSOperation
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
#import "ImageOperation.h"
@implementation ImageOperation
//使用NSOperation自定制对象创建线程时的执行体
-(void)main
{
//下载图片
NSURL *url = [NSURLURLWithString:self.urlString];
NSData *data = [NSDatadataWithContentsOfURL:url];
//回到主线程修改UI
    [selfperformSelectorOnMainThread:@selector(refreshUI:)withObject:data
waitUntilDone:YES];
}
- (void)refreshUI:(NSData *)data
{
self.imgView.image = [UIImageimageWithData:data];
}
@end
2.使用NSOperation创建多线程：
- (void)viewDidLoad {
    [superviewDidLoad];
// Do any additional setup after loading the view, typically from a nib.
//NSOperation实现多线程
//1.创建队列
_queue = [[NSOperationQueuealloc]
init];
//2.创建NSInvocationOperation对象
/*
第一个参数:线程执行体方法所属的对象
第二个参数:线程执行体方法
第三个参数:线程执行体方法传递的实参
     */
NSInvocationOperation *op1 = [[NSInvocationOperationalloc]
initWithTarget:selfselector:@selector(threadOne)object:nil];
//线程结束之后执行的代码块
    [op1 setCompletionBlock:^{
NSLog(@"线程一结束");
    }];
//3.开启线程
    [_queueaddOperation:op1];
//第二种实现线程的方式
NSBlockOperation *op2 = [NSBlockOperationblockOperationWithBlock:^{
//线程的执行体
for (int i=0; i<100;
 i++) {
NSLog(@"执行线程二:%d",i);
        }
    }];
//线程执行结束后调用的代码块
    [op2 setCompletionBlock:^{
NSLog(@"线程二结束");
    }];
//添加到队列里面
    [_queueaddOperation:op2];
}
//线程执行体方法
- (void)threadOne
{
for (int i=0; i<100;
 i++) {
NSLog(@"执行了线程一:%d", i);
    }
}
三：GCD：
1.用GCD实现多线程:
#import "ViewController.h"
@interfaceViewController ()
@end
@implementation ViewController
- (void)viewDidLoad {
    [superviewDidLoad];
// Do any additional setup after loading the view, typically from a nib.
//1.同一块代码执行多次
//[self testApply];
//2.代码只执行一次
//    [self testOnce];
//    [self testOnce];
//    [self testOnce];
//    [self testOnce];
//3.在一段时间之后执行代码块
//[self testAfter];
//4.线程组(group)
//[self testGroup];
//5.在某几个线程都执行完成之后，并在另外几个线程都执行完成之前,需要执行一些代码
    [selftestBarrier];
}
//5.在某几个线程都执行完成之后，并在另外几个线程都执行完成之前,需要执行一些代码
- (void)testBarrier
{
//队列不能是系统的全局队列
dispatch_queue_t concurrentQueue =dispatch_queue_create("concurrentQueue",DISPATCH_QUEUE_CONCURRENT);
//线程一
dispatch_async(concurrentQueue, ^{
for (int i=0; i<100;
 i++) {
NSLog(@"线程一:%d", i);
        }
    });
//线程二
dispatch_async(concurrentQueue, ^{
for (int i=0; i<100;
 i++) {
NSLog(@"线程二:%d", i);
        }
    });
//在线程一和线程二都执行完成之后，并且在线程三执行之前，调用一段代码块
dispatch_barrier_async(concurrentQueue, ^{
NSLog(@"barrier");
    });
//线程三
dispatch_async(concurrentQueue, ^{
for (int i=0; i<100;
 i++) {
NSLog(@"线程三:%d", i);
        }
    });
}
//4.多个线程都执行之后去执行一些代码
- (void)testGroup
{
//线程组
dispatch_group_t group =dispatch_group_create();
/*
第一个参数:线程组
第二个参数:线程所在的队列
第三个参数:线程的执行体
     */
dispatch_group_async(group,dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT,0),
 ^{
for (int i=0; i<100;
 i++) {
NSLog(@"执行了线程一:%d", i);
        }
    });
dispatch_group_async(group,dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT,0),
 ^{
for (int i=0; i<100;
 i++) {
NSLog(@"执行了线程二:%d", i);
        }
    });
//在该组的所有线程都执行结束时做执行某段代码
dispatch_group_notify(group,dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT,0),
 ^{
NSLog(@"所有线程都执行完成");
    });
}
//3.在一段时间之后执行代码块
- (void)testAfter
{
NSLog(@"执行之前");
//DISPATCH_TIME_NOW:表示当前的时间
//当前时间的十秒之后
dispatch_time_t myTime =dispatch_time(DISPATCH_TIME_NOW,NSEC_PER_SEC*10);
//队列
//创建一个并行的队列
dispatch_queue_t concurrentQueue =dispatch_queue_create("concurrentQueue",DISPATCH_QUEUE_CONCURRENT);
/*
第一个参数:代码块执行的时间
第二个参数:代码块的线程所在的队列
第三个参数:执行的代码块
     */
dispatch_after(myTime, concurrentQueue, ^{
NSLog(@"执行了代码块");
    });
}
//2.代码只执行一次
//通常用来实现单例
- (void)testOnce
{
staticdispatch_once_t onceToken;
dispatch_once(&onceToken, ^{
NSLog(@"执行了一次");
    });
}
//1.同一块代码执行多次
- (void)testApply
{
/*
第一个参数:代码块执行多少次
第二个参数:线程所在的队列
第三个参数:线程的执行体
     */
dispatch_queue_t globalQueue =dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT,0);
//参数表示当前是第几次执行
dispatch_apply(10, globalQueue, ^(size_t time) {
//线程的执行体
NSLog(@"执行到了第%ld次",
 time);
    });
}
- (void)didReceiveMemoryWarning {
    [superdidReceiveMemoryWarning];
// Dispose of any resources that can be recreated.
}
@end
2.GCD实现多线程2：
- (void)viewDidLoad {
    [superviewDidLoad];
// Do any additional setup after loading the view, typically from a nib.
//GCD实现多线程
//一、GCD的队列
//1.主线程所在的串行队列
//系统自动创建了这个队列，我们只要获取后使用
dispatch_queue_t mainQueue =dispatch_get_main_queue();
//2.系统的全局并行队列
//系统自动创建了这个队列，我们只能获取后使用
/*
第一个参数:队列的优先级
第二个参数:APPLE公司保留的参数
     */
dispatch_queue_t globalQueue =dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT,0);
//3.自己创建的队列
//1)串行队列
/*
第一个参数:队列的标识符
第二个参数:用来区分是串行队列还是并行队列
     DISPATCH_QUEUE_SERIAL:串行队列
     DISPATCH_QUEUE_CONCURRENT:并行队列
     */
dispatch_queue_t serialQueue =dispatch_queue_create("serialQueue",DISPATCH_QUEUE_SERIAL);
//2)并行队列
dispatch_queue_t concurrentQueue =dispatch_queue_create("concurrentQueue",DISPATCH_QUEUE_CONCURRENT);
//二、GCD创建线程
//1、异步将线程的执行体代码放到队列执行
/*
第一个参数:队列
第二个参数:线程的执行体代码
     */
dispatch_queue_t queueOne =dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT,0);
dispatch_async(queueOne, ^{
for (int i=0; i<100;
 i++) {
NSLog(@"执行了线程一:%d", i);
        }
    });
//2、同步将线程的执行体放到队列执行
dispatch_sync(queueOne, ^{
for (int i=0; i<100;
 i++) {
NSLog(@"执行了线程二:%d", i);
        }
    });
}
- (void)didReceiveMemoryWarning {
    [superdidReceiveMemoryWarning];
// Dispose of any resources that can be recreated.
}
@end
3.GCD的串行和并行队列：
#import "ViewController.h"
@interfaceViewController ()
@end
@implementation ViewController
- (void)viewDidLoad {
    [superviewDidLoad];
// Do any additional setup after loading the view, typically from a nib.
//1.以同步的方式在串行队列创建两个线程
//[self testSyncWithSerialQueue];
//2.以同步的方式在并行队列创建两个线程
//[self testSyncWithConcurrentQueue];
//3.以异步的方式在串行队列创建两个线程
//[self testAsyncWithSerialQueue];
//4.以异步的方式在并行队列创建两个线程
    [selftestAsyncWithConcurrentQueue];
}
//4.以异步的方式在并行队列创建两个线程
//前面的线程和后面的线程同时执行
- (void)testAsyncWithConcurrentQueue
{
//创建并行队列
dispatch_queue_t concurrentQueue =dispatch_queue_create("concurrentQueue",DISPATCH_QUEUE_CONCURRENT);
//线程一
dispatch_async(concurrentQueue, ^{
for (int i=0; i<100;
 i++) {
NSLog(@"线程一:%d", i);
        }
    });
//线程二
dispatch_async(concurrentQueue, ^{
for (int i=0; i<100;
 i++) {
NSLog(@"线程二:%d", i);
        }
    });
}
//3.以异步的方式在串行队列创建两个线程
//前面的线程执行完成，后面的线程才开始执行
- (void)testAsyncWithSerialQueue
{
//串行队列
dispatch_queue_t serialQueue =dispatch_queue_create("myQueue",DISPATCH_QUEUE_SERIAL);
//线程一
dispatch_async(serialQueue, ^{
for (int i=0; i<100;
 i++) {
NSLog(@"线程一:%d", i);
        }
    });
//线程二
dispatch_async(serialQueue, ^{
for (int i=0; i<100;
 i++) {
NSLog(@"线程二:%d", i);
        }
    });
}
//2.以同步的方式在并行队列创建两个线程
//前面的线程执行结束，才开始执行后面的线程
- (void)testSyncWithConcurrentQueue
{
//获取全局并行队列
dispatch_queue_t globalQueue =dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT,0);
//线程一
dispatch_sync(globalQueue, ^{
for (int i=0; i<100;
 i++) {
NSLog(@"线程一:%d", i);
        }
    });
//线程二
dispatch_sync(globalQueue, ^{
for (int i=0; i<100;
 i++) {
NSLog(@"线程二:%d", i);
        }
    });
}
//1.以同步的方式在串行队列创建两个线程
//前面的线程执行完成之后，才开始执行后面的线程
- (void)testSyncWithSerialQueue
{
//创建一个串行队列
dispatch_queue_t serialQueue =dispatch_queue_create("serialQueue",DISPATCH_QUEUE_SERIAL);
//线程一
dispatch_sync(serialQueue, ^{
for (int i=0; i<100;
 i++) {
NSLog(@"线程一:%d", i);
        }
    });
//线程二
dispatch_sync(serialQueue, ^{
for (int i=0; i<100;
 i++) {
NSLog(@"线程二:%d", i);
        }
    });
}
