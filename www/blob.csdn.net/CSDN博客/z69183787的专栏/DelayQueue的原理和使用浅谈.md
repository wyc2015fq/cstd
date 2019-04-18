# DelayQueue的原理和使用浅谈 - z69183787的专栏 - CSDN博客
2018年05月31日 10:51:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：864
[https://www.cnblogs.com/wxgblogs/p/5464867.html](https://www.cnblogs.com/wxgblogs/p/5464867.html)
在谈到DelayQueue的使用和原理的时候，我们首先介绍一下DelayQueue，DelayQueue是一个无界阻塞队列，只有在延迟期满时才能从中提取元素。该队列的头部是延迟期满后保存时间最长的Delayed 元素。
　　DelayQueue阻塞队列在我们系统开发中也常常会用到，例如：缓存系统的设计，缓存中的对象，超过了空闲时间，需要从缓存中移出；任务调度系统，能够准确的把握任务的执行时间。我们可能需要通过线程处理很多时间上要求很严格的数据，如果使用普通的线程，我们就需要遍历所有的对象，一个一个的检 查看数据是否过期等，首先这样在执行上的效率不会太高，其次就是这种设计的风格也大大的影响了数据的精度。一个需要12:00点执行的任务可能12:01 才执行,这样对数据要求很高的系统有更大的弊端。由此我们可以使用DelayQueue。
　　为了具有调用行为，存放到DelayDeque的元素必须继承Delayed接口。Delayed接口使对象成为延迟对象，它使存放在DelayQueue类中的对象具有了激活日期。该接口强制执行下列两个方法。
- CompareTo(Delayed o)：Delayed接口继承了Comparable接口，因此有了这个方法。
- getDelay(TimeUnit unit):这个方法返回到激活日期的剩余时间，时间单位由单位参数指定。
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
public class DelayEvent implements Delayed {
    private Date startDate;
    public DelayEvent(Date startDate) {
        super();
        this.startDate = startDate;
    }
    @Override
    public int compareTo(Delayed o) {
        long result = this.getDelay(TimeUnit.NANOSECONDS)
                - o.getDelay(TimeUnit.NANOSECONDS);
        if (result < 0) {
            return -1;
        } else if (result > 0) {
            return 1;
        } else {
            return 0;
        }
    }
    @Override
    public long getDelay(TimeUnit unit) {
        Date now = new Date();
        long diff = startDate.getTime() - now.getTime();
        return unit.convert(diff, TimeUnit.MILLISECONDS);
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
public class DelayTask implements Runnable {
    private int id;
    private DelayQueue<DelayEvent> queue;
    public DelayTask(int id, DelayQueue<DelayEvent> queue) {
        super();
        this.id = id;
        this.queue = queue;
    }
    @Override
    public void run() {
        Date now = new Date();
        Date delay = new Date();
        delay.setTime(now.getTime() + id * 1000);
        System.out.println("Thread " + id + " " + delay);
        for (int i = 0; i < 100; i++) {
            DelayEvent delayEvent = new DelayEvent(delay);
            queue.add(delayEvent);
        }
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
public class DelayDequeMain {
    public static void main(String[] args) throws Exception {
        DelayQueue<DelayEvent> queue = new DelayQueue<DelayEvent>();
        Thread threads[] = new Thread[5];
        for (int i = 0; i < threads.length; i++) {
            DelayTask task = new DelayTask(i + 1, queue);
            threads[i] = new Thread(task);
        }
        for (int i = 0; i < threads.length; i++) {
            threads[i].start();
        }
        for (int i = 0; i < threads.length; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        do {
            int counter = 0;
            DelayEvent delayEvent;
            do {
                delayEvent = queue.poll();
                if (delayEvent != null) {
                    counter++;
                }
            } while (delayEvent != null);
            System.out.println("At " + new Date() + " you have read " + counter+ " event");
            TimeUnit.MILLISECONDS.sleep(500);
        } while (queue.size() > 0);
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
```
```java
Thread
```
```java
3
```
```java
Fri May
```
```java
06
```
```java
11
```
```java
:
```
```java
00
```
```java
:
```
```java
20
```
```java
CST
```
```java
2016
```
```java
Thread
```
```java
1
```
```java
Fri May
```
```java
06
```
```java
11
```
```java
:
```
```java
00
```
```java
:
```
```java
18
```
```java
CST
```
```java
2016
```
```java
Thread
```
```java
5
```
```java
Fri May
```
```java
06
```
```java
11
```
```java
:
```
```java
00
```
```java
:
```
```java
22
```
```java
CST
```
```java
2016
```
```java
Thread
```
```java
4
```
```java
Fri May
```
```java
06
```
```java
11
```
```java
:
```
```java
00
```
```java
:
```
```java
21
```
```java
CST
```
```java
2016
```
```java
Thread
```
```java
2
```
```java
Fri May
```
```java
06
```
```java
11
```
```java
:
```
```java
00
```
```java
:
```
```java
19
```
```java
CST
```
```java
2016
```
```java
At Fri May
```
```java
06
```
```java
11
```
```java
:
```
```java
00
```
```java
:
```
```java
17
```
```java
CST
```
```java
2016
```
```java
you have read
```
```java
0
```
```java
event
```
```java
At Fri May
```
```java
06
```
```java
11
```
```java
:
```
```java
00
```
```java
:
```
```java
18
```
```java
CST
```
```java
2016
```
```java
you have read
```
```java
0
```
```java
event
```
```java
At Fri May
```
```java
06
```
```java
11
```
```java
:
```
```java
00
```
```java
:
```
```java
18
```
```java
CST
```
```java
2016
```
```java
you have read
```
```java
100
```
```java
event
```
```java
At Fri May
```
```java
06
```
```java
11
```
```java
:
```
```java
00
```
```java
:
```
```java
19
```
```java
CST
```
```java
2016
```
```java
you have read
```
```java
0
```
```java
event
```
```java
At Fri May
```
```java
06
```
```java
11
```
```java
:
```
```java
00
```
```java
:
```
```java
19
```
```java
CST
```
```java
2016
```
```java
you have read
```
```java
100
```
```java
event
```
```java
At Fri May
```
```java
06
```
```java
11
```
```java
:
```
```java
00
```
```java
:
```
```java
20
```
```java
CST
```
```java
2016
```
```java
you have read
```
```java
0
```
```java
event
```
```java
At Fri May
```
```java
06
```
```java
11
```
```java
:
```
```java
00
```
```java
:
```
```java
20
```
```java
CST
```
```java
2016
```
```java
you have read
```
```java
100
```
```java
event
```
```java
At Fri May
```
```java
06
```
```java
11
```
```java
:
```
```java
00
```
```java
:
```
```java
21
```
```java
CST
```
```java
2016
```
```java
you have read
```
```java
0
```
```java
event
```
```java
At Fri May
```
```java
06
```
```java
11
```
```java
:
```
```java
00
```
```java
:
```
```java
21
```
```java
CST
```
```java
2016
```
```java
you have read
```
```java
100
```
```java
event
```
```java
At Fri May
```
```java
06
```
```java
11
```
```java
:
```
```java
00
```
```java
:
```
```java
22
```
```java
CST
```
```java
2016
```
```java
you have read
```
```java
0
```
```java
event
```
```java
At Fri May
```
```java
06
```
```java
11
```
```java
:
```
```java
00
```
```java
:
```
```java
22
```
```java
CST
```
```java
2016
```
```java
you have read
```
```java
100
```
```java
event
```
```
