# 谈谈java的ThreadLocal - Spark高级玩法 - CSDN博客
2018年04月12日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：36
**简单介绍**
ThreadLocal一般称为**线程本地变量，**它是一种特殊的线程绑定机制，将变量与线程**绑定**在一起，为每一个线程维护一个独立的变量副本。通过ThreadLocal可以将对象的可见范围限制在同一个线程内。
**跳出误区**
需要重点强调的的是，不要拿ThreadLocal和synchronized做类比，因为这种比较压根就是无意义的！sysnchronized是一种互斥同步机制，是为了保证在多线程环境下对于共享资源的正确访问。而ThreadLocal从本质上讲，无非是提供了一个**“线程级”**的**变量作用域**，它是一种**线程封闭**（每个线程独享变量）技术，更直白点讲，ThreadLocal可以理解为将对象的作用范围限制在一个**线程上下文**中，使得变量的作用域为“**线程级**”。
没有ThreadLocal的时候，一个线程在其生命周期内，可能穿过多个层级，多个方法，如果有个对象需要在此线程周期内多次调用，且是跨层级的（线程内共享），通常的做法是通过参数进行传递；而ThreadLocal将变量绑定在线程上，在一个线程周期内，无论“你身处何地”，只需通过其提供的get方法就可轻松获取到对象。极大地提高了对于“线程级变量”的访问便利性。
**来看个简单的例子**
假设我们要为每个线程关联一个唯一的序号，在每个线程周期内，我们需要多次访问这个序号，这时我们就可以使用ThreadLocal了.（当然下面这个例子没有完全体现出跨层级跨方法的调用，理解就可以了）
package concurrent;
import java.util.concurrent.atomic.AtomicInteger;
/**
 * Created by chengxiao on 2016/12/12.
 */
public class ThreadLocalDemo {
    public static void main(String []args){
        for(int i=0;i<5;i++){
            final Thread t = new Thread(){
                @Override
                public void run(){
                    System.out.println("当前线程:"+
                    Thread.currentThread().getName()+",已分配ID:"
                    +ThreadId.get());
                }
            };
            t.start();
        }
    }
    static   class ThreadId{
        //一个递增的序列，使用AtomicInger原子变量保证线程安全
        private static final AtomicInteger nextId = new AtomicInteger(0);
        //线程本地变量，为每个线程关联一个唯一的序号
        private static final ThreadLocal<Integer> threadId =
                new ThreadLocal<Integer>() {
                    @Override
                    protected Integer initialValue() {
                        return nextId.getAndIncrement();//相当于nextId++,
                        由于nextId++这种操作是个复合操作而非原子操作，
                        会有线程安全问题(可能在初始化时就获取到相同的ID，
                        所以使用原子变量
                    }
                };
       //返回当前线程的唯一的序列，如果第一次get，会先调用initialValue，
       后面看源码就了解了
        public static int get() {
            return threadId.get();
        }
    }
}
执行结果,可以看到每个线程都分配到了一个唯一的ID，同时在此线程范围内的"任何地点"，我们都可以通过ThreadId.get()这种方式直接获取。
当前线程:Thread-4,已分配ID:1
当前线程:Thread-0,已分配ID:0
当前线程:Thread-2,已分配ID:3
当前线程:Thread-1,已分配ID:4
当前线程:Thread-3,已分配ID:2
set操作，为线程绑定变量
public void set(T value) {
    //1.首先获取当前线程对象
    Thread t = Thread.currentThread();
    //2.获取该线程对象的ThreadLocalMap
    ThreadLocalMap map = getMap(t);
    //如果map不为空，执行set操作，以当前threadLocal对象为key，
    //实际存储对象为value进行set操作
    if (map != null)
        map.set(this, value);
    else
    //如果map为空，则为该线程创建ThreadLocalMap
        createMap(t, value);
}
可以看到，ThreadLocal不过是个入口，真正的变量是绑定在线程上的。
ThreadLocalMap getMap(Thread t) {
//线程对象持有ThreadLocalMap的引用
    return t.threadLocals;
}
下面给是Thread类中的定义，每个线程对象都拥有一个ThreadLocalMap对象
ThreadLocal.ThreadLocalMap threadLocals = null;
现在，我们能看出ThreadLocal的设计思想了：
1.ThreadLocal仅仅是个变量访问的入口；
2.每一个Thread对象都有一个ThreadLocalMap对象，这个ThreadLocalMap持有对象的引用；
3.ThreadLocalMap以当前的threadlocal对象为key，以真正的存储对象为value。get时通过threadlocal实例就可以找到绑定在当前线程上的对象。
乍看上去，这种设计确实有些绕。我们完全可以在设计成Map<Thread,T>这种形式，一个线程对应一个存储对象。
**ThreadLocal这样设计的目的主要有两个：**
**一是**可以保证当前线程结束时相关对象能尽快被回收；
**二是**ThreadLocalMap中的元素会大大减少，我们都知道map过大更容易造成哈希冲突而导致性能变差。
我们再来看看get方法
public T get() {
 //1.首先获取当前线程
 Thread t = Thread.currentThread();
 //2.获取线程的map对象
 ThreadLocalMap map = getMap(t);
 //3.如果map不为空，以threadlocal实例为key
 //获取到对应Entry，然后从Entry中取出对象即可。
 if (map != null) {
     ThreadLocalMap.Entry e = map.getEntry(this);
     if (e != null)
         return (T)e.value;
 }
 //如果map为空，也就是第一次没有调用set直接get
 //（或者调用过set，又调用了remove）时，为其设定初始值
 return setInitialValue();
}
**setInitialValue**
private T setInitialValue() {
    T value = initialValue();//获取初始值
    Thread t = Thread.currentThread();
    ThreadLocalMap map = getMap(t);
    if (map != null)
       map.set(this, value);
    else
        createMap(t, value);
    return value;
}
initialValue方法，默认是null，访问权限是protected，即允许重写。
protected T initialValue() {
return null;
}
 谈到这儿，我们应该已经对ThreadLocal的设计目的及设计思想有一定的了解了。
还有一个会引起疑惑的问题，我们说ThreadLocal为每一个线程维护一个独立的**变量副本**，那么是不是说各个线程之间真正的做到对于对象的**“完全自治”**而不对其他线程的对象产生影响呢？其实这已经不属于对于ThreadLocal的讨论，而是你出于何种目的去使用ThreadLocal。如果我们为一个线程关联的对象是“完全独享”的，也就是每个线程拥有一整套的新的 **栈中的对象引用+堆中的对象，**那么这种情况下是真正的彻底的“**线程独享变量**”，相当于一种深度拷贝，每个线程自己玩自己的，对该对象做任何的操作也不会对别的线程有任何影响。
另一种更普遍的情况，所谓的独享变量副本，其实也就是每个线程都拥有一个独立的对象引用，而堆中的对象还是**线程间共享**的，这种情况下，自然还是会涉及到对共享资源的访问操作，依然会有线程不安全的风险。所以说，ThreadLocal无法解决线程安全问题。
所以，需不需要完全独享变量，进行完全隔离，就取决于你的应用场景了。可以想象，对象过大的时候，如果每个线程都有这么一份“深拷贝”，并发又比较大，对于服务器的压力自然是很大的。像web开发中的servlet，servlet是线程不安全的，一请求一线程，多个线程共享一个servlet对象；而早期的CGI设计中，N个请求就对应N个对象，并发量大了之后性能自然就很差。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXJHvlTLzfsKKtAxeKO7cKkfzHb9RQ3KePlWEsItM8J2hl6hiathkvnzQzsxlhjQkqEJB2n61dicKaA/640?wx_fmt=png)
**ThreadLocal使用注意**
ThreadLocal<T>的出现是一种空间换时间的思想的运用，是为了多线程环境下单线程内变量共享的问题。它的原理就是每个线程通过ThreadLocal.ThreadLocalMap，保存当前线程中所有ThreadLocal变量引用的key和值。相当于每个线程有各自的变量副本，线程内共享这个变量数据，线程间互不影响。
ThreadLocal<T>有它自己的使用场景，比如Spring中用它了解决Session、Connection等多线程并发访问问题，但不能它不能用来代替为了解决多线程安全问题的同步关键字，因为它实际上没有多线程间的变量共享，而线程安全问题是指多线程间变量共享，且共享变量可修改，进而可能会出现多线程并发修改共享变量的问题，这种需要通过同步手段解决。
ThreadLocal<T>变量一般要声名成static类型，即当前线程中只有一个T类型变量的实例，线程内可共享该实例数据且不会出问题，如将其声名成非static，则一个线程内就存储多个T类型变量的实例，有点存储空间的浪费，一般很少有这样的应用场景。另外根据实际情况，ThreadLocal变量声名时也多加上private final关键词表明它时类内私有、引用不可修改。
在线程池环境下，由于线程是一直运行且复用的，使用ThreadLocal<T>时会出现这个任务看到上个任务ThreadLocal变量值以及内存泄露等问题，解决方法就是在当前任务执行完后将ThreadLocal变量remove或设置为初始值，类似在Struts2 框架中Filter里的处理方法。
**Spark中如何使用ThreadLocal的呢？**
spark使用的是InheritableThreadLocal，该类扩展了 ThreadLocal，为子线程提供从父线程那里继承的值：在创建子线程时，子线程会接收所有可继承的线程局部变量的初始值，以获得父线程所具有的值。通常，子线程的值与父线程的值是一致的；但是，通过重写这个类中的 childValue 方法，子线程的值可以作为父线程值的一个任意函数。
当必须将变量（如用户 ID 和 事务 ID）中维护的每线程属性（per-thread-attribute）自动传送给创建的所有子线程时，应尽可能地采用可继承的线程局部变量，而不是采用普通的线程局部变量。
InheritableThreadLocal，在spark中主要有三个地方用到：
1，SparkContext内部
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXJHvlTLzfsKKtAxeKO7cKko3IkpmIRVuRHic7OCDwTnW9zrFktKY7Wd2Zewv4bpCvY5fXQrQsgCMA/640?wx_fmt=png)
2，DynamicVariable内部
动态变量是scala的特性
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXJHvlTLzfsKKtAxeKO7cKkvVibdyJgw7CLCfj2QIyT5tgoJ8KPewXzMBRfnSiaJ9ib6ics2libe04AP0A/640?wx_fmt=png)
3，InputFileBlockHolder内部
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXJHvlTLzfsKKtAxeKO7cKkEL5xnjQRMQqPhRHMROghP6Oxic7C1zFEPogd4g7KTFQHtRic2ZoUFOAQ/640?wx_fmt=png)
本文部分摘取自网络，如有侵权，请联系浪尖删除。
**推荐阅读：**
1，[Java面试官最爱的volatile关键字](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484461&idx=1&sn=013d09c9985bca849a3a6de396648bcb&chksm=9f38e705a84f6e1304e3390a492b6ef93c72225ee4579a2887e316c881f90ac5e84e2cbbccbd&scene=21#wechat_redirect)
2，[浅析Java中的final关键字](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484460&idx=1&sn=3a6920291100ea83c5ccf1867ff5349a&chksm=9f38e704a84f6e12ec015f8f7ded1c2c9a25e495c07b8227f347287ab241f027f4beabed8e50&scene=21#wechat_redirect)
3，[Java反射机制深入详解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484454&idx=1&sn=f5e1d27d311011df0663bb0dd16a7ec2&chksm=9f38e70ea84f6e18d0be5a310e45bab759b0681fe990aa7f616f3531e6c70c4354f5d60449bd&scene=21#wechat_redirect)
4，[不要以工作时长论英雄，不要以加班加点为美德](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484406&idx=1&sn=2cd52c08d47d85623fa32a73cad18ffd&chksm=9f38e0dea84f69c854a41ba0bb5fb10dc298caec37c65c6613131288eaae43dc506357bf2611&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
