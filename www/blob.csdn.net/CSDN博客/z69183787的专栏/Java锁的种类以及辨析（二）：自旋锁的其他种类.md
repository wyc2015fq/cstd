# Java锁的种类以及辨析（二）：自旋锁的其他种类 - z69183787的专栏 - CSDN博客
2017年09月29日 15:40:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：222
个人分类：[锁-概述](https://blog.csdn.net/z69183787/article/category/7202705)

锁作为并发共享数据，保证一致性的工具，在JAVA平台有多种实现(如 synchronized 和 ReentrantLock等等 ) 。这些已经写好提供的锁为我们开发提供了便利，但是锁的具体性质以及类型却很少被提及。本系列文章将分析JAVA下常见的锁名称以及特性，为大家答疑解惑。
### 2.自旋锁的其他种类
上篇我们讲到了自旋锁，在自旋锁中 另有三种常见的锁形式:TicketLock ，CLHlock 和MCSlock
Ticket锁主要解决的是访问顺序的问题，主要的问题是在多核cpu上
|`01`|`package``com.alipay.titan.dcc.dal.entity;`|
|`02`||
|`03`|`import``java.util.concurrent.atomic.AtomicInteger;`|
|`04`||
|`05`|`public``class````TicketLock {```|
|`06`|`    ``private````AtomicInteger                     serviceNum =````new``AtomicInteger();`|
|`07`|`    ``private````AtomicInteger                     ticketNum  =````new``AtomicInteger();`|
|`08`|`    ``private``static``final````ThreadLocal<Integer> LOCAL      =````new``ThreadLocal<Integer>();`|
|`09`||
|`10`|`    ``public``void````lock() {```|
|`11`|`        ``int````myticket = ticketNum.getAndIncrement();```|
|`12`|`        ``LOCAL.set(myticket);`|
|`13`|`        ``while````(myticket != serviceNum.get()) {```|
|`14`|`        ``}`|
|`15`||
|`16`|`    ``}`|
|`17`||
|`18`|`    ``public``void````unlock() {```|
|`19`|`        ``int````myticket = LOCAL.get();```|
|`20`|`        ````serviceNum.compareAndSet(myticket, myticket +````1``);`|
|`21`|`    ``}`|
|`22`|`}`|
每次都要查询一个serviceNum 服务号，影响性能（必须要到主内存读取，并阻止其他cpu修改）。
CLHLock 和MCSLock 则是两种类型相似的公平锁，采用链表的形式进行排序，
|`01`|`import``java.util.concurrent.atomic.AtomicReferenceFieldUpdater;`|
|`02`||
|`03`|`public``class````CLHLock {```|
|`04`|`    ``public``static``class````CLHNode {```|
|`05`|`        ``private``volatile``boolean````isLocked =````true``;`|
|`06`|`    ``}`|
|`07`||
|`08`|`    ``@SuppressWarnings``(``"unused"``)`|
|`09`|`    ``private``volatile````CLHNode                                           tail;```|
|`10`|`    ``private``static``final````ThreadLocal<CLHNode>                          LOCAL   =````new``ThreadLocal<CLHNode>();`|
|`11`|`    ``private``static``final````AtomicReferenceFieldUpdater<CLHLock, CLHNode> UPDATER = AtomicReferenceFieldUpdater.newUpdater(CLHLock.````class``,`|
|`12`|`                                                                                   ``CLHNode.``class``, ``"tail"``);`|
|`13`||
|`14`|`    ``public``void````lock() {```|
|`15`|`        ````CLHNode node =````new``CLHNode();`|
|`16`|`        ``LOCAL.set(node);`|
|`17`|`        ````CLHNode preNode = UPDATER.getAndSet(````this````, node);```|
|`18`|`        ``if````(preNode !=````null````) {```|
|`19`|`            ``while````(preNode.isLocked) {```|
|`20`|`            ``}`|
|`21`|`            ````preNode =````null``;`|
|`22`|`            ``LOCAL.set(node);`|
|`23`|`        ``}`|
|`24`|`    ``}`|
|`25`||
|`26`|`    ``public``void````unlock() {```|
|`27`|`        ````CLHNode node = LOCAL.get();```|
|`28`|`        ``if``(!UPDATER.compareAndSet(``this````, node,````null````)) {```|
|`29`|`            ````node.isLocked =````false``;`|
|`30`|`        ``}`|
|`31`|`        ````node =````null``;`|
|`32`|`    ``}`|
|`33`|`}`|
CLHlock是不停的查询前驱变量， 导致不适合在NUMA 架构下使用（在这种结构下，每个线程分布在不同的物理内存区域）
MCSLock则是对本地变量的节点进行循环。不存在CLHlock 的问题。
[view
 source](http://ifeve.com/java_lock_see2/#viewSource)
[print](http://ifeve.com/java_lock_see2/#printSource)[?](http://ifeve.com/java_lock_see2/#about)
|`01`|`import``java.util.concurrent.atomic.AtomicReferenceFieldUpdater;`|
|`02`||
|`03`|`public``class````MCSLock {```|
|`04`|`    ``public``static``class````MCSNode {```|
|`05`|`        ``volatile````MCSNode next;```|
|`06`|`        ``volatile``boolean````isLocked =````true``;`|
|`07`|`    ``}`|
|`08`||
|`09`|`    ``private``static``final````ThreadLocal<MCSNode>                          NODE    =````new``ThreadLocal<MCSNode>();`|
|`10`|`    ``@SuppressWarnings``(``"unused"``)`|
|`11`|`    ``private``volatile````MCSNode                                           queue;```|
|`12`|`    ``private``static``final````AtomicReferenceFieldUpdater<MCSLock, MCSNode> UPDATER = AtomicReferenceFieldUpdater.newUpdater(MCSLock.````class``,`|
|`13`|`                                                                                   ``MCSNode.``class``, ``"queue"``);`|
|`14`||
|`15`|`    ``public``void````lock() {```|
|`16`|`        ````MCSNode currentNode =````new``MCSNode();`|
|`17`|`        ``NODE.set(currentNode);`|
|`18`|`        ````MCSNode preNode = UPDATER.getAndSet(````this````, currentNode);```|
|`19`|`        ``if````(preNode !=````null````) {```|
|`20`|`            ````preNode.next = currentNode;```|
|`21`|`            ``while````(currentNode.isLocked) {```|
|`22`||
|`23`|`            ``}`|
|`24`|`        ``}`|
|`25`|`    ``}`|
|`26`||
|`27`|`    ``public``void````unlock() {```|
|`28`|`        ````MCSNode currentNode = NODE.get();```|
|`29`|`        ``if````(currentNode.next ==````null````) {```|
|`30`|`            ``if``(UPDATER.compareAndSet(``this````, currentNode,````null````)) {```|
|`31`||
|`32`|`            ``} ``else``{`|
|`33`|`                ``while````(currentNode.next ==````null````) {```|
|`34`|`                ``}`|
|`35`|`            ``}`|
|`36`|`        ``} ``else``{`|
|`37`|`            ````currentNode.next.isLocked =````false``;`|
|`38`|`            ````currentNode.next =````null``;`|
|`39`|`        ``}`|
|`40`|`    ``}`|
|`41`|`}`|
从代码上 看，CLH 要比 MCS 更简单，
CLH 的队列是隐式的队列，没有真实的后继结点属性。
MCS 的队列是显式的队列，有真实的后继结点属性。
JUC ReentrantLock 默认内部使用的锁 即是 CLH锁（有很多改进的地方，将自旋锁换成了阻塞锁等等）。
