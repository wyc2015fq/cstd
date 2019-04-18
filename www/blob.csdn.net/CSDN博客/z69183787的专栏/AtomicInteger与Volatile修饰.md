# AtomicInteger与Volatile修饰 - z69183787的专栏 - CSDN博客
2015年09月04日 21:59:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1057
今天写代码，尝试使用了AtomicInteger这个类，感觉使用起来很爽，特别适用于高并发访问，下面贴一个简单的例子：
Java代码  ![收藏代码](http://haininghacker-foxmail-com.iteye.com/images/icon_star.png)
- CashierContext类部分代码:  
- 
- private Map<String, AtomicInteger> counter          = new HashMap<String, AtomicInteger>();  
- 
- 
- privatevoid initCounter() {  
-      counter.put("cvm", new AtomicInteger(0));  
- }  
- 
- //被调用一次自动+1
- 
- public MobileCashierViewModel getCvm() {  
- 
- if (cvm != null) {  
-             counter.get("cvm").incrementAndGet();  
-         }  
- return cvm;  
-     }  
使用场景：
因为通过WS服务获取MobileCashierViewModel 这个对象比较频繁，会很影响系统资源，可以将cvm存入缓存中，想要查看缓存cvm有多大价值，那么可以设置一个计数，来统记cvm被调用的次数
然后将CashierContext放入ThreadLocal中，然后再写一个过滤器，在过滤器里面可以得到获取这个服务从缓存中取的次数，这个就可以很容易看出来缓存价值。
那么为什么不使用记数器自加呢，例如count++这样的，因为这种计数是线程不安全的，高并发访问时统计会有误，而AtomicInteger为什么能够达到多而不乱，处理高并发应付自如呢，我们才看看AtomicInteger的源代码：
Java代码  ![收藏代码](http://haininghacker-foxmail-com.iteye.com/images/icon_star.png)
- privatevolatileint value;  
大家可以看到有这个变量，value就是你设置的自加起始值。注意看它的访问控制符，是volatile，这个就是保证AtomicInteger线程安全的根源，熟悉并发的同学一定知道在java中处理并发主要有两种方式：
1，synchronized关键字，这个大家应当都各种面试和笔试中经常遇到。
2，volatile修饰符的使用，相信这个修饰符大家平时在项目中使用的也不是很多。
这里重点说一下volatile:
Volatile修饰的成员变量在每次被线程访问时，都强迫从共享内存重新读取该成员的值，而且，当成员变量值发生变化时，强迫将变化的值重新写入共享内存，这样两个不同的线程在访问同一个共享变量的值时，始终看到的是同一个值。
java语言规范指出：为了获取最佳的运行速度，允许线程保留共享变量的副本，当这个线程进入或者离开同步代码块时，才与共享成员变量进行比对，如果有变化再更新共享成员变量。这样当多个线程同时访问一个共享变量时，可能会存在值不同步的现象。
而volatile这个值的作用就是告诉VM：对于这个成员变量不能保存它的副本，要直接与共享成员变量交互。
建议：当多个线程同时访问一个共享变量时，可以使用volatile，而当访问的变量已在synchronized代码块中时，不必使用。
缺点：使用volatile将使得VM优化失去作用，导致效率较低，所以要在必要的时候使用。
