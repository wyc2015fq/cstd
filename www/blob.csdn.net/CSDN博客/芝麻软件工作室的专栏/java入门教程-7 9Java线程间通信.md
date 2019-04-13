
# java入门教程-7.9Java线程间通信 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月27日 09:27:52[seven-soft](https://me.csdn.net/softn)阅读数：95个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



上述例题无条件的阻塞了其他线程异步访问某个方法。Java对象中隐式管程的应用是很强大的，但是你可以通过进程间通信达到更微妙的境界。这在Java中是尤为简单的。
像前面所讨论过的，多线程通过把任务分成离散的和合乎逻辑的单元代替了事件循环程序。线程还有第二优点：它远离了轮询。轮询通常由重复监测条件的循环实现。一旦条件成立，就要采取适当的行动。这浪费了CPU时间。举例来说，考虑经典的序列问题，当一个线程正在产生数据而另一个程序正在消费它。为使问题变得更有趣，假设数据产生器必须等待消费者完成工作才能产生新的数据。在轮询系统，消费者在等待生产者产生数据时会浪费很多CPU周期。一旦生产者完成工作，它将启动轮询，浪费更多的CPU时间等待消费者的工作结束，如此下去。很明显，这种情形不受欢迎。
为避免轮询，Java包含了通过wait( )，notify( )和notifyAll( )方法实现的一个进程间通信机制。这些方法在对象中是用final方法实现的，所以所有的类都含有它们。这三个方法仅在synchronized方法中才能被调用。尽管这些方法从计算机科学远景方向上来说具有概念的高度先进性，实际中用起来是很简单的：wait( ) 告知被调用的线程放弃管程进入睡眠直到其他线程进入相同管程并且调用notify( )。
notify( ) 恢复相同对象中第一个调用 wait( ) 的线程。
notifyAll( ) 恢复相同对象中所有调用 wait( ) 的线程。具有最高优先级的线程最先运行。
这些方法在Object中被声明，如下所示：
final void wait( ) throws InterruptedException
final void notify( )
final void notifyAll( )
wait( )存在的另外的形式允许你定义等待时间。
下面的例子程序错误的实行了一个简单生产者/消费者的问题。它由四个类组成：Q，设法获得同步的序列；Producer，产生排队的线程对象；Consumer，消费序列的线程对象；以及PC，创建单个Q，Producer，和Consumer的小类。
// An incorrect implementation of a producer and consumer.
class Q {
int n;
synchronized int get() {
System.out.println("Got: " + n);
return n;
}
synchronized void put(int n) {
this.n = n;
System.out.println("Put: " + n);
}
}
class Producer implements Runnable {
Q q;
Producer(Q q) {
this.q = q;
new Thread(this, "Producer").start();
}
public void run() {
int i = 0;
while(true) {
q.put(i++);
}
}
}
class Consumer implements Runnable {
Q q;
Consumer(Q q) {
this.q = q;
new Thread(this, "Consumer").start();
}
public void run() {
while(true) {
q.get();
}
}
}
class PC {
public static void main(String args[]) {
Q q = new Q();
new Producer(q);
new Consumer(q);
System.out.println("Press Control-C to stop.");
}
}
尽管Q类中的put( )和get( )方法是同步的，没有东西阻止生产者超越消费者，也没有东西阻止消费者消费同样的序列两次。这样，你就得到下面的错误输出（输出将随处理器速度和装载的任务而改变）：
Put: 1
Got: 1
Got: 1
Got: 1
Got: 1
Got: 1
Put: 2
Put: 3
Put: 4
Put: 5
Put: 6
Put: 7
Got: 7
生产者生成1后，消费者依次获得同样的1五次。生产者在继续生成2到7，消费者没有机会获得它们。
用Java正确的编写该程序是用wait( )和notify( )来对两个方向进行标志，如下所示：
// A correct implementation of a producer and consumer.
class Q {
int n;
boolean valueSet = false;
synchronized int get() {
if(!valueSet)
try {
wait();
} catch(InterruptedException e) {
System.out.println("InterruptedException caught");
}
System.out.println("Got: " + n);
valueSet = false;
notify();
return n;
}
synchronized void put(int n) {
if(valueSet)
try {
wait();
} catch(InterruptedException e) {
System.out.println("InterruptedException caught");
}
this.n = n;
valueSet = true;
System.out.println("Put: " + n);
notify();
}
}
class Producer implements Runnable {
Q q;
Producer(Q q) {
this.q = q;
new Thread(this, "Producer").start();
}
public void run() {
int i = 0;
while(true) {
q.put(i++);
}
}
}
class Consumer implements Runnable {
Q q;
Consumer(Q q) {
this.q = q;
new Thread(this, "Consumer").start();
}
public void run() {
while(true) {
q.get();
}
}
}
class PCFixed {
public static void main(String args[]) {
Q q = new Q();
new Producer(q);
new Consumer(q);
System.out.println("Press Control-C to stop.");
}
}
内部get( ), wait( )被调用。这使执行挂起直到Producer 告知数据已经预备好。这时，内部get( ) 被恢复执行。获取数据后，get( )调用notify( )。这告诉Producer可以向序列中输入更多数据。在put(
 )内，wait( )挂起执行直到Consumer取走了序列中的项目。当执行再继续，下一个数据项目被放入序列，notify( )被调用，这通知Consumer它应该移走该数据。
下面是该程序的输出，它清楚的显示了同步行为：
Put: 1
Got: 1
Put: 2
Got: 2
Put: 3
Got: 3
Put: 4
Got: 4
Put: 5
Got: 5

