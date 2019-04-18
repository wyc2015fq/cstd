# Java基础：多线程 - weixin_33985507的博客 - CSDN博客
2018年03月01日 11:31:54[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
## 1.创建线程的方式
1.继承Thread，重写run方法。
```java
public class MyThread extends Thread {
    @Override
    public void run() {
        int i = 0;
        while(i < 100){
            i++;
            System.out.println("thread: i = "+i);
        }
    }
}
class Demo{
    public static void main(String[] args) {
        MyThread myThread = new MyThread();
        myThread.start();
        for (int j = 0; j < 100; j++) {
            System.out.println("main: j = "+j);
        }        
    }
}
```
2.实现Runnable接口
```java
public class CountTask implements Runnable {
    @Override
    public void run() {
        int i = 0;
        while(i < 100){
            i++;
            System.out.println("thread: i = "+i);
        }
    }
}
class Demo22{
    public static void main(String[] args) {
        Thread thread = new Thread(new CountTask());
        thread.start();
    }
}
```
## 2.启动线程的方式
调用Thread类型对象的`start()`方法。注意如果调用`run()`方法的话，只是简单的方法调用，而不是开启一条新的线程。
## 3.线程的状态
在Thread类的源码中可以看到定义了以下的线程的状态
|状态|含义|详情|
|----|----|----|
|NEW|新建|线程被创建了，但未开始运行|
|RUNNABLE|可运行|调用start()后会变成该状态。该状态的线程位于可运行线程池中，变得可运行，等待获取cpu的执行权|
|BLOCKED|阻塞|表明当前线程（A）正在等待获取一个`锁`（L），获取锁后才能进入或再次进入`同步的`代码块或方法（即临界区）。当另一条持有这个锁的线程（B），调用了锁对象（L）的wait()方法后，当前线程（A线程）才有机会获取到锁（L）|
|WAITING|等待|当前线程（A）已获得`锁`了，已在临界区中。当调用了`Object.wait()、Thread.join()、LockSupport.park()`等方法（都不包含超时参数）后就会进入到等待状态。    `等待`的含义是当前线程（A）正在等待其它线程（B）执行对应的操作，当前线程才能回到RUNNABLE状态。`比如：`A线程调用了Object.wait()，那么A需要等待B线程调用Object.notify()或Object.notifyAll()，A才能回到RUNNABLE状态；A线程调用了B线程的join()，表明A需要等待B执行完，才能回到RUNNABLE状态，才有机会获得cpu执行权继续运行代码。|
|TIMED_WAITING|等待超时|当前线程（A）已获得`锁`了，已在临界区中。当调用了以下方法，就会进入等待超时的状态（含超时参数）：`Thread.sleep()、Object.wait()、Thread.join()、LockSupport.parkNanos、LockSupport.parkUntil`。请注意`wait和sleep的区别：`wait是Object上的方法，sleep是Thread上的方法。wait会释放锁，但sleep不会释放锁。|
|TERMINATED|终结|Thread.run()方法运行完，就进入该状态|
