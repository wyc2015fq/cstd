# WeakReference与SoftReference - z69183787的专栏 - CSDN博客
2017年04月12日 10:36:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：343
个人分类：[Java-对象引用](https://blog.csdn.net/z69183787/article/category/6562208)
转自：[http://flyneil.iteye.com/blog/1345177](http://flyneil.iteye.com/blog/1345177)
WeakReference与SoftReference都可以用来保存对象的实例引用，这两个类与垃圾回收有关。
WeakReference是弱引用，其中保存的对象实例可以被GC回收掉。这个类通常用于在某处保存对象引用，而又不干扰该对象被GC回收，通常用于Debug、内存监视工具等程序中。因为这类程序一般要求即要观察到对象，又不能影响该对象正常的GC过程。
最近在JDK的Proxy类的实现代码中也发现了Weakrefrence的应用，Proxy会把动态生成的Class实例暂存于一个由Weakrefrence构成的Map中作为Cache。
SoftReference是强引用，它保存的对象实例，除非JVM即将OutOfMemory，否则不会被GC回收。这个特性使得它特别适合设计对象Cache。对于Cache，我们希望被缓存的对象最好始终常驻内存，但是如果JVM内存吃紧，为了不发生OutOfMemoryError导致系统崩溃，必要的时候也允许JVM回收Cache的内存，待后续合适的时机再把数据重新Load到Cache中。这样可以系统设计得更具弹性。
WeakReference的一个测试程序：
Java代码 ![复制代码](http://flyneil.iteye.com/images/icon_copy.gif)![收藏代码](http://flyneil.iteye.com/images/icon_star.png)
- **import** java.lang.ref.WeakReference;   
- 
- publicclass WeakReferenceTest {   
- 
- /** 
-      * @param args 
-      */
- publicstaticvoid main(String[] args) {
   
-         A a = new A();   
-         a.str = "Hello, reference";   
-         WeakReference<A> weak = new WeakReference<A>(a);   
-         a = null;   
- int i = 0;   
- while (weak.get() != null) {
   
-             System.out.println(String.format("Get str from object of WeakReference: %s, count: %d", weak.get().str, ++i));   
- if (i % 10 == 0) {
   
-                 System.gc();   
-                 System.out.println("System.gc() was invoked!");   
-             }   
- try {   
-                 Thread.sleep(500);   
-             } catch (InterruptedException e) {   
- 
-             }   
-         }   
-         System.out.println("object a was cleared by JVM!");   
-     }   
- 
- }  
Java代码  ![收藏代码](http://wiseideal.iteye.com/images/icon_star.png)
- import java.lang.ref.WeakReference;  
- 
- publicclass WeakReferenceTest {  
- 
- /**
-      * @param args
-      */
- publicstaticvoid main(String[] args) {  
-         A a = new A();  
-         a.str = "Hello, reference";  
-         WeakReference<A> weak = new WeakReference<A>(a);  
-         a = null;  
- int i = 0;  
- while (weak.get() != null) {  
-             System.out.println(String.format("Get str from object of WeakReference: %s, count: %d", weak.get().str, ++i));  
- if (i % 10 == 0) {  
-                 System.gc();  
-                 System.out.println("System.gc() was invoked!");  
-             }  
- try {  
-                 Thread.sleep(500);  
-             } catch (InterruptedException e) {  
- 
-             }  
-         }  
-         System.out.println("object a was cleared by JVM!");  
-     }  
- 
- }  
 程序运行结果：
Java代码 ![复制代码](http://flyneil.iteye.com/images/icon_copy.gif)![收藏代码](http://flyneil.iteye.com/images/icon_star.png)
- Get str from object of WeakReference: Hello, reference, count: 1
- Get str from object of WeakReference: Hello, reference, count: 2
- Get str from object of WeakReference: Hello, reference, count: 3
- Get str from object of WeakReference: Hello, reference, count: 4
- Get str from object of WeakReference: Hello, reference, count: 5
- Get str from object of WeakReference: Hello, reference, count: 6
- Get str from object of WeakReference: Hello, reference, count: 7
- Get str from object of WeakReference: Hello, reference, count: 8
- Get str from object of WeakReference: Hello, reference, count: 9
- Get str from object of WeakReference: Hello, reference, count: 10
- System.gc() was invoked!   
- object a was cleared by JVM!  
Java代码  ![收藏代码](http://wiseideal.iteye.com/images/icon_star.png)
- Get str from object of WeakReference: Hello, reference, count: 1
- Get str from object of WeakReference: Hello, reference, count: 2
- Get str from object of WeakReference: Hello, reference, count: 3
- Get str from object of WeakReference: Hello, reference, count: 4
- Get str from object of WeakReference: Hello, reference, count: 5
- Get str from object of WeakReference: Hello, reference, count: 6
- Get str from object of WeakReference: Hello, reference, count: 7
- Get str from object of WeakReference: Hello, reference, count: 8
- Get str from object of WeakReference: Hello, reference, count: 9
- Get str from object of WeakReference: Hello, reference, count: 10
- System.gc() was invoked!  
- object a was cleared by JVM!  
SoftReference的一个测试程序：
Java代码 ![复制代码](http://flyneil.iteye.com/images/icon_copy.gif)![收藏代码](http://flyneil.iteye.com/images/icon_star.png)
- **import** java.lang.ref.SoftReference;   
- 
- publicclass SoftReferenceTest {   
- 
- /** 
-      * @param args 
-      */
- publicstaticvoid main(String[] args) {
   
-         A a = new A();   
-         a.str = "Hello, reference";   
-         SoftReference<A> sr = new SoftReference<A>(a);   
-         a = null;   
- int i = 0;   
- while (sr.get() != null) {
   
-             System.out.println(String.format("Get str from object of SoftReference: %s, count: %d", sr.get().str, ++i));   
- if (i % 10 == 0) {
   
-                 System.gc();   
-                 System.out.println("System.gc() was invoked!");   
-             }   
- try {   
-                 Thread.sleep(500);   
-             } catch (InterruptedException e) {   
- 
-             }   
-         }   
-         System.out.println("object a was cleared by JVM!");   
-     }   
- 
- }  
Java代码  ![收藏代码](http://wiseideal.iteye.com/images/icon_star.png)
- import java.lang.ref.SoftReference;  
- 
- publicclass SoftReferenceTest {  
- 
- /**
-      * @param args
-      */
- publicstaticvoid main(String[] args) {  
-         A a = new A();  
-         a.str = "Hello, reference";  
-         SoftReference<A> sr = new SoftReference<A>(a);  
-         a = null;  
- int i = 0;  
- while (sr.get() != null) {  
-             System.out.println(String.format("Get str from object of SoftReference: %s, count: %d", sr.get().str, ++i));  
- if (i % 10 == 0) {  
-                 System.gc();  
-                 System.out.println("System.gc() was invoked!");  
-             }  
- try {  
-                 Thread.sleep(500);  
-             } catch (InterruptedException e) {  
- 
-             }  
-         }  
-         System.out.println("object a was cleared by JVM!");  
-     }  
- 
- }  
 程序运行结果：
Java代码 ![复制代码](http://flyneil.iteye.com/images/icon_copy.gif)![收藏代码](http://flyneil.iteye.com/images/icon_star.png)
- Get str from object of SoftReference: Hello, reference, count: 1
- Get str from object of SoftReference: Hello, reference, count: 2
- Get str from object of SoftReference: Hello, reference, count: 3
- Get str from object of SoftReference: Hello, reference, count: 4
- Get str from object of SoftReference: Hello, reference, count: 5
- Get str from object of SoftReference: Hello, reference, count: 6
- Get str from object of SoftReference: Hello, reference, count: 7
- Get str from object of SoftReference: Hello, reference, count: 8
- Get str from object of SoftReference: Hello, reference, count: 9
- Get str from object of SoftReference: Hello, reference, count: 10
- System.gc() was invoked!   
- Get str from object of SoftReference: Hello, reference, count: 11
- Get str from object of SoftReference: Hello, reference, count: 12
- Get str from object of SoftReference: Hello, reference, count: 13
- Get str from object of SoftReference: Hello, reference, count: 14
- Get str from object of SoftReference: Hello, reference, count: 15
- Get str from object of SoftReference: Hello, reference, count: 16
- Get str from object of SoftReference: Hello, reference, count: 17
- Get str from object of SoftReference: Hello, reference, count: 18
- Get str from object of SoftReference: Hello, reference, count: 19
- Get str from object of SoftReference: Hello, reference, count: 20
- System.gc() was invoked!   
- Get str from object of SoftReference: Hello, reference, count: 21
- Get str from object of SoftReference: Hello, reference, count: 22
- Get str from object of SoftReference: Hello, reference, count: 23
- Get str from object of SoftReference: Hello, reference, count: 24
- Get str from object of SoftReference: Hello, reference, count: 25
- Get str from object of SoftReference: Hello, reference, count: 26
- Get str from object of SoftReference: Hello, reference, count: 27
- Get str from object of SoftReference: Hello, reference, count: 28
自己的标注 写道
上面的打印结果会一直持续下去。 因为soft.get()一直不会为空
SoftReference比WeakReference生命力更强，当JVM的内存不吃紧时，即使引用的对象被置为空了，Soft还可以保留对该对象的引用，此时的JVM内存池实际上还保有原来对象，只有当内存吃紧的情况下JVM才会清除Soft的引用对象，并且会在未来重新加载该引用的对象。 
而WeakReference则当清理内存池时会自动清理掉引用的对象。 
jvm是如何判断即将OutOfMemory的呢? 这个判断标准是否能够配置呢? 比如说, jvm判断当可分配的内存少于5M时为OutOfMemory, 那么这个5M能否被配置为10M或者其他任意数字呢?
-XX:CMSInitiatingOccupancyFraction=70
表示old区用到70%就开始回收，这个是默认的，你可以根据情况调整它。例如99%才回收，当然这个可能会比较危险，会造成真的OOM。
