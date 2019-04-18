# 多线程（Java） - zwvista - 博客园

## [多线程（Java）](https://www.cnblogs.com/zwvista/p/8413031.html)

### Thread 类 和 Runnable 接口

在Java中实现多线程有两种方法
- 继承 Thread 类

优点：通过覆盖Thread 类的方法，可以改变线程的行为。
- 实现 Runnable 接口

优点：不需要继承Thread类，可以继承其他类。

```
package threads.samples3;

public class ThreadTest {
    public static void main(String[] args) {
        // 创建 Thread 子类的实例
        Thread1 t1 = new Thread1();
        // 创建实现了 Runnable 接口的类的实例，将其作为参数传给 Thread 类的实例
        Thread t2 = new Thread(new Runnable1());
        // 通过调用 start 方法启动线程
        t1.start();
        t2.start();
    }
}

class Thread1 extends Thread {
    @Override
    public void run() {
        for(int i = 0; i < 3; i++) {
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(i + " in Thread1");
        }
    }
}

class Runnable1 implements Runnable {
    @Override
    public void run() {
        for(int i = 0; i < 3; i++) {
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            System.out.println(i + " in Runnable1");
        }
    }
}

/*
0 in Thread1
0 in Runnable1
1 in Runnable1
1 in Thread1
2 in Runnable1
2 in Thread1
*/
```

### Thread 类常用方法
- sleep 方法 ：Thread.sleep(1000) 当前线程休眠一秒钟。
- join 方法：mt.join() 让当前线程等待mt线程运行终止。
- interrupt 方法：mt.interrupt() 中断mt线程的运行。

### synchronized 关键字
- 在代码块前面加上 synchronized（同步） 关键字并且指定某个需要同步的对象，可以实现该代码块被执行期间指定对象的线程同步和线程安全。
- 排他锁：加上 synchronized 关键字的代码块在被某个线程执行期间，所指定的同步对象将被加上排他锁。代码块执行完毕，同步对象的排他锁将被解除。
- 线程同步：在同步对象的排他锁被解除之前，任何企图执行加上了 synchronized 关键字并且指定相同同步对象的代码块的线程都将处于阻塞状态。只有在同步对象的排他锁被解除之后，这些代码块才能被执行。
- 线程安全：加上 synchronized 关键字的代码块任何时候都不会同时被两个以上的线程执行，即这些代码块是线程安全的。


