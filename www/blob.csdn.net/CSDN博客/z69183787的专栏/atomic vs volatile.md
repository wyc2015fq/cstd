# atomic vs volatile - z69183787的专栏 - CSDN博客
2015年09月04日 21:58:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：454
个人分类：[多线程-原子变量AtomicX](https://blog.csdn.net/z69183787/article/category/5578339)

结论： atomic比volatile靠谱
java.util.concurrent.atomic.Atomic*原子类和volatile关键字是java中两种常见的处理多线程下数据共享读写的机制。二者看似相同，但是在实际应用中有着不小的差别。
volatile关键字是通过本地代码实现的写锁，只保证知有一个线程在写某个数据。JVM为了提高数据存取的速度，允许每个线程在自己独立的数据块，对进程中共享的数据进行私有拷贝。volatile就是保证每次读数据时，读的都是存在共享数据块里的数据，而不是私有拷贝。然而，这种机制在有些情况下并不安全。当两个线程T1,T2同时对volatitle int i;作i++;时，可能出现问题。i++相当于为i=i+1。
T1 LOAD i
T2 LOAD i
T1 STORE i+1
T2 STORE i+1
这里应该执行两次i=i+1，得到i=i+2的，但是结果确实i=i+1。
因此，这边就有了Atomic原子类存在的价值了。Atomic类被设计来解决这个问题。
以AtomicInteger为例，public class AtomicInteger extends Number implements java.io.Serializable。
它的父类Number没有一个Field，直接继承与Object，方法也只有abstract的int intValue(),long longValue(),float floatValue(),double doubleValue(),和通过intValue()实现的byte byteValue(),short shortValue()。简单的说，除了序列化外啥field都没有都没有。
AtomicInteger有四个field，除了序列化那个无视掉外，还剩三个。
    // setup to use Unsafe.compareAndSwapInt for updates
    private static final Unsafe unsafe = Unsafe.getUnsafe();
    private static final long valueOffset;
    static {
      try {
        valueOffset = unsafe.objectFieldOffset
            (AtomicInteger.class.getDeclaredField("value"));
      } catch (Exception ex) { throw new Error(ex); }
    }
    private volatile int value;
valueOffset只在这个静态块里被写过，从函数命名上猜测应该是获取这个类中名为value的field到这个object的头指针的偏移（offset）。这边用到了unsafe。
Unsafe是干什么的？sun.misc.Unsafe，未开源。这个名字让我想起了C#里的关键字unsafe。C#为了方便C++程序员转C#，方便使用COM组件，给出了不安全代码unsafe关键字。Java这个Unsafe，能的到一个field距离它Object头的偏移，对Java来说显然是不安全的操作，他给用户直接操作内存的可能。
除了objectFieldOffset(Field)外，这个类中还使用到Unsafe的putOrderedInt(Object,long,int)和compareAndSwapInt(Object,long,int,int)。从方法名和参数看，功能分别为向Object偏移long位置，写入有序整型int，和比较两值是否相同，并填入新值。
这边有篇IBM关于compareAndSwapInt的文档，我写得差不多了才看到，怨念。。。[http://www.ibm.com/developerworks/cn/java/j-jtp11234/](http://www.ibm.com/developerworks/cn/java/j-jtp11234/)
顺带说一句，调用到compareAndSwapInt的两个方法compareAndSet和weakCompareAndSet的在1.6里源码是一样的，样的，的。。。冗余代码很好玩吗,API上写得各种看不懂。。。
剩下一个field就是private volatile int value了。也用了volatitle，由此可以得出，Atomic类在效率上是低于直接用volatitle，能使用volatitle是，就别用Atomic了。
然后看看AtomicInteger的method吧
从Number父类继承来的那几个可以看作getter函数的value类，清一色的调用了get()方法，然后转了一下类型。
    public final int get() {
        return value;
    }
直接把value扔回去了，可见Atomic类在处理读操作时和volatitle没啥区别。
看上去像setter函数的一共有五个：
    public final void set(int newValue) {
        value = newValue;
    }
     public final void lazySet(int newValue) {
        unsafe.putOrderedInt(this, valueOffset, newValue);
    }
    public final int getAndSet(int newValue) {
        for (;;) {
            int current = get();
            if (compareAndSet(current, newValue))
                return current;
        }
    }
    public final boolean compareAndSet(int expect, int update) {
‍return unsafe.compareAndSwapInt(this, valueOffset, expect, update);
    }
    public final boolean weakCompareAndSet(int expect, int update) {
return unsafe.compareAndSwapInt(this, valueOffset, expect, update);
    }
weakCompareAndSet和compareAndSet实现上没有区别，所以无视掉吧，反正AtomicInteger自己也只用compareAndSet。。。
set(int)和直接用volatitle没啥区别，所以只用set作写差不多就和完全没有用到Atomic类一样，还多调用一次函数。
lazySet(int)调用了Unsafe.putOrderedInt，我们只能猜测是直接给内存把个数字写上去的。这样的话其实。。。我也没觉得有啥区别，就是告诉大家一下，你用Unsafe可以把Java当C++写嘛。。。
getAndSet(int)调用了compareAndSet(int,int)，所作的基本就是不停的刷，看看自己改成了没有，改成了救过，没改成继续改。这边要出现两个线程不停对改怎么办。。。
最后一个compareAndSet(int,int)才是实现Atomic类功能的地方。就是比一下预想的和实际的是不是一样，一样就就用新值盖掉他，不一样就返回个false。怎么用呢，看看其他函数就是到了。
对比getAndIncrement()和incrementAndGet()
    public final int getAndIncrement() {
        for (;;) {
            int current = get();
            int next = current + 1;
            if (compareAndSet(current, next))
                return current;
        }
    }
    public final int incrementAndGet() {
        for (;;) {
            int current = get();
            int next = current + 1;
            if (compareAndSet(current, next))
                return next;
        }
    }
