
# Design Pattern - Abstract Factory（Java） - 了解→熟悉→掌握→精通 - CSDN博客


2019年03月23日 16:30:24[Chimomo](https://me.csdn.net/chimomo)阅读数：57标签：[Design Pattern](https://so.csdn.net/so/search/s.do?q=Design Pattern&t=blog)[Abstract Factory](https://so.csdn.net/so/search/s.do?q=Abstract Factory&t=blog)[Java](https://so.csdn.net/so/search/s.do?q=Java&t=blog)[](https://so.csdn.net/so/search/s.do?q=Abstract Factory&t=blog)[](https://so.csdn.net/so/search/s.do?q=Design Pattern&t=blog)个人分类：[Design Pattern](https://blog.csdn.net/chimomo/article/category/611164)
[](https://so.csdn.net/so/search/s.do?q=Design Pattern&t=blog)


分享一个大牛的人工智能教程。零基础！通俗易懂！风趣幽默！希望你也加入到人工智能的队伍中来！请点击[http://www.captainbed.net](http://www.captainbed.net/troubleshooter)
# Definition
Provide an interface for creating families of related or dependent objects without specifying their concrete classes.
# Participants
The classes and/or objects participating in this pattern are:
AbstractFactory(ContinentFactory)Declares an interface for operations that create abstract products
ConcreteFactory(AfricaFactory, AmericaFactory)Implements the operations to create concrete product objects
AbstractProduct(Herbivore, Carnivore)Declares an interface for a type of product object
Product(Wildebeest, Lion, Bison, Wolf)Defines a product object to be created by the corresponding concrete factory
Implements the AbstractProduct interface
Client(AnimalWorld)Uses interfaces declared by AbstractFactory and AbstractProduct classes

# Sample Code in Java
---
This structural code demonstrates the Abstract Factory pattern creating parallel hierarchies of objects. Object creation has been abstracted and there is no need for hard-coded class names in the client code.
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.sample;
/**
 * The 'AbstractFactory' abstract class.
 *
 * @author Chimomo
 */
abstract class AbstractFactory {
    /**
     * Create product a.
     *
     * @return
     */
    public abstract AbstractProductA createProductA();
    /**
     * Create product b.
     *
     * @return
     */
    public abstract AbstractProductB createProductB();
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.sample;
/**
 * The 'AbstractProductA' abstract class.
 *
 * @author Chimomo
 */
abstract class AbstractProductA {
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.sample;
/**
 * The 'AbstractProductB' abstract class.
 *
 * @author Chimomo
 */
abstract class AbstractProductB {
    /**
     * Interact.
     *
     * @param a
     */
    public abstract void interact(AbstractProductA a);
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.sample;
/**
 * The 'Client' class: Interaction environment for the products.
 *
 * @author Chimomo
 */
class Client {
    // The abstract product a.
    private AbstractProductA abstractProductA;
    // The abstract product b.
    private AbstractProductB abstractProductB;
    /**
     * Initializes a new instance of the Client class.
     *
     * @param abstractFactory
     */
    public Client(AbstractFactory abstractFactory) {
        this.abstractProductA = abstractFactory.createProductA();
        this.abstractProductB = abstractFactory.createProductB();
    }
    /**
     * Run.
     */
    public void run() {
        this.abstractProductB.interact(this.abstractProductA);
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.sample;
/**
 * The 'ConcreteFactory1' class.
 *
 * @author Chimomo
 */
class ConcreteFactory1 extends AbstractFactory {
    /**
     * Create product a.
     *
     * @return
     */
    @Override
    public AbstractProductA createProductA() {
        return new ProductA1();
    }
    /**
     * Create product b.
     *
     * @return
     */
    @Override
    public AbstractProductB createProductB() {
        return new ProductB1();
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.sample;
/**
 * The 'ConcreteFactory2' class.
 *
 * @author Chimomo
 */
class ConcreteFactory2 extends AbstractFactory {
    /**
     * Create product a.
     *
     * @return
     */
    @Override
    public AbstractProductA createProductA() {
        return new ProductA2();
    }
    /**
     * Create product b.
     *
     * @return
     */
    @Override
    public AbstractProductB createProductB() {
        return new ProductB2();
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.sample;
/**
 * Startup class for Structural Abstract Factory Design Pattern.
 *
 * @author Chimomo
 */
class MainApp {
    /**
     * Entry point.
     *
     * @param args
     */
    public static void main(String[] args) {
        // Abstract factory #1.
        AbstractFactory abstractFactory1 = new ConcreteFactory1();
        Client client1 = new Client(abstractFactory1);
        client1.run();
        // Abstract factory #2.
        AbstractFactory abstractFactory2 = new ConcreteFactory2();
        Client client2 = new Client(abstractFactory2);
        client2.run();
    }
}
/* Output:
ProductB1 interacts with ProductA1
ProductB2 interacts with ProductA2
 */
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.sample;
/**
 * The 'ProductA1' class.
 *
 * @author Chimomo
 */
class ProductA1 extends AbstractProductA {
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.sample;
/**
 * The 'ProductA2' class.
 *
 * @author Chimomo
 */
class ProductA2 extends AbstractProductA {
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.sample;
/**
 * The 'ProductB1' class.
 *
 * @author Chimomo
 */
class ProductB1 extends AbstractProductB {
    /**
     * Interact.
     *
     * @param a
     */
    @Override
    public void interact(AbstractProductA a) {
        System.out.println(this.getClass().getSimpleName() + " interacts with " + a.getClass().getSimpleName());
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.sample;
/**
 * The 'ProductB2' class.
 *
 * @author Chimomo
 */
class ProductB2 extends AbstractProductB {
    /**
     * Interact.
     *
     * @param a
     */
    @Override
    public void interact(AbstractProductA a) {
        System.out.println(this.getClass().getSimpleName() + " interacts with " + a.getClass().getSimpleName());
    }
}
```
---
This real-world code demonstrates the creation of different animal worlds for a computer game using different factories. Although the animals created by the Continent factories are different, the interactions among the animals remain the same.
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.realworld;
/**
 * The 'ConcreteFactory1' class.
 *
 * @author Chimomo
 */
class AfricaFactory extends ContinentFactory {
    /**
     * Create carnivore.
     *
     * @return
     */
    @Override
    public Carnivore createCarnivore() {
        return new Lion();
    }
    /**
     * Create herbivore.
     *
     * @return
     */
    @Override
    public Herbivore createHerbivore() {
        return new Wildebeest();
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.realworld;
/**
 * The 'ConcreteFactory2' class.
 *
 * @author Chimomo
 */
class AmericaFactory extends ContinentFactory {
    /**
     * Create carnivore.
     *
     * @return
     */
    @Override
    public Carnivore createCarnivore() {
        return new Wolf();
    }
    /**
     * Create herbivore.
     *
     * @return
     */
    @Override
    public Herbivore createHerbivore() {
        return new Bison();
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.realworld;
/**
 * The 'Client' class.
 *
 * @author Chimomo
 */
class AnimalWorld {
    private Carnivore carnivore;
    private Herbivore herbivore;
    /**
     * Initializes a new instance of AnimalWorld class.
     *
     * @param factory
     */
    public AnimalWorld(ContinentFactory factory) {
        this.carnivore = factory.createCarnivore();
        this.herbivore = factory.createHerbivore();
    }
    /**
     * Run food chain.
     */
    public void runFoodChain() {
        this.carnivore.eat(this.herbivore);
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.realworld;
/**
 * The 'ProductA2' class.
 *
 * @author Chimomo
 */
class Bison extends Herbivore {
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.realworld;
/**
 * The 'AbstractProductB' abstract class.
 *
 * @author Chimomo
 */
abstract class Carnivore {
    /**
     * Eat.
     *
     * @param h
     */
    public abstract void eat(Herbivore h);
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.realworld;
/**
 * The 'AbstractFactory' abstract class.
 *
 * @author Chimomo
 */
abstract class ContinentFactory {
    /**
     * Create carnivore.
     *
     * @return
     */
    public abstract Carnivore createCarnivore();
    /**
     * Create herbivore.
     *
     * @return
     */
    public abstract Herbivore createHerbivore();
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.realworld;
/**
 * The 'AbstractProductA' abstract class.
 *
 * @author Chimomo
 */
abstract class Herbivore {
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.realworld;
/**
 * The 'ProductB1' class.
 *
 * @author Chimomo
 */
class Lion extends Carnivore {
    /**
     * Eat.
     *
     * @param h
     */
    public void eat(Herbivore h) {
        System.out.println(this.getClass().getSimpleName() + " eats " + h.getClass().getSimpleName());
    }
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.realworld;
/**
 * Startup class for Real-World Abstract Factory Design Pattern.
 *
 * @author Chimomo
 */
class MainApp {
    /**
     * Entry point.
     *
     * @param args
     */
    public static void main(String[] args) {
        // Create and run the African animal world.
        ContinentFactory africa = new AfricaFactory();
        AnimalWorld world = new AnimalWorld(africa);
        world.runFoodChain();
        // Create and run the American animal world.
        ContinentFactory america = new AmericaFactory();
        world = new AnimalWorld(america);
        world.runFoodChain();
    }
}
/* Output:
Lion eats Wildebeest
Wolf eats Bison
 */
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.realworld;
/**
 * The 'ProductA1' class.
 *
 * @author Chimomo
 */
class Wildebeest extends Herbivore {
}
```
```python
/*
 *  Chimomo's Blog: https://blog.csdn.net/chimomo/
 */
package chimomo.learning.java.designpattern.abstractfactory.realworld;
/**
 * The 'ProductB2' class.
 *
 * @author Chimomo
 */
public class Wolf extends Carnivore {
    /**
     * Eat.
     *
     * @param h
     */
    @Override
    public void eat(Herbivore h) {
        System.out.println(this.getClass().getSimpleName() + " eats " + h.getClass().getSimpleName());
    }
}
```

