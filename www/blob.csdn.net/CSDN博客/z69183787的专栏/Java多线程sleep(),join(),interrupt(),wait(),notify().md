# Java多线程sleep(),join(),interrupt(),wait(),notify() - z69183787的专栏 - CSDN博客
2014年05月05日 20:24:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：992
关于Java多线程知识可以看看《Thinking in Java 》中的多线程部分和《Java网络编程》中第5章多线程的部分
以下是参考<<Java多线程模式>>的 
1. sleep() & interrupt() 
    线程A正在使用sleep()暂停着: Thread.sleep(100000); 
    如果要取消他的等待状态,可以在正在执行的线程里(比如这里是B)调用 
        a.interrupt(); 
    令线程A放弃睡眠操作,这里a是线程A对应到的Thread实例 
    执行interrupt()时,并不需要获取Thread实例的锁定.任何线程在任何时刻,都可以调用其他线程interrupt().当sleep中的线程被调用interrupt()时,就会放弃暂停的状态.并抛出InterruptedException.丢出异常的,是A线程. 
2. wait() & interrupt() 
    线程A调用了wait()进入了等待状态,也可以用interrupt()取消. 
    不过这时候要小心锁定的问题.线程在进入等待区,会把锁定解除,当对等待中的线程调用interrupt()时(注意是等待的线程调用其自己的interrupt()),会先重新获取锁定,再抛出异常.在获取锁定之前,是无法抛出异常的. 
3. join() & interrupt() 
    当线程以join()等待其他线程结束时,一样可以使用interrupt()取消之.因为调用join()不需要获取锁定,故与sleep()时一样,会马上跳到catch块里. 注意是随调用interrupt()方法,一定是阻塞的线程来调用其自己的interrupt方法.如在线程a中调用来线程t.join().则a会等t执行完后在执行t.join后的代码,当在线程b中调用来a.interrupt()方法,则会抛出InterruptedException
4. interrupt()只是改变中断状态而已 
    interrupt()不会中断一个正在运行的线程。这一方法实际上完成的是，在线程受到阻塞时抛出一个中断信号，这样线程就得以退出阻塞的状态。更确切的说，如果线程被Object.wait, Thread.join和Thread.sleep三种方法之一阻塞，那么，它将接收到一个中断异常（InterruptedException），从而提早地终结被阻塞状态。 
    如果线程没有被阻塞，这时调用interrupt()将不起作用；否则，线程就将得到异常（该线程必须事先预备好处理此状况），接着逃离阻塞状态。 
    线程A在执行sleep,wait,join时,线程B调用A的interrupt方法,的确这一个时候A会有InterruptedException异常抛出来.但这其实是在sleep,wait,join这些方法内部会不断检查中断状态的值,而自己抛出的InterruptedException。 
    如果线程A正在执行一些指定的操作时如赋值,for,while,if,调用方法等,都不会去检查中断状态,所以线程A不会抛出InterruptedException,而会一直执行着自己的操作.当线程A终于执行到wait(),sleep(),join()时,才马上会抛出InterruptedException. 
    若没有调用sleep(),wait(),join()这些方法,或是没有在线程里自己检查中断状态自己抛出InterruptedException的话,那InterruptedException是不会被抛出来的.
顺便加个与Thread.sleep()相同效果的代码: 
public static void amethod(long x) throws InterruptedExcetion{ 
    if (x != 0) { 
        Object o = new Object(); 
        synchronized (o) { 
            o.wait(x); 
        } 
    } 
} 
