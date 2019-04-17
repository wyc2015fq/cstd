# [CareerCup] 16.5 Semphore 信号旗 - Grandyang - 博客园







# [[CareerCup] 16.5 Semphore 信号旗](https://www.cnblogs.com/grandyang/p/5400632.html)







16.5 Suppose we have the following code:
public class Foo {
	　　public Foo() { . . . }
	　　public void first() { ... }
	　　public void second() { ... }
	　　public void thirdQ { ... }
}
The same instance of Foo will be passed to three different threads. ThreadA will call first, threads will call second, and threadC will call third. Design a mechanism to ensure that first is called before second and second is called before third.



按照题目中要求的顺序即需满足在执行second()前检测first()是否完成了，在执行third()前检测second()是否完成了，由于我们需要考虑线程安全，所以布尔型的标记不行，那么使用锁呢，看如下代码：



```
import java.util.concurrent.locks.ReentrantLock;

public class FooBad {
    public int pauseTime = 10000;
    public ReentrantLock lock1;
    public ReentrantLock lock2;
    
    public FooBad() {
        try {
            lock1 = new ReentrantLock();
            lock2 = new ReentrantLock();
            lock1.lock();
            lock2.lock();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
    
    public void first() {
        try {
            System.out.println("Started Executing 1");
            Thread.sleep(pauseTime);
            System.out.println("Finished Executing 1");
            lock1.unlock();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
    
    public void second () {
        try {
            lock1.lock();
            lock1.unlock();
            System.out.println("Started Executing 2");
            Thread.sleep(pauseTime);
            System.out.println("Finished Executing 2");
            lock2.unlock();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
    
    public void third() {
        try {
            lock2.lock();
            lock2.unlock();
            System.out.println("Started Executing 3");
            Thread.sleep(pauseTime);
            System.out.println("Finished Executing 3");
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
}

public class MyThread extends Thread {
    private String method;
    private FooBad foo;
    
    public MyThread(FooBad foo, String method) {
        this.method = method;
        this.foo = foo;
    }
    
    public void run() {
        if (method == "first") {
            foo.first();
        } else if (method == "second") {
            foo.second();
        } else if (method == "third") {
            foo.third();
        }
    }
}

public class j {
    public static void main(String[] args) {
        FooBad foo = new FooBad();
        
        MyThread thread1 = new MyThread(foo, "first");
        MyThread thread2 = new MyThread(foo, "second");
        MyThread thread3 = new MyThread(foo, "third");
        
        thread3.start();
        thread2.start();
        thread1.start();
    }
}
```



上述代码并不能很好的完成题目中要求的顺序，因为锁的所有权的问题。一个线程操作一个锁，当别的线程是无法解这个线程的锁的，所以用锁是不行的。我们可以使用信号旗Semaphores，参见代码如下：



```
import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Foo {
    public int pauseTime = 1000;
    public Semaphore sem1;
    public Semaphore sem2;
    
    public Foo() {
        try {
            sem1 = new Semaphore(1);
            sem2 = new Semaphore(1);
            sem1.acquire();
            sem2.acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    
    public void first() {
        try {
            System.out.println("Started Executing 1");
            Thread.sleep(pauseTime);
            System.out.println("Finished Executing");
            sem1.release();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
    
    public void second() {
        try {
            sem1.acquire();
            sem1.release();
            System.out.println("Started Executing 2");
            Thread.sleep(pauseTime);
            System.out.println("Finished Executing 2");
            sem2.release();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
    
    public void third() {
        try {
            sem2.acquire();
            sem2.release();
            System.out.println("Started Executing 3");
            Thread.sleep(pauseTime);
            System.out.println("Finished Executing 3");
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
}

public class MyThread extends Thread {
    private String method;
    private Foo foo;
    
    public MyThread(Foo foo, String method) {
        this.method = method;
        this.foo = foo;
    }
    
    public void run() {
        if (method == "first") {
            foo.first();
        } else if (method == "second") {
            foo.second();
        } else if (method == "third") {
            foo.third();
        }
    }
}

public class j {
    public static void main(String[] args) {
        Foo foo = new Foo();
        
        MyThread thread1 = new MyThread(foo, "first");
        MyThread thread2 = new MyThread(foo, "second");
        MyThread thread3 = new MyThread(foo, "third");
        
        thread3.start();
        thread2.start();
        thread1.start();
    }
}
```



[CareerCup All in One 题目汇总](http://www.cnblogs.com/grandyang/p/5162994.html)














