# Java并发编程 - weixin_33985507的博客 - CSDN博客
2018年09月10日 11:16:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
## 一、同步
### 1 synchronized关键字
synchronized锁什么？锁对象。
可能锁对象包括： this， 临界资源对象，Class类对象。
#### 1.1 同步方法
synchronized T methodName(){}
同步方法锁定的是当前对象。当多线程通过同一个对象引用多次调用当前同步方法时，需同步执行。
#### 1.2 同步代码块
同步代码块的同步粒度更加细致，是商业开发中推荐的编程方式。可以定位到具体的同步位置，而不是简单的将方法整体实现同步逻辑。在效率上，相对更高。
##### 1.2.1 锁定临界对象
T methodName(){
synchronized(object){}
}
同步代码块在执行时，是锁定object对象。当多个线程调用同一个方法时，锁定对象不变的情况下，需同步执行。
##### 1.2.2 锁定当前对象
T methodName(){
synchronized(this){}
}
当锁定对象为this时，相当于[1.1](#_%E5%90%8C%E6%AD%A5%E6%96%B9%E6%B3%95)同步方法。
#### 1.3 锁的底层实现
Java 虚拟机中的同步(Synchronization)基于进入和退出管程(Monitor)对象实现。同步方法 并不是由 monitor enter 和 monitor exit 指令来实现同步的，而是由方法调用指令读取运行时常量池中方法的 ACC_SYNCHRONIZED 标志来隐式实现的。
##### 1.3.1 对象内存简图
![4639175-c6245103f0aeac42.gif](https://upload-images.jianshu.io/upload_images/4639175-c6245103f0aeac42.gif)
clip_image001.gif
对象头：存储对象的hashCode、锁信息或分代年龄或GC标志，类型指针指向对象的类元数据，JVM通过这个指针确定该对象是哪个类的实例等信息。
实例变量：存放类的属性数据信息，包括父类的属性信息
填充数据：由于虚拟机要求对象起始地址必须是8字节的整数倍。填充数据不是必须存在的，仅仅是为了字节对齐
当在对象上加锁时，数据是记录在对象头中。当执行synchronized同步方法或同步代码块时，会在对象头中记录锁标记，锁标记指向的是monitor对象（也称为管程或监视器锁）的起始地址。每个对象都存在着一个 monitor 与之关联，对象与其 monitor 之间的关系有存在多种实现方式，如monitor可以与对象一起创建销毁或当线程试图获取对象锁时自动生成，但当一个 monitor 被某个线程持有后，它便处于锁定状态。
在Java虚拟机(HotSpot)中，monitor是由ObjectMonitor实现的。
ObjectMonitor中有两个队列，_WaitSet 和 _EntryList，以及_Owner标记。其中_WaitSet是用于管理等待队列(wait)线程的，_EntryList是用于管理锁池阻塞线程的，_Owner标记用于记录当前执行线程。线程状态图如下：
![4639175-f1c9f9f7bb20bed4.gif](https://upload-images.jianshu.io/upload_images/4639175-f1c9f9f7bb20bed4.gif)
clip_image003.gif
当多线程并发访问同一个同步代码时，首先会进入_EntryList，当线程获取锁标记后，monitor中的_Owner记录此线程，并在monitor中的计数器执行递增计算（+1），代表锁定，其他线程在_EntryList中继续阻塞。若执行线程调用wait方法，则monitor中的计数器执行赋值为0计算，并将_Owner标记赋值为null，代表放弃锁，执行线程进如_WaitSet中阻塞。若执行线程调用notify/notifyAll方法，_WaitSet中的线程被唤醒，进入_EntryList中阻塞，等待获取锁标记。若执行线程的同步代码执行结束，同样会释放锁标记，monitor中的_Owner标记赋值为null，且计数器赋值为0计算。
#### 1.4 锁的种类
Java中锁的种类大致分为偏向锁，自旋锁，轻量级锁，重量级锁。
锁的使用方式为：先提供偏向锁，如果不满足的时候，升级为轻量级锁，再不满足，升级为重量级锁。自旋锁是一个过渡的锁状态，不是一种实际的锁类型。
锁只能升级，不能降级。
##### 1.4.1 重量级锁
在[1.3](#_%E9%94%81%E7%9A%84%E5%BA%95%E5%B1%82%E5%AE%9E%E7%8E%B0)中解释的就是重量级锁。
##### 1.4.2 偏向锁
是一种编译解释锁。如果代码中不可能出现多线程并发争抢同一个锁的时候，JVM编译代码，解释执行的时候，会自动的放弃同步信息。消除synchronized的同步代码结果。使用锁标记的形式记录锁状态。在Monitor中有变量ACC_SYNCHRONIZED。当变量值使用的时候，代表偏向锁锁定。可以避免锁的争抢和锁池状态的维护。提高效率。
##### 1.4.3 轻量级锁
过渡锁。当偏向锁不满足，也就是有多线程并发访问，锁定同一个对象的时候，先提升为轻量级锁。也是使用标记ACC_SYNCHRONIZED标记记录的。ACC_UNSYNCHRONIZED标记记录未获取到锁信息的线程。就是只有两个线程争抢锁标记的时候，优先使用轻量级锁。
两个线程也可能出现重量级锁。
##### 1.4.4 自旋锁
是一个过渡锁，是偏向锁和轻量级锁的过渡。
当获取锁的过程中，未获取到。为了提高效率，JVM自动执行若干次空循环，再次申请锁，而不是进入阻塞状态的情况。称为自旋锁。自旋锁提高效率就是避免线程状态的变更。
### 2 volatile关键字
变量的线程可见性。在CPU计算过程中，会将计算过程需要的数据加载到CPU计算缓存中，当CPU计算中断时，有可能刷新缓存，重新读取内存中的数据。在线程运行的过程中，如果某变量被其他线程修改，可能造成数据不一致的情况，从而导致结果错误。而volatile修饰的变量是线程可见的，当JVM解释volatile修饰的变量时，会通知CPU，在计算过程中，每次使用变量参与计算时，都会检查内存中的数据是否发生变化，而不是一直使用CPU缓存中的数据，可以保证计算结果的正确。
volatile只是通知底层计算时，CPU检查内存数据，而不是让一个变量在多个线程中同步。
### 3 wait¬ify
### 4 AtomicXxx类型组
原子类型。
在concurrent.atomic包中定义了若干原子类型，这些类型中的每个方法都是保证了原子操作的。多线程并发访问原子类型对象中的方法，不会出现数据错误。在多线程开发中，如果某数据需要多个线程同时操作，且要求计算原子性，可以考虑使用原子类型对象。
注意：原子类型中的方法是保证了原子操作，但多个方法之间是没有原子性的。如：
AtomicInteger i = new AtomicInteger(0);
if(i.get() != 5)  i.incrementAndGet();
在上述代码中，get方法和incrementAndGet方法都是原子操作，但复合使用时，无法保证原子性，仍旧可能出现数据错误。
### 5 CountDownLatch门闩
门闩是concurrent包中定义的一个类型，是用于多线程通讯的一个辅助类型。
门闩相当于在一个门上加多个锁，当线程调用await方法时，会检查门闩数量，如果门闩数量大于0，线程会阻塞等待。当线程调用countDown时，会递减门闩的数量，当门闩数量为0时，await阻塞线程可执行。
### 6 锁的重入
在Java中，同步锁是可以重入的。只有同一线程调用同步方法或执行同步代码块，对同一个对象加锁时才可重入。
当线程持有锁时，会在monitor的计数器中执行递增计算，若当前线程调用其他同步代码，且同步代码的锁对象相同时，monitor中的计数器继续递增。每个同步代码执行结束，monitor中的计数器都会递减，直至所有同步代码执行结束，monitor中的计数器为0时，释放锁标记，_Owner标记赋值为null。
### 7 ReentrantLock
重入锁，建议应用的同步方式。相对效率比synchronized高。量级较轻。
synchronized在JDK1.5版本开始，尝试优化。到JDK1.7版本后，优化效率已经非常好了。在绝对效率上，不比reentrantLock差多少。
使用重入锁，必须必须必须手工释放锁标记。一般都是在finally代码块中定义释放锁标记的unlock方法。
#### 7.1 公平锁
![4639175-73f893e9da1b0e0e.gif](https://upload-images.jianshu.io/upload_images/4639175-73f893e9da1b0e0e.gif)
clip_image004.gif
### 8 ThreadLocal
remove问题
![4639175-a516ef0223df2d97.gif](https://upload-images.jianshu.io/upload_images/4639175-a516ef0223df2d97.gif)
clip_image005.gif
## 二、同步容器
解决并发情况下的容器线程安全问题的。给多线程环境准备一个线程安全的容器对象。
线程安全的容器对象： Vector, Hashtable。线程安全容器对象，都是使用synchronized方法实现的。
concurrent包中的同步容器，大多数是使用系统底层技术实现的线程安全。类似native。Java8中使用CAS。
### 1 Map/Set
#### 1.1 ConcurrentHashMap/ConcurrentHashSet
底层哈希实现的同步Map(Set)。效率高，线程安全。使用系统底层技术实现线程安全。
量级较synchronized低。key和value不能为null。
#### 1.2 ConcurrentSkipListMap/ConcurrentSkipListSet
底层跳表（SkipList）实现的同步Map(Set)。有序，效率比ConcurrentHashMap稍低。
![4639175-c4d3c592c3e1720b.gif](https://upload-images.jianshu.io/upload_images/4639175-c4d3c592c3e1720b.gif)
clip_image006.gif
### 2 List
#### 2.1 CopyOnWriteArrayList
写时复制集合。写入效率低，读取效率高。每次写入数据，都会创建一个新的底层数组。
### 3 Queue
#### 3.1 ConcurrentLinkedQueue
基础链表同步队列。
#### 3.2 LinkedBlockingQueue
阻塞队列，队列容量不足自动阻塞，队列容量为0自动阻塞。
#### 3.3 ArrayBlockingQueue
底层数组实现的有界队列。自动阻塞。根据调用API（add/put/offer）不同，有不同特性。
当容量不足的时候，有阻塞能力。
add方法在容量不足的时候，抛出异常。
put方法在容量不足的时候，阻塞等待。
offer方法，
单参数offer方法，不阻塞。容量不足的时候，返回false。当前新增数据操作放弃。
三参数offer方法（offer(value,times,timeunit)），容量不足的时候，阻塞times时长（单位为timeunit），如果在阻塞时长内，有容量空闲，新增数据返回true。如果阻塞时长范围内，无容量空闲，放弃新增数据，返回false。
#### 3.4 DelayQueue
延时队列。根据比较机制，实现自定义处理顺序的队列。常用于定时任务。
如：定时关机。
#### 3.5 LinkedTransferQueue
转移队列，使用transfer方法，实现数据的即时处理。没有消费者，就阻塞。
#### 3.6 SynchronusQueue
同步队列，是一个容量为0的队列。是一个特殊的TransferQueue。
必须现有消费线程等待，才能使用的队列。
add方法，无阻塞。若没有消费线程阻塞等待数据，则抛出异常。
put方法，有阻塞。若没有消费线程阻塞等待数据，则阻塞。
## 三、ThreadPool&Executor
### 1 Executor
线程池顶级接口。定义方法，void execute(Runnable)。方法是用于处理任务的一个服务方法。调用者提供Runnable接口的实现，线程池通过线程执行这个Runnable。服务方法无返回值的。是Runnable接口中的run方法无返回值。
常用方法 - void execute(Runnable)
作用是： 启动线程任务的。
### 2 ExecutorService
Executor接口的子接口。提供了一个新的服务方法，submit。有返回值（Future类型）。submit方法提供了overload方法。其中有参数类型为Runnable的，不需要提供返回值的；有参数类型为Callable，可以提供线程执行后的返回值。
Future，是submit方法的返回值。代表未来，也就是线程执行结束后的一种结果。如返回值。
常见方法 - void execute(Runnable)， Future submit(Callable)， Future submit(Runnable)
线程池状态： Running， ShuttingDown， Termitnaed
Running - 线程池正在执行中。活动状态。
ShuttingDown - 线程池正在关闭过程中。优雅关闭。一旦进入这个状态，线程池不再接收新的任务，处理所有已接收的任务，处理完毕后，关闭线程池。
Terminated - 线程池已经关闭。
### 3 Future
未来结果，代表线程任务执行结束后的结果。获取线程执行结果的方式是通过get方法获取的。get无参，阻塞等待线程执行结束，并得到结果。get有参，阻塞固定时长，等待线程执行结束后的结果，如果在阻塞时长范围内，线程未执行结束，抛出异常。
常用方法： T get()  T get(long, TimeUnit)
### 4 Callable
可执行接口。 类似Runnable接口。也是可以启动一个线程的接口。其中定义的方法是call。call方法的作用和Runnable中的run方法完全一致。call方法有返回值。
接口方法 ： Object call();相当于Runnable接口中的run方法。区别为此方法有返回值。不能抛出已检查异常。
和Runnable接口的选择 - 需要返回值或需要抛出异常时，使用Callable，其他情况可任意选择。
### 5 Executors
工具类型。为Executor线程池提供工具方法。可以快速的提供若干种线程池。如：固定容量的，无限容量的，容量为1等各种线程池。
线程池是一个进程级的重量级资源。默认的生命周期和JVM一致。当开启线程池后，直到JVM关闭为止，是线程池的默认生命周期。如果手工调用shutdown方法，那么线程池执行所有的任务后，自动关闭。
开始 - 创建线程池。
结束 - JVM关闭或调用shutdown并处理完所有的任务。
类似Arrays，Collections等工具类型的功用。
### 6 FixedThreadPool
容量固定的线程池。活动状态和线程池容量是有上限的线程池。所有的线程池中，都有一个任务队列。使用的是BlockingQueue<Runnable>作为任务的载体。当任务数量大于线程池容量的时候，没有运行的任务保存在任务队列中，当线程有空闲的，自动从队列中取出任务执行。
使用场景： 大多数情况下，使用的线程池，首选推荐FixedThreadPool。OS系统和硬件是有线程支持上限。不能随意的无限制提供线程池。
线程池默认的容量上限是Integer.MAX_VALUE。
常见的线程池容量： PC - 200。 服务器 - 1000~10000
queued tasks - 任务队列
completed tasks - 结束任务队列
### 7 CachedThreadPool
缓存的线程池。容量不限（Integer.MAX_VALUE）。自动扩容。容量管理策略：如果线程池中的线程数量不满足任务执行，创建新的线程。每次有新任务无法即时处理的时候，都会创建新的线程。当线程池中的线程空闲时长达到一定的临界值（默认60秒），自动释放线程。
默认线程空闲60秒，自动销毁。
应用场景： 内部应用或测试应用。 内部应用，有条件的内部数据瞬间处理时应用，如：电信平台夜间执行数据整理（有把握在短时间内处理完所有工作，且对硬件和软件有足够的信心）。 测试应用，在测试的时候，尝试得到硬件或软件的最高负载量，用于提供FixedThreadPool容量的指导。
### 8 ScheduledThreadPool
计划任务线程池。可以根据计划自动执行任务的线程池。
scheduleAtFixedRate(Runnable, start_limit, limit, timeunit)
runnable - 要执行的任务。
start_limit - 第一次任务执行的间隔。
limit - 多次任务执行的间隔。
timeunit - 多次任务执行间隔的时间单位。
使用场景： 计划任务时选用（DelaydQueue），如：电信行业中的数据整理，没分钟整理，没消失整理，每天整理等。
### 9 SingleThreadExceutor
单一容量的线程池。
使用场景： 保证任务顺序时使用。如： 游戏大厅中的公共频道聊天。秒杀。
### 10 ForkJoinPool
分支合并线程池（mapduce类似的设计思想）。适合用于处理复杂任务。
初始化线程容量与CPU核心数相关。
线程池中运行的内容必须是ForkJoinTask的子类型（RecursiveTask,RecursiveAction）。
ForkJoinPool - 分支合并线程池。 可以递归完成复杂任务。要求可分支合并的任务必须是ForkJoinTask类型的子类型。其中提供了分支和合并的能力。ForkJoinTask类型提供了两个抽象子类型，RecursiveTask有返回结果的分支合并任务,RecursiveAction无返回结果的分支合并任务。（Callable/Runnable）compute方法：就是任务的执行逻辑。
ForkJoinPool没有所谓的容量。默认都是1个线程。根据任务自动的分支新的子线程。当子线程任务结束后，自动合并。所谓自动是根据fork和join两个方法实现的。
应用： 主要是做科学计算或天文计算的。数据分析的。
### 11 WorkStealingPool
JDK1.8新增的线程池。工作窃取线程池。当线程池中有空闲连接时，自动到等待队列中窃取未完成任务，自动执行。
初始化线程容量与CPU核心数相关。此线程池中维护的是精灵线程。
ExecutorService.newWorkStealingPool();
### 12 ThreadPoolExecutor
线程池底层实现。除ForkJoinPool外，其他常用线程池底层都是使用ThreadPoolExecutor实现的。
public ThreadPoolExecutor
(int corePoolSize, // 核心容量，创建线程池的时候，默认有多少线程。也是线程池保持的最少线程数
int maximumPoolSize, // 最大容量，线程池最多有多少线程
long keepAliveTime, // 生命周期，0为永久。当线程空闲多久后，自动回收。
TimeUnit unit, // 生命周期单位，为生命周期提供单位，如：秒，毫秒
BlockingQueue<Runnable> workQueue // 任务队列，阻塞队列。注意，泛型必须是Runnable
);
使用场景： 默认提供的线程池不满足条件时使用。如：初始线程数据4，最大线程数200，线程空闲周期30秒。
## 四、JVM优化
### 1          JVM简单结构图
![4639175-a833ce344926a121.jpg](https://upload-images.jianshu.io/upload_images/4639175-a833ce344926a121.jpg)
clip_image008.jpg
#### 1.1 类加载子系统与方法区：
类加载子系统负责从文件系统或者网络中加载Class信息，加载的类信息存放于一块称为方法区的内存空间。除了类的信息外，方法区中可能还会存放运行时常量池信息，包括字符串字面量和数字常量（这部分常量信息是Class文件中常量池部分的内存映射）。
#### 1.2 Java堆
java堆在虚拟机启动的时候建立，它是java程序最主要的内存工作区域。几乎所有的java对象实例都存放在java堆中。堆空间是所有线程共享的，这是一块与java应用密切相关的内存空间。
#### 1.3 直接内存
java的NIO库允许java程序使用直接内存。直接内存是在java堆外的、直接向系统申请的内存空间。通常访问直接内存的速度会优于java堆。因此出于性能的考虑，读写频繁的场合可能会考虑使用直接内存。由于直接内存在java堆外，因此它的大小不会直接受限于Xmx指定的最大堆大小，但是系统内存是有限的，java堆和直接内存的总和依然受限于操作系统能给出的最大内存。
#### 1.4 垃圾回收系统
垃圾回收系统是java虚拟机的重要组成部分，垃圾回收器可以对方法区、java堆和直接内存进行回收。其中，java堆是垃圾收集器的工作重点。和C/C++不同，java中所有的对象空间释放都是隐式的，也就是说，java中没有类似free()或者delete()这样的函数释放指定的内存区域。对于不再使用的垃圾对象，垃圾回收系统会在后台默默工作，默默查找、标识并释放垃圾对象，完成包括java堆、方法区和直接内存中的全自动化管理。
#### 1.5 Java栈
每一个java虚拟机线程都有一个私有的java栈，一个线程的java栈在线程创建的时候被创建，java栈中保存着帧信息，java栈中保存着局部变量、方法参数，同时和java方法的调用、返回密切相关。
#### 1.6 本地方法栈
本地方法栈和java栈非常类似，最大的不同在于java栈用于方法的调用，而本地方法栈则用于本地方法的调用，作为对java虚拟机的重要扩展，java虚拟机允许java直接调用本地方法（通常使用C编写）
#### 1.7 PC寄存器
PC（Program Counter）寄存器也是每一个线程私有的空间，java虚拟机会为每一个java线程创建PC寄存器。在任意时刻，一个java线程总是在执行一个方法，这个正在被执行的方法称为当前方法。如果当前方法不是本地方法，PC寄存器就会指向当前正在被执行的指令。如果当前方法是本地方法，那么PC寄存器的值就是undefined
#### 1.8 执行引擎
执行引擎是java虚拟机的最核心组件之一，它负责执行虚拟机的字节码，现代虚拟机为了提高执行效率，会使用即时编译(just in time)技术将方法编译成机器码后再执行。
Java HotSpot Client VM(-client)，为在客户端环境中减少启动时间而优化的执行引擎；本地应用开发使用。（如：eclipse）
Java HotSpot Server VM(-server)，为在服务器环境中最大化程序执行速度而设计的执行引擎。应用在服务端程序。（如：tomcat）
Java HotSpot Client模式和Server模式的区别
当虚拟机运行在-client模式的时候,使用的是一个代号为C1的轻量级编译器, 而-server模式启动的虚拟机采用相对重量级,代号为C2的编译器. C2比C1编译器编译的相对彻底,服务起来之后,性能更高
JDK安装目录/jre/lib/（x86、i386、amd32、amd64）/jvm.cfg
文件中的内容，-server和-client哪一个配置在上，执行引擎就是哪一个。如果是JDK1.5版本且是64位系统应用时，-client无效。
--64位系统内容
-server KNOWN
-client IGNORE
--32位系统内容
-server KNOWN
-client KNOWN
注意：在部分JDK1.6版本和后续的JDK版本(64位系统)中，-client参数已经不起作用了，Server模式成为唯一
### 2 堆结构及对象分代
#### 2.1 什么是分代，分代的必要性是什么
Java虚拟机根据对象存活的周期不同，把堆内存划分为几块，一般分为新生代、老年代和永久代（对HotSpot虚拟机而言），这就是JVM的内存分代策略。
堆内存是虚拟机管理的内存中最大的一块，也是垃圾回收最频繁的一块区域，我们程序所有的对象实例都存放在堆内存中。给堆内存分代是为了提高对象内存分配和垃圾回收的效率。试想一下，如果堆内存没有区域划分，所有的新创建的对象和生命周期很长的对象放在一起，随着程序的执行，堆内存需要频繁进行垃圾收集，而每次回收都要遍历所有的对象，遍历这些对象所花费的时间代价是巨大的，会严重影响我们的GC效率。
有了内存分代，情况就不同了，新创建的对象会在新生代中分配内存，经过多次回收仍然存活下来的对象存放在老年代中，静态属性、类信息等存放在永久代中，新生代中的对象存活时间短，只需要在新生代区域中频繁进行GC，老年代中对象生命周期长，内存回收的频率相对较低，不需要频繁进行回收，永久代中回收效果太差，一般不进行垃圾回收，还可以根据不同年代的特点采用合适的垃圾收集算法。分代收集大大提升了收集效率，这些都是内存分代带来的好处。
#### 2.2 分代的划分
Java虚拟机将堆内存划分为新生代、老年代和永久代，永久代是HotSpot虚拟机特有的概念（JDK1.8之后为metaspace替代永久代），它采用永久代的方式来实现方法区，其他的虚拟机实现没有这一概念，而且HotSpot也有取消永久代的趋势，在JDK 1.7中HotSpot已经开始了“去永久化”，把原本放在永久代的字符串常量池移出。永久代主要存放常量、类信息、静态变量等数据，与垃圾回收关系不大，新生代和老年代是垃圾回收的主要区域。
内存简图如下：
![4639175-af571f0b8f4fd48b.jpg](https://upload-images.jianshu.io/upload_images/4639175-af571f0b8f4fd48b.jpg)
clip_image010.jpg
##### 2.2.1 新生代（Young Generation）
新生成的对象优先存放在新生代中，新生代对象朝生夕死，存活率很低，在新生代中，常规应用进行一次垃圾收集一般可以回收70% ~ 95% 的空间，回收效率很高。
HotSpot将新生代划分为三块，一块较大的Eden（伊甸）空间和两块较小的Survivor（幸存者）空间，默认比例为8：1：1。划分的目的是因为HotSpot采用复制算法来回收新生代，设置这个比例是为了充分利用内存空间，减少浪费。新生成的对象在Eden区分配（大对象除外，大对象直接进入老年代），当Eden区没有足够的空间进行分配时，虚拟机将发起一次Minor GC。
GC开始时，对象只会存在于Eden区和From Survivor区，To Survivor区是空的（作为保留区域）。GC进行时，Eden区中所有存活的对象都会被复制到To Survivor区，而在From Survivor区中，仍存活的对象会根据它们的年龄值决定去向，年龄值达到年龄阀值（默认为15，新生代中的对象每熬过一轮垃圾回收，年龄值就加1，GC分代年龄存储在对象的header中）的对象会被移到老年代中，没有达到阀值的对象会被复制到To Survivor区。接着清空Eden区和From Survivor区，新生代中存活的对象都在To Survivor区。接着， From Survivor区和To Survivor区会交换它们的角色，也就是新的To Survivor区就是上次GC清空的From Survivor区，新的From Survivor区就是上次GC的To Survivor区，总之，不管怎样都会保证To Survivor区在一轮GC后是空的。GC时当To Survivor区没有足够的空间存放上一次新生代收集下来的存活对象时，需要依赖老年代进行分配担保，将这些对象存放在老年代中。
##### 2.2.2 老年代（Old Generationn）
在新生代中经历了多次（具体看虚拟机配置的阀值）GC后仍然存活下来的对象会进入老年代中。老年代中的对象生命周期较长，存活率比较高，在老年代中进行GC的频率相对而言较低，而且回收的速度也比较慢。
##### 2.2.3 永久代（Permanent Generationn）
永久代存储类信息、常量、静态变量、即时编译器编译后的代码等数据，对这一区域而言，Java虚拟机规范指出可以不进行垃圾收集，一般而言不会进行垃圾回收。
### 3 垃圾回收算法及分代垃圾收集器
#### 3.1 垃圾收集器的分类
##### 3.1.1 次收集器
Scavenge GC，指发生在新生代的GC，因为新生代的Java对象大多都是朝生夕死，所以Scavenge GC非常频繁，一般回收速度也比较快。当Eden空间不足以为对象分配内存时，会触发Scavenge GC。
一般情况下，当新对象生成，并且在Eden申请空间失败时，就会触发Scavenge GC，对Eden区域进行GC，清除非存活对象，并且把尚且存活的对象移动到Survivor区。然后整理Survivor的两个区。这种方式的GC是对年轻代的Eden区进行，不会影响到年老代。因为大部分对象都是从Eden区开始的，同时Eden区不会分配的很大，所以Eden区的GC会频繁进行。因而，一般在这里需要使用速度快、效率高的算法，使Eden去能尽快空闲出来。
当年轻代堆空间紧张时会被触发
相对于全收集而言，收集间隔较短
##### 3.1.2 全收集器
Full GC，指发生在老年代的GC，出现了Full GC一般会伴随着至少一次的Minor GC（老年代的对象大部分是Scavenge GC过程中从新生代进入老年代），比如：分配担保失败。Full GC的速度一般会比Scavenge GC慢10倍以上。当老年代内存不足或者显式调用System.gc()方法时，会触发Full GC。
当老年代或者持久代堆空间满了，会触发全收集操作
可以使用System.gc()方法来显式的启动全收集
全收集一般根据堆大小的不同，需要的时间不尽相同，但一般会比较长。
##### 3.1.3 垃圾回收器的常规匹配
![4639175-06e2cc8b78df72cd.gif](https://upload-images.jianshu.io/upload_images/4639175-06e2cc8b78df72cd.gif)
clip_image011.gif
#### 3.2 常见垃圾回收算法
##### 3.2.1 引用计数（Reference Counting）
比较古老的回收算法。原理是此对象有一个引用，即增加一个计数，删除一个引用则减少一个计数。垃圾回收时，只用收集计数为0的对象。此算法最致命的是无法处理循环引用的问题。
##### 3.2.2 复制（Copying）
此算法把内存空间划为两个相等的区域，每次只使用其中一个区域。垃圾回收时，遍历当前使用区域，把正在使用中的对象复制到另外一个区域中。此算法每次只处理正在使用中的对象，因此复制成本比较小，同时复制过去以后还能进行相应的内存整理，不会出现“碎片”问题。当然，此算法的缺点也是很明显的，就是需要两倍内存空间。简图如下：
![4639175-6e6c5ce69f75a88a.gif](https://upload-images.jianshu.io/upload_images/4639175-6e6c5ce69f75a88a.gif)
clip_image013.gif
##### 3.2.3 标记-清除（Mark-Sweep）
此算法执行分两阶段。第一阶段从引用根节点开始标记所有被引用的对象，第二阶段遍历整个堆，把未标记的对象清除。此算法需要暂停整个应用，同时，会产生内存碎片。简图如下：
![4639175-8ce3db12523d3408.jpg](https://upload-images.jianshu.io/upload_images/4639175-8ce3db12523d3408.jpg)
clip_image015.jpg
##### 3.2.4  标记-整理（Mark-Compact）
此算法结合了“标记-清除”和“复制”两个算法的优点。也是分两阶段，第一阶段从根节点开始标记所有被引用对象，第二阶段遍历整个堆，把清除未标记对象并且把存活对象“压缩”到堆的其中一块，按顺序排放。此算法避免了“标记-清除”的碎片问题，同时也避免了“复制”算法的空间问题。简图如下：
![4639175-e0ba736ac9d4230f.jpg](https://upload-images.jianshu.io/upload_images/4639175-e0ba736ac9d4230f.jpg)
clip_image017.jpg
#### 3.3 分代垃圾收集器
##### 3.3.1 串行收集器（Serial）
Serial收集器是Hotspot运行在Client模式下的默认新生代收集器, 它的特点是：只用一个CPU（计算核心）/一条收集线程去完成GC工作, 且在进行垃圾收集时必须暂停其他所有的工作线程(“Stop The World” -后面简称STW)。可以使用-XX:+UseSerialGC打开。
虽然是单线程收集, 但它却简单而高效, 在VM管理内存不大的情况下(收集几十M~一两百M的新生代), 停顿时间完全可以控制在几十毫秒~一百多毫秒内。
![4639175-ca4f962cf0633878.gif](https://upload-images.jianshu.io/upload_images/4639175-ca4f962cf0633878.gif)
clip_image019.gif
##### 3.3.2 并行收集器（ParNew）
ParNew收集器其实是前面Serial的多线程版本, 除使用多条线程进行GC外, 包括Serial可用的所有控制参数、收集算法、STW、对象分配规则、回收策略等都与Serial完全一样(也是VM启用CMS收集器-XX: +UseConcMarkSweepGC的默认新生代收集器)。
由于存在线程切换的开销, ParNew在单CPU的环境中比不上Serial, 且在通过超线程技术实现的两个CPU的环境中也不能100%保证能超越Serial. 但随着可用的CPU数量的增加, 收集效率肯定也会大大增加(ParNew收集线程数与CPU的数量相同, 因此在CPU数量过大的环境中, 可用-XX:ParallelGCThreads=<N>参数控制GC线程数)。
![4639175-2fc78148b59ee88e.gif](https://upload-images.jianshu.io/upload_images/4639175-2fc78148b59ee88e.gif)
clip_image021.gif
##### 3.3.3 Parallel Scavenge收集器
与ParNew类似, Parallel Scavenge也是使用复制算法, 也是并行多线程收集器. 但与其他收集器关注尽可能缩短垃圾收集时间不同, Parallel Scavenge更关注系统吞吐量:
系统吞吐量=运行用户代码时间/(运行用户代码时间+垃圾收集时间)
停顿时间越短就越适用于用户交互的程序-良好的响应速度能提升用户的体验;而高吞吐量则适用于后台运算而不需要太多交互的任务-可以最高效率地利用CPU时间,尽快地完成程序的运算任务. Parallel Scavenge提供了如下参数设置系统吞吐量:
|Parallel   Scavenge参数|描述|
|----|----|
|-XX:MaxGCPauseMillis|(毫秒数) 收集器将尽力保证内存回收花费的时间不超过设定值, 但如果太小将会导致GC的频率增加.|
|-XX:GCTimeRatio|(整数:0 < GCTimeRatio < 100) 是垃圾收集时间占总时间的比率|
|XX:+UseAdaptiveSizePolicy|启用GC自适应的调节策略: 不再需要手工指定-Xmn、-XX:SurvivorRatio、-XX:PretenureSizeThreshold等细节参数, VM会根据当前系统的运行情况收集性能监控信息, 动态调整这些参数以提供最合适的停顿时间或最大的吞吐量|
##### 3.3.4 Serial Old收集器
Serial Old是Serial收集器的老年代版本, 同样是单线程收集器,使用“标记-整理”算法
![4639175-57bdd50a23adaddd.gif](https://upload-images.jianshu.io/upload_images/4639175-57bdd50a23adaddd.gif)
clip_image023.gif
3.3.5 Parallel Old收集器
Parallel Old是Parallel Scavenge收集器的老年代版本, 使用多线程和“标记－整理”算法, 吞吐量优先, 主要与Parallel Scavenge配合在注重吞吐量及CPU资源敏感系统内使用；
![4639175-d8252a9299c6d04d.gif](https://upload-images.jianshu.io/upload_images/4639175-d8252a9299c6d04d.gif)
clip_image025.gif
##### 3.3.6 CMS收集器（Concurrent Mark Sweep）
CMS(Concurrent Mark Sweep)收集器是一款具有划时代意义的收集器, 一款真正意义上的并发收集器, 虽然现在已经有了理论意义上表现更好的G1收集器, 但现在主流互联网企业线上选用的仍是CMS(如Taobao、微店).
CMS是一种以获取最短回收停顿时间为目标的收集器(CMS又称多并发低暂停的收集器), 基于”标记-清除”算法实现, 整个GC过程分为以下4个步骤:
- 
初始标记(CMS initial mark)
- 
并发标记(CMS concurrent mark: GC Roots Tracing过程)
- 
重新标记(CMS remark)
- 
并发清除(CMS concurrent sweep: 已死对象将会就地释放, 注意:此处没有压缩)
其中1，3两个步骤(初始标记、重新标记)仍需STW. 但初始标记仅只标记一下GC Roots能直接关联到的对象, 速度很快; 而重新标记则是为了修正并发标记期间因用户程序继续运行而导致标记产生变动的那一部分对象的标记记录, 虽然一般比初始标记阶段稍长, 但要远小于并发标记时间.
![4639175-d2060e8057333fe4.gif](https://upload-images.jianshu.io/upload_images/4639175-d2060e8057333fe4.gif)
clip_image027.gif
CMS特点：
1.CMS默认启动的回收线程数=(CPU数目+3)4
当CPU数>4时, GC线程一般占用不超过25%的CPU资源, 但是当CPU数<=4时, GC线程可能就会过多的占用用户CPU资源, 从而导致应用程序变慢, 总吞吐量降低.
2.无法处理浮动垃圾, 可能出现Promotion Failure、Concurrent Mode Failure而导致另一次Full GC的产生: 浮动垃圾是指在CMS并发清理阶段用户线程运行而产生的新垃圾. 由于在GC阶段用户线程还需运行, 因此还需要预留足够的内存空间给用户线程使用, 导致CMS不能像其他收集器那样等到老年代几乎填满了再进行收集. 因此CMS提供了-XX:CMSInitiatingOccupancyFraction参数来设置GC的触发百分比(以及-XX:+UseCMSInitiatingOccupancyOnly来启用该触发百分比), 当老年代的使用空间超过该比例后CMS就会被触发(JDK 1.6之后默认92%). 但当CMS运行期间预留的内存无法满足程序需要, 就会出现上述Promotion Failure等失败, 这时VM将启动后备预案: 临时启用Serial Old收集器来重新执行Full GC(CMS通常配合大内存使用, 一旦大内存转入串行的Serial GC, 那停顿的时间就是大家都不愿看到的了).
3.最后, 由于CMS采用”标记-清除”算法实现, 可能会产生大量内存碎片. 内存碎片过多可能会导致无法分配大对象而提前触发Full GC. 因此CMS提供了-XX:+UseCMSCompactAtFullCollection开关参数, 用于在Full GC后再执行一个碎片整理过程. 但内存整理是无法并发的, 内存碎片问题虽然没有了, 但停顿时间也因此变长了, 因此CMS还提供了另外一个参数-XX:CMSFullGCsBeforeCompaction用于设置在执行N次不进行内存整理的Full GC后, 跟着来一次带整理的(默认为0: 每次进入Full GC时都进行碎片整理).
##### 3.3.7 分区收集- G1收集器
G1(Garbage-First)是一款面向服务端应用的收集器, 主要目标用于配备多颗CPU的服务器治理大内存.
- G1 is planned as the long term replacement for the Concurrent Mark-Sweep Collector (CMS).
-XX:+UseG1GC启用G1收集器.
与其他基于分代的收集器不同, G1将整个Java堆划分为多个大小相等的独立区域(Region), 虽然还保留有新生代和老年代的概念, 但新生代和老年代不再是物理隔离的了, 它们都是一部分Region(不需要连续)的集合.如：
![4639175-35e6f29a86e4081d.gif](https://upload-images.jianshu.io/upload_images/4639175-35e6f29a86e4081d.gif)
clip_image029.gif
每块区域既有可能属于O区、也有可能是Y区, 因此不需要一次就对整个老年代/新生代回收. 而是当线程并发寻找可回收的对象时, 有些区块包含可回收的对象要比其他区块多很多. 虽然在清理这些区块时G1仍然需要暂停应用线程, 但可以用相对较少的时间优先回收垃圾较多的Region. 这种方式保证了G1可以在有限的时间内获取尽可能高的收集效率.
G1的新生代收集跟ParNew类似: 存活的对象被转移到一个/多个Survivor Regions. 如果存活时间达到阀值, 这部分对象就会被提升到老年代.如图：
![4639175-80e2b49ab914c2b8.jpg](https://upload-images.jianshu.io/upload_images/4639175-80e2b49ab914c2b8.jpg)
clip_image031.jpg
![4639175-14dda1e8092edaf4.gif](https://upload-images.jianshu.io/upload_images/4639175-14dda1e8092edaf4.gif)
clip_image033.gif
其特定是：
一整块堆内存被分为多个Regions.
存活对象被拷贝到新的Survivor区或老年代.
年轻代内存由一组不连续的heap区组成, 这种方法使得可以动态调整各代区域尺寸.
Young GC会有STW事件, 进行时所有应用程序线程都会被暂停.
多线程并发GC.
G1老年代GC特点如下:
并发标记阶段
1 在与应用程序并发执行的过程中会计算活跃度信息.
2 这些活跃度信息标识出那些regions最适合在STW期间回收(which regions will be best to reclaim during an evacuation pause).
3 不像CMS有清理阶段.
再次标记阶段
1 使用Snapshot-at-the-Beginning(SATB)算法比CMS快得多.
2 空region直接被回收.
拷贝/清理阶段(Copying/Cleanup Phase)
1 年轻代与老年代同时回收.
2 老年代内存回收会基于他的活跃度信息.
### 4  JVM优化
#### 4.1 JDK常用JVM优化相关命令
|bin|描述|功能|
|----|----|----|
|jps|打印Hotspot VM进程|VMID、JVM参数、main()函数参数、主类名/Jar路径|
|jstat|查看Hotspot VM 运行时信息|类加载、内存、GC[可分代查看]、JIT编译   命令格式：jstat -gc 10340 250 20|
|jinfo|查看和修改虚拟机各项配置|-flag   name=value|
|jmap|heapdump: 生成VM堆转储快照、查询finalize执行队列、Java堆和永久代详细信息|jmap -dump:live,format=b,file=heap.bin   [VMID]|
|jstack|查看VM当前时刻的线程快照: 当前VM内每一条线程正在执行的方法堆栈集合|Thread.getAllStackTraces()提供了类似的功能|
|javap|查看经javac之后产生的JVM字节码代码|自动解析.class文件, 避免了去理解class文件格式以及手动解析class文件内容|
|jcmd|一个多功能工具, 可以用来导出堆, 查看Java进程、导出线程信息、   执行GC、查看性能相关数据等|几乎集合了jps、jstat、jinfo、jmap、jstack所有功能|
|jconsole|基于JMX的可视化监视、管理工具|可以查看内存、线程、类、CPU信息, 以及对JMX MBean进行管理|
|jvisualvm|JDK中最强大运行监视和故障处理工具|可以监控内存泄露、跟踪垃圾回收、执行时内存分析、CPU分析、线程分析…|
##### 4.1.1   jps
jps - l
显示线程id和执行线程的主类名
jps -v
显示线程id和执行线程的主类名和JVM配置信息
![4639175-05c0f5c8bc530cc5.jpg](https://upload-images.jianshu.io/upload_images/4639175-05c0f5c8bc530cc5.jpg)
clip_image035.jpg
##### 4.1.2  jstat
jstat -参数 线程id 执行时间（单位毫秒） 执行次数
jstat -gc 4488 30 10
```
S0C     S1C      S0U    S1U      EC       EU        
10752.0  10752.0  0.0    0.0      65536.0  18351.3   
OC         OU       PC     PU    YGC     YGCT    FGC    FGCT     GCT
173568.0    0.0     21504.0  2659.6  0      0.000    0      0.000    0.000
```
SXC - survivor 初始空间大小，单位字节。
SXU - survivor 使用空间大小， 单位字节。
EC - eden 初始空间大小
EU - eden 使用空间大小
OC - old 初始空间大小
OU - old 使用空间大小
PC - permanent 初始空间大小
PU - permanent 使用空间大小
YGC - youngGC 收集次数
YGCT - youngGC收集使用时长， 单位秒
FGC - fullGC收集次数
FGCT - fullGC收集使用时长
GCT - 总计收集使用总时长 YGCT+FGCT
##### 4.1.3  jvisualvm
一个JDK内置的图形化VM监视管理工具
##### 4.1.4  visualgc插件
#### 4.2 JVM常见参数
配置方式：java [options] MainClass [arguments]
options - JVM启动参数。 配置多个参数的时候，参数之间使用空格分隔。
参数命名： 常见为 -参数名
参数赋值： 常见为 -参数名=参数值  |  -参数名:参数值
##### 4.2.1 内存设置
-Xms:初始堆大小，JVM启动的时候，给定堆空间大小。
-Xmx:最大堆大小，JVM运行过程中，如果初始堆空间不足的时候，最大可以扩展到多少。
-Xmn：设置年轻代大小。整个堆大小=年轻代大小+年老代大小+持久代大小。持久代一般固定大小为64m，所以增大年轻代后，将会减小年老代大小。此值对系统性能影响较大，Sun官方推荐配置为整个堆的3/8。
-Xss： 设置每个线程的Java栈大小。JDK5.0以后每个线程Java栈大小为1M，以前每个线程堆栈大小为256K。根据应用的线程所需内存大小进行调整。在相同物理内存下，减小这个值能生成更多的线程。但是操作系统对一个进程内的线程数还是有限制的，不能无限生成，经验值在3000~5000左右。
-XX:NewSize=n:设置年轻代大小
-XX:NewRatio=n:设置年轻代和年老代的比值。如:为3，表示年轻代与年老代比值为1：3，年轻代占整个年轻代+年老代和的1/4
-XX:SurvivorRatio=n:年轻代中Eden区与两个Survivor区的比值。注意Survivor区有两个。如：3，表示Eden：Survivor=3：2，一个Survivor区占整个年轻代的1/5
-XX:MaxPermSize=n:设置持久代大小
-XX:MaxTenuringThreshold：设置垃圾最大年龄。如果设置为0的话，则年轻代对象不经过Survivor区，直接进入年老代。对于年老代比较多的应用，可以提高效率。如果将此值设置为一个较大值，则年轻代对象会在Survivor区进行多次复制，这样可以增加对象再年轻代的存活时间，增加在年轻代即被回收的概率。
##### 4.2.2 内存设置经验分享
JVM中最大堆大小有三方面限制：相关操作系统的数据模型（32-bt还是64-bit）限制；系统的可用虚拟内存限制；系统的可用物理内存限制。32位系统 下，一般限制在1.5G~2G；64为操作系统对内存无限制。
Tomcat配置方式： 编写catalina.bat|catalina.sh，增加JAVA_OPTS参数设置。windows和linux配置方式不同。windows - set "JAVA_OPTS=%JAVA_OPTS% 自定义参数"；linux - JAVA_OPTS="$JAVA_OPTS 自定义参数"
常见设置：
-Xmx3550m -Xms3550m -Xmn2g -Xss128k 适合开发过程的测试应用。要求物理内存大于4G。
-Xmx3550m -Xms3550m -Xss128k -XX:NewRatio=4 -XX:SurvivorRatio=4 -XX:MaxPermSize=160m -XX:MaxTenuringThreshold=0 适合高并发本地测试使用。且大数据对象相对较多（如IO流）
环境： 16G物理内存，高并发服务，重量级对象中等（线程池，连接池等），常用对象比例为40%（运行过程中产生的对象40%是生命周期较长的）
-Xmx10G -Xms10G -Xss1M -XX:NewRatio=3 -XX:SurvivorRatio=4 -XX:MaxPermSize=2048m -XX:MaxTenuringThreshold=5
##### 4.2.3 收集器设置
收集器配置的时候，次收集器和全收集器必须匹配。具体匹配规则参考[3.1.3](#_%E5%9E%83%E5%9C%BE%E5%9B%9E%E6%94%B6%E5%99%A8%E7%9A%84%E5%B8%B8%E8%A7%84%E5%8C%B9%E9%85%8D)
-XX:+UseSerialGC:设置串行收集器，年轻带收集器， 次收集器
-XX:+UseParallelGC:设置并行收集器
-XX:+UseParNewGC:设置年轻代为并行收集。可与CMS收集同时使用。JDK5.0以上，JVM会根据系统配置自行设置，所以无需再设置此值。
-XX:+UseParallelOldGC:设置并行年老代收集器，JDK6.0支持对年老代并行收集。
-XX:+UseConcMarkSweepGC:设置年老代并发收集器，测试中配置这个以后，-XX:NewRatio的配置失效，原因不明。所以，此时年轻代大小最好用-Xmn设置。
-XX:+UseG1GC:设置G1收集器
##### 4.2.4 垃圾回收统计信息
类似日志的配置信息。会有控制台相关信息输出。 商业项目上线的时候，不允许使用。一定使用loggc
-XX:+PrintGC
-XX:+Printetails
-XX:+PrintGCTimeStamps
-Xloggc:filename
##### 4.2.5 并行收集器设置
-XX:ParallelGCThreads=n:设置并行收集器收集时最大线程数使用的CPU数。并行收集线程数。
-XX:MaxGCPauseMillis=n:设置并行收集最大暂停时间，单位毫秒。可以减少STW时间。
-XX:GCTimeRatio=n:设置垃圾回收时间占程序运行时间的百分比。公式为1/(1+n)并发收集器设置
-XX:+CMSIncrementalMode:设置为增量模式。适用于单CPU情况。
-XX:+UseAdaptiveSizePolicy：设置此选项后，并行收集器会自动选择年轻代区大小和相应的Survivor区比例，以达到目标系统规定的最低相应时间或者收集频率等，此值建议使用并行收集器时，一直打开。
-XX:CMSFullGCsBeforeCompaction=n：由于并发收集器不对内存空间进行压缩、整理，所以运行一段时间以后会产生“碎片”，使得运行效率降低。此值设置运行多少次GC以后对内存空间进行压缩、整理。
-XX:+UseCMSCompactAtFullCollection：打开对年老代的压缩。可能会影响性能，但是可以消除碎片
##### 4.2.6 收集器设置经验分享
关于收集器的选择JVM给了三种选择：串行收集器、并行收集器、并发收集器，但是串行收集器只适用于小数据量的情况，所以这里的选择主要针对并行收集器和并发收集器。默认情况下，JDK5.0以前都是使用串行收集器，如果想使用其他收集器需要在启动时加入相应参数。JDK5.0以后，JVM会根据当前系统配置进行判断。
常见配置：
并行收集器主要以到达一定的吞吐量为目标，适用于科学计算和后台处理等。
-Xmx3800m -Xms3800m -Xmn2g -Xss128k -XX:+UseParallelGC -XX:ParallelGCThreads=20
使用ParallelGC作为并行收集器， GC线程为20（CPU核心数>=20时），内存问题根据硬件配置具体提供。建议使用物理内存的80%左右作为JVM内存容量。
-Xmx3550m -Xms3550m -Xmn2g -Xss128k -XX:+UseParallelGC -XX:ParallelGCThreads=20 -XX:+UseParallelOldGC
指定老年代收集器，在JDK5.0之后的版本，ParallelGC对应的全收集器就是ParallelOldGC。可以忽略
-Xmx3550m -Xms3550m -Xmn2g -Xss128k -XX:+UseParallelGC -XX:MaxGCPauseMillis=100
指定GC时最大暂停时间。单位是毫秒。每次GC最长使用100毫秒。可以尽可能提高工作线程的执行资源。
-Xmx3550m -Xms3550m -Xmn2g -Xss128k -XX:+UseParallelGC -XX:MaxGCPauseMillis=100 -XX:+UseAdaptiveSizePolicy
UseAdaptiveSizePolicy是提高年轻代GC效率的配置。次收集器执行效率。
并发收集器主要是保证系统的响应时间，减少垃圾收集时的停顿时间。适用于应用服务器、电信领域、互联网领域等。
-Xmx3550m -Xms3550m -Xmn2g -Xss128k -XX:ParallelGCThreads=20 -XX:+UseConcMarkSweepGC -XX:+UseParNewGC
指定年轻代收集器为ParNew，年老代收集器ConcurrentMarkSweep，并发GC线程数为20（CPU核心>=20），并发GC的线程数建议使用（CPU核心数+3）/4或CPU核心数【不推荐使用】。
-Xmx3550m -Xms3550m -Xmn2g -Xss128k -XX:+UseConcMarkSweepGC -XX:CMSFullGCsBeforeCompaction=5 -XX:+UseCMSCompactAtFullCollection
CMSFullGCsBeforeCompaction=5执行5次GC后，运行一次内存的整理。
UseCMSCompactAtFullCollection执行老年代内存整理。可以避免内存碎片，提高GC过程中的效率，减少停顿时间。
##### 4.2.7 简单总结
年轻代大小选择
响应时间优先的应用：尽可能设大，直到接近系统的最低响应时间限制（根据实际情况选择）。在此种情况下，年轻代收集发生的频率也是最小的。同时，减少到达年老代的对象。
吞吐量优先的应用：尽可能的设置大，可能到达Gbit的程度。因为对响应时间没有要求，垃圾收集可以并行进行，一般适合8CPU以上的应用。
年老代大小选择
响应时间优先的应用：年老代使用并发收集器，所以其大小需要小心设置，一般要考虑并发会话率和会话持续时间等一些参数。如果堆设置小了，可以会造成内存碎片、高回收频率以及应用暂停而使用传统的标记清除方式；如果堆大了，则需要较长的收集时间。最优化的方案，一般需要参考以下数据获得：
并发垃圾收集信息
持久代并发收集次数
传统GC信息
花在年轻代和年老代回收上的时间比例
减少年轻代和年老代花费的时间，一般会提高应用的效率
吞吐量优先的应用：一般吞吐量优先的应用都有一个很大的年轻代和一个较小的年老代。原因是，这样可以尽可能回收掉大部分短期对象，减少中期的对象，而年老代存放长期存活对象。
较小堆引起的碎片问题，因为年老代的并发收集器使用标记、清除算法，所以不会对堆进行压缩。当收集器回收时，他会把相邻的空间进行合并，这样可以分配给较大的对象。但是，当堆空间较小时，运行一段时间以后，就会出现“碎片”，如果并发收集器找不到足够的空间，那么并发收集器将会停止，然后使用传统的标记、整理方式进行回收。如果出现“碎片”，可能需要进行如下配置：
-XX:+UseCMSCompactAtFullCollection：使用并发收集器时，开启对年老代的压缩。
-XX:CMSFullGCsBeforeCompaction=0：上面配置开启的情况下，这里设置多少次Full GC后，对年老代进行压缩
##### 4.2.8 测试代码
```java
package jvm;
import java.io.IOException;
import java.lang.management.GarbageCollectorMXBean;
import java.lang.management.ManagementFactory;
import java.util.List;
public class Test {
    public static void main(String[] args) {
        List<GarbageCollectorMXBean> l = 
                ManagementFactory.getGarbageCollectorMXBeans();  
        for(GarbageCollectorMXBean b : l) {  
            System.out.println(b.getName());  
        }  
        try {
            System.in.read();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```
