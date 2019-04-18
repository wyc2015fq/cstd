# 使用sun.misc.Cleaner或者PhantomReference实现堆外内存的自动释放 - z69183787的专栏 - CSDN博客
2018年02月12日 14:29:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：294
[http://blog.csdn.net/aitangyong/article/details/39455229](http://blog.csdn.net/aitangyong/article/details/39455229)
我之前的一篇博客：[System.gc()和-XX:+DisableExplicitGC启动参数，以及DirectByteBuffer的内存释放](http://blog.csdn.net/aitangyong/article/details/39403031)文章末尾处：提到java
 NIO包是通过sun.misc.Cleaner和PhantomReference来实现堆外内存的自动释放的。现在我们来学习下Cleaner和PhantomReference的使用，自己封装实现堆外内存的自动释放。
sun.misc.Cleaner是JDK内部提供的用来释放非堆内存资源的API。JVM只会帮我们自动释放堆内存资源，但是它提供了回调机制，通过这个类能方便的释放系统的其他资源。我们先看下如何使用Cleaner。
[java][view
 plain](http://blog.csdn.net/aitangyong/article/details/39455229#)[copy](http://blog.csdn.net/aitangyong/article/details/39455229#)
- package direct;  
- publicclass FreeMemoryTask implements Runnable  
- {  
- privatelong address = 0;  
- 
- public FreeMemoryTask(long address)  
-     {  
- this.address = address;  
-     }  
- 
- @Override
- publicvoid run()  
-     {  
-         System.out.println("runing FreeMemoryTask");  
- 
- if (address == 0)  
-         {  
-             System.out.println("already released");  
-         } else
-         {  
-             GetUsafeInstance.getUnsafeInstance().freeMemory(address);  
-         }  
- 
-     }  
- }  
这个实现了Runnable接口的类，功能就是释放堆外内存。这是我们必须要做的事，JVM没有办法帮我们做。
[java][view
 plain](http://blog.csdn.net/aitangyong/article/details/39455229#)[copy](http://blog.csdn.net/aitangyong/article/details/39455229#)
- publicclass ObjectInHeapUseCleaner  
- {  
- privatelong address = 0;  
- 
- public ObjectInHeapUseCleaner()  
-     {  
-         address = GetUsafeInstance.getUnsafeInstance().allocateMemory(  
- 2 * 1024 * 1024);  
-     }  
- 
- publicstaticvoid main(String[] args)  
-     {  
- while (true)  
-         {  
-             System.gc();  
- 
-             ObjectInHeapUseCleaner heap = new ObjectInHeapUseCleaner();  
- 
- // 增加heap的虚引用,定义清理的接口FreeMemoryTask
-             Cleaner.create(heap, new FreeMemoryTask(heap.address));  
-         }  
-     }  
- }  
运行这段代码，可以发现程序正常运行，不会出现OOM。
 Cleaner.create()需要2个参数：第一个参数：需要监控的堆内存对象，第二个参数：程序释放资源的回调。当JVM进行GC的时候，如果发现我们监控的对象，不存在强引用了（只被Cleaner对象引用，这是个幽灵引用），就会调用第二个参数Runnable.run()方法的逻辑，执行完Runnable.run()的时候（这个时候已经释放了堆外内存），JVM会自动释放堆内存中我们监控的对象。可以看到使用sun.misc.Cleaner很简单。
接下来我们看下，不使用sun.misc.Cleaner的情况下，如何释放资源。
[java][view
 plain](http://blog.csdn.net/aitangyong/article/details/39455229#)[copy](http://blog.csdn.net/aitangyong/article/details/39455229#)
- import java.lang.ref.PhantomReference;  
- import java.lang.ref.Reference;  
- import java.lang.ref.ReferenceQueue;  
- import java.util.HashMap;  
- import java.util.Map;  
- 
- publicclass MyOwnCleaner  
- {  
- 
- privatestatic ReferenceQueue<Object> refQueue = new ReferenceQueue<Object>();  
- 
- privatestatic Map<Reference<Object>, Runnable> taskMap = new HashMap<Reference<Object>, Runnable>();  
- 
- static
-     {  
- new CleanerThread().start();  
-     }  
- 
- publicstaticvoid clear(Object heapObject, Runnable task)  
-     {  
- // 当heapObject没有强引用的时候,reference会自动被JVM加入到引用队列中
- // 不管使用有人持有reference对象的强引用
-         PhantomReference<Object> reference = new PhantomReference<Object>(  
-                 heapObject, refQueue);  
- 
-         taskMap.put(reference, task);  
- 
-     }  
- 
- // 清理线程
- privatestaticclass CleanerThread extends Thread  
-     {  
- @Override
- publicvoid run()  
-         {  
- while (true)  
-             {  
- try
-                 {  
- @SuppressWarnings("unchecked")  
-                     Reference<Object> refer = (Reference<Object>) refQueue  
-                             .remove();  
- 
-                     Runnable r = taskMap.remove(refer);  
-                     r.run();  
-                 } catch (InterruptedException e)  
-                 {  
- 
-                 }  
- 
-             }  
-         }  
-     }  
- 
- }  
这里使用到了PhantomReference和ReferenceQueue，这是JVM内部的对象销毁机制。当堆中的对象不存在强引用，只存在幽灵引用的时候，JVM会自动将这个对象的幽灵引用加入到与之相关联的的引用队列中。
[java][view
 plain](http://blog.csdn.net/aitangyong/article/details/39455229#)[copy](http://blog.csdn.net/aitangyong/article/details/39455229#)
- privatestatic ReferenceQueue<Object> refQueue = new ReferenceQueue<Object>();  
这个就是引用队列，JVM会自动将幽灵引用PhantomReference加入到队列中。也就是说，我们只要轮询这个队列，就能够知道哪些对象即将被JVM回收（这些对象只存在幽灵引用了）。
[java][view
 plain](http://blog.csdn.net/aitangyong/article/details/39455229#)[copy](http://blog.csdn.net/aitangyong/article/details/39455229#)
- publicstaticvoid clear(Object heapObject, Runnable task)  
- {  
- // 当heapObject没有强引用的时候,reference会自动被JVM加入到引用队列中
- // 不管使用有人持有reference对象的强引用
-     PhantomReference<Object> reference = new PhantomReference<Object>(  
-             heapObject, refQueue);  
-     taskMap.put(reference, task);  
- }  
这段代码，相当于是我们给堆中的对象heapObject添加了一个监控（注册了一个幽灵引用）。taskMap记录幽灵引用和相应的代码回收逻辑。之后我们在后台开启了一个CleanerThread线程，不断的轮询引用队列，一旦发现队列中有数据(PhantomReference对象)，就找出对应的Runnable，调用它的run方法，释放堆对象heapObject中引用的堆外内存。测试代码如下：
[java][view
 plain](http://blog.csdn.net/aitangyong/article/details/39455229#)[copy](http://blog.csdn.net/aitangyong/article/details/39455229#)
- publicclass Test  
- {  
- privatelong address = 0;  
- 
- public Test()  
-     {  
-         address = GetUsafeInstance.getUnsafeInstance().allocateMemory(  
- 2 * 1024 * 1024);  
-     }  
- 
- publicstaticvoid main(String[] args)  
-     {  
- while (true)  
-         {  
-             Test heap = new Test();  
- 
-             MyOwnCleaner.clear(heap, new FreeMemoryTask(heap.address));  
- 
-             System.gc();  
-         }  
-     }  
- }  
运行测试代码，可以发现也不会报OOM，即正确地实现了堆外内存的自动释放。
