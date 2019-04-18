# Java线程(四)：线程中断、线程让步、线程睡眠、线程合并 - z69183787的专栏 - CSDN博客
2017年03月24日 14:26:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：739
      本文来自：[高爽|Coder](http://blog.csdn.net/ghsau)，原文地址：[http://blog.csdn.net/ghsau/article/details/17560467](http://blog.csdn.net/ghsau/article/details/17560467)，转载请注明。
       最近在Review线程专栏，修改了诸多之前描述不够严谨的地方，凡是带有Review标记的文章都是修改过了。本篇文章是插进来的，因为原来没有写，现在来看传统线程描述的不太完整，所以就补上了。理解了线程同步和线程通信之后，再来看本文的知识点就会简单的多了，本文是做为传统线程知识点的一个补充。有人会问：JDK5之后有了更完善的处理多线程问题的类(并发包)，我们还需要去了解传统线程吗？答：需要。在实际开发中，无外乎两种情况，一个是开发新内容，另一个是维护原有程序。开发新内容可以使用新的技术手段，但是我们不能保证原有程序是用什么实现的，所以我们需要了解原有的。另外一点，了解传统线程的工作原理，使我们在使用并发包时更加得心应手。
# 线程中断
       线程中断涉及到三个方法，如下：
|`void`|`**[interrupt](http://blog.csdn.net/ghsau/article/details/17560467)**()`          中断线程。|
|----|----|
|`static boolean`|`**[interrupted](http://blog.csdn.net/ghsau/article/details/17560467)**()`          测试当前线程是否已经中断。|
|`boolean`|`**[isInterrupted](http://blog.csdn.net/ghsau/article/details/17560467)**()`          测试线程是否已经中断。|
       interrupt()方法用于中断线程，通常的理解来看，只要某个线程启动后，调用了该方法，则该线程不能继续执行了，来看个小例子：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/17560467#)[copy](http://blog.csdn.net/ghsau/article/details/17560467#)
[print](http://blog.csdn.net/ghsau/article/details/17560467#)[?](http://blog.csdn.net/ghsau/article/details/17560467#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/129594/fork)
- publicclass InterruptTest {  
- publicstaticvoid main(String[] args) throws InterruptedException {  
-         MyThread t = new MyThread("MyThread");  
-         t.start();  
-         Thread.sleep(100);// 睡眠100毫秒
-         t.interrupt();// 中断t线程
-     }  
- }  
- class MyThread extends Thread {  
- int i = 0;  
- public MyThread(String name) {  
- super(name);  
-     }  
- publicvoid run() {  
- while(true) {// 死循环，等待被中断
-             System.out.println(getName() + getId() + "执行了" + ++i + "次");  
-         }  
-     }  
- }  
       运行后，我们发现，线程t一直在执行，没有被中断，原来interrupt()是骗人的，汗！其实interrupt()方法并不是中断线程的执行，而是为调用该方法的线程对象打上一个标记，设置其中断状态为true，通过isInterrupted()方法可以得到这个线程状态，我们将上面的程序做一个小改动：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/17560467#)[copy](http://blog.csdn.net/ghsau/article/details/17560467#)
[print](http://blog.csdn.net/ghsau/article/details/17560467#)[?](http://blog.csdn.net/ghsau/article/details/17560467#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/129594/fork)
- publicclass InterruptTest {  
- publicstaticvoid main(String[] args) throws InterruptedException {  
-         MyThread t = new MyThread("MyThread");  
-         t.start();  
-         Thread.sleep(100);// 睡眠100毫秒
-         t.interrupt();// 中断t线程
-     }  
- }  
- class MyThread extends Thread {  
- int i = 0;  
- public MyThread(String name) {  
- super(name);  
-     }  
- publicvoid run() {  
- while(!isInterrupted()) {// 当前线程没有被中断，则执行
-             System.out.println(getName() + getId() + "执行了" + ++i + "次");  
-         }  
-     }  
- }  
       这样的话，线程被顺利的中断执行了。很多人实现一个线程类时，都会再加一个flag标记，以便控制线程停止执行，其实完全没必要，通过线程自身的中断状态，就可以完美实现该功能。如果线程在调用 Object 类的 wait()、wait(long) 或 wait(long, int) 方法，或者该类的 join()、join(long)、join(long, int)、sleep(long)
 或 sleep(long, int) 方法过程中受阻，则其中断状态将被清除，它还将收到一个 InterruptedException。 我们可以捕获该异常，并且做一些处理。另外，Thread.interrupted()方法是一个静态方法，它是判断当前线程的中断状态，需要注意的是，线程的中断状态会由该方法清除。换句话说，如果连续两次调用该方法，则第二次调用将返回 false（在第一次调用已清除了其中断状态之后，且第二次调用检验完中断状态前，当前线程再次中断的情况除外）。
# 线程让步
       线程让步，其方法如下：
|`static void`|`**[yield](http://blog.csdn.net/ghsau/article/details/17560467)**()`          暂停当前正在执行的线程对象，并执行其他线程|
       线程让步用于正在执行的线程，在某些情况下让出CPU资源，让给其它线程执行，来看一个小例子：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/17560467#)[copy](http://blog.csdn.net/ghsau/article/details/17560467#)
[print](http://blog.csdn.net/ghsau/article/details/17560467#)[?](http://blog.csdn.net/ghsau/article/details/17560467#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/129594/fork)
- publicclass YieldTest {  
- publicstaticvoid main(String[] args) throws InterruptedException {  
- // 创建线程对象
-         YieldThread t1 = new YieldThread("t1");  
-         YieldThread t2 = new YieldThread("t2");  
- // 启动线程
-         t1.start();  
-         t2.start();  
- // 主线程休眠100毫秒
-         Thread.sleep(100);  
- // 终止线程
-         t1.interrupt();  
-         t2.interrupt();  
-     }  
- }  
- class YieldThread extends Thread {  
- int i = 0;  
- public YieldThread(String name) {  
- super(name);  
-     }  
- publicvoid run() {  
- while(!isInterrupted()) {  
-             System.out.println(getName() + "执行了" + ++i + "次");  
- if(i % 10 == 0) {// 当i能对10整除时，则让步
-                 Thread.yield();  
-             }  
-         }  
-     }  
- }  
       输出结果略，从输出结果可以看到，当某个线程(t1或者t2)执行到10次、20次、30次等时，就会马上切换到另一个线程执行，接下来再交替执行，如此往复。**注意，如果存在synchronized线程同步的话，线程让步不会释放锁(监视器对象)**。
# 线程睡眠
       线程睡眠涉及到两个方法，如下：
|`static void`|`**[sleep](http://blog.csdn.net/ghsau/article/details/17560467)**(long millis)`          在指定的毫秒数内让当前正在执行的线程休眠（暂停执行）。|
|----|----|
|`static void`|`**[sleep](http://blog.csdn.net/ghsau/article/details/17560467)**(long millis, int nanos)`          在指定的毫秒数加指定的纳秒数内让当前正在执行的线程休眠（暂停执行）。|
线程睡眠的过程中，如果是在synchronized线程同步内，是持有锁(监视器对象)的，也就是说，线程是关门睡觉的，别的线程进不来，来看一个小例子：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/17560467#)[copy](http://blog.csdn.net/ghsau/article/details/17560467#)
[print](http://blog.csdn.net/ghsau/article/details/17560467#)[?](http://blog.csdn.net/ghsau/article/details/17560467#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/129594/fork)
- publicclass SleepTest {  
- publicstaticvoid main(String[] args) {  
- // 创建共享对象
-         Service service = new Service();  
- // 创建线程
-         SleepThread t1 = new SleepThread("t1", service);  
-         SleepThread t2 = new SleepThread("t2", service);  
- // 启动线程
-         t1.start();  
-         t2.start();  
-     }  
- 
- }  
- class SleepThread extends Thread {  
- private Service service;  
- public SleepThread(String name, Service service) {  
- super(name);  
- this.service = service;  
-     }  
- publicvoid run() {  
-         service.calc();  
-     }  
- }  
- class Service {  
- publicsynchronizedvoid calc() {  
-         System.out.println(Thread.currentThread().getName() + "准备计算");  
-         System.out.println(Thread.currentThread().getName() + "感觉累了，开始睡觉");  
- try {  
-             Thread.sleep(10000);// 睡10秒
-         } catch (InterruptedException e) {  
- return;  
-         }  
-         System.out.println(Thread.currentThread().getName() + "睡醒了，开始计算");  
-         System.out.println(Thread.currentThread().getName() + "计算完成");  
-     }  
- }  
       输出结果：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/17560467#)[copy](http://blog.csdn.net/ghsau/article/details/17560467#)
[print](http://blog.csdn.net/ghsau/article/details/17560467#)[?](http://blog.csdn.net/ghsau/article/details/17560467#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/129594/fork)
- t1准备计算  
- t1感觉累了，开始睡觉  
- t1睡醒了，开始计算  
- t1计算完成  
- t2准备计算  
- t2感觉累了，开始睡觉  
- t2睡醒了，开始计算  
- t2计算完成  
# 线程合并
       线程合并涉及到三个方法，如下：
|` void`|`**[join](http://blog.csdn.net/ghsau/article/details/17560467)**()`          等待该线程终止。|
|----|----|
|` void`|`**[join](http://blog.csdn.net/ghsau/article/details/17560467)**(long millis)`          等待该线程终止的时间最长为 `millis` 毫秒。|
|` void`|`**[join](http://blog.csdn.net/ghsau/article/details/17560467)**(long millis, int nanos)`          等待该线程终止的时间最长为 `millis` 毫秒 + `nanos` 纳秒。|
       线程合并是优先执行调用该方法的线程，再执行当前线程，来看一个小例子：
**[java]**[view
 plain](http://blog.csdn.net/ghsau/article/details/17560467#)[copy](http://blog.csdn.net/ghsau/article/details/17560467#)
[print](http://blog.csdn.net/ghsau/article/details/17560467#)[?](http://blog.csdn.net/ghsau/article/details/17560467#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/129594/fork)
- publicclass JoinTest {  
- publicstaticvoid main(String[] args) throws InterruptedException {  
-         JoinThread t1 = new JoinThread("t1");  
-         JoinThread t2 = new JoinThread("t2");  
-         t1.start();  
-         t2.start();  
-         t1.join();  
-         t2.join();  
-         System.out.println("主线程开始执行！");  
-     }  
- }  
- class JoinThread extends Thread {  
- public JoinThread(String name) {  
- super(name);  
-     }  
- publicvoid run() {  
- for(int i = 1; i <= 10; i++)  
-             System.out.println(getName() + getId() + "执行了" + i + "次");  
-     }  
- }  
       t1和t2都执行完才继续主线程的执行，所谓合并，就是等待其它线程执行完，再执行当前线程，执行起来的效果就好像把其它线程合并到当前线程执行一样。
# 线程优先级
       线程最低优先级为1，最高优先级为10，看起来就有10个级别，但这10个级别能不能和CPU对应上，还未可知，Thread类中提供了优先级的三个常量，如下：
`public static final int``[NORM_PRIORITY](http://blog.csdn.net/ghsau/article/details/17560467)``5`
       我们创建线程对象后，如果不显示的设置优先级的话，默认为5。优先级可以看成一种特权，优先级高的，获取CPU调度的机会就大，优先级低的，获取CPU调度的机会就小，这个和我们现实生活很一样啊，优胜劣汰。线程优先级的示例就不写了，比较简单。
# wait()和sleep()区别
       区别太大了，但是在Java线程面试题中是很常见的问题，相信你阅读过本专栏后，能够轻松的解答，这里不再赘述。
       本文来自：[高爽|Coder](http://blog.csdn.net/ghsau)，原文地址：[http://blog.csdn.net/ghsau/article/details/17560467](http://blog.csdn.net/ghsau/article/details/17560467)，转载请注明。
