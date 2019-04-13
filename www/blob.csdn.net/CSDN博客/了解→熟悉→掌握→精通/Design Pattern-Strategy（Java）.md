
# Design Pattern - Strategy（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 17:25:07[Chimomo](https://me.csdn.net/chimomo)阅读数：402


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
# Definition
Define a family of algorithms, encapsulate each one, and make them interchangeable. Strategy lets the algorithm vary independently from clients that use it.
# When To Use
The strategy design pattern is used to encapsulate changes and allows you to easily swap in and out alternative implementations, even at runtime. This makes it a great tool for benchmarking algorithms, responding to volatile requirements and quickly trying out new ideas.
# Participants
The classes and/or objects participating in this pattern are:
Strategy(SortStrategy)Declares an interface common to all supported algorithms. Context uses this interface to call the algorithm defined by a ConcreteStrategy
ConcreteStrategy(QuickSort, ShellSort, MergeSort)Implements the algorithm using the Strategy interface
Context(SortedList)Is configured with a ConcreteStrategy object
Maintains a reference to a Strategy object
May define an interface that lets Strategy access its data

# Sample Code in Java
This structural code demonstrates the Strategy pattern which encapsulates functionality in the form of an object. This allows clients to dynamically change algorithmic strategies.
```python
package chimomo.learning.java.designpattern.strategy.sample;
/**
 * A 'ConcreteStrategy' class
 *
 * @author Chimomo
 */
class ConcreteStrategyA extends Strategy {
    /**
     * Algorithm interface
     */
    @Override
    public void algorithmInterface() {
        System.out.println("Called ConcreteStrategyA.algorithmInterface()");
    }
}
```
```python
package chimomo.learning.java.designpattern.strategy.sample;
/**
 * A 'ConcreteStrategy' class
 *
 * @author Chimomo
 */
class ConcreteStrategyB extends Strategy {
    /**
     * Algorithm interface
     */
    @Override
    public void algorithmInterface() {
        System.out.println("Called ConcreteStrategyB.algorithmInterface()");
    }
}
```
```python
package chimomo.learning.java.designpattern.strategy.sample;
/**
 * A 'ConcreteStrategy' class
 *
 * @author Chimomo
 */
class ConcreteStrategyC extends Strategy {
    /**
     * Algorithm interface
     */
    @Override
    public void algorithmInterface() {
        System.out.println("Called ConcreteStrategyC.algorithmInterface()");
    }
}
```
```python
package chimomo.learning.java.designpattern.strategy.sample;
/**
 * The 'Context' class
 *
 * @author Chimomo
 */
class Context {
    // The strategy
    private Strategy strategy;
    /**
     * Initializes a new instance of the "Context" class
     *
     * @param strategy The strategy
     */
    Context(Strategy strategy) {
        this.strategy = strategy;
    }
    /**
     * Context interface
     */
    void contextInterface() {
        this.strategy.algorithmInterface();
    }
}
```
```python
package chimomo.learning.java.designpattern.strategy.sample;
/**
 * Startup class for Structural Strategy Design Pattern
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
        Context context;
        // Three contexts following different strategies
        context = new Context(new ConcreteStrategyA());
        context.contextInterface();
        context = new Context(new ConcreteStrategyB());
        context.contextInterface();
        context = new Context(new ConcreteStrategyC());
        context.contextInterface();
    }
}
/* ------ Running Results ------
Called ConcreteStrategyA.algorithmInterface()
Called ConcreteStrategyB.algorithmInterface()
Called ConcreteStrategyC.algorithmInterface()
*/
```
```python
package chimomo.learning.java.designpattern.strategy.sample;
/**
 * The 'Strategy' abstract class
 *
 * @author Chimomo
 */
abstract class Strategy {
    /**
     * Algorithm interface
     */
    public abstract void algorithmInterface();
}
```

---
This real-world code demonstrates the Strategy pattern which encapsulates sorting algorithms in the form of sorting objects. This allows clients to dynamically change sorting strategies including Quicksort, Shellsort, and Mergesort.
```python
package chimomo.learning.java.designpattern.strategy.realworld;
import java.util.List;
/**
 * A 'ConcreteStrategy' class
 *
 * @author Chimomo
 */
class MergeSort extends SortStrategy {
    /**
     * Sort
     */
    @Override
    public void sort(List<String> list) {
        // list.MergeSort(); not-implemented
        System.out.println("MergeSorted list:");
    }
}
```
```python
package chimomo.learning.java.designpattern.strategy.realworld;
/**
 * Startup class for Real-World Strategy Design Pattern
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
        // Context following different strategies
        SortedList studentRecords = new SortedList();
        studentRecords.add("Samual");
        studentRecords.add("Jimmy");
        studentRecords.add("Sandra");
        studentRecords.add("Vivek");
        studentRecords.add("Anna");
        studentRecords.setSortStrategy(new QuickSort());
        studentRecords.sort();
        studentRecords.setSortStrategy(new ShellSort());
        studentRecords.sort();
        studentRecords.setSortStrategy(new MergeSort());
        studentRecords.sort();
    }
}
/* ------ Running Results ------
QuickSorted list:
 Anna
 Jimmy
 Samual
 Sandra
 Vivek
ShellSorted list:
 Anna
 Jimmy
 Samual
 Sandra
 Vivek
MergeSorted list:
 Anna
 Jimmy
 Samual
 Sandra
 Vivek

*/
```
```python
package chimomo.learning.java.designpattern.strategy.realworld;
import java.util.Comparator;
import java.util.List;
/**
 * A 'ConcreteStrategy' class
 *
 * @author Chimomo
 */
class QuickSort extends SortStrategy {
    /**
     * Sort
     */
    @Override
    public void sort(List<String> list) {
        // Default is QuickSort
        list.sort(Comparator.naturalOrder());
        System.out.println("QuickSorted list:");
    }
}
```
```python
package chimomo.learning.java.designpattern.strategy.realworld;
import java.util.List;
/**
 * A 'ConcreteStrategy' class
 *
 * @author Chimomo
 */
class ShellSort extends SortStrategy {
    /**
     * Sort
     */
    @Override
    public void sort(List<String> list) {
        // list.ShellSort(); not-implemented
        System.out.println("ShellSorted list:");
    }
}
```
```python
package chimomo.learning.java.designpattern.strategy.realworld;
import java.util.ArrayList;
import java.util.List;
/**
 * The 'Context' class
 *
 * @author Chimomo
 */
class SortedList {
    // The list
    private List<String> list = new ArrayList<>();
    // The sort strategy
    private SortStrategy sortStrategy;
    /**
     * Set sort strategy
     *
     * @param sortStrategy The sort strategy
     */
    void setSortStrategy(SortStrategy sortStrategy) {
        this.sortStrategy = sortStrategy;
    }
    /**
     * Add
     *
     * @param name The name
     */
    public void add(String name) {
        this.list.add(name);
    }
    /**
     * Sort
     */
    public void sort() {
        this.sortStrategy.sort(this.list);
        // Iterate over list and display results
        for (String name : this.list) {
            System.out.println(" " + name);
        }
        System.out.println();
    }
}
```
```python
package chimomo.learning.java.designpattern.strategy.realworld;
import java.util.List;
/**
 * The 'Strategy' abstract class
 *
 * @author Chimomo
 */
abstract class SortStrategy {
    /**
     * Sort
     */
    public abstract void sort(List<String> list);
}
```


