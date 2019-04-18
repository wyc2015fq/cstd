# Java多线程同步设计中使用Mutex - z69183787的专栏 - CSDN博客
2016年08月31日 11:42:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4688
　Mutex是互斥体，广泛地[应用](http://search.yesky.com/search.do?wd=%D3%A6%D3%C3)在多线程编程中。本文以广为流程的Doug
 Lea的concurrent工具包的Mutex实现为例，进行一点探讨。在Doug Lea的concurrent工具包中，Mutex实现了Sync接口，该接口是concurrent工具包中所有锁（lock）、门（gate）和条件变量（condition）的公共接口，Sync的实现类主要有：Mutex、Semaphore及其子类、Latch、CountDown、ReentrantLock等。这也体现了面向抽象编程的思想，使我们可以在不改变代码或者改变少量代码的情况下，选择使用Sync的不同实现。下面是Sync接口的定义： 
public interface Sync
{
　public void acquire() throws InterruptedException; 
　//获取许可 
　public boolean attempt(long msecs) throws InterruptedException;
　//尝试获取许可 
　public void release(); 
　//释放许可
}
　　通过使用Sync可以替代Java synchronized关键字，并提供更加灵活的同步控制。当然，并不是说　concurrent工具包是和Java synchronized独立的技术，其实concurrent工具包也是在synchronized的基础上搭建的，从下面对Mutex源码的解析即可以看到这一点。synchronized关键字仅在方法内或者代码块内有效，而使用Sync却可以跨越方法甚至通过在对象之间传递，跨越对象进行同步。这是Sync及concurrent工具包比直接使用synchronized更加强大的地方。
　　注意Sync中的acquire()和attempt()都会抛出InterruptedException，所以使用Sync及其子类时，调用这些方法一定要捕获InterruptedException。而release()方法并不会抛出InterruptedException，这是因为在acquire()和attempt()方法中可能会调用wait（）等待其它线程释放锁。而release()在实现上进行了简化，直接释放锁，不管是否真的持有。所以，你可以对一个并没有acquire()的线程调用release()这也不会有什么问题。而由于release()不会抛出InterruptedException，所以我们可以在catch或finally子句中调用release()以保证获得的锁能够被正确释放。比如：
class X 
{
　Sync gate; // ...
　public void m() 
　{
　　try
　　{
　　　gate.acquire();
　　　// block until condition holds
　　　try 
　　　{
　　　　// ... method body
　　　}
　　　finally { gate.release(); }
　　}
　　catch (InterruptedException ex) { // ... evasive action } 
　}
}
　　Mutex是一个非重入的互斥锁。Mutex广泛地用在需要跨越方法的before/after类型的同步环境中。下面是Doug Lea的concurrent工具包中的Mutex的实现。
public class Mutex implements Sync
{
　/** The lock status **/
　protected boolean inuse_ = false;
　public void acquire() throws InterruptedException 
　{
　　if (Thread.interrupted()) throw new InterruptedException();//(1) 
　　synchronized(this)
　　{ 
　　　try 
　　　{
　　　　while (inuse_) wait();
　　　　inuse_ = true;
　　　}
　　　catch (InterruptedException ex) 
　　　{
　　　　//(2) 
　　　　notify();
　　　　throw ex;
　　　}
　　}
　}
　public synchronized void release() 
　{
　　inuse_ = false; 
　　notify();
　}
　public boolean attempt(long msecs) throws InterruptedException 
　{
　　if (Thread.interrupted()) throw new InterruptedException();
　　synchronized(this) 
　　{
　　　if (!inuse_) 
　　　{
　　　　inuse_ = true;
　　　　return true;
　　　}
　　　else if (msecs <= 0)
　　　　return false;
　　　else
　　　{
　　　　long waitTime = msecs; 
　　　　long start = System.currentTimeMillis();
　　　　try
　　　　{
　　　　　for (;;) 
　　　　　{
　　　　　　wait(waitTime); 
　　　　　　if (!inuse_)
　　　　　　{
　　　　　　　inuse_ = true;
　　　　　　　return true;
　　　　　　}
　　　　　　else
　　　　　　{
　　　　　　　waitTime = msecs - (System.currentTimeMillis() - start);
　　　　　　　if (waitTime <= 0) // (3) 
　　　　　　　　return false;
　　　　　　 }
　　　　　}
　　　　}
　　　　catch (InterruptedException ex)
　　　　{
　　　　　notify();
　　　　　throw ex; 
　　　　}
　　　}
　　}
　}
}
　　为什么要在acquire()和attempt(0方法的开始都要检查当前线程的中断标志呢？这是为了在当前线程已经被打断时，可以立即返回，而不会仍然在锁标志上等待。调用一个线程的interrupt()方法根据当前线程所处的状态，可能产生两种不同的结果：当线程在运行过程中被打断，则设置当前线程的中断标志为true；如果当前线程阻塞于wait()、sleep()、join()，则当前线程的中断标志被清空，同时抛出InterruptedException。所以在上面代码的位置（2）也捕获了InterruptedException，然后再次抛出InterruptedException。
　　release()方法简单地重置inuse_标志，并通知其它线程。
　　attempt()方法是利用Java的Object.wait(long)进行计时的，由于Object.wait(long)不是一个精确的时钟，所以attempt(long)方法也是一个粗略的计时。注意代码中位置（3），在超时时返回。
Mutex是Sync的一个基本实现，除了实现了Sync接口中的方法外，并没有添加新的方法。所以，Mutex的使用和Sync的完全一样。在concurrent包的API中Doug给出了一个精细锁定的List的实现示例，我们这儿也给出，作为对Mutex和Sync使用的一个例子：
class Node
{
　Object item; Node next;
　Mutex lock = new Mutex();
　// 每一个节点都持有一个锁 
　Node(Object x, Node n) 
　{
　　item = x;
　　next = n; 
　}
}
class List
{ 
　protected Node head; 
　// 指向列表的头 
　// 使用Java的synchronized保护head域 
　// (我们当然可以使用Mutex，但是这儿似乎没有这样做的必要 
　protected synchronized Node getHead() 
　{ return head; }
　boolean search(Object x) throws InterruptedException 
　{
　　Node p = getHead();
　　if (p == null) return false; 
　　// (这儿可以更加紧凑，但是为了演示的清楚，各种情况都分别进行处理) 
　　p.lock.acquire(); 
　　// Prime loop by acquiring first lock.
　　// (If the acquire fails due to
　　// interrupt, the method will throw
　　// InterruptedException now, 
　　// so there is no need for any 
　　// further cleanup.) 
　　for (;;) 
　　{
　　　if (x.equals(p.item)) 
　　　{
　　　　p.lock.release(); 
　　　　// 释放当前节点的锁 
　　　　return true;
　　　}
　　　else
　　　{
　　　　Node nextp = p.next; 
　　　　if (nextp == null)
　　　　{
　　　　　p.lock.release();
　　　　　// 释放最后持有的锁 
　　　　　return false;
　　　　}
　　　　else
　　　　{
　　　　　try
　　　　　{
　　　　　　nextp.lock.acquire(); 
　　　　　　// 在释放当前锁之前获取下一个节点的锁 
　　　　　}
　　　　　catch (InterruptedException ex)
　　　　　{
　　　　　　p.lock.release();
　　　　　　// 如果获取失败，也释放当前的锁 throw ex; 
　　　　　}
　　　　　p.lock.release();
　　　　　// 释放上个节点的锁，现在已经持有新的锁了 
　　　　　p = nextp;
　　　　}
　　　}
　　}
　}
　synchronized void add(Object x)
　{
　　// 使用synchronized保护head域 
　　head = new Node(x, head);
　}
　// ... other similar traversal and update methods ... 
}
