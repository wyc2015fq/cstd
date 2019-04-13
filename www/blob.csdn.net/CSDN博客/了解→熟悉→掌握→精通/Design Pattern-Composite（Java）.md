
# Design Pattern - Composite（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 16:37:00[Chimomo](https://me.csdn.net/chimomo)阅读数：67


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
# Definition
Compose objects into tree structures to represent part-whole hierarchies. Composite lets clients treat individual objects and compositions of objects uniformly.
# Participants
The classes and/or objects participating in this pattern are:
Component(DrawingElement)Declares the interface for objects in the composition.
Implements default behavior for the interface common to all classes, as appropriate.
Declares an interface for accessing and managing its child components.
(optional) Defines an interface for accessing a component's parent in the recursive structure, and implements it if that's appropriate.
Leaf(PrimitiveElement)Represents leaf objects in the composition. A leaf has no children.
Defines behavior for primitive objects in the composition.
Composite(CompositeElement)Defines behavior for components having children.
Stores child components.
Implements child-related operations in the Component interface.
Client(CompositeApp)Manipulates objects in the composition through the Component interface.

# Sample Code in Java
---
Thisstructuralcode demonstrates the Composite pattern which allows the creation of a tree structure in which individual nodes are accessed uniformly whether they are leaf nodes or branch (composite) nodes.
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.composite.sample;
/**
 * The 'Component' abstract class.
 *
 * @author Chimomo
 */
abstract class Component {
    // The name.
    protected String name;
    /**
     * Initializes a new instance of the "Component" class.
     *
     * @param name
     */
    protected Component(String name) {
        this.name = name;
    }
    /**
     * Add.
     *
     * @param c
     */
    public abstract void add(Component c);
    /**
     * Display.
     *
     * @param depth
     */
    public abstract void display(int depth);
    /**
     * Remove.
     *
     * @param c
     */
    public abstract void remove(Component c);
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.composite.sample;
import java.util.ArrayList;
import java.util.List;
/**
 * The 'Composite' class.
 *
 * @author Chimomo
 */
class Composite extends Component {
    // The children.
    private final List<Component> children = new ArrayList<>();
    /**
     * Initializes a new instance of the "Composite" class.
     *
     * @param name
     */
    public Composite(String name) {
        super(name);
    }
    /**
     * Add.
     *
     * @param component
     */
    @Override
    public void add(Component component) {
        this.children.add(component);
    }
    /**
     * Display.
     *
     * @param depth
     */
    @Override
    public void display(int depth) {
        for (int i = 0; i < depth; i++) {
            System.out.print("-");
        }
        System.out.println(this.name);
        // Recursively display child nodes.
        this.children.forEach((component) -> {
            component.display(depth + 2);
        });
    }
    /**
     * Remove.
     *
     * @param component
     */
    @Override
    public void remove(Component component) {
        this.children.remove(component);
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.composite.sample;
/**
 * The 'Leaf' class.
 *
 * @author Chimomo
 */
class Leaf extends Component {
    /**
     * Initializes a new instance of the "Leaf" class.
     *
     * @param name
     */
    public Leaf(String name) {
        super(name);
    }
    /**
     * Add.
     *
     * @param component
     */
    @Override
    public void add(Component component) {
        System.out.println("Cannot add to a leaf");
    }
    /**
     * Display.
     *
     * @param depth
     */
    @Override
    public void display(int depth) {
        for (int i = 0; i < depth; i++) {
            System.out.print("-");
        }
        System.out.println(this.name);
    }
    /**
     * Remove.
     *
     * @param component
     */
    @Override
    public void remove(Component component) {
        System.out.println("Cannot remove from a leaf");
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.composite.sample;
/**
 * Startup class for Structural Composite Design Pattern.
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
        // Create a tree structure.
        Composite root = new Composite("root");
        root.add(new Leaf("Leaf A"));
        root.add(new Leaf("Leaf B"));
        Composite comp = new Composite("Composite X");
        comp.add(new Leaf("Leaf XA"));
        comp.add(new Leaf("Leaf XB"));
        root.add(comp);
        root.add(new Leaf("Leaf C"));
        // Add and remove a leaf.
        Leaf leaf = new Leaf("Leaf D");
        root.add(leaf);
        root.remove(leaf);
        // Recursively display tree.
        root.display(1);
    }
}
/*
Output:
-root
---Leaf A
---Leaf B
---Composite X
-----Leaf XA
-----Leaf XB
---Leaf C
 */
```
---
This real-world code demonstrates the Composite pattern used in building a graphical tree structure made up of primitive nodes (lines, circles, etc.) and composite nodes (groups of drawing elements that make up more complex elements).
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.composite.realworld;
import java.util.ArrayList;
import java.util.List;
/**
 * The 'Composite' class.
 *
 * @author Chimomo
 */
class CompositeElement extends DrawingElement {
    // The elements.
    private final List<DrawingElement> elements = new ArrayList<>();
    /**
     * Initializes a new instance of the "CompositeElement" class.
     *
     * @param name
     */
    public CompositeElement(String name) {
        super(name);
    }
    /**
     * Add.
     *
     * @param d
     */
    @Override
    public void add(DrawingElement d) {
        this.elements.add(d);
    }
    /**
     * Display.
     *
     * @param indent
     */
    @Override
    public void display(int indent) {
        for (int i = 0; i < indent; i++) {
            System.out.print("-");
        }
        System.out.println("+ " + this.name);
        // Display each child element on this node.
        this.elements.forEach((d) -> {
            d.display(indent + 2);
        });
    }
    /**
     * Remove.
     *
     * @param d
     */
    @Override
    public void remove(DrawingElement d) {
        this.elements.remove(d);
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.composite.realworld;
/**
 * The 'Component' tree node.
 *
 * @author Chimomo
 */
abstract class DrawingElement {
    // The name.
    protected String name;
    /**
     * Initializes a new instance of the "DrawingElement" class.
     *
     * @param name
     */
    protected DrawingElement(String name) {
        this.name = name;
    }
    /**
     * Add.
     *
     * @param d
     */
    public abstract void add(DrawingElement d);
    /**
     * Display.
     *
     * @param indent
     */
    public abstract void display(int indent);
    /**
     * Remove.
     *
     * @param d
     */
    public abstract void remove(DrawingElement d);
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.composite.realworld;
/**
 * The 'Leaf' class.
 *
 * @author Chimomo
 */
class PrimitiveElement extends DrawingElement {
    /**
     * Initializes a new instance of the "PrimitiveElement" class.
     *
     * @param name
     */
    public PrimitiveElement(String name) {
        super(name);
    }
    /**
     * Add.
     *
     * @param c
     */
    @Override
    public void add(DrawingElement c) {
        System.out.println("Cannot add to a PrimitiveElement");
    }
    /**
     * Display.
     *
     * @param indent
     */
    @Override
    public void display(int indent) {
        for (int i = 0; i < indent; i++) {
            System.out.print("-");
        }
        System.out.println(" " + this.name);
    }
    /**
     * Remove.
     *
     * @param c
     */
    @Override
    public void remove(DrawingElement c) {
        System.out.println("Cannot remove from a PrimitiveElement");
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.composite.realworld;
/**
 * Startup class for Real-World Composite Design Pattern.
 *
 * @author Chimomo
 */
class Program {
    /**
     * Entry point into console application.
     *
     * @param args
     */
    public static void main(String[] args) {
        // Create a tree structure.
        CompositeElement root = new CompositeElement("Picture");
        root.add(new PrimitiveElement("Red Line"));
        root.add(new PrimitiveElement("Blue Circle"));
        root.add(new PrimitiveElement("Green Box"));
        // Create a branch.
        CompositeElement comp = new CompositeElement("Two Circles");
        comp.add(new PrimitiveElement("Black Circle"));
        comp.add(new PrimitiveElement("White Circle"));
        root.add(comp);
        // Add and remove a PrimitiveElement.
        PrimitiveElement pe = new PrimitiveElement("Yellow Line");
        root.add(pe);
        root.remove(pe);
        // Recursively display nodes.
        root.display(1);
    }
}
/*
Output:
-+ Picture
--- Red Line
--- Blue Circle
--- Green Box
---+ Two Circles
----- Black Circle
----- White Circle
 */
```

