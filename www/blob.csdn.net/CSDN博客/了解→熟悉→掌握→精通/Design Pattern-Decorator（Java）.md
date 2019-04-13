
# Design Pattern - Decorator（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 17:13:35[Chimomo](https://me.csdn.net/chimomo)阅读数：73


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
# Definition
Attach additional responsibilities to an object dynamically. Decorators provide a flexible alternative to subclassing for extending functionality.
# Participants
The classes and/or objects participating in this pattern are:
Component(LibraryItem)Defines the interface for objects that can have responsibilities added to them dynamically.
ConcreteComponent(Book, Video)Defines an object to which additional responsibilities can be attached.
Decorator(Decorator)Maintains a reference to a Component object and defines an interface that conforms to Component's interface.
ConcreteDecorator(Borrowable)Adds responsibilities to the component.

# Sample Code in Java
---
Thisstructuralcode demonstrates the Decorator pattern which dynamically adds extra functionality to an existing object.
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.decorator.sample;
/**
 * The 'Component' abstract class.
 *
 * @author Chimomo
 */
abstract class Component {
    /**
     * Operation.
     */
    public abstract void operation();
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.decorator.sample;
/**
 *
 * @author Chimomo
 */
class ConcreteComponent extends Component {
    /**
     * Operation.
     */
    @Override
    public void operation() {
        System.out.println("ConcreteComponent.operation()");
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.decorator.sample;
/**
 * The 'ConcreteDecoratorA' class.
 *
 * @author Chimomo
 */
class ConcreteDecoratorA extends Decorator {
    /**
     * Operation.
     */
    @Override
    public void operation() {
        super.operation();
        System.out.println("ConcreteDecoratorA.operation()");
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.decorator.sample;
/**
 * The 'ConcreteDecoratorB' class.
 *
 * @author Chimomo
 */
class ConcreteDecoratorB extends Decorator {
    /**
     * Operation.
     */
    @Override
    public void operation() {
        super.operation();
        this.addBehavior();
        System.out.println("ConcreteDecoratorB.operation()");
    }
    /**
     * Add behavior.
     */
    private void addBehavior() {
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.decorator.sample;
/**
 * The 'Decorator' abstract class.
 *
 * @author Chimomo
 */
abstract class Decorator extends Component {
    // The component.
    protected Component component;
    /**
     * Operation.
     */
    @Override
    public void operation() {
        if (this.component != null) {
            this.component.operation();
        }
    }
    /**
     * Set component.
     *
     * @param component
     */
    public void setComponent(Component component) {
        this.component = component;
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.decorator.sample;
/**
 * Startup class for Structural Decorator Design Pattern.
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
        // Create ConcreteComponent and two Decorators.
        ConcreteComponent c = new ConcreteComponent();
        ConcreteDecoratorA d1 = new ConcreteDecoratorA();
        ConcreteDecoratorB d2 = new ConcreteDecoratorB();
        // Link decorators.
        d1.setComponent(c);
        d2.setComponent(d1);
        d2.operation();
    }
}
/*
Output:
ConcreteComponent.operation()
ConcreteDecoratorA.operation()
ConcreteDecoratorB.operation()
 */
```
---
This real-world code demonstrates the Decorator pattern in which 'borrowable' functionality is added to existing library items (books and videos).
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.decorator.realworld;
/**
 * The 'ConcreteComponent' class.
 *
 * @author Chimomo
 */
class Book extends LibraryItem {
    // The author.
    private final String author;
    // The title.
    private final String title;
    /**
     * Initializes a new instance of the "Book" class.
     *
     * @param author
     * @param title
     * @param numCopies
     */
    public Book(String author, String title, int numCopies) {
        this.author = author;
        this.title = title;
        this.numCopies = numCopies;
    }
    /**
     * Display.
     */
    @Override
    public void display() {
        System.out.println("Book ------ ");
        System.out.println(" Author: " + this.author);
        System.out.println(" Title: " + this.title);
        System.out.println(" # Copies: " + this.numCopies);
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.decorator.realworld;
import java.util.ArrayList;
import java.util.List;
/**
 * The 'ConcreteDecorator' class.
 *
 * @author Chimomo
 */
class Borrowable extends Decorator {
    // The borrowers.
    protected List<String> borrowers = new ArrayList<>();
    /**
     * Initializes a new instance of the "Borrowable" class.
     *
     * @param libraryItem
     */
    public Borrowable(LibraryItem libraryItem) {
        super(libraryItem);
    }
    /**
     * Borrow item.
     *
     * @param name
     */
    public void borrowItem(String name) {
        this.borrowers.add(name);
        this.libraryItem.numCopies--;
    }
    /**
     * Display.
     */
    @Override
    public void display() {
        super.display();
        this.borrowers.forEach((borrower) -> {
            System.out.println(" Borrower: " + borrower);
        });
    }
    /**
     * Return item.
     *
     * @param name
     */
    public void returnItem(String name) {
        this.borrowers.remove(name);
        this.libraryItem.numCopies++;
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.decorator.realworld;
/**
 * The 'Decorator' abstract class.
 *
 * @author Chimomo
 */
abstract class Decorator extends LibraryItem {
    // The library item.
    protected LibraryItem libraryItem;
    /**
     * Initializes a new instance of the "Decorator" class.
     *
     * @param libraryItem
     */
    protected Decorator(LibraryItem libraryItem) {
        this.libraryItem = libraryItem;
    }
    /**
     * Display.
     */
    @Override
    public void display() {
        this.libraryItem.display();
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.decorator.realworld;
/**
 * The 'Component' abstract class.
 *
 * @author Chimomo
 */
abstract class LibraryItem {
    // The number of copies.
    public int numCopies;
    /**
     * Display.
     */
    public abstract void display();
    /**
     * Get number of copies.
     *
     * @return
     */
    public int getNumCopies() {
        return this.numCopies;
    }
    /**
     * Set number of copies.
     *
     * @param numCopies
     */
    public void setNumCopies(int numCopies) {
        this.numCopies = numCopies;
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.decorator.realworld;
/**
 * Startup class for Real-World Decorator Design Pattern.
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
        // Create book.
        Book book = new Book("Worley", "Inside ASP.NET", 10);
        book.display();
        // Create video.
        Video video = new Video("Spielberg", "Jaws", 23, 92);
        video.display();
        // Make video borrowable, then borrow and display.
        System.out.println("Making video borrowable:");
        Borrowable borrowVideo = new Borrowable(video);
        borrowVideo.borrowItem("Customer #1");
        borrowVideo.borrowItem("Customer #2");
        borrowVideo.display();
    }
}
/*
Output:
Book ------ 
 Author: Worley
 Title: Inside ASP.NET
 # Copies: 10
Video ------ 
 Director: Spielberg
 Title: Jaws
 # Copies: 23
 Playtime: 92
Making video borrowable:
Video ------ 
 Director: Spielberg
 Title: Jaws
 # Copies: 21
 Playtime: 92
 Borrower: Customer #1
 Borrower: Customer #2
 */
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.decorator.realworld;
/**
 * The 'ConcreteComponent' class.
 *
 * @author Chimomo
 */
class Video extends LibraryItem {
    // The director.
    private final String director;
    // The play time.
    private final int playTime;
    // The title.
    private final String title;
    /**
     * Initializes a new instance of the "Video" class.
     *
     * @param director
     * @param title
     * @param numCopies
     * @param playTime
     */
    public Video(String director, String title, int numCopies, int playTime) {
        this.director = director;
        this.title = title;
        this.numCopies = numCopies;
        this.playTime = playTime;
    }
    /**
     * Display.
     */
    @Override
    public void display() {
        System.out.println("Video ------ ");
        System.out.println(" Director: " + this.director);
        System.out.println(" Title: " + this.title);
        System.out.println(" # Copies: " + this.numCopies);
        System.out.println(" Playtime: " + this.playTime);
    }
}
```

