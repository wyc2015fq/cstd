# Java Atomic - z69183787的专栏 - CSDN博客
2015年09月04日 21:53:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：494
个人分类：[多线程-原子变量AtomicX](https://blog.csdn.net/z69183787/article/category/5578339)

我们知道volatile修饰的变量可以实现基本的加载和赋值的原子性,但是对于像i++等操作就不能保证原子性了,在JDK1.5之前我们只能通过 synchronized(阻塞的方式)实现这些复合操作的原子性,在JDK1.5中java.util.concurrent.atomic 包提供了若干个类能实现对int,long,boolean,reference的几个特殊方法非阻塞原子性,这一系列类的主要基于以下两点 
1.volatile修饰变量,保证get()/set()的原子性 
2.利用系统底层的CAS原语来实现非阻塞的其它方法原子操作 
  compareAndSwap(memorylocation,expectedValue,newValue);该操作接受一个预计值和新的赋值,当预计值与实际值相符合时,就表明该变量在此期间没有被别的线程改变(可能有ABA问题),就把新值赋给该引用. 
  通常将 CAS 用于同步的方式是从地址 V 读取值 A，执行多步计算来获得新值 B，然后使用 CAS 将 V 的值从 A 改为 B。如果 V 处的值尚未同时更改，则 CAS 操作成功 
各个类的介绍 
**1.AtomicInteger **
  实现了对Int的各种操作的原子化,我们看看其中的方法 
   //实现atomic类的最大诀窍##### 
    public final boolean compareAndSet(int expect, int update) { 
        //如果内存位置的值与期望值相同则,赋予新值,并返回true 
return unsafe.compareAndSwapInt(this, valueOffset, expect, update); 
    } 
   //该方法实现了i++的非阻塞的原子操作 
   public final int getAndIncrement() { 
        for (;;) { //循环,使用CAS的经典方式,这是实现non-blocking方式的代价 
            int current = get();//得到现在的值 
            int next = current + 1;//通过计算得到要赋予的新值 
            if (compareAndSet(current, next)) //关键点,调用CAS原子更新, 
                return current; 
        } 
    } 
**2.AtmoicLong **
  与AtomicInteger相似,只是是基于Long 
**3.AtmoicBoolean **
  与AtomimcInteger相似,基于boolean 
**4.AtomicReference **
  与上述三个类不同,该类是用于原子地更新某个引用,只提供操作保证某个引用的更新会被原子化,常用封装某个引用会被多个线程频繁更新的场景,保证线程安全性 
public final V getAndSet(V newValue) { 
        while (true) { 
            V x = get(); 
            if (compareAndSet(x, newValue)) 
                return x; 
        } 
    } 
====================================================================== 
**5.AtomicIntegerArray **
  对数组中的制定int提供几种特定的原子操作,注意不是对这个数组对象进行原子操作 
    //对数组的第i个元素进行原子的i--操作 
    //注意该类的原子操作都是针对数组中的某个指定元素的 
     public final int getAndDecrement(int i) { 
        while (true) { 
            int current = get(i); 
            int next = current - 1; 
            if (compareAndSet(i, current, next)) 
                return current; 
        } 
    } 
**6.AtomicLongArray **
  与AtomicIntegerArray相似,基于long的 
**7.AtomicReferenceArray **
  与AtomicIntegerArray相似,基于reference的 
====================================================================== 
**8.AtomicLongFieldUpdater **
  上述的7个类都是基于对类自身的volatile数据提供原子操作,但是如何对已经存在的类中的volatile数据提供原子支持呢,这个需求是很常见 的,因为我们在日常开发中经常要使用第三方的class.8#9#10这三个类就是利用反射机制对指定类的指定的volatile field提供原子操作的工具类.这三个类在原子数据结构中被用到,可以参考源码.注意只对public volatile的非static 成员起作用 
//工厂方法,给指定类的指定fild(必须是long)建立一个update,field必须是volatile的 
public static <U> AtomicLongFieldUpdater<U> newUpdater(Class<U> tclass, String fieldName) 
**9.AtomicIntegerFieldUpdater **
与上述类相似,只是基于int 
**10.AtomicReferenceFieldUpdater **
  基于Reference 
====================================================================== 
**11.AtomicMarkableReference 12.AtomicStampedReference**
J2SE 5.0提供了一组atomic class来帮助我们简化同步处理。基本工作原理是使用了同步synchronized的方法实现了对一个long, integer, 对象的增、减、赋值（更新）操作. 比如对于++运算符AtomicInteger可以将它持有的integer 能够atomic 地递增。在需要访问两个或两个以上 atomic变量的程序代码（或者是对单一的atomic变量执行两个或两个以上的操作）通常都需要被synchronize以便两者的操作能够被当作是 一个atomic的单元。 
对array atomic变量来说，一次只有一个索引变量可以变动，并没有功能可以对整个array做atomic化的变动。 
关于Atomic的几个方法 
getAndSet() : 设置新值，返回旧值. 
compareAndSet(expectedValue, newValue) : 如果当前值(current value)等于期待的值(expectedValue), 则原子地更新指定值为新值(newValue), 如果更新成功，返回true, 否则返回false, 换句话可以这样说: 将原子变量设置为新的值, 但是如果从我上次看到的这个变量之后到现在被其他线程修改了(和我期望看到的值不符), 那么更新失败 
从effective java (2)中拿来的一个关于AtomicReference的一个例子: 
Java代码
Java代码 ![复制代码](http://meng-lin.iteye.com/images/icon_copy.gif)![收藏代码](http://meng-lin.iteye.com/images/icon_star.png)![](http://meng-lin.iteye.com/images/spinner.gif)
- publicclass  AtomicTest {      
- privateint  x, y;      
- 
- privateenum  State {      
-         NEW, INITIALIZING, INITIALIZED      
-     };      
- 
- privatefinal  AtomicReference<State> init =  new  AtomicReference<State>(State.NEW);      
- 
- public  AtomicTest() {      
-     }      
- 
- public  AtomicTest( int  x,  int  y) {      
-         initialize(x, y);      
-     }      
- 
- privatevoid  initialize( int  x,  int  y) {      
- if  (!init.compareAndSet(State.NEW, State.INITIALIZING)) {      
- thrownew  IllegalStateException( "initialize is error" );      
-         }      
- this .x = x;      
- this .y = y;      
-         init.set(State.INITIALIZED);      
-     }      
- 
- publicint  getX() {      
-         checkInit();      
- return  x;      
-     }      
- 
- publicint  getY() {      
-         checkInit();      
- return  y;      
-     }      
- 
- privatevoid  checkInit() {      
- if  (init.get() == State.INITIALIZED) {      
- thrownew  IllegalStateException( "uninitialized" );      
-         }      
-     }      
- 
- }    
上面的例子比较容易懂, 不过貌似没什么价值, 而在实际的应用中, 我们一般采用下面的方式来使用atomic class: 
Java代码
Java代码 ![复制代码](http://meng-lin.iteye.com/images/icon_copy.gif)![收藏代码](http://meng-lin.iteye.com/images/icon_star.png)![](http://meng-lin.iteye.com/images/spinner.gif)
- publicclass  CounterTest {      
-     AtomicInteger counter =  new  AtomicInteger( 0 );      
- 
- publicint  count() {      
- int  result;      
- boolean  flag;      
- do  {      
-             result = counter.get();      
- // 断点    
- // 单线程下, compareAndSet返回永远为true,    
- // 多线程下, 在与result进行compare时, counter可能被其他线程set了新值, 这时需要重新再取一遍再比较,    
- // 如果还是没有拿到最新的值, 则一直循环下去, 直到拿到最新的那个值    
-             flag = counter.compareAndSet(result, result +  1 );      
-         }  while  (!flag);      
- 
- return  result;      
-     }      
- 
- publicstaticvoid  main(String[] args) {      
- final  CounterTest c =  new  CounterTest();      
- new  Thread() {      
- @Override
- publicvoid  run() {      
-                 c.count();      
-             }      
-         }.start();      
- 
- new  Thread() {      
- @Override
- publicvoid  run() {      
-                 c.count();      
-             }      
-         }.start();      
- 
- new  Thread() {      
- @Override
- publicvoid  run() {      
-                 c.count();      
-             }      
-         }.start();      
-     }      
- }  
类似i++这样的"读-改-写"复合操作(在一个操作序列中, 后一个操作依赖前一次操作的结果), 在多线程并发处理的时候会出现问题, 因为可能一个线程修改了变量, 而另一个线程没有察觉到这样变化, 当使用原子变量之后, 则将一系列的复合操作合并为一个原子操作,从而避免这种问题, i++=>i.incrementAndGet() 
原子变量只能保证对一个变量的操作是原子的, 如果有多个原子变量之间存在依赖的复合操作, 也不可能是安全的, 另外一种情况是要将更多的复合操作作为一个原子操作, 则需要使用synchronized将要作为原子操作的语句包围起来. 因为涉及到可变的共享变量(类实例成员变量)才会涉及到同步, 否则不必使用synchronized
