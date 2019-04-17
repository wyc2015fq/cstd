# Java并发编程规则：不可变对象永远是线程安全的 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年12月12日 18:06:14[boonya](https://me.csdn.net/boonya)阅读数：4723
所属专栏：[Java并发编程实践](https://blog.csdn.net/column/details/13974.html)









创建后状态不能被修改的对象叫作不可变对象。不可变对象天生就是线程安全的。它们的常量（变量）是在构造函数中创建的，既然它们的状态无法被修改，那么这些常量永远不会被改变——不可变对象永远是线程安全的。

## 不可变性的理解：

无论是Java语言规范还是Java存储模型都没有对不可变性做出正式的定义。不可变性并不是将域简单地等于将对象的所有变量都声明为final类型，所有域都是final类型的对象仍然可以是可变的，因为final域可以获得一个可变对象的引用。

一个不可变的对象必须满足的条件：它的状态在创建后不能再修改，所有域都是final类型，并且它被正确创建（创建期间没有发生this引用的逸出）。来看个不可变类示例：



```java
package net.jcip.examples;

import java.util.*;

import net.jcip.annotations.*;

/**
 * ThreeStooges
 * <p/>
 * Immutable class built out of mutable underlying objects,
 * demonstration of candidate for lock elision
 *
 * @author Brian Goetz and Tim Peierls
 */
@Immutable
 public final class ThreeStooges {
    private final Set<String> stooges = new HashSet<String>();

    public ThreeStooges() {
        stooges.add("Moe");
        stooges.add("Larry");
        stooges.add("Curly");
    }

    public boolean isStooge(String name) {
        return stooges.contains(name);
    }

    public String getStoogeNames() {
        List<String> stooges = new Vector<String>();
        stooges.add("Moe");
        stooges.add("Larry");
        stooges.add("Curly");
        return stooges.toString();
    }
}
```

注意：“对象是不可变的”与“对象的引用是不可变的”之间并不相等。


## 可用final修饰可变变量：

即使对象是可变的，将一些属性变量声明为final类型仍然有助于简化对其状态的判断。因为限制了对象的可见性，也就约束了其可能的状态集，即使有一两个可变的状态变量，这样一个“几乎不可变”的对象仍然比有很多可变变量的对象要简单。将变量声明为final类型，还向维护人员明确指出这些域是不可能变的。



```java
package net.jcip.examples;

import java.math.BigInteger;
import java.util.*;

import net.jcip.annotations.*;

/**
 * OneValueCache
 * <p/>
 * Immutable holder for caching a number and its factors
 *
 * @author Brian Goetz and Tim Peierls
 */
@Immutable
public class OneValueCache {
    private final BigInteger lastNumber;
    private final BigInteger[] lastFactors;

    public OneValueCache(BigInteger i,
                         BigInteger[] factors) {
        lastNumber = i;
        lastFactors = Arrays.copyOf(factors, factors.length);
    }

    public BigInteger[] getFactors(BigInteger i) {
        if (lastNumber == null || !lastNumber.equals(i))
            return null;
        else
            return Arrays.copyOf(lastFactors, lastFactors.length);
    }
}
```



## 用volatile发布不可变对象：

使用volatile变量（仅可见性，而不具备同步性）不能保证线程安全性，但有时不可变对象也可以提供一种弱形式的原子性。




```java
package net.jcip.examples;

import java.math.BigInteger;
import javax.servlet.*;

import net.jcip.annotations.*;

/**
 * VolatileCachedFactorizer
 * <p/>
 * Caching the last result using a volatile reference to an immutable holder object
 *
 * @author Brian Goetz and Tim Peierls
 */
@ThreadSafe
public class VolatileCachedFactorizer extends GenericServlet implements Servlet {
    private volatile OneValueCache cache = new OneValueCache(null, null);

    public void service(ServletRequest req, ServletResponse resp) {
        BigInteger i = extractFromRequest(req);
        BigInteger[] factors = cache.getFactors(i);
        if (factors == null) {
            factors = factor(i);
            cache = new OneValueCache(i, factors);
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


## 简单地发布安全对象：

**--------------------------安全发布对象的条件：----------------------------**
1、通过静态初始化对象的引用；
2、将引用存储到volatile变量或AutomaticReference;
3、将引用存储到final域字段中；
4、将引用存储到由锁正确保护的域中；

在多线程中，常量（变量）最好是声明为不可变类型，即使用final关键字。如下是一个不正确发布的对象：



```java
package net.jcip.examples;

/**
 * Holder
 * <p/>
 * Class at risk of failure if not properly published
 *
 * @author Brian Goetz and Tim Peierls
 */
public class Holder {
    private int n;

    public Holder(int n) {
        this.n = n;
    }

    public void assertSanity() {
        if (n != n)
            throw new AssertionError("This statement is false.");
    }
}
```


修正方式：将变量n用final修饰，使之不可变（即线程安全）。





```java
package net.jcip.examples;

/**
 * Holder
 * <p/>
 * Class at risk of failure if not properly published
 *
 * @author Brian Goetz and Tim Peierls
 */
public class Holder {
    private final int n;

    public Holder(int n) {
        this.n = n;
    }

    public void assertSanity() {
        if (n != n)
            throw new AssertionError("This statement is false.");
    }
}
```
注意：不可变对象可以在没有任何额外同步的情况下，安全地用于任意线程；甚至发布它时也不需要同步。


## **发布对象必要条件依赖于对象的可变性：**

1、不可变对象可以依赖任何机制发布；

2、高效不可变对象必须安全地发布；
3、可变对象必须要安全发布，同时必须要线程安全或被锁保护。

## Java中支持线程安全保证的类：

1、置入HashTable、synchronizedMap、ConcurrentMap中的主键以及值，会安全地发布到可以从Map获取他们的任意线程中，无论是直接还是通过迭代器（Iterator）获得。

2、置入Vector、CopyOnWriteArrayList、CopyOnWriteArraySet、synchronizedList或者synchronizedSet中的元素，会安全地发布到可以从容器中获取它的任意线程中。

3、置入BlockingQueue或ConcurrentLinkedQueue的元素，会安全地发布到可以从容器中获取它的任意线程中。
通常，以最简单最安全的方式发布一个类就是使用静态初始化：



```java
public static Holder holder=new Holder(99);
```

任何线程都可以在没有额外的同步下安全地使用一个安全发布的高效不可变对象。




```java
public Map<String,Date> lastLogin=Collections.synchronizedMap(new HashMap<String, Date>());
```
Date是可变类型，通过Collections.synchronizedMap安全的数据结构使得使用它的结果不可变，从而不需要额外的同步。



## 安全地共享对象：

在并发程序中，使用和共享对象的一些最有效的策略如下：

**-------------线程限制：--------------**

一个线程限制的对象，通过限制在线程中，而被线程独占，且只能被占有它的线程修改。

**-------------共享只读(read-only)：-------------**

一个共享的只读对象，在没有额外同步的情况下，可以被多个线程并发地访问，但任何线程都不能修改它。共享只读对象包括可变对象与高效不可变对象。


**-------------共享线程安全(thread-safe)：-------------**

一个线程安全的对象在内部进行同步，所以其它线程无额外同步，就可以通过公共接口随意地访问它。


**-------------被守护（Guarded）：-------------**


一个被守护的对象只能通过特定的锁来访问。被守护的对象包括哪些被线程安全对象封装的对象，和已知特定的锁保护起来的已发布对象。




































