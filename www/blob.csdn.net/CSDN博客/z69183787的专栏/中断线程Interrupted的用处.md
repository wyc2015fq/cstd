# 中断线程Interrupted的用处 - z69183787的专栏 - CSDN博客
2014年05月05日 20:13:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：740
中断线程 —— interrupt()
 一个正在运行的线程除了正常的时间片中断之外，能否被其他线程控制？或者说其他线程能否让指定线程放弃CPU或者提前结束运行？ 除了线程同步机制之外，还有两种方法：
       (1) Thread.stop(), Thread.suspend(), Thread.resume() 和Runtime.runFinalizersOnExit() 这些终止线程运行的方法 。这些方法已经被废弃，使用它们是极端不安全的。
       (2) Thread.interrupt() 方法是很好的选择。但是使用的时候我们必须好好理解一下它的用处。
**[java]**[view
 plain](http://blog.csdn.net/budapest/article/details/6941802#)[copy](http://blog.csdn.net/budapest/article/details/6941802#)
- //无法中断正在运行的线程代码  
- class TestRunnable implements Runnable{    
- publicvoid run(){    
- while(true)    
-             {    
-                   System.out.println( "Thread is running..." );    
- long time = System.currentTimeMillis();//去系统时间的毫秒数  
- while((System.currentTimeMillis()-time < 1000)) {    
- //程序循环1秒钟，不同于sleep(1000)会阻塞进程。  
-             }    
-               }    
-        }    
- }    
- publicclass ThreadDemo{    
- publicstaticvoid main(String[] args){    
-                Runnable r=new TestRunnable();    
-                Thread th1=new Thread(r);    
-                th1.start();    
-                th1.interrupt();             
-         }    
- }    
- /运行结果：一秒钟打印一次Thread is running...。程序没有终止的任何迹象   
上面的代码说明interrupt()并没有中断一个正在运行的线程，或者说让一个running中的线程放弃CPU。那么interrupt到底中断什么。
       首先我们看看interrupt究竟在干什么。
       当我们调用th1.interrput()的时候，线程th1的中断状态(interrupted status) 会被置位。我们可以通过Thread.currentThread().isInterrupted() 来检查这个布尔型的中断状态。
        在Core Java中有这样一句话："没有任何语言方面的需求要求一个被中断的程序应该终止。中断一个线程只是为了引起该线程的注意，被中断线程可以决定如何应对中断 "。好好体会这句话的含义，看看下面的代码：
**[java]**[view
 plain](http://blog.csdn.net/budapest/article/details/6941802#)[copy](http://blog.csdn.net/budapest/article/details/6941802#)
- //Interrupted的经典使用代码  
- publicvoid run(){    
- try{    
-              ....    
- while(!Thread.currentThread().isInterrupted()&& more work to do){    
- // do more work;  
-              }    
-         }catch(InterruptedException e){    
- // thread was interrupted during sleep or wait  
-         }    
- finally{    
- // cleanup, if required  
-         }    
- }    
很显然，在上面代码中，while循环有一个决定因素就是需要不停的检查自己的中断状态。当外部线程调用该线程的interrupt 时，使得中断状态置位。这是该线程将终止循环，不在执行循环中的do more work了。
       这说明: interrupt中断的是线程的某一部分业务逻辑，前提是线程需要检查自己的中断状态(isInterrupted())。
       但是当th1被阻塞的时候，比如被Object.wait, Thread.join和Thread.sleep三种方法之一阻塞时。调用它的interrput()方法。可想而知，没有占用CPU运行的线程是不可能给自己的中断状态置位的。这就会产生一个InterruptedException异常。
**[java]**[view
 plain](http://blog.csdn.net/budapest/article/details/6941802#)[copy](http://blog.csdn.net/budapest/article/details/6941802#)
- //中断一个被阻塞的线程代码
- class TestRunnable implements Runnable{  
- publicvoid run(){  
- try{  
-         Thread.sleep(1000000); //这个线程将被阻塞1000秒
-        }catch(InterruptedException e){  
-          e.printStackTrace();  
- //do more work and return.
-           }  
-      }  
- }  
- publicclass TestDemo2{  
- publicstaticvoid main(String[] args) {  
-             Runnable tr=new TestRunnable();  
-             Thread th1=new Thread(tr);  
-             th1.start(); //开始执行分线程
- while(true){  
-        th1.interrupt();  //中断这个分线程
-         }  
-       }  
- }  
- /*运行结果:
-    java.lang.InterruptedException: sleep interrupted
-         at java.lang.Thread.sleep(Native Method)
-         at TestRunnable.run(TestDemo2.java:4)
-         at java.lang.Thread.run(Unknown Source)*/
* 如果线程被阻塞，它便不能核查共享变量，也就不能停止。这在许多情况下会发生，例如调用
* Object.wait()、ServerSocket.accept()和DatagramSocket.receive()时，他们都可能永
* 久的阻塞线程。即使发生超时，在超时期满之前持续等待也是不可行和不适当的，所以，要使
* 用某种机制使得线程更早地退出被阻塞的状态。很不幸运，不存在这样一种机制对所有的情况
* 都适用，但是，根据情况不同却可以使用特定的技术。使用Thread.interrupt()中断线程正
* 如Example1中所描述的，Thread.interrupt()方法不会中断一个正在运行的线程。这一方法
* 实际上完成的是，在线程受到阻塞时抛出一个中断信号，这样线程就得以退出阻塞的状态。更
* 确切的说，如果线程被Object.wait, Thread.join和Thread.sleep三种方法之一阻塞，那么，
* 它将接收到一个中断异常（InterruptedException），从而提早地终结被阻塞状态。
