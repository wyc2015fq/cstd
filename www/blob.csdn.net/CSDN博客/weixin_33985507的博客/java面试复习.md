# java面试复习 - weixin_33985507的博客 - CSDN博客
2018年08月27日 19:58:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
# 1Java基础
## 1.HashMap的底层实现原理
1)实际上HashMap是一个“链表散列”， 每次新建一个HashMap时，都会初始化一个table数组。table数组的元素为Entry节点。其中Entry为HashMap的内部类，它包含了键key、值value、下一个节点next，以及hash值， Entry链表构成了table数组。
2)存储过程，首先判断key是否为null，若为null，则直接调用putForNullKey方法。若不为空则先计算key的hash值，然后根据hash值搜索在table数组中的索引位置，如果table数组在该位置处有元素，则通过比较是否存在相同的key，若存在则覆盖原来key的value，否则将该元素保存在链头（最先保存的元素放在链尾）。若table在该处没有元素，则直接保存。
3)默认初始容量 16和默认加载因子 (0.75)
参考文献：HTTP://WWW.CNBLOGS.COM/SKYWANG12345/P/3310835.HTML
参考文献：HTTPS://WWW.CNBLOGS.COM/CHENSSY/P/3521565.HTML
## 2.hashMap和Hashtable的区别
1)Hashtable是线程安全的，效率比较低。
2)Hashtable没有遵循驼峰命名法
3)继承的父类不同：HashMap是继承自AbstractMap类，而HashTable是继承自Dictionary类4)Hashtable既不支持Null key也不支持Null value。HashMap中，null可以作为键，这样的键只有一个；可以有一个或多个键所对应的值为null。
5)遍历的方式不同：Hashtable、HashMap都使用了 Iterator。而由于历史原因，Hashtable还使用了Enumeration的方式 。
6)初始容量大小和每次扩充容量大小的不同 Hashtable默认的初始大小为11，之后每次扩充，容量变为原来的2n+1。HashMap默认的初始化大小为16。之后每次扩充，容量变为原来的2倍。
7)计算hash值的方法不同 。Hashtable直接使用key对象的hashCode。HashMap重新根据key对象的hashcode计算hash值，减少hash冲突。
## 3.ArrayList和LinkedList区别
1)ArrayList实现了List接口,它是以数组的方式来实现的,数组的特性是可以使用索引的方式来快速定位对象的位置,因此对于快速的随机取得对象的需求,使用ArrayList实现执行效率上会比较好2)LinkedList是采用链表的方式来实现List接口的,它本身有自己特定的方法，如: addFirst(),addLast(),getFirst(),removeFirst()等. 由于是采用链表实现的,因此在进行insert和remove动作时在效率上要比ArrayList要好得多!适合用来实现Stack(堆栈)与Queue(队列),前者先进后出，后者是先进先出.
## 4.在删除可插入对象的动作时，为什么ArrayList的效率会比较低呢?
 解析：因为ArrayList是使用数组实现的,若要从数组中删除或插入某一个对象，需要移动后段的数组元素，从而会重新调整索引顺序,调整索引顺序会消耗一定的时间，所以速度上就会比LinkedList要慢许多. 相反,LinkedList是使用链表实现的,若要从链表中删除或插入某一个对象,只需要改变前后对象的引用即可!
# 2线程
## 1.什么是分布式锁
1)线程锁：主要用来给方法、代码块加锁。当某个方法或代码使用锁，在同一时刻仅有一个线程执行该方法或该代码段。线程锁只在同一JVM中有效果，因为线程锁的实现在根本上是依靠线程之间共享内存实现的，比如synchronized是共享对象头，显示锁Lock是共享某个变量（state）。
2)进程锁：为了控制同一操作系统中多个进程访问某个共享资源，因为进程具有独立性，各个进程无法访问其他进程的资源，因此无法通过synchronized等线程锁实现进程锁。
3)分布式锁：当多个进程不在同一个系统中，用分布式锁控制多个进程对资源的访问。2.分布式线程锁的应用场景解析：有这样一个情境，线程A和线程B都共享某个变量X。如果是单机情况下（单JVM），线程之间共享内存，只要使用线程锁就可以解决并发问题。如果是分布式情况下（多JVM），线程A和线程B很可能不是在同一JVM中，这样线程锁就无法起到作用了，这时候就要用到分布式锁来解决。
## 3.分布式锁的Redis实现方式
解析：分布式锁实现的关键是在分布式的应用服务器外，搭建一个存储服务器，存储锁信息，我们可以利用Redis，数据库，zookeeper实现。首先我们要搭建一个Redis服务器，用Redis服务器来存储锁信息。
在实现的时候要注意的几个关键点：
a.锁信息必须是会过期超时的，不能让一个线程长期占有一个锁而导致死锁；
b.同一时刻只能有一个线程获取到锁。
分布式锁的redis命令：
setnx(key, value)：“set if not exits”，若该key-value不存在，则成功加入缓存并且返回1，否则返回0。
get(key)：获得key对应的value值，若不存在则返回null。
getset(key, value)：先获取key对应的value值，若不存在则返回null，然后将旧的value更新为新的value。
expire(key, seconds)：设置key-value的有效期为seconds秒。 
##  4.Zookeeper方式实现分布式锁
解析：Zookeeper提供一个多层级的节点命名空间（节点称为znode），每个节点都用一个以斜杠（/）分隔的路径表示，而且每个节点都有父节点（根节点除外），非常类似于文件系统。例如，/foo/doo这个表示一个znode，它的父节点为/foo，父父节点为/，而/为根节点没有父节点。与文件系统不同的是，这些节点都可以设置关联的数据，而文件系统中只有文件节点可以存放数据而目录节点不行。Zookeeper为了保证高吞吐和低延迟，在内存中维护了这个树状的目录结构，这种特性使得Zookeeper不能用于存放大量的数据，每个节点的存放数据上限为1M。
## 5.并发的状态 
## 6.Runnable相比Thread和的优势
1)适合多个相同的程序代码的线程去处理同一个资源
2)可以避免java中的单继承的限制
3)增加程序的健壮性，代码可以被多个线程共享，代码和数据独立
4)线程池只能放入实现Runable或callable类线程，不能直接放入继承Thread的类
## 7.线程的优先级
Java线程有优先级，优先级高的线程会获得较多的运行机会。Java线程的优先级用整数表示，取值范围是1~10。的setPriority()和getPriority()方法分别用来设置和获取线程的优先级。主线程的默认优先级为Thread.NORM_PRIORITY。线程的优先级有继承关系，比如A线程中创建了B线程，那么B将和A具有相同的优先级。JVM提供了10个线程优先级，但与常见的操作系统都不能很好的映射。如果希望程序能移植到各个操作系统中，应该仅仅使用Thread类有以下三个静态常量作为优先级，这样能保证同样的优先级采用了同样的调度方式。
## 8.wait和sleep区别
共同点：
1)他们都是在多线程的环境下，都可以在程序的调用处阻塞指定的毫秒数，并返回。 
2)wait()和sleep()都可以通过interrupt()方法 打断线程的暂停状态 ，从而使线程立刻抛出InterruptedException。 
不同点： 
1)Thread类的方法：sleep(),yield()等 Object的方法：wait()和notify()等 
2)每个对象都有一个锁来控制同步访问。Synchronized关键字可以和对象的锁交互，来实现线程的同步。 sleep方法没有释放锁，而wait方法释放了锁，使得其他线程可以使用同步控制块或者方法。 
3)wait，notify和notifyAll只能在同步控制方法或者同步控制块里面使用，而sleep可以在任何地方使用 
4)sleep必须捕获异常，而wait，notify和notifyAll不需要捕获异常
## 9.Java中Runnable和Callable有什么不同？
Runnable和Callable都代表那些要在不同的线程中执行的任务。Runnable从JDK1.0开始就有了，Callable是在 JDK1.5增加的。它们的主要区别是Callable的 call() 方法可以返回值和抛出异常，而Runnable的run()方法没有这些功能。Callable可以返回装载有计算结果的Future对象。
## 10.Java中的volatile 变量是什么？
volatile是一个特殊的修饰符，只有成员变量才能使用它。在Java并发程序缺少同步类的情况下，多线程对成员变量的操作对其它线程是透明的。volatile变量可以保证下一个读取操作会在前一个写操作之后发生。
## 11.什么是线程安全？Vector是一个线程安全类吗？
如果代码所在的进程中有多个线程在同时运行，而这些线程可能会同时运行这段代码。如果每次运行结果和单线程运行的结果是一样的，而且其他的变量的值也和预期的是一样的，就是线程安全的。一个线程安全的计数器类的同一个实例对象在被多个线程使用的情况下也不会出现计算失误。可以将集合类分成两组，线程安全和非线程安全的。Vector 是用同步方法来实现线程安全的, 而和它相似的ArrayList不是线程安全的。
## 12.为什么wait, notify 和 notifyAll这些方法不在thread类里面
一个很明显的原因是JAVA提供的锁是对象级的而不是线程级的，每个对象都有锁，通 过线程获得。如果线程需要等待某些锁那么调用对象中的wait()方法就有意义了。如果wait()方法定义在Thread类中，线程正在等待的是哪个锁 就不明显了。简单的说，由于wait，notify和notifyAll都是锁级别的操作，所以把他们定义在Object类中因为锁属于对象。
## 13.ThreadLocal变量
ThreadLocal是Java里一种特殊的变量。每个线程都有一个ThreadLocal就是每个线程都拥有了自己独立的一个变量，竞争条件被彻底消除了。它是为创建代价高昂的对象获取线程安全的好方法，比如你可以用ThreadLocal让SimpleDateFormat变成线程安全的，因为那个类创建代价高昂且每次调用都需要创建不同的实例所以不值得在局部范围使用它，如果为每个线程提供一个自己独有的变量拷贝，将大大提高效率。首先，通 过复用减少了代价高昂的对象的创建个数。其次，你在没有使用高代价的同步或者不变性的情况下获得了线程安全。线程局部变量的另一个不错的例子是 ThreadLocalRandom类，它在多线程环境中减少了创建代价高昂的Random对象的个数。
## 14.Java多线程中的死锁
死锁是指两个或两个以上的进程在执行过程中，因争夺资源而造成的一种互相等待的现象，若无外力作用，它们都将无法推进下去。这是一个严重的问题，因为死锁会让你的程序挂起无法完成任务，死锁的发生必须满足以下四个条件：
1)互斥条件：一个资源每次只能被一个进程使用。
2)请求与保持条件：一个进程因请求资源而阻塞时，对已获得的资源保持不放。
3)不剥夺条件：进程已获得的资源，在末使用完之前，不能强行剥夺。
4)循环等待条件：若干进程之间形成一种头尾相接的循环等待资源关系。避免死锁最简单的方法就是阻止循环等待条件，将系统中所有的资源设置标志位、排序，规定所有的进程申请资源必须以一定的顺序（升序或降序）做操作来避免死锁。
## 15. 有三个线程T1，T2，T3，怎么确保它们按顺序执行
在多线程中有多种方法让线程按特定顺序执行，你可以用线程类的join()方法在一个线程中启动另一个线程，另外一个线程完成该线程继续执行。为了确保三个线程的顺序你应该先启动最后一个(T3调用T2，T2调用T1)，这样T1就会先完成而T3最后完成。
## 16. Java线程池中submit() 和 execute()方法有什么区别
两个方法都可以向线程池提交任务，execute()方法的返回类型是void，它定义在Executor接口中, 而submit()方法可以返回持有计算结果的Future对象，它定义在ExecutorService接口中，它扩展了Executor接口，其它线 程池类像ThreadPoolExecutor和ScheduledThreadPoolExecutor都有这些方法。
## 17.单例双检索
创建一个单例时，通过两次检查，一次同步来防止多次线程下的多例发生。但由于无序写入的特性，并不一定保证成功。
## 18.写出3条你遵循的多线程最佳实践
1)给你的线程起个有意义的名字。这样可以方便找bug或追踪。OrderProcessor, QuoteProcessor or TradeProcessor 这种名字比 Thread-1. Thread-2 and Thread-3 好多了，给线程起一个和它要完成的任务相关的名字，所有的主要框架甚至JDK都遵循这个最佳实践。
2)避免锁定和缩小同步的范围锁花费的代价高昂且上下文切换更耗费时间空间，试试最低限度的使用同步和锁，缩小临界区。因此相对于同步方法我更喜欢同步块，它给我拥有对锁的绝对控制权。
3)多用同步类少用wait 和 notify首先，CountDownLatch, Semaphore, CyclicBarrier 和 Exchanger 这些同步类简化了编码操作，而用wait和notify很难实现对复杂控制流的控制。其次，这些类是由最好的企业编写和维护在后续的JDK中它们还会不断 优化和完善，使用这些更高等级的同步工具你的程序可以不费吹灰之力获得优化。
4)多用并发集合少用同步集合这是另外一个容易遵循且受益巨大的最佳实践，并发集合比同步集合的可扩展性更好，所以在并发编程时使用并发集合效果更好。如果下一次你需要用到map，你应该首先想到用ConcurrentHashMap。
参考文献：HTTPS://WWW.CNBLOGS.COM/WXD0108/P/5479442.HTML
参考文献：HTTPS://WWW.CNBLOGS.COM/YJD_HYCF_SPACE/P/7526608.HTML
参考文献：HTTPS://WWW.CNBLOGS.COM/JANSENS520/P/8624708.HTML
# 3持久层
## 1.hibernate和mybatis有什么区别？
a)Hibernate也是持久层框架，它把EJB替代，基于HQL（面向对象编程方式）from User，一般用于中小型项目，压力负载不大。Hibernate完全面向对象，成为全ORM映射框架。它封装SQL语句，开发者无需直接接触SQL。
b)Mybatis也是持久层框架，它轻量级封装SQL，SQL比框架更加接近数据库的，性能高，mybatis执行性能高于hibernate，所有如果项目在高并发时，性能就非常好。它是半ORM映射。还需要写SQL，后半部分是面向过程，SQL执行完，会自动把数据封装到java对象中，所以前半部分是面向对象的。
## 2. Maven工程，mybatis jar支持
a)Mysql驱动
b)Mybatis 3.2.8
c)Lo4j日志
d)Cglib
## 3.开发mybatis步骤
a)创建Maven工程
b)创建全局核心配置文件，默认名称sqlmapConfig.xml（环境、事务、数据源、别名、全局设置、映射-有顺序性要求）
c)创建pojo类，序列化BasePojo，私有属性（驼峰规则userName）对应表的字段（user_name）；对象关联：对一 Door door；对多：ListorderDetails
d)创建映射文件。Mybatis最核心，也是最麻烦，也是最难理解地方
e)创建接口，接口的方法、参数、返回值和xml映射文件的每个CRUD标签对应
f)PersonMapper mapper = session.getMapper(PersonMapper.class); 
g)mapper.find(); 实现类（不是我们写的，使用jdk动态代理来自动生成。好处：动态，工作量变小）
h)mybatis底层调用selectList，还是去找到xml中的 映射作用；小作用
## 6.	resultMap关联查询3种
第一种：全写
a)	配置主键普通属性
b)	对一
c)	对多
第二种：驼峰规则
只写结构，主键最好都写上。把普通属性都可以删除
第三种：懒加载
## 7.	懒加载（延时加载）
默认的方式是立即加载。
开启：
   映射文件中要修改结构
	分别写多个sql语句，单表的查询，但是有where；主表先查询，查询完，有外键值（pojo必须有这个字段），第二次查询时，这个值就作为参数
	ResultMap中配置关联association，增加select属性，属性名称=另外SQL的标签id值
配合驼峰规则，自动映射，autoMapping=true
## 8.	构建多表查询SQL
1）	先写出两个结果集，SQL优化，尽量让每个中间结果集尽量小（where条件）。
2）	把每个结果集括起来（），给它一个别名，习惯使用表的业务的首字母 order o
3）	Left join on 条件：一个表的主键=另一个表的外键
4）	从上面构建结果集中筛选字段，注意不要有别名。
9.	在mybatis底层支持缓存
1）	一级缓存SqlSession级别（默认开启，不需开发者动）
2）	二级缓存SqlSessionFactory级别（默认不开启，开发者可以动）
3）	三级缓存（查询缓存）第三方ehache,oache，都被redis替代
# 4	Redis技术
## 1.	Redis支持的数据类型
Strings
Lists
Sets 求交集、并集
Sorted Set
hashes
## 2.	Redis的回收策略
1)	volatile-lru：从已设置过期时间的数据集（server.db[i].expires）中挑选最近最少使用的数据淘汰
2)	volatile-ttl：从已设置过期时间的数据集（server.db[i].expires）中挑选将要过期的数据淘汰
3)	volatile-random：从已设置过期时间的数据集（server.db[i].expires）中任意选择数据淘汰
4)	allkeys-lru：从数据集（server.db[i].dict）中挑选最近最少使用的数据淘汰
5)	allkeys-random：从数据集（server.db[i].dict）中任意选择数据淘汰
6)	no-enviction（驱逐）：禁止驱逐数据
# 5	Spring框架
