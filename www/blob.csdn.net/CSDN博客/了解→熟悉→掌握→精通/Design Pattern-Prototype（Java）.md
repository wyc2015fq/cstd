
# Design Pattern - Prototype（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 16:35:47[Chimomo](https://me.csdn.net/chimomo)阅读数：83


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
# Definition
Specify the kind of objects to create using a prototypical instance, and create new objects by copying this prototype.
# Participants
The classes and/or objects participating in this pattern are:
Prototype(ColorPrototype)Declares an interface for cloning itself
ConcretePrototype(Color)Implements an operation for cloning itself
Client(ColorManager)Creates a new object by asking a prototype to clone itself

# Sample Code in Java
---
This structural code demonstrates the Prototype pattern in which new objects are created by copying pre-existing objects (prototypes) of the same class.
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.prototype.sample;
/**
 * A 'ConcretePrototype' class.
 *
 * @author Chimomo
 */
class ConcretePrototype1 extends Prototype implements Cloneable {
    /**
     * Initializes a new instance of the "ConcretePrototype1" class.
     *
     * @param id
     */
    public ConcretePrototype1(String id) {
        super(id);
    }
    /**
     * Shallow clone.
     *
     * @return
     */
    @Override
    public Prototype shallowClone() {
        Prototype p = null;
        try {
            p = (Prototype) this.clone();
        } catch (CloneNotSupportedException e) {
            System.out.println(e.getMessage());
        }
        return p;
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.prototype.sample;
/**
 * A 'ConcretePrototype' class.
 *
 * @author Chimomo
 */
class ConcretePrototype2 extends Prototype implements Cloneable {
    /**
     * Initializes a new instance of the "ConcretePrototype2" class.
     *
     * @param id
     */
    public ConcretePrototype2(String id) {
        super(id);
    }
    /**
     * Shallow clone.
     *
     * @return
     */
    @Override
    public Prototype shallowClone() {
        Prototype p = null;
        try {
            p = (Prototype) this.clone();
        } catch (CloneNotSupportedException e) {
            System.out.println(e.getMessage());
        }
        return p;
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.prototype.sample;
/**
 * Startup class for Structural Prototype Design Pattern.
 *
 * @author Chimomo
 */
public class Program {
    /**
     * Entry point into console application.
     *
     * @param args
     * @throws java.lang.CloneNotSupportedException
     */
    public static void main(String[] args) throws CloneNotSupportedException {
        // Create two instances and clone each.
        Prototype p1 = new ConcretePrototype1("I");
        ConcretePrototype1 c1 = (ConcretePrototype1) p1.shallowClone();
        System.out.println("Cloned: " + c1.getId());
        Prototype p2 = new ConcretePrototype2("II");
        ConcretePrototype2 c2 = (ConcretePrototype2) p2.shallowClone();
        System.out.println("Cloned: " + c2.getId());
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.prototype.sample;
/**
 * The 'Prototype' abstract class.
 *
 * @author Chimomo
 */
abstract class Prototype {
    // The id.
    protected String id;
    /**
     * Initializes a new instance of the "Prototype" class.
     *
     * @param id
     */
    protected Prototype(String id) {
        this.id = id;
    }
    /**
     * Get id.
     */
    public String getId() {
        return this.id;
    }
    /**
     * Shallow clone.
     *
     * @return
     */
    public abstract Prototype shallowClone();
}
```
---
This real-world code demonstrates the Prototype pattern in which new Color objects are created by copying pre-existing, user-defined Colors of the same type.
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.prototype.realworld;
/**
 * The 'ConcretePrototype' class.
 *
 * @author Chimomo
 */
class Color extends ColorPrototype implements Cloneable {
    // The red.
    private final int red;
    // The green.
    private final int green;
    // The blue.
    private final int blue;
    /**
     * Initializes a new instance of the "Color" class.
     *
     * @param red
     * @param green
     * @param blue
     */
    public Color(int red, int green, int blue) {
        this.red = red;
        this.green = green;
        this.blue = blue;
    }
    @Override
    public ColorPrototype shallowClone() {
        System.out.println(String.format("Cloning color RGB: %-3d,%-3d,%-3d", this.red, this.green, this.blue));
        ColorPrototype c = null;
        try {
            c = (ColorPrototype) this.clone();
        } catch (CloneNotSupportedException e) {
            System.out.println(e.getMessage());
        }
        return c;
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.prototype.realworld;
import java.util.HashMap;
import java.util.Map;
/**
 * Prototype manager.
 *
 * @author Chimomo
 */
class ColorManager {
    // The colors.
    private final Map<String, ColorPrototype> colors = new HashMap<>();
    /**
     * Get color prototype.
     *
     * @param key
     * @return
     */
    public ColorPrototype getColorPrototype(String key) {
        return this.colors.get(key);
    }
    /**
     * Add entry to colors.
     *
     * @param key
     * @param value
     */
    public void addToColors(String key, ColorPrototype value) {
        this.colors.put(key, value);
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.prototype.realworld;
/**
 * The 'Prototype' abstract class.
 *
 * @author Chimomo
 */
abstract class ColorPrototype {
    /**
     * Shallow clone.
     *
     * @return
     */
    public abstract ColorPrototype shallowClone();
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.prototype.realworld;
/**
 * Startup class for Real-World Prototype Design Pattern.
 *
 * @author Chimomo
 */
public class Program {
    /**
     * Entry point into console application.
     *
     * @param args
     */
    public static void main(String[] args) {
        ColorManager colormanager = new ColorManager();
        // Initialize with standard colors.
        colormanager.addToColors("red", new Color(255, 0, 0));
        colormanager.addToColors("green", new Color(0, 255, 0));
        colormanager.addToColors("blue", new Color(0, 0, 255));
        // User adds personalized colors.
        colormanager.addToColors("angry", new Color(255, 54, 0));
        colormanager.addToColors("peace", new Color(128, 211, 128));
        colormanager.addToColors("flame", new Color(211, 34, 20));
        // User clones selected colors.
        Color color1 = (Color) colormanager.getColorPrototype("red").shallowClone();
        Color color2 = (Color) colormanager.getColorPrototype("peace").shallowClone();
        Color color3 = (Color) colormanager.getColorPrototype("flame").shallowClone();
    }
}
/*
Output:
Cloning color RGB: 255,0  ,0  
Cloning color RGB: 128,211,128
Cloning color RGB: 211,34 ,20 
 */
```

