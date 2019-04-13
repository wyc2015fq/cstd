
# java入门教程-7.6Java isAlive()和join()的使用 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月26日 05:56:01[seven-soft](https://me.csdn.net/softn)阅读数：156个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



如前所述，通常你希望主线程最后结束。在前面的例子中，这点是通过在main()中调用sleep()来实现的，经过足够长时间的延迟以确保所有子线程都先于主线程结束。然而，这不是一个令人满意的解决方法，它也带来一个大问题：一个线程如何知道另一线程已经结束？幸运的是，Thread类提供了回答此问题的方法。
有两种方法可以判定一个线程是否结束。第一，可以在线程中调用isAlive()。这种方法由Thread定义，它的通常形式如下：
final boolean isAlive( )
如果所调用线程仍在运行，isAlive()方法返回true，如果不是则返回false。但isAlive（）很少用到，等待线程结束的更常用的方法是调用join()，描述如下：
final void join( ) throws InterruptedException
该方法等待所调用线程结束。该名字来自于要求线程等待直到指定线程参与的概念。join()的附加形式允许给等待指定线程结束定义一个最大时间。下面是前面例子的改进版本。运用join()以确保主线程最后结束。同样，它也演示了isAlive()方法。
// Using join() to wait for threads to finish.
class NewThread implements Runnable {
String name; // name of thread
Thread t;
NewThread(String threadname) {
name = threadname;
t = new Thread(this, name);
System.out.println("New thread: " + t);
t.start(); // Start the thread
}
// This is the entry point for thread.
public void run() {
try {
for(int i = 5; i > 0; i--) {
System.out.println(name + ": " + i);
Thread.sleep(1000);
}
} catch (InterruptedException e) {
System.out.println(name + " interrupted.");
}
System.out.println(name + " exiting.");
}
}
class DemoJoin {
public static void main(String args[]) {
NewThread ob1 = new NewThread("One");
NewThread ob2 = new NewThread("Two");
NewThread ob3 = new NewThread("Three");
System.out.println("Thread One is alive: "+ ob1.t.isAlive());
System.out.println("Thread Two is alive: "+ ob2.t.isAlive());
System.out.println("Thread Three is alive: "+ ob3.t.isAlive());
// wait for threads to finish
try {
System.out.println("Waiting for threads to finish.");
ob1.t.join();
ob2.t.join();
ob3.t.join();
} catch (InterruptedException e) {
System.out.println("Main thread Interrupted");
}
System.out.println("Thread One is alive: "+ ob1.t.isAlive());
System.out.println("Thread Two is alive: "+ ob2.t.isAlive());
System.out.println("Thread Three is alive: "+ ob3.t.isAlive());
System.out.println("Main thread exiting.");
}
}
程序输出如下所示：
New thread: Thread[One,5,main]
New thread: Thread[Two,5,main]
New thread: Thread[Three,5,main]
Thread One is alive: true
Thread Two is alive: true
Thread Three is alive: true
Waiting for threads to finish.
One: 5
Two: 5
Three: 5
One: 4
Two: 4
Three: 4
One: 3
Two: 3
Three: 3
One: 2
Two: 2
Three: 2
One: 1
Two: 1
Three: 1
Two exiting.
Three exiting.
One exiting.
Thread One is alive: false
Thread Two is alive: false
Thread Three is alive: false
Main thread exiting.
如你所见，调用join()后返回，线程终止执行。

