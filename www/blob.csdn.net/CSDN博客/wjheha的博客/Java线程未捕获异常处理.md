# Java线程未捕获异常处理 - wjheha的博客 - CSDN博客
2017年05月21日 19:22:05[wjheha](https://me.csdn.net/wjheha)阅读数：918
原文地址：http://blog.csdn.net/hongxingxiaonan/article/details/50527169
# Java线程未捕获异常处理
线程执行完run方法即可正常退出，如果发生了没有捕获的异常则会异常退出。大多数情况下，由于异常导致的线程退出都不是我们想要的。所以在编写的代码的时候要尽可能的捕获处理可以处理的异常，但是也不能光简单的捕获异常然后什么也不做。下面介绍线程异常的处理办法。
   JVM为我们提供了线程的未捕获异常处理机制，通过Thread的setUncaughtExceptionHandler方法：
               public void setUncaughtExceptionHandler(UncaughtExceptionHandler eh)
   可以设置当前线程的未捕获异常处理器。如下面的例子就通过设置uncaughtExceptionHandler成功捕获到了除0异常：
**[java]**[view
 plain](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)[copy](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)
- publicstaticvoid main(String[] args) throws InterruptedException {                  
- 
-     Thread t = new Thread(new UncaughtException.Run());  
-     t.setUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {  
- 
- @Override
- publicvoid uncaughtException(Thread t, Throwable e) {  
-             System.out.println("uncaughtExceptionHandler catch a Exception---------");  
-             System.out.println(e.getMessage());  
-         }  
-     });  
- 
-     t.start();  
-     Thread.sleep(100);  
- }  
- 
- staticclass Run implements Runnable{  
- @Override
- publicvoid run() {  
-         System.out.println("runnable run---------------");        
- int i = 1/0;  
-     }  
- }  
结果：
**[java]**[view
 plain](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)[copy](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)
- runnable run---------------  
- uncaughtExceptionHandler catch a Exception---------  
- / by zero  
     线程出现未捕获异常后，JVM将调用Thread中的dispatchUncaughtException方法把异常传递给线程的未捕获异常处理器。
**[java]**[view
 plain](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)[copy](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)
- /**
-  * Dispatch an uncaught exception to the handler. This method is
-  * intended to be called only by the JVM.
-  */
- privatevoid dispatchUncaughtException(Throwable e) {  
-     getUncaughtExceptionHandler().uncaughtException(this, e);  
- }  
    方法的描述已经清楚地说明了这个方法只是提供给JVM调用的，getUncaughtExceptionHandler方法并没有简单返回设置好的uncaughtExceptionHandler：
**[java]**[view
 plain](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)[copy](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)
- public UncaughtExceptionHandler getUncaughtExceptionHandler() {  
- return uncaughtExceptionHandler != null ?  
-         uncaughtExceptionHandler : group;  
- }  
   可见，如果没有设置uncaughtExceptionHandler，将使用线程所在的线程组来处理这个未捕获异常。线程组ThreadGroup实现了UncaughtExceptionHandler，所以可以用来处理未捕获异常。ThreadGroup类定义：
**[java]**[view
 plain](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)[copy](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)
- class ThreadGroup implements Thread.UncaughtExceptionHandler  
   ThreadGroup实现的uncaughtException如下：
**[java]**[view
 plain](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)[copy](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)
- publicvoid uncaughtException(Thread t, Throwable e) {  
- if (parent != null) {  
-         parent.uncaughtException(t, e);  
-     } else {  
-         Thread.UncaughtExceptionHandler ueh =  
-             Thread.getDefaultUncaughtExceptionHandler();  
- if (ueh != null) {  
-             ueh.uncaughtException(t, e);  
-         } elseif (!(e instanceof ThreadDeath)) {  
-             System.err.print("Exception in thread \""
-                              + t.getName() + "\" ");  
-             e.printStackTrace(System.err);  
-         }  
-     }  
- }  
     默认情况下，线程组处理未捕获异常的逻辑是，首先将异常消息通知给父线程组，然后尝试利用一个默认的defaultUncaughtExceptionHandler来处理异常，如果没有默认的异常处理器则将错误信息输出到System.err。也就是JVM提供给我们设置每个线程的具体的未捕获异常处理器，也提供了设置默认异常处理器的方法。
     设置了默认的异常处理器后，系统中所有未直接设置异常处理器的线程将使用这个默认的异常处理器。
**[java]**[view
 plain](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)[copy](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)
- publicvoid defaultWay(){  
-     Thread.setDefaultUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {  
- @Override
- publicvoid uncaughtException(Thread t, Throwable e) {  
-             System.out.println("I catch a exception from  " + Thread.currentThread().getName() + ":" + Thread.currentThread().getThreadGroup().getName());  
-         }  
-     });  
- 
-     ThreadGroup myGroup = new ThreadGroup("myGroup");  
- new Thread(myGroup, new Runnable() {  
- @Override
- publicvoid run() {  
- int i = 1/0;  
-                 }  
-             }, "thread1").start();  
- 
- new Thread(myGroup, new Runnable() {  
- @Override
- publicvoid run() {  
- int i = 1/0;  
-         }  
-     }, "thread2").start();  
- 
- }  
   这段代码创建了两个线程，并且它们都会抛出异常，最终由统一的默认异常处理器来处理。结果：
**[java]**[view
 plain](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)[copy](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)
- I catch a exception from  thread1:myGroup  
- I catch a exception from  thread2:myGroup  
    当然，出现上面的结果是因为使用了默认的ThreadGroup，我们可以破坏它这个机制。如果把上面代码中的ThreadGroup换成下面的BadGroup则情况会发生变化：
**[java]**[view
 plain](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)[copy](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)
- class BadGroup extends ThreadGroup{  
- public BadGroup(String name) {  
- super(name);  
-     }  
- 
- @Override
- publicvoid uncaughtException(Thread t, Throwable e) {  
-         System.out.println("I am a bad group and do nothing");  
-     }  
- }  
   如果使用了BadGroup得出结果将是打印两条I am a bad group and do nothing。
   上面的例子中，不论是ThreadGroup或者BadGroup都主动的给线程设置了线程组，那么如果不给线程设置线程组会怎么样呢？还会正常的使用默认异常处理器吗？这些跟线程组的来源相关，先看一个例子：
**[java]**[view
 plain](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)[copy](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)
- publicvoid mainGroup() throws InterruptedException {  
-     Thread.setDefaultUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {  
- @Override
- publicvoid uncaughtException(Thread t, Throwable e) {  
-             System.out.println("I catch a exception from  " + Thread.currentThread().getName() + ":" + Thread.currentThread().getThreadGroup().getName());  
-         }  
-     });  
- new Thread(new Runnable() {  
- @Override
- publicvoid run() {  
-             System.out.println(Thread.currentThread().getName() + " said my thread group is " + Thread.currentThread().getThreadGroup().getName());  
- new Thread(new Runnable() {  
- @Override
- publicvoid run() {  
-                      System.out.println(Thread.currentThread().getName() + " said my thread group is " + Thread.currentThread().getThreadGroup().getName());  
- int i = 1/0;  
-                  }  
-              }, "thread2").start();  
-         }  
-     }, "thread1").start();  
- 
-     Thread.sleep(10);  
- }  
     这个方法中，首先创建启动了一个线程，然后又在这个线程中创建启动了另一个线程，它们都没有主动设置线程组。然而，它们都是有线程组的，其执行的结果如下：
**[java]**[view
 plain](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)[copy](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)
- thread1 said my thread group is main  
- thread2 said my thread group is main  
- I catch a exception from  thread2:main  
    虚拟机执行main方法的线程属于一个名字叫做“main”的线程组。在应用程序中，创建一个线程的时候如果没有重新制定线程组，则会继承这个“main”线程组。也就是说所有的线程都会有线程组，即使我们在构造线程时显示的传入null值的线程组，最终JVM也会为我们分配到一个线程组。Thread的init方法决定了这个特性：
**[java]**[view
 plain](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)[copy](http://blog.csdn.net/hongxingxiaonan/article/details/50527169#)
- if (g == null) {  
- /* Determine if it's an applet or not */
- 
- /* If there is a security manager, ask the security manager
-        what to do. */
- if (security != null) {  
-         g = security.getThreadGroup();  
-     }  
- 
- /* If the security doesn't have a strong opinion of the matter
-        use the parent thread group. */
- if (g == null) {  
-         g = parent.getThreadGroup();  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
