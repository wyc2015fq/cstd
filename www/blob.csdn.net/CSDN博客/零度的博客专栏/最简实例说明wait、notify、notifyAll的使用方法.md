# 最简实例说明wait、notify、notifyAll的使用方法 - 零度的博客专栏 - CSDN博客
2016年04月18日 10:22:26[零度anngle](https://me.csdn.net/zmx729618)阅读数：312
                
wait()、notify()、notifyAll()是三个定义在Object类里的方法，可以用来控制线程的状态。
这三个方法最终调用的都是jvm级的native方法。随着jvm运行平台的不同可能有些许差异。
- 如果对象调用了wait方法就会使持有该对象的线程把该对象的控制权交出去，然后处于等待状态。
- 如果对象调用了notify方法就会通知某个正在等待这个对象的控制权的线程可以继续运行。
- 如果对象调用了notifyAll方法就会通知所有等待这个对象控制权的线程继续运行。
其中wait方法有三个over load方法：
wait()
wait(long)
wait(long,int)
wait方法通过参数可以指定等待的时长。如果没有指定参数，默认一直等待直到被通知。
以下是一个演示代码，以最简洁的方式说明复杂的问题：
简要说明下：
NotifyThread是用来模拟3秒钟后通知其他等待状态的线程的线程类；
WaitThread是用来模拟等待的线程类；
等待的中间对象是flag，一个String对象；
main方法中同时启动一个Notify线程和三个wait线程；
Java代码  ![收藏代码](http://longdick.iteye.com/images/icon_star.png)
- publicclass NotifyTest {  
- private  String flag = "true";  
- 
- class NotifyThread extends Thread{  
- public NotifyThread(String name) {  
- super(name);  
-         }  
- publicvoid run() {       
- try {  
-                 sleep(3000);//推迟3秒钟通知
-             } catch (InterruptedException e) {  
-                 e.printStackTrace();  
-             }  
- 
-                 flag = "false";  
-                 flag.notify();  
-         }  
-     };  
- 
- class WaitThread extends Thread {  
- public WaitThread(String name) {  
- super(name);  
-         }  
- 
- publicvoid run() {  
- 
- while (flag!="false") {  
-                     System.out.println(getName() + " begin waiting!");  
- long waitTime = System.currentTimeMillis();  
- try {  
-                         flag.wait();  
-                     } catch (InterruptedException e) {  
-                         e.printStackTrace();  
-                     }  
-                     waitTime = System.currentTimeMillis() - waitTime;  
-                     System.out.println("wait time :"+waitTime);  
-                 }  
-                 System.out.println(getName() + " end waiting!");  
- 
-         }  
-     }  
- 
- publicstaticvoid main(String[] args) throws InterruptedException {  
-         System.out.println("Main Thread Run!");  
-         NotifyTest test = new NotifyTest();  
-         NotifyThread notifyThread =test.new NotifyThread("notify01");  
-         WaitThread waitThread01 = test.new WaitThread("waiter01");  
-         WaitThread waitThread02 = test.new WaitThread("waiter02");  
-         WaitThread waitThread03 = test.new WaitThread("waiter03");  
-         notifyThread.start();  
-         waitThread01.start();  
-         waitThread02.start();  
-         waitThread03.start();  
-     }  
- 
- }  
OK，如果你拿这段程序去运行下的话， 会发现根本运行不了，what happened？满屏的java.lang.IllegalMonitorStateException。
没错，这段程序有很多问题，我们一个个来看。
首先，这儿要非常注意的几个事实是
- 任何一个时刻，对象的控制权（monitor）只能被一个线程拥有。
- 无论是执行对象的wait、notify还是notifyAll方法，必须保证当前运行的线程取得了该对象的控制权（monitor）
- 如果在没有控制权的线程里执行对象的以上三种方法，就会报java.lang.IllegalMonitorStateException异常。
- JVM基于多线程，默认情况下不能保证运行时线程的时序性
基于以上几点事实，我们需要确保让线程拥有对象的控制权。
也就是说在waitThread中执行wait方法时，要保证waitThread对flag有控制权；
在notifyThread中执行notify方法时，要保证notifyThread对flag有控制权。
线程取得控制权的方法有三：
- 执行对象的某个同步实例方法。
- 执行对象对应类的同步静态方法。
- 执行对该对象加同步锁的同步块。
我们用第三种方法来做说明：
将以上notify和wait方法包在同步块中
Java代码  ![收藏代码](http://longdick.iteye.com/images/icon_star.png)
- synchronized (flag) {  
-                 flag = "false";  
-                 flag.notify();  
-             }  
Java代码  ![收藏代码](http://longdick.iteye.com/images/icon_star.png)
- synchronized (flag) {  
- while (flag!="false") {  
-                     System.out.println(getName() + " begin waiting!");  
- long waitTime = System.currentTimeMillis();  
- try {  
-                         flag.wait();  
-                     } catch (InterruptedException e) {  
-                         e.printStackTrace();  
-                     }  
-                     waitTime = System.currentTimeMillis() - waitTime;  
-                     System.out.println("wait time :"+waitTime);  
-                 }  
-                 System.out.println(getName() + " end waiting!");  
-             }  
我们向前进了一步。
问题解决了吗？
好像运行还是报错java.lang.IllegalMonitorStateException。what happened？
这时的异常是由于在针对flag对象同步块中，更改了flag对象的状态所导致的。如下：
flag="false";
flag.notify();
对在同步块中对flag进行了赋值操作，使得flag引用的对象改变，这时候再调用notify方法时，因为没有控制权所以抛出异常。
我们可以改进一下，将flag改成一个JavaBean，然后更改它的属性不会影响到flag的引用。
我们这里改成数组来试试，也可以达到同样的效果：
Java代码  ![收藏代码](http://longdick.iteye.com/images/icon_star.png)
- private   String flag[] = {"true"};  
Java代码  ![收藏代码](http://longdick.iteye.com/images/icon_star.png)
- synchronized (flag) {  
-             flag[0] = "false";  
-             flag.notify();  
-         }  
Java代码  ![收藏代码](http://longdick.iteye.com/images/icon_star.png)
- synchronized (flag) {  
-                 flag[0] = "false";  
-                 flag.notify();  
-             }synchronized (flag) {  
- while (flag[0]!="false") {  
-                     System.out.println(getName() + " begin waiting!");  
- long waitTime = System.currentTimeMillis();  
- try {  
-                         flag.wait();  
- 
-                     } catch (InterruptedException e) {  
-                         e.printStackTrace();  
-                     }  
这时候再运行，不再报异常，但是线程没有结束是吧，没错，还有线程堵塞，处于wait状态。
原因很简单，我们有三个wait线程，只有一个notify线程，notify线程运行notify方法的时候，是随机通知一个正在等待的线程，所以，现在应该还有两个线程在waiting。
我们只需要将NotifyThread线程类中的flag.notify()方法改成notifyAll()就可以了。notifyAll方法会通知所有正在等待对象控制权的线程。
最终完成版如下：
Java代码  ![收藏代码](http://longdick.iteye.com/images/icon_star.png)
- publicclass NotifyTest {  
- private String flag[] = { "true" };  
- 
- class NotifyThread extends Thread {  
- public NotifyThread(String name) {  
- super(name);  
-         }  
- 
- publicvoid run() {  
- try {  
-                 sleep(3000);  
-             } catch (InterruptedException e) {  
-                 e.printStackTrace();  
-             }  
- synchronized (flag) {  
-                 flag[0] = "false";  
-                 flag.notifyAll();  
-             }  
-         }  
-     };  
- 
- class WaitThread extends Thread {  
- public WaitThread(String name) {  
- super(name);  
-         }  
- 
- publicvoid run() {  
- synchronized (flag) {  
- while (flag[0] != "false") {  
-                     System.out.println(getName() + " begin waiting!");  
- long waitTime = System.currentTimeMillis();  
- try {  
-                         flag.wait();  
- 
-                     } catch (InterruptedException e) {  
-                         e.printStackTrace();  
-                     }  
-                     waitTime = System.currentTimeMillis() - waitTime;  
-                     System.out.println("wait time :" + waitTime);  
-                 }  
-                 System.out.println(getName() + " end waiting!");  
-             }  
-         }  
-     }  
- 
- publicstaticvoid main(String[] args) throws InterruptedException {  
-         System.out.println("Main Thread Run!");  
-         NotifyTest test = new NotifyTest();  
-         NotifyThread notifyThread = test.new NotifyThread("notify01");  
-         WaitThread waitThread01 = test.new WaitThread("waiter01");  
-         WaitThread waitThread02 = test.new WaitThread("waiter02");  
-         WaitThread waitThread03 = test.new WaitThread("waiter03");  
-         notifyThread.start();  
-         waitThread01.start();  
-         waitThread02.start();  
-         waitThread03.start();  
-     }  
- 
- }  
