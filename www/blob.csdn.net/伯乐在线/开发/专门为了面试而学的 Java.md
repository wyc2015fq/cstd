# 专门为了面试而学的 Java - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [顾星竹](http://www.jobbole.com/members/harjoogoo) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
# 1. hashmap hashtable
HashMap 是一个散列表，它存储的内容是键值对(key-value)映射。
HashMap 继承于AbstractMap，实现了Map、Cloneable、java.io.Serializable接口。
HashMap 的实现不是同步的，这意味着它不是线程安全的。它的key、value都可以为null。此外，HashMap中的映射不是有序的。
HashMap 的实例有两个参数影响其性能：“初始容量” 和 “加载因子”。容量 是哈希表中桶的数量，初始容量 只是哈希表在创建时的容量。加载因子 是哈希表在其容量自动增加之前可以达到多满的一种尺度。当哈希表中的条目数超出了加载因子与当前容量的乘积时，则要对该哈希表进行 rehash 操作（即重建内部数据结构），从而哈希表将具有大约两倍的桶数。
![](http://jbcdn2.b0.upaiyun.com/2016/08/12925041b736192bf2a40b31a550b5d5.jpg)
*继承关系*
hashmap底层就是一个数组，然后数组里每个元素装了个链表。
这个数组元素称为bucket桶
![](http://jbcdn2.b0.upaiyun.com/2016/08/1c25ff9f195ec80bdbdeda01510962ce.jpg)
*图例*
HashTable和HashMap区别
第一，继承不同。

Java
```
public class Hashtable extends Dictionary implements Map
public class HashMap  extends AbstractMap implements Map
```
第二，Hashtable 中的方法是同步的，而HashMap中的方法在缺省情况下是非同步的。在多线程并发的环境下，可以直接使用Hashtable，但是要使用HashMap的话就要自己增加同步处理了，对HashMap的同步处理可以使用Collections类提供的synchronizedMap静态方法，或者直接使用JDK 5.0之后提供的java.util.concurrent包里的ConcurrentHashMap类。
第三，Hashtable中，key和value都不允许出现null值。在HashMap中，null可以作为键，这样的键只有一个；可以有一个或多个键所对应的值为null。当get()方法返回null值时，即可以表示 HashMap中没有该键，也可以表示该键所对应的值为null。因此，在HashMap中不能由get()方法来判断HashMap中是否存在某个键，而应该用containsKey()方法来判断。
第四，两个遍历方式的内部实现上不同。
Hashtable、HashMap都使用了 Iterator。而由于历史原因，Hashtable还使用了Enumeration的方式 。
第五，哈希值的使用不同，HashTable直接使用对象的hashCode。而HashMap重新计算hash值（ indexFor ）。
第六，Hashtable和HashMap它们两个内部实现方式的数组的初始大小和扩容的方式。HashTable中hash数组默认大小是11，增加的方式是 old*2+1。HashMap中hash数组的默认大小是16，而且一定是2的指数。
# 2. 为什么用枚举实现的单例是最好的方式
- 枚举写法简单

Java
```
public enum Singleton{
  INSTANCE；
}
```
- 枚举自己处理序列化
在序列化的时候Java仅仅是将枚举对象的name属性输出到结果中，反序列化的时候则是通过java.lang.Enum的valueOf方法来根据名字查找枚举对象。
# 3. jvm 内存模型
![](http://img.blog.csdn.net/20131231175136859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2luZ29md29ybGQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
*内存模型架构图*
- 程序计数器
每个线程有要有一个独立的程序计数器，记录下一条要运行的指令。线程私有的内存区域。如果执行的是JAVA方法，计数器记录正在执行的java字节码地址，如果执行的是native方法，则计数器为空。
- 虚拟机栈
线程私有的，与线程在同一时间创建。管理JAVA方法执行的内存模型。
- 本地方法区
和虚拟机栈功能相似，但管理的不是JAVA方法，是本地方法
- 方法区
线程共享的，用于存放被虚拟机加载的类的元数据信息：如常量、静态变量、即时编译器编译后的代码。也称为永久代。
- JAVA 堆
线程共享的，存放所有对象实例和数组。垃圾回收的主要区域。可以分为新生代和老年代(tenured)。
# 4. jdk 线程池


```
package cn.gaialine.threadpool;  
import java.util.concurrent.ArrayBlockingQueue;  
import java.util.concurrent.BlockingQueue;  
import java.util.concurrent.ThreadPoolExecutor;  
import java.util.concurrent.ThreadPoolExecutor.AbortPolicy;  
import java.util.concurrent.TimeUnit;  
/** 
 * 线程池测试用例 
 * @author yangyong 
 * 
 */  
public class TestThreadPool {  
    //线程池维护线程的最少数量  
    private static final int COREPOOLSIZE = 2;  
    //线程池维护线程的最大数量  
    private static final int MAXINUMPOOLSIZE = 5;  
    //线程池维护线程所允许的空闲时间  
    private static final long KEEPALIVETIME = 4;  
    //线程池维护线程所允许的空闲时间的单位  
    private static final TimeUnit UNIT = TimeUnit.SECONDS;  
    //线程池所使用的缓冲队列,这里队列大小为3  
    private static final BlockingQueue<Runnable> WORKQUEUE = new ArrayBlockingQueue<Runnable>(3);  
    //线程池对拒绝任务的处理策略：AbortPolicy为抛出异常；CallerRunsPolicy为重试添加当前的任务，他会自动重复调用execute()方法；DiscardOldestPolicy为抛弃旧的任务，DiscardPolicy为抛弃当前的任务  
    private static final AbortPolicy HANDLER = new ThreadPoolExecutor.AbortPolicy();  
    public static void main(String[] args) {  
        // TODO 初始化线程池  
        ThreadPoolExecutor threadPool = new ThreadPoolExecutor(COREPOOLSIZE, MAXINUMPOOLSIZE, KEEPALIVETIME, UNIT, WORKQUEUE, HANDLER);  
        for (int i = 1; i < 11; i++) {  
            String task = "task@"+i;  
            System.out.println("put->"+task);  
            //一个任务通过 execute(Runnable)方法被添加到线程池，任务就是一个 Runnable类型的对象，任务的执行方法就是 Runnable类型对象的run()方法  
            //处理任务的优先级为：核心线程corePoolSize、任务队列workQueue、最大线程maximumPoolSize，如果三者都满了，使用handler处理被拒绝的任务  
            //设此时线程池中的数量为currentPoolSize,若currentPoolSize>corePoolSize,则创建新的线程执行被添加的任务,  
            //当corePoolSize+workQueue>currentPoolSize>=corePoolSize,新增任务被放入缓冲队列,  
            //当maximumPoolSize>currentPoolSize>=corePoolSize+workQueue,建新线程来处理被添加的任务,  
            //当currentPoolSize>=maximumPoolSize,通过 handler所指定的策略来处理新添加的任务  
            //本例中可以同时可以被处理的任务最多为maximumPoolSize+WORKQUEUE=8个，其中最多5个在线程中正在处理，3个在缓冲队列中等待被处理  
            //当currentPoolSize>corePoolSize时，如果某线程空闲时间超过keepAliveTime，线程将被终止。这样，线程池可以动态的调整池中的线程数  
            threadPool.execute(new ThreadPoolTask(task));  
            try {  
                Thread.sleep(1000);  
            } catch (InterruptedException e) {  
                // TODO Auto-generated catch block  
                e.printStackTrace();  
            }  
        }  
        threadPool.shutdown();//关闭主线程，但线程池会继续运行，直到所有任务执行完才会停止。若不调用该方法线程池会一直保持下去，以便随时添加新的任务  
    }  
}
```


```
package cn.gaialine.threadpool;  
import java.io.Serializable;  
/** 
 * 任务task 
 * @author yangyong 
 * 
 */  
public class ThreadPoolTask implements Runnable,Serializable{  
    private static final long serialVersionUID = -8568367025140842876L;  
    private Object threadPoolTaskData;  
    private static int produceTaskSleepTime = 10000;  
    public ThreadPoolTask(Object threadPoolTaskData) {  
        super();  
        this.threadPoolTaskData = threadPoolTaskData;  
    }  
    public void run() {  
        // TODO Auto-generated method stub  
        System.out.println("start..."+threadPoolTaskData);  
        try {  
            //模拟线程正在执行任务  
            Thread.sleep(produceTaskSleepTime);  
        } catch (InterruptedException e) {  
            // TODO Auto-generated catch block  
            e.printStackTrace();  
        }  
        System.out.println("stop..."+threadPoolTaskData);  
        threadPoolTaskData = null;  
    }  
    public Object getTask(){  
        return this.threadPoolTaskData;  
    }  
//---------------
put->task@1  
start...task@1  
put->task@2  
start...task@2  
put->task@3  
put->task@4  
put->task@5  
put->task@6  
start...task@6  
put->task@7  
start...task@7  
put->task@8  
start...task@8  
put->task@9  
Exception in thread "main" java.util.concurrent.RejectedExecutionException  
    at java.util.concurrent.ThreadPoolExecutor$AbortPolicy.rejectedExecution(Unknown Source)  
    at java.util.concurrent.ThreadPoolExecutor.reject(Unknown Source)  
    at java.util.concurrent.ThreadPoolExecutor.execute(Unknown Source)  
    at cn.gaialine.threadpool.TestThreadPool.main(TestThreadPool.java:42)  
stop...task@1  
start...task@3  
stop...task@2  
start...task@4  
stop...task@6  
start...task@5  
stop...task@7  
stop...task@8  
stop...task@3  
stop...task@4  
stop...task@5
```
从中可以看出task1和task2依次最先执行，这时候currentPoolSize=2达到了corePoolSize，task3、task4、task5被送入缓冲队列，达到了workQueue最大值3，task6、task7、task8开启新的线程开始执行，此时currentPoolSize=5达到了maximumPoolSize，task9、task10根据AbortPolicy策略抛出异常，不再执行task9和task10。10秒钟后task1、task2….依次执行完毕释放线程，开始执行队列里的task3、task4、task5，最后task3、4、5执行完毕，所有任务完成。
JDK根据ThreadPoolExecutor配置好的线程池


```
// 固定工作线程数量的线程池  
ExecutorService executorService1 = Executors.newFixedThreadPool(3);  
// 一个可缓存的线程池  
ExecutorService executorService2 = Executors.newCachedThreadPool();  
// 单线程化的Executor  
ExecutorService executorService3 = Executors.newSingleThreadExecutor();  
// 支持定时的以及周期性的任务执行  
ExecutorService executorService4 = Executors.newScheduledThreadPool(3);
```
# 5. Java 内存模型
内存模型描述了程序中各个变量（实例域、静态域和数组元素）之间的关系，以及在实际计算机系统中将变量存储到内存和从内存中取出变量这样的底层细节。
JVM中存在一个主存区（Main Memory或Java Heap Memory），Java中所有变量都是存在主存中的，对于所有线程进行共享，而每个线程又存在自己的工作内存（Working Memory），工作内存中保存的是主存中某些变量的拷贝，线程对所有变量的操作并非发生在主存区，而是发生在工作内存中，而线程之间是不能直接相互访问，变量在程序中的传递，是依赖主存来完成的。
JMM的最初目的，就是为了能够支持多线程程序设计的，每个线程可以认为是和其他线程不同的CPU上运行，或者对于多处理器的机器而言，该模型需要实现的就是使得每一个线程就像运行在不同的机器、不同的CPU或者本身就不同的线程上一样。
# 6. volatile 与 synchronized
在Java中,为了保证多线程读写数据时保证数据的一致性,可以采用两种方式:
- 同步
如用synchronized关键字,或者使用锁对象.
- volatile
使用volatile关键字
用一句话概括volatile,它能够使变量在值发生改变时能尽快地让其他线程知道.
volatile本质是在告诉jvm当前变量在寄存器中的值是不确定的,需要从主存中读取,synchronized则是锁定当前变量,只有当前线程可以访问该变量,其他线程被阻塞住.
volatile仅能使用在变量级别,synchronized则可以使用在变量,方法.
volatile仅能实现变量的修改可见性,但不具备原子特性,而synchronized则可以保证变量的修改可见性和原子性.
volatile不会造成线程的阻塞,而synchronized可能会造成线程的阻塞.
volatile标记的变量不会被编译器优化,而synchronized标记的变量可以被编译器优化.
# 7. 面试中遇到的问题
## 求一个无序数组的中位数，白板写 code
因为时间有限，没有多想，直接使用冒泡排序冒一半的数据，另一半保持无序。如果有更好的方法请告诉我谢谢！
## 数据库表的行转列
使用case when就可以实现了，但是要注意需要对每个case when做max，以及最后的group by，这样可以去除null值。
## 使用map ＝ new hashmap 比 hashmap ＝ new hashmap 的好处
一时间没想明白，因为觉得不会有人写成hashmap ＝ new hashmap，就答了一下实现依赖抽象，方便复用和修改，减少栈存储之类乱七八糟的东西。。。如果有更好的答案也请告诉我谢谢😢
## == 和 equals 的区别
Object类中的equals方法和“==”是一样的，没有区别，而String类，Integer类等等一些类，是重写了equals方法，才使得equals和“==不同”，所以，当自己创建类时，自动继承了Object的equals方法，要想实现不同的等于比较，必须重写equals方法。
“==”比”equal”运行速度快,因为”==”只是比较引用.
## hashcode 和 equals 的具体实现方式
这真是个高频问题，从大四找实习面试到研究生找工作面试到两次跳槽一共几十场技术面试几乎全都问到了。唯一一次没问到的一次是因为我面的是算法工程师的职位，那次都在面机器学习的算法和实现……
默认 equals 方法直接调用了 ==


```
public boolean equals(Object obj) {
    return (this == obj);
}
```
String 改写了 equals


```
public boolean equals(Object anObject) {
         if (this == anObject) {
             return true;
         }
         if (anObject instanceof String) {
             String anotherString = (String) anObject;
             int n = value.length;
             if (n == anotherString.value.length) {
                 char v1[] = value;
                 char v2[] = anotherString.value;
                 int i = 0;
                 while (n-- != 0) {
                     if (v1[i] != v2[i])
                             return false;
                     i++;
                 }
                 return true;
             }
         }
         return false;
     }
```
hashCode是根类Obeject中的方法。默认情况下，Object中的hashCode() 返回对象的32位jvm内存地址。也就是说如果对象不重写该方法，则返回相应对象的32为JVM内存地址。
String类源码中重写的hashCode方法如下：


```
public int hashCode() {
    int h = hash;    //Default to 0 ### String类中的私有变量，
    if (h == 0 && value.length > 0) {    //private final char value[]; ### Sting类中保存的字符串内容的的数组
        char val[] = value;
        for (int i = 0; i < value.length; i++) {
            h = 31 * h + val[i];
        }
        hash = h;
    }
    return h;
}
```
# 总结：
(1)绑定。当equals方法被重写时，通常有必要重写 hashCode 方法，以维护 hashCode 方法的常规协定，该协定声明相等对象必须具有相等的哈希码。
(2)绑定原因。Hashtable实现一个哈希表，为了成功地在哈希表中存储和检索对象，用作键的对象必须实现 hashCode 方法和 equals 方法。同(1)，必须保证equals相等的对象，hashCode 也相等。因为哈希表通过hashCode检索对象。
(3)默认。
==默认比较对象在JVM中的地址。
hashCode 默认返回对象在JVM中的存储地址。
equal比较对象，默认也是比较对象在JVM中的地址，同==
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/08/477333504f50970253d45c7a6f7c359b.jpg)
