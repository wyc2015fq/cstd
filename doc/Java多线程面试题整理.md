# Java多线程面试题整理

# 部分一：多线程部分：

#### 1) 什么是线程？

线程是操作系统能够进行运算调度的最小单位，它被包含在进程之中，是进程中的实际运作单位。程序员可以通过它进行多处理器编程，你可以使用多线程对运算密集型任务提速。比如，如果一个线程完成一个任务要100毫秒，那么用十个线程完成改任务只需10毫秒。Java在语言层面对多线程提供了卓越的支持，它也是一个很好的卖点。

 

#### 2) 线程和进程有什么区别？

线程是进程的子集，一个进程可以有很多线程，每条线程并行执行不同的任务。不同的进程使用不同的内存空间，而所有的线程共享一片相同的内存空间。别把它和栈内存搞混，每个线程都拥有单独的栈内存用来存储本地数据。

 

#### 3) 如何在Java中实现线程？

1)java.lang.Thread 类的实例就是一个线程但是它需要调用java.lang.Runnable接口来执行，

2)由于线程类本身就是调用的Runnable接口所以你可以继承java.lang.Thread 类或者直接调用Runnable接口来重写run()方法实现线程。

3).**实现Callable接口通过FutureTask包装器来创建Thread线程**

Callable接口（也只有一个方法）定义如下：   

```
public interface Callable<V>   { 
  V call（） throws Exception;   } 
```







```
public class SomeCallable<V> extends OtherClass implements Callable<V> {

    @Override
    public V call() throws Exception {
        // TODO Auto-generated method stub
        return null;
    }

}
```

 

```
Callable<V> oneCallable = new SomeCallable<V>();   
//由Callable<Integer>创建一个FutureTask<Integer>对象：   
FutureTask<V> oneTask = new FutureTask<V>(oneCallable);   
//注释：FutureTask<Integer>是一个包装器，它通过接受Callable<Integer>来创建，它同时实现了Future和Runnable接口。 
  //由FutureTask<Integer>创建一个Thread对象：   
Thread oneThread = new Thread(oneTask);   
oneThread.start();   
//至此，一个线程就创建完成了。
```





#### 4).使用ExecutorService、Callable、Future实现有返回结果的线程

ExecutorService、Callable、Future三个接口实际上都是属于Executor框架。返回结果的线程是在JDK1.5中引入的新特征，有了这种特征就不需要再为了得到返回值而大费周折了。而且自己实现了也可能漏洞百出。

可返回值的任务必须实现Callable接口。类似的，无返回值的任务必须实现Runnable接口。

执行Callable任务后，可以获取一个Future的对象，在该对象上调用get就可以获取到Callable任务返回的Object了。

注意：get方法是阻塞的，即：线程无返回结果，get方法会一直等待。

再结合线程池接口ExecutorService就可以实现传说中有返回结果的多线程了。

**5).Thread 类中的start() 和 run() 方法有什么区别**

1.start（）方法来启动线程，真正实现了多线程运行。这时无需等待run方法体代码执行完毕，可以直接继续执行下面的代码；通过调用Thread类的start()方法来启动一个线程， 这时此线程是处于就绪状态， 并没有运行。 然后通过此Thread类调用方法run()来完成其运行操作的， 这里方法run()称为线程体，它包含了要执行的这个线程的内容， Run方法运行结束， 此线程终止。然后CPU再调度其它线程。
2.run（）方法当作普通方法的方式调用。程序还是要顺序执行，要等待run方法体执行完毕后，才可继续执行下面的代码； 程序中只有主线程——这一个线程， 其程序执行路径还是只有一条， 这样就没有达到写线程的目的。

记住：多线程就是分时利用CPU，宏观上让所有线程一起执行 ，也叫并发

 

#### 6).Java中CyclicBarrier 和 CountDownLatch有什么不同

| CountDownLatch                                               | CyclicBarrier                                                |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| 减计数方式                                                   | 加计数方式                                                   |
| 计算为0时释放所有等待的线程                                  | 计数达到指定值时释放所有等待线程                             |
| 计数为0时，无法重置                                          | 计数达到指定值时，计数置为0重新开始                          |
| 调用countDown()方法计数减一，调用await()方法只进行阻塞，对计数没任何影响 | 调用await()方法计数加1，若加1后的值不等于构造方法的值，则线程阻塞 |
| 不可重复利用                                                 | 可重复利用                                                   |

 

#### 7).Java内存模型是什么？

Java内存模型规定和指引Java程序在不同的内存架构、CPU和操作系统间有确定性地行为。它在多线程的情况下尤其重要。Java内存模型对一个线程所做的变动能被其它线程可见提供了保证，它们之间是先行发生关系。这个关系定义了一些规则让程序员在并发编程时思路更清晰。比如，先行发生关系确保了：

- 线程内的代码能够按先后顺序执行，这被称为程序次序规则。
- 对于同一个锁，一个解锁操作一定要发生在时间上后发生的另一个锁定操作之前，也叫做管程锁定规则。
- 前一个对`volatile`的写操作在后一个`volatile`的读操作之前，也叫`volatile`变量规则。
- 一个线程内的任何操作必需在这个线程的start()调用之后，也叫作线程启动规则。
- 一个线程的所有操作都会在线程终止之前，线程终止规则。
- 一个对象的终结操作必需在这个对象构造完成之后，也叫对象终结规则。
- 可传递性

#### 8).Java中的volatile 变量是什么

**可见性，是指线程之间的可见性，一个线程修改的状态对另一个线程是可见的。**也就是一个线程修改的结果。另一个线程马上就能看到。比如：用volatile修饰的变量，就会具有可见性。volatile修饰的变量不允许线程内部缓存和重排序，即直接修改内存。所以对其他线程是可见的。但是这里需要注意一个问题，volatile只能让被他修饰内容具有可见性，但不能保证它具有原子性。比如  volatile int a = 0；之后有一个操作 a++；这个变量a具有可见性，但是a++  依然是一个非原子操作，也就是这个操作同样存在线程安全问题。

Java语言提供了一种稍弱的同步机制，即volatile变量，用来确保将变量的更新操作通知到其他线程。当把变量声明为volatile类型后，编译器与运行时都会注意到这个变量是共享的，因此不会将该变量上的操作与其他内存操作一起重排序。volatile变量不会被缓存在寄存器或者对其他处理器不可见的地方，因此在读取volatile类型的变量时总会返回最新写入的值。在访问volatile变量时不会执行加锁操作，因此也就不会使执行线程阻塞，因此volatile变量是一种比sychronized关键字更轻量级的同步机制。

当对非 volatile  变量进行读写的时候，每个线程先从内存拷贝变量到CPU缓存中。如果计算机有多个CPU，每个线程可能在不同的CPU上被处理，这意味着每个线程可以拷贝到不同的  CPU cache 中。而声明变量是 volatile 的，JVM 保证了每次读变量都从内存中读，跳过 CPU cache 这一步。

当一个变量定义为 volatile 之后，将具备两种特性：**保证此变量对所有的线程的可见性;禁止指令重排序优化。有volatile修饰的变量，赋值后多执行了一个“load addl $0x0, (%esp)”操作，这个操作相当于一个内存屏障（指令重排序时不能把后面的指令重排序到内存屏障之前的位置），只有一个CPU访问内存时，并不需要内存屏障；（什么是指令重排序：是指CPU采用了允许将多条指令不按程序规定的顺序分开发送给各相应电路单元处理）。**

#### 9).什么是线程安全？Vector是一个线程安全类吗？ 

如果你的代码所在的进程中有多个线程在同时运行，而这些线程可能会同时运行这段代码。如果每次运行结果和单线程运行的结果是一样的，而且其他的变量的值也和预期的是一样的，就是线程安全的。一个线程安全的计数器类的同一个实例对象在被多个线程使用的情况下也不会出现计算失误。很显然你可以将集合类分成两组，线程安全和非线程安全的。Vector  是用同步方法来实现线程安全的, 而和它相似的ArrayList不是线程安全的。

#### 10).Java中什么是竞态条件？ 举个例子说明。

当某个计算正确性取决于多个线程的交替执行时序时, 就会发生静态条件,即争取的结果要取决于运气, 最常见的静态条件就是"先检查后执行",通过一个可能失效的观测结果来决定下一步的动作. 

例如:

  `class` `Counter { ` `    ``protected` `long` `count = ``0``; ` `    ``public` `void` `add(``long` `value) { ` `        ``this``.count = ``this``.count + value; ` `    ``} ` `} `          

 观察线程A和B交错执行会发生什么，两个线程分别加了2和3到count变量上，两个线程执行结束后count变量的值应该等于5。然而由于两个线程是交叉执行的，两个线程从内存中读出的初始值都是0。然后各自加了2和3，并分别写回内存。最终的值并不是期望的5，而是最后写回内存的那个线程的值，上面例子中最后写回内存的是线程A，但实际中也可能是线程B。如果没有采用合适的同步机制，线程间的交叉执行情况就无法预料。
add()方法就是一个临界区,它会产生竞态条件。

#### 11).一个线程运行时发生异常会怎样

所以这里存在两种情形： 
① 如果该异常被捕获或抛出，则程序继续运行。 
② 如果异常没有被捕获该线程将会停止执行。 
Thread.UncaughtExceptionHandler是用于处理未捕获异常造成线程突然中断情况的一个内嵌接口。当一个未捕获异常将造成线程中断的时候JVM会使用Thread.getUncaughtExceptionHandler()来查询线程的UncaughtExceptionHandler，并将线程和异常作为参数传递给handler的uncaughtException()方法进行处理。

 

#### 12).线程间如何通信,进程间如何通信?

多线程间的通信: 1). 共享变量; 2),wait, notify; 3)Lock/Condition机制;  4).管道机制,创建管道输出流PipedOutputStream pos和管道输入流PipedInputStream  pis,将pos和pis匹配，pos.connect(pis),将pos赋给信息输入线程，pis赋给信息获取线程，就可以实现线程间的通讯了.

管道流虽然使用起来方便，但是也有一些缺点

1）管道流只能在两个线程之间传递数据

线程consumer1和consumer2同时从pis中read数据，当线程producer往管道流中写入一段数据后，每一个时刻只有一个线程能获取到数据，并不是两个线程都能获取到producer发送来的数据，因此一个管道流只能用于两个线程间的通讯。不仅仅是管道流，其他IO方式都是一对一传输。

2）管道流只能实现单向发送，如果要两个线程之间互通讯，则需要两个管道流.

进程间通信:

1）管道（Pipe）：管道可用于具有亲缘关系进程间的通信，允许一个进程和另一个与它有共同祖先的进程之间进行通信。
（2）命名管道（named pipe）：命名管道克服了管道没有名字的限制，因此，除具有管道所具有的功能外，它还允许无亲缘关 系 进程间的通信。命名管道在文件系统中有对应的文件名。命名管道通过命令mkfifo或系统调用mkfifo来创建。
（3）信号（Signal）：信号是比较复杂的通信方式，用于通知接受进程有某种事件发生，除了用于进程间通信外，进程还可以发送   信号给进程本身；linux除了支持Unix早期信号语义函数sigal外，还支持语义符合Posix.1标准的信号函数sigaction（实际上，该函数是基于BSD的，BSD为了实现可靠信号机制，又能够统一对外接口，用sigaction函数重新实现了signal函数）。
（4）消息（Message）队列：消息队列是消息的链接表，包括Posix消息队列system   V消息队列。有足够权限的进程可以向队列中添加消息，被赋予读权限的进程则可以读走队列中的消息。消息队列克服了信号承载信息量少，管道只能承载无格式字节流以及缓冲区大小受限等缺
（5）共享内存：使得多个进程可以访问同一块内存空间，是最快的可用IPC形式。是针对其他通信机制运行效率较低而设计的。往往与其它通信机制，如信号量结合使用，来达到进程间的同步及互斥。
（6）内存映射（mapped memory）：内存映射允许任何多个进程间通信，每一个使用该机制的进程通过把一个共享的文件映射到自己的进程地址空间来实现它。
（7）信号量（semaphore）：主要作为进程间以及同一进程不同线程之间的同步手段。

（8）套接口（Socket）：更为一般的进程间通信机制，可用于不同机器之间的进程间通信。起初是由Unix系统的BSD分支开发出来的，但现在一般可以移植到其它类Unix系统上：Linux和System V的变种都支持套接字。

#### 13).Java中notify 和 notifyAll有什么区别？

notify()&notifyall()的共同点：均能唤醒正在等待的线程，并且均是最后只有一个线程获取资源对象的锁。

​       不同点：notify() 只能唤醒一个线程，而notifyall()能够唤醒所有的线程，当线程被唤醒以后所有被唤醒的线程竞争获取资源对象的锁，其中只有一个能够得到对象锁，执行代码。

​      注意：wait()方法并不是在等待资源的锁，而是在等待被唤醒（notify()），一旦被唤醒后，被唤醒的线程就具备了资源锁（因为无需竞争），直至再次执行wait()方法或者synchronized代码块执行完毕。

#### 14).为什么wait, notify 和 notifyAll这些方法不在thread类里面？

一个很明显的原因是JAVA提供的锁是对象级的而不是线程级的，每个对象都有锁，通过线程获得。如果线程需要等待某些锁那么调用对象中的wait()方法就有意义了。如果wait()方法定义在Thread类中，线程正在等待的是哪个锁就不明显了。简单的说，由于wait，notify和notifyAll都是锁级别的操作，所以把他们定义在Object类中因为锁属于对象。

#### 15).什么是ThreadLocal变量？

ThreadLocal一般称为**线程本地变量，**它是一种特殊的线程绑定机制，将变量与线程**绑定**在一起，为每一个线程维护一个独立的变量副本。通过ThreadLocal可以将对象的可见范围限制在同一个线程内。

**跳出误区**

　　需要重点强调的的是，不要拿ThreadLocal和synchronized做类比，因为这种比较压根就是无意义的！sysnchronized是一种互斥同步机制，是为了保证在多线程环境下对于共享资源的正确访问。而ThreadLocal从本质上讲，无非是提供了一个**“线程级”**的**变量作用域**，它是一种**线程封闭**（每个线程独享变量）技术，更直白点讲，ThreadLocal可以理解为将对象的作用范围限制在一个**线程上下文**中，使得变量的作用域为“**线程级**”。

　　没有ThreadLocal的时候，一个线程在其声明周期内，可能穿过多个层级，多个方法，如果有个对象需要在此线程周期内多次调用，且是跨层级的（线程内共享），通常的做法是通过参数进行传递；而ThreadLocal将变量绑定在线程上，在一个线程周期内，无论“你身处何地”，只需通过其提供的get方法就可轻松获取到对象。极大地提高了对于“线程级变量”的访问便利性。

#### 16).Java中ThreadLocal变量, volatile变量, synchronized的区别

volatile主要是用来在多线程中同步变量。 
在一般情况下，为了提升性能，每个线程在运行时都会将主内存中的变量保存一份在自己的内存中作为变量副本，但是这样就很容易出现多个线程中保存的副本变量不一致，或与主内存的中的变量值不一致的情况。
而当一个变量被volatile修饰后，该变量就不能被缓存到线程的内存中，它会告诉编译器不要进行任何移出读取和写入操作的优化，换句话说就是不允许有不同于“主”内存区域的变量拷贝，所以当该变量有变化时，所有调用该变量的线程都会获得相同的值，这就确保了该变量在应用中的可视性(当一个任务做出了修改在应用中必须是可视的)，同时性能也相应的降低了(还是比synchronized高)。
但需要注意volatile只能确保操作的是同一块内存，并不能保证操作的原子性。所以volatile一般用于声明简单类型变量，使得这些变量具有原子性，即一些简单的赋值与返回操作将被确保不中断。但是当该变量的值由自身的上一个决定时，volatile的作用就将失效，这是由volatile关键字的性质所决定的。
所以在volatile时一定要谨慎，千万不要以为用volatile修饰后该变量的所有操作都是原子操作，不再需要synchronized关键字了。

ThreadLocal是一个线程的局部变量(其实就是一个Map),ThreadLocal会为每个使用该变量的线程提供独立的变量副本，所以每一个线程都可以独立地改变自己的副本，而不会影响其它线程所对应的副本。这样做其实就是以空间换时间的方式(与synchronized相反)，以耗费内存为代价，单大大减少了线程同步(如synchronized)所带来性能消耗以及减少了线程并发控制的复杂度。

synchronized关键字是Java利用锁的机制自动实现的，一般有同步方法和同步代码块两种使用方式。Java中所有的对象都自动含有单一的锁(也称为监视器)，当在对象上调用其任意的synchronized方法时，此对象被加锁(一个任务可以多次获得对象的锁，计数会递增)，同时在线程从该方法返回之前，该对象内其他所有要调用类中被标记为synchronized的方法的线程都会被阻塞。

#### 17).什么是Future, FutureTask？

　Future就是对于具体的Runnable或者Callable任务的执行结果进行取消、查询是否完成、获取结果。必要时可以通过get方法获取执行结果，该方法会阻塞直到任务返回结果。

　　Future类位于java.util.concurrent包下，它是一个接口：

  `public` `interface` `Future<V> {` `    ``boolean` `cancel(``boolean` `mayInterruptIfRunning);` `    ``boolean` `isCancelled();` `    ``boolean` `isDone();` `    ``V get() ``throws` `InterruptedException, ExecutionException;` `    ``V get(``long` `timeout, TimeUnit unit)` `        ``throws` `InterruptedException, ExecutionException, TimeoutException;` `}`          

 　　在Future接口中声明了5个方法，下面依次解释每个方法的作用：

- cancel方法用来取消任务，如果取消任务成功则返回true，如果取消任务失败则返回false。参数mayInterruptIfRunning表示是否允许取消正在执行却没有执行完毕的任务，如果设置true，则表示可以取消正在执行过程中的任务。如果任务已经完成，则无论mayInterruptIfRunning为true还是false，此方法肯定返回false，即如果取消已经完成的任务会返回false；如果任务正在执行，若mayInterruptIfRunning设置为true，则返回true，若mayInterruptIfRunning设置为false，则返回false；如果任务还没有执行，则无论mayInterruptIfRunning为true还是false，肯定返回true。
- isCancelled方法表示任务是否被取消成功，如果在任务正常完成前被取消成功，则返回 true。
- isDone方法表示任务是否已经完成，若任务完成，则返回true；
- get()方法用来获取执行结果，这个方法会产生阻塞，会一直等到任务执行完毕才返回；
- get(long timeout, TimeUnit unit)用来获取执行结果，如果在指定时间内，还没获取到结果，就直接返回null。

　　也就是说Future提供了三种功能：

　　1）判断任务是否完成；

　　2）能够中断任务；

　　3）能够获取任务执行结果。

  `public` `class` `FutureTask<V> ``implements` `RunnableFuture<V>`          

 　　**FutureTask**类实现了RunnableFuture接口，我们看一下RunnableFuture接口的实现：

  `public` `interface` `RunnableFuture<V> ``extends` `Runnable, Future<V> {` `    ``void` `run();` `}`          

在Java并发程序中FutureTask表示一个可以取消的异步运算。它有启动和取消运算、查询运算是否完成和取回运算结果等方法。只有当运算完成的时候结果才能取回，如果运算尚未完成get方法将会阻塞。一个FutureTask对象可以对调用了Callable和Runnable的对象进行包装，由于FutureTask也是调用了Runnable接口所以它可以提交给Executor来执行。

#### 18).Java中interrupted 和 isInterruptedd方法的区别？

*interrupted()* 和 *isInterrupted()*的主要区别是前者会将中断状态清除而后者不会。Java多线程的中断机制是用内部标识来实现的，调用*Thread.interrupt()*来中断一个线程就会设置中断标识为true。当中断线程调用[静态方法](http://java67.blogspot.com/2012/11/what-is-static-class-variable-method.html)*Thread.interrupted()*来检查中断状态时，中断状态会被清零。而非静态方法isInterrupted()用来查询其它线程的中断状态且不会改变中断状态标识。简单的说就是任何抛出InterruptedException异常的方法都会将中断状态清零。无论如何，一个线程的中断状态有有可能被其它线程调用中断来改变。

interrupt方法是用于中断线程的，调用该方法的线程的状态将被置为"中断"状态。注意：线程中断仅仅是设置线程的中断状态位，不会停止线程。需要用户自己去监视线程的状态为并做处理。支持线程中断的方法（也就是线程中断后会抛出InterruptedException的方法，比如这里的sleep，以及Object.wait等方法）就是在监视线程的中断状态，一旦线程的中断状态被置为“中断状态”，就会抛出中断异常。

interrupted方法的实现：

1. public static boolean interrupted() {  
2. ​    return currentThread().isInterrupted(true);  
3. }  


和isInterrupted的实现

 

1. public boolean isInterrupted() {  
2. ​    return isInterrupted(false);  
3. }  

这两个方法一个是static的，一个不是，但实际上都是在调用同一个方法，只是interrupted方法传入的参数为true，而inInterrupted传入的参数为false。这是一个native方法，看不到源码没有关系，参数名字ClearInterrupted已经清楚的表达了该参数的作用----是否清除中断状态。方法的注释也清晰的表达了“中断状态将会根据传入的ClearInterrupted参数值确定是否重置”。所以，静态方法interrupted将会清除中断状态（传入的参数ClearInterrupted为true），而实例方法isInterrupted则不会（传入的参数ClearInterrupted为false）。

#### 19).Java中volatile和原子类？

如果一个变量加了volatile关键字，就会告诉编译器和JVM的内存模型：这个变量是对所有线程共享的、可见的，每次jvm都会读取最新写入的值并使其最新值在所有CPU可见。volatile似乎是有时候可以代替简单的锁，似乎加了volatile关键字就省掉了锁。但又说volatile不能保证原子性（java程序员很熟悉这句话：volatile仅仅用来保证该变量对所有线程的可见性，但不保证原子性）。如果你的字段是volatile，Java内存模型将在写操作后插入一个写屏障指令，在读操作前插入一个读屏障指令。这意味着如果你对一个volatile字段进行写操作，你必须知道：1、一旦你完成写入，任何访问这个字段的线程将会得到最新的值。2、在你写入前，会保证所有之前发生的事已经发生，并且任何更新过的数据值也是可见的，因为内存屏障会把之前的写入值都刷新到缓存。

#### volatile为什么没有原子性?

明白了内存屏障（[memory barrier](http://en.wikipedia.org/wiki/Memory_barrier)）这个CPU指令，回到前面的JVM指令：从Load到store到内存屏障，一共4步，其中最后一步jvm让这个最新的变量的值在所有线程可见，也就是最后一步让所有的CPU内核都获得了最新的值，但中间的几步（从Load到Store）是不安全的，中间如果其他的CPU修改了值将会丢失。

**原子类保证了解决了上述的volatile的原子性没有保证的问题, 用到了CAS操作,** 

**因为CAS是基于乐观锁的，也就是说当写入的时候，如果寄存器旧值已经不等于现值，说明有其他CPU在修改，那就继续尝试。**所以这就保证了操作的原子性。

[![ConcurrencyCAS](https://images0.cnblogs.com/blog/28306/201402/191824497484728.png)](https://images0.cnblogs.com/blog/28306/201402/191824486252285.png)

#### 19).为什么wait和notify方法要在同步块中调用？

主要是因为Java API强制要求这样做，如果你不这么做，你的代码会抛出IllegalMonitorStateException异常。还有一个原因是为了避免wait和notify之间产生竞态条件。 

#### 20).Java中的同步集合与并发集合有什么区别

Synchronized vs Concurrent Collections

不管是同步集合还是并发集合他们都支持线程安全，他们之间主要的区别体现在**性能**和**可扩展性**，还有他们如何实现的线程安全。同步HashMap,  Hashtable, HashSet, Vector, ArrayList 相比他们并发的实现（比如：ConcurrentHashMap,  CopyOnWriteArrayList, CopyOnWriteHashSet）会慢得多。造成如此慢的主要原因是**锁**， 同步集合会把整个Map或List锁起来，而并发集合不会。并发集合实现线程安全是通过使用先进的和成熟的技术像锁剥离。比如ConcurrentHashMap 会把整个Map 划分成几个片段，只对相关的几个片段上锁，同时允许多线程访问其他未上锁的片段。

同样的，CopyOnWriteArrayList 允许多个线程以非同步的方式读，当有线程写的时候它会将整个List复制一个副本给它。

如果在读多写少这种对并发集合有利的条件下使用并发集合，这会比使用同步集合更具有可伸缩性。

同步集合与并发集合都为多线程和并发提供了合适的线程安全的集合，不过并发集合的可扩展性更高。在Java1.5之前程序员们只有同步集合来用且在多线程并发的时候会导致争用，阻碍了系统的扩展性。Java5介绍了并发集合像ConcurrentHashMap，不仅提供线程安全还用锁分离和内部分区等现代技术提高了可扩展性。

#### 21).Java中堆和栈有什么不同？

因为栈是一块和线程紧密相关的内存区域。每个线程都有自己的栈内存，用于存储本地变量，方法参数和栈调用，一个线程中存储的变量对其它线程是不可见的。而堆是所有线程共享的一片公用内存区域。对象都在堆里创建，为了提升效率线程会从堆中弄一个缓存到自己的栈，如果多个线程使用该变量就可能引发问题，这时volatile  变量就可以发挥作用了，它要求线程从主存中读取变量的值。

####  22).什么是线程池？ 为什么要使用它？

创建线程要花费昂贵的资源和时间，如果任务来了才创建线程那么响应时间会变长，而且一个进程能创建的线程数有限。为了避免这些问题，在程序启动的时候就创建若干线程来响应处理，它们被称为线程池，里面的线程叫工作线程。从JDK1.5开始，Java   API提供了Executor框架让你可以创建不同的线程池。比如单线程池，每次处理一个任务；数目固定的线程池或者是缓存线程池（一个适合很多生存期短的任务的程序的可扩展线程池）。

####  22).实现生产者消费者模式





```
package ProducterAndConsumer.Version1;

import java.util.Random;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * 生产者

 * 生产者消费者模型
 */

public class Producer implements Runnable {
    private volatile boolean isRunning = true;
    private BlockingQueue<PCData> queue;// 内存缓冲区
    private static AtomicInteger count = new AtomicInteger();// 总数 原子操作
    private static final int SLEEPTIME = 1000;

    public Producer(BlockingQueue<PCData> queue) {
        this.queue = queue;
    }

    @Override
    public void run() {
        PCData data = null;
        Random r = new Random();
        System.out.println("start producting id:" + Thread.currentThread().getId());
        try {
            while (isRunning) {
                Thread.sleep(r.nextInt(SLEEPTIME));
                data = new PCData(count.incrementAndGet());
                System.out.println(data + " 加入队列");
                if (!queue.offer(data, 2, TimeUnit.SECONDS)) {
                    System.err.println(" 加入队列失败");
                }
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
            Thread.currentThread().interrupt();
        }

    }

    public void stop() {
        isRunning = false;
    }
}
```













```
package ProducterAndConsumer.Version1;
/**
 * 消费者
 * 
 */
import java.text.MessageFormat;
import java.util.Random;
import java.util.concurrent.BlockingQueue;

public class Consumer implements Runnable{
    private BlockingQueue<PCData> queue;
    private static final int SLEEPTIME = 1000;
    public Consumer(BlockingQueue<PCData> queue){
        this.queue = queue;
    }

    @Override
    public void run() {
        System.out.println("start Consumer id :"+Thread.currentThread().getId());
        Random r = new Random();
        try{
            while(true){
                PCData data = queue.take();
                if(data != null)
                {
                    int re = data.getData() * data.getData();
                    System.out.println(MessageFormat.format("{0}*{1}={2}", data.getData(),data.getData(),re));
                    Thread.sleep(r.nextInt(SLEEPTIME));
                }
            }
        }catch (InterruptedException e) {
            e.printStackTrace();
            Thread.currentThread().interrupt();
        }
    }

}
```













```
package ProducterAndConsumer.Version1;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.LinkedBlockingDeque;

/**
 * 主函数
 * @author ctk
 *
 */
public class Main {
    public static void main(String[] args) throws InterruptedException {
        BlockingQueue<PCData> queue = new LinkedBlockingDeque<>(10);
        Producer p1 = new Producer(queue);
        Producer p2 = new Producer(queue);
        Producer p3 = new Producer(queue);
        Consumer c1 = new Consumer(queue);
        Consumer c2 = new Consumer(queue);
        Consumer c3 = new Consumer(queue);
        ExecutorService service = Executors.newCachedThreadPool();
        service.execute(p1);
        service.execute(p2);
        service.execute(p3);
        service.execute(c1);
        service.execute(c2);
        service.execute(c3);
        Thread.sleep(10*1000);
        p1.stop();
        p2.stop();
        p3.stop();
        Thread.sleep(3000);
        service.shutdown();
    }
}
```













```
package ProducterAndConsumer.Version1;
/**
 * 容器数据类型
 * @author ctk
 *
 */
public class PCData {
    private final int intData;
    public PCData(int d){
        intData = d;
    }
    public PCData(String d){
        intData = Integer.valueOf(d);
    }
    public int getData(){
        return intData;
    }
    @Override
    public String toString(){
        return "data:"+intData;
    }
}
```







#### 23).如何避免死锁？

Java多线程中的死锁
死锁是指两个或两个以上的进程在执行过程中，因争夺资源而造成的一种互相等待的现象，若无外力作用，它们都将无法推进下去。这是一个严重的问题，因为死锁会让你的程序挂起无法完成任务，死锁的发生必须满足以下四个条件：

- 互斥条件：一个资源每次只能被一个进程使用。
- 请求与保持条件：一个进程因请求资源而阻塞时，对已获得的资源保持不放。
- 不剥夺条件：进程已获得的资源，在末使用完之前，不能强行剥夺。
- 循环等待条件：若干进程之间形成一种头尾相接的循环等待资源关系。

避免死锁最简单的方法就是阻止循环等待条件，将系统中所有的资源设置标志位、排序，规定所有的进程申请资源必须以一定的顺序（升序或降序）做操作来避免死锁。

#### 24).Java中活锁和死锁有什么区别？

活锁和死锁类似，不同之处在于处于活锁的线程或进程的状态是不断改变的，活锁可以认为是一种特殊的饥饿。一个现实的活锁例子是两个人在狭小的走廊碰到，两个人都试着避让对方好让彼此通过，但是因为避让的方向都一样导致最后谁都不能通过走廊。简单的说就是，活锁和死锁的主要区别是前者进程的状态可以改变但是却不能继续执行。

#### 25)怎么检测一个线程是否拥有锁？

在java.lang.Thread中有一个方法叫holdsLock()，它返回true如果当且仅当当前线程拥有某个具体对象的锁。

#### 26).JVM中哪个参数是用来控制线程的栈堆栈小的

-Xss参数用来控制线程的堆栈大小。

#### 27).Java中synchronized 和 ReentrantLock 有什么不同？

相似点：

​        这两种同步方式有很多相似之处，它们都是加锁方式同步，而且都是阻塞式的同步，也就是说当如果一个线程获得了对象锁，进入了同步块，其他访问该同步块的线程都必须阻塞在同步块外面等待，而进行线程阻塞和唤醒的代价是比较高的（操作系统需要在用户态与内核态之间来回切换，代价很高，不过可以通过对锁优化进行改善）。

区别：

​       这两种方式最大区别就是对于Synchronized来说，它是java语言的关键字，是原生语法层面的互斥，需要jvm实现。而ReentrantLock它是JDK  1.5之后提供的API层面的互斥锁，需要lock()和unlock()方法配合try/finally语句块来完成。

 Synchronized进过编译，会在同步块的前后分别形成monitorenter和monitorexit这个两个字节码指令。在执行monitorenter指令时，首先要尝试获取对象锁。如果这个对象没被锁定，或者当前线程已经拥有了那个对象锁，把锁的计算器加1，相应的，在执行monitorexit指令时会将锁计算器就减1，当计算器为0时，锁就被释放了。如果获取对象锁失败，那当前线程就要阻塞，直到对象锁被另一个线程释放为止。

由于ReentrantLock是java.util.concurrent包下提供的一套互斥锁，相比Synchronized，ReentrantLock类提供了一些高级功能，主要有以下3项：

​        1.等待可中断，持有锁的线程长期不释放的时候，正在等待的线程可以选择放弃等待，这相当于Synchronized来说可以避免出现死锁的情况。

​        2.公平锁，多个线程等待同一个锁时，必须按照申请锁的时间顺序获得锁，Synchronized锁非公平锁，ReentrantLock默认的构造函数是创建的非公平锁，可以通过参数true设为公平锁，但公平锁表现的性能不是很好。

​        3.锁绑定多个条件，一个ReentrantLock对象可以同时绑定对个对象。

#### 28).有三个线程T1，T2，T3，怎么确保它们按顺序执行？

在多线程中有多种方法让线程按特定顺序执行，你可以用线程类的join()方法在一个线程中启动另一个线程，另外一个线程完成该线程继续执行。为了确保三个线程的顺序你应该先启动最后一个(T3调用T2，T2调用T1)，这样T1就会先完成而T3最后完成。

#### 29).Thread类中的yield方法有什么作用？

yield()方法可以让当前正在执行的线程暂停，但它**不会阻塞该线程**，它只是**将该线程从运行状态转入就绪状态**。

只是让当前的线程暂停一下，让系统的线程调度器重新调度一次。

很有可能，当某个线程调用了yield()方法暂停之后进入就绪状态，它又马上抢占了CPU的执行权，继续执行。

实际上，当某个线程调用了yield()方法暂停之后，只有**优先级与当前线程相同**，或者**优先级比当前线程更高**的处于就绪状态的线程才会获得执行的机会。

【关于sleep和yield的区别】

1.sleep()方法暂停当前线程后，会给其他线程执行机会，线程优先级对此没有影响。

yield()方法会给优先级相同或更高的线程更高的执行机会。

 

2.sleep()方法会将线程转入阻塞状态，直到阻塞时间结束，才会转入就绪状态。

yield()方法会将当前线程直接转入就绪状态。

 

3.sleep()方法声明抛出了InterruptedException异常，所以调用sleep()方法时要么捕捉该异常，要么显示声明抛出该异常。

yield()方法则没有声明抛出任何异常。

 

4.sleep()方法比yield()方法有更好的移植性，通常不建议使用yield()方法来控制并发线程的执行.

#### 30).Java中Semaphore是什么？

Java中的Semaphore是一种新的同步类，它是一个计数信号。从概念上讲，从概念上讲，信号量维护了一个许可集合。如有必要，在许可可用前会阻塞每一个  acquire()，然后再获取该许可。每个  release()添加一个许可，从而可能释放一个正在阻塞的获取者。但是，不使用实际的许可对象，Semaphore只对可用许可的号码进行计数，并采取相应的行动。信号量常常用于多线程的代码中，比如数据库连接池。

#### 31).如果你提交任务时，线程池队列已满。会时发会生什么？

如果一个任务不能被调度执行那么ThreadPoolExecutor’s submit()方法将会抛出一个RejectedExecutionException异常。

#### 32).Java线程池中submit() 和 execute()方法有什么区别？

1.对返回值的处理不同
execute方法不关心返回值。
submit方法有返回值，Future.
2.对异常的处理不同
excute方法会抛出异常。
sumbit方法不会抛出异常。除非你调用Future.get()

#### 33).什么是阻塞式方法？

阻塞式方法是指程序会一直等待该方法完成期间不做其他事情，ServerSocket的accept()方法就是一直等待客户端连接。这里的阻塞是指调用结果返回之前，当前线程会被挂起，直到得到结果之后才会返回。此外，还有异步和非阻塞式方法在任务完成前就返回。

#### 34)Swing是线程安全的吗？ 为什么？

Swing不是线程安全的，但是你应该解释这么回答的原因即便面试官没有问你为什么。当我们说swing不是线程安全的常常提到它的组件，这些组件不能在多线程中进行修改，所有对GUI组件的更新都要在AWT线程中完成，而Swing提供了同步和异步两种回调方法来进行更新。

#### 35).Java中invokeAndWait 和 invokeLater有什么区别？

invokeAndWait:后面的程序必须等这个线程(参数中的线程)的东西执行完才能执行
invokeLater:后面的程序和这个参数的线程对象可以并行,异步地执行

invokeLater一般用于在线程里修改swing组件的外观,因为swing组件是非同步的,所以不能在线程中直接修改,会不同步,得不到期望的效果,所以要把修改外观的代码放在一个单独的线程中,交给invokeLater:后面的程序和这个参数的线程对象可以并行,异步地执行.

#### 36).Swing API中那些方法是线程安全的？

这个问题又提到了swing和线程安全，虽然组件不是线程安全的但是有一些方法是可以被多线程安全调用的，比如repaint(),  revalidate()。 JTextComponent的setText()方法和JTextArea的insert() 和 append()  方法也是线程安全的。

#### 37).如何在Java中创建Immutable(不可变)对象？

Immutable对象可以在没有同步的情况下共享，降低了对该对象进行并发访问时的同步化开销。可是Java没有@Immutable这个注解符，要创建不可变类，要实现下面几个步骤：通过构造方法初始化所有成员、对变量不要提供setter方法、将所有的成员声明为私有的，这样就不允许直接访问这些成员、在getter方法中，不要直接返回对象本身，而是克隆对象，并返回对象的拷贝。

#### 38).Java中的ReadWriteLock是什么？

Java中的ReadWriteLock是Java 5  中新增的一个接口，一个ReadWriteLock维护一对关联的锁，一个用于只读操作一个用于写。在没有写线程的情况下一个读锁可能会同时被多个读线程持有。写锁是独占的，你可以使用JDK中的ReentrantReadWriteLock来实现这个规则，它最多支持65535个写锁和65535个读锁。

#### 39).多线程中的忙循环是什么?

忙循环就是程序员用循环让一个线程等待，不像传统方法wait(), sleep() 或 yield()  它们都放弃了CPU控制，而忙循环不会放弃CPU，它就是在运行一个空循环。这么做的目的是为了保留CPU缓存，在多核系统中，一个等待线程醒来的时候可能会在另一个内核运行，这样会重建缓存。为了避免重建缓存和减少等待重建的时间就可以使用它了。

#### 40).如果同步块内的线程抛出异常会发生什么？

无论你的同步块是正常还是异常退出的，里面的线程都会释放锁.

#### 41).单例模式的双检锁是什么？

```
public static Singleton getInstance()
{
  if (instance == null)
  {
    synchronized(Singleton.class) {  //1
      if (instance == null)          //2
        instance = new Singleton();  //3
    }
  }
  return instance;
}
```

双重检查锁定背后的理论是：在 //2 处的第二次检查使（如清单 3 中那样）创建两个不同的 `Singleton` 对象成为不可能。假设有下列事件序列：

1. 线程 1 进入 `getInstance()` 方法。 

   

2. 由于 `instance` 为 `null`，线程 1 在 //1 处进入 `synchronized` 块。 

   

3. 线程 1 被线程 2 预占。

   

4. 线程 2 进入 `getInstance()` 方法。

   

5. 由于 `instance` 仍旧为 `null`，线程 2 试图获取 //1 处的锁。然而，由于线程 1 持有该锁，线程 2 在 //1 处阻塞。

   

6. 线程 2 被线程 1 预占。

   

7. 线程 1 执行，由于在 //2 处实例仍旧为 `null`，线程 1 还创建一个 `Singleton` 对象并将其引用赋值给 `instance`。

   

8. 线程 1 退出 `synchronized` 块并从 `getInstance()` 方法返回实例。 

   

9. 线程 1 被线程 2 预占。

   

10. 线程 2 获取 //1 处的锁并检查 `instance` 是否为 `null`。 

    

11. 由于 `instance` 是非 `null` 的，并没有创建第二个 `Singleton` 对象，由线程 1 创建的对象被返回。

双重检查锁定背后的理论是完美的。不幸地是，现实完全不同。双重检查锁定的问题是：并不能保证它会在单处理器或多处理器计算机上顺利运行。

双重检查锁定失败的问题并不归咎于 JVM 中的实现 bug，而是归咎于 Java 平台内存模型。内存模型允许所谓的“无序写入”，这也是这些习语失败的一个主要原因。

无序写入

为解释该问题，需要重新考察上述清单 4 中的 //3 行。此行代码创建了一个 `Singleton` 对象并初始化变量 `instance` 来引用此对象。这行代码的问题是：在 `Singleton` 构造函数体执行之前，变量 `instance` 可能成为非 `null` 的。

```

```

#### 42).写出3条你遵循的多线程最佳实践

这种问题我最喜欢了，我相信你在写并发代码来提升性能的时候也会遵循某些最佳实践。以下三条最佳实践我觉得大多数Java程序员都应该遵循：

- 给你的线程起个有意义的名字。
  这样可以方便找bug或追踪。OrderProcessor, QuoteProcessor or  TradeProcessor 这种名字比 Thread-1. Thread-2 and Thread-3  好多了，给线程起一个和它要完成的任务相关的名字，所有的主要框架甚至JDK都遵循这个最佳实践。
- 避免锁定和缩小同步的范围
  锁花费的代价高昂且上下文切换更耗费时间空间，试试最低限度的使用同步和锁，缩小临界区。因此相对于同步方法我更喜欢同步块，它给我拥有对锁的绝对控制权。
- 多用同步类少用wait 和 notify
  首先，CountDownLatch, Semaphore, CyclicBarrier 和  Exchanger  这些同步类简化了编码操作，而用wait和notify很难实现对复杂控制流的控制。其次，这些类是由最好的企业编写和维护在后续的JDK中它们还会不断优化和完善，使用这些更高等级的同步工具你的程序可以不费吹灰之力获得优化。
- 多用并发集合少用同步集合
  这是另外一个容易遵循且受益巨大的最佳实践，并发集合比同步集合的可扩展性更好，所以在并发编程时使用并发集合效果更好。如果下一次你需要用到map，你应该首先想到用ConcurrentHashMap。

#### 43).Java中的fork join框架是什么？

fork  join框架是JDK7中出现的一款高效的工具，Java开发人员可以通过它充分利用现代服务器上的多处理器。它是专门为了那些可以递归划分成许多子模块设计的，目的是将所有可用的处理能力用来提升程序的性能。fork  join框架一个巨大的优势是它使用了工作窃取算法，可以完成更多任务的工作线程可以从其它线程中窃取任务来执行。

#### 44).Java多线程中调用wait() 和 sleep()方法有什么不同？

Java程序中wait 和 sleep都会造成某种形式的暂停，它们可以满足不同的需要。wait()方法用于线程间通信，如果等待条件为真且其它线程被唤醒时它会释放锁，而sleep()方法仅仅释放CPU资源或者让当前线程停止执行一段时间，但不会释放锁。

#  部分二：全量部分

## 1、一个".java"源文件中是否可以包括多个类（不是内部类）？有什么限制？ 

　　**可以有多个类，但只能有一个public的类，并且public的类名必须与文件名相一致。**

## 2、Java有没有goto? 

　　**java中的保留字，现在没有在java中使用。**

## 3、说说&和&&的区别。 

　　**&和&&都可以用作逻辑与的运算符，表示逻辑与（and），当运算符两边的表达式的结果都为true时，整个运算结果才为true，否则，只要有一方为false，则结果为false。**

　　**&&还具有短路的功能，即如果第一个表达式为false，则不再计算第二个表达式**，例如，对于if(str != null && !str.equals(“”))表达式，当str为null时，后面的表达式不会执行，所以不会出现NullPointerException如果将&&改为&，则会抛出NullPointerException异常。If(x==33 & ++y>0) y会增长，If(x==33 && ++y>0)不会增长

　　**&还可以用作位运算符，当&操作符两边的表达式不是boolean类型时，&表示按位与操作**，我们通常使用0x0f来与一个整数进行&运算，来获取该整数的最低4个bit位，例如，0x31 & 0x0f的结果为0x01。 

## 4、switch语句能否作用在byte上，能否作用在long上，能否作用在String上? 

　　**在switch（expr1）中，expr1只能是一个整数表达式或者枚举常量，整数表达式可以是int基本类型或Integer包装类型**，由于，byte,short,char都可以隐含转换为int，所以，这些类型以及这些类型的包装类型也是可以的。显然，long和String类型都不符合switch的语法规定，并且不能被隐式转换成int类型，所以，它们不能作用于swtich语句中。 

## 5、short s1 = 1; s1 = s1 + 1;有什么错? short s1 = 1; s1 += 1;有什么错? 

　　对于short s1 = 1; s1 = s1 + 1; 由于s1+1运算时会自动提升表达式的类型，所以结果是int型，再赋值给short类型s1时，编译器将报告需要强制转换类型的错误。

　　**对于short s1 = 1; s1 += 1;由于 += 是java语言规定的运算符，java编译器会对它进行特殊处理，因此可以正确编译。** 

## 6、char型变量中能不能存贮一个中文汉字?为什么? 

　　**char型变量是用来存储Unicode编码的字符的，unicode编码字符集中包含了汉字，所以，char型变量中当然可以存储汉字啦。不过，如果某个特殊的汉字没有被包含在unicode编码字符集中，那么，这个char型变量中就不能存储这个特殊汉字**。补充说明：**unicode编码占用两个字节，所以，char类型的变量也是占用两个字节。**

## 7、使用final关键字修饰一个变量时，是引用不能变，还是引用的对象不能变？ 

　　**使用final关键字修饰一个变量时，是指引用变量不能变，引用变量所指向的对象中的内容还是可以改变的**。

　　例如，对于如下语句：

 　　　　final StringBuffer a=new StringBuffer("immutable");
　　执行如下语句将报告编译期错误：

　　　　a=new StringBuffer("");
　　但是，执行如下语句则可以通过编译：

　　　　a.append(" broken!"); 

　　有人在定义方法的参数时，可能想采用如下形式来阻止方法内部修改传进来的参数对象：

　　　　public void method(final  StringBuffer  param){

　　　　} 

　　实际上，这是办不到的，在该方法内部仍然可以增加如下代码来修改参数对象：

　　　　param.append("a");

## 8、"=="和equals方法究竟有什么区别？

　　**“==”操作符专门用来比较两个变量的值是否相等，也就是用于比较变量所对应的内存中所存储的数值是否相同，要比较两个基本类型的数据或两个引用变量是否相等，只能用==操作符。**

　　**如果一个变量指向的数据是对象类型的，那么，这时候涉及了两块内存，对象本身占用一块内存（堆内存），变量也占用一块内存**(栈内存)，例如Objet obj = new Object();变量obj是一个内存，new Object()是另一个内存，此时，变量obj所对应的内存中存储的数值就是对象占用的那块内存的首地址。**对于指向对象类型的变量，如果要比较两个变量是否指向同一个对象，即要看这两个变量所对应的内存中的数值是否相等，这时候就需要用==操作符进行比较**。

　　**equals方法是用于比较两个独立对象的内容是否相同，就好比去比较两个人的长相是否相同，它比较的两个对象是独立的**。例如，对于下面的代码：

　　**String a=new String("foo");**

　　**String b=new String("foo");**

　　**两条new语句创建了两个对象，然后用a,b这两个变量分别指向了其中一个对象，这是两个不同的对象，它们的首地址是不同的，即a和b中存储的数值是不相同的，所以，表达式a==b将返回false，而这两个对象中的内容是相同的，所以，表达式a.equals(b)将返回true。**

　　在实际开发中，我们经常要比较传递进行来的字符串内容是否等，例如，String input = …;input.equals(“quit”)，许多人稍不注意就使用==进行比较了，这是错误的，记住，字符串的比较基本上都是使用equals方法。

　　如果一个类没有自己定义equals方法，那么它将继承Object类的equals方法，Object类的equals方法的实现代码如下：

　　**boolean equals(Object o){**

　　　　**return this==o;**

　　**}**

　　这说明，**如果一个类没有自己定义equals方法，它默认的equals方法（从Object 类继承的）就是使用==操作符，也是在比较两个变量指向的对象是否是同一对象，这时候使用equals和使用==会得到同样的结果，如果比较的是两个独立的对象则总返回false。****如果你编写的类希望能够比较该类创建的两个实例对象的内容是否相同，那么你必须覆盖equals方法，由你自己写代码来决定在什么情况即可认为两个对象的内容是相同的。**

## 9、静态变量和实例变量的区别？

　　**在语法定义上的区别：****静态变量前要加static****关键字，而实例变量前则不加**。

　　**在程序运行时的区别：实例变量属于某个对象的属性，必须创建了实例对象，其中的实例变量才会被分配空间，才能使用这个实例变量**。**静态变量不属于某个实例对象，而是属于类，所以也称为类变量，只要程序加载了类的字节码，不用创建任何实例对象，静态变量就会被分配空间，静态变量就可以被使用了**。总之，**实例变量必须创建对象后才可以通过这个对象来使用，静态变量则可以直接使用类名来引用。**

　　例如，对于下面的程序，无论创建多少个实例对象，永远都只分配了一个staticVar变量，并且每创建一个实例对象，这个staticVar就会加1；但是，每创建一个实例对象，就会分配一个instanceVar，即可能分配多个instanceVar，并且每个instanceVar的值都只自加了1次。

public class VariantTest{

　　public static int staticVar = 0; 

　　public int instanceVar = 0; 

　　public VariantTest(){

　　　　staticVar++;

　　　　instanceVar++;

　　　　System.out.println(“staticVar=” + staticVar + ”,instanceVar=” + instanceVar);

　　}

}

## 10、是否可以从一个static方法内部发出对非static方法的调用？

　　不可以。因为**非static方法是要与对象关联在一起的，必须创建一个对象后，才可以在该对象上进行方法调用，而static方法调用时不需要创建对象，可以直接调用**。也就是说，当一个static方法被调用时，可能还没有创建任何实例对象，如果从一个static方法中发出对非static方法的调用，那个非static方法是关联到哪个对象上的呢？这个逻辑无法成立，所以，**一个static方法内部不可以发出对非static****方法的调用**。

## 11、Integer与int的区别

　　**int是java提供的8种原始数据类型之一。Java为每个原始类型提供了封装类，Integer是java为int提供的封装类。****int的默认值为0，而Integer的默认值为null，即Integer可以区分出未赋值和值为0的区别，int则无法表达出未赋值的情况**，例如，要想表达出没有参加考试和考试成绩为0的区别，则只能使用Integer。**在JSP开发中，Integer的默认为null，所以用el表达式在文本框中显示时，值为空白字符串，而int默认的默认值为0，所以用el表达式在文本框中显示时，结果为0，所以，int不适合作为web层的表单数据的类型。**

　　在Hibernate中，如果将OID定义为Integer类型，那么Hibernate就可以根据其值是否为null而判断一个对象是否是临时的，如果将OID定义为了int类型，还需要在hbm映射文件中设置其unsaved-value属性为0。

　　另外，Integer提供了多个与整数相关的操作方法，例如，将一个字符串转换成整数，Integer中还定义了表示整数的最大值和最小值的常量。

## 12、请说出作用域public，private，protected，以及不写时的区别

这四个作用域的可见范围如下表所示。

说明：**如果在修饰的元素上面没有写任何访问修饰符，则表示friendly。**

![img](data:image/png;base64,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)

## 13、Overload和Override的区别。Overloaded的方法是否可以改变返回值的类型? 

　　**Overload是重载**的意思，**Override是覆盖**的意思，**也就是重写**。

　　**重载Overload表示同一个类中可以有多个名称相同的方法，但这些方法的参数列表各不相同（即参数个数或类型不同）。**

　　**重写Override表示子类中的方法可以与父类中的某个方法的名称和参数完全相同，通过子类创建的实例对象调用这个方法时，将调用子类中的定义方法，这相当于把父类中定义的那个完全相同的方法给覆盖了，这也是面向对象编程的多态性的一种表现。****子类覆盖父类的方法时，只能比父类抛出更少的异常，或者是抛出父类抛出的异常的子异常，因为子类可以解决父类的一些问题，不能比父类有更多的问题。子类方法的访问权限只能比父类的更大，不能更小。****如果父类的方法是private类型，那么，子类则不存在覆盖的限制，相当于子类中增加了一个全新的方法。**

　　至于Overloaded的方法是否可以改变返回值的类型这个问题，要看你倒底想问什么呢？这个题目很模糊。如果几个Overloaded的方法的参数列表不一样，它们的返回者类型当然也可以不一样。但我估计你想问的问题是：**如果两个方法的参数列表完全一样，是否可以让它们的返回值不同来实现重载**Overload？这是不行的，我们可以用反证法来说明这个问题，**因为我们有时候调用一个方法时也可以不定义返回结果变量，即不要关心其返回结果**，例如，我们调用map.remove(key)方法时，虽然remove方法有返回值，但是我们通常都不会定义接收返回结果的变量，这时候**假设该类中有两个名称和参数列表完全相同的方法，仅仅是返回类型不同，java就无法确定编程者倒底是想调用哪个方法了，因为它无法通过返回结果类型来判断。** 

　　**override可以翻译为覆盖，从字面就可以知道，它是覆盖了一个方法并且对其重写，以求达到不同的作用。**对我们来说**最熟悉的覆盖就是对接口方法的实现，在接口中一般只是对方法进行了声明，而我们在实现时，就需要实现接口声明的所有方法**。除了这个典型的用法以外，我们**在继承中也可能会在子类覆盖父类中的方法，在覆盖要注意以下的几点：**

　　**1、覆盖的方法的标志必须要和被覆盖的方法的标志完全匹配，才能达到覆盖的效果；**

　　**2、覆盖的方法的返回值必须和被覆盖的方法的返回一致；**

　　**3、覆盖的方法所抛出的异常必须和被覆盖方法的所抛出的异常一致，或者是其子类；**

　　**4、被覆盖的方法不能为private，否则在其子类中只是新定义了一个方法，并没有对其进行覆盖。**

　　**overload**对我们来说可能比较熟悉，**可以翻译为重载**，它是**指我们可以定义一些名称相同的方法，通过定义不同的输入参数来区分这些方法，然后再调用时，VM就会根据不同的参数样式，来选择合适的方法执行。****在使用重载要注意以下的几点：**

1. **在使用重载时只能通过不同的参数样式。**例如，不同的参数类型，不同的参数个数，不同的参数顺序（当然，同一方法内的几个参数类型必须不一样，例如可以是fun(int,float)，但是不能为fun(int,int)）；
2. **不能通过访问权限、返回类型、抛出的异常进行重载；**
3. **方法的异常类型和数目不会对重载造成影响；**
4. **对于继承来说，如果某一方法在父类中是访问权限是priavte，那么就不能在子类对其进行重载，如果定义的话，也只是定义了一个新方法，而不会达到重载的效果。**

## 14、has a与is a的区别

　　**is-a表示的是属于的关系**。比如兔子属于一种动物（继承关系）。

　　**has-a表示组合，包含关系**。比如兔子包含有腿，头等组件。

## 15、ClassLoader如何加载class？

　　**jvm里有多个类加载器，每个类加载器可以负责加载特定位置的类**，例如，**bootstrap类加载负责加载jre/lib/rt.jar中的类， 我们平时用的jdk中的类都位于rt.jar中。extclassloader负责加载jar/lib/ext/\*.jar中的类，appclassloader负责classpath指定的目录或jar中的类**。**除了bootstrap之外，其他的类加载器本身也都是java类，它们的父类是ClassLoader。**

## 16、分层设计的好处

　　**把各个功能按调用流程进行了模块化，模块化带来的好处就是可以随意组合**，举例说明：如果要注册一个用户，流程为显示界面并通过界面接收用户的输入，接着进行业务逻辑处理，在处理业务逻辑又访问数据库，如果我们将这些步骤全部按流水帐的方式放在一个方法中编写，这也是可以的，但这其中的坏处就是，当界面要修改时，由于代码全在一个方法内，可能会碰坏业务逻辑和数据库访问的码，同样，当修改业务逻辑或数据库访问的代码时，也会碰坏其他部分的代码。**分层就是要把界面部分、业务逻辑部分、数据库访问部分的代码放在各自独立的方法或类中编写，这样就不会出现牵一发而动全身的问题了。**这样分层后，还可以方便切换各层，譬如原来的界面是Swing，现在要改成BS界面，如果最初是按分层设计的，这时候不需要涉及业务和数据访问的代码，只需编写一条web界面就可以了。

  **分层的好处：**

　　**1.实现了软件之间的解耦；**

　　**2.便于进行分工**

　　**3.便于维护**

　　**4.提高软件组件的重用**

　　**5.便于替换某种产品**，比如持久层用的是hibernate,需要更换产品用toplink，就不用该其他业务代码，直接把配置一改。

　　**6.便于产品功能的扩展。**

　　**7.便于适用用户需求的不断变化**

## 17、hashCode方法的作用？

　　**hashcode这个方法是用来鉴定2个对象是否相等的。**

　　**equals方法和hashCode方法这2个方法都是用来判断2个对象是否相等的，但是他们是有区别的。**

　　**一般来讲，equals这个方法是给用户调用的，如果你想判断2个对象是否相等，你可以重写equals方法，然后在代码中调用，就可以判断他们是否相等了。简单来讲，equals方法主要是用来判断从表面上看或者从内容上看，2个对象是不是相等。**举个例子，有个学生类，属性只有姓名和性别，那么我们可以认为只要姓名和性别相等，那么就说这2个对象是相等的。

　　**hashcode方法一般用户不会去调用**，比如在hashmap中，由于key是不可以重复的，他在判断key是不是重复的时候就判断了hashcode这个方法，而且也用到了equals方法。这里不可以重复是说equals和hashcode只要有一个不等就可以了！所以简单来讲，**hashcode****相当于是一个对象的编码，就好像文件中的****md5，他和equals不同就在于他返回的是int型的，比较起来不直观。****我们一般在覆盖equals的同时也要覆盖hashcode，让他们的逻辑一致**。举个例子，还是刚刚的例子，如果姓名和性别相等就算2个对象相等的话，那么hashcode的方法也要返回姓名的hashcode值加上性别的hashcode值，这样从逻辑上，他们就一致了。

　　**要从物理上判断2个对象是否相等，用==就可以了，如果两个对象的物理(内存)地址相等，那么这两个对象肯定就是同一个对象。**

## 18、什么是AOP？

### **1.AOP概念介绍**

　　**所谓AOP，即Aspect orientied program,就是面向方面(切面)的编程。**

　　**面向切面编程Aspect-Orlented-Programming，即AOP是对面向对象的思维方式的有力补充。**

　　**AOP的好处是可以动态地添加和删除在切面上的逻辑而不影响原来的执行代码**

### **2.解释什么是方面(切面)**

　　**所谓方面(切面)**，**指的是贯穿到系统的各个模块中的系统一个功能就是一个方面****(切面****)**，比如，记录日志，统一异常处理，事务处理，权限检查，这些功能都是软件系统的一个面，而不是一点，在各个模块中都要出现。

### **3.什么是面向方面编程**

　　把系统的一个方面的功能封装成对象的形式来处理就是面向方面(切面)编程

### **4.怎么进行面向方面编程**

　　**把功能模块对应的对象作为切面嵌入到原来的各个系统模块中，采用代理技术，代理会调用目标，同时把切面功能的代码（对象）加入进来。**所以，用spring配置代理对象时只要要配两个属性，分别表示目标和切面对象（Advisor）。

##  19.谈谈你对mvc的理解

　　MVC是Model—View—Controler的简称。即模型—视图—控制器。MVC是一种设计模式，它强制性的把应用程序的输入、处理和输出分开。

　　MVC中的模型、视图、控制器它们分别担负着不同的任务。

- 视图: 视图是用户看到并与之交互的界面。视图向用户显示相关的数据，并接受用户的输入。视图不进行任何业务逻辑处理。
- 模型: 模型表示业务数据和业务处理。相当于JavaBean。一个模型能为多个视图提供数据。这提高了应用程序的重用性
- 控制器: 当用户单击Web页面中的提交按钮时,控制器接受请求并调用相应的模型去处理请求。然后根据处理的结果调用相应的视图来显示处理的结果。

　　MVC的处理过程：首先控制器接受用户的请求，调用相应的模型来进行业务处理，并返回数据给控制器。控制器调用相应的视图来显示处理的结果。并通过视图呈现给用户。

　　IT公司面试手册：http://www.mianwww.com/

 

# 部分三：

## 面向对象的三个特征

封装，继承，多态，这个应该是人人皆知，有时候也会加上抽象。

## 多态的好处

允许不同类对象对同一消息做出响应，即同一消息可以根据发送对象的不同而采用多种不同的行为方式(发送消息就是函数调用)。主要有以下优点：

1. 可替换性：多态对已存在代码具有可替换性
2. 可扩充性：增加新的子类不影响已经存在的类结构
3. 接口性：多态是超类通过方法签名,向子类提供一个公共接口,由子类来完善或者重写它来实现的。
4. 灵活性
5. 简化性

### 代码中如何实现多态

实现多态主要有以下三种方式：
\1. 接口实现 
\2. 继承父类重写方法 
\3. 同一类中进行方法重载

### 虚拟机是如何实现多态的

动态绑定技术(dynamic binding)，执行期间判断所引用对象的实际类型，根据实际类型调用对应的方法。

## 接口的意义

接口的意义用三个词就可以概括：规范，扩展，回调。

## 抽象类的意义

抽象类的意义可以用三句话来概括：

1. 为其他子类提供一个公共的类型
2. 封装子类中重复定义的内容
3. 定义抽象方法,子类虽然有不同的实现，但是定义时一致的

## 接口和抽象类的区别

| 比较         | 抽象类                                                       | 接口                                                       |
| ------------ | ------------------------------------------------------------ | ---------------------------------------------------------- |
| 默认方法     | 抽象类可以有默认的方法实现                                   | java 8之前,接口中不存在方法的实现.                         |
| 实现方式     | 子类使用extends关键字来继承抽象类.如果子类不是抽象类,子类需要提供抽象类中所声明方法的实现. | 子类使用implements来实现接口,需要提供接口中所有声明的实现. |
| 构造器       | 抽象类中可以有构造器,                                        | 接口中不能                                                 |
| 和正常类区别 | 抽象类不能被实例化                                           | 接口则是完全不同的类型                                     |
| 访问修饰符   | 抽象方法可以有public,protected和default等修饰                | 接口默认是public,不能使用其他修饰符                        |
| 多继承       | 一个子类只能存在一个父类                                     | 一个子类可以存在多个接口                                   |
| 添加新方法   | 想抽象类中添加新方法,可以提供默认的实现,因此可以不修改子类现有的代码 | 如果往接口中添加新方法,则子类中需要实现该方法.             |

##  

## 父类的静态方法能否被子类重写

不能。重写只适用于实例方法,不能用于静态方法，而子类当中含有和父类相同签名的静态方法，我们一般称之为隐藏。

## 什么是不可变对象

不可变对象指对象一旦被创建，状态就不能再改变。任何修改都会创建一个新的对象，如 String、Integer及其它包装类。

## 静态变量和实例变量的区别?

静态变量存储在方法区，属于类所有。实例变量存储在堆当中，其引用存在当前线程栈。

## 能否创建一个包含可变对象的不可变对象?

当然可以创建一个包含可变对象的不可变对象的，你只需要谨慎一点，不要共享可变对象的引用就可以了，如果需要变化时，就返回原对象的一个拷贝。最常见的例子就是对象中包含一个日期对象的引用。

## java 创建对象的几种方式

1. 采用new
2. 通过反射
3. 采用clone
4. 通过序列化机制

前2者都需要显式地调用构造方法。造成耦合性最高的恰好是第一种，因此你发现无论什么框架，只要涉及到解耦必先减少new的使用。

## switch中能否使用string做参数

在idk 1.7之前，switch只能支持byte, short, char, int或者其对应的封装类以及Enum类型。从idk 1.7之后switch开始支持String。

## switch能否作用在byte, long上?

可以用在byte上，但是不能用在long上。

## String s1=”ab”, String s2=”a”+”b”, String s3=”a”, String s4=”b”, s5=s3+s4请问s5==s2返回什么？

返回false。在编译过程中，编译器会将s2直接优化为”ab”，会将其放置在常量池当中，s5则是被创建在堆区，相当于s5=new String(“ab”);

## 你对String对象的intern()熟悉么?

intern()方法会首先从常量池中查找是否存在该常量值，如果常量池中不存在则现在常量池中创建，如果已经存在则直接返回。
比如 
String s1=”aa”; 
String s2=s1.intern(); 
System.out.print(s1==s2);//返回true

## Object中有哪些公共方法?

1. `equals()`

2. `clone()`

3. `getClass()`

4. `notify(),notifyAll(),wait()`

5. `toString`

    

## java当中的四种引用

强引用，软引用，弱引用，虚引用。不同的引用类型主要体现在GC上:

1. 强引用：如果一个对象具有强引用，它就不会被垃圾回收器回收。即使当前内存空间不足，JVM也不会回收它，而是抛出  OutOfMemoryError  错误，使程序异常终止。如果想中断强引用和某个对象之间的关联，可以显式地将引用赋值为null，这样一来的话，JVM在合适的时间就会回收该对象。
2. 软引用：在使用软引用时，如果内存的空间足够，软引用就能继续被使用，而不会被垃圾回收器回收，只有在内存不足时，软引用才会被垃圾回收器回收。
3. 弱引用：具有弱引用的对象拥有的生命周期更短暂。因为当 JVM 进行垃圾回收，一旦发现弱引用对象，无论当前内存空间是否充足，都会将弱引用回收。不过由于垃圾回收器是一个优先级较低的线程，所以并不一定能迅速发现弱引用对象。
4. 虚引用：顾名思义，就是形同虚设，如果一个对象仅持有虚引用，那么它相当于没有引用，在任何时候都可能被垃圾回收器回收。

 

更多了解参见深入对象引用：

http://blog.csdn.net/dd864140130/article/details/49885811

## WeakReference与SoftReference的区别?

这点在四种引用类型中已经做了解释,这里简单说明一下即可: 
虽然 WeakReference 与 SoftReference 都有利于提高 GC 和 内存的效率，但是 WeakReference ，一旦失去最后一个强引用，就会被 GC 回收，而软引用虽然不能阻止被回收，但是可以延迟到 JVM 内存不足的时候。

## 为什么要有不同的引用类型

不像C语言，我们可以控制内存的申请和释放，在Java中有时候我们需要适当的控制对象被回收的时机，因此就诞生了不同的引用类型，可以说不同的引用类型实则是对GC回收时机不可控的妥协。有以下几个使用场景可以充分的说明：

1. 利用软引用和弱引用解决OOM问题：用一个HashMap来保存图片的路径和相应图片对象关联的软引用之间的映射关系，在内存不足时，JVM会自动回收这些缓存图片对象所占用的空间，从而有效地避免了OOM的问题.
2. 通过软引用实现Java对象的高速缓存:比如我们创建了一Person的类，如果每次需要查询一个人的信息,哪怕是几秒中之前刚刚查询过的，都要重新构建一个实例，这将引起大量Person对象的消耗，并且由于这些对象的生命周期相对较短，会引起多次GC影响性能。此时，通过软引用和  HashMap 的结合可以构建高速缓存，提供性能。

## java中==和`eqauls()`的区别,`equals()`和`hashcode的区别

==是运算符，用于比较两个变量是否相等，而equals是Object类的方法，用于比较两个对象是否相等。默认Object类的equals方法是比较两个对象的地址，此时和==的结果一样。换句话说：基本类型比较用==，比较的是他们的值。默认下，对象用==比较时，比较的是内存地址，如果需要比较对象内容，需要重写equal方法。

## `equals()`和`hashcode()`的联系

`hashCode()`是Object类的一个方法，返回一个哈希值。如果两个对象根据equal()方法比较相等，那么调用这两个对象中任意一个对象的hashCode()方法必须产生相同的哈希值。
如果两个对象根据eqaul()方法比较不相等，那么产生的哈希值不一定相等(碰撞的情况下还是会相等的。)

## a.hashCode()有什么用?与a.equals(b)有什么关系

hashCode() 方法是相应对象整型的 hash 值。它常用于基于 hash 的集合类，如  Hashtable、HashMap、LinkedHashMap等等。它与 equals() 方法关系特别紧密。根据 Java 规范，使用  equal() 方法来判断两个相等的对象，必须具有相同的 hashcode。

将对象放入到集合中时，首先判断要放入对象的hashcode是否已经在集合中存在，不存在则直接放入集合。如果hashcode相等，然后通过equal()方法判断要放入对象与集合中的任意对象是否相等：如果equal()判断不相等，直接将该元素放入集合中，否则不放入。

## 有没有可能两个不相等的对象有相同的hashcode

有可能，两个不相等的对象可能会有相同的 hashcode 值，这就是为什么在 hashmap 中会有冲突。如果两个对象相等，必须有相同的hashcode 值，反之不成立。

## 可以在hashcode中使用随机数字吗?

不行，因为同一对象的 hashcode 值必须是相同的

## a==b与a.equals(b)有什么区别

如果a 和b 都是对象，则 a==b 是比较两个对象的引用，只有当 a 和 b 指向的是堆中的同一个对象才会返回 true，而  a.equals(b) 是进行逻辑比较，所以通常需要重写该方法来提供逻辑一致性的比较。例如，String 类重写 equals()  方法，所以可以用于两个不同对象，但是包含的字母相同的比较。

## `3*0.1==0.3`返回值是什么

false，因为有些浮点数不能完全精确的表示出来。

## a=a+b与a+=b有什么区别吗?

+=操作符会进行隐式自动类型转换，此处a+=b隐式的将加操作的结果类型强制转换为持有结果的类型，而a=a+b则不会自动进行类型转换。如：
byte a = 127; 
byte b = 127; 
b = a + b; // error : cannot convert from int to byte 
b += a; // ok 
（译者注：这个地方应该表述的有误，其实无论 a+b 的值为多少，编译器都会报错，因为 a+b 操作会将 a、b 提升为 int 类型，所以将 int 类型赋值给 byte 就会编译出错）

## short s1= 1; s1 = s1 + 1; 该段代码是否有错,有的话怎么改？

有错误，short类型在进行运算时会自动提升为int类型，也就是说`s1+1`的运算结果是int类型。

## short s1= 1; s1 += 1; 该段代码是否有错，有的话怎么改？

+=操作符会自动对右边的表达式结果强转匹配左边的数据类型，所以没错。

## & 和 &&的区别

首先记住&是位操作，而&&是逻辑运算符。另外需要记住逻辑运算符具有短路特性，而&不具备短路特性。

1.  

   public class Test{

2.  

      static String name;

3.  

    

4.  

      public static void main(String[] args){

5.  

   ​       if(name!=null&userName.equals("")){

6.  

   ​           System.out.println("ok");

7.  

   ​       }else{

8.  

   ​           System.out.println("erro");

9.  

   ​       }

10.  

       }

11.  

    }

 

以上代码将会抛出空指针异常。

## 一个java文件内部可以有类？(非内部类)

只能有一个public公共类，但是可以有多个default修饰的类。

## 如何正确的退出多层嵌套循环？

1. 使用标号和break;

2. 通过在外层循环中添加标识符

    

    

## 内部类的作用

内部类可以有多个实例，每个实例都有自己的状态信息，并且与其他外围对象的信息相互独立.在单个外围类当中，可以让多个内部类以不同的方式实现同一接口，或者继承同一个类.创建内部类对象的时刻不依赖于外部类对象的创建。内部类并没有令人疑惑的”is-a”管系，它就像是一个独立的实体。

内部类提供了更好的封装，除了该外围类，其他类都不能访问。

## final, finalize和finally的不同之处

final 是一个修饰符，可以修饰变量、方法和类。如果 final 修饰变量，意味着该变量的值在初始化后不能被改变。finalize  方法是在对象被回收之前调用的方法，给对象自己最后一个复活的机会，但是什么时候调用 finalize 没有保证。finally 是一个关键字，与  try 和 catch 一起用于异常的处理。finally 块一定会被执行，无论在 try 块中是否有发生异常。

## clone()是哪个类的方法?

java.lang.Cloneable 是一个标示性接口，不包含任何方法，clone 方法在 object 类中定义。并且需要知道 clone() 方法是一个本地方法，这意味着它是由 c 或 c++ 或 其他本地语言实现的。

## 深拷贝和浅拷贝的区别是什么?

浅拷贝：被复制对象的所有变量都含有与原来的对象相同的值，而所有的对其他对象的引用仍然指向原来的对象。换言之，浅拷贝仅仅复制所考虑的对象，而不复制它所引用的对象。

深拷贝：被复制对象的所有变量都含有与原来的对象相同的值，而那些引用其他对象的变量将指向被复制过的新对象，而不再是原有的那些被引用的对象。换言之，深拷贝把要复制的对象所引用的对象都复制了一遍。

## static都有哪些用法?

几乎所有的人都知道static关键字这两个基本的用法：静态变量和静态方法。也就是被static所修饰的变量/方法都属于类的静态资源，类实例所共享。

除了静态变量和静态方法之外，static也用于静态块，多用于初始化操作：

1.  

   public calss PreCache{

2.  

      static{

3.  

   ​       *//执行相关操作*

4.  

      }

5.  

   }

 

此外static也多用于修饰内部类，此时称之为静态内部类。

最后一种用法就是静态导包，即`import static`.import static是在JDK 1.5之后引入的新特性，可以用来指定导入某个类中的静态资源，并且不需要使用类名。资源名，可以直接使用资源名，比如：

1.  

   import static java.lang.Math.*;

2.  

    

3.  

   public class Test{

4.  

    

5.  

      public static void main(String[] args){

6.  

   ​       *//System.out.println(Math.sin(20));传统做法*

7.  

   ​       System.out.println(sin(20));

8.  

      }

9.  

   }

##  

## final有哪些用法

final也是很多面试喜欢问的地方，能回答下以下三点就不错了：
1.被final修饰的类不可以被继承 
2.被final修饰的方法不可以被重写 
3.被final修饰的变量不可以被改变。如果修饰引用，那么表示引用不可变，引用指向的内容可变。
4.被final修饰的方法，JVM会尝试将其内联，以提高运行效率 
5.被final修饰的常量，在编译阶段会存入常量池中。

回答出编译器对final域要遵守的两个重排序规则更好：
1.在构造函数内对一个final域的写入，与随后把这个被构造对象的引用赋值给一个引用变量,这两个操作之间不能重排序。
2.初次读一个包含final域的对象的引用，与随后初次读这个final域,这两个操作之间不能重排序。

# 数据类型相关

## java中int char,long各占多少字节?

| 类型   | 位数 | 字节数 |
| ------ | ---- | ------ |
| short  | 2    | 16     |
| int    | 4    | 32     |
| long   | 8    | 64     |
| float  | 4    | 32     |
| double | 8    | 64     |
| char   | 2    | 16     |

##  64位的JVM当中,int的长度是多少?

Java 中，int 类型变量的长度是一个固定值，与平台无关，都是 32 位。意思就是说，在 32 位 和 64 位 的Java 虚拟机中，int 类型的长度是相同的。

## int和Integer的区别

Integer是int的包装类型，在拆箱和装箱中，二者自动转换。int是基本类型，直接存数值，而integer是对象，用一个引用指向这个对象。

## int 和Integer谁占用的内存更多?

Integer 对象会占用更多的内存。Integer是一个对象，需要存储对象的元数据。但是 int 是一个原始类型的数据，所以占用的空间更少。

## String, StringBuffer和StringBuilder区别

String是字符串常量，final修饰：StringBuffer字符串变量(线程安全)；
StringBuilder 字符串变量(线程不安全)。

### String和StringBuffer

String和StringBuffer主要区别是性能：String是不可变对象，每次对String类型进行操作都等同于产生了一个新的String对象，然后指向新的String对象。所以尽量不在对String进行大量的拼接操作，否则会产生很多临时对象，导致GC开始工作，影响系统性能。

StringBuffer是对对象本身操作，而不是产生新的对象，因此在有大量拼接的情况下，我们建议使用StringBuffer。

但是需要注意现在JVM会对String拼接做一定的优化：
`String s=“This is only ”+”simple”+”test”`会被虚拟机直接优化成`String s=“This is only simple test”`，此时就不存在拼接过程。

### StringBuffer和StringBuilder

StringBuffer是线程安全的可变字符串，其内部实现是可变数组。StringBuilder是jdk 1.5新增的，其功能和StringBuffer类似，但是非线程安全。因此，在没有多线程问题的前提下，使用StringBuilder会取得更好的性能。

## 什么是编译器常量？使用它有什么风险？

公共静态不可变（public static final ）变量也就是我们所说的编译期常量，这里的 public  可选的。实际上这些变量在编译时会被替换掉，因为编译器知道这些变量的值，并且知道这些变量在运行时不能改变。这种方式存在的一个问题是你使用了一个内部的或第三方库中的公有编译时常量，但是这个值后面被其他人改变了，但是你的客户端仍然在使用老的值，甚至你已经部署了一个新的jar。为了避免这种情况，当你在更新依赖  JAR 文件时，确保重新编译你的程序。

## java当中使用什么类型表示价格比较好?

如果不是特别关心内存和性能的话，使用BigDecimal，否则使用预定义精度的 double 类型。

## 如何将byte转为String

可以使用 String 接收 byte[] 参数的构造器来进行转换，需要注意的点是要使用的正确的编码，否则会使用平台默认编码，这个编码可能跟原来的编码相同，也可能不同。

## 可以将int强转为byte类型么?会产生什么问题?

我们可以做强制转换，但是Java中int是32位的而byte是8 位的，所以,如果强制转化int类型的高24位将会被丢弃，byte 类型的范围是从-128到128

------

# 关于垃圾回收

## 你知道哪些垃圾回收算法?

垃圾回收从理论上非常容易理解,具体的方法有以下几种: 
1. 标记-清除 
2. 标记-复制 
3. 标记-整理 
4. 分代回收 
更详细的内容参见深入理解垃圾回收算法：

http://blog.csdn.net/dd864140130/article/details/50084471

## 如何判断一个对象是否应该被回收

这就是所谓的对象存活性判断，常用的方法有两种：1.引用计数法;　2.对象可达性分析。由于引用计数法存在互相引用导致无法进行GC的问题，所以目前JVM虚拟机多使用对象可达性分析算法。

## 简单的解释一下垃圾回收

Java  垃圾回收机制最基本的做法是分代回收。内存中的区域被划分成不同的世代，对象根据其存活的时间被保存在对应世代的区域中。一般的实现是划分成3个世代：年轻、年老和永久。内存的分配是发生在年轻世代中的。当一个对象存活时间足够长的时候，它就会被复制到年老世代中。对于不同的世代可以使用不同的垃圾回收算法。进行世代划分的出发点是对应用中对象存活时间进行研究之后得出的统计规律。一般来说，一个应用中的大部分对象的存活时间都很短。比如局部变量的存活时间就只在方法的执行过程中。基于这一点，对于年轻世代的垃圾回收算法就可以很有针对性。

## 调用System.gc()会发生什么?

通知GC开始工作，但是GC真正开始的时间不确定。

------

# 进程,线程相关

## 说说进程，线程，协程之间的区别

简而言之，进程是程序运行和资源分配的基本单位，一个程序至少有一个进程，一个进程至少有一个线程。进程在执行过程中拥有独立的内存单元，而多个线程共享内存资源，减少切换次数，从而效率更高。线程是进程的一个实体，是cpu调度和分派的基本单位，是比程序更小的能独立运行的基本单位。同一进程中的多个线程之间可以并发执行。

## 你了解守护线程吗？它和非守护线程有什么区别

程序运行完毕，jvm会等待非守护线程完成后关闭，但是jvm不会等待守护线程。守护线程最典型的例子就是GC线程。

## 什么是多线程上下文切换

多线程的上下文切换是指CPU控制权由一个已经正在运行的线程切换到另外一个就绪并等待获取CPU执行权的线程的过程。

## 创建两种线程的方式?他们有什么区别?

通过实现java.lang.Runnable或者通过扩展java.lang.Thread类。相比扩展Thread，实现Runnable接口可能更优.原因有二：

1. Java不支持多继承。因此扩展Thread类就代表这个子类不能扩展其他类。而实现Runnable接口的类还可能扩展另一个类。
2. 类可能只要求可执行即可，因此继承整个Thread类的开销过大。

## Thread类中的start()和run()方法有什么区别?

start()方法被用来启动新创建的线程，而且start()内部调用了run()方法，这和直接调用run()方法的效果不一样。当你调用run()方法的时候，只会是在原来的线程中调用，没有新的线程启动，start()方法才会启动新线程。

## 怎么检测一个线程是否持有对象监视器

Thread类提供了一个holdsLock(Object obj)方法，当且仅当对象obj的监视器被某条线程持有的时候才会返回true，注意这是一个static方法，这意味着”某条线程”指的是当前线程。

## Runnable和Callable的区别

Runnable接口中的run()方法的返回值是void，它做的事情只是纯粹地去执行run()方法中的代码而已；Callable接口中的call()方法是有返回值的，是一个泛型，和Future、FutureTask配合可以用来获取异步执行的结果。 
这其实是很有用的一个特性，因为多线程相比单线程更难、更复杂的一个重要原因就是因为多线程充满着未知性，某条线程是否执行了？某条线程执行了多久？某条线程执行的时候我们期望的数据是否已经赋值完毕？无法得知，我们能做的只是等待这条多线程的任务执行完毕而已。而Callable+Future/FutureTask却可以方便获取多线程运行的结果，可以在等待时间太长没获取到需要的数据的情况下取消该线程的任务。

## 什么导致线程阻塞

阻塞指的是暂停一个线程的执行以等待某个条件发生（如某资源就绪），学过操作系统的同学对它一定已经很熟悉了。Java 提供了大量方法来支持阻塞，下面让我们逐一分析。

| 方法                  | 说明                                                         |
| --------------------- | ------------------------------------------------------------ |
| sleep()               | sleep() 允许 指定以毫秒为单位的一段时间作为参数，它使得线程在指定的时间内进入阻塞状态，不能得到CPU  时间，指定的时间一过，线程重新进入可执行状态。 典型地，sleep()  被用在等待某个资源就绪的情形：测试发现条件不满足后，让线程阻塞一段时间后重新测试，直到条件满足为止 |
| suspend() 和 resume() | 两个方法配套使用，suspend()使得线程进入阻塞状态，并且不会自动恢复，必须其对应的resume()  被调用，才能使得线程重新进入可执行状态。典型地，suspend() 和 resume()  被用在等待另一个线程产生的结果的情形：测试发现结果还没有产生后，让线程阻塞，另一个线程产生了结果后，调用 resume() 使其恢复。 |
| yield()               | yield() 使当前线程放弃当前已经分得的CPU 时间，但不使当前线程阻塞，即线程仍处于可执行状态，随时可能再次分得 CPU 时间。调用 yield() 的效果等价于调度程序认为该线程已执行了足够的时间从而转到另一个线程 |
| wait() 和 notify()    | 两个方法配套使用，wait() 使得线程进入阻塞状态，它有两种形式，一种允许 指定以毫秒为单位的一段时间作为参数，另一种没有参数，前者当对应的 notify() 被调用或者超出指定时间时线程重新进入可执行状态，后者则必须对应的 notify() 被调用。 |

##  wait(),notify()和suspend(),resume()之间的区别

初看起来它们与 suspend() 和 resume() 方法对没有什么分别，但是事实上它们是截然不同的。区别的核心在于，前面叙述的所有方法，阻塞时都不会释放占用的锁（如果占用了的话），而这一对方法则相反。上述的核心区别导致了一系列的细节上的区别。

首先，前面叙述的所有方法都隶属于 Thread 类，但是这一对却直接隶属于 Object  类，也就是说，所有对象都拥有这一对方法。初看起来这十分不可思议，但是实际上却是很自然的，因为这一对方法阻塞时要释放占用的锁，而锁是任何对象都具有的，调用任意对象的  wait() 方法导致线程阻塞，并且该对象上的锁被释放。而调用 任意对象的notify()方法则导致从调用该对象的 wait()  方法而阻塞的线程中随机选择的一个解除阻塞（但要等到获得锁后才真正可执行）。

其次，前面叙述的所有方法都可在任何位置调用，但是这一对方法却必须在 synchronized  方法或块中调用，理由也很简单，只有在synchronized  方法或块中当前线程才占有锁，才有锁可以释放。同样的道理，调用这一对方法的对象上的锁必须为当前线程所拥有，这样才有锁可以释放。因此，这一对方法调用必须放置在这样的  synchronized  方法或块中，该方法或块的上锁对象就是调用这一对方法的对象。若不满足这一条件，则程序虽然仍能编译，但在运行时会出现IllegalMonitorStateException  异常。

wait() 和 notify()  方法的上述特性决定了它们经常和synchronized关键字一起使用，将它们和操作系统进程间通信机制作一个比较就会发现它们的相似性：synchronized方法或块提供了类似于操作系统原语的功能，它们的执行不会受到多线程机制的干扰，而这一对方法则相当于  block 和wakeup 原语（这一对方法均声明为  synchronized）。它们的结合使得我们可以实现操作系统上一系列精妙的进程间通信的算法（如信号量算法），并用于解决各种复杂的线程间通信问题。

关于 wait() 和 notify() 方法最后再说明两点： 
第一：调用 notify() 方法导致解除阻塞的线程是从因调用该对象的 wait() 方法而阻塞的线程中随机选取的，我们无法预料哪一个线程将会被选择，所以编程时要特别小心，避免因这种不确定性而产生问题。

第二：除了 notify()，还有一个方法 notifyAll() 也可起到类似作用，唯一的区别在于，调用 notifyAll()  方法将把因调用该对象的 wait() 方法而阻塞的所有线程一次性全部解除阻塞。当然，只有获得锁的那一个线程才能进入可执行状态。

谈到阻塞，就不能不谈一谈死锁，略一分析就能发现，suspend() 方法和不指定超时期限的 wait() 方法的调用都可能产生死锁。遗憾的是，Java 并不在语言级别上支持死锁的避免，我们在编程中必须小心地避免死锁。

以上我们对 Java 中实现线程阻塞的各种方法作了一番分析，我们重点分析了 wait() 和 notify()  方法，因为它们的功能最强大，使用也最灵活，但是这也导致了它们的效率较低，较容易出错。实际使用中我们应该灵活使用各种方法，以便更好地达到我们的目的。

## 产生死锁的条件

1.互斥条件：一个资源每次只能被一个进程使用。 
2.请求与保持条件：一个进程因请求资源而阻塞时，对已获得的资源保持不放。 
3.不剥夺条件:进程已获得的资源，在末使用完之前，不能强行剥夺。 
4.循环等待条件:若干进程之间形成一种头尾相接的循环等待资源关系。

## 为什么wait()方法和notify()/notifyAll()方法要在同步块中被调用

这是JDK强制的，wait()方法和notify()/notifyAll()方法在调用前都必须先获得对象的锁

## wait()方法和notify()/notifyAll()方法在放弃对象监视器时有什么区别

wait()方法和notify()/notifyAll()方法在放弃对象监视器的时候的区别在于：wait()方法立即释放对象监视器，notify()/notifyAll()方法则会等待线程剩余代码执行完毕才会放弃对象监视器。

## wait()与sleep()的区别

关于这两者已经在上面进行详细的说明,这里就做个概括好了:

- sleep()来自Thread类，和wait()来自Object类。调用sleep()方法的过程中，线程不会释放对象锁。而 调用 wait 方法线程会释放对象锁

- sleep()睡眠后不出让系统资源，wait让其他线程可以占用CPU

- sleep(milliseconds)需要指定一个睡眠时间，时间一到会自动唤醒.而wait()需要配合notify()或者notifyAll()使用

   

   

## 为什么wait, nofity和nofityAll这些方法不放在Thread类当中

一个很明显的原因是JAVA提供的锁是对象级的而不是线程级的，每个对象都有锁，通过线程获得。如果线程需要等待某些锁那么调用对象中的wait()方法就有意义了。如果wait()方法定义在Thread类中，线程正在等待的是哪个锁就不明显了。简单的说，由于wait，notify和notifyAll都是锁级别的操作，所以把他们定义在Object类中因为锁属于对象。

## 怎么唤醒一个阻塞的线程

如果线程是因为调用了wait()、sleep()或者join()方法而导致的阻塞，可以中断线程，并且通过抛出InterruptedException来唤醒它；如果线程遇到了IO阻塞，无能为力，因为IO是操作系统实现的，Java代码并没有办法直接接触到操作系统。

## 什么是多线程的上下文切换

多线程的上下文切换是指CPU控制权由一个已经正在运行的线程切换到另外一个就绪并等待获取CPU执行权的线程的过程。

## synchronized和ReentrantLock的区别

synchronized是和if、else、for、while一样的关键字，ReentrantLock是类，这是二者的本质区别。既然ReentrantLock是类，那么它就提供了比synchronized更多更灵活的特性，可以被继承、可以有方法、可以有各种各样的类变量，ReentrantLock比synchronized的扩展性体现在几点上： 
（1）ReentrantLock可以对获取锁的等待时间进行设置，这样就避免了死锁 
（2）ReentrantLock可以获取各种锁的信息 
（3）ReentrantLock可以灵活地实现多路通知 
另外，二者的锁机制其实也是不一样的:ReentrantLock底层调用的是Unsafe的park方法加锁，synchronized操作的应该是对象头中mark word。

## FutureTask是什么

这个其实前面有提到过，FutureTask表示一个异步运算的任务。FutureTask里面可以传入一个Callable的具体实现类，可以对这个异步运算的任务的结果进行等待获取、判断是否已经完成、取消任务等操作。当然，由于FutureTask也是Runnable接口的实现类，所以FutureTask也可以放入线程池中。

## 一个线程如果出现了运行时异常怎么办?

如果这个异常没有被捕获的话，这个线程就停止执行了。另外重要的一点是：如果这个线程持有某个某个对象的监视器，那么这个对象监视器会被立即释放。

## Java当中有哪几种锁

1. 自旋锁:  自旋锁在JDK1.6之后就默认开启了。基于之前的观察，共享数据的锁定状态只会持续很短的时间，为了这一小段时间而去挂起和恢复线程有点浪费，所以这里就做了一个处理，让后面请求锁的那个线程在稍等一会，但是不放弃处理器的执行时间，看看持有锁的线程能否快速释放。为了让线程等待，所以需要让线程执行一个忙循环也就是自旋操作。在jdk6之后，引入了自适应的自旋锁，也就是等待的时间不再固定了，而是由上一次在同一个锁上的自旋时间及锁的拥有者状态来决定。
2. 偏向锁: 在JDK1.之后引入的一项锁优化，目的是消除数据在无竞争情况下的同步原语。进一步提升程序的运行性能。  偏向锁就是偏心的偏，意思是这个锁会偏向第一个获得他的线程，如果接下来的执行过程中，改锁没有被其他线程获取，则持有偏向锁的线程将永远不需要再进行同步。偏向锁可以提高带有同步但无竞争的程序性能，也就是说他并不一定总是对程序运行有利，如果程序中大多数的锁都是被多个不同的线程访问，那偏向模式就是多余的，在具体问题具体分析的前提下，可以考虑是否使用偏向锁。
3. 轻量级锁: 为了减少获得锁和释放锁所带来的性能消耗，引入了“偏向锁”和“轻量级锁”，所以在Java  SE1.6里锁一共有四种状态，无锁状态，偏向锁状态，轻量级锁状态和重量级锁状态，它会随着竞争情况逐渐升级。锁可以升级但不能降级，意味着偏向锁升级成轻量级锁后不能降级成偏向锁。

## 如何在两个线程间共享数据

通过在线程之间共享对象就可以了，然后通过wait/notify/notifyAll、await/signal/signalAll进行唤起和等待，比方说阻塞队列BlockingQueue就是为线程之间共享数据而设计的。

## 如何正确的使用wait()？使用if还是while？

wait() 方法应该在循环调用，因为当线程获取到 CPU 开始执行的时候，其他条件可能还没有满足，所以在处理前，循环检测条件是否满足会更好。下面是一段标准的使用 wait 和 notify 方法的代码：

1.  

   synchronized (obj) {

2.  

      while (condition does not hold)

3.  

   ​     obj.wait(); *// (Releases lock, and reacquires on wakeup)*

4.  

   ​     ... *// Perform action appropriate to condition*

5.  

   }

##  

## 什么是线程局部变量ThreadLocal

线程局部变量是局限于线程内部的变量，属于线程自身所有，不在多个线程间共享。Java提供ThreadLocal类来支持线程局部变量，是一种实现线程安全的方式。但是在管理环境下（如  web  服务器）使用线程局部变量的时候要特别小心，在这种情况下，工作线程的生命周期比任何应用变量的生命周期都要长。任何线程局部变量一旦在工作完成后没有释放，Java  应用就存在内存泄露的风险。

## ThreadLoal的作用是什么?

简单说ThreadLocal就是一种以空间换时间的做法在每个Thread里面维护了一个ThreadLocal.ThreadLocalMap把数据进行隔离，数据不共享，自然就没有线程安全方面的问题了。

## 生产者消费者模型的作用是什么?

（1）通过平衡生产者的生产能力和消费者的消费能力来提升整个系统的运行效率，这是生产者消费者模型最重要的作用。
（2）解耦，这是生产者消费者模型附带的作用，解耦意味着生产者和消费者之间的联系少，联系越少越可以独自发展而不需要收到相互的制约。

## 写一个生产者-消费者队列

可以通过阻塞队列实现，也可以通过wait-notify来实现。

### 使用阻塞队列来实现

```java
//消费者
public class Producer implements Runnable{
private final BlockingQueue<Integer> queue;
public Producer(BlockingQueue q){
       this.queue=q;
}
@Override
public void run() {
  try {
      while (true){
          Thread.sleep(1000);*//模拟耗时*
          queue.put(produce());
      }
  }catch (InterruptedException e){
  }
}
private int produce() {
   int n=new Random().nextInt(10000);
   System.out.println("Thread:" + Thread.currentThread().getId() + " produce:" + n);
   return n;
}
}
*//消费者*
public class Consumer implements Runnable {
private final BlockingQueue<Integer> queue;
public Consumer(BlockingQueue q){
    this.queue=q;
}
@Override
public void run() {
    while (true){
        try {
            Thread.sleep(2000);*//模拟耗时*
            consume(queue.take());
        }catch (InterruptedException e){
        }
    }
}
private void consume(Integer n) {
?       System.out.println("Thread:" + Thread.currentThread().getId() + " consume:" + n);
}
}
//测试
public class Main {
public static void main(String[] args) {
    BlockingQueue<Integer> queue=new ArrayBlockingQueue<Integer>(100);
    Producer p=new Producer(queue);
    Consumer c1=new Consumer(queue);
    Consumer c2=new Consumer(queue);
    new Thread(p).start();
    new Thread(c1).start();
    new Thread(c2).start();
}
}
```

   


###  

### 使用wait-notify来实现

该种方式应该最经典，这里就不做说明了。

## 如果你提交任务时，线程池队列已满，这时会发生什么

如果你使用的LinkedBlockingQueue，也就是无界队列的话，没关系，继续添加任务到阻塞队列中等待执行，因为LinkedBlockingQueue可以近乎认为是一个无穷大的队列，可以无限存放任务；如果你使用的是有界队列比方说ArrayBlockingQueue的话，任务首先会被添加到ArrayBlockingQueue中，ArrayBlockingQueue满了，则会使用拒绝策略RejectedExecutionHandler处理满了的任务，默认是AbortPolicy。

## 为什么要使用线程池

避免频繁地创建和销毁线程，达到线程对象的重用。另外，使用线程池还可以根据项目灵活地控制并发的数目。

## java中用到的线程调度算法是什么

抢占式。一个线程用完CPU之后，操作系统会根据线程优先级、线程饥饿情况等数据算出一个总的优先级并分配下一个时间片给某个线程执行。

## Thread.sleep(0)的作用是什么

由于Java采用抢占式的线程调度算法，因此可能会出现某条线程常常获取到CPU控制权的情况，为了让某些优先级比较低的线程也能获取到CPU控制权，可以使用Thread.sleep(0)手动触发一次操作系统分配时间片的操作，这也是平衡CPU控制权的一种操作。

## 什么是CAS

CAS，全称为Compare and  Swap，即比较-替换。假设有三个操作数：内存值V、旧的预期值A、要修改的值B，当且仅当预期值A和内存值V相同时，才会将内存值修改为B并返回true，否则什么都不做并返回false。当然CAS一定要volatile变量配合，这样才能保证每次拿到的变量是主内存中最新的那个值，否则旧的预期值A对某条线程来说，永远是一个不会变的值A，只要某次CAS操作失败，永远都不可能成功。

## 什么是乐观锁和悲观锁

乐观锁：乐观锁认为竞争不总是会发生，因此它不需要持有锁，将比较-替换这两个动作作为一个原子操作尝试去修改内存中的变量，如果失败则表示发生冲突，那么就应该有相应的重试逻辑。

悲观锁：悲观锁认为竞争总是会发生，因此每次对某资源进行操作时，都会持有一个独占的锁，就像synchronized，不管三七二十一，直接上了锁就操作资源了。

## ConcurrentHashMap的并发度是什么?

ConcurrentHashMap的并发度就是segment的大小，默认为16，这意味着最多同时可以有16条线程操作ConcurrentHashMap，这也是ConcurrentHashMap对Hashtable的最大优势，任何情况下，Hashtable能同时有两条线程获取Hashtable中的数据吗？

## ConcurrentHashMap的工作原理

ConcurrentHashMap在jdk 1.6和jdk 1.8实现原理是不同的。

### jdk 1.6:

ConcurrentHashMap是线程安全的，但是与Hashtablea相比，实现线程安全的方式不同。Hashtable是通过对hash表结构进行锁定，是阻塞式的，当一个线程占有这个锁时，其他线程必须阻塞等待其释放锁。ConcurrentHashMap是采用分离锁的方式，它并没有对整个hash表进行锁定，而是局部锁定，也就是说当一个线程占有这个局部锁时，不影响其他线程对hash表其他地方的访问。 
具体实现:ConcurrentHashMap内部有一个Segment.

### jdk 1.8

在jdk 8中，ConcurrentHashMap不再使用Segment分离锁，而是采用一种乐观锁CAS算法来实现同步问题，但其底层还是“数组+链表->红黑树”的实现。

## CyclicBarrier和CountDownLatch区别

这两个类非常类似，都在java.util.concurrent下，都可以用来表示代码运行到某个点上，二者的区别在于：

- CyclicBarrier的某个线程运行到某个点上之后，该线程即停止运行，直到所有的线程都到达了这个点，所有线程才重新运行；CountDownLatch则不是，某线程运行到某个点上之后，只是给某个数值-1而已，该线程继续运行。
- CyclicBarrier只能唤起一个任务，CountDownLatch可以唤起多个任务
- CyclicBarrier可重用，CountDownLatch不可重用，计数值为0该CountDownLatch就不可再用了。

## java中的++操作符线程安全么?

不是线程安全的操作。它涉及到多个指令，如读取变量值，增加，然后存储回内存，这个过程可能会出现多个线程交差。

## 你有哪些多线程开发良好的实践?

1. 给线程命名
2. 最小化同步范围
3. 优先使用volatile
4. 尽可能使用更高层次的并发工具而非wait和notify()来实现线程通信,如BlockingQueue,Semeaphore
5. 优先使用并发容器而非同步容器.
6. 考虑使用线程池

------

# 关于volatile关键字

## 可以创建Volatile数组吗?

Java 中可以创建 volatile类型数组，不过只是一个指向数组的引用，而不是整个数组。如果改变引用指向的数组，将会受到volatile 的保护，但是如果多个线程同时改变数组的元素，volatile标示符就不能起到之前的保护作用了。

## volatile能使得一个非原子操作变成原子操作吗?

一个典型的例子是在类中有一个 long 类型的成员变量。如果你知道该成员变量会被多个线程访问，如计数器、价格等，你最好是将其设置为  volatile。为什么？因为 Java 中读取 long 类型变量不是原子的，需要分成两步，如果一个线程正在修改该 long  变量的值，另一个线程可能只能看到该值的一半（前 32 位）。但是对一个 volatile 型的 long 或 double 变量的读写是原子。

一种实践是用 volatile 修饰 long 和 double 变量，使其能按原子类型来读写。double 和 long  都是64位宽，因此对这两种类型的读是分为两部分的，第一次读取第一个 32 位，然后再读剩下的 32 位，这个过程不是原子的，但 Java 中  volatile 型的 long 或 double 变量的读写是原子的。volatile 修复符的另一个作用是提供内存屏障（memory  barrier），例如在分布式框架中的应用。简单的说，就是当你写一个 volatile 变量之前，Java 内存模型会插入一个写屏障（write  barrier），读一个 volatile 变量之前，会插入一个读屏障（read barrier）。意思就是说，在你写一个 volatile  域时，能保证任何线程都能看到你写的值，同时，在写之前，也能保证任何数值的更新对所有线程是可见的，因为内存屏障会将其他所有写的值更新到缓存。

## volatile类型变量提供什么保证?

volatile 主要有两方面的作用:1.避免指令重排2.可见性保证.例如，JVM 或者 JIT为了获得更好的性能会对语句重排序，但是  volatile 类型变量即使在没有同步块的情况下赋值也不会与其他语句重排序。 volatile 提供 happens-before  的保证，确保一个线程的修改能对其他线程是可见的。某些情况下，volatile 还能提供原子性，如读 64 位数据类型，像 long 和  double 都不是原子的(低32位和高32位)，但 volatile 类型的 double 和 long 就是原子的。

------

# 关于集合

## Java中的集合及其继承关系

关于集合的体系是每个人都应该烂熟于心的,尤其是对我们经常使用的List,Map的原理更该如此.这里我们看这张图即可: 

更多内容可见集合类总结：

http://write.blog.csdn.net/postedit/40826423

## poll()方法和remove()方法区别？

poll() 和 remove() 都是从队列中取出一个元素，但是 poll() 在获取元素失败的时候会返回空，但是 remove() 失败的时候会抛出异常。

## LinkedHashMap和PriorityQueue的区别

PriorityQueue 是一个优先级队列,保证最高或者最低优先级的的元素总是在队列头部，但是 LinkedHashMap  维持的顺序是元素插入的顺序。当遍历一个 PriorityQueue 时，没有任何顺序保证，但是 LinkedHashMap  课保证遍历顺序是元素插入的顺序。

## WeakHashMap与HashMap的区别是什么?

WeakHashMap 的工作与正常的 HashMap 类似，但是使用弱引用作为 key，意思就是当 key 对象没有任何引用时，key/value 将会被回收。

## ArrayList和LinkedList的区别?

最明显的区别是 ArrrayList底层的数据结构是数组，支持随机访问，而 LinkedList 的底层数据结构是双向循环链表，不支持随机访问。使用下标访问一个元素，ArrayList 的时间复杂度是 O(1)，而 LinkedList 是 O(n)。

## ArrayList和Array有什么区别?

1. Array可以容纳基本类型和对象，而ArrayList只能容纳对象。
2. Array是指定大小的，而ArrayList大小是固定的

## ArrayList和HashMap默认大小?

在 Java 7 中，ArrayList 的默认大小是 10 个元素，HashMap 的默认大小是16个元素（必须是2的幂）。这就是 Java 7 中 ArrayList 和 HashMap 类的代码片段。

```
private static final int DEFAULT_CAPACITY = 10;
//from HashMap.java JDK 7*
static final int DEFAULT_INITIAL_CAPACITY = 1 << 4; // aka 16
```

   


##  

## Comparator和Comparable的区别?

Comparable 接口用于定义对象的自然顺序，而 comparator 通常用于定义用户定制的顺序。Comparable 总是只有一个，但是可以有多个 comparator 来定义对象的顺序。

## 如何实现集合排序?

你可以使用有序集合，如 TreeSet 或 TreeMap，你也可以使用有顺序的的集合，如 list，然后通过 Collections.sort() 来排序。

## 如何打印数组内容

你可以使用 Arrays.toString() 和 Arrays.deepToString() 方法来打印数组。由于数组没有实现  toString() 方法，所以如果将数组传递给 System.out.println() 方法，将无法打印出数组的内容，但是  Arrays.toString() 可以打印每个元素。

## LinkedList的是单向链表还是双向?

双向循环列表，具体实现自行查阅源码。

## TreeMap是实现原理

采用红黑树实现，具体实现自行查阅源码。

## 遍历ArrayList时如何正确移除一个元素

该问题的关键在于面试者使用的是 ArrayList 的 remove() 还是 Iterator 的  remove()方法。这有一段示例代码，是使用正确的方式来实现在遍历的过程中移除元素，而不会出现  ConcurrentModificationException 异常的示例代码。

## 什么是ArrayMap?它和HashMap有什么区别?

ArrayMap是Android SDK中提供的，非Android开发者可以略过。
ArrayMap是用两个数组来模拟map，更少的内存占用空间,更高的效率。
具体参考这篇文章：ArrayMap VS HashMap：http://lvable.com/?p=217%5D

## HashMap的实现原理

\1. HashMap概述： HashMap是基于哈希表的Map接口的非同步实现。此实现提供所有可选的映射操作，并允许使用null值和null键。此类不保证映射的顺序，特别是它不保证该顺序恒久不变。 
\2.  HashMap的数据结构：  在java编程语言中，最基本的结构就是两种，一个是数组，另外一个是模拟指针（引用），所有的数据结构都可以用这两个基本结构来构造的，HashMap也不例外。HashMap实际上是一个“链表散列”的数据结构，即数组和链表的结合体。

当我们往Hashmap中put元素时,首先根据key的hashcode重新计算hash值,根绝hash值得到这个元素在数组中的位置(下标),如果该数组在该位置上已经存放了其他元素,那么在这个位置上的元素将以链表的形式存放,新加入的放在链头,最先加入的放入链尾.如果数组中该位置没有元素,就直接将该元素放到数组的该位置上.

需要注意Jdk 1.8中对HashMap的实现做了优化,当链表中的节点数据超过八个之后,该链表会转为红黑树来提高查询效率,从原来的O(n)到O(logn)

## 你了解Fail-Fast机制吗？

Fail-Fast即我们常说的快速失败，

更多内容参看fail-fast机制：http://blog.csdn.net/chenssy/article/details/38151189

## Fail-fast和Fail-safe有什么区别

Iterator的fail-fast属性与当前的集合共同起作用，因此它不会受到集合中任何改动的影响。Java.util包中的所有集合类都被设计为fail->fast的，而java.util.concurrent中的集合类都为fail-safe的。当检测到正在遍历的集合的结构被改变时，Fail-fast迭代器抛出ConcurrentModificationException，而fail-safe迭代器从不抛出ConcurrentModificationException。

------

# 关于日期

## SimpleDateFormat是线程安全的吗?

非常不幸，DateFormat 的所有实现，包括 SimpleDateFormat  都不是线程安全的，因此你不应该在多线程序中使用，除非是在对外线程安全的环境中使用，如 将 SimpleDateFormat 限制在  ThreadLocal  中。如果你不这么做，在解析或者格式化日期的时候，可能会获取到一个不正确的结果。因此，从日期、时间处理的所有实践来说，我强力推荐  joda-time 库。

## 如何格式化日期?

Java 中，可以使用 SimpleDateFormat 类或者 joda-time 库来格式日期。DateFormat  类允许你使用多种流行的格式来格式化日期。参见答案中的示例代码，代码中演示了将日期格式化成不同的格式，如 dd-MM-yyyy 或  ddMMyyyy。

------

# 关于异常

## 简单描述java异常体系

相比没有人不了解异常体系,关于异常体系的更多信息可以见

白话异常机制：http://blog.csdn.net/dd864140130/article/details/42504189

## 什么是异常链

详情直接参见上面的白话异常机制，不做解释了。

## throw和throws的区别

throw用于主动抛出java.lang.Throwable 类的一个实例化对象，意思是说你可以通过关键字 throw 抛出一个 Error 或者 一个Exception，如：`throw new IllegalArgumentException(“size must be multiple of 2″)`, 
而throws 的作用是作为方法声明和签名的一部分，方法被抛出相应的异常以便调用者能处理。Java 中，任何未处理的受检查异常强制在 throws 子句中声明。

------

## 关于序列化

## Java 中，Serializable 与 Externalizable 的区别

Serializable 接口是一个序列化 Java 类的接口，以便于它们可以在网络上传输或者可以将它们的状态保存在磁盘上，是 JVM  内嵌的默认序列化方式，成本高、脆弱而且不安全。Externalizable 允许你控制整个序列化过程，指定特定的二进制格式，增加安全机制。

------

# 关于JVM

## JVM特性

平台无关性. 
Java语言的一个非常重要的特点就是与平台的无关性。而使用Java虚拟机是实现这一特点的关键。一般的高级语言如果要在不同的平台上运行，至少需要编译成不同的目标代码。而引入Java语言虚拟机后，Java语言在不同平台上运行时不需要重新编译。Java语言使用模式Java虚拟机屏蔽了与具体平台相关的信息，使得Java语言编译程序只需生成在Java虚拟机上运行的目标代码（字节码），就可以在多种平台上不加修改地运行。Java虚拟机在执行字节码时，把字节码解释成具体平台上的机器指令执行。

## 简单解释一下类加载器

有关类加载器一般会问你四种类加载器的应用场景以及双亲委派模型,

更多的内容参看深入理解JVM加载器：

http://blog.csdn.net/dd864140130/article/details/49817357

## 简述堆和栈的区别

VM 中堆和栈属于不同的内存区域，使用目的也不同。栈常用于保存方法帧和局部变量，而对象总是在堆上分配。栈通常都比堆小，也不会在多个线程之间共享，而堆被整个 JVM 的所有线程共享。

## 简述JVM内存分配

1. 基本数据类型比变量和对象的引用都是在栈分配的。
2. 堆内存用来存放由new创建的对象和数组。
3. 类变量（static修饰的变量），程序在一加载的时候就在堆中为类变量分配内存，堆中的内存地址存放在栈中。
4. 实例变量：当你使用java关键字new的时候，系统在堆中开辟并不一定是连续的空间分配给变量，是根据零散的堆内存地址，通过哈希算法换算为一长串数字以表征这个变量在堆中的”物理位置”,实例变量的生命周期–当实例变量的引用丢失后，将被GC（垃圾回收器）列入可回收“名单”中，但并不是马上就释放堆中内存。
5. 局部变量: 由声明在某方法，或某代码段里（比如for循环），执行到它的时候在栈中开辟内存，当局部变量一但脱离作用域，内存立即释放。

------

# 其他

## java当中采用的是大端还是小端?

## XML解析的几种方式和特点

DOM, SAX, PULL三种解析方式：

- DOM:消耗内存：先把xml文档都读到内存中，然后再用DOM API来访问树形结构，并获取数据。这个写起来很简单，但是很消耗内存。要是数据过大，手机不够牛逼，可能手机直接死机
- SAX:解析效率高，占用内存少，基于事件驱动的：更加简单地说就是对文档进行顺序扫描，当扫描到文档(document)开始与结束、元素(element)开始与结束、文档(document)结束等地方时通知事件处理函数，由事件处理函数做相应动作，然后继续同样的扫描，直至文档结束。
- PULL:与 SAX  类似，也是基于事件驱动，我们可以调用它的next（）方法，来获取下一个解析事件（就是开始文档，结束文档，开始标签，结束标签），当处于某个元素时可以调用XmlPullParser的getAttributte()方法来获取属性的值，也可调用它的nextText()获取本节点的值。

## JDK 1.7特性

然 JDK 1.7 不像 JDK 5 和 8 一样的大版本，但是，还是有很多新的特性，如 try-with-resource  语句，这样你在使用流或者资源的时候，就不需要手动关闭，Java 会自动关闭。Fork-Join 池某种程度上实现 Java 版的  Map-reduce。允许 Switch 中有 String  变量和文本。菱形操作符(<>)用于类型推断，不再需要在变量声明的右边申明泛型，因此可以写出可读写更强、更简洁的代码。

## JDK 1.8特性

java 8 在 Java 历史上是一个开创新的版本，下面 JDK 8 中 5 个主要的特性： 
Lambda 表达式，允许像对象一样传递匿名函数 
Stream API，充分利用现代多核 CPU，可以写出很简洁的代码 
Date 与 Time API，最终，有一个稳定、简单的日期和时间库可供你使用 
扩展方法，现在，接口中可以有静态、默认方法。 
重复注解，现在你可以将相同的注解在同一类型上使用多次。

## Maven和ANT有什么区别?

虽然两者都是构建工具，都用于创建 Java 应用，但是 Maven 做的事情更多，在基于“约定优于配置”的概念下，提供标准的Java 项目结构，同时能为应用自动管理依赖（应用中所依赖的 JAR 文件。

## JDBC最佳实践

- 优先使用批量操作来插入和更新数据
- 使用PreparedStatement来避免SQL漏洞
- 使用数据连接池
- 通过列名来获取结果集

## IO操作最佳实践

- 使用有缓冲的IO类,不要单独读取字节或字符
- 使用NIO和NIO 2或者AIO,而非BIO
- 在finally中关闭流
- 使用内存映射文件获取更快的IO

 

# 部分四：

https://blog.csdn.net/u014042066/article/details/77584668