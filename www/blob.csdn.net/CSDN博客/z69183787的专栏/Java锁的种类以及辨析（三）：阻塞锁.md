# Java锁的种类以及辨析（三）：阻塞锁 - z69183787的专栏 - CSDN博客
2017年09月29日 15:44:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：377
锁作为并发共享数据，保证一致性的工具，在JAVA平台有多种实现(如 synchronized 和 ReentrantLock等等 ) 。这些已经写好提供的锁为我们开发提供了便利，但是锁的具体性质以及类型却很少被提及。本系列文章将分析JAVA下常见的锁名称以及特性，为大家答疑解惑。
### 三、阻塞锁：
阻塞锁，与自旋锁不同，改变了线程的运行状态。
在JAVA环境中，线程Thread有如下几个状态：
1，新建状态
2，就绪状态
3，运行状态
4，阻塞状态
5，死亡状态
阻塞锁，可以说是让线程进入阻塞状态进行等待，当获得相应的信号（唤醒，时间） 时，才可以进入线程的准备就绪状态，准备就绪状态的所有线程，通过竞争，进入运行状态。
JAVA中，能够进入\退出、阻塞状态或包含阻塞锁的方法有 ，synchronized 关键字（其中的重量锁），ReentrantLock，Object.wait()\notify(),LockSupport.park()/unpart()(j.u.c经常使用)
下面是一个JAVA 阻塞锁实例
|`01`|`package``lock;`|
|`02`||
|`03`|`import``java.util.concurrent.atomic.AtomicReferenceFieldUpdater;`|
|`04`|`import``java.util.concurrent.locks.LockSupport;`|
|`05`||
|`06`|`public``class````CLHLock1 {```|
|`07`|`    ``public``static``class````CLHNode {```|
|`08`|`        ``private``volatile````Thread isLocked;```|
|`09`|`    ``}`|
|`10`||
|`11`|`    ``@SuppressWarnings``(``"unused"``)`|
|`12`|`    ``private``volatile````CLHNode                                            tail;```|
|`13`|`    ``private``static``final````ThreadLocal<CLHNode>                           LOCAL   =````new``ThreadLocal<CLHNode>();`|
|`14`|`    ``private``static``final````AtomicReferenceFieldUpdater<CLHLock1, CLHNode> UPDATER = AtomicReferenceFieldUpdater.newUpdater(CLHLock1.````class``,`|
|`15`|`                                                                                    ``CLHNode.``class``, ``"tail"``);`|
|`16`||
|`17`|`    ``public``void````lock() {```|
|`18`|`        ````CLHNode node =````new``CLHNode();`|
|`19`|`        ``LOCAL.set(node);`|
|`20`|`        ````CLHNode preNode = UPDATER.getAndSet(````this````, node);```|
|`21`|`        ``if````(preNode !=````null````) {```|
|`22`|`            ````preNode.isLocked = Thread.currentThread();```|
|`23`|`            ``LockSupport.park(``this``);`|
|`24`|`            ````preNode =````null``;`|
|`25`|`            ``LOCAL.set(node);`|
|`26`|`        ``}`|
|`27`|`    ``}`|
|`28`||
|`29`|`    ``public``void````unlock() {```|
|`30`|`        ````CLHNode node = LOCAL.get();```|
|`31`|`        ``if``(!UPDATER.compareAndSet(``this````, node,````null````)) {```|
|`32`|`            ``System.out.println(``"unlock\t"````+ node.isLocked.getName());```|
|`33`|`            ``LockSupport.unpark(node.isLocked);`|
|`34`|`        ``}`|
|`35`|`        ````node =````null``;`|
|`36`|`    ``}`|
|`37`|`}`|
在这里我们使用了LockSupport.unpark()的阻塞锁。 该例子是将CLH锁修改而成。
阻塞锁的优势在于，阻塞的线程不会占用cpu时间， 不会导致 CPu占用率过高，但进入时间以及恢复时间都要比自旋锁略慢。
在竞争激烈的情况下 阻塞锁的性能要明显高于 自旋锁。
理想的情况则是; 在线程竞争不激烈的情况下，使用自旋锁，竞争激烈的情况下使用，阻塞锁。
