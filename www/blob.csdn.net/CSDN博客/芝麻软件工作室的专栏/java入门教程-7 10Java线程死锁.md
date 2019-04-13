
# java入门教程-7.10Java线程死锁 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月27日 09:29:18[seven-soft](https://me.csdn.net/softn)阅读数：115个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



需要避免的与多任务处理有关的特殊错误类型是死锁（deadlock）。死锁发生在当两个线程对一对同步对象有循环依赖关系时。例如，假定一个线程进入了对象X的管程而另一个线程进入了对象Y的管程。如果X的线程试图调用Y的同步方法，它将像预料的一样被锁定。而Y的线程同样希望调用X的一些同步方法，线程永远等待，因为为到达X，必须释放自己的Y的锁定以使第一个线程可以完成。死锁是很难调试的错误，因为：通常，它极少发生，只有到两线程的时间段刚好符合时才能发生。
它可能包含多于两个的线程和同步对象（也就是说，死锁在比刚讲述的例子有更多复杂的事件序列的时候可以发生）。
为充分理解死锁，观察它的行为是很有用的。下面的例子生成了两个类，A和B，分别有foo( )和bar( )方法。这两种方法在调用其他类的方法前有一个短暂的停顿。主类，名为Deadlock，创建了A和B的实例，然后启动第二个线程去设置死锁环境。foo(
 )和bar( )方法使用sleep( )强迫死锁现象发生。
// An example of deadlock.
class A {
synchronized void foo(B b) {
String name = Thread.currentThread().getName();
System.out.println(name + " entered A.foo");
try {
Thread.sleep(1000);
} catch(Exception e) {
System.out.println("A Interrupted");
}
System.out.println(name + " trying to call B.last()");
b.last();
}
synchronized void last() {
System.out.println("Inside A.last");
}
}
class B {
synchronized void bar(A a) {
String name = Thread.currentThread().getName();
System.out.println(name + " entered B.bar");
try {
Thread.sleep(1000);
} catch(Exception e) {
System.out.println("B Interrupted");
}
System.out.println(name + " trying to call A.last()");
a.last();
}
synchronized void last() {
System.out.println("Inside A.last");
}
}
class Deadlock implements Runnable {
A a = new A();
B b = new B();
Deadlock() {
Thread.currentThread().setName("MainThread");
Thread t = new Thread(this, "RacingThread");
t.start();
a.foo(b); // get lock on a in this thread.
System.out.println("Back in main thread");
}
public void run() {
b.bar(a); // get lock on b in other thread.
System.out.println("Back in other thread");
}
public static void main(String args[]) {
new Deadlock();
}
}
运行程序后，输出如下：
MainThread entered A.foo
RacingThread entered B.bar
MainThread trying to call B.last()
RacingThread trying to call A.last()
因为程序死锁，你需要按CTRL-C来结束程序。在PC机上按CTRL-BREAK（或在Solaris下按CTRL-\）你可以看到全线程和管程缓冲堆。你会看到RacingThread在等待管程a时占用管程b，同时，MainThread占用a等待b。该程序永远都不会结束。像该例阐明的，你的多线程程序经常被锁定，死锁是你首先应检查的问题。

