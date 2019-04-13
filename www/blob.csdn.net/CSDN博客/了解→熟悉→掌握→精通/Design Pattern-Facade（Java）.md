
# Design Pattern - Facade（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 17:14:03[Chimomo](https://me.csdn.net/chimomo)阅读数：181


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
# Definition
Provide a unified interface to a set of interfaces in a subsystem. Facade defines a higher-level interface that makes the subsystem easier to use.
# Participants
The classes and/or objects participating in this pattern are:
Facade(MortgageApplication)Knows which subsystem classes are responsible for a request
Delegates client requests to appropriate subsystem objects
Subsystem classes(Bank, Credit, Loan)Implement subsystem functionality
Handle work assigned by the Facade object
Have no knowledge of the facade and keep no reference to it

# Sample Code in Java
---
Thisstructuralcode demonstrates the Facade pattern which provides a simplified and uniform interface to a large subsystem of classes.
```python
package chimomo.learning.java.designpattern.facade.sample;
/**
 * The 'Facade' class
 *
 * @author Chimomo
 */
class Facade {
    // The sub system one
    private SubSystemOne one;
    // The sub system two
    private SubSystemTwo two;
    // The sub system three
    private SubSystemThree three;
    // The sub system four
    private SubSystemFour four;
    /**
     * Initializes a new instance of the "Facade" class
     */
    Facade() {
        this.one = new SubSystemOne();
        this.two = new SubSystemTwo();
        this.three = new SubSystemThree();
        this.four = new SubSystemFour();
    }
    /**
     * Method a
     */
    void methodA() {
        System.out.println("MethodA() ------ ");
        this.one.methodOne();
        this.two.methodTwo();
        this.four.methodFour();
    }
    /**
     * Method b
     */
    void methodB() {
        System.out.println("MethodB() ------ ");
        this.two.methodTwo();
        this.three.methodThree();
    }
}
```
```python
package chimomo.learning.java.designpattern.facade.sample;
/**
 * Startup class for Structural Facade Design Pattern
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
        Facade facade = new Facade();
        facade.methodA();
        facade.methodB();
    }
}
/* ------ Running Results ------
MethodA() ------
 SubSystemOne Method
 SubSystemTwo Method
 SubSystemFour Method
MethodB() ------
 SubSystemTwo Method
 SubSystemThree Method
*/
```
```python
package chimomo.learning.java.designpattern.facade.sample;
/**
 * The 'Subsystem ClassD' class
 *
 * @author Chimomo
 */
class SubSystemFour {
    /**
     * Method four
     */
    void methodFour() {
        System.out.println(" SubSystemFour Method");
    }
}
```
```python
package chimomo.learning.java.designpattern.facade.sample;
/**
 * The 'Subsystem ClassA' class
 *
 * @author Chimomo
 */
class SubSystemOne {
    /**
     * Method one.
     */
    void methodOne() {
        System.out.println(" SubSystemOne Method");
    }
}
```
```python
package chimomo.learning.java.designpattern.facade.sample;
/**
 * The 'Subsystem ClassC' class
 *
 * @author Chimomo
 */
class SubSystemThree {
    /**
     * Method three
     */
    void methodThree() {
        System.out.println(" SubSystemThree Method");
    }
}
```
```python
package chimomo.learning.java.designpattern.facade.sample;
/**
 * The 'Subsystem ClassB' class
 *
 * @author Chimomo
 */
class SubSystemTwo {
    /**
     * Method two
     */
    void methodTwo() {
        System.out.println(" SubSystemTwo Method");
    }
}
```
---
This real-world code demonstrates the Facade pattern as a Mortgage Application object which provides a simplified interface to a large subsystem of classes measuring the credit worthiness of an applicant.
```python
package chimomo.learning.java.designpattern.facade.realworld;
/**
 * The 'Subsystem ClassA' class
 *
 * @author Chimomo
 */
class Bank {
    /**
     * Whether has sufficient savings
     *
     * @param c The customer
     * @return True if customer has sufficient savings; false otherwise.
     */
    boolean hasSufficientSavings(Customer c) {
        System.out.println(String.format("Check bank for %s", c.getName()));
        return true;
    }
}
```
```python
package chimomo.learning.java.designpattern.facade.realworld;
/**
 * The 'Subsystem ClassB' class
 *
 * @author Chimomo
 */
class Credit {
    /**
     * Whether has good credit
     *
     * @param c The customer
     * @return True if customer has good credit; false otherwise.
     */
    boolean hasGoodCredit(Customer c) {
        System.out.println("Check credit for " + c.getName());
        return true;
    }
}
```
```python
package chimomo.learning.java.designpattern.facade.realworld;
/**
 * The Customer class
 *
 * @author Chimomo
 */
class Customer {
    // The name
    private String name;
    /**
     * Initializes a new instance of the "Customer" class
     *
     * @param name The name
     */
    Customer(String name) {
        this.name = name;
    }
    /**
     * Get name
     */
    public String getName() {
        return this.name;
    }
}
```
```python
package chimomo.learning.java.designpattern.facade.realworld;
/**
 * The 'Subsystem ClassC' class
 *
 * @author Chimomo
 */
class Loan {
    /**
     * Whether has no bad loans
     *
     * @param c The customer
     * @return True if customer has no bad loans; false otherwise.
     */
    boolean hasNoBadLoans(Customer c) {
        System.out.println("Check loans for " + c.getName());
        return true;
    }
}
```
```python
package chimomo.learning.java.designpattern.facade.realworld;
/**
 * The 'Facade' class
 *
 * @author Chimomo
 */
class Mortgage {
    // The bank
    private Bank bank = new Bank();
    // The credit
    private Credit credit = new Credit();
    // The loan
    private Loan loan = new Loan();
    /**
     * Whether customer is eligible
     *
     * @param c The customer
     * @param amount The amount
     * @return True if customer is eligible; false otherwise.
     */
    boolean isEligible(Customer c, int amount) {
        System.out.println(String.format("%s applies for %d loan", c.getName(), amount));
        boolean eligible = true;
        // Check credit worthiness of applicant
        if (!this.bank.hasSufficientSavings(c)) {
            eligible = false;
        } else if (!this.loan.hasNoBadLoans(c)) {
            eligible = false;
        } else if (!this.credit.hasGoodCredit(c)) {
            eligible = false;
        }
        return eligible;
    }
}
```
```python
package chimomo.learning.java.designpattern.facade.realworld;
/**
 * Startup class for Real-World Facade Design Pattern
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
        // Create facade
        Mortgage mortgage = new Mortgage();
        // Evaluate mortgage eligibility for customer
        Customer customer = new Customer("Ann McKinsey");
        boolean eligible = mortgage.isEligible(customer, 125000);
        System.out.println(customer.getName() + " has been " + (eligible ? "Approved" : "Rejected"));
    }
}
/* ------ Running Results ------
Ann McKinsey applies for 125000 loan
Check bank for Ann McKinsey
Check loans for Ann McKinsey
Check credit for Ann McKinsey
Ann McKinsey has been Approved
*/
```

