# 使用ThreadLocal解决多线程的并发问题 - z69183787的专栏 - CSDN博客
2014年08月07日 18:03:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2261
早在JDK 1.2的版本中就提供java.lang.ThreadLocal，ThreadLocal为解决多线程程序的并发问题提供了一种新的思路。使用这个工具类可以很简洁地编写出优美的多线程程序。
当使用ThreadLocal维护变量时，ThreadLocal为每个使用该变量的线程提供独立的变量副本，所以每一个线程都可以独立地改变自己的副本，而不会影响其它线程所对应的副本。
从线程的角度看，目标变量就象是线程的本地变量，这也是类名中“Local”所要表达的意思。
ThreadLocal的接口方法
ThreadLocal类接口很简单，只有4个方法，我们先来了解一下：
    * void set(Object value)
设置当前线程的线程局部变量的值。
    * public Object get()
该方法返回当前线程所对应的线程局部变量。
    * public void remove()
将当前线程局部变量的值删除，目的是为了减少内存的占用，该方法是JDK 5.0新增的方法。需要指出的是，当线程结束后，对应该线程的局部变量将自动被垃圾回收，所以显式调用该方法清除线程的局部变量并不是必须的操作，但它可以加快内存回收的速度。
    * protected Object initialValue()
返回该线程局部变量的初始值，该方法是一个protected的方法，显然是为了让子类覆盖而设计的。这个方法是一个延迟调用方法，在线程第1次调用get()或set(Object)时才执行，并且仅执行1次。ThreadLocal中的缺省实现直接返回一个null。
在JDK5.0中，ThreadLocal已经支持泛型，该类的类名已经变为ThreadLocal<T>。API方法也相应进行了调整，新版本的API方法分别是void set(T value)、T get()以及T initialValue()。
ThreadLocal是如何做到为每一个线程维护变量的副本的呢？其实实现的思路很简单：在ThreadLocal类中有一个Map，用于存储每一个线程的变量副本，Map中元素的键为线程对象，而值对应线程的变量副本。
（1）数字生成器
**[java]**[view
 plain](http://blog.csdn.net/heyutao007/article/details/5950759#)[copy](http://blog.csdn.net/heyutao007/article/details/5950759#)
- //数字生成器。该类对象将被多个线程共享。
- publicclass NumCreator implements INumCreator {  
- 
- privateint nextNum;//当前生成的数字    
- 
- public NumCreator() {  
-         nextNum=0;  
-     }  
- publicint getNextNum() {  
- return nextNum++;  
-     }  
- }  
（2）使用数字生成器的线程
**[java]**[view
 plain](http://blog.csdn.net/heyutao007/article/details/5950759#)[copy](http://blog.csdn.net/heyutao007/article/details/5950759#)
- //使用数字生成器对象的线程。
- publicclass DemoThread extends Thread {  
- 
- private INumCreator creator;  
- public DemoThread(NumCreator creator) {  
- this.creator=creator;  
-     }      
- 
- publicvoid run() {  
- //使用数字生成器生成三个数字
- for (int i = 0; i < 3; i++)   
-         {  
-             System.out.println(Thread.currentThread().getName()+ "print num:"+creator.getNextNum());              
-         }          
-     }  
- }  
（3）main函数 
**[java]**[view
 plain](http://blog.csdn.net/heyutao007/article/details/5950759#)[copy](http://blog.csdn.net/heyutao007/article/details/5950759#)
- publicclass DemoMain {  
- publicstaticvoid main(String[] args) {  
- //三个线程共享同一个资源        
-         NumCreator creator=new NumCreator();  
-         DemoThread thread1=new DemoThread(creator);  
-         DemoThread thread2=new DemoThread(creator);  
-         DemoThread thread3=new DemoThread(creator);  
- 
-         thread1.start();  
-         thread2.start();  
-         thread3.start();  
- }  
- }  
可能的结果：
　　　Thread-0print num:0
        Thread-0print num:1
        Thread-0print num:2
        Thread-1print num:3
        Thread-1print num:4
        Thread-1print num:5
        Thread-2print num:6
        Thread-2print num:7
        Thread-2print num:8 
可以看出，每个线程得到的数字是不同的。 因为三个线程共享同一个数字生成器。一个线程的运行会影响其他线程的结果。
修改程序：
（1）自定义ThreadLocal
**[java]**[view
 plain](http://blog.csdn.net/heyutao007/article/details/5950759#)[copy](http://blog.csdn.net/heyutao007/article/details/5950759#)
- publicclass DemoThreadLocal extends ThreadLocal {  
- //重写initialValue
- protected Object initialValue() {  
- // TODO 自动生成方法存根
- returnnew Integer(0);  
-     }  
- }  
（2） 使用ThreadLocal的数字生成器 
**[java]**[view
 plain](http://blog.csdn.net/heyutao007/article/details/5950759#)[copy](http://blog.csdn.net/heyutao007/article/details/5950759#)
- publicclass NumCreator2 implements INumCreator {  
- privatestatic DemoThreadLocal threadLocal=new DemoThreadLocal();  
- 
- public NumCreator2() {  
-     }  
- publicint getNextNum() {          
- int num= ((Integer)threadLocal.get()).intValue();  
-         threadLocal.set(new Integer(num+1));  
- return num;  
-     }  
（3）置换线程的数字生成器 
**[java]**[view
 plain](http://blog.csdn.net/heyutao007/article/details/5950759#)[copy](http://blog.csdn.net/heyutao007/article/details/5950759#)
- //使用数字生成器对象的线程。
- publicclass DemoThread extends Thread {  
- public DemoThread(NumCreator2 creator) {  
- this.creator=creator;  
-     }  
- 
- publicvoid run() {  
- //使用数字生成器生成三个数字
- for (int i = 0; i < 3; i++)   
-         {  
-             System.out.println(Thread.currentThread().getName()+ "print num:"+creator.getNextNum());              
-         }          
-     }  
- }  
（4） 重写main
**[java]**[view
 plain](http://blog.csdn.net/heyutao007/article/details/5950759#)[copy](http://blog.csdn.net/heyutao007/article/details/5950759#)
- publicstaticvoid main(String[] args) {  
- //三个线程共享同一个资源
-         NumCreator2 creator2=new NumCreator2();  
-         DemoThread thread4=new DemoThread(creator2);  
-         DemoThread thread5=new DemoThread(creator2);  
-         DemoThread thread6=new DemoThread(creator2);  
- 
-         thread4.start();  
-         thread5.start();  
-         thread6.start();  
-     }  
 结果：
　　　Thread-0print num:0
        Thread-0print num:1
        Thread-0print num:2
        Thread-1print num:0
        Thread-1print num:1
        Thread-1print num:2
        Thread-2print num:0
        Thread-2print num:1
        Thread-2print num:2 
