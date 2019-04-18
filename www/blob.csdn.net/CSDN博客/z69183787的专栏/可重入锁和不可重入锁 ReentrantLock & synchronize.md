# 可重入锁和不可重入锁 ReentrantLock & synchronize - z69183787的专栏 - CSDN博客
2018年02月21日 21:48:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3910
[http://blog.csdn.net/qq838642798/article/details/65441415](http://blog.csdn.net/qq838642798/article/details/65441415)
[https://www.cnblogs.com/dj3839/p/6580765.html](https://www.cnblogs.com/dj3839/p/6580765.html)
用lock来保证原子性（this.count++这段代码称为临界区）
什么是原子性，就是不可分，从头执行到尾，不能被其他线程同时执行。
可通过CAS来实现原子操作
CAS(Compare and Swap):
CAS操作需要输入两个数值，一个旧值（期望操作前的值）和一个新值，在操作期间先比较下旧值有没有发生变化，如果没有发生变化，才交换成新值，发生了变化则不交换。
CAS主要通过compareAndSwapXXX()方法来实现，而这个方法的实现需要涉及底层的unsafe类
unsafe类：java不能直接访问操作系统底层，而是通过本地方法来访问。Unsafe类提供了硬件级别的原子操作
这里有个介绍原子操作的博客
[https://my.oschina.net/xinxingegeya/blog/499223](https://my.oschina.net/xinxingegeya/blog/499223)
还有对unsafe类详解的博客
[http://www.cnblogs.com/mickole/articles/3757278.html](http://www.cnblogs.com/mickole/articles/3757278.html)
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1 public class Counter{
 2     private Lock lock = new Lock();
 3     private int count = 0;
 4     public int inc(){
 5         lock.lock();
 6         this.count++;
 7         lock.unlock();
 8         return count;
 9     }
10 }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
不可重入锁
先来设计一种锁
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1 public class Lock{
 2     private boolean isLocked = false;
 3     public synchronized void lock() throws InterruptedException{
 4         while(isLocked){    
 5             wait();
 6         }
 7         isLocked = true;
 8     }
 9     public synchronized void unlock(){
10         isLocked = false;
11         notify();
12     }
13 }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
这其实是个不可重入锁，举个例子
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1 public class Count{
 2     Lock lock = new Lock();
 3     public void print(){
 4         lock.lock();
 5         doAdd();
 6         lock.unlock();
 7     }
 8     public void doAdd(){
 9         lock.lock();
10         //do something
11         lock.unlock();
12     }
13 }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
当调用print()方法时，获得了锁，这时就无法再调用doAdd()方法，这时必须先释放锁才能调用，所以称这种锁为不可重入锁，也叫自旋锁。
可重入锁
设计如下:
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1 public class Lock{
 2     boolean isLocked = false;
 3     Thread  lockedBy = null;
 4     int lockedCount = 0;
 5     public synchronized void lock()
 6             throws InterruptedException{
 7         Thread thread = Thread.currentThread();
 8         while(isLocked && lockedBy != thread){
 9             wait();
10         }
11         isLocked = true;
12         lockedCount++;
13         lockedBy = thread;
14     }
15     public synchronized void unlock(){
16         if(Thread.currentThread() == this.lockedBy){
17             lockedCount--;
18             if(lockedCount == 0){
19                 isLocked = false;
20                 notify();
21             }
22         }
23     }
24 }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
相对来说，可重入就意味着：线程可以进入任何一个它已经拥有的锁所同步着的代码块。
第一个线程执行print()方法，得到了锁，使lockedBy等于当前线程，也就是说，执行的这个方法的线程获得了这个锁，执行add()方法时，同样要先获得锁，因不满足while循环的条件，也就是不等待，继续进行，将此时的lockedCount变量，也就是当前获得锁的数量加一，当释放了所有的锁，才执行notify()。如果在执行这个方法时，有第二个线程想要执行这个方法，因为lockedBy不等于第二个线程，导致这个线程进入了循环，也就是等待，不断执行wait()方法。只有当第一个线程释放了所有的锁，执行了notify()方法，第二个线程才得以跳出循环，继续执行。
这就是可重入锁的特点。
java中常用的可重入锁
synchronized
java.util.concurrent.locks.ReentrantLock
ps:顺便记录下java中实现原子操作的类（记录至[http://blog.csdn.net/huzhigenlaohu/article/details/51646455](http://blog.csdn.net/huzhigenlaohu/article/details/51646455)）
- AtomicIntegerFieldUpdater:原子更新整型的字段的更新器
- AtomicLongFieldUpdater：原子更新长整型字段的更新器
- AtomicStampedReference:原子更新带有版本号的引用类型。该类将整型数值与引用关联起来，可用于原子的更新数据和数据的版本号，可以解决使用CAS进行原子更新时可能出现的ABA问题。
- AtomicReference ：原子更新引用类型
- AtomicReferenceFieldUpdater ：原子更新引用类型里的字段
- AtomicMarkableReference：原子更新带有标记位的引用类型。可以原子更新一个布尔类型的标记位和应用类型
- AtomicIntegerArray ：原子更新整型数组里的元素
- AtomicLongArray :原子更新长整型数组里的元素
- AtomicReferenceArray : 原子更新引用类型数组的元素
- AtomicBooleanArray ：原子更新布尔类型数组的元素
- AtomicBoolean ：原子更新布尔类型
- AtomicInteger： 原子更新整型
- AtomicLong: 原子更新长整型
ReenTrantLock可重入锁（和synchronized的区别）总结
**可重入性：**
从名字上理解，ReenTrantLock的字面意思就是再进入的锁，其实synchronized关键字所使用的锁也是可重入的，两者关于这个的区别不大。两者都是同一个线程没进入一次，锁的计数器都自增1，所以要等到锁的计数器下降为0时才能释放锁。
**锁的实现：**
Synchronized是依赖于JVM实现的，而ReenTrantLock是JDK实现的，有什么区别，说白了就类似于操作系统来控制实现和用户自己敲代码实现的区别。前者的实现是比较难见到的，后者有直接的源码可供阅读。
**性能的区别：**
在Synchronized优化以前，synchronized的性能是比ReenTrantLock差很多的，但是自从Synchronized引入了偏向锁，轻量级锁（自旋锁）后，两者的性能就差不多了，在两种方法都可用的情况下，官方甚至建议使用synchronized，其实synchronized的优化我感觉就借鉴了ReenTrantLock中的CAS技术。都是试图在用户态就把加锁问题解决，避免进入内核态的线程阻塞。
**功能区别：**
便利性：很明显Synchronized的使用比较方便简洁，并且由编译器去保证锁的加锁和释放，而ReenTrantLock需要手工声明来加锁和释放锁，为了避免忘记手工释放锁造成死锁，所以最好在finally中声明释放锁。
锁的细粒度和灵活度：很明显ReenTrantLock优于Synchronized
**ReenTrantLock独有的能力：**
1.      ReenTrantLock可以指定是公平锁还是非公平锁。而synchronized只能是非公平锁。所谓的公平锁就是先等待的线程先获得锁。
2.      ReenTrantLock提供了一个Condition（条件）类，用来实现分组唤醒需要唤醒的线程们，而不是像synchronized要么随机唤醒一个线程要么唤醒全部线程。
3.      ReenTrantLock提供了一种能够中断等待锁的线程的机制，通过lock.lockInterruptibly()来实现这个机制。
**ReenTrantLock实现的原理：**
在网上看到相关的源码分析，本来这块应该是本文的核心，但是感觉比较复杂就不一一详解了，简单来说，ReenTrantLock的实现是一种自旋锁，通过循环调用CAS操作来实现加锁。它的性能比较好也是因为避免了使线程进入内核态的阻塞状态。**想尽办法避免线程进入内核的阻塞状态是我们去分析和理解锁设计的关键钥匙。**
**什么情况下使用ReenTrantLock：**
答案是，如果你需要实现ReenTrantLock的三个独有功能时。
