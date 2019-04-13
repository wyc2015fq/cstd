
# Design Pattern - Observer（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 17:21:59[Chimomo](https://me.csdn.net/chimomo)阅读数：94


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
# Definition
Define a one-to-many dependency between objects so that when one object changes state, all its dependents are notified and updated automatically.
# Participants
The classes and/or objects participating in this pattern are:
Subject(Stock)Knows its observers. Any number of Observer objects may observe a subject
Provides an interface for attaching and detaching Observer objects
ConcreteSubject(IBM)Stores state of interest to ConcreteObserver
Sends a notification to its observers when its state changes
Observer(IInvestor)Defines an updating interface for objects that should be notified of changes in a subject
ConcreteObserver(Investor)Maintains a reference to a ConcreteSubject object
Stores state that should stay consistent with the subject's
Implements the Observer updating interface to keep its state consistent with the subject's

# Sample Code in Java
---
This structural code demonstrates the Observer pattern in which registered objects are notified of and updated with a state change.
```python
package chimomo.learning.java.designpattern.observer.sample;
/**
 * The 'ConcreteObserver' class
 *
 * @author Chimomo
 */
class ConcreteObserver extends Observer {
    // The name
    private String name;
    // The subject
    private ConcreteSubject subject;
    /**
     * Initializes a new instance of the "ConcreteObserver" class
     *
     * @param name The name
     * @param subject The subject
     */
    ConcreteObserver(String name, ConcreteSubject subject) {
        this.name = name;
        this.subject = subject;
    }
    /**
     * Update
     */
    @Override
    public void update() {
        // The observer state
        String observerState = this.subject.getSubjectState();
        System.out.println(String.format("Observer %s's new state is %s", this.name, observerState));
    }
}
```
```python
package chimomo.learning.java.designpattern.observer.sample;
/**
 * The 'ConcreteSubject' class
 *
 * @author Chimomo
 */
class ConcreteSubject extends Subject {
    // The subject state
    private String subjectState;
    /**
     * Get subject state
     *
     * @return The subject state
     */
    String getSubjectState() {
        return this.subjectState;
    }
    /**
     * Set subject state
     *
     * @param subjectState The subject state
     */
    void setSubjectState(String subjectState) {
        this.subjectState = subjectState;
    }
}
```
```python
package chimomo.learning.java.designpattern.observer.sample;
/**
 * The 'Observer' abstract class
 *
 * @author Chimomo
 */
abstract class Observer {
    /**
     * Update.
     */
    public abstract void update();
}
```
```python
package chimomo.learning.java.designpattern.observer.sample;
/**
 * Startup class for Structural Observer Design Pattern
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
        // Configure observer pattern
        ConcreteSubject s = new ConcreteSubject();
        ConcreteObserver w = new ConcreteObserver("W", s);
        s.attach(w);
        s.attach(new ConcreteObserver("X", s));
        s.attach(new ConcreteObserver("Y", s));
        s.attach(new ConcreteObserver("Z", s));
        // Change subject and notifies observers
        s.setSubjectState("ABC");
        s.notifies();
        // Detach observer
        s.detach(w);
        // Change subject and notifies observers
        s.setSubjectState("DEF");
        s.notifies();
    }
}
/* ------ Running Results ------
Observer W's new state is ABC
Observer X's new state is ABC
Observer Y's new state is ABC
Observer Z's new state is ABC
Observer X's new state is DEF
Observer Y's new state is DEF
Observer Z's new state is DEF
*/
```
```python
package chimomo.learning.java.designpattern.observer.sample;
import java.util.ArrayList;
import java.util.List;
/**
 * The 'Subject' abstract class
 *
 * @author Chimomo
 */
abstract class Subject {
    // The observers
    private List<Observer> observers = new ArrayList<>();
    /**
     * Attach observer
     *
     * @param observer The observer
     */
    public void attach(Observer observer) {
        this.observers.add(observer);
    }
    /**
     * Detach observer
     *
     * @param observer The observer
     */
    void detach(Observer observer) {
        this.observers.remove(observer);
    }
    /**
     * Notify
     */
    void notifies() {
        for (Observer o : this.observers) {
            o.update();
        }
    }
}
```

---
This real-world code demonstrates the Observer pattern in which registered investors are notified every time a stock changes value.
```python
package chimomo.learning.java.designpattern.observer.realworld;
/**
 * The 'ConcreteSubject' class
 *
 * @author Chimomo
 */
class IBM extends Stock {
    /**
     * Initializes a new instance of the "IBM" class
     *
     * @param symbol The symbol
     * @param price The price
     */
    IBM(String symbol, double price) {
        super(symbol, price);
    }
}
```
```python
package chimomo.learning.java.designpattern.observer.realworld;
/**
 * The 'Observer' interface
 *
 * @author Chimomo
 */
interface IInvestor {
    /**
     * Update
     *
     * @param stock The stock
     */
    void update(Stock stock);
}
```
```python
package chimomo.learning.java.designpattern.observer.realworld;
/**
 * The 'ConcreteObserver' class
 *
 * @author Chimomo
 */
class Investor implements IInvestor {
    // The name
    private String name;
    /**
     * Initializes a new instance of the "Investor" class
     *
     * @param name The name
     */
    Investor(String name) {
        this.name = name;
    }
    /**
     * Update
     */
    @Override
    public void update(Stock stock) {
        System.out.println(String.format("Notified %s of %s's " + "change to $%f", this.name, stock.getSymbol(), stock.getPrice()));
    }
}
```
```python
package chimomo.learning.java.designpattern.observer.realworld;
/**
 * Startup class for Real-World Observer Design Pattern
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
        // Create IBM stock and attach investors
        IBM ibm = new IBM("IBM", 120.00);
        Investor kyo = new Investor("Kyo");
        ibm.attach(kyo);
        ibm.attach(new Investor("Sorros"));
        ibm.attach(new Investor("Berkshire"));
        // Fluctuating prices will notify investors
        ibm.setPrice(120.10);
        ibm.setPrice(121.00);
        ibm.setPrice(120.50);
        // Detach investors
        ibm.detach(kyo);
        // Fluctuating prices will notify investors
        ibm.setPrice(120.00);
        ibm.setPrice(120.01);
    }
}
/* ------ Running Results ------
Notified Kyo of IBM's change to $120.100000
Notified Sorros of IBM's change to $120.100000
Notified Berkshire of IBM's change to $120.100000
Notified Kyo of IBM's change to $121.000000
Notified Sorros of IBM's change to $121.000000
Notified Berkshire of IBM's change to $121.000000
Notified Kyo of IBM's change to $120.500000
Notified Sorros of IBM's change to $120.500000
Notified Berkshire of IBM's change to $120.500000
Notified Sorros of IBM's change to $120.000000
Notified Berkshire of IBM's change to $120.000000
Notified Sorros of IBM's change to $120.010000
Notified Berkshire of IBM's change to $120.010000
*/
```
```python
package chimomo.learning.java.designpattern.observer.realworld;
import java.util.ArrayList;
import java.util.List;
/**
 * The 'Subject' abstract class
 *
 * @author Chimomo
 */
abstract class Stock {
    // The investors
    private List<IInvestor> investors = new ArrayList<>();
    // The symbol
    private String symbol;
    // The price
    private double price;
    /**
     * Initializes a new instance of the "Stock" class
     *
     * @param symbol The symbol
     * @param price The price
     */
    Stock(String symbol, double price) {
        this.symbol = symbol;
        this.price = price;
    }
    /**
     * Get symbol
     *
     * @return The symbol
     */
    String getSymbol() {
        return this.symbol;
    }
    /**
     * Get price
     *
     * @return The price
     */
    double getPrice() {
        return this.price;
    }
    /**
     * Set price
     * @param price The price
     */
    void setPrice(double price) {
        if (this.price != price) {
            this.price = price;
            this.notifies();
        }
    }
    /**
     * Attach
     *
     * @param investor The investor
     */
    public void attach(IInvestor investor) {
        this.investors.add(investor);
    }
    /**
     * Detach
     *
     * @param investor The investor
     */
    void detach(IInvestor investor) {
        this.investors.remove(investor);
    }
    /**
     * Notify
     */
    private void notifies() {
        for (IInvestor investor : this.investors) {
            investor.update(this);
        }
    }
}
```


