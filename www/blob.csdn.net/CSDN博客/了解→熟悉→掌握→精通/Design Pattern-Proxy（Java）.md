
# Design Pattern - Proxy（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 17:15:50[Chimomo](https://me.csdn.net/chimomo)阅读数：187


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
# Definition
Provide a surrogate or placeholder for another object to control access to it.
# Participants
The classes and/or objects participating in this pattern are:
Proxy(MathProxy)Maintains a reference that lets the proxy access the real subject. Proxy may refer to a Subject if the RealSubject and Subject interfaces are the same.
Provides an interface identical to Subject's so that a proxy can be substituted for the real subject
Controls access to the real subject and may be responsible for creating and deleting it
Other responsibilites depend on the kind of proxy:Remote proxiesare responsible for encoding a request and its arguments and for sending the encoded request to the real subject in a different address space
Virtual proxiesmay cache additional information about the real subject so that they can postpone accessing it. For example, the ImageProxy from the Motivation caches the real images's extent.
Protection proxiescheck that the caller has the access permissions required to perform a request

Subject(IMath)Defines the common interface for RealSubject and Proxy so that a Proxy can be used anywhere a RealSubject is expected
RealSubject(Math)Defines the real object that the proxy represents

# Sample Code in Java
---
Thisstructuralcode demonstrates the Proxy pattern which provides a representative object (proxy) that controls access to another similar object.
```python
package chimomo.learning.java.designpattern.proxy.sample;
/**
 * Startup class for Structural Proxy Design Pattern
 *
 * @author Chimomo
 */
class Program {
    /**
     * Entry point into console application
     *
     * @param args The arguments
     */
    public static void main(String[] args) {
        // Create proxy and request a service
        Proxy proxy = new Proxy();
        proxy.request();
    }
}
/* ------ Running Results ------
Called RealSubject.request()
*/
```
```python
package chimomo.learning.java.designpattern.proxy.sample;
/**
 * The 'Proxy' class
 *
 * @author Chimomo
 */
class Proxy extends Subject {
    // The real subject
    private RealSubject realSubject;
    /**
     * Request
     */
    @Override
    public void request() {
        // Use 'lazy initialization'
        if (this.realSubject == null) {
            this.realSubject = new RealSubject();
        }
        this.realSubject.request();
    }
}
```
```python
package chimomo.learning.java.designpattern.proxy.sample;
/**
 * The 'RealSubject' class
 *
 * @author Chimomo
 */
class RealSubject extends Subject {
    /**
     * Request
     */
    @Override
    public void request() {
        System.out.println("Called RealSubject.request()");
    }
}
```
```python
package chimomo.learning.java.designpattern.proxy.sample;
/**
 * The 'Subject' abstract class
 *
 * @author Chimomo
 */
abstract class Subject {
    /**
     * Request
     */
    public abstract void request();
}
```
---
This real-world code demonstrates the Proxy pattern for a Math object represented by a MathProxy object.
```python
package chimomo.learning.java.designpattern.proxy.realworld;
/**
 * The 'Subject' interface
 *
 * @author Chimomo
 */
public interface IMath {
    /**
     * Add
     *
     * @param x The x
     * @param y The y
     * @return The sum of x and y
     */
    double add(double x, double y);
    /**
     * Sub
     *
     * @param x The x
     * @param y The y
     * @return The difference of x and y
     */
    double sub(double x, double y);
    /**
     * Mul
     *
     * @param x The x
     * @param y The y
     * @return The product of x and y
     */
    double mul(double x, double y);
    /**
     * Div
     *
     * @param x The x
     * @param y The y
     * @return The quotient of x and y
     */
    double div(double x, double y);
}
```
```python
package chimomo.learning.java.designpattern.proxy.realworld;
/**
 * The 'RealSubject' class
 *
 * @author Chimomo
 */
class Math implements IMath {
    /**
     * Add
     *
     * @param x The x
     * @param y The y
     * @return The sum of x and y
     */
    @Override
    public double add(double x, double y) {
        return x + y;
    }
    /**
     * Sub
     *
     * @param x The x
     * @param y The y
     * @return The difference of x and y
     */
    @Override
    public double sub(double x, double y) {
        return x - y;
    }
    /**
     * Mul
     *
     * @param x The x
     * @param y The y
     * @return The product of x and y
     */
    @Override
    public double mul(double x, double y) {
        return x * y;
    }
    /**
     * Div
     *
     * @param x The x
     * @param y The y
     * @return The quotient of x and y
     */
    @Override
    public double div(double x, double y) {
        return x / y;
    }
}
```
```python
package chimomo.learning.java.designpattern.proxy.realworld;
/**
 * The 'Proxy Object' class
 *
 * @author Chimomo
 */
class MathProxy implements IMath {
    // The math
    private Math math = new Math();
    /**
     * Add
     *
     * @param x The x
     * @param y The y
     * @return The sum of x and y
     */
    @Override
    public double add(double x, double y) {
        return this.math.add(x, y);
    }
    /**
     * Sub
     *
     * @param x The x
     * @param y The y
     * @return The difference of x and y
     */
    @Override
    public double sub(double x, double y) {
        return this.math.sub(x, y);
    }
    /**
     * Mul
     *
     * @param x The x
     * @param y The y
     * @return The product of x and y
     */
    @Override
    public double mul(double x, double y) {
        return this.math.mul(x, y);
    }
    /**
     * Div
     *
     * @param x The x
     * @param y The y
     * @return The quotient of x and y
     */
    @Override
    public double div(double x, double y) {
        return this.math.div(x, y);
    }
}
```
```python
package chimomo.learning.java.designpattern.proxy.realworld;
/**
 * Startup class for Real-World Proxy Design Pattern
 *
 * @author Chimomo
 */
class Program {
    /**
     * Entry point into console application
     *
     * @param args The arguments
     */
    public static void main(String[] args) {
        // Create math proxy
        MathProxy proxy = new MathProxy();
        // Do the math
        System.out.println("4 + 2 = " + proxy.add(4, 2));
        System.out.println("4 - 2 = " + proxy.sub(4, 2));
        System.out.println("4 * 2 = " + proxy.mul(4, 2));
        System.out.println("4 / 2 = " + proxy.div(4, 2));
    }
}
/* ------ Running Results ------
4 + 2 = 6.0
4 - 2 = 2.0
4 * 2 = 8.0
4 / 2 = 2.0
*/
```

