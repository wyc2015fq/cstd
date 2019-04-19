# 完成C++不能做到的事 - Visitor模式 - 文章 - 伯乐在线
原文出处： [loveis715](http://www.cnblogs.com/loveis715/p/4423464.html)
拿着刚磨好的热咖啡，我坐在了显示器前。“美好的一天又开始了”，我想。
昨晚做完了一个非常困难的任务并送给美国同事Review，因此今天只需要根据他们提出的意见适当修改代码并提交，一周的任务就完成了。剩下的两三天里，我就可以有一些空余的时间看看其它资料来继续充实自己了。
打开Review Board，可以看到我的代码已经被标记为可以提交，但是下面所留的注解引起了我的注意：
“*Great job! With this solution, we can start our integration work and perform testing earlier. One thing is that we have used several “instance of” in the overrided function. That’s double dispatch, an obvious signature for using Visitor pattern. We can switch to that pattern in our future work.*”
Visitor模式我知道，但是Double Dispatch是什么意思？我打开了搜索引擎，找了几篇有关Double Dispatch的介绍性文章开始读了起来。
**Double Dispatch**
当然，对Double Dispatch描述最为清晰和准确的还是在Wikipedia上：
*In software engineering, double dispatch is a special form of multiple dispatch, and a mechanism that dispatches a function call to different concrete functions depending on the runtime types of two objects involved in the call. In most object-oriented systems, the concrete function that is called from a function call in the code depends on the dynamic type of a single object and therefore they are known as single dispatch calls, or simply virtual function calls.*
而在该段文字的最后，我看到了一个再熟悉不过的名词“virtual function”。一看到这个词，我脑中就开始回忆对虚函数进行调用的步骤：在调用虚函数的时候，C++运行时将首先查找对象所对应的虚函数表，然后根据虚函数表中所记录的地址来调用相应的虚函数实现。由于虚函数表是与类型相关联的，因此对虚函数进行调用所执行的逻辑就与对象本身的类型相关。
而Double Dispatch则需要和参与函数调用的两个对象相关。于是我想：那通过为类型添加一个函数重载，不就可以实现Double Dispatch了么？我打开Visual Studio，并在其中写下了如下的代码：

C++
```
// 普通汽车，折扣为0.03
class Vehicle
{
public:
    virtual double GetBaseDiscountRate() { return 0.03; }
};
// 由于是奔驰特销商，因此可以得到更大的折扣
class Benz : public Vehicle
{
public:
    virtual double GetBaseDiscountRate() { return 0.06; }
};
// 普通的销售人员，只能按照公司规定的折扣进行销售
class Sales
{
public:
    virtual double GetDiscountRate(Vehicle& vehicle)
    {
        return vehicle.GetBaseDiscountRate();
    }
    virtual double GetDiscountRate(Benz& benz)
    {
        return benz.GetBaseDiscountRate();
    }
};
// 销售经理，可以针对奔驰提供额外的优惠
class SalesManager : public Sales
{
public:
    virtual double GetDiscountRate(Vehicle& vehicle)
    {
        return vehicle.GetBaseDiscountRate();
    }
    virtual double GetDiscountRate(Benz& benz)
    {
        return benz.GetBaseDiscountRate() * 1.1;
    }
};
int _tmain(int argc, _TCHAR* argv[])
{
    // 有两辆车需要销售，一辆是普通轿车，而另一辆则是奔驰
    Vehicle& vehicle = Vehicle();
    Vehicle& benz = Benz();
    // 向普通销售询问这两辆车的折扣
    Sales* pSales = new Sales();
    double rate = pSales->GetDiscountRate(vehicle);
    cout << "Sales: The rate for common vehicle is: " << rate << endl;
    rate = pSales->GetDiscountRate(benz);
    cout << "Sales: The rate for benz is: " << rate << endl;
    // 向销售经理询问这两辆车的折扣
    SalesManager* pSalesManager = new SalesManager();
    rate = pSalesManager->GetDiscountRate(vehicle);
    cout << "Sales Manager: The rate for common vehicle is: " << rate << endl;
    rate = pSalesManager->GetDiscountRate(benz);
    cout << "Sales Manager: The rate for benz is: " << rate << endl;
    return 0;
}
```
点击运行，答案却不是我想的那样：
![132149452929155](http://jbcdn2.b0.upaiyun.com/2016/01/73d5718a2cc004aaaf7defa901be479f.png)
啊，销售经理并没有提供额外的折扣。这可是个大麻烦。启动Visual Studio的调试功能，我看到了语句“pSalesManager->GetDiscountRate(benz)”所调用的是SalesManager类中定义的为普通汽车所定义的重载：

C++
```
class SalesManager : public Sales
{
public:
    virtual double GetDiscountRate(Vehicle& vehicle) <----传入的参数的运行时类型是Benz，却调用了为Vehicle定义的重载
    {
        return vehicle.GetBaseDiscountRate();
    }
    ……
};
```
难道我对函数重载的理解不对？在搜索引擎中键入“C++ overload resolution”，我打开了C++标准中有关函数重载决议的讲解。其开始的一段话就给了我答案：
*In order to compile a function call, the compiler must first perform name lookup, which, for functions, may involve argument-dependent lookup, and for function templates may be followed by template argument deduction. If these steps produce more than one candidate function, then overload resolution is performed to select the function that will actually be called.*
哦，对！函数重载决议是在编译时完成的。也正因为我们传入的是Vehicle类型的引用，编译器并没有办法知道在运行时传入GetDiscountRate()这个函数的参数到底是Vehicle实例还是Benz实例，因此编译器只可能选择调用接受Vehicle类型引用的重载。如果传入参数benz的类型不再是Vehicle的引用，而是更具体的Benz的引用，那么编译器将会正确地决定到底其所需要调用的函数：
![132157339641208](http://jbcdn2.b0.upaiyun.com/2016/01/7ee7c5d2a7aed6c1e33eceaabd55cc41.png)
但这就不再是根据参数的类型动态决定需要调用的逻辑了，也就不再是Double Dispatch了。要如何达到这种效果呢？我苦苦地思索着。
“你在想什么？”身边的同事递给我今天公司派发的水果，一边吃着一边问我。我就把我刚刚写出的程序以及我现在正在考虑的问题告诉了他。
“既然你要动态决定需要调用的逻辑，那么就把这些逻辑放到动态运行的地方去啊，比如说放到你那些汽车类里面然后暴露一个虚函数，就可以根据所传入的汽车类型决定该汽车所需要使用的折扣率了啊。”
“哦对”，我恍然大悟。C++在运行时动态决议的基本方法就是虚函数，也就是一种Single Dispatch，如果依次在对象和传入参数上连续调用两次虚函数，那么它不就是Double Dispatch了么？在销售汽车这个例子中，我希望同时根据销售人员的职称和所销售的汽车类型一起决定需要执行的逻辑。那么我们首先需要通过Sales类型的指针调用一个虚函数，从而可以根据销售人员的实际类型来决定其在销售时所需要执行的实际逻辑。而在执行这些逻辑的过程中，我们还可以继续调用传入参数实例上定义的虚函数，就可以根据传入参数的类型来决定需要执行的逻辑了！
说做就做。我在Vehicle类中添加一个新的虚函数GetManagerDiscountRate()，以允许SalesManager类的函数实现中调用以获得销售经理所能拿到的折扣，并在Benz类中重写它以返回针对奔驰的特有折扣率。而在Sales以及SalesManager类的实现中，我们则需要分别调用GetBaseDiscountRate()以及新的GetManagerDiscountRate()函数来分别返回普通销售和销售经理所能拿到的折扣率。通过这种方式，我们就可以同时根据销售人员的职务以及所销售车型来共同决定所使用的折扣率了。更改后的代码如下所示：

C++
```
// 普通汽车，折扣为0.03
class Vehicle
{
public:
    virtual double GetBaseDiscountRate() { return 0.03; }
    virtual double GetManagerDiscountRate() { return 0.03; }
};
// 由于是奔驰特销商，因此可以得到更大的折扣
class Benz : public Vehicle
{
public:
    virtual double GetBaseDiscountRate() { return 0.06; }
    virtual double GetManagerDiscountRate() { return 0.066; }
};
// 普通的销售人员，只能按照公司规定的折扣进行销售
class Sales
{
public:
    virtual double GetDiscountRate(Vehicle& vehicle)
    {
        return vehicle.GetBaseDiscountRate();
    }
};
// 销售经理，可以针对某些车型提供额外的优惠
class SalesManager : public Sales
{
public:
    virtual double GetDiscountRate(Vehicle& vehicle)
    {
        return vehicle.GetManagerDiscountRate();
    }
};
int _tmain(int argc, _TCHAR* argv[])
{
    // 需要销售的两辆车
    Vehicle& vehicle = Vehicle();
    Benz& benz = Benz();
    // 向普通销售询问这两辆车的折扣
    Sales* pSales = new Sales();
    double rate = pSales->GetDiscountRate(vehicle);
    cout << "Sales: The rate for common vehicle is: " << rate << endl;
    rate = pSales->GetDiscountRate(benz);
    cout << "Sales: The rate for benz is: " << rate << endl;
    // 向销售经理询问这两辆车的折扣
    SalesManager* pSalesManager = new SalesManager();
    rate = pSalesManager->GetDiscountRate(vehicle);
    cout << "Sales Manager: The rate for common vehicle is: " << rate << endl;
    rate = pSalesManager->GetDiscountRate(benz);
    cout << "Sales Manager: The rate for benz is: " << rate << endl;
    return 0;
}
```
再次运行程序，我发现现在已经可以得到正确的结果了：
![132206516984449](http://jbcdn2.b0.upaiyun.com/2016/01/d89b16dc8e9366ff2005dedc9682632f.png)
也就是说，我自创的Double Dispatch实现已经能够正确地运行了。
**你好，****Visitor**
“你说为什么C++这些高级语言不直接支持Double Dispatch？”我问身边正在和水果奋斗的同事。
“不需要呗。”他头也不抬，随口回答了一句，又拿起了另一只水果。
话说，他可真能吃。
“真的不需要么？”我心里想，就又在搜索引擎中输入了“why C++ double dispatch”。
在多年的工作中，我已经养成了一种固定的学习习惯。例如对于一个知识点，我常常首先了解How，即它是如何工作的；然后是Why，也就是为什么按照这样的方式来工作；然后才是When，即在知道了为什么按照这样的方式来工作后，我们才能在适当的情况下使用它。
幸运的是，在很多论坛中已经讨论过为什么这些语言不直接支持Double Dispatch了。简单地说，一个语言常常不能支持所有的功能，否则这个语言将会变得非常复杂，编写它的编译器及运行时也将变成非常困难的事情。因此到底支持哪些功能实际上由一个语言的目标领域所决定的。在一个语言可以通过一种简单明了的方式解决一种特定问题的时候，该语言就不再必须为该特定问题提供一个内置的解决方案。这些解决方案会逐渐固定下来，并被赋予了一个特有的名字。例如C++中的一种常用模式就是Observer。该模式实现起来非常简单，也易于理解。而在其它语言中就可能提供了对Observer的原生支持，如C#中的delegate。而Visitor模式实际上就是C++对Double Dispatch功能的标准模拟。
接下来，我又搜索了几个Visitor模式的标准实现并开始比较自己所实现的Double Dispatch与Visitor模式标准实现之间的不同之处。这又是我的另一个习惯：实践常常可以检验出自己对于某个知识点的理解是否有偏差。就像我刚刚所犯下的对重载决议的理解错误一样，形成自己解决方案的过程常常会使自己理解某项技术为什么这么做有更深的理解。而通过对比自己的解决方案和标准解决方案，我可以发现别人所做的一些非常精巧的解决方案，并标准化自己的实现。
我仔细地检查了自己刚才所写的有关销售汽车的实例与标准Visitor模式实现之间的不同。显然Visitor模式的标准实现更为聪明：在Sales和SalesManager的成员函数中，编译器知道this所指向的实例的类型，因此将*this当作参数传入到函数中就可以正确地利用C++所提供的函数重载决议功能。这比我那种在实现中调用不同函数的方法高明了不知多少：

C++
```
class SalesManager : public Sales
{
public:
    virtual double GetDiscountRate(Vehicle& vehicle)
    {
        return vehicle.GetDiscountRate(*this); <----编译器知道*this是SalesManager类型实例，因此可以正确地选择接受SalesManager类型参数的重载
    }
};
```
那么在Vehicle类以及Benz类中，我们只需要创建接收不同类型参数的函数重载即可：

C++
```
class Benz : public Vehicle
{
public:
    virtual double GetDiscountRate(Sales& sales) { return 0.06; }
    virtual double GetDiscountRate(SalesManager& salesManager) { return 0.066; }
};
```
而在Visitor模式的标准实现中，我们则需要使用Visit()及Accept()函数对替换上面的各成员函数，并为所诱得汽车及销售人员定义一个公共接口。因此对于上面的销售汽车的示例，其标准的Visitor模式实现为：

C++
```
class Sales;
class SalesManager;
// 汽车接口
class IVehicle
{
public:
    virtual double Visit(Sales& sales) = 0;
    virtual double Visit(SalesManager& sales) = 0;
};
// 普通汽车，折扣为0.03
class Vehicle : public IVehicle
{
public:
    virtual double Visit(Sales& sales) { return 0.03; }
    virtual double Visit(SalesManager& salesManager) { return 0.03; }
};
// 由于是奔驰特销商，因此可以得到更大的折扣
class Benz : public IVehicle
{
public:
    virtual double Visit(Sales& sales) { return 0.06; }
    virtual double Visit(SalesManager& salesManager) { return 0.066; }
};
class ISales
{
public:
    virtual double Accept(IVehicle& vehicle) = 0;
};
// 普通的销售人员，只能按照公司规定的折扣进行销售
class Sales : public ISales
{
public:
    virtual double Accept(IVehicle& vehicle)
    {
        return vehicle.Visit(*this);
    }
};
// 销售经理，可以针对某些车型提供额外的优惠
class SalesManager : public ISales
{
public:
    virtual double Accept(IVehicle& vehicle)
    {
        return vehicle.Visit(*this);
    }
};
int _tmain(int argc, _TCHAR* argv[])
{
    // 需要销售的两辆车
    Vehicle& vehicle = Vehicle();
    Benz& benz = Benz();
    // 向普通销售询问这两辆车的折扣
    Sales* pSales = new Sales();
    double rate = pSales->Accept(vehicle);
    cout << "Sales: The rate for common vehicle is: " << rate << endl;
    rate = pSales->Accept(benz);
    cout << "Sales: The rate for benz is: " << rate << endl;
    // 向销售经理询问这两辆车的折扣
    SalesManager* pSalesManager = new SalesManager();
    rate = pSalesManager->Accept(vehicle);
    cout << "Sales Manager: The rate for common vehicle is: " << rate << endl;
    rate = pSalesManager->Accept(benz);
    cout << "Sales Manager: The rate for benz is: " << rate << endl;
    return 0;
}
```
“那Visitor模式该如何进行扩展呢？”我自己问自己。毕竟在企业级应用中，各组成的扩展性可以很大程度上决定系统的维护性和扩展性。
我注意到上面的Visitor模式实现中主要分为两大类类型：IVehicle和ISales。在该Visitor实现中添加一个新的汽车类型十分容易。从IVehicle派生并实现相应的逻辑即可：

C++
```
class Fiat : public IVehicle
{
public:
    virtual double Visit(Sales& sales) { return 0.05; }
    virtual double Visit(SalesManager& salesManager) { return 0.06; }
};
```
但是添加一个实现了ISales接口的类型则非常困难：需要更改所有已知的汽车类型并添加特定于该接口实现类型的重载。
那在遇到两部分组成都需要更改的情况该怎么办呢？经过查找，我也发现了一种允许同时添加两类型的模式：Acyclic Visitor。除此之外，还有一系列相关的模式，如Hierachical Visitor Pattern。看来和Visitor模式相关的各种知识还真是不少呢。
我再次打开搜索引擎，继续我的自我学习之旅。而身边的同事也继续和水果奋斗着。
**关联阅读**
面试中的Singleton：[http://blog.jobbole.com/96963/](http://blog.jobbole.com/96963/)
**Reference**
有关C++重载决议的讲解：[http://en.cppreference.com/w/cpp/language/overload_resolution](http://en.cppreference.com/w/cpp/language/overload_resolution)
Acyclic Visitor模式：[http://www.objectmentor.com/resources/articles/acv.pdf](http://www.objectmentor.com/resources/articles/acv.pdf)
Hierachical Visitor Pattern模式：[http://en.wikipedia.org/wiki/Hierarchical_visitor_pattern](http://en.wikipedia.org/wiki/Hierarchical_visitor_pattern)
