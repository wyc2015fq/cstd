
# Design Pattern - Visitor（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 17:23:47[Chimomo](https://me.csdn.net/chimomo)阅读数：125


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
# Definition
Represent an operation to be performed on the elements of an object structure. Visitor lets you define a new operation without changing the classes of the elements on which it operates.
# Participants
The classes and/or objects participating in this pattern are:
Visitor(Visitor)Declares a Visit operation for each class of ConcreteElement in the object structure. The operation's name and signature identifies the class that sends the Visit request to the visitor. That lets the visitor determine the concrete class of the element being visited. Then the visitor can access the elements directly through its particular interface.
ConcreteVisitor(IncomeVisitor, VacationVisitor)Implements each operation declared by Visitor. Each operation implements a fragment of the algorithm defined for the corresponding class or object in the structure. ConcreteVisitor provides the context for the algorithm and stores its local state. This state often accumulates results during the traversal of the structure.
Element(Element)Defines an Accept operation that takes a visitor as an argument
ConcreteElement(Employee)Implements an Accept operation that takes a visitor as an argument
ObjectStructure(Employees)Can enumerate its elements
May provide a high-level interface to allow the visitor to visit its elements
May either be a Composite (pattern) or a collection such as a list or a set

# Sample Code in Java
---
This structural code demonstrates the Visitor pattern in which an object traverses an object structure and performs the same operation on each node in this structure. Different visitor objects define different operations.
```python
package chimomo.learning.java.designpattern.visitor.sample;
/**
 * A 'ConcreteElement' class
 *
 * @author Chimomo
 */
class ConcreteElementA extends Element {
    /**
     * Accept
     *
     * @param visitor The visitor
     */
    @Override
    public void accept(Visitor visitor) {
        visitor.visitConcreteElementA(this);
    }
}
```
```python
package chimomo.learning.java.designpattern.visitor.sample;
/**
 * A 'ConcreteElement' class
 *
 * @author Chimomo
 */
class ConcreteElementB extends Element {
    /**
     * Accept
     *
     * @param visitor The visitor
     */
    @Override
    public void accept(Visitor visitor) {
        visitor.visitConcreteElementB(this);
    }
}
```
```python
package chimomo.learning.java.designpattern.visitor.sample;
/**
 * A 'ConcreteVisitor' class
 *
 * @author Chimomo
 */
class ConcreteVisitor1 extends Visitor {
    /**
     * Visit ConcreteElementA
     *
     * @param concreteElementA The concrete element a
     */
    @Override
    public void visitConcreteElementA(ConcreteElementA concreteElementA) {
        System.out.println(String.format("%s visited by %s", concreteElementA.getClass().getSimpleName(), this.getClass().getSimpleName()));
    }
    /**
     * Visit ConcreteElementB
     *
     * @param concreteElementB The concrete element b
     */
    @Override
    public void visitConcreteElementB(ConcreteElementB concreteElementB) {
        System.out.println(String.format("%s visited by %s", concreteElementB.getClass().getSimpleName(), this.getClass().getSimpleName()));
    }
}
```
```python
package chimomo.learning.java.designpattern.visitor.sample;
/**
 * A 'ConcreteVisitor' class
 *
 * @author Chimomo
 */
class ConcreteVisitor2 extends Visitor {
    /**
     * Visit ConcreteElementA
     *
     * @param concreteElementA The concrete element a
     */
    @Override
    public void visitConcreteElementA(ConcreteElementA concreteElementA) {
        System.out.println(String.format("%s visited by %s", concreteElementA.getClass().getSimpleName(), this.getClass().getSimpleName()));
    }
    /**
     * Visit ConcreteElementB
     *
     * @param concreteElementB The concrete element b
     */
    @Override
    public void visitConcreteElementB(ConcreteElementB concreteElementB) {
        System.out.println(String.format("%s visited by %s", concreteElementB.getClass().getSimpleName(), this.getClass().getSimpleName()));
    }
}
```
```python
package chimomo.learning.java.designpattern.visitor.sample;
/**
 * The 'Element' abstract class
 *
 * @author Chimomo
 */
abstract class Element {
    /**
     * Accept
     *
     * @param visitor The visitor
     */
    public abstract void accept(Visitor visitor);
}
```
```python
package chimomo.learning.java.designpattern.visitor.sample;
import java.util.ArrayList;
import java.util.List;
/**
 * The 'ObjectStructure' class
 *
 * @author Chimomo
 */
class ObjectStructure extends Element {
    // The elements
    private final List<Element> elements = new ArrayList<>();
    /**
     * Attach
     *
     * @param element The element
     */
    public void attach(Element element) {
        this.elements.add(element);
    }
    /**
     * Accept
     *
     * @param visitor The visitor
     */
    public void accept(Visitor visitor) {
        for (Element element : this.elements) {
            element.accept(visitor);
        }
    }
}
```
```python
package chimomo.learning.java.designpattern.visitor.sample;
/**
 * Startup class for Structural Visitor Design Pattern
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
        // Setup structure
        ObjectStructure o = new ObjectStructure();
        o.attach(new ConcreteElementA());
        o.attach(new ConcreteElementB());
        // Create visitor objects
        ConcreteVisitor1 v1 = new ConcreteVisitor1();
        ConcreteVisitor2 v2 = new ConcreteVisitor2();
        // Structure accepting visitors
        o.accept(v1);
        o.accept(v2);
    }
}
/* ------ Running Results ------
ConcreteElementA visited by ConcreteVisitor1
ConcreteElementB visited by ConcreteVisitor1
ConcreteElementA visited by ConcreteVisitor2
ConcreteElementB visited by ConcreteVisitor2
*/
```
```python
package chimomo.learning.java.designpattern.visitor.sample;
/**
 * The 'Visitor' abstract class
 *
 * @author Chimomo
 */
abstract class Visitor {
    /**
     * Visit ConcreteElementA
     *
     * @param concreteElementA The concrete element a
     */
    public abstract void visitConcreteElementA(ConcreteElementA concreteElementA);
    /**
     * Visit ConcreteElementB
     *
     * @param concreteElementB The concrete element b
     */
    public abstract void visitConcreteElementB(ConcreteElementB concreteElementB);
}
```
---
This real-world code demonstrates the Visitor pattern in which two objects traverse a list of Employees and performs the same operation on each Employee.  The two visitor objects define different operations -- one adjusts vacation days and the other income.
```python
package chimomo.learning.java.designpattern.visitor.realworld;
/**
 * The 'Clerk' class
 *
 * @author Chimomo
 */
class Clerk extends Employee {
    /**
     * Initializes a new instance of the "Clerk" class
     */
    Clerk() {
        super("Hank", 25000.0, 14);
    }
}
```
```python
package chimomo.learning.java.designpattern.visitor.realworld;
/**
 * The 'Director' class
 *
 * @author Chimomo
 */
class Director extends Employee {
    /**
     * Initializes a new instance of the "Director" class
     */
    Director() {
        super("Elly", 35000.0, 16);
    }
}
```
```python
package chimomo.learning.java.designpattern.visitor.realworld;
/**
 * The 'Element' abstract class
 *
 * @author Chimomo
 */
abstract class Element {
    /**
     * Accept
     *
     * @param visitor The visitor
     */
    public abstract void accept(IVisitor visitor);
}
```
```python
package chimomo.learning.java.designpattern.visitor.realworld;
/**
 * The 'ConcreteElement' class
 *
 * @author Chimomo
 */
class Employee extends Element {
    // The name
    private String name;
    // The income
    private double income;
    // The vacation days
    private int vacationDays;
    /**
     * Initializes a new instance of the "Employee" class
     *
     * @param name The name
     * @param income The income
     * @param vacationDays The vacation days
     */
    Employee(String name, double income, int vacationDays) {
        this.name = name;
        this.income = income;
        this.vacationDays = vacationDays;
    }
    /**
     * Accept
     *
     * @param visitor The visitor
     */
    @Override
    public void accept(IVisitor visitor) {
        visitor.visit(this);
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
     * Get income
     *
     * @return The income
     */
    double getIncome() {
        return this.income;
    }
    /**
     * Set income
     *
     * @param income The income
     */
    void setIncome(double income) {
        this.income = income;
    }
    /**
     * Get vacation days
     *
     * @return The vacation days
     */
    int getVacationDays() {
        return this.vacationDays;
    }
    /**
     * Set vacation days
     *
     * @param vacationDays The vacation days
     */
    void setVacationDays(int vacationDays) {
        this.vacationDays = vacationDays;
    }
}
```
```python
package chimomo.learning.java.designpattern.visitor.realworld;
import java.util.ArrayList;
import java.util.List;
/**
 * The 'ObjectStructure' class
 *
 * @author Chimomo
 */
class Employees {
    // The employees
    private List<Employee> employees = new ArrayList<>();
    /**
     * Attach
     *
     * @param employee The employee
     */
    public void attach(Employee employee) {
        this.employees.add(employee);
    }
    /**
     * Accept
     *
     * @param visitor The visitor
     */
    public void accept(IVisitor visitor) {
        for (Employee e : this.employees) {
            e.accept(visitor);
        }
        System.out.println();
    }
}
```
```python
package chimomo.learning.java.designpattern.visitor.realworld;
/**
 * A 'ConcreteVisitor' class
 *
 * @author Chimomo
 */
class IncomeVisitor implements IVisitor {
    /**
     * Visit
     *
     * @param element The element
     */
    public void visit(Element element) {
        Employee employee = (Employee) element;
        // Provide 10% pay raise
        employee.setIncome(employee.getIncome() * 1.10);
        System.out.println(String.format("%s %s's new income: %.2f", employee.getClass().getSimpleName(), employee.getName(), employee.getIncome()));
    }
}
```
```python
package chimomo.learning.java.designpattern.visitor.realworld;
/**
 * The 'Visitor' interface
 *
 * @author Chimomo
 */
public interface IVisitor {
    /**
     * Visit
     *
     * @param element The element
     */
    void visit(Element element);
}
```
```python
package chimomo.learning.java.designpattern.visitor.realworld;
/**
 * The 'President' class
 *
 * @author Chimomo
 */
class President extends Employee {
    /**
     * Initializes a new instance of the "President" class
     */
    President() {
        super("Dick", 45000.0, 21);
    }
}
```
```python
package chimomo.learning.java.designpattern.visitor.realworld;
/**
 * Startup class for Real-World Visitor Design Pattern
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
        // Setup employee collection
        Employees e = new Employees();
        e.attach(new Clerk());
        e.attach(new Director());
        e.attach(new President());
        // Employees are 'visited'
        e.accept(new IncomeVisitor());
        e.accept(new VacationVisitor());
    }
}
/* ------ Running Results ------
Clerk Hank's new income: 27500.00
Director Elly's new income: 38500.00
President Dick's new income: 49500.00
Clerk Hank's new vacation days: 17
Director Elly's new vacation days: 19
President Dick's new vacation days: 24

*/
```
```python
package chimomo.learning.java.designpattern.visitor.realworld;
/**
 * A 'ConcreteVisitor' class
 *
 * @author Chimomo
 */
class VacationVisitor implements IVisitor {
    /**
     * Visit
     *
     * @param element The element
     */
    public void visit(Element element) {
        Employee employee = (Employee) element;
        // Provide 3 extra vacation days
        employee.setVacationDays(employee.getVacationDays() + 3);
        System.out.println(String.format("%s %s's new vacation days: %d", employee.getClass().getSimpleName(), employee.getName(), employee.getVacationDays()));
    }
}
```


