# java线程同步的三种方法[synchronized关键字,Lock加锁,信号量Semaphore] - 零度的博客专栏 - CSDN博客
2017年04月01日 15:10:17[零度anngle](https://me.csdn.net/zmx729618)阅读数：1349
java多线程的难点是在：处理多个线程同步与并发运行时线程间的通信问题。java在处理线程同步时，常用方法有：
1、synchronized关键字。
2、Lock显示加锁。
3、信号量Semaphore。
线程同步问题引入：
      创建一个银行账户Account类，在创建并启动100个线程往同一个Account类实例里面添加一块钱。在没有使用上面三种方法的情况下：
代码：
```
`import java.util.concurrent.ExecutorService;`
`import java.util.concurrent.Executors;`
`public``class``AccountWithoutSync {`
`    ``private``static`
```
Account account =
```
`new`
```
Account();
```
`//实例化一个账户`
`    `
`    ``public``static``void``main(String[] args)`
`    ``{`
`        ``long``start = System.currentTimeMillis();`
`        ``//使用ExecutorService创建线程池`
`        ``ExecutorService executor  = Executors.newCachedThreadPool();`
`        `
`        ``for``(``int``i=0;i<100;i++)`
`        ``{`
`            ``executor.execute(``new``AddPennyTask());`
`        ``}`
`        ``//关闭线程池 即使线程池中还有未完成的线程 返回未完成的清单`
`        ``executor.shutdown();`
`        `
`        ``//关闭之后还是要保证未完成的线程继续完成  如果线程池中所有任务都完成了，isTerminated返回true`
`        ``while``(!executor.isTerminated())`
`        ``{}`
`        ``long``end = System.currentTimeMillis();`
`        ``//balance有余额的意思`
`        ``System.``out``.println(``"现在账户里面的余额是："``+ account.getBalance());`
`        ``System.``out``.println(``"花费的时间以微秒为单位："``+(end-start)+``"微秒"``);`
`    ``}`
`    `
`    ``//这个线程只调用了一个方法`
`    ``public``static``class``AddPennyTask implements Runnable`
`    ``{`
`        ``@Override`
`        ``public``void``run() {`
`            ``account.deposit(1);`
`        ``}`
`    ``}`
`    `
`    ``//一个内部类  用于 账户的相关处理`
`    ``public``static``class``Account`
`    ``{`
`        ``private``int``balance =0;`
`        ``public``int``getBalance()`
`        ``{`
`            ``return``balance;`
`        ``}`
`        ``public``void``deposit(``int``amount)`
`        ``{`
`            ``int``newBalance = balance + amount;`
`            ``//为了让错误体现的更明显`
`            ``try``{`
`                `
```
Thread.sleep(4);
```
`//5毫秒`
`            `
```
}
```
`catch``(InterruptedException e) {`
`                ``// TODO Auto-generated catch block`
`                ``e.printStackTrace();`
`            ``}`
`            ``balance= newBalance;`
`            ``//其实就是balance +=amount;//不过换成这一段代码结果就在100和99左右`
`        ``}`
`    ``}`
`}`
```
运行截图：
       错的不是一般的明显，明明存入了100块，显示只有2块，原因也很简单，就是100个线程同时对acount进行修改，当第100该线程把钱改成100时，第2个线程最后修改，把它改成了2，这群不听话的线程，那就好好管管他们吧，让他们乖乖听话，在这之前，还是要记住以下一些名词：
竞争状态：当多个线程访问同一公共资源并引发冲突造成线程不同步，我们称这种状态为竞争状态。
线程安全：是针对类来说的，如果一个类的对象在多线程程序中不会导致竞争状态，我们就称这类为线程安全的，上面的Account是线性不安全的，而又如StringBuffer是线程安全的，而StringBuilder则是线程不安全的。
临界区：造成竞争状态的原因是多个线程同时进入了程序中某一特定部分，如上面程序中的deposit方法，我们称这部分为程序中的临界区，我们要解决多线程不同步问题，就是要解决如何让多个线程有序的访问临界区。
使用synchronized关键字
1、同步方法：在deposit方法前加synchronized关键字，使得该方法变成同步方法：public synchronized void deposit(double amount){}
2、同步语句：对某一代码块加synchronized关键字，常用格式：
synchronized(exper) {statement}   其中exper是对象的引用，如上面的程序，在要在调用depsoit方法时，改成这样:  synchronized(account){account.deposit(1);}
      使用synchronized，其实是隐式地给方法或者代码块加了锁，任何同步的实例方法都可以转换为同步语句：
public synchronized void  method(){}
转换为同步语句： public  void  method{sysnchronized(this){}}
利用lock加锁同步
  java也可以用Lock显示的对临界区代码加锁以及解锁，这比用synchronized关键字更加直观灵活。
       一个锁是一个Lock接口的实例，该接口定义了加锁解锁的方法，且一个锁可以多次调用其newCondition()方法创建名为Condition对象的实例，以此进行线程间的通信（在后面用到）。
     有了Lock接口，我们还要实现它，java提供了RenentrantLock类，该类是为创建相互排斥锁而实现了Lock接口，由此就好办了，下面看一下书上的图：
[](http://image.lxway.com/upload/c/a7/ca7c841d7fe567330d84820bf265357e.png)
代码如下：
```
`public``static``class``Account2`
`{`
`    ``private``static``Lock ``lock``= ``new``ReentrantLock();`
`    ``private``int``balance =0;`
`    ``public``int``getBalance()`
`    ``{`
`        ``return``balance;`
`    ``}`
`    ``public``void``deposit(``int``amount)`
`    ``{`
`        ``lock``.``lock``();`
`        ``try``{`
`            ``int``newBalance = balance + amount;`
`            `
```
Thread.sleep(4);
```
`            ``balance= newBalance;`
`        ``}``catch``(InterruptedException e) {`
`            ``e.printStackTrace();`
`        ``}`
`        ``finally``{`
`            ``lock``.unlock();`
`        ``}`
`    ``}`
`}`
```
给个运行截图：
         100块存入，且时间明显比之前久了，100个线程都乖乖的排队访问临界区。另外注意在对lock()的调用后，紧跟随try catch finnaly语句，这是个好习惯。
利用信号量同步
            信号量是个好东西，信号量机制在操作系统方面有着广泛的应用，如linux进程同步信号量，而在java里，Semaphore包包含了一些访问信号量的方法。
信号量可以用来限制访问共享资源的线程数，在访问临界区资源前，线程必须获取一个信号量，在访问完之后返回一个信号量。下图是关于类Semaphore，该类包含访问信号量的方法：
[](http://image.lxway.com/upload/1/50/1506264a3e8bbd8aad40a0fe6ab8df4a.png)
利用信号量Semaphorre,可以将上面的Account类改成这样：
```
``public``static``class``Account`{`
`    ``private``static`
```
Semaphore semaphore =
```
`new`
```
Semaphore(1);
```
`//创建一个信号量`
`    ``private``int``balance =0;`
`    ``public``int``getBalance()`
`    ``{`
`        ``return``balance;`
`    ``}`
`    ``public``void``deposit(``int``amount)`
`    ``{`
`        ``try``{`
`            ``semaphore.acquire();`
`            ``int``newBalance = balance + amount;`
`            `
```
Thread.sleep(4);
```
`            ``balance= newBalance;`
`        `
```
}
```
`catch``(InterruptedException e) {`
`            ``e.printStackTrace();`
`        ``}``finally``{`
`            `
```
semaphore.release();
```
`//返回一个信号量`
`        ``}`
`    ``}`
`}`
```
