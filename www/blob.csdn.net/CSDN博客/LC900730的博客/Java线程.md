# Java线程 - LC900730的博客 - CSDN博客
2017年11月12日 14:02:09[lc900730](https://me.csdn.net/LC900730)阅读数：107标签：[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
## 继承自Thread方法
thread.run() 
和thread的start()方法 
必须是start， 
如果是run()的话，那么就是普通的方法调用。 
如果是调用thread的run(),那么就是普通的方法调用,不会创建新线程。
## 继承自runnable方法
new Thread(传入runnable对象，名字)：这样就产生一个线程
### Synchronized
```
public function get(){
}
thread1.run(){
    get();
}
thread2.run(){
    get();
}
当一个线程获取了对应锁，并执行该代码块时，其他线程便只能一直等待，等待获取锁的线程释放锁，而这里获取锁的线程释放锁只有2种情况
```
#### 用法
synchronized()
```
public class MySynchronized{
    public static void main(String[] args){
     final MySynchronized mySynchronized=new MySynchronized(); 
     final MySynchronized mySynchronized2=new MySynchronized(); 
        new Thread("thread1"){
            public void run(){
             synchronized(mySynchronized){
             try{
                System.out.println(this.getName()+"start");
                Thread.sleep(5000);
                System.out.println(this.getName()+"醒了");
                System.out.println(this.getName()+"end");
             }catch(InterruptdException e){
                    e.printStackTrace();
             }
            }.start();
        new Thread("thread2"){
            public void run(){
             synchronized(mySynchronized){
             try{
                System.out.println(this.getName()+"start");
                Thread.sleep(5000);
                System.out.println(this.getName()+"醒了");
                System.out.println(this.getName()+"end");
             }catch(InterruptdException e){
                    e.printStackTrace();
             }
            }.start();
            }
        }
    }
}
```
### 缺点：
但是Sychrnoized不会解决线程之间读的问题，即时多个线程读写也会被堵塞。
### LOCK实现
Lock是一个类，不是java关键字。Concurrent包中的对象。 
Synchronized不需要用户自己释放锁，Lock必须手动释放锁。 
java.util.concurrent.Lock： 
Lock是一个接口。
```
public interface Lock(){
    void lock();
    void lockInterruptibly() throws InterruptedException;
    boolean tryLock();
    boolean tryLock(longtime,TimeUnit unit )throwsInterruptedException;
    void unlock();
}
//其中lock() tryLock() tryLock(long time,TimeUnit unit) 
lockInterruptibly()是用来获取锁的
unlock()是用来释放锁的
```
### lockInterruptibly
通过这个方式获取锁的话，如果线程正在等待获取锁，安么这个线程能够响应中断，即中断线程的等待状态。也就是说，当2个线程同时通过lock.lockInterruptibly()想获取锁，如果线程A获取到了锁，线程B只有等待，那么线程B调用threadB.Interrupt()能够中断线程B的等待过程。
当一个线程获取到了锁之后，是不会被interrupt方法打断的。 
当通过lockInterruptibly（）方法 获取到某个锁，如果获取不到，只有进行等待的情况下，是可以被打断的 
而用synchronized（）如果处于等待状态，是无法被中断的，只有一直等待下去。 
tryLock()方法是有返回值的，表示尝试获取锁，获取成功返回true，失败返回false。
