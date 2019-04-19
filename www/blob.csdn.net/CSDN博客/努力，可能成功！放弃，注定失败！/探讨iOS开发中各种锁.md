# 探讨iOS开发中各种锁 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年12月22日 15:03:41[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：862标签：[iOS 资源大全																[iOS学习资料																[iOS常用方法																[探讨iOS开发中各种锁](https://so.csdn.net/so/search/s.do?q=探讨iOS开发中各种锁&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS常用方法&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
### 摘要
平常项目开发中需要手动加锁的地方少之又少，而且一般是简单粗暴使用@synchronized搞定，不过打算封装框架时就会发现远不满足。本文的目的是希望站在iOS开发中锁的使用之上的层面分析，探讨各种加锁方式的大致实现原理以及它们之间的区别和性能对比。由于加锁必然涉及到多线程相关知识，所以本文涉猎的知识点可能较多，本着相互学习探讨的心态，希望大家多多指正。
首先借鉴一张ibireme各种锁性能对比图镇楼，同时也是我们本文的叙述脉络：
![](http://upload-images.jianshu.io/upload_images/674591-176434d65ad6f5b6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
来源：ibireme
上图，Y轴从下到上性能越来越来，越来越接近底层，平时使用也越来越少o(╯□╰)o。接下来我们将从我们最熟悉的@synchronized说起，逐步深入，步步为营。
在开始分析之前，有几个概念需要先拎出来溜溜，方便后文理解。
## 时间片轮转调度算法
[时间片轮转调度算法](http://baike.baidu.com/view/3186651.htm)
这是目前操作系统中大量使用的线程管理方式，大致就是操作系统会给每个线程分配一段时间片（通常100ms左右）这些线程都被放在一个队列中，cpu只需要维护这个队列，当队首的线程时间片耗尽就会被强制放到队尾等待，然后提取下一个队首线程执行。
## 原子操作
“原子”：一般指最小粒度，不可分割；原子操作也就是不可分割，不可中断的操作。我们最重要是知道这个概念，至于具体实现就不在本文的讨论范围。可以参考以下文章：
[http://www.jianshu.com/p/cb7b726e943c](http://www.jianshu.com/p/cb7b726e943c)
[http://southpeak.github.io/2014/10/17/osatomic-operation/](http://southpeak.github.io/2014/10/17/osatomic-operation/)
至此，我们正式进入iOS开发中各种锁的探讨。
## @synchronized
显然，这是我们最熟悉的加锁方式，因为这是OC层面的为我们封装的，使用起来简单粗暴。使用时 @synchronized 后面需要紧跟一个 OC 对象，它实际上是把这个对象当做锁来使用。这是通过一个哈希表来实现的，OC 在底层使用了一个互斥锁的数组(也就是锁池)，通过对对象去哈希值来得到对应的互斥锁。
对于@synchronized更深入的实现理解，这里[http://yulingtianxia.com/blog/2015/11/01/More-than-you-want-to-know-about-synchronized/](http://yulingtianxia.com/blog/2015/11/01/More-than-you-want-to-know-about-synchronized/)会有更好的答案。
## OSSpinLock 自旋锁
首选作为效率最优选择的OSSpinLock是自旋锁，在 bireme 的[不再安全的OSSpinLock](http://blog.ibireme.com/2016/01/16/spinlock_is_unsafe_in_ios/)文中，已经指出潜在的bug：优先级反转，因此目前不建议使用。[什么是优先级反转？](http://baike.baidu.com/link?url=T9Im_xVyniC5SdvZVH1Y1iEgkFhcjGm-Hnme803oWgRi3vZa1JVMy-RsljCPDuvPiOcWIrrUedw7CLYRjqhWtuO4CLSgugUUPoHrCKsB-T2GNionyGgXYRUaJ43joO6ZeGGvxIDsW6s2ham_x1vkv_)
自旋锁的目的是为了确保临界区只有一个线程可以访问，自旋锁的实现思路很简单，理论上来说只要定义一个全局变量，用来表示锁的可用情况即可，自旋锁是使用忙等机制。
```
bool lock = false; // 一开始没有锁上，任何线程都可以申请锁
do {
while(test_and_set(&lock); // test_and_set 是一个原子操作
Critical section  // 临界区
lock = false; // 相当于释放锁，这样别的线程可以进入临界区
Reminder section // 不需要锁保护的代码
}
```
如果临界区的执行时间过长，不建议使用自旋锁，因为在 while 循环中，线程处于忙等状态，白白浪费 CPU 时间，最终因为超时被操作系统抢占时间片。
## dispatch_semaphore 信号量
##### 使用
> 
**dispatch_semaphore_create(1)：**传入值必须>=0, 若传入为0则阻塞线程并等待timeout,时间到后会执行其后的语句
**dispatch_semaphore_wait(signal, overTime)：**可以理解为lock,会使得signal值-1
**dispatch_semaphore_signal(signal)：**可以理解为unlock,会使得signal值+1
##### 理解
> 
停车场剩余4个车位，那么即使同时来了四辆车也能停的下。如果此时来了五辆车，那么就有一辆需要等待。
**信号量的值（signal）**就相当于剩余车位的数目，dispatch_semaphore_wait函数就相当于来了一辆车，dispatch_semaphore_signal就相当于走了一辆车。停车位的剩余数目在初始化的时候就已经指明了（dispatch_semaphore_create（long value）），调用一次 dispatch_semaphore_signal，剩余的车位就增加一个；调用一次dispatch_semaphore_wait 剩余车位就减少一个；当剩余车位为
 0 时，再来车（即调用 dispatch_semaphore_wait）就只能等待。有可能同时有几辆车等待一个停车位。有些车主没有耐心，给自己设定了一段等待时间，这段时间内等不到停车位就走了，如果等到了就开进去停车。而有些车主就像把车停在这，所以就一直等下去。
##### 实现原理
参考[介绍 GCD 底层实现的文章](https://bestswifter.com/deep-gcd/)中简单描述了信号量 dispatch_semaphore_t 的实现原理，它最终会调用到 sem_wait 方法，这个方法在
[glibc](http://www.gnu.org/software/libc/) 中被实现如下:
```cpp
```cpp
int sem_wait (sem_t *sem) {
  int *futex = (int *) sem;
  if (atomic_decrement_if_positive (futex) > 0)
    return 0;
  int err = lll_futex_wait (futex, 0);
    return -1;
)
```
```
> 
首先会把信号量的值减一，并判断是否大于零。如果大于零，说明不用等待，所以立刻返回。具体的等待操作在lll_futex_wait
函数中实现，**lll** 是 low level lock 的简称。这个函数通过汇编代码实现，调用到SYS_futex
这个系统调用，使线程进入睡眠状态，主动让出时间片，这个函数在互斥锁的实现中，也有可能被用到。
主动让出时间片并不总是代表效率高。让出时间片会导致操作系统切换到另一个线程，这种上下文切换通常需要 10 微秒左右，而且至少需要两次切换。如果等待时间很短，比如只有几个微秒，忙等就比线程睡眠更高效。
可以看到，自旋锁和信号量的实现都非常简单，这也是两者的加解锁耗时分别排在第一和第二的原因。再次强调，加解锁耗时不能准确反应出锁的效率(比如时间片切换就无法发生)，它只能从一定程度上衡量锁的实现复杂程度。
## pthread_mutex 互斥锁
pthread 表示 POSIX thread，定义了一组跨平台的线程相关的 API，pthread_mutex 表示互斥锁。互斥锁的实现原理与信号量非常相似，不是使用忙等，而是阻塞线程并睡眠，需要进行上下文切换。
##### 常见用法
```cpp
```cpp
pthread_mutexattr_t attr;
pthread_mutexattr_init(&attr);
pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);  // 定义锁的属性
pthread_mutex_t mutex;
pthread_mutex_init(&mutex, &attr) // 创建锁
pthread_mutex_lock(&mutex); // 申请锁
    // 临界区
pthread_mutex_unlock(&mutex); // 释放锁
```
```
##### 互斥锁的实现
> 
互斥锁在申请锁时，调用了 pthread_mutex_lock 方法，它在不同的系统上实现各有不同，有时候它的内部是使用信号量来实现，即使不用信号量，也会调用到 lll_futex_wait 函数，从而导致线程休眠。
上文说到如果临界区很短，忙等的效率也许更高，所以在有些版本的实现中，会首先尝试一定次数(比如 1000 次)的 test_and_test，这样可以在错误使用互斥锁时提高性能。
## pthread_mutex(recursive) 递归锁
一般情况下，一个线程只能申请一次锁，也只能在获得锁的情况下才能释放锁，多次申请锁或释放未获得的锁都会导致崩溃。假设在已经获得锁的情况下再次申请锁，线程会因为等待锁的释放而进入睡眠状态，因此就不可能再释放锁，从而导致死锁。
然而这种情况经常会发生，比如某个函数申请了锁，在临界区内又递归调用了自己，由此也就引出了递归锁：**允许同一个线程在未释放其拥有的锁时反复对该锁进行加锁操作。**
递归锁的使用和pthread_mutex很类似，主要就是要设置锁的类型为PTHREAD_MUTEX_RECURSIVE即可。
> 
由于 pthread_mutex 有多种类型，可以支持递归锁等，因此在申请加锁时，需要对锁的类型加以判断，这也就是为什么它和信号量的实现类似，但效率略低的原因。
## NSLock
NSLock 是 Objective-C 以对象的形式暴露给开发者的一种锁，它的实现非常简单，通过宏，定义了 lock 方法:
```
#define    MLOCK \
- (void) lock\
{\
  int err = pthread_mutex_lock(&_mutex);\
  // 错误处理 ……
}
```
NSLock只是在内部封装了一个pthread_mutex，属性为PTHREAD_MUTEX_ERRORCHECK，它会损失一定性能换来错误提示。这里使用宏定义的原因是，OC 内部还有其他几种锁，他们的 lock 方法都是一模一样，仅仅是内部pthread_mutex互斥锁的类型不同。通过宏定义，可以简化方法的定义。
NSLock比pthread_mutex略慢的原因在于它需要经过方法调用，同时由于缓存的存在，多次方法调用不会对性能产生太大的影响。
## NSRecursiveLock 递归锁
前文我们已经介绍过，递归锁也是通过 pthread_mutex_lock 函数来实现，在函数内部会判断锁的类型。NSRecursiveLock 与 NSLock 的区别在于内部封装的 pthread_mutex_t 对象的类型不同，前者的类型为 PTHREAD_MUTEX_RECURSIVE。
## NSConditionLock 条件锁
NSCondition的底层是通过条件变量(condition variable)pthread_cond_t来实现的。条件变量有点像信号量，提供了线程阻塞与信号机制，因此可以用来阻塞某个线程，并等待某个数据就绪，随后唤醒线程，比如常见的生产者-消费者模式。[关于生产者消费者问题](https://zh.wikipedia.org/wiki/%E7%94%9F%E4%BA%A7%E8%80%85%E6%B6%88%E8%B4%B9%E8%80%85%E9%97%AE%E9%A2%98)
##### 条件变量使用
```cpp
```cpp
void consumer () { // 消费者
    pthread_mutex_lock(&mutex);
    while (data == NULL) {
        pthread_cond_wait(&condition_variable_signal, &mutex); // 等待数据
    }
    // --- 有新的数据，以下代码负责处理 ↓↓↓↓↓↓
    // temp = data;
    // --- 有新的数据，以上代码负责处理 ↑↑↑↑↑↑
    pthread_mutex_unlock(&mutex);
}
void producer () {
    pthread_mutex_lock(&mutex);
    // 生产数据
    pthread_cond_signal(&condition_variable_signal); // 发出信号给消费者，告诉他们有了新的数据
    pthread_mutex_unlock(&mutex);
}
```
```
“如果不用互斥锁，只用条件变量会有什么问题呢？”。问题在于，temp = data; 这段代码不是线程安全的，也许在你把 data 读出来以前，已经有别的线程修改了数据。因此我们需要保证消费者拿到的数据是线程安全的。
##### 这也就是NSCondition的实现原理：
NSCondition 其实是封装了一个互斥锁和条件变量， 它把前者的 lock 方法和后者的 wait/signal 统一在 NSCondition 对象中，暴露给使用者。NSCondition的加解锁过程与 NSLock 几乎一致，理论上来说耗时也应该一样(实际测试也是如此)。在图中显示它耗时略长，有可能是测试者在每次加解锁的前后还附带了变量的初始化和销毁操作。
```
- (void) signal {
  pthread_cond_signal(&_condition);
}
// 其实这个函数是通过宏来定义的，展开后就是这样
- (void) lock {
  int err = pthread_mutex_lock(&_mutex);
}
```
##### NSConditionLock是通过NSCondition实现的
##### 使用
```cpp
```cpp
NSConditionLock *cLock = [[NSConditionLock alloc] initWithCondition:0];
//线程1
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
    if([cLock tryLockWhenCondition:0]){
        NSLog(@"线程1");
       [cLock unlockWithCondition:1];
    }else{
         NSLog(@"失败");
    }
});
//线程2
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
    [cLock lockWhenCondition:3];
    NSLog(@"线程2");
    [cLock unlockWithCondition:2];
});
//线程3
dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
    [cLock lockWhenCondition:1];
    NSLog(@"线程3");
    [cLock unlockWithCondition:3];
});
```
```
> 
我们在初始化 NSConditionLock 对象时，给了他的标示为0
执行tryLockWhenCondition:时，我们传入的条件标示也是0,所 **以线程1** 加锁成功
执行unlockWithCondition:时，**这时候会把condition由0修改为1因为condition修改为了1， 会先走到 **线程3**，然后 **线程3** 又将condition修改为3最后 走了 **线程2** 的流程
##### 实现原理
NSConditionLock 借助 NSCondition 来实现，它的本质就是一个生产者-消费者模型。“条件被满足”可以理解为生产者提供了新的内容。NSConditionLock 的内部持有一个 NSCondition 对象，以及 _condition_value 属性，在初始化时就会对这个属性进行赋值:
```
// 模拟代码
- (id) initWithCondition: (NSInteger)value {
    if (nil != (self = [super init])) {
        _condition = [NSCondition new]
        _condition_value = value;
    }
    return self;
}
```
它的 lockWhenCondition 方法其实就是消费者方法:
```
- (void) lockWhenCondition: (NSInteger)value {
    [_condition lock];
    while (value != _condition_value) {
        [_condition wait];
    }
}
```
对应的 unlockWhenCondition 方法则是生产者，使用了 broadcast 方法通知了所有的消费者:
```
```
- (void) unlockWithCondition: (NSInteger)value {
    _condition_value = value;
    [_condition broadcast];
    [_condition unlock];
}
```
```
## 小结
至此，本文已经对iOS开发中常用的8种锁对了简单的探讨和学习，有几个关键点需要强调的
（1）加解锁速度不表示锁的效率，只表示加解锁操作在执行时的复杂程度，因为还需要考虑时间片切换耗时等因素
（2）实际开发中，并不是总要选择性能最好的锁来实现，需要根据业务需求和开发成本，代码维护等方面综合选择，这也是@synchronized和NSLock的原因。
（3）关于死锁的理解，受文章篇幅影响，暂不分析，可以参考:[]()[http://www.superqq.com/blog/2015/10/16/five-case-know-gcd/](http://www.superqq.com/blog/2015/10/16/five-case-know-gcd/)
文／MrLeoZou（简书作者）
原文链接：http://www.jianshu.com/p/6773757a6cd5
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
