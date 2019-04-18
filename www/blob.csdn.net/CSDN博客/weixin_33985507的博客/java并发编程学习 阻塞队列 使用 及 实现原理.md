# java并发编程学习: 阻塞队列 使用 及 实现原理 - weixin_33985507的博客 - CSDN博客
2015年10月08日 16:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
队列(Queue)与栈(Stack)是数据结构中的二种常用结构，队列的特点是先进先出(First In First Out)，而Stack是先进后出(First In Last Out)，说得通俗点：Queue就是电影院入场时人们排起来的进场队伍，先来的人（即：前排在前面的人）先入场，而Statck则是一队人依次进入了一个死胡同想出来，先进去(最里面)的人，必须等后面的人(后进入的人)出来了，自己才能出来。
队列在多线程应用中，常用于生产-消费场景，打个通俗的比方：很多人早上喜欢去买油条，买油条的人相当于消费者，做油条的师傅则是生产者。而油锅边上用于放油条的铁架子，可以看成一个共享的队列，师傅做好油条后，一根一根的捞出来放在架子上，而顾客则按排队的顺序一根根的付好钱从架子上拿。 即：队列的一头，不断有人在放入东西（生产元素），另一头不断有人的消费(拿走元素)。这里就有一个很有趣的现象，如果买的人多，师傅来不及做，那么第一个顾客就会一直等着（后面的所有人也得等着，或称为阻塞了后面的人），直到师傅炸好一根，然后第一个顾客买完走了，后面的人才能顶上来，类似的道理，如果架子放满了，没有人来买，师傅就会停下来，等有人来买了，才会继续做，这就是所谓的队列阻塞，而能产生阻塞行为的队列称为阻塞队列。
从刚才的描述可以看出，发生阻塞起码得满足下面至少一个条件： (前提：队列是有界的)
1.从队列里取元素时，如果队列为空，则代码一直等在这里（即阻塞），直到队列里有东西了，拿到元素了，后面的代码才能继续
2.向队列里放元素时，如果队列满了（即放不下更多元素），则代码也会卡住，直到队列里的东西被取走了（即：有空位可以放新元素了），后面的代码才能继续
JDK7提供了以下7个阻塞队列：
ArrayBlockingQueue ：由数组结构组成的有界阻塞队列。
LinkedBlockingQueue ：由链表结构组成的有界阻塞队列。
PriorityBlockingQueue ：支持优先级排序的无界阻塞队列。
DelayQueue：使用优先级队列实现的无界阻塞队列。
SynchronousQueue：不存储元素的阻塞队列。
LinkedTransferQueue：链表结构组成的无界阻塞队列。
LinkedBlockingDeque：链表结构组成的双向阻塞队列。
阻塞队列提供了下列四种处理方法:
|方法\处理方式|抛出异常|返回true/false|一直阻塞|超时退出|
|----|----|----|----|----|
|插入方法|add(e)|offer(e)|put(e)|offer(e,time,unit)|
|移除方法|remove()|poll()|take()|poll(time,unit)|
|检查方法|element()|peek()|||
这4类方法中，在队列已满（或为空）的情况下，有些会抛出异常，有些则返回true/false，有些则一直阻塞，还有些则可以设置超时时间，时间到了后，自动退出阻塞状态，实际项目中可根据需要选取适合的方法。
下面是一个基本示例：
模拟了买油条的场景，1个老板在做油条，3个顾客在排队买
```java
package yjmyzz.test;
import java.util.concurrent.ArrayBlockingQueue;
public class BlockingQueueTest {
    private static final int queueSize = 3;
    private static final ArrayBlockingQueue<String> queue = new ArrayBlockingQueue<String>(queueSize);
    private static final int produceSpeed = 2000;//生产速度(越小越快)
    private static final int consumeSpeed = 10;//消费速度(越小越快)
    public static void main(String[] args) {
        Thread producer = new Producer();
        Thread consumer = new Consumer();
        producer.start();
        consumer.start();
    }
    static class Producer extends Thread {
        public void run() {
            while (true) {
                try {
                    System.out.println("老板准备炸油条了，架子上还能放：" + (queueSize - queue.size()) + "根油条");
                    queue.put("1根油条");
                    System.out.println("老板炸好了1根油条，架子上还能放：" + (queueSize - queue.size()) + "根油条");
                    Thread.sleep(produceSpeed);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }
    static class Consumer extends Thread {
        public void run() {
            while (true) {
                try {
                    System.out.println("A 准备买油条了，架子上还剩" + queue.size() + "根油条");
                    queue.take();
                    System.out.println("A 买到1根油条，架子上还剩" + queue.size() + "根油条");
                    Thread.sleep(consumeSpeed);
                    System.out.println("B 准备买油条了，架子上还剩" + queue.size() + "根油条");
                    queue.take();
                    System.out.println("B 买到1根油条，架子上还剩" + queue.size() + "根油条");
                    Thread.sleep(consumeSpeed);
                    System.out.println("C 准备买油条了，架子上还剩" + queue.size() + "根油条");
                    queue.take();
                    System.out.println("C 买到1根油条，架子上还剩" + queue.size() + "根油条");
                    Thread.sleep(consumeSpeed);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
```
输出：(只取了前几行)
```
1 老板准备炸油条了，架子上还能放：3根油条
 2 老板炸好了1根油条，架子上还能放：2根油条
 3 A 准备买油条了，架子上还剩1根油条
 4 A 买到1根油条，架子上还剩0根油条
 5 B 准备买油条了，架子上还剩0根油条
 6 老板准备炸油条了，架子上还能放：3根油条
 7 老板炸好了1根油条，架子上还能放：2根油条
 8 B 买到1根油条，架子上还剩0根油条
 9 C 准备买油条了，架子上还剩0根油条
10 ...
```
观察5-8行，因为消费的速度远大于生产速度，即：生意太好，老板来不及做。B要买油条时，发现卖完了，只能等老板再做一根出来，后面的C才有机会继续购买。
这是一个很有意思的程序，可以把生产速度与消费速度值对换，输出结果如下：
```
1 老板准备炸油条了，架子上还能放：3根油条
 2 老板炸好了1根油条，架子上还能放：2根油条
 3 A 准备买油条了，架子上还剩1根油条
 4 A 买到1根油条，架子上还剩0根油条
 5 老板准备炸油条了，架子上还能放：3根油条
 6 老板炸好了1根油条，架子上还能放：2根油条
 7 老板准备炸油条了，架子上还能放：2根油条
 8 老板炸好了1根油条，架子上还能放：1根油条
 9 老板准备炸油条了，架子上还能放：1根油条
10 老板炸好了1根油条，架子上还能放：0根油条
11 老板准备炸油条了，架子上还能放：0根油条
12 B 准备买油条了，架子上还剩3根油条
13 B 买到1根油条，架子上还剩2根油条
14 老板炸好了1根油条，架子上还能放：0根油条
15 老板准备炸油条了，架子上还能放：0根油条
16 C 准备买油条了，架子上还剩3根油条
17 C 买到1根油条，架子上还剩2根油条
18 老板炸好了1根油条，架子上还能放：0根油条
19 老板准备炸油条了，架子上还能放：0根油条
20 A 准备买油条了，架子上还剩3根油条
21 A 买到1根油条，架子上还剩2根油条
22 老板炸好了1根油条，架子上还能放：0根油条
23 老板准备炸油条了，架子上还能放：0根油条
```
因为生产速度远大于消费速度，即：生意不好，油条做得比卖得快。观察一下5-11行，老板卯足了劲做，但是这时一直没人来买。然后12-13行，终于来了一个客户B买了一根，然后老板又要开始做（14-15行），发现架子上放满了，不得不停下，等C再买一根（16-17行），才能继续做(18行)
实现原理：
[聊聊并发（七）——Java中的阻塞队列](http://ifeve.com/java-blocking-queue/) 一文中已经对ArrayBlockingQueue的源码进行比较详细的分析了，这里只贴几段主要的代码，体会一下思想：
```java
/** Main lock guarding all access */
    final ReentrantLock lock;
    /** Condition for waiting takes */
    private final Condition notEmpty;
    /** Condition for waiting puts */
    private final Condition notFull;
```
这3个变量很重要，ReentrantLock重入锁，notEmpty检查不为空的Condition 以及  notFull用来检查队列未满的Condition
Condition是一个接口，里面有二个重要的方法：
await() ： Causes the current thread to wait until it is signalled or interrupted. 即阻塞当前线程，直到被通知（唤醒）或中断
singal(): Wakes up one waiting thread. 唤醒阻塞的线程
再来看put方法：(jdk 1.8)
```java
public void put(E e) throws InterruptedException {
        checkNotNull(e);
        final ReentrantLock lock = this.lock;
        lock.lockInterruptibly();
        try {
            while (count == items.length)
                notFull.await();
            enqueue(e);
        } finally {
            lock.unlock();
        }
    }
```
1.先获取锁
2.然后用while循环检测元素个数是否等于items长度，如果相等，表示队列满了,调用notFull的await()方法阻塞线程
3.否则调用enqueue()方法添加元素
4.最后解锁
```java
private void enqueue(E x) {
        // assert lock.getHoldCount() == 1;
        // assert items[putIndex] == null;
        final Object[] items = this.items;
        items[putIndex] = x;
        if (++putIndex == items.length)
            putIndex = 0;
        count++;
        notEmpty.signal();
    }
```
这是添加元素的代码（jdk 1.8）,注意最后一行notEmpty.signal()方法，表示添加完元素后，调用singal()通知等待（从队列中取元素）的线程，队列不空(有值)啦，可以来取东西了。
类似的take()与dequeue()方法则相当于逆过程（注：同样都是jdk 1.8）
```java
public E take() throws InterruptedException {
        final ReentrantLock lock = this.lock;
        lock.lockInterruptibly();
        try {
            while (count == 0)
                notEmpty.await();
            return dequeue();
        } finally {
            lock.unlock();
        }
    }
```
类似的：
1. 先加锁
2. 如果元素个数为空，表示队列已空，调用notEmpty的await()阻塞线程，直接队列里又有新元素加入为止
3. 然后调用dequeue 从队列里删除元素
4. 解锁
dequeue方法：
```java
private E dequeue() {
        // assert lock.getHoldCount() == 1;
        // assert items[takeIndex] != null;
        final Object[] items = this.items;
        @SuppressWarnings("unchecked")
        E x = (E) items[takeIndex];
        items[takeIndex] = null;
        if (++takeIndex == items.length)
            takeIndex = 0;
        count--;
        if (itrs != null)
            itrs.elementDequeued();
        notFull.signal();
        return x;
    }
```
倒数第2行，元素移除后，调用notFull.singnal唤醒等待（向队列添加元素的）线程，队列有空位了，可以向里面添加元素了。 
参考文章：
[http://ifeve.com/java-blocking-queue/](http://ifeve.com/java-blocking-queue/)
[http://www.cnblogs.com/dolphin0520/p/3932906.html](http://www.cnblogs.com/dolphin0520/p/3932906.html)
