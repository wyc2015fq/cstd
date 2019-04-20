# 锁机制-java面试 - Spark高级玩法 - CSDN博客
2018年04月08日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：558
何为同步？JVM规范规定JVM基于进入和退出Monitor对象来实现方法同步和代码块同步，但两者的实现细节不一样。代码块同步是使用monitorenter和monitorexit指令实现，而方法同步是使用另外一种方式实现的，细节在JVM规范里并没有详细说明，但是方法的同步同样可以使用这两个指令来实现。monitorenter指令是在编译后插入到同步代码块的开始位置，而monitorexit是插入到方法结束处和异常处， JVM要保证每个monitorenter必须有对应的monitorexit与之配对。任何对象都有一个 monitor 与之关联，当且一个monitor 被持有后，它将处于锁定状态。线程执行到 monitorenter 指令时，将会尝试获取对象所对应的 monitor 的所有权，即尝试获得对象的锁。
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXJHvlTLzfsKKtAxeKO7cKkXe5bfficOopfYMbgcYRuU54wR4gicccy9xw0IXRD11jDdiaoenhsty7Zg/640?wx_fmt=png&wxfrom=5&wx_lazy=1)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXJHvlTLzfsKKtAxeKO7cKk7aVDu8rPJLAyzRxVKzN4ubWDu6L51A2TDzcrmWSwlJfZsmFDLDtCCQ/640?wx_fmt=png)
## Java对象头
锁存在Java对象头里。如果对象是数组类型，则虚拟机用3个Word（字宽）存储对象头，如果对象是非数组类型，则用2字宽存储对象头。在32位虚拟机中，一字宽等于四字节，即32bit。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXJHvlTLzfsKKtAxeKO7cKk0ogoABvtZpNCnVBC41SwNHguh5CicLkNGicpld4eRhftrBKvzQL4SqTg/640?wx_fmt=png)
Java对象头里的Mark Word里默认存储对象的HashCode，分代年龄和锁标记位。32位JVM的Mark Word的默认存储结构如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXJHvlTLzfsKKtAxeKO7cKks6dIlZialXLpTPYxKbf1RicEu62UfTmoFPxlQzcVezbXyjTmxfHMbfBw/640?wx_fmt=png)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFXJHvlTLzfsKKtAxeKO7cKkh2wicloHwKrc08PyicAs7RqqjJbXAyUjkLUxficLNWYkTdiaf5ibFM7dVYA/640?wx_fmt=jpeg)在运行期间Mark Word里存储的数据会随着锁标志位的变化而变化。Mark Word可能变化为存储以下4种数据：
## **几种锁的类型**
**线程的阻塞和唤醒需要CPU从用户态转为核心态，频繁的阻塞和唤醒对CPU来说是一件负担很重的工作。**
Java SE1.6为了减少获得锁和释放锁所带来的性能消耗，引入了“偏向锁”和“轻量级锁”，所以在Java SE1.6里锁一共有四种状态，无锁状态，偏向锁状态，轻量级锁状态和重量级锁状态，它会随着竞争情况逐渐升级。**锁可以升级但不能降级，意味着偏向锁升级成轻量级锁后不能降级成偏向锁。这种锁升级却不能降级的策略，目的是为了提高获得锁和释放锁的效率。**
### **偏向锁**
大多数情况下锁不仅不存在多线程竞争，而且总是由同一线程多次获得。偏向锁的目的是在某个线程获得锁之后，消除这个线程锁重入（CAS）的开销，看起来让这个线程得到了偏护。另外，JVM对那种会有多线程加锁，但不存在锁竞争的情况也做了优化，听起来比较拗口，但在现实应用中确实是可能出现这种情况，因为线程之前除了互斥之外也可能发生同步关系，被同步的两个线程（一前一后）对共享对象锁的竞争很可能是没有冲突的。对这种情况，JVM用一个epoch表示一个偏向锁的时间戳（真实地生成一个时间戳代价还是蛮大的，因此这里应当理解为一种类似时间戳的identifier）
偏向锁的获取
当一个线程访问同步块并获取锁时，会在对象头和栈帧中的锁记录里存储锁偏向的线程ID，以后该线程在进入和退出同步块时不需要花费CAS操作来加锁和解锁，而只需简单的测试一下对象头的Mark Word里是否存储着指向当前线程的偏向锁，如果测试成功，表示线程已经获得了锁，如果测试失败，则需要再测试下Mark Word中偏向锁的标识是否设置成1（表示当前是偏向锁），如果没有设置，则使用CAS竞争锁，如果设置了，则尝试使用CAS将对象头的偏向锁指向当前线程。
偏向锁的撤销
偏向锁使用了一种等到竞争出现才释放锁的机制，所以当其他线程尝试竞争偏向锁时，持有偏向锁的线程才会释放锁。偏向锁的撤销，需要等待全局安全点（在这个时间点上没有字节码正在执行），它会首先暂停拥有偏向锁的线程，然后检查持有偏向锁的线程是否活着，如果线程不处于活动状态，则将对象头设置成无锁状态，如果线程仍然活着，拥有偏向锁的栈会被执行，遍历偏向对象的锁记录，栈中的锁记录和对象头的Mark Word，要么重新偏向于其他线程，要么恢复到无锁或者标记对象不适合作为偏向锁，最后唤醒暂停的线程。
偏向锁的设置
关闭偏向锁：偏向锁在Java 6和Java 7里是默认启用的，但是它在应用程序启动几秒钟之后才激活，如有必要可以使用JVM参数来关闭延迟-XX：BiasedLockingStartupDelay = 0。如果你确定自己应用程序里所有的锁通常情况下处于竞争状态，可以通过JVM参数关闭偏向锁-XX:-UseBiasedLocking=false，那么默认会进入轻量级锁状态。
### **自旋锁**
线程的阻塞和唤醒需要CPU从用户态转为核心态，频繁的阻塞和唤醒对CPU来说是一件负担很重的工作。同时我们可以发现，很多对象锁的锁定状态只会持续很短的一段时间，例如整数的自加操作，在很短的时间内阻塞并唤醒线程显然不值得，为此引入了自旋锁。
所谓“自旋”，就是让线程去执行一个无意义的循环，循环结束后再去重新竞争锁，如果竞争不到继续循环，循环过程中线程会一直处于running状态，但是基于JVM的线程调度，会出让时间片，所以其他线程依旧有申请锁和释放锁的机会。
自旋锁省去了阻塞锁的时间空间（队列的维护等）开销，但是长时间自旋就变成了“忙式等待”，忙式等待显然还不如阻塞锁。所以自旋的次数一般控制在一个范围内，例如10,100等，在超出这个范围后，自旋锁会升级为阻塞锁。
### **轻量级锁**
加锁
线程在执行同步块之前，JVM会先在当前线程的栈桢中创建用于存储锁记录的空间，并将对象头中的Mark Word复制到锁记录中，官方称为Displaced Mark Word。**然后线程尝试使用CAS将对象头中的Mark Word替换为指向锁记录的指针。如果成功，当前线程获得锁，如果失败，则自旋获取锁，当自旋获取锁仍然失败时，表示存在其他线程竞争锁(两条或两条以上的线程竞争同一个锁)，则轻量级锁会膨胀成重量级锁。**
解锁
**轻量级解锁时，会使用原子的CAS操作来将Displaced Mark Word替换回到对象头，如果成功，则表示同步过程已完成。**如果失败，表示有其他线程尝试过获取该锁，则要在释放锁的同时唤醒被挂起的线程。
### **重量级锁**
重量锁在JVM中又叫对象监视器（Monitor），它很像C中的Mutex，除了具备Mutex(0|1)互斥的功能，它还负责实现了Semaphore(信号量)的功能，也就是说它至少包含一个竞争锁的队列，和一个信号阻塞队列（wait队列），前者负责做互斥，后一个用于做线程同步。
## **锁的优缺点对比**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXJHvlTLzfsKKtAxeKO7cKk7QlcvGWvD5jkvT0cdlh85R2dr0kh4LEZDcQ12LwzouzU8IpdKGHd1w/640?wx_fmt=png)
**可重入锁**
本文里面讲的是广义上的可重入锁，而不是单指JAVA下的ReentrantLock。
可重入锁，也叫做递归锁，指的是同一线程 外层函数获得锁之后 ，内层递归函数仍然有获取该锁的代码，但不受影响。
在JAVA环境下 ReentrantLock 和synchronized 都是 可重入锁。
public class Test implements Runnable{
  public synchronized void get(){
    System.out.println(Thread.currentThread().getId());
set();
}
  public synchronized void set(){
    System.out.println(Thread.currentThread().getId());
}
  @Override
  public void run() {
    get();
}
  public static void main(String[] args) {
    Test ss=new Test();
new Thread(ss).start();
new Thread(ss).start();
new Thread(ss).start();
}
}
结果如下，是正确的，即同一个线程id被连续输出两次。
Threadid: 8
Threadid: 8
Threadid: 10
Threadid: 10
Threadid: 9
Threadid: 9
可重入锁最大的作用是避免死锁。
我们以自旋锁作为例子。（注：自旋锁，就是拿不到锁的情况会不停自旋循环检测来等待，不进入内核态沉睡，而是在用户态自旋尝试）
public class SpinLock {
private AtomicReference<Thread> owner =new AtomicReference<>();
public void lock(){
    Thread current = Thread.currentThread();
while(!owner.compareAndSet(null, current)){
    }
  }
  public void unlock (){
    Thread current = Thread.currentThread();
owner.compareAndSet(current, null);
}
}
上面是自旋锁的一种实现。
对于自旋锁来说：
1、若有同一线程两调用lock() ，会导致第二次调用lock位置进行自旋，产生了死锁
说明这个锁并不是可重入的。（在lock函数内，应验证线程是否为已经获得锁的线程）
2、若1问题已经解决，当unlock（）第一次调用时，就已经将锁释放了。实际上不应释放锁。
（采用计数次进行统计）
修改之后，如下：
public class SpinLock1 {
private AtomicReference<Thread> owner =new AtomicReference<>();
private int count =0;
public void lock(){
    Thread current = Thread.currentThread();
if(current==owner.get()) {
      count++;
return ;
}
while(!owner.compareAndSet(null, current)){
    }
  }
  public void unlock (){
    Thread current = Thread.currentThread();
if(current==owner.get()){
if(count!=0){
        count--;
}else{
        owner.compareAndSet(current, null);
}
    }
  }
}
这种方式实现的自旋锁即为可重入锁。
另，看一下mutex的情况：
Mutex可以分为递归锁(recursive mutex)和非递归锁(non-recursive mutex)。可递归锁也可称为可重入锁(reentrant mutex)，
非递归锁又叫不可重入锁(non-reentrant mutex)。
二者唯一的区别是，同一个线程可以多次获取同一个递归锁，不会产生死锁。而如果一个线程多次获取同一个非递归锁，则会产生死锁。
Windows下的Mutex和Critical Section是可递归的。
Linux下的pthread_mutex_t锁默认是非递归的。可以显示的设置PTHREAD_MUTEX_RECURSIVE属性，将pthread_mutex_t设为递归锁。
**公平锁**
公平和非公平锁的队列都基于锁内部维护的一个双向链表，表结点Node的值就是每一个请求当前锁的线程。公平锁则在于每次都是依次从队首取值。
锁的实现方式是基于如下几点： 
表结点Node和状态state的volatile关键字。
sum.misc.Unsafe.compareAndSet的原子操作(见附录)。
**非公平锁**
在等待锁的过程中， 如果有任意新的线程妄图获取锁，都是有很大的几率直接获取到锁的。
本文综合参考与网络，如有侵权，请联系删除。
**推荐阅读：**
1，[JAVA程序员面试总结，高手整理加强版](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484457&idx=1&sn=550d4c58e05c021602cef7a93ed0ba8b&chksm=9f38e701a84f6e179cd3760fdfef8fb240fcdc8cd31af049621680a9be5af5376b428e6d492f&scene=21#wechat_redirect)
2，[超越Spark，大数据集群计算的生产实践](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484395&idx=1&sn=82ae3b4fd460d4666d46aa2348f540c6&chksm=9f38e0c3a84f69d5b0d871133d9f94c3612c7a98e9dc93ac74d0f2ccce12a13f18207cbd9ce0&scene=21#wechat_redirect)
3，[基于zookeeper leader选举方式一](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483890&idx=1&sn=95938c9d25e2273129798acefa08f0c1&chksm=9f38e2daa84f6bcc7212262354735617c1a5f14eb9738e7a658930a261e068b1f97cd2dff27c&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
