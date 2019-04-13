
# Design Pattern - Flyweight（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 17:14:36[Chimomo](https://me.csdn.net/chimomo)阅读数：162


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
# Definition
Use sharing to support large numbers of fine-grained objects efficiently.
# Participants
The classes and/or objects participating in this pattern are:
Flyweight(Character)Declares an interface through which flyweights can receive and act on extrinsic state
ConcreteFlyweight(CharacterA, CharacterB, ..., CharacterZ)Implements the Flyweight interface and adds storage for intrinsic state, if any. A ConcreteFlyweight object must be sharable. Any state it stores must be intrinsic, that is, it must be independent of the ConcreteFlyweight object's context.
UnsharedConcreteFlyweight(not used)Not all Flyweight subclasses need to be shared. The Flyweight interfaceenablessharing, but it doesn't enforce it. It is common for UnsharedConcreteFlyweight objects to have ConcreteFlyweight objects as children at some level in the flyweight object structure (as the Row and Column classes have).
FlyweightFactory(CharacterFactory)Creates and manages flyweight objects
Ensures that flyweight are shared properly. When a client requests a flyweight, the FlyweightFactory objects assets an existing instance or creates one, if none exists.
Client(FlyweightApp)Maintains a reference to flyweight(s)
Computes or stores the extrinsic state of flyweight(s)

# Sample code in Java
---
Thisstructuralcode demonstrates the Flyweight pattern in which a relatively small number of objects is shared many times by different clients.
```python
package chimomo.learning.java.designpattern.flyweight.sample;
/**
 * The 'ConcreteFlyweight' class
 *
 * @author Chimomo
 */
class ConcreteFlyweight extends Flyweight {
    /**
     * Operation
     *
     * @param extrinsicState The extrinsic state
     */
    @Override
    public void operation(int extrinsicState) {
        System.out.println("ConcreteFlyweight: " + extrinsicState);
    }
}
```
```python
package chimomo.learning.java.designpattern.flyweight.sample;
/**
 * The 'Flyweight' abstract class
 *
 * @author Chimomo
 */
abstract class Flyweight {
    /**
     * Operation
     *
     * @param extrinsicState The extrinsic state
     */
    public abstract void operation(int extrinsicState);
}
```
```python
package chimomo.learning.java.designpattern.flyweight.sample;
import java.util.Hashtable;
/**
 * The 'FlyweightFactory' class
 *
 * @author Chimomo
 */
class FlyweightFactory {
    // The flyweights
    private Hashtable<String, Object> flyweights = new Hashtable<>();
    /**
     * Initializes a new instance of the "FlyweightFactory" class
     */
    FlyweightFactory() {
        this.flyweights.put("X", new ConcreteFlyweight());
        this.flyweights.put("Y", new ConcreteFlyweight());
        this.flyweights.put("Z", new ConcreteFlyweight());
    }
    /**
     * Get flyweight
     *
     * @param key The key
     * @return The flyweight
     */
    Flyweight getFlyweight(String key) {
        return (Flyweight) this.flyweights.get(key);
    }
}
```
```python
package chimomo.learning.java.designpattern.flyweight.sample;
/**
 * Startup class for Structural Flyweight Design Pattern
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
        // Arbitrary extrinsic state
        int extrinsicState = 22;
        FlyweightFactory factory = new FlyweightFactory();
        // Work with different flyweight instances
        Flyweight fx = factory.getFlyweight("X");
        fx.operation(--extrinsicState);
        Flyweight fy = factory.getFlyweight("Y");
        fy.operation(--extrinsicState);
        Flyweight fz = factory.getFlyweight("Z");
        fz.operation(--extrinsicState);
        UnsharedConcreteFlyweight fu = new UnsharedConcreteFlyweight();
        fu.operation(--extrinsicState);
    }
}
/* ------ Running Results ------
ConcreteFlyweight: 21
ConcreteFlyweight: 20
ConcreteFlyweight: 19
UnsharedConcreteFlyweight: 18
*/
```
```python
package chimomo.learning.java.designpattern.flyweight.sample;
/**
 * The 'UnsharedConcreteFlyweight' class
 *
 * @author Chimomo
 */
class UnsharedConcreteFlyweight extends Flyweight {
    /**
     * Operation
     *
     * @param extrinsicState The extrinsic state
     */
    @Override
    public void operation(int extrinsicState) {
        System.out.println("UnsharedConcreteFlyweight: " + extrinsicState);
    }
}
```
---
This real-world code demonstrates the Flyweight pattern in which a relatively small number of Character objects is shared many times by a document that has potentially many characters.
```python
package chimomo.learning.java.designpattern.flyweight.realworld;
/**
 * The 'Flyweight' abstract class
 *
 * @author Chimomo
 */
abstract class Character {
    // The symbol
    char symbol;
    // The point size
    int pointSize;
    // The width
    int width;
    // The height
    int height;
    // The ascent
    int ascent;
    // The descent
    int descent;
    /**
     * Display
     *
     * @param pointSize The point size
     */
    public abstract void display(int pointSize);
}
```
```python
package chimomo.learning.java.designpattern.flyweight.realworld;
/**
 * The 'ConcreteFlyweight' class
 *
 * @author Chimomo
 */
class CharacterA extends Character {
    /**
     * Initializes a new instance of the "CharacterA" class
     */
    CharacterA() {
        this.symbol = 'A';
        this.width = 120;
        this.height = 100;
        this.ascent = 70;
        this.descent = 0;
    }
    /**
     * Display
     *
     * @param pointSize The point size
     */
    @Override
    public void display(int pointSize) {
        this.pointSize = pointSize;
        System.out.println(this.symbol + " (point size " + this.pointSize + ")");
    }
}
```
```python
package chimomo.learning.java.designpattern.flyweight.realworld;
/**
 * The 'ConcreteFlyweight' class
 *
 * @author Chimomo
 */
class CharacterB extends Character {
    /**
     * Initializes a new instance of the "CharacterB" class
     */
    CharacterB() {
        this.symbol = 'B';
        this.width = 140;
        this.height = 100;
        this.ascent = 72;
        this.descent = 0;
    }
    /**
     * Display
     *
     * @param pointSize The point size
     */
    @Override
    public void display(int pointSize) {
        this.pointSize = pointSize;
        System.out.println(this.symbol + " (point size " + this.pointSize + ")");
    }
}
```
```python
package chimomo.learning.java.designpattern.flyweight.realworld;
import java.util.HashMap;
import java.util.Map;
/**
 * The 'FlyweightFactory' class
 *
 * @author Chimomo
 */
class CharacterFactory {
    // The characters
    private Map<java.lang.Character, Character> characters = new HashMap<>();
    /**
     * Get character
     *
     * @param key The key
     * @return The character
     */
    Character getCharacter(char key) {
        // Uses "lazy initialization"
        Character character = null;
        if (this.characters.containsKey(key)) {
            character = this.characters.get(key);
        } else {
            switch (key) {
                case 'A':
                    character = new CharacterA();
                    break;
                case 'B':
                    character = new CharacterB();
                    break;
                // ...
                case 'Z':
                    character = new CharacterZ();
                    break;
            }
            this.characters.put(key, character);
        }
        return character;
    }
}
```
```python
package chimomo.learning.java.designpattern.flyweight.realworld;
/**
 * The 'ConcreteFlyweight' class
 *
 * @author Chimomo
 */
class CharacterZ extends Character {
    /**
     * Initializes a new instance of the "CharacterZ" class
     */
    CharacterZ() {
        this.symbol = 'Z';
        this.width = 100;
        this.height = 100;
        this.ascent = 68;
        this.descent = 0;
    }
    /**
     * Display
     *
     * @param pointSize The point size
     */
    @Override
    public void display(int pointSize) {
        this.pointSize = pointSize;
        System.out.println(this.symbol + " (point size " + this.pointSize + ")");
    }
}
```
```python
package chimomo.learning.java.designpattern.flyweight.realworld;
/**
 * Startup class for Real-World Flyweight Design Pattern
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
        // Build a document with text
        final String document = "AAZZBBZB";
        char[] chars = document.toCharArray();
        CharacterFactory factory = new CharacterFactory();
        // Extrinsic state
        int pointSize = 10;
        // For each character use a flyweight object
        for (char c : chars) {
            pointSize++;
            Character character = factory.getCharacter(c);
            character.display(pointSize);
        }
    }
}
/* ------ Running Results ------
A (point size 11)
A (point size 12)
Z (point size 13)
Z (point size 14)
B (point size 15)
B (point size 16)
Z (point size 17)
B (point size 18)
*/
```

