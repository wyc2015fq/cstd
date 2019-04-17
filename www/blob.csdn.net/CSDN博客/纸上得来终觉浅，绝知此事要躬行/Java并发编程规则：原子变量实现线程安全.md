# Java并发编程规则：原子变量实现线程安全 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年12月07日 18:10:51[boonya](https://me.csdn.net/boonya)阅读数：1989
所属专栏：[Java并发编程实践](https://blog.csdn.net/column/details/13974.html)









## 判定规则：

如果一个类中存在变量，并且此变量的操作不是原子操作，那么这个类就是非线程安全的类。在线程产生竞争条件的情况下，多线程访问导致原子性不可保证。

## 竞争条件产生的原因：

当计算的正确性依赖于运行时中相关的时序或多线程的交替时，会产生竞争条件。多线程情况下，线程的时序不能确定，所以一旦程序依赖于特定的执行时序就会产生竞争条件。所以，在多线程环境下必须保证线程的可见性可控或避免产生竞争条件。内存可见性，可以理解为变量是否对外可访问，是否受保护。**线程之间出现竞争，是因为业务依赖于时序操作或变量的可见性未加以约束。**一般如果要实现内存可见性控制有两种方法，即使用synchronized和volatile关键字。



```java
class SafetyWithSynchronized{

    private int something = 0;

    public synchronized  int getSomething ( ) {
        return something;
    }

    public synchronized  void setSomething (int something) {
        this.something = something;
    }
}

class SafetyWithVolatile{
   private volatile int something = 0;

   public int getSomething ( ) {
        return something;
   }
   public void setSomething (int something) {
        this.something = something;
   }
}
```


使用volatile 关键字的方案，在性能上要好很多，因为volatile是一个轻量级的同步，只能保证多线程的内存可见性，不能保证多线程的执行有序性。因此开销远比synchronized要小。**Volatile 变量具有 synchronized 的可见性特性，但是不具备原子特性。**这就是说线程能够自动发现 volatile 变量的最新值。Volatile 变量可用于提供线程安全，但是只能应用于非常有限的一组用例：多个变量之间或者某个变量的当前值与修改后值之间没有约束。出于简易性或可伸缩性的考虑，大多数情况下可能倾向于使用
 volatile 变量而不是锁。当使用 volatile 变量而非锁时，某些习惯用法（idiom）更加易于编码和阅读。此外，volatile 变量不会像锁那样造成线程阻塞，因此也很少造成可伸缩性问题。在某些情况下，如果读操作远远大于写操作，volatile 变量还可以提供优于锁的性能优势。


## CAS原理实现原子操作：



**原子操作：**

在复杂的业务处理中，假设有操作A和B。若果从执行A的线程看，当其它线程执行B时，要么B全部执行完成，要么一点都没有执行，这样A和B互为原子操作。一个原子操作是指：该操作对于所有的操作，包括它自己，都满足前面描述的状态。


CAS 指的是现代 CPU 广泛支持的一种对内存中的共享数据进行操作的一种特殊指令。这个指令会对内存中的共享数据做原子的读写操作。**简单介绍一下这个指令的操作过程：首先，CPU 会将内存中将要被更改的数据与期望的值做比较。然后，当这两个值相等时，CPU 才会将内存中的数值替换为新的值。否则便不做操作。最后，CPU 会将旧的数值返回。这一系列的操作是原子的。它们虽然看似复杂，但却是 Java 5 并发机制优于原有锁机制的根本。简单来说，CAS 的含义是“我认为原有的值应该是什么，如果是，则将原有的值更新为新值，否则不做修改，并告诉我原来的值是多少”**。（这段描述引自《Java并发编程实践》）

**java.util.concurrent并发包实现原子操作：**





由于java的CAS同时具有 volatile 读和volatile写的内存语义，因此Java线程之间的通信现在有了下面四种方式：
- A线程写volatile变量，随后B线程读这个volatile变量。
- A线程写volatile变量，随后B线程用CAS更新这个volatile变量。
- A线程用CAS更新一个volatile变量，随后B线程用CAS更新这个volatile变量。
- A线程用CAS更新一个volatile变量，随后B线程读这个volatile变量。


Java的CAS会使用现代处理器上提供的高效机器级别原子指令，这些原子指令以原子方式对内存执行读-改-写操作，这是在多处理器中实现同步的关键（从本质上来说，能够支持原子性读-改-写指令的计算机器，是顺序计算图灵机的异步等价机器，因此任何现代的多处理器都会去支持某种能对内存执行原子性读-改-写操作的原子指令）。同时，volatile变量的读/写和CAS可以实现线程之间的通信。把这些特性整合在一起，就形成了整个concurrent包得以实现的基石。如果我们仔细分析concurrent包的源代码实现，会发现一个通用化的实现模式：
- 首先，声明共享变量为volatile；
- 然后，使用CAS的原子条件更新来实现线程之间的同步；
- 同时，配合以volatile的读/写和CAS所具有的volatile读和写的内存语义来实现线程之间的通信。

## 常见原子变量：

在java.util.concurrent.atomic包下还有很多类，使用这些类可以保证对这些类的诸如“获取-更新”操作是原子性的，从而避发生竞争条件。[jdk1.7 java.util.concurrent.atomic](http://docs.oracle.com/javase/7/docs/api/java/util/concurrent/atomic/package-summary.html)的原子变量：

```java
AtomicBoolean 	//A boolean value that may be updated atomically.
AtomicInteger 	//An int value that may be updated atomically.
AtomicIntegerArray 	//An int array in which elements may be updated atomically.
AtomicIntegerFieldUpdater<T> 	//A reflection-based utility that enables atomic updates to designated volatile int fields of designated classes.
AtomicLong 	//A long value that may be updated atomically.
AtomicLongArray 	//A long array in which elements may be updated atomically.
AtomicLongFieldUpdater<T> 	//A reflection-based utility that enables atomic updates to designated volatile long fields of designated classes.
AtomicMarkableReference<V> 	//An AtomicMarkableReference maintains an object reference along with a mark bit, that can be updated atomically.
AtomicReference<V> 	//An object reference that may be updated atomically.
AtomicReferenceArray<E> 	//An array of object references in which elements may be updated atomically.
AtomicReferenceFieldUpdater<T,V> 	//A reflection-based utility that enables atomic updates to designated volatile reference fields of designated classes.
AtomicStampedReference<V> 	//An AtomicStampedReference maintains an object reference along with an integer "stamp", that can be updated atomically.
```

注：学习并发编程的时候尽可能地通过二手资料去发掘一手资料，这样可以学到更多，最好是从Java API中去寻找。



## 非线程安全示例：



```java
package net.jcip.examples;

import java.math.BigInteger;
import javax.servlet.*;

import net.jcip.annotations.*;

/**
 * UnsafeCountingFactorizer
 *
 * Servlet that counts requests without the necessary synchronization
 *
 * @author Brian Goetz and Tim Peierls
 */
@NotThreadSafe
public class UnsafeCountingFactorizer extends GenericServlet implements Servlet {
    private long count = 0;

    public long getCount() {
        return count;
    }

    public void service(ServletRequest req, ServletResponse resp) {
        BigInteger i = extractFromRequest(req);
        BigInteger[] factors = factor(i);
        ++count;
        encodeIntoResponse(resp, factors);
    }

    void encodeIntoResponse(ServletResponse res, BigInteger[] factors) {
    }

    BigInteger extractFromRequest(ServletRequest req) {
        return new BigInteger("7");
    }

    BigInteger[] factor(BigInteger i) {
        // Doesn't really factor
        return new BigInteger[] { i };
    }
}
```


注：此类在单线程环境中可以很好的运行，但是在多线程环境中就是不是线程安全的了，因为++count不是原子操作。



## 线程安全示例：



```java
package net.jcip.examples;

import java.math.BigInteger;
import java.util.concurrent.atomic.*;
import javax.servlet.*;

import net.jcip.annotations.*;

/**
 * CountingFactorizer
 *
 * Servlet that counts requests using AtomicLong
 *
 * @author Brian Goetz and Tim Peierls
 */
@ThreadSafe
public class CountingFactorizer extends GenericServlet implements Servlet {
    private final AtomicLong count = new AtomicLong(0);

    public long getCount() { return count.get(); }

    public void service(ServletRequest req, ServletResponse resp) {
        BigInteger i = extractFromRequest(req);
        BigInteger[] factors = factor(i);
        count.incrementAndGet();
        encodeIntoResponse(resp, factors);
    }

    void encodeIntoResponse(ServletResponse res, BigInteger[] factors) {}
    BigInteger extractFromRequest(ServletRequest req) {return null; }
    BigInteger[] factor(BigInteger i) { return null; }
}
```

## 利用原子变量实现简单的自增序列实例：



```java
class Sequencer {
   private final AtomicLong sequenceNumber= new AtomicLong(0);

  public long next() {
     return sequenceNumber.getAndIncrement();
   }
 }
```

## 对原子变量操作的总结：



1. 知道++操作不是原子操作。

2. 非原子操作有线程安全问题。

3. 并发下的内存可见性可以通过synchronized和volatile加以控制。

4. Atomic类通过CAS + volatile可以比synchronized做的更高效，推荐使用。

5.利用AtomicLong这样已有的线程安全管理类的状态是非常实用的，相比于非线程安全对象，判断一个线程安全对象的可能状态和状态的装换要容易得多。这简化了维护和验证线程安全性的工作。






