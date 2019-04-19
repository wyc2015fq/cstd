# iOS 任务调度器：为 CPU 和内存减负 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年04月10日 11:26:12[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：10
GitHub 地址：[YBTaskScheduler](https://github.com/indulgeIn/YBTaskScheduler)
**支持 cocopods，使用简便，效率不错，一个性能优化的基础组件。**
# 前言
前些时间有好几个技术朋友问过笔者类似的问题：主线程需要执行大量的任务导致卡顿如何处理？异步任务量级过大导致 CPU 和内存压力过高如何优化？
解决类似的问题可以用几个思路：降频、淘汰、优先级调度。
本来解决这些问题并不需要很复杂的代码，但是涉及到一些 C 代码并且要注意线程安全的问题，所以笔者就做了这样一个轮子，以解决任务调度引发的性能问题。
本文讲述 **YBTaskScheduler** 的原理，读者朋友需要有一定的 iOS 基础，了解一些性能优化的知识，基本用法可以先看看 GitHub [README](https://github.com/indulgeIn/YBTaskScheduler)，DEMO 中也有一个相册列表的应用案例。
# 一、需求分析
就拿 DEMO 中的案例来说明，一个显示相册图片的列表：
![](https://upload-images.jianshu.io/upload_images/2909132-797c53fa8502a5a8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/300/format/webp)
实现图中业务，必然考虑到几个耗时操作：
- 从相册读取图片
- 解压图片
- 圆角处理
- 绘制图片
理所当然的想到处理方案（DEMO中有实现）：
- 异步读取图片
- 异步裁剪图片为正方形（这个过程中就解压了）
- 异步裁剪圆角
- 回到主线程绘制图片
一整套流程下来，貌似需求很好的解决了，但是当快速滑动列表时，会发现 CPU 和内存的占用会比较高（这取决于从相册中读取并显示多大的图片）。当然 DEMO 中按照屏幕的物理像素处理，就算不使用任务调度器组件快速滑动列表也基本不会有掉帧的现象。考虑到老旧设备或者技术人员的水平，很多时候这种需求会导致严重的 CPU 和内存负担，甚至导致闪退。
以上处理方案可能存在的性能瓶颈：
- 从相册读取图片、裁剪图片，处理圆角、主线程绘制等操作会导致 CPU 计算压力过大。
- 同时解压的图片、同时绘制的图片过多导致内存峰值飙升（更不要说做了图片的缓存）。
任何一种情况都可能导致客户端卡死或者闪退，结合业务来分析问题，会发现优化的思路还是不难找到：
- 滑出屏幕的图片不会存在绘制压力，而当前屏幕中的图片会在一个 RunLoop 循环周期绘制，可能造成掉帧。所以可以减少一个 RunLoop 循环周期所绘制的图片数量。
- 快速滑动列表，大量的异步任务直接交由 CPU 执行，然而滑出屏幕的图片已经没有处理它的意义了。所以可以提前删除掉已经滑出屏幕的异步任务，以此来降低 CPU 和内存压力。
没错， [YBTaskScheduler](https://github.com/indulgeIn/YBTaskScheduler) 组件就是替你做了这些事情 ，而且还不止于此。
# 二、命令模式与 RunLoop
想要管理这些复杂的任务，并且在合适的时机调用它们，自然而然的就想到了命令模式。意味着任务不能直接执行，而是把任务作为一个命令装入容器。
在 Objective-C 中，显然 Block 代码块能解决延迟执行这个问题：
```
[_scheduler addTask:^{
     /* 
     具体任务代码
     解压图片、裁剪图片、访问磁盘等 
     */
}];
```
然后组件将这些代码块“装起来”，组件由此“掌握”了所有的任务，可以自由的决定何时调用这些代码块，何时对某些代码块进行淘汰，还可以实现优先级调度。
既然是命令模式，还差一个 Invoker (调用程序)，即何时去触发这些任务。结合 iOS 的技术特点，可以监听 RunLoop 循环周期来实现：
```
static void addRunLoopObserver() {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        taskSchedulers = [NSHashTable weakObjectsHashTable];
        CFRunLoopObserverRef observer = CFRunLoopObserverCreate(CFAllocatorGetDefault(), kCFRunLoopBeforeWaiting | kCFRunLoopExit, true, 0xFFFFFF, runLoopObserverCallBack, NULL);
        CFRunLoopAddObserver(CFRunLoopGetMain(), observer, kCFRunLoopCommonModes);
        CFRelease(observer);
    });
}
```
然后在回调函数中进行任务的调度。
# 三、策略模式
考虑到任务的淘汰策略和优先级调度，必然需要一些高效数据结构来支撑，为了提高处理效率，笔者直接使用了 C++ 的数据结构：`deque`和`priority_queue`。
因为要实现任务淘汰，所以使用`deque`双端队列来模拟栈和队列，而不是直接使用`stack`和`queue`。使用`priority_queue`优先队列来处理自定义的优先级调度，它的缺点是不能删除低优先级节点，为了节约时间成本姑且够用。
具体的策略：
- 栈：后加入的任务先执行（可以理解为后加入的任务优先级高），优先淘汰先加入的任务。
- 队列：先加入的任务先执行（可以理解为先加入的任务优先级高），优先淘汰后加入的任务。
- 优先队列：自定义任务优先级，不支持任务淘汰。
实际上组件是推荐使用栈和队列这两种策略，因为插入和取出的时间复杂度是常数级的，需要定制任务的优先级时才考虑使用优先队列，因为其插入复杂度是 O(logN) 的。
至此，整个组件的业务是比较清晰了，组件需要让这三种处理方式可以自由的变动，所以采用策略模式来处理，下面是 UML 类图：
![](https://upload-images.jianshu.io/upload_images/2909132-1795e2e563b021a8.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/581/format/webp)
UML类图
嗯，这是个挺标准的策略模式。
# 四、线程安全
由于任务的调度可能在任意线程，所以必须要做好容器（栈、队列、优先队列）访问的线程安全问题，组件是使用`pthread_mutex_t`和`dispatch_once`来保证线程安全，同时笔者尽量减少临界区来提高性能。值得注意的是，如果不会存在线程安全的代码就不要去加锁了。
关于多线程及安全可以看笔者的另一篇文章：[iOS 如何高效的使用多线程](https://www.jianshu.com/p/594d15d6c6a7)，这里就不赘述了。
# 后语
部分技术细节就不多说了，组件代码量比较少，如果感兴趣可以直接看源码。实际上这个组件的应用场景并不是很多，在项目稳定需要做深度的性能优化时可能会比较需要它，并且希望使用它的人也能了解一些原理，做到胸有成竹，才能灵活的运用。
作者：indulge_in
链接：https://www.jianshu.com/p/f2a610c77d26
来源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
