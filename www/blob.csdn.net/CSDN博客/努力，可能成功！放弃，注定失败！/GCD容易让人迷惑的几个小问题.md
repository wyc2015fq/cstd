# GCD容易让人迷惑的几个小问题 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年12月21日 16:36:27[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：304
![](http://upload-images.jianshu.io/upload_images/2702646-4d2087abfe541c3a.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
写在开头：
本文旨在阐述一些大家容易产生迷惑的GCD相关内容，如果是需要了解一些GCD概念或者基础用法，可以看看这两篇文章：[GCD
 扫盲篇](https://github.com/ChenYilong/ParseSourceCodeStudy/blob/master/01_Parse%E7%9A%84%E5%A4%9A%E7%BA%BF%E7%A8%8B%E5%A4%84%E7%90%86%E6%80%9D%E8%B7%AF/GCD%E6%89%AB%E7%9B%B2%E7%AF%87.md)、[巧谈GCD](http://www.jianshu.com/p/665261814e24)。
目录：
迷惑一：队列和线程的关系
迷惑二：GCD的死锁
迷惑三：以下这些API的异同与作用场景：
`dispatch_async`、`dispatch_sync`、`dispatch_barrier_async`、`dispatch_barrier_sync`
###### 迷惑一：队列和线程的关系
###### 错误理解：
- 有些人会产生一种错觉，觉得队列就是线程。又有些人会有另外一种错觉，一个追加Block就是一个线程。
###### 正确理解：
对我们使用者来说，**与其说GCD是面向线程的，不如说是面向队列的。 **它隐藏了内部线程的调度。
- 我们所做的仅仅是创建不同的队列，把Block追加到队列中去执行，而队列是FIFO（先进先出）的。
- 它会按照我们追加的Block的顺序，在综合我们调用的gcd的api（sync、async、dispatch_barrier_async等等），以及根据系统负载来增减线程并发数， 来调度线程执行Block。
###### 我们来看看以下几个例子：
###### 例一：我们在主线程中，往一个并行queue，以sync的方式提交了一个Block，结果Block在主线程中执行。
```
dispatch_queue_t queue1 = dispatch_queue_create("并行", DISPATCH_QUEUE_CONCURRENT);
dispatch_sync(queue1, ^{
    NSLog(@"%@",[NSThread currentThread]);
});
```
> 
输出结果：<NSThread: 0x60000007fc80>{number = 1, name = main}
###### 例二：我们在主线程中用aync方式提交一个Block，结果Block在分线程中执行。
```
dispatch_queue_t queue1 = dispatch_queue_create("并行", DISPATCH_QUEUE_CONCURRENT);
dispatch_async(queue1, ^{
    NSLog(@"%@",[NSThread currentThread]);
});
```
> 
输出结果：<NSThread: 0x7fea68607750>{number = 2, name = (null)}
###### 例三：我们分别用sync和async向主队列提交Block，结果Block都是在主线程中执行：
注意：我们不能直接在主线程用sync如下的形式去提交Block，否则会引起死锁：
```
dispatch_sync(dispatch_get_main_queue(), ^{
    NSLog(@"%@",[NSThread currentThread]);
});
```
我们用sync如下的方式去提交Block：
```
dispatch_queue_t queue1 = dispatch_queue_create("并行", DISPATCH_QUEUE_CONCURRENT);
dispatch_async(queue1, ^{
    dispatch_sync(dispatch_get_main_queue(), ^{
        NSLog(@"%@",[NSThread currentThread]);
    });
});
```
> 
输出结果：<NSThread: 0x60000007fc80>{number = 1, name = main}
```
dispatch_async(dispatch_get_main_queue(), ^{
    NSLog(@"%@",[NSThread currentThread]);
});
```
> 
输出结果：<NSThread: 0x60000007fc80>{number = 1, name = main}
###### 总结一下：
- 往主队列提交Block，无论是sync，还是async，都是在主线程中执行。
- 往非主队列中提交，**如果是sync，会在当前提交Block的线程中执行。如果是async，则会在分线程中执行。**
上文需要注意以下两点:
- 这里的`sync`，`async`并不局限于`dispatch_sync`、`dispatch_async`，**而指的是GCD中所有同步异步的API。**
- 这里我们用的是并行queue，如果用串行queue，结果也是一样的。**唯一的区别是并行queue会权衡当前系统负载，去同时并发几条线程去执行Block，而串行queue中，始终只会在同一条线程中执行Block。**
###### 迷惑二：GCD的死锁
因为很多人因为不理解发生死锁的原因，所以导致从不会去用sync相关的API，而sync的应用场景还是非常多的，我们不能因噎废食，所以我们了解死锁原理还是很重要的。
###### 简单举个死锁例子：
```
dispatch_sync(dispatch_get_main_queue(), ^{
    NSLog(@"任务一");
});
NSLog(@"任务二");
```
首先造成死锁的原因很简单，两个任务间互相等待。
为了加深大家的理解，在这里我们尽可能用最详尽，同时也有点绕的方式总结下这个死锁的流程：
- 如上，在主线程中，往主队列同步提交了任务一。因为往queue中提交Block，总是追加在队列尾部的，而queue执行Block的顺序为先进先出（FIFO），所以任务一需要在当前队列它之前的任务（任务二）全部执行完，才能轮到它。
- 而任务二因为任务一的sync，被阻塞了，它需要等任务一执行完才能被执行。两者互相等待对方执行完，才能执行，程序被死锁在这了。
- 这里需要注意这里死锁的很重要一个条件也因为**主队列是一个串行的队列**(主队列中只有一条主线程)。如果我们如下例，在并行队列中提交，则不会造成死锁：
```
dispatch_async(dispatch_get_global_queue(0, 0), ^{
  dispatch_sync(dispatch_get_global_queue(0, 0), ^{
      NSLog(@"任务一");
  });
  NSLog(@"任务二");
});
```
原因是并行队列中任务一虽被提交仍然是在queue的队尾，在任务二之后，但是因为是并行的，所以任务一并不会一直等任务二结束才去执行，而是直接执行完。此时任务二的因为任务一的结束，sync阻塞也就消除，任务二得以执行。
上述第一个死锁的例子，我们很简单的改写一下，死锁就被消除了：
```
dispatch_sync(dispatch_get_global_queue(0, 0), ^{
    NSLog(@"任务一");
});
NSLog(@"任务二");
```
我们在主线程中，往全局队列同步提交了Block，因为全局队列和主队列是两个队列，所以任务一的执行，并不需要等待任务二。所以等任务一结束，任务二也可以被执行。
当然这里因为提交Block所在队列，Block被执行的队列是完全不同的两个队列，所以这里用串行queue，也是不会死锁的。大家可以自己写个例子试试，这里就不赘述了。
看到这我们可以知道一些sync的阻塞机制：
- **sync提交Block，首先是阻塞的当前提交Block的线程**（简单理解下就是阻塞sync之后的代码）。例如我们之前举的例子中，sync总是阻塞了任务二的执行。
- 而在队列中，轮到sync提交的Block，**仅仅阻塞串行queue，而不会阻塞并行queue。**（`dispatch_barrier_sync`除外，我们后面会讲到。）
我们了解了sync的阻塞机制，再结合发生死锁的根本原因来自于互相等待，我们用下面一句话来总结一下，会引起GCD死锁的行为：
**如果同步（sync）提交一个Block到一个串行队列，而提交Block这个动作所处的线程，也是在当前队列，就会引起死锁。**
我相信，如果看明白了上述所说的，基本上可以放心的使用sync相关api，而不用去担心死锁的问题。
关于更多死锁例子这里就不写了，基本上都是基于上述所说的，只是在不同队列中，sync，async组合形式不同，但是原理都是和上述一样。如果实在感兴趣的，可以看看这篇文章：[一篇专题让你秒懂GCD死锁问题!](https://my.oschina.net/doxing/blog/618132)
###### 迷惑三：以下4个GCD方法的区别：
```
dispatch_async(<#dispatch_queue_t  _Nonnull queue#>, <#^(void)block#>)
dispatch_sync(<#dispatch_queue_t  _Nonnull queue#>, <#^(void)block#>)
dispatch_barrier_async(<#dispatch_queue_t  _Nonnull queue#>, <#^(void)block#>)
dispatch_barrier_sync(<#dispatch_queue_t  _Nonnull queue#>, <#^(void)block#>)
```
1)`dispatch_async` 这个就不用说了，估计大家都用的非常熟悉。
2)`dispatch_barrier_async`， 这个想必大家也知道是干嘛用的，如果不知道，我也大概讲讲：
它的作用可以用一个词概括－－承上启下，**它保证此前的任务都先于自己执行，此后的任务也迟于自己执行。**当然它的作用导致它只有在并行队列中有意义。
```cpp
```cpp
dispatch_queue_t queue = dispatch_queue_create("queue", DISPATCH_QUEUE_CONCURRENT);
dispatch_async(queue, ^{
    // 任务1
    ...
});
dispatch_async(queue, ^{
    // 任务2
    ...
});
dispatch_async(queue, ^{
    // 任务3
    ...
});
dispatch_barrier_async(queue, ^{
    // 任务4
    ...
});
dispatch_async(queue, ^{
    // 任务5
    ...
});
dispatch_async(queue, ^{
    // 任务6
    ...
});
```
```
例如上述任务，任务1，2，3的顺序不一定，4在中间，最后是5，6任务顺序不一定。**它就像一个栅栏一样，挡在了一个并行队列中间。**
当然这里有一点需要注意的是：dispatch_barrier_(a)sync只在自己创建的并发队列上有效，在全局(Global)并发队列、串行队列上，效果跟dispatch_(a)sync效果一样。
我们讲到这，顺便来讲讲它的用途，例如我们在一个读写操作中：
我们要知道一个数据，读与读之间是可以用线程并行的，但是写与写、写与读之间，就必须串行同步或者使用线程锁来保证线程安全。但是我们有了`dispatch_barrier_async`，我们就可以如下使用：
```cpp
```cpp
dispatch_queue_t queue = dispatch_queue_create("queue", DISPATCH_QUEUE_CONCURRENT);
dispatch_async(queue, ^{
    //读操作
});
dispatch_async(queue, ^{
    // 读操作
});
dispatch_barrier_async(queue, ^{
    // 写操作
});
dispatch_barrier_async(queue, ^{
    // 写操作
});
dispatch_async(queue, ^{
    // 读操作
});
```
```
这样写操作的时候，始终只有它这一条线程在进行。而读操作一直是并行的。这么做充分利用了多线程的优势，还不需要加锁，减少了相当一部分的性能开销。实现了读写操作的线程安全。
3)`dispatch_barrier_sync`这个方法和`dispatch_barrier_async`作用几乎一样，都可以在并行queue中当做栅栏。
唯一的区别就是：`dispatch_barrier_sync`有GCD的sync共有特性，会阻塞提交Block的当前线程，而`dispatch_barrier_async`是异步提交，不会阻塞。
4）`dispatch_sync`，我们来讲讲它和`dispatch_barrier_sync`的区别。二者因为是sync提交，所以都是阻塞当前提交Block线程。
而它俩唯一的区别是：`dispatch_sync`并不能阻塞并行队列。其实之前死锁有提及过，担心大家感觉疑惑，还是写个例子：
```
dispatch_queue_t queue = dispatch_queue_create("并行", DISPATCH_QUEUE_CONCURRENT);
dispatch_sync(queue, ^{
    dispatch_async(queue, ^{
        NSLog(@"任务二");
    });
    dispatch_async(queue, ^{
        NSLog(@"任务三");
    });
    //睡眠2秒
    [NSThread sleepForTimeInterval:2];
    NSLog(@"任务一");
});
```
> 
输出结果 :
任务三
任务二
任务一
很显然，并行队列没有被sync所阻塞。
而`dispatch_barrier_sync`可以阻塞并行队列（栅栏作用的体现）：
```
dispatch_queue_t queue = dispatch_queue_create("并行", DISPATCH_QUEUE_CONCURRENT);
dispatch_barrier_sync(queue, ^{
    dispatch_async(queue, ^{
        NSLog(@"任务二");
    });
    dispatch_async(queue, ^{
        NSLog(@"任务三");
    });
    //睡眠2秒
    [NSThread sleepForTimeInterval:2];
    NSLog(@"任务一");
});
```
> 
输出结果 :
任务一
任务二
任务三
总结一下：
这些API都是有各自应用场景的，苹果也不会给我们提供重复而且毫无意义的方法。
其中在AF的图片缓存处理中，就有大量组合的用到：
`dispatch_barrier_sync`、`dispatch_barrier_async`、`dispatch_sync`
这些API，主要是为了保证在不使用锁下，缓存数据的读写的线程安全。感兴趣的可以去楼主之前的文章中看看：
[AFNetworking之UIKit扩展与缓存实现](http://www.jianshu.com/p/4ffeb1ba3046)
大概就写到这里了，如果小伙伴有其它感到迷惑的问题，可以评论，楼主会一一回复，如果这个问题问的多的话，会继续补充在本文中。
文／涂耀辉（简书作者）
原文链接：http://www.jianshu.com/p/ff444d664e51
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
