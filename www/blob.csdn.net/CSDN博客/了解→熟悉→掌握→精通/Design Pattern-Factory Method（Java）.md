
# Design Pattern - Factory Method（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 16:33:18[Chimomo](https://me.csdn.net/chimomo)阅读数：80


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
# Definition
Define an interface for creating an object, but let sub classes decide which class to instantiate. Factory Method lets a class defer instantiation to sub classes.
# Participants
The classes and/or objects participating in this pattern are:
Product(Page)Defines the interface of objects the factory method creates
ConcreteProduct(SkillsPage, EducationPage, ExperiencePage)Implements the Product interface
Creator(Document)Declares the factory method, which returns an object of type Product. Creator may also define a default implementation of the factory method that returns a default ConcreteProduct object.
May call the factory method to create a Product object.
ConcreteCreator(Report, Resume)Overrides the factory method to return an instance of a ConcreteProduct.

# Sample Code in Java
---
Thisstructuralcode demonstrates the Factory Method offering great flexibility in creating different objects. The abstract class may provide a default object, but each subclass can instantiate an extended version of the object.
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.sample;
/**
 * A 'ConcreteCreator' class.
 *
 * @author Chimomo
 */
class ConcreteCreatorA extends Creator {
    /**
     * The factory method.
     *
     * @return
     */
    @Override
    public Product factoryMethod() {
        return new ConcreteProductA();
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.sample;
/**
 * A 'ConcreteCreator' class.
 *
 * @author Chimomo
 */
class ConcreteCreatorB extends Creator {
    /**
     * The factory method.
     *
     * @return
     */
    @Override
    public Product factoryMethod() {
        return new ConcreteProductB();
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.sample;
/**
 * A 'ConcreteProduct' class.
 *
 * @author Chimomo
 */
public class ConcreteProductA extends Product {
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.sample;
/**
 * A 'ConcreteProduct' class.
 *
 * @author Chimomo
 */
public class ConcreteProductB extends Product {
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.sample;
/**
 * The 'Creator' abstract class.
 *
 * @author Chimomo
 */
abstract class Creator {
    /**
     * The factory method.
     *
     * @return
     */
    public abstract Product factoryMethod();
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.sample;
/**
 * The 'Product' abstract class.
 *
 * @author Chimomo
 */
abstract class Product {
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.sample;
/**
 * Startup class for Structural Factory Method Design Pattern.
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
        // An array of creators.
        Creator[] creators = new Creator[2];
        creators[0] = new ConcreteCreatorA();
        creators[1] = new ConcreteCreatorB();
        // Iterate over creators and create products.
        for (Creator c : creators) {
            Product product = c.factoryMethod();
            System.out.println("Created " + product.getClass().getSimpleName());
        }
    }
}
/*
Output:
Created ConcreteProductA
Created ConcreteProductB
 */
```
---
This real-world code demonstrates the Factory Method offering flexibility in creating different documents. The derived Document classes Report and Resume instantiate extended versions of the Document class. Here, the Factory Method is called in the constructor of the Document base class.
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.realworld;
/**
 * A 'ConcreteProduct' class.
 *
 * @author Chimomo
 */
class BibliographyPage extends Page {
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.realworld;
/**
 * A 'ConcreteProduct' class.
 *
 * @author Chimomo
 */
class ConclusionPage extends Page {
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.realworld;
import java.util.ArrayList;
import java.util.List;
/**
 * The 'Creator' abstract class.
 *
 * @author Chimomo
 */
abstract class Document {
    // The pages.
    private final List<Page> pages = new ArrayList<>();
    /**
     * Initializes a new instance of the "Document" class. Constructor calls
     * abstract Factory method.
     */
    protected Document() {
        this.createPages();
    }
    /**
     * Get pages.
     *
     * @return
     */
    public List<Page> getPages() {
        return this.pages;
    }
    /**
     * Create pages.
     */
    public abstract void createPages();
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.realworld;
/**
 * A 'ConcreteProduct' class.
 *
 * @author Chimomo
 */
class EducationPage extends Page {
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.realworld;
/**
 * A 'ConcreteProduct' class.
 *
 * @author Chimomo
 */
class ExperiencePage extends Page {
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.realworld;
/**
 * A 'ConcreteProduct' class.
 *
 * @author Chimomo
 */
class IntroductionPage extends Page {
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.realworld;
/**
 * The 'Product' abstract class.
 *
 * @author Chimomo
 */
abstract class Page {
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.realworld;
/**
 * Startup class for Real-World Factory Method Design Pattern.
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
        // Note: constructors call Factory Method.
        Document[] documents = new Document[2];
        documents[0] = new Resume();
        documents[1] = new Report();
        // Display document pages.
        for (Document document : documents) {
            System.out.println(document.getClass().getSimpleName() + " ------ ");
            document.getPages().forEach((page) -> {
                System.out.println(page.getClass().getSimpleName());
            });
        }
    }
}
/*
Output:
Resume ------ 
SkillsPage
EducationPage
ExperiencePage
Report ------ 
IntroductionPage
ResultsPage
ConclusionPage
SummaryPage
BibliographyPage
 */
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.realworld;
/**
 * A 'ConcreteCreator' class.
 *
 * @author Chimomo
 */
class Report extends Document {
    /**
     * Create pages.
     */
    @Override
    public void createPages() {
        this.getPages().add(new IntroductionPage());
        this.getPages().add(new ResultsPage());
        this.getPages().add(new ConclusionPage());
        this.getPages().add(new SummaryPage());
        this.getPages().add(new BibliographyPage());
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.realworld;
/**
 * A 'ConcreteProduct' class.
 *
 * @author Chimomo
 */
class ResultsPage extends Page {
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.realworld;
/**
 * A 'ConcreteCreator' class.
 *
 * @author Chimomo
 */
class Resume extends Document {
    /**
     * Create pages.
     */
    @Override
    public void createPages() {
        this.getPages().add(new SkillsPage());
        this.getPages().add(new EducationPage());
        this.getPages().add(new ExperiencePage());
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.realworld;
/**
 * A 'ConcreteProduct' class.
 *
 * @author Chimomo
 */
class SkillsPage extends Page {
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.factorymethod.realworld;
/**
 * A 'ConcreteProduct' class.
 *
 * @author Chimomo
 */
class SummaryPage extends Page {
}
```

