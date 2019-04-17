# java实现死锁 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年03月09日 19:37:36[QuJack](https://me.csdn.net/A784586)阅读数：398标签：[操作系统](https://so.csdn.net/so/search/s.do?q=操作系统&t=blog)
个人分类：[操作系统](https://blog.csdn.net/A784586/article/category/6777855)









1.死锁：

死锁的定义是，两个或两个以上的线程或进程在执行过程中，由于竞争资源或者由于彼此通信而造成的一种阻塞的现象。

我们简化一下，就当成只有两个线程。现在有线程1和线程2。线程1执行过程中，先锁定了对象a，然后需要再锁定b才能继续执行代码；而线程2正巧相反，先锁定了b，需要再锁定a才能继续执行代码。这时，两个线程都等着对方解锁，才能继续执行，这时，两个线程就进入等待状态，最终不会有线程执行。这就变成了死锁。

尽量避免加多个锁，就可以尽量避免死锁。

package com.mytest.test001;




public class DeadLockTest {

public static void main(String[] args) {

Thread t9 = new Thread(new DeadLock(true));

Thread t10 = new Thread(new DeadLock(false));

t9.start();

t10.start();

}

}




class DeadLock implements Runnable {




boolean lockFormer;

static Object o1 = new Object();

static Object o2 = new Object();




DeadLock(boolean lockFormer) {

this.lockFormer = lockFormer;

}




@Override

public void run() {

if (this.lockFormer) {

synchronized (o1)
{

try {

Thread.sleep(500);

} catch (InterruptedException e) {

e.printStackTrace();

}

synchronized (o2)
 {

System.out.println("o2 1ock");

}

}

} else {

synchronized (o2)
 {

try {

Thread.sleep(500);

} catch (InterruptedException e) {

e.printStackTrace();

}

synchronized (o1)
 {

System.out.println("o1 1ock");

}

}

}

}

}











