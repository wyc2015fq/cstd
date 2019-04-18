# ArrayBlockingQueue跟LinkedBlockingQueue的区别 - z69183787的专栏 - CSDN博客
2015年07月21日 15:32:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7184
1.队列中的锁的实现不同
       ArrayBlockingQueue中的锁是没有分离的，即生产和消费用的是同一个锁；
       LinkedBlockingQueue中的锁是分离的，即生产用的是putLock，消费是takeLock
2.在生产或消费时操作不同
     ArrayBlockingQueue基于数组，在生产和消费的时候，是直接将枚举对象插入或移除的，不会产生或销毁任何额外的对象实例；
     LinkedBlockingQueue基于链表，在生产和消费的时候，需要把枚举对象转换为Node<E>进行插入或移除，会生成一个额外的Node对象，这在长时间内需要高效并发地处理大批量数据的系统中，其对于GC的影响还是存在一定的区别。
3.队列大小初始化方式不同
     ArrayBlockingQueue是有界的，必须指定队列的大小；
     LinkedBlockingQueue是无界的，可以不指定队列的大小，但是默认是Integer.MAX_VALUE。当然也可以指定队列大小，从而成为有界的。
注意: 
- 在使用LinkedBlockingQueue时，若用默认大小且当生产速度大于消费速度时候，有可能会内存溢出。
- 在使用ArrayBlockingQueue和LinkedBlockingQueue分别对1000000个简单字符做入队操作时，
       LinkedBlockingQueue的消耗是ArrayBlockingQueue消耗的10倍左右，
即LinkedBlockingQueue消耗在1500毫秒左右，而ArrayBlockingQueue只需150毫秒左右。
- 按照实现原理来分析，ArrayBlockingQueue完全可以采用分离锁，从而实现生产者和消费者操作的完全并行运行。Doug
 Lea之所以没这样去做，也许是因为ArrayBlockingQueue的数据写入和获取操作已经足够轻巧，以至于引入独立的锁机制，除了给代码带来额外的复杂性外，其在性能上完全占不到任何便宜。
1 楼 dieslrae 2014-09-15  
数组的生产和消费怎么可以分离,每一次消费都涉及到数组元素的移动
2 楼 kidding87 2014-09-15  
dieslrae 写道
数组的生产和消费怎么可以分离,每一次消费都涉及到数组元素的移动
ArrayBlockingQueue take操作并没有元素的移动 
采用的是 Circularly increment 
ArrayBlockingQueue的速度是优于LinkedBlocingQeque 
他们主要的区别是在bound上 
