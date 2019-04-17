# [CareerCup] 16.3 Dining Philosophers 哲学家聚餐问题 - Grandyang - 博客园







# [[CareerCup] 16.3 Dining Philosophers 哲学家聚餐问题](https://www.cnblogs.com/grandyang/p/5394611.html)







16.3 In the famous dining philosophers problem, a bunch of philosophers are sitting around a circular table with one chopstick between each of them. A philosopher needs both chopsticks to eat, and always picks up the left chopstick before the right one. A deadlock could potentially occur if all the philosophers reached for the left chopstick at the same time. Using threads and locks, implement a simulation of the dining philosophers problem that prevents deadlocks.



经典的哲学家聚餐问题，说是有一堆哲学家围着一个圆桌子坐着吃饭，每两个人之间都有一根筷子，每个人吃饭需要都需要左右两边的筷子，而且是先拿起左边的筷子，再拿右边的筷子，那么如果当所有的哲学家都拿着左边的筷子，那么就会产生死锁的情况。如果我们先不考虑死锁的问题，先来实现这个问题。我们可以把每个哲学家都当做一个线程，然后筷子被哲学家拿起后可以调用锁，当被放下后调用解锁，参见代码如下：



```
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Chopstick {
    private Lock lock;
    public Chopstick() {
        lock = new ReentrantLock();
    }
    public void pickUp() {
        lock.lock();
    }
    public void putDown() {
        lock.unlock();
    }
}

public class Philosopher extends Thread {
    private final int maxPause = 100;
    private int bites = 10;
    private Chopstick left;
    private Chopstick right;
    private int index;
    
    public Philosopher(int i, Chopstick left, Chopstick right) {
        this.left = left;
        this.right = right;
    }
    
    public void eat() {
        System.out.println("Philosopher" + index + ": start eating");
        pickUp();
        chew();
        putDown();
        System.out.println("Philosopher " + index + ": done eating");
    }
    
    public void pickUp() {
        pause();
        left.pickUp();
        pause();
        right.pickUp();
    }
    
    public void chew() {
        System.out.println("Philosopher " + index + ": eating");
        pause();
    }
    
    public void pause() {
        try {
            int pause = (int)(Math.random() * maxPause);
            Thread.sleep(pause);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    
    public void putDown() {
        left.putDown();
        right.putDown();
    }
    
    public void run() {
        for (int i = 0; i < bites; ++i) {
            eat();
        }
    }
}

public class j {
    public static int size = 3;
    
    public static int leftOf(int i) {
        return i;
    }
    
    public static int rightOf(int i) {
        return (i + 1) % size;
    }
    
    public static void main(String[] args) {
        Chopstick[] chopsticks = new Chopstick[size + 1];
        for (int i = 0; i < size + 1; ++i) {
            chopsticks[i] = new Chopstick();
        }
        
        Philosopher[] philosophers = new Philosopher[size];
        for (int i = 0; i < size; ++i) {
            Chopstick left = chopsticks[leftOf(i)];
            Chopstick right = chopsticks[rightOf(i)];
            philosophers[i] = new Philosopher(i, left, right);
        }
        
        for (int i = 0; i < size; ++i) {
            philosophers[i].start();
        }
    }
}
```



上面的代码在执行中基本都会陷入死循环，因为发生了死锁的情况，所以我们应该想机制来避免死锁的发生，那么怎么做呢，我们首先想想死锁是怎么形成的，是因为每个人都拿着左边的筷子不放，又无法拿到右边的筷子，所以就一直僵持着，那么我们换个思路想想，如果每个人在拿了左筷子，发现没法取得右筷子后，就把左筷子放下，这样就可以避免死锁的形成。那么我们在Chopstik类中的pickUp函数中就应该使用tryLock()来代替lock，这样只有在有左筷子的时候才能锁上左筷子，而且在Philosopher类中的pickUp函数中，先判断能不能拿左筷子，不能拿直接返回false，能拿的话再来看能不能拿右筷子，不能拿的话，先把左筷子放下，再返回false，能拿的话返回true。这样在eat函数中先看pickUp是否能返回true，能返回的话再继续运行之后的东西，参见代码如下：



```
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Chopstick {
    private Lock lock;
    public Chopstick() {
        lock = new ReentrantLock();
    }
    public boolean pickUp() {
        return lock.tryLock();
    }
    public void putDown() {
        lock.unlock();
    }
}

public class Philosopher extends Thread {
    private final int maxPause = 100;
    private int bites = 10;
    private Chopstick left;
    private Chopstick right;
    private int index;
    
    public Philosopher(int i, Chopstick left, Chopstick right) {
        index = i;
        this.left = left;
        this.right = right;
    }
    
    public void eat() {
        System.out.println("Philosopher" + index + ": start eating");
        if (pickUp()) {
            chew();
            putDown();
            System.out.println("Philosopher " + index + ": done eating");
        } else {
            System.out.println("Philosopher " + index + ": gave up on eating");
        }
    }
    
    public boolean pickUp() {
        pause();
        if (!left.pickUp()) {
            return false;
        }
        pause();
        if (!right.pickUp()) {
            left.putDown();
            return false;
        }
        pause();
        return true;
    }
    
    public void chew() {
        System.out.println("Philosopher " + index + ": eating");
        pause();
    }
    
    public void pause() {
        try {
            int pause = (int)(Math.random() * maxPause);
            Thread.sleep(pause);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    
    public void putDown() {
        left.putDown();
        right.putDown();
    }
    
    public void run() {
        for (int i = 0; i < bites; ++i) {
            eat();
        }
    }
}

public class j {
    public static int size = 3;
    
    public static int leftOf(int i) {
        return i;
    }
    
    public static int rightOf(int i) {
        return (i + 1) % size;
    }
    
    public static void main(String[] args) {
        Chopstick[] chopsticks = new Chopstick[size + 1];
        for (int i = 0; i < size + 1; ++i) {
            chopsticks[i] = new Chopstick();
        }
        
        Philosopher[] philosophers = new Philosopher[size];
        for (int i = 0; i < size; ++i) {
            Chopstick left = chopsticks[leftOf(i)];
            Chopstick right = chopsticks[rightOf(i)];
            philosophers[i] = new Philosopher(i, left, right);
        }
        
        for (int i = 0; i < size; ++i) {
            philosophers[i].start();
        }
    }
}
```



[CareerCup All in One 题目汇总](http://www.cnblogs.com/grandyang/p/5162994.html)














