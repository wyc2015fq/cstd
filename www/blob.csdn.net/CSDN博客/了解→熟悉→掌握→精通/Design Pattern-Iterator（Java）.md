
# Design Pattern - Iterator（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 17:19:09[Chimomo](https://me.csdn.net/chimomo)阅读数：285


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
### Definition
Provide a way to access the elements of an aggregate object sequentially without exposing its underlying representation.
### Participants
The classes and/or objects participating in this pattern are:
Iterator(AbstractIterator)Defines an interface for accessing and traversing elements
ConcreteIterator(Iterator)Implements the Iterator interface
Keeps track of the current position in the traversal of the aggregate
Aggregate(AbstractCollection)Defines an interface for creating an Iterator object
ConcreteAggregate(Collection)Implements the Iterator creation interface to return an instance of the proper ConcreteIterator

### Sample Code in Java
---
This structural code demonstrates the Iterator pattern which provides for a way to traverse (iterate) over a collection of items without detailing the underlying structure of the collection.
```python
package chimomo.learning.java.designpattern.iterator.sample;
/**
 * The 'Aggregate' abstract class
 *
 * @author Chimomo
 */
abstract class Aggregate {
    /**
     * Create iterator
     *
     * @return The iterator
     */
    public abstract Iterator createIterator();
}
```
```python
package chimomo.learning.java.designpattern.iterator.sample;
import java.util.ArrayList;
/**
 * The 'ConcreteAggregate' class
 *
 * @author Chimomo
 */
class ConcreteAggregate extends Aggregate {
    // The items
    private ArrayList<Object> items = new ArrayList<>();
    /**
     * Get item
     *
     * @param index The index
     * @return The item
     */
    Object getItem(int index) {
        return this.items.get(index);
    }
    /**
     * Set item
     *
     * @param index The index
     * @param value The value
     */
    void setItem(int index, Object value) {
        this.items.add(index, value);
    }
    /**
     * Get count
     *
     * @return The count
     */
    int getCount() {
        return this.items.size();
    }
    /**
     * Create iterator
     *
     * @return The iterator
     */
    @Override
    public Iterator createIterator() {
        return new ConcreteIterator(this);
    }
}
```
```python
package chimomo.learning.java.designpattern.iterator.sample;
/**
 * The 'Iterator' abstract class
 *
 * @author Chimomo
 */
class ConcreteIterator extends Iterator {
    // The aggregate
    private ConcreteAggregate aggregate;
    // The current
    private int current;
    /**
     * Initializes a new instance of the "ConcreteIterator" class
     *
     * @param aggregate The aggregate
     */
    ConcreteIterator(ConcreteAggregate aggregate) {
        this.aggregate = aggregate;
    }
    /**
     * Get the first item
     *
     * @return The first item
     */
    @Override
    public Object first() {
        return this.aggregate.getItem(0);
    }
    /**
     * Get current item
     *
     * @return The current item
     */
    @Override
    public Object currentItem() {
        return this.aggregate.getItem(this.current);
    }
    /**
     * Get the next item
     *
     * @return The next item
     */
    @Override
    public Object next() {
        Object o = null;
        if (this.current < this.aggregate.getCount() - 1) {
            o = this.aggregate.getItem(++this.current);
        }
        return o;
    }
    /**
     * Is done
     *
     * @return True if done; false otherwise.
     */
    @Override
    public boolean isDone() {
        return this.current >= this.aggregate.getCount() - 1;
    }
}
```
```python
package chimomo.learning.java.designpattern.iterator.sample;
/**
 * The 'Iterator' abstract class
 *
 * @author Chimomo
 */
abstract class Iterator {
    /**
     * Get the first item
     *
     * @return The first item
     */
    public abstract Object first();
    /**
     * Get the current item
     *
     * @return The current item
     */
    public abstract Object currentItem();
    /**
     * Get the next item
     *
     * @return The next item
     */
    public abstract Object next();
    /**
     * Is done
     *
     * @return True if done; false otherwise.
     */
    public abstract boolean isDone();
}
```
```python
package chimomo.learning.java.designpattern.iterator.sample;
/**
 * Startup class for Structural Iterator Design Pattern
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
        // Create aggregate
        ConcreteAggregate a = new ConcreteAggregate();
        a.setItem(0, "Item A");
        a.setItem(1, "Item B");
        a.setItem(2, "Item C");
        a.setItem(3, "Item D");
        // Create iterator and provide aggregate
        Iterator i = a.createIterator();
        System.out.println("Iterating over collection:");
        Object item = i.first();
        while (item != null) {
            System.out.println(String.format("%s (Is Done: %s)", i.currentItem(), i.isDone()));
            item = i.next();
        }
    }
}
/* ------ Running Results ------
Iterating over collection:
Item A (Is Done: false)
Item B (Is Done: false)
Item C (Is Done: false)
Item D (Is Done: true)
*/
```
---
This real-world code demonstrates the Iterator pattern which is used to iterate over a collection of items and skip a specific number of items each iteration.
```python
package chimomo.learning.java.designpattern.iterator.realworld;
import java.util.ArrayList;
/**
 * The 'ConcreteAggregate' class
 *
 * @author Chimomo
 */
class Collection implements IAbstractCollection {
    // The items
    private ArrayList<Object> items = new ArrayList<>();
    /**
     * Get item
     *
     * @param index The index
     * @return The item
     */
    Object getItem(int index) {
        return this.items.get(index);
    }
    /**
     * Set item
     *
     * @param index The index
     * @param value The value
     */
    void setItem(int index, Object value) {
        this.items.add(index, value);
    }
    /**
     * Get count
     *
     * @return The count
     */
    int getCount() {
        return this.items.size();
    }
    /**
     * Create iterator
     *
     * @return The iterator
     */
    @Override
    public Iterator createIterator() {
        return new Iterator(this);
    }
}
```
```python
package chimomo.learning.java.designpattern.iterator.realworld;
/**
 * The 'Aggregate' interface
 *
 * @author Chimomo
 */
interface IAbstractCollection {
    /**
     * Create iterator
     *
     * @return The iterator
     */
    Iterator createIterator();
}
```
```python
package chimomo.learning.java.designpattern.iterator.realworld;
/**
 * The 'Iterator' interface
 *
 * @author Chimomo
 */
interface IAbstractIterator {
    /**
     * Get the first item
     *
     * @return The first item
     */
    Item first();
    /**
     * Get the current item
     *
     * @return The current item
     */
    Item currentItem();
    /**
     * Get the next itema
     *
     * @return The next item
     */
    Item next();
    /**
     * Is done
     *
     * @return True if done; false otherwise.
     */
    boolean isDone();
}
```
```python
package chimomo.learning.java.designpattern.iterator.realworld;
/**
 * A collection item
 *
 * @author Chimomo
 */
class Item {
    // The name
    private String name;
    /**
     * Initializes a new instance of the "Item" class
     *
     * @param name The name
     */
    Item(String name) {
        this.name = name;
    }
    /**
     * Get name
     *
     * @return The name
     */
    public String getName() {
        return this.name;
    }
}
```
```python
package chimomo.learning.java.designpattern.iterator.realworld;
/**
 * The 'ConcreteIterator' class
 *
 * @author Chimomo
 */
class Iterator implements IAbstractIterator {
    // The collection
    private Collection collection;
    // The current
    private int current;
    // The step
    private int step = 1;
    /**
     * Initializes a new instance of the "Iterator" class
     *
     * @param collection The collection
     */
    Iterator(Collection collection) {
        this.collection = collection;
    }
    /**
     * Set step
     *
     * @param step The step
     */
    void setStep(int step) {
        this.step = step;
    }
    /**
     * The first item
     *
     * @return The first item
     */
    @Override
    public Item first() {
        this.current = 0;
        return (Item) this.collection.getItem(this.current);
    }
    /**
     * Get the current item
     *
     * @return The current item
     */
    @Override
    public Item currentItem() {
        return (Item) this.collection.getItem(this.current);
    }
    /**
     * Get the next item
     *
     * @return The next item
     */
    @Override
    public Item next() {
        this.current += this.step;
        if (!this.isDone()) {
            return (Item) this.collection.getItem(this.current);
        }
        return null;
    }
    /**
     * Is done
     *
     * @return True if done; false otherwise.
     */
    @Override
    public boolean isDone() {
        return this.current >= this.collection.getCount();
    }
}
```
```python
package chimomo.learning.java.designpattern.iterator.realworld;
/**
 * Startup class for Real-World Iterator Design Pattern
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
        // Build a collection
        Collection collection = new Collection();
        collection.setItem(0, new Item("Item 0"));
        collection.setItem(1, new Item("Item 1"));
        collection.setItem(2, new Item("Item 2"));
        collection.setItem(3, new Item("Item 3"));
        collection.setItem(4, new Item("Item 4"));
        collection.setItem(5, new Item("Item 5"));
        collection.setItem(6, new Item("Item 6"));
        collection.setItem(7, new Item("Item 7"));
        collection.setItem(8, new Item("Item 8"));
        // Create iterator
        Iterator iterator = collection.createIterator();
        iterator.setStep(2);
        // Skip every other item
        System.out.println("Iterating over collection:");
        for (iterator.first(); !iterator.isDone(); iterator.next()) {
            System.out.println(iterator.currentItem().getName());
        }
    }
}
/* ------ Running Results ------
Iterating over collection:
Item 0
Item 2
Item 4
Item 6
Item 8
*/
```


