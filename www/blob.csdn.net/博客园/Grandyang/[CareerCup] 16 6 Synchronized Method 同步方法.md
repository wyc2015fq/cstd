# [CareerCup] 16.6 Synchronized Method 同步方法 - Grandyang - 博客园







# [[CareerCup] 16.6 Synchronized Method 同步方法](https://www.cnblogs.com/grandyang/p/5400727.html)







16.6 You are given a class with synchronized method A and a normal method B. If you have two threads in one instance of a program, can they both execute A at the same time? Can they execute A and B at the same time? 



当我们给一个方法加了synchronized关键字，我们确保了两个线程不能同时执行同一个对象的该方法。所以对于第一问的答案是根据情况而定，如果两个线程有对象的同一个实例，那么答案是不行，它们不能同时执行方法A，但是当它们有对象的不同实例，那么它们就可以。我们可以用锁的概念来理解，一个synchronized方法就是在该实例对象上加了锁的方法，这样就阻止了其他线程执行该实例的其他synchronized方法。

对于第二部分，由于方法B并没有synchronized关键字，所以当线程2运行方法B时不会有东西阻碍线程1运行方法A，而且不论线程1和2是否有相同的实例对象。总而言之，需要牢记的是，只有一个同步synchronized方法可以在对象中的一个实例中执行，其他线程可以执行该实例的非同步non-synchronized方法，或者他们可以执行该对象的其他实例的任何方法。



```
public class Foo {
    private String name;
    
    public Foo(String nm) {
        name = nm;
    }
    
    public String getName() {
        return name;
    }
    
    public void pause() {
        try {
            Thread.sleep(1000 * 3);
        } catch(InterruptedException e) {
            e.printStackTrace();
        }
    }
    
    public synchronized void methodA (String threadName) {
        System.out.println("thread" + threadName + " starting; " + name + ".methodA()");
        pause();
        System.out.println("thread" + threadName + " ending: " + name + ".methodA()");
    }
    
    public void methodB(String threadName) {
        System.out.println("thread " + threadName + " starting: " + name + ".methodB()");
        pause();
        System.out.println("thread " + threadName + " ending: " + name + ".methodB()");
    }
}

public class MyThread extends Thread {
    private Foo foo;
    public String name;
    public String firstMethod;
    public MyThread(Foo f, String nm, String fM) {
        foo = f;
        name = nm;
        firstMethod = fM;
    }
    
    public void run() {
        if (firstMethod.equals("A")) {
            foo.methodA(name);
        } else {
            foo.methodB(name);
        }
    }
}

public class j {
    public static void main(String[] args) {
        System.out.println("Part 1 Demo with same instance.");
        Foo fooA = new Foo("ObjectOne");
        MyThread thread1a = new MyThread(fooA, "Dog", "A");
        MyThread thread2a = new MyThread(fooA, "Cat", "A");
        thread1a.start();
        thread2a.start();
        while (thread1a.isAlive() || thread2a.isAlive()) {}
        System.out.println("\n\n");
        
        System.out.println("Part 1 Demo with different instance.");
        Foo fooB1 = new Foo("ObjectOne");
        Foo fooB2 = new Foo("ObejctTwo");
        MyThread thread1b = new MyThread(fooB1, "Dog", "A");
        MyThread thread2b = new MyThread(fooB1, "Cat", "A");
        thread1b.start();
        thread2b.start();
        while (thread1b.isAlive() || thread2b.isAlive()) {};
        System.out.println("\n\n");
        
        System.out.println("Part 2 Demo.");
        Foo fooC = new Foo("ObjectOne");
        MyThread thread1c = new MyThread(fooC, "Dog", "A");
        MyThread thread2c = new MyThread(fooC, "Cat", "B");
        thread1c.start();
        thread2c.start();
    }
}
```



[CareerCup All in One 题目汇总](http://www.cnblogs.com/grandyang/p/5162994.html)












