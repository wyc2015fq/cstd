# Java 中的线程管理概念梳理 - 我相信...... - CSDN博客





2015年03月09日 07:21:50[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1856










在Java中，“线程”指java.lang.Thread类的一个实例以及线程的执行，主要使用的线程池是ThreadPoolExecutor以及ScheduledThreadPoolExecutor，要使用固定线程上限的线程池。

用synchronized 修饰静态方法时，表示任何两个不同线程的调用互斥；修饰成员函数时，表示同一对象的多线程方法调用互斥；当然了，synchronized 后的参数可以是任意对象。Synchronized保证了synchronized块中变量的可见性，而volatile保证了所修饰变量的可见性。ReentrantLock是java.util.concurrent.locks中的一个类，与synchronized用法类似，但需要显示的解锁，它提供了ReentrantReadWriteLock，主要用于读多写少且读不需要互斥的场景。

Java.util.concurrent.atomic包中以Atomic开头的类提供了一些相关的原子操作，性能提升的原因是内部通过JNI的方式使用了硬件支持的CAS指令。 




Object对象上的三个必备方法：wait、notify、notifyall。Nofity唤醒一个等待线程，notifyall会唤醒所有的等待线程。Wait的使用一般嵌在一个循环中，判断相关的数据状态是否达到预期，如果没有则继续等待，主要是为了防止虚假唤醒。



Java.util.concurrent 中CountDownLatch提供的机制是当多个线程都到达了预期状态或者完成预期工作时触发事件，其他线程可以等待这个事件来触发自己后续的工作。CyclicBarrier可以协同多个线程，让多个线程在mybarrier.wait()前等待，直到所有线程都到达了这个屏障时，再一起执行后面的动作。



CountDownLatch 与CyclicBarrier都有用于线程协调的，主要差别有：CountDownLatch在多个线程都执行了latch.countDown后才触发事件，唤醒wait在latch上的线程，而执行countDown的线程在执行完countDown后会继续自己线程的工作；CyclicBarrier是个栅栏，用于同步所有调用wait方法的线程，并且等所有线程都到了wait的方法时，这些线程才一起返回继续各自的工作。另外，CountDownLatch不能循环使用，而CyclicBarrier可以循环使用。




Semaphore用于管理信号量，信号量对象构造时传入信号个数就是控制并发的数量。执行前通过acquire获取信号，执行后通过release归还，如果没有可用的信号，aquire调用就会阻塞。与控制线程数来控制并发数相比，semaphore控制并发数的控制粒度更细。



Exchanger用于在两个线程之间进行数据交换。线程会阻塞在Exchanger的exchange方法，直到另外一个线程也到了同一个Exchanger的exchange方法时，二者进行交换，然后两个线程会继续执行自身相关的代码。



异步调用的实现方式常用的有：future 和回调函数。调用的方法会返回一个future对象，然后接着进行自己的处理，后面通过future.get()来获取真正的返回值。 FutureTask是future的一个具体实现类。ThreadPoolExecutor的submit方法返回的就是一个FutureTask的具体实现。FutureTask帮助实现了具体的任务执行以及与future接口中的get等方法的关联。FutureTask帮助ThreadPoolExecutor实现了对加入线程池任务的future支持，也使我们能够实现支持future的任务调度。



加锁互斥能够方便地实现线程安全，但代价是降低了性能，而并发容器不仅追求线程安全，而且考虑并发，提升容器在并发环境下的性能。比较有代表性的是以CopyOnWrite和Concurrent开头的几个容器。



