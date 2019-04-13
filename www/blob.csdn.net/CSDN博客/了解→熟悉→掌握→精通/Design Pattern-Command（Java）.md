
# Design Pattern - Command（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 17:17:06[Chimomo](https://me.csdn.net/chimomo)阅读数：375


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
# Definition
Encapsulate a request as an object, thereby letting you parameterize clients with different requests, queue or log requests, and support undoable operations.
# Participants
The classes and/or objects participating in this pattern are:
Command(Command)Declares an interface for executing an operation
ConcreteCommand(CalculatorCommand)Defines a binding between a Receiver object and an action
Implements Execute by invoking the corresponding operation(s) on Receiver
Client(CommandApp)Creates a ConcreteCommand object and sets its receiver
Invoker(User)Asks the command to carry out the request
Receiver(Calculator)Knows how to perform the operations associated with carrying out the request.

# Sample Code in Java
---
Thisstructuralcode demonstrates the Command pattern which stores requests as objects allowing clients to execute or playback the requests.
```python
package chimomo.learning.java.designpattern.command.sample;
/**
 * The 'Command' abstract class
 *
 * @author Chimomo
 */
abstract class Command {
    // The receiver
    final Receiver receiver;
    /**
     * Initializes a new instance of the "Command" class
     *
     * @param receiver The receiver
     */
    Command(Receiver receiver) {
        this.receiver = receiver;
    }
    /**
     * Execute
     */
    public abstract void execute();
}
```
```python
package chimomo.learning.java.designpattern.command.sample;
/**
 * The 'ConcreteCommand' class
 *
 * @author Chimomo
 */
class ConcreteCommand extends Command {
    /**
     * Initializes a new instance of the "ConcreteCommand" class
     *
     * @param receiver The receiver
     */
    ConcreteCommand(Receiver receiver) {
        super(receiver);
    }
    /**
     * Execute
     */
    @Override
    public void execute() {
        this.receiver.action();
    }
}
```
```python
package chimomo.learning.java.designpattern.command.sample;
/**
 * The 'Invoker' class
 *
 * @author Chimomo
 */
class Invoker {
    // The command
    private Command command;
    /**
     * Execute command
     */
    void executeCommand() {
        this.command.execute();
    }
    /**
     * Set command
     *
     * @param command The command
     */
    public void setCommand(Command command) {
        this.command = command;
    }
}
```
```python
package chimomo.learning.java.designpattern.command.sample;
/**
 * Startup class for Structural Command Design Pattern
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
        // Create receiver, command, and invoker.
        Receiver receiver = new Receiver();
        Command command = new ConcreteCommand(receiver);
        Invoker invoker = new Invoker();
        // Set and execute command
        invoker.setCommand(command);
        invoker.executeCommand();
    }
}
/* ------ Running Results ------
Called Receiver.action()
*/
```
```python
package chimomo.learning.java.designpattern.command.sample;
/**
 * The 'Receiver' class
 *
 * @author Chimomo
 */
class Receiver {
    /**
     * The action
     */
    void action() {
        System.out.println("Called Receiver.action()");
    }
}
```
---
This real-world code demonstrates the Command pattern used in a simple calculator with unlimited number of undo's and redo's.
```python
package chimomo.learning.java.designpattern.command.realworld;
/**
 * The 'Receiver' class
 *
 * @author Chimomo
 */
class Calculator {
    // The current value
    private int currentValue;
    /**
     * The operation
     *
     * @param operand The operand
     */
    public void operation(char operator, int operand) {
        switch (operator) {
            case '+':
                this.currentValue += operand;
                break;
            case '-':
                this.currentValue -= operand;
                break;
            case '*':
                this.currentValue *= operand;
                break;
            case '/':
                this.currentValue /= operand;
                break;
        }
        System.out.println(String.format("Current value = %3d (following %s %d)", this.currentValue, operator, operand));
    }
}
```
```python
package chimomo.learning.java.designpattern.command.realworld;
/**
 * The 'ConcreteCommand' class
 *
 * @author Chimomo
 */
class CalculatorCommand extends Command {
    // The calculator
    private final Calculator calculator;
    // The operator
    private char operator;
    // The operand
    private int operand;
    /**
     * Initializes a new instance of the "CalculatorCommand" class
     *
     * @param calculator The calculator
     * @param operator The operator
     * @param operand The operand
     */
    CalculatorCommand(Calculator calculator, char operator, int operand) {
        this.calculator = calculator;
        this.operator = operator;
        this.operand = operand;
    }
    /**
     * Execute
     */
    @Override
    public void execute() {
        this.calculator.operation(this.operator, this.operand);
    }
    /**
     * Unexecute
     */
    @Override
    public void unExecute() {
        this.calculator.operation(this.undo(this.operator), this.operand);
    }
    /**
     * undo
     *
     * @return The undo operator
     */
    private char undo(char operator) {
        switch (operator) {
            case '+':
                return '-';
            case '-':
                return '+';
            case '*':
                return '/';
            case '/':
                return '*';
            default:
                throw new IllegalArgumentException("operator: " + operator);
        }
    }
}
```
```python
package chimomo.learning.java.designpattern.command.realworld;
/**
 * The 'Command' abstract class
 *
 * @author Chimomo
 */
abstract class Command {
    /**
     * Execute
     */
    public abstract void execute();
    /**
     * Unexecute
     */
    public abstract void unExecute();
}
```
```python
package chimomo.learning.java.designpattern.command.realworld;
/**
 * Startup class for Real-World Command Design Pattern
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
        // Create user and let her compute
        User user = new User();
        // User presses calculator buttons
        user.compute('+', 100);
        user.compute('-', 50);
        user.compute('*', 10);
        user.compute('/', 2);
        // undo 4 commands
        user.undo(4);
        // redo 3 commands
        user.redo(3);
    }
}
/* ------ Running Results ------
Current value = 100 (following + 100)
Current value =  50 (following - 50)
Current value = 500 (following * 10)
Current value = 250 (following / 2)
---- undo 4 levels
Current value = 500 (following * 2)
Current value =  50 (following / 10)
Current value = 100 (following + 50)
Current value =   0 (following - 100)
---- redo 3 levels
Current value = 100 (following + 100)
Current value =  50 (following - 50)
Current value = 500 (following * 10)
*/
```
```python
package chimomo.learning.java.designpattern.command.realworld;
import java.util.ArrayList;
import java.util.List;
/**
 * The 'Invoker' class
 *
 * @author Chimomo
 */
class User {
    // The calculator
    private final Calculator calculator = new Calculator();
    // The commands
    private final List<Command> commands = new ArrayList<>();
    // The current command
    private int currentCommand;
    void compute(char operator, int operand) {
        // Create command operation and execute it
        Command command = new CalculatorCommand(this.calculator, operator, operand);
        command.execute();
        // Add command to undo list
        this.commands.add(command);
        this.currentCommand++;
    }
    /**
     * Redo
     *
     * @param levels The levels to redo
     */
    void redo(int levels) {
        System.out.println(String.format("---- redo %d levels ", levels));
        // Perform redo operations
        for (int i = 0; i < levels; i++) {
            if (this.currentCommand < this.commands.size() - 1) {
                Command command = this.commands.get(this.currentCommand++);
                command.execute();
            }
        }
    }
    /**
     * Undo
     *
     * @param levels The levels to undo
     */
    void undo(int levels) {
        System.out.println(String.format("---- undo %d levels ", levels));
        // Perform undo operations
        for (int i = 0; i < levels; i++) {
            if (this.currentCommand > 0) {
                Command command = this.commands.get(--this.currentCommand);
                command.unExecute();
            }
        }
    }
}
```

