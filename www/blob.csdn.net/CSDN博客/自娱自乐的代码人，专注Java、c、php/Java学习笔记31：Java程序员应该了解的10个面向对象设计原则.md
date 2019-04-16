# Java学习笔记31：Java程序员应该了解的10个面向对象设计原则 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年02月19日 18:45:52[initphp](https://me.csdn.net/initphp)阅读数：1505
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)








**摘要：**Java编程最基本的原则就是要追求高内聚和低耦合的解决方案和代码模块设计。查看Apache和Sun的开放源代码能帮助你发现其他Java设计原则在这些代码中的实际运用。


面向对象设计原则是OOPS（Object-Oriented Programming System，面向对象的程序设计系统）编程的核心，但大多数Java程序员追逐像[Singleton](http://en.wikipedia.org/wiki/Singleton_pattern)、[Decorator](http://en.wikipedia.org/wiki/Decorator_pattern)、[Observer](http://en.wikipedia.org/wiki/Observer_pattern)这样的设计模式，而不重视面向对象的分析和设计。甚至还有经验丰富的Java程序员没有听说过OOPS和[SOLID](http://en.wikipedia.org/wiki/SOLID)设计原则，他们根本不知道设计原则的好处，也不知道如何依照这些原则来进行编程。


众所周知，Java编程最基本的原则就是要追求高内聚和低耦合的解决方案和代码模块设计。查看Apache和Sun的开放源代码能帮助你发现其他Java设计原则在这些代码中的实际运用。Java Development Kit则遵循以下模式：BorderFactory类中的[工厂模式](http://en.wikipedia.org/wiki/Factory_Pattern)、Runtime类中的[单件模式](http://en.wikipedia.org/wiki/Singleton_pattern)。你可以通过Joshua
 Bloch的[《Effective
 Java》](http://www.amazon.com/gp/product/0321356683/ref=as_li_ss_tl?ie=UTF8&tag=javamysqlanta-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=0321356683)一书来了解更多信息。我个人偏向的另一种面向对象的设计模式是Kathy Sierra的[Head
 First Design Pattern](http://www.amazon.com/gp/product/0596007124/ref=as_li_ss_tl?ie=UTF8&tag=javamysqlanta-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=0596007124)以及[Head
 First Object Oriented Analysis and Design](http://www.amazon.com/gp/product/0596008678/ref=as_li_ss_tl?ie=UTF8&tag=javamysqlanta-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=0596008678)。


虽然实际案例是学习设计原则或模式的最佳途径，但通过本文的介绍，没有接触过这些原则或还在学习阶段的Java程序员也能够了解这10个面向对象的设计原则。其实每条原则都需要大量的篇幅才能讲清楚，但我会尽力做到言简意赅。

**原则1：DRY（Don't repeat yourself）**


即不要写重复的代码，而是用“abstraction”类来抽象公有的东西。如果你需要多次用到一个硬编码值，那么可以设为公共常量；如果你要在两个以上的地方使用一个代码块，那么可以将它设为一个独立的方法。SOLID设计原则的优点是易于维护，但要注意，不要滥用，duplicate 不是针对代码，而是针对功能。这意味着，即使用公共代码来验证OrderID和SSN，二者也不会是相同的。使用公共代码来实现两个不同的功能，其实就是近似地把这两个功能永远捆绑到了一起，如果OrderID改变了其格式，SSN验证代码也会中断。因此要慎用这种组合，不要随意捆绑类似但不相关的功能。

**原则2：封装变化**


在软件领域中唯一不变的就是“Change”，因此封装你认为或猜测未来将发生变化的代码。OOPS设计模式的优点在于易于测试和维护封装的代码。如果你使用Java编码，可以默认私有化变量和方法，并逐步增加访问权限，比如从private到protected和not public。有几种Java设计模式也使用封装，比如Factory设计模式是封装“对象创建”，其灵活性使得之后引进新代码不会对现有的代码造成影响。

**原则3：开闭原则**


即对扩展开放，对修改关闭。这是另一种非常棒的设计原则，可以防止其他人更改已经测试好的代码。理论上，可以在不修改原有的模块的基础上，扩展功能。这也是[开闭原则](http://en.wikipedia.org/wiki/Open/closed_principle)的宗旨。

**原则4：单一职责原则**


类被修改的几率很大，因此应该专注于单一的功能。如果你把多个功能放在同一个类中，功能之间就形成了关联，改变其中一个功能，有可能中止另一个功能，这时就需要新一轮的测试来避免可能出现的问题。

**原则5：依赖注入或倒置原则**


这个设计原则的亮点在于任何被DI框架注入的类很容易用mock对象进行测试和维护，因为对象创建代码集中在框架中，客户端代码也不混乱。有很多方式可以实现依赖倒置，比如像AspectJ等的AOP（Aspect Oriented programming）框架使用的字节码技术，或Spring框架使用的代理等。

**原则6：优先利用组合而非继承**


如果可能的话，优先利用组合而不是继承。一些人可能会质疑，但我发现，组合比继承灵活得多。组合允许在运行期间通过设置类的属性来改变类的行为，也可以通过使用接口来组合一个类，它提供了更高的灵活性，并可以随时实现。[《Effective
 Java》](http://www.amazon.com/gp/product/0321356683/ref=as_li_ss_tl?ie=UTF8&tag=javamysqlanta-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=0321356683)也推荐此原则。

**原则7：里氏代换原则（LSP）**


根据该原则，子类必须能够替换掉它们的基类，也就是说使用基类的方法或函数能够顺利地引用子类对象。LSP原则与单一职责原则和接口分离原则密切相关，如果一个类比子类具备更多功能，很有可能某些功能会失效，这就违反了LSP原则。为了遵循该设计原则，派生类或子类必须增强功能。

**原则8：接口分离原则**


采用多个与特定客户类有关的接口比采用一个通用的涵盖多个业务方法的接口要好。设计接口很棘手，因为一旦释放接口，你就无法在不中断执行的情况下改变它。在Java中，该原则的另一个优势在于，在任何类使用接口之前，接口不利于实现所有的方法，所以单一的功能意味着更少的实现方法。

**原则9：针对接口编程，而不是针对实现编程**


该原则可以使代码更加灵活，以便可以在任何接口实现中使用。因此，在Java中最好使用变量接口类型、方法返回类型、方法参数类型等。《Effective Java》 和《head first design pattern》书中也有提到。

**原则10：委托原则**


该原则最典型的例子是Java中的equals() 和 hashCode() 方法。为了平等地比较两个对象，我们用类本身而不是客户端类来做比较。这个设计原则的好处是没有重复的代码，而且很容易对其进行修改。


总之，希望这些面向对象的设计原则能帮助你写出更灵活更好的代码。理论是第一步，更重要的是需要开发者在实践中去运用和体会。

java接口类型变量实例：

说明：我们定义的p就是接口类型的变量，而Test实现了Person接口，能成功调用 ，你可以自己测试。 补充说明：通常接口不会和class在一个文件中，这里为了方便说明，把它们写在一个文件中。

```java
public class Test implements Person {
public static void main(String[] args) {
  Person p = new Test();
  p.eat();
  p.sleep();
}

public void eat() {
  System.out.println("吃");
}

public void sleep() {
  System.out.println("睡");
}
}

interface Person {
void eat();
void sleep();
}

 -----------------

//A实现IA接口
public class A implements IA {
    void m() {}
}

//A1继承A
public class A1 extends A {
    void aaaa1(){}
}
//A2继承A
public class A2 extends A {
    void aaaa2(){}
}

//包含有A1、A2这些IA的实现类的一个包租婆类
public class BaoZuPo {
    private IA a1;
    private IA a2;
    ……
    //get set 方法略
}

//经常遇到的情况：
public class Test {
    void test() {
        IA a1 = BaoZuPo.getA1();
        a1.m(); //这里才是意义所在
        ((A1)a1).aaaa1(); //这里是调用子类A1自己的方法，体现不出意义
        //类似的
        IA a2 = BaoZuPo.getA2();
        a2.m(); //这里才是意义所谓
        ((A2)a2).aaaa2(); //这里是调用子类A2自己的方法，体现不出意义
    }
}
```







