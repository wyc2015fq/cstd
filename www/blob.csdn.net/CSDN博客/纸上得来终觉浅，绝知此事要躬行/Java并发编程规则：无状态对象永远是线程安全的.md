# Java并发编程规则：无状态对象永远是线程安全的 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年12月07日 17:44:52[boonya](https://me.csdn.net/boonya)阅读数：3044
所属专栏：[Java并发编程实践](https://blog.csdn.net/column/details/13974.html)









## 规则说明：

无状态对象即无状态类，是指其本身没有内部变量和外部变量的操作的，在每个用户访问的线程栈中都是一个各自的实例。

## 线程安全的表现：

一个线程对该类的访问不会影响其他线程的访问结果。

## 线程安全的无状态类示例：



```java
package net.jcip.examples;

import java.math.BigInteger;
import javax.servlet.*;

import net.jcip.annotations.*;

/**
 * StatelessFactorizer
 *
 * A stateless servlet
 * 
 * @author Brian Goetz and Tim Peierls
 */
@ThreadSafe
public class StatelessFactorizer extends GenericServlet implements Servlet {

    public void service(ServletRequest req, ServletResponse resp) {
        BigInteger i = extractFromRequest(req);
        BigInteger[] factors = factor(i);
        encodeIntoResponse(resp, factors);
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


注意：一个线程访问的StatelessFactorizer不会影响其他线程访问StatelessFactorizer的结果，两个线程不共享状态，如同是在访问两个实例。








