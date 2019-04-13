
# Design Pattern - Memento（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 17:21:28[Chimomo](https://me.csdn.net/chimomo)阅读数：72


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
### Definition
Without violating encapsulation, capture and externalize an object's internal state so that the object can be restored to this state later.
### Participants
The classes and/or objects participating in this pattern are:
Memento(Memento)Stores internal state of the Originator object. The memento may store as much or as little of the originator's internal state as necessary at its originator's discretion.
Protect against access by objects of other than the originator. Mementos have effectively two interfaces. Caretaker sees a narrow interface to the Memento -- it can only pass the memento to the other objects. Originator, in contrast, sees a wide interface, one that lets it access all the data necessary to restore itself to its previous state. Ideally, only the originator that produces the memento would be permitted to access the memento's internal state.
Originator(SalesProspect)Creates a memento containing a snapshot of its current internal state.
Uses the memento to restore its internal state
Caretaker(Caretaker)Is responsible for the memento's safekeeping
Never operates on or examines the contents of a memento.

### Sample Code in Java
---
This structural code demonstrates the Memento pattern which temporary saves and restores another object's internal state.
```python
package chimomo.learning.java.designpattern.memento.sample;
/**
 * The 'Caretaker' class
 *
 * @author Chimomo
 */
class Caretaker {
    // The memento
    private Memento memento;
    /**
     * Get memento
     *
     * @return The memento
     */
    Memento getMemento() {
        return this.memento;
    }
    /**
     * Set memento
     *
     * @param memento The memento
     */
    void setMemento(Memento memento) {
        this.memento = memento;
    }
}
```
```python
package chimomo.learning.java.designpattern.memento.sample;
/**
 * The 'Memento' class
 *
 * @author Chimomo
 */
class Memento {
    // The state
    private String state;
    /**
     * Initializes a new instance of the "Memento" class
     *
     * @param state The state
     */
    Memento(String state) {
        this.state = state;
    }
    /**
     * Get state
     *
     * @return The state
     */
    public String getState() {
        return this.state;
    }
}
```
```python
package chimomo.learning.java.designpattern.memento.sample;
/**
 * The 'Originator' class
 *
 * @author Chimomo
 */
class Originator {
    // The state
    private String state;
    /**
     * Get state
     *
     * @return The state
     */
    public String getState() {
        return this.state;
    }
    /**
     * Set state
     *
     * @param state The state
     */
    public void setState(String state) {
        this.state = state;
        System.out.println("State = " + this.state);
    }
    /**
     * Create memento
     *
     * @return The memento
     */
    Memento createMemento() {
        return new Memento(this.state);
    }
    /**
     * Set memento
     *
     * @param memento The memento
     */
    void setMemento(Memento memento) {
        System.out.println("Restoring state...");
        this.state = memento.getState();
        System.out.println("State = " + this.state);
    }
}
```
```python
package chimomo.learning.java.designpattern.memento.sample;
/**
 * Startup class for Structural Memento Design Pattern
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
        Originator o = new Originator();
        o.setState("On");
        // Store internal state
        Caretaker c = new Caretaker();
        c.setMemento(o.createMemento());
        // Continue changing originator
        o.setState("Off");
        // Restore saved state
        o.setMemento(c.getMemento());
    }
}
/* ------ Running Results ------
State = On
State = Off
Restoring state...
State = On
*/
```
---
This real-world code demonstrates the Memento pattern which temporarily saves and then restores the Sales Prospect's internal state.
```python
package chimomo.learning.java.designpattern.memento.realworld;
/**
 * The 'Memento' class
 *
 * @author Chimomo
 */
class Memento {
    // The name
    private String name;
    // The phone
    private String phone;
    // The budget
    private double budget;
    /**
     * Initializes a new instance of the "Memento" class
     *
     * @param name The name
     * @param phone The phone
     * @param budget The budget
     */
    Memento(String name, String phone, double budget) {
        this.name = name;
        this.phone = phone;
        this.budget = budget;
    }
    /**
     * Get name
     *
     * @return The name
     */
    public String getName() {
        return this.name;
    }
    /**
     * Set name
     *
     * @param name The name
     */
    public void setName(String name) {
        this.name = name;
    }
    /**
     * Get phone
     *
     * @return The phone
     */
    String getPhone() {
        return this.phone;
    }
    /**
     * Get budget
     *
     * @return The budget
     */
    double getBudget() {
        return this.budget;
    }
}
```
```python
package chimomo.learning.java.designpattern.memento.realworld;
/**
 * Startup class for Real-World Memento Design Pattern
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
        SalesProspect s = new SalesProspect();
        s.setName("Noel van Halen");
        s.setPhone("(412) 256-0990");
        s.setBudget(25000.0);
        // Store internal state
        ProspectMemory m = new ProspectMemory();
        m.setMemento(s.saveMemento());
        // Continue changing originator
        s.setName("Leo Welch");
        s.setPhone("(310) 209-7111");
        s.setBudget(1000000.0);
        // Restore saved state
        s.restoreMemento(m.getMemento());
    }
}
/* ------ Running Results ------
Name: Noel van Halen
Phone: (412) 256-0990
Budget: 25000.0
Saving state ------
Name: Leo Welch
Phone: (310) 209-7111
Budget: 1000000.0
Restoring state ------
Name = Noel van Halen
Phone = (412) 256-0990
Budget = 25000.0
*/
```
```python
package chimomo.learning.java.designpattern.memento.realworld;
/**
 * The 'Caretaker' class
 *
 * @author Chimomo
 */
class ProspectMemory {
    // The memento
    private Memento memento;
    /**
     * Get memento
     *
     * @return The memento
     */
    Memento getMemento() {
        return this.memento;
    }
    /**
     * Set memento
     *
     * @param memento The memento
     */
    void setMemento(Memento memento) {
        this.memento = memento;
    }
}
```
```python
package chimomo.learning.java.designpattern.memento.realworld;
/**
 * The 'Originator' class
 *
 * @author Chimomo
 */
class SalesProspect {
    // The name
    private String name;
    // The phone
    private String phone;
    // The budget
    private double budget;
    /**
     * Get name
     *
     * @return The name
     */
    public String getName() {
        return this.name;
    }
    /**
     * Set name
     *
     * @param name The name
     */
    public void setName(String name) {
        this.name = name;
        System.out.println("Name: " + this.name);
    }
    /**
     * Set phone
     *
     * @param phone The phone
     */
    void setPhone(String phone) {
        this.phone = phone;
        System.out.println("Phone: " + this.phone);
    }
    /**
     * Set budget
     *
     * @param budget The budget
     */
    void setBudget(double budget) {
        this.budget = budget;
        System.out.println("Budget: " + this.budget);
    }
    /**
     * Save memento
     *
     * @return The memento
     */
    Memento saveMemento() {
        System.out.println("Saving state ------");
        return new Memento(this.name, this.phone, this.budget);
    }
    /**
     * Restore memento
     *
     * @param memento The memento
     */
    void restoreMemento(Memento memento) {
        System.out.println("Restoring state ------");
        this.name = memento.getName();
        this.phone = memento.getPhone();
        this.budget = memento.getBudget();
        System.out.println("Name = " + this.name);
        System.out.println("Phone = " + this.phone);
        System.out.println("Budget = " + this.budget);
    }
}
```


