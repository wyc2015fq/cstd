# Java并发编程中的join()与interrupt()函数 - z69183787的专栏 - CSDN博客
2014年05月05日 20:22:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：679
     刚刚写了下Java的并发程序，在此做个笔记。对于线程a和b，在线程b中调用a.join()，那么此时线程b将会被挂起，直至线程a执行完才会有线程b执行的机会，若想打破这种机制，可以调用a.interrupt()，这时，线程b可以不必受刚才的约束。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
import java.util.concurrent.*;
class Sleeper extends Thread {
    //private String name;
    private int duration;
    
    public Sleeper(String name, int duration){
        super(name);
        this.duration = duration;
        start();
    }
    public void run(){
        try{
            System.out.println(getName() + " before sleep.");
            TimeUnit.MILLISECONDS.sleep(duration);
            System.out.println(getName() + " is asleep.");
        }catch(InterruptedException e){
            System.out.println(getName() + " was interrupted. " + " isInterrupted(): " + isInterrupted());
            return;
        }
        System.out.println(getName() + " has awakened.");
    }
}
class Joiner extends Thread{
    private Sleeper sleeper;
    public Joiner(String name, Sleeper sleeper){
        super(name);
        this.sleeper = sleeper;
        start();
    }
    
    public void run(){
        try{
            System.out.println(getName() + " before join.");
            sleeper.join();
            System.out.println(getName() + " after join.");
        }catch (InterruptedException e) {
            System.out.println(getName() + " was interrupted.");
        }
        System.out.println(getName() + " join completed.");
    }
}
public class ConcurrentBasic {
    public static void main(String args[]){
        Sleeper sleepy = new Sleeper("Sleepy", 3000);
        Sleeper grumpy = new Sleeper("Grumpy", 3000);
        Joiner dopey = new Joiner("Dopey", sleepy);
        Joiner doc = new Joiner("Doc", grumpy);
        
        grumpy.interrupt();
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
      grumpy在这被打断了，那么和它join的doc将会被唤醒，从而导致doc线程的执行，这里可以把grumpy.interrupt()改为doc.interrupt()，运行情况与预想差不多。
