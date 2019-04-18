# C++设计模式实现--工厂模式 - L_Andy的专栏 - CSDN博客

2014年06月13日 14:58:15[卡哥](https://me.csdn.net/L_Andy)阅读数：909
个人分类：[设计模式](https://blog.csdn.net/L_Andy/article/category/1335578)



工厂模式属于创建型模式，大致可以分为三类，简单工厂模式、工厂方法模式、抽象工厂模式。听上去差不多，都是工厂模式。下面一个个介绍，首先介绍简单工厂模式，它的主要特点是需要在工厂类中做判断，从而创造相应的产品。当增加新的产品时，就需要修改工厂类。有点抽象，举个例子就明白了。有一家生产处理器核的厂家，它只有一个工厂，能够生产两种型号的处理器核。客户需要什么样的处理器核，一定要显示地告诉生产工厂。下面给出一种实现方案。

**[cpp]**[view
 plain](http://blog.csdn.net/wuzhekai1985/article/details/6660462#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6660462#)

- enum CTYPE {COREA, COREB};     
- class SingleCore    
- {    
- public:    
- virtualvoid Show() = 0;  
- };    
- //单核A  
- class SingleCoreA: public SingleCore    
- {    
- public:    
- void Show() { cout<<"SingleCore A"<<endl; }    
- };    
- //单核B  
- class SingleCoreB: public SingleCore    
- {    
- public:    
- void Show() { cout<<"SingleCore B"<<endl; }    
- };    
- //唯一的工厂，可以生产两种型号的处理器核，在内部判断  
- class Factory    
- {    
- public:     
-     SingleCore* CreateSingleCore(enum CTYPE ctype)    
-     {    
- if(ctype == COREA) //工厂内部判断  
- returnnew SingleCoreA(); //生产核A  
- elseif(ctype == COREB)    
- returnnew SingleCoreB(); //生产核B  
- else
- return NULL;    
-     }    
- };    

       这样设计的主要缺点之前也提到过，就是要增加新的核类型时，就需要修改工厂类。这就违反了开放封闭原则：软件实体（类、模块、函数）可以扩展，但是不可修改。于是，工厂方法模式出现了。所谓工厂方法模式，是指定义一个用于创建对象的接口，让子类决定实例化哪一个类。Factory Method使一个类的实例化延迟到其子类。

       听起来很抽象，还是以刚才的例子解释。这家生产处理器核的产家赚了不少钱，于是决定再开设一个工厂专门用来生产B型号的单核，而原来的工厂专门用来生产A型号的单核。这时，客户要做的是找好工厂，比如要A型号的核，就找A工厂要；否则找B工厂要，不再需要告诉工厂具体要什么型号的处理器核了。下面给出一个实现方案。

**[cpp]**[view
 plain](http://blog.csdn.net/wuzhekai1985/article/details/6660462#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6660462#)

- class SingleCore    
- {    
- public:    
- virtualvoid Show() = 0;  
- };    
- //单核A  
- class SingleCoreA: public SingleCore    
- {    
- public:    
- void Show() { cout<<"SingleCore A"<<endl; }    
- };    
- //单核B  
- class SingleCoreB: public SingleCore    
- {    
- public:    
- void Show() { cout<<"SingleCore B"<<endl; }    
- };    
- class Factory    
- {    
- public:    
- virtual SingleCore* CreateSingleCore() = 0;  
- };    
- //生产A核的工厂  
- class FactoryA: public Factory    
- {    
- public:    
-     SingleCoreA* CreateSingleCore() { returnnew SingleCoreA; }    
- };    
- //生产B核的工厂  
- class FactoryB: public Factory    
- {    
- public:    
-     SingleCoreB* CreateSingleCore() { returnnew SingleCoreB; }    
- };    

       工厂方法模式也有缺点，每增加一种产品，就需要增加一个对象的工厂。如果这家公司发展迅速，推出了很多新的处理器核，那么就要开设相应的新工厂。在C++实现中，就是要定义一个个的工厂类。显然，相比简单工厂模式，工厂方法模式需要更多的类定义。

       既然有了简单工厂模式和工厂方法模式，为什么还要有抽象工厂模式呢？它到底有什么作用呢？还是举这个例子，这家公司的技术不断进步，不仅可以生产单核处理器，也能生产多核处理器。现在简单工厂模式和工厂方法模式都鞭长莫及。抽象工厂模式登场了。它的定义为提供一个创建一系列相关或相互依赖对象的接口，而无需指定它们具体的类。具体这样应用，这家公司还是开设两个工厂，一个专门用来生产A型号的单核多核处理器，而另一个工厂专门用来生产B型号的单核多核处理器，下面给出实现的代码。

**[cpp]**[view
 plain](http://blog.csdn.net/wuzhekai1985/article/details/6660462#)[copy](http://blog.csdn.net/wuzhekai1985/article/details/6660462#)

- //单核  
- class SingleCore     
- {    
- public:    
- virtualvoid Show() = 0;  
- };    
- class SingleCoreA: public SingleCore      
- {    
- public:    
- void Show() { cout<<"Single Core A"<<endl; }    
- };    
- class SingleCoreB :public SingleCore    
- {    
- public:    
- void Show() { cout<<"Single Core B"<<endl; }    
- };    
- //多核  
- class MultiCore      
- {    
- public:    
- virtualvoid Show() = 0;  
- };    
- class MultiCoreA : public MultiCore      
- {    
- public:    
- void Show() { cout<<"Multi Core A"<<endl; }    
- 
- };    
- class MultiCoreB : public MultiCore      
- {    
- public:    
- void Show() { cout<<"Multi Core B"<<endl; }    
- };    
- //工厂  
- class CoreFactory      
- {    
- public:    
- virtual SingleCore* CreateSingleCore() = 0;  
- virtual MultiCore* CreateMultiCore() = 0;  
- };    
- //工厂A，专门用来生产A型号的处理器  
- class FactoryA :public CoreFactory    
- {    
- public:    
-     SingleCore* CreateSingleCore() { returnnew SingleCoreA(); }    
-     MultiCore* CreateMultiCore() { returnnew MultiCoreA(); }    
- };    
- //工厂B，专门用来生产B型号的处理器  
- class FactoryB : public CoreFactory    
- {    
- public:    
-     SingleCore* CreateSingleCore() { returnnew SingleCoreB(); }    
-     MultiCore* CreateMultiCore() { returnnew MultiCoreB(); }    
- };   

        至此，工厂模式介绍完了。利用Rational Rose 2003软件，给出三种工厂模式的UML图，加深印象。

         简单工厂模式的UML图：

![](http://hi.csdn.net/attachment/201108/6/0_13126164442m1F.gif)

         工厂方法的UML图：

![](http://hi.csdn.net/attachment/201108/6/0_1312616449DlvG.gif)

         抽象工厂模式的UML图：

![](http://hi.csdn.net/attachment/201108/6/0_1312616458PVae.gif)

