# iOS并发编程--8种加锁方式及比较 - 月若无涯的专栏 - CSDN博客
2015年05月18日 15:03:16[月若无涯](https://me.csdn.net/u010124617)阅读数：2998
iOS开发中不可避免的会遇到加锁的情况，今天就来比较一下各种加锁方式。
目前我所知道的有如下几种加锁方式： 
1. @synchronized 关键字加锁 
2. NSLock 对象锁 
3. NSCondition  
4. NSConditionLock 条件锁 
5. NSRecursiveLock 递归锁 
6. pthread_mutex 互斥锁（C语言） 
7. dispatch_semaphore 信号量实现加锁（GCD） 
8. OSSpinLock （暂不建议使用，原因参见[这里](http://blog.ibireme.com/2016/01/16/spinlock_is_unsafe_in_ios/)）
这里分别使用8种方式加锁解锁1千万次，执行方法如下：
```
#import <pthread.h>
#import <libkern/OSAtomic.h>
- (void)runLock{
     CFTimeInterval timeBefore;
    CFTimeInterval timeCurrent;
    NSUInteger i;
    NSUInteger count = 1000*10000;//执行一千万次
    //@synchronized
    id obj = [[NSObject alloc]init];;
    timeBefore = CFAbsoluteTimeGetCurrent();
    for(i=0; i<count; i++){
        @synchronized(obj){
        }
    }
    timeCurrent = CFAbsoluteTimeGetCurrent();
    printf("@synchronized used : %f\n", timeCurrent-timeBefore);
    //NSLock
    NSLock *lock = [[NSLock alloc]init];
    timeBefore = CFAbsoluteTimeGetCurrent();
    for(i=0; i<count; i++){
        [lock lock];
        [lock unlock];
    }
    timeCurrent = CFAbsoluteTimeGetCurrent();
    printf("NSLock used : %f\n", timeCurrent-timeBefore);
    //NSCondition
    NSCondition *condition = [[NSCondition alloc]init];
    timeBefore = CFAbsoluteTimeGetCurrent();
    for(i=0; i<count; i++){
        [condition lock];
        [condition unlock];
    }
    timeCurrent = CFAbsoluteTimeGetCurrent();
    printf("NSCondition used : %f\n", timeCurrent-timeBefore);
    //NSConditionLock
    NSConditionLock *conditionLock = [[NSConditionLock alloc]init];
    timeBefore = CFAbsoluteTimeGetCurrent();
    for(i=0; i<count; i++){
        [conditionLock lock];
        [conditionLock unlock];
    }
    timeCurrent = CFAbsoluteTimeGetCurrent();
    printf("NSConditionLock used : %f\n", timeCurrent-timeBefore);
    //NSRecursiveLock
    NSRecursiveLock *recursiveLock = [[NSRecursiveLock alloc]init];
    timeBefore = CFAbsoluteTimeGetCurrent();
    for(i=0; i<count; i++){
        [recursiveLock lock];
        [recursiveLock unlock];
    }
    timeCurrent = CFAbsoluteTimeGetCurrent();
    printf("NSRecursiveLock used : %f\n", timeCurrent-timeBefore);
    //pthread_mutex
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    timeBefore = CFAbsoluteTimeGetCurrent();
    for(i=0; i<count; i++){
        pthread_mutex_lock(&mutex);
        pthread_mutex_unlock(&mutex);
    }
    timeCurrent = CFAbsoluteTimeGetCurrent();
    printf("pthread_mutex used : %f\n", timeCurrent-timeBefore);
    //dispatch_semaphore
    dispatch_semaphore_t semaphore = dispatch_semaphore_create(1);
    timeBefore = CFAbsoluteTimeGetCurrent();
    for(i=0; i<count; i++){
        dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
        dispatch_semaphore_signal(semaphore);
    }
    timeCurrent = CFAbsoluteTimeGetCurrent();
    printf("dispatch_semaphore used : %f\n", timeCurrent-timeBefore);
    //OSSpinLockLock
    OSSpinLock spinlock = OS_SPINLOCK_INIT;
    timeBefore = CFAbsoluteTimeGetCurrent();
    for(i=0; i<count; i++){
        OSSpinLockLock(&spinlock);
        OSSpinLockUnlock(&spinlock);
    }
    timeCurrent = CFAbsoluteTimeGetCurrent();
    printf("OSSpinLock used : %f\n", timeCurrent-timeBefore);
}
```
执行结果： 
![执行1千万次的结果](https://img-blog.csdn.net/20150518144954562)
由图可以发现： 
OSSpinLock的性能最好（暂不建议使用），GCD的dispatch_semaphore紧随其后； 
NSConditionLock和@synchronized性能较差；
PS： 
1. 需要注意的是这里仅仅是对各种锁直接Lock和Unlock的性能测试，其中部分锁的使用条件上还是有细微的差异的，比如NSLock之类的还有tryLock等方法用于加锁，不同对象锁的功能偏向不一样等等，有兴趣的可以逐个搜索再更深入的研究不同锁之间的区别。 
2. 另外，一般来说客户端很少会有这么大量的加锁解锁操作，所以日常来说这些锁的性能都是可以满足使用需求的。
本文对于iOS中的锁也是抛砖引玉，欢迎各位留言讨论。
