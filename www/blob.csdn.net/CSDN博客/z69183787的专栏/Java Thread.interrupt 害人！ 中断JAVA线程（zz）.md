# Java Thread.interrupt 害人！ 中断JAVA线程（zz） - z69183787的专栏 - CSDN博客
2014年05月05日 20:31:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2135
    一些轻率的家伙可能被另一种方法Thread.interrupt所迷惑。尽管，其名称似乎在暗示着什么，然而，这种方法并不会中断一个正在运行的线程（待会将进一步说明），正如Listing A中描述的那样。它创建了一个线程，并且试图使用Thread.interrupt方法停止该线程。Thread.sleep()方法的调用，为线程的初始化和中止提供了充裕的时间。线程本身并不参与任何有用的操作。
class Example1 extends Thread {
            boolean stop=false;
            public static void main( String args[] ) throws Exception {
            Example1 thread = new Example1();
            System.out.println( "Starting thread..." );
            thread.start();
            Thread.sleep( 3000 );
            System.out.println( "Interrupting thread..." );
            thread.interrupt();
            Thread.sleep( 3000 );
            System.out.println("Stopping application..." );
            //System.exit(0);
            }
            public void run() {
            while(!stop){
            System.out.println( "Thread is running..." );
            long time = System.currentTimeMillis();
            while((System.currentTimeMillis()-time < 1000)) {
            }
            }
            System.out.println("Thread exiting under request..." );
            }
            }
如果你运行了Listing A中的代码，你将在控制台看到以下输出：
Starting thread...
Thread is running...
Thread is running...
Thread is running...
Interrupting thread...
Thread is running...
Thread is running...
Thread is running...
Stopping application...
Thread is running...
Thread is running...
Thread is running...
............................... 
甚至，在Thread.interrupt()被调用后，线程仍然继续运行。
真正地中断一个线程
    中断线程最好的，最受推荐的方式是，使用共享变量（shared variable）发出信号，告诉线程必须停止正在运行的任务。线程必须周期性的核查这一变量（尤其在冗余操作期间），然后有秩序地中止任务。Listing B描述了这一方式。
Listing B
class Example2 extends Thread {
  volatile boolean stop = false;
  public static void main( String args[] ) throws Exception {
    Example2 thread = new Example2();
   System.out.println( "Starting thread..." );
   thread.start();
   Thread.sleep( 3000 );
   System.out.println( "Asking thread to stop..." );
   thread.stop = true;
   Thread.sleep( 3000 );
   System.out.println( "Stopping application..." );
   //System.exit( 0 );
  }
  public void run() {
    while ( !stop ) {
     System.out.println( "Thread is running..." );
      long time = System.currentTimeMillis();
      while ( (System.currentTimeMillis()-time < 1000) && (!stop) ) {
      }
    }
   System.out.println( "Thread exiting under request..." );
  }
}
运行Listing B中的代码将产生如下输出（注意线程是如何有秩序的退出的）
Starting thread...
Thread is running...
Thread is running...
Thread is running...
Asking thread to stop...
Thread exiting under request...
Stopping application...
   虽然该方法要求一些编码，但并不难实现。同时，它给予线程机会进行必要的清理工作，这在任何一个多线程应用程序中都是绝对需要的。请确认将共享变量定义成volatile 类型或将对它的一切访问封入同步的块/方法（synchronized blocks/methods）中。
到目前为止一切顺利!但是，当线程等待某些事件发生而被阻塞，又会发生什么？当然，如果线程被阻塞，它便不能核查共享变量，也就不能停止。这在许多情况下会发生，例如调用Object.wait()、ServerSocket.accept()和DatagramSocket.receive()时，这里仅举出一些。
他们都可能永久的阻塞线程。即使发生超时，在超时期满之前持续等待也是不可行和不适当的，所以，要使用某种机制使得线程更早地退出被阻塞的状态。
很不幸运，不存在这样一种机制对所有的情况都适用，但是，根据情况不同却可以使用特定的技术。在下面的环节，我将解答一下最普遍的例子。
使用Thread.interrupt()中断线程
  正如Listing A中所描述的，Thread.interrupt()方法不会中断一个正在运行的线程。这一方法实际上完成的是，在线程受到阻塞时抛出一个中断信号，这样线程就得以退出阻塞的状态。更确切的说，如果线程被Object.wait, Thread.join和Thread.sleep三种方法之一阻塞，那么，它将接收到一个中断异常（InterruptedException），从而提早地终结被阻塞状态。
    因此，如果线程被上述几种方法阻塞，正确的停止线程方式是设置共享变量，并调用interrupt()（注意变量应该先设置）。如果线程没有被阻塞，这时调用interrupt()将不起作用；否则，线程就将得到异常（该线程必须事先预备好处理此状况），接着逃离阻塞状态。在任何一种情况中，最后线程都将检查共享变量然后再停止。Listing C这个示例描述了该技术。
Listing C
class Example3 extends Thread {
  volatile boolean stop = false;
  public static void main( String args[] ) throws Exception {
   Example3 thread = new Example3();
   System.out.println( "Starting thread..." );
   thread.start();
   Thread.sleep( 3000 );
   System.out.println( "Asking thread to stop..." );
   thread.stop = true;//如果线程阻塞，将不会检查此变量
   thread.interrupt();
   Thread.sleep( 3000 );
   System.out.println( "Stopping application..." );
   //System.exit( 0 );
  }
  public void run() {
    while ( !stop ) {
     System.out.println( "Thread running..." );
      try {
      Thread.sleep( 1000 );
      } catch ( InterruptedException e ) {
      System.out.println( "Thread interrupted..." );
      }
    }
   System.out.println( "Thread exiting under request..." );
  }
}
一旦Listing C中的Thread.interrupt()被调用，线程便收到一个异常，于是逃离了阻塞状态并确定应该停止。运行以上代码将得到下面的输出：
Starting thread...
Thread running...
Thread running...
Thread running...
Asking thread to stop...
Thread interrupted...
Thread exiting under request...
Stopping application...
中断I/O操作
    然而，如果线程在I/O操作进行时被阻塞，又会如何？I/O操作可以阻塞线程一段相当长的时间，特别是牵扯到网络应用时。例如，服务器可能需要等待一个请求（request），又或者，一个网络应用程序可能要等待远端主机的响应。
如果你正使用通道（channels）（这是在Java 1.4中引入的新的I/O API），那么被阻塞的线程将收到一个ClosedByInterruptException异常。如果情况是这样，其代码的逻辑和第三个例子中的是一样的，只是异常不同而已。
但是，你可能正使用Java1.0之前就存在的传统的I/O，而且要求更多的工作。既然这样，Thread.interrupt()将不起作用，因为线程将不会退出被阻塞状态。Listing D描述了这一行为。尽管interrupt()被调用，线程也不会退出被阻塞状态
Listing D
import java.io.*;
class Example4 extends Thread {
  public static void main( String args[] ) throws Exception {
    Example4 thread = new Example4();
   System.out.println( "Starting thread..." );
   thread.start();
   Thread.sleep( 3000 );
   System.out.println( "Interrupting thread..." );
   thread.interrupt();
   Thread.sleep( 3000 );
   System.out.println( "Stopping application..." );
   //System.exit( 0 );
  }
  public void run() {
   ServerSocket socket;
    try {
      socket = new ServerSocket(7856);
    } catch ( IOException e ) {
     System.out.println( "Could not create the socket..." );
      return;
    }
    while ( true ) {
     System.out.println( "Waiting for connection..." );
      try {
       Socket sock = socket.accept();
      } catch ( IOException e ) {
      System.out.println( "accept() failed or interrupted..." );
      }
    }
  }
}
   很幸运，Java平台为这种情形提供了一项解决方案，即调用阻塞该线程的套接字的close()方法。在这种情形下，如果线程被I/O操作阻塞，该线程将接收到一个SocketException异常，这与使用interrupt()方法引起一个InterruptedException异常被抛出非常相似。
唯一要说明的是，必须存在socket的引用（reference），只有这样close()方法才能被调用。这意味着socket对象必须被共享。Listing E描述了这一情形。运行逻辑和以前的示例是相同的。
Listing E
import java.net.*;
import java.io.*;
class Example5 extends Thread {
  volatile boolean stop = false;
  volatile ServerSocket socket;
  public static void main( String args[] ) throws Exception {
    Example5 thread = new Example5();
   System.out.println( "Starting thread..." );
   thread.start();
   Thread.sleep( 3000 );
   System.out.println( "Asking thread to stop..." );
   thread.stop = true;
   thread.socket.close();
   Thread.sleep( 3000 );
   System.out.println( "Stopping application..." );
   //System.exit( 0 );
  }
  public void run() {
    try {
      socket = new ServerSocket(7856);
    } catch ( IOException e ) {
     System.out.println( "Could not create the socket..." );
      return;
    }
    while ( !stop ) {
     System.out.println( "Waiting for connection..." );
      try {
       Socket sock = socket.accept();
      } catch ( IOException e ) {
      System.out.println( "accept() failed or interrupted..." );
      }
    }
   System.out.println( "Thread exiting under request..." );
  }
}
以下是运行Listing E中代码后的输出：
Starting thread...
Waiting for connection...
Asking thread to stop...
accept() failed or interrupted...
Thread exiting under request...
Stopping application...
多线程是一个强大的工具，然而它正呈现出一系列难题。其中之一是如何中断一个正在运行的线程。如果恰当地实现，使用上述技术中断线程将比使用Java平台上已经提供的内嵌操作更为简单。
============================================
Writing multithreaded programs in Java, with its built-in support for threads, is fairly straightforward. However, multithreading presents
 a whole set of new challenges to the programmer that, if not correctly addressed, can lead to unexpected behavior and subtle, hard-to-find errors. In this article, we address one of those challenges: how to interrupt a running thread.
Background
Interrupting a thread means stopping what it is doing before it has completed its task, effectively aborting its current operation.
 Whether the thread dies, waits for new tasks, or goes on to the next step depends on the application.
Although it may seem simple at first, you must take some precautions in order to achieve the desired result. There are some caveats
 you must be aware of as well.
First of all, forget the Thread.stop method.
 Although it indeed stops a running thread, the method is unsafe and was [deprecated](http://java.sun.com/j2se/1.3/docs/guide/misc/threadPrimitiveDeprecation.html),
 which means it may not be available in future versions of the Java.
Another method that can be confusing for the unadvised is Thread.interrupt.
 Despite what its name may imply, the method does not interrupt a running thread (more on this later), as[Listing
 A](http://articles.techrepublic.com.com/5100-22-5144546.html#) demonstrates. It creates a thread and tries to stop it using Thread.interrupt.
 The calls to Thread.sleep() give
 plenty of time for the thread initialization and termination. The thread itself does not do anything useful.
If you run the code in Listing A, you should see something like this on your console:
Starting thread...
Thread is running...
Thread is running...
Thread is running...
Interrupting thread...
Thread is running...
Thread is running...
Thread is running...
Stopping application...
Even after Thread.interrupt() is
 called, the thread continues to run for a while.
Really interrupting a thread
The best, recommended way to interrupt a thread is to use a shared variable to signal that it must stop what it is doing. The thread
 must check the variable periodically, especially during lengthy operations, and terminate its task in an orderly manner. [Listing
 B](http://articles.techrepublic.com.com/5100-22-5144546.html#) demonstrates this technique.
Running the code in Listing B will generate output like this (notice how the thread exits in an orderly fashion):
Starting thread...
Thread is running...
Thread is running...
Thread is running...
Asking thread to stop...
Thread exiting under request...
Stopping application...
Although this method requires some coding, it is not difficult to implement and give the thread the opportunity to do any cleanup needed,
 which is an absolute requirement for any multithreaded application. Just be sure to declare the shared variable as volatile or
 enclose any access to it into synchronized blocks/methods.
So far, so good! But what happens if the thread is blocked waiting for some event? Of course, if the thread is blocked, it can't check
 the shared variable and can't stop. There are plenty of situations when that may occur, such as calling Object.wait(), ServerSocket.accept(),
 and DatagramSocket.receive(),
 to name a few.
They all can block the thread forever. Even if a timeout is employed, it may not be feasible or desirable to wait until the timeout
 expires, so a mechanism to prematurely exit the blocked state must be used.
Unfortunately there is no such mechanism that works for all cases, but the particular technique to use depends on each situation. In
 the following sections, I'll give solutions for the most common cases.
Interrupting a thread with *Thread.interrupt()*
As demonstrated in Listing A, the method Thread.interrupt() does
 not interrupt a running thread. What the method actually does is to throw an interrupt if the thread is blocked, so that it exits the blocked state. More precisely, if the thread is blocked at one of the methods Object.wait, Thread.join,
 or Thread.sleep, it receives
 an InterruptedException,
 thus terminating the blocking method prematurely.
So, if a thread blocks in one of the aforementioned methods, the correct way to stop it is to set the shared variable and then call
 the interrupt() method on
 it (notice that it is important to set the variable first). If the thread is not blocked, calling interrupt() will
 not hurt; otherwise, the thread will get an exception (the thread must be prepared to handle this condition) and escape the blocked state. In either case, eventually the thread will test the shared variable and stop. [Listing
 C](http://articles.techrepublic.com.com/5100-22-5144546.html#) is a simple example that demonstrates this technique.
As soon as Thread.interrupt() is
 called in Listing C, the thread gets an exception so that it escapes the blocked state and determines that it should stop. Running this code produces output like this:
Starting thread...
Thread running...
Thread running...
Thread running...
Asking thread to stop...
Thread interrupted...
Thread exiting under request...
Stopping application...
Interrupting an I/O operation
But what happens if the thread is blocked on an I/O operation? I/O can block a thread for a considerable amount of time, particularly
 if network communication is involved. For example, a server may be waiting for a request, or a network application may be waiting for an answer from a remote host.
If you're using channels, available with the new I/O API introduced in Java 1.4, the blocked thread will get a ClosedByInterruptException exception.
 If that is the case, the logic is the same as that used in the third example—only the exception is different.
But you might be using the traditional I/O available since Java 1.0, since the new I/O is so recent and requires more work. In this
 case, Thread.interrupt() doesn't
 help, since the thread will not exit the blocked state. [Listing
 D](http://articles.techrepublic.com.com/5100-22-5144546.html#) demonstrates that behavior. Although the interrupt() method
 is called, the thread does not exit the blocked state.
Fortunately, the Java Platform provides a solution for that case by calling the close() method
 of the socket the thread is blocked in. In this case, if the thread is blocked in an I/O operation, the thread will get a SocketException exception,
 much like the interrupt() method
 causes an InterruptedException to
 be thrown.
The only caveat is that a reference to the socket must be available so that its close() method
 can be called. That means the socket object must also be shared. [Listing
 E](http://articles.techrepublic.com.com/5100-22-5144546.html#) demonstrates this case. The logic is the same as in the examples presented so far.
And here's the sample output you can expect from running Listing E:
Starting thread...
Waiting for connection...
Asking thread to stop...
accept() failed or interrupted...
Thread exiting under request...
Stopping application...
Multithreading is a powerful tool, but it presents its own set of challenges. One of these is how to interrupt a running thread. If
 properly implemented, these techniques make interrupting a thread no more difficult than using the built-in operations already provided by the Java Platform.
