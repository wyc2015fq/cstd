
# Design Pattern - Interpreter（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 17:17:51[Chimomo](https://me.csdn.net/chimomo)阅读数：219


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
# Definition
Given a language, define a representation for its grammar along with an interpreter that uses the representation to interpret sentences in the language.
# Participants
The classes and/or objects participating in this pattern are:
AbstractExpression(Expression)Declares an interface for executing an operation
TerminalExpression(ThousandExpression, HundredExpression, TenExpression, OneExpression)Implements an Interpret operation associated with terminal symbols in the grammar
An instance is required for every terminal symbol in the sentence
NonterminalExpression(not used)One such class is required for every rule R ::= R1R2...Rn in the grammar
Maintains instance variables of type AbstractExpression for each of the symbols R1 through Rn
Implements an Interpret operation for nonterminal symbols in the grammar. Interpret typically calls itself recursively on the variables representing R1 through Rn
Context(Context)Contains information that is global to the interpreter
Client(InterpreterApp)Builds (or is given) an abstract syntax tree representing a particular sentence in the language that the grammar defines. The abstract syntax tree is assembled from instances of the NonterminalExpression and TerminalExpression classes.
Invokes the Interpret operation

# Sample Code in Java
---
Thisstructuralcode demonstrates the Interpreter patterns, which using a defined grammar, provides the interpreter that processes parsed statements.
```python
package chimomo.learning.java.designpattern.interpreter.sample;
/**
 * The 'AbstractExpression' abstract class
 *
 * @author Chimomo
 */
abstract class AbstractExpression {
    /**
     * Interpret
     *
     * @param context The context
     */
    public abstract void interpret(Context context);
}
```
```python
package chimomo.learning.java.designpattern.interpreter.sample;
/**
 * The 'Context' class
 *
 * @author Chimomo
 */
class Context {
}
```
```python
package chimomo.learning.java.designpattern.interpreter.sample;
/**
 * The 'NonterminalExpression' class
 *
 * @author Chimomo
 */
class NonterminalExpression extends AbstractExpression {
    /**
     * Interpret
     *
     * @param context The context
     */
    @Override
    public void interpret(Context context) {
        System.out.println("Called Nonterminal.interpret()");
    }
}
```
```python
package chimomo.learning.java.designpattern.interpreter.sample;
import java.util.ArrayList;
import java.util.List;
/**
 * Startup class for Structural Interpreter Design Pattern
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
        Context context = new Context();
        // Usually a tree
        List<AbstractExpression> list = new ArrayList<>();
        list.add(new TerminalExpression());
        list.add(new NonterminalExpression());
        list.add(new TerminalExpression());
        list.add(new TerminalExpression());
        // Interpret
        for (AbstractExpression exp : list) {
            exp.interpret(context);
        }
    }
}
/* ------ Running Results ------
Called Terminal.interpret()
Called Nonterminal.interpret()
Called Terminal.interpret()
Called Terminal.interpret()
*/
```
```python
package chimomo.learning.java.designpattern.interpreter.sample;
/**
 * The 'TerminalExpression' class
 *
 * @author Chimomo
 */
class TerminalExpression extends AbstractExpression {
    /**
     * Interpret
     *
     * @param context The context
     */
    @Override
    public void interpret(Context context) {
        System.out.println("Called Terminal.interpret()");
    }
}
```
---
This real-world code demonstrates the Interpreter pattern which is used to convert a Roman numeral to a decimal.
```python
package chimomo.learning.java.designpattern.interpreter.realworld;
/**
 * The 'Context' class
 *
 * @author Chimomo
 */
class Context {
    // The input
    private String input;
    // The output
    private int output;
    /**
     * Initializes a new instance of the "Context" class
     *
     * @param input The input
     */
    Context(String input) {
        this.input = input;
    }
    /**
     * Get input
     *
     * @return The input
     */
    String getInput() {
        return this.input;
    }
    /**
     * Set input
     *
     * @param input The input
     */
    void setInput(String input) {
        this.input = input;
    }
    /**
     * Get output
     *
     * @return The output
     */
    public int getOutput() {
        return this.output;
    }
    /**
     * Set output
     *
     * @param output The output
     */
    public void setOutput(int output) {
        this.output = output;
    }
}
```
```python
package chimomo.learning.java.designpattern.interpreter.realworld;
/**
 * The 'AbstractExpression' abstract class
 *
 * @author Chimomo
 */
abstract class Expression {
    /**
     * The multiplier
     *
     * @return The multiplier
     */
    public abstract int multiplier();
    /**
     * The one
     *
     * @return The one
     */
    public abstract String one();
    /**
     * The four
     *
     * @return The four
     */
    public abstract String four();
    /**
     * The five
     *
     * @return The five
     */
    public abstract String five();
    /**
     * The nine
     *
     * @return The nine
     */
    public abstract String nine();
    /**
     * Interpret
     *
     * @param context The context
     */
    void interpret(Context context) {
        if (context.getInput().length() == 0) {
            return;
        }
        if (context.getInput().startsWith(this.nine())) {
            context.setOutput(context.getOutput() + 9 * this.multiplier());
            context.setInput(context.getInput().substring(2));
        } else if (context.getInput().startsWith(this.five())) {
            context.setOutput(context.getOutput() + 5 * this.multiplier());
            context.setInput(context.getInput().substring(1));
        } else if (context.getInput().startsWith(this.four())) {
            context.setOutput(context.getOutput() + 4 * this.multiplier());
            context.setInput(context.getInput().substring(2));
        }
        while (context.getInput().startsWith(this.one())) {
            context.setOutput(context.getOutput() + this.multiplier());
            context.setInput(context.getInput().substring(1));
        }
    }
}
```
```python
package chimomo.learning.java.designpattern.interpreter.realworld;
/**
 * The 'TerminalExpression' class
 * Hundred checks C, CD, D or CM.
 *
 * @author Chimomo
 */
class HundredExpression extends Expression {
    /**
     * The multiplier
     *
     * @return The multiplier
     */
    @Override
    public int multiplier() {
        return 100;
    }
    /**
     * The one
     *
     * @return The one
     */
    @Override
    public String one() {
        return "C";
    }
    /**
     * The four
     *
     * @return The four
     */
    @Override
    public String four() {
        return "CD";
    }
    /**
     * The five
     *
     * @return The five
     */
    @Override
    public String five() {
        return "D";
    }
    /**
     * The nine
     *
     * @return The nine
     */
    public String nine() {
        return "CM";
    }
}
```
```python
package chimomo.learning.java.designpattern.interpreter.realworld;
/**
 * The 'TerminalExpression' class
 * One checks for I, II, III, IV, V, VI, VI, VII, VIII, IX.
 *
 * @author Chimomo
 */
class OneExpression extends Expression {
    /**
     * The multiplier
     *
     * @return The multiplier
     */
    @Override
    public int multiplier() {
        return 1;
    }
    /**
     * The one
     *
     * @return The one
     */
    @Override
    public String one() {
        return "I";
    }
    /**
     * The four
     *
     * @return The four
     */
    @Override
    public String four() {
        return "IV";
    }
    /**
     * The five
     *
     * @return The five
     */
    @Override
    public String five() {
        return "V";
    }
    /**
     * The nine
     *
     * @return The nine
     */
    public String nine() {
        return "IX";
    }
}
```
```python
package chimomo.learning.java.designpattern.interpreter.realworld;
import java.util.ArrayList;
import java.util.List;
/**
 * Startup class for Real-World Interpreter Design Pattern
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
        String roman = "MCMXXVIII";
        Context context = new Context(roman);
        // Build the 'parse tree'
        List<Expression> tree = new ArrayList<>();
        tree.add(new ThousandExpression());
        tree.add(new HundredExpression());
        tree.add(new TenExpression());
        tree.add(new OneExpression());
        // Interpret
        for (Expression exp : tree) {
            exp.interpret(context);
        }
        System.out.println(String.format("%s = %s", roman, context.getOutput()));
    }
}
/* ------ Running Results ------
MCMXXVIII = 1928
*/
```
```python
package chimomo.learning.java.designpattern.interpreter.realworld;
/**
 * The 'TerminalExpression' class
 * Ten checks for X, XL, L and XC.
 *
 * @author Chimomo
 */
class TenExpression extends Expression {
    /**
     * The multiplier
     *
     * @return The multiplier
     */
    @Override
    public int multiplier() {
        return 10;
    }
    /**
     * The one
     *
     * @return The one
     */
    @Override
    public String one() {
        return "X";
    }
    /**
     * The four
     *
     * @return The four
     */
    @Override
    public String four() {
        return "XL";
    }
    /**
     * The five
     *
     * @return The five
     */
    @Override
    public String five() {
        return "L";
    }
    /**
     * The nine
     *
     * @return The nine
     */
    public String nine() {
        return "XC";
    }
}
```
```python
package chimomo.learning.java.designpattern.interpreter.realworld;
/**
 * The 'TerminalExpression' class
 * Thousand checks for the Roman Numeral M.
 *
 * @author Chimomo
 */
class ThousandExpression extends Expression {
    /**
     * The multiplier
     *
     * @return The multiplier
     */
    @Override
    public int multiplier() {
        return 1000;
    }
    /**
     * The one
     *
     * @return The one
     */
    @Override
    public String one() {
        return "M";
    }
    /**
     * The four
     *
     * @return The four
     */
    @Override
    public String four() {
        return " ";
    }
    /**
     * The five
     *
     * @return The five
     */
    @Override
    public String five() {
        return " ";
    }
    /**
     * The nine
     *
     * @return The nine
     */
    public String nine() {
        return " ";
    }
}
```

