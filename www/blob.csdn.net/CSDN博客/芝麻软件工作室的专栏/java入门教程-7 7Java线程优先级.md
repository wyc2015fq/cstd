
# java入门教程-7.7Java线程优先级 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月27日 09:25:13[seven-soft](https://me.csdn.net/softn)阅读数：153个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



线程优先级被线程调度用来判定何时每个线程允许运行。理论上，优先级高的线程比优先级低的线程获得更多的CPU时间。实际上，线程获得的CPU时间通常由包括优先级在内的多个因素决定（例如，一个实行多任务处理的操作系统如何更有效的利用CPU时间）。
一个优先级高的线程自然比优先级低的线程优先。举例来说，当低优先级线程正在运行，而一个高优先级的线程被恢复（例如从沉睡中或等待I/O中），它将抢占低优先级线程所使用的CPU。
理论上，等优先级线程有同等的权利使用CPU。但你必须小心了。记住，Java是被设计成能在很多环境下工作的。一些环境下实现多任务处理从本质上与其他环境不同。为安全起见，等优先级线程偶尔也受控制。这保证了所有线程在无优先级的操作系统下都有机会运行。实际上，在无优先级的环境下，多数线程仍然有机会运行，因为很多线程不可避免的会遭遇阻塞，例如等待输入输出。遇到这种情形，阻塞的线程挂起，其他线程运行。
但是如果你希望多线程执行的顺利的话，最好不要采用这种方法。同样，有些类型的任务是占CPU的。对于这些支配CPU类型的线程，有时你希望能够支配它们，以便使其他线程可以运行。
设置线程的优先级，用setPriority()方法，该方法也是Tread 的成员。它的通常形式为：
final void setPriority(int level)
这 里 ， level 指 定了对所调用的线程的新的优先权的设置。Level的值必须在MIN_PRIORITY到MAX_PRIORITY范围内。通常，它们的值分别是1和10。要返回一个线程为默认的优先级，指定NORM_PRIORITY，通常值为5。这些优先级在Thread中都被定义为final型变量。
你可以通过调用Thread的getPriority()方法来获得当前的优先级设置。该方法如下：
final int getPriority( )
当涉及调度时，Java的执行可以有本质上不同的行为。Windows 95/98/NT/2000 的工作或多或少如你所愿。但其他版本可能工作的完全不同。大多数矛盾发生在你使用有优先级行为的线程，而不是协同的腾出CPU时间。最安全的办法是获得可预先性的优先权，Java获得跨平台的线程行为的方法是自动放弃对CPU的控制。
下面的例子阐述了两个不同优先级的线程，运行于具有优先权的平台，这与运行于无优先级的平台不同。一个线程通过Thread.NORM_PRIORITY设置了高于普通优先级两级的级数，另一线程设置的优先级则低于普通级两级。两线程被启动并允许运行10秒。每个线程执行一个循环，记录反复的次数。10秒后，主线程终止了两线程。每个线程经过循环的次数被显示。
// Demonstrate thread priorities.
class clicker implements Runnable {
int click = 0;
Thread t;
private volatile boolean running = true;
public clicker(int p) {
t = new Thread(this);
t.setPriority(p);
}
public void run() {
while (running) {
click++;
}
}
public void stop() {
running = false;
}
public void start() {
t.start();
}
}
class HiLoPri {
public static void main(String args[]) {
Thread.currentThread().setPriority(Thread.MAX_PRIORITY);
clicker hi = new clicker(Thread.NORM_PRIORITY + 2);
clicker lo = new clicker(Thread.NORM_PRIORITY - 2);
lo.start();
hi.start();
try {
Thread.sleep(10000);
} catch (InterruptedException e) {
System.out.println("Main thread interrupted.");
}
lo.stop();
hi.stop();
// Wait for child threads to terminate.
try {
hi.t.join();
lo.t.join();
} catch (InterruptedException e) {
System.out.println("InterruptedException caught");
}
System.out.println("Low-priority thread: " + lo.click);
System.out.println("High-priority thread: " + hi.click);
}
}
该程序在Windows 98下运行的输出，表明线程确实上下转换，甚至既不屈从于CPU，也不被输入输出阻塞。优先级高的线程获得大约90%的CPU时间。
Low-priority thread: 4408112
High-priority thread: 589626904
当然，该程序的精确的输出结果依赖于你的CPU的速度和运行的其他任务的数量。当同样的程序运行于无优先级的系统，将会有不同的结果。
上述程序还有个值得注意的地方。注意running前的关键字volatile。尽管volatile 在下章会被很仔细的讨论，用在此处以确保running的值在下面的循环中每次都得到验证。
while (running) {
click++;
}
如果不用volatile，Java可以自由的优化循环：running的值被存在CPU的一个寄存器中，
每次重复不一定需要复检。volatile的运用阻止了该优化，告知Java running可以改变，改变
方式并不以直接代码形式显示。

