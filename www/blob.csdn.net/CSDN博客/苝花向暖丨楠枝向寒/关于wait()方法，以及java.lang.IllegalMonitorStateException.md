# 关于wait()方法，以及java.lang.IllegalMonitorStateException - 苝花向暖丨楠枝向寒 - CSDN博客

2018年03月21日 22:36:38[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：731


今天在 run方法内部  通过Thread.currentThread().wait()调用 wait方法 

 然后报错java.lang.IllegalMonitorStateException

网上找了很多说法


 void notify() 
          唤醒在此对象监视器上等待的单个线程。 
 void notifyAll() 
          唤醒在此对象监视器上等待的所有线程。 
 void wait() 
          导致当前的线程等待，直到其他线程调用此对象的 notify() 方法或 notifyAll() 方法。

根据jdk的void notifyAll()的描述，“解除那些在该对象上调用wait()方法的线程的阻塞状态。该方法只能在同步方法或同步块内部调用。如果当前线程不是对象锁的持有者，该方法抛出一个java.lang.IllegalMonitorStateException 异常”

在我看来标红色的这句话 说的 太含糊，我思考了半天，再加上网上各种找资料，终于明白什么意思了。下面做出通俗的解释。

首先这些方法需要在 同步方法里或 同步代码块中用， 那么就涉及 一个问题，锁是什么？

如果是同步方法，那锁就是当前对象（锁也是一个对象，这里成为对象锁）。

如果是同步代码块，那就要看我们里面的参数是什么了。

synchronized(this) {

.....

}

这就是对象锁。

synchronized(Test.class) {

.....

}

这就是类锁。

重点： 当我们去调用wait()方法的时候，必须用锁这个对象 去掉用。

如 对象锁就必须用   this.wait() 调用 ，this 可以省略。

如果是 类锁 ，就必须用 this.getClass().wait()去调用。

再如  

Object obj = new Object();

synchronized(obj ) {

.....

}

那就必须用 obj .wait()去调用。

个人理解：wait()方法是让线程把锁弃掉，说白了就是我刚开始让你进来，现在不跟你这个权利，让你进来了。那这个权利的拥有者是谁？ 就是 我们 在synchronized（obj）参数中 设置的 锁对象，只有通过这个对象，才能调用wait()方法。 你这个线程没权利决定，是否给自己上锁。




