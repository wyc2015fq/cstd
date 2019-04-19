# Java CountDownLatch 和 CyclicBarrier - aisoo的专栏 - CSDN博客
2012年12月12日 17:55:15[aisoo](https://me.csdn.net/aisoo)阅读数：1692
在多线程程序设计中，经常会碰到一个线程等待一个或多个线程的场景，碰到这样的场景应该如何解决？
假如是一个线程等待一个线程，则可以通过await()和notify()来实现；
假如是一个线程等待多个线程，则就可以使用CountDownLatch和CyclicBarrier来实现比较好的控制。
Java的concurrent包里面的**CountDownLatch**其实可以把它看作一个计数器，只不过这个计数器的操作是原子操作，同时只能有一个线程去操作这个计数器，也就是同时只能有一个线程去减这个计数器里面的值。
      你可以向CountDownLatch对象设置一个初始的数字作为计数值，任何调用这个对象上的await()方法都会阻塞，直到这个计数器的计数值被其他的线程减为0为止。
      CountDownLatch的一个非常典型的应用场景是：有一个任务想要往下执行，但必须要等到其他的任务执行完毕后才可以继续往下执行。假如我们这个想要继续往下执行的任务调用一个CountDownLatch对象的await()方法，其他的任务执行完自己的任务后调用同一个CountDownLatch对象上的countDown()方法，这个调用await()方法的任务将一直阻塞等待，直到这个CountDownLatch对象的计数值减到0为止。
**CyclicBarrier**类有两个常用的构造方法：
1. CyclicBarrier(int parties)
      这里的parties也是一个计数器，例如，初始化时parties里的计数是3，于是拥有该CyclicBarrier对象的线程当parties的计数为3时就唤醒，注：这里parties里的计数在运行时当调用CyclicBarrier:await()时,计数就加1，一直加到初始的值
2. CyclicBarrier(int parties, Runnable barrierAction)
     这里的parties与上一个构造方法的解释是一样的，这里需要解释的是第二个进参（Runnable barrierAction）,这个参数是一个实现Runnable接口的类的对象，也就是说当parties加到初始值时就触发barrierAction的内容。
