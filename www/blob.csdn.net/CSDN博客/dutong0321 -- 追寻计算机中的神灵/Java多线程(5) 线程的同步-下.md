# Java多线程(5) 线程的同步-下 - dutong0321 -- 追寻计算机中的神灵 - CSDN博客
2018年01月11日 20:08:44[dutong0321](https://me.csdn.net/dutong0321)阅读数：144
#### 概述
线程的同步终于要进入了尾声了，我曾经想着把线程的同步划分为一篇来讲，结果我再查阅资料的时候，感觉其他博客放在一起真的不太想看，所以我就拆开来讲。本章中的代码量应该也不太多，估计是扯得可能比较厉害。
#### 监视器
监视器是为了使程序员在不考虑如何加锁的情况下，就可以保证多线程的安全性。概念是Per Brinch Hansen和Tony Hoare在20世纪70年代提出的。在上一章节中提到的线程内部锁就是Java对监视器的一种不安全的实现。
#### Volatile域
如果仅仅读写一两个实例域，需不需要加锁？其实在Java多线程共享变量时，内存与寄存器之间还有一层线程栈。也就是说CPU在做计算时，需要做的有5步： 
1.把内存中的值取到线程栈中 
2.把线程栈的值取到寄存器中 
3.寄存器中的值进行运算 
4.把运算后的值写到线程栈中 
5.把线程栈的值写到寄存器中 
现在，如果这5步要求原子性，那么我们可以直接使用锁机制来完成，但是现实中如果仅仅是取值或者是写值，也就是说仅仅需要第1，2步或者是第3，4步的话，当然采用锁机制也可以实现，但是锁机制性能却是比较差的。如果可以直接从内存中取值，直接往内存中写值的话，那么就不需要锁机制了，这就是可见性，也是Volatile关键字的作用，接下来看看用法：
```
private volatile int x;
public int getX()
{
    return x;
}
public void setX(int x)
{
    this.x = x;
}
```
但是，如果诸如下面这种语句，那么就不可以采用volatile域了，请注意：volatile只能保证可见性，不可保证原子性。
```
public void xInc()
{
    x++;
}
```
PS：在去年校招时（我大三升大四的暑假–2016年），我忘记百度、京东、阿里、腾讯和4399这5家公司不知道哪2家曾经考过volatile这个关键字，一家考的是java中的，一家考的是C/C++中的，可惜我两个都没答出来，呵呵。。。看来我被刷下去是活该，连这个都不知道。 
PPS：在Java多线程中,volatile是保证从主内存加载到寄存器的值是最新的；在C/C++中是不希望编译器将变量值缓存到寄存器或者其他高速缓存中，而是每次都通过总线操作从内存中读取值（可怜我上次写C/C++多线程连volatile是啥都不知道更别提涉及了）。
#### final变量
除了使用锁和volatile还有一种情况可以安全的访问共享域，就是该域被声明为final时。 
这是因为final一经初始化，将无法改变其引用地址。虽然，这个变量不可以变为其他数组或对象，但是对象中的元素依然是可以改变的，如果线程对这些元素进行读写，那还是需要同步的。
#### 死锁
简单来讲就是： 
老婆：你给我买礼物，我就给你零花钱； 
老公：你给我零花钱，我就给你买礼物。 
Java中是没有避免和打破这种死锁现象的，好像任何编程语言都没有吧，只能在设计的时候考虑周到些。 
PS：话说，“老婆”够可以的，先给他钱，程序不就通了么，不过也可以理解，没有安全感么；“老公”也可以的，可以想想办法比如借点钱么，不过也可以理解，这不是无理取闹么，所以就赌气必须先给零花钱。这个段子我觉得就可以解决死锁问题了。
#### 线程局部变量
共享变量是有风险的，虽然可以用同步，但是开销也是很大的。例如，假设有静态变量：
```java
public static final SimpleDateFormat dataFormat = new SimpleDateFormat("yyyy-MM-dd");
```
如果，两个进程均执行以下操作：
```java
String dateStamp = dateFormat.format(new Date());
```
结果可能就会很混乱。 
虽然，也可以在需要时构造一个局部的DataFormat对象，但开销依然很大。 
要为每个线程构造实例，可以使用以下代码：
```java
public static final ThreadLocal<SimpleDateFormat> dateFormat = new ThreadLocal<SimpleDateFormat>(){
    protected SimpleDateFormat initialValue(){
        return new SimpleDateFormat("yyyy-MM-dd");
    }
}
```
访问的方法是：
`String dateStamp = dateFormat.get().format(new Date());`
java.util.Random类是安全的，但是如果共享的话那么多个线程等待一个共享的随机数生成器会很低效。 
不过，在Java 7提供了一个便利类：
`int random = ThreadLocalRandom.current.nextInt();`
PS:以上内容几乎完全是来自于《Java核心技术》中。
#### 锁测试与超时
线程在调用lock方法获得其他线程持有锁时，很可能会造成阻塞。tryLock方法可以尝试申请锁，申请成功返回true，执行后续操作，申请失败返回false，可以不必阻塞执行其他操作。如：
```
if(myLock.tryLock()){
    //the thread owns the lock
} else {
    //do something else
}
```
在tryLock方法当中还可以传入超时参数，如：
```
if(myLock.tryLock(100,TimeUnit.MILLISECONDS)){
    //the thread owns the lock
} else {
    //do something else
}
```
如果调用带有超时参数的tryLock方法线程在等待期间被中断，将会抛出InterruptedException异常，这样就可以允许程序打破死锁了。 
PS：这节和下节内容依旧几乎完全来源于《Java核心技术》
#### 读写锁
java.util.concurrent.locks包定义了2个锁类：ReentrantLock类和ReentrantReadWriteLock类。如果多个线程读取数据很少线程修改数据，ReentrantReadWriteLock类是很有用的，在这种情况下允许多个读线程共享访问是合适的，写线程当然必须是互斥访问的。
```
Lock readLock()
//获得一个可以被多个读操作共用的读锁，排斥所有的写操作
Lock writeLock()
//获得一个写锁，排斥所有的读操作和写操作
```
例如：
```
//构造一个ReentrantReadWriteLock对象
private ReentrantReadWriteLock rwl = new ReentrantReadWriteLock();
//抽取读锁和写锁
private Lock readLock = rwl.readLock();
private Lock writeLock = rwl.writeLock();
//对所有读取方法加读锁
public int getBalance()
{
    readLock.lock();
    // 获取值
    int balance = this.balance;
    readLock.unlock();
    return balance;
}
//对所有的修改方法加写锁
public void setBalance(int balance)
{
    writeLock.lock();
    this.balance = balance;
    writeLock.unlock();
}
```
#### 废弃的stop和suspend
在第1篇中就讲到stop废弃的原因是破坏了安全性，却没有多讲。例如：在从A账户转账到B账户的过程中发生了stop，那么A账户已经减少钱，此时中断，B账户也并没有收到钱，破坏了原子性。 
suspend方法废弃的原因与破坏对象无关，但是却会造成我们之前说的死锁。suspend挂起一个持有所得的进程，那么该锁在恢复之前是不可用的。如果调用suspend方法的线程试图获得同一个锁，就会造成死锁：被挂起的线程等着被恢复，而将其挂起的线程等待获得锁。
