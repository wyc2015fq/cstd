# Java 并发专题 ：闭锁 CountDownLatch 之一家人一起吃个饭 - z69183787的专栏 - CSDN博客
2015年10月23日 11:57:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：794
最近一直整并发这块东西，顺便写点Java并发的例子，给大家做个分享，也强化下自己记忆。
每天起早贪黑的上班，父母每天也要上班，话说今天定了个饭店，一家人一起吃个饭，通知大家下班去饭店集合。假设：3个人在不同的地方上班，必须等到3个人到场才能吃饭，用程序如何实现呢？
作为一名资深屌丝程序猿，开始写代码实现：
**[java]**[view
 plain](http://blog.csdn.net/lmj623565791/article/details/26626391#)[copy](http://blog.csdn.net/lmj623565791/article/details/26626391#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/359183/fork)
- package com.zhy.concurrency.latch;  
- 
- publicclass Test1  
- {  
- /**
-      * 模拟爸爸去饭店
-      */
- publicstaticvoid fatherToRes()  
-     {  
-         System.out.println("爸爸步行去饭店需要3小时。");  
-     }  
- 
- /**
-      * 模拟我去饭店
-      */
- publicstaticvoid motherToRes()  
-     {  
-         System.out.println("妈妈挤公交去饭店需要2小时。");  
-     }  
- 
- /**
-      * 模拟妈妈去饭店
-      */
- publicstaticvoid meToRes()  
-     {  
-         System.out.println("我乘地铁去饭店需要1小时。");  
-     }  
- 
- /**
-      * 模拟一家人到齐了
-      */
- publicstaticvoid togetherToEat()  
-     {  
-         System.out.println("一家人到齐了，开始吃饭");  
-     }  
- 
- publicstaticvoid main(String[] args)  
-     {  
-         fatherToRes();  
-         motherToRes();  
-         meToRes();  
-         togetherToEat();  
-     }  
- }  
输出结果：
**[java]**[view
 plain](http://blog.csdn.net/lmj623565791/article/details/26626391#)[copy](http://blog.csdn.net/lmj623565791/article/details/26626391#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/359183/fork)
- 爸爸步行去饭店需要3小时。  
- 妈妈挤公交去饭店需要2小时。  
- 我乘地铁去饭店需要1小时。  
- 一家人到齐了，开始吃饭  
看似实现了，但是吃个饭，光汇合花了6个小时，第一个到的等了3个小时；话说回来，大家下班同时往饭店聚集，怎么也是个并行的过程，于是不用我说，大家肯定都行想到使用多线程，于是作为一名资深屌丝程序猿，开始改造我们的代码：
**[java]**[view
 plain](http://blog.csdn.net/lmj623565791/article/details/26626391#)[copy](http://blog.csdn.net/lmj623565791/article/details/26626391#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/359183/fork)
- publicstaticvoid main(String[] args)  
-     {  
- new Thread()  
-         {  
- publicvoid run()  
-             {  
-                 fatherToRes();  
-             };  
-         }.start();  
- new Thread()  
-         {  
- publicvoid run()  
-             {  
-                 motherToRes();  
-             };  
-         }.start();  
- new Thread()  
-         {  
- publicvoid run()  
-             {  
-                 meToRes();  
-             };  
-         }.start();  
- 
-         togetherToEat();  
-     }  
直接启动了3个线程，但是运行结果貌似也不对：
**[java]**[view
 plain](http://blog.csdn.net/lmj623565791/article/details/26626391#)[copy](http://blog.csdn.net/lmj623565791/article/details/26626391#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/359183/fork)
- 一家人到齐了，开始吃饭  
- 我乘地铁去饭店需要1小时。  
- 妈妈挤公交去饭店需要2小时。  
- 爸爸步行去饭店需要3小时。  
一个都没到，就开始吃饭了，，，（为了更好的显示，我在每个方法中休息了一段时间，模拟到达饭店的过程）。还是不行，那就继续完善：
**[java]**[view
 plain](http://blog.csdn.net/lmj623565791/article/details/26626391#)[copy](http://blog.csdn.net/lmj623565791/article/details/26626391#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/359183/fork)
- privatestaticvolatileint i = 3;  
- 
- publicstaticvoid main(String[] args)  
-     {  
- 
- new Thread()  
-         {  
- publicvoid run()  
-             {  
-                 fatherToRes();  
-                 i--;  
-             };  
-         }.start();  
- new Thread()  
-         {  
- publicvoid run()  
-             {  
-                 motherToRes();  
-                 i--;  
-             };  
-         }.start();  
- new Thread()  
-         {  
- publicvoid run()  
-             {  
-                 meToRes();  
-                 i--;  
-             };  
-         }.start();  
- 
- while (i != 0);  
-         togetherToEat();  
-     }  
我们定义了一个volatile修饰的int类型变量，初始值为3，当为0时代表一家人齐了，于是我们在主线程使用了一个忙等，一直等待所有人到达，这次效果看起来不错哦：
**[java]**[view
 plain](http://blog.csdn.net/lmj623565791/article/details/26626391#)[copy](http://blog.csdn.net/lmj623565791/article/details/26626391#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/359183/fork)
- 我乘地铁去饭店需要1小时。  
- 妈妈挤公交去饭店需要2小时。  
- 爸爸步行去饭店需要3小时。  
- 一家人到齐了，开始吃饭  
但是，忙等这样的代码对于CPU的消耗太巨大了，我们需要更好的实现方式。顺便说一下volatile，为什么我们用volatile修饰 i 呢， 因为当多个线程操作同一个变量时，为了保证变量修改对于其他线程的可见性，必须使用同步，volatile对于可见性的实现是个不错的选择，但是我们代码中的 i -- 也有可能因为并发造成一定的问题，毕竟i--不是原子操作，正常最好使用同步块或者AtomicLong.decrementAndGet()实现--。
说了这么多，标题上的CountLatchDown竟然没出现，所以最终版，必须让这哥们出来亮相了：
**[java]**[view
 plain](http://blog.csdn.net/lmj623565791/article/details/26626391#)[copy](http://blog.csdn.net/lmj623565791/article/details/26626391#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/359183/fork)
- privatestatic CountDownLatch latch = new CountDownLatch(3);  
- 
- publicstaticvoid main(String[] args) throws InterruptedException  
-     {  
- 
- new Thread()  
-         {  
- publicvoid run()  
-             {  
-                 fatherToRes();  
-                 latch.countDown();  
-             };  
-         }.start();  
- new Thread()  
-         {  
- publicvoid run()  
-             {  
-                 motherToRes();  
-                 latch.countDown();  
-             };  
-         }.start();  
- new Thread()  
-         {  
- publicvoid run()  
-             {  
-                 meToRes();  
-                 latch.countDown();  
-             };  
-         }.start();  
- 
-         latch.await();  
-         togetherToEat();  
-     }  
输出结果：
**[java]**[view
 plain](http://blog.csdn.net/lmj623565791/article/details/26626391#)[copy](http://blog.csdn.net/lmj623565791/article/details/26626391#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/359183/fork)
- 我乘地铁去饭店需要1小时。  
- 妈妈挤公交去饭店需要2小时。  
- 爸爸步行去饭店需要3小时。  
- 一家人到齐了，开始吃饭  
避免使用忙等，我们使用了CountDowmLatch实现了我们的需求。下面具体介绍一下这个哥们：
Latch闭锁的意思，是一种同步的工具类。类似于一扇门：在闭锁到达结束状态之前，这扇门一直是关闭着的，不允许任何线程通过，当到达结束状态时，这扇门会打开并允许所有的线程通过。且当门打开了，就永远保持打开状态。
作用：可以用来确保某些活动直到其他活动都完成后才继续执行。
使用场景：
1、例如我们上例中所有人都到达饭店然后吃饭；
2、某个操作需要的资源初始化完毕
3、某个服务依赖的线程全部开启等等...
CountDowmLatch是一种灵活的闭锁实现，包含一个计数器，该计算器初始化为一个正数，表示需要等待事件的数量。countDown方法递减计数器，表示有一个事件发生，而await方法等待计数器到达0，表示所有需要等待的事情都已经完成。
