
# Design Pattern - Chain of Responsibility（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 17:16:25[Chimomo](https://me.csdn.net/chimomo)阅读数：160


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
# Definition
Avoid coupling the sender of a request to its receiver by giving more than one object a chance to handle the request. Chain the receiving objects and pass the request along the chain until an object handles it.
# Participants
The classes and/or objects participating in this pattern are:
Handler(Approver)Defines an interface for handling the requests
(optional) Implements the successor link
ConcreteHandler(Director, VicePresident, President)Handles requests it is responsible for
Can access its successor
If the ConcreteHandler can handle the request, it does so; otherwise it forwards the request to its successor
Client(ChainApp)Initiates the request to a ConcreteHandler object on the chain

# Sample Code in Java
---
Thisstructuralcode demonstrates the Chain of Responsibility pattern in which several linked objects (the Chain) are offered the opportunity to respond to a request or hand it off to the object next in line.
```python
package chimomo.learning.java.designpattern.chainofresponsibility.sample;
/**
 * The 'ConcreteHandler1' class
 *
 * @author Chimomo
 */
class ConcreteHandler1 extends Handler {
    /**
     * Handle request
     *
     * @param request The request
     */
    @Override
    public void handleRequest(int request) {
        if (request >= 0 && request < 10) {
            System.out.println(String.format("%s handled request %d", this.getClass().getSimpleName(), request));
        } else if (this.successor != null) {
            this.successor.handleRequest(request);
        }
    }
}
```
```python
package chimomo.learning.java.designpattern.chainofresponsibility.sample;
/**
 * The 'ConcreteHandler2' class
 *
 * @author Chimomo
 */
class ConcreteHandler2 extends Handler {
    /**
     * Handle request
     *
     * @param request The request
     */
    @Override
    public void handleRequest(int request) {
        if (request >= 10 && request < 20) {
            System.out.println(String.format("%s handled request %d", this.getClass().getSimpleName(), request));
        } else if (this.successor != null) {
            this.successor.handleRequest(request);
        }
    }
}
```
```python
package chimomo.learning.java.designpattern.chainofresponsibility.sample;
/**
 * The 'ConcreteHandler3' class
 *
 * @author Chimomo
 */
class ConcreteHandler3 extends Handler {
    /**
     * Handle request
     *
     * @param request The request
     */
    @Override
    public void handleRequest(int request) {
        if (request >= 20 && request < 30) {
            System.out.println(String.format("%s handled request %d", this.getClass().getSimpleName(), request));
        } else if (this.successor != null) {
            this.successor.handleRequest(request);
        }
    }
}
```
```python
package chimomo.learning.java.designpattern.chainofresponsibility.sample;
/**
 * The 'Handler' abstract class
 *
 * @author Chimomo
 */
abstract class Handler {
    // The successor
    Handler successor;
    /**
     * Handle request
     *
     * @param request The request
     */
    public abstract void handleRequest(int request);
    /**
     * Set successor
     *
     * @param successor The successor
     */
    void setSuccessor(Handler successor) {
        this.successor = successor;
    }
}
```
```python
package chimomo.learning.java.designpattern.chainofresponsibility.sample;
/**
 * Startup class for Structural Chain of Responsibility Design Pattern
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
        // Setup Chain of Responsibility
        Handler h1 = new ConcreteHandler1();
        Handler h2 = new ConcreteHandler2();
        Handler h3 = new ConcreteHandler3();
        h1.setSuccessor(h2);
        h2.setSuccessor(h3);
        // Generate and process request
        int[] requests = {2, 5, 14, 22, 18, 3, 27, 20};
        for (int request : requests) {
            h1.handleRequest(request);
        }
    }
}
/* ------ Running Results ------
ConcreteHandler1 handled request 2
ConcreteHandler1 handled request 5
ConcreteHandler2 handled request 14
ConcreteHandler3 handled request 22
ConcreteHandler2 handled request 18
ConcreteHandler1 handled request 3
ConcreteHandler3 handled request 27
ConcreteHandler3 handled request 20
*/
```
---
This real-world code demonstrates the Chain of Responsibility pattern in which several linked managers and executives can respond to a purchase request or hand it off to a superior. Each position has can its own set of rules which orders they can approve.
```python
package chimomo.learning.java.designpattern.chainofresponsibility.realworld;
/**
 * The 'Handler' abstract class
 *
 * @author Chimomo
 */
abstract class Approver {
    // The successor
    Approver successor;
    /**
     * Process request
     *
     * @param purchase The purchase
     */
    public abstract void processRequest(Purchase purchase);
    /**
     * Set successor
     *
     * @param successor The successor
     */
    void setSuccessor(Approver successor) {
        this.successor = successor;
    }
}
```
```python
package chimomo.learning.java.designpattern.chainofresponsibility.realworld;
/**
 * The 'ConcreteHandler' class
 *
 * @author Chimomo
 */
class Director extends Approver {
    /**
     * Process request
     *
     * @param purchase The purchase
     */
    @Override
    public void processRequest(Purchase purchase) {
        if (purchase.getAmount() < 10000.0) {
            System.out.println(String.format("%s approved request #%d", this.getClass().getSimpleName(), purchase.getNumber()));
        } else if (this.successor != null) {
            this.successor.processRequest(purchase);
        }
    }
}
```
```python
package chimomo.learning.java.designpattern.chainofresponsibility.realworld;
/**
 * The 'ConcreteHandler' class
 *
 * @author Chimomo
 */
class President extends Approver {
    /**
     * Process request
     *
     * @param purchase The purchase
     */
    @Override
    public void processRequest(Purchase purchase) {
        if (purchase.getAmount() < 100000.0) {
            System.out.println(String.format("%s approved request #%d", this.getClass().getSimpleName(), purchase.getNumber()));
        } else {
            System.out.println(String.format("Request #%d requires an executive meeting!", purchase.getNumber()));
        }
    }
}
```
```python
package chimomo.learning.java.designpattern.chainofresponsibility.realworld;
/**
 * Startup class for Real-World Chain of Responsibility Design Pattern
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
        // Setup Chain of Responsibility
        Approver larry = new Director();
        Approver sam = new VicePresident();
        Approver tammy = new President();
        larry.setSuccessor(sam);
        sam.setSuccessor(tammy);
        // Generate and process purchase requests
        Purchase p = new Purchase(2034, 350.00);
        larry.processRequest(p);
        p = new Purchase(2035, 32590.10);
        larry.processRequest(p);
        p = new Purchase(2036, 122100.00);
        larry.processRequest(p);
    }
}
/* ------ Running Results ------
Director approved request #2034
President approved request #2035
Request #2036 requires an executive meeting!
*/
```
```python
package chimomo.learning.java.designpattern.chainofresponsibility.realworld;
/**
 * Class holding request details
 *
 * @author Chimomo
 */
class Purchase {
    // The amount
    private double amount;
    // The number
    private int number;
    /**
     * Initializes a new instance of the "Purchase" class
     *
     * @param number The number
     * @param amount The amount
     */
    Purchase(int number, double amount) {
        this.number = number;
        this.amount = amount;
    }
    /**
     * Get number
     *
     * @return The number
     */
    int getNumber() {
        return this.number;
    }
    /**
     * Get amount
     *
     * @return The amount
     */
    double getAmount() {
        return this.amount;
    }
}
```
```python
package chimomo.learning.java.designpattern.chainofresponsibility.realworld;
/**
 * The 'ConcreteHandler' class
 *
 * @author Chimomo
 */
class VicePresident extends Approver {
    /**
     * Process request
     *
     * @param purchase The purchase
     */
    @Override
    public void processRequest(Purchase purchase) {
        if (purchase.getAmount() < 25000.0) {
            System.out.println(String.format("%s approved request #%d", this.getClass().getSimpleName(), purchase.getNumber()));
        } else if (this.successor != null) {
            this.successor.processRequest(purchase);
        }
    }
}
```

