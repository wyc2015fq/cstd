# LinkedBlockingQueue - z69183787的专栏 - CSDN博客
2014年03月24日 20:57:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：872
LinkedBlockingQueue是一个基于已链接节点的、范围任意的blocking queue的实现。 
    此队列按 FIFO（先进先出）排序元素。队列的头部 是在队列中时间最长的元素。队列的尾部 是在队列中时间最短的元素。 
    新元素插入到队列的尾部，并且队列检索操作会获得位于队列头部的元素。链接队列的吞吐量通常要高于基于数组的队列， 
    但是在大多数并发应用程序中，其可预知的性能要低。 
    可选的容量范围构造方法参数作为防止队列过度扩展的一种方法。 
    如果未指定容量，则它等于 Integer.MAX_VALUE。除非插入节点会使队列超出容量，否则每次插入后会动态地创建链接节点。 
    1:如果未指定容量，默认容量为Integer.MAX_VALUE ，容量范围可以在构造方法参数中指定作为防止队列过度扩展。
    2:此对象是 线程阻塞-安全的   
　  3：不接受 null 元素 
    4:它实现了BlockingQueue接口。
    5:实现了 Collection 和 Iterator 接口的所有可选 方法。    
    6：在JDK5/6中，LinkedBlockingQueue和ArrayBlocingQueue等对象的poll(long timeout, TimeUnit unit)存在内存泄露Leak的对象AbstractQueuedSynchronizer.Node，据称JDK5会在Update12里Fix，JDK6会在Update2里Fix
　　下面介绍几种常用的方法：
      定义一个输出方法：
　　static void v(Object s){System.out.println(s.toString());}
　　LinkedBlockingQueue<String>  bq=new LinkedBlockingQueue<String>();
　　  for(int i=0;i<100;i++)
  　　{
　　　　   bq.add("i"+i);//如果空间已满，此方法会抛出异常，所以这就是put，或者offer方法的优势所在
　　　}
        String s1=bq.take();//i0
　　  String s2=bq.take();//i1
        bq.offer("ix",5,TimeUnit.SECONDS);//在尾部插入一个元素，如果有必要    ，等待 指定的时间，使得队列变得可用。返回boolean值  表示是否插入成功。
        bq.put("ixx");//将指定的元素添加到队列的尾部，如有必要，则等待空间变得可用，如果空间满了，则会一直等到空间可用时，进行插入。
　　　bq.poll();//`[poll]()()` //`[poll]()(long timeout, [TimeUnit]() unit)` 
 检索并移除此队列的头，如果此队列为空，则返回 null。
　　　bq.`[clear]()()` 从队列彻底移除所有元素。
         bq.`[peek]()()`检索，但是不移除此队列的头，如果此队列为空，则返回 null。
//区别一下几种方法：
1.　offer(E e) offer(E e,long timeout,TimeUnit unit)
　　和
　　put(E e)
都是想队列中插入元素，如果使用offer，则当队列可用或者等待指定时间后队列可用时，才能将元素插入成功。
如果使用put(E e)，则会一直等待队列可用时插入元素。因为此队列是线程阻塞的，所以会存在被其他线程锁住，不可使用的时期段
此队列的容量 要么为指定的固定容量，不指定，容量则为[`Integer.MAX_VALUE`]().
2.poll() poll(long timeout,TimeUnit unit)和peek();
　　poll方法是 立刻或者等待指定时间后，获取并且移除队列的头。如果队列为空，则为null
　　peek方法是  获取但不移除此队列的头，如果此队列为空，则为null
