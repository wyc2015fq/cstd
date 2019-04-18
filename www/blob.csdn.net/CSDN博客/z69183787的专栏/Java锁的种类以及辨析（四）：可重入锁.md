# Java锁的种类以及辨析（四）：可重入锁 - z69183787的专栏 - CSDN博客
2017年09月29日 15:47:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：256
个人分类：[锁-概述](https://blog.csdn.net/z69183787/article/category/7202705)

锁作为并发共享数据，保证一致性的工具，在JAVA平台有多种实现(如 synchronized 和 ReentrantLock等等 ) 。这些已经写好提供的锁为我们开发提供了便利，但是锁的具体性质以及类型却很少被提及。本系列文章将分析JAVA下常见的锁名称以及特性，为大家答疑解惑。
### 四、可重入锁：
本文里面讲的是广义上的可重入锁，而不是单指JAVA下的ReentrantLock。
可重入锁，也叫做递归锁，指的是同一线程 外层函数获得锁之后 ，内层递归函数仍然有获取该锁的代码，但不受影响。
在JAVA环境下 ReentrantLock 和synchronized 都是 可重入锁
下面是使用实例
|`01`|`public``class``Test ``implements``Runnable{`|
|`02`||
|`03`|`    ``public``synchronized``void``get(){`|
|`04`|`        ``System.out.println(Thread.currentThread().getId());`|
|`05`|`        ``set();`|
|`06`|`    ``}`|
|`07`||
|`08`|`    ``public``synchronized``void``set(){`|
|`09`|`        ``System.out.println(Thread.currentThread().getId());`|
|`10`|`    ``}`|
|`11`||
|`12`|`    ``@Override`|
|`13`|`    ``public``void````run() {```|
|`14`|`        ``get();`|
|`15`|`    ``}`|
|`16`|`    ``public``static``void````main(String[] args) {```|
|`17`|`        ````Test ss=````new``Test();`|
|`18`|`        ``new``Thread(ss).start();`|
|`19`|`        ``new``Thread(ss).start();`|
|`20`|`        ``new``Thread(ss).start();`|
|`21`|`    ``}`|
|`22`|`}`|
|`01`|`public``class``Test ``implements````Runnable {```|
|`02`|`    ````ReentrantLock lock =````new``ReentrantLock();`|
|`03`||
|`04`|`    ``public``void````get() {```|
|`05`|`        ``lock.lock();`|
|`06`|`        ``System.out.println(Thread.currentThread().getId());`|
|`07`|`        ``set();`|
|`08`|`        ``lock.unlock();`|
|`09`|`    ``}`|
|`10`||
|`11`|`    ``public``void````set() {```|
|`12`|`        ``lock.lock();`|
|`13`|`        ``System.out.println(Thread.currentThread().getId());`|
|`14`|`        ``lock.unlock();`|
|`15`|`    ``}`|
|`16`||
|`17`|`    ``@Override`|
|`18`|`    ``public``void````run() {```|
|`19`|`        ``get();`|
|`20`|`    ``}`|
|`21`||
|`22`|`    ``public``static``void````main(String[] args) {```|
|`23`|`        ````Test ss =````new``Test();`|
|`24`|`        ``new``Thread(ss).start();`|
|`25`|`        ``new``Thread(ss).start();`|
|`26`|`        ``new``Thread(ss).start();`|
|`27`|`    ``}`|
|`28`|`}`|
两个例子最后的结果都是正确的，即 同一个线程id被连续输出两次。
结果如下：
Threadid: 8
Threadid: 8
Threadid: 10
Threadid: 10
Threadid: 9
Threadid: 9
可重入锁最大的作用是避免死锁
我们以自旋锁作为例子，
|`01`|`public``class````SpinLock {```|
|`02`|`    ``private````AtomicReference<Thread> owner =````new``AtomicReference<>();`|
|`03`|`    ``public``void``lock(){`|
|`04`|`        ````Thread current = Thread.currentThread();```|
|`05`|`        ``while``(!owner.compareAndSet(``null````, current)){```|
|`06`|`        ``}`|
|`07`|`    ``}`|
|`08`|`    ``public``void````unlock (){```|
|`09`|`        ````Thread current = Thread.currentThread();```|
|`10`|`        ``owner.compareAndSet(current, ``null``);`|
|`11`|`    ``}`|
|`12`|`}`|
对于自旋锁来说，
1、若有同一线程两调用lock() ，会导致第二次调用lock位置进行自旋，产生了死锁
说明这个锁并不是可重入的。（在lock函数内，应验证线程是否为已经获得锁的线程）
2、若1问题已经解决，当unlock（）第一次调用时，就已经将锁释放了。实际上不应释放锁。
（采用计数次进行统计）
修改之后，如下：
|`01`|`public``class````SpinLock1 {```|
|`02`|`    ``private````AtomicReference<Thread> owner =````new``AtomicReference<>();`|
|`03`|`    ``private``int````count =````0``;`|
|`04`|`    ``public``void``lock(){`|
|`05`|`        ````Thread current = Thread.currentThread();```|
|`06`|`        ``if````(current==owner.get()) {```|
|`07`|`            ``count++;`|
|`08`|`            ``return``;`|
|`09`|`        ``}`|
|`10`||
|`11`|`        ``while``(!owner.compareAndSet(``null````, current)){```|
|`12`||
|`13`|`        ``}`|
|`14`|`    ``}`|
|`15`|`    ``public``void````unlock (){```|
|`16`|`        ````Thread current = Thread.currentThread();```|
|`17`|`        ``if``(current==owner.get()){`|
|`18`|`            ``if``(count!=``0``){`|
|`19`|`                ``count--;`|
|`20`|`            ``}``else``{`|
|`21`|`                ``owner.compareAndSet(current, ``null``);`|
|`22`|`            ``}`|
|`23`||
|`24`|`        ``}`|
|`25`||
|`26`|`    ``}`|
|`27`|`}`|
该自旋锁即为可重入锁。
