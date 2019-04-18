# UML+模式设计概述 - wishchinYang的专栏 - CSDN博客
2014年02月10日 09:35:17[wishchin](https://me.csdn.net/wishchin)阅读数：587
个人分类：[工程/设计师](https://blog.csdn.net/wishchin/article/category/1834223)
转自于：[http://blog.csdn.net/rexuefengye/article/details/13020225]()
工程学：工程庞大到一定程度必须是用工程学方法，好比直接用水泥沙子建设实用的摩天大厦是极其困难的。
标准类模型：这或许是AI实现的必经之路吧！类似于Java托管代码的实现方式...
UML软件工程组织：[状态图与流程图](http://www.uml.org.cn/oobject/200811072.asp)
Uml图的使用入门：[http://www.open-open.com/lib/view/open1328059700311.html](http://www.open-open.com/lib/view/open1328059700311.html)     来自于[http://www.open-open.com](http://www.open-open.com)
画流程图需要注意的几个问题：[http://developer.51cto.com/art/201007/209790.htm](http://developer.51cto.com/art/201007/209790.htm)
详细解释各种图：[http://www.cnblogs.com/jiqing9006/p/3344221.html](http://www.cnblogs.com/jiqing9006/p/3344221.html)
# 一、设计模式分类：
      按照目的来分，设计模式可以分为创建型模式、结构型模式和行为型模式。(分类如下表)
-       创建型模式用来处理对象的创建过程；
-       结构型模式用来处理类或者对象的组合；
-       行为型模式用来对类或对象怎样交互和怎样分配职责进行描述。
表1-1
|设计模式分类|说明|备注| |
|----|----|----|----|
|创建型模式|[工厂方法模式](http://blog.csdn.net/rexuefengye/article/details/12704821)（Factory Method Pattern）|定义一个创建产品对象的工厂接口，将实际创建工作推迟到子类中。||
|[抽象工厂模式](http://blog.csdn.net/rexuefengye/article/details/12706593)(Abstract Factory Pattern）|提供一个创建一系列相关或者相互依赖的接口，而无需指定它们具体的类。|| |
|[建造者模式](http://blog.csdn.net/rexuefengye/article/details/12780555)（Builder Pattern）|将一个复杂的构建与其表示相分离，使得同样的构建过程可以创建不同的表示。|| |
|[原型模式](http://blog.csdn.net/rexuefengye/article/details/12783895)（Prototype Pattern）|用原型实例指定创建对象的种类，并且通过拷贝这些原型创建新的对象。|| |
|[单例模式](http://blog.csdn.net/rexuefengye/article/details/12709989)（Singleton Pattern）|保证一个类仅有一个实例，并提供一个访问它的全局访问点。|| |
|| | | |
|结构型模式|[适配器模式](http://blog.csdn.net/rexuefengye/article/details/12904927)（Adapter Pattern）|将一个类的接口转换成客户希望的另外一个接口。使得原本由于接口不兼容而不能一起工作的那些类可以一起工作。||
|[桥接模式](http://blog.csdn.net/rexuefengye/article/details/12834631)（Bridge Pattern）|将抽象部分与实际部分分离，使它们都可以独立的变化。|| |
|[组合模式](http://blog.csdn.net/rexuefengye/article/details/12967681)（Composite Pattern）|将对象组合成树形结构以表示“部分--整体”的层次结构。使得用户对单个对象和组合对象的使用具有一致性。|| |
|[装饰者模式](http://blog.csdn.net/rexuefengye/article/details/12907047)（Decorator Pattern）|动态的给一个对象添加一些额外的职责。就增加功能来说，此模式比生成子类更为灵活。|| |
|[外观模式](http://blog.csdn.net/rexuefengye/article/details/12972537)（Facade Pattern）|为子系统中的一组接口提供一个一致的界面，此模式定义了一个高层接口，这个接口使得这一子系统更加容易使用。|| |
|[享元模式](http://blog.csdn.net/rexuefengye/article/details/12970207)（Flyweight Pattern）|以共享的方式高效的支持大量的细粒度的对象。|| |
|[代理模式](http://blog.csdn.net/rexuefengye/article/details/12974947)（Proxy Pattern）|为其他对象提供一种代理以控制对这个对象的访问。|| |
|| | | |
|行为型模式|[责任链模式](http://blog.csdn.net/rexuefengye/article/details/13017003)（Chain of Responsibility Pattern）|在该模式里，很多对象由每一个对象对其下家的引用而连接起来形成一条链。请求在这个链上传递，直到链上的某一个对象决定处理此请求，这使得系统可以在不影响客户端的情况下动态地重新组织链和分配责任。||
|[命令模式](http://blog.csdn.net/rexuefengye/article/details/13004205)（Command Pattern）|将一个请求封装为一个对象，从而使你可用不同的请求对客户端进行参数化；对请求排队或记录请求日志，以及支持可撤销的操作。|| |
|[解释器模式](http://blog.csdn.net/rexuefengye/article/details/13019859)（Interpreter Pattern）|描述了如何为简单的语言定义一个语法，如何在该语言中表示一个句子，以及如何解释这些句子。|| |
|[迭代器模式](http://blog.csdn.net/rexuefengye/article/details/13019353)（Iterator Pattern）|提供了一种方法顺序来访问一个聚合对象中的各个元素，而又不需要暴露该对象的内部表示。|| |
|[中介者模式](http://blog.csdn.net/rexuefengye/article/details/13002385)（Mediator Pattern）|定义一个中介对象来封装系列对象之间的交互。终结者使各个对象不需要显示的相互调用 ，从而使其耦合性松散，而且可以独立的改变他们之间的交互。|| |
|[备忘录模式](http://blog.csdn.net/rexuefengye/article/details/12999605)（Memento Pattern）|在不破坏封装的前提下，捕获一个对象的内部状态，并在该对象之外保存这个状态。|| |
|[观察者模式](http://blog.csdn.net/rexuefengye/article/details/12998445)（Observer Pattern）|定义对象间的一种一对多的依赖关系，当一个对象的状态发生改变时，所有依赖于它的对象都得到通知并被自动更新。|| |
|[状态模式](http://blog.csdn.net/rexuefengye/article/details/12994161)（State Pattern）|允许一个对象在其内部状态改变时改变它的行为，使对象看起来似乎修改了它的类。|| |
|[策略模式](http://blog.csdn.net/rexuefengye/article/details/12977687)（Strategy Pattern）|准备一组算法，并将每一个算法封装起来，使得它们可以互换。|| |
|[模板方法模式](http://blog.csdn.net/rexuefengye/article/details/12975847)（Template Method Pattern）|使得子类可以不改变一个算法的结构即可重定义该算法的某些特定步骤。|| |
|[访问者模式](http://blog.csdn.net/rexuefengye/article/details/13015941)（Visitor Pattern）|表示一个作用于某对象结构中的各元素的操作，它使你可以在不改变各元素的类的前提下定义作用于这些元素的新操作。|| |
# 二、六大设计原则:
|六大设计原则|说明|备注|
|----|----|----|
|**SRP（单一职责原则）**|有且只有一个原因引起类的变更。||
|**LSP（里氏替换原则）**|子类必须完全实现父类接口，在系统中完全可以用子类替换父类，即子类型必须能够替换它们的基类型。||
|**DIP（依赖倒转原则）**|1. 高层模块不应该依赖于低层模块，二者都应该依赖于抽象。2. 抽象不应该依赖于实现细节，实现细节应该依赖于抽象。||
|**ISP（接口隔离原则）**|不能强迫用户去依赖那些他们不使用的接口。||
|**LOD（迪米特原则）**|只和友元类交流，出现在成员变量，函数输入输出参数中的类，是朋友类。强调类之间的松散耦合。||
|**OCP（开放-封闭原则）**|对扩展开放，对修改封闭。||
