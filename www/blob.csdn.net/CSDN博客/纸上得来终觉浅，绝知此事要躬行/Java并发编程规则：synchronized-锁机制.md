# Java并发编程规则：synchronized-锁机制 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年12月08日 14:17:59[boonya](https://me.csdn.net/boonya)阅读数：900
所属专栏：[Java并发编程实践](https://blog.csdn.net/column/details/13974.html)









前面说过的，即使是线程安全的类，也不一定就是线程安全的。当一个不变约束涉及多个变量时，变量间不是彼此独立的：某个变量的值会制约其他变量的值。因此更新一个变量的时候，要在同一原子操作中更新其他变量的值。为了保护状态的一致性，要在单一的原子操作中更新相互关联的状态变量。



## 用锁来保护状态：



对于每个可被多个线程访问的可变状态，如果所有访问它的线程在执行时都占有同一个锁，这种情况，我们称这个变量是由这个锁保护的。

每个共享的可变变量都需要由唯一一个确定的锁保护，而维护者应该清楚这个锁。

对于每一个涉及多个变量的不变约束，需要同一个锁保护其所有变量。


## 使用线程安全类的非线程安全示例：



```java
package net.jcip.examples;

import java.math.BigInteger;
import java.util.concurrent.atomic.*;
import javax.servlet.*;

import net.jcip.annotations.*;

/**
 * UnsafeCachingFactorizer
 *
 * Servlet that attempts to cache its last result without adequate atomicity
 *
 * @author Brian Goetz and Tim Peierls
 */

@NotThreadSafe
public class UnsafeCachingFactorizer extends GenericServlet implements Servlet {
    private final AtomicReference<BigInteger> lastNumber
            = new AtomicReference<BigInteger>();
    private final AtomicReference<BigInteger[]> lastFactors
            = new AtomicReference<BigInteger[]>();

    public void service(ServletRequest req, ServletResponse resp) {
        BigInteger i = extractFromRequest(req);
        if (i.equals(lastNumber.get()))
            encodeIntoResponse(resp, lastFactors.get());
        else {
            BigInteger[] factors = factor(i);
            lastNumber.set(i);
            lastFactors.set(factors);
            encodeIntoResponse(resp, factors);
        }
    }

    void encodeIntoResponse(ServletResponse resp, BigInteger[] factors) {
    }

    BigInteger extractFromRequest(ServletRequest req) {
        return new BigInteger("7");
    }

    BigInteger[] factor(BigInteger i) {
        // Doesn't really factor
        return new BigInteger[]{i};
    }
}
```

线程安全的定义要求，无论是多线程中的时序或交替操作，都要保证那些不变约束。UnsafeCachingFactorizer的一个不变约束是：缓存在lastFactors的各个因子的乘积应该等于缓存在lastNumber的数值。只有遵循这个不变约束，那么该Servlet才是正确的。



## 最影响性能的同步锁方法（不推荐）：

同步锁方法，即在方法上加synchronized修饰符加以标记。这种方法在多线程环境中访问时，禁止多个用户同时访问service方法，这是不合理的设计。



```java
package net.jcip.examples;

import java.math.BigInteger;
import javax.servlet.*;

import net.jcip.annotations.*;

/**
 * SynchronizedFactorizer
 *
 * Servlet that caches last result, but with unnacceptably poor concurrency
 *
 * @author Brian Goetz and Tim Peierls
 */

@ThreadSafe
public class SynchronizedFactorizer extends GenericServlet implements Servlet {
    @GuardedBy("this") private BigInteger lastNumber;
    @GuardedBy("this") private BigInteger[] lastFactors;

    public synchronized void service(ServletRequest req,
                                     ServletResponse resp) {
        BigInteger i = extractFromRequest(req);
        if (i.equals(lastNumber))
            encodeIntoResponse(resp, lastFactors);
        else {
            BigInteger[] factors = factor(i);
            lastNumber = i;
            lastFactors = factors;
            encodeIntoResponse(resp, factors);
        }
    }

    void encodeIntoResponse(ServletResponse resp, BigInteger[] factors) {
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



synchronized方法是基于“每调用(per-invocation)”的模式，并没有发挥出多线程的优势，所以性能（performance）自然不言而喻。虽然使用synchronized的方法使我们获得了安全性，但是付出了高昂的代价（牺牲性能），这是一种弱并发处理模式。



## 可重入的内部锁（推荐）：



Java提供了强制原子性的内置锁：synchronized块。一个synchronized块包含两部分：锁保护的对象，以及锁保护的代码块。例如：

```java
// 所保护的对象this
synchronized(this){
   // 锁保护的代码块（访问或修改被保护的共享状态）
   // do something..........
}
```

获得内部锁唯一的途径是：进入这个内部锁保护的同步块或方法。内部锁在Java中扮演了互斥锁的角色，意味着至多只有一个线程可以拥有锁。方法内部的同步块是可重入的。当一个线程请求其他线程已经占有的锁时，请求线程将被阻塞，然而内部锁时可重进入的，因此线程在试图获得它自己占有的锁时，请求会成功。重进入是基于“每线程(per-thread)”的，而不是“每调用(per-invocation)”，这就是内部锁的优势所在。




## 进一步缩小synchronized的范围提高性能：

通过缩小synchronized块的范围来提高性能，谨慎地控制synchronized块的大小，保证其原子性操作不被破坏，分离不影响共享状态的耗时操作。来看一个示例：



```java
package net.jcip.examples;

import java.math.BigInteger;
import javax.servlet.*;

import net.jcip.annotations.*;

/**
 * CachedFactorizer
 * <p/>
 * Servlet that caches its last request and result
 *
 * @author Brian Goetz and Tim Peierls
 */
@ThreadSafe
public class CachedFactorizer extends GenericServlet implements Servlet {
    @GuardedBy("this") private BigInteger lastNumber;
    @GuardedBy("this") private BigInteger[] lastFactors;
    @GuardedBy("this") private long hits;
    @GuardedBy("this") private long cacheHits;

    public synchronized long getHits() {
        return hits;
    }

    public synchronized double getCacheHitRatio() {
        return (double) cacheHits / (double) hits;
    }

    public void service(ServletRequest req, ServletResponse resp) {
        BigInteger i = extractFromRequest(req);
        BigInteger[] factors = null;
        synchronized (this) {
            ++hits;
            if (i.equals(lastNumber)) {
                ++cacheHits;
                factors = lastFactors.clone();
            }
        }
        if (factors == null) {
            factors = factor(i);
            synchronized (this) {
                lastNumber = i;
                lastFactors = factors.clone();
            }
        }
        encodeIntoResponse(resp, factors);
    }

    void encodeIntoResponse(ServletResponse resp, BigInteger[] factors) {
    }

    BigInteger extractFromRequest(ServletRequest req) {
        return new BigInteger("7");
    }

    BigInteger[] factor(BigInteger i) {
        // Doesn't really factor
        return new BigInteger[]{i};
    }
}
```



为什么这里不用AtomicLong了呢？因为提供线程安全的代价是性能，使用AtomicLong有时也不能保证线程安全，所以安全性和性能都不能得到保证。

注意：通常简单性与性能之间是相互牵制的，实现一个同步策略时，不要过早地为了性能而牺牲简单性（这是对安全性潜在的妥协）。有些耗时的计算或操作，比如网络或控制I/O，难以快速完成，执行这些操作期间不要占用锁。










