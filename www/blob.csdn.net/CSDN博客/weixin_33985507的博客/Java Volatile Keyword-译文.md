# Java Volatile Keyword - 译文 - weixin_33985507的博客 - CSDN博客
2017年03月16日 07:55:19[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
## Java Volatile Keyword
  并发是程序界的量子物理，然而volatile又是量子物理中薛定谔的猫。本篇文章试图系统的梳理一下java中的Volatile关键字。这篇译文可能帮助你更好的理解volatile关键字。
   使用volatile关键字是解决同步问题的一种有效手段。 java  volatile关键字预示着这个变量始终是“存储进入了主存”。更精确的表述就是每一次读一个volatile变量，都会从主存读取，而不是CPU的缓存。同样的道理，每次写一个volatile变量，都是写回主存，而不仅仅是CPU的缓存。
  事实上，JAVA5的volatile关键字不只是保证了每次从主存读写数据。下面将着重介绍volatile关键字的特性。
  Java 保证volatile关键字保证变量的改变对各个线程是可见的。这看起来有点抽象，不过将紧接着说明这一点。
我们知道，每一个线程都有自己的线程栈。多线程在操作非volatile变量的时候，都会从主存拷贝变量值到自己的栈内存中间，然后再操作变量。在多个线程的情况下，如果一个线程修改了变量值还未回写到主内存，另一个线程读取的就是一个旧的值，这样会出现问题，因为读到的变量不是最新的。实际上，在多核CPU中间，由于每个CPU都有自己的缓存，同样会存在主存与CPU缓存之间数据不一致的情况。因此，在C语言中，也有volatile关键字。（译者注：实际上，如果在CPU的层面满足volatile特性，那么线程栈就一定满足。因为从volatile语义来讲，jvm线程每次只从主存读写volatile变量，而主存的volatile变量又在CPU层面满足volatile语义）
想象一种这样的情况，有两个或者更多的线程访问一个共享对象，这个共享对象包括了一个counter变量：
public class SharedObject {
```java
public int counter = 0;复制代码
```
}
  再想象一下，只有线程1对counter变量加一，但是线程一和线程2却是同时读到这个变量。
  如果这个contouer变量没有被声明为volatile。
就不能保证counter变量从cpu缓存回写到主存。这就意味着counter变量在cpu缓存中的值与主存中值不一致。
  这就是所谓的线程不能看到变量最新值的问题。因为另外一个线程并没有及时将变量写回到主存。这样一个线程的人更新对其他线程是不可见的。
  通过声明counter变量是一个volatile变量，这样所有counter变量的更改就会被立即写入主存。同样，对counter变量的读也从主存里面读。下面是如何声明一个volatile变量：
```java
public class SharedObject {
    public volatile int counter = 0;
}复制代码
```
  通过声明volatile变量就保证了对其他线程写的可见性。
## java volatile的happen-before保证
  java5中的volatile关键字不只是保证从主存中读写数据，实际上，volatile还保证如下的情况：
- 如果线程a写一个volatile变量，随后线程b读取这个变量，然后所有的变量在线程a写之前可见，所有的变量也在b读之后对线程b可见了。（译者注：volatile有两个语义：可见性与读写原子性。a在写变量的过程中，b是无法读取的。因为CPU会锁定这块内存区域的缓存并回写到内存。此时B才可以读取，如果A在写的过程中B可以读取，那么线程B读取的是脏数据。i++之所以无法用volatile保证原子性。是因为volatile仅仅保证读取加锁，赋值加锁，而对于中间的加1操作是不会加锁的。线程B如果在这个期间读取值，那肯定会是脏数据。）
  读和写volatile变量的指令无法被JVM重排序（JVM为了提高性能可以重排序一些指令，只要程序的行为与排序前一样）但是volatile变量却无法重排序，也就是volatile变量的读和写无法被打乱在其他变量中间。不管是什么指令，总是在volatile变量读写之后发生。
  下面将会详细的解释这一点。
  当一个线程写一个volatile变量，然后不仅仅是volatile变量本身自身写入到主存。所有其他的在写volatile变量之前也会被刷入主存。当一个线程读volatile变量的时候，它也会从主存读取其他变量。（译者注：注意是所有的变量。每次在写入volatile变量的时候，线程栈里面的所有的共享变量都将刷回主存，而不仅仅是在volatile变量声明之前的变量）
  看下面这个例子，sharedObject.counter是一个volatile变量：
```java
Thread A:
    sharedObject.nonVolatile = 123;
    sharedObject.counter     = sharedObject.counter + 1;
Thread B:
    int counter     = sharedObject.counter;
    int nonVolatile = sharedObject.nonVolatile;复制代码
```
  当线程A写在写入volatile变量sharedObject.counter之前写入一个非volatile变量，然后再写入volatile变量，这个时候非volatile变量sharedObject.nonVolatile 也会被写入主存。
  当线程B开始读一个volatile变量sharedObject.counter，然后所有的sharedObject.nonVolatile以及
  sharedObject.counter都会从主存读取。这个时候sharedObject.nonVolatile值与线程A中的值是一样的。
  开发者可以使用这种扩展的可视性来优化线程之间的可视性：不是对每个变量都声明为volatile变量，而是只需要声明其中一部分变量为volatile。下面是Exchanger类，就利用了上述的原则：
```java
public class Exchanger {
    private Object   object       = null;
    private volatile hasNewObject = false;
    public void put(Object newObject) {
        while(hasNewObject) {
            //wait - do not overwrite existing new object
        }
        object = newObject;
        hasNewObject = true; //volatile write
    }
    public Object take(){
        while(!hasNewObject){ //volatile read
            //wait - don't take old object (or null)
        }
        Object obj = object;
        hasNewObject = false; //volatile write
        return obj;
    }
}复制代码
```
  线程A一遍又一遍的调用put()方法。线程B一遍又一遍的调用take方法。这个Exchanger能够在合理使用volatile关键字的情况下工作的很好。只要线程A只调用put方法，线程b只调用take方法。
  然而，JVM是可以对指令进行优化的。如果JVM对指令优化，打乱了顺序，会出现什么样的效果呢？下面这段代码可能是执行的顺序之一：
```java
while(hasNewObject) {
    //wait - do not overwrite existing new object
}
hasNewObject = true; //volatile write
object = newObject;复制代码
```
  注意到volatile变量hasNewObject现在在object被设置之前执行了。这个对于JVM来说看起来好像是合法的，因为这两个值的写入指令相互是没有依赖的，JVM可以对它们重排序。
  然而，重排指令有可能影响到object变量的可见性。首先，线程B看见在线程A还没有对object赋值之前就看见了hasNewObject是一个true变量，这样操作线程B读取了一个空值。其次，这甚至不能保证object变量会被及时的写入到主存。（当然，下一次线程A更改volatile变量的时候就会被刷进主存）
  为了阻止上面的任何一种情况发生，volatile保证了“happens before ”特性。happens-before特性保证volatile变量的读写不能被重排序。也就是对volatile变量的读写不能插入到其他的任何指令中。
  看下面这个例子：
```java
sharedObject.nonVolatile1 = 123;
sharedObject.nonVolatile2 = 456;
sharedObject.nonVolatile3 = 789;
sharedObject.volatile     = true; //一个 volatile 变量
int someValue1 = sharedObject.nonVolatile4;
int someValue2 = sharedObject.nonVolatile5;
int someValue3 = sharedObject.nonVolatile6;复制代码
```
  JVM 可能重排序前三个指令。只要他们全部在volatile写入指令前发生（他们必须在volatile写入前全部执行）
  类似的，JVM可能重排序最后三个指令。只要volatile变量写操作在它们前发生。最后这三个指令都不能被排在volatile变量写指令前面。
  这就是最基本的javavolatile变量的happens before原则。
## volatile通常是不够的。
  即使是volatile关键字保证了读写都是从主存读取，然而仍然有写情况不能简单的使用variable变量来解决。在早先讲到的例子中，当线程1写入一个变量counter这个volatile之后，就能保证线程2读到这个最新的值。
  事实上，如果线程在写volatile变量并不依赖于这个volatile之前的值，那么在写的过程中，主存中仍然是当前的值。
  然后一个线程开始读这个volatile变量。那么这个线程读到的值就是旧的值，可见性就是不正确的。这就会造成读变量和写变量之间的竞争。volatile关键字只是保证了下一次读取的是最新的变量，但是在另外一个变量写入的过程中，读到的值仍然是旧的。（**译者注：如果是多个CPU先写后读，在写的过程中实际上会发出信号，告知其缓存已经失效，所以并不会存在这种情况；至于先读后写，读取一个旧的值的时候要在代码里保证并不会引发任何错误。**）。
