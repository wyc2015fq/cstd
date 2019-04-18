# java线程中的interrupt,isInterrupt,interrupted方法 - z69183787的专栏 - CSDN博客
2014年05月05日 20:19:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：796
在java的线程Thread类中有三个方法，比较容易混淆，在这里解释一下
（1）interrupt：置线程的中断状态
（2）isInterrupt：线程是否中断
（3）interrupted：返回线程的上次的中断状态，并清除中断状态
举个例子：
**[java]**[view
 plain](http://blog.csdn.net/gtuu0123/article/details/6040105#)[copy](http://blog.csdn.net/gtuu0123/article/details/6040105#)
- 用法：  
- class MyThread extends Thread {  
-     ......  
-     ......  
- publicvoid run() {  
- try {  
- while(!Thread.currentThread().isInterrupted()) {  
- //当达到队列容量时，在这里会阻塞
- //put的内部会调用LockSupport.park()这个是用来阻塞线程的方法
- //当其他线程，调用此线程的interrupt()方法时，会设置一个中断标志
- //LockSupport.part()中检测到这个中断标志，会抛出InterruptedException，并清除线程的中断标志
- //因此在异常段调用Thread.currentThread().isInterrupted()返回为false
-                 ArrayBlockingQueue.put(somevalue);   
-             }  
-         } catch (InterruptedException e) {  
- //由于阻塞库函数，如：Object.wait,Thread.sleep除了抛出异常外，还会清除线程中断状态，因此可能在这里要保留线程的中断状态
-             Thread.currentThread().interrupt();  
-         }  
-     }  
- publicvoid cancel() {  
-         interrupt();  
-     }  
- }  
- 外部调用  
- MyThread thread = new MyThread();  
- thread.start();  
- ......  
- thread.cancel();  
- thread.isInterrupted();  
一般来说，阻塞函数，如：Thread.sleep、Thread.join、Object.wait、LockSupport.park等在检查到线程的中断状态时，会抛出InterruptedException，同时会清除线程的中断状态
对于InterruptedException的处理，可以有两种情况：
（1）外层代码可以处理这个异常，直接抛出这个异常即可
（2）如果不能抛出这个异常，比如在run()方法内，因为在得到这个异常的同时，线程的中断状态已经被清除了，需要保留线程的中断状态，则需要调用Thread.currentThread().interrupt()
另外，Thread.interrupted()在jdk库的源代码中比较常用，因为它既可以得到上一次线程的中断标志值，又可以同时清除线程的中断标志，一举两得，但同时也有坏处，就是这个函数有清除中断状态的副作用，不容易理解
