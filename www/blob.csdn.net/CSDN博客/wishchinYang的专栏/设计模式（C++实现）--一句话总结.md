# 设计模式（C++实现）--一句话总结 - wishchinYang的专栏 - CSDN博客
2014年02月10日 09:39:12[wishchin](https://me.csdn.net/wishchin)阅读数：591
个人分类：[工程/设计师](https://blog.csdn.net/wishchin/article/category/1834223)
原文链接：[http://blog.csdn.net/LCL_data/article/details/12117349](http://blog.csdn.net/LCL_data/article/details/12117349)
**按照目的来分，设计模式可以分为创建型模式、结构型模式和行为型模式。**
**按照目的来分，可分为创建型模式、结构型模式和行为型模式。创建型模式：工厂模式（工厂方法、抽象工厂、建造者、原型、单例） 结构型模式：连接模式（适配器、桥接、组合、修饰者、外观、享元、代理）行为型模式：责任链、命令、解释器、迭代器、中介者、备忘录、观察者、状态、策略、模板方法、访问者模式。**
创建型模式用来处理对象的创建过程；结构型模式用来处理类或者对象的组合；行为型模式用来对类或对象怎样交互和怎样分配职责进行描述。
**创建型模式用来处理对象的创建过程，主要包含以下5****种设计模式：**
1，[工厂方法模式](http://blog.csdn.net/lcl_data/article/details/8712834)（Factory Method Pattern）的用意是定义一个创建产品对象的工厂接口，将实际创建工作推迟到子类中。
2，[抽象工厂模式](http://blog.csdn.net/lcl_data/article/details/8733933)（Abstract Factory Pattern）的意图是提供一个创建一系列相关或者相互依赖的接口，而无需指定它们具体的类。
3，[建造者模式](http://blog.csdn.net/lcl_data/article/details/8758477)（Builder Pattern）的意图是将一个复杂的构建与其表示相分离，使得同样的构建过程可以创建不同的表示。
4，[原型模式](http://blog.csdn.net/lcl_data/article/details/8764228)（Prototype Pattern）是用原型实例指定创建对象的种类，并且通过拷贝这些原型创建新的对象。
5，[单例模式](http://blog.csdn.net/lcl_data/article/details/8709701)（Singleton Pattern）是保证一个类仅有一个实例，并提供一个访问它的全局访问点。
**结构型模式用来处理类或者对象的组合，主要包含以下7种设计模式：**
6，[适配器模式](http://blog.csdn.net/lcl_data/article/details/8780140)（Adapter Pattern）是将一个类的接口转换成客户希望的另外一个接口。使得原本由于接口不兼容而不能一起工作的那些类可以一起工作。 
7，[桥接模式](http://blog.csdn.net/lcl_data/article/details/8710134)（Bridge Pattern）是将抽象部分与实际部分分离，使它们都可以独立的变化。 
8，[组合模式](http://blog.csdn.net/lcl_data/article/details/8811101)（Composite Pattern）是将对象组合成树形结构以表示“部分--整体”的层次结构。使得用户对单个对象和组合对象的使用具有一致性。
9，[装饰者模式](http://blog.csdn.net/lcl_data/article/details/8830455)（Decorator Pattern）动态的给一个对象添加一些额外的职责。就增加功能来说，此模式比生成子类更为灵活。 
10，[外观模式](http://blog.csdn.net/lcl_data/article/details/8841779)（Facade Pattern）是为子系统中的一组接口提供一个一致的界面，此模式定义了一个高层接口，这个接口使得这一子系统更加容易使用。
11，[享元模式](http://blog.csdn.net/lcl_data/article/details/8974679)（Flyweight Pattern）是以共享的方式高效的支持大量的细粒度的对象。
12，[代理模式](http://blog.csdn.net/lcl_data/article/details/8989420)（Proxy Pattern）就是为其他对象提供一种代理以控制对这个对象的访问。 
**行为型模式用来对类或对象怎样交互和怎样分配职责进行描述，主要包含以下11种设计模式：**
13，[责任链模式](http://blog.csdn.net/lcl_data/article/details/9042739)（Chain of Responsibility Pattern），在该模式里，很多对象由每一个对象对其下家的引用而连接起来形成一条链。请求在这个链上传递，直到链上的某一个对象决定处理此请求，这使得系统可以在不影响客户端的情况下动态地重新组织链和分配责任。 
14，[命令模式](http://blog.csdn.net/lcl_data/article/details/9080909)（Command Pattern）是将一个请求封装为一个对象，从而使你可用不同的请求对客户端进行参数化；对请求排队或记录请求日志，以及支持可撤销的操作。 
15，[解释器模式](http://blog.csdn.net/lcl_data/article/details/9259905)（Interpreter Pattern）就是描述了如何为简单的语言定义一个语法，如何在该语言中表示一个句子，以及如何解释这些句子。
16，[迭代器模式](http://blog.csdn.net/lcl_data/article/details/9310313)（Iterator Pattern）是提供了一种方法顺序来访问一个聚合对象中的各个元素，而又不需要暴露该对象的内部表示。 
17，[中介者模式](http://blog.csdn.net/lcl_data/article/details/9529427)（Mediator Pattern）就是定义一个中介对象来封装系列对象之间的交互。终结者使各个对象不需要显示的相互调用 ，从而使其耦合性松散，而且可以独立的改变他们之间的交互。
18，[备忘录模式](http://blog.csdn.net/lcl_data/article/details/9745019)（Memento Pattern）是在不破坏封装的前提下，捕获一个对象的内部状态，并在该对象之外保存这个状态。
19，[观察者模式](http://blog.csdn.net/lcl_data/article/details/9208561)（Observer Pattern）定义对象间的一种一对多的依赖关系，当一个对象的状态发生改变时，所有依赖于它的对象都得到通知并被自动更新。 
20，[状态模式](http://blog.csdn.net/lcl_data/article/details/10248415)（State Pattern）就是允许一个对象在其内部状态改变时改变它的行为，使对象看起来似乎修改了它的类。
21，[策略模式](http://blog.csdn.net/lcl_data/article/details/10255125)（Strategy Pattern）就是准备一组算法，并将每一个算法封装起来，使得它们可以互换。
22，[模板方法模式](http://blog.csdn.net/lcl_data/article/details/9199961)（Template Method Pattern）使得子类可以不改变一个算法的结构即可重定义该算法的某些特定步骤。 
23，[访问者模式](http://blog.csdn.net/lcl_data/article/details/11607377)（Visitor Pattern）就是表示一个作用于某对象结构中的各元素的操作，它使你可以在不改变各元素的类的前提下定义作用于这些元素的新操作。
