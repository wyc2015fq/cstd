# 每日必读DZone News—IoC(控制反转)与DI（依赖注入） - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年12月28日 22:28:19[boonya](https://me.csdn.net/boonya)阅读数：111








每日坚持必读，就是紧随时代发展的步伐，技术之路虽艰辛，但终会有所收获。每天进步一小步，程序的世界已然不同。[Java Zone](https://dzone.com/java-jdk-development-tutorials-tools-news)成就每个程序员的不同。英文原文地址：[https://dzone.com/articles/ioc-vs-di?edition=347127&utm_source=Daily%20Digest&utm_medium=email&utm_campaign=Daily%20Digest%202017-12-27](https://dzone.com/articles/ioc-vs-di?edition=347127&utm_source=Daily%20Digest&utm_medium=email&utm_campaign=Daily%20Digest%202017-12-27)


有些人可以互换地使用控制反转和依赖注入。让我们看看他们是如何不同以及他们如何一起工作。

![](https://dz2cdn3.dzone.com/thumbnail?fid=4651702&w=80&h=19) by    

[Francisco Alvarez](https://dzone.com/users/1314093/fjab76.html)


最近，我一直在面试开发者角色的候选人，并且意识到仍然存在着多少混乱，围绕着控制反转和依赖注入的概念。大多数人交替使用这两个术语。也许，这种混淆来自于在[Spring](https://spring.io/)中大量使用这些概念，其中使用控制反转来实现依赖注入。

这篇文章旨在以简单的方式解释这两个想法。


## 控制反转

### 基本概念


这里是IoC的一个非正式的定义：“IoC是当你有别人为你创建对象的时候”。因此，而不是在你的代码中编写“new MyObject”，这个对象是由别人创建的。这个“别人”通常被称为IoC容器。

这个简单的解释说明了一些非常重要的想法

- 它被称为IoC，因为对象的控制是反转的。这不是程序员，而是控制对象的其他人。
- IoC是相对的，它只适用于应用程序的某些对象。所以对于某些对象可能有IoC，而其他的则由程序员直接控制。


除了Spring之外，还有其他IoC的例子，比如Java Servlets和Akka Actors。

### 细节


我们来进一步研究一下IoC的定义。IoC不仅仅是创建对象：Spring上下文或Servlet容器不仅创建对象，而且管理整个生命周期。这包括创建对象，销毁它们，以及在其生命周期的不同阶段调用对象的某些方法。这些方法通常被描述为回调。再次注意，术语：容器调用的方法是回调函数，而不是程序员对自己的代码进行的直接调用。

前面提到的所有IoC容器都实现了一些生命周期：[Spring Bean生命周期](https://www.concretepage.com/spring/spring-bean-life-cycle-tutorial)，[Servlet生命周期](http://tutorials.jenkov.com/java-servlets/servlet-life-cycle.html)和[Akka Actor生命周期](https://alvinalexander.com/scala/understand-methods-akka-actors-scala-lifecycle)。




另一件要考虑的事情是，尽管程序员放弃了对对象的控制，但仍然需要定义IoC容器使用的模板来创建所述对象。

例如，在Spring中，类使用@Service或@Component（等等）进行注释，以指示Spring容器管理这些类的实例（也可以使用XML配置而不是注释）。如你所知，Spring管理的对象被称为Beans。

在Servlet应用程序中，任何实现Servlet接口的类都将由Servlet容器管理。

在Akka应用程序中，IoC容器被称为ActorSystem，被管理对象是扩展特性Actor并通过名为Props的配置对象创建的类的实例。

以下是到目前为止讨论的想法的简要总结：
- IoC容器控制和管理一些对象的生命周期：创建，销毁和回调调用。
- 程序员必须识别其实例将由IoC容器管理的类。有几种方法可以做到这一点：使用注释，扩展一些特定的类，使用外部配置。
- 程序员可以在一定程度上影响对象由IoC容器管理的方式。通常，这是通过覆盖对象回调的默认行为来实现的。



|IoC Container|Managed Object Name|Managed Objects Definition|
|----|----|----|
|Spring Container|Bean|Classes defined with annotations/XML configuration|
|Servlet Container|Servlet|Classes implementing interface Servlet|
|Actor System|Actor|Classes extending trait Actor|
到目前为止，我们已经设法解释IoC，而不需要谈论依赖注入。

## 依赖注入


依赖注入已成为现代软件工程的基石之一，因为它是进行正确测试的基础。简而言之，具有DI是与具有硬编码依赖关系相反。




```java
//Hardcoded dependency

public class MyClass { 

    private MyDependency myDependency = new MyDependency(); 

}

//Injected dependency

public class MyClass { 

    private MyDependency myDependency;

    public MyClass(MyDependency myDependency){

        this.myDependency = myDependency;

    }

}
```

依赖可以通过几种方式注入，比如构造函数中的参数或“set”方法。

与DI一样重要，它的使用有一个缺点，即：依赖管理是不方便的。我们来看一个例子：MyClass1依赖于MyClass2，依次取决于MyClass3：


```java
public class MyClass3 {

    public void doSomething(){}

}

//MyClass2 depends on MyClass3

public class MyClass2 {

    private MyClass3 myClass3;

    public MyClass2(MyClass3 myClass3){

        this.myClass3 = myClass3;

    }

    public void doSomething(){

        myClass3.doSomething();

    }

}

//MyClass1 depends on MyClass2

public class MyClass1 {

    private MyClass2 myClass2;

    public MyClass1(MyClass2 myClass2){

        this.myClass2 = myClass2;

    }

    public void doSomething(){

        myClass2.doSomething();

    }

}

public class Main {

    public static void main(String[] args) {

        //All dependencies need to be managed by the developer

        MyClass3 myClass3 = new MyClass3();

        MyClass2 myClass2 = new MyClass2(myClass3);

        MyClass1 myClass1 = new MyClass1(myClass2);

        myClass1.doSomething();

    }

}
```

现在我们假设MyClass2需要一个新的依赖：MyClass4。我们需要对这个新的依赖关系进行更改：



```java
public class MyClass2 {

    private MyClass3 myClass3;

    private MyClass4 myClass4;

    public MyClass2(MyClass3 myClass3, MyClass4 myClass4){

        this.myClass3 = myClass3;

        this.myClass4 = myClass4;

    }

    public void doSomething(){

        myClass3.doSomething();

        myClass4.doSomething();

    }

}

public class Main {

    public static void main(String[] args) {

        MyClass4 myClass4 = new MyClass4();

        MyClass3 myClass3 = new MyClass3();

        MyClass2 myClass2 = new MyClass2(myClass3, myClass4);

        MyClass1 myClass1 = new MyClass1(myClass2);

        myClass1.doSomething();

    }

}
```
尽管在这个例子中描述的情况并不算太糟糕，但实际应用程序可能有数以百计的依赖关系遍布整个代码库，其创建和管理将需要像上例那样集中。
## 控制反转和依赖注入一起玩


我们刚刚讨论了在真实应用程序中管理数百个依赖关系的问题，可能带有非常复杂的依赖关系图。

所以这里是IoC来救援的地方。通过IoC，依赖关系由容器管理，程序员免除了这个负担。

使用@Autowired之类的注释，容器被要求在需要的地方注入一个依赖项，程序员不需要自己创建/管理这些依赖项。



```java
public class MyClass1 {

    @Autowired

    private MyClass2 myClass2;

    public void doSomething(){

        myClass2.doSomething();

    }

}

public class MyClass2 {

    @Autowired

    private MyClass3 myClass3;

    @Autowired

    private MyClass4 myClass4;

    public void doSomething(){

        myClass3.doSomething();

        myClass4.doSomething();

    }

}
```


## 结论


我们已经提出了控制反转和依赖注入作为单独的概念，并证明在某些情况下，这两个概念可以结合起来提供优越的解决方案。







