# 面试|再次讲解Threadlocal使用及其内存溢出 - Spark高级玩法 - CSDN博客
2019年01月27日 22:40:19[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：89
ThreadLocal彻底详解
浪尖整理本文主要是想帮助大家完全消化面试中常见的ThreadLocal问题。希望读懂此文以后大家可以掌握：
- 
简单介绍原理
- 
ThreadLocal使用案例场景
- 
Threadlocal的底层原理
- 
Threadlocal内存溢出原因和解决方法
**1. 简介**
高并发处理起来比较麻烦，很多新手对此都会非常头疼。要知道避免并发的最简单办法就是线程封闭，也即是把对象封装到一个线程里，那么对象就只会被当前线程能看到，使得对象就算不是线程安全的也不会出现任何安全问题。Threadlocal是实现该策略的最好的方法。Threadlocal为每个线程提供了一个私有变量，然后线程访问该变量（get或者set）的时候实际上是读写的自己的局部变量从而避免了并发法问题。
**2. 案例使用**
首先定义一个ThreadLocal的封装工具类
```
package bigdata.spark.study.ThreadLocalTest;
public class Bank {
    ThreadLocal<Integer> t = new ThreadLocal<Integer>(){
        @Override
        protected Integer initialValue() {
            return 100;
        }
    };
    public int get(){
        return t.get();
    }
    public void set(){
        t.set(t.get()+10);
    }
}
```
实现一个Runnable对象然后使用bank对象
```
package bigdata.spark.study.ThreadLocalTest;
public class Transfer implements Runnable {
    Bank bank;
    public Transfer(Bank bank) {
        this.bank = bank;
    }
    @Override
    public void run() {
        for (int i =0 ;i < 10;i++){
            bank.set();
            System.out.println(Thread.currentThread()+" : " +bank.get());
        }
    }
}
```
**定义两个线程t1和t2，运行之后查看结果：**
```
package bigdata.spark.study.ThreadLocalTest;
public class Test {
    public static void main(String[] args) {
        Bank bank = new Bank();
        Transfer t = new Transfer(bank);
        
        Thread t1 = new Thread(t);
        t1.start();
        Thread t2 = new Thread(t);
        t2.start();
        try {
            t1.join();
            t2.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println(bank.get());
    }
}
```
查看输出结果就会发现，发现主线程，线程t1，线程t2之间相互不影响～
```
Thread[Thread-0,5,main] : 110
Thread[Thread-0,5,main] : 120
Thread[Thread-0,5,main] : 130
Thread[Thread-0,5,main] : 140
Thread[Thread-0,5,main] : 150
Thread[Thread-0,5,main] : 160
Thread[Thread-0,5,main] : 170
Thread[Thread-0,5,main] : 180
Thread[Thread-0,5,main] : 190
Thread[Thread-0,5,main] : 200
Thread[Thread-1,5,main] : 110
Thread[Thread-1,5,main] : 120
Thread[Thread-1,5,main] : 130
Thread[Thread-1,5,main] : 140
Thread[Thread-1,5,main] : 150
Thread[Thread-1,5,main] : 160
Thread[Thread-1,5,main] : 170
Thread[Thread-1,5,main] : 180
Thread[Thread-1,5,main] : 190
Thread[Thread-1,5,main] : 200
100
```
**3. 底层源码**
每个线程Thread内部都会有ThreadLocal.ThreadLocalMap对象，该对象是一个自定义的map，key是弱引用包装的ThreadLocal类型，value就是我们的值。
**初始值**
Threadlocal直接在构造的时候设置初始值。主要是要实现其initialValue方法：
```
new ThreadLocal<Integer>(){
    @Override
    protected IntegerinitialValue() {
        return 100;
    }
};
```
追踪一下该方法，会发现其仅仅被一个私有方法调用了
```
private T setInitialValue() {
    T value = initialValue();
    Thread t = Thread.currentThread();
    ThreadLocalMap map = getMap(t);
    if (map != null)
        map.set(this, value);
    else
        createMap(t, value);
    return value;
}
```
解读一下setInitialValue私有方法
- 
首先调用initialVaule方法，获取初始值。
- 
然后获取当前线程对象的引用。
- 
通过线程对象引用获取ThreadLocal.ThreadLocalMap对象 map。
- 
map对象不为空，就将当前threadlocal弱引用作为key，初始值为value完成初始化。
- 
Map对象为空，调用createMap方法，并完成初始化。
```
void createMap(Thread t, T firstValue) {
    t.threadLocals = new ThreadLocalMap(this, firstValue);
}
```
读到这可能会很好奇，为啥只是被私有方法调用，我们又无权调用该私有方法，如何实现初始化呢？也是很简单的在我们第一次调用get的时候，会调用该私有初始化方法，来真正完成初始化。
**Get方法**
具体代码如下：
public T get() {
Thread t = Thread.*currentThread*();
ThreadLocalMap map = getMap(t);
if (map != null) {
ThreadLocalMap.Entry e = map.getEntry(this);
if (e != null) {
**@SuppressWarnings**(**"unchecked"**)
T result = (T)e.value;
return result;
}
    }
return setInitialValue();
}
我们来解读一下get方法，此处就真正暴露ThreadLocal的真实面目了。
- 
获取当前线程对象，t
- 
通过getMap(t)方法来获取t内部的ThreadLocal.ThreadLocalMap对象。
- 
然后判断ThreadLocalMap对象是否为空，不为空就可以通过当前Threadlocal对象获取对应的value值，存在返回，不存在跳过。
- 
假如map为空或者当前threadlocal对象对应的value为空，那么就调用初始化方法setInitialValue初始化并返回初始值。
**Set**
接下来解读一下threadlocal变量的set方法。Set的方法源码如下：
public void set(T value) {
Thread t = Thread.*currentThread*();
ThreadLocalMap map = getMap(t);
if (map != null)
map.set(this, value);
else
createMap(t, value);
}
1. 获取当前线程对象 t
2. 通过getMap(t)方法来获取t内部的ThreadLocal.ThreadLocalMap对象 map。
3. map不为空，当前threadlocal对象作为key（弱引用），要设置的value作为value完成值的设置。
4. 假如map为空，就调用createMap方法，给当前线程创建一个ThreadlocalMap
```
void createMap(Thread t, T firstValue) {
    t.threadLocals = new ThreadLocalMap(this, firstValue);
}
```
**remove方法**
threadlocal的remove方法主要作用是删除当前threadlocal对应的键值对。
```
public void remove() {
    ThreadLocalMap m = getMap(Thread.currentThread());
    if (m != null)
        m.remove(this);
}
```
**4. 内存泄漏**
根据前面对threadlocal的整理，其实可以画出来一个结构图：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWLQcjNXWsNmoWI2ej5AJpmsLjxJVHn3VqOxoCuQb8SLfI9RjKENMO6qVJQDDGNW5e07LuntzUj2A/640?wx_fmt=png)
对value的引用线路有两条：
- 
threadlocalref 是ThreadLocal强引用，key是ThreadLocal变量的弱引用。由于key是弱引用，当ThreadLocalRef因不用而释放掉的时候，ThreadLocal对象就会被回收，由于是key到threadLocal对象为弱引用，一旦进行垃圾回收key就会被回收而相应位置变为null，当然value依然存在。
- 
通过当前线程的引用可以获取当前线程对象，当前线程对象就可以获取到ThreadLocalMap，那么只要当前线程一直存在，ThreadLocalMap对象就会一直存在。
由于ThreadlocalMap存活时间和线程一样，比如我们采用的是常驻线程池，使用线程过程中没有清空ThreadLocalMap，也没有调用threadlocal的remove方法，就将线程放回线程池，虽然ThreadLocal的强引用ThreadLocalRef被清除，弱引用key在GC的时候也会被设置为null，但是对于value值还存在一条强引用链条：
**currentThreadRef-àcurrentThread-àThreadLocalMap-àEntry(value)**，所以value并没有释放，就造成了内存泄漏了。
那这时候你或许会问为啥ThreadLocalMap存储value的时候不采用弱引用呢？这样不就可以避免内存泄漏了么？value是弱引用是不行的，原因很简单：我们**存储的对象**除了ThreadLocalMap的Value就没有其他的引用了，value一但是对象的弱引用，GC的时候被回收，对象就无法访问了，这显然不是我们想要的。
**5. 避免内存泄漏**
为避免内存泄漏最好在使用完ThreadLocal之后调用其remove方法，将数据清除掉。
当然，对于Java8 ThreadLocalMap 的 set 方法通过调用 replaceStaleEntry 方法回收键为 null 的 Entry 对象的值（即为具体实例）以及 Entry 对象本身从而防止内存泄漏
get方法会间接调用expungeStaleEntry 方法将键和值为 null 的 Entry 设置为 null 从而使得该 Entry 可被回收
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWD0HKRNyZbjOPNVCKh0d98G5ydFibUfnEFa6LDKoUfXU5IYnFsBG8lhxmTFJtOrruqNl2NIFQAHhw/640?wx_fmt=png)
欢迎加入500位球友一起学习～
