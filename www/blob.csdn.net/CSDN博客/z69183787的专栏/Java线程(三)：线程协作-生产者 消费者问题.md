# Java线程(三)：线程协作-生产者/消费者问题 - z69183787的专栏 - CSDN博客
2014年06月05日 16:57:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：592
个人分类：[多线程-专栏](https://blog.csdn.net/z69183787/article/category/6816123)

        上一篇讲述了线程的互斥(同步)，但是在很多情况下，仅仅同步是不够的，还需要线程与线程协作(通信)，生产者/消费者问题是一个经典的线程同步以及通信的案例。该问题描述了两个共享固定大小缓冲区的线程，即所谓的“生产者”和“消费者”在实际运行时会发生的问题。生产者的主要作用是生成一定量的数据放到缓冲区中，然后重复此过程。与此同时，消费者也在缓冲区消耗这些数据。该问题的关键就是要保证生产者不会在缓冲区满时加入数据，消费者也不会在缓冲区中空时消耗数据。要解决该问题，就必须让生产者在缓冲区满时休眠（要么干脆就放弃数据），等到下次消费者消耗缓冲区中的数据的时候，生产者才能被唤醒，开始往缓冲区添加数据。同样，也可以让消费者在缓冲区空时进入休眠，等到生产者往缓冲区添加数据之后，再唤醒消费者，通常采用线程间通信的方法解决该问题。如果解决方法不够完善，则容易出现死锁的情况。出现死锁时，两个线程都会陷入休眠，等待对方唤醒自己。该问题也能被推广到多个生产者和消费者的情形。本文讲述了JDK5之前传统线程的通信方式，更高级的通信方式可参见[Java线程(九)：Condition-线程通信更高效的方式](http://blog.csdn.net/ghsau/article/details/7481142)和[Java线程(篇外篇)：阻塞队列BlockingQueue](http://blog.csdn.net/ghsau/article/details/8108292)。
        假设有这样一种情况，有一个盘子，盘子里只能放一个鸡蛋，A线程专门往盘子里放鸡蛋，如果盘子里有鸡蛋，则一直等到盘子里没鸡蛋，B线程专门从盘子里取鸡蛋，如果盘子里没鸡蛋，则一直等到盘子里有鸡蛋。这里盘子是一个互斥区，每次放鸡蛋是互斥的，每次取鸡蛋也是互斥的，A线程放鸡蛋，如果这时B线程要取鸡蛋，由于A没有释放锁，B线程处于等待状态，进入阻塞队列，放鸡蛋之后，要通知B线程取鸡蛋，B线程进入就绪队列，反过来，B线程取鸡蛋，如果A线程要放鸡蛋，由于B线程没有释放锁，A线程处于等待状态，进入阻塞队列，取鸡蛋之后，要通知A线程放鸡蛋，A线程进入就绪队列。我们希望当盘子里有鸡蛋时，A线程阻塞，B线程就绪，盘子里没鸡蛋时，A线程就绪，B线程阻塞，代码如下：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/7433673#)[copy](http://blog.csdn.net/ghsau/article/details/7433673#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/125318/fork)
- import java.util.ArrayList;  
- import java.util.List;  
- /** 定义一个盘子类，可以放鸡蛋和取鸡蛋 */
- publicclass Plate {  
- /** 装鸡蛋的盘子 */
-     List<Object> eggs = new ArrayList<Object>();  
- /** 取鸡蛋 */
- publicsynchronized Object getEgg() {  
- while (eggs.size() == 0) {  
- try {  
-                 wait();  
-             } catch (InterruptedException e) {  
-                 e.printStackTrace();  
-             }  
-         }  
-         Object egg = eggs.get(0);  
-         eggs.clear();// 清空盘子
-         notify();// 唤醒阻塞队列的某线程到就绪队列
-         System.out.println("拿到鸡蛋");  
- return egg;  
-     }  
- /** 放鸡蛋 */
- publicsynchronizedvoid putEgg(Object egg) {  
- while (eggs.size() > 0) {  
- try {  
-                 wait();  
-             } catch (InterruptedException e) {  
-                 e.printStackTrace();  
-             }  
-         }  
-         eggs.add(egg);// 往盘子里放鸡蛋
-         notify();// 唤醒阻塞队列的某线程到就绪队列
-         System.out.println("放入鸡蛋");  
-     }  
- staticclass AddThread implements Runnable  {  
- private Plate plate;  
- private Object egg = new Object();  
- public AddThread(Plate plate) {  
- this.plate = plate;  
-         }  
- publicvoid run() {  
-             plate.putEgg(egg);  
-         }  
-     }  
- staticclass GetThread implements Runnable  {  
- private Plate plate;  
- public GetThread(Plate plate) {  
- this.plate = plate;  
-         }  
- publicvoid run() {  
-             plate.getEgg();  
-         }  
-     }  
- publicstaticvoid main(String args[]) {  
-         Plate plate = new Plate();  
- for(int i = 0; i < 10; i++) {  
- new Thread(new AddThread(plate)).start();  
- new Thread(new GetThread(plate)).start();  
-         }  
-     }  
- }  
        输出结果：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/7433673#)[copy](http://blog.csdn.net/ghsau/article/details/7433673#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/125318/fork)
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
- 放入鸡蛋  
- 拿到鸡蛋  
        程序开始，A线程判断盘子是否为空，放入一个鸡蛋，并且唤醒在阻塞队列的一个线程，阻塞队列为空；假设CPU又调度了一个A线程，盘子非空，执行等待，这个A线程进入阻塞队列；然后一个B线程执行，盘子非空，取走鸡蛋，并唤醒阻塞队列的A线程，A线程进入就绪队列，此时就绪队列就一个A线程，马上执行，放入鸡蛋；如果再来A线程重复第一步，在来B线程重复第二步，整个过程就是生产者(A线程)生产鸡蛋，消费者(B线程)消费鸡蛋。
        前段时间看了张孝祥老师线程的视频，讲述了一个其学员的面试题，也是线程通信的，在此也分享一下。
        题目：子线程循环10次，主线程循环100次，如此循环100次，好像是空中网的笔试题。
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/7433673#)[copy](http://blog.csdn.net/ghsau/article/details/7433673#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/125318/fork)
- publicclass ThreadTest2 {  
- publicstaticvoid main(String[] args) {  
- final Business business = new Business();  
- new Thread(new Runnable() {  
- @Override
- publicvoid run() {  
-                 threadExecute(business, "sub");  
-             }  
-         }).start();  
-         threadExecute(business, "main");  
-     }     
- publicstaticvoid threadExecute(Business business, String threadType) {  
- for(int i = 0; i < 100; i++) {  
- try {  
- if("main".equals(threadType)) {  
-                     business.main(i);  
-                 } else {  
-                     business.sub(i);  
-                 }  
-             } catch (InterruptedException e) {  
-                 e.printStackTrace();  
-             }  
-         }  
-     }  
- }  
- class Business {  
- privateboolean bool = true;  
- publicsynchronizedvoid main(int loop) throws InterruptedException {  
- while(bool) {  
- this.wait();  
-         }  
- for(int i = 0; i < 100; i++) {  
-             System.out.println("main thread seq of " + i + ", loop of " + loop);  
-         }  
-         bool = true;  
- this.notify();  
-     }     
- publicsynchronizedvoid sub(int loop) throws InterruptedException {  
- while(!bool) {  
- this.wait();  
-         }  
- for(int i = 0; i < 10; i++) {  
-             System.out.println("sub thread seq of " + i + ", loop of " + loop);  
-         }  
-         bool = false;  
- this.notify();  
-     }  
- }  
       大家注意到没有，在调用wait方法时，都是用while判断条件的，而不是if，在wait方法说明中，也推荐使用while，因为在某些特定的情况下，线程有可能被假唤醒，使用while会循环检测更稳妥。wait和notify方法必须工作于synchronized内部，且这两个方法只能由锁对象来调用。另附这两种方法的JavaDoc说明：
### notify
public final void **notify**()唤醒在此对象监视器上等待的单个线程。如果所有线程都在此对象上等待，则会选择唤醒其中一个线程。选择是任意性的，并在对实现做出决定时发生。线程通过调用其中一个`wait` 方法，在对象的监视器上等待。
直到当前的线程放弃此对象上的锁定，才能继续执行被唤醒的线程。被唤醒的线程将以常规方式与在该对象上主动同步的其他所有线程进行竞争；例如，唤醒的线程在作为锁定此对象的下一个线程方面没有可靠的特权或劣势。
此方法只应由作为此对象监视器的所有者的线程来调用。通过以下三种方法之一，线程可以成为此对象监视器的所有者：
- 通过执行此对象的同步 (Sychronized) 实例方法。
- 通过执行在此对象上进行同步的 `synchronized` 语句的正文。
- 对于 `Class` 类型的对象，可以通过执行该类的同步静态方法。
一次只能有一个线程拥有对象的监视器。
- **抛出：**
`[IllegalMonitorStateException](http://blog.csdn.net/ghsau/article/details/7433673)` - 如果当前的线程不是此对象监视器的所有者。
### notifyAll
public final void **notifyAll**()唤醒在此对象监视器上等待的所有线程。线程通过调用其中一个 `wait` 方法，在对象的监视器上等待。
直到当前的线程放弃此对象上的锁定，才能继续执行被唤醒的线程。被唤醒的线程将以常规方式与在该对象上主动同步的其他所有线程进行竞争；例如，唤醒的线程在作为锁定此对象的下一个线程方面没有可靠的特权或劣势。
此方法只应由作为此对象监视器的所有者的线程来调用。请参阅 `notify` 方法，了解线程能够成为监视器所有者的方法的描述。
- **抛出：**
`[IllegalMonitorStateException](http://blog.csdn.net/ghsau/article/details/7433673)` - 如果当前的线程不是此对象监视器的所有者。
### wait
public final void **wait**(long timeout)
                throws [InterruptedException](http://blog.csdn.net/ghsau/article/details/7433673)导致当前的线程等待，直到其他线程调用此对象的 [`notify()`](http://blog.csdn.net/ghsau/article/details/7433673) 方法或[`notifyAll()`](http://blog.csdn.net/ghsau/article/details/7433673) 方法，或者超过指定的时间量。
当前的线程必须拥有此对象监视器。
此方法导致当前线程（称之为 T）将其自身放置在对象的等待集中，然后放弃此对象上的所有同步要求。出于线程调度目的，线程 T 被禁用，且处于休眠状态，直到发生以下四种情况之一：
- 其他某个线程调用此对象的 notify 方法，并且线程 T 碰巧被任选为被唤醒的线程。
- 其他某个线程调用此对象的 notifyAll 方法。
- 其他某个线程[`中断`](http://blog.csdn.net/ghsau/article/details/7433673)线程 T。
- 已经到达指定的实际时间。但是，如果 timeout 为零，则不考虑实际时间，该线程将一直等待，直到获得通知。
然后，从对象的等待集中删除线程 T，并重新进行线程调度。然后，该线程以常规方式与其他线程竞争，以获得在该对象上同步的权利；一旦获得对该对象的控制权，该对象上的所有其同步声明都将被还原到以前的状态 - 这就是调用wait 方法时的情况。然后，线程T 从wait 方法的调用中返回。所以，从wait 方法返回时，该对象和线程T 的同步状态与调用wait 方法时的情况完全相同。
在没有被通知、中断或超时的情况下，线程还可以唤醒一个所谓的*虚假唤醒* (spurious wakeup)。虽然这种情况在实践中很少发生，但是应用程序必须通过以下方式防止其发生，即对应该导致该线程被提醒的条件进行测试，如果不满足该条件，则继续等待。换句话说，等待应总是发生在循环中，如下面的示例：
synchronized (obj) {
while (<condition does not hold>)
obj.wait(timeout);
... // Perform action appropriate to condition
     }
 
（有关这一主题的更多信息，请参阅 Doug Lea 撰写的《Concurrent Programming in Java (Second Edition)》(Addison-Wesley, 2000) 中的第 3.2.3 节或 Joshua Bloch 撰写的《Effective Java Programming Language Guide》(Addison-Wesley, 2001) 中的第 50 项。
如果当前线程在等待时被其他线程[`中断`](http://blog.csdn.net/ghsau/article/details/7433673)，则会抛出InterruptedException。在按上述形式恢复此对象的锁定状态时才会抛出此异常。
注意，由于 wait 方法将当前的线程放入了对象的等待集中，所以它只能解除此对象的锁定；可以同步当前线程的任何其他对象在线程等待时仍处于锁定状态。
此方法只应由作为此对象监视器的所有者的线程来调用。请参阅 `notify` 方法，了解线程能够成为监视器所有者的方法的描述。
- **参数：**
`timeout` - 要等待的最长时间（以毫秒为单位）。- **抛出：**
`[IllegalArgumentException](http://blog.csdn.net/ghsau/article/details/7433673)` - 如果超时值为负。`[IllegalMonitorStateException](http://blog.csdn.net/ghsau/article/details/7433673)` - 如果当前的线程不是此对象监视器的所有者。`[InterruptedException](http://blog.csdn.net/ghsau/article/details/7433673)` - 如果在当前线程等待通知之前或者正在等待通知时，另一个线程中断了当前线程。在抛出此异常时，当前线程的*中断状态* 被清除。
### wait
public final void **wait**(long timeout,
                       int nanos)
                throws [InterruptedException](http://blog.csdn.net/ghsau/article/details/7433673)导致当前的线程等待，直到其他线程调用此对象的 [`notify()`](http://blog.csdn.net/ghsau/article/details/7433673) 方法或[`notifyAll()`](http://blog.csdn.net/ghsau/article/details/7433673) 方法，或者其他某个线程中断当前线程，或者已超过某个实际时间量。
此方法类似于一个参数的 `wait` 方法，但它允许更好地控制在放弃之前等待通知的时间量。用毫微秒度量的实际时间量可以通过以下公式计算出来：
> 1000000*timeout+nanos
在其他所有方面，此方法执行的操作与带有一个参数的 [`wait(long)`](http://blog.csdn.net/ghsau/article/details/7433673) 方法相同。需要特别指出的是，wait(0, 0) 与wait(0) 相同。
当前的线程必须拥有此对象监视器。该线程发布对此监视器的所有权，并等待下面两个条件之一发生：
- 其他线程通过调用 `notify` 方法，或 `notifyAll` 方法通知在此对象的监视器上等待的线程醒来。
- `timeout` 毫秒值与 `nanos` 毫微秒参数值之和指定的超时时间已用完。
然后，该线程等到重新获得对监视器的所有权后才能继续执行。
对于某一个参数的版本，实现中断和虚假唤醒是有可能的，并且此方法应始终在循环中使用：
synchronized (obj) {
while (<condition does not hold>)
obj.wait(timeout, nanos);
... // Perform action appropriate to condition
     }
 
此方法只应由作为此对象监视器的所有者的线程来调用。请参阅 `notify` 方法，了解线程能够成为监视器所有者的方法的描述。
- **参数：**
`timeout` - 要等待的最长时间（以毫秒为单位）。`nanos` - 额外时间（以毫微秒为单位，范围是 0-999999）。- **抛出：**
`[IllegalArgumentException](http://blog.csdn.net/ghsau/article/details/7433673)` - 如果超时值是负数，或者毫微秒值不在 0-999999 范围内。`[IllegalMonitorStateException](http://blog.csdn.net/ghsau/article/details/7433673)` - 如果当前线程不是此对象监视器的所有者。`[InterruptedException](http://blog.csdn.net/ghsau/article/details/7433673)` - 如果在当前线程等待通知之前或者正在等待通知时，其他线程中断了当前线程。在抛出此异常时，当前线程的*中断状态* 被清除。
### wait
public final void **wait**()
                throws [InterruptedException](http://blog.csdn.net/ghsau/article/details/7433673)导致当前的线程等待，直到其他线程调用此对象的 [`notify()`](http://blog.csdn.net/ghsau/article/details/7433673) 方法或[`notifyAll()`](http://blog.csdn.net/ghsau/article/details/7433673) 方法。换句话说，此方法的行为就好像它仅执行wait(0) 调用一样。
当前的线程必须拥有此对象监视器。该线程发布对此监视器的所有权并等待，直到其他线程通过调用`notify` 方法，或 `notifyAll` 方法通知在此对象的监视器上等待的线程醒来。然后该线程将等到重新获得对监视器的所有权后才能继续执行。
对于某一个参数的版本，实现中断和虚假唤醒是可能的，而且此方法应始终在循环中使用：
synchronized (obj) {
while (<condition does not hold>)
obj.wait();
... // Perform action appropriate to condition
     }
 
此方法只应由作为此对象监视器的所有者的线程来调用。请参阅 `notify` 方法，了解线程能够成为监视器所有者的方法的描述。
- **抛出：**
`[IllegalMonitorStateException](http://blog.csdn.net/ghsau/article/details/7433673)` - 如果当前的线程不是此对象监视器的所有者。`[InterruptedException](http://blog.csdn.net/ghsau/article/details/7433673)` - 如果在当前线程等待通知之前或者正在等待通知时，另一个线程中断了当前线程。在抛出此异常时，当前线程的*中断状态* 被清除。 
        本文来自：[高爽|Coder](http://blog.csdn.net/ghsau)，原文地址：[http://blog.csdn.net/ghsau/article/details/7433673](http://blog.csdn.net/ghsau/article/details/7433673)，转载请注明。
