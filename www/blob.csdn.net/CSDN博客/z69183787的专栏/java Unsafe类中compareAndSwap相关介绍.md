# java Unsafe类中compareAndSwap相关介绍 - z69183787的专栏 - CSDN博客
2017年09月29日 16:29:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：346
最近在看jdk7中java.util.concurrent下面的源码中，发现许多类中使用了Unsafe类中的方法来保证并发的安全性，而java 7 api中并没有这个类的相关介绍，在网上查了许多资料，其中[http://ifeve.com/sun-misc-unsafe/](http://ifeve.com/sun-misc-unsafe/)这个网站详细的讲解了Unsafe的相关用法，而下面是结合网站中的介绍和具体的AtomicInteger类来讲解一下其相关的用法。
**[java]**[view
 plain](http://blog.csdn.net/sherld/article/details/42492259#)[copy](http://blog.csdn.net/sherld/article/details/42492259#)
- privatestaticfinal Unsafe unsafe = Unsafe.getUnsafe();  
- privatestaticfinallong valueOffset;  
- 
- static {  
- try {  
-         valueOffset = unsafe.objectFieldOffset  
-             (AtomicInteger.class.getDeclaredField("value"));  
-     } catch (Exception ex) { thrownew Error(ex); }  
- }  
- 
- privatevolatileint value;  
首先可以看到AtomicInteger类在域中声明了这两个私有变量unsafe和valueOffset。其中unsafe实例采用Unsafe类中静态方法getUnsafe()得到，但是这个方法如果我们写的时候调用会报错，因为这个方法在调用时会判断类加载器，我们的代码是没有“受信任”的，而在jdk源码中调用是没有任何问题的；valueOffset这个是指类中相应字段在该类的偏移量，在这里具体即是指value这个字段在AtomicInteger类的内存中相对于该类首地址的偏移量。
然后可以看一个有一个静态初始化块，这个块的作用即是求出value这个字段的偏移量。具体的方法使用的反射的机制得到value的Field对象，再根据objectFieldOffset这个方法求出value这个变量内存中在该对象中的偏移量。
volatile关键字保证了在多线程中value的值是可见的，任何一个线程修改了value值，会将其立即写回内存当中
**[java]**[view
 plain](http://blog.csdn.net/sherld/article/details/42492259#)[copy](http://blog.csdn.net/sherld/article/details/42492259#)
- publicfinalint getAndSet(int newValue) {  
- for (;;) {  
- int current = get();  
- if (compareAndSet(current, newValue))  
- return current;  
-     }  
- }  
- 
- publicfinalboolean compareAndSet(int expect, int update) {  
- return unsafe.compareAndSwapInt(this, valueOffset, expect, update);  
- }  
getAndSet这个方法作用为将value值设置为newValue，并返回修改前的value值
在for循环中，保证每次如果compareAndSet这个方法失败之后，能重新进行尝试，直到成功将value值设置为newValue。
compareAndSet这个方法主要调用unsafe.compareAndSwapInt这个方法，这个方法有四个参数，其中第一个参数为需要改变的对象，第二个为偏移量(即之前求出来的valueOffset的值)，第三个参数为期待的值，第四个为更新后的值。整个方法的作用即为若调用该方法时，value的值与expect这个值相等，那么则将value修改为update这个值，并返回一个true，如果调用该方法时，value的值与expect这个值不相等，那么不做任何操作，并范围一个false。
因此之所以在getAndSet方法中调用一个for循环，即保证如果调用compareAndSet这个方法返回为false时，能再次尝试进行修改value的值，直到修改成功，并返回修改前value的值。
整个代码能保证在多线程时具有线程安全性，并且没有使用java中任何锁的机制，所依靠的便是Unsafe这个类中调用的该方法具有原子性，这个原子性的保证并不是靠java本身保证，而是靠一个更底层的与操作系统相关的特性实现。
