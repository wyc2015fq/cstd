# java并发之DelayQueue实际运用示例 - z69183787的专栏 - CSDN博客
2018年05月31日 10:56:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：243
[https://www.cnblogs.com/shamo89/p/7055039.html](https://www.cnblogs.com/shamo89/p/7055039.html)
在学习Java 多线程并发开发过程中，了解到DelayQueue类的主要作用：是一个无界的BlockingQueue，用于放置实现了Delayed接口的对象，其中的对象只能在其到期时才能从队列中取走。这种队列是有序的，即队头对象的延迟到期时间最长。注意：不能将null元素放置到这种队列中。
Delayed，一种混合风格的接口，用来标记那些应该在给定延迟时间之后执行的对象。此接口的实现必须定义一个 compareTo 方法，该方法提供与此接口的 getDelay 方法一致的排序。
在网上也看到两个示例，但这两个示例个人在实际运行时均没有达到满足业务场景的效果，因而对其进行了修改，供大家参考讨论。
### 业务场景一：多考生考试
该场景来自于[http://ideasforjava.iteye.com/blog/657384](http://ideasforjava.iteye.com/blog/657384)，模拟一个考试的日子，考试时间为120分钟，30分钟后才可交卷，当时间到了，或学生都交完卷了考试结束。
这个场景中几个点需要注意：
- 考试时间为120分钟，30分钟后才可交卷，初始化考生完成试卷时间最小应为30分钟
- 对于能够在120分钟内交卷的考生，如何实现这些考生交卷
- 对于120分钟内没有完成考试的考生，在120分钟考试时间到后需要让他们强制交卷
- 在所有的考生都交完卷后，需要将控制线程关闭
实现思想：用DelayQueue存储考生（Student类），每一个考生都有自己的名字和完成试卷的时间，Teacher线程对DelayQueue进行监控，收取完成试卷小于120分钟的学生的试卷。当考试时间120分钟到时，先关闭Teacher线程，然后强制DelayQueue中还存在的考生交卷。每一个考生交卷都会进行一次countDownLatch.countDown()，当countDownLatch.await()不再阻塞说明所有考生都交完卷了，而后结束考试。
![复制代码](https://common.cnblogs.com/images/copycode.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
package com.my.base.concurrent.delayQueue;
import java.util.Iterator;
import java.util.Random;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.DelayQueue;
import java.util.concurrent.Delayed;
import java.util.concurrent.TimeUnit;
/**
 *this project is created for my partactice.
 *In the  project I will write the mybatis by myself
 *
 *2014-1-10  下午9:43:48
 *@author 孙振超   mychaoyue2011@163.com
 */
public class Exam {
    /**
     *
     *2014-1-10 下午9:43:48 by 孙振超
     *
     *@param args
     *void
     * @throws InterruptedException 
     */
    public static void main(String[] args) throws InterruptedException {
        // TODO Auto-generated method stub
        int studentNumber = 20;
        CountDownLatch countDownLatch = new CountDownLatch(studentNumber+1);
        DelayQueue< Student> students = new DelayQueue<Student>();
        Random random = new Random();
        for (int i = 0; i < studentNumber; i++) {
            students.put(new Student("student"+(i+1), 30+random.nextInt(120),countDownLatch));
        }
        Thread teacherThread =new Thread(new Teacher(students)); 
        students.put(new EndExam(students, 120,countDownLatch,teacherThread));
        teacherThread.start();
        countDownLatch.await();
        System.out.println(" 考试时间到，全部交卷！");  
    }
}
class Student implements Runnable,Delayed{
    private String name;
    private long workTime;
    private long submitTime;
    private boolean isForce = false;
    private CountDownLatch countDownLatch;
    
    public Student(){}
    
    public Student(String name,long workTime,CountDownLatch countDownLatch){
        this.name = name;
        this.workTime = workTime;
        this.submitTime = TimeUnit.NANOSECONDS.convert(workTime, TimeUnit.NANOSECONDS)+System.nanoTime();
        this.countDownLatch = countDownLatch;
    }
    
    @Override
    public int compareTo(Delayed o) {
        // TODO Auto-generated method stub
        if(o == null || ! (o instanceof Student)) return 1;
        if(o == this) return 0; 
        Student s = (Student)o;
        if (this.workTime > s.workTime) {
            return 1;
        }else if (this.workTime == s.workTime) {
            return 0;
        }else {
            return -1;
        }
    }
    @Override
    public long getDelay(TimeUnit unit) {
        // TODO Auto-generated method stub
        return unit.convert(submitTime - System.nanoTime(),  TimeUnit.NANOSECONDS);
    }
    @Override
    public void run() {
        // TODO Auto-generated method stub
        if (isForce) {
            System.out.println(name + " 交卷, 希望用时" + workTime + "分钟"+" ,实际用时 120分钟" );
        }else {
            System.out.println(name + " 交卷, 希望用时" + workTime + "分钟"+" ,实际用时 "+workTime +" 分钟");  
        }
        countDownLatch.countDown();
    }
    public boolean isForce() {
        return isForce;
    }
    public void setForce(boolean isForce) {
        this.isForce = isForce;
    }
    
}
class EndExam extends Student{
    private DelayQueue<Student> students;
    private CountDownLatch countDownLatch;
    private Thread teacherThread;
    
    public EndExam(DelayQueue<Student> students, long workTime, CountDownLatch countDownLatch,Thread teacherThread) {
        super("强制收卷", workTime,countDownLatch);
        this.students = students;
        this.countDownLatch = countDownLatch;
        this.teacherThread = teacherThread;
    }
    
    
    
    @Override
    public void run() {
        // TODO Auto-generated method stub
        
        teacherThread.interrupt();
        Student tmpStudent;
        for (Iterator<Student> iterator2 = students.iterator(); iterator2.hasNext();) {
            tmpStudent = iterator2.next();
            tmpStudent.setForce(true);
            tmpStudent.run();
        }
        countDownLatch.countDown();
    }
    
}
class Teacher implements Runnable{
    private DelayQueue<Student> students;
    public Teacher(DelayQueue<Student> students){
        this.students = students;
    }
    
    @Override
    public void run() {
        // TODO Auto-generated method stub
        try {
            System.out.println(" test start");
            while(!Thread.interrupted()){
                students.take().run();
            }
        } catch (Exception e) {
            // TODO: handle exception
            e.printStackTrace();
        }
    }
    
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
### 业务场景二：具有过期时间的缓存
该场景来自于[http://www.cnblogs.com/jobs/archive/2007/04/27/730255.html](http://www.cnblogs.com/jobs/archive/2007/04/27/730255.html)，向缓存添加内容时，给每一个key设定过期时间，系统自动将超过过期时间的key清除。
这个场景中几个点需要注意：
- 当向缓存中添加key-value对时，如果这个key在缓存中存在并且还没有过期，需要用这个key对应的新过期时间
- 为了能够让DelayQueue将其已保存的key删除，需要重写实现Delayed接口添加到DelayQueue的DelayedItem的hashCode函数和equals函数
- 当缓存关闭，监控程序也应关闭，因而监控线程应当用守护线程
具体实现如下：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
package com.my.base.concurrent.delayQueue;
import java.util.Random;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.DelayQueue;
import java.util.concurrent.Delayed;
import java.util.concurrent.TimeUnit;
/**
 *Cache.java
 *
 * Created on 2014-1-11 上午11:30:36 by sunzhenchao mychaoyue2011@163.com
 */
public class Cache<K, V> {
    public ConcurrentHashMap<K, V> map = new ConcurrentHashMap<K, V>();
    public DelayQueue<DelayedItem<K>> queue = new DelayQueue<DelayedItem<K>>();
    
    
    public void put(K k,V v,long liveTime){
        V v2 = map.put(k, v);
        DelayedItem<K> tmpItem = new DelayedItem<K>(k, liveTime);
        if (v2 != null) {
            queue.remove(tmpItem);
        }
        queue.put(tmpItem);
    }
    
    public Cache(){
        Thread t = new Thread(){
            @Override
            public void run(){
                dameonCheckOverdueKey();
            }
        };
        t.setDaemon(true);
        t.start();
    }
    
    public void dameonCheckOverdueKey(){
        while (true) {
            DelayedItem<K> delayedItem = queue.poll();
            if (delayedItem != null) {
                map.remove(delayedItem.getT());
                System.out.println(System.nanoTime()+" remove "+delayedItem.getT() +" from cache");
            }
            try {
                Thread.sleep(300);
            } catch (Exception e) {
                // TODO: handle exception
            }
        }
    }
    
    /**
     * TODO
     * @param args
     * 2014-1-11 上午11:30:36
     * @author:孙振超
     * @throws InterruptedException 
     */
    public static void main(String[] args) throws InterruptedException {
        Random random = new Random();
        int cacheNumber = 10;
        int liveTime = 0;
        Cache<String, Integer> cache = new Cache<String, Integer>();
        
        for (int i = 0; i < cacheNumber; i++) {
            liveTime = random.nextInt(3000);
            System.out.println(i+"  "+liveTime);
            cache.put(i+"", i, random.nextInt(liveTime));
            if (random.nextInt(cacheNumber) > 7) {
                liveTime = random.nextInt(3000);
                System.out.println(i+"  "+liveTime);
                cache.put(i+"", i, random.nextInt(liveTime));
            }
        }
        Thread.sleep(3000);
        System.out.println();
    }
}
class DelayedItem<T> implements Delayed{
    private T t;
    private long liveTime ;
    private long removeTime;
    
    public DelayedItem(T t,long liveTime){
        this.setT(t);
        this.liveTime = liveTime;
        this.removeTime = TimeUnit.NANOSECONDS.convert(liveTime, TimeUnit.NANOSECONDS) + System.nanoTime();
    }
    
    @Override
    public int compareTo(Delayed o) {
        if (o == null) return 1;
        if (o == this) return  0;
        if (o instanceof DelayedItem){
            DelayedItem<T> tmpDelayedItem = (DelayedItem<T>)o;
            if (liveTime > tmpDelayedItem.liveTime ) {
                return 1;
            }else if (liveTime == tmpDelayedItem.liveTime) {
                return 0;
            }else {
                return -1;
            }
        }
        long diff = getDelay(TimeUnit.NANOSECONDS) - o.getDelay(TimeUnit.NANOSECONDS);
        return diff > 0 ? 1:diff == 0? 0:-1;
    }
    @Override
    public long getDelay(TimeUnit unit) {
        return unit.convert(removeTime - System.nanoTime(), unit);
    }
    public T getT() {
        return t;
    }
    public void setT(T t) {
        this.t = t;
    }
    @Override
    public int hashCode(){
        return t.hashCode();
    }
    
    @Override
    public boolean equals(Object object){
        if (object instanceof DelayedItem) {
            return object.hashCode() == hashCode() ?true:false;
        }
        return false;
    }
    
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
