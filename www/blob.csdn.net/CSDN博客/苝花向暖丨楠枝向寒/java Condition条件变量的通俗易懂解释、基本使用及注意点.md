# java Condition条件变量的通俗易懂解释、基本使用及注意点 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年03月26日 22:29:59[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：103


# 注 ： 转自https://www.cnblogs.com/zhjh256/p/6389168.html

# [java Condition条件变量的通俗易懂解释、基本使用及注意点](http://www.cnblogs.com/zhjh256/p/6389168.html)

最近在看pthread方面的书，看到条件变量一节的时候，回忆了下java中条件变量的使用方式。

java中条件变量都实现了java.util.concurrent.locks.Condition接口，条件变量的实例化是通过一个Lock对象上调用newCondition()方法来获取的，这样，条件就和一个锁对象绑定起来了。因此，Java中的条件变量只能和锁配合使用，来控制并发程序访问竞争资源的安全。

条件变量的出现是为了更精细控制线程等待与唤醒，在Java5之前，线程的等待与唤醒依靠的是Object对象的wait()和notify()/notifyAll()方法，这样的处理不够精细。

通熟易懂的说，就是消费者/生产者的场景中，在原来的基础上，增加了队列满时及时通知消费者，队列空时及时通知生产者的优化，通常是两个条件变量一起出现，一个控制值，但两个条件变量可以毫无关系，终归来说还是在Lock的范围内。**所以，从本质上来说，是对Object监视器的场景性优化，而不是全新机制的引入。**

从现实应用角度来说，它们常被用于下列场景：

1、写log。比如每1秒或者commit或者日志大于1/3m时候都写入。缓存中大于1/3m时需要等写入完成才能commit。

而在Java5中，一个锁可以有多个条件，每个条件上可以有多个线程等待，通过调用await()方法，可以让线程在该条件下等待。当调用signalAll()方法，又可以唤醒该条件下的等待的线程。有关Condition接口详细说明可以具体参考JavaAPI文档。

如下：publicclass TestConditon

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
*public static void main(String[] args) {        // 创建并发访问的账户        MyCount myCount = new MyCount("95599200901215522", 10000);        // 创建一个线程池        ExecutorService pool = Executors.newFixedThreadPool(3); #假设改成2会怎么样??        Thread t1 = new SaveThread("张三", myCount, 1000);        Thread t2 = new SaveThread("李四", myCount, 1000);        Thread t3 = new DrawThread("王五", myCount, 12600);        Thread t4 = new SaveThread("老张", myCount, 600);        Thread t5 = new DrawThread("老牛", myCount, 1300);        Thread t6 = new DrawThread("胖子", myCount, 800);        Thread t7 = new SaveThread("测试", myCount, 2100);        // 执行各个线程        pool.execute(t1);        pool.execute(t2);        pool.execute(t3);        pool.execute(t4);        pool.execute(t5);        pool.execute(t6);        pool.execute(t7);        // 关闭线程池        pool.shutdown();    }}/** * 存款线程类 */class SaveThread extends Thread {    private String name; // 操作人    private MyCount myCount; // 账户    private int x; // 存款金额    SaveThread(String name, MyCount myCount, int x) {        this.name = name;        this.myCount = myCount;        this.x = x;    }    public void run() {        myCount.saving(x, name);    }}/** * 取款线程类 */class DrawThread extends Thread {    private String name; // 操作人    private MyCount myCount; // 账户    private int x; // 存款金额    DrawThread(String name, MyCount myCount, int x) {        this.name = name;        this.myCount = myCount;        this.x = x;    }    public void run() {        myCount.drawing(x, name);    }}/** * 普通银行账户，不可透支 */class MyCount {    private String oid; // 账号    private int cash; // 账户余额    private Lock lock = new ReentrantLock(); // 账户锁    private Condition _save = lock.newCondition(); // 存款条件    private Condition _draw = lock.newCondition(); // 取款条件    MyCount(String oid, int cash) {        this.oid = oid;        this.cash = cash;    }    /**     * 存款     *      * @param x     *            操作金额     * @param name     *            操作人     */    public void saving(int x, String name) {        lock.lock(); // 获取锁        if (x > 0) {            cash += x; // 存款            System.out.println(name + "存款" + x + "，当前余额为" + cash);        }        _draw.signalAll(); // 唤醒所有等待线程。        lock.unlock(); // 释放锁    }    /**     * 取款     *      * @param x     *            操作金额     * @param name     *            操作人     */    public void drawing(int x, String name) {        lock.lock(); // 获取锁        try {            while (cash - x < 0) {                 _draw.await(); // 阻塞取款操作, await之后就隐示自动释放了lock，直到被唤醒自动获取                System.out.println(name + "阻塞中");            }            {                cash -= x; // 取款                System.out.println(name + "取款" + x + "，当前余额为" + cash);            }            _save.signalAll(); // 唤醒所有存款操作        } catch (InterruptedException e) {            e.printStackTrace();        } finally {            lock.unlock(); // 释放锁        }    }}*
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

需要注意的是，在共用一个线程池的设计中，特别要注意饿死现象（就像上下高速如果公用车道的话，万一进入的10车全部占坑了，高速里面又满了的话，想出的都出不来，进的进不去，就出现饿死现象了），如果有大量的消费者使得生产者线程无法再运行的话，就会出现该问题，在上述例子中，将线程池数量从3改成2就可以多次测试中发现程序hang了。

所以，我们可以看到典型的在RDBMS系统中都是各种线程各司其职。

