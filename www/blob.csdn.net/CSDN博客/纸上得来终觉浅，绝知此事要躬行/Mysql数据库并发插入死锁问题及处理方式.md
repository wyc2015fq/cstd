# Mysql数据库并发插入死锁问题及处理方式 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月22日 15:47:57[boonya](https://me.csdn.net/boonya)阅读数：19193








Mysql有很多坑，对Mysql多线程支持这块不是很熟的话就会莫名其妙地发生一些诡异的问题。多线程线程并发操作时最容易产生死锁问题。所以很多大数据的操作一般都采用NoSQL数据库方案来处理，或者读写分离，只需要做好幂等设计即可。




## 如何避免数据库并发




1.通过数据库连接池做分发处理：设置何时的数据库连接池参数。

如配置：线程池大小，空闲时间，超时时间等。


2.永远不要手动开启线程插入数据：不要手动开启线程去执行批量插入。



```java
new Thread(new Runnable(){ public void run(){// 批量插入数据}}).start(); //线程内部批量执行插入
```
3.执行线程的时候尽量使用线程池而不是直接使用Thread




```java
package com.boonya.thread;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

class TaskRunnable implements Runnable {
    @Override
    public void run() {
        for (int x = 0; x < 100; x++) {
          Thread.sleep(1000);//休眠1000ms
         System.out.println("Do something:"+x);
       }
    }
}

public class ExecutorServiceDemo {
    public static void main(String[] args) {
     // 创建一个线程池对象，控制要创建几个线程对象。
     // public static ExecutorService newFixedThreadPool(int nThreads)
     ExecutorService pool = Executors.newFixedThreadPool(2);

     // 可以执行Runnable对象或者Callable对象代表的线程
     pool.submit(new TaskRunnable ());
     pool.submit(new TaskRunnable ());

    //结束线程池
    pool.shutdown();
   }
}
```


注：上面的线程池示例主要是为了节省系统资源占用，顺带提一下。






## 如何处理死锁问题




Mysql对插入问题的描述：
INSERT sets an exclusive lock on the inserted row. This lock is an index-record lock, not a next-key lock (that is, there is no gap lock) and does not prevent other sessions from inserting into the gap before the inserted row.Prior to inserting the row, a type of gap lock called an insertion intention gap lock is set. This lock signals the intent to insert in such a way that multiple transactions inserting into the same index gap need not wait for each other if they are not inserting at the same position within the gap.If a duplicate-key error occurs, a shared lock on the duplicate index record is set. This use of a shared lock can result in deadlock should there be multiple sessions trying to insert the same row if another session already has an exclusive lock.  

大体的意思是：insert会对插入成功的行加上排它锁，这个排它锁是个记录锁，而非next-key锁（当然更不是gap锁了），不会阻止其他并发的事务往这条记录之前插入记录。在插入之前，会先在插入记录所在的间隙加上一个插入意向gap锁（简称I锁吧），并发的事务可以对同一个gap加I锁。如果insert 的事务出现了duplicate-key error ，事务会对duplicate index record加共享锁。这个共享锁在并发的情况下是会产生死锁的，比如有两个并发的insert都对要对同一条记录加共享锁，而此时这条记录又被其他事务加上了排它锁，排它锁的事务提交或者回滚后，两个并发的insert操作是会发生死锁的。



1. 并发插入时，不在一个事务内进行再次事务提交。
2. 通过其他手段，如缓存方案，解决这个要并发插入的问题。
3. 改并发为串行执行。

参考文档：[一次Mysql死锁排查过程](http://825635381.iteye.com/blog/2339434)


## 将并发处理都放入事务进行管理

事务管理的数据量不能太大，太大会造成数据库插入并发死锁。

**处理死锁表可以将数据库表备份：然后重新创建导入数据。**




### 解锁

第一种：
show processlist;
找到锁进程，kill id ;

第二种：
mysql>UNLOCK TABLES;


### 锁表

锁定数据表，避免在备份过程中，表被更新

mysql>LOCK TABLES tbl_name READ;

为表增加一个写锁定：
mysql>LOCK TABLES tbl_name WRITE;





参考-JDBC批量插入事务分批插入#实战演练：[http://blog.csdn.net/boonya/article/details/70157820#t43](http://blog.csdn.net/boonya/article/details/70157820#t43)













