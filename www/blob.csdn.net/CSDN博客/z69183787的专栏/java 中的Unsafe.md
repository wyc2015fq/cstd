# java 中的Unsafe - z69183787的专栏 - CSDN博客
2015年09月04日 22:01:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：681
在阅读AtomicInteger的源码时，看到了这个类：sum.msic.Unsafe，之前从没见过。所以花了点时间google了一下。
Unsafe的源码：http://www.docjar.com/html/api/sun/misc/Unsafe.java.html
Unsafe源码中的描述如下：
 写道
A collection of methods for performing low-level, unsafe operations. Although the class and all methods are public, use of this class is limited because only trusted code can obtain instances of it.
这个类是用于执行低级别、不安全操作的方法集合。尽管这个类和所有的方法都是公开的（public），但是这个类的使用仍然受限，你无法在自己的java程序中直接使用该类，因为只有授信的代码才能获得该类的实例。
从上面的描述，可以了解到该类是用来执行较低级别的操作的，比如获取某个属性在内存中的位置，不过一般人很少会有这样的需求。在AtomicInteger的源码中相关的代码如下：
Java代码  ![收藏代码](http://aswang.iteye.com/images/icon_star.png)
- // setup to use Unsafe.compareAndSwapInt for updates
- privatestaticfinal Unsafe unsafe = Unsafe.getUnsafe();  
上面这行代码是获取Unsafe实例的。一般情况下，我们是拿不到该类的实例的，当然jdk库里面是可以随意使用的。
Java代码  ![收藏代码](http://aswang.iteye.com/images/icon_star.png)
- static {  
- try {  
-        valueOffset = unsafe.objectFieldOffset  
-            (AtomicInteger.class.getDeclaredField("value"));  
-      } catch (Exception ex) { thrownew Error(ex); }  
-    }  
上面这几行代码，是用来获取AtomicInteger实例中的value属性在内存中的位置。这里使用了Unsafe的objectFieldOffset方法。这个方法是一个本地方法， 该方法用来获取一个给定的静态属性的位置。
Java代码  ![收藏代码](http://aswang.iteye.com/images/icon_star.png)
- publicnativelong objectFieldOffset(Field f);  
这里有个疑问，为什么需要获取属性在内存中的位置？通过查看AtomicInteger源码发现，在这样几个地方使用到了这个valueOffset值：
Java代码  ![收藏代码](http://aswang.iteye.com/images/icon_star.png)
- publicfinalvoid lazySet(int newValue) {  
-         unsafe.putOrderedInt(this, valueOffset, newValue);  
-     }  
Java代码  ![收藏代码](http://aswang.iteye.com/images/icon_star.png)
- publicfinalboolean compareAndSet(int expect, int update) {  
- return unsafe.compareAndSwapInt(this, valueOffset, expect, update);  
-     }  
Java代码  ![收藏代码](http://aswang.iteye.com/images/icon_star.png)
- publicfinalboolean weakCompareAndSet(int expect, int update) {  
- return unsafe.compareAndSwapInt(this, valueOffset, expect, update);  
-     }  
 查找资料后，发现lazySet方法大多用在并发的数据结构中，用于低级别的优化。compareAndSet这个方法多见于并发控制中，简称CAS(Compare And Swap)，意思是如果valueOffset位置包含的值与expect值相同，则更新valueOffset位置的值为update，并返回true，否则不更新，返回false。
这里可以举个例子来说明compareAndSet的作用，如支持并发的计数器，在进行计数的时候，首先读取当前的值，假设值为a，对当前值 + 1得到b，但是+1操作完以后，并不能直接修改原值为b，因为在进行+1操作的过程中，可能会有其它线程已经对原值进行了修改，所以在更新之前需要判断原值是不是等于a，如果不等于a，说明有其它线程修改了，需要重新读取原值进行操作，如果等于a，说明在+1的操作过程中，没有其它线程来修改值，我们就可以放心的更新原值了。
Unsafe类中还包含有很多其它的方法，如果想使用其中的方法，可以参考这篇文章来获取Unsafe的实例：http://javafans.info/java/corejava/15.html。原理是通过反射机制拿到Unsafe中的一个静态属性theUnsafe ，这个静态属性本身已经初始化了，所以拿到以后可以直接使用。
