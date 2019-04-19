# java线程之中断线程Interrupted用法 - 零度的博客专栏 - CSDN博客
2016年06月08日 09:46:06[零度anngle](https://me.csdn.net/zmx729618)阅读数：4932
#### 线程状态 
      Java虚拟机将线程运行过程分成四种状态 。 (1) New 新生；(2) Runnable 可运行；(3) Blocked 阻塞；(4) Dead 死亡。
      值得注意的是： 线程的可运行状态并不代表线程一定在运行(runnable != running ) 。 大家都知道：所有现代桌面和服务器操作系统都使用了抢占式的线程调度策略 。一旦线程开始执行，并不是总是保持持续运行状态的。当系统分给它的时间片(非常小的运行时间单位)用完以后，不管程序有没有执行完，线程被强制放弃CPU，进入就绪状态，直到下次被调度后开始继续执行。也就是说， Runnable可运行状态的线程处于两种可能的情况下：(1)占用CPU运行中，(2)等待调度的就绪状态。 这里要声明一下：处于等待调度的就绪状态线程和处于阻塞的线程是完全不同的。就绪的线程是因为时间片用完而放弃CPU，其随时都有可能再次获得CPU而运行，这一切取决于分时OS的线程调度策略。
       在很多操作系统的专业术语中，这种因时间片用完而被剥夺CPU的情况我们叫做线程中断 。注意这和我们下面要将得中断线程是两个完全不同的概念。事实上，我们不可能通过应用程序来控制CPU的线程中断，除非我们能够自由调用OS的内核。
#### 中断线程-interrupt() 
        一个正在运行的线程除了正常的时间片中断之外，能否被其他线程控制？或者说其他线程能否让指定线程放弃CPU或者提前结束运行？ 除了线程同步机制之外，还有两种方法：
       (1) Thread.stop(), Thread.suspend(), Thread.resume() 和Runtime.runFinalizersOnExit() 这些终止线程运行的方法 。这些方法已经被废弃，使用它们是极端不安全的。
       (2) Thread.interrupt() 方法是很好的选择。但是使用的时候我们必须好好理解一下它的用处。
Java代码  ![收藏代码](http://hxraid.iteye.com/images/icon_star.png)
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
Java代码  ![收藏代码](http://hxraid.iteye.com/images/icon_star.png)
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
Java代码  ![收藏代码](http://hxraid.iteye.com/images/icon_star.png)
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
-             th1.interrupt();  //中断这个分线程
-         }  
-       }  
- }  
-    /*运行结果:
-    java.lang.InterruptedException: sleep interrupted
-         at java.lang.Thread.sleep(Native Method)
-         at TestRunnable.run(TestDemo2.java:4)
-         at java.lang.Thread.run(Unknown Source)*/
[](http://blog.csdn.net/z69183787/article/details/25076319#)[](http://blog.csdn.net/z69183787/article/details/25076319#)[](http://blog.csdn.net/z69183787/article/details/25076319#)[](http://blog.csdn.net/z69183787/article/details/25076319#)[](http://blog.csdn.net/z69183787/article/details/25076319#)[](http://blog.csdn.net/z69183787/article/details/25076319#)
