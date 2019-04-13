
# java入门教程-7.11Java线程的挂起、恢复和终止 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月27日 09:30:20[seven-soft](https://me.csdn.net/softn)阅读数：170个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



有时，线程的挂起是很有用的。例如，一个独立的线程可以用来显示当日的时间。如果用户不希望用时钟，线程被挂起。在任何情形下，挂起线程是很简单的，一旦挂起，重新启动线程也是一件简单的事。
挂起，终止和恢复线程机制在Java 2和早期版本中有所不同。尽管你运用Java 2的途径编写代码，你仍需了解这些操作在早期Java环境下是如何完成的。例如，你也许需要更新或维护老的代码。你也需要了解为什么Java 2会有这样的变化。因为这些原因，下面内容描述了执行线程控制的原始方法，接着是Java
 2的方法。
## Java 1.1或更早版本的线程的挂起、恢复和终止
先于Java2的版本，程序用Thread 定义的suspend() 和 resume() 来暂停和再启动线程。它们的形式如下：
final void suspend( )
final void resume( )
下面的程序描述了这些方法：
// Using suspend() and resume().
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
for(int i = 15; i > 0; i--) {
System.out.println(name + ": " + i);
Thread.sleep(200);
}
} catch (InterruptedException e) {
System.out.println(name + " interrupted.");
}
System.out.println(name + " exiting.");
}
}
class SuspendResume {
public static void main(String args[]) {
NewThread ob1 = new NewThread("One");
NewThread ob2 = new NewThread("Two");
try {
Thread.sleep(1000);
ob1.t.suspend();
System.out.println("Suspending thread One");
Thread.sleep(1000);
ob1.t.resume();
System.out.println("Resuming thread One");
ob2.t.suspend();
System.out.println("Suspending thread Two");
Thread.sleep(1000);
ob2.t.resume();
System.out.println("Resuming thread Two");
} catch (InterruptedException e) {
System.out.println("Main thread Interrupted");
}
// wait for threads to finish
try {
System.out.println("Waiting for threads to finish.");
ob1.t.join();
ob2.t.join();
} catch (InterruptedException e) {
System.out.println("Main thread Interrupted");
}
System.out.println("Main thread exiting.");
}
}
程序的部分输出如下：
New thread: Thread[One,5,main]
One: 15
New thread: Thread[Two,5,main]
Two: 15
One: 14
Two: 14
One: 13
Two: 13
One: 12
Two: 12
One: 11
Two: 11
Suspending thread One
Two: 10
Two: 9
Two: 8
Two: 7
Two: 6
Resuming thread One
Suspending thread Two
One: 10
One: 9
One: 8
One: 7
One: 6
Resuming thread Two
Waiting for threads to finish.
Two: 5
One: 5
Two: 4
One: 4
Two: 3
One: 3
Two: 2
One: 2
Two: 1
One: 1
Two exiting.
One exiting.
Main thread exiting.
Thread类同样定义了stop() 来终止线程。它的形式如下：
void stop( )
一旦线程被终止，它不能被resume() 恢复继续运行。
## Java 2中挂起、恢复和终止线程
Thread定义的suspend()，resume()和stop()方法看起来是管理线程的完美的和方便的方法，它们不能用于新Java版本的程序。下面是其中的原因。Thread类的suspend()方法在Java2中不被赞成，因为suspend()有时会造成严重的系统故障。假定对关键的数据结构的一个线程被锁定的情况，如果该线程在那里挂起，这些锁定的线程并没有放弃对资源的控制。其他的等待这些资源的线程可能死锁。
Resume()方法同样不被赞同。它不引起问题，但不能离开suspend()方法而独立使用。Thread类的stop()方法同样在Java 2中受到反对。这是因为该方法可能导致严重的系统故障。设想一个线程正在写一个精密的重要的数据结构且仅完成一个零头。如果该线程在此刻终止，则数据结构可能会停留在崩溃状态。
因为在Java 2中不能使用suspend()，resume()和stop() 方法来控制线程，你也许会想那就没有办法来停止，恢复和结束线程。其实不然。相反，线程必须被设计以使run() 方法定期检查以来判定线程是否应该被挂起，恢复或终止它自己的执行。有代表性的，这由建立一个指示线程状态的标志变量来完成。只要该标志设为“running”，run()方法必须继续让线程执行。如果标志为“suspend”，线程必须暂停。若设为“stop”，线程必须终止。
当然，编写这样的代码有很多方法，但中心主题对所有的程序应该是相同的。
下面的例题阐述了从Object继承的wait()和notify()方法怎样控制线程的执行。该例与前面讲过的程序很像。然而，不被赞同的方法都没有用到。让我们思考程序的执行。
NewTread 类包含了用来控制线程执行的布尔型的实例变量suspendFlag。它被构造函数初始化为false。Run()方法包含一个监测suspendFlag 的同步声明的块。如果变量是true，wait()方法被调用以挂起线程。Mysuspend()方法设置suspendFlag为true。Myresume()方法设置suspendFlag为false并且调用notify()方法来唤起线程。最后，main()方法被修改以调用mysuspend()和myresume()方法。
// Suspending and resuming a thread for Java2
class NewThread implements Runnable {
String name; // name of thread
Thread t;
boolean suspendFlag;
NewThread(String threadname) {
name = threadname;
t = new Thread(this, name);
System.out.println("New thread: " + t);
suspendFlag = false;
t.start(); // Start the thread
}
// This is the entry point for thread.
public void run() {
try {
for(int i = 15; i > 0; i--) {
System.out.println(name + ": " + i);
Thread.sleep(200);
synchronized(this) {
while(suspendFlag) {
wait();
}
}
}
} catch (InterruptedException e) {
System.out.println(name + " interrupted.");
}
System.out.println(name + " exiting.");
}
void mysuspend() {
suspendFlag = true;
}
synchronized void myresume() {
suspendFlag = false;
notify();
}
}
class SuspendResume {
public static void main(String args[]) {
NewThread ob1 = new NewThread("One");
NewThread ob2 = new NewThread("Two");
try {
Thread.sleep(1000);
ob1.mysuspend();
System.out.println("Suspending thread One");
Thread.sleep(1000);
ob1.myresume();
System.out.println("Resuming thread One");
ob2.mysuspend();
System.out.println("Suspending thread Two");
Thread.sleep(1000);
ob2.myresume();
System.out.println("Resuming thread Two");
} catch (InterruptedException e) {
System.out.println("Main thread Interrupted");
}
// wait for threads to finish
try {
System.out.println("Waiting for threads to finish.");
ob1.t.join();
ob2.t.join();
} catch (InterruptedException e) {
System.out.println("Main thread Interrupted");
}
System.out.println("Main thread exiting.");
}
}
该程序的输出与前面的程序相同。此书的后面部分，你将看到用Java 2机制控制线程的更多例子。尽管这种机制不像老方法那样“干净”，然而，它是确保运行时不发生错误的方法。它是所有新的代码必须采用的方法。

