# Java线程：CAS浅析 - 零度的博客专栏 - CSDN博客
2017年03月28日 14:11:42[零度anngle](https://me.csdn.net/zmx729618)阅读数：296
# 前言
       在[Java](http://lib.csdn.net/base/javase)并发包中有这样一个包，java.util.concurrent.atomic，该包是对Java部分数据类型的原子封装，在原有数据类型的基础上，提供了原子性的操作方法，保证了线程安全。下面以AtomicInteger为例，来看一下是如何实现的。
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/38471987#)[copy](http://blog.csdn.net/ghsau/article/details/38471987#)
[print](http://blog.csdn.net/ghsau/article/details/38471987#)[?](http://blog.csdn.net/ghsau/article/details/38471987#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/447205/fork)
- publicfinalint incrementAndGet() {  
- for (;;) {  
- int current = get();  
- int next = current + 1;  
- if (compareAndSet(current, next))  
- return next;  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
       以这两个方法为例，incrementAndGet方法相当于原子性的++i，decrementAndGet方法相当于原子性的--i（根据[第一章](http://blog.csdn.net/ghsau/article/details/7421217)和[第二章](http://blog.csdn.net/ghsau/article/details/7424694)我们知道++i或--i不是一个原子性的操作），这两个方法中都没有使用阻塞式的方式来保证原子性（如Synchronized），那它们是如何保证原子性的呢，下面引出CAS。
# Compare And Swap
       CAS 指的是现代 CPU 广泛支持的一种对内存中的共享数据进行操作的一种特殊指令。这个指令会对内存中的共享数据做原子的读写操作。简单介绍一下这个指令的操作过程：首先，CPU 会将内存中将要被更改的数据与期望的值做比较。然后，当这两个值相等时，CPU 才会将内存中的数值替换为新的值。否则便不做操作。最后，CPU 会将旧的数值返回。这一系列的操作是原子的。它们虽然看似复杂，但却是 [Java](http://lib.csdn.net/base/java)5 
 并发机制优于原有锁机制的根本。简单来说，CAS 的含义是“我认为原有的值应该是什么，如果是，则将原有的值更新为新值，否则不做修改，并告诉我原来的值是多少”。（这段描述引自《Java并发编程实践》）
       简单的来说，CAS有3个操作数，内存值V，旧的预期值A，要修改的新值B。**当且仅当预期值A和内存值V相同时，将内存值V修改为B，否则返回V**。这是一种乐观锁的思路，它相信在它修改之前，没有其它线程去修改它；而Synchronized是一种悲观锁，它认为在它修改之前，一定会有其它线程去修改它，悲观锁效率很低。下面来看一下AtomicInteger是如何利用CAS实现原子性操作的。
## volatile变量
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/38471987#)[copy](http://blog.csdn.net/ghsau/article/details/38471987#)
[print](http://blog.csdn.net/ghsau/article/details/38471987#)[?](http://blog.csdn.net/ghsau/article/details/38471987#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/447205/fork)
- 
- privatevolatileint value; 
- 
[]()
       首先声明了一个volatile变量value，在[第二章](http://blog.csdn.net/ghsau/article/details/7424694)我们知道volatile保证了变量的内存可见性，也就是所有工作线程中同一时刻都可以得到一致的值。
## Compare And Set
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/38471987#)[copy](http://blog.csdn.net/ghsau/article/details/38471987#)
[print](http://blog.csdn.net/ghsau/article/details/38471987#)[?](http://blog.csdn.net/ghsau/article/details/38471987#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/447205/fork)
- // setup to use Unsafe.compareAndSwapInt for updates
- privatestaticfinal Unsafe unsafe = Unsafe.getUnsafe();  
- privatestaticfinallong valueOffset;// 注意是静态的
- 
- static {  
- try {  
-     valueOffset = unsafe.objectFieldOffset  
-         (AtomicInteger.class.getDeclaredField("value"));// 反射出value属性，获取其在内存中的位置
-   } catch (Exception ex) { thrownew Error(ex); }  
- }  
- 
- publicfinalboolean compareAndSet(int expect, int update) {  
- return unsafe.compareAndSwapInt(this, valueOffset, expect, update);  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
       比较并设置，这里利用Unsafe类的JNI方法实现，使用CAS指令，可以保证读-改-写是一个原子操作。compareAndSwapInt有4个参数，this - 当前AtomicInteger对象，Offset - value属性在内存中的位置(需要强调的是不是value值在内存中的位置)，expect - 预期值，update - 新值，根据上面的CAS操作过程，当内存中的value值等于expect值时，则将内存中的value值更新为update值，并返回true，否则返回false。在这里我们有必要对Unsafe有一个简单点的认识，从名字上来看，不安全，确实，这个类是用于执行低级别的、不安全操作的方法集合，这个类中的方法大部分是对内存的直接操作，所以不安全，但当我们使用反射、并发包时，都间接的用到了Unsafe。
## 循环设置
       现在在来看开篇提到的两个方法，我们拿incrementAndGet来分析一下其实现过程。
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/38471987#)[copy](http://blog.csdn.net/ghsau/article/details/38471987#)
[print](http://blog.csdn.net/ghsau/article/details/38471987#)[?](http://blog.csdn.net/ghsau/article/details/38471987#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/447205/fork)
- publicfinalint incrementAndGet() {  
- for (;;) {// 这样优于while(true)
- int current = get();// 获取当前值
- int next = current + 1;// 设置更新值
- if (compareAndSet(current, next))  
- return next;  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
       循环内，获取当前值并设置更新值，调用compareAndSet进行CAS操作，如果成功就返回更新至，否则重试到成功为止。这里可能存在一个隐患，那就是循环时间过长，总是在当前线程compareAndSet时，有另一个线程设置了value(点子太背了)，这个当然是属于小概率时间，目前Java貌似还不能处理这种情况。
# 缺点
       虽然使用CAS可以实现非阻塞式的原子性操作，但是会产生ABA问题，关于ABA问题，计划单拿出一章来整理。
