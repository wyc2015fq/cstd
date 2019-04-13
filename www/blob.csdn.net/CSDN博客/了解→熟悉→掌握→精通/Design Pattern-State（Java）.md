
# Design Pattern - State（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 17:23:03[Chimomo](https://me.csdn.net/chimomo)阅读数：114


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
# Definition
Allow an object to alter its behavior when its internal state changes. The object will appear to change its class.
# Participants
The classes and/or objects participating in this pattern are:
Context(Account)Defines the interface of interest to clients
Maintains an instance of a ConcreteState subclass that defines the current state.
State(State)Defines an interface for encapsulating the behavior associated with a particular state of the Context.
Concrete State(RedState, SilverState, GoldState)Each subclass implements a behavior associated with a state of Context

# Sample Code in Java
This structural code demonstrates the State pattern which allows an object to behave differently depending on its internal state. The difference in behavior is delegated to objects that represent this state.
```python
package chimomo.learning.java.designpattern.state.sample;
/**
 * A 'ConcreteState' class
 *
 * @author Chimomo
 */
class ConcreteStateA extends State {
    /**
     * Handle
     *
     * @param context The context
     */
    @Override
    public void handle(Context context) {
        context.setState(new ConcreteStateB());
    }
}
```
```python
package chimomo.learning.java.designpattern.state.sample;
/**
 * A 'ConcreteState' class
 *
 * @author Chimomo
 */
class ConcreteStateB extends State {
    /**
     * Handle
     *
     * @param context The context
     */
    @Override
    public void handle(Context context) {
        context.setState(new ConcreteStateA());
    }
}
```
```python
package chimomo.learning.java.designpattern.state.sample;
/**
 * The 'Context' class
 *
 * @author Chimomo
 */
class Context {
    // The state
    private State state;
    /**
     * Initializes a new instance of the "Account" class
     *
     * @param state The state
     */
    Context(State state) {
        this.setState(state);
    }
    /**
     * Get state
     *
     * @return The state
     */
    public State getState() {
        return this.state;
    }
    /**
     * Set state
     *
     * @param state The state
     */
    public void setState(State state) {
        this.state = state;
        System.out.println("State: " + this.state.getClass().getSimpleName());
    }
    /**
     * Request
     */
    public void request() {
        this.state.handle(this);
    }
}
```
```python
package chimomo.learning.java.designpattern.state.sample;
/**
 * Startup class for Structural State Design Pattern
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
        // Setup context in a state
        Context c = new Context(new ConcreteStateA());
        // Issue requests, which toggles state
        c.request();
        c.request();
        c.request();
        c.request();
    }
}
/* ------ Running Results ------
State: ConcreteStateA
State: ConcreteStateB
State: ConcreteStateA
State: ConcreteStateB
State: ConcreteStateA
*/
```
```python
package chimomo.learning.java.designpattern.state.sample;
/**
 * The 'State' abstract class
 *
 * @author Chimomo
 */
abstract class State {
    /**
     * Handle
     *
     * @param context The context
     */
    public abstract void handle(Context context);
}
```

---
This real-world code demonstrates the State pattern which allows an Account to behave differently depending on its balance. The difference in behavior is delegated to State objects called RedState, SilverState and GoldState. These states represent overdrawn accounts, starter accounts, and accounts in good standing.
```python
package chimomo.learning.java.designpattern.state.realworld;
/**
 * The 'Context' class
 *
 * @author Chimomo
 */
class Account {
    // The owner
    private String owner;
    // The state
    private State state;
    /**
     * Initializes a new instance of the "Account" class
     *
     * @param owner The owner
     */
    Account(String owner) {
        // New accounts are 'Silver' by default
        this.owner = owner;
        this.state = new SilverState(this, 0.0);
    }
    /**
     * Get balance
     *
     * @return The balance
     */
    public double getBalance() {
        return this.state.getBalance();
    }
    /**
     * Get state
     *
     * @return The state
     */
    public State getState() {
        return this.state;
    }
    /**
     * Set state
     *
     * @param state The state
     */
    public void setState(State state) {
        this.state = state;
    }
    /**
     * Deposit
     *
     * @param amount The amount
     */
    void deposit(double amount) {
        this.state.deposit(amount);
        System.out.println(String.format("%s deposited %f ------ ", this.owner, amount));
        System.out.println(String.format("  Balance = %f", this.getBalance()));
        System.out.println(String.format("  Status = %s", this.getState().getClass().getSimpleName()));
    }
    /**
     * Pay interest
     */
    void payInterest() {
        this.state.payInterest();
        System.out.println(String.format("Interest paid for %s ------ ", this.owner));
        System.out.println(String.format("  Balance = %f", this.getBalance()));
        System.out.println(String.format("  Status = %s", this.getState().getClass().getSimpleName()));
    }
    /**
     * Withdraw
     *
     * @param amount The amount
     */
    void withdraw(double amount) {
        this.state.withdraw(amount);
        System.out.println(String.format("%s withdraw %f ------ ", this.owner, amount));
        System.out.println(String.format("  Balance = %f", this.getBalance()));
        System.out.println(String.format("  Status = %s", this.getState().getClass().getSimpleName()));
    }
}
```
```python
package chimomo.learning.java.designpattern.state.realworld;
/**
 * A 'ConcreteState' class
 * Gold indicates an interest bearing state
 *
 * @author Chimomo
 */
class GoldState extends State {
    /**
     * Initializes a new instance of the "GoldState" class
     *
     * @param state The state
     */
    GoldState(State state) {
        this(state.getAccount(), state.getBalance());
    }
    /**
     * Initializes a new instance of the "GoldState" class
     *
     * @param account The account
     * @param balance The balance
     */
    private GoldState(Account account, double balance) {
        this.account = account;
        this.balance = balance;
        this.initialize();
    }
    /**
     * Initialize
     */
    private void initialize() {
        // Should come from a database
        this.interest = 0.05;
        this.lowerLimit = 1000.0;
        this.upperLimit = 10000000.0;
    }
    /**
     * Check state change
     */
    private void checkStateChange() {
        if (this.balance < 0.0) {
            this.account.setState(new RedState(this));
        } else if (this.balance < this.lowerLimit) {
            this.account.setState(new SilverState(this));
        }
    }
    /**
     * Deposit
     *
     * @param amount The amount
     */
    @Override
    public void deposit(double amount) {
        this.balance += amount;
        this.checkStateChange();
    }
    /**
     * Pay interest
     */
    @Override
    public void payInterest() {
        this.balance += this.interest * this.balance;
        this.checkStateChange();
    }
    /**
     * Withdraw
     *
     * @param amount The amount
     */
    @Override
    public void withdraw(double amount) {
        this.balance -= amount;
        this.checkStateChange();
    }
}
```
```python
package chimomo.learning.java.designpattern.state.realworld;
/**
 * Startup class for Real-World State Design Pattern
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
        // Open a new account
        Account account = new Account("Jim Johnson");
        // Apply financial transactions
        account.deposit(500.0);
        account.deposit(300.0);
        account.deposit(550.0);
        account.payInterest();
        account.withdraw(2000.00);
        account.withdraw(1100.00);
    }
}
/* ------ Running Results ------
Jim Johnson deposited 500.000000 ------
  Balance = 500.000000
  Status = SilverState
Jim Johnson deposited 300.000000 ------
  Balance = 800.000000
  Status = SilverState
Jim Johnson deposited 550.000000 ------
  Balance = 1350.000000
  Status = GoldState
Interest paid for Jim Johnson ------
  Balance = 1417.500000
  Status = GoldState
Jim Johnson withdraw 2000.000000 ------
  Balance = -582.500000
  Status = RedState
No funds available for withdrawal!
Jim Johnson withdraw 1100.000000 ------
  Balance = -582.500000
  Status = RedState
*/
```
```python
package chimomo.learning.java.designpattern.state.realworld;
/**
 * A 'ConcreteState' class
 * Red indicates that account is overdrawn
 *
 * @author Chimomo
 */
class RedState extends State {
    /**
     * Initializes a new instance of the "RedState" class
     *
     * @param state The state
     */
    RedState(State state) {
        this.account = state.getAccount();
        this.balance = state.getBalance();
        this.initialize();
    }
    /**
     * Initialize
     */
    private void initialize() {
        // Should come from a data source
        this.interest = 0.0;
        this.lowerLimit = -100.0;
        this.upperLimit = 0.0;
    }
    /**
     * Check state change
     */
    private void checkStateChange() {
        if (this.balance > this.upperLimit) {
            this.account.setState(new SilverState(this));
        }
    }
    /**
     * Deposit
     *
     * @param amount The amount
     */
    @Override
    public void deposit(double amount) {
        this.balance += amount;
        this.checkStateChange();
    }
    /**
     * Pay interest
     */
    @Override
    public void payInterest() {
        // No interest is paid
    }
    /**
     * Withdraw
     *
     * @param amount The amount
     */
    @Override
    public void withdraw(double amount) {
        System.out.println("No funds available for withdrawal!");
    }
}
```
```python
package chimomo.learning.java.designpattern.state.realworld;
/**
 * A 'ConcreteState' class
 * Silver indicates a non-interest bearing state
 *
 * @author Chimomo
 */
class SilverState extends State {
    /**
     * Initializes a new instance of the "SilverState" class
     *
     * @param state The state
     */
    SilverState(State state) {
        this(state.getAccount(), state.getBalance());
    }
    /**
     * Initializes a new instance of the "SilverState" class
     *
     * @param account The account
     * @param balance The balance
     */
    SilverState(Account account, double balance) {
        this.account = account;
        this.balance = balance;
        this.initialize();
    }
    /**
     * Initialize
     */
    private void initialize() {
        // Should come from a data source
        this.interest = 0.0;
        this.lowerLimit = 0.0;
        this.upperLimit = 1000.0;
    }
    /**
     * Check state change
     */
    private void checkStateChange() {
        if (this.balance < this.lowerLimit) {
            this.account.setState(new RedState(this));
        } else if (this.balance > this.upperLimit) {
            this.account.setState(new GoldState(this));
        }
    }
    /**
     * Deposit
     *
     * @param amount The amount
     */
    @Override
    public void deposit(double amount) {
        this.balance += amount;
        this.checkStateChange();
    }
    /**
     * Pay interest
     */
    @Override
    public void payInterest() {
        this.balance += this.interest * this.balance;
        this.checkStateChange();
    }
    /**
     * Withdraw
     *
     * @param amount The amount
     */
    public void withdraw(double amount) {
        this.balance -= amount;
        this.checkStateChange();
    }
}
```
```python
package chimomo.learning.java.designpattern.state.realworld;
/**
 * The 'State' abstract class
 *
 * @author Chimomo
 */
abstract class State {
    // The account
    protected Account account;
    // The balance
    protected double balance;
    // The interest
    double interest;
    // The lower limit
    double lowerLimit;
    // The upper limit
    double upperLimit;
    /**
     * Get account
     *
     * @return The account
     */
    public Account getAccount() {
        return this.account;
    }
    /**
     * Get balance
     *
     * @return The balance
     */
    public double getBalance() {
        return this.balance;
    }
    /**
     * Deposit
     *
     * @param amount The amount
     */
    public abstract void deposit(double amount);
    /**
     * Pay interest
     */
    public abstract void payInterest();
    /**
     * Withdraw
     *
     * @param amount The amount
     */
    public abstract void withdraw(double amount);
}
```


