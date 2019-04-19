# 滥用单例之dispatch_once死锁 - game3108的专栏 - CSDN博客
2017年03月13日 23:03:03[game3108](https://me.csdn.net/game3108)阅读数：501
个人分类：[iOS																[转载](https://blog.csdn.net/game3108/article/category/2926393)](https://blog.csdn.net/game3108/article/category/2844789)
 不错的一篇libdispatch源代码的文章，虽然看过，但记录一下。
转载连接：http://satanwoo.github.io/2016/04/11/dispatch-once/
### 现象
上周排查了一个bug，现象很简单，就是个Crash问题。但是读了一下crash Log以后，却发现堆栈报的错误信息却是第一次见到（吹牛的说，我在国内的iOS也能算第十二人了），包含以下还未符号化信息：
```
Application Specific Information:
com.xxx.yyy failed to scene-create in time
Elapsed total CPU time (seconds): hhh秒 (user hhh, system 0.000), k% CPU
Elapsed application CPU time (seconds): 0.h秒, k% CPU
Thread 0 name:  Dispatch queue: com.apple.main-thread
Thread 0:
0   libsystem_kernel.dylib            0x36cb2540 semaphore_wait_trap + 8
1   libsystem_platform.dylib          0x36d3d430 _os_semaphore_wait + 8
2   libdispatch.dylib                 0x36be04a6 dispatch_once_f + 250
3   xxxx                              偏移量 0x4000 + 947290
...
...
```
无符号化的crash 堆栈暂时不去管它，我们重点关注com.xxx.yyy failed to scene-create in time。如果理解无误的话，这句话提示我们：我们的应用程序在规定的时间没能加载成功，无法显示。看起来这个原因是启动加载过长直接被干掉。那么问题来了，原因具体是啥？
### 查看堆栈
首先我们需要符号化一下，这里涉及公司内部信息，所以我们自己构造个demo试试。
demo的代码很简单，如下：
```
#import "ManageA.h"
@implementation ManageA
+ (ManageA *)sharedInstance
{
    static ManageA *manager = nil;
    static dispatch_once_t token;
    dispatch_once(&token, ^{
        manager = [[ManageA alloc] init];
    });
    return manager;
}
- (instancetype)init
{
    self = [super init];
    if (self) {
        [ManageB sharedInstance];
    }
    return self;
}
@end
@implementation ManageB
+ (ManageB *)sharedInstance
{
    static ManageB *manager = nil;
    static dispatch_once_t token;
    dispatch_once(&token, ^{
        manager = [[ManageB alloc] init];
    });
    return manager;
}
- (instancetype)init
{
    self = [super init];
    if (self) {
        [ManageA sharedInstance];
    }
    return self;
}
```
运行后的堆栈基本如下：
```
#0    0x000000011054acd2 in semaphore_wait_trap ()
#1    0x00000001101b1b1a in _dispatch_thread_semaphore_wait ()
#2    0x00000001101b1d48 in dispatch_once_f ()
#3    0x000000010d01c857 in _dispatch_once [inlined] at once.h:68
#4    0x000000010d01c839 in +[ManageA sharedInstance] at ManageA.m:18
#5    0x000000010d01cad8 in -[ManageB init] at ManageA.m:54
#6    0x000000010d01ca42 in __25+[ManageB sharedInstance]_block_invoke at ManageA.m:44
#7    0x00000001101c649b in _dispatch_client_callout ()
#8    0x00000001101b1e28 in dispatch_once_f ()
#9    0x000000010d01c9e7 in _dispatch_once [inlined] at once.h:68
#10    0x000000010d01c9c9 in +[ManageB sharedInstance] at ManageA.m:43
#11    0x000000010d01c948 in -[ManageA init] at ManageA.m:29
#12    0x000000010d01c8b2 in __25+[ManageA sharedInstance]_block_invoke at ManageA.m:19
#13    0x00000001101c649b in _dispatch_client_callout ()
#14    0x00000001101b1e28 in dispatch_once_f ()
#15    0x000000010d01c857 in _dispatch_once [inlined] at once.h:68
#16    0x000000010d01c839 in +[ManageA sharedInstance] at /ManageA.m:18
#17    0x000000010d01c5cc in -[AppDelegate application:didFinishLaunchingWithOptions:]         at /AppDelegate.m:21
```
从中我们可以发现，的确在这段调用栈中，出现了多次敏感字样`sharedInstance`和`dispatch_once_f`字样。
在查阅相关资料后，感觉是`dispatch_once_f`函数造成了信号量的永久等待，从而引发死锁。那么，为什么`dispatch_once`会死锁呢？以前说的最安全的单例构造方式还正确不正确呢？
所以，我们一起来看看下面关于`dispatch_once`的源码分析。
### dispatch_once源码分析
从[libdispatch]()获取最新版本代码，进入对应的文件once.c。去除注释后代码如下，共66行代码，但是真的是有很多奇妙的地方。
```
#include "internal.h"
#undef dispatch_once
#undef dispatch_once_f
struct _dispatch_once_waiter_s {
    volatile struct _dispatch_once_waiter_s *volatile dow_next;
    _dispatch_thread_semaphore_t dow_sema;
};
#define DISPATCH_ONCE_DONE ((struct _dispatch_once_waiter_s *)~0l)
#ifdef __BLOCKS__
// 1. 我们的应用程序调用的入口
void
dispatch_once(dispatch_once_t *val, dispatch_block_t block)
{
    struct Block_basic *bb = (void *)block;
    // 2. 内部逻辑
    dispatch_once_f(val, block, (void *)bb->Block_invoke);
}
#endif
DISPATCH_NOINLINE
void
dispatch_once_f(dispatch_once_t *val, void *ctxt, dispatch_function_t func)
{
    struct _dispatch_once_waiter_s * volatile *vval =
            (struct _dispatch_once_waiter_s**)val;
    // 3. 地址类似于简单的哨兵位
    struct _dispatch_once_waiter_s dow = { NULL, 0 };
    // 4. 在Dispatch_Once的block执行期进入的dispatch_once_t更改请求的链表
    struct _dispatch_once_waiter_s *tail, *tmp;
    // 5.局部变量，用于在遍历链表过程中获取每一个在链表上的更改请求的信号量
    _dispatch_thread_semaphore_t sema;
    // 6. Compare and Swap（用于首次更改请求）
    if (dispatch_atomic_cmpxchg(vval, NULL, &dow)) {
        dispatch_atomic_acquire_barrier();
        // 7.调用dispatch_once的block
        _dispatch_client_callout(ctxt, func);
        dispatch_atomic_maximally_synchronizing_barrier();
        //dispatch_atomic_release_barrier(); // assumed contained in above
        // 8. 更改请求成为DISPATCH_ONCE_DONE(原子性的操作)
        tmp = dispatch_atomic_xchg(vval, DISPATCH_ONCE_DONE);
        tail = &dow;
        // 9. 发现还有更改请求，继续遍历
        while (tail != tmp) {
            // 10. 如果这个时候tmp的next指针还没更新完毕，等一会
            while (!tmp->dow_next) {
                _dispatch_hardware_pause();
            }
            // 11. 取出当前的信号量，告诉等待者，我这次更改请求完成了，轮到下一个了
            sema = tmp->dow_sema;
            tmp = (struct _dispatch_once_waiter_s*)tmp->dow_next;
            _dispatch_thread_semaphore_signal(sema);
        }
    } else {
        // 12. 非首次请求，进入这块逻辑块
        dow.dow_sema = _dispatch_get_thread_semaphore();
        for (;;) {
            // 13. 遍历每一个后续请求，如果状态已经是Done，直接进行下一个
            // 同时该状态检测还用于避免在后续wait之前，信号量已经发出(signal)造成
            // 的死锁
            tmp = *vval;
            if (tmp == DISPATCH_ONCE_DONE) {
                break;
            }
            dispatch_atomic_store_barrier();
            // 14. 如果当前dispatch_once执行的block没有结束，那么就将这些
            // 后续请求添加到链表当中
            if (dispatch_atomic_cmpxchg(vval, tmp, &dow)) {
                dow.dow_next = tmp;
                _dispatch_thread_semaphore_wait(dow.dow_sema);
            }
        }
        _dispatch_put_thread_semaphore(dow.dow_sema);
    }
}
```
根据以上注释对源代码的分析，我们可以大致知道如下几点：
- `dispatch_once`并不是简单的只执行一次那么简单
- `dispatch_once`本质上可以接受多次请求，会对此维护一个请求链表
- 
如果在block执行期间，多次进入调用同类的`dispatch_once`函数（即单例函数），会导致整体链表无限增长，造成永久性死锁。(其实只要进入两次就完蛋，其原因在于`block_invoke`的完成依赖于第二次进入的请求的完成，而第二次请求的完成又必须依赖之前信号量的出发。可是第一次block不结束，信号量压根不会触发)
### 备注
- 
根据以上分析，相对应地写了一个简易的死锁Demo，就是在两个单例的初始化调用中直接相互调用。A<->B。也许这个Demo过于简单，大家轻易不会犯。但是如果是A->B->C->A，甚至是更多个模块的相互引用，那又该如何轻易避免呢？
- 
以上的Demo，如果在Xcode模拟器测试环境下，是不会死锁从而导致应用启动被杀。这是因为模拟器不具备守护进程，如果要观察现象，可以输出Log或者直接利用真机进行测试。
- 
有时候，启动耗时是因为占用了太多的CPU资源。但是从我们的Crash Log中可以发现，我们仅仅占用了Elapsed application CPU time (seconds): 0.h秒, k% CPU。通过这个，我们也可以发现，CPU占用率高并不是导致启动阶段APP
 Crash的唯一原因。
### 反思
虽然这次的问题直接原因是`dispatch_once`引出的死锁问题，但是个人认为，这却是滥用单例造成的后果。各位可以打开自己公司的app源代码查看一下，究竟存在着多少的单例。
实话实说，单例和全局变量几乎没有任何区别，不仅仅占用了全生命周期的内存，还对解耦造成了巨大的负作用。写起来容易，但是对于整个项目的架构梳理却是有着巨大的影响，因为在不读完整个相关代码的前提下，你压根不知道究竟哪里会触发单例的调用。
因此在这里，谈谈个人认为可以不使用单例的几个方面：
- 
仅仅使用一次的模块，可以不使用单例，可以采用在对应的周期内维护成员实例变量进行替换。
- 
和状态无关的模块，可以采用静态（类）方法直接替换。
- 
可以通过页面跳转进行依赖注入的模块，可以采用依赖注入或者变量传递等方式解决。
当然，的确有一些情况我们仍然需要使用单例。那在这种情况，也请将`dispatch_once`调用的block内减少尽可能多的任务，最好是仅仅负责初始化，剩下的配置、调用等等在后续进行。
