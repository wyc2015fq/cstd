
# Design Pattern - Bridge（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 16:36:21[Chimomo](https://me.csdn.net/chimomo)阅读数：60


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
# Definition
Decouple an abstraction from its implementation so that the two can vary independently.
# Participants
The classes and/or objects participating in this pattern are:
Abstraction(BusinessObject)Defines the abstraction's interface.
Maintains a reference to an object of type Implementor.
RefinedAbstraction(CustomersBusinessObject)Extends the interface defined by Abstraction.
Implementor(DataObject)Defines the interface for implementation classes. This interface doesn't have to correspond exactly to Abstraction's interface; in fact the two interfaces can be quite different. Typically the Implementation interface provides only primitive operations, and Abstraction defines higher-level operations based on these primitives.
ConcreteImplementor(CustomersDataObject)Implements the Implementor interface and defines its concrete implementation.

# Sample Code in Java
---
Thisstructuralcode demonstrates the Bridge pattern which separates (decouples) the interface from its implementation. The implementation can evolve without changing clients which use the abstraction of the object.
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.bridge.sample;
/**
 * The 'Abstraction' class.
 *
 * @author Chimomo
 */
class Abstraction {
    // The implementor.
    protected Implementor implementor;
    /**
     * Set implementor.
     *
     * @param value
     */
    public void setImplementor(Implementor value) {
        this.implementor = value;
    }
    /**
     * Operation.
     */
    public void operation() {
        this.implementor.operation();
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.bridge.sample;
/**
 * The 'ConcreteImplementorA' class.
 *
 * @author Chimomo
 */
class ConcreteImplementorA extends Implementor {
    /**
     * Operation.
     */
    @Override
    public void operation() {
        System.out.println("ConcreteImplementorA operation");
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.bridge.sample;
/**
 * The 'ConcreteImplementorB' class.
 *
 * @author Chimomo
 */
class ConcreteImplementorB extends Implementor {
    /**
     * Operation.
     */
    @Override
    public void operation() {
        System.out.println("ConcreteImplementorB operation");
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.bridge.sample;
/**
 * The 'Implementor' abstract class.
 *
 * @author Chimomo
 */
abstract class Implementor {
    /**
     * Operation.
     */
    public abstract void operation();
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.bridge.sample;
/**
 * Startup class for Structural Bridge Design Pattern.
 *
 * @author Chimomo
 */
class Program {
    /**
     * Entry point into console application.
     *
     * @param args
     */
    public static void main(String[] args) {
        Abstraction ab = new RefinedAbstraction();
        // Set implementation and call.
        ab.implementor = new ConcreteImplementorA();
        ab.operation();
        // Change implementation and call.
        ab.implementor = new ConcreteImplementorB();
        ab.operation();
    }
}
/*
Output:
ConcreteImplementorA operation
ConcreteImplementorB operation
 */
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.bridge.sample;
/**
 * The 'RefinedAbstraction' class.
 *
 * @author Chimomo
 */
class RefinedAbstraction extends Abstraction {
    /**
     * Operation.
     */
    @Override
    public void operation() {
        this.implementor.operation();
    }
}
```
---
This real-world code demonstrates the Bridge pattern in which a Business Object abstraction is decoupled from the implementation in DataObject. The DataObject implementations can evolve dynamically without changing any clients.
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.bridge.realworld;
/**
 * The 'Abstraction' class.
 *
 * @author Chimomo
 */
class CustomerBase {
    // The group.
    protected String group;
    // The data object.
    private DataObject dataObject;
    /**
     * Initializes a new instance of the "CustomersBase" class.
     *
     * @param group
     */
    public CustomerBase(String group) {
        this.group = group;
    }
    /**
     * Get data object.
     *
     * @return
     */
    public DataObject getDataObject() {
        return this.dataObject;
    }
    /**
     * Set data object.
     *
     * @param dataObject
     */
    public void setDataObject(DataObject dataObject) {
        this.dataObject = dataObject;
    }
    /**
     * Add.
     *
     * @param customer
     */
    public void add(String customer) {
        this.dataObject.addRecord(customer);
    }
    /**
     * Delete.
     *
     * @param customer
     */
    public void delete(String customer) {
        this.dataObject.deleteRecord(customer);
    }
    /**
     * Next.
     */
    public void next() {
        this.dataObject.nextRecord();
    }
    /**
     * Prior.
     */
    public void prior() {
        this.dataObject.priorRecord();
    }
    /**
     * Show.
     */
    public void show() {
        this.dataObject.showRecord();
    }
    /**
     * Show all.
     */
    public void showAll() {
        System.out.println("Customer Group: " + this.group);
        this.dataObject.showAllRecords();
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.bridge.realworld;
/**
 * The 'RefinedAbstraction' class.
 *
 * @author Chimomo
 */
class Customers extends CustomerBase {
    /**
     * Initializes a new instance of the "Customers" class.
     *
     * @param group
     */
    public Customers(String group) {
        super(group);
    }
    /**
     * Show all.
     */
    @Override
    public void showAll() {
        // Add separator lines.
        System.out.println();
        System.out.println("----------------");
        super.showAll();
        System.out.println("----------------");
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.bridge.realworld;
import java.util.ArrayList;
import java.util.List;
/**
 * The 'ConcreteImplementor' class.
 *
 * @author Chimomo
 */
class CustomersData extends DataObject {
    // The current.
    private int current;
    // The customers.
    private final List<String> customers = new ArrayList<>();
    /**
     * Initializes a new instance of the "CustomersData" class.
     */
    public CustomersData() {
        // Loaded from a database.
        this.customers.add("Jim Jones");
        this.customers.add("Samual Jackson");
        this.customers.add("Allen Good");
        this.customers.add("Ann Stills");
        this.customers.add("Lisa Giolani");
    }
    /**
     * Add record.
     *
     * @param customer
     */
    @Override
    public void addRecord(String customer) {
        this.customers.add(customer);
    }
    /**
     * Delete record.
     *
     * @param customer
     */
    @Override
    public void deleteRecord(String customer) {
        this.customers.remove(customer);
    }
    /**
     * Next record.
     */
    @Override
    public void nextRecord() {
        if (this.current <= this.customers.size() - 1) {
            this.current++;
        }
    }
    /**
     * Prior record.
     */
    @Override
    public void priorRecord() {
        if (this.current > 0) {
            this.current--;
        }
    }
    /**
     * Show record.
     */
    @Override
    public void showRecord() {
        System.out.println(this.customers.get(this.current));
    }
    /**
     * Show all records.
     */
    @Override
    public void showAllRecords() {
        this.customers.forEach((customer) -> {
            System.out.println(" " + customer);
        });
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.bridge.realworld;
/**
 * The 'Implementor' abstract class.
 *
 * @author Chimomo
 */
abstract class DataObject {
    /**
     * Add record.
     *
     * @param name
     */
    public abstract void addRecord(String name);
    /**
     * Delete record.
     *
     * @param name
     */
    public abstract void deleteRecord(String name);
    /**
     * Next record.
     */
    public abstract void nextRecord();
    /**
     * Prior record.
     */
    public abstract void priorRecord();
    /**
     * Show record.
     */
    public abstract void showRecord();
    /**
     * Show all records.
     */
    public abstract void showAllRecords();
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.bridge.realworld;
/**
 * Startup class for Real-World Bridge Design Pattern.
 *
 * @author Chimomo
 */
class Program {
    /**
     * Entry point into console application.
     *
     * @param args
     */
    public static void main(String[] args) {
        // Create RefinedAbstraction.
        Customers customers = new Customers("Chicago");
        customers.setDataObject(new CustomersData());
        // Set ConcreteImplementor.
        // Exercise the bridge.
        customers.show();
        customers.next();
        customers.show();
        customers.next();
        customers.show();
        customers.add("Henry Velasquez");
        customers.showAll();
    }
}
/*
Output:
Jim Jones
Samual Jackson
Allen Good
----------------
Customer Group: Chicago
 Jim Jones
 Samual Jackson
 Allen Good
 Ann Stills
 Lisa Giolani
 Henry Velasquez
----------------
 */
```

