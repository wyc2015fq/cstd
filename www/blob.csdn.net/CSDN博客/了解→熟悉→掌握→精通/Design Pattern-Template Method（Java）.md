
# Design Pattern - Template Method（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 17:24:37[Chimomo](https://me.csdn.net/chimomo)阅读数：136


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
# Definition
Define the skeleton of an algorithm in an operation, deferring some steps to subclasses. Template Method lets subclasses redefine certain steps of an algorithm without changing the algorithm's structure.
# Participants
The classes and/or objects participating in this pattern are:
AbstractClass(DataObject)Defines abstractprimitive operationsthat concrete subclasses define to implement steps of an algorithm
Implements a template method defining the skeleton of an algorithm. The template method calls primitive operations as well as operations defined in AbstractClass or those of other objects
ConcreteClass(CustomerDataObject)Implements the primitive operations or carry out subclass-specific steps of the algorithm

# Sample Code in Java
This structural code demonstrates the Template Method which provides a skeleton calling sequence of methods. One or more steps can be deferred to subclasses which implement these steps without changing the overall calling sequence.
```python
package chimomo.learning.java.designpattern.templatemethod.sample;
/**
 * The 'AbstractClass' abstract class
 *
 * @author Chimomo
 */
abstract class AbstractClass {
    /**
     * Primitive operation 1
     */
    public abstract void primitiveOperation1();
    /**
     * Primitive operation 2
     */
    public abstract void primitiveOperation2();
    /**
     * Template method
     */
    void templateMethod() {
        this.primitiveOperation1();
        this.primitiveOperation2();
        System.out.println();
    }
}
```
```python
package chimomo.learning.java.designpattern.templatemethod.sample;
/**
 * A 'ConcreteClass' class
 *
 * @author Chimomo
 */
class ConcreteClassA extends AbstractClass {
    /**
     * Primitive operation 1
     */
    @Override
    public void primitiveOperation1() {
        System.out.println("ConcreteClassA.primitiveOperation1()");
    }
    /**
     * Primitive operation 2
     */
    @Override
    public void primitiveOperation2() {
        System.out.println("ConcreteClassA.primitiveOperation2()");
    }
}
```
```python
package chimomo.learning.java.designpattern.templatemethod.sample;
/**
 * A 'ConcreteClass' class
 *
 * @author Chimomo
 */
class ConcreteClassB extends AbstractClass {
    /**
     * Primitive operation 1
     */
    @Override
    public void primitiveOperation1() {
        System.out.println("ConcreteClassB.primitiveOperation1()");
    }
    /**
     * Primitive operation 2
     */
    @Override
    public void primitiveOperation2() {
        System.out.println("ConcreteClassB.primitiveOperation2()");
    }
}
```
```python
package chimomo.learning.java.designpattern.templatemethod.sample;
/**
 * Startup class for Sample Template Method design pattern
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
        AbstractClass aA = new ConcreteClassA();
        aA.templateMethod();
        AbstractClass aB = new ConcreteClassB();
        aB.templateMethod();
    }
}
/* ------ Running Results ------
ConcreteClassA.primitiveOperation1()
ConcreteClassA.primitiveOperation2()
ConcreteClassB.primitiveOperation1()
ConcreteClassB.primitiveOperation2()

*/
```


