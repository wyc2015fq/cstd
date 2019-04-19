# Java多线程(11) Fork-Join框架 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2018年02月26日 20:31:37[dutong0321](https://me.csdn.net/dutong0321)阅读数：73
#### 概述
在JDK1.7版本以后，引入了一个新的分布式Fork-JoinJDK框架。这个框架的特点就是可以把任务线程分布到每个CPU核上，如果4核的CPU，那么如果创建一个线程，那么这个线程就在CPU的一个核上，如果创建3个线程，呢么就分布到CPU的3个核，如果想要创建大于CPU核的线程，对不起，不能创建会阻塞，所以这个Fork-Join框架的优点就在于会把线程分布到每个CPU核上，这会加快程序运行速度，但是缺陷也很明显就是要避免创建大于CPU核心数的线程，以免发生阻塞。
#### Fork-Join框架
简单来讲，使用Fork-Join框架主要需要提供一个扩展RecursiveTask< T>的类（如果计算会生成一个类型为T的结果）或者提供一个扩展RecursiveAction的类（如果不生成任何结果），然后覆盖compute方法来生成并调用子任务，最后合并结果。（看不懂没关系，关键看代码，如果自己写一遍的话建议按照我贴代码的顺序去写）
```
package com.utopia.forkJoin;
import java.util.concurrent.RecursiveTask;
public class Counter extends RecursiveTask<Integer> {
    private static final long serialVersionUID = 1L;
    public static final int THRESHOLD = 1000;
    private double[] values;
    private int from;
    private int to;
    private Filter filter;
    public Counter(double[] values,int from,int to,Filter filter) {
        this.values = values;
        this.from = from;
        this.to = to;
        this.filter = filter;
    }
    @Override
    protected Integer compute() {
        // TODO Auto-generated method stub
        if (to - from < THRESHOLD) {
            // 如果任务足够小，则直接计算
            int count = 0;
            for (int i = from;i < to;i++){
                if(filter.accept(values[i]))
                    count++;
            }
            return count;
        } else {
            // 如果任务太大，则拆分该任务
            int mid = (from + to) / 2;
            Counter first = new Counter(values, from, mid, filter);
            Counter second = new Counter(values, mid, to, filter);
            // 这里可以直接采用first.fork();和second.fork()方法将2个任务执行起来
            // 但是这样的话就会有3个线程在执行，本线程和2个子线程。
            // fork join框架在一个CPU核上只可以运行一个线程
            // 采用invokeAll方法的原因在于本线程中会完成一个线程的任务，然后把其他线程再分别创建子线程
            // 也就是在本例子当中只有2个线程
            invokeAll(first,second);
            return first.join() + second.join();
        }
    }
}
```
```java
package com.utopia.forkJoin;
public interface Filter {
    boolean accept(double t);
}
```
```java
package com.utopia.forkJoin;
import java.util.concurrent.ForkJoinPool;
public class ForkJoinTest {
    public static void main(String[] args) {
        // TODO Auto-generated method stub
        final int SIZE = 10000000;
        double[] numbers = new double[SIZE];
        for(int i = 0;i < SIZE;i++) {
            // 生成10,000,000个随机数
            numbers[i] = Math.random();
        }
        // 创建RecursiveTask<Integer>的子类，也就是ForkJoin模型任务的编写
        Counter counter = new Counter(numbers, 0, numbers.length, new Filter() {
            // 对Filter接口的实现
            @Override
            public boolean accept(double t) {
                // TODO Auto-generated method stub
                return t > 0.5;
            }
        });
        // 新建ForkJoinPool,并接收RecursiveTask任务
        ForkJoinPool pool = new ForkJoinPool();
        pool.invoke(counter);
        System.out.println(counter.join());
    }
}
```
