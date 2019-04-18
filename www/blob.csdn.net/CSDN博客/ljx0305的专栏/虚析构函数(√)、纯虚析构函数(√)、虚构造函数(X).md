# 虚析构函数(√)、纯虚析构函数(√)、虚构造函数(X)  - ljx0305的专栏 - CSDN博客
2008年06月01日 21:25:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：458标签：[delete																[编译器																[语言																[class																[gcc																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=gcc&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
[虚析构函数(√)、纯虚析构函数(√)、虚构造函数(X)](http://blog.csdn.net/fisher_jiang/archive/2008/05/24/2477577.aspx)
一. 虚析构函数
我们知道，为了能够正确的调用对象的析构函数，一般要求具有层次结构的顶级类定义其析构函数为虚函数。因为在delete一个抽象类指针时候，必须要通过虚函数找到真正的析构函数。
如：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)class Base
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)public:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)   Base()...{}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)virtual~Base()...{}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)class Derived: public Base
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)public:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)   Derived()...{};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)~Derived()...{};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)void foo()
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   Base *pb;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   pb =new Derived;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   delete pb;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
这是正确的用法，会发生动态绑定，它会先调用Derived的析构函数，然后是Base的析构函数
如果析构函数不加virtual，delete pb只会执行Base的析构函数，而不是真正的Derived析构函数。
因为不是virtual函数，所以调用的函数依赖于指向静态类型，即Base
二. 纯虚析构函数
现在的问题是，我们想把Base做出抽象类，不能直接构造对象，需要在其中定义一个纯虚函数。如果其中没有其他合适的函数，可以把析构函数定义为纯虚的，即将前面的CObject定义改成：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)class Base
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)public:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)   Base()...{}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)virtual~Base()=0
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)};
可是，这段代码不能通过编译，通常是link错误，不能找到~Base()的引用(gcc的错误报告)。这是因为，析构函数、构造函数和其他内部函数不一样，在调用时，编译器需要产生一个调用链。也就是，Derived的析构函数里面隐含调用了Base的析构函数。而刚才的代码中，缺少～Base()的函数体，当然会出现错误。
这里面有一个误区，有人认为，virtual f()=0这种纯虚函数语法就是没有定义体的语义。
其实，这是不对的。这种语法只是表明这个函数是一个纯虚函数，因此这个类变成了抽象类，不能产生对象。我们完全可以为纯虚函数指定函数体 。通常的纯虚函数不需要函数体，是因为我们一般不会调用抽象类的这个函数，只会调用派生类的对应函数。这样，我们就有了一个纯虚析构函数的函数体，上面的代码需要改成：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)class Base
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)public:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   Base()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)   }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)virtual~Base() =0; //pure virtual
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Base::~Base()//function body
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
从语法角度来说，不可以将上面的析构函数直接写入类声明中（内联函数的写法）。这或许是一个不正交化的地方。但是这样做的确显得有点累赘
这个问题看起来有些学术化，因为一般我们完全可以在Base中找到一个更加适合的函数，通过将其定义为没有实现体的纯虚函数，而将整个类定义为抽象类。但这种技术也有一些应用，如这个例子：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)class Base  //abstract class
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)public:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)virtual~Base()...{};//virtual, not pure
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)virtualvoid Hiberarchy() const=0;//pure virtual
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)void Base::Hiberarchy() const//pure virtual also can have function body
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   std::cout <<"Base::Hiberarchy";
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)class Derived : public Base
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)public:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)   Derived()...{}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)virtualvoid Hiberarchy() const
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)       CB::Hiberarchy();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)       std::cout <<"Derived::Hiberarchy";
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)   }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)virtualvoid foo()...{}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
int main()...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   Base* pb=new Derived();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   pb->Hiberarchy();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)   pb->Base::Hiberarchy();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)return0;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
在这个例子中，我们试图打印出类的继承关系。在根基类中定义了虚函数Hiberarchy，然后在每个派生类中都重载此函数。我们再一次看到，由于想把Base做成个抽象类，而这个类中没有其他合适的方法成员可以定义为纯虚的，我们还是只好将Hiberarchy定义为纯虚的。（当然，完全可以定义～Base函数，这就和上面的讨论一样了。^_^）
另外，可以看到，在main中有两种调用方法，第一种是普通的方式，进行动态链接，执行虚函数，得到结果"Derived::Hiberarchy"；第二种是指定类的方式，就不再执行虚函数的动态链接过程了，结果是"Base::Hiberarchy"。
通过上面的分析可以看出，**定义纯虚函数的真正目的是为了定义抽象类**，而并不是函数本身。与之对比，在java中，定义抽象类的语法是 abstract class，也就是在类的一级作指定（当然虚函数还是也要加上abstract关键字）。是不是这种方式更好一些呢？在Stroustrup的《C++语言的设计与演化》中我找到这样一段话：
“我选择的是将个别的函数描述为纯虚的方式，没有采用将完整的类声明定义为抽象的形式，这是因为纯虚函数的概念更加灵活一些。我很看重能够分阶段定义类的能力；也就是说，我发现预先定义一些纯虚函数，并把另外一些留给进一步的派生类去定义也是很有用的”。
我还没有完全理解后一句话，我想从另外一个角度来阐述这个概念。那就是，在一个多层复杂类结构中，中间层次的类应该具体化一些抽象函数，但很可能并不是所有的。中间类没必要知道是否具体化了所有的虚函数，以及其祖先已经具体化了哪些函数，只要关注自己的职责就可以了。也就是说，中间类没必要知道自己是否是一个真正的抽象类，设计者也就不用考虑是否需要在这个中间类的类级别上加上类似abstract的说明了。
当然，一个语言的设计有多种因素，好坏都是各个方面的。这只是一个解释而已。
最后，总结一下关于虚函数的一些常见问题：
1) 虚函数是动态绑定的，也就是说，使用虚函数的指针和引用能够正确找到实际类的对应函数，而不是执行定义类的函数。这是虚函数的基本功能，就不再解释了。 
2) 构造函数不能是虚函数。而且，在构造函数中调用虚函数，实际执行的是父类的对应函数，因为自己还没有构造好, 多态是被disable的。 
3) 析构函数可以是虚函数，而且，在一个复杂类结构中，这往往是必须的。
4) 将一个函数定义为纯虚函数，实际上是将这个类定义为抽象类，不能实例化对象。 
5) 纯虚函数通常没有定义体，但也完全可以拥有。
6)  析构函数可以是纯虚的，但纯虚析构函数必须有定义体，因为析构函数的调用是在子类中隐含的。 
7) 非纯的虚函数必须有定义体，不然是一个错误。 
8) 派生类的override虚函数定义必须和父类完全一致。除了一个特例，如果父类中返回值是一个指针或引用，子类override时可以返回这个指针（或引用）的派生。例如，在上面的例子中，在Base中定义了 virtual Base* clone(); 在Derived中可以定义为 virtual Derived* clone()。可以看到，这种放松对于Clone模式是非常有用的。 
其他，有待补充。
引用于：[http://blog.csdn.net/fisher_jiang/archive/2008/05/24/2477577.aspx](http://blog.csdn.net/fisher_jiang/archive/2008/05/24/2477577.aspx)[](http://blog.csdn.net/fisher_jiang/archive/2008/05/24/2477577.aspx)
